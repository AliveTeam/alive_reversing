#include "stdafx.h"
#include "Greeter.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

struct MotionDetector : public BaseAnimatedWithPhysicsGameObject
{
    EXPORT MotionDetector* MotionDetector::ctor_4683B0(int /*pTlv*/, int /*tlvInfo*/, BaseAnimatedWithPhysicsGameObject* /*pOwner*/)
    {
        return this;
    }

    int field_E4_not_used[4];
    int field_F4;
    int field_F8_laser_id;
    int field_FC_owner_id;
    __int16 field_100;
    __int16 field_102;
    __int16 field_104;
    __int16 field_106;
    __int16 field_108;
    __int16 field_10A;
    __int16 field_10C;
    __int16 field_10E;
    __int16 field_110;
    __int16 field_112;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    __int16 field_124;
    __int16 field_126;
    __int16 field_128;
    __int16 field_12A;
    __int16 field_12C;
    __int16 field_12E;
    __int16 field_130;
    __int16 field_132;
    __int16 field_134;
    __int16 field_136;
    __int16 field_138;
    __int16 field_13A;
    __int16 field_13C;
    __int16 field_13E;
    __int16 field_140;
    __int16 field_142;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    __int16 field_14E;
    __int16 field_150;
    __int16 field_152;
    __int16 field_154;
    __int16 field_156;
    __int16 field_158;
    __int16 field_15A;
    __int16 field_15C;
    __int16 field_15E;
    __int16 field_160;
    __int16 field_162;
    __int16 field_164;
    __int16 field_166;
    __int16 field_168;
    __int16 field_16A;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    int field_174;
    __int16 field_178;
    __int16 field_17A;
};
// 17c

EXPORT Greeter* Greeter::ctor_4465B0(Path_Greeter* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x54566C);

    field_4_typeId = Types::eGreeter_64;
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kGreeterResID);
    Animation_Init_424E10(50028, 65, 60, ppRes, 1, 1);

    field_DC_bApplyShadows |= 2u;

    if (pTlv->field_10_scale)
    {
        field_20_animation.field_C_render_layer = 14;
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_20_animation.field_C_render_layer = 33;
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }


    field_6_flags.Set(BaseGameObject::eCanExplode);

    if (pTlv->field_14_direction == 0)
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
    }

    field_134_speed = FP_FromInteger(pTlv->field_12_motion_detector_speed);
    field_13C_state = 0;
    field_12E = 1;
    field_118_tlvInfo = tlvInfo;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    int lineType = field_D6_scale != 0 ? 0xFFFFFFF1 : 0;// probably just 0xF1

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        lineType + 0x10) == 1)
    {
        field_BC_ypos = hitY;
    }

    auto pMotionDetctor = alive_new<MotionDetector>();
    if (pMotionDetctor)
    {
        pMotionDetctor->ctor_4683B0(0, 0, this);
        field_11C_motionDetectorId = pMotionDetctor->field_8_object_id;
    }

    field_140 = 0;
    field_13E = 0;
    
    field_128_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(70, 210);

    Add_Resource_4DC130(ResourceManager::Resource_Animation, 365);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 301);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, 25);

    field_12C = 0;
    field_E0_176_ptr = alive_new<Shadow>();
    if (field_E0_176_ptr)
    {
        field_E0_176_ptr->ctor_4AC990();
    }

    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);
    field_130 = 0;

    return this;
}
