#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "Psx.hpp"
#include "BaseGameObject.hpp"

class Animation
{
public:
    EXPORT virtual void Animation__vdecode_40AC90();
    EXPORT virtual char Animation_v_40B820(signed int a2, int a3, int a4, __int16 a5, signed int op1);
    EXPORT virtual signed __int16 Animationv_40C630();
   
    EXPORT virtual __int16 Animationv_40B200();
    EXPORT virtual char Animation_v_40BEE0(__int16 a2, __int16 a3, int a4, __int16 a5, __int16 op1);


    EXPORT static void CC AnimateAll_40AC20(DynamicArrayT<Animation>* pAnimations);

    DWORD field_4_flags;
    BYTE field_8_r;
    BYTE field_9_g;
    BYTE field_A_b;
    BYTE field_B_render_mode;
    WORD field_C_render_layer;
    WORD field_E_frame_change_counter;
};
ALIVE_ASSERT_SIZEOF(Animation, 0x10);



#pragma pack(push)
#pragma pack(2)
class AnimationEx : public Animation
{
    WORD field_10_frame_delay;
    DWORD field_12_scale;
    DWORD field_16_dataOffset;
    WORD field_1A;
    DWORD field_1C_fn_ptr_array;
    DWORD field_20_ppBlock; // // pointer to a pointer which points to anim data
    DWORD field_24_dbuf;
    
    WORD field_28_dbuf_size;

    BYTE field_2A[38];
    BYTE field_50[38];

    DWORD field_76_pad;

    PSX_RECT field_7A;
    PSX_RECT field_82;

    WORD field_8A_x;
    WORD field_8C_y;
    WORD field_8E_num_cols;
    WORD field_90_pad; // padding ??
    WORD field_92_current_frame;
    DWORD field_94_unknown_pointer; // Pointer to something
};
ALIVE_ASSERT_SIZEOF(AnimationEx, 0x98);
#pragma pack(pop)

class BaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
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
    int field_B8_xpos;
    int field_BC_ypos;
    __int16 field_C0_path_number;
    __int16 field_C2_lvl_number;
    int field_C4_velx;
    int field_C8_vely;
    int field_CC_sprite_scale;
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

struct AnimHeader
{
    __int16 field_0_max_w;
    __int16 field_2_max_h;
    int field_4_frame_table_offset;
};
ALIVE_ASSERT_SIZEOF(AnimHeader, 0x8);

struct BackgroundAnimation_Params
{
    __int16 field_0_flags;
    __int16 field_2_length;
    int field_4_type;
    __int16 field_8_xpos;
    __int16 field_A_ypos;
    __int16 field_C_width;
    __int16 field_E_height;
    unsigned __int16 field_10_res_id;
    __int16 field_12_is_semi_trans;
    __int16 field_14_semi_trans_mode;
    __int16 field_16_sound_effect;
    __int16 field_18_id;
    unsigned __int16 field_1A_layer;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation_Params, 0x1C);


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
            field_B8_xpos = (sTweakX_5C1BD0 << 16) + field_FC_xpos;
            field_BC_ypos = (sTweakY_5C1BD4 << 16) + field_100_ypos;
        }
    }
    */

    void vsub_40D550()
    {
        field_6_flags |= 4u;
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
    int field_FC_xpos;
    int field_100_ypos;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x104);
