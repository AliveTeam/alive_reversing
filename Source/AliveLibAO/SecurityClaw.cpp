#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityClaw.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Midi.hpp"
#include "MotionDetector.hpp"
#include "Gibs.hpp"
#include "Explosion.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "ZapSpark.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "Flash.hpp"
#include "../relive_lib/Events.hpp"
#include "Alarm.hpp"
#include "ScreenShake.hpp"
#include "PossessionFlicker.hpp"

namespace AO {

const TintEntry kSecurityClawTints_4C5488[] = {
    {EReliveLevelIds::eStockYards, 105u, 55u, 55u},
    {EReliveLevelIds::eStockYardsReturn, 105u, 55u, 55u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u},
};

const TintEntry kClawTints_4C5498[3] = {
    {EReliveLevelIds::eStockYards, 80u, 55u, 55u},
    {EReliveLevelIds::eStockYardsReturn, 80u, 55u, 55u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

Claw::Claw()
{
    mBaseGameObjectTypeId = ReliveTypes::eClawOrBirdPortalTerminator;
    
    const AnimRecord rec = AO::AnimRec(AnimId::Security_Claw_Lower_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Security_Claw_Lower_Idle, ppRes);
}


void Claw::VScreenChanged()
{
    // Keep alive as the Claw is owned by the SecurityClaw
}

SecurityClaw::SecurityClaw(relive::Path_SecurityClaw* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eSecurityClaw;

    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    field_12C_pDetector = 1;

    const AnimRecord rec = AO::AnimRec(AnimId::Security_Claw_Upper_Rotating);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Security_Claw_Upper_Rotating, ppRes);

    mTlvInfo = tlvId;

    mClawX = FP_FromInteger(pTlv->mTopLeftX);
    mClawY = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    field_124 = 0; // LOBYTE

    mXPos = mClawX + ((Math_Sine_451110(0) * mSpriteScale) * FP_FromInteger(8)) * FP_FromDouble(0.25);
    mYPos = mClawY + ((Math_Cosine_4510A0(0) * mSpriteScale) * FP_FromInteger(8));
    SetTint(&kSecurityClawTints_4C5488[0], gMap.mCurrentLevel);

    field_134_top_left.x = pTlv->mTopLeftX;
    field_134_top_left.y = pTlv->mTopLeftY;
    field_138_bottom_right.x = pTlv->mBottomRightX;
    field_138_bottom_right.y = pTlv->mBottomRightY;

    mAlarmSwitchId = pTlv->mAlarmSwitchId;
    mAlarmDuration = pTlv->mAlarmDuration;

    field_110_state = SecurityClawStates::eCamSwap_0;

    mClaw = relive_new Claw();
    if (mClaw)
    {
        mClaw->mSpriteScale = mSpriteScale;
        mClaw->mAnim.mRenderLayer = mSpriteScale == FP_FromInteger(1) ? Layer::eLayer_ZapLinesElumMuds_28 : Layer::eLayer_ZapLinesMudsElum_Half_9;

        mClaw->mXPos = mClawX;
        mClaw->mYPos = mClawY;
        mClaw->SetTint(&kClawTints_4C5498[0], gMap.mCurrentLevel);
    }

    mBaseGameObjectFlags.Set(Options::eUpdateDuringCamSwap_Bit10);
    field_13C_pArray = nullptr;
    field_128_sound_channels = 0;
}

SecurityClaw::~SecurityClaw()
{
    if (field_12C_pDetector)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }

    if (mClaw)
    {
        mClaw->mBaseGameObjectFlags.Set(Options::eDead);
        mClaw = nullptr;
    }

    if (field_13C_pArray)
    {
        for (s32 i = 0; i < field_13C_pArray->Size(); i++)
        {
            auto pObjIter = field_13C_pArray->ItemAt(i);

            pObjIter->SetDontComeBack(field_12C_pDetector);
            pObjIter->mBaseGameObjectRefCount--;
            pObjIter->mBaseGameObjectFlags.Set(Options::eDead);
        }

        relive_delete field_13C_pArray;
    }

    if (field_128_sound_channels)
    {
        SND_Stop_Channels_Mask(field_128_sound_channels);
    }
}

void SecurityClaw::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

s16 SecurityClaw::VTakeDamage(BaseGameObject* pFrom)
{
    if (!mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        if (pFrom->mBaseGameObjectTypeId == ReliveTypes::eAbilityRing || pFrom->mBaseGameObjectTypeId == ReliveTypes::eShrykull)
        {
            field_12C_pDetector = 0;
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);

            relive_new Explosion(
                mXPos,
                mYPos - mSpriteScale * FP_FromInteger(5),
                mSpriteScale);

            relive_new Gibs(
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

            relive_new Gibs(
                GibType::Metal_5,
                mXPos,
                mYPos + FP_FromInteger(50),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mSpriteScale);

            relive_new Gibs(
                GibType::Metal_5,
                mXPos,
                mYPos + FP_FromInteger(50),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mSpriteScale);

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
    return 1;
}

void SecurityClaw::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!(sGnFrame % 20))
    {
        if (field_128_sound_channels)
        {
            SND_Stop_Channels_Mask(field_128_sound_channels);
        }
        field_128_sound_channels = SFX_Play_Pitch(SoundEffect::SecurityOrb_56, 55, -300, 0);
    }

    if (sActiveHero == sControlledCharacter)
    {
        if (sActiveHero->mXPos < mClawX)
        {
            if (FP_GetExponent(mClawX) > field_134_top_left.x)
            {
                mClawX -= FP_FromDouble(0.5);
            }
        }

        if (sActiveHero->mXPos >= mClawX)
        {
            if (FP_GetExponent(mClawX) < field_138_bottom_right.x)
            {
                mClawX += FP_FromDouble(0.5);
            }
        }
    }

    mXPos = mClawX + (((Math_Sine_451110(field_124) * mSpriteScale) * FP_FromInteger(8)) * FP_FromDouble(0.25));

    field_124 += 2;

    mYPos = mClawY + ((Math_Cosine_4510A0(field_124) * mSpriteScale) * FP_FromInteger(8));

    mClaw->mXPos = mXPos;
    mClaw->mYPos = mYPos;

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

                if (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eMotionDetector)
                {
                    auto pDetector = static_cast<MotionDetector*>(pObjIter);
                    if (!field_13C_pArray)
                    {
                        mAnim.Set_Animation_Data(AnimId::Security_Claw_Upper_NoRotation, nullptr);
                        field_13C_pArray = relive_new DynamicArrayT<MotionDetector>(10);
                    }

                    pDetector->mXPos = mXPos - FP_FromInteger(1);
                    pDetector->mYPos = mYPos - FP_FromInteger(11);
                    pDetector->mBaseGameObjectRefCount++;
                    field_13C_pArray->Push_Back(pDetector);
                }
            }
            field_110_state = SecurityClawStates::eIdle_1;
            mBaseGameObjectFlags.Clear(Options::eUpdateDuringCamSwap_Bit10);
            break;

        case SecurityClawStates::eIdle_1:
            if (EventGet(kEventAbeOhm))
            {
                field_114_timer = sGnFrame + 20;
                field_110_state = SecurityClawStates::eDoZapEffects_2;
                mClaw->mAnim.Set_Animation_Data(AnimId::Security_Claw_Lower_Open, nullptr);
                SfxPlayMono(SoundEffect::IndustrialNoise3_95, 60, 0);
                SFX_Play_Pitch(SoundEffect::IndustrialNoise3_95, 90, -1000, 0);
            }

            if (EventGet(kEventShooting))
            {
                if (!alarmInstanceCount_5076A8)
                {
                    relive_new Alarm(mAlarmDuration, mAlarmSwitchId, 30, Layer::eLayer_Above_FG1_39);
                }
            }
            break;

        case SecurityClawStates::eDoZapEffects_2:
            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                const PSX_RECT rect = sActiveHero->VGetBoundingRect();
                const FP hero_mid_x = FP_FromInteger((rect.w + rect.x) / 2);
                const FP hero_mid_y = FP_FromInteger((rect.h + rect.y) / 2);

                relive_new ScreenShake(1);

                relive_new ZapLine(
                    mXPos - (FP_FromInteger(3) * mSpriteScale),
                    mYPos + (FP_FromInteger(5) * mSpriteScale),
                    hero_mid_x,
                    hero_mid_y,
                    8, ZapLineType::eThick_0,
                    Layer::eLayer_ZapLinesElumMuds_28);

                relive_new PossessionFlicker(sActiveHero, 8, 255, 100, 100);

                sActiveHero->VTakeDamage(this);

                auto pSpark = relive_new ZapSpark(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * mSpriteScale),
                    mSpriteScale);
                if (pSpark)
                {
                    pSpark->mRGB.SetRGB(255, 65, 65);
                }

                auto pSpark2 = relive_new ZapSpark(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * mSpriteScale),
                    mSpriteScale);
                if (pSpark2)
                {
                    pSpark2->mRGB.SetRGB(255, 65, 65);
                }

                for (s32 i = 0; i < 9; i++)
                {
                    auto pSpark3 = relive_new ZapSpark(
                        hero_mid_x,
                        hero_mid_y,
                        mSpriteScale);
                    if (pSpark3)
                    {
                        pSpark3->mRGB.SetRGB(255, 65, 65);
                    }
                }

                field_110_state = SecurityClawStates::eAnimateClaw_DoFlashAndSound_3;
                field_114_timer = sGnFrame + 8;
            }
            break;

        case SecurityClawStates::eAnimateClaw_DoFlashAndSound_3:
            if (static_cast<s32>(sGnFrame) == field_114_timer - 5 || static_cast<s32>(sGnFrame) == field_114_timer - 1)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 0);
            }

            if (static_cast<s32>(sGnFrame) == field_114_timer - 4)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 0, TPageAbr::eBlend_1, 1);
            }

            if (field_114_timer - sGnFrame == 4)
            {
                SfxPlayMono(SoundEffect::Zap1_57, 0, 0);
            }
            else if (field_114_timer - sGnFrame == 1)
            {
                SfxPlayMono(SoundEffect::Zap2_58, 0, 0);
            }

            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                field_110_state = SecurityClawStates::eIdle_1;
                mClaw->mAnim.Set_Animation_Data(AnimId::Security_Claw_Lower_Close, nullptr);
                SfxPlayMono(SoundEffect::IndustrialTrigger_97, 0, 0);
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
