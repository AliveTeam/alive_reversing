#pragma once

#include "Primitives.hpp"

enum AnimFlags
{
    // Bit 1 = Some unknown effect on frame ordering.
    eBit1 = 0x1,

    // Bit 2 = Visible/render flag?
    eBit2_Animate = 0x2,

    // Bit 3 = Nothing?
    eBit3_Render = 0x4,

    // Bit 4 = Horizontal flip.
    eBit4 = 0x8,

    // Bit 5 = Vertical flip.
    eBit5_FlipX = 0x10,

    // Bit 6 = Nothing?
    eBit6_FlipY = 0x20,

    // Bit 7 = Loop.
    eBit7_SwapXY = 0x40,

    // Bit 8 = ?
    eBit8_Loop = 0x80,

    // Bit 9 = ?
    eBit9 = 0x100,

    // Bit 10 = ?
    eBit10_alternating_flag = 0x200,

    // Bit 11 = ?
    eBit11_bToggle_Bit10 = 0x400,

    // Bit 12 = Prevents updating or gets anims stuck??
    eBit12_ForwardLoopCompleted = 0x800,

    // Bit 13 = Colour depth related.
    eBit13_Is8Bit = 0x1000,

    // Bit 14 = Transparency enabled.
    eBit14_Is16Bit = 0x2000,

    // Bit 15 = Disable RGB changes? Shadows no longer work, all Muds look like Abe?
    eBit15_bSemiTrans = 0x4000,

    // Bit 16 = nothing ?
    eBit16_bBlending = 0x8000,

    // Bit 17 = Is last frame? Causes instance chiselling of muds.
    eBit17 = 0x10000,

    // Bit 18 = Backwards (or forwards?) frame order.
    eBit18_IsLastFrame = 0x20000,

    // Bit 19 = Offset Y by H?
    eBit19_LoopBackwards = 0x40000,

    // Bit 20 = Nothing?
    eBit20_use_xy_offset = 0x80000,

    // Bit 21 = Hang/crash.
    eBit21 = 0x100000,

    // Bit 22 = Nothing?
    eBit22_DeadMode = 0x200000,

    // Bit 23 = Nothing?
    eBit23 = 0x400000,

    // Bit 24 = Display vram?
    eBit24 = 0x800000,

    eBit25_bDecompressDone = 0x1000000,

    // Bit 26-32 = Nothing?
};

class AnimationBase
{
public:
    // TODO: Virtuals must be on the base type, yet there is only 1 vtable pointing to derived?
    virtual void vDecode_40AC90();

    virtual void vRender_40B820(int xpos, int ypos, PrimHeader** ppOt, __int16 width, signed int height);
    virtual void vCleanUp_40C630() = 0;

    // TODO: Restore vTable entry.
    //EXPORT virtual __int16 Animationv_40B200();
    EXPORT virtual char Animation_v_40BEE0(__int16 a2, __int16 a3, int a4, __int16 a5, __int16 op1);

    EXPORT static void CC AnimateAll_40AC20(DynamicArrayT<AnimationBase>* pAnimations);

    BitField32<AnimFlags> field_4_flags;
    BYTE field_8_r;
    BYTE field_9_g;
    BYTE field_A_b;
    BYTE field_B_render_mode;
    WORD field_C_render_layer;
    WORD field_E_frame_change_counter;
};
ALIVE_ASSERT_SIZEOF(AnimationBase, 0x10);
