#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"
#include "DynamicArray.hpp"
#include "BitField.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

namespace AO {

using TFrameCallBackType = short* (CC*)(void*, __int16*);

extern TFrameCallBackType kAbe_Anim_Frame_Fns_4CEBEC[];
extern TFrameCallBackType kSlig_Anim_Frame_Fns_4CEBF0[];
extern TFrameCallBackType kSlog_Anim_Frame_Fns_4CEBF4[];
extern TFrameCallBackType kZBall_Anim_Frame_Fns_4CEBF8[];


struct AnimHeader
{
    __int16 field_0_max_w;
    __int16 field_2_max_h;
    int field_4_frame_table_offset;
};
ALIVE_ASSERT_SIZEOF(AnimHeader, 0x8);

struct AnimationHeader
{
    // Meta data - the offset where this record was read from
    WORD field_0_fps;            // Seems to be 0x1 or 0x2
    short field_2_num_frames;      // Number of frames in the set

                                       // If loop flag set then this is the frame to loop back to
    short field_4_loop_start_frame;

    // These where reversed by editing data in memory on PSX version
    enum eFlags
    {
        eFlipXFlag = 0x4,
        eFlipYFlag = 0x8,
        eNeverUnload = 0x1,
        eLoopFlag = 0x2
    };
    WORD field_6_flags;
    DWORD mFrameOffsets[1]; // Reading past 1 is UB.. will need to change this later (copy out the data or something)
};

enum class CompressionType : BYTE
{
    eType_0_NoCompression = 0,
    eType_1_NotUsed = 1,
    eType_2_ThreeToFourBytes = 2,
    eType_3_RLE_Blocks = 3,
    eType_4_RLE = 4,
    eType_5_RLE = 5,
};

struct FrameHeader
{
    DWORD field_0_clut_offset;
    BYTE field_4_width;
    BYTE field_5_height;
    BYTE field_6_colour_depth;
    CompressionType field_7_compression_type;
    WORD field_8_width2;
    WORD mHeight2;
};

struct Point
{
    __int16 x = 0;
    __int16 y = 0;
};



struct OffsetAndBoundingRect
{
    Point mOffset;
    Point mMin;
    Point mMax;
};

union PointsUnion
{
    PointsUnion() {}
    OffsetAndBoundingRect offsetAndRect;
    Point points[3];
};

struct FrameInfoHeader
{
    DWORD field_0_frame_header_offset;
    short field_4_magic;
    short field_6_count;
    PointsUnion field_8_data;
};

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
    eBit17_bFreeResource = 0x10000,

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
    virtual void vDecode() = 0;

    virtual void vRender(int xpos, int ypos, PrimHeader** ppOt, __int16 width, __int16 height) = 0;

    virtual void vCleanUp() = 0;


    static EXPORT void CC AnimateAll_4034F0(DynamicArrayT<AnimationBase>* pAnimList);

    BitField32<AnimFlags> field_4_flags;

    BYTE field_8_r;
    BYTE field_9_g;
    BYTE field_A_b;
    TPageAbr field_B_render_mode;

    Layer field_C_layer;
    __int16 field_E_frame_change_counter;
};

class BaseGameObject;

class Animation : public AnimationBase
{
public:
    virtual void vDecode() override;

    virtual void vRender(int xpos, int ypos, PrimHeader** ppOt, __int16 width, __int16 height) override;

    EXPORT void VRender_403AE0(int xpos, int ypos, PrimHeader** ppOt, __int16 width, __int16 height);

    EXPORT void VDecode_403550();

    EXPORT signed __int16 Set_Animation_Data_402A40(int frameTable, BYTE** resBlock);

    EXPORT void SetFrame_402AC0(__int16 newFrame);

    EXPORT signed __int16 Init_402D20(int frameTableOffset, DynamicArray* animList, BaseGameObject* pGameObj, unsigned __int16 maxW, unsigned __int16 maxH, BYTE** ppAnimData, unsigned __int8 bFlag_17, signed int b_StartingAlternationState, char bEnable_flag10_alternating);

    EXPORT __int16 Get_Frame_Count_403540();

    EXPORT FrameInfoHeader* Get_FrameHeader_403A00(int frame);

    EXPORT void LoadPal_403090(BYTE** pPalData, int palOffset);

    EXPORT void Get_Frame_Rect_402B50(PSX_RECT* pRect);

    EXPORT void Get_Frame_Width_Height_403E80(short* pWidth, short* pHeight);

    EXPORT void Get_Frame_Offset_403EE0(short* pBoundingX, short* pBoundingY);

    virtual void vCleanUp() override;

    EXPORT void VCleanUp_403F40();

    bool EnsureDecompressionBuffer();

    void UploadTexture(const FrameHeader* pFrameHeader, const PSX_RECT& vram_rect, short width_bpp_adjusted);

    int field_10_frame_delay;
    FP field_14_scale;
    int field_18_frame_table_offset;
    TFrameCallBackType* field_1C_fn_ptr_array;
    BYTE** field_20_ppBlock;
    BYTE** field_24_dbuf;
    int field_28_dbuf_size;
    Poly_FT4 field_2C_ot_data[2];
    PSX_RECT field_84_vram_rect;
    PSX_Point field_8C_pal_vram_xy;
    __int16 field_90_pal_depth;
    __int16 field_92_current_frame;
    BaseGameObject* field_94_pGameObj;
};
ALIVE_ASSERT_SIZEOF(Animation, 0x98);


// TODO: Figure out how this differs from the standard Animation
class AnimationUnknown : public AnimationBase
{
public:
    virtual void vDecode() override;

    virtual void vRender(int xpos, int ypos, PrimHeader** ppOt, __int16 width, __int16 height) override;

    virtual void vCleanUp() override;

    virtual void VRender2(int xpos, int ypos, PrimHeader** ppOt);

    EXPORT void GetRenderedSize_404220(PSX_RECT* pRect);

private:
    EXPORT void VCleanUp2_404280();

    // New virtual
    EXPORT void VRender2_403FD0(int xpos, int ypos, PrimHeader** ppOt);

public:
    Poly_FT4 field_10_polys[2];
    Animation* field_68_anim_ptr;
    FP field_6C_scale;
};
ALIVE_ASSERT_SIZEOF(AnimationUnknown, 0x70);



}
