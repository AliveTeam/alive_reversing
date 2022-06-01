#include "stdafx_ao.h"
#include "EffectBase.hpp"
#include "Function.hpp"
#include "DynamicArray.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"

namespace AO {

EffectBase::EffectBase(Layer layer, TPageAbr abr)
    : BaseGameObject(TRUE)
{
    field_4_typeId = Types::eEffectBase_79;
    gObjList_drawables_504618->Push_Back(this);
    mFlags.Set(BaseGameObject::eDrawable_Bit4);
    field_10_path_id = gMap.mCurrentPath;
    field_12_level_id = gMap.mCurrentLevel;
    for (s32 i = 0; i < 2; i++)
    {
        Init_SetTPage_495FB0(&field_3C_tPage[i], 0, 0, static_cast<s16>(PSX_getTPage_4965D0(TPageMode::e16Bit_2, abr, 0, 0)));
    }
    field_5C_layer = layer;
    field_64_bSemiTrans = 1;
}

EffectBase::~EffectBase()
{
    gObjList_drawables_504618->Remove_Item(this);
}

void EffectBase::VRender(PrimHeader** ppOt)
{
    Prim_Tile* pTile = &field_14_tile[gPsxDisplay_504C78.field_A_buffer_index];
    Init_Tile(pTile);

    SetRGB0(pTile,
            static_cast<u8>(field_5E_r),
            static_cast<u8>(field_60_g),
            static_cast<u8>(field_62_b));
    SetXY0(pTile, 0, 0);

    pTile->field_14_w = 640;
    pTile->field_16_h = gPsxDisplay_504C78.field_2_height;

    Poly_Set_SemiTrans_498A40(&pTile->mBase.header, field_64_bSemiTrans);
    OrderingTable_Add_498A80(OtLayer(ppOt, field_5C_layer), &pTile->mBase.header);
    OrderingTable_Add_498A80(OtLayer(ppOt, field_5C_layer), &field_3C_tPage[gPsxDisplay_504C78.field_A_buffer_index].mBase);
    pScreenManager_4FF7C8->InvalidateRectCurrentIdx(0, 0, 640, gPsxDisplay_504C78.field_2_height);
}

} // namespace AO
