#include "stdafx_ao.h"
#include "Function.hpp"
#include "MotionDetector.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"

START_NS_AO

MotionDetector* MotionDetector::ctor_437A50(Path_MotionDetector* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BB878);
    field_4_typeId = Types::eGreeterBody_59;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6002, 1, 0);
    Animation_Init_417FD0(1108, 32, 22, ppRes, 1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_10_anim.field_B_render_mode = 1;
    field_10_anim.field_C_layer = 36;
    field_C8 = 0;
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

    field_F8 =  FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_100 =  FP_FromInteger(pTlv->field_14_bottom_right.field_0_x);
    field_FC =  FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_104 =  FP_FromInteger(pTlv->field_14_bottom_right.field_2_y);

    field_A8_xpos =FP_FromInteger(pTlv->field_1A_device_x);
    field_AC_ypos =  FP_FromInteger(pTlv->field_1C_device_y);

    field_15C_speed = FP_FromRaw(pTlv->field_1E_speed_x256 << 8);

    if ( pTlv->field_20_start_on == 0)
    {
        field_E8_state = 0;
        auto pMotionDetectors = ao_new<MotionDetectorLaser>();
        if (pMotionDetectors)
        {
            pMotionDetectors->ctor_417C10();
            SetVTable(pMotionDetectors, 0x4BB840);
            pMotionDetectors->field_4_typeId = Types::eRedLaser_76;
            BYTE** v16 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6001, 1, 0);
            pMotionDetectors->Animation_Init_417FD0(
                23660,
                37,
                60,
                v16,
                1);
            pMotionDetectors->field_10_anim.field_B_render_mode = 1;
            pMotionDetectors->field_10_anim.field_C_layer = 36;
            pMotionDetectors->field_A8_xpos = field_F8;
            pMotionDetectors->field_AC_ypos = field_104;
            pMotionDetectors->field_BC_sprite_scale = field_BC_sprite_scale;
            pMotionDetectors->field_C8 = 0;
            field_108_pLaser = pMotionDetectors;
        }
    }
    else
    {
        field_E8_state = 2;
        auto pMotionDetectors = ao_new<MotionDetectorLaser>();
        if (pMotionDetectors)
        {
            pMotionDetectors->ctor_417C10();
            SetVTable(pMotionDetectors, 0x4BB840);
            pMotionDetectors->field_4_typeId = Types::eRedLaser_76;
            BYTE** ppRes_1 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6001, 1, 0);
            pMotionDetectors->Animation_Init_417FD0(
                23660,
                37,
                60,
                ppRes_1,
                1);
            pMotionDetectors->field_10_anim.field_B_render_mode = 1;
            pMotionDetectors->field_10_anim.field_C_layer = 36;
            pMotionDetectors->field_A8_xpos = field_F8;
            pMotionDetectors->field_AC_ypos = field_104;
            pMotionDetectors->field_BC_sprite_scale = field_BC_sprite_scale;
            pMotionDetectors->field_C8 = 0;
            field_108_pLaser = pMotionDetectors;
        }
    }

    field_108_pLaser->field_C_refCount++;

    field_F0_alarm_id = pTlv->field_24_disable_id;

    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render, SwitchStates_Get(field_F0_alarm_id) == 0);

    field_108_pLaser->field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render,  pTlv->field_22_draw_flare & 1);

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


BaseGameObject* MotionDetectorLaser::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
