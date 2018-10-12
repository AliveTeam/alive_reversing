#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "Psx.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"

namespace Test
{
    void AnimationTests();
}

struct AnimHeader
{
    __int16 field_0_max_w;
    __int16 field_2_max_h;
    int field_4_frame_table_offset;
};
ALIVE_ASSERT_SIZEOF(AnimHeader, 0x8);

struct Point
{
    __int16 x = 0;
    __int16 y = 0;
};

struct FrameInfoHeader
{
    DWORD field_0_frame_header_offset = 0;
    short field_4_magic = 0;
    short field_6_count = 0;

    WORD mOffx = 0;
    WORD mOffy = 0;

    // Collision bounding rectangle
    Point mTopLeft; // x0, y0
    Point mBottomRight; // x1, y2
};


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
    eBit7 = 0x40,

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
    eBit20 = 0x80000,

    // Bit 21 = Hang/crash
    eBit21 = 0x100000,

    // Bit 22 = nothing ?
    eBit22_DeadMode = 0x200000,

    // Bit 23 = nothing ?
    eBit23 = 0x400000,

    // Bit 24 = Display vram ?
    eBit24 = 0x800000,

    eBit25_NotUsedMode = 0x1000000,

    // Bit 26-32 = nothing ?
};

class Animation
{
public:
    // TODO: Virtuals must be on the base type, yet there is only 1 vtable pointing to derived ?
    virtual void vDecode_40AC90();

    virtual void vRender_40B820(int xpos, int ypos, int** pOt, __int16 width, signed int height);
    EXPORT virtual signed __int16 Animationv_40C630();
   
    // TODO: Restore vTable entry
    //EXPORT virtual __int16 Animationv_40B200();
    EXPORT virtual char Animation_v_40BEE0(__int16 a2, __int16 a3, int a4, __int16 a5, __int16 op1);

    EXPORT static void CC AnimateAll_40AC20(DynamicArrayT<Animation>* pAnimations);

    BitField32<AnimFlags> field_4_flags;
    BYTE field_8_r;
    BYTE field_9_g;
    BYTE field_A_b;
    BYTE field_B_render_mode;
    WORD field_C_render_layer;
    WORD field_E_frame_change_counter;
};
ALIVE_ASSERT_SIZEOF(Animation, 0x10);

struct BanHeader
{
    WORD mMaxW = 0;
    WORD mMaxH = 0;
    DWORD mFrameTableOffSet = 0;
    DWORD mPaltSize = 0;
};

struct FrameHeader
{
    DWORD field_0_clut_offset;
    BYTE field_4_width;
    BYTE field_5_height;
    BYTE field_6_colour_depth;
    BYTE field_7_compression_type;
    WORD field_8_width2;
    WORD mHeight2;
};

class AnimationEx : public Animation
{
public:
    EXPORT signed __int16 Set_Animation_Data_409C80(int frameTableOffset, BYTE **pAnimRes);
    EXPORT void Animation_Pal_Free_40C4C0();

    bool EnsureDecompressionBuffer();
    void DecompressFrame_VramAlloc();
    void DecompressFrame();

    EXPORT virtual void vRender_40B820(int xpos, int ypos, int** pOt, __int16 width, signed int height) override;

    EXPORT void vDecode2_40B200();
    EXPORT void vDecode_40AC90();
    bool DecodeCommon();

    EXPORT void Invoke_CallBacks_40B7A0();

    WORD field_10_frame_delay;
    WORD field_12_scale; // padding?
    FP field_14_scale;
    DWORD field_18_frame_table_offset;
    int(__cdecl **field_1C_fn_ptr_array)(void *, __int16 *);
    BYTE** field_20_ppBlock; // // pointer to a pointer which points to anim data
    BYTE** field_24_dbuf;
    
    DWORD field_28_dbuf_size;
    Poly_FT4 field_2C_ot_data[2];

    PSX_RECT field_84_vram_rect;
    PSX_Point field_8C_pal_vram_x;

    __int16 field_90_pal_depth;
    __int16 field_92_current_frame;
    void *field_94_pGameObj;

public:
    EXPORT void SetFrame_409D50(__int16 newFrame);
    EXPORT FrameInfoHeader* Get_FrameHeader_40B730(__int16 frame);
    EXPORT void Get_Frame_Rect_409E10(PSX_RECT *pRect);
    EXPORT WORD Get_Frame_Count_40AC70();
    EXPORT signed __int16 Init_40A030(int frameTableOffset, DynamicArray *animList, void *pGameObj, unsigned __int16 maxW, unsigned __int16 maxH, BYTE **ppAnimData, unsigned __int8 unknown1, signed int pal_depth, char unknown3);
    //EXPORT void Get_Bounding_Rect_Top_Left_40C480(signed __int16 frameNum, __int16* pBoundingX, __int16* pBoundingY);
    EXPORT void Load_Pal_40A530(BYTE** pAnimData, int palOffset);
};
ALIVE_ASSERT_SIZEOF(AnimationEx, 0x98);
