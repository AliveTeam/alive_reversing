#pragma once

#include "file_system.hpp"
#include "../AliveLibCommon/AnimResources.hpp"
#include "Animation.hpp"
#include <vector>

struct AnimationPal final
{
    u16 mPal[256] = {};
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

private:
    // Calc the max width and height because the ones in the header are often way too big for some reason
    // at least for a single animation in a BAN file
    MaxWH CalcMaxWH(const AnimationHeader* pAnimationHeader);

    void ConvertPalToTGAFormat(const AnimationFileHeader* pAnimationFileHeader, AnimationPal& pal);

    void DecompressAnimFrame(std::vector<u8>& decompressionBuffer, const FrameHeader* pFrameHeader);

    static constexpr u32 kResHeaderSize = 16;

    u32 CalcWidthAdjustedForBPP(const FrameHeader* pFrameHeader);

    u32 CalcImageWidth(const FrameHeader* pFrameHeader);

    u32 CalcDecompressionBufferSize(const AnimRecord& rec, const FrameHeader* pFrameHeader);

    const FrameInfoHeader* GetFrameInfoHeader(const AnimationHeader* pAnimationHeader, u32 idx);

    const FrameHeader* GetFrame(const AnimationHeader* pAnimationHeader, u32 idx);

    const std::vector<u8>& mFileData;
    bool mIsAoData = false;
};
