#include "stdafx.h"
#include "ElectricWall.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "BaseAliveGameObject.hpp"
#include "Flash.hpp"
#include "Abe.hpp"
#include "Electrocute.hpp"
#include "Function.hpp"

const s16 sElecticWallFrames_55165C[6] = {0, 6, 10, 18, 22, 0};

ElectricWall* ElectricWall::ctor_421DA0(Path_ElectricWall* pTlv, s32 tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x544B60);

    SetType(AETypes::eElectricWall_39);

    const AnimRecord& rec = AnimRec(AnimId::Electric_Wall);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;
    field_20_animation.field_C_render_layer = Layer::eLayer_Foreground_36;

    if (field_20_animation.Get_Frame_Count_40AC70() > 0)
    {
        field_20_animation.SetFrame_409D50(sElecticWallFrames_55165C[Math_RandomRange_496AB0(0, 4)]);
    }

    field_DC_bApplyShadows &= ~1;
    field_D4_b = 80;
    field_D2_g = 80;
    field_D0_r = 80;
    field_F4_tlvInfo = tlvInfo;
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_F8_switch_id = pTlv->field_12_switch_id;
    field_FA_start_state = pTlv->field_14_start_state;

    if (SwitchStates_Get_466020(field_F8_switch_id) == field_FA_start_state)
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_FC_sound_timer = 0;
    return this;
}

BaseGameObject* ElectricWall::VDestructor(s32 flags)
{
    return vdtor_421F70(flags);
}

void ElectricWall::VUpdate()
{
    vUpdate_422030();
}

void ElectricWall::VScreenChanged()
{
    vScreenChanged_422530();
}

ElectricWall* ElectricWall::vdtor_421F70(s32 flags)
{
    dtor_421FA0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void ElectricWall::dtor_421FA0()
{
    SetVTable(this, 0x544B60);
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void ElectricWall::vScreenChanged_422530()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path || gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) == CameraPos::eCamInvalid_m1)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void ElectricWall::vUpdate_422030()
{
    const CameraPos soundDirection = gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos);

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (SwitchStates_Get_466020(field_F8_switch_id) == field_FA_start_state)
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        // If we are about to become visible set a random starting frame
        if (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render)))
        {
            if (field_20_animation.Get_Frame_Count_40AC70() > 0)
            {
                field_20_animation.SetFrame_409D50(sElecticWallFrames_55165C[Math_RandomRange_496AB0(0, 4)]);
            }
        }

        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

        // Keep flipping direction
        if (!(sGnFrame_5C1B84 % 8))
        {
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }

        // Play sound every so often
        if (static_cast<s32>(sGnFrame_5C1B84) >= field_FC_sound_timer)
        {
            SFX_Play_46FC20(SoundEffect::BirdPortalSpark_41, 45, soundDirection, field_CC_sprite_scale);
            field_FC_sound_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(24, 40);
        }

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);

        PSX_RECT bRectBigger;
        bRectBigger.x = FP_GetExponent(field_B8_xpos - FP_FromInteger(4));
        bRectBigger.y = static_cast<s16>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(field_B8_xpos + FP_FromInteger(4));
        bRectBigger.h = static_cast<s16>(bRect.h + 5);

        for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            switch (pObj->Type())
            {
                // Can't zap this
                case AETypes::eBone_11:
                case AETypes::eRockSpawner_48:
                case AETypes::eGrenade_65:
                case AETypes::eMeat_84:
                case AETypes::eRock_105:
                    break;

                default:
                    if (pObj->field_D6_scale == field_D6_scale)
                    {
                        PSX_RECT objRect = {};
                        pObj->vGetBoundingRect_424FD0(&objRect, 1);

                        // If touching rect then we are fried
                        if (!RectsOverlap(bRectBigger, objRect))
                        {
                            // Not touching, so every so often check if we are near
                            if (!(sGnFrame_5C1B84 % 3))
                            {
                                // Make each side of the rect wider
                                objRect.x -= 50;
                                objRect.w += 50;

                                if (RectsOverlap(bRectBigger, objRect) && pObj->field_10C_health > FP_FromInteger(0))
                                {
                                    // When near play the buzzing sound
                                    SFX_Play_46FC20(SoundEffect::ElectricGateLoud_40, 45, soundDirection, field_CC_sprite_scale);
                                }
                            }
                        }
                        else
                        {
                            // Touching the wall, rip
                            if (!(pObj->field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted)) && (pObj != sActiveHero_5C1B68 || !gAbeBulletProof_5C1BDA))
                            {
                                pObj->field_114_flags.Set(Flags_114::e114_Bit7_Electrocuted);

                                auto pElectrocute = ae_new<Electrocute>();
                                if (pElectrocute)
                                {
                                    pElectrocute->ctor_4E5E80(pObj, 1, 1);
                                }

                                pObj->VTakeDamage_408730(this);

                                SFX_Play_46FC20(SoundEffect::ElectricZap_39, 127, soundDirection, field_CC_sprite_scale);

                                auto pFlash = ae_new<Flash>();
                                if (pFlash)
                                {
                                    pFlash->ctor_428570(Layer::eLayer_Above_FG1_39, 255, 255, 255, 1, TPageAbr::eBlend_3, 1);
                                }
                            }
                        }
                    }
                    break;
            }
        }
    }
}
