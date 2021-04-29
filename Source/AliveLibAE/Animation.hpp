#pragma once

#include "AnimationBase.hpp"
#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "Psx.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

using TFrameCallBackType = s32(CC*)(void*, s16*);

extern TFrameCallBackType kAbe_Anim_Frame_Fns_55EF98[5];
extern TFrameCallBackType kSlig_Anim_Frame_Fns_55EFAC[4];
extern TFrameCallBackType kSlog_Anim_Frame_Fns_55EFBC[2];
extern TFrameCallBackType kFlyingSlig_Anim_Frames_Fns_55EFC4[3];
extern TFrameCallBackType kFleech_Anim_Frame_Fns_55EFD0[3];

namespace AETest::TestsAnimation {
void AnimationTests();
}

struct AnimHeader final
{
    s16 field_0_max_w;
    s16 field_2_max_h;
    s32 field_4_frame_table_offset;
};
ALIVE_ASSERT_SIZEOF(AnimHeader, 0x8);

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
//ALIVE_ASSERT_SIZEOF(AnimationHeader, 0x8);

struct BanHeader final
{
    u16 mMaxW = 0;
    u16 mMaxH = 0;
    u32 mFrameTableOffSet = 0;
    u32 mPaltSize = 0;
};

enum class CompressionType : u8
{
    eType_0_NoCompression = 0,
    eType_1_NotUsed = 1,
    eType_2_ThreeToFourBytes = 2,
    eType_3_RLE_Blocks = 3,
    eType_4_RLE = 4,
    eType_5_RLE = 5,
    eType_6_RLE = 6,
    eType_7_NotUsed = 7,
    eType_8_NotUsed = 8,
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

class Animation final : public AnimationBase
{
public:
    EXPORT s16 Set_Animation_Data_409C80(s32 frameTableOffset, u8** pAnimRes);
    EXPORT void Animation_Pal_Free_40C4C0();

    bool EnsureDecompressionBuffer();
    void DecompressFrame();

    EXPORT virtual void vRender_40B820(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s32 height) override;
    EXPORT virtual void vCleanUp_40C630() override;

    EXPORT void vDecode2_40B200();
    EXPORT virtual void vDecode_40AC90() override;
    bool DecodeCommon();

    EXPORT void Invoke_CallBacks_40B7A0();

    void UploadTexture(const FrameHeader* pFrameHeader, const PSX_RECT& vram_rect, s16 width_bpp_adjusted);

    u16 field_10_frame_delay;
    u16 field_12_scale; // padding?
    FP field_14_scale;
    u32 field_18_frame_table_offset;
    s32(CC** field_1C_fn_ptr_array)(void*, s16*);
    u8** field_20_ppBlock; // // pointer to a pointer which points to anim data
    u8** field_24_dbuf;

    u32 field_28_dbuf_size;
    Poly_FT4 field_2C_ot_data[2];

    PSX_RECT field_84_vram_rect;
    PSX_Point field_8C_pal_vram_xy;

    s16 field_90_pal_depth;
    s16 field_92_current_frame;
    BaseGameObject* field_94_pGameObj;

public:
    EXPORT void SetFrame_409D50(s16 newFrame);
    EXPORT FrameInfoHeader* Get_FrameHeader_40B730(s16 frame);
    EXPORT void Get_Frame_Rect_409E10(PSX_RECT* pRect);
    EXPORT u16 Get_Frame_Count_40AC70();
    EXPORT s16 Init_40A030(s32 frameTableOffset, DynamicArray* animList, BaseGameObject* pGameObj, u16 maxW, u16 maxH, u8** ppAnimData, u8 unknown1, s32 pal_depth, s8 unknown3);
    EXPORT void Get_Frame_Offset_40C480(s16* pBoundingX, s16* pBoundingY);
    EXPORT void Get_Frame_Width_Height_40C400(s16* pWidth, s16* pHeight);

    EXPORT void Load_Pal_40A530(u8** pAnimData, s32 palOffset);
};
ALIVE_ASSERT_SIZEOF(Animation, 0x98);

inline bool IsLastFrame(const Animation* pAnim)
{
    const u8* pAnimData = (*pAnim->field_20_ppBlock);
    const auto pHeader = reinterpret_cast<const AnimationHeader*>(&pAnimData[pAnim->field_18_frame_table_offset]);
    return (pAnim->field_92_current_frame == pHeader->field_2_num_frames - 1);
}
