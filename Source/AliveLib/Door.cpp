#include "stdafx.h"
#include "Door.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "MusicTrigger.hpp"
#include "Midi.hpp"

struct Door_Info
{
    int field_0_frameTableOffset;
    int field_4_maxW;
    int field_8_maxH;
    int field_C;
};
ALIVE_ASSERT_SIZEOF(Door_Info, 0x10);

const Door_Info sDoorInfos_54488C[] =
{
    { 8704, 77, 69, 8692 },
    { 8704, 77, 69, 5436 },
    { 5448, 67, 62, 5436 },
    { 5448, 67, 62, 5436 },
    { 5448, 67, 62, 7636 },
    { 7648, 62, 70, 5652 },
    { 5664, 56, 62, 5436 },
    { 5448, 67, 62, 6304 },
    { 6316, 59, 58, 6224 },
    { 6236, 54, 71, 6224 },
    { 6236, 54, 71, 5436 },
    { 5448, 67, 62, 7636 },
    { 7648, 62, 70, 5652 },
    { 5664, 56, 62, 6304 },
    { 6316, 59, 58, 8692 },
    { 8704, 77, 69, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

Door* Door::ctor_41E250(Path_Door* /*pTlvData*/, int /*tlvInfo*/)
{
    NOT_IMPLEMENTED();
    return this;
}

BOOL Door::vIsOpen_41EB00()
{
    return field_FC_current_state == eOpen;
}

void Door::vOpen_41EB20()
{
    if (field_FC_current_state != eOpen)
    {
        field_FC_current_state = eOpening;
    }
}

void Door::vClose_41EB50()
{
    if (field_FC_current_state != eClosed)
    {
        field_FE_start_state = eClosed;
        field_FC_current_state = eClosing;
        Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_F4_tlvInfo);
        pTlv->field_1_unknown = 1;
    }
}

void Door::vSetOpen_41EBA0()
{
    field_FC_current_state = eOpen;
}

void Door::vSetClosed_41EBC0()
{
    field_FC_current_state = eClosed;
}

void Door::vScreenChange_41F080()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

Door* Door::vdtor_41E9D0(signed int flags)
{
    dtor_41EA00();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Door::PlaySound_41EA90()
{
    __int16 volume = 0;
    if (field_F8_door_type != 0 || field_CC_sprite_scale != FP_FromInteger(1))
    {
        volume = 60;
    }
    else
    {
        volume = 90;
    }

    SFX_Play_46FBA0(0x39u, volume, 900, 0x10000);
    SFX_Play_46FBA0(0x39u, volume, 0, 0x10000);
}

void Door::dtor_41EA00()
{
    NOT_IMPLEMENTED();

    SetVTable(this, 0x5449BC); // vTbl_Door_5449BC
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    // TODO
    //dtor_41E130();
}

void Door::vUpdate_41EBE0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (sActiveHero_5C1B68->field_106_current_state == 114 || sActiveHero_5C1B68->field_106_current_state == 115)
    {
        if (field_FC_current_state == eClosed && field_FA_door_number == sActiveHero_5C1B68->field_1A0_door_id)
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_FC_current_state = eOpen;
        }
    }
    else
    {
        field_FA_door_number = -1;
        if (field_F8_door_type == 2 || field_F8_door_type == 3)
        {
            if (SwitchStates_Get_466020(field_102_hub_ids[0])
                && SwitchStates_Get_466020(field_102_hub_ids[1])
                && SwitchStates_Get_466020(field_102_hub_ids[2])
                && SwitchStates_Get_466020(field_102_hub_ids[3])
                && SwitchStates_Get_466020(field_102_hub_ids[4])
                && SwitchStates_Get_466020(field_102_hub_ids[5])
                && SwitchStates_Get_466020(field_102_hub_ids[6])
                && SwitchStates_Get_466020(field_102_hub_ids[7]))
            {
                if (!SwitchStates_Get_466020(field_100_switch_id) && field_F8_door_type == 2)
                {
                    SND_SEQ_Play_4CAB10(0x20u, 1, 127, 127);
                    auto pMusicTrigger = alive_new<MusicTrigger>();
                    if (pMusicTrigger)
                    {
                        pMusicTrigger->ctor_47FF10(5, 0, 0, 0);
                    }
                }
                SwitchStates_Do_Operation_465F00(field_100_switch_id, eSetTrue);
            }
            else
            {
                SwitchStates_Do_Operation_465F00(field_100_switch_id, eSetFalse);
            }
        }
        switch (field_FC_current_state)
        {
        case eOpen:
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

            if (!field_FE_start_state && SwitchStates_Get_466020(field_100_switch_id)
                || field_FE_start_state == 1 && !SwitchStates_Get_466020(field_100_switch_id))
            {
                field_FC_current_state = eClosing;
                if (gMap_5C3030.field_22 == 108)
                {
                    field_20_animation.Set_Animation_Data_409C80(6616, nullptr);
                }
                else
                {
                    field_20_animation.Set_Animation_Data_409C80(
                        sDoorInfos_54488C[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_0_frameTableOffset,
                        nullptr);
                }

                field_20_animation.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }
            break;

        case eClosed:
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

            if (field_FE_start_state == eClosed && SwitchStates_Get_466020(field_100_switch_id) ||
                !field_FE_start_state && !SwitchStates_Get_466020(field_100_switch_id))
            {
                field_FC_current_state = eOpening;
                if (gMap_5C3030.field_22 == 108)
                {
                    field_20_animation.Set_Animation_Data_409C80(6616, nullptr);
                }
                else
                {
                    field_20_animation.Set_Animation_Data_409C80(
                        sDoorInfos_54488C[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_0_frameTableOffset,
                        nullptr);
                }

                field_20_animation.SetFrame_409D50(3);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                PlaySound_41EA90();
            }
            break;

        case eOpening:
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FC_current_state = eOpening;
            }
            break;

        case eClosing:
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FC_current_state = eClosed;
                PlaySound_41EA90();
            }
            break;

        default:
            return;
        }
    }
}
