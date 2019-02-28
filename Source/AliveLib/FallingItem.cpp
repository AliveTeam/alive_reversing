#include "stdafx.h"
#include "FallingItem.hpp"
#include "Shadow.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

const FallingItem_Data sFallingItemData_544DC0[18] =
{
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 8076, 8100, 86, 47 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 4232, 4256, 64, 28 },
    { 8076, 8100, 86, 47 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

ALIVE_VAR(1, 0x5BC208, FallingItem*, pPrimaryFallingItem_5BC208, nullptr);

EXPORT FallingItem* FallingItem::ctor_4272C0(Path_FallingItem* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x544E98);

    field_4_typeId = Types::eRockSpawner_48;

    field_6_flags.Set(BaseGameObject::eCanExplode);
    field_118_tlvInfo = tlvInfo;

    Add_Resource_4DC130(ResourceManager::Resource_Animation, 301);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 2007);
    const int lvlIdx = static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030);
    Animation_Init_424E10(
        sFallingItemData_544DC0[lvlIdx].field_0,
        sFallingItemData_544DC0[lvlIdx].field_8,
        sFallingItemData_544DC0[lvlIdx].field_A,
        ppRes,
        1,
        1);
    
    field_11E_id = pTlv->field_10_id;

    if (pTlv->field_12_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 12;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 31;
    }

    field_124_delay_time = pTlv->field_14_delay_time;
    field_120_num_items = pTlv->field_16_number_of_items;
    field_122_num_items_remaining = pTlv->field_16_number_of_items;
    field_134_bUnknown = 0;
    field_12C_reset_id = pTlv->field_18_reset_id;
    field_12E_do_sound_in_state_3 = 1;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (field_BC_ypos > pScreenManager_5BB5F4->field_20_pCamPos->field_4_y)
    {
        field_BC_ypos = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
    }

    field_138_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_13C_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_130_yPosStart = field_BC_ypos;
    field_11C_state = 0;
    field_140_sound_channels = 0;

    if (!pPrimaryFallingItem_5BC208)
    {
        pPrimaryFallingItem_5BC208 = this;
        field_144_created_gnFrame = sGnFrame_5C1B84;
    }

    field_E0_176_ptr = alive_new<Shadow>();
    if (field_E0_176_ptr)
    {
        field_E0_176_ptr->ctor_4AC990();
    }

    return this;
}

FallingItem* FallingItem::ctor_427560(__int16 xpos, __int16 ypos, __int16 scale, __int16 id, __int16 delayTime, __int16 numItems, __int16 resetId)
{
    ctor_408240(0);
   
    SetVTable(this, 0x544E98);
    field_4_typeId = Types::eRockSpawner_48;

    field_6_flags.Set(BaseGameObject::eCanExplode);
    field_118_tlvInfo = -1;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 2007);
    const int lvlIdx = static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030);
    Animation_Init_424E10(
        sFallingItemData_544DC0[lvlIdx].field_0,
        sFallingItemData_544DC0[lvlIdx].field_8,
        sFallingItemData_544DC0[lvlIdx].field_A,
        ppRes,
        1,
        1);

    field_20_animation.field_C_render_layer = 31;

    if (id)
    {
        field_11E_id = id;
    }
    else
    {
        field_11E_id = 1;
    }

    if (scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_124_delay_time = delayTime;
    
    field_120_num_items = numItems;
    field_122_num_items_remaining = numItems;

    const FP xFixed = FP_FromInteger(xpos);
    const FP yFixed = FP_FromInteger(ypos);

    field_12C_reset_id = resetId;
    field_134_bUnknown = 0;
    field_12E_do_sound_in_state_3 = 1;
    field_B8_xpos = xFixed;
    field_BC_ypos = yFixed;
    field_138_xpos = xFixed;
    field_13C_ypos = yFixed;
    field_130_yPosStart = yFixed;
    field_11C_state = 0;
    field_140_sound_channels = 0;

    if (!pPrimaryFallingItem_5BC208)
    {
        pPrimaryFallingItem_5BC208 = this;
        field_144_created_gnFrame = sGnFrame_5C1B84;
    }

    field_E0_176_ptr = alive_new<Shadow>();
    if (field_E0_176_ptr)
    {
        field_E0_176_ptr->ctor_4AC990();
    }

    return this;
}

BaseGameObject* FallingItem::VDestructor(signed int flags)
{
    return vdtor_427530(flags);
}

void FallingItem::VScreenChanged()
{
    vScreenChanged_428180();
}

void FallingItem::dtor_427EB0()
{
    SetVTable(this, 0x544E98);
    if (pPrimaryFallingItem_5BC208 == this)
    {
        pPrimaryFallingItem_5BC208 = nullptr;
    }
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

FallingItem* FallingItem::vdtor_427530(signed int flags)
{
    dtor_427EB0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void FallingItem::vScreenChanged_428180()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        field_11C_state != 3)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}
