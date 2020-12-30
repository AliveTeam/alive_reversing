#pragma once

#include "AnimationBase.hpp"
#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "Psx.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

using TFrameCallBackType = int(CC *)(void *, __int16 *);

extern TFrameCallBackType kAbe_Anim_Frame_Fns_55EF98[5];
extern TFrameCallBackType kSlig_Anim_Frame_Fns_55EFAC[4];
extern TFrameCallBackType kSlog_Anim_Frame_Fns_55EFBC[2];
extern TFrameCallBackType kFlyingSlig_Anim_Frames_Fns_55EFC4[3];
extern TFrameCallBackType kFleech_Anim_Frame_Fns_55EFD0[3];

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
//ALIVE_ASSERT_SIZEOF(AnimationHeader, 0x8);

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

class Animation : public AnimationBase
{
public:
    EXPORT signed __int16 Set_Animation_Data_409C80(int frameTableOffset, BYTE** pAnimRes);
    EXPORT void Animation_Pal_Free_40C4C0();

    bool EnsureDecompressionBuffer();
    void DecompressFrame();

    EXPORT virtual void vRender_40B820(int xpos, int ypos, int** pOt, __int16 width, signed int height) override;
    EXPORT virtual void vCleanUp_40C630() override;

    EXPORT void vDecode2_40B200();
    EXPORT virtual void vDecode_40AC90() override;
    bool DecodeCommon();

    EXPORT void Invoke_CallBacks_40B7A0();

    WORD field_10_frame_delay;
    WORD field_12_scale; // padding?
    FP field_14_scale;
    DWORD field_18_frame_table_offset;
    int(CC **field_1C_fn_ptr_array)(void *, __int16 *);
    BYTE** field_20_ppBlock; // // pointer to a pointer which points to anim data
    BYTE** field_24_dbuf;
    
    DWORD field_28_dbuf_size;
    Poly_FT4 field_2C_ot_data[2];

    PSX_RECT field_84_vram_rect;
    PSX_Point field_8C_pal_vram_xy;

    __int16 field_90_pal_depth;
    __int16 field_92_current_frame;
    BaseGameObject* field_94_pGameObj;

public:
    EXPORT void SetFrame_409D50(__int16 newFrame);
    EXPORT FrameInfoHeader* Get_FrameHeader_40B730(__int16 frame);
    EXPORT void Get_Frame_Rect_409E10(PSX_RECT *pRect);
    EXPORT WORD Get_Frame_Count_40AC70();
    EXPORT signed __int16 Init_40A030(int frameTableOffset, DynamicArray* animList, BaseGameObject* pGameObj, unsigned __int16 maxW, unsigned __int16 maxH, BYTE** ppAnimData, unsigned __int8 unknown1, signed int pal_depth, char unknown3);
    EXPORT void Get_Frame_Offset_40C480(__int16* pBoundingX, __int16* pBoundingY);
    EXPORT void Get_Frame_Width_Height_40C400(__int16* pWidth, __int16* pHeight);

    EXPORT void Load_Pal_40A530(BYTE** pAnimData, int palOffset);
};
ALIVE_ASSERT_SIZEOF(Animation, 0x98);

inline bool IsLastFrame(const Animation* pAnim)
{
    const BYTE* pAnimData = (*pAnim->field_20_ppBlock);
    const auto pHeader = reinterpret_cast<const AnimationHeader*>(&pAnimData[pAnim->field_18_frame_table_offset]);
    return (pAnim->field_92_current_frame == pHeader->field_2_num_frames - 1);
}
