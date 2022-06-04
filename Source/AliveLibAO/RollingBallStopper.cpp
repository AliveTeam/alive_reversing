#include "stdafx_ao.h"
#include "RollingBallStopper.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "ResourceManager.hpp"
#include "FixedPoint.hpp"
#include "Sfx.hpp"

namespace AO {

RollingBallStopper::RollingBallStopper(Path_RollingBallStopper* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eRollingBallStopper;
    
    const AnimRecord& rec = AO::AnimRec(AnimId::Stone_Ball_Stopper);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    field_10_anim.mRenderLayer = Layer::eLayer_FG1_37;

    field_114_release_switch_id = pTlv->field_18_stopper_switch_id;

    if (pTlv->field_1A_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }

    field_116_ball_switch_id = pTlv->field_1C_ball_switch_id;

    if (pTlv->field_1E_direction == XDirection_short::eLeft_0)
    {
        field_10_anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_B4_velx = field_10_anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(22) : FP_FromInteger(-22);
    field_B8_vely = FP_FromInteger(0);

    field_10C_tlvInfo = tlvInfo;

    // Check its enabled ?
    if (pTlv->field_1_unknown)
    {
        field_AC_ypos += field_BC_sprite_scale * FP_FromInteger(70);
        if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            field_112_state = States::eMovingDone_2;
            field_A8_xpos += field_BC_sprite_scale * FP_FromInteger(35);
        }
        else
        {
            field_112_state = States::eMovingDone_2;
            field_A8_xpos -= field_BC_sprite_scale * FP_FromInteger(35);
        }
    }
    else
    {
        field_112_state = States::eWaitForTrigger_0;
        SwitchStates_Set(pTlv->field_1C_ball_switch_id, 0);
        SwitchStates_Set(pTlv->field_18_stopper_switch_id, 0);
    }

    const auto oldXPos = field_A8_xpos;
    MapFollowMe_401D30(TRUE);
    field_A8_xpos = oldXPos;

    FP lineXPos = {};
    if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        lineXPos = (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2)) + FP_NoFractional(oldXPos);
    }
    else
    {
        lineXPos = FP_NoFractional(oldXPos) - (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
    }

    const auto x1 = FP_GetExponent(lineXPos);
    const auto y1 = FP_GetExponent(field_AC_ypos);
    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        field_118_pLine = sCollisions_DArray_504C6C->Add_Dynamic_Collision_Line(x1, y1 - 70, x1, y1, eLineTypes::eWallLeft_1);
    }
    else
    {
        field_118_pLine = sCollisions_DArray_504C6C->Add_Dynamic_Collision_Line(x1, y1 - 35, x1, y1, eLineTypes::eBackgroundWallLeft_5);
    }
}

RollingBallStopper::~RollingBallStopper()
{
    if (field_112_state != States::eWaitForTrigger_0)
    {
        gMap.TLV_Reset(field_10C_tlvInfo, 1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_10C_tlvInfo, 0, 0, 0);
    }

    if (field_118_pLine)
    {
        Rect_Clear(&field_118_pLine->field_0_rect);
    }
}

void RollingBallStopper::VScreenChanged()
{
    if (field_112_state == States::eMoveStopper_1)
    {
        SwitchStates_Set(field_116_ball_switch_id, 1);
    }
    mBaseGameObjectFlags.Set(Options::eDead);
}

void RollingBallStopper::VUpdate()
{
    switch (field_112_state)
    {
        case States::eWaitForTrigger_0:
            if (SwitchStates_Get(field_114_release_switch_id))
            {
                Rect_Clear(&field_118_pLine->field_0_rect);
                field_118_pLine = nullptr;
                field_112_state = States::eMoveStopper_1;
                SFX_Play_Pitch(SoundEffect::PickupItem_33, 100, -2400, 0);
                SFX_Play_Pitch(SoundEffect::LiftStop_35, 80, -800, 0);
            }
            break;

        case States::eMoveStopper_1:
            field_B8_vely += (field_BC_sprite_scale * FP_FromInteger(25));
            if (field_B8_vely <= (field_BC_sprite_scale * FP_FromInteger(70)))
            {
                field_A8_xpos += field_B4_velx;
                field_AC_ypos += (field_BC_sprite_scale * FP_FromInteger(25));
            }
            else
            {
                field_112_state = States::eMovingDone_2;
                SwitchStates_Set(field_116_ball_switch_id, 1);
            }
            break;
    }
}

} // namespace AO
