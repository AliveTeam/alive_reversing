#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Animation.hpp"

class BackgroundAnimation : public BaseAnimatedWithPhysicsGameObject
{
public:
    // VTable @ 0x5440F0
    /*
    // dtor
    BackgroundAnimation__dtor_40D420

    // update
    BackgroundAnimation__vsub_40D450

    // render
    BaseAliveGameObject__Render_424B90

    // ??
    BackgroundAnimation__vsub_40D550

    // all base past here
    */

    void ctor_40D270(BackgroundAnimation_Params* pPathParams, int a3);

    /*
    void vsub_40D450()
    {
    if (sub_422C00(5))
    {
    field_6_flags |= 4u;
    }
    else
    {
    field_B8_xpos = (sTweakX_5C1BD0) + field_FC_xpos;
    field_BC_ypos = (sTweakY_5C1BD4) + field_100_ypos;
    }
    }
    */

    void vsub_40D550()
    {
        field_6_flags |= BaseGameObject::eDead;
    }

    void dtor_40D4C0()
    {
        //sub_4DB8E0(this->field_F8_arg_a3, -1, 0, 0);
        //BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    /*
    void dtor_40D420(signed int flags)
    {
    dtor_40D4C0();
    if (flags & 1)
    {
    Mem_Free_495540(this);
    }
    }
    */

    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    AnimHeader **field_F4_res;
    int field_F8_arg_a3;
    FP field_FC_xpos;
    FP field_100_ypos;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x104);
