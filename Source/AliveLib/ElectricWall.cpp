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

const __int16 sElecticWallFrames_55165C[6] = { 0, 6, 10, 18, 22, 0 };

ElectricWall* ElectricWall::ctor_421DA0(Path_ElectricWall* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x544B60);
    
    field_4_typeId = Types::eElectricWall_39;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kElecwallResID);
    Animation_Init_424E10(15384, 50, 80, ppRes, 1, 1);
    field_20_animation.field_4_flags.Set(AnimFlags::eBit7_SwapXY);
    field_20_animation.field_B_render_mode = 1;
    field_20_animation.field_C_render_layer = 36;

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

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_F8_switch_id = pTlv->field_12_id;
    field_FA_start_state = pTlv->field_14_start_state;

    if (SwitchStates_Get_466020(field_F8_switch_id) == field_FA_start_state)
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_FC_sound_timer = 0;
    return this;
}

BaseGameObject* ElectricWall::VDestructor(signed int flags)
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

ElectricWall* ElectricWall::vdtor_421F70(signed int flags)
{
    dtor_421FA0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
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
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) == CameraPos::eCamInvalid_m1)
    {
        field_6_flags.Set(BaseGameObject::eDead);
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
        field_6_flags.Set(BaseGameObject::eDead);
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
            if (field_20_animation.Get_Frame_Count_40AC70() > 0 )
            {
                field_20_animation.SetFrame_409D50(sElecticWallFrames_55165C[Math_RandomRange_496AB0(0, 4)]);
            }
        }

        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

        // Keep flipped direction
        if (!(sGnFrame_5C1B84 % 8))
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            }
            else
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            }
        }

        // Play sound every so often
        if (static_cast<int>(sGnFrame_5C1B84) >= field_FC_sound_timer)
        {
            SFX_Play_46FC20(0x29u, 45, soundDirection, field_CC_sprite_scale);
            field_FC_sound_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(24, 40);
        }

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);
        
        PSX_RECT bRectBigger;
        bRectBigger.x = FP_GetExponent(field_B8_xpos - FP_FromInteger(4));
        bRectBigger.y = static_cast<short>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(field_B8_xpos + FP_FromInteger(4));
        bRectBigger.h = static_cast<short>(bRect.h + 5);

        for (int i=0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            switch (pObj->field_4_typeId)
            {
            // Can't zap this
            case Types::eBone_11:
            case Types::eRockSpawner_48:
            case Types::eGrenade_65:
            case Types::eMeat_84:
            case Types::eRock_105:
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
                                SFX_Play_46FC20(0x28u, 45, soundDirection, field_CC_sprite_scale);
                            }
                        }
                    }
                    else
                    {
                        // Touching the wall, rip
                        if (!(pObj->field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted)) && (pObj != sActiveHero_5C1B68 || !gAbeBulletProof_5C1BDA))
                        {
                            pObj->field_114_flags.Set(Flags_114::e114_Bit7_Electrocuted);

                            auto pElectrocute = alive_new<Electrocute>();
                            if (pElectrocute)
                            {
                                pElectrocute->ctor_4E5E80(pObj, 1, 1);
                            }

                            pObj->VTakeDamage_408730(this);

                            SFX_Play_46FC20(39u, 127, soundDirection, field_CC_sprite_scale);

                            auto pFlash = alive_new<Flash>();
                            if (pFlash)
                            {
                                pFlash->ctor_428570(39, 0xFFu, 0xFFu, 0xFFu, 1, 3, 1);
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}
