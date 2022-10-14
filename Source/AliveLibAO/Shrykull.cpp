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
#include "Flash.hpp"
#include "Electrocute.hpp"
#include "AbilityRing.hpp"
#include "Math.hpp"
#include "Game.hpp"

namespace AO {

Shrykull::~Shrykull()
{
    if (field_118_zap_line)
    {
        field_118_zap_line->mBaseGameObjectRefCount--;
        field_118_zap_line->mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (field_11C_obj_being_zapped)
    {
        field_11C_obj_being_zapped->mBaseGameObjectRefCount--;
    }
}

void Shrykull::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mOverlayId != gMap.GetOverlayId())
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
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    SetType(ReliveTypes::eShrykull);
    
    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::ShrykullStart));
    field_118_zap_line = nullptr;
    field_11C_obj_being_zapped = nullptr;

    mXPos = sActiveHero->mXPos;
    mYPos = sActiveHero->mYPos;
    mSpriteScale = sActiveHero->mSpriteScale;
    mScale = sActiveHero->mScale;
    field_10C_state = State::eTransform_0;

    GetAnimation().mFlags.Set(AnimFlags::eFlipX, sActiveHero->GetAnimation().mFlags.Get(AnimFlags::eFlipX));

    mShadow = relive_new Shadow();

    field_122_bResetRingTimer = 0;
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
    switch (field_10C_state)
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
                field_10C_state = State::eZapTargets_1;
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
                BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (CanKill(pObj) && !pObj->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit3))
                {
                    pObj->mBaseGameObjectRefCount++;
                    field_11C_obj_being_zapped = pObj;

                    const PSX_RECT objRect = pObj->VGetBoundingRect();
                    const PSX_RECT ourRect = VGetBoundingRect();
                    
                    if (field_118_zap_line)
                    {
                        field_118_zap_line->CalculateSourceAndDestinationPositions(
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
                            field_118_zap_line = pZapLine;
                        }
                    }

                    field_120_bElectrocute = CanElectrocute(pObj);
                    if (field_120_bElectrocute)
                    {
                        relive_new Electrocute(pObj, 0);
                        field_114_timer = sGnFrame + 3;

                        if (pObj->Type() == ReliveTypes::eBackgroundGlukkon)
                        {
                            pObj->VTakeDamage(this);
                        }
                    }

                    relive_new PossessionFlicker(field_11C_obj_being_zapped, 8, 255, 255, 255);

                    relive_new AbilityRing(
                        FP_FromInteger((objRect.x + objRect.w) / 2),
                        FP_FromInteger((objRect.y + objRect.h) / 2),
                        RingTypes::eShrykull_Pulse_Large_5);

                    relive_new PossessionFlicker(this, 8, 255, 255, 255);

                    relive_new AbilityRing(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        RingTypes::eShrykull_Pulse_Large_5);

                    field_11C_obj_being_zapped->mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit3);

                    SFX_Play_Pitch(relive::SoundEffects::Respawn, 100, 2000);
                    SfxPlayMono(relive::SoundEffects::Zap1, 0);

                    field_10C_state = State::eKillTargets_4;
                    field_110_timer = sGnFrame + 12;
                    field_122_bResetRingTimer = 1;
                    return;
                }
            }

            if (field_118_zap_line)
            {
                field_118_zap_line->mBaseGameObjectRefCount--;
                field_118_zap_line->mBaseGameObjectFlags.Set(Options::eDead);
                field_118_zap_line = nullptr;
            }
            field_10C_state = State::eDetransform_2;
            break;

        case State::eDetransform_2:
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::ShrykullDetransform));
                field_10C_state = State::eFinish_3;
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
                sActiveHero->ExitShrykull_42F440(field_122_bResetRingTimer);
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

            if (field_11C_obj_being_zapped)
            {
                if (field_11C_obj_being_zapped->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    field_11C_obj_being_zapped->mBaseGameObjectRefCount--;
                    field_11C_obj_being_zapped = nullptr;
                }
                else
                {
                    const PSX_RECT zapRect = field_11C_obj_being_zapped->VGetBoundingRect();
                    const PSX_RECT ourRect = VGetBoundingRect();
                    if (static_cast<s32>(sGnFrame) == field_114_timer)
                    {
                        relive_new ParticleBurst(
                            FP_FromInteger((zapRect.x + zapRect.w) / 2),
                            FP_FromInteger((zapRect.y + zapRect.h) / 2),
                            20,
                            mSpriteScale,
                            BurstType::eBigPurpleSparks_2);

                       relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
                    }
                    field_118_zap_line->CalculateSourceAndDestinationPositions(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        FP_FromInteger((zapRect.x + zapRect.w) / 2),
                        FP_FromInteger((zapRect.y + zapRect.h) / 2));
                }
            }

            if (static_cast<s32>(sGnFrame) > field_110_timer)
            {
                field_10C_state = State::eZapTargets_1;

                if (field_11C_obj_being_zapped)
                {
                    if (!field_120_bElectrocute)
                    {
                        field_11C_obj_being_zapped->VTakeDamage(this);
                    }

                    field_11C_obj_being_zapped->mBaseGameObjectRefCount--;
                    field_11C_obj_being_zapped = nullptr;
                }
            }
            break;

        default:
            return;
    }
}

} // namespace AO
