#include "stdafx.h"
#include "OrbWhirlWindParticle.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"

OrbWhirlWindParticle* OrbWhirlWindParticle::ctor_4E40C0(FP xpos, FP ypos, FP scale, __int16 bIsMudokonSpirit)
{
    SetVTable(&field_8_Anim, 0x544290);

    SetVTable(this, 0x5480D4);

    BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kOmmflareResID, TRUE, FALSE);
    field_108_res = ppRes;

    field_8_Anim.Init_40A030(1632, gObjList_animations_5C1A24, 0, 39, 21, ppRes, 1, 0, 0);

    field_B6_unused = bIsMudokonSpirit;

    field_8_Anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_8_Anim.field_C_render_layer = 32;
    field_8_Anim.field_B_render_mode = 1;
    if (bIsMudokonSpirit == 1)
    {
        field_8_Anim.field_8_r = 0;
        field_8_Anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_8_Anim.field_9_g = 255;
        field_8_Anim.field_A_b = 32;
    }
    else
    {
        field_8_Anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
        field_8_Anim.field_8_r = 80;
        field_8_Anim.field_9_g = 80;
        field_8_Anim.field_A_b = 80;
    }

    field_8_Anim.SetFrame_409D50(Math_RandomRange_496AB0(0, field_8_Anim.Get_Frame_Count_40AC70() - 1));
    SetActive_4E4340(0);
    field_B4_state = State::State_0_Start;
    field_B8_render_angle = Math_RandomRange_496AB0(0, 255);
    field_BC_counter = 1;
    field_BE_max = Math_RandomRange_496AB0(9, 15);
    field_CC_xpos_mid = xpos;
    field_D0_ypos_mid = ypos + FP_FromInteger(Math_RandomRange_496AB0(-12, 12));
    field_E0_yMove = ypos - FP_FromInteger(16);
    field_D4_radiusX = FP_FromInteger(Math_RandomRange_496AB0(37, 43));
    field_D8_radiusY = FP_FromDouble(0.25) * field_D4_radiusX;
    field_C0_current_scale = scale;
    field_C4_randomized_scale = FP_FromInteger(Math_RandomRange_496AB0(7, 10)) / FP_FromInteger(10);
    field_A8_render_as_scale = (field_C0_current_scale * field_C4_randomized_scale);
    return this;
}

int OrbWhirlWindParticle::IsActive_4E4370()
{
    return field_4_flags & 1;
}

void OrbWhirlWindParticle::Spin_4E4A10(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
{
    field_DC_position_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(0, 16);
    field_B4_state = State::State_1_Spin;
    field_E4_pObj = pObj;
    field_B0_ypos_increment = (field_C0_current_scale * (field_E0_yMove - field_D0_ypos_mid)) / FP_FromInteger(16);
    field_EC_ypos = ypos;
    field_E8_xpos = xpos;
    field_F0_scale = scale;
}

void OrbWhirlWindParticle::ToStop_4E4AD0()
{
    field_B4_state = State::State_4_Stop;
    field_DC_position_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(0, 32);
}

OrbWhirlWindParticle* OrbWhirlWindParticle::vdtor_4E42D0(char flags)
{
    dtor_4E4300();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void OrbWhirlWindParticle::VUpdate_4E4510()
{
    switch (field_B4_state)
    {
        case State::State_0_Start:
            CalculateRenderProperties_4E4390(1);
            break;

        case State::State_1_Spin:
            if (static_cast<int>(sGnFrame_5C1B84) < field_DC_position_timer + 16)
            {
                if (static_cast<int>(sGnFrame_5C1B84) >= field_DC_position_timer)
                {
                    field_D0_ypos_mid += field_B0_ypos_increment;
                    field_D4_radiusX -= FP_FromInteger(2);
                    field_D8_radiusY -= FP_FromInteger(1);
                }
                CalculateRenderProperties_4E4390(1);
            }
            else
            {
                if (field_E4_pObj && field_E4_pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    ToStop_4E4AD0();
                }
                else
                {
                    field_FC_xpos_offset2 = field_CC_xpos_mid;
                    field_F4_xpos_offset = field_CC_xpos_mid;
                    field_100_ypos_offset2 = field_D0_ypos_mid;
                    field_F8_ypos_offset = field_D0_ypos_mid;
                    field_C8_scale_offset_fly_to_target = (field_F0_scale - field_C0_current_scale) / FP_FromInteger(16);
                    field_DC_position_timer = sGnFrame_5C1B84 + 16;
                    field_B4_state = State::State_2_FlyToTarget;
                    CalculateRenderProperties_4E4390(1);
                }
            }
            break;

        case State::State_2_FlyToTarget:
            if (field_E4_pObj && field_E4_pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                ToStop_4E4AD0();
            }
            else
            {
                FP xpos = {};
                FP ypos = {};
                if (field_E4_pObj)
                {
                    PSX_RECT bRect = {};
                    static_cast<BaseAliveGameObject*>(field_E4_pObj)->vGetBoundingRect_424FD0(&bRect, 1);

                    xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
                    ypos = FP_FromInteger((bRect.y + bRect.h) / 2);
                }
                else
                {
                    xpos = field_E8_xpos;
                    ypos = field_EC_ypos;
                }

                if (static_cast<int>(sGnFrame_5C1B84) < field_DC_position_timer)
                {
                    field_C0_current_scale += field_C8_scale_offset_fly_to_target;
                    const FP v16 = FP_FromInteger(16 - (field_DC_position_timer - sGnFrame_5C1B84 )) / FP_FromInteger(16);
                    field_FC_xpos_offset2 =  ((xpos - field_F4_xpos_offset) * v16) + field_F4_xpos_offset;
                    field_100_ypos_offset2 = ((ypos - field_F8_ypos_offset) * v16) + field_F8_ypos_offset;
                    field_CC_xpos_mid = (FP_FromInteger(32) * field_C0_current_scale) *   Math_Sine_496DF0(FP_FromInteger(128) * v16) + field_FC_xpos_offset2;
                    field_D0_ypos_mid = (FP_FromInteger(32) * field_C0_current_scale) * Math_Cosine_496D60(FP_FromInteger(128) * v16) + field_100_ypos_offset2;
                }
                else
                {
                    field_FC_xpos_offset2 = xpos;
                    field_100_ypos_offset2 = ypos;
                    field_CC_xpos_mid = xpos;
                    field_D0_ypos_mid = ypos;
                    field_B8_render_angle = 192;
                    field_D4_radiusX = FP_FromInteger(40);
                    field_AC_radiusX_offset = field_D4_radiusX / FP_FromInteger(32);
                    field_104_scale_offset_spin_at_target = field_C0_current_scale * FP_FromInteger(Math_RandomRange_496AB0(-16, 16));
                    field_DC_position_timer = sGnFrame_5C1B84 + 32;
                    field_B4_state = State::State_3_SpinAtTarget;
                }
                CalculateRenderProperties_4E4390(1);
            }
            break;

        case State::State_3_SpinAtTarget:
            if (static_cast<int>(sGnFrame_5C1B84) >= field_DC_position_timer)
            {
                SetActive_4E4340(1);
            }

            field_D0_ypos_mid = (field_104_scale_offset_spin_at_target * Math_Cosine_496D60((FP_FromInteger(128) * FP_FromInteger(32  - (field_DC_position_timer - sGnFrame_5C1B84)) / FP_FromInteger(32)))) + field_100_ypos_offset2;
            field_D4_radiusX = field_D4_radiusX - field_AC_radiusX_offset;
            CalculateRenderProperties_4E4390(1);
            break;

        case State::State_4_Stop:
            if (static_cast<int>(sGnFrame_5C1B84) >= field_DC_position_timer)
            {
                SetActive_4E4340(1);
            }
            CalculateRenderProperties_4E4390(0);
            break;

        default:
            return;
    }
}

void OrbWhirlWindParticle::VRender_4E4B10(int** ppOt)
{
    const FP x = std::min(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x,
                          pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + FP_FromInteger(367));

    const FP w = std::max(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x,
                          pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + FP_FromInteger(367));

    const FP y = std::min(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y,
                          pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(239));
          
    const FP h = std::max(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y,
                          pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(239));

    if (field_A0_xpos_render_offset >= x && field_A0_xpos_render_offset <= w)
    {
        if (field_A4_ypos_render_offset + FP_FromInteger(5) >= y && field_A4_ypos_render_offset + FP_FromInteger(5) <= h)
        {
            field_8_Anim.field_14_scale = field_A8_render_as_scale;
            const FP xpos = field_A0_xpos_render_offset - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x;
            const FP ypos = field_A4_ypos_render_offset - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + FP_FromInteger(5);

            field_8_Anim.vRender_40B820(
                FP_GetExponent(xpos),
                FP_GetExponent(ypos),
                ppOt,
                0,
                0);

            PSX_RECT r = {};
            field_8_Anim.Get_Frame_Rect_409E10(&r);
            pScreenManager_5BB5F4->InvalidateRect_40EC90(r.x, r.y, r.w, r.h, pScreenManager_5BB5F4->field_3A_idx);
        }
    }
}

void OrbWhirlWindParticle::dtor_4E4300()
{
    field_8_Anim.vCleanUp_40C630();
    ResourceManager::FreeResource_49C330(field_108_res);
}

void OrbWhirlWindParticle::CalculateRenderProperties_4E4390(__int16 bStarted)
{
    field_B8_render_angle += field_BC_counter;

    if (bStarted)
    {
        if (field_BC_counter <= field_BE_max && !(static_cast<int>(sGnFrame_5C1B84) % 3))
        {
            field_BC_counter++;
        }
    }
    else if (field_BC_counter >= 1 && !(static_cast<int>(sGnFrame_5C1B84) % 3))
    {
        field_BC_counter--;
        field_D4_radiusX = field_D4_radiusX + FP_FromInteger(4);
    }

    field_A0_xpos_render_offset = ((field_C0_current_scale * field_D4_radiusX) * Math_Sine_496DD0(static_cast<BYTE>(field_B8_render_angle))) + field_CC_xpos_mid;
    field_A4_ypos_render_offset = ((field_C0_current_scale * field_D8_radiusY) * Math_Cosine_496CD0(static_cast<BYTE>(field_B8_render_angle))) + field_D0_ypos_mid;
    field_A8_render_as_scale = field_C0_current_scale * field_C4_randomized_scale;
    
    if (field_C0_current_scale > FP_FromDouble(0.599)) // TODO: Check VS 39321
    {
        field_8_Anim.field_C_render_layer = 32;
    }
    else
    {
        field_8_Anim.field_C_render_layer = 13;
    }
}

void OrbWhirlWindParticle::SetActive_4E4340(unsigned __int8 active)
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
