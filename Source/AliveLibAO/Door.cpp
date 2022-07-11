#include "stdafx_ao.h"
#include "Function.hpp"
#include "Door.hpp"
#include "Abe.hpp"
#include "SwitchStates.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "MusicTrigger.hpp"
#include "Grid.hpp"

namespace AO {

struct Door_Data final
{
    s32 field_0_closed_frame_table_offset;
    s32 field_4_open_frame_table_offset;
    s32 field_8_maxW;
    s32 field_C_maxH;
    s32 field_10_closed_frame_table_offset;
    s32 field_14_open_frame_table_offset;
    s32 field_18_maxW;
    s32 field_1C_maxH;
    s32 field_20_closed_frame_table_offset;
    s32 field_24_open_frame_table_offset;
    s32 field_28_maxW;
    s32 field_2C_maxH;
};
ALIVE_ASSERT_SIZEOF(Door_Data, 0x30);

/*const Door_Data sDoorData_4BA508[16] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // menu
    {6632, 6608, 56, 62, 6632, 6608, 56, 62, 6632, 6608, 56, 62}, // rupture farms
    {4784, 4760, 55, 48, 4784, 4760, 55, 48, 4784, 4760, 55, 48}, // lines
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // forest
    {6624, 6600, 63, 62, 2036, 2012, 34, 29, 2072, 2048, 51, 27}, // forest temple
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // stock yards
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // stock yards return
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // removed
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // desert
    {3176, 3152, 52, 69, 1048, 1024, 21, 29, 1016, 992, 26, 31}, // desert temple
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // credits
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // removed
    {6632, 6608, 56, 62, 6632, 6608, 56, 62, 6632, 6608, 56, 62}, // board room
    {6632, 6608, 56, 62, 6632, 6608, 56, 62, 6632, 6608, 56, 62}, // rupture farms return
    {6624, 6600, 63, 62, 2036, 2012, 34, 29, 2072, 2048, 51, 27}, // forest chase
    {3176, 3152, 52, 69, 1048, 1024, 21, 29, 1016, 992, 26, 31}}; // desert escape*/

const AnimId sDoorAnimdIdTable[16][6] = {
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

Door::Door(Path_Door* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eDoor;

    field_E4_tlvInfo = tlvInfo;

    field_E8_start_state = pTlv->field_26_start_state;
    field_EE_door_closed = pTlv->field_28_door_closed;
    field_F0_switch_id = pTlv->field_22_switch_id;

    if (field_F0_switch_id == 1)
    {
        field_F0_switch_id = 0;
    }

    field_EA_door_number = pTlv->field_20_door_number;

    field_EC_current_state = (field_EE_door_closed == Choice_short::eNo_0) == SwitchStates_Get(field_F0_switch_id) ? DoorStates::eClosed_1 : DoorStates::eOpen_0;

    if ((sActiveHero->mCurrentMotion == eAbeMotions::Motion_156_DoorEnter_42D370 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_157_DoorExit_42D780) &&
        field_EC_current_state == DoorStates::eClosed_1 && field_EA_door_number == sActiveHero->field_196_door_id)
    {
        field_EC_current_state = DoorStates::eOpen_0;
    }

    const s32 idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

    FP scale = {};
    PathLine* pLine = nullptr;
    u8** ppRes = nullptr;
    PSX_Point mapCoords = {};
    switch (field_E8_start_state)
    {
        case DoorStates::eOpen_0:
        {
            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable[idx][1]);
            ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, openDoor.mResourceId, 1, 0);
            if (!ppRes)
            {
                mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            if (openDoor.mFrameTableOffset)
            {
                if (field_EC_current_state == DoorStates::eOpen_0)
                {
                    Animation_Init(sDoorAnimdIdTable[idx][1], ppRes);
                }
                else
                {
                    Animation_Init(sDoorAnimdIdTable[idx][0], ppRes);
                }

                if (pTlv->field_1E_scale == Scale_short::eHalf_1)
                {
                    mSpriteScale = FP_FromDouble(0.5);
                    mScale = Scale::Bg;
                    mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
                }
                else
                {
                    mSpriteScale = FP_FromInteger(1);
                    mScale = Scale::Fg;
                    mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
                }

                if (sCollisions->Raycast(
                    FP_FromInteger(pTlv->mTopLeft.x + (pTlv->mBottomRight.x - pTlv->mTopLeft.x) / 2),
                    FP_FromInteger(pTlv->mTopLeft.y),
                    FP_FromInteger(pTlv->mTopLeft.x + (pTlv->mBottomRight.x - pTlv->mTopLeft.x) / 2),
                    FP_FromInteger(pTlv->mBottomRight.y),
                    &pLine,
                    &mXPos,
                    &mYPos,
                        mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
                {
                    mYPos -= (FP_FromInteger(12) * mSpriteScale);
                    gMap.GetCurrentCamCoords(&mapCoords);
                    auto aux = SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos) - mapCoords.x);
                    mXPos = FP_FromInteger((aux)+mapCoords.x);
                }
                else
                {
                    mXPos = FP_FromInteger(pTlv->mTopLeft.x + 12);
                    mYPos = FP_FromInteger(pTlv->mTopLeft.y + 24);
                }
                break;
            }
            ResourceManager::FreeResource_455550(ppRes);
            mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return;
        }

        case DoorStates::eClosed_1:
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
                scale = FP_FromInteger(1);
            }
            else
            {
                mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
                scale = FP_FromDouble(0.5);
            }
            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable[idx][3]);
            ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, openDoor.mResourceId, 1, 0);
            if (!ppRes || openDoor.mFrameTableOffset == 0)
            {
                mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            if (field_EC_current_state == DoorStates::eOpen_0)
            {
                Animation_Init(sDoorAnimdIdTable[idx][3], ppRes);
            }
            else
            {
                Animation_Init(sDoorAnimdIdTable[idx][2], ppRes);
            }

            if (sCollisions->Raycast(
                FP_FromInteger(pTlv->mTopLeft.x) + FP_FromInteger((pTlv->mBottomRight.x - pTlv->mTopLeft.x) / 2),
                FP_FromInteger(pTlv->mTopLeft.y),
                FP_FromInteger(pTlv->mTopLeft.x) + FP_FromInteger((pTlv->mBottomRight.x - pTlv->mTopLeft.x) / 2),
                FP_FromInteger(pTlv->mBottomRight.y),
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
                mXPos = FP_FromInteger(pTlv->mTopLeft.x);
                mYPos = FP_FromInteger(pTlv->mTopLeft.y);
            }
            mSpriteScale = FP_FromInteger(1);
            break;
        }

        case DoorStates::eHubDoorClosed_2:
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms)
            {
                ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kF2p3dorAOResID, 1, 0);
            }
            else
            {
                ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kRockdoorAOResID, 1, 0);
            }

            if (ppRes)
            {
                const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable[idx][5]);
                if (openDoor.mFrameTableOffset)
                {
                    if (field_EC_current_state == DoorStates::eOpen_0)
                    {
                        Animation_Init(sDoorAnimdIdTable[idx][5], ppRes);
                    }
                    else
                    {
                        Animation_Init(sDoorAnimdIdTable[idx][4], ppRes);
                    }

                    mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;

                    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms)
                    {
                        if (sCollisions->Raycast(
                                FP_FromInteger(pTlv->mTopLeft.x + (pTlv->mBottomRight.x - pTlv->mTopLeft.x) / 2),
                                FP_FromInteger(pTlv->mTopLeft.y),
                                FP_FromInteger(pTlv->mTopLeft.x + (pTlv->mBottomRight.x - pTlv->mTopLeft.x) / 2),
                                FP_FromInteger(pTlv->mBottomRight.y),
                                &pLine,
                                &mXPos,
                                &mYPos,
                                kFgWallsOrFloor)) // ?? only check bg for some reason
                        {
                            mYPos -= (FP_FromInteger(12) * mSpriteScale);
                            gMap.GetCurrentCamCoords(&mapCoords);
                            mXPos = FP_FromInteger(SnapToXGrid(FP_FromInteger(1), FP_GetExponent(mXPos) - mapCoords.x) + mapCoords.x);
                        }
                        else
                        {
                            mXPos = FP_FromInteger(pTlv->mTopLeft.x + 12);
                            mYPos = FP_FromInteger(pTlv->mTopLeft.y + 24);
                        }
                    }
                    else
                    {
                        mXPos = FP_FromInteger(pTlv->mTopLeft.x + 9);
                        mYPos = FP_FromInteger(pTlv->mTopLeft.y + 20);
                    }

                    mSpriteScale = FP_FromInteger(1);

                    field_F2_hubs_ids[0] = pTlv->field_2A_hub1;
                    field_F2_hubs_ids[1] = pTlv->field_2A_hub2;
                    field_F2_hubs_ids[2] = pTlv->field_2A_hub3;
                    field_F2_hubs_ids[3] = pTlv->field_2A_hub4;
                    field_F2_hubs_ids[4] = pTlv->field_2A_hub5;
                    field_F2_hubs_ids[5] = pTlv->field_2A_hub6;
                    field_F2_hubs_ids[6] = pTlv->field_2A_hub7;
                    field_F2_hubs_ids[7] = pTlv->field_2A_hub8;
                    break;
                }
            }
            mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return;
    }

    mXPos += FP_FromInteger(pTlv->field_3E_x_offset);
    mYPos += FP_FromInteger(pTlv->field_40_y_offset);

    if (field_EC_current_state == DoorStates::eOpen_0)
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
    }
    mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
}

Door::~Door()
{
    Path::TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
}

void Door::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

bool Door::vIsOpen_40E800()
{
    return field_EC_current_state == DoorStates::eOpen_0;
}

void Door::vClose()
{
    if (field_EC_current_state != DoorStates::eClosed_1)
    {
        field_EC_current_state = DoorStates::eClosing_3;
    }
}

void Door::vOpen()
{
    if (field_EC_current_state != DoorStates::eOpen_0)
    {
        field_EC_current_state = DoorStates::eHubDoorClosed_2;
    }
}

void Door::vSetOpen()
{
    field_EC_current_state = DoorStates::eOpen_0;
}

void Door::vSetClosed()
{
    field_EC_current_state = DoorStates::eClosed_1;
}

void Door::PlaySound()
{
    s16 volume = 0;

    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
    {
        volume = mSpriteScale != FP_FromDouble(0.5) ? 90 : 127;
        SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 75, 75);
    }
    else if (field_E8_start_state == DoorStates::eOpen_0 && mSpriteScale == FP_FromInteger(1))
    {
        volume = 90;
    }
    else
    {
        volume = 60;
    }
    SFX_Play_Pitch(SoundEffect::DoorEffect_66, volume, 900);
    SFX_Play_Pitch(SoundEffect::DoorEffect_66, volume, 0);
}

void Door::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_156_DoorEnter_42D370 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_157_DoorExit_42D780)
    {
        if (field_EC_current_state == DoorStates::eClosed_1 && field_EA_door_number == sActiveHero->field_196_door_id)
        {
            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
            field_EC_current_state = DoorStates::eOpen_0;
        }
    }
    else
    {
        field_EA_door_number = -1;

        if (field_E8_start_state == DoorStates::eHubDoorClosed_2)
        {
            if (SwitchStates_Get(field_F2_hubs_ids[0]) &&
                SwitchStates_Get(field_F2_hubs_ids[1]) &&
                SwitchStates_Get(field_F2_hubs_ids[2]) &&
                SwitchStates_Get(field_F2_hubs_ids[3]) &&
                SwitchStates_Get(field_F2_hubs_ids[4]) &&
                SwitchStates_Get(field_F2_hubs_ids[5]) &&
                SwitchStates_Get(field_F2_hubs_ids[6]) &&
                SwitchStates_Get(field_F2_hubs_ids[7]))

            {
                if (!SwitchStates_Get(field_F0_switch_id))
                {
                    SND_SEQ_Play_477760(SeqId::eSecretMusic_46, 1, 127, 127);
                    relive_new MusicTrigger(MusicTriggerMusicType::eChime_5, TriggeredBy::eTimer_0, 0, 300);
                }
                SwitchStates_Do_Operation(field_F0_switch_id, SwitchOp::eSetTrue_0);
            }
            else
            {
                SwitchStates_Do_Operation(field_F0_switch_id, SwitchOp::eSetFalse_1);
            }
        }

        const s32 lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

        switch (field_EC_current_state)
        {
            case DoorStates::eOpen_0:
                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

                if ((field_EE_door_closed == Choice_short::eNo_0 && SwitchStates_Get(field_F0_switch_id)) || (field_EE_door_closed == Choice_short::eYes_1 && !SwitchStates_Get(field_F0_switch_id)))
                {
                    field_EC_current_state = DoorStates::eClosing_3;

                    switch (field_E8_start_state)
                    {
                        case DoorStates::eOpen_0:
                        {
                            mAnim.Set_Animation_Data(sDoorAnimdIdTable[lvl][1], nullptr);
                            break;
                        }

                        case DoorStates::eClosed_1:
                        {
                            mAnim.Set_Animation_Data(sDoorAnimdIdTable[lvl][3], nullptr);
                            break;
                        }

                        case DoorStates::eHubDoorClosed_2:
                        {
                        default:
                            mAnim.Set_Animation_Data(sDoorAnimdIdTable[lvl][5], nullptr);
                            break;
                        }
                    }

                    mAnim.mFlags.Clear(AnimFlags::eBit19_LoopBackwards);
                    mAnim.mFlags.Set(AnimFlags::eBit3_Render);
                }
                break;

            case DoorStates::eClosed_1:
                mAnim.mFlags.Set(AnimFlags::eBit3_Render);
                mAnim.mFlags.Set(AnimFlags::eBit2_Animate);

                if ((field_EE_door_closed == Choice_short::eYes_1 && SwitchStates_Get(field_F0_switch_id)) || (field_EE_door_closed == Choice_short::eNo_0 && !SwitchStates_Get(field_F0_switch_id)))
                {
                    field_EC_current_state = DoorStates::eHubDoorClosed_2;

                    switch (field_E8_start_state)
                    {
                        case DoorStates::eOpen_0:
                        {
                            mAnim.Set_Animation_Data(sDoorAnimdIdTable[lvl][1], nullptr);
                            break;
                        }

                        case DoorStates::eClosed_1:
                        {
                            mAnim.Set_Animation_Data(sDoorAnimdIdTable[lvl][3], nullptr);
                            break;
                        }

                        case DoorStates::eHubDoorClosed_2:
                        {
                        default:
                            mAnim.Set_Animation_Data(sDoorAnimdIdTable[lvl][5], nullptr);
                            break;
                        }
                    }

                    mAnim.SetFrame(3u);
                    mAnim.mFlags.Set(AnimFlags::eBit19_LoopBackwards);
                    mAnim.mFlags.Set(AnimFlags::eBit3_Render);
                    PlaySound();
                }
                break;

            case DoorStates::eHubDoorClosed_2:
                mAnim.mFlags.Set(AnimFlags::eBit3_Render);
                mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
                if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    field_EC_current_state = DoorStates::eOpen_0;
                }
                break;

            case DoorStates::eClosing_3:
                mAnim.mFlags.Set(AnimFlags::eBit3_Render);
                mAnim.mFlags.Set(AnimFlags::eBit2_Animate);
                if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    field_EC_current_state = DoorStates::eClosed_1;
                    PlaySound();
                }
                break;

            default:
                return;
        }
    }
}

} // namespace AO
