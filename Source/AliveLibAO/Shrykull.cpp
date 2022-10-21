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

BaseGameObject* Shrykull::dtor_463990()
{
    SetVTable(this, 0x4BC9B8);

    if (field_118_zap_line)
    {
        field_118_zap_line->field_C_refCount--;
        field_118_zap_line->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_11C_obj_being_zapped)
    {
        field_11C_obj_being_zapped->field_C_refCount--;
    }

    return dtor_401000();
}

Shrykull* Shrykull::Vdtor_464300(s32 flags)
{
    dtor_463990();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* Shrykull::VDestructor(s32 flags)
{
    return Vdtor_464300(flags);
}

void Shrykull::VScreenChanged_464280()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Shrykull::VScreenChanged()
{
    VScreenChanged_464280();
}

Shrykull* Shrykull::ctor_463880()
{
    ctor_401090();
    field_6_flags.Set(Options::eCanExplode_Bit7);
    SetVTable(this, 0x4BC9B8);
    field_4_typeId = Types::eShrykull_85;
    
    const AnimRecord& rec = AO::AnimRec(AnimId::Mudokon_ToShrykull);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    field_118_zap_line = nullptr;
    field_11C_obj_being_zapped = nullptr;

    field_A8_xpos = sActiveHero_507678->field_A8_xpos;
    field_AC_ypos = sActiveHero_507678->field_AC_ypos;
    field_BC_sprite_scale = sActiveHero_507678->field_BC_sprite_scale;
    field_C6_scale = sActiveHero_507678->field_C6_scale;
    field_10C_state = State::eTransform_0;

    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX));

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }
    field_122_bResetRingTimer = 0;
    return this;
}

void Shrykull::VOnThrowableHit(BaseGameObject*)
{
    // Empty
}

void Shrykull::VUpdate()
{
    VUpdate_463AE0();
}

bool Shrykull::CanKill(BaseAnimatedWithPhysicsGameObject* pObj)
{
    return (pObj->field_4_typeId == Types::eTimedMine_8
            || pObj->field_4_typeId == Types::eMine_57
            || pObj->field_4_typeId == Types::eUXB_99
            || pObj->field_4_typeId == Types::eSlig_88
            || pObj->field_4_typeId == Types::eSlog_89
            || pObj->field_4_typeId == Types::eBackgroundGlukkon_42
            || pObj->field_4_typeId == Types::eSecurityClaw_31
            || pObj->field_4_typeId == Types::SecurityOrb_53)
        && pObj->field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render)
        && !pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)
        && gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            pObj->field_B2_lvl_number,
            pObj->field_B0_path_number,
            pObj->field_A8_xpos,
            pObj->field_AC_ypos,
            0);
}

bool Shrykull::CanElectrocute(BaseGameObject* pObj) const
{
    switch (pObj->field_4_typeId)
    {
        case Types::eElum_26:
        case Types::eBackgroundGlukkon_42:
        case Types::eMudokon_52:
        case Types::eParamite_62:
        case Types::eMudokon_75:
        case Types::eScrab_77:
        case Types::eSlig_88:
        case Types::eSlog_89:
            return true;
        default:
            return false;
    }
}

void Shrykull::VUpdate_463AE0()
{
    switch (field_10C_state)
    {
        case State::eTransform_0:
            if (field_10_anim.field_92_current_frame == 0)
            {
                SFX_Play_43AE60(SoundEffect::Shrykull1_108, 127, -2000);
                SFX_Play_43AE60(SoundEffect::Shrykull2_109, 127, 0);
                SFX_Play_43AD70(SoundEffect::IngameTransition_107, 127);
            }

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                field_10_anim.Set_Animation_Data_402A40(92040, nullptr);
                field_10C_state = State::eZapTargets_1;
            }
            break;

        case State::eZapTargets_1:
            if (field_10_anim.field_92_current_frame == 0)
            {
                if (Math_NextRandom() >= 128u)
                {
                    SFX_Play_43AE60(SoundEffect::Shrykull2_109, 127, 0);
                }
                else
                {
                    SFX_Play_43AE60(SoundEffect::Shrykull1_108, 127, 0);
                }
            }

            for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
            {
                BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (CanKill(pObj) && !pObj->field_10A_flags.Get(Flags_10A::e10A_Bit3))
                {
                    pObj->field_C_refCount++;
                    field_11C_obj_being_zapped = pObj;

                    PSX_RECT objRect = {};
                    pObj->VGetBoundingRect(&objRect, 1);

                    PSX_RECT ourRect = {};
                    VGetBoundingRect(&ourRect, 1);

                    if (field_118_zap_line)
                    {
                        field_118_zap_line->CalculateSourceAndDestinationPositions_478CF0(
                            FP_FromInteger((ourRect.x + ourRect.w) / 2),
                            FP_FromInteger((ourRect.y + ourRect.h) / 2),
                            FP_FromInteger((objRect.x + objRect.w) / 2),
                            FP_FromInteger((objRect.y + objRect.h) / 2));
                    }
                    else
                    {
                        auto pZapLine = ao_new<ZapLine>();
                        if (pZapLine)
                        {
                            pZapLine->ctor_4789A0(
                                FP_FromInteger((ourRect.x + ourRect.w) / 2),
                                FP_FromInteger((ourRect.y + ourRect.h) / 2),
                                FP_FromInteger((objRect.x + objRect.w) / 2),
                                FP_FromInteger((objRect.y + objRect.h) / 2),
                                0, ZapLineType::eThin_1,
                                Layer::eLayer_ZapLinesElum_28);
                        }
                        pZapLine->field_C_refCount++;
                        field_118_zap_line = pZapLine;
                    }

                    field_120_bElectrocute = CanElectrocute(pObj);
                    if (field_120_bElectrocute)
                    {
                        auto pElectrocute = ao_new<Electrocute>();
                        if (pElectrocute)
                        {
                            pElectrocute->ctor_48D3A0(pObj, 0);
                        }
                        field_114_timer = gnFrameCount_507670 + 3;

                        if (pObj->field_4_typeId == Types::eBackgroundGlukkon_42)
                        {
                            pObj->VTakeDamage(this);
                        }
                    }

                    auto pFlicker1 = ao_new<PossessionFlicker>();
                    if (pFlicker1)
                    {
                        pFlicker1->ctor_41A8C0(field_11C_obj_being_zapped, 8, 255, 255, 255);
                    }

                    auto pRing1 = ao_new<AbilityRing>();
                    if (pRing1)
                    {
                        pRing1->ctor_455860(
                            FP_FromInteger((objRect.x + objRect.w) / 2),
                            FP_FromInteger((objRect.y + objRect.h) / 2),
                            RingTypes::eShrykull_Pulse_Large_5);
                    }

                    auto pFlicker2 = ao_new<PossessionFlicker>();
                    if (pFlicker2)
                    {
                        pFlicker2->ctor_41A8C0(this, 8, 255, 255, 255);
                    }

                    auto pAbilityRing2 = ao_new<AbilityRing>();
                    if (pAbilityRing2)
                    {
                        pAbilityRing2->ctor_455860(
                            FP_FromInteger((ourRect.x + ourRect.w) / 2),
                            FP_FromInteger((ourRect.y + ourRect.h) / 2),
                            RingTypes::eShrykull_Pulse_Large_5);
                    }

                    field_11C_obj_being_zapped->field_10A_flags.Set(Flags_10A::e10A_Bit3);

                    SFX_Play_43AE60(SoundEffect::Respawn_22, 100, 2000);
                    SFX_Play_43AD70(SoundEffect::Zap1_57, 0);

                    field_10C_state = State::eKillTargets_4;
                    field_110_timer = gnFrameCount_507670 + 12;
                    field_122_bResetRingTimer = 1;
                    return;
                }
            }

            if (field_118_zap_line)
            {
                field_118_zap_line->field_C_refCount--;
                field_118_zap_line->field_6_flags.Set(Options::eDead_Bit3);
                field_118_zap_line = nullptr;
            }
            field_10C_state = State::eDetransform_2;
            break;

        case State::eDetransform_2:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_10_anim.Set_Animation_Data_402A40(92152, nullptr);
                field_10C_state = State::eFinish_3;
            }
            break;

        case State::eFinish_3:
            if (field_10_anim.field_92_current_frame == 0)
            {
                SFX_Play_43AE60(SoundEffect::Shrykull1_108, 127, -2000);
                SFX_Play_43AE60(SoundEffect::Shrykull2_109, 127, 0);
            }

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                sActiveHero_507678->ExitShrykull_42F440(field_122_bResetRingTimer);
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            break;

        case State::eKillTargets_4:
            if (field_10_anim.field_92_current_frame == 0)
            {
                if (Math_NextRandom() >= 128u)
                {
                    SFX_Play_43AE60(SoundEffect::Shrykull2_109, 127, 0);
                }
                else
                {
                    SFX_Play_43AE60(SoundEffect::Shrykull1_108, 127, 0);
                }
            }

            if (field_11C_obj_being_zapped)
            {
                if (field_11C_obj_being_zapped->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    field_11C_obj_being_zapped->field_C_refCount--;
                    field_11C_obj_being_zapped = nullptr;
                }
                else
                {
                    PSX_RECT zapRect = {};
                    field_11C_obj_being_zapped->VGetBoundingRect(&zapRect, 1);

                    PSX_RECT ourRect = {};
                    VGetBoundingRect(&ourRect, 1);
                    if (static_cast<s32>(gnFrameCount_507670) == field_114_timer)
                    {
                        auto pParticleBurst = ao_new<ParticleBurst>();
                        if (pParticleBurst)
                        {
                            pParticleBurst->ctor_40D0F0(
                                FP_FromInteger((zapRect.x + zapRect.w) / 2),
                                FP_FromInteger((zapRect.y + zapRect.h) / 2),
                                20,
                                field_BC_sprite_scale,
                                BurstType::eBigPurpleSparks_2);
                        }

                        auto pFlash = ao_new<Flash>();
                        if (pFlash)
                        {
                            pFlash->ctor_41A810(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
                        }
                    }
                    field_118_zap_line->CalculateSourceAndDestinationPositions_478CF0(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        FP_FromInteger((zapRect.x + zapRect.w) / 2),
                        FP_FromInteger((zapRect.y + zapRect.h) / 2));
                }
            }

            if (static_cast<s32>(gnFrameCount_507670) > field_110_timer)
            {
                field_10C_state = State::eZapTargets_1;

                if (field_11C_obj_being_zapped)
                {
                    if (!field_120_bElectrocute)
                    {
                        field_11C_obj_being_zapped->VTakeDamage(this);
                    }

                    field_11C_obj_being_zapped->field_C_refCount--;
                    field_11C_obj_being_zapped = nullptr;
                }
            }
            break;

        default:
            return;
    }
}

} // namespace AO
