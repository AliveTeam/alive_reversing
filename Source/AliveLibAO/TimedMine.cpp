#include "stdafx_ao.h"
#include "Function.hpp"
#include "TimedMine.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "LiftPoint.hpp"

START_NS_AO

TintEntry stru_4C3140[3] =
{
  { 5, 60u, 60u, 60u },
  { 6, 60u, 60u, 60u },
  { -1, 127u, 127u, 127u },
};


TimedMine* TimedMine::ctor_4083F0(Path_TimedMine* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(&field_118_anim, 0x4BA2B8);
    SetVTable(this, 0x4BA2C8);
    field_4_typeId = Types::eTimedMine_8;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1005, 1, 0);
    Animation_Init_417FD0(792, 35, 17, ppRes, 1);

    field_6_flags.Set(Options::eInteractive_Bit8);
    field_1B8 &= ~1u;
    field_10C = 0;

    if (pTlv->field_1C_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = 16;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = 35;
    }

    if (field_118_anim.Init_402D20(
        372,
        gObjList_animations_505564,
        this,
        37,
        21,
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1011, 1, 0),
        1,
        0,
        0))
    {
        field_118_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_118_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
        field_118_anim.field_C_layer = field_10_anim.field_C_layer;
        field_118_anim.field_14_scale = field_BC_sprite_scale;
        field_118_anim.field_8_r = 128;
        field_118_anim.field_9_g = 128;
        field_118_anim.field_A_b = 128;
        field_118_anim.field_B_render_mode = 1;
    }
    else
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }

    field_10E = pTlv->field_1E_ticks_before_explode;
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 12);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 24);

    field_114_timer = gnFrameCount_507670;
    field_110_tlvInfo = tlvInfo;

    SetBaseAnimPaletteTint_4187C0(stru_4C3140, gMap_507BA8.field_0_current_level, 1005);
    field_D4 = field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
    field_DC = field_A8_xpos + (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2)); 
    field_E0 = field_AC_ypos;
    field_D8 = field_AC_ypos - ScaleToGridSize_41FA30(field_BC_sprite_scale);

    field_6_flags.Set(Options::eInteractive_Bit8);
    field_F8_pLiftPoint = nullptr;
    return this;
}

BaseGameObject* TimedMine::dtor_408690()
{
    SetVTable(this, 0x4BA2C8);
    if (field_10C != 1 || static_cast<int>(gnFrameCount_507670) < field_114_timer)
    {
        gMap_507BA8.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 1);
    }

    field_118_anim.vCleanUp();

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }

    field_6_flags.Clear(Options::eInteractive_Bit8);
    return dtor_401000();
}

BaseGameObject* TimedMine::VDestructor(signed int flags)
{
    return Vdtor_408E10(flags);
}

TimedMine* TimedMine::Vdtor_408E10(signed int flags)
{
    dtor_408690();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
