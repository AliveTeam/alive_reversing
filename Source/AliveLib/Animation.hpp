#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "Psx.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"

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
    DWORD mFrameHeaderOffset = 0;
    DWORD mMagic = 0;

    // Collision bounding rectangle
    Point mTopLeft;
    Point mBottomRight;

    WORD mOffx = 0;
    WORD mOffy = 0;
};

class Animation
{
public:
    EXPORT virtual void Animation__vdecode_40AC90();
    EXPORT virtual char Animation_v_40B820(signed int a2, int a3, int a4, __int16 a5, signed int op1);
    EXPORT virtual signed __int16 Animationv_40C630();
   
    EXPORT virtual __int16 Animationv_40B200();
    EXPORT virtual char Animation_v_40BEE0(__int16 a2, __int16 a3, int a4, __int16 a5, __int16 op1);

    EXPORT signed __int16 Set_Animation_Data_409C80(int frameTableOffset, BYTE **pAnimRes);
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

struct BanHeader
{
    WORD mMaxW = 0;
    WORD mMaxH = 0;
    DWORD mFrameTableOffSet = 0;
    DWORD mPaltSize = 0;
};

struct FrameHeader
{
    DWORD mClutOffset;
    BYTE mWidth;
    BYTE mHeight;
    BYTE mColourDepth;
    BYTE mCompressionType;
    WORD mWidth2;
    WORD mHeight2;
};

class AnimationEx : public Animation
{
public:
    WORD field_10_frame_delay;
    WORD field_12_scale; // padding?
    FP field_14_scale;
    DWORD field_18_frame_table_offset;
    DWORD field_1C_fn_ptr_array;
    BYTE** field_20_ppBlock; // // pointer to a pointer which points to anim data
    DWORD field_24_dbuf;
    
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
    EXPORT signed __int16 Init_40A030(int frameTableOffset, DynamicArray *animList, void *pGameObj, int maxW, unsigned __int16 maxH, BYTE **ppAnimData, unsigned __int8 unknown1, signed int pal_depth, char unknown3);
    //EXPORT void Get_Bounding_Rect_Top_Left_40C480(signed __int16 frameNum, __int16* pBoundingX, __int16* pBoundingY);
    EXPORT void Load_Pal_40A530(BYTE** pAnimData, int palOffset);
};
ALIVE_ASSERT_SIZEOF(AnimationEx, 0x98);
