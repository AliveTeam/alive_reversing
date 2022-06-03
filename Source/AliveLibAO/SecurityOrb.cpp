#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityOrb.hpp"
#include "ResourceManager.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "Sparks.hpp"
#include "Abe.hpp"
#include "PossessionFlicker.hpp"
#include "ScreenShake.hpp"
#include "Flash.hpp"
#include "Game.hpp"
#include "Events.hpp"

namespace AO {

SecurityOrb::SecurityOrb(Path_SecurityOrb* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);

    mBaseGameObjectTypeId = Types::SecurityOrb_53;

    const AnimRecord& rec = AO::AnimRec(AnimId::Security_Orb);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_10C_tlvInfo = tlvInfo;

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    field_110_state = SecurityOrbStates::eIdle_0;
    field_118_sound_channels = 0;
}

SecurityOrb::~SecurityOrb()
{
    if (field_118_sound_channels)
    {
        SND_Stop_Channels_Mask_4774A0(field_118_sound_channels);
    }

    if (mHealth > FP_FromInteger(0))
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 1);
    }
}

void SecurityOrb::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

s16 SecurityOrb::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->mBaseGameObjectTypeId)
    {
        case Types::eAbilityRing_69:
        case Types::eShrykull_85:
        {
            ao_new<Explosion>(
                field_A8_xpos,
                field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(5)),
                field_BC_sprite_scale);

            ao_new<Gibs>(
                GibType::Metal_5,
                field_A8_xpos,
                field_AC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_BC_sprite_scale);

            mHealth = FP_FromInteger(0);
        }
        break;

        default:
            break;
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

    return 1;
}

void SecurityOrb::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_110_state)
    {
        case SecurityOrbStates::eIdle_0:
            if (field_10_anim.field_92_current_frame == 2 || field_10_anim.field_92_current_frame == 6 || field_10_anim.field_92_current_frame == 10)
            {
                if (field_118_sound_channels)
                {
                    SND_Stop_Channels_Mask_4774A0(field_118_sound_channels);
                }

                if (field_BC_sprite_scale == FP_FromDouble(0.5))
                {
                    field_118_sound_channels = SFX_Play_Pitch(SoundEffect::SecurityOrb_56, 35, 720);
                }
                else
                {
                    field_118_sound_channels = SFX_Play_Pitch(SoundEffect::SecurityOrb_56, 55, 700);
                }
            }

            if (Event_Get(kEventAbeOhm_8))
            {
                field_110_state = SecurityOrbStates::eDoZapEffects_1;
                field_114_timer = sGnFrame + 20;
            }
            break;

        case SecurityOrbStates::eDoZapEffects_1:
            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                PSX_RECT abeRect = {};
                sActiveHero_507678->VGetBoundingRect(&abeRect, 1);

                const s32 width = abeRect.w + abeRect.x;
                const s32 height = abeRect.h + abeRect.y;

                ao_new<ZapLine>(
                    field_A8_xpos,
                    field_AC_ypos - (FP_FromInteger(8) * field_BC_sprite_scale),
                    FP_FromInteger(width / 2),
                    FP_FromInteger(height / 2),
                    8,
                    ZapLineType::eThick_0,
                    Layer::eLayer_ZapLinesElum_28);

                ao_new<PossessionFlicker>(sActiveHero_507678, 8, 255, 100, 100);

                sActiveHero_507678->VTakeDamage(this);
                field_114_timer = sGnFrame + 8;
                field_110_state = SecurityOrbStates::eDoFlashAndSound_2;

                ao_new<ScreenShake>(1);

                auto pSpark1 = ao_new<Sparks>(
                    field_A8_xpos,
                    field_AC_ypos - (FP_FromInteger(8) * field_BC_sprite_scale),
                    field_BC_sprite_scale);
                if (pSpark1)
                {
                    pSpark1->field_C2_g = 65;
                    pSpark1->field_C4_b = 65;
                    pSpark1->field_C0_r = 255;
                }

                auto pSpark2 = ao_new<Sparks>(
                    field_A8_xpos,
                    field_AC_ypos - (FP_FromInteger(8) * field_BC_sprite_scale),
                    field_BC_sprite_scale);
                if (pSpark2)
                {
                    pSpark2->field_C2_g = 65;
                    pSpark2->field_C4_b = 65;
                    pSpark2->field_C0_r = 255;
                }


                for (s32 i = 0; i < 9; i++)
                {
                    auto pSparks = ao_new<Sparks>(
                        FP_FromInteger(width / 2),
                        FP_FromInteger(height / 2),
                        field_BC_sprite_scale);
                    if (pSparks)
                    {
                        pSparks->field_C2_g = 65;
                        pSparks->field_C4_b = 65;
                        pSparks->field_C0_r = 255;
                    }
                }
            }
            break;

        case SecurityOrbStates::eDoFlashAndSound_2:
            if (static_cast<s32>(sGnFrame) == field_114_timer - 5 || static_cast<s32>(sGnFrame) == field_114_timer - 1)
            {
                ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 0, 0);
            }

            if (static_cast<s32>(sGnFrame) == field_114_timer - 4)
            {
                ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 0, 0, 1, TPageAbr::eBlend_1, 1);
            }

            if (field_114_timer - sGnFrame == 4)
            {
                SFX_Play_Mono(SoundEffect::Zap1_57, 0, 0);
            }
            else if (field_114_timer - sGnFrame == 1)
            {
                SFX_Play_Mono(SoundEffect::Zap2_58, 0, 0);
            }

            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                field_110_state = SecurityOrbStates::eIdle_0;
            }
            break;
    }
}

} // namespace AO
