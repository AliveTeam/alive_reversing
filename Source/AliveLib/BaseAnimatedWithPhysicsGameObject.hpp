#pragma once

#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "FunctionFwd.hpp"
#include "Animation.hpp"

class BaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
public:
    BaseAnimatedWithPhysicsGameObject() {}
    BaseAnimatedWithPhysicsGameObject(signed __int16 resourceArraySize);
    EXPORT BaseAnimatedWithPhysicsGameObject * BaseAnimatedWithPhysicsGameObject_ctor_424930(signed __int16 resourceArraySize);

    EXPORT void dtor_408210(signed int flags);
    virtual void VDestructor(signed int flags);

    EXPORT signed __int16 Animation_Init_424E10(int frameTableOffset, int maxW, unsigned __int16 maxH, AnimHeader *a5, __int16 a6, unsigned __int8 a7);

public:
    // VTable @ 0x544C9C
    /*
    BaseAnimatedWithPhysicsGameObject__dtor_424A40
    BaseAnimatedWithPhysicsGameObject__vnullsub_51
    BaseAliveGameObject__Render_424B90
    BaseGameObject__vsub_4DC0A0
    BaseGameObject__vnullsub_4DC0F0
    BaseGameObject__GetSaveState_4DC110
    BaseAliveGameObject__vsub_424EE0
    BaseAliveGameObject__vsub_424FD0
    BaseAliveGameObject__vsub_4253B0
    BaseAliveGameObject__vsub_425420
    BaseAliveGameObject__vsub_4254A0
    BaseAliveGameObject__vsub_425520
    BaseAliveGameObject__vsub_425840
    BaseAliveGameObject__vnullsub_53
    BaseAliveGameObject__vnullsub_54
    */


    AnimationEx field_20_animation;
    FP field_B8_xpos;
    FP field_BC_ypos;
    __int16 field_C0_path_number;
    __int16 field_C2_lvl_number;
    int field_C4_velx;
    int field_C8_vely;
    FP field_CC_sprite_scale;
    __int16 field_D0_r;
    __int16 field_D2_g;
    __int16 field_D4_b;
    __int16 field_D6_scale;
    __int16 field_D8_yOffset;
    __int16 field_DA_xOffset;
    __int16 field_DC_bApplyShadows;
    __int16 field_DE_pad;
    void* field_E0_176_ptr;
};
ALIVE_ASSERT_SIZEOF(BaseAnimatedWithPhysicsGameObject, 0xE4);
