#include "CamConverter.hpp"
#include "../../AliveLibAE/ResourceManager.hpp"
#include "LvlReaderWriter.hpp"
#include "relive_api_exceptions.hpp"
#include "../../AliveLibAE/ScreenManager.hpp"
#include "../../AliveLibCommon/CamDecompressor.hpp"
#include <lodepng/lodepng.h>
#include <iostream>

namespace ReliveAPI {
static void AppendCamSegment(s32 x, s32 y, s32 width, s32 height, u16* pDst, const u16* pSrcPixels)
{
    const u16* pSrc = pSrcPixels;
    for (s32 y1 = y; y1 < height; y1++)
    {
        memcpy(&pDst[x + (y1 * 640)], &pSrc[(y1 * width)], width * sizeof(s16));
    }
}

static u32 RGB565ToRGB888(u16 pixel)
{
    const u8 r5 = ((pixel >> 11) & 0x1F);
    const u8 g6 = ((pixel >> 5) & 0x3F);
    const u8 b5 = (pixel & 0x1F);

    const u32 r8 = ((r5 * 527) + 23) >> 6;
    const u32 g8 = ((g6 * 259) + 33) >> 6;
    const u32 b8 = ((b5 * 527) + 23) >> 6;

    const u32 rgb888 = (b8 << 16) | (g8 << 8) | r8;
    return rgb888;
}

static void SaveCamPng(const u16* camBuffer, const char_type* pFileName)
{
    u32 dst[240][640] = {};
    for (u32 y = 0; y < 240; y++)
    {
        for (u32 x = 0; x < 640; x++)
        {
            dst[y][x] = RGB565ToRGB888(*camBuffer);
            camBuffer++;
        }
    }

    // we're going to encode with a state rather than a convenient function, because enforcing a color type requires setting options
    lodepng::State state;
    // input color type
    state.info_raw.colortype = LCT_RGBA;
    state.info_raw.bitdepth = 8;

    // output color type
    state.info_png.color.colortype = LCT_RGB;
    state.info_png.color.bitdepth = 8;
    state.encoder.auto_convert = 0; // without this, it would ignore the output color type specified above and choose an optimal one instead

    //encode and save
    std::vector<u8> buffer;
    const auto error = lodepng::encode(buffer, reinterpret_cast<const u8*>(&dst[0][0]), 640, 240, state);
    if (error)
    {
        std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    else
    {
        lodepng::save_file(buffer, pFileName);
    }
}

CamConverterAO::CamConverterAO(const std::string& fileName, const ChunkedLvlFile& camFile)
{
    std::optional<LvlFileChunk> bitsRes = camFile.ChunkByType(ResourceManager::Resource_Bits);
    if (bitsRes)
    {
        u16 camBuffer[640 * 240] = {};
        const u16* pIter = reinterpret_cast<const u16*>(bitsRes->Data().data());
        for (s16 xpos = 0; xpos < 640; xpos += 16)
        {
            const u16 slice_len = *pIter;
            pIter++; // Skip len

            AppendCamSegment(xpos, 0, 16, 240, camBuffer, pIter);

            // To next slice
            pIter += (slice_len / sizeof(s16));
        }
        SaveCamPng(camBuffer, fileName.c_str());
    }

    std::optional<LvlFileChunk> fg1Res = camFile.ChunkByType(ResourceManager::Resource_FG1);
    if (fg1Res)
    {
        // AO only has 2 FG1 layers
        // TODO: Implement later
    }
}

CamConverterAE::CamConverterAE(const std::string& fileName, const ChunkedLvlFile& camFile)
{
    std::optional<LvlFileChunk> bitsRes = camFile.ChunkByType(ResourceManager::Resource_Bits);
    if (bitsRes)
    {
        u16 camBuffer[640 * 240] = {};
        u8 vlcBuffer[0x7E00] = {};
        CamDecompressor decompressor;
        const u16* pIter = reinterpret_cast<const u16*>(bitsRes->Data().data());
        for (s16 xpos = 0; xpos < 640; xpos += 16)
        {
            const u16 stripSize = *pIter;
            pIter++;

            if (stripSize > 0)
            {
                decompressor.vlc_decode(pIter, reinterpret_cast<u16*>(vlcBuffer));
                decompressor.process_segment(reinterpret_cast<u16*>(vlcBuffer), 0);
                AppendCamSegment(xpos, 0, 16, 240, camBuffer, decompressor.mDecompressedStrip);
            }

            pIter += (stripSize / sizeof(u16));
        }
        SaveCamPng(camBuffer, fileName.c_str());
    }

    std::optional<LvlFileChunk> fg1Res = camFile.ChunkByType(ResourceManager::Resource_FG1);
    if (fg1Res)
    {
        // AE has 4 FG1 layers
        // TODO: Implement later
    }
}
} // namespace ReliveAPI
