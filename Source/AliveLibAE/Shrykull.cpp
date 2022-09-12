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
#include "Flash.hpp"
#include "PossessionFlicker.hpp"
#include "AbilityRing.hpp"
#include "Electrocute.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

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

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    field_128_obj_being_zapped_id = Guid{};
    field_124_zap_line_id = Guid{};

    const AnimRecord& shrykullRec = AnimRec(AnimId::ShrykullStart);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, shrykullRec.mResourceId);
    Animation_Init(AnimId::ShrykullStart, ppRes);

    mXPos = sActiveHero->mXPos;
    mYPos = sActiveHero->mYPos;
    mSpriteScale = sActiveHero->mSpriteScale;
    mScale = sActiveHero->mScale;

    field_118_state = State::eTransform_0;

    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, sActiveHero->mAnim.mFlags.Get(AnimFlags::eBit5_FlipX));

    mShadow = relive_new Shadow();

    field_12E_bResetRingTimer = 0;
}

Shrykull::~Shrykull()
{
    BaseGameObject* pZapLine = sObjectIds.Find_Impl(field_124_zap_line_id);
    if (pZapLine)
    {
        pZapLine->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        field_124_zap_line_id = Guid{};
    }

    field_128_obj_being_zapped_id = Guid{};
}


void Shrykull::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 Shrykull::CanElectrocute(BaseGameObject* pObj)
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
            return 1;
        default:
            return 0;
    }
}

s16 Shrykull::CanKill(BaseAnimatedWithPhysicsGameObject* pObj)
{
    return (
               pObj->Type() == ReliveTypes::eTimedMine_or_MovingBomb || pObj->Type() == ReliveTypes::eMine || pObj->Type() == ReliveTypes::eUXB || pObj->Type() == ReliveTypes::eSlig || pObj->Type() == ReliveTypes::eFlyingSlig || pObj->Type() == ReliveTypes::eCrawlingSlig || pObj->Type() == ReliveTypes::eSlog || pObj->Type() == ReliveTypes::eGlukkon || pObj->Type() == ReliveTypes::eSecurityClaw || pObj->Type() == ReliveTypes::eSecurityOrb)
        && pObj->mAnim.mFlags.Get(AnimFlags::eBit3_Render) && !pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && gMap.Is_Point_In_Current_Camera(pObj->mCurrentLevel, pObj->mCurrentPath, pObj->mXPos, pObj->mYPos, 0);
}

void Shrykull::VUpdate()
{
    auto pExistingBeingZappedObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_128_obj_being_zapped_id));
    auto pExistingZapLine = static_cast<ZapLine*>(sObjectIds.Find_Impl(field_124_zap_line_id));

    switch (field_118_state)
    {
        case State::eTransform_0:
            if (mAnim.mCurrentFrame == 0)
            {
                SFX_Play_Pitch(relive::SoundEffects::Shrykull1, 127, -2000);
                SFX_Play_Pitch(relive::SoundEffects::Shrykull2, 127, 0);
                SfxPlayMono(relive::SoundEffects::IngameTransition, 127);
            }

            if (mAnim.mFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::ShrykullTransform));
                field_118_state = State::eZapTargets_1;
            }
            break;

        case State::eZapTargets_1:
            if (mAnim.mCurrentFrame == 0)
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

                if (CanKill(pObj) && !pObj->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit5_ZappedByShrykull))
                {
                    field_128_obj_being_zapped_id = pObj->mBaseGameObjectId;

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
                            field_124_zap_line_id = pZapLine->mBaseGameObjectId;
                        }
                    }

                    field_12C_bElectrocute = CanElectrocute(pObj);
                    if (field_12C_bElectrocute)
                    {
                        relive_new Electrocute(pObj, 0, 1);
                        field_120_timer = sGnFrame + 3;

                        if (pObj->Type() == ReliveTypes::eGlukkon)
                        {
                            pObj->VTakeDamage(this);
                        }
                    }

                    relive_new PossessionFlicker(pObj, 8, 255, 255, 255);
                    AbilityRing::Factory(
                        FP_FromInteger((objRect.x + objRect.w) / 2),
                        FP_FromInteger((objRect.y + objRect.h) / 2),
                        RingTypes::eShrykull_Pulse_Large_5, pObj->mSpriteScale);

                    relive_new PossessionFlicker(this, 8, 255, 255, 255);
                    AbilityRing::Factory(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        RingTypes::eShrykull_Pulse_Large_5, mSpriteScale);

                    pObj->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit5_ZappedByShrykull);

                    SFX_Play_Pitch(relive::SoundEffects::ShrykullZap, 100, 2000);
                    SfxPlayMono(relive::SoundEffects::Zap1, 0);

                    field_118_state = State::eKillTargets_4;
                    field_11C_timer = sGnFrame + 12;
                    field_12E_bResetRingTimer = 1;
                    return;
                }
            }

            if (pExistingZapLine)
            {
                pExistingZapLine->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                field_124_zap_line_id = Guid{};
            }
            field_118_state = State::eDetransform_2;
            break;

        case State::eDetransform_2:
            if (mAnim.mCurrentFrame == 7)
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
            else if (mAnim.mCurrentFrame == 20)
            {
                SFX_Play_Pitch(relive::SoundEffects::Shrykull1, 127, -2000);
                SFX_Play_Pitch(relive::SoundEffects::Shrykull2, 127, 0);
            }

            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mAnim.Set_Animation_Data(GetAnimRes(AnimId::ShrykullDetransform));
                field_118_state = State::eFinish_3;
            }
            break;

        case State::eFinish_3:
            if (mAnim.mFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                sActiveHero->ExitShrykull_45A9D0(field_12E_bResetRingTimer);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        case State::eKillTargets_4:
            if (mAnim.mCurrentFrame == 0)
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
                if (pExistingBeingZappedObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    field_128_obj_being_zapped_id = Guid{};
                }
                else
                {
                    const PSX_RECT zapRect = pExistingBeingZappedObj->VGetBoundingRect();
                    const PSX_RECT ourRect = VGetBoundingRect();

                    if (static_cast<s32>(sGnFrame) == field_120_timer)
                    {
                        relive_new ParticleBurst(
                            FP_FromInteger((zapRect.x + zapRect.w) / 2),
                            FP_FromInteger((zapRect.y + zapRect.h) / 2),
                            20, mSpriteScale, BurstType::eBigPurpleSparks_2, 13);

                        relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, TPageAbr::eBlend_3, 1);
                    }
                    pExistingZapLine->CalculateSourceAndDestinationPositions(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        FP_FromInteger((zapRect.x + zapRect.w) / 2),
                        FP_FromInteger((zapRect.y + zapRect.h) / 2));
                }
            }

            if (static_cast<s32>(sGnFrame) > field_11C_timer)
            {
                field_118_state = State::eZapTargets_1;
                if (pExistingBeingZappedObj)
                {
                    if (!field_12C_bElectrocute)
                    {
                        pExistingBeingZappedObj->VTakeDamage(this);
                    }
                    field_128_obj_being_zapped_id = Guid{};
                }
            }
            break;

        default:
            return;
    }
}
