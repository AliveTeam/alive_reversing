#include "stdafx.h"
#include "Switch.hpp"
#include "Function.hpp"
#include "Collisions.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "PathData.hpp"
#include "SwitchStates.hpp"

const TintEntry kSwitchTints_563228[18] =
{
    { 1, 127u, 127u, 127u },
    { 2, 127u, 127u, 127u },
    { 3, 127u, 127u, 127u },
    { 4, 127u, 127u, 127u },
    { 5, 127u, 127u, 127u },
    { 6, 127u, 127u, 127u },
    { 7, 127u, 127u, 127u },
    { 8, 127u, 127u, 127u },
    { 9, 127u, 127u, 127u },
    { 10, 127u, 127u, 127u },
    { 11, 127u, 127u, 127u },
    { 12, 127u, 127u, 127u },
    { 13, 127u, 127u, 127u },
    { 14, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0, 0u, 0u, 0u },
    { 0, 0u, 0u, 0u },
    { 0, 0u, 0u, 0u }
};

BaseGameObject* Switch::VDestructor(signed int flags)
{
    return vdtor_4D5AD0(flags);
}

void Switch::VUpdate()
{
    vUpdate_4D5C00();
}

void Switch::VScreenChanged()
{
    vScreenChanged_4D5B90();
}

__int16 Switch::Vsub_4D6050(__int16 a2)
{
    return vsub_4D6050(a2);
}

Switch* Switch::ctor_4D5860(Path_Switch* pTlv, DWORD tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x547A5C);

    field_4_typeId = BaseGameObject::Types::eLever_139;
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSwitchResID);
    Animation_Init_424E10(5696, 66, 40, ppRes, 1, 1);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_F4_trigger_id = pTlv->field_1A_trigger_id;
    field_102_target_action = pTlv->field_10_target_action;
    field_100_flags = 0;
    field_100_flags = 2 * (pTlv->field_1C_persist_offscreen & 1);

    if (pTlv->field_12_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 6;
        field_D6_scale = 0;
    }
    else if (pTlv->field_12_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 25;
        field_D6_scale = 1;
    }

    SetTint_425600(&kSwitchTints_563228[0], gMap_5C3030.sCurrentLevelId_5C3030);
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &pPathLine,
        &hitX,
        &hitY,
        (field_D6_scale != 0) ? 1 : 16))
    {
        field_BC_ypos = hitY;
    }

    field_104_on_sound = pTlv->field_14_on_sound;
    field_106_off_sound = pTlv->field_16_off_sound;
    field_FC_tlvInfo = tlvInfo;
    field_108_sound_direction = pTlv->field_18_sound_direction;

    field_F8_state = 0;
    field_DC_bApplyShadows |= 2u;

    return this;
}

void Switch::dtor_4D5B00()
{
    SetVTable(this, 0x547A5C); // vTbl_Switch_547A5C
    Path::TLV_Reset_4DB8E0(field_FC_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    //Switch::dtor_4D5840(); // Omiited interface base nop
}

Switch* Switch::vdtor_4D5AD0(signed int flags)
{
    dtor_4D5B00();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Switch::vScreenChanged_4D5B90()
{
    if (!(field_100_flags & 2) ||
        gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Switch::vUpdate_4D5C00()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_F8_state == 1)
    {
        if (field_20_animation.field_92_current_frame == 3)
        {
            SFX_Play_46FA90(0x3Fu, 0);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMines_1
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_Ender_14
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_5
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_Ender_12
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBarracks_6
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBarracks_Ender_13
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_9
                || gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBrewery_Ender_10)
            {
                SFX_Play_46FBA0(0x50u, 30, 400);
            }

            field_F8_state = 2;

            if (field_100_flags & 1)
            {
                field_20_animation.Set_Animation_Data_409C80(5760, 0);
            }
            else
            {
                field_20_animation.Set_Animation_Data_409C80(5848, 0);
            }

            const int switch_state = SwitchStates_Get_466020(field_F4_trigger_id);
            SwitchStates_Do_Operation_465F00(field_F4_trigger_id, field_102_target_action);
            const int switch_state_after_op = SwitchStates_Get_466020(field_F4_trigger_id);
            if (switch_state != switch_state_after_op)
            {
                int volLeft = 0;
                int volRight = 0;

                if (field_108_sound_direction == 1)
                {
                    volLeft = 1;
                    volRight = 0;
                }
                else
                {
                    volLeft = field_108_sound_direction != 2;
                    volRight = 1;
                }

                if (switch_state_after_op)
                {
                    switch (field_104_on_sound)
                    {
                    case 1:
                        SFX_Play_46FB10(0x14u, 80 * volLeft + 25, 80 * volRight + 25);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    case 2:
                        SFX_Play_46FB10(0xBu, 100 * volLeft + 25, 100 * volRight + 25);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    case 3:
                        SFX_Play_46FB10(0x39u, 75 * volLeft + 15, 75 * volRight + 15);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    case 4:
                        SFX_Play_46FB10(0x31u, 35 * volLeft + 25, 35 * volRight + 25);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    case 5:
                        SFX_Play_46FB10(0x30u, 35 * volLeft + 25, 35 * volRight + 25);
                        return;

                    case 6:
                        SFX_Play_46FB10(0x1Eu, 35 * volLeft + 25, 35 * volRight + 25);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    default:
                        return;
                    }
                }
                else
                {
                    switch (field_106_off_sound)
                    {
                    case 1:
                        SFX_Play_46FB10(0x14u, 80 * volLeft + 25, 80 * volRight + 25);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    case 2:
                        SFX_Play_46FB10(0xBu, 110 * volLeft + 25, 110 * volRight + 25);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    case 3:
                        SFX_Play_46FB10(0x39u, 75 * volLeft + 15, 75 * volRight + 15);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    case 4:
                        SFX_Play_46FB10(0x31u, 80 * volLeft + 25, 80 * volRight + 25);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    case 5:
                        SFX_Play_46FB10(0x30u, 35 * volLeft + 75, 35 * volRight + 75);
                        break;

                    case 6:
                        SFX_Play_46FB10(0x1Eu, 35 * volLeft + 25, 35 * volRight + 25);
                        Event_Broadcast_422BC0(kEventNoise, this);
                        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                        break;

                    default:
                        return;
                    }
                }
            }
        }
    }
    else if (field_F8_state == 2)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_F8_state = 0;
            field_20_animation.Set_Animation_Data_409C80(5696, 0);
        }
    }
}

__int16 Switch::vsub_4D6050(__int16 a2)
{
    if (field_F8_state != 0)
    {
        return 0;
    }

    field_F8_state = 1;

    if (a2)
    {
        field_20_animation.Set_Animation_Data_409C80(5708, nullptr);
        field_100_flags |= 1u;
    }
    else
    {
        field_20_animation.Set_Animation_Data_409C80(5796, nullptr);
        field_100_flags &= ~1u;
    }
    return 1;
}
