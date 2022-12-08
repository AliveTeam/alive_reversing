#include "stdafx.h"
#include "Shrykull.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "ParticleBurst.hpp"
#include "../relive_lib/Flash.hpp"
#include "PossessionFlicker.hpp"
#include "AbilityRing.hpp"
#include "Electrocute.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Math.hpp"

void Shrykull::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ShrykullStart));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ShrykullTransform));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ShrykullDetransform));
}

Shrykull::Shrykull()
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eShrykull);

    LoadAnimations();

    SetCanExplode(true);

    mZapTargetId = Guid{};
    mZapLineId = Guid{};

    Animation_Init(GetAnimRes(AnimId::ShrykullStart));

    mXPos = sActiveHero->mXPos;
    mYPos = sActiveHero->mYPos;
    SetSpriteScale(sActiveHero->GetSpriteScale());
    SetScale(sActiveHero->GetScale());

    mState = State::eTransform_0;

    GetAnimation().mFlags.Set(AnimFlags::eFlipX, sActiveHero->GetAnimation().mFlags.Get(AnimFlags::eFlipX));

    CreateShadow();

    mResetRingTimer = false;
}

Shrykull::~Shrykull()
{
    BaseGameObject* pZapLine = sObjectIds.Find_Impl(mZapLineId);
    if (pZapLine)
    {
        pZapLine->SetDead(true);
        mZapLineId = Guid{};
    }

    mZapTargetId = Guid{};
}


void Shrykull::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

bool Shrykull::CanElectrocute(BaseGameObject* pObj)
{
    switch (pObj->Type())
    {
        case ReliveTypes::eCrawlingSlig:
        case ReliveTypes::eFlyingSlig:
        case ReliveTypes::eGlukkon:
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

bool Shrykull::CanKill(BaseAnimatedWithPhysicsGameObject* pObj)
{
    return (
               pObj->Type() == ReliveTypes::eTimedMine_or_MovingBomb || pObj->Type() == ReliveTypes::eMine || pObj->Type() == ReliveTypes::eUXB || pObj->Type() == ReliveTypes::eSlig || pObj->Type() == ReliveTypes::eFlyingSlig || pObj->Type() == ReliveTypes::eCrawlingSlig || pObj->Type() == ReliveTypes::eSlog || pObj->Type() == ReliveTypes::eGlukkon || pObj->Type() == ReliveTypes::eSecurityClaw || pObj->Type() == ReliveTypes::eSecurityOrb)
        && pObj->GetAnimation().mFlags.Get(AnimFlags::eRender) && !pObj->GetDead() && gMap.Is_Point_In_Current_Camera(pObj->mCurrentLevel, pObj->mCurrentPath, pObj->mXPos, pObj->mYPos, 0);
}

void Shrykull::VUpdate()
{
    auto pExistingBeingZappedObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mZapTargetId));
    auto pExistingZapLine = static_cast<ZapLine*>(sObjectIds.Find_Impl(mZapLineId));

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
                    mZapTargetId = pObj->mBaseGameObjectId;

                    const PSX_RECT objRect = pObj->VGetBoundingRect();
                    const PSX_RECT ourRect = VGetBoundingRect();

                    if (pExistingZapLine)
                    {
                        pExistingZapLine->CalculateSourceAndDestinationPositions(
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
                            mZapLineId = pZapLine->mBaseGameObjectId;
                        }
                    }

                    mCanElectrocute = CanElectrocute(pObj);
                    if (mCanElectrocute)
                    {
                        relive_new Electrocute(pObj, 0, 1);
                        mFlashTimer = sGnFrame + 3;

                        if (pObj->Type() == ReliveTypes::eGlukkon)
                        {
                            pObj->VTakeDamage(this);
                        }
                    }

                    relive_new PossessionFlicker(pObj, 8, 255, 255, 255);
                    AbilityRing::Factory(
                        FP_FromInteger((objRect.x + objRect.w) / 2),
                        FP_FromInteger((objRect.y + objRect.h) / 2),
                        RingTypes::eShrykull_Pulse_Large_5, pObj->GetSpriteScale());

                    relive_new PossessionFlicker(this, 8, 255, 255, 255);
                    AbilityRing::Factory(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        RingTypes::eShrykull_Pulse_Large_5, GetSpriteScale());

                    pObj->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eZappedByShrykull);

                    SFX_Play_Pitch(relive::SoundEffects::ShrykullZap, 100, 2000);
                    SfxPlayMono(relive::SoundEffects::Zap1, 0);

                    mState = State::eKillTargets_4;
                    mZapIntervalTimer = sGnFrame + 12;
                    mResetRingTimer = true;
                    return;
                }
            }

            if (pExistingZapLine)
            {
                pExistingZapLine->SetDead(true);
                mZapLineId = Guid{};
            }
            mState = State::eDetransform_2;
            break;

        case State::eDetransform_2:
            if (GetAnimation().GetCurrentFrame() == 7)
            {
                if (Math_NextRandom() >= 0x80u)
                {
                    SFX_Play_Pitch(relive::SoundEffects::Shrykull2, 127, -512);
                }
                else
                {
                    SFX_Play_Pitch(relive::SoundEffects::Shrykull1, 127, -512);
                }
            }
            else if (GetAnimation().GetCurrentFrame() == 20)
            {
                SFX_Play_Pitch(relive::SoundEffects::Shrykull1, 127, -2000);
                SFX_Play_Pitch(relive::SoundEffects::Shrykull2, 127, 0);
            }

            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::ShrykullDetransform));
                mState = State::eFinish_3;
            }
            break;

        case State::eFinish_3:
            if (GetAnimation().mFlags.Get(AnimFlags::eForwardLoopCompleted))
            {
                sActiveHero->ExitShrykull_45A9D0(mResetRingTimer);
                SetDead(true);
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

            if (pExistingBeingZappedObj)
            {
                if (pExistingBeingZappedObj->GetDead())
                {
                    mZapTargetId = Guid{};
                }
                else
                {
                    const PSX_RECT zapRect = pExistingBeingZappedObj->VGetBoundingRect();
                    const PSX_RECT ourRect = VGetBoundingRect();

                    if (static_cast<s32>(sGnFrame) == mFlashTimer)
                    {
                        relive_new ParticleBurst(
                            FP_FromInteger((zapRect.x + zapRect.w) / 2),
                            FP_FromInteger((zapRect.y + zapRect.h) / 2),
                            20, GetSpriteScale(), BurstType::eBigPurpleSparks_2, 13);

                        relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, TPageAbr::eBlend_3, 1);
                    }
                    pExistingZapLine->CalculateSourceAndDestinationPositions(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        FP_FromInteger((zapRect.x + zapRect.w) / 2),
                        FP_FromInteger((zapRect.y + zapRect.h) / 2));
                }
            }

            if (static_cast<s32>(sGnFrame) > mZapIntervalTimer)
            {
                mState = State::eZapTargets_1;
                if (pExistingBeingZappedObj)
                {
                    if (!mCanElectrocute)
                    {
                        pExistingBeingZappedObj->VTakeDamage(this);
                    }
                    mZapTargetId = Guid{};
                }
            }
            break;

        default:
            return;
    }
}
