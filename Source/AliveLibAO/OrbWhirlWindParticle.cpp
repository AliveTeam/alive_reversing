#include "stdafx_ao.h"
#include "Function.hpp"
#include "OrbWhirlWindParticle.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

void OrbWhirlWindParticle::CalculateRenderProperties_48BDC0(__int16 bStarted)
{
    field_B8_render_angle += field_BC_counter;

    if (bStarted && field_BC_counter <= field_BE_max && !(gnFrameCount_507670 % 3))
    {
        field_BC_counter++;
    }
    else if (!bStarted && field_BC_counter >= 1 && !(gnFrameCount_507670 % 3))
    {
        field_BC_counter--;
        field_D4_radiusX += FP_FromInteger(4);
    }

    field_A0_xpos_render_offset = ((field_C0_current_scale * field_D4_radiusX) * Math_Sine_451110(static_cast<BYTE>(field_B8_render_angle))) + field_CC_xpos_mid;
    field_A4_ypos_render_offset = ((field_C0_current_scale * field_D8_radiosY) * Math_Cosine_4510A0(static_cast<BYTE>(field_B8_render_angle))) + field_D0_ypos_mid;
    field_A8_render_as_scale = (field_C0_current_scale * field_C4_randomized_scale);
    if (field_C0_current_scale > FP_FromDouble(0.599))
    {
        field_8_anim.field_C_layer = 32;
    }
    else
    {
        field_8_anim.field_C_layer = 13;
    }
}

void OrbWhirlWindParticle::ToStop()
{
    field_B4_state = State::State_4_Stop;
    field_DC_position_timer = gnFrameCount_507670 + Math_RandomRange_450F20(0, 32);
}

int OrbWhirlWindParticle::IsActive()
{
    return field_4_flags & 1;
}

void OrbWhirlWindParticle::VRender_48C430(int** ppOt)
{
    field_8_anim.field_14_scale = field_A8_render_as_scale;

    const short xpos = FP_GetExponent(FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)
        + field_A0_xpos_render_offset
        - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x);

    const short ypos = FP_GetExponent(FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + 5)
        + field_A4_ypos_render_offset
        - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y);

    field_8_anim.VRender_403AE0(
        xpos,
        ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    field_8_anim.Get_Frame_Rect_402B50(&rect);
    pScreenManager_4FF7C8->InvalidateRect_406E40(
        rect.x,
        rect.y,
        rect.w,
        rect.h,
        pScreenManager_4FF7C8->field_2E_idx);
}

void OrbWhirlWindParticle::SetActive(unsigned __int8 active)
{
    if (active)
    {
        field_4_flags |= 1;
    }
    else
    {
        field_4_flags &= ~1;
    }
}

void OrbWhirlWindParticle::VUpdate_48BF00()
{
    switch (field_B4_state)
    {
    case State::State_0_Start:
        CalculateRenderProperties_48BDC0(1);
        break;

    case State::State_1_Spin:
        if (static_cast<int>(gnFrameCount_507670) < field_DC_position_timer + 16)
        {
            if (static_cast<int>(gnFrameCount_507670) >= field_DC_position_timer)
            {
                field_D0_ypos_mid += field_B0_ypos_increment;
                field_D4_radiusX -= FP_FromInteger(2);
                field_D8_radiosY -= FP_FromInteger(1);
            }
            CalculateRenderProperties_48BDC0(1);
        }
        else
        {
            if (!field_E4_pObj || field_E4_pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                ToStop();
            }
            else
            {
                field_F8_ypos_offset = field_CC_xpos_mid;
                field_F0_scale = field_CC_xpos_mid;
                field_FC_xpos_offset2 = field_D0_ypos_mid;
                field_F4_xpos_offset = field_D0_ypos_mid;
                field_C8_scale_offset_fly_to_target = (field_E4_pObj->field_BC_sprite_scale - field_C0_current_scale) / FP_FromInteger(16);
                field_DC_position_timer = gnFrameCount_507670 + 16;
                field_B4_state = State::State_2_FlyToTarget;
                CalculateRenderProperties_48BDC0(1);
            }
        }
        break;

    case State::State_2_FlyToTarget:
        if (!field_E4_pObj || field_E4_pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            ToStop();
        }
        else
        {
            PSX_RECT bRect = {};
            field_E4_pObj->VGetBoundingRect(&bRect, 1);

            const FP xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP ypos = FP_FromInteger((bRect.y + bRect.h) / 2);

            if (static_cast<int>(gnFrameCount_507670) < field_DC_position_timer)
            {
                field_C0_current_scale += field_C8_scale_offset_fly_to_target;
                const FP v25 = FP_FromInteger(16 - (field_DC_position_timer - gnFrameCount_507670)) / FP_FromInteger(16);
                field_F8_ypos_offset = ((xpos - field_F0_scale) * v25) + field_F0_scale;
                field_FC_xpos_offset2 = ((ypos - field_F4_xpos_offset) * v25) + field_F4_xpos_offset;
                field_CC_xpos_mid = field_F8_ypos_offset + ((FP_FromInteger(32) * field_C0_current_scale) * Math_Sine_451110(FP_GetExponent(FP_FromInteger(128) * v25) & 0xFF));
                field_D0_ypos_mid = field_FC_xpos_offset2 + ((FP_FromInteger(32) * field_C0_current_scale) * Math_Cosine_4510A0(FP_GetExponent(FP_FromInteger(128) * v25) & 0xFF));
            }
            else
            {
                field_F8_ypos_offset = xpos;
                field_FC_xpos_offset2 = ypos;
                field_CC_xpos_mid = xpos;
                field_D0_ypos_mid = ypos;
                field_B8_render_angle = 192;
                field_D4_radiusX = FP_FromInteger(40);
                field_AC_radiusX_offset = field_D4_radiusX / FP_FromInteger(32);
                field_104_scale_offset_spin_at_target = field_C0_current_scale * FP_FromInteger(Math_RandomRange_450F20(-16, 16));
                field_DC_position_timer = gnFrameCount_507670 + 32;
                field_B4_state = State::State_3_SpinAtTarget;
            }
            CalculateRenderProperties_48BDC0(1);
        }
        break;

    case State::State_3_SpinAtTarget:
        if (static_cast<int>(gnFrameCount_507670) >= field_DC_position_timer)
        {
            SetActive(IsActive() ? 0 : 1);
        }
        field_D0_ypos_mid = (field_104_scale_offset_spin_at_target * Math_Cosine_4510A0(
            FP_GetExponent(
                FP_FromInteger(128) *
                FP_FromInteger(32 - (field_DC_position_timer + gnFrameCount_507670)) / FP_FromInteger(32)) & 0xFF)) + field_FC_xpos_offset2;
        field_D4_radiusX -= field_AC_radiusX_offset;
        CalculateRenderProperties_48BDC0(1);
        break;

    case State::State_4_Stop:
        if (static_cast<int>(gnFrameCount_507670) >= field_DC_position_timer)
        {
            SetActive(IsActive() ? 0 : 1);
        }
        CalculateRenderProperties_48BDC0(0);
        break;

    default:
        return;
    }
}

OrbWhirlWindParticle* OrbWhirlWindParticle::Vdtor_48C510(signed int flags)
{
    SetVTable(this, 0x4BD7B8);

    field_8_anim.vCleanUp();

    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

OrbWhirlWindParticle* OrbWhirlWindParticle::ctor_48BC10(FP xpos, FP ypos, FP scale)
{
    SetVTable(&field_8_anim, 0x4BA2B8);

    SetVTable(this, 0x4BD7B8);

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kOmmflareResID, 1, 0);
    field_8_anim.Init_402D20(1492, gObjList_animations_505564, 0, 38, 21, ppRes, 1, 0, 0);

    field_8_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_8_anim.field_C_layer = 32;
    field_8_anim.field_B_render_mode = 1;

    field_8_anim.field_8_r = 80;
    field_8_anim.field_9_g = 80;
    field_8_anim.field_A_b = 80;

    field_8_anim.SetFrame_402AC0(Math_RandomRange_450F20(0, field_8_anim.Get_Frame_Count_403540() - 1));
    field_4_flags &= ~1u;
    field_B4_state = State::State_0_Start;
    field_B8_render_angle = Math_RandomRange_450F20(0, 255);
    field_BC_counter = 1;
    field_BE_max = Math_RandomRange_450F20(9, 15);
    field_CC_xpos_mid = xpos;
    field_D0_ypos_mid = ypos + FP_FromInteger(Math_RandomRange_450F20(-12, 12));
    field_E0_yMove = ypos - FP_FromInteger(16);
    field_D4_radiusX = FP_FromInteger(Math_RandomRange_450F20(37, 43));
    field_D8_radiosY = FP_FromDouble(0.25) * field_D4_radiusX;
    field_C0_current_scale = scale;
    field_C4_randomized_scale = FP_FromInteger(Math_RandomRange_450F20(7, 10)) / FP_FromInteger(10);
    field_A8_render_as_scale = field_C0_current_scale * field_C4_randomized_scale;
    return this;
}

void OrbWhirlWindParticle::Spin(FP xpos, FP ypos, BaseAliveGameObject* pObj)
{
    field_DC_position_timer = gnFrameCount_507670 + Math_RandomRange_450F20(0, 16);
    field_B4_state = State::State_1_Spin;
    field_E4_pObj = pObj;

    field_B0_ypos_increment = (field_C0_current_scale * field_E0_yMove - field_D0_ypos_mid) / FP_FromInteger(16);
    field_E8_xpos = xpos;
    field_EC_ypos = ypos;
}

}

