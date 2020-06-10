#include "stdafx_ao.h"
#include "GasCountDown.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

START_NS_AO

GasCountDown* GasCountDown::ctor_40BF60(Path_GasCountDown* /*pTlv*/, int /*tlvInfo*/)
{
    NOT_IMPLEMENTED();
    return this;
}

BaseGameObject* GasCountDown::dtor_40C050()
{
    SetVTable(this, 0x4BA440);
    gObjList_drawables_504618->Remove_Item(this);
    gMap_507BA8.TLV_Reset_446870(field_58_tlvInfo, -1, 0, 0);
    field_20_font.dtor_41C130();
    field_10_font_context.dtor_41C110();
    return dtor_487DF0();
}

BaseGameObject* GasCountDown::Vdtor_40C3F0(int flags)
{
    dtor_40C050();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* GasCountDown::VDestructor(int flags)
{
    return Vdtor_40C3F0(flags);
}

END_NS_AO
