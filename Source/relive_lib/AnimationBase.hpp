#pragma once

#include "Primitives.hpp"
#include "Layer.hpp"
#include "ResourceManagerWrapper.hpp"
#include "DynamicArray.hpp"
#include "../AliveLibCommon/BitField.hpp"

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
   // eBit9 = 0x100,

    // Bit 10 = ?
    eBit10_alternating_flag = 0x200,

    // Bit 11 = ?
    //eBit11_bToggle_Bit10 = 0x400,

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
    eBit17_bOwnPal = 0x10000,

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

    //eBit25_bDecompressDone = 0x1000000,

    // Bit 26-32 = Nothing?
};

class AnimationBase
{
public:
    static void CreateAnimationArray();
    static void FreeAnimationArray();

    virtual ~AnimationBase() { }

    virtual void VDecode() = 0;

    virtual void VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s32 height) = 0;

    virtual void VCleanUp() = 0;

    static void AnimateAll(DynamicArrayT<AnimationBase>* pAnimations);

    BitField32<AnimFlags> mFlags = {};
    AnimResource mAnimRes;
    u8 mRed = 0;
    u8 mGreen = 0;
    u8 mBlue = 0;
    TPageAbr mRenderMode = TPageAbr::eBlend_0;
    Layer mRenderLayer = Layer::eLayer_0;
    u32 mFrameChangeCounter = 0;

    static DynamicArrayT<AnimationBase>* gAnimations;
};
