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

    if (pTlv->mCloseOnExit)
    {
        if (pTlv->mTlvSpecificMeaning)
        {
            mStartState = relive::Path_Door::DoorStates::eClosed;
        }
    }

    if (pTlv->mSwitchId == 1)
    {
        mSwitchId = 0;
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot)
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

    if ((gAbe->mCurrentMotion == eAbeMotions::Motion_114_DoorEnter || gAbe->mCurrentMotion == eAbeMotions::Motion_115_DoorExit) && mCurrentState == relive::Path_Door::DoorStates::eClosed && mDoorId == gAbe->field_1A0_door_id)
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

    FP tlvXMid = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    PathLine* pathLine = nullptr;

    if (gCollisions->Raycast(
            tlvXMid,
            FP_FromInteger(pTlv->mTopLeftY),
            tlvXMid,
            FP_FromInteger(pTlv->mBottomRightY),
            &pathLine,
            xOff,
            yOff,
            (GetScale() == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {        // Move up off the line we hit
        *yOff -= FP_FromInteger(12) * GetSpriteScale();

        // Snap on X
        *xOff = FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(*xOff)));
    }
    else
    {
        // Couldn't glue to the floor.. just use the TLV pos
        *xOff = FP_FromInteger(pTlv->mTopLeftX + 12);
        *yOff = FP_FromInteger(pTlv->mTopLeftY + 24);
    }

    // Add on the TLV offset
    *xOff += FP_FromInteger(pTlv->mDoorOffsetX);
    *yOff += FP_FromInteger(pTlv->mDoorOffsetY);

    // Another OWI special
    FP yAdjustHack = {};
    if ((gMap.mCurrentLevel != EReliveLevelIds::eBarracks && gMap.mCurrentLevel != EReliveLevelIds::eBarracks_Ender) || gMap.mOverlayId == 108)
    {
        if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender)
        {
            yAdjustHack = FP_FromInteger(10) * GetSpriteScale();
        }
    }
    else
    {
        yAdjustHack = FP_FromInteger(14) * GetSpriteScale();
    }
    *yOff += yAdjustHack;

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
        mStartState = relive::Path_Door::DoorStates::eClosed;
        mCurrentState = relive::Path_Door::DoorStates::eClosing;
        relive::Path_TLV* pTlv = gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvId);
        pTlv->mTlvSpecificMeaning = 1;
    }
}

void Door::Open()
{
    if (mCurrentState != relive::Path_Door::DoorStates::eOpen)
    {
        mCurrentState = relive::Path_Door::DoorStates::eOpening;
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
    if (mDoorType != relive::Path_Door::DoorTypes::eBasicDoor || GetSpriteScale() != FP_FromInteger(1))
    {
        volume = 60;
    }
    else
    {
        volume = 90;
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

    if (gAbe->mCurrentMotion == eAbeMotions::Motion_114_DoorEnter || gAbe->mCurrentMotion == eAbeMotions::Motion_115_DoorExit)
    {
        if (mCurrentState == relive::Path_Door::DoorStates::eClosed && mDoorId == gAbe->field_1A0_door_id)
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
        if (gAbe->mCurrentMotion != eAbeMotions::Motion_115_DoorExit && gAbe->mCurrentMotion != eAbeMotions::Motion_114_DoorEnter)
        {
            // Then close
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Door_Train_Closing));
            GetAnimation().SetAnimate(true);
            GetAnimation().SetRender(true);
            mCurrentState = relive::Path_Door::DoorStates::eClosed;
        }
    }
}
