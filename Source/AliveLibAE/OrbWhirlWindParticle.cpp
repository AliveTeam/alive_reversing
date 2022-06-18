#include "stdafx.h"
#include "OrbWhirlWindParticle.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"

OrbWhirlWindParticle::OrbWhirlWindParticle(FP xpos, FP ypos, FP scale, s16 bIsMudokonSpirit)
{
    const AnimRecord& orbRec = AnimRec(AnimId::ChantOrb_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, orbRec.mResourceId, TRUE, FALSE);
    field_108_res = ppRes;

    field_8_Anim.Init(orbRec.mFrameTableOffset, gAnimations, 0, orbRec.mMaxW, orbRec.mMaxH, ppRes, 1);

    field_B6_unused = bIsMudokonSpirit;

    field_8_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    field_8_Anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
    field_8_Anim.mRenderMode = TPageAbr::eBlend_1;
    if (bIsMudokonSpirit == 1)
    {
        field_8_Anim.mRed = 0;
        field_8_Anim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
        field_8_Anim.mGreen = 255;
        field_8_Anim.mBlue = 32;
    }
    else
    {
        field_8_Anim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);
        field_8_Anim.mRed = 80;
        field_8_Anim.mGreen = 80;
        field_8_Anim.mBlue = 80;
    }

    field_8_Anim.SetFrame(Math_RandomRange(0, field_8_Anim.Get_Frame_Count() - 1));
    SetActive(0);
    field_B4_state = State::State_0_Start;
    field_B8_render_angle = Math_RandomRange(0, 255);
    field_BC_counter = 1;
    field_BE_max = Math_RandomRange(9, 15);
    field_CC_xpos_mid = xpos;
    field_D0_ypos_mid = ypos + FP_FromInteger(Math_RandomRange(-12, 12));
    field_E0_yMove = ypos - FP_FromInteger(16);
    field_D4_radiusX = FP_FromInteger(Math_RandomRange(37, 43));
    field_D8_radiusY = FP_FromDouble(0.25) * field_D4_radiusX;
    field_C0_current_scale = scale;
    field_C4_randomized_scale = FP_FromInteger(Math_RandomRange(7, 10)) / FP_FromInteger(10);
    field_A8_render_as_scale = (field_C0_current_scale * field_C4_randomized_scale);
}

s32 OrbWhirlWindParticle::IsActive()
{
    return mAnimFlags.Get(Flags_4::eBit1_is_active);
}

void OrbWhirlWindParticle::Spin(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
{
    field_DC_position_timer = sGnFrame + Math_RandomRange(0, 16);
    field_B4_state = State::State_1_Spin;
    field_E4_pObj = pObj;
    field_B0_ypos_increment = (field_C0_current_scale * (field_E0_yMove - field_D0_ypos_mid)) / FP_FromInteger(16);
    field_EC_ypos = ypos;
    field_E8_xpos = xpos;
    field_F0_scale = scale;
}

void OrbWhirlWindParticle::ToStop()
{
    field_B4_state = State::State_4_Stop;
    field_DC_position_timer = sGnFrame + Math_RandomRange(0, 32);
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
                    field_D8_radiusY -= FP_FromInteger(1);
                }
                CalculateRenderProperties(1);
            }
            else
            {
                if (field_E4_pObj && field_E4_pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    ToStop();
                }
                else
                {
                    field_FC_xpos_offset2 = field_CC_xpos_mid;
                    field_F4_xpos_offset = field_CC_xpos_mid;
                    field_100_ypos_offset2 = field_D0_ypos_mid;
                    field_F8_ypos_offset = field_D0_ypos_mid;
                    field_C8_scale_offset_fly_to_target = (field_F0_scale - field_C0_current_scale) / FP_FromInteger(16);
                    field_DC_position_timer = sGnFrame + 16;
                    field_B4_state = State::State_2_FlyToTarget;
                    CalculateRenderProperties(1);
                }
            }
            break;

        case State::State_2_FlyToTarget:
            if (field_E4_pObj && field_E4_pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                ToStop();
            }
            else
            {
                FP xpos = {};
                FP ypos = {};
                if (field_E4_pObj)
                {
                    const PSX_RECT bRect = static_cast<BaseAliveGameObject*>(field_E4_pObj)->VGetBoundingRect();

                    xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
                    ypos = FP_FromInteger((bRect.y + bRect.h) / 2);
                }
                else
                {
                    xpos = field_E8_xpos;
                    ypos = field_EC_ypos;
                }

                if (static_cast<s32>(sGnFrame) < field_DC_position_timer)
                {
                    field_C0_current_scale += field_C8_scale_offset_fly_to_target;
                    const FP v16 = FP_FromInteger(16 - (field_DC_position_timer - sGnFrame)) / FP_FromInteger(16);
                    field_FC_xpos_offset2 = ((xpos - field_F4_xpos_offset) * v16) + field_F4_xpos_offset;
                    field_100_ypos_offset2 = ((ypos - field_F8_ypos_offset) * v16) + field_F8_ypos_offset;
                    field_CC_xpos_mid = (FP_FromInteger(32) * field_C0_current_scale) * Math_Sine_496DF0(FP_FromInteger(128) * v16) + field_FC_xpos_offset2;
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
                    field_104_scale_offset_spin_at_target = field_C0_current_scale * FP_FromInteger(Math_RandomRange(-16, 16));
                    field_DC_position_timer = sGnFrame + 32;
                    field_B4_state = State::State_3_SpinAtTarget;
                }
                CalculateRenderProperties(1);
            }
            break;

        case State::State_3_SpinAtTarget:
            if (static_cast<s32>(sGnFrame) >= field_DC_position_timer)
            {
                SetActive(1);
            }

            field_D0_ypos_mid = (field_104_scale_offset_spin_at_target * Math_Cosine_496D60((FP_FromInteger(128) * FP_FromInteger(32 - (field_DC_position_timer - sGnFrame)) / FP_FromInteger(32)))) + field_100_ypos_offset2;
            field_D4_radiusX = field_D4_radiusX - field_AC_radiusX_offset;
            CalculateRenderProperties(1);
            break;

        case State::State_4_Stop:
            if (static_cast<s32>(sGnFrame) >= field_DC_position_timer)
            {
                SetActive(1);
            }
            CalculateRenderProperties(0);
            break;

        default:
            return;
    }
}

void OrbWhirlWindParticle::Render(PrimHeader** ppOt)
{
    const FP x = std::min(pScreenManager->CamXPos(),
                          pScreenManager->CamXPos() + FP_FromInteger(367));

    const FP w = std::max(pScreenManager->CamXPos(),
                          pScreenManager->CamXPos() + FP_FromInteger(367));

    const FP y = std::min(pScreenManager->CamYPos(),
                          pScreenManager->CamYPos() + FP_FromInteger(239));

    const FP h = std::max(pScreenManager->CamYPos(),
                          pScreenManager->CamYPos() + FP_FromInteger(239));

    if (field_A0_xpos_render_offset >= x && field_A0_xpos_render_offset <= w)
    {
        if (field_A4_ypos_render_offset + FP_FromInteger(5) >= y && field_A4_ypos_render_offset + FP_FromInteger(5) <= h)
        {
            field_8_Anim.field_14_scale = field_A8_render_as_scale;
            const FP xpos = field_A0_xpos_render_offset - pScreenManager->CamXPos();
            const FP ypos = field_A4_ypos_render_offset - pScreenManager->CamYPos() + FP_FromInteger(5);

            field_8_Anim.VRender(
                FP_GetExponent(xpos),
                FP_GetExponent(ypos),
                ppOt,
                0,
                0);

            PSX_RECT r = {};
            field_8_Anim.Get_Frame_Rect(&r);
            pScreenManager->InvalidateRectCurrentIdx(r.x, r.y, r.w, r.h);
        }
    }
}

OrbWhirlWindParticle::~OrbWhirlWindParticle()
{
    field_8_Anim.VCleanUp();
    ResourceManager::FreeResource_49C330(field_108_res);
}

void OrbWhirlWindParticle::CalculateRenderProperties(s16 bStarted)
{
    field_B8_render_angle += field_BC_counter;

    if (bStarted)
    {
        if (field_BC_counter <= field_BE_max && !(static_cast<s32>(sGnFrame) % 3))
        {
            field_BC_counter++;
        }
    }
    else if (field_BC_counter >= 1 && !(static_cast<s32>(sGnFrame) % 3))
    {
        field_BC_counter--;
        field_D4_radiusX = field_D4_radiusX + FP_FromInteger(4);
    }

    field_A0_xpos_render_offset = ((field_C0_current_scale * field_D4_radiusX) * Math_Sine_496DD0(static_cast<u8>(field_B8_render_angle))) + field_CC_xpos_mid;
    field_A4_ypos_render_offset = ((field_C0_current_scale * field_D8_radiusY) * Math_Cosine_496CD0(static_cast<u8>(field_B8_render_angle))) + field_D0_ypos_mid;
    field_A8_render_as_scale = field_C0_current_scale * field_C4_randomized_scale;

    if (field_C0_current_scale > FP_FromDouble(0.599)) // TODO: Check VS 39321
    {
        field_8_Anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
    }
    else
    {
        field_8_Anim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
    }
}

void OrbWhirlWindParticle::SetActive(u8 active)
{
    if (active)
    {
        mAnimFlags.Set(Flags_4::eBit1_is_active);
    }
    else
    {
        mAnimFlags.Clear(Flags_4::eBit1_is_active);
    }
}
