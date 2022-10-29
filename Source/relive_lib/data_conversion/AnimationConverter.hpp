#pragma once

#include "file_system.hpp"
#include "../../AliveLibCommon/AnimResources.hpp"
#include <vector>

struct AnimationPal final
{
    u16 mPal[256] = {};
};

struct AnimationHeader;
struct FrameHeader;
struct FrameInfoHeader;

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
