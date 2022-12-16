#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Door.hpp"
#include "Abe.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Midi.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "MusicTrigger.hpp"
#include "Grid.hpp"
#include "../AliveLibAE/FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

static const AnimId sDoorAnimdIdTable[16][6] = {
    {AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None}, // menu
    {AnimId::Door_RuptureFarms_Closed, AnimId::Door_RuptureFarms_Open, AnimId::Door_RuptureFarms_Closed, AnimId::Door_RuptureFarms_Open, AnimId::Door_RuptureFarms_Closed, AnimId::Door_RuptureFarms_Open}, // rupture farms
    {AnimId::Door_Lines_Closed, AnimId::Door_Lines_Open, AnimId::Door_Lines_Closed, AnimId::Door_Lines_Open, AnimId::Door_Lines_Closed, AnimId::Door_Lines_Open}, // lines
    {AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None}, // forest
    {AnimId::Door_Forest_Closed, AnimId::Door_Forest_Open, AnimId::HubDoor_Forest_Closed, AnimId::HubDoor_Forest_Open, AnimId::FinalTestDoor_Forest_Closed, AnimId::FinalTestDoor_Forest_Open}, // forest temple
    {AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None}, // stock yards
    {AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None}, // stock yards return
    {AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None}, // removed
    {AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None}, // desert
    {AnimId::Door_Desert_Closed, AnimId::Door_Desert_Open, AnimId::HubDoor_Desert_Closed, AnimId::HubDoor_Desert_Open, AnimId::FinalTestDoor_Desert_Closed, AnimId::FinalTestDoor_Desert_Open}, // desert temple
    {AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None}, // credits
    {AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None, AnimId::None}, // removed
    {AnimId::Door_RuptureFarms_Closed, AnimId::Door_RuptureFarms_Open, AnimId::Door_RuptureFarms_Closed, AnimId::Door_RuptureFarms_Open, AnimId::Door_RuptureFarms_Closed, AnimId::Door_RuptureFarms_Open}, // board room
    {AnimId::Door_RuptureFarms_Closed, AnimId::Door_RuptureFarms_Open, AnimId::Door_RuptureFarms_Closed, AnimId::Door_RuptureFarms_Open, AnimId::Door_RuptureFarms_Closed, AnimId::Door_RuptureFarms_Open}, // rupture farms return
    {AnimId::Door_Forest_Closed, AnimId::Door_Forest_Open, AnimId::HubDoor_Forest_Closed, AnimId::HubDoor_Forest_Open, AnimId::FinalTestDoor_Forest_Closed, AnimId::FinalTestDoor_Forest_Open}, // forest chase
    {AnimId::Door_Desert_Closed, AnimId::Door_Desert_Open, AnimId::HubDoor_Desert_Closed, AnimId::HubDoor_Desert_Open, AnimId::FinalTestDoor_Desert_Closed, AnimId::FinalTestDoor_Desert_Open}}; // desert escape

void Door::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_RuptureFarms_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_RuptureFarms_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Lines_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Lines_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Forest_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Forest_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HubDoor_Forest_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HubDoor_Forest_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FinalTestDoor_Forest_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FinalTestDoor_Forest_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Desert_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Door_Desert_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HubDoor_Desert_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HubDoor_Desert_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FinalTestDoor_Desert_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FinalTestDoor_Desert_Open));
}


Door::Door(relive::Path_Door* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eDoor);

    LoadAnimations();

    mTlvInfo = tlvId;

    mDoorType = pTlv->mDoorType;
    mDoorClosed = pTlv->mDoorClosed;
    mSwitchId = pTlv->mSwitchId;

    if (mSwitchId == 1)
    {
        mSwitchId = 0;
    }

    mDoorId = pTlv->mDoorId;

    mCurrentState = (mDoorClosed == relive::reliveChoice::eNo) == SwitchStates_Get(mSwitchId) ? relive::Path_Door::DoorStates::eClosed : relive::Path_Door::DoorStates::eOpen;

    if ((sActiveHero->mCurrentMotion == eAbeMotions::Motion_156_DoorEnter || sActiveHero->mCurrentMotion == eAbeMotions::Motion_157_DoorExit) &&
        mCurrentState == relive::Path_Door::DoorStates::eClosed && mDoorId == sActiveHero->field_196_door_id)
    {
        mCurrentState = relive::Path_Door::DoorStates::eOpen;
    }

    const s32 idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

    FP scale = {};
    PathLine* pLine = nullptr;
    PSX_Point mapCoords = {};
    switch (mDoorType)
    {
        case relive::Path_Door::DoorTypes::eBasicDoor:
        {
            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable[idx][1]);
            if (openDoor.mFrameTableOffset)
            {
                if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
                {
                    Animation_Init(GetAnimRes(sDoorAnimdIdTable[idx][1]));
                }
                else
                {
                    Animation_Init(GetAnimRes(sDoorAnimdIdTable[idx][0]));
                }

                if (pTlv->mScale == relive::reliveScale::eHalf)
                {
                    SetSpriteScale(FP_FromDouble(0.5));
                    SetScale(Scale::Bg);
                    GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
                }
                else
                {
                    SetSpriteScale(FP_FromInteger(1));
                    SetScale(Scale::Fg);
                    GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
                }

                if (sCollisions->Raycast(
                    FP_FromInteger(pTlv->mTopLeftX + (pTlv->Width()) / 2),
                    FP_FromInteger(pTlv->mTopLeftY),
                    FP_FromInteger(pTlv->mTopLeftX + (pTlv->Width()) / 2),
                    FP_FromInteger(pTlv->mBottomRightY),
                    &pLine,
                    &mXPos,
                    &mYPos,
                        GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
                {
                    mYPos -= (FP_FromInteger(12) * GetSpriteScale());
                    gMap.GetCurrentCamCoords(&mapCoords);
                    auto aux = SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos) - mapCoords.x);
                    mXPos = FP_FromInteger((aux)+mapCoords.x);
                }
                else
                {
                    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
                    mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);
                }
                break;
            }
            SetDrawable(false);
            SetDead(true);
            return;
        }

        case relive::Path_Door::DoorTypes::eTrialDoor:
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
                scale = FP_FromInteger(1);
            }
            else
            {
                GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
                scale = FP_FromDouble(0.5);
            }
            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable[idx][3]);
            if (openDoor.mFrameTableOffset == 0)
            {
                SetDrawable(false);
                SetDead(true);
                return;
            }

            if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
            {
                Animation_Init(GetAnimRes(sDoorAnimdIdTable[idx][3]));
            }
            else
            {
                Animation_Init(GetAnimRes(sDoorAnimdIdTable[idx][2]));
            }

            if (sCollisions->Raycast(
                FP_FromInteger(pTlv->mTopLeftX) + FP_FromInteger((pTlv->Width()) / 2),
                FP_FromInteger(pTlv->mTopLeftY),
                FP_FromInteger(pTlv->mTopLeftX) + FP_FromInteger((pTlv->Width()) / 2),
                FP_FromInteger(pTlv->mBottomRightY),
                &pLine,
                &mXPos,
                &mYPos,
                    scale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos += FP_FromInteger(4);
                gMap.GetCurrentCamCoords(&mapCoords);
                mXPos = FP_FromInteger(SnapToXGrid(scale, FP_GetExponent(mXPos) - mapCoords.x) + mapCoords.x);
            }
            else
            {
                mXPos = FP_FromInteger(pTlv->mTopLeftX);
                mYPos = FP_FromInteger(pTlv->mTopLeftY);
            }
            SetSpriteScale(FP_FromInteger(1));
            break;
        }

        case relive::Path_Door::DoorTypes::eHubDoor:
        {
            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable[idx][5]);
            if (openDoor.mFrameTableOffset)
            {
                if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
                {
                    Animation_Init(GetAnimRes(sDoorAnimdIdTable[idx][5]));
                }
                else
                {
                    Animation_Init(GetAnimRes(sDoorAnimdIdTable[idx][4]));
                }

                GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms)
                {
                    if (sCollisions->Raycast(
                        FP_FromInteger(pTlv->mTopLeftX + (pTlv->Width()) / 2),
                        FP_FromInteger(pTlv->mTopLeftY),
                        FP_FromInteger(pTlv->mTopLeftX + (pTlv->Width()) / 2),
                        FP_FromInteger(pTlv->mBottomRightY),
                        &pLine,
                        &mXPos,
                        &mYPos,
                        kFgWallsOrFloor)) // ?? only check bg for some reason
                    {
                        mYPos -= (FP_FromInteger(12) * GetSpriteScale());
                        gMap.GetCurrentCamCoords(&mapCoords);
                        mXPos = FP_FromInteger(SnapToXGrid(FP_FromInteger(1), FP_GetExponent(mXPos) - mapCoords.x) + mapCoords.x);
                    }
                    else
                    {
                        mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
                        mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);
                    }
                }
                else
                {
                    mXPos = FP_FromInteger(pTlv->mTopLeftX + 9);
                    mYPos = FP_FromInteger(pTlv->mTopLeftY + 20);
                }

                SetSpriteScale(FP_FromInteger(1));

                mHubIds[0] = pTlv->mHub1;
                mHubIds[1] = pTlv->mHub2;
                mHubIds[2] = pTlv->mHub3;
                mHubIds[3] = pTlv->mHub4;
                mHubIds[4] = pTlv->mHub5;
                mHubIds[5] = pTlv->mHub6;
                mHubIds[6] = pTlv->mHub7;
                mHubIds[7] = pTlv->mHub8;
                break;
            }
        }
    }

    mXPos += FP_FromInteger(pTlv->mDoorOffsetX);
    mYPos += FP_FromInteger(pTlv->mDoorOffsetY);

    if (mCurrentState == relive::Path_Door::DoorStates::eOpen)
    {
        GetAnimation().SetRender(false);
    }
    GetAnimation().SetAnimate(false);
}

Door::~Door()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

void Door::VScreenChanged()
{
    SetDead(true);
}

bool Door::vIsOpen_40E800()
{
    return mCurrentState == relive::Path_Door::DoorStates::eOpen;
}

void Door::vClose()
{
    if (mCurrentState != relive::Path_Door::DoorStates::eClosed)
    {
        mCurrentState = relive::Path_Door::DoorStates::eClosing;
    }
}

void Door::vOpen()
{
    if (mCurrentState != relive::Path_Door::DoorStates::eOpen)
    {
        mCurrentState = relive::Path_Door::DoorStates::eOpening;
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

    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
    {
        volume = GetSpriteScale() != FP_FromDouble(0.5) ? 90 : 127;
        SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 75, 75);
    }
    else if (mDoorType == relive::Path_Door::DoorTypes::eBasicDoor && GetSpriteScale() == FP_FromInteger(1))
    {
        volume = 90;
    }
    else
    {
        volume = 60;
    }
    SFX_Play_Pitch(relive::SoundEffects::DoorEffect, volume, 900);
    SFX_Play_Pitch(relive::SoundEffects::DoorEffect, volume, 0);
}

void Door::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_156_DoorEnter || sActiveHero->mCurrentMotion == eAbeMotions::Motion_157_DoorExit)
    {
        if (mCurrentState == relive::Path_Door::DoorStates::eClosed && mDoorId == sActiveHero->field_196_door_id)
        {
            GetAnimation().SetRender(false);
            mCurrentState = relive::Path_Door::DoorStates::eOpen;
        }
    }
    else
    {
        mDoorId = -1;

        if (mDoorType == relive::Path_Door::DoorTypes::eHubDoor)
        {
            if (SwitchStates_Get(mHubIds[0]) &&
                SwitchStates_Get(mHubIds[1]) &&
                SwitchStates_Get(mHubIds[2]) &&
                SwitchStates_Get(mHubIds[3]) &&
                SwitchStates_Get(mHubIds[4]) &&
                SwitchStates_Get(mHubIds[5]) &&
                SwitchStates_Get(mHubIds[6]) &&
                SwitchStates_Get(mHubIds[7]))

            {
                if (!SwitchStates_Get(mSwitchId))
                {
                    SND_SEQ_Play_477760(SeqId::eSecretMusic_46, 1, 127, 127);
                    relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eChime, relive::Path_MusicTrigger::TriggeredBy::eTimer, 0, 300);
                }
                SwitchStates_Do_Operation(mSwitchId, relive::reliveSwitchOp::eSetTrue);
            }
            else
            {
                SwitchStates_Do_Operation(mSwitchId, relive::reliveSwitchOp::eSetFalse);
            }
        }

        const s32 lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

        switch (mCurrentState)
        {
            case relive::Path_Door::DoorStates::eOpen:
                GetAnimation().SetRender(false);

                if ((mDoorClosed == relive::reliveChoice::eNo && SwitchStates_Get(mSwitchId)) || (mDoorClosed == relive::reliveChoice::eYes && !SwitchStates_Get(mSwitchId)))
                {
                    mCurrentState = relive::Path_Door::DoorStates::eClosing;

                    switch (mDoorType)
                    {
                        case relive::Path_Door::DoorTypes::eBasicDoor:
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(sDoorAnimdIdTable[lvl][1]));
                            break;
                        }

                        case relive::Path_Door::DoorTypes::eTrialDoor:
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(sDoorAnimdIdTable[lvl][3]));
                            break;
                        }

                        //relive::Path_Door::DoorTypes::eHubDoor:
                        default:
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(sDoorAnimdIdTable[lvl][5]));
                            break;
                        }
                    }

                    GetAnimation().SetLoopBackwards(false);
                    GetAnimation().SetRender(true);
                }
                break;

            case relive::Path_Door::DoorStates::eClosed:
                GetAnimation().SetRender(true);
                GetAnimation().SetAnimate(true);

                if ((mDoorClosed == relive::reliveChoice::eYes && SwitchStates_Get(mSwitchId)) || (mDoorClosed == relive::reliveChoice::eNo && !SwitchStates_Get(mSwitchId)))
                {
                    mCurrentState = relive::Path_Door::DoorStates::eOpening;

                    switch (mDoorType)
                    {
                        case relive::Path_Door::DoorTypes::eBasicDoor:
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(sDoorAnimdIdTable[lvl][1]));
                            break;
                        }

                        case relive::Path_Door::DoorTypes::eTrialDoor:
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(sDoorAnimdIdTable[lvl][3]));
                            break;
                        }

                        //relive::Path_Door::DoorTypes::eHubDoor
                        default:
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(sDoorAnimdIdTable[lvl][5]));
                            break;
                        }
                    }

                    GetAnimation().SetFrame(3u);
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

} // namespace AO
