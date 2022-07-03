#include "CamConverter.hpp"
#include "../../AliveLibAE/ResourceManager.hpp"
#include "LvlReaderWriter.hpp"
#include "relive_api_exceptions.hpp"
#include "../../relive_lib/ScreenManager.hpp"
#include "../../AliveLibCommon/CamDecompressor.hpp"
#include "../../AliveLibCommon/FG1Reader.hpp"
#include "../../AliveLibCommon/logger.hpp"
#include "Base64.hpp"
#include "JsonModelTypes.hpp"
#include "ApiFG1Reader.hpp"
#include <lodepng/lodepng.h>
#include <iostream>
#include <memory>

namespace ReliveAPI {


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

struct TmpBuffer final
{
    u32 dst[240][640] = {};
};

static void RGB565ToPngBuffer(const u16* camBuffer, std::vector<u8>& outPngData)
{
    auto tmpBuffer = std::make_unique<TmpBuffer>();
    for (u32 y = 0; y < 240; y++)
    {
        for (u32 x = 0; x < 640; x++)
        {
            tmpBuffer->dst[y][x] = RGB565ToRGB888(*camBuffer);
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

    const auto error = lodepng::encode(outPngData, reinterpret_cast<const u8*>(&tmpBuffer->dst[0][0]), 640, 240, state);
    if (error)
    {
        std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
}

std::string RGB565ToBase64PngString(const u16* pRgb565Buffer)
{
    std::vector<u8> outPngData;
    RGB565ToPngBuffer(pRgb565Buffer, outPngData);
    return ToBase64(outPngData);
}

static void AppendCamSegment(s32 x, s32 y, s32 width, s32 height, u16* pDst, const u16* pSrcPixels)
{
    const u16* pSrc = pSrcPixels;
    for (s32 y1 = y; y1 < height; y1++)
    {
        memcpy(&pDst[x + (y1 * 640)], &pSrc[(y1 * width)], width * sizeof(s16));
    }
}

static void MergeFG1BlocksAndConvertToPng(const ChunkedLvlFile& camFile, BaseFG1Reader::FG1Format fg1Format, CameraImageAndLayers& outData)
{
    // For some crazy reason there can be multiple FG1 blocks, here we squash them down into a single
    // image for each "layer".
    std::optional<LvlFileChunk> anyFG1 = camFile.ChunkByType(ResourceManager::Resource_FG1);
    if (anyFG1)
    {
        const u8* pFg1Data = anyFG1->Data().data();
        const bool isReliveFormat = ApiFG1Reader::IsReliveFG1(reinterpret_cast<const FG1ResourceBlockHeader*>(pFg1Data));
        if (isReliveFormat)
        {
            // For relive format its very slightly tweaked AE format, move past the extra u32 we added and process the FG1
            // we also know there is only 1 chunk in relive format.
            pFg1Data += sizeof(u32);
            ApiFG1Reader reader(BaseFG1Reader::FG1Format::AE);
            reader.Iterate(reinterpret_cast<const FG1ResourceBlockHeader*>(pFg1Data));
            reader.LayersToPng(outData);
        }
        else
        {
            ApiFG1Reader reader(fg1Format);
            for (u32 i = 0; i < camFile.ChunkCount(); i++)
            {
                if (camFile.ChunkAt(i).Header().mResourceType == ResourceManager::Resource_FG1)
                {
                    reader.Iterate(reinterpret_cast<const FG1ResourceBlockHeader*>(camFile.ChunkAt(i).Data().data()));
                }
            }
            reader.LayersToPng(outData);
        }
    }
}

static bool AEcamIsAOCam(const LvlFileChunk& bitsRes)
{
    const u16* pIter = reinterpret_cast<const u16*>(bitsRes.Data().data());
    for (s16 xpos = 0; xpos < 640; xpos += 16)
    {
        const u16 stripSize = *pIter;
        if (stripSize != (16 * 240 * sizeof(u16)))
        {
            return false;
        }
    }
    return true;
}

static void ConvertAECamera(const LvlFileChunk& bitsRes, std::string& cameraPngBase64)
{
    std::vector<u16> camBuffer(640 * 240);
    std::vector<u8> vlcBuffer(0x7E00);
    CamDecompressor decompressor;
    const u16* pIter = reinterpret_cast<const u16*>(bitsRes.Data().data());
    for (s16 xpos = 0; xpos < 640; xpos += 16)
    {
        const u16 stripSize = *pIter;
        pIter++;

        if (stripSize > 0)
        {
            decompressor.vlc_decode(pIter, reinterpret_cast<u16*>(vlcBuffer.data()));
            decompressor.process_segment(reinterpret_cast<u16*>(vlcBuffer.data()), 0);
            AppendCamSegment(xpos, 0, 16, 240, camBuffer.data(), decompressor.mDecompressedStrip);
        }

        pIter += (stripSize / sizeof(u16));
    }
    cameraPngBase64 = RGB565ToBase64PngString(camBuffer.data());
}

static void ConvertAOCamera(const LvlFileChunk& bitsRes, std::string& cameraPngBase64)
{
    std::vector<u16> camBuffer(640 * 240);
    const u16* pIter = reinterpret_cast<const u16*>(bitsRes.Data().data());
    for (s16 xpos = 0; xpos < 640; xpos += 16)
    {
        const u16 slice_len = *pIter;
        pIter++; // Skip len

        AppendCamSegment(xpos, 0, 16, 240, camBuffer.data(), pIter);

        // To next slice
        pIter += (slice_len / sizeof(s16));
    }
    cameraPngBase64 = RGB565ToBase64PngString(camBuffer.data());
}

CamConverter::CamConverter(const ChunkedLvlFile& camFile, CameraImageAndLayers& outData)
{
    std::optional<LvlFileChunk> bitsRes = camFile.ChunkByType(ResourceManager::Resource_Bits);
    if (bitsRes)
    {
        if (AEcamIsAOCam(*bitsRes))
        {
            ConvertAOCamera(*bitsRes, outData.mCameraImage);
            MergeFG1BlocksAndConvertToPng(camFile, BaseFG1Reader::FG1Format::AO, outData);
        }
        else
        {
            ConvertAECamera(*bitsRes, outData.mCameraImage);
            MergeFG1BlocksAndConvertToPng(camFile, BaseFG1Reader::FG1Format::AE, outData);
        }
    }
}

u32 CamConverter::CamBitsIdFromName(const std::string& pCamName)
{
    if (pCamName.length() < 7)
    {
        // todo: throw
        LOG_WARNING("Bad camera name, can't get resource id " << pCamName);
        return 0;
    }
    // Given R1P20C15 returns 2015
    return 1 * (pCamName[7] - '0') + 10 * (pCamName[6] - '0') + 100 * (pCamName[4] - '0') + 1000 * (pCamName[3] - '0');
}

} // namespace ReliveAPI
