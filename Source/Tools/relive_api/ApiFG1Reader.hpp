#pragma once

#include "../../AliveLibCommon/FG1Reader.hpp"
#include <string>

namespace ReliveAPI {
class CameraImageAndLayers;

struct FG1Buffers final
{
    void MergePixel(u32 layer, u32 x, u32 y, u16 pixel)
    {
        mFg1[layer][y][x] |= pixel;
    }
    u16 mFg1[4][240][640];
};

class ApiFG1Reader final : public BaseFG1Reader
{
public:
    explicit ApiFG1Reader(FG1Format format);
    ~ApiFG1Reader();
    u16 ConvertPixel(u16 pixel);
    void BltRectMerged(u32 xpos, u32 ypos, u32 width, u32 height, u32 layer, const u16* pSrcPixels, const u32* pBitMask);
    void OnPartialChunk(const Fg1Chunk& rChunk) override;
    void OnFullChunk(const Fg1Chunk& rChunk) override;
    u8** Allocate(u32 len) override;
    void Deallocate(u8** ptr) override;
    void LayersToPng(CameraImageAndLayers& outData);
    static void DebugSave(const std::string& prefix, const CameraImageAndLayers& outData);
    static void DebugRead(const std::string& prefix, CameraImageAndLayers& outData);

private:
    std::string& BufferForLayer(CameraImageAndLayers& outData, u32 layer);

    // 2 layers in AO, 4 layers in AE
    bool mUsedLayers[4] = {};
    FG1Buffers* mFg1Buffers = nullptr;
};
} // namespace ReliveAPI