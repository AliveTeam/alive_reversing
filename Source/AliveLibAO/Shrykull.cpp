#include "stdafx_ao.h"
#include "Function.hpp"
#include "Shrykull.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Abe.hpp"
#include "ZapLine.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Sfx.hpp"
#include "PossessionFlicker.hpp"
#include "ParticleBurst.hpp"
#include "../relive_lib/Flash.hpp"
#include "Electrocute.hpp"
#include "AbilityRing.hpp"
#include "Math.hpp"
#include "Game.hpp"

namespace AO {

Shrykull::~Shrykull()
{
    if (mZapLine)
    {
        mZapLine->mBaseGameObjectRefCount--;
        mZapLine->mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mZapTarget)
    {
        mZapTarget->mBaseGameObjectRefCount--;
    }
}

void Shrykull::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Shrykull::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ShrykullStart));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ShrykullTransform));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ShrykullDetransform));
}

Shrykull::Shrykull()
    : BaseAliveGameObject()
{
    SetCanExplode(true);
    SetType(ReliveTypes::eShrykull);
    
    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::ShrykullStart));
    mZapLine = nullptr;
    mZapTarget = nullptr;

    mXPos = sActiveHero->mXPos;
    mYPos = sActiveHero->mYPos;
    SetSpriteScale(sActiveHero->GetSpriteScale());
    SetScale(sActiveHero->GetScale());
    mState = State::eTransform_0;

    GetAnimation().mFlags.Set(AnimFlags::eFlipX, sActiveHero->GetAnimation().mFlags.Get(AnimFlags::eFlipX));

    CreateShadow();

    mResetRingTimer = false;
}

void Shrykull::VOnThrowableHit(BaseGameObject*)
{
    // Empty
}

bool Shrykull::CanKill(BaseAnimatedWithPhysicsGameObject* pObj)
{
    return (pObj->Type() == ReliveTypes::eTimedMine
            || pObj->Type() == ReliveTypes::eMine
            || pObj->Type() == ReliveTypes::eUXB
            || pObj->Type() == ReliveTypes::eSlig
            || pObj->Type() == ReliveTypes::eSlog
            || pObj->Type() == ReliveTypes::eBackgroundGlukkon
            || pObj->Type() == ReliveTypes::eSecurityClaw
            || pObj->Type() == ReliveTypes::eSecurityOrb)
        && pObj->GetAnimation().mFlags.Get(AnimFlags::eRender)
        && !pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead)
        && gMap.Is_Point_In_Current_Camera(
            pObj->mCurrentLevel,
            pObj->mCurrentPath,
            pObj->mXPos,
            pObj->mYPos,
            0);
}

bool Shrykull::CanElectrocute(BaseGameObject* pObj) const
{
    switch (pObj->Type())
    {
        case ReliveTypes::eElum:
        case ReliveTypes::eBackgroundGlukkon:
        case ReliveTypes::eRingOrLiftMud:
        case ReliveTypes::eParamite:
        case ReliveTypes::eMudokon:
        case ReliveTypes::eScrab:
        case ReliveTypes::eSlig:
        case ReliveTypes::eSlog:
            return true;
        default:
            return false;
    }
}

void Shrykull::VUpdate()
{
    switch (mState)
    {
        case State::eTransform_0:
            if (GetAnimation().GetCurrentFrame() == 0)
            {
                SFX_Play_Pitch(relive::SoundEffects::Shrykull1, 127, -2000);
                SFX_Play_Pitch(relive::SoundEffects::Shrykull2, 127, 0);
                SfxPlayMono(relive::SoundEffects::IngameTransition, 127);
            }

            if (GetAnimation().mFlags.Get(AnimFlags::eForwardLoopCompleted))
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::ShrykullTransform));
                mState = State::eZapTargets_1;
            }
            break;

        case State::eZapTargets_1:
            if (GetAnimation().GetCurrentFrame() == 0)
            {
                if (Math_NextRandom() >= 128u)
                {
                    SFX_Play_Pitch(relive::SoundEffects::Shrykull2, 127, 0);
                }
                else
                {
                    SFX_Play_Pitch(relive::SoundEffects::Shrykull1, 127, 0);
                }
            }

            for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
            {
                IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (CanKill(pObj) && !pObj->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eZappedByShrykull))
                {
                    pObj->mBaseGameObjectRefCount++;
                    mZapTarget = pObj;

                    const PSX_RECT objRect = pObj->VGetBoundingRect();
                    const PSX_RECT ourRect = VGetBoundingRect();
                    
                    if (mZapLine)
                    {
                        mZapLine->CalculateSourceAndDestinationPositions(
                            FP_FromInteger((ourRect.x + ourRect.w) / 2),
                            FP_FromInteger((ourRect.y + ourRect.h) / 2),
                            FP_FromInteger((objRect.x + objRect.w) / 2),
                            FP_FromInteger((objRect.y + objRect.h) / 2));
                    }
                    else
                    {
                        auto pZapLine = relive_new ZapLine(
                            FP_FromInteger((ourRect.x + ourRect.w) / 2),
                            FP_FromInteger((ourRect.y + ourRect.h) / 2),
                            FP_FromInteger((objRect.x + objRect.w) / 2),
                            FP_FromInteger((objRect.y + objRect.h) / 2),
                            0, ZapLineType::eThin_1,
                            Layer::eLayer_ZapLinesElumMuds_28);
                        if (pZapLine)
                        {
                            pZapLine->mBaseGameObjectRefCount++;
                            mZapLine = pZapLine;
                        }
                    }

                    mCanElectrocute = CanElectrocute(pObj);
                    if (mCanElectrocute)
                    {
                        relive_new Electrocute(pObj, 0);
                        mFlashTimer = sGnFrame + 3;

                        if (pObj->Type() == ReliveTypes::eBackgroundGlukkon)
                        {
                            pObj->VTakeDamage(this);
                        }
                    }

                    relive_new PossessionFlicker(mZapTarget, 8, 255, 255, 255);

                    relive_new AbilityRing(
                        FP_FromInteger((objRect.x + objRect.w) / 2),
                        FP_FromInteger((objRect.y + objRect.h) / 2),
                        RingTypes::eShrykull_Pulse_Large_5);

                    relive_new PossessionFlicker(this, 8, 255, 255, 255);

                    relive_new AbilityRing(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        RingTypes::eShrykull_Pulse_Large_5);

                    mZapTarget->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eZappedByShrykull);

                    SFX_Play_Pitch(relive::SoundEffects::Respawn, 100, 2000);
                    SfxPlayMono(relive::SoundEffects::Zap1, 0);

                    mState = State::eKillTargets_4;
                    mZapIntervalTimer = sGnFrame + 12;
                    mResetRingTimer = true;
                    return;
                }
            }

            if (mZapLine)
            {
                mZapLine->mBaseGameObjectRefCount--;
                mZapLine->mBaseGameObjectFlags.Set(Options::eDead);
                mZapLine = nullptr;
            }
            mState = State::eDetransform_2;
            break;

        case State::eDetransform_2:
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::ShrykullDetransform));
                mState = State::eFinish_3;
            }
            break;

        case State::eFinish_3:
            if (GetAnimation().GetCurrentFrame() == 0)
            {
                SFX_Play_Pitch(relive::SoundEffects::Shrykull1, 127, -2000);
                SFX_Play_Pitch(relive::SoundEffects::Shrykull2, 127, 0);
            }

            if (GetAnimation().mFlags.Get(AnimFlags::eForwardLoopCompleted))
            {
                sActiveHero->ExitShrykull_42F440(mResetRingTimer);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        case State::eKillTargets_4:
            if (GetAnimation().GetCurrentFrame() == 0)
            {
                if (Math_NextRandom() >= 128u)
                {
                    SFX_Play_Pitch(relive::SoundEffects::Shrykull2, 127, 0);
                }
                else
                {
                    SFX_Play_Pitch(relive::SoundEffects::Shrykull1, 127, 0);
                }
            }

            if (mZapTarget)
            {
                if (mZapTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    mZapTarget->mBaseGameObjectRefCount--;
                    mZapTarget = nullptr;
                }
                else
                {
                    const PSX_RECT zapRect = mZapTarget->VGetBoundingRect();
                    const PSX_RECT ourRect = VGetBoundingRect();
                    if (static_cast<s32>(sGnFrame) == mFlashTimer)
                    {
                        relive_new ParticleBurst(
                            FP_FromInteger((zapRect.x + zapRect.w) / 2),
                            FP_FromInteger((zapRect.y + zapRect.h) / 2),
                            20,
                            GetSpriteScale(),
                            BurstType::eBigPurpleSparks_2);

                       relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
                    }
                    mZapLine->CalculateSourceAndDestinationPositions(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        FP_FromInteger((zapRect.x + zapRect.w) / 2),
                        FP_FromInteger((zapRect.y + zapRect.h) / 2));
                }
            }

            if (static_cast<s32>(sGnFrame) > mZapIntervalTimer)
            {
                mState = State::eZapTargets_1;

                if (mZapTarget)
                {
                    if (!mCanElectrocute)
                    {
                        mZapTarget->VTakeDamage(this);
                    }

                    mZapTarget->mBaseGameObjectRefCount--;
                    mZapTarget = nullptr;
                }
            }
            break;

        default:
            return;
    }
}

} // namespace AO
