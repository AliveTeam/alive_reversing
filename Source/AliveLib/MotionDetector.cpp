#include "stdafx.h"
#include "MotionDetector.hpp"
#include "stdlib.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Function.hpp"

class MotionDetectorLaser : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MotionDetectorLaser* ctor_468290(FP xpos, FP ypos, FP scale, __int16 layer)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x545FB0);
        field_4_typeId = Types::eRedLaser_111;
        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMotionResID);
        Animation_Init_424E10(23280, 37, 60, ppRes, 1, 1);
        field_20_animation.field_C_render_layer = layer;
        field_B8_xpos = xpos;
        field_CC_sprite_scale = scale;
        field_20_animation.field_B_render_mode = 1;
        field_BC_ypos = ypos;
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_468360(flags);
    }

private:
    EXPORT MotionDetectorLaser* vdtor_468360(signed int flags)
    {
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

private:
    int field_E4_not_used[4];
};
ALIVE_ASSERT_SIZEOF(MotionDetectorLaser, 0xF4);


MotionDetector* MotionDetector::ctor_4683B0(Path_MotionDetector* pTlv, int tlvInfo, BaseAnimatedWithPhysicsGameObject* pOwner)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x545FF8);
    field_4_typeId = Types::eGreeterBody_91;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMflareResID);
    Animation_Init_424E10(1736, 55, 22, ppRes, 1, 1);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_B_render_mode = 1;
    field_20_animation.field_C_render_layer = 36;
    
    field_D8_yOffset = 0;

    field_D0_r = 64;
    field_D4_b = 0;
    field_D2_g = 0;

    field_178 = 0;

    if (!pOwner)
    {
        field_110_bDontComeBack = 1;
        field_10E_bUnknown = 0;
        field_FC_owner_id = -1;

        field_F4_tlvInfo = tlvInfo;
        field_CC_sprite_scale = FP_FromInteger(1);

        if (pTlv->field_10_scale)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
        }

        field_114_x1_fp = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
        field_11C_y1_fp = FP_FromInteger(pTlv->field_C_bottom_right.field_0_x);
        field_118_x2_fp = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
        field_120_y2_fp = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);

        PSX_Point pos = {};
        gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&pos);
        if (pTlv->field_12_device_x)
        {
            field_B8_xpos = FP_FromInteger(pTlv->field_12_device_x - pos.field_0_x);
            field_BC_ypos = FP_FromInteger(pTlv->field_14_device_y - pos.field_2_y);
        }
        else
        {
            field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
            field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
        }

        field_174_speed_x256 = (unsigned __int16)pTlv->field_16_speed_x256 << 8;

        MotionDetectorLaser* pLaser = nullptr;
        if (pTlv->field_18_start_on)
        {
            field_100_state = 2;
            pLaser = alive_new<MotionDetectorLaser>();
            if (pLaser)
            {
                pLaser->ctor_468290(field_11C_y1_fp, field_120_y2_fp, field_CC_sprite_scale, 36);
            }
        }
        else
        {
            field_100_state = 0;
            pLaser = alive_new<MotionDetectorLaser>();
            if (pLaser)
            {
                pLaser->ctor_468290(field_114_x1_fp, field_120_y2_fp, field_CC_sprite_scale, 36);
            }
        }

        if (pTlv->field_1A_draw_flare & 1)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        }

        if (pLaser)
        {
            field_F8_laser_id = pLaser->field_8_object_id;
            field_108_disable_id = pTlv->field_1C_disable_id;

            if (SwitchStates_Get_466020(static_cast<WORD>(field_F8_laser_id)) == 0)
            {
                pLaser->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }
            else
            {
                pLaser->field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            }
        }

        field_10A_alarm_id = pTlv->field_1E_alarm_id;
        field_10C_alarm_ticks = pTlv->field_20_alarm_ticks;
        return this;
    }

    field_10E_bUnknown = 1;
    field_CC_sprite_scale = pOwner->field_CC_sprite_scale;

    field_114_x1_fp = pOwner->field_B8_xpos - (field_CC_sprite_scale * FP_FromInteger(75));
    field_11C_y1_fp = (field_CC_sprite_scale * FP_FromInteger(75)) + pOwner->field_B8_xpos;
    field_118_x2_fp = pOwner->field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(20));
    field_120_y2_fp = pOwner->field_BC_ypos;

    field_B8_xpos = pOwner->field_B8_xpos;
    field_BC_ypos = pOwner->field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(20));

    field_174_speed_x256 = 0x20000;
    field_100_state = 0;

    auto pLaserMem = alive_new<MotionDetectorLaser>();
    if (pLaserMem)
    {
        pLaserMem->ctor_468290(pOwner->field_B8_xpos, pOwner->field_BC_ypos, field_CC_sprite_scale, 36);
    }

    field_F8_laser_id = pLaserMem->field_8_object_id;
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
    field_FC_owner_id = pOwner->field_8_object_id;
    field_10A_alarm_id = 0;
    field_10C_alarm_ticks = 0;
    return this;
}


MotionDetector* MotionDetector::vdtor_468850(signed int flags)
{
    dtor_468880();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void MotionDetector::dtor_468880()
{
    SetVTable(this, 0x545FF8);

    if (!field_10E_bUnknown)
    {
        if (field_110_bDontComeBack)
        {
            Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
        }
        else
        {
            Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 1);
        }
    }

    BaseGameObject* pLaser = sObjectIds_5C1B70.Find_449CF0(field_F8_laser_id);
    if (pLaser)
    {
        pLaser->field_6_flags.Set(BaseGameObject::eDead);
    }

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void MotionDetector::vScreenChanged_469460()
{
    BaseGameObject::VScreenChanged();

    BaseGameObject* pOwner = sObjectIds_5C1B70.Find_449CF0(field_FC_owner_id);
    if (!pOwner)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}
