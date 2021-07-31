#include "stdlib.hpp"
#include "BaseGameObject.hpp"
#include "Events.hpp"
#include "Math.hpp"
#include "Rock.hpp"
#include "RockSack.hpp"
#include "Sfx.hpp"
#include "ThrowableArray.hpp"
#include "AnimResources.hpp"
#include "ResourceManager.hpp"
#include "FixedPoint_common.hpp"
#include "Abe.hpp"
#include "Function.hpp"
#include "Shadow.hpp"

namespace AO {

void RockSack::VUpdate()
{
    VUpdate_4575F0();
}

void RockSack::VUpdate_4575F0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_10_anim.field_92_current_frame == 2)
    {
        if (field_114_can_play_wobble_sound)
        {
            if (Math_NextRandom() < 40u || field_116_force_wobble_sound)
            {
                field_114_can_play_wobble_sound = 0;
                field_116_force_wobble_sound = 0;
                SFX_Play_43AE60(SoundEffect::SackWobble_34, 24, Math_RandomRange_450F20(-2400, -2200), 0);
            }
        }
    }
    else
    {
        field_114_can_play_wobble_sound = 1;
    }

    if (field_110_has_been_hit)
    {
        if (field_110_has_been_hit == 1)
        {
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_10_anim.Set_Animation_Data_402A40(13756, 0);
                field_110_has_been_hit = 0;
            }
        }
    }
    else
    {
        if (field_10_anim.field_E_frame_change_counter == 0)
        {
            field_10_anim.field_E_frame_change_counter = Math_RandomRange_450F20(2, 10);
        }

        PSX_RECT bPlayerRect = {};
        sActiveHero_507678->VGetBoundingRect(&bPlayerRect, 1);

        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        if (bRect.x <= bPlayerRect.w
            && bRect.w >= bPlayerRect.x
            && bRect.h >= bPlayerRect.y
            && bRect.y <= bPlayerRect.h
            && field_BC_sprite_scale == sActiveHero_507678->field_BC_sprite_scale)
        {
            if (!gpThrowableArray_50E26C || !gpThrowableArray_50E26C->field_10_count)
            {
                if (!gpThrowableArray_50E26C)
                {
                    gpThrowableArray_50E26C = ao_new<ThrowableArray>();
                    if (gpThrowableArray_50E26C)
                    {
                        gpThrowableArray_50E26C->ctor_453EE0();
                    }
                }

                gpThrowableArray_50E26C->Add_453F70(field_112_num_rocks);

                auto pRock = ao_new<Rock>();
                if (pRock)
                {
                    pRock->ctor_456960(
                        field_A8_xpos,
                        field_AC_ypos - FP_FromInteger(30),
                        field_112_num_rocks);
                }
                pRock->VThrow(field_118_x_vel, field_11C_y_vel);

                SFX_Play_43AD70(SoundEffect::SackHit_30, 0, 0);
                Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);
            }

            if (sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_33_RunJumpMid_426FA0)
            {
                field_10_anim.Set_Animation_Data_402A40(13708, 0);
            }
            else
            {
                field_10_anim.Set_Animation_Data_402A40(13780, 0);
            }

            field_110_has_been_hit = 1;
        }
    }
}

void RockSack::VScreenChanged()
{
    VScreenChanged_457890();
}

RockSack* RockSack::Vdtor_4578A0(s32 flags)
{
    dtor_457580();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* RockSack::VDestructor(s32 flags)
{
    return Vdtor_4578A0(flags);
}

BaseGameObject* RockSack::dtor_457580()
{
    SetVTable(this, 0x4BC120);
    gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    return dtor_401000();
}

RockSack* RockSack::ctor_4573F0(Path_RockSack* pTlv, s32 tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BC120);

    field_4_typeId = Types::eRockSack_71;

    const AnimRecord rec = AO::AnimRec(AnimId::Rock_Sack_A);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);

    //  Set RockSack idle anim speed
    auto pAnimationHeader = reinterpret_cast<AnimationHeader*>(*ppRes + 6878);
    pAnimationHeader->field_0_fps = 0;

    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_10C_tlvInfo = tlvInfo;
    field_110_has_been_hit = 0;
    field_CC_bApplyShadows &= ~1u;
    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
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
        u8** ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kP2c2bagResID, 0, 0);
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

}
