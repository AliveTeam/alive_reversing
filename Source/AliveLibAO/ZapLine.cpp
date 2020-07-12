#include "stdafx_ao.h"
#include "Function.hpp"
#include "ZapLine.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"

START_NS_AO

BaseGameObject* ZapLine::VDestructor(signed int flags)
{
    return Vdtor_479B20(flags);
}

BaseGameObject* ZapLine::dtor_478E90()
{
    SetVTable(this, 0x4BCDE8);
    ResourceManager::FreeResource_455550(field_E8_ppRes);
    ao_delete_free_450770(field_128_buf1);
    ao_delete_free_450770(field_12C_buf2);
    ao_delete_free_450770(field_130_buf3);
    return dtor_417D10();
}

ZapLine* ZapLine::ctor_4789A0(FP /*x1*/, FP /*y1*/, FP /*x2*/, FP /*y2*/, __int16 /*aliveTime*/, signed __int16 /*type*/, __int16 /*layer*/)
{
    NOT_IMPLEMENTED();
    return this;
}

ZapLine* ZapLine::Vdtor_479B20(signed int flags)
{
    dtor_478E90();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
