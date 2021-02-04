#include "stdafx_ao.h"
#include "Function.hpp"
#include "Flash.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

namespace AO {

Flash* Flash::ctor_41A810(Layer layer, unsigned __int8 r, unsigned __int8 g, unsigned __int8 b, int /*not_used*/, unsigned __int8 abr, int time)
{
    ctor_461550(layer, abr);
    SetVTable(this, 0x4BAC40);
    field_4_typeId = Types::eFlash_81;
    field_5E_r = r;
    field_60_g = g;
    field_62_b = b;
    field_68_flash_time = time + gnFrameCount_507670;
    return this;
}

BaseGameObject* Flash::VDestructor(signed int flags)
{
    return Vdtor_41A890(flags);
}

BaseGameObject* Flash::Vdtor_41A890(signed int flags)
{
    dtor_461630();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Flash::VUpdate_41A870()
{
    if (static_cast<int>(gnFrameCount_507670) >= field_68_flash_time)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Flash::VUpdate()
{
    VUpdate_41A870();
}

}
