#include "stdafx_ao.h"
#include "Function.hpp"
#include "Rock.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Shadow.hpp"

START_NS_AO

void RockSack::VScreenChanged()
{
    VScreenChanged_457890();
}

RockSack* RockSack::Vdtor_4578A0(signed int flags)
{
    dtor_457580();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* RockSack::VDestructor(signed int flags)
{
    return Vdtor_4578A0(flags);
}

BaseGameObject* RockSack::dtor_457580()
{
    SetVTable(this, 0x4BC120);
    gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    return dtor_401000();
}

RockSack* RockSack::ctor_4573F0(Path_RockSack* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BC120);

    field_4_typeId = Types::eRockSack_71;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1002, 1, 0);

    //  Set RockSack idle anim speed
    auto pAnimationHeader = reinterpret_cast<AnimationHeader*>(*ppRes + 6878);
    pAnimationHeader->field_0_fps = 0;

    Animation_Init_417FD0(13756, 71, 60, ppRes, 1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_10C_tlvInfo = tlvInfo;
    field_110 = 0;
    field_CC_bApplyShadows &= ~1u;
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_118_x_vel = FP_FromRaw(pTlv->field_1A_x_vel << 8);
    field_11C_y_vel = FP_FromRaw(-256 * pTlv->field_1C_y_vel);

    if (!pTlv->field_18_side)
    {
        field_118_x_vel = -field_118_x_vel;
    }

    if (pTlv->field_1E_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    field_112_num_rocks = pTlv->field_20_num_rocks;
    field_114_can_play_wobble_sound = 1;
    field_116_force_wobble_sound = 1;

    if (gMap_507BA8.field_0_current_level == LevelIds::eStockYards_5 || gMap_507BA8.field_0_current_level == LevelIds::eStockYardsReturn_6)
    {
        BYTE** ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 1002, 0, 0);
        field_10_anim.LoadPal_403090(ppPal, 0);
    }

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

void RockSack::VScreenChanged_457890()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

END_NS_AO

