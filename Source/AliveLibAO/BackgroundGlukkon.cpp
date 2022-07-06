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

BackgroundGlukkon::~BackgroundGlukkon()
{
    if (mHealth <= FP_FromInteger(0))
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
    }
}

BackgroundGlukkon::BackgroundGlukkon(Path_BackgroundGlukkon* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eBackgroundGlukkon;

    const AnimRecord rec = AO::AnimRec(AnimId::Background_Glukkon_Idle);
    u8** ppRes2 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Background_Glukkon_Idle, ppRes2);

    field_10C_tlvInfo = tlvInfo;

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    mSpriteScale = FP_FromInteger(pTlv->field_18_scale_percent) / FP_FromInteger(100);

    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, pTlv->field_1A_pal_id, 0, 0);
    if (ppRes)
    {
        mAnim.LoadPal(ppRes, 0);
    }

    field_120_target_id = pTlv->field_1C_target_id;
    field_11C_voice_adjust = pTlv->field_1E_voice_adjust;
    field_110_state = BackgroundGlukkon::State::eToSetSpeakPauseTimer_0;
}

void BackgroundGlukkon::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

s16 BackgroundGlukkon::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    if (pFrom->mBaseGameObjectTypeId == ReliveTypes::eShrykull)
    {
        mAnim.Set_Animation_Data(AnimId::Background_Glukkon_Dying, nullptr);
        const auto rndVol = Math_RandomRange(110, 127);
        const auto rndPitch = (75 * (Math_NextRandom() % 4)) + 200;

        if (Math_NextRandom() >= 128u)
        {
            SFX_Play_Pitch(SoundEffect::Empty_106, rndVol, rndPitch, 0);
        }
        else
        {
            SFX_Play_Pitch(SoundEffect::Empty_105, rndVol, rndPitch, 0);
        }

        mAnim.Set_Animation_Data(AnimId::Background_Glukkon_Dying, nullptr);
        field_110_state = BackgroundGlukkon::State::eKilledByShrykull_7;
    }
    else if (pFrom->mBaseGameObjectTypeId == ReliveTypes::eElectrocute && mHealth > FP_FromInteger(0))
    {
        mHealth = FP_FromInteger(0);

        relive_new Explosion(
            mXPos,
            mYPos - (mSpriteScale * FP_FromInteger(40)),
            mSpriteScale);

        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    return 1;
}

void BackgroundGlukkon::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_110_state)
    {
        case BackgroundGlukkon::State::eToSetSpeakPauseTimer_0:
            field_110_state = BackgroundGlukkon::State::eSetSpeakPauseTimer_1;
            field_118_never_read = sGnFrame + Math_RandomRange(20, 40);
            break;

        case BackgroundGlukkon::State::eSetSpeakPauseTimer_1:
            field_110_state = BackgroundGlukkon::State::eRandomizedLaugh_2;
            field_114_speak_pause_timer = sGnFrame + Math_RandomRange(12, 20);
            break;

        case BackgroundGlukkon::State::eRandomizedLaugh_2:
            if (static_cast<s32>(sGnFrame) > field_114_speak_pause_timer)
            {
                const auto rndVol = Math_RandomRange(110, 127);
                const auto rndPitch = ((Math_NextRandom() % 4) * 128) + 200;

                switch (Math_NextRandom() % 5)
                {
                    case 0:
                        if (sActiveHero_507678->mHealth <= FP_FromInteger(0))
                        {
                            mAnim.Set_Animation_Data(AnimId::Background_Glukkon_Laugh, nullptr);
                            SFX_Play_Pitch(SoundEffect::GlukkonLaugh1_103, rndVol, rndPitch, 0);
                        }
                        else
                        {
                            mAnim.Set_Animation_Data(AnimId::Background_Glukkon_KillHim1, nullptr);
                            SFX_Play_Pitch(SoundEffect::GlukkonKillHim1_101, rndVol, rndPitch, 0);
                        }
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;

                    case 1:
                        if (sActiveHero_507678->mHealth <= FP_FromInteger(0))
                        {
                            mAnim.Set_Animation_Data(AnimId::Background_Glukkon_Laugh, nullptr);
                            SFX_Play_Pitch(SoundEffect::GlukkonLaugh1_103, rndVol, rndPitch, 0);
                        }
                        else
                        {
                            mAnim.Set_Animation_Data(AnimId::Background_Glukkon_KillHim2, nullptr);
                            SFX_Play_Pitch(SoundEffect::GlukkonKillHim2_102, rndVol, rndPitch, 0);
                        }
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;

                    case 2:
                        if (sActiveHero_507678->mHealth <= FP_FromInteger(0))
                        {
                            mAnim.Set_Animation_Data(AnimId::Background_Glukkon_Laugh, nullptr);
                            SFX_Play_Pitch(SoundEffect::GlukkonLaugh1_103, rndVol, rndPitch, 0);
                        }
                        else
                        {
                            mAnim.Set_Animation_Data(AnimId::Background_Glukkon_KillHim1, nullptr);
                            SFX_Play_Pitch(SoundEffect::Empty_105, rndVol, rndPitch, 0);
                        }
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;

                    case 3:
                        if (sActiveHero_507678->mHealth > FP_FromInteger(0))
                        {
                            mAnim.Set_Animation_Data(AnimId::Background_Glukkon_KillHim2, nullptr);
                            SFX_Play_Pitch(SoundEffect::Empty_106, rndVol, rndPitch, 0);
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
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mAnim.Set_Animation_Data(AnimId::Background_Glukkon_Idle, nullptr);
                field_110_state = BackgroundGlukkon::State::eSetSpeakPauseTimer_1;
            }
            break;

        default:
            return;
    }
}

} // namespace AO
