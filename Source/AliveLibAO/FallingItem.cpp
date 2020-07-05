#include "stdafx_ao.h"
#include "Function.hpp"
#include "FallingItem.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Game.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FFA54, FallingItem*, pPrimaryFallingItem_4FFA54, nullptr);

const FallingItem_Data sFallingItemData_4BAB20[16] =
{
  { 11504, 11544, 76, 38 },
  { 12136, 12148, 66, 42 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 },
  { 12136, 12148, 66, 42 },
  { 11504, 11544, 76, 38 },
  { 11504, 11544, 76, 38 }
};

FallingItem* FallingItem::ctor_419F30(Path_FallingItem* pTlv, int tlvInfo)
{
    ctor_401090();
    field_6_flags.Set(Options::eCanExplode_Bit7);
    SetVTable(this, 0x4BABE0);

    field_4_typeId = Types::eRockSpawner_32;

    field_10C_tlvInfo = tlvInfo;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2007, 1, 0);
    const int lvlIdx = static_cast<int>(gMap_507BA8.field_0_current_level);
    Animation_Init_417FD0(
        sFallingItemData_4BAB20[lvlIdx].field_0,
        sFallingItemData_4BAB20[lvlIdx].field_8,
        sFallingItemData_4BAB20[lvlIdx].field_A,
        ppRes,
        1);

    field_10_anim.field_C_layer = 31;
    if (gMap_507BA8.field_0_current_level == LevelIds::eLines_2)
    {
        field_C0_r = 77;
        field_C2_g = 120;
        field_C4_b = 190;
    }

    field_112_id = pTlv->field_18_id;
    if (pTlv->field_1A_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }


    field_118_delay_time = pTlv->field_1C_delay_time;
    field_114_num_items = pTlv->field_1E_number_of_items;
    field_116_num_items_remaining = pTlv->field_1E_number_of_items;

    field_120_reset_id = pTlv->field_20_reset_id;
    field_122_do_sound_in_state_falling = 1;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    field_128_xpos = FP_FromInteger((pTlv->field_14_bottom_right.field_0_x + pTlv->field_10_top_left.field_0_x) / 2);
    field_12C_ypos = FP_FromInteger(pTlv->field_14_bottom_right.field_2_y);

    field_124_yPosStart = field_AC_ypos;
    field_110_state = 0;
    field_130_sound_channels = 0;

    // Not sure why this rupture farms primary item hack is required
    if (!pPrimaryFallingItem_4FFA54 && (gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarms_1 || gMap_507BA8.field_0_current_level == LevelIds::eRuptureFarmsReturn_13))
    {
        pPrimaryFallingItem_4FFA54 = this;
        field_134_created_gnFrame = gnFrameCount_507670;
    }

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseGameObject* FallingItem::dtor_41A660()
{
    SetVTable(this, 0x4BABE0);
    if (pPrimaryFallingItem_4FFA54 == this)
    {
        pPrimaryFallingItem_4FFA54 = nullptr;
    }
    gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    return dtor_401000();
}

FallingItem* FallingItem::Vdtor_41A7F0(signed int flags)
{
    dtor_41A660();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void FallingItem::VScreenChanged_41A7C0()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path
        || field_110_state != 3)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void FallingItem::VScreenChanged()
{
    VScreenChanged_41A7C0();
}

BaseGameObject* FallingItem::VDestructor(signed int flags)
{
    return Vdtor_41A7F0(flags);
}

END_NS_AO
