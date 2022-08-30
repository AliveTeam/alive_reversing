#include "AnimationConverter.hpp"
#include "nlohmann/json.hpp"
#include "../../AliveLibCommon/BaseGameAutoPlayer.hpp"
#include "../Compression.hpp"
#include "../../AliveLibAE/Compression.hpp" // TODO: combine with common compression files

struct PerFrameInfo final
{
    s32 mXOffset = 0;
    s32 mYOffset = 0;
    u32 mWidth = 0;
    u32 mHeight = 0;
    u32 mSpriteSheetX = 0;
    u32 mSpriteSheetY = 0;
};

void to_json(nlohmann::json& j, const PerFrameInfo& p)
{
    j = nlohmann::json{
        {"x_offset", p.mXOffset},
        {"y_offset", p.mYOffset},
        {"width", p.mWidth},
        {"height", p.mHeight},
        {"sprite_sheet_x", p.mSpriteSheetX},
        {"sprite_sheet_y", p.mSpriteSheetY},

    };
}

void from_json(const nlohmann::json& j, PerFrameInfo& p)
{
    j.at("x_offset").get_to(p.mXOffset);
    j.at("y_offset").get_to(p.mYOffset);
    j.at("width").get_to(p.mWidth);
    j.at("height").get_to(p.mHeight);
    j.at("sprite_sheet_x").get_to(p.mSpriteSheetX);
    j.at("sprite_sheet_y").get_to(p.mSpriteSheetY);
}


// TODO: Should be its own  file 
class TgaFile final
{
public:
    void Save(const char_type* pFileName, const AnimationPal& pal256, const std::vector<u8>& pixelData, u32 width, u32 height)
    {
        // The TGA header uses a var length id string which means we can't just use
        // a struct to represent it since the alignment is not fixed until after this field.
        AutoFILE f;
        f.Open(pFileName, "wb", false);

        u8 mIdLength = 0;
        u8 mColourMapType = 1; // Pal based TGA
        u8 mImageType = 1;     // Pal based TGA

        f.Write(mIdLength);
        f.Write(mColourMapType);
        f.Write(mImageType);

        // Colour Map
        u16 mFirstEntry = 0;
        u16 mNumEntries = 256;
        u8 mBitsPerEntry = 16;
        f.Write(mFirstEntry);
        f.Write(mNumEntries);
        f.Write(mBitsPerEntry);

        u16 mXOrigin = 0;
        u16 mYOrigin = 0;
        u16 mWidth = static_cast<u16>(width);
        u16 mHeight = static_cast<u16>(height);
        u8 mBitsPerPixel = 8;
        u8 mDescriptor = 0x20; // 0x30
        f.Write(mXOrigin);
        f.Write(mYOrigin);
        f.Write(mWidth);
        f.Write(mHeight);
        f.Write(mBitsPerPixel);
        f.Write(mDescriptor);

        f.Write(reinterpret_cast<const u8*>(&pal256.mPal[0]), sizeof(u16) * 256);

        // Write pixel data
        f.Write(pixelData);
    }
};

AnimationConverter::AnimationConverter(const FileSystem::Path& outputFile, const AnimRecord& rec, const std::vector<u8>& fileData, bool isAoData)
    : mFileData(fileData)
    , mIsAoData(isAoData)
{
    const auto pAnimationFileHeader = reinterpret_cast<const AnimationFileHeader*>(&mFileData[kResHeaderSize]);

    // Get the CLUT/pal
    AnimationPal pal;
    ConvertPalToTGAFormat(pAnimationFileHeader, pal);

    // Get the animation for this record (each has its own frame table offset)
    const auto pAnimationHeader = reinterpret_cast<const AnimationHeader*>(&mFileData[rec.mFrameTableOffset + kResHeaderSize]);

    // Get the size required to decompres a single frame
    const u32 decompressionBufferSize = CalcDecompressionBufferSize(rec, GetFrame(pAnimationHeader, 0));

    std::vector<u8> decompressionBuffer(decompressionBufferSize);

    // TODO: Handle 16 bit sprites
    const MaxWH bestMaxSize = CalcMaxWH(pAnimationHeader);

    const u32 sheetWidth = bestMaxSize.mMaxW * pAnimationHeader->field_2_num_frames;
    std::vector<u8> spriteSheetBuffer(sheetWidth * bestMaxSize.mMaxH);

    // Add each frame
    std::vector<PerFrameInfo> perFrameInfos(pAnimationHeader->field_2_num_frames);
    for (s32 i = 0; i < pAnimationHeader->field_2_num_frames; i++)
    {
        const FrameHeader* pFrameHeader = GetFrame(pAnimationHeader, i);
        DecompressAnimFrame(decompressionBuffer, pFrameHeader);

        // Add frame to the sprite sheet
        const u32 imageWidth = CalcImageWidth(pFrameHeader);
        for (u32 x = 0; x < pFrameHeader->field_4_width; x++)
        {
            for (u32 y = 0; y < pFrameHeader->field_5_height; y++)
            {
                spriteSheetBuffer[(y * sheetWidth) + (x + (bestMaxSize.mMaxW * i))] = decompressionBuffer[(y * imageWidth) + x];
            }
        }

        perFrameInfos[i].mWidth = pFrameHeader->field_4_width;
        perFrameInfos[i].mHeight = pFrameHeader->field_5_height;

        const FrameInfoHeader* pFrameInfoHeader = GetFrameInfoHeader(pAnimationHeader, i);
        perFrameInfos[i].mXOffset = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x;
        perFrameInfos[i].mYOffset = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y;

        perFrameInfos[i].mSpriteSheetX = bestMaxSize.mMaxW * i;
        perFrameInfos[i].mSpriteSheetY = 0;

        // Clear because the buffer is re-used to reduce memory allocs
        memset(decompressionBuffer.data(), 0, decompressionBuffer.size());
    }

    TgaFile tgaFile;
    tgaFile.Save((outputFile.GetPath() + ".tga").c_str(), pal, spriteSheetBuffer, sheetWidth, bestMaxSize.mMaxH);

    // Write json file
    nlohmann::json animJsonInfo = {
        // TODO: Current values are kind of nonsense, map to something sane
        {"frame_rate", pAnimationHeader->field_0_fps},
        {"flip_x", (pAnimationHeader->field_6_flags & AnimationHeader::eFlipXFlag) ? true : false},
        {"flip_y", (pAnimationHeader->field_6_flags & AnimationHeader::eFlipYFlag) ? true : false},
        {"loop", (pAnimationHeader->field_6_flags & AnimationHeader::eLoopFlag) ? true : false},
        {"loop_start_frame", pAnimationHeader->field_4_loop_start_frame},
        //{"number_of_frames", pAnimationHeader->field_2_num_frames},
        // TODO: Remove if not really needed (check after loader is impl'd)
        {"max_width", bestMaxSize.mMaxW},
        {"max_height", bestMaxSize.mMaxH}};

    animJsonInfo["frames"] = perFrameInfos;

    const std::string animJsonInfoString = animJsonInfo.dump(4);
    AutoFILE jsonFile;
    jsonFile.Open((outputFile.GetPath() + ".json").c_str(), "wb", false);
    jsonFile.Write(reinterpret_cast<const u8*>(animJsonInfoString.c_str()), static_cast<u32>(animJsonInfoString.size()));
}

// Calc the max width and height because the ones in the header are often way too big for some reason
// at least for a single animation in a BAN file
AnimationConverter::MaxWH AnimationConverter::CalcMaxWH(const AnimationHeader* pAnimationHeader)
{
    MaxWH maxSize;
    for (s32 i = 0; i < pAnimationHeader->field_2_num_frames; i++)
    {
        const FrameHeader* pFrameHeader = GetFrame(pAnimationHeader, i);
        if (pFrameHeader->field_4_width > maxSize.mMaxW)
        {
            maxSize.mMaxW = pFrameHeader->field_4_width;
        }

        if (pFrameHeader->field_5_height > maxSize.mMaxH)
        {
            maxSize.mMaxH = pFrameHeader->field_5_height;
        }
    }
    return maxSize;
}

void AnimationConverter::ConvertPalToTGAFormat(const AnimationFileHeader* pAnimationFileHeader, AnimationPal& pal)
{
    for (u32 i = 0; i < pAnimationFileHeader->mClutSize; i++)
    {
        const u8 r = pAnimationFileHeader->mClutData[i] & 31;
        const u8 g = (pAnimationFileHeader->mClutData[i] >> 5) & 31;
        const u8 b = (pAnimationFileHeader->mClutData[i] >> 10) & 31;
        const u8 semiTrans = (pAnimationFileHeader->mClutData[i] >> 15) & 1;

        //  color value: x[RRRRR][GG GGG][BBBBB] 1,5,5,5
        const u16 pixel = (b) | (g << 5) | (r << 10) | (semiTrans << 15);
        pal.mPal[i] = pixel;
    }
}

void AnimationConverter::DecompressAnimFrame(std::vector<u8>& decompressionBuffer, const FrameHeader* pFrameHeader)
{
    switch (pFrameHeader->field_7_compression_type)
    {
        case CompressionType::eType_0_NoCompression:
            // reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2)
            ALIVE_FATAL("todo");
            break;

        case CompressionType::eType_2_ThreeToFourBytes:
            /*
            CompressionType2_Decompress_40AA50(
                reinterpret_cast<const u8*>(&pFrameHeader[1]),
                *mDbuf,
                width_bpp_adjusted * pFrameHeader->field_5_height * 2);
            // AnimFlagsToBitDepth(mFlags)
            */
            ALIVE_FATAL("todo");
            break;

        case CompressionType::eType_3_RLE_Blocks:
            if (mIsAoData)
            {
                AO::Decompress_Type_3(
                    (u8*) &pFrameHeader[1],
                    decompressionBuffer.data(),
                    *(u32*) &pFrameHeader->field_8_width2,
                    2 * pFrameHeader->field_5_height * CalcWidthAdjustedForBPP(pFrameHeader));
            }
            else
            {
                CompressionType_3Ae_Decompress_40A6A0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), decompressionBuffer.data());
            }
            // renderer.Upload(AnimFlagsToBitDepth(mFlags), *mDbuf);

            break;

        case CompressionType::eType_4_RLE:
        case CompressionType::eType_5_RLE:
            ALIVE_FATAL("todo");
            // CompressionType_4Or5_Decompress_4ABAB0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *mDbuf);
            // renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, *mDbuf);
            break;

        case CompressionType::eType_6_RLE:
            ALIVE_FATAL("todo");
            // CompressionType6Ae_Decompress_40A8A0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *mDbuf);
            // renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, *mDbuf);
            break;

        case CompressionType::eType_7_NotUsed:
        case CompressionType::eType_8_NotUsed:
            ALIVE_FATAL("Decompression 7 and 8 never expected to be used");
            break;

        default:
            LOG_ERROR("Unknown compression type " << static_cast<s32>(pFrameHeader->field_7_compression_type));
            ALIVE_FATAL("Unknown compression type");
            break;
    }

    const u32 imageWidth = CalcImageWidth(pFrameHeader);
    const u32 originalWidth = pFrameHeader->field_4_width;
    const u32 compressionPadding = std::abs(static_cast<s32>(originalWidth - imageWidth));
    if (compressionPadding > 0)
    {
        // Set pixels to 0 on each row after originalWidth as they contain decompression artifacts
        for (u32 x = originalWidth; x < imageWidth; x++)
        {
            for (u32 y = 0; y < pFrameHeader->field_5_height; y++)
            {
                decompressionBuffer[(y * imageWidth) + x] = 0;
            }
        }
    }
}

u32 AnimationConverter::CalcWidthAdjustedForBPP(const FrameHeader* pFrameHeader)
{
    u32 width_bpp_adjusted = 0;
    switch (pFrameHeader->field_6_colour_depth)
    {
        case 4:
        {
            // 4 bit divide by quarter
            width_bpp_adjusted = ((pFrameHeader->field_4_width + 7) / 4) & ~1;
        }
        break;

        case 8:
        {
            // 8 bit, divided by half
            width_bpp_adjusted = ((pFrameHeader->field_4_width + 3) / 2) & ~1;
        }
        break;

        case 16:
        {
            // 16 bit, only multiple of 2 rounding
            width_bpp_adjusted = (pFrameHeader->field_4_width + 1) & ~1;
        }
        break;

        default:
        {
            ALIVE_FATAL("Bad colour depth");
        }
    }
    return width_bpp_adjusted;
}

u32 AnimationConverter::CalcImageWidth(const FrameHeader* pFrameHeader)
{
    u32 width = CalcWidthAdjustedForBPP(pFrameHeader);
    switch (pFrameHeader->field_6_colour_depth)
    {
        case 4:
        {
            // 4 bit divide by quarter
            width *= 4;
        }
        break;

        case 8:
        {
            // 8 bit, divided by half
            width *= 2;
        }
        break;

        case 16:
        {
            // Nothing to change
        }
        break;

        default:
        {
            ALIVE_FATAL("Bad colour depth");
        }
    }
    return width;
}

u32 AnimationConverter::CalcDecompressionBufferSize(const AnimRecord& rec, const FrameHeader* pFrameHeader)
{
    u32 decompression_width = 0;
    switch (pFrameHeader->field_6_colour_depth)
    {
        case 4:
        {
            decompression_width = (rec.mMaxW % 2) + (rec.mMaxW / 2);
        }
        break;

        case 8:
        {
            decompression_width = rec.mMaxW;
        }
        break;

        case 16:
        {
            decompression_width = rec.mMaxW * 2;
        }
        break;

        default:
        {
            ALIVE_FATAL("Bad colour depth");
        }
    }

    u32 decompressionBufferSize = rec.mMaxH * (decompression_width + 3);
    decompressionBufferSize += 8; // Add 8 for some reason
    return decompressionBufferSize;
}

const FrameInfoHeader* AnimationConverter::GetFrameInfoHeader(const AnimationHeader* pAnimationHeader, u32 idx)
{
    const u32 frameOffset = pAnimationHeader->mFrameOffsets[idx];
    auto pFrameInfoHeader = reinterpret_cast<const FrameInfoHeader*>(&mFileData[frameOffset + kResHeaderSize]);
    return pFrameInfoHeader;
}

const FrameHeader* AnimationConverter::GetFrame(const AnimationHeader* pAnimationHeader, u32 idx)
{
    const FrameInfoHeader* pFrameInfoHeader = GetFrameInfoHeader(pAnimationHeader, idx);
    auto pFrameHeader = reinterpret_cast<const FrameHeader*>(&mFileData[pFrameInfoHeader->field_0_frame_header_offset + kResHeaderSize]);
    return pFrameHeader;
}
