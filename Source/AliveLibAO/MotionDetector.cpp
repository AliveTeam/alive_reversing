#include "stdafx_ao.h"
#include "Function.hpp"
#include "MotionDetector.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "CameraSwapper.hpp"
#include "Abe.hpp"
#include "DDCheat.hpp"
#include "Game.hpp"
#include "Alarm.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "Sys_common.hpp"

namespace AO {

#undef min
#undef max

MotionDetector* MotionDetector::ctor_437A50(Path_MotionDetector* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BB878);
    field_4_typeId = Types::eMotionDetector_59;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMflareResID, 1, 0);
    Animation_Init_417FD0(1108, 32, 22, ppRes, 1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit7_SwapXY);

    field_10_anim.field_B_render_mode = 1;
    field_10_anim.field_C_layer = Layer::eLayer_36;
    field_C8_yOffset = 0;
    field_C0_r = 64;
    field_C4_b = 0;
    field_C2_g = 0;
    field_160_bObjectInLaser = 0;
    field_F6_bDontComeBack = 1;
    field_E4_tlvInfo = tlvInfo;

    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
    }

    field_F8_top_left_x = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_100_bottom_right_x = FP_FromInteger(pTlv->field_14_bottom_right.field_0_x);

    field_FC_top_left_y = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_104_bottom_right_y = FP_FromInteger(pTlv->field_14_bottom_right.field_2_y);

    field_A8_xpos = FP_FromInteger(pTlv->field_1A_device_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_1C_device_y);

    field_15C_speed = FP_FromRaw(pTlv->field_1E_speed_x256 << 8);

    if (pTlv->field_20_start_move_direction == Path_MotionDetector::StartMoveDirection::eRight_0)
    {
        field_E8_state = States::eMoveRight_0;
        auto pMotionDetectors = ao_new<MotionDetectorLaser>();
        if (pMotionDetectors)
        {
            pMotionDetectors->ctor_417C10();
            SetVTable(pMotionDetectors, 0x4BB840);
            pMotionDetectors->field_4_typeId = Types::eRedLaser_76;
            BYTE** ppMotionRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMotionResID, 1, 0);
            pMotionDetectors->Animation_Init_417FD0(
                23660,
                37,
                60,
                ppMotionRes,
                1);
            pMotionDetectors->field_10_anim.field_B_render_mode = 1;
            pMotionDetectors->field_10_anim.field_C_layer = Layer::eLayer_36;

            pMotionDetectors->field_A8_xpos = field_F8_top_left_x;
            pMotionDetectors->field_AC_ypos = field_104_bottom_right_y;

            pMotionDetectors->field_BC_sprite_scale = field_BC_sprite_scale;
            pMotionDetectors->field_C8_yOffset = 0;
            field_108_pLaser = pMotionDetectors;
        }
    }
    else if (pTlv->field_20_start_move_direction == Path_MotionDetector::StartMoveDirection::eLeft_1)
    {
        field_E8_state = States::eMoveLeft_2;
        auto pMotionDetectors = ao_new<MotionDetectorLaser>();
        if (pMotionDetectors)
        {
            pMotionDetectors->ctor_417C10();
            SetVTable(pMotionDetectors, 0x4BB840);
            pMotionDetectors->field_4_typeId = Types::eRedLaser_76;
            BYTE** ppMotionRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMotionResID, 1, 0);
            pMotionDetectors->Animation_Init_417FD0(
                23660,
                37,
                60,
                ppMotionRes,
                1);
            pMotionDetectors->field_10_anim.field_B_render_mode = 1;
            pMotionDetectors->field_10_anim.field_C_layer = Layer::eLayer_36;
            pMotionDetectors->field_A8_xpos = field_100_bottom_right_x;
            pMotionDetectors->field_AC_ypos = field_104_bottom_right_y;
            pMotionDetectors->field_BC_sprite_scale = field_BC_sprite_scale;
            pMotionDetectors->field_C8_yOffset = 0;
            field_108_pLaser = pMotionDetectors;
        }
    }
    else
    {
        ALIVE_FATAL("couldn't find start move direction for motion detector");
    }

    field_108_pLaser->field_C_refCount++;

    field_F0_disable_id = pTlv->field_24_disable_id;

    field_108_pLaser->field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render, SwitchStates_Get(field_F0_disable_id) == 0);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render, pTlv->field_22_draw_flare & 1);

    field_F4_alarm_time = pTlv->field_28_alarm_ticks;

    field_F2_alarm_trigger = pTlv->field_26_alarm_id;

    return this;
}

void MotionDetector::SetDontComeBack_437E00(__int16 bDontComeBack)
{
    field_F6_bDontComeBack = bDontComeBack;
}

BaseGameObject* MotionDetector::dtor_437D70()
{
    SetVTable(this, 0x4BB878);

    if (field_F6_bDontComeBack)
    {
        gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 1);
    }

    if (field_108_pLaser)
    {
        field_108_pLaser->field_C_refCount--;
        field_108_pLaser->field_6_flags.Set(Options::eDead_Bit3);
    }

    return dtor_417D10();
}

void MotionDetector::VScreenChanged()
{
    VScreenChanged_438520();
}

void MotionDetector::VScreenChanged_438520()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

BaseGameObject* MotionDetector::VDestructor(signed int flags)
{
    return Vdtor_438530(flags);
}

MotionDetector* MotionDetector::Vdtor_438530(signed int flags)
{
    dtor_437D70();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }

    return this;
}


void MotionDetector::VUpdate()
{
    VUpdate_437E90();
}

void MotionDetector::VUpdate_437E90()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (!sNumCamSwappers_507668)
    {
        if (SwitchStates_Get(field_F0_disable_id))
        {
            field_108_pLaser->field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        }
        else
        {
            field_108_pLaser->field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

            PSX_RECT laserRect = {};
            field_108_pLaser->VGetBoundingRect(&laserRect, 1);

            field_160_bObjectInLaser = FALSE;
           
            for (int i=0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
            {
                BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_4_typeId != Types::eTimedMine_8)
                {
                    PSX_RECT objRect = {};
                    pObj->VGetBoundingRect(&objRect, 1);

                    if (laserRect.x <= (objRect.w - 8) &&
                        laserRect.w >= (objRect.x + 8) &&
                        laserRect.h >= objRect.y &&
                        laserRect.y <= objRect.h &&
                        pObj->field_BC_sprite_scale == field_BC_sprite_scale)
                    {
                        if (pObj == sActiveHero_507678)
                        {
                            if (gnFrameCount_507670 % 2)
                            {
                                SFX_Play_43AD70(SoundEffect::Zap2_58, 45, 0);
                            }
                        }

                        bool alarm = false;
                        if (pObj->field_4_typeId == Types::eAbe_43)
                        {
                            if (pObj->field_FC_current_motion != eAbeStates::State_0_Idle_423520 &&
                                pObj->field_FC_current_motion != eAbeStates::State_19_CrouchIdle_4284C0 &&
                                pObj->field_FC_current_motion != eAbeStates::State_66_LedgeHang_428D90 &&
                                pObj->field_FC_current_motion != eAbeStates::State_62_LoadedSaveSpawn_45ADD0 &&
                                pObj->field_FC_current_motion != eAbeStates::State_60_Dead_42C4C0 &&
                                !sDDCheat_FlyingEnabled_50771C)
                            {
                                alarm = true;
                            }
                        }
                        else if (FP_GetExponent(pObj->field_B4_velx) ||
                                 FP_GetExponent(pObj->field_B8_vely))
                        {
                            alarm = true;
                            break;
                        }

                        if (alarm)
                        {
                            field_160_bObjectInLaser = TRUE;

                            if (alarmInstanceCount_5076A8 == 0)
                            {
                                auto pAlarm = ao_new<Alarm>();
                                if (pAlarm)
                                {
                                    pAlarm->ctor_402570(
                                        field_F4_alarm_time,
                                        field_F2_alarm_trigger,
                                        0,
                                        Layer::eLayer_39);
                                }

                                if (pObj == sActiveHero_507678)
                                {
                                    Mudokon_SFX_42A4D0(MudSounds::eOops_16, 0, 0, nullptr);
                                }
                            }
                        }
                    }
                }
            }


            switch (field_E8_state)
            {
            case States::eMoveRight_0:
                if (field_108_pLaser->field_A8_xpos >= field_100_bottom_right_x)
                {
                    field_E8_state = States::eWaitThenMoveLeft_1;
                    field_EC_timer = gnFrameCount_507670 + 15;
                    SFX_Play_43AD70(SoundEffect::MenuNavigation_61, 0);
                }
                else
                {
                    field_108_pLaser->field_A8_xpos += field_15C_speed;
                }
                break;

            case States::eWaitThenMoveLeft_1:
                if (static_cast<int>(gnFrameCount_507670) > field_EC_timer)
                {
                    field_E8_state = States::eMoveLeft_2;
                }
                break;

            case States::eMoveLeft_2:
                if (field_108_pLaser->field_A8_xpos <= field_F8_top_left_x)
                {
                    field_E8_state = States::eWaitThenMoveRight_3;
                    field_EC_timer = gnFrameCount_507670 + 15;
                    SFX_Play_43AD70(SoundEffect::MenuNavigation_61, 0);
                }
                else
                {
                    field_108_pLaser->field_A8_xpos -= field_15C_speed;
                }
                break;

            case States::eWaitThenMoveRight_3:
                if (static_cast<int>(gnFrameCount_507670) > field_EC_timer)
                {
                    field_E8_state = States::eMoveRight_0;
                }
                break;

            default:
                return;
            }
        }
    }
}


void MotionDetector::VRender(PrimHeader** ppOt)
{
    VRender_438250(ppOt);
}

void MotionDetector::VRender_438250(PrimHeader** ppOt)
{
    field_A8_xpos += FP_FromInteger(11);
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    field_A8_xpos -= FP_FromInteger(11);

    if (!SwitchStates_Get(field_F0_disable_id))
    {
        const short screen_top =  FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos));
    
        const short screen_left = FP_GetExponent(pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos));

        PSX_RECT bLaserRect = {};
        field_108_pLaser->VGetBoundingRect(&bLaserRect, 1);

        const short x0 = static_cast<short>(PsxToPCX(FP_GetExponent(field_A8_xpos) - screen_left, 11));
        const short y0 = FP_GetExponent(field_AC_ypos) - screen_top;
        const short y1 = FP_GetExponent(field_108_pLaser->field_AC_ypos) - screen_top;
        const short y2 = y1 + bLaserRect.y - bLaserRect.h;
        const short x1 = static_cast<short>(PsxToPCX(FP_GetExponent(field_108_pLaser->field_A8_xpos) - screen_left, 11));

        Poly_F3* pPrim = &field_10C_prims[gPsxDisplay_504C78.field_A_buffer_index];
        PolyF3_Init(pPrim);

        SetXY0(pPrim, x0, y0);
        SetXY1(pPrim, x1, y1);
        SetXY2(pPrim, x1, y2);

        SetRGB0(pPrim, 64, 0, 0);

        // Add triangle
        Poly_Set_SemiTrans_498A40(&pPrim->mBase.header, TRUE);
        OrderingTable_Add_498A80(&ppOt[field_10_anim.field_C_layer], &pPrim->mBase.header);

        // Add tpage
        Init_SetTPage_495FB0(&field_13C_tPage[gPsxDisplay_504C78.field_A_buffer_index], 0, 0,  PSX_getTPage_4965D0(2, field_160_bObjectInLaser != 0 ? 1 : 3, 0, 0)); // When detected transparency is off, gives the "solid red" triangle
        OrderingTable_Add_498A80(&ppOt[field_10_anim.field_C_layer], &field_13C_tPage[gPsxDisplay_504C78.field_A_buffer_index].mBase);
        
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            std::min(x0, std::min(x1, x1)),
            std::min(y0, std::min(y1, y2)),
            std::max(x0, std::max(x1, x1)),
            std::max(y0, std::max(y1, y2)),
            pScreenManager_4FF7C8->field_2E_idx);
    }
}

BaseGameObject* MotionDetectorLaser::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

}
