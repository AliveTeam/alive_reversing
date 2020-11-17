#include "stdafx_ao.h"
#include "Function.hpp"
#include "DoorLight.hpp"
#include "SwitchStates.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "MusicTrigger.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "ScreenManager.hpp"
#include "CameraSwapper.hpp"

namespace AO {

ALIVE_VAR(1, 0x4C30A8, int, gNextDoorLightUpdate_4C30A8, -1);
ALIVE_VAR(1, 0x4FC8A4, int, gDoorLightUpdateTimer_4FC8A4, 0);

DoorLight* DoorLight::ctor_405D90(Path_LightEffect* pTlv, int tlvInfo)
{
    ctor_417C10();

    field_E4_tlvInfo = tlvInfo;
    field_E8_width = pTlv->field_1A_size;
    field_EA_height = pTlv->field_1A_size;
  
    SetVTable(this, 0x4BA1A8);

    field_4_typeId = Types::eNone_0;
    field_EC = 0;
    field_F0 = pTlv->field_1C_id;
    field_EE_switch_value = SwitchStates_Get(pTlv->field_1C_id);

    int xOff = 0;
    switch (pTlv->field_18_type)
    {
    case Path_LightEffect::Type::RedGoldGlow_1:
        Animation_Init_417FD0(236, 19, 15, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6011, 1, 0), 1);
        break;

    case Path_LightEffect::Type::GreenGlow_2:
        Animation_Init_417FD0(96, 5, 6, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6010, 1, 0), 1);
        break;

    case Path_LightEffect::Type::FlintGlow_3:
        Animation_Init_417FD0(756, 59, 54, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6028, 1, 0), 1);
        field_EC = 1;
        break;

    case Path_LightEffect::Type::FlintDoor_4:
        field_E8_width = 0;
        field_EA_height = 0;
        if (SwitchStates_Get(pTlv->field_1C_id))
        {
            Animation_Init_417FD0(448, 20, 11, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6031, 1, 0), 1);
        }
        else
        {
            Animation_Init_417FD0(460, 20, 11, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6032, 1, 0), 1);
            xOff = 6;
        }
        break;

    case Path_LightEffect::Type::FlintHub_5:
        field_E8_width = 0;
        field_EA_height = 0;
        if (SwitchStates_Get(pTlv->field_1C_id))
        {
            Animation_Init_417FD0(460, 20, 11, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6031, 1, 0), 1);
        }
        else
        {
            Animation_Init_417FD0(448, 20, 11, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6032, 1, 0), 1);
        }
        break;

    default:
        break;
    }

    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_1E_flip_x == 0);

    if (gNextDoorLightUpdate_4C30A8 < 0)
    {
        gNextDoorLightUpdate_4C30A8 = gnFrameCount_507670;
        gDoorLightUpdateTimer_4FC8A4 = gNextDoorLightUpdate_4C30A8 + Math_RandomRange_450F20(30, 45);
    }
    
    field_10_anim.field_4_flags.Set(AnimFlags::eBit20_use_xy_offset);

    field_CC_bApplyShadows &= ~1u;
    field_10_anim.field_C_layer = 17;
    field_10_anim.field_B_render_mode = 3;

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x - xOff);
    }
    else
    {
        field_A8_xpos = FP_FromInteger(xOff + pTlv->field_10_top_left.field_0_x);
    }

    field_BC_sprite_scale = FP_FromInteger(1);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    return this;
}

BaseGameObject* DoorLight::dtor_4062F0()
{
    SetVTable(this, 0x4BA1A8);
    gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

BaseGameObject* DoorLight::VDestructor(signed int flags)
{
    return Vdtor_4064A0(flags);
}

DoorLight* DoorLight::Vdtor_4064A0(signed int flags)
{
    dtor_4062F0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void DoorLight::VScreenChanged()
{
    VScreenChanged_406360();
}

void DoorLight::VScreenChanged_406360()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    gNextDoorLightUpdate_4C30A8 = -1;
}


void DoorLight::VUpdate()
{
    VUpdate_4060A0();
}

void DoorLight::VUpdate_4060A0()
{
    if (static_cast<int>(gnFrameCount_507670) > gDoorLightUpdateTimer_4FC8A4)
    {
        gNextDoorLightUpdate_4C30A8 = gnFrameCount_507670 + Math_RandomRange_450F20(6, 20);
        gDoorLightUpdateTimer_4FC8A4 = gNextDoorLightUpdate_4C30A8 + Math_RandomRange_450F20(30, 45);
        field_C0_r = 32;
        field_C2_g = 32;
        field_C4_b = 32;
    }
    else if (static_cast<int>(gnFrameCount_507670) >= gNextDoorLightUpdate_4C30A8)
    {
        const FP lightAngle = (
            FP_FromInteger(128) * FP_FromInteger(gnFrameCount_507670 - gNextDoorLightUpdate_4C30A8) /
            FP_FromInteger(gDoorLightUpdateTimer_4FC8A4 - gNextDoorLightUpdate_4C30A8));

        const FP lightAngleCosine = -Math_Cosine_4510A0(FP_GetExponent(lightAngle) & 0xFF);
        const int rgbVal = FP_GetExponent(FP_FromInteger(255) * lightAngleCosine) + 32;

        BYTE rgb = 0;
        if (rgbVal <= 255)
        {
            rgb = rgbVal & 0xFF;
        }
        else
        {
            rgb = 255;
        }

        if (field_EC)
        {
            if (SwitchStates_Get(field_F0))
            {
                if (field_EE_switch_value == 0)
                {
                    field_EE_switch_value = 1;

                    if (sControlledCharacter_50767C == sActiveHero_507678)
                    {
                        auto pMusic = ao_new<MusicTrigger>();
                        if (pMusic)
                        {
                            pMusic->ctor_443A60(6, 1, 0, 15);
                        }
                    }
                    else
                    {
                        SND_SEQ_Play_477760(SeqId::Unknown_45, 1, 127, 127);
                    }
                }
                field_C0_r = 32;
                field_C2_g = rgb;
                field_C4_b = 32;
            }
            else
            {
                field_C0_r = rgb;
                field_C2_g = 32;
                field_C4_b = 32;
            }
        }
        else
        {
            field_C0_r = rgb;
            field_C2_g = rgb;
            field_C4_b = rgb;
        }
    }
}

void DoorLight::VRender(int** pOrderingTable)
{
    VRender_406370(pOrderingTable);
}

void DoorLight::VRender_406370(int** ppOt)
{
    if (sNumCamSwappers_507668 == 0)
    {
        const FP xpos = FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) + field_A8_xpos - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x;
        const FP ypos = FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) + field_AC_ypos - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y;

        field_10_anim.field_8_r = static_cast<BYTE>(field_C0_r);
        field_10_anim.field_9_g = static_cast<BYTE>(field_C2_g);
        field_10_anim.field_A_b = static_cast<BYTE>(field_C4_b);

        field_10_anim.vRender(
            FP_GetExponent(FP_FromInteger((FP_GetExponent(xpos) - field_E8_width / 2))),
            FP_GetExponent(FP_FromInteger((FP_GetExponent(ypos) - field_EA_height / 2))),
            ppOt,
            field_E8_width,
            field_EA_height);

        PSX_RECT rect = {};
        field_10_anim.Get_Frame_Rect_402B50(&rect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager_4FF7C8->field_2E_idx);
    }
}

}
