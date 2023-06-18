#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "SecurityClaw.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Midi.hpp"
#include "MotionDetector.hpp"
#include "Gibs.hpp"
#include "AirExplosion.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "ZapSpark.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Alarm.hpp"
#include "ScreenShake.hpp"
#include "PossessionFlicker.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

static const TintEntry kSecurityClawTints[] = {
    {EReliveLevelIds::eStockYards, 105u, 55u, 55u},
    {EReliveLevelIds::eStockYardsReturn, 105u, 55u, 55u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u},
};

static const TintEntry kClawTints[3] = {
    {EReliveLevelIds::eStockYards, 80u, 55u, 55u},
    {EReliveLevelIds::eStockYardsReturn, 80u, 55u, 55u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

Claw::Claw()
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eClawOrBirdPortalTerminator);
    
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Claw_Lower_Idle));
    Animation_Init(GetAnimRes(AnimId::Security_Claw_Lower_Idle));
}


void Claw::VScreenChanged()
{
    // Keep alive as the Claw is owned by the SecurityClaw
}

void SecurityClaw::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Claw_Upper_Rotating));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Claw_Upper_NoRotation));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Claw_Lower_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Claw_Lower_Close));
}

SecurityClaw::SecurityClaw(relive::Path_SecurityClaw* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eSecurityClaw);

    LoadAnimations();

    SetCanExplode(true);
    mDetectorComeBack = true;

    Animation_Init(GetAnimRes(AnimId::Security_Claw_Upper_Rotating));

    mTlvInfo = tlvId;

    mClawX = FP_FromInteger(pTlv->mTopLeftX);
    mClawY = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    mAngle = 0;

    mXPos = mClawX + ((Math_Sine(0) * GetSpriteScale()) * FP_FromInteger(8)) * FP_FromDouble(0.25);
    mYPos = mClawY + ((Math_Cosine(0) * GetSpriteScale()) * FP_FromInteger(8));
    SetTint(&kSecurityClawTints[0], gMap.mCurrentLevel);

    mTlvTopLeft.x = pTlv->mTopLeftX;
    mTlvTopLeft.y = pTlv->mTopLeftY;
    mTlvBottomRight.x = pTlv->mBottomRightX;
    mTlvBottomRight.y = pTlv->mBottomRightY;

    mAlarmSwitchId = pTlv->mAlarmSwitchId;
    mAlarmDuration = pTlv->mAlarmDuration;

    mState = SecurityClawStates::eCamSwap_0;

    auto pClaw = relive_new Claw();
    if (pClaw)
    {
        pClaw->SetSpriteScale(GetSpriteScale());
        pClaw->GetAnimation().SetRenderLayer(GetSpriteScale() == FP_FromInteger(1) ? Layer::eLayer_ZapLinesElumMuds_28 : Layer::eLayer_ZapLinesMudsElum_Half_9);

        pClaw->mXPos = mClawX;
        pClaw->mYPos = mClawY;
        pClaw->SetTint(&kClawTints[0], gMap.mCurrentLevel);
        mClawId = pClaw->mBaseGameObjectId;
    }

    SetUpdateDuringCamSwap(true);
    mMotionDetectorArray = nullptr;
    mOrbSoundChannels = 0;
}

SecurityClaw::~SecurityClaw()
{
    if (mDetectorComeBack)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }

    BaseGameObject* pClaw = sObjectIds.Find_Impl(mClawId);
    if (pClaw)
    {
        pClaw->SetDead(true);
        mClawId = {};
    }

    if (mMotionDetectorArray)
    {
        for (s32 i = 0; i < mMotionDetectorArray->Size(); i++)
        {
            auto pObjIter = mMotionDetectorArray->ItemAt(i);

            pObjIter->SetDontComeBack(mDetectorComeBack);
            pObjIter->mBaseGameObjectRefCount--;
            pObjIter->SetDead(true);
        }

        relive_delete mMotionDetectorArray;
    }

    if (mOrbSoundChannels)
    {
        SND_Stop_Channels_Mask(mOrbSoundChannels);
    }
}

void SecurityClaw::VScreenChanged()
{
    SetDead(true);
}

bool SecurityClaw::VTakeDamage(BaseGameObject* pFrom)
{
    if (!GetDead())
    {
        if (pFrom->Type() == ReliveTypes::eAbilityRing || pFrom->Type() == ReliveTypes::eShrykull)
        {
            mDetectorComeBack = false;
            SetDead(true);

            relive_new AirExplosion(
                mXPos,
                mYPos - GetSpriteScale() * FP_FromInteger(5),
                GetSpriteScale());

            relive_new Gibs(
                GibType::Metal_5,
                mXPos,
                mYPos + FP_FromInteger(50),
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale());
        }
        else
        {
            mDetectorComeBack = false;

            relive_new Gibs(
                GibType::Metal_5,
                mXPos,
                mYPos + FP_FromInteger(50),
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale());

            relive_new Gibs(
                GibType::Metal_5,
                mXPos,
                mYPos + FP_FromInteger(50),
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale());

            SetDead(true);
        }
    }
    return true;
}

void SecurityClaw::VUpdate()
{
    auto pClaw = static_cast<Claw*>(sObjectIds.Find_Impl(mClawId));

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!(sGnFrame % 20))
    {
        if (mOrbSoundChannels)
        {
            SND_Stop_Channels_Mask(mOrbSoundChannels);
        }
        mOrbSoundChannels = SFX_Play_Pitch(relive::SoundEffects::SecurityOrb, 55, -300);
    }

    if (sActiveHero == sControlledCharacter)
    {
        if (sActiveHero->mXPos < mClawX)
        {
            if (FP_GetExponent(mClawX) > mTlvTopLeft.x)
            {
                mClawX -= FP_FromDouble(0.5);
            }
        }

        if (sActiveHero->mXPos >= mClawX)
        {
            if (FP_GetExponent(mClawX) < mTlvBottomRight.x)
            {
                mClawX += FP_FromDouble(0.5);
            }
        }
    }

    mXPos = mClawX + (((Math_Sine(mAngle) * GetSpriteScale()) * FP_FromInteger(8)) * FP_FromDouble(0.25));

    mAngle += 2;

    mYPos = mClawY + ((Math_Cosine(mAngle) * GetSpriteScale()) * FP_FromInteger(8));

    pClaw->mXPos = mXPos;
    pClaw->mYPos = mYPos;

    if (mMotionDetectorArray)
    {
        for (s32 i = 0; i < mMotionDetectorArray->Size(); i++)
        {
            MotionDetector* pObj = mMotionDetectorArray->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            pObj->mXPos = mXPos - FP_FromInteger(1);
            pObj->mYPos = mYPos - FP_FromInteger(11);
        }
    }

    switch (mState)
    {
        case SecurityClawStates::eCamSwap_0:
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->Type() == ReliveTypes::eMotionDetector)
                {
                    auto pDetector = static_cast<MotionDetector*>(pObjIter);
                    if (!mMotionDetectorArray)
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Claw_Upper_NoRotation));
                        mMotionDetectorArray = relive_new DynamicArrayT<MotionDetector>(10);
                    }

                    pDetector->mXPos = mXPos - FP_FromInteger(1);
                    pDetector->mYPos = mYPos - FP_FromInteger(11);
                    pDetector->mBaseGameObjectRefCount++;
                    mMotionDetectorArray->Push_Back(pDetector);
                }
            }
            mState = SecurityClawStates::eIdle_1;
            SetUpdateDuringCamSwap(false);
            break;

        case SecurityClawStates::eIdle_1:
            if (EventGet(kEventAbeOhm))
            {
                mTimer = MakeTimer(20);
                mState = SecurityClawStates::eDoZapEffects_2;
                pClaw->GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Claw_Lower_Open));
                SfxPlayMono(relive::SoundEffects::IndustrialNoise3, 60);
                SFX_Play_Pitch(relive::SoundEffects::IndustrialNoise3, 90, -1000);
            }

            if (EventGet(kEventShooting))
            {
                if (!gAlarmInstanceCount)
                {
                    relive_new Alarm(mAlarmDuration, mAlarmSwitchId, 30, Layer::eLayer_Above_FG1_39);
                }
            }
            break;

        case SecurityClawStates::eDoZapEffects_2:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                const PSX_RECT rect = sActiveHero->VGetBoundingRect();
                const FP hero_mid_x = FP_FromInteger((rect.w + rect.x) / 2);
                const FP hero_mid_y = FP_FromInteger((rect.h + rect.y) / 2);

                relive_new ScreenShake(1);

                relive_new ZapLine(
                    mXPos - (FP_FromInteger(3) * GetSpriteScale()),
                    mYPos + (FP_FromInteger(5) * GetSpriteScale()),
                    hero_mid_x,
                    hero_mid_y,
                    8, ZapLineType::eThick_0,
                    Layer::eLayer_ZapLinesElumMuds_28);

                relive_new PossessionFlicker(sActiveHero, 8, 255, 100, 100);

                sActiveHero->VTakeDamage(this);

                auto pSpark = relive_new ZapSpark(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * GetSpriteScale()),
                    GetSpriteScale());
                if (pSpark)
                {
                    pSpark->mRGB.SetRGB(255, 65, 65);
                }

                auto pSpark2 = relive_new ZapSpark(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * GetSpriteScale()),
                    GetSpriteScale());
                if (pSpark2)
                {
                    pSpark2->mRGB.SetRGB(255, 65, 65);
                }

                for (s32 i = 0; i < 9; i++)
                {
                    auto pSpark3 = relive_new ZapSpark(
                        hero_mid_x,
                        hero_mid_y,
                        GetSpriteScale());
                    if (pSpark3)
                    {
                        pSpark3->mRGB.SetRGB(255, 65, 65);
                    }
                }

                mState = SecurityClawStates::eAnimateClaw_DoFlashAndSound_3;
                mTimer = MakeTimer(8);
            }
            break;

        case SecurityClawStates::eAnimateClaw_DoFlashAndSound_3:
            if (static_cast<s32>(sGnFrame) == mTimer - 5 || static_cast<s32>(sGnFrame) == mTimer - 1)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 0);
            }

            if (static_cast<s32>(sGnFrame) == mTimer - 4)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 0, TPageAbr::eBlend_1, 1);
            }

            if (mTimer - sGnFrame == 4)
            {
                SfxPlayMono(relive::SoundEffects::Zap1, 0);
            }
            else if (mTimer - sGnFrame == 1)
            {
                SfxPlayMono(relive::SoundEffects::Zap2, 0);
            }

            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = SecurityClawStates::eIdle_1;
                pClaw->GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Security_Claw_Lower_Close));
                SfxPlayMono(relive::SoundEffects::IndustrialTrigger, 0);
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
