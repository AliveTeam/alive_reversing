#include "stdafx_ao.h"
#include "Function.hpp"
#include "PullRingRope.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Sfx.hpp"
#include "BaseAliveGameObject.hpp"
#include "Rope.hpp"
#include "Events.hpp"

namespace AO {

PullRingRope* PullRingRope::ctor_4546B0(Path_PullRingRope* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BC058);
    field_4_typeId = Types::ePullRingRope_68;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1014, 1, 0);

    int lvl_x_off = 0;
    switch (gMap_507BA8.field_0_current_level)
    {
    case LevelIds::eRuptureFarms_1:
    case LevelIds::eBoardRoom_12:
    case LevelIds::eRuptureFarmsReturn_13:
        Animation_Init_417FD0(4832, 33, 35, ppRes, 1);
        lvl_x_off = -2;
        break;
    case LevelIds::eDesert_8:
        Animation_Init_417FD0(2912, 32, 22, ppRes, 1);
        lvl_x_off = 2;
        break;
    default:
        Animation_Init_417FD0(2912, 32, 22, ppRes, 1);
        lvl_x_off = 0;
        break;
    }

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_EE_id = pTlv->field_18_id;
    field_F0_action = pTlv->field_1A_action;
    field_E8 = tlvInfo;
    field_EC_state = 0;
    field_E4_stay_in_state_ticks = 0;

    field_AC_ypos += FP_FromInteger(pTlv->field_1C_rope_length + pTlv->field_C_sound_pos.field_2_y + 24);
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 12);

    if (pTlv->field_1E_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 8;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = 27;
        field_C6_scale = 1;
    }

    field_100_sound_direction = pTlv->field_24_sound_direction;

    field_FC_on_sound = pTlv->field_20_on_sound;
    field_FE_off_sound = pTlv->field_22_off_sound;

    field_F4_pPuller = nullptr;

    field_F8_pRope = ao_new<Rope>();
    if (field_F8_pRope)
    {
        field_F8_pRope->ctor_458520(
            FP_GetExponent(field_A8_xpos + FP_FromInteger((lvl_x_off + 1))),
            FP_GetExponent(field_AC_ypos) - pTlv->field_1C_rope_length,
            FP_GetExponent(field_AC_ypos + (FP_FromInteger(field_C8_yOffset))),
            field_BC_sprite_scale);

        field_F8_pRope->field_C_refCount++;
    }
    return this;
}

BOOL PullRingRope::sub_454D60()
{
    return field_EC_state != 1;
}

void PullRingRope::VScreenChanged_454D70()
{
    if (!field_F4_pPuller)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void PullRingRope::VScreenChanged()
{
    VScreenChanged_454D70();
}

PullRingRope* PullRingRope::Vdtor_454D80(signed int flags)
{
    dtor_454910();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* PullRingRope::VDestructor(signed int flags)
{
    return Vdtor_454D80(flags);
}

BaseGameObject* PullRingRope::dtor_454910()
{
    SetVTable(this, 0x4BC058);
    gMap_507BA8.TLV_Reset_446870(field_E8, -1, 0, 0);

    if (field_F4_pPuller)
    {
        field_F4_pPuller->field_C_refCount--;
    }

    if (field_F8_pRope)
    {
        field_F8_pRope->field_6_flags.Set(Options::eDead_Bit3);
        field_F8_pRope->field_C_refCount--;
    }

    return dtor_417D10();
}

__int16 PullRingRope::Pull_454CB0(BaseAliveGameObject* pFrom)
{
    if (!pFrom)
    {
        return 0;
    }

    if (field_EC_state != 0)
    {
        return 0;
    }

    field_F4_pPuller = pFrom;
    field_F4_pPuller->field_C_refCount++;

    field_EC_state = 1;
    field_B8_vely = FP_FromInteger(2);
    field_E4_stay_in_state_ticks = 6;

    SwitchStates_Do_Operation_436A10(field_EE_id, field_F0_action);

    if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 ||
        gMap_507BA8.field_0_current_level == LevelIds::eRemoved_11 ||
        gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12 ||
        gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
    {
        field_10_anim.Set_Animation_Data_402A40(4872, 0);
    }
    else
    {
        field_10_anim.Set_Animation_Data_402A40(2952, 0);
    }
    SFX_Play_43AD70(SoundEffect::RingRopePull_65, 0, 0);
    return 1;
}

void PullRingRope::VUpdate()
{
    VUpdate_4549A0();
}

void PullRingRope::VUpdate_4549A0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_F4_pPuller)
    {
        if (field_F4_pPuller->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_F4_pPuller->field_C_refCount--;
            field_F4_pPuller = nullptr;
        }
    }

    switch (field_EC_state)
    {
    case 1:
        if (field_10_anim.field_92_current_frame == 2)
        {
            SFX_Play_43AD70(SoundEffect::RingRopePull_65, 0);
        }

        field_AC_ypos += field_B8_vely;
        field_F4_pPuller->field_AC_ypos += field_B8_vely;
        field_E4_stay_in_state_ticks--;

        if (field_E4_stay_in_state_ticks == 0)
        {
            field_B8_vely = FP_FromInteger(0);
            field_EC_state = 2;

            if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 ||
                gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12 ||
                gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
            {
                SFX_Play_43AD70(SoundEffect::IndustrialTrigger_97, 0);
            }

            const auto oldSwitchValue = SwitchStates_Get(field_EE_id);
            // TODO: OG bug - operation isn't applied to the switch ??
            const auto switchValAfterOperation = SwitchStates_Get(field_EE_id);

            // Due to seemingly OG bug this can never execute
            if (oldSwitchValue != switchValAfterOperation)
            {
                int volLeft = 0;
                int volRight = 0;
                if (field_100_sound_direction == 1)
                {
                    volLeft = 1;
                    volRight = 0;
                }
                else
                {
                    volLeft = field_100_sound_direction != 2;
                    volRight = 1;
                }

                if (SwitchStates_Get(field_EE_id))
                {
                    switch (field_FC_on_sound)
                    {
                    case 1:
                        SFX_Play_43ADE0(SoundEffect::WellExit_24, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                        break;

                    case 2:
                        SFX_Play_43ADE0(SoundEffect::RingBellHammer_9, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                        break;

                    case 3:
                        SFX_Play_43ADE0(SoundEffect::DoorEffect_66, 75 * volLeft + 15, 75 * volRight + 15, nullptr);
                        break;
                    }
                }
                else
                {
                    switch (field_FE_off_sound)
                    {
                    case 1:
                        SFX_Play_43ADE0(SoundEffect::WellExit_24, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                        break;

                    case 2:
                        SFX_Play_43ADE0(SoundEffect::RingBellHammer_9, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                        break;

                    case 3:
                        SFX_Play_43ADE0(SoundEffect::DoorEffect_66, 75 * volLeft + 15, 75 * volRight + 15, nullptr);
                        break;
                    }
                }
            }
        }
        break;

    case 2:
        field_B8_vely = FP_FromInteger(4);
        field_EC_state = 3;
        field_F4_pPuller->field_C_refCount--;
        field_F4_pPuller = nullptr;

        field_E4_stay_in_state_ticks = 3;

        if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 ||
            gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12 ||
            gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
        {
            field_10_anim.Set_Animation_Data_402A40(4904, nullptr);
        }
        else
        {
            field_10_anim.Set_Animation_Data_402A40(2984, nullptr);
        }
        break;

    case 3:
        field_AC_ypos -= field_B8_vely;
        field_E4_stay_in_state_ticks--;
        if (field_E4_stay_in_state_ticks == 0)
        {
            field_B8_vely = FP_FromInteger(0);
            field_EC_state = 0;

            if (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 ||
                gMap_507BA8.field_0_current_level == LevelIds::eBoardRoom_12 ||
                gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13)
            {
                field_10_anim.Set_Animation_Data_402A40(4832, nullptr);
            }
            else
            {
                field_10_anim.Set_Animation_Data_402A40(2912, nullptr);
            }
        }
        break;

    default:
        break;
    }

    field_F8_pRope->field_AC_ypos = FP_NoFractional(FP_FromInteger(field_C8_yOffset - 16) + field_AC_ypos);
}

}
