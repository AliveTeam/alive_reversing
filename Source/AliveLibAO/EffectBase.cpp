#include "stdafx_ao.h"
#include "EffectBase.hpp"
#include "Function.hpp"
#include "DynamicArray.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

START_NS_AO

EffectBase* EffectBase::ctor_461550(__int16 layer, char abr)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BC900);
    field_4_typeId = Types::eEffectBase_79;
    gObjList_drawables_504618->Push_Back(this);
    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);
    field_10_path_id = gMap_507BA8.field_2_current_path;
    field_12_level_id = gMap_507BA8.field_0_current_level;
    Init_SetTPage_495FB0(&field_3C_tPage[0], 0, 0, static_cast<short>(PSX_getTPage_4965D0(2, abr, 0, 0)));
    Init_SetTPage_495FB0(&field_3C_tPage[1], 0, 0, static_cast<short>(PSX_getTPage_4965D0(2, abr, 0, 0)));
    field_5C_layer = layer;
    field_64_bSemiTrans = 1;
    return this;
}

BaseGameObject* EffectBase::dtor_461630()
{
    SetVTable(this, 0x4BC900);
    gObjList_drawables_504618->Remove_Item(this);
    return dtor_487DF0();
}

EffectBase* EffectBase::Vdtor_461750(signed int flags)
{
    dtor_461630();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* EffectBase::VDestructor(signed int flags)
{
    return Vdtor_461750(flags);
}

END_NS_AO
