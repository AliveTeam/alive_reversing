#pragma once

#include "file_system.hpp"
#include "rgb_conversion.hpp"
#include "../../relive_lib/AnimResources.hpp"
#include <vector>

struct AnimationPal final
{
    RGBA32 mPal[256] = {};
};

struct AnimationFileHeader final
{
    s16 field_0_max_w;
    s16 field_2_max_h;
    u32 field_4_frame_table_offset;
    u32 mClutSize;
    u16 mClutData[1];
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

class AnimationConverter final
{
public:
    struct MaxWH final
    {
        u32 mMaxW = 0;
        u32 mMaxH = 0;
    };

    AnimationConverter(const FileSystem::Path& outputFile, const AnimRecord& rec, const std::vector<u8>& fileData, bool isAoData);
    static u16 ToTGAPixelFormat(u16 pixel);
private:
    // Calc the max width and height because the ones in the header are often way too big for some reason
    // at least for a single animation in a BAN file
    MaxWH CalcMaxWH(const AnimationHeader* pAnimationHeader);

    void ConvertPalToTGAFormat(const std::vector<u8>& fileData, u32 clutOffset, AnimationPal& pal);

    void DecompressAnimFrame(std::vector<u8>& decompressionBuffer, const FrameHeader* pFrameHeader, const std::vector<u8>& fileData);

    static constexpr u32 kResHeaderSize = 16;

    u32 CalcWidthAdjustedForBPP(const FrameHeader* pFrameHeader);

    u32 CalcImageWidth(const FrameHeader* pFrameHeader);

    u32 CalcDecompressionBufferSize(const AnimRecord& rec, const FrameHeader* pFrameHeader);

    const FrameInfoHeader* GetFrameInfoHeader(const AnimationHeader* pAnimationHeader, u32 idx);

    const FrameHeader* GetFrame(const AnimationHeader* pAnimationHeader, u32 idx);

    const std::vector<u8>& mFileData;
    bool mIsAoData = false;
};
