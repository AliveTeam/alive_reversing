#include "stdafx_ao.h"
#include "Function.hpp"
#include "MeatSaw.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "Math.hpp"
#include "Blood.hpp"
#include "ScreenManager.hpp"

namespace AO {

void MeatSaw::VScreenChanged()
{
    VScreenChanged_43A060();
}

MeatSaw* MeatSaw::Vdtor_43A0B0(signed int flags)
{
    dtor_4398F0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* MeatSaw::VDestructor(signed int flags)
{
    return Vdtor_43A0B0(flags);
}

BaseGameObject* MeatSaw::dtor_4398F0()
{
    SetVTable(this, 0x4BB990);
    if ((field_1A8_flags & 1) && SwitchStates_Get(field_EE_switch_id) != field_F2_switch_value)
    {
        gMap_507BA8.TLV_Reset_446870(field_100_tlvInfo, 1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_100_tlvInfo, 0, 0, 0);
    }

    field_110_anim.vCleanUp();
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    return dtor_417D10();
}

MeatSaw* MeatSaw::ctor_439570(Path_MeatSaw* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BB990);
    field_4_typeId = Types::eMeatSaw_56;

    SetVTable(&field_110_anim, 0x4BA2B8);

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMeatsawResID, 1, 0);
    Animation_Init_417FD0(15200, 104, 36, ppRes, 1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_B_render_mode = TPageAbr::eBlend_0;

    if (pTlv->field_18_scale_background)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = Layer::eLayer_5;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = Layer::eLayer_24;
        field_C6_scale = 1;
    }

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 8);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    field_F6_min_time_off1 = pTlv->field_1A_min_time_off1;
    field_F8_field_1C_max_time_off1 = pTlv->field_1C_max_time_off1;

    field_E6_max_rise_time = pTlv->field_1E_max_rise_time;
    field_EE_switch_id = pTlv->field_20_id;

    field_C8_yOffset = 0;
    field_F4 = 0;

    if (pTlv->field_22_type == 1)
    {
        field_1A8_flags &= ~2;
        field_1A8_flags |= 1;
    }
    else if (pTlv->field_22_type == 2)
    {
        field_1A8_flags |= 3u;
    }
    else // 0
    {
        field_1A8_flags &= ~3u;
       
    }

    field_EA_speed1 = pTlv->field_24_speed;
    field_E8_speed2 = pTlv->field_24_speed;
    if (pTlv->field_26_start_state == 0)
    {
        field_F0_switch_value = SwitchStates_Get(pTlv->field_20_id) == 0;
    }
    else
    {
        if (pTlv->field_20_id)
        {
            field_F0_switch_value = SwitchStates_Get(pTlv->field_20_id);
        }
        else
        {
            field_F0_switch_value = 0;
        }
    }

    if (pTlv->field_20_id)
    {
        field_F2_switch_value = SwitchStates_Get(pTlv->field_20_id);
    }
    else
    {
        field_F2_switch_value = 0;
    }

    field_FA_min_time_off2 = pTlv->field_2A_min_time_off2;

    field_EC_off_speed = pTlv->field_28_off_speed;
    field_FC_max_time_off2 = pTlv->field_2C_max_time_off2;

    if (pTlv->field_1_unknown)
    {
        field_F0_switch_value = field_F0_switch_value == 0;
        field_F2_switch_value = field_F2_switch_value == 0;
    }

    field_104_idle_timer = 0;
    field_E4_state = MeatSawStates::eIdle_0;
    field_10C_FrameCount = 0;
    field_AC_ypos -= FP_FromInteger(pTlv->field_1E_max_rise_time);
    field_100_tlvInfo = tlvInfo;

    if (!pTlv->field_2E_inital_position == 0)
    {
        field_E4_state = MeatSawStates::eGoingUp_2;
        field_F4 = pTlv->field_1E_max_rise_time + pTlv->field_24_speed - pTlv->field_1E_max_rise_time % pTlv->field_24_speed;
    }

    BYTE** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMeatsawResID, 1, 0);
    if (field_110_anim.Init_402D20(
        15252,
        gObjList_animations_505564,
        this,
        104,
        36,
        ppRes2,
        1,
        0,
        0))
    {
        field_110_anim.field_C_layer = field_10_anim.field_C_layer;
        field_110_anim.field_14_scale = field_BC_sprite_scale;

        field_110_anim.field_8_r = static_cast<BYTE>(field_C0_r);
        field_110_anim.field_9_g = static_cast<BYTE>(field_C2_g);
        field_110_anim.field_A_b = static_cast<BYTE>(field_C4_b);

        field_110_anim.field_B_render_mode = TPageAbr::eBlend_0;

        field_110_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_110_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_D0_pShadow = ao_new<Shadow>();
        if (field_D0_pShadow)
        {
            field_D0_pShadow->ctor_461FB0();
        }
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 1, 0);
    }
    else
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }
    return this;
}

void MeatSaw::VScreenChanged_43A060()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path ||
        !sControlledCharacter_50767C || // Can be nullptr during the game ender
        FP_Abs(sControlledCharacter_50767C->field_A8_xpos - field_A8_xpos) > FP_FromInteger(1024))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void MeatSaw::VUpdate()
{
    VUpdate_4399D0();
}

void MeatSaw::VUpdate_4399D0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    GrindUpObjects_439CD0();

    const CameraPos direction = gMap_507BA8.GetDirection(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos);

    if (!(field_10C_FrameCount % 87))
    {
        SFX_Play_43AED0(SoundEffect::MeatsawOffscreen_88, 45, direction);
    }

    if (!(field_10C_FrameCount % 25))
    {
        SFX_Play_43AED0(SoundEffect::MeatsawIdle_89, 45, direction);
    }

    field_10C_FrameCount++;

    switch (field_E4_state)
    {
    case MeatSawStates::eIdle_0:
        if ((field_104_idle_timer <= static_cast<int>(gnFrameCount_507670) || (field_1A8_flags & 2)) &&
            ((!(field_1A8_flags & 1)) ||  SwitchStates_Get(field_EE_switch_id) == field_F0_switch_value))
        {
            field_E4_state = MeatSawStates::eGoingDown_1;
            field_10_anim.Set_Animation_Data_402A40(15232, nullptr);
            field_1A8_flags &= ~4u;
            field_E8_speed2 = field_EA_speed1;
            field_108_SFX_timer = gnFrameCount_507670 + 2;
        }
        else
        {
            if (field_1A8_flags & 1)
            {
                if (!(field_1A8_flags & 2))
                {
                    if (field_EC_off_speed)
                    {
                        if (field_104_idle_timer <= static_cast<int>(gnFrameCount_507670))
                        {
                            field_E4_state = MeatSawStates::eGoingDown_1;
                            field_10_anim.Set_Animation_Data_402A40(15232, nullptr);
                            field_1A8_flags |= 4u;
                            field_E8_speed2 = field_EC_off_speed;
                            field_108_SFX_timer = gnFrameCount_507670 + 2;
                        }
                    }
                }
            }
        }
        break;

    case MeatSawStates::eGoingDown_1:
        field_F4 += field_E8_speed2;

        if (!((gnFrameCount_507670 - field_108_SFX_timer) % 8))
        {
            SFX_Play_43AED0(SoundEffect::MeatsawDown_91, 50, direction);
        }

        if (field_F4 >= field_E6_max_rise_time)
        {
            field_E4_state = MeatSawStates::eGoingUp_2;
            field_108_SFX_timer = gnFrameCount_507670 + 2;
        }
        break;

    case MeatSawStates::eGoingUp_2:
        if (!((gnFrameCount_507670 - field_108_SFX_timer) % 10))
        {
            field_108_SFX_timer = gnFrameCount_507670;
            SFX_Play_43AED0(SoundEffect::MeatsawUp_90, 50, direction);
        }

        field_F4 -= field_E8_speed2;

        if (field_F4 <= 0)
        {
            field_E4_state = MeatSawStates::eIdle_0;
            short minRnd = 0;
            short maxRnd = 0;
            if ((field_1A8_flags >> 2) & 1)
            {
                maxRnd = field_FC_max_time_off2;
                minRnd = field_FA_min_time_off2;
            }
            else
            {
                maxRnd = field_F8_field_1C_max_time_off1;
                minRnd = field_F6_min_time_off1;
            }

            field_104_idle_timer = gnFrameCount_507670 + Math_RandomRange_450F20(minRnd, maxRnd);
            field_10_anim.Set_Animation_Data_402A40(15200, 0);
            if (((field_1A8_flags) >> 1) & 1)
            {
                SwitchStates_Set(field_EE_switch_id, field_F0_switch_value == 0 ? 1 : 0);
            }
        }
        break;
    }
}

void MeatSaw::GrindUpObjects_439CD0()
{
    PSX_RECT ourRect = {};
    VGetBoundingRect(&ourRect, 1);

    ourRect.y += field_C8_yOffset;
    ourRect.h += field_C8_yOffset;

    for (int i=0; i< gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            if (pObjIter->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
            {
                // Can't grind meat with a meat saw, that would be grindception
                if (pObjIter->field_4_typeId != Types::eMeat_54)
                {
                    PSX_RECT objRect = {};
                    pObjIter->VGetBoundingRect(&objRect, 1);
                    
                    if (RectsOverlap(ourRect, objRect) &&
                        pObjIter->field_BC_sprite_scale == field_BC_sprite_scale &&
                        pObjIter->field_100_health > FP_FromInteger(0))
                    {
                        if (pObjIter->field_A8_xpos >= FP_FromInteger(ourRect.x) &&
                            pObjIter->field_A8_xpos <= FP_FromInteger(ourRect.w))
                        {
                            if (!pObjIter->VTakeDamage(this))
                            {
                                return;
                            }

                            auto pBlood1 = ao_new<Blood>();
                            if (pBlood1)
                            {
                                pBlood1->ctor_4072B0(
                                    pObjIter->field_A8_xpos,
                                    FP_FromInteger(ourRect.h - 10),
                                    FP_FromInteger(-5),
                                    FP_FromInteger(5),
                                    field_BC_sprite_scale,
                                    50);
                            }

                            auto pBlood2 = ao_new<Blood>();
                            if (pBlood2)
                            {
                                pBlood2->ctor_4072B0(
                                    pObjIter->field_A8_xpos,
                                    FP_FromInteger(ourRect.h - 10),
                                    FP_FromInteger(0),
                                    FP_FromInteger(5),
                                    field_BC_sprite_scale,
                                    50);
                            }

                            auto pBlood3 = ao_new<Blood>();
                            if (pBlood3)
                            {
                                pBlood3->ctor_4072B0(
                                    pObjIter->field_A8_xpos,
                                    FP_FromInteger(ourRect.h - 10),
                                    FP_FromInteger(5),
                                    FP_FromInteger(5),
                                    field_BC_sprite_scale,
                                    50);
                            }
                            SFX_Play_43AD70(SoundEffect::KillEffect_78, 127, 0);
                            SFX_Play_43AE60(SoundEffect::KillEffect_78, 127, -700, 0);
                            return;
                        }
                    }
                }
            }
        }
    }
}

void MeatSaw::VRender(PrimHeader** ppOt)
{
    VRender_439F50(ppOt);
}

void MeatSaw::VRender_439F50(PrimHeader** ppOt)
{
    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0))
    {
        field_C8_yOffset = field_F4;
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

        field_110_anim.vRender(
            FP_GetExponent(field_A8_xpos
                + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)
                - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(field_AC_ypos
                + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + field_E6_max_rise_time))
                - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        field_110_anim.Get_Frame_Rect_402B50(&rect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            rect.x,
            rect.y,
            rect.w,
            rect.h,
            pScreenManager_4FF7C8->field_2E_idx);
    }
}

}
