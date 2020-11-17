#include "stdafx_ao.h"
#include "Function.hpp"
#include "Honey.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

namespace AO {

void Honey::VScreenChanged()
{
    VScreenChanged_431EF0();
}

void Honey::VUpdate()
{
    // Empty
}

Honey* Honey::ctor_431E30(FP xpos, FP ypos)
{
    ctor_417C10();
    SetVTable(this, 0x4BB2F8);
    field_4_typeId = Types::eHoney_47;

    field_C4_b = 128;
    field_C2_g = 128;
    field_C0_r = 128;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 337, 1, 0);
    Animation_Init_417FD0(3680, 62, 126, ppRes, 1);

    field_AC_ypos = ypos;
    field_A8_xpos = xpos;

    return this;
}

void Honey::VScreenChanged_431EF0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
}


BaseGameObject* Honey::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

}
