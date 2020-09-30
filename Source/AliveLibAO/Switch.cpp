#include "stdafx_ao.h"
#include "Function.hpp"
#include "Switch.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"

START_NS_AO

struct Switch_Data
{
    int field_0_frameTableOffset;
    int field_4_maxW;
    int field_8_maxH;
    int field_C_frameTable;
    int field_10_frameTable2;
    int field_14_frameTable;
    int field_18_frameTable3;
};
ALIVE_ASSERT_SIZEOF(Switch_Data, 0x1C);

const Switch_Data gSwitchData_4BCF40[16] =
{
    { 16272, 66, 41, 16284, 16336, 16372, 16424 },
    { 16168, 67, 40, 16180, 16232, 16268, 16320 },
    { 5440,  66, 41, 5576,  5452,  5488,  5540  },
    { 16272, 66, 41, 16284, 16336, 16372, 16424 },
    { 16272, 66, 41, 16284, 16336, 16372, 16424 },
    { 16272, 66, 41, 16284, 16336, 16372, 16424 },
    { 16168, 67, 40, 16180, 16232, 16268, 16320 },
    { 5440,  66, 41, 5576,  5452,  5488,  5540  },
    { 16272, 66, 41, 16284, 16336, 16372, 16424 },
    { 16272, 66, 41, 16284, 16336, 16372, 16424 },
    { 16272, 66, 41, 16284, 16336, 16372, 16424 },
    { 16272, 66, 41, 16284, 16336, 16372, 16424 },
    { 16168, 67, 40, 16180, 16232, 16268, 16320 },
    { 16168, 67, 40, 16180, 16232, 16268, 16320 },
    { 16272, 66, 41, 16284, 16336, 16372, 16424 },
    { 16272, 66, 41, 16284, 16336, 16372, 16424 }
};

int Switch::VPull_481640(__int16 a2)
{
    return vPull_481640(a2);
}

void Switch::VUpdate_4812D0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_E8_state == 1)
    {
        if (field_10_anim.field_92_current_frame == 3)
        {
            SFX_Play_43AD70(SoundEffect::LeverPull_75, 0, 0);
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            Event_Broadcast_417220(kEventNoise_0, this);
            Event_Broadcast_417220(kEvent_10, this);
            const int lvl_idx = static_cast<int>(gMap_507BA8.field_0_current_level);
            if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1
                || gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12
                || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
            {
                SFX_Play_43AD70(SoundEffect::IndustrialTrigger_97, 60, 0);
            }
            field_E8_state = 2;

            int frameTable = 0;
            if (field_F0 == 0)
            {
                frameTable = gSwitchData_4BCF40[lvl_idx].field_18_frameTable3;
            }
            else
            {
                frameTable = gSwitchData_4BCF40[lvl_idx].field_10_frameTable2;
            }

            field_10_anim.Set_Animation_Data_402A40(frameTable, nullptr);

            const auto oldSwitchState = SwitchStates_Get(field_E4_switchId);
            SwitchStates_Do_Operation_436A10(field_E4_switchId, field_F2_operation);
            const auto newSwitchState = SwitchStates_Get(field_E4_switchId);

            if (oldSwitchState != newSwitchState)
            {
                int leftVol = 0;
                int rightVol = 0;

                if (field_F8_sound_direction == 1)
                {
                    leftVol = 1;
                    rightVol = 0;
                }
                else
                {
                    leftVol = field_F8_sound_direction != 2;
                    rightVol = 1;
                }

                if (SwitchStates_Get(field_E4_switchId))
                {
                    switch (field_F4_on_sound)
                    {
                    case 1:
                        SFX_Play_43ADE0(SoundEffect::WellExit_24, 50 * leftVol + 10, 50 * rightVol + 10, 0);
                        break;
                    case 2:
                        SFX_Play_43ADE0(SoundEffect::Unknown_12, 100 * leftVol + 25, 100 * rightVol + 25, 0);
                        return;
                    case 3:
                        SFX_Play_43ADE0(SoundEffect::Unknown_66, 75 * leftVol + 15, 75 * rightVol + 15, 0);
                        break;
                    case 4:
                        SFX_Play_43ADE0(SoundEffect::Unknown_57, 35 * leftVol + 25, 35 * rightVol + 25, 0);
                        break;
                    case 5:
                        SFX_Play_43ADE0(SoundEffect::SecurityOrb_56, 35 * leftVol + 25, 35 * rightVol + 25, 0);
                        break;
                    default:
                        return;
                    }
                }
                else
                {
                    switch (field_F6_off_sound)
                    {
                    case 1:
                        SFX_Play_43ADE0(SoundEffect::WellExit_24, 50 * leftVol + 10, 50 * rightVol + 10, 0);
                        break;
                    case 2:
                        SFX_Play_43ADE0(SoundEffect::Unknown_12, 110 * leftVol + 25, 110 * rightVol + 25, 0);
                        break;
                    case 3:
                        SFX_Play_43ADE0(SoundEffect::Unknown_66, 75 * leftVol + 15, 75 * rightVol + 15, 0);
                        break;
                    case 4:
                        SFX_Play_43ADE0(SoundEffect::Unknown_57, 80 * leftVol + 25, 80 * rightVol + 25, 0);
                        break;
                    case 5:
                        SFX_Play_43ADE0(SoundEffect::SecurityOrb_56, 35 * leftVol + 75, 35 * rightVol + 75, 0);
                        break;
                    default:
                        return;
                    }
                }
            }
        }
    }
    else if (field_E8_state == 2)
    {
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_E8_state = 0;
            field_10_anim.Set_Animation_Data_402A40(
                gSwitchData_4BCF40[static_cast<int>(gMap_507BA8.field_0_current_level)].field_0_frameTableOffset,
                nullptr);
        }
    }
}

void Switch::VUpdate()
{
    VUpdate_4812D0();
}

void Switch::VScreenChanged_4816F0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void Switch::VScreenChanged()
{
    VScreenChanged_4816F0();
}

Switch* Switch::Vdtor_481700(signed int flags)
{
    dtor_481260();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* Switch::VDestructor(signed int flags)
{
    return Vdtor_481700(flags);
}

BaseGameObject* Switch::dtor_481260()
{
    SetVTable(this, 0x4BD100);
    gMap_507BA8.TLV_Reset_446870(field_EC_tlvInfo, -1, 0, 0);
    return dtor_417D10(); // Note: intermediate base skipped
}

Switch* Switch::ctor_481110(Path_Switch* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BD100);

    field_4_typeId = Types::eLever_97;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1009, 1, 0);

    const int idx = static_cast<int>(gMap_507BA8.field_0_current_level);
    Animation_Init_417FD0(
        gSwitchData_4BCF40[idx].field_0_frameTableOffset,
        gSwitchData_4BCF40[idx].field_4_maxW,
        gSwitchData_4BCF40[idx].field_8_maxH,
        ppRes,
        1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_A8_xpos = FP_FromInteger((pTlv->field_14_bottom_right.field_0_x
        + pTlv->field_10_top_left.field_0_x)
        / 2);

    field_E4_switchId = pTlv->field_18_trigger_object;
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
    field_F2_operation = pTlv->field_1A_trigger_object_action;

    if (pTlv->field_1C_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 6;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = 25;
        field_C6_scale = 1;
    }

    field_F4_on_sound = pTlv->field_1E_on_sound;
    field_F6_off_sound = pTlv->field_20_off_sound;
    field_EC_tlvInfo = tlvInfo;
    field_F8_sound_direction = pTlv->field_22_sound_direction;

    field_E8_state = 0;

    return this;
}

int Switch::vPull_481640(__int16 a2)
{
    if (field_E8_state == 0)
    {
        const int lvl_idx = static_cast<int>(gMap_507BA8.field_0_current_level);
        field_E8_state = 1;
        if (a2)
        {
            field_10_anim.Set_Animation_Data_402A40(
                gSwitchData_4BCF40[lvl_idx].field_C_frameTable,
                nullptr);
            field_F0 = 1;
        }
        else
        {
            field_10_anim.Set_Animation_Data_402A40(
                gSwitchData_4BCF40[lvl_idx].field_14_frameTable,
                nullptr);
            field_F0 = 0;
        }
    }
    return SwitchStates_Get(field_E4_switchId);
}

END_NS_AO

