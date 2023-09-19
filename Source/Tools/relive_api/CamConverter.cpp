#include "CamConverter.hpp"
#include "../../AliveLibAE/ResourceManager.hpp"
#include "../../relive_lib/data_conversion/LvlReaderWriter.hpp"
#include "../../relive_lib/GameObjects/ScreenManager.hpp"
#include "../../relive_lib/CamDecompressor.hpp"
#include "../../relive_lib/FG1Reader.hpp"
#include "../../relive_lib/logger.hpp"
#include "Base64.hpp"
#include "JsonModelTypes.hpp"
#include "ApiFG1Reader.hpp"
#include "../../relive_lib/data_conversion/rgb_conversion.hpp"
#include "../../relive_lib/data_conversion/PNGFile.hpp"
#include "../../relive_lib/data_conversion/file_system.hpp"

namespace ReliveAPI {

struct TmpBuffer final
{
    u32 dst[240][640] = {};
};

void RGB565ToPngBuffer(const u16* camBuffer, std::vector<u8>& outPngData)
{
    auto tmpBuffer = std::make_unique<TmpBuffer>();
    for (u32 y = 0; y < 240; y++)
    {
        for (u32 x = 0; x < 640; x++)
        {
            tmpBuffer->dst[y][x] = RGBConversion::RGB565ToRGB888(*camBuffer) | (0xFF << 24);
            camBuffer++;
        }
    }

    PNGFile png;
    outPngData = png.Encode(&tmpBuffer->dst[0][0], 640, 240);
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

static std::unique_ptr<ApiFG1Reader> MergeFG1Blocks(const ChunkedLvlFile& camFile, BaseFG1Reader::FG1Format fg1Format)
{

    // For some crazy reason there can be multiple FG1 blocks, here we squash them down into a single
    // image for each "layer".
    std::optional<ApiFG1Reader> ret;
    std::optional<LvlFileChunk> anyFG1 = camFile.ChunkByType(ResourceManagerWrapper::Resource_FG1);
    if (anyFG1)
    {
        const u8* pFg1Data = anyFG1->Data().data();
        const bool isReliveFormat = ApiFG1Reader::IsReliveFG1(reinterpret_cast<const FG1ResourceBlockHeader*>(pFg1Data));
        if (isReliveFormat)
        {
            // For relive format its very slightly tweaked AE format, move past the extra u32 we added and process the FG1
            // we also know there is only 1 chunk in relive format.
            pFg1Data += sizeof(u32);
            auto reader = std::make_unique<ApiFG1Reader>(BaseFG1Reader::FG1Format::AE);
            reader->Iterate(reinterpret_cast<const FG1ResourceBlockHeader*>(pFg1Data));
            return reader;
        }
        else
        {
            auto reader = std::make_unique<ApiFG1Reader>(fg1Format);
            for (u32 i = 0; i < camFile.ChunkCount(); i++)
            {
                if (camFile.ChunkAt(i).Header().mResourceType == ResourceManagerWrapper::Resource_FG1)
                {
                    reader->Iterate(reinterpret_cast<const FG1ResourceBlockHeader*>(camFile.ChunkAt(i).Data().data()));
                }
            }
            return reader;
        }
    }
    return nullptr;
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

static std::vector<u16> StitchAECamera(const LvlFileChunk& bitsRes)
{
    std::vector<u16> camBuffer(640 * 240);
    std::vector<u8> vlcBuffer(0x7E00);
    CamDecompressor decompressor;
    std::vector<u8> biggerData = bitsRes.Data();

    // vlc_decode reads 1 word past the end on the last strip, do a slow copy of everything
    // just to avoid the out of bounds read
    biggerData.resize(biggerData.size() + 2);
    const u16* pIter = reinterpret_cast<const u16*>(biggerData.data());
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
    return camBuffer;
}

static std::vector<u16> StitchAOCamera(const LvlFileChunk& bitsRes)
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
    return camBuffer;
}

std::pair<std::unique_ptr<ApiFG1Reader>, u32> CamConverter::Convert(const ChunkedLvlFile& camFile, const std::string& baseName, bool isAo)
{
    std::optional<LvlFileChunk> bitsRes = camFile.ChunkByType(ResourceManagerWrapper::Resource_Bits);
    if (bitsRes)
    {
        const bool isAoCam = AEcamIsAOCam(*bitsRes);
        auto pixels = isAoCam ? StitchAOCamera(*bitsRes) : StitchAECamera(*bitsRes);
        std::vector<u8> outPngData;
        RGB565ToPngBuffer(pixels.data(), outPngData);
        FileSystem fs;
        fs.Save((baseName + ".png").c_str(), outPngData);

        BaseFG1Reader::FG1Format FG1Format = isAoCam ? BaseFG1Reader::FG1Format::AO : BaseFG1Reader::FG1Format::AE;

        // The old editor saves AE levels with AO cams but still uses the AE FG1 format
        if (isAoCam && !isAo)
        {
            FG1Format = BaseFG1Reader::FG1Format::AE;
        }

        auto reader = MergeFG1Blocks(camFile, FG1Format);
        if (reader)
        {
            u32 fg1BlocksCount = 0;
            for (u32 i = 0; i < camFile.ChunkCount(); i++)
            {
                if (camFile.ChunkAt(i).Header().mResourceType == ResourceManagerWrapper::Resource_FG1)
                {
                    fg1BlocksCount++;
                }
            }
            reader->SaveAsPng(baseName);
            return {std::move(reader), fg1BlocksCount};
        }
    }
    return {};
}

CamConverter::CamConverter(const ChunkedLvlFile& camFile, CameraImageAndLayers& outData)
{
    std::optional<LvlFileChunk> bitsRes = camFile.ChunkByType(ResourceManagerWrapper::Resource_Bits);
    if (bitsRes)
    {
        const bool isAO = AEcamIsAOCam(*bitsRes);
        outData.mCameraImage = RGB565ToBase64PngString((isAO ? StitchAOCamera(*bitsRes) : StitchAECamera(*bitsRes)).data());
        auto reader = MergeFG1Blocks(camFile, isAO ? BaseFG1Reader::FG1Format::AO : BaseFG1Reader::FG1Format::AE);
        if (reader)
        {
            reader->LayersToPng(outData);
        }
    }
}

u32 CamConverter::CamBitsIdFromName(const std::string& pCamName)
{
    if (pCamName.length() < 7)
    {
        // todo: throw
        LOG_WARNING("Bad camera name, can't get resource id %s", pCamName.c_str());
        return 0;
    }
    // Given R1P20C15 returns 2015
    return 1 * (pCamName[7] - '0') + 10 * (pCamName[6] - '0') + 100 * (pCamName[4] - '0') + 1000 * (pCamName[3] - '0');
}

} // namespace ReliveAPI
