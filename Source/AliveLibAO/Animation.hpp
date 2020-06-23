#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"
#include "DynamicArray.hpp"
#include "BitField.hpp"

START_NS_AO

using TFrameCallBackType = short* (CC*)(void*, __int16*);

// TODO: Assumed to be the same as AE - test this, some do match
enum AnimFlags
{
    // Bit 1 = some unknown effect on frame ordering
    eBit1 = 0x1,

    // Bit 2 = Visible/render flag?
    eBit2_Animate = 0x2,

    // Bit 3 = nothing ?
    eBit3_Render = 0x4,

    // Bit 4 = Horizontal flip
    eBit4 = 0x8,

    // Bit 5 = Vertical flip
    eBit5_FlipX = 0x10,

    // Bit 6 = nothing ?
    eBit6_FlipY = 0x20,

    // Bit 7 = loop
    eBit7_SwapXY = 0x40,

    // Bit 8 = ?
    eBit8_Loop = 0x80,

    // Bit 9 = ?
    eBit9 = 0x100,

    // Bit 10 = ?
    eBit10_alternating_flag = 0x200,

    // Bit 11 = ?
    eBit11_bToggle_Bit10 = 0x400,

    // Bit 12 = prevents updating or gets anims stuck ??
    eBit12_ForwardLoopCompleted = 0x800,

    // Bit 13 = colour depth related
    eBit13_Is8Bit = 0x1000,

    // Bit 14 = transparency enabled
    eBit14_Is16Bit = 0x2000,

    // Bit 15 = disable RGB changes? Shadows no longer work, all muds look like abe ?
    eBit15_bSemiTrans = 0x4000,

    // Bit 16 = nothing ?
    eBit16_bBlending = 0x8000,

    // Bit 17 = is last frame? causes instance chiseling of muds
    eBit17 = 0x10000,

    // Bit 18 = backwards (or forwards?) frame order
    eBit18_IsLastFrame = 0x20000,

    // Bit 19 = offset Y by H?
    eBit19_LoopBackwards = 0x40000,

    // Bit 20 = nothing ?
    eBit20_use_xy_offset = 0x80000,

    // Bit 21 = Hang/crash
    eBit21 = 0x100000,

    // Bit 22 = nothing ?
    eBit22_DeadMode = 0x200000,

    // Bit 23 = nothing ?
    eBit23 = 0x400000,

    // Bit 24 = Display vram ?
    eBit24 = 0x800000,

    eBit25_bDecompressDone = 0x1000000,

    // Bit 26-32 = nothing ?
};


class AnimationBase
{
public:
    // TODO: Non empty so standalone builds
    virtual void vDecode() {}
    virtual void vRender(int /*xpos*/, int /*ypos*/, int** /*pOt*/, __int16 /*width*/, signed int /*height*/) {}
    virtual void vCleanUp() {} // TODO: pure

    static EXPORT void CC AnimateAll_4034F0(DynamicArrayT<AnimationBase>* pAnimList);
};

class BaseGameObject;

class Animation : public AnimationBase
{
public:

    EXPORT void SetFrame_402AC0(unsigned __int16 frame);

    EXPORT signed __int16 Init_402D20(int frameTableOffset, DynamicArray* animList, BaseGameObject* pGameObj, unsigned __int16 maxW, unsigned __int16 maxH, BYTE** ppAnimData, unsigned __int8 bFlag_17, signed int b_StartingAlternationState, char bEnable_flag10_alternating);

    BitField32<AnimFlags> field_4_flags;
    
    char field_8;
    char field_9;
    char field_A;
    char field_B_render_mode;

    __int16 field_C_layer;
    __int16 field_E; // frame counter ?
    // TODO: Above data part of base ?

    int field_10;
    FP field_14_scale;
    int field_18;
    TFrameCallBackType* field_1C_fn_ptrs;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    __int16 field_84;
    __int16 field_86;
    __int16 field_88;
    __int16 field_8A;
    __int16 field_8C_vram_x_pal;
    __int16 field_8E_vram_y_pal;
    __int16 field_90_pal_depth;
    __int16 field_92_current_frame;
    int field_94;
};
ALIVE_ASSERT_SIZEOF(Animation, 0x98);

END_NS_AO
