#include "CamConverter.hpp"
#include "../AliveLibAE/ResourceManager.hpp"
#include "LvlReaderWriter.hpp"
#include "relive_api_exceptions.hpp"
#include <lodepng/lodepng.h>

static void AppendCamSegment(s32 x, s32 y, s32 width, s32 height, u16* pDst, const u16* pSrcPixels)
{
    const u16* pSrc = pSrcPixels;
    for (s32 y1 = y; y1 < height; y1++)
    {
        memcpy(&pDst[x + (y1 * 640)], &pSrc[(y1 * width)], width * sizeof(s16));
    }
}

static void SaveCamPng(const u16* camBuffer, const char_type* pFileName)
{
    u8 dst[640 * 240 * 3] = {};
    SDL_ConvertPixels(640, 240, SDL_PIXELFORMAT_RGB565, camBuffer, 640 * 2, SDL_PIXELFORMAT_RGB24, dst, 640 * 3);

    // we're going to encode with a state rather than a convenient function, because enforcing a color type requires setting options
    lodepng::State state;
    // input color type
    state.info_raw.colortype = LCT_RGB;
    state.info_raw.bitdepth = 8;

    // output color type
    state.info_png.color.colortype = LCT_RGB;
    state.info_png.color.bitdepth = 8;
    state.encoder.auto_convert = 0; // without this, it would ignore the output color type specified above and choose an optimal one instead

    //encode and save
    std::vector<u8> buffer;
    const auto error = lodepng::encode(buffer, &dst[0], 640, 240, state);
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
        SaveCamPng(camBuffer, (fileName + ".png").c_str());
    }

    std::optional<LvlFileChunk> fg1Res = camFile.ChunkByType(ResourceManager::Resource_FG1);
    if (fg1Res)
    {
        // AO only has 2 FG1 layers
    }
}

CamConverterAE::CamConverterAE(const std::string& /*fileName*/, const ChunkedLvlFile& /*camFile*/)
{

}
