#include "stdafx.h"
#include "ExplosionSet.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"

ALIVE_VAR(1, 0x5BBF6, ExplosionSet*, pExplosionSet_5BBF68, nullptr);
ALIVE_VAR(1, 0x5C1BB6, __int16, bEnabled_5C1BB6, FALSE);

ExplosionSet* ExplosionSet::ctor_414CA0()
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5444FC);

    field_4_typeId = Types::eExplosionSet_18;

    if (pExplosionSet_5BBF68)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        pExplosionSet_5BBF68 = this;
        field_6_flags.Set(BaseGameObject::eDrawable);
        field_50_scale = FP_FromInteger(1);
        field_40 = 0;
        field_42 = 1;
        field_44_start_delay = 0;
        field_46 = 0;
        bEnabled_5C1BB6 = FALSE;
        field_5C_flags.Clear(Flags_5C::eBit3);
        gObjList_drawables_5C1124->Push_Back(this);
    }
    return this;
}

BaseGameObject* ExplosionSet::VDestructor(signed int flags)
{
    return vdtor_414D80(flags);
}

void ExplosionSet::VScreenChanged()
{
    vScreenChanged_415190();
}

void ExplosionSet::Start_415300()
{
    bEnabled_5C1BB6 = 1;
}

void ExplosionSet::Init_4151D0(Path_ExplosionSet* pTlv)
{
    field_48_tlv_rect.x = pTlv->field_8_top_left.field_0_x;
    field_48_tlv_rect.y = pTlv->field_8_top_left.field_2_y;
    field_48_tlv_rect.w = pTlv->field_C_bottom_right.field_0_x - pTlv->field_8_top_left.field_0_x;
    field_48_tlv_rect.h = pTlv->field_C_bottom_right.field_2_y - pTlv->field_8_top_left.field_2_y;

    if (pTlv->field_20_scale)
    {
        field_50_scale = FP_FromDouble(0.5);
    }

    field_44_start_delay = pTlv->field_16_start_delay;

    field_5C_flags.Set(Flags_5C::eBit1, pTlv->field_14_big_rocks & 1);
    field_5C_flags.Set(Flags_5C::eBit2, pTlv->field_18_direction & 1);
    field_56_delay = pTlv->field_1A_delay;
    field_58_grid_spacing = FP_GetExponent(FP_FromInteger(pTlv->field_1C_grid_spacing) * ScaleToGridSize_4498B0(field_50_scale));
    field_5A_scale = FP_GetExponent(FP_FromInteger(pTlv->field_1E_scale) * ScaleToGridSize_4498B0(field_50_scale));
    field_54_switch_id = pTlv->field_12_id;

    if (!bEnabled_5C1BB6)
    {
        bEnabled_5C1BB6 = pTlv->field_10_start_instantly;
    }

    field_5C_flags.Set(Flags_5C::eBit3);
    field_46 = 0;
}

ExplosionSet* ExplosionSet::vdtor_414D80(signed int flags)
{
    dtor_414DB0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ExplosionSet::dtor_414DB0()
{
    SetVTable(this, 0x5444FC);
    gObjList_drawables_5C1124->Remove_Item(this);
    pExplosionSet_5BBF68 = nullptr;
    BaseGameObject_dtor_4DBEC0();
}

void ExplosionSet::vScreenChanged_415190()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 == gMap_5C3030.field_A_5C303A_levelId &&
        gMap_5C3030.sCurrentPathId_5C3032 == gMap_5C3030.field_C_5C303C_pathId)
    {
        field_5C_flags.Clear(Flags_5C::eBit3);
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}
