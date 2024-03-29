#include "ApiFG1Reader.hpp"
#include "JsonModelTypes.hpp"
#include "Base64.hpp"
#include "CamConverter.hpp"
#include <lodepng/lodepng.h>

namespace ReliveAPI {

ApiFG1Reader::ApiFG1Reader(FG1Format format)
    : BaseFG1Reader(format)
{
    // Dynamically allocated due to the huge stack space it would consume
    mFg1Buffers = new FG1Buffers();
}

ApiFG1Reader::~ApiFG1Reader()
{
    delete mFg1Buffers;
}

u16 ApiFG1Reader::ConvertPixel(u16 pixel)
{
    return ((pixel >> 15) << 5)
         | ((pixel & 31) << 11)
         | (((pixel >> 5) & 31) << 6)
         | (((pixel >> 10) & 31) << 0);
}

void ApiFG1Reader::BltRectMerged(u32 xpos, u32 ypos, u32 width, u32 height, u32 layer, const u16* pSrcPixels, const u32* pBitMask)
{
    mUsedLayers[layer] = true;

    for (u32 y = ypos; y < height; y++)
    {
        for (u32 x = xpos; x < width; x++)
        {
            if (x < 640 && y < 240)
            {
                u16 pixelVal = 0xFFFF;
                if (pSrcPixels)
                {
                    // Read RGB565 pixel value for AO
                    pixelVal = ConvertPixel(*pSrcPixels);

                    // If its not a "transparent" pixel set to white
                    if (pixelVal != 0)
                    {
                        pixelVal = 0xFFFF;
                    }
                    pSrcPixels++;
                }
                else if (pBitMask)
                {
                    // Bitfield in AE
                    const u32 bits = pBitMask[y - ypos];
                    if (!((bits >> (x - xpos)) & 1))
                    {
                        pixelVal = 0;
                    }
                }

                mFg1Buffers->MergePixel(layer, x, y, pixelVal);
            }
        }
    }
}

void ApiFG1Reader::OnPartialChunk(const Fg1Chunk& rChunk)
{
    const u16* pPixels = nullptr;
    const u32* pBitMap = nullptr;
    if (mFormat == FG1Format::AO)
    {
        pPixels = reinterpret_cast<const u16*>((&rChunk) + 1);
    }
    else
    {
        pBitMap = reinterpret_cast<const u32*>((&rChunk) + 1);
    }
    BltRectMerged(rChunk.field_4_xpos_or_compressed_size,
                  rChunk.field_6_ypos,
                  rChunk.field_8_width + rChunk.field_4_xpos_or_compressed_size,
                  rChunk.field_A_height + rChunk.field_6_ypos,
                  rChunk.field_2_layer_or_decompressed_size,
                  pPixels,
                  pBitMap);
}

void ApiFG1Reader::OnFullChunk(const Fg1Chunk& rChunk)
{
    BltRectMerged(rChunk.field_4_xpos_or_compressed_size,
                  rChunk.field_6_ypos,
                  rChunk.field_8_width + rChunk.field_4_xpos_or_compressed_size,
                  rChunk.field_A_height + rChunk.field_6_ypos,
                  rChunk.field_2_layer_or_decompressed_size,
                  nullptr,
                  nullptr);
}

u8** ApiFG1Reader::Allocate(u32 len)
{
    u8** pHolder = new u8*;
    *pHolder = new u8[len];
    return pHolder;
}

void ApiFG1Reader::Deallocate(u8** ptr)
{
    delete[] * ptr;
    delete ptr;
}

void ApiFG1Reader::LayersToPng(CameraImageAndLayers& outData)
{
    for (u32 i = 0; i < 4; i++)
    {
        if (mUsedLayers[i])
        {
            const u16* pPixels = &mFg1Buffers->mFg1[i][0][0];
            BufferForLayer(outData, i) = RGB565ToBase64PngString(pPixels);
        }
    }
}

void ApiFG1Reader::DebugSave(const std::string& prefix, const CameraImageAndLayers& outData)
{
    if (!outData.mCameraImage.empty())
    {
        lodepng::save_file(FromBase64(outData.mCameraImage), prefix + "_cam.png");
    }

    if (!outData.mBackgroundLayer.empty())
    {
        lodepng::save_file(FromBase64(outData.mBackgroundLayer), prefix  + "_bg.png");
    }

    if (!outData.mForegroundLayer.empty())
    {
        lodepng::save_file(FromBase64(outData.mForegroundLayer), prefix + "_fg.png");
    }

    if (!outData.mBackgroundWellLayer.empty())
    {
        lodepng::save_file(FromBase64(outData.mBackgroundWellLayer), prefix + "_bg_well.png");
    }

    if (!outData.mForegroundWellLayer.empty())
    {
        lodepng::save_file(FromBase64(outData.mForegroundWellLayer), prefix + "_fg_well.png");
    }
}

static void FileToBase64(std::string& outData, const std::string& pngFileName)
{
    std::vector<u8> rawPngData;
    auto err = lodepng::load_file(rawPngData, pngFileName);
    if (!err)
    {
        outData = ToBase64(rawPngData);
    }
}

void ApiFG1Reader::DebugRead(const std::string& prefix, CameraImageAndLayers& outData)
{
    FileToBase64(outData.mCameraImage, prefix + "_cam.png");
    FileToBase64(outData.mBackgroundLayer, prefix + "_bg.png");
    FileToBase64(outData.mForegroundLayer, prefix + "_fg.png");
    FileToBase64(outData.mBackgroundWellLayer, prefix + "_bg_well.png");
    FileToBase64(outData.mForegroundWellLayer, prefix + "_fg_well.png");
}

std::string& ApiFG1Reader::BufferForLayer(CameraImageAndLayers& outData, u32 layer)
{
    if (mFormat == FG1Format::AO)
    {
        switch (layer)
        {
            case 0:
                return outData.mForegroundLayer;

            case 1:
                return outData.mBackgroundLayer;
        }
    }
    else
    {
        switch (layer)
        {
            case 0:
                return outData.mBackgroundWellLayer;

            case 1:
                return outData.mBackgroundLayer;

            case 2:
                return outData.mForegroundWellLayer;

            case 3:
                return outData.mForegroundLayer;
        }
    }

    // Should never get here
    return outData.mBackgroundLayer;
}
} // namespace ReliveAPI
