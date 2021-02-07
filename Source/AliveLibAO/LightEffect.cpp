#include "stdafx_ao.h"
#include "Function.hpp"
#include "LightEffect.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "CameraSwapper.hpp"
#include "ScreenManager.hpp"

namespace AO {

LightEffect* LightEffect::ctor_4064C0(Path_LightEffect* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BA1E0);

    field_4_typeId = Types::eNone_0;
    field_E4_tlvInfo = tlvInfo;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kBGStarResID, 1, 0);
    Animation_Init_417FD0(748, 20, 12, ppRes, 1);

    field_EC_rnd1 = gnFrameCount_507670 + Math_RandomRange_450F20(2, 8);
    field_F0_rnd2 = field_EC_rnd1 + Math_RandomRange_450F20(4, 8);
    field_F4_rnd3 = Math_RandomRange_450F20(96, 128);
    field_F8_rnd4 = Math_RandomRange_450F20(190, 255);

    field_CC_bApplyShadows &= ~1u;

    field_10_anim.field_4_flags.Set(AnimFlags::eBit20_use_xy_offset);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);

    field_10_anim.field_C_layer = Layer::eLayer_17;
    field_10_anim.field_B_render_mode = TPageAbr::eBlend_0;
    field_10_anim.SetFrame_402AC0(1);

    // maybe width height like in door effect
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


void LightEffect::VRender_4067F0(PrimHeader** /*ppOt*/)
{
    // Pretty much the same as door effect render - commented out because OG is empty which means the "stock yard stars" are missing.
    // However rendering as-is produces over bright ugly looking stars and is probably why DD turned it off
    /*
    if (sNumCamSwappers_507668 == 0)
    {
        const FP xpos = FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) + field_A8_xpos - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x;
        const FP ypos = FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) + field_AC_ypos - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y;

        field_10_anim.field_8_r = static_cast<BYTE>(field_C0_r/4);
        field_10_anim.field_9_g = static_cast<BYTE>(field_C2_g/4);
        field_10_anim.field_A_b = static_cast<BYTE>(field_C4_b/4);

        field_10_anim.vRender(
            FP_GetExponent(FP_FromInteger((FP_GetExponent(xpos)))),
            FP_GetExponent(FP_FromInteger((FP_GetExponent(ypos)))),
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        field_10_anim.Get_Frame_Rect_402B50(&rect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager_4FF7C8->field_2E_idx);
    }
    */
}

void LightEffect::VRender(PrimHeader** ppOt)
{
    VRender_4067F0(ppOt);
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
    
    if (static_cast<int>(gnFrameCount_507670) >= field_EC_rnd1)
    {
        const int v6 = field_F0_rnd2 - field_EC_rnd1;
        if (static_cast<int>(gnFrameCount_507670) == v6 / 2)
        {
            field_F4_rnd3 = Math_RandomRange_450F20(96, 128);
        }

        const FP v9 = (FP_FromInteger(128) * FP_FromInteger(gnFrameCount_507670 - field_EC_rnd1) / FP_FromInteger(v6));
        const FP v11 = -Math_Cosine_4510A0(FP_GetExponent(v9) & 0xFF);

        int tmp = field_F4_rnd3 + FP_GetExponent(FP_FromInteger(field_F8_rnd4) * v11);
        
        BYTE rgb = 0;
        if (tmp <= 255)
        {
            rgb = tmp & 0xFF;
        }
        else
        {
            rgb = 255;
        }

        field_C0_r = rgb;
        field_C2_g = rgb;
        field_C4_b = rgb;
    }
    else if (static_cast<int>(gnFrameCount_507670) > field_F0_rnd2)
    {
        field_EC_rnd1 = gnFrameCount_507670 + Math_RandomRange_450F20(2, 8);
        field_F0_rnd2 = field_EC_rnd1 + Math_RandomRange_450F20(4, 8);
        field_F8_rnd4 = Math_RandomRange_450F20(150, 180);
        BYTE rgb = 96;

        field_C0_r = rgb;
        field_C2_g = rgb;
        field_C4_b = rgb;
    }

}

}
