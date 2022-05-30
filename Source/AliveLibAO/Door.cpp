#include "stdafx_ao.h"
#include "Function.hpp"
#include "Door.hpp"
#include "Abe.hpp"
#include "SwitchStates.hpp"
#include "ResourceManager.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
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

const AnimId sDoorAnimdIdTable_4BA508[16][6] = {
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
    field_4_typeId = Types::eDoor_21;

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

    if ((sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_156_DoorEnter_42D370 || sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_157_DoorExit_42D780) &&
        field_EC_current_state == DoorStates::eClosed_1 && field_EA_door_number == sActiveHero_507678->field_196_door_id)
    {
        field_EC_current_state = DoorStates::eOpen_0;
    }

    const s32 idx = static_cast<s32>(gMap.mCurrentLevel);

    FP scale = {};
    PathLine* pLine = nullptr;
    u8** ppRes = nullptr;
    PSX_Point mapCoords = {};
    switch (field_E8_start_state)
    {
        case DoorStates::eOpen_0:
        {
            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[idx][1]);
            ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, openDoor.mResourceId, 1, 0);
            if (!ppRes)
            {
                mFlags.Clear(BaseGameObject::eDrawable_Bit4);
                mFlags.Set(BaseGameObject::eDead);
                return;
            }

            if (openDoor.mFrameTableOffset)
            {
                if (field_EC_current_state == DoorStates::eOpen_0)
                {
                    Animation_Init_417FD0(
                        openDoor.mFrameTableOffset,
                        openDoor.mMaxW,
                        openDoor.mMaxH,
                        ppRes,
                        1);
                }
                else
                {
                    const AnimRecord& closedDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[idx][0]);
                    Animation_Init_417FD0(
                        closedDoor.mFrameTableOffset,
                        closedDoor.mMaxW,
                        closedDoor.mMaxH,
                        ppRes,
                        1);
                }

                if (pTlv->field_1E_scale == Scale_short::eHalf_1)
                {
                    field_BC_sprite_scale = FP_FromDouble(0.5);
                    field_C6_scale = 0;
                    field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_Half_6;
                }
                else
                {
                    field_BC_sprite_scale = FP_FromInteger(1);
                    field_C6_scale = 1;
                    field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_25;
                }

                if (sCollisions_DArray_504C6C->RayCast(
                    FP_FromInteger(pTlv->field_10_top_left.field_0_x + (pTlv->field_14_bottom_right.field_0_x - pTlv->field_10_top_left.field_0_x) / 2),
                    FP_FromInteger(pTlv->field_10_top_left.field_2_y),
                    FP_FromInteger(pTlv->field_10_top_left.field_0_x + (pTlv->field_14_bottom_right.field_0_x - pTlv->field_10_top_left.field_0_x) / 2),
                    FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
                    &pLine,
                    &field_A8_xpos,
                    &field_AC_ypos,
                    field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70))
                {
                    field_AC_ypos -= (FP_FromInteger(12) * field_BC_sprite_scale);
                    gMap.GetCurrentCamCoords_444890(&mapCoords);
                    auto aux = SnapToXGrid(field_BC_sprite_scale, FP_GetExponent(field_A8_xpos) - mapCoords.field_0_x);
                    field_A8_xpos = FP_FromInteger((aux)+mapCoords.field_0_x);
                }
                else
                {
                    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
                    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 24);
                }
                break;
            }
            ResourceManager::FreeResource_455550(ppRes);
            mFlags.Clear(BaseGameObject::eDrawable_Bit4);
            mFlags.Set(BaseGameObject::eDead);
            return;
        }

        case DoorStates::eClosed_1:
        {
            if (gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
            {
                field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_25;
                scale = FP_FromInteger(1);
            }
            else
            {
                field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_Half_6;
                scale = FP_FromDouble(0.5);
            }
            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[idx][3]);
            ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, openDoor.mResourceId, 1, 0);
            if (!ppRes || openDoor.mFrameTableOffset == 0)
            {
                mFlags.Clear(BaseGameObject::eDrawable_Bit4);
                mFlags.Set(BaseGameObject::eDead);
                return;
            }

            if (field_EC_current_state == DoorStates::eOpen_0)
            {
                Animation_Init_417FD0(
                    openDoor.mFrameTableOffset,
                    openDoor.mMaxW,
                    openDoor.mMaxH,
                    ppRes,
                    1);
            }
            else
            {
                const AnimRecord& closedDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[idx][2]);
                Animation_Init_417FD0(
                    closedDoor.mFrameTableOffset,
                    closedDoor.mMaxW,
                    closedDoor.mMaxH,
                    ppRes,
                    1);
            }

            if (sCollisions_DArray_504C6C->RayCast(
                FP_FromInteger(pTlv->field_10_top_left.field_0_x) + FP_FromInteger((pTlv->field_14_bottom_right.field_0_x - pTlv->field_10_top_left.field_0_x) / 2),
                FP_FromInteger(pTlv->field_10_top_left.field_2_y),
                FP_FromInteger(pTlv->field_10_top_left.field_0_x) + FP_FromInteger((pTlv->field_14_bottom_right.field_0_x - pTlv->field_10_top_left.field_0_x) / 2),
                FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
                &pLine,
                &field_A8_xpos,
                &field_AC_ypos,
                scale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                field_AC_ypos += FP_FromInteger(4);
                gMap.GetCurrentCamCoords_444890(&mapCoords);
                field_A8_xpos = FP_FromInteger(SnapToXGrid(scale, FP_GetExponent(field_A8_xpos) - mapCoords.field_0_x) + mapCoords.field_0_x);
            }
            else
            {
                field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
                field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
            }
            field_BC_sprite_scale = FP_FromInteger(1);
            break;
        }

        case DoorStates::eHubDoorClosed_2:
            if (gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13 || gMap.mCurrentLevel == LevelIds::eRuptureFarms_1)
            {
                ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kF2p3dorAOResID, 1, 0);
            }
            else
            {
                ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kRockdoorAOResID, 1, 0);
            }

            if (ppRes)
            {
                const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[idx][5]);
                if (openDoor.mFrameTableOffset)
                {
                    if (field_EC_current_state == DoorStates::eOpen_0)
                    {
                        Animation_Init_417FD0(
                            openDoor.mFrameTableOffset,
                            openDoor.mMaxW,
                            openDoor.mMaxH,
                            ppRes,
                            1);
                    }
                    else
                    {
                        const AnimRecord& closedDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[idx][4]);
                        Animation_Init_417FD0(
                            closedDoor.mFrameTableOffset,
                            closedDoor.mMaxW,
                            closedDoor.mMaxH,
                            ppRes,
                            1);
                    }

                    field_10_anim.field_C_layer = Layer::eLayer_BeforeShadow_Half_6;

                    if (gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13 || gMap.mCurrentLevel == LevelIds::eRuptureFarms_1)
                    {
                        if (sCollisions_DArray_504C6C->RayCast(
                                FP_FromInteger(pTlv->field_10_top_left.field_0_x + (pTlv->field_14_bottom_right.field_0_x - pTlv->field_10_top_left.field_0_x) / 2),
                                FP_FromInteger(pTlv->field_10_top_left.field_2_y),
                                FP_FromInteger(pTlv->field_10_top_left.field_0_x + (pTlv->field_14_bottom_right.field_0_x - pTlv->field_10_top_left.field_0_x) / 2),
                                FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
                                &pLine,
                                &field_A8_xpos,
                                &field_AC_ypos,
                                7))
                        {
                            field_AC_ypos -= (FP_FromInteger(12) * field_BC_sprite_scale);
                            gMap.GetCurrentCamCoords_444890(&mapCoords);
                            field_A8_xpos = FP_FromInteger(SnapToXGrid(FP_FromInteger(1), FP_GetExponent(field_A8_xpos) - mapCoords.field_0_x) + mapCoords.field_0_x);
                        }
                        else
                        {
                            field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
                            field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 24);
                        }
                    }
                    else
                    {
                        field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 9);
                        field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 20);
                    }

                    field_BC_sprite_scale = FP_FromInteger(1);

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
            mFlags.Clear(BaseGameObject::eDrawable_Bit4);
            mFlags.Set(BaseGameObject::eDead);
            return;
    }

    field_A8_xpos += FP_FromInteger(pTlv->field_3E_x_offset);
    field_AC_ypos += FP_FromInteger(pTlv->field_40_y_offset);

    if (field_EC_current_state == DoorStates::eOpen_0)
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
}

Door::~Door()
{
    gMap.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
}

void Door::VScreenChanged()
{
    mFlags.Set(BaseGameObject::eDead);
}

Bool32 Door::vIsOpen_40E800()
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

    if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
    {
        volume = field_BC_sprite_scale != FP_FromDouble(0.5) ? 90 : 127;
        SND_SEQ_Play_477760(SeqId::eHitBottomOfDeathPit_10, 1, 75, 75);
    }
    else if (field_E8_start_state == DoorStates::eOpen_0 && field_BC_sprite_scale == FP_FromInteger(1))
    {
        volume = 90;
    }
    else
    {
        volume = 60;
    }
    SFX_Play_43AE60(SoundEffect::DoorEffect_66, volume, 900);
    SFX_Play_43AE60(SoundEffect::DoorEffect_66, volume, 0);
}

void Door::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mFlags.Set(Options::eDead);
    }

    if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_156_DoorEnter_42D370 || sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_157_DoorExit_42D780)
    {
        if (field_EC_current_state == DoorStates::eClosed_1 && field_EA_door_number == sActiveHero_507678->field_196_door_id)
        {
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
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
                    ao_new<MusicTrigger>(MusicTriggerMusicType::eChime_5, TriggeredBy::eTimer_0, 0, 300);
                }
                SwitchStates_Do_Operation_436A10(field_F0_switch_id, SwitchOp::eSetTrue_0);
            }
            else
            {
                SwitchStates_Do_Operation_436A10(field_F0_switch_id, SwitchOp::eSetFalse_1);
            }
        }

        const s32 lvl = static_cast<s32>(gMap.mCurrentLevel);

        switch (field_EC_current_state)
        {
            case DoorStates::eOpen_0:
                field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);

                if ((field_EE_door_closed == Choice_short::eNo_0 && SwitchStates_Get(field_F0_switch_id)) || (field_EE_door_closed == Choice_short::eYes_1 && !SwitchStates_Get(field_F0_switch_id)))
                {
                    field_EC_current_state = DoorStates::eClosing_3;

                    switch (field_E8_start_state)
                    {
                        case DoorStates::eOpen_0:
                        {
                            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[lvl][1]);
                            field_10_anim.Set_Animation_Data(openDoor.mFrameTableOffset, nullptr);
                            break;
                        }

                        case DoorStates::eClosed_1:
                        {
                            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[lvl][3]);
                            field_10_anim.Set_Animation_Data(openDoor.mFrameTableOffset, nullptr);
                            break;
                        }

                        case DoorStates::eHubDoorClosed_2:
                        {
                        default:
                            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[lvl][5]);
                            field_10_anim.Set_Animation_Data(openDoor.mFrameTableOffset, nullptr);
                            break;
                        }
                    }

                    field_10_anim.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
                    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
                }
                break;

            case DoorStates::eClosed_1:
                field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);

                if ((field_EE_door_closed == Choice_short::eYes_1 && SwitchStates_Get(field_F0_switch_id)) || (field_EE_door_closed == Choice_short::eNo_0 && !SwitchStates_Get(field_F0_switch_id)))
                {
                    field_EC_current_state = DoorStates::eHubDoorClosed_2;

                    switch (field_E8_start_state)
                    {
                        case DoorStates::eOpen_0:
                        {
                            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[lvl][1]);
                            field_10_anim.Set_Animation_Data(openDoor.mFrameTableOffset, nullptr);
                            break;
                        }

                        case DoorStates::eClosed_1:
                        {
                            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[lvl][3]);
                            field_10_anim.Set_Animation_Data(openDoor.mFrameTableOffset, nullptr);
                            break;
                        }

                        case DoorStates::eHubDoorClosed_2:
                        {
                        default:
                            const AnimRecord& openDoor = AO::AnimRec(sDoorAnimdIdTable_4BA508[lvl][5]);
                            field_10_anim.Set_Animation_Data(openDoor.mFrameTableOffset, nullptr);
                            break;
                        }
                    }

                    field_10_anim.SetFrame(3u);
                    field_10_anim.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
                    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
                    PlaySound();
                }
                break;

            case DoorStates::eHubDoorClosed_2:
                field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    field_EC_current_state = DoorStates::eOpen_0;
                }
                break;

            case DoorStates::eClosing_3:
                field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_10_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
                if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
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
