#include "stdafx_ao.h"
#include "Function.hpp"
#include "ElectricWall.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "Electrocute.hpp"
#include "Flash.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

ElectricWall* ElectricWall::ctor_40FCF0(Path_ElectricWall* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BA8C0);
    field_4_typeId = Types::eElectricWall_25;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElecwallResID, 1, 0);
    Animation_Init_417FD0(14536, 50, 80, ppRes, 1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_B_render_mode = 1;
    field_10_anim.field_C_layer = 36;

    if (field_10_anim.Get_Frame_Count_403540() > 0)
    {
        field_10_anim.SetFrame_402AC0(Math_NextRandom() % field_10_anim.Get_Frame_Count_403540());
    }

    field_C4_b = 80;
    field_C2_g = 80;
    field_C0_r = 80;
    field_E4_tlv = tlvInfo;
    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    field_E8_switch_id = pTlv->field_1A_id;
    field_EA_start_state = pTlv->field_1C_start_state;

    if (SwitchStates_Get(pTlv->field_1A_id) == field_EA_start_state)
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_EC_sound_timer = 0;
    return this;
}

BaseGameObject* ElectricWall::dtor_40FE80()
{
    SetVTable(this, 0x4BA8C0);
    gMap_507BA8.TLV_Reset_446870(field_E4_tlv, -1, 0, 0);
    return dtor_417D10();
}

BaseGameObject* ElectricWall::VDestructor(signed int flags)
{
    return Vdtor_410280(flags);
}

ElectricWall* ElectricWall::Vdtor_410280(signed int flags)
{
    dtor_40FE80();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void ElectricWall::VScreenChanged()
{
    VScreenChanged_410220();
}

void ElectricWall::VScreenChanged_410220()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path ||
        gMap_507BA8.GetDirection(field_B2_lvl_number, field_B0_path_number, field_A8_xpos, field_AC_ypos) == CameraPos::eCamInvalid_m1)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void ElectricWall::VUpdate()
{
    VUpdate_40FEF0();
}

void ElectricWall::VUpdate_40FEF0()
{
    const CameraPos soundDirection = gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos);

    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (SwitchStates_Get(field_E8_switch_id) == field_EA_start_state)
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

        // Keep flipping direction
        if (!(gnFrameCount_507670 % 8))
        {
            field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        }

         // Play sound every so often
        if (static_cast<int>(gnFrameCount_507670) >= field_EC_sound_timer)
        {
            // set a random starting frame
            SFX_Play_43AED0(SoundEffect::BirdPortalSpark_48, 45, soundDirection);
            field_EC_sound_timer = gnFrameCount_507670 + Math_RandomRange_450F20(24, 40);
        }

     
        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        PSX_RECT bRectBigger;
        bRectBigger.x = FP_GetExponent(field_A8_xpos - FP_FromInteger(4));
        bRectBigger.y = static_cast<short>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(field_A8_xpos + FP_FromInteger(4));
        bRectBigger.h = static_cast<short>(bRect.h + 5);

        for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
        {
            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            // Don't kill nades
            if (pObjIter->field_4_typeId != Types::eGrenade_40)
            {
                PSX_RECT objRect = {};
                pObjIter->VGetBoundingRect(&objRect, 1);

                if (!RectsOverlap(bRectBigger, objRect))
                {
                    // Not touching, so every so often check if we are near
                    if (!(gnFrameCount_507670 % 3))
                    {
                        // Make each side of the rect wider
                        objRect.x -= 50;
                        objRect.w += 50;

                         if (RectsOverlap(bRectBigger, objRect) && pObjIter->field_100_health > FP_FromInteger(0))
                        {
                            SFX_Play_43AED0(SoundEffect::ElectricGateLoud_47, 45, soundDirection);
                        }
                    }
                }
                else
                {
                    // Touching the wall, rip
                    if (!pObjIter->field_10A_flags.Get(Flags_10A::e10A_Bit5_Electrocuted))
                    {
                        pObjIter->field_10A_flags.Set(Flags_10A::e10A_Bit5_Electrocuted);
                        auto pElectrocute = ao_new<Electrocute>();
                        if (pElectrocute)
                        {
                            pElectrocute->ctor_48D3A0(pObjIter, 1);
                        }

                        pObjIter->VTakeDamage(this);

                        SFX_Play_43AED0(SoundEffect::ElectricZap_46, 127, soundDirection);

                        auto pFlash = ao_new<Flash>();
                        if (pFlash)
                        {
                            pFlash->ctor_41A810(39, 255u, 255u, 255u, 1, 3u, 1);
                        }
                    }
                }
            }
        }
    }
}

}
