#include "stdafx_ao.h"
#include "Function.hpp"
#include "LightEffect.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

START_NS_AO

LightEffect* LightEffect::ctor_4064C0(Path_LightEffect* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BA1E0);

    field_4_typeId = Types::eNone_0;
    field_E4_tlvInfo = tlvInfo;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1039, 1, 0);
    Animation_Init_417FD0(748, 20, 12, ppRes, 1);

    field_EC_rnd1 = gnFrameCount_507670 + Math_RandomRange_450F20(2, 8);
    field_F0_rnd2 = field_EC_rnd1 + Math_RandomRange_450F20(4, 8);
    field_F4_rnd3 = Math_RandomRange_450F20(96, 128);
    field_F8_rnd4 = Math_RandomRange_450F20(190, 255);

    field_CC_bApplyShadows &= ~1u;

    field_10_anim.field_4_flags.Set(AnimFlags::eBit20_use_xy_offset);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

    field_10_anim.field_C_layer = 17;
    field_10_anim.field_B_render_mode = 0;
    field_10_anim.SetFrame_402AC0(1);

    field_E8 = 0;
    field_EA = 0;

    field_BC_sprite_scale = FP_FromDouble(0.4);
    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    return this;
}

BaseGameObject* LightEffect::dtor_406770()
{
    SetVTable(this, 0x4BA1E0);
    gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

BaseGameObject* LightEffect::VDestructor(signed int flags)
{
    return Vdtor_406800(flags);
}

LightEffect* LightEffect::Vdtor_406800(signed int flags)
{
    dtor_406770();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void LightEffect::VScreenChanged()
{
    VScreenChanged_4067E0();
}

void LightEffect::VScreenChanged_4067E0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void LightEffect::VUpdate()
{
    VUpdate_406610();
}

void LightEffect::VUpdate_406610()
{
    BYTE rgb = 0;
    if (static_cast<int>(gnFrameCount_507670) <= field_F0_rnd2)
    {
        if (static_cast<int>(gnFrameCount_507670) < field_EC_rnd1)
        {
            return;
        }

        const int v6 = field_F0_rnd2 - field_EC_rnd1;
        if (static_cast<int>(gnFrameCount_507670) == v6 / 2)
        {
            field_F4_rnd3 = Math_RandomRange_450F20(96, 128);
        }

        const FP v9 = (FP_FromInteger(128) * FP_FromInteger(gnFrameCount_507670 - field_EC_rnd1) / FP_FromInteger(v6));
        const FP v11 = -Math_Cosine_4510A0(FP_GetExponent(v9) & 0xFF);

        int tmp = field_F4_rnd3 + FP_GetExponent(FP_FromInteger(field_F8_rnd4) * v11);
        if (tmp <= 255)
        {
            rgb = tmp & 0xFF;
        }
        else
        {
            rgb = 255;
        }
    }
    else
    {
        field_EC_rnd1 = gnFrameCount_507670 + Math_RandomRange_450F20(2, 8);
        field_F0_rnd2 = field_EC_rnd1 + Math_RandomRange_450F20(4, 8);
        field_F8_rnd4 = Math_RandomRange_450F20(150, 180);
        rgb = 96;
    }
    field_C0_r = rgb;
    field_C2_g = rgb;
    field_C4_b = rgb;
}

END_NS_AO
