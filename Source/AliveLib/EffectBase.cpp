#include "stdafx.h"
#include "EffectBase.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "stdlib.hpp"

BaseGameObject* EffectBase::VDestructor(signed int flags)
{
    return vdtor_4AB8C0(flags);
}

void EffectBase::VRender(int** pOrderingTable)
{
    vRender_4AB970(pOrderingTable);
}

EffectBase* EffectBase::ctor_4AB7A0(__int16 layer, char abr)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x54700C); // vTbl_EffectBase_54700C
    field_4_typeId = Types::eEffectBase_115;
    gObjList_drawables_5C1124->Push_Back(this);
    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);
    field_20_path_id = gMap_5C3030.sCurrentPathId_5C3032;
    field_22_level_id = gMap_5C3030.sCurrentLevelId_5C3030;
    Init_SetTPage_4F5B60(&field_4C_tPage[0], 0, 0, PSX_getTPage_4F60E0(2, abr, 0, 0));
    Init_SetTPage_4F5B60(&field_4C_tPage[1], 0, 0, PSX_getTPage_4F60E0(2, abr, 0, 0));
    field_6C_layer = layer;
    field_74_bSemiTrans = 1;
    return this;
}

void EffectBase::dtor_4AB8F0()
{
    SetVTable(this, 0x54700C); // vTbl_EffectBase_54700C
    gObjList_drawables_5C1124->Remove_Item(this);
    BaseGameObject_dtor_4DBEC0();
}

EffectBase* EffectBase::vdtor_4AB8C0(signed int flags)
{
    dtor_4AB8F0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void EffectBase::vRender_4AB970(int** pOt)
{
    Prim_Tile* pTile = &field_24_tile[gPsxDisplay_5C1130.field_C_buffer_index];
    Init_Tile(pTile);
    SetRGB0(pTile,
        static_cast<BYTE>(field_6E_b),
        static_cast<BYTE>(field_70_g),
        static_cast<BYTE>(field_72_r));
    SetXY0(pTile, 0, 0);
    pTile->field_14_w = 640;
    pTile->field_16_h = gPsxDisplay_5C1130.field_2_height;

    if (field_6E_b || field_70_g || field_72_r || !field_74_bSemiTrans)
    {
        Poly_Set_SemiTrans_4F8A60(&pTile->mBase.header, field_74_bSemiTrans);
        OrderingTable_Add_4F8AA0(&pOt[field_6C_layer], &pTile->mBase.header);
        OrderingTable_Add_4F8AA0(&pOt[field_6C_layer], &field_4C_tPage[gPsxDisplay_5C1130.field_C_buffer_index].mBase);
        if (!dword_5CA4D4 || !(PSX_getTPage_4F60E0(0, 1, 0, 0) & field_4C_tPage[0].field_C_tpage))
        {
            pScreenManager_5BB5F4->InvalidateRect_40EC10(
                0,
                0,
                640,
                gPsxDisplay_5C1130.field_2_height);
        }
    }
}
