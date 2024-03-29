#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "FixedPoint.hpp"
#include "DynamicArray.hpp"
#include "../AliveLibCommon/BitField.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

namespace AO {

using TFrameCallBackType = s16*(CC*) (void*, s16*);

extern TFrameCallBackType kAbe_Anim_Frame_Fns_4CEBEC[];
extern TFrameCallBackType kSlig_Anim_Frame_Fns_4CEBF0[];
extern TFrameCallBackType kSlog_Anim_Frame_Fns_4CEBF4[];
extern TFrameCallBackType kZBall_Anim_Frame_Fns_4CEBF8[];


struct AnimHeader final
{
    s16 field_0_max_w;
    s16 field_2_max_h;
    s32 field_4_frame_table_offset;
};
ALIVE_ASSERT_SIZEOF(AnimHeader, 0x8);

struct AnimationHeader final
{
    // Meta data - the offset where this record was read from
    u16 field_0_fps;        // Seems to be 0x1 or 0x2
    s16 field_2_num_frames; // Number of frames in the set

    // If loop flag set then this is the frame to loop back to
    s16 field_4_loop_start_frame;

    // These where reversed by editing data in memory on PSX version
    enum eFlags
    {
        eFlipXFlag = 0x4,
        eFlipYFlag = 0x8,
        eNeverUnload = 0x1,
        eLoopFlag = 0x2
    };
    u16 field_6_flags;
    u32 mFrameOffsets[1]; // Reading past 1 is UB.. will need to change this later (copy out the data or something)
};

enum class CompressionType : u8
{
    eType_0_NoCompression = 0,
    eType_1_NotUsed = 1,
    eType_2_ThreeToFourBytes = 2,
    eType_3_RLE_Blocks = 3,
    eType_4_RLE = 4,
    eType_5_RLE = 5,
};

struct FrameHeader final
{
    u32 field_0_clut_offset;
    u8 field_4_width;
    u8 field_5_height;
    u8 field_6_colour_depth;
    CompressionType field_7_compression_type;
    u16 field_8_width2;
    u16 mHeight2;
};

struct Point final
{
    s16 x = 0;
    s16 y = 0;
};



struct OffsetAndBoundingRect final
{
    Point mOffset;
    Point mMin;
    Point mMax;
};

union PointsUnion
{
    PointsUnion()
    { }
    OffsetAndBoundingRect offsetAndRect;
    Point points[3];
};

struct FrameInfoHeader final
{
    u32 field_0_frame_header_offset;
    s16 field_4_magic;
    s16 field_6_count;
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

    virtual void vRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s16 height) = 0;

    virtual void vCleanUp() = 0;


    static EXPORT void CC AnimateAll_4034F0(DynamicArrayT<AnimationBase>* pAnimList);

    BitField32<AnimFlags> field_4_flags;

    u8 field_8_r;
    u8 field_9_g;
    u8 field_A_b;
    TPageAbr field_B_render_mode;

    Layer field_C_layer;
    s16 field_E_frame_change_counter;
};

class BaseGameObject;

class Animation final : public AnimationBase
{
public:
    virtual void vDecode() override;

    virtual void vRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s16 height) override;

    EXPORT void VRender_403AE0(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s16 height);

    EXPORT void VDecode_403550();

    EXPORT s16 Set_Animation_Data_402A40(s32 frameTable, u8** resBlock);

    EXPORT void SetFrame_402AC0(s16 newFrame);

    EXPORT s16 Init_402D20(s32 frameTableOffset, DynamicArray* animList, BaseGameObject* pGameObj, u16 maxW, u16 maxH, u8** ppAnimData, u8 bFlag_17, s32 b_StartingAlternationState, s8 bEnable_flag10_alternating);

    EXPORT s16 Get_Frame_Count_403540();

    EXPORT FrameInfoHeader* Get_FrameHeader_403A00(s32 frame);

    EXPORT void LoadPal_403090(u8** pPalData, s32 palOffset);

    EXPORT void Get_Frame_Rect_402B50(PSX_RECT* pRect);

    EXPORT void Get_Frame_Width_Height_403E80(s16* pWidth, s16* pHeight);

    EXPORT void Get_Frame_Offset_403EE0(s16* pBoundingX, s16* pBoundingY);

    virtual void vCleanUp() override;

    EXPORT void VCleanUp_403F40();

    bool EnsureDecompressionBuffer();

    void UploadTexture(const FrameHeader* pFrameHeader, const PSX_RECT& vram_rect, s16 width_bpp_adjusted);

    s32 field_10_frame_delay;
    FP field_14_scale;
    s32 field_18_frame_table_offset;
    TFrameCallBackType* field_1C_fn_ptr_array;
    u8** field_20_ppBlock;
    u8** field_24_dbuf;
    s32 field_28_dbuf_size;
    Poly_FT4 field_2C_ot_data[2];
    PSX_RECT field_84_vram_rect;
    PSX_Point field_8C_pal_vram_xy;
    s16 field_90_pal_depth;
    s16 field_92_current_frame;
    BaseGameObject* field_94_pGameObj;
};
ALIVE_ASSERT_SIZEOF(Animation, 0x98);


// TODO: Figure out how this differs from the standard Animation
class AnimationUnknown final : public AnimationBase
{
public:
    virtual void vDecode() override;

    virtual void vRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s16 height) override;

    virtual void vCleanUp() override;

    virtual void VRender2(s32 xpos, s32 ypos, PrimHeader** ppOt);

    EXPORT void GetRenderedSize_404220(PSX_RECT* pRect);

private:
    EXPORT void VCleanUp2_404280();

    // New virtual
    EXPORT void VRender2_403FD0(s32 xpos, s32 ypos, PrimHeader** ppOt);

public:
    Poly_FT4 field_10_polys[2];
    Animation* field_68_anim_ptr;
    FP field_6C_scale;
};
ALIVE_ASSERT_SIZEOF(AnimationUnknown, 0x70);



} // namespace AO
