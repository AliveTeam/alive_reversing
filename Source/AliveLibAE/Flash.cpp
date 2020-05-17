#include "stdafx.h"
#include "Flash.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

Flash* Flash::ctor_428570(__int16 layer, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, int /*not_used*/, unsigned __int8 abr, int time)
{
    ctor_4AB7A0(layer, abr);
    SetVTable(this, 0x544F0C); // vTbl_Flash_544F0C
    field_4_typeId = Types::eFlash_117;
    field_6E_r = r;
    field_70_g = g;
    field_72_b = b;
    field_78_flash_time = time + sGnFrame_5C1B84;
    return this;
}

BaseGameObject* Flash::VDestructor(signed int flags)
{
    return vdtor_4285F0(flags);
}

void Flash::VUpdate()
{
    vUpdate_428640();
}

Flash* Flash::vdtor_4285F0(signed int flags)
{
    dtor_4AB8F0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Flash::vUpdate_428640()
{
    if (static_cast<int>(sGnFrame_5C1B84) >= field_78_flash_time)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}
