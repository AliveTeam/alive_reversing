#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityClaw.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "MotionDetector.hpp"
#include "Gibs.hpp"
#include "Explosion.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "Sparks.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "Flash.hpp"
#include "Events.hpp"
#include "Alarm.hpp"
#include "ScreenShake.hpp"
#include "PossessionFlicker.hpp"

namespace AO {

const TintEntry kSecurityClawTints_4C5488[] = {
    {LevelIds_s8::eStockYards_5, 105u, 55u, 55u},
    {LevelIds_s8::eStockYardsReturn_6, 105u, 55u, 55u},
    {LevelIds_s8::eNone, 127u, 127u, 127u},
};

const TintEntry kClawTints_4C5498[3] = {
    {LevelIds_s8::eStockYards_5, 80u, 55u, 55u},
    {LevelIds_s8::eStockYardsReturn_6, 80u, 55u, 55u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};

Claw::Claw()
{
    mTypeId = Types::eClawOrBirdPortalTerminator_48;
    
    const AnimRecord& rec = AO::AnimRec(AnimId::Security_Claw_Lower_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
}


void Claw::VScreenChanged()
{
    // Keep alive as the Claw is owned by the SecurityClaw
}

SecurityClaw::SecurityClaw(Path_SecurityClaw* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mTypeId = Types::eSecurityClaw_31;

    mGameObjectFlags.Set(Options::eCanExplode_Bit7);
    field_12C_pDetector = 1;

    const AnimRecord& rec = AO::AnimRec(AnimId::Security_Claw_Upper_Rotating);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_10C_tlvInfo = tlvInfo;

    field_11C_clawX = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_120_clawY = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = 0;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = 1;
    }

    field_124 = 0; // LOBYTE

    mXPos = field_11C_clawX + ((Math_Sine_451110(0) * mSpriteScale) * FP_FromInteger(8)) * FP_FromDouble(0.25);
    mYPos = field_120_clawY + ((Math_Cosine_4510A0(0) * mSpriteScale) * FP_FromInteger(8));
    SetTint_418750(&kSecurityClawTints_4C5488[0], gMap.mCurrentLevel);

    field_134 = pTlv->field_10_top_left;
    field_138 = pTlv->field_14_bottom_right;

    field_118_alarm_switch_id = pTlv->field_1A_alarm_switch_id;
    field_11A_alarm_duration = pTlv->field_1C_alarm_duration;

    field_110_state = SecurityClawStates::eCamSwap_0;

    field_130_pClaw = ao_new<Claw>();
    if (field_130_pClaw)
    {
        field_130_pClaw->mSpriteScale = mSpriteScale;
        field_130_pClaw->mAnim.mRenderLayer = mSpriteScale == FP_FromInteger(1) ? Layer::eLayer_ZapLinesElum_28 : Layer::eLayer_ZapLinesElum_Half_9;

        field_130_pClaw->mXPos = field_11C_clawX;
        field_130_pClaw->mYPos = field_120_clawY;
        field_130_pClaw->SetTint_418750(&kClawTints_4C5498[0], gMap.mCurrentLevel);
    }

    mGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);
    field_13C_pArray = nullptr;
    field_128_sound_channels = 0;
}

SecurityClaw::~SecurityClaw()
{
    if (field_12C_pDetector)
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 1);
    }

    if (field_130_pClaw)
    {
        field_130_pClaw->mGameObjectFlags.Set(Options::eDead);
        field_130_pClaw = nullptr;
    }

    if (field_13C_pArray)
    {
        for (s32 i = 0; i < field_13C_pArray->Size(); i++)
        {
            auto pObjIter = field_13C_pArray->ItemAt(i);

            pObjIter->SetDontComeBack(field_12C_pDetector);
            pObjIter->field_C_refCount--;
            pObjIter->mGameObjectFlags.Set(Options::eDead);
        }

        relive_delete field_13C_pArray;
    }

    if (field_128_sound_channels)
    {
        SND_Stop_Channels_Mask_4774A0(field_128_sound_channels);
    }
}

void SecurityClaw::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}

s16 SecurityClaw::VTakeDamage(BaseGameObject* pFrom)
{
    if (!mGameObjectFlags.Get(BaseGameObject::eDead))
    {
        if (pFrom->mTypeId == Types::eAbilityRing_69 || pFrom->mTypeId == Types::eShrykull_85)
        {
            field_12C_pDetector = 0;
            mGameObjectFlags.Set(BaseGameObject::eDead);

            ao_new<Explosion>(
                mXPos,
                mYPos - mSpriteScale * FP_FromInteger(5),
                mSpriteScale);

            ao_new<Gibs>(
                GibType::Metal_5,
                mXPos,
                mYPos + FP_FromInteger(50),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mSpriteScale);
        }
        else
        {
            field_12C_pDetector = 0;

            ao_new<Gibs>(
                GibType::Metal_5,
                mXPos,
                mYPos + FP_FromInteger(50),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mSpriteScale);

            ao_new<Gibs>(
                GibType::Metal_5,
                mXPos,
                mYPos + FP_FromInteger(50),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mSpriteScale);

            mGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
    return 1;
}

void SecurityClaw::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!(gnFrameCount_507670 % 20))
    {
        if (field_128_sound_channels)
        {
            SND_Stop_Channels_Mask_4774A0(field_128_sound_channels);
        }
        field_128_sound_channels = SFX_Play_Pitch(SoundEffect::SecurityOrb_56, 55, -300, 0);
    }

    if (sActiveHero == sControlledCharacter)
    {
        if (sActiveHero->mXPos < field_11C_clawX)
        {
            if (FP_GetExponent(field_11C_clawX) > field_134.field_0_x)
            {
                field_11C_clawX -= FP_FromDouble(0.5);
            }
        }

        if (sActiveHero->mXPos >= field_11C_clawX)
        {
            if (FP_GetExponent(field_11C_clawX) < field_138.field_0_x)
            {
                field_11C_clawX += FP_FromDouble(0.5);
            }
        }
    }

    mXPos = field_11C_clawX + (((Math_Sine_451110(field_124) * mSpriteScale) * FP_FromInteger(8)) * FP_FromDouble(0.25));

    field_124 += 2;

    mYPos = field_120_clawY + ((Math_Cosine_4510A0(field_124) * mSpriteScale) * FP_FromInteger(8));

    field_130_pClaw->mXPos = mXPos;
    field_130_pClaw->mYPos = mYPos;

    if (field_13C_pArray)
    {
        for (s32 i = 0; i < field_13C_pArray->Size(); i++)
        {
            MotionDetector* pObj = field_13C_pArray->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            pObj->mXPos = mXPos - FP_FromInteger(1);
            pObj->mYPos = mYPos - FP_FromInteger(11);
        }
    }

    switch (field_110_state)
    {
        case SecurityClawStates::eCamSwap_0:
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->mTypeId == Types::eMotionDetector_59)
                {
                    auto pDetector = static_cast<MotionDetector*>(pObjIter);
                    if (!field_13C_pArray)
                    {
                        const AnimRecord& rec = AO::AnimRec(AnimId::Security_Claw_Upper_NoRotation);
                        mAnim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                        field_13C_pArray = ao_new<DynamicArrayT<MotionDetector>>(10);
                    }

                    pDetector->mXPos = mXPos - FP_FromInteger(1);
                    pDetector->mYPos = mYPos - FP_FromInteger(11);
                    pDetector->field_C_refCount++;
                    field_13C_pArray->Push_Back(pDetector);
                }
            }
            field_110_state = SecurityClawStates::eIdle_1;
            mGameObjectFlags.Clear(Options::eUpdateDuringCamSwap_Bit10);
            break;

        case SecurityClawStates::eIdle_1:
            if (Event_Get(kEventAbeOhm_8))
            {
                field_114_timer = gnFrameCount_507670 + 20;
                field_110_state = SecurityClawStates::eDoZapEffects_2;
                const AnimRecord& rec = AO::AnimRec(AnimId::Security_Claw_Lower_Open);
                field_130_pClaw->mAnim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                SFX_Play_Mono(SoundEffect::IndustrialNoise3_95, 60, 0);
                SFX_Play_Pitch(SoundEffect::IndustrialNoise3_95, 90, -1000, 0);
            }

            if (Event_Get(kEvent_2))
            {
                if (!alarmInstanceCount_5076A8)
                {
                    ao_new<Alarm>(field_11A_alarm_duration, field_118_alarm_switch_id, 30, Layer::eLayer_Above_FG1_39);
                }
            }
            break;

        case SecurityClawStates::eDoZapEffects_2:
            if (static_cast<s32>(gnFrameCount_507670) > field_114_timer)
            {
                PSX_RECT rect = {};
                sActiveHero->VGetBoundingRect(&rect, 1);
                const FP hero_mid_x = FP_FromInteger((rect.w + rect.x) / 2);
                const FP hero_mid_y = FP_FromInteger((rect.h + rect.y) / 2);

                ao_new<ScreenShake>(1);

                ao_new<ZapLine>(
                    mXPos - (FP_FromInteger(3) * mSpriteScale),
                    mYPos + (FP_FromInteger(5) * mSpriteScale),
                    hero_mid_x,
                    hero_mid_y,
                    8, ZapLineType::eThick_0,
                    Layer::eLayer_ZapLinesElum_28);

                ao_new<PossessionFlicker>(sActiveHero, 8, 255, 100, 100);

                sActiveHero->VTakeDamage(this);

                auto pSpark = ao_new<Sparks>(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * mSpriteScale),
                    mSpriteScale);
                if (pSpark)
                {
                    pSpark->mGreen = 65;
                    pSpark->mBlue = 65;
                    pSpark->mRed = 255;
                }

                auto pSpark2 = ao_new<Sparks>(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * mSpriteScale),
                    mSpriteScale);
                if (pSpark2)
                {
                    pSpark2->mGreen = 65;
                    pSpark2->mBlue = 65;
                    pSpark2->mRed = 255;
                }

                for (s32 i = 0; i < 9; i++)
                {
                    auto pSpark3 = ao_new<Sparks>(
                        hero_mid_x,
                        hero_mid_y,
                        mSpriteScale);
                    if (pSpark3)
                    {
                        pSpark3->mGreen = 65;
                        pSpark3->mBlue = 65;
                        pSpark3->mRed = 255;
                    }
                }

                field_110_state = SecurityClawStates::eAnimateClaw_DoFlashAndSound_3;
                field_114_timer = gnFrameCount_507670 + 8;
            }
            break;

        case SecurityClawStates::eAnimateClaw_DoFlashAndSound_3:
            if (static_cast<s32>(gnFrameCount_507670) == field_114_timer - 5 || static_cast<s32>(gnFrameCount_507670) == field_114_timer - 1)
            {
                ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 0, 0);
            }

            if (static_cast<s32>(gnFrameCount_507670) == field_114_timer - 4)
            {
                ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 0, 0, 1, TPageAbr::eBlend_1, 1);
            }

            if (field_114_timer - gnFrameCount_507670 == 4)
            {
                SFX_Play_Mono(SoundEffect::Zap1_57, 0, 0);
            }
            else if (field_114_timer - gnFrameCount_507670 == 1)
            {
                SFX_Play_Mono(SoundEffect::Zap2_58, 0, 0);
            }

            if (static_cast<s32>(gnFrameCount_507670) > field_114_timer)
            {
                field_110_state = SecurityClawStates::eIdle_1;
                const AnimRecord& rec = AO::AnimRec(AnimId::Security_Claw_Lower_Close);
                field_130_pClaw->mAnim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                SFX_Play_Mono(SoundEffect::IndustrialTrigger_97, 0, 0);
            }
            break;

        default:
            return;
    }
}

void SecurityClaw::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    // Empty
}

} // namespace AO
