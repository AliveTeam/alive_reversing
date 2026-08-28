#include "GameObjects/BaseAbe.hpp"
#include "GameType.hpp"
#include "stdafx.h"
#include "Door.hpp"

#include "../AliveLibAE/Sfx.hpp"
#include "../AliveLibAE/Abe.hpp"
#include "../AliveLibAE/MusicTrigger.hpp"
#include "../AliveLibAE/DDCheat.hpp"
#include "../AliveLibAE/Map.hpp"
#include "../AliveLibAE/Path.hpp"

#include "../Function.hpp"
#include "../SwitchStates.hpp"
#include "../Events.hpp"
#include "../Sound/Midi.hpp"
#include "../Grid.hpp"
#include "../Collisions.hpp"
#include "../FixedPoint.hpp"

static const AnimId sTrainDoorAnimIds[2] =
{
    AnimId::Door_Train_Closed,
    AnimId::Door_Train_Closing
};

Door::Door()
    : BaseAnimatedWithPhysicsGameObject(0)
{
}

void Door::LoadAnimations(const std::string& theme)
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Themed_Closed, theme));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Themed_Open, theme));
}

Door::Door(relive::Path_Door* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0),
    mTlvId(tlvId),
    mDoorType(pTlv->mDoorType),
    mStartState(pTlv->mStartState),
    mSwitchId(pTlv->mSwitchId),
    mDoorId(pTlv->mDoorId)
{
    SetType(ReliveTypes::eDoor);

    LoadAnimations(pTlv->mTheme);

    if (GetGameType() == GameType::eAe)
    {
        if (pTlv->mCloseOnExit)
        {
            if (pTlv->mTlvSpecificMeaning)
            {
                mStartState = relive::Path_Door::DoorStates::eClosed;
            }
        }
    }

    if (pTlv->mSwitchId == 1)
    {
        mSwitchId = 0;
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot)
    {
        HandleFeeCoDepotSwitches();
    }

    if (mDoorType == relive::Path_Door::DoorTypes::eTasksDoor)
    {
        mHubIds[0] = pTlv->mHub1;
        mHubIds[1] = pTlv->mHub2;
        mHubIds[2] = pTlv->mHub3;
        mHubIds[3] = pTlv->mHub4;
        mHubIds[4] = pTlv->mHub5;
        mHubIds[5] = pTlv->mHub6;
        mHubIds[6] = pTlv->mHub7;
        mHubIds[7] = pTlv->mHub8;

        if (SwitchStates_Get(mHubIds[0]) && SwitchStates_Get(mHubIds[1]) && SwitchStates_Get(mHubIds[2]) && SwitchStates_Get(mHubIds[3]) && SwitchStates_Get(mHubIds[4]) && SwitchStates_Get(mHubIds[5]) && SwitchStates_Get(mHubIds[6]) && SwitchStates_Get(mHubIds[7]))
        {
            SwitchStates_Do_Operation(mSwitchId, relive::reliveSwitchOp::eSetTrue);
        }
        else
        {
            SwitchStates_Do_Operation(mSwitchId, relive::reliveSwitchOp::eSetFalse);
        }
    }

    if (mStartState == relive::Path_Door::DoorStates::eOpen)
    {
        if (SwitchStates_Get(mSwitchId))
        {
            // Its open but the switch id is on which causes it to close
            mCurrentState = relive::Path_Door::DoorStates::eClosed;
        }
        else
        {
            mCurrentState = relive::Path_Door::DoorStates::eOpen;
        }
    }
    else
    {
        if (SwitchStates_Get(mSwitchId))
        {
            // Its closed but the id is on which causes it to open
            mCurrentState = relive::Path_Door::DoorStates::eOpen;
        }
        else
        {
            mCurrentState = relive::Path_Door::DoorStates::eClosed;
        }
    }

    if (GetAbe()->IsEnteringOrExitingDoor() && 
        mCurrentState == relive::Path_Door::DoorStates::eClosed && 
        mDoorId == GetAbe()->DoorId())
    {
        // Force open is abe is in the door
        mCurrentState = relive::Path_Door::DoorStates::eOpen;
    }

    if (mDoorType == relive::Path_Door::DoorTypes::eTasksDoorWithSecretMusic)
    {
        mHubIds[0] = pTlv->mHub1;
        mHubIds[1] = pTlv->mHub2;
        mHubIds[2] = pTlv->mHub3;
        mHubIds[3] = pTlv->mHub4;
        mHubIds[4] = pTlv->mHub5;
        mHubIds[5] = pTlv->mHub6;
        mHubIds[6] = pTlv->mHub7;
        mHubIds[7] = pTlv->mHub8;
    }

    if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
    {
        Animation_Init(GetAnimRes(AnimId::Door_Themed_Open));
    }
    else
    {
        Animation_Init(GetAnimRes(AnimId::Door_Themed_Closed));
    }

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        if (pTlv->mScale == relive::reliveScale::eHalf)
        {
            SetSpriteScale(FP_FromDouble(0.5));
            SetScale(Scale::Bg);
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        }
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
    }

    FP* xOff = &mXPos;
    FP* yOff = &mYPos;

    FP tlvXMid = FP_FromInteger(pTlv->MidPointX());
    
    PSX_Point cam;
    gMap.GetCurrentCamCoords(&cam);
    
    // Bottom of *this* camera in world space
    s32 screenHeight = 260;
    FP rayEndY = FP_FromInteger(cam.y + screenHeight);
    
    PathLine* pathLine = nullptr;
    if (gCollisions->Raycast(
            tlvXMid,
            FP_FromInteger(pTlv->mTopLeftY),  // start at TLV top (no lines above)
            tlvXMid,
            rayEndY,                          // extend down to camera bottom
            &pathLine,
            xOff,
            yOff,
            (GetScale() == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {        
        // Snap on X
        *xOff = FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(*xOff)));
    
        *yOff -= FP_FromInteger(12) * GetSpriteScale();
    }
    else
    {
        ALIVE_FATAL("Door is floating in the void, ensure a collision line is below it");
    }

    FP yAdjustHack = FP_FromInteger(0);

    if ((gMap.mCurrentLevel == EReliveLevelIds::eBarracks ||
        gMap.mCurrentLevel== EReliveLevelIds::eBarracks_Ender) &&
        gMap.mOverlayId != 108)
    {
        // Barracks 14 (unless overlay 108)
        yAdjustHack = FP_FromInteger(14) * GetSpriteScale();
    }
    else if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz ||
            gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender)
    {
        // Bonewerkz 10
        yAdjustHack = FP_FromInteger(10) * GetSpriteScale();
    }


    *yOff += yAdjustHack;

    // Add on the TLV offset
    *xOff += FP_FromInteger(pTlv->mDoorOffsetX);
    *yOff += FP_FromInteger(pTlv->mDoorOffsetY);

    if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
    {
        GetAnimation().SetRender(false);
    }

    GetAnimation().SetAnimate(false);
    SetDoPurpleLightEffect(true);
}

bool Door::IsOpen()
{
    return mCurrentState == relive::Path_Door::DoorStates::eOpen;
}

void Door::Close()
{
    if (mCurrentState != relive::Path_Door::DoorStates::eClosed)
    {
        mCurrentState = relive::Path_Door::DoorStates::eClosing;

        if (GetGameType() == GameType::eAe)
        {
            mStartState = relive::Path_Door::DoorStates::eClosed;
            relive::Path_TLV* pTlv = gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvId).GetTlv();
            pTlv->mTlvSpecificMeaning = 1;
        }
    }
}

void Door::Open()
{
    if (mCurrentState != relive::Path_Door::DoorStates::eOpen)
    {
        mCurrentState = relive::Path_Door::DoorStates::eOpening;
    }
}

void Door::HandleFeeCoDepotSwitches()
{
    switch (mDoorId)
    {
        case 30000:
            if (gVisitedBonewerkz)
            {
                mStartState = relive::Path_Door::DoorStates::eClosed;
            }
            break;

        case 30001:
        case 30004:
            if (gVisitedBarracks)
            {
                mStartState = relive::Path_Door::DoorStates::eClosed;
            }
            break;

        case 30002:
            if (gVisitedBonewerkz)
            {
                SwitchStates_Set(255u, 1);
            }

            if (gVisitedBarracks)
            {
                SwitchStates_Set(254u, 1);
            }

            if (gVisitedFeecoEnder)
            {
                SwitchStates_Set(255u, 0);
                SwitchStates_Set(254u, 0);
            }
            break;

        case 30003:
            if (gVisitedBonewerkz)
            {
                if (gVisitedBarracks)
                {
                    if (gVisitedFeecoEnder)
                    {
                        mStartState = relive::Path_Door::DoorStates::eOpen;
                    }
                }
                if (gVisitedBonewerkz)
                {
                    SwitchStates_Set(250u, 1);
                }
            }

            if (gVisitedBarracks)
            {
                SwitchStates_Set(251u, 1);
            }
            if (gVisitedFeecoEnder)
            {
                SwitchStates_Set(253u, 1);
            }
            break;
        default:
            break;
    }
}

void Door::SetOpen()
{
    mCurrentState = relive::Path_Door::DoorStates::eOpen;
}

void Door::SetClosed()
{
    mCurrentState = relive::Path_Door::DoorStates::eClosed;
}

void Door::PlaySound()
{
    s16 volume = 0;

    const bool isRuptureFarms =
        gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms ||
        gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn;

    if (isRuptureFarms)
    {
        if (GetScale() == Scale::Fg)
        {
            volume = 127;
        }
        else
        {
            volume = 90;
        }
        SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 75, 75);
    }
    else
    {
        // AE logic (also used by AO outside RuptureFarms)
        if (mDoorType == relive::Path_Door::DoorTypes::eBasicDoor &&
            GetScale() == Scale::Fg)
        {
            volume = 90;
        }
        else
        {
            volume = 60;
        }
    }

    SFX_Play_Pitch(relive::SoundEffects::DoorEffect, volume, 900);
    SFX_Play_Pitch(relive::SoundEffects::DoorEffect, volume, 0);
}


Door::~Door()
{
    Path::TLV_Reset(mTlvId);
}

void Door::VScreenChanged()
{
    SetDead(true);
}

void Door::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    if (GetAbe()->IsEnteringOrExitingDoor())
    {
        if (mCurrentState == relive::Path_Door::DoorStates::eClosed && mDoorId == GetAbe()->DoorId())
        {
            GetAnimation().SetRender(false);
            mCurrentState = relive::Path_Door::DoorStates::eOpen;
        }
    }
    else
    {
        mDoorId = -1;
        if (mDoorType == relive::Path_Door::DoorTypes::eTasksDoorWithSecretMusic || mDoorType == relive::Path_Door::DoorTypes::eTasksDoor)
        {
            if (SwitchStates_Get(mHubIds[0])
                && SwitchStates_Get(mHubIds[1])
                && SwitchStates_Get(mHubIds[2])
                && SwitchStates_Get(mHubIds[3])
                && SwitchStates_Get(mHubIds[4])
                && SwitchStates_Get(mHubIds[5])
                && SwitchStates_Get(mHubIds[6])
                && SwitchStates_Get(mHubIds[7]))
            {
                if (!SwitchStates_Get(mSwitchId) && mDoorType == relive::Path_Door::DoorTypes::eTasksDoorWithSecretMusic)
                {
                    SND_SEQ_Play(SeqId::SecretMusic_32, 1, 127, 127);
                    relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, relive::Path_MusicTrigger::TriggeredBy::eTimer, 0);
                }
                SwitchStates_Do_Operation(mSwitchId, relive::reliveSwitchOp::eSetTrue);
            }
            else
            {
                SwitchStates_Do_Operation(mSwitchId, relive::reliveSwitchOp::eSetFalse);
            }
        }
        switch (mCurrentState)
        {
            case relive::Path_Door::DoorStates::eOpen:
                GetAnimation().SetRender(false);

                if ((mStartState == relive::Path_Door::DoorStates::eOpen && SwitchStates_Get(mSwitchId))
                    || (mStartState == relive::Path_Door::DoorStates::eClosed && !SwitchStates_Get(mSwitchId)))
                {
                    mCurrentState = relive::Path_Door::DoorStates::eClosing;
                    // TODO: AE overlay 108 set Open anim here?? Bug?
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Door_Themed_Closed));
                    GetAnimation().SetLoopBackwards(false);
                    GetAnimation().SetRender(true);
                }
                break;

            case relive::Path_Door::DoorStates::eClosed:
                GetAnimation().SetRender(true);
                GetAnimation().SetAnimate(true);

                if ((mStartState == relive::Path_Door::DoorStates::eClosed && SwitchStates_Get(mSwitchId)) || 
                    (mStartState == relive::Path_Door::DoorStates::eOpen && !SwitchStates_Get(mSwitchId)))
                {
                    mCurrentState = relive::Path_Door::DoorStates::eOpening;
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Door_Themed_Open));
                    GetAnimation().SetFrame(3);
                    GetAnimation().SetLoopBackwards(true);
                    GetAnimation().SetRender(true);
                    PlaySound();
                }
                break;

            case relive::Path_Door::DoorStates::eOpening:
                GetAnimation().SetRender(true);
                GetAnimation().SetAnimate(true);

                if (GetAnimation().GetIsLastFrame())
                {
                    mCurrentState = relive::Path_Door::DoorStates::eOpen;
                }
                break;

            case relive::Path_Door::DoorStates::eClosing:
                GetAnimation().SetRender(true);
                GetAnimation().SetAnimate(true);

                if (GetAnimation().GetIsLastFrame())
                {
                    mCurrentState = relive::Path_Door::DoorStates::eClosed;
                    PlaySound();
                }
                break;

            default:
                return;
        }
    }
}


// ================================================================================================

void TrainDoor::LoadAnimations()
{
    for (auto& animId : sTrainDoorAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

TrainDoor::TrainDoor(relive::Path_TrainDoor* pTlv, const Guid& tlvId)
{
    SetType(ReliveTypes::eDoor);
    mTlvId = tlvId;

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::Door_Train_Closing));

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);

    if (pTlv->mTlvSpecificMeaning)
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Door_Train_Closed));
        mCurrentState = relive::Path_Door::DoorStates::eClosed;
    }
    else
    {
        GetAnimation().SetAnimate(false);
        GetAnimation().SetRender(false);
        mCurrentState = relive::Path_Door::DoorStates::eOpen;
    }
    GetAnimation().SetFlipX(pTlv->mDirection == relive::reliveXDirection::eRight);
}

TrainDoor::~TrainDoor()
{
    if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
    {
        Path::TLV_Reset(mTlvId);
    }
    else
    {
        Path::TLV_Reset(mTlvId, 1);
    }
}

void TrainDoor::VUpdate()
{
    if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
    {
        // Wait for Abe to GTFO
        if (!GetAbe()->IsEnteringOrExitingDoor())
        {
            // Then close
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Door_Train_Closing));
            GetAnimation().SetAnimate(true);
            GetAnimation().SetRender(true);
            mCurrentState = relive::Path_Door::DoorStates::eClosed;
        }
    }
}
