#include "stdafx_ao.h"
#include "Function.hpp"
#include "OrbWhirlWindParticle.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

void OrbWhirlWindParticle::CalculateRenderProperties(s16 bStarted)
{
    field_B8_render_angle += field_BC_counter;

    if (bStarted && field_BC_counter <= field_BE_max && !(sGnFrame % 3))
    {
        field_BC_counter++;
    }
    else if (!bStarted && field_BC_counter >= 1 && !(sGnFrame % 3))
    {
        field_BC_counter--;
        field_D4_radiusX += FP_FromInteger(4);
    }

    field_A0_xpos_render_offset = ((field_C0_current_scale * field_D4_radiusX) * Math_Sine_451110(static_cast<u8>(field_B8_render_angle))) + field_CC_xpos_mid;
    field_A4_ypos_render_offset = ((field_C0_current_scale * field_D8_radiosY) * Math_Cosine_4510A0(static_cast<u8>(field_B8_render_angle))) + field_D0_ypos_mid;
    field_A8_render_as_scale = (field_C0_current_scale * field_C4_randomized_scale);
    if (field_C0_current_scale > FP_FromDouble(0.599))
    {
        field_8_anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
    }
    else
    {
        field_8_anim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
    }
}

void OrbWhirlWindParticle::ToStop()
{
    field_B4_state = State::State_4_Stop;
    field_DC_position_timer = sGnFrame + Math_RandomRange_450F20(0, 32);
}

s32 OrbWhirlWindParticle::IsActive()
{
    return mAnimFlags & 1;
}

void OrbWhirlWindParticle::Render(PrimHeader** ppOt)
{
    field_8_anim.field_14_scale = field_A8_render_as_scale;

    const s16 xpos = FP_GetExponent(FP_FromInteger(pScreenManager->mCamXOff)
                                    + field_A0_xpos_render_offset
                                    - pScreenManager->mCamPos->field_0_x);

    const s16 ypos = FP_GetExponent(FP_FromInteger(pScreenManager->mCamYOff + 5)
                                    + field_A4_ypos_render_offset
                                    - pScreenManager->mCamPos->field_4_y);

    field_8_anim.VRender(
        xpos,
        ypos,
        ppOt,
        0,
        0);

    PSX_RECT rect = {};
    field_8_anim.Get_Frame_Rect(&rect);
    pScreenManager->InvalidateRect(
        rect.x,
        rect.y,
        rect.w,
        rect.h,
        pScreenManager->mIdx);
}

void OrbWhirlWindParticle::SetActive(u8 active)
{
    if (active)
    {
        mAnimFlags |= 1;
    }
    else
    {
        mAnimFlags &= ~1;
    }
}

void OrbWhirlWindParticle::Update()
{
    switch (field_B4_state)
    {
        case State::State_0_Start:
            CalculateRenderProperties(1);
            break;

        case State::State_1_Spin:
            if (static_cast<s32>(sGnFrame) < field_DC_position_timer + 16)
            {
                if (static_cast<s32>(sGnFrame) >= field_DC_position_timer)
                {
                    field_D0_ypos_mid += field_B0_ypos_increment;
                    field_D4_radiusX -= FP_FromInteger(2);
                    field_D8_radiosY -= FP_FromInteger(1);
                }
                CalculateRenderProperties(1);
            }
            else
            {
                if (!field_E4_pObj || field_E4_pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    ToStop();
                }
                else
                {
                    field_F8_ypos_offset = field_CC_xpos_mid;
                    field_F0_scale = field_CC_xpos_mid;
                    field_FC_xpos_offset2 = field_D0_ypos_mid;
                    field_F4_xpos_offset = field_D0_ypos_mid;
                    field_C8_scale_offset_fly_to_target = (field_E4_pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale - field_C0_current_scale) / FP_FromInteger(16);
                    field_DC_position_timer = sGnFrame + 16;
                    field_B4_state = State::State_2_FlyToTarget;
                    CalculateRenderProperties(1);
                }
            }
            break;

        case State::State_2_FlyToTarget:
            if (!field_E4_pObj || field_E4_pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                ToStop();
            }
            else
            {
                PSX_RECT bRect = {};
                field_E4_pObj->VGetBoundingRect(&bRect, 1);

                const FP xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
                const FP ypos = FP_FromInteger((bRect.y + bRect.h) / 2);

                if (static_cast<s32>(sGnFrame) < field_DC_position_timer)
                {
                    field_C0_current_scale += field_C8_scale_offset_fly_to_target;
                    const FP v25 = FP_FromInteger(16 - (field_DC_position_timer - sGnFrame)) / FP_FromInteger(16);
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
                    field_DC_position_timer = sGnFrame + 32;
                    field_B4_state = State::State_3_SpinAtTarget;
                }
                CalculateRenderProperties(1);
            }
            break;

        case State::State_3_SpinAtTarget:
            if (static_cast<s32>(sGnFrame) >= field_DC_position_timer)
            {
                SetActive(IsActive() ? 0 : 1);
            }
            field_D0_ypos_mid = (field_104_scale_offset_spin_at_target * Math_Cosine_4510A0(FP_GetExponent(FP_FromInteger(128) * FP_FromInteger(32 - (field_DC_position_timer + sGnFrame)) / FP_FromInteger(32)) & 0xFF)) + field_FC_xpos_offset2;
            field_D4_radiusX -= field_AC_radiusX_offset;
            CalculateRenderProperties(1);
            break;

        case State::State_4_Stop:
            if (static_cast<s32>(sGnFrame) >= field_DC_position_timer)
            {
                SetActive(IsActive() ? 0 : 1);
            }
            CalculateRenderProperties(0);
            break;

        default:
            return;
    }
}

OrbWhirlWindParticle::OrbWhirlWindParticle(FP xpos, FP ypos, FP scale)
{
    const AnimRecord& orbRec = AO::AnimRec(AnimId::ChantOrb_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, orbRec.mResourceId, 1, 0);
    field_8_anim.Init(orbRec.mFrameTableOffset, gObjList_animations_505564, 0, orbRec.mMaxW, orbRec.mMaxH, ppRes, 1, 0, 0);

    field_8_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    field_8_anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
    field_8_anim.mRenderMode = TPageAbr::eBlend_1;

    field_8_anim.mRed = 80;
    field_8_anim.mGreen = 80;
    field_8_anim.mBlue = 80;

    field_8_anim.SetFrame(Math_RandomRange_450F20(0, field_8_anim.Get_Frame_Count() - 1));
    mAnimFlags &= ~1u;
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
}


OrbWhirlWindParticle::~OrbWhirlWindParticle()
{
    field_8_anim.VCleanUp();
}

void OrbWhirlWindParticle::Spin(FP xpos, FP ypos, BaseAliveGameObject* pObj)
{
    field_DC_position_timer = sGnFrame + Math_RandomRange_450F20(0, 16);
    field_B4_state = State::State_1_Spin;
    field_E4_pObj = pObj;

    field_B0_ypos_increment = (field_C0_current_scale * (field_E0_yMove - field_D0_ypos_mid)) / FP_FromInteger(16);
    field_E8_xpos = xpos;
    field_EC_ypos = ypos;
}

} // namespace AO
