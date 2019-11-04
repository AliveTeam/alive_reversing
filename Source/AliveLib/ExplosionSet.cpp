#include "stdafx.h"
#include "ExplosionSet.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "Events.hpp"
#include "Explosion.hpp"
#include "SwitchStates.hpp"
#include "FallingItem.hpp"

ALIVE_VAR(1, 0x5BBF68, ExplosionSet*, pExplosionSet_5BBF68, nullptr);
ALIVE_VAR(1, 0x5C1BB6, __int16, bEnabled_5C1BB6, FALSE);

ExplosionSet* ExplosionSet::ctor_414CA0()
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5444FC);

    field_4_typeId = Types::eExplosionSet_18;

    if (pExplosionSet_5BBF68)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        pExplosionSet_5BBF68 = this;
        field_6_flags.Set(BaseGameObject::eDrawable_Bit4);
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

void ExplosionSet::VRender(int** pOrderingTable)
{
    vRender_4150D0(pOrderingTable);
}

void ExplosionSet::VUpdate()
{
    vUpdate_414E30();
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
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

struct Point2
{
    signed char x, y;
};

const Point2 stru_550F38[12] =
{
    { 0, -2 },
    { -2, 0 },
    { 2, -2 },
    { -2, 2 },
    { 0, -3 },
    { -3, 0 },
    { 3, -3 },
    { -3, 3 },
    { 0,  0 },
    { 0,  0 },
    { 0,  0 },
    { 0,  0 }
};

void ExplosionSet::vRender_4150D0(int** pOt)
{
    if (bEnabled_5C1BB6)
    {
        Prim_ScreenOffset* pScreenOff = &field_20[gPsxDisplay_5C1130.field_C_buffer_index];

        PSX_Pos16 point = {};
        point.x = stru_550F38[field_40].x;
        point.y = stru_550F38[field_40].y;
        if (gPsxDisplay_5C1130.field_C_buffer_index)
        {
            point.y += 256;
        }

        InitType_ScreenOffset_4F5BB0(pScreenOff, &point);
        OrderingTable_Add_4F8AA0(pOt, &pScreenOff->mBase);

        /* constant 1, so always false, dead code.
        if (!dword_55EF94)
        {
            pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
        }*/
    }
}

void ExplosionSet::vUpdate_414E30()
{
    BaseGameObject* pDeathResetEvent = Event_Get_422C00(kEventDeathReset);
    if (pDeathResetEvent)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (bEnabled_5C1BB6)
    {
        field_40 += field_42;

        if (field_40 >= 7 || field_40 <= 0)
        {
            field_42 = -field_42;
        }

        if (field_5C_flags.Get(Flags_5C::eBit3) && field_5C_flags.Get(Flags_5C::eBit1))
        {
            if (field_44_start_delay > 0)
            {
                field_44_start_delay--;
                return;
            }

            short xpos = 0;
            if (field_5C_flags.Get(Flags_5C::eBit2))
            {
                xpos = field_48_tlv_rect.w + field_48_tlv_rect.x - (field_46 * field_5A_scale) - field_58_grid_spacing;
                if (xpos <= field_48_tlv_rect.x)
                {
                    xpos = field_48_tlv_rect.w + field_48_tlv_rect.x - field_58_grid_spacing;
                    field_46 = 0;
                }
            }
            else
            {
                xpos = field_58_grid_spacing + field_48_tlv_rect.x + (field_46 * field_5A_scale);
                if (xpos >= field_48_tlv_rect.x + field_48_tlv_rect.w)
                {
                    xpos = field_48_tlv_rect.x + field_58_grid_spacing;
                    field_46 = 0;
                }
            }

            auto pFallingItem = alive_new<FallingItem>();
            if (pFallingItem)
            {
                pFallingItem->ctor_427560(xpos, field_48_tlv_rect.y, field_50_scale < FP_FromInteger(1), 0, 0, 1, 0);
            }
            
            field_46++;
            field_44_start_delay = field_56_delay;

            if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMines_1 && Math_RandomRange_496AB0(1, 5) >= 4)
            {
                auto pExplosion = alive_new<Explosion>();
                if (pExplosion)
                {
                    const FP explodeX = FP_FromInteger(Math_RandomRange_496AB0(field_48_tlv_rect.y + 20, field_48_tlv_rect.y + 230));
                    const FP explodeY = FP_FromInteger(Math_RandomRange_496AB0(field_48_tlv_rect.x, xpos));
                    pExplosion->ctor_4A1200(explodeY, explodeX, field_50_scale, 0);
                }
            }
        }
    }
    else
    {
        if (field_5C_flags.Get(Flags_5C::eBit3))
        {
            if (field_54_switch_id > 0)
            {
                if (SwitchStates_Get_466020(field_54_switch_id))
                {
                    bEnabled_5C1BB6 = 1;
                }
            }
        }
    }
}
