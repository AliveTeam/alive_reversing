#include "stdafx_ao.h"
#include "Function.hpp"
#include "Shrykull.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Abe.hpp"
#include "ZapLine.hpp"
#include "Shadow.hpp"
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
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

Shrykull::Shrykull()
    : BaseAliveGameObject()
{
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    mBaseGameObjectTypeId = ReliveTypes::eShrykull;
    
    const AnimRecord& rec = AO::AnimRec(AnimId::Mudokon_ToShrykull);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    field_118_zap_line = nullptr;
    field_11C_obj_being_zapped = nullptr;

    mBaseAnimatedWithPhysicsGameObject_XPos = sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos;
    mBaseAnimatedWithPhysicsGameObject_YPos = sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos;
    field_BC_sprite_scale = sActiveHero_507678->field_BC_sprite_scale;
    field_C6_scale = sActiveHero_507678->field_C6_scale;
    field_10C_state = State::eTransform_0;

    field_10_anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, sActiveHero_507678->field_10_anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX));

    field_D0_pShadow = relive_new Shadow();

    field_122_bResetRingTimer = 0;
}

void Shrykull::VOnThrowableHit(BaseGameObject*)
{
    // Empty
}

bool Shrykull::CanKill(BaseAnimatedWithPhysicsGameObject* pObj)
{
    return (pObj->mBaseGameObjectTypeId == ReliveTypes::eTimedMine
            || pObj->mBaseGameObjectTypeId == ReliveTypes::eMine
            || pObj->mBaseGameObjectTypeId == ReliveTypes::eUXB
            || pObj->mBaseGameObjectTypeId == ReliveTypes::eSlig
            || pObj->mBaseGameObjectTypeId == ReliveTypes::eSlog
            || pObj->mBaseGameObjectTypeId == ReliveTypes::eBackgroundGlukkon
            || pObj->mBaseGameObjectTypeId == ReliveTypes::eSecurityClaw
            || pObj->mBaseGameObjectTypeId == ReliveTypes::SecurityOrb)
        && pObj->field_10_anim.mAnimFlags.Get(AnimFlags::eBit3_Render)
        && !pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead)
        && gMap.Is_Point_In_Current_Camera_4449C0(
            pObj->field_B2_lvl_number,
            pObj->field_B0_path_number,
            pObj->mBaseAnimatedWithPhysicsGameObject_XPos,
            pObj->mBaseAnimatedWithPhysicsGameObject_YPos,
            0);
}

bool Shrykull::CanElectrocute(BaseGameObject* pObj) const
{
    switch (pObj->mBaseGameObjectTypeId)
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
            if (field_10_anim.field_92_current_frame == 0)
            {
                SFX_Play_Pitch(SoundEffect::Shrykull1_108, 127, -2000);
                SFX_Play_Pitch(SoundEffect::Shrykull2_109, 127, 0);
                SFX_Play_Mono(SoundEffect::IngameTransition_107, 127);
            }

            if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                field_10_anim.Set_Animation_Data(92040, nullptr);
                field_10C_state = State::eZapTargets_1;
            }
            break;

        case State::eZapTargets_1:
            if (field_10_anim.field_92_current_frame == 0)
            {
                if (Math_NextRandom() >= 128u)
                {
                    SFX_Play_Pitch(SoundEffect::Shrykull2_109, 127, 0);
                }
                else
                {
                    SFX_Play_Pitch(SoundEffect::Shrykull1_108, 127, 0);
                }
            }

            for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
            {
                BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (CanKill(pObj) && !pObj->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit3))
                {
                    pObj->mBaseGameObjectRefCount++;
                    field_11C_obj_being_zapped = pObj;

                    PSX_RECT objRect = {};
                    pObj->VGetBoundingRect(&objRect, 1);

                    PSX_RECT ourRect = {};
                    VGetBoundingRect(&ourRect, 1);

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
                            Layer::eLayer_ZapLinesElum_28);
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

                        if (pObj->mBaseGameObjectTypeId == ReliveTypes::eBackgroundGlukkon)
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

                    SFX_Play_Pitch(SoundEffect::Respawn_22, 100, 2000);
                    SFX_Play_Mono(SoundEffect::Zap1_57, 0);

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
            if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_10_anim.Set_Animation_Data(92152, nullptr);
                field_10C_state = State::eFinish_3;
            }
            break;

        case State::eFinish_3:
            if (field_10_anim.field_92_current_frame == 0)
            {
                SFX_Play_Pitch(SoundEffect::Shrykull1_108, 127, -2000);
                SFX_Play_Pitch(SoundEffect::Shrykull2_109, 127, 0);
            }

            if (field_10_anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                sActiveHero_507678->ExitShrykull_42F440(field_122_bResetRingTimer);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        case State::eKillTargets_4:
            if (field_10_anim.field_92_current_frame == 0)
            {
                if (Math_NextRandom() >= 128u)
                {
                    SFX_Play_Pitch(SoundEffect::Shrykull2_109, 127, 0);
                }
                else
                {
                    SFX_Play_Pitch(SoundEffect::Shrykull1_108, 127, 0);
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
                    PSX_RECT zapRect = {};
                    field_11C_obj_being_zapped->VGetBoundingRect(&zapRect, 1);

                    PSX_RECT ourRect = {};
                    VGetBoundingRect(&ourRect, 1);
                    if (static_cast<s32>(sGnFrame) == field_114_timer)
                    {
                        relive_new ParticleBurst(
                            FP_FromInteger((zapRect.x + zapRect.w) / 2),
                            FP_FromInteger((zapRect.y + zapRect.h) / 2),
                            20,
                            field_BC_sprite_scale,
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
