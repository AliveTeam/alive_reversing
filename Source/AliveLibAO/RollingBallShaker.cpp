#include "stdafx_ao.h"
#include "RollingBallShaker.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "Primitives.hpp"

void RollingBallShaker_ForceLink() {}

START_NS_AO

const static PSX_Pos16 sRollingBallShakerScreenOffsets_4BB740[18] =
{
  { 1, 0 },
  { 0, 0 },
  { -1, 1 },
  { 0, 0 },
  { -1, -1 },
  { 0, 0 },
  { 1, -1 },
  { 0, 0 },
  { 0, 1 },
  { 0, 0 },
  { 1, 0 },
  { 0, 0 },
  { 1, 1 },
  { 0, 0 },
  { -1, -1 },
  { 0, 0 },
  { 0, -1 },
  { 0, 0 }
};

RollingBallShaker* RollingBallShaker::ctor_4361A0()
{
    ctor_487E10(1);

    field_6_flags.Set(Options::eDrawable_Bit4);

    SetVTable(this, 0x4BB788);

    field_4_typeId = Types::eRollingBallStopperShaker_58;
    field_30_shake_table_idx = 0;
    field_32_bKillMe = 0; // Set externally
    gObjList_drawables_504618->Push_Back(this);
    return this;
}

BaseGameObject* RollingBallShaker::dtor_436200()
{
    SetVTable(this, 0x4BB788);
    gObjList_drawables_504618->Remove_Item(this);
    return dtor_487DF0();
}

void RollingBallShaker::VUpdate_436260()
{
    field_30_shake_table_idx++;

    if (field_30_shake_table_idx >= ALIVE_COUNTOF(sRollingBallShakerScreenOffsets_4BB740))
    {
        field_30_shake_table_idx = 0;
    }
}

void RollingBallShaker::VRender_436280(int** ppOt)
{
    Prim_ScreenOffset* pPrim = &field_10_prim_screen_offset[gPsxDisplay_504C78.field_A_buffer_index + 1];
    if (field_32_bKillMe != 0)
    {
        // Unshake the screen
        PSX_Pos16 screenOff = {};
        if (gPsxDisplay_504C78.field_A_buffer_index)
        {
            screenOff.y = gPsxDisplay_504C78.field_2_height;
        }
        InitType_ScreenOffset_496000(pPrim, &screenOff);
        OrderingTable_Add_498A80(ppOt, &pPrim->mBase);

        // Kill yourself
        field_6_flags.Set(Options::eDead_Bit3);
    }
    else
    {
        PSX_Pos16 screenOff = sRollingBallShakerScreenOffsets_4BB740[field_30_shake_table_idx];
        if (gPsxDisplay_504C78.field_A_buffer_index)
        {
            screenOff.y += gPsxDisplay_504C78.field_2_height;
        }

        InitType_ScreenOffset_496000(pPrim, &screenOff);
        OrderingTable_Add_498A80(ppOt, &pPrim->mBase);
    }
    pScreenManager_4FF7C8->InvalidateRect_406CC0(0, 0, 640, gPsxDisplay_504C78.field_2_height);
}

BaseGameObject* RollingBallShaker::VDestructor(signed int flags)
{
    return Vdtor_436350(flags);
}

void RollingBallShaker::VUpdate()
{
    return VUpdate_436260();
}

void RollingBallShaker::VRender(int** ppOt)
{
    VRender_436280(ppOt);
}

RollingBallShaker* RollingBallShaker::Vdtor_436350(signed int flags)
{
    dtor_436200();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
