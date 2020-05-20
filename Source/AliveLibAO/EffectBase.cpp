#include "stdafx_ao.h"
#include "EffectBase.hpp"
#include "Function.hpp"
#include "DynamicArray.hpp"
#include "Map.hpp"

START_NS_AO

ALIVE_VAR(1, 0x504618, DynamicArray*, gObjList_drawables_504618, nullptr);



EffectBase* EffectBase::ctor_461550(__int16 layer, char abr)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BC900);
    field_4_typeId = 79;
    gObjList_drawables_504618->Push_Back_404450(this);
    field_6_flags |= 8u;
    field_10_path_id = gMap_507BA8.field_2_current_path;
    field_12_level_id = gMap_507BA8.field_0_current_level;
    Init_SetTPage_495FB0(&field_3C_tPage[0], 0, 0, static_cast<short>(PSX_getTPage_4965D0(2, abr, 0, 0)));
    Init_SetTPage_495FB0(&field_3C_tPage[1], 0, 0, static_cast<short>(PSX_getTPage_4965D0(2, abr, 0, 0)));
    field_5C_layer = layer;
    field_64_bSemiTrans = 1;
    return this;
}

END_NS_AO
