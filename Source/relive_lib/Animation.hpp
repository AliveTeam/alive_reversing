#pragma once

#include "../relive_lib/AnimationBase.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibAE/FixedPoint.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"

enum class AnimId;
struct AnimRecord;

const AnimRecord PerGameAnimRec(AnimId id);
class BaseGameObject;

using TFrameCallBackType = void(*)(BaseGameObject*, u32& pointIdx, const Point32&);

// TODO: Move to converter, won't be used in engine
struct AnimationFileHeader final
{
    s16 field_0_max_w;
    s16 field_2_max_h;
    u32 field_4_frame_table_offset;
    u32 mClutSize;
    u16 mClutData[1];
};

// TODO: Move to converter, won't be used in engine
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

// TODO: Move to converter, won't be used in engine
enum class CompressionType : u8
{
    eType_0_NoCompression = 0,
    eType_2_ThreeToFourBytes = 2,
    eType_3_RLE_Blocks = 3,
    eType_4_RLE = 4,
    eType_5_RLE = 5,
    eType_6_RLE = 6,
    eType_7_NotUsed = 7,
    eType_8_NotUsed = 8,
};

// TODO: Move to converter, won't be used in engine
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

// TODO: Move to converter, won't be used in engine
struct Point final
{
    s16 x = 0;
    s16 y = 0;
};

// TODO: Move to converter, won't be used in engine
struct OffsetAndBoundingRect final
{
    Point mOffset;
    Point mMin;
    Point mMax;
};

// TODO: Move to converter, won't be used in engine
union PointsUnion
{
    PointsUnion()
    { }
    OffsetAndBoundingRect offsetAndRect;
    Point points[3];
};

// TODO: Move to converter, won't be used in engine
struct FrameInfoHeader final
{
    u32 field_0_frame_header_offset;
    s16 field_4_magic;
    s16 field_6_count;
    PointsUnion field_8_data;
};

class Animation final : public AnimationBase
{
public:
    virtual void VDecode() override;
    virtual void VRender(s32 xpos, s32 ypos, PrimHeader** ppOt, s16 width, s32 height) override;
    virtual void VCleanUp() override;

    s16 Set_Animation_Data(AnimResource& pAnimRes);
    s16 Init(const AnimResource& ppAnimData, BaseGameObject* pGameObj);

    void SetFrame(s32 newFrame);
    u32 Get_Frame_Count();
    const PerFrameInfo* Get_FrameHeader(s32 frame);
    void LoadPal(const PalResource& pal);
    void ReloadPal();
    void Get_Frame_Rect(PSX_RECT* pRect);
    void Get_Frame_Width_Height(s16* pWidth, s16* pHeight);
    void Get_Frame_Offset(s16* pBoundingX, s16* pBoundingY);
    void Invoke_CallBacks();
    bool DecodeCommon();
    void DecompressFrame();

    u32 mFrameDelay = 0;
    u16 field_12_scale = 0; // padding?
    FP field_14_scale = {};
    TFrameCallBackType* mFnPtrArray = nullptr;
    Poly_FT4 mOtData[2] = {};
    s32 mCurrentFrame = 0;
    BaseGameObject* mGameObj = nullptr;
};

inline bool IsLastFrame(const Animation* pAnim)
{
    return pAnim->mCurrentFrame == static_cast<s32>(pAnim->mAnimRes.mJsonPtr->mFrames.size() - 1);
}
