#include "stdafx.h"
#include "Shrykull.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "ObjectIds.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "ParticleBurst.hpp"
#include "Flash.hpp"
#include "PossessionFlicker.hpp"
#include "AbilityRing.hpp"
#include "Electrocute.hpp"
#include "Function.hpp"

Shrykull* Shrykull::ctor_4AEA20()
{
    ctor_408240(0);
    SetVTable(this, 0x5470F0);

    field_4_typeId = Types::eShrykull_121;

    field_6_flags.Set(BaseGameObject::eCanExplode_Bit7);

    field_128_obj_being_zapped_id = -1;
    field_124_zap_line_id = -1;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kShrmorphResID);
    Animation_Init_424E10(82676, 123, 79u, ppRes, 1, 1u);

    field_B8_xpos = sActiveHero_5C1B68->field_B8_xpos;
    field_BC_ypos = sActiveHero_5C1B68->field_BC_ypos;
    field_CC_sprite_scale = sActiveHero_5C1B68->field_CC_sprite_scale;
    field_D6_scale = sActiveHero_5C1B68->field_D6_scale;

    field_118_state = State::eState0;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, sActiveHero_5C1B68->field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX));

    field_E0_pShadow = ae_new<Shadow>();
    field_E0_pShadow->ctor_4AC990();
    
    field_12E_bUnknown = 0;

    return this;
}

BaseGameObject* Shrykull::VDestructor(signed int flags)
{
    return vdtor_4AEB90(flags);
}

void Shrykull::VUpdate()
{
    vUpdate_4AEDE0();
}

void Shrykull::VScreenChanged()
{
    vScreenChange_4AF650();
}

void Shrykull::dtor_4AEBC0()
{
    SetVTable(this, 0x5470F0);
    BaseGameObject* pZapLine = sObjectIds_5C1B70.Find_449CF0(field_124_zap_line_id);

    if (pZapLine)
    {
        pZapLine->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        field_124_zap_line_id = -1;
    }

    field_128_obj_being_zapped_id = -1;
    dtor_4080B0();
}

Shrykull* Shrykull::vdtor_4AEB90(signed int flags)
{
    dtor_4AEBC0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Shrykull::vScreenChange_4AF650()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_22 != gMap_5C3030.GetOverlayId_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

__int16 CCSTD Shrykull::Electrocute_4AED10(BaseGameObject* pObj)
{
    switch (pObj->field_4_typeId)
    {
    case Types::eCrawlingSlig_26:
    case Types::eType_40:
    case Types::eFlyingSlig_54:
    case Types::eGlukkon_67:
    case Types::eMudokon2_81:
    case Types::eParamite_96:
    case Types::eMudokon_110:
    case Types::eScrab_112:
    case Types::eSlig_125:
    case Types::eSlog_126:
        return 1;
    default:
        return 0;
    }
}

__int16 CCSTD Shrykull::CanKill_4AEC50(BaseAnimatedWithPhysicsGameObject* pObj)
{
    return 
        (
        pObj->field_4_typeId == Types::eTimedMine_or_MovingBomb_10 ||
        pObj->field_4_typeId == Types::eMine_88 ||
        pObj->field_4_typeId == Types::eUXB_143 ||
        pObj->field_4_typeId == Types::eSlig_125 ||
        pObj->field_4_typeId == Types::eFlyingSlig_54 ||
        pObj->field_4_typeId == Types::eCrawlingSlig_26 ||
        pObj->field_4_typeId == Types::eSlog_126 ||
        pObj->field_4_typeId == Types::eGlukkon_67 ||
        pObj->field_4_typeId == Types::eType_47 ||
        pObj->field_4_typeId == Types::eSecurityOrb_83
        ) &&
        pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render) && 
        !pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3) &&
        gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pObj->field_C2_lvl_number, pObj->field_C0_path_number, pObj->field_B8_xpos, pObj->field_BC_ypos, 0);
}

void Shrykull::vUpdate_4AEDE0()
{
    auto pExistingBeingZappedObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_128_obj_being_zapped_id));
    auto pExistingZapLine = static_cast<ZapLine*>(sObjectIds_5C1B70.Find_449CF0(field_124_zap_line_id));

    switch (field_118_state)
    {
    case State::eState0:
        if (field_20_animation.field_92_current_frame == 0)
        {
            SFX_Play_46FBA0(SoundEffect::Shrykull1_85, 127, -2000);
            SFX_Play_46FBA0(SoundEffect::Shrykull2_86, 127, 0);
            SFX_Play_46FA90(SoundEffect::IngameTransition_84, 127);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_20_animation.Set_Animation_Data_409C80(82712, 0);
            field_118_state = State::eState1;
        }
        break;

    case State::eState1:
        if (field_20_animation.field_92_current_frame == 0)
        {
            if (Math_NextRandom() >= 0x80u)
            {
                SFX_Play_46FBA0(SoundEffect::Shrykull2_86, 127, 0);
            }
            else
            {
                SFX_Play_46FBA0(SoundEffect::Shrykull1_85, 127, 0);
            }
        }

        for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (CanKill_4AEC50(pObj) && !pObj->field_114_flags.Get(Flags_114::e114_Bit5))
            {
                field_128_obj_being_zapped_id = pObj->field_8_object_id;

                PSX_RECT objRect = {};
                pObj->vGetBoundingRect_424FD0(&objRect, 1);

                PSX_RECT ourRect = {};
                vGetBoundingRect_424FD0(&ourRect, 1);

                if (pExistingZapLine)
                {
                    pExistingZapLine->sub_4CCAD0(
                        FP_FromInteger((ourRect.x + ourRect.w) / 2),
                        FP_FromInteger((ourRect.y + ourRect.h) / 2),
                        FP_FromInteger((objRect.x + objRect.w) / 2),
                        FP_FromInteger((objRect.y + objRect.h) / 2));
                }
                else
                {
                    auto pZapLineMem = ae_new<ZapLine>();
                    if (pZapLineMem)
                    {
                        pZapLineMem->ctor_4CC690(
                            FP_FromInteger((ourRect.x + ourRect.w) / 2),
                            FP_FromInteger((ourRect.y + ourRect.h) / 2),
                            FP_FromInteger((objRect.x + objRect.w) / 2),
                            FP_FromInteger((objRect.y + objRect.h) / 2),
                            0, 1, 28);
                    }
                    field_124_zap_line_id = pZapLineMem->field_8_object_id;
                }

                field_12C_bElectrocute = Electrocute_4AED10(pObj);
                if (field_12C_bElectrocute)
                {
                    auto pElectrocute = ae_new<Electrocute>();
                    if (pElectrocute)
                    {
                        pElectrocute->ctor_4E5E80(pObj, 0, 1);
                    }
                    field_120_timer = sGnFrame_5C1B84 + 3;

                    if (pObj->field_4_typeId == Types::eGlukkon_67)
                    {
                        pObj->VTakeDamage_408730(this);
                    }
                }

                auto pFlicker1 = ae_new<PossessionFlicker>();
                if (pFlicker1)
                {
                    pFlicker1->ctor_4319E0(pObj, 8, 255, 255, 255);
                }

                AbilityRing::Factory_482F80(
                    FP_FromInteger((objRect.x + objRect.w) / 2),
                    FP_FromInteger((objRect.y + objRect.h) / 2),
                    RingTypes::eShrykull_Pulse_Large_5, pObj->field_CC_sprite_scale);

                auto pFlicker2 = ae_new<PossessionFlicker>();
                if (pFlicker2)
                {
                    pFlicker2->ctor_4319E0(this, 8, 255, 255, 255);
                }

                AbilityRing::Factory_482F80(
                    FP_FromInteger((ourRect.x + ourRect.w) / 2),
                    FP_FromInteger((ourRect.y + ourRect.h) / 2),
                    RingTypes::eShrykull_Pulse_Large_5, field_CC_sprite_scale);
                
                pObj->field_114_flags.Set(Flags_114::e114_Bit5);

                SFX_Play_46FBA0(SoundEffect::ShrykullZap_18, 100, 2000);
                SFX_Play_46FA90(SoundEffect::Zap1_49, 0);

                field_118_state = State::eState4;
                field_11C_timer = sGnFrame_5C1B84 + 12;
                field_12E_bUnknown = 1;
                return;
            }
        }

        if (pExistingZapLine)
        {
            pExistingZapLine->field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_124_zap_line_id = -1;
        }
        field_118_state = State::eState2;
        break;

    case State::eState2:
        if (field_20_animation.field_92_current_frame == 7)
        {
            if (Math_NextRandom() >= 0x80u)
            {
                SFX_Play_46FBA0(SoundEffect::Shrykull2_86, 127, -512);
            }
            else
            {
                SFX_Play_46FBA0(SoundEffect::Shrykull1_85, 127, -512);
            }
        }
        else if (field_20_animation.field_92_current_frame == 20)
        {
            SFX_Play_46FBA0(SoundEffect::Shrykull1_85, 127, -2000);
            SFX_Play_46FBA0(SoundEffect::Shrykull2_86, 127, 0);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_20_animation.Set_Animation_Data_409C80(82824, nullptr);
            field_118_state = State::eState3;
        }
        break;

    case State::eState3:
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            sActiveHero_5C1B68->ExitShrykull_45A9D0(field_12E_bUnknown);
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        break;

    case State::eState4:
        if (field_20_animation.field_92_current_frame == 0)
        {
            if (Math_NextRandom()>= 0x80u)
            {
                SFX_Play_46FBA0(SoundEffect::Shrykull2_86, 127, 0);
            }
            else
            {
                SFX_Play_46FBA0(SoundEffect::Shrykull1_85, 127, 0);
            }
        }

        if (pExistingBeingZappedObj)
        {
            if (pExistingBeingZappedObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                field_128_obj_being_zapped_id = -1;
            }
            else
            {
                PSX_RECT zapRect = {};
                pExistingBeingZappedObj->vGetBoundingRect_424FD0(&zapRect, 1);

                PSX_RECT ourRect = {};
                vGetBoundingRect_424FD0(&ourRect, 1);

                if (static_cast<int>(sGnFrame_5C1B84) == field_120_timer)
                {
                    auto pParticleBurst = ae_new<ParticleBurst>();
                    if (pParticleBurst)
                    {
                        pParticleBurst->ctor_41CF50(
                            FP_FromInteger((zapRect.x + zapRect.w) / 2),
                            FP_FromInteger((zapRect.y + zapRect.h) / 2),
                            20, field_CC_sprite_scale, BurstType::eBigPurpleSparks_2, 13);
                    }

                    auto pFlash = ae_new<Flash>();
                    if (pFlash)
                    {
                        pFlash->ctor_428570(39, 255u, 255u, 255u, 1, 3u, 1);
                    }
                }
                pExistingZapLine->sub_4CCAD0(
                    FP_FromInteger((ourRect.x + ourRect.w) / 2),
                    FP_FromInteger((ourRect.y + ourRect.h) / 2),
                    FP_FromInteger((zapRect.x + zapRect.w) / 2),
                    FP_FromInteger((zapRect.y + zapRect.h) / 2));
            }
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_11C_timer)
        {
            field_118_state = State::eState1;
            if (pExistingBeingZappedObj)
            {
                if (!field_12C_bElectrocute)
                {
                    pExistingBeingZappedObj->VTakeDamage_408730(this);
                }
                field_128_obj_being_zapped_id = -1;
            }
        }
        break;

    default:
        return;
    }
}
