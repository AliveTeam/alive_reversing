#include "stdafx_ao.h"
#include "Function.hpp"
#include "BackgroundGlukkon.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Explosion.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Game.hpp"

namespace AO {

void BackgroundGlukkon::VScreenChanged()
{
    VScreenChanged_41E0E0();
}

BackgroundGlukkon* BackgroundGlukkon::Vdtor_41E0F0(s32 flags)
{
    dtor_41DCE0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* BackgroundGlukkon::VDestructor(s32 flags)
{
    return Vdtor_41E0F0(flags);
}

BaseGameObject* BackgroundGlukkon::dtor_41DCE0()
{
    SetVTable(this, 0x4BAF70);

    if (field_100_health <= FP_FromInteger(0))
    {
        gMap.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    }

    return dtor_401000();
}

BackgroundGlukkon* BackgroundGlukkon::ctor_41DBD0(Path_BackgroundGlukkon* pTlv, s32 tlvInfo)
{
    BaseAliveGameObject();
    SetVTable(this, 0x4BAF70);

    field_4_typeId = Types::eBackgroundGlukkon_42;

    const AnimRecord& rec = AO::AnimRec(AnimId::Background_Glukkon_Idle);
    u8** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes2, 1);

    field_10C_tlvInfo = tlvInfo;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_BC_sprite_scale = FP_FromInteger(pTlv->field_18_scale_percent) / FP_FromInteger(100);

    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, pTlv->field_1A_pal_id, 0, 0);
    if (ppRes)
    {
        field_10_anim.LoadPal_403090(ppRes, 0);
    }

    field_120_target_id = pTlv->field_1C_target_id;
    field_11C_voice_adjust = pTlv->field_1E_voice_adjust;
    field_110_state = BackgroundGlukkon::State::eToSetSpeakPauseTimer_0;
    return this;
}

void BackgroundGlukkon::VScreenChanged_41E0E0()
{
    mFlags.Set(BaseGameObject::eDead);
}

s16 BackgroundGlukkon::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_41DF80(pFrom);
}

s16 BackgroundGlukkon::VTakeDamage_41DF80(BaseGameObject* pFrom)
{
    if (mFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    if (pFrom->field_4_typeId == Types::eShrykull_85)
    {
        field_10_anim.Set_Animation_Data_402A40(46232, 0);
        const auto rndVol = Math_RandomRange_450F20(110, 127);
        const auto rndPitch = (75 * (Math_NextRandom() % 4)) + 200;

        if (Math_NextRandom() >= 128u)
        {
            SFX_Play_43AE60(SoundEffect::Empty_106, rndVol, rndPitch, 0);
        }
        else
        {
            SFX_Play_43AE60(SoundEffect::Empty_105, rndVol, rndPitch, 0);
        }

        field_10_anim.Set_Animation_Data_402A40(46232, 0);
        field_110_state = BackgroundGlukkon::State::eKilledByShrykull_7;
    }
    else if (pFrom->field_4_typeId == Types::eElectrocute_103 && field_100_health > FP_FromInteger(0))
    {
        field_100_health = FP_FromInteger(0);
        auto pExplosion = ao_new<Explosion>();
        if (pExplosion)
        {
            pExplosion->ctor_458B80(
                field_A8_xpos,
                field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(40)),
                field_BC_sprite_scale);
        }
        mFlags.Set(BaseGameObject::eDead);
    }
    return 1;
}

void BackgroundGlukkon::VUpdate()
{
    VUpdate_41DD60();
}

void BackgroundGlukkon::VUpdate_41DD60()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    switch (field_110_state)
    {
        case BackgroundGlukkon::State::eToSetSpeakPauseTimer_0:
            field_110_state = BackgroundGlukkon::State::eSetSpeakPauseTimer_1;
            field_118_never_read = gnFrameCount_507670 + Math_RandomRange_450F20(20, 40);
            break;

        case BackgroundGlukkon::State::eSetSpeakPauseTimer_1:
            field_110_state = BackgroundGlukkon::State::eRandomizedLaugh_2;
            field_114_speak_pause_timer = gnFrameCount_507670 + Math_RandomRange_450F20(12, 20);
            break;

        case BackgroundGlukkon::State::eRandomizedLaugh_2:
            if (static_cast<s32>(gnFrameCount_507670) > field_114_speak_pause_timer)
            {
                const auto rndVol = Math_RandomRange_450F20(110, 127);
                const auto rndPitch = ((Math_NextRandom() % 4) * 128) + 200;

                switch (Math_NextRandom() % 5)
                {
                    case 0:
                        if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
                        {
                            const AnimRecord& rec = AO::AnimRec(AnimId::Background_Glukkon_Laugh);
                            field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, 0);
                            SFX_Play_43AE60(SoundEffect::GlukkonLaugh1_103, rndVol, rndPitch, 0);
                        }
                        else
                        {
                            const AnimRecord& rec = AO::AnimRec(AnimId::Background_Glukkon_KillHim1);
                            field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, 0);
                            SFX_Play_43AE60(SoundEffect::GlukkonKillHim1_101, rndVol, rndPitch, 0);
                        }
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;

                    case 1:
                        if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
                        {
                            const AnimRecord& rec = AO::AnimRec(AnimId::Background_Glukkon_Laugh);
                            field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, 0);
                            SFX_Play_43AE60(SoundEffect::GlukkonLaugh1_103, rndVol, rndPitch, 0);
                        }
                        else
                        {
                            const AnimRecord& rec = AO::AnimRec(AnimId::Background_Glukkon_KillHim2);
                            field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, 0);
                            SFX_Play_43AE60(SoundEffect::GlukkonKillHim2_102, rndVol, rndPitch, 0);
                        }
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;

                    case 2:
                        if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
                        {
                            const AnimRecord& rec = AO::AnimRec(AnimId::Background_Glukkon_Laugh);
                            field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, 0);
                            SFX_Play_43AE60(SoundEffect::GlukkonLaugh1_103, rndVol, rndPitch, 0);
                        }
                        else
                        {
                            const AnimRecord& rec = AO::AnimRec(AnimId::Background_Glukkon_KillHim1);
                            field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, 0);
                            SFX_Play_43AE60(SoundEffect::Empty_105, rndVol, rndPitch, 0);
                        }
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;

                    case 3:
                        if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
                        {
                            const AnimRecord& rec = AO::AnimRec(AnimId::Background_Glukkon_KillHim2);
                            field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, 0);
                            SFX_Play_43AE60(SoundEffect::Empty_106, rndVol, rndPitch, 0);
                        }
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;

                    case 4:
                        return;

                    default:
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;
                }
            }
            break;

        case BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_10_anim.Set_Animation_Data_402A40(46096, 0);
                field_110_state = BackgroundGlukkon::State::eSetSpeakPauseTimer_1;
            }
            break;

        default:
            return;
    }
}

} // namespace AO
