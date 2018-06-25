#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "Psx.hpp"

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
