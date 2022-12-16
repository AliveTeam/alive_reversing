#include "stdafx.h"
#include "Door.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "MusicTrigger.hpp"
#include "Sound/Midi.hpp"
#include "DDCheat.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/Collisions.hpp"
#include "FixedPoint.hpp"

static const AnimId sDoorAnimIdTable[16][2] = {
    {AnimId::Door_Mines_Closed, AnimId::Door_Mines_Open},
    {AnimId::Door_Mines_Closed, AnimId::Door_Mines_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Feeco_Closed, AnimId::Door_Feeco_Open},
    {AnimId::Door_Barracks_Closed, AnimId::Door_Barracks_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Bonewerkz_Closed, AnimId::Door_Bonewerkz_Open},
    {AnimId::Door_Brewery_Closed, AnimId::Door_Brewery_Open},
    {AnimId::Door_Brewery_Closed, AnimId::Door_Brewery_Open},
    {AnimId::Door_Temple_Closed, AnimId::Door_Temple_Open},
    {AnimId::Door_Feeco_Closed, AnimId::Door_Feeco_Open},
    {AnimId::Door_Barracks_Closed, AnimId::Door_Barracks_Open},
    {AnimId::Door_Bonewerkz_Closed, AnimId::Door_Bonewerkz_Open},
    {AnimId::Door_Mines_Closed, AnimId::Door_Mines_Open}};

static const AnimId sTrainDoorAnimIds[2] =
{
    AnimId::Door_Train_Closed,
    AnimId::Door_Train_Closing
};

Door::Door()
    : BaseAnimatedWithPhysicsGameObject(0)
{
}

void Door::LoadAnimations()
{
    const AnimId doorAnimIds[] =
    {
        AnimId::Door_Mines_Closed,
        AnimId::Door_Mines_Open,
        AnimId::Door_Temple_Closed,
        AnimId::Door_Temple_Open,
        AnimId::Door_Feeco_Closed,
        AnimId::Door_Feeco_Open,
        AnimId::Door_Barracks_Closed,
        AnimId::Door_Barracks_Open,
        AnimId::Door_Bonewerkz_Closed,
        AnimId::Door_Bonewerkz_Open,
        AnimId::Door_Brewery_Closed,
        AnimId::Door_Brewery_Open,
        AnimId::Door_BarracksMetal_Open,
        AnimId::Door_BarracksMetal_Closed
    };

    for (auto& animId : doorAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Door::Door(relive::Path_Door* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eDoor);

    LoadAnimations();

    mTlvId = tlvId;
    mDoorType = pTlv->mDoorType;
    mStartState = pTlv->mStartState;

    if (pTlv->mCloseOnExit == relive::reliveChoice::eYes)
    {
        if (pTlv->mTlvSpecificMeaning)
        {
            mStartState = relive::Path_Door::DoorStates::eClosed;
        }
    }

    mSwitchId = pTlv->mSwitchId;
    if (pTlv->mSwitchId == 1)
    {
        mSwitchId = 0;
    }

    mDoorId = pTlv->mDoorId;
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

    if ((sActiveHero->mCurrentMotion == eAbeMotions::Motion_114_DoorEnter || sActiveHero->mCurrentMotion == eAbeMotions::Motion_115_DoorExit) && mCurrentState == relive::Path_Door::DoorStates::eClosed && mDoorId == sActiveHero->field_1A0_door_id)
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

    const AnimId closedDoor = sDoorAnimIdTable[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0];
    const AnimId openDoor = sDoorAnimIdTable[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1];

    const AnimRecord& openRec = AnimRec(openDoor);
    if (openRec.mFrameTableOffset == 0)
    {
        SetDrawable(false);
        SetDead(true);
        return;
    }

    if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
    {
        if (gMap.mOverlayId == 108)
        {
            Animation_Init(GetAnimRes(AnimId::Door_BarracksMetal_Open));
        }
        else
        {
            Animation_Init(GetAnimRes(openDoor));
        }
    }
    else
    {
        if (gMap.mOverlayId == 108)
        {
            Animation_Init(GetAnimRes(AnimId::Door_BarracksMetal_Closed));
        }
        else
        {
            Animation_Init(GetAnimRes(closedDoor));
        }
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

    if (sCollisions->Raycast(
            tlvXMid,
            FP_FromInteger(pTlv->mTopLeftY),
            tlvXMid,
            FP_FromInteger(pTlv->mBottomRightY),
            &pathLine,
            xOff,
            yOff,
            (GetScale() == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {
        // Move up off the line we hit
        *yOff -= FP_FromInteger(12) * GetSpriteScale();

        // Snap on X
        *xOff = FP_FromInteger(SnapToXGrid(GetSpriteScale(), FP_GetExponent(*xOff)));
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

bool Door::vIsOpen()
{
    return mCurrentState == relive::Path_Door::DoorStates::eOpen;
}

void Door::vOpen()
{
    if (mCurrentState != relive::Path_Door::DoorStates::eOpen)
    {
        mCurrentState = relive::Path_Door::DoorStates::eOpening;
    }
}

void Door::vClose()
{
    if (mCurrentState != relive::Path_Door::DoorStates::eClosed)
    {
        mStartState = relive::Path_Door::DoorStates::eClosed;
        mCurrentState = relive::Path_Door::DoorStates::eClosing;
        relive::Path_TLV* pTlv = gPathInfo->TLV_From_Offset_Lvl_Cam(mTlvId);
        pTlv->mTlvSpecificMeaning = 1;
    }
}

void Door::vSetOpen()
{
    mCurrentState = relive::Path_Door::DoorStates::eOpen;
}

void Door::vSetClosed()
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
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void Door::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_114_DoorEnter || sActiveHero->mCurrentMotion == eAbeMotions::Motion_115_DoorExit)
    {
        if (mCurrentState == relive::Path_Door::DoorStates::eClosed && mDoorId == sActiveHero->field_1A0_door_id)
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
                    relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, relive::Path_MusicTrigger::TriggeredBy::eTimer, 0, 0);
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

                if ((!mStartState && SwitchStates_Get(mSwitchId))
                    || (mStartState == relive::Path_Door::DoorStates::eClosed && !SwitchStates_Get(mSwitchId)))
                {
                    mCurrentState = relive::Path_Door::DoorStates::eClosing;
                    if (gMap.mOverlayId == 108)
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Door_BarracksMetal_Open));
                    }
                    else
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(sDoorAnimIdTable[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]));
                    }

                    GetAnimation().SetLoopBackwards(false);
                    GetAnimation().SetRender(true);
                }
                break;

            case relive::Path_Door::DoorStates::eClosed:
                GetAnimation().SetRender(true);
                GetAnimation().SetAnimate(true);

                if ((mStartState == relive::Path_Door::DoorStates::eClosed && SwitchStates_Get(mSwitchId)) || (mStartState == relive::Path_Door::DoorStates::eOpen && !SwitchStates_Get(mSwitchId)))
                {
                    mCurrentState = relive::Path_Door::DoorStates::eOpening;
                    if (gMap.mOverlayId == 108)
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Door_BarracksMetal_Open));
                    }
                    else
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(sDoorAnimIdTable[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]));
                    }

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

void Door::VScreenChanged()
{
    SetDead(true);
}

// ================================================================================================

static FP sTrainDoorXPos = {};
static FP sTrainDoorYPos = {};

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

    sTrainDoorXPos = mXPos;
    sTrainDoorYPos = mYPos;

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
        Path::TLV_Reset(mTlvId, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvId, 1, 0, 0);
    }
}

void TrainDoor::VUpdate()
{
    if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
    {
        // Wait for Abe to GTFO
        if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_115_DoorExit && sActiveHero->mCurrentMotion != eAbeMotions::Motion_114_DoorEnter)
        {
            // Then close
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Door_Train_Closing));
            GetAnimation().SetAnimate(true);
            GetAnimation().SetRender(true);
            mCurrentState = relive::Path_Door::DoorStates::eClosed;
        }
    }

    mXPos = sTrainDoorXPos + FP_FromInteger(gTweakX);
    mYPos = sTrainDoorYPos + FP_FromInteger(gTweakY);
}
