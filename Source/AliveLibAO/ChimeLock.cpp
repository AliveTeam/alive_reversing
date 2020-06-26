#include "stdafx_ao.h"
#include "Function.hpp"
#include "ChimeLock.hpp"
#include "ResourceManager.hpp"
#include "Bells.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"

START_NS_AO

const int dword_4C5058[10] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

ChimeLock* ChimeLock::ctor_40AB20(Path_ChimeLock* pTlv, signed int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BA3C8);
    field_4_typeId = Types::eChimeLock_14;

    field_10C_tlvInfo = tlvInfo;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2024, 1, 0);
    Animation_Init_417FD0(136, 16, 8, ppRes, 1);
    field_10_anim.field_C_layer = 37;

    FP scale = {};
    if (pTlv->field_18_scale == 1)
    {
        scale = FP_FromDouble(0.5);
    }
    else
    {
        scale = FP_FromInteger(1);
    }

    field_114_bells = ao_new<Bells>();
    if (field_114_bells)
    {
        field_114_bells->ctor_40A650(
            BellType::eType_0,
            FP_FromInteger(pTlv->field_C_sound_pos.field_0_x),
            FP_FromInteger(pTlv->field_C_sound_pos.field_2_y),
            scale);
    }

    field_118_bells = ao_new<Bells>();
    if (field_118_bells)
    {
        field_118_bells->ctor_40A650(
            BellType::eType_1,
            FP_FromInteger(pTlv->field_C_sound_pos.field_0_x),
            FP_FromInteger(pTlv->field_C_sound_pos.field_2_y),
            scale);
    }

    field_11C_bells = ao_new<Bells>();
    if (field_11C_bells)
    {
        field_11C_bells->ctor_40A650(
            BellType::eType_2,
            FP_FromInteger(pTlv->field_C_sound_pos.field_0_x),
            FP_FromInteger(pTlv->field_C_sound_pos.field_2_y),
            scale);
    }

    field_114_bells->field_C_refCount++;
    field_118_bells->field_C_refCount++;
    field_11C_bells->field_C_refCount++;

    field_124_code1 = pTlv->field_1C_code1;

    int code2 = pTlv->field_1E_code2;
    if (code2)
    {
        field_120 = 0;
        while (code2 / dword_4C5058[field_120])
        {
            field_120++;
        }
        field_124_code1 = code2 + field_124_code1 * dword_4C5058[field_120];
    }

    field_120 = 0;
    for (int i = 0; i < 10; i++)
    {
        if (!(field_124_code1 / dword_4C5058[field_120]))
        {
            break;
        }
        field_120++;
    }

    field_15E = 0;

    field_140_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 40);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 40);
   
    field_B8_vely = FP_FromInteger(0);

    field_13C = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_14C = FP_FromInteger(1);

    field_130 = 0;
    if (SwitchStates_Get(pTlv->field_20_id))
    {
        if (!SwitchStates_Get(pTlv->field_1A_solve_id))
        {
            field_130 = 1;
        }
    }

    field_138 &= ~2u;

    field_10A_flags.Clear(Flags_10A::e10A_Bit2);
    field_10A_flags.Set(Flags_10A::e10A_Bit1);

    field_132_solve_id = pTlv->field_1A_solve_id;

    field_15C = 0;
    field_128 = 0;
    field_164 = 0;
    field_110 = 0;

    return this;
}


BaseGameObject* ChimeLock::dtor_40AE60()
{
    SetVTable(this, 0x4BA3C8);

    if (field_114_bells)
    {
        field_114_bells->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_118_bells)
    {
        field_118_bells->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_11C_bells)
    {
        field_11C_bells->field_6_flags.Set(Options::eDead_Bit3);
    }

    gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);

    return dtor_401000();
}

ChimeLock* ChimeLock::Vdtor_40BD40(signed int flags)
{
    dtor_40AE60();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* ChimeLock::VDestructor(signed int flags)
{
    return Vdtor_40BD40(flags);
}

END_NS_AO

