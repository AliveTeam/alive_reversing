#include "stdafx_ao.h"
#include "Function.hpp"
#include "Bells.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"

START_NS_AO

Bells* Bells::ctor_40A650(BellType bellType, FP xpos, FP ypos, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BA390);

    field_6_flags.Clear(Options::eCanExplode_Bit7);

    field_4_typeId = Types::eBells_13;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2005, 1, 0);
    
    switch (bellType)
    {
    case BellType::eType_0:
        field_EA_sound = 0;
        Animation_Init_417FD0(19240, 49, 46, ppRes, 1);
        break;

    case BellType::eType_1:
        field_EA_sound = 1;
        Animation_Init_417FD0(19368, 49, 46, ppRes, 1);
        break;
    case BellType::eType_2:
        field_EA_sound = 2;
        Animation_Init_417FD0(19252, 49, 46, ppRes, 1);
        break;
    }

    field_CC_bApplyShadows &= ~1u;
    field_BC_sprite_scale = scale;

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    field_10_anim.field_C_layer = 36;

    field_E8 = 0;

    field_EC_timer = 0;
    field_F4_timer = 0;
    field_F0_timer = 0;

    return this;
}

BaseGameObject* Bells::dtor_40A760()
{
    SetVTable(this, 0x4BA390);
    return dtor_417D10();
}

Bells* Bells::Vdtor_40AB00(signed int flags)
{
    dtor_40A760();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* Bells::VDestructor(signed int flags)
{
    return Vdtor_40AB00(flags);
}

END_NS_AO
