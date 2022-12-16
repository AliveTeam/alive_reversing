#include "AnimationConverter.hpp"
#include "nlohmann/json.hpp"
#include "../../relive_lib/BaseGameAutoPlayer.hpp"
#include "../Compression.hpp"
#include "../../AliveLibAE/Compression.hpp" // TODO: combine with common compression files
#include "../PsxDisplay.hpp" // PsxToPCX
#include "ResourceManagerWrapper.hpp"
#include "Animation.hpp"
#include "PNGFile.hpp"
#include <mapbox/shelf-pack.hpp>

inline void to_json(nlohmann::json&  j, const Point32& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
    };
}

inline void to_json(nlohmann::json& j, const IndexedPoint& p)
{
    j = nlohmann::json{
        {"index", p.mIndex},
        {"point", p.mPoint},
    };
}

inline void to_json(nlohmann::json& j, const PerFrameInfo& p)
{
    j = nlohmann::json{
        {"x_offset", p.mXOffset},
        {"y_offset", p.mYOffset},
        {"width", p.mWidth},
        {"height", p.mHeight},
        {"sprite_sheet_x", p.mSpriteSheetX},
        {"sprite_sheet_y", p.mSpriteSheetY},
        {"bound_max", p.mBoundMax},
        {"bound_min", p.mBoundMin},
        {"points_count", p.mPointCount},
    };

    if (p.mPointCount > 0)
    {
        j["points"] = p.mPoints;
    }
}

inline void to_json(nlohmann::json& j, const AnimAttributes& p)
{
    j = nlohmann::json{
        {"frame_rate", p.mFrameRate},
        {"flip_x", p.mFlipX},
        {"flip_y", p.mFlipY},
        {"loop", p.mLoop},
        {"loop_start_frame", p.mLoopStartFrame},
        {"max_width", p.mMaxWidth},
        {"max_height", p.mMaxHeight},
    };
}

static bool AttemptHalfSpaceBinPack(std::vector<mapbox::Bin>& myBins, std::vector<mapbox::Bin*>& packed, u32& allocTextureSize)
{
    mapbox::ShelfPack binPackerHalf(allocTextureSize / 2, allocTextureSize / 2);
    auto packedHalf = binPackerHalf.pack(myBins);
    if (packedHalf.size() == packed.size())
    {
        // As we didn't use inPlace=true in case this failed now that it has worked
        // we need to copy all of the x,y's over.
        for (u32 i = 0u; i < myBins.size(); i++)
        {
            myBins[i].x = packedHalf[i]->x;
            myBins[i].y = packedHalf[i]->y;
        }

        LOG_INFO("Winner winner! Half size");
        allocTextureSize = allocTextureSize / 2;

        // It worked so we can try even smaller
        return AttemptHalfSpaceBinPack(myBins, packedHalf, allocTextureSize);
    }
    return false;
}

AnimationConverter::AnimationConverter(const FileSystem::Path& outputFile, const AnimRecord& rec, const std::vector<u8>& fileData, bool isAoData)
    : mFileData(fileData)
    , mIsAoData(isAoData)
{
    // Get the animation for this record (each has its own frame table offset)
    const auto pAnimationHeader = reinterpret_cast<const AnimationHeader*>(&mFileData[rec.mFrameTableOffset]);

    const FrameHeader* pFirstFrame = GetFrame(pAnimationHeader, 0);

    // Get the CLUT/pal
    AnimationPal pal;
    ConvertPalToTGAFormat(fileData, pFirstFrame->field_0_clut_offset, pal);

    // Calculate the size of the nearest power-of-two that will fit all
    // the sprites
    constexpr u32 BIGGEST_TEXTURE_SIZE = 2048;

    const MaxWH biggestFrameSize = CalcMaxWH(pAnimationHeader);

    u32 allocTextureSize = 4; // Good starting size
    bool foundFit = false;
    s32 spritesFitX = 0;
    s32 spritesFitY = 0;

    if (pAnimationHeader->field_2_num_frames == 0)
    {
        ALIVE_FATAL("Anim with no frames encountered");
    }

    while (!foundFit && allocTextureSize <= BIGGEST_TEXTURE_SIZE)
    {
        spritesFitX = allocTextureSize / biggestFrameSize.mMaxW;
        spritesFitY = allocTextureSize / biggestFrameSize.mMaxH;

        if (spritesFitX * spritesFitY >= pAnimationHeader->field_2_num_frames)
        {
            foundFit = true;
        }
        else
        {
            allocTextureSize *= 2;
        }
    }

    if (!foundFit)
    {
        ALIVE_FATAL("Could not fit anim into biggest texture size %d", BIGGEST_TEXTURE_SIZE);
    }

    // Get the size required to decompress a single frame
    const u32 decompressionBufferSize = CalcDecompressionBufferSize(rec, pFirstFrame);
    std::vector<u8> decompressionBuffer(decompressionBufferSize);

    // Add each frame
    std::vector<PerFrameInfo> perFrameInfos(pAnimationHeader->field_2_num_frames);

    std::vector<mapbox::Bin> myBins;
    myBins.reserve(pAnimationHeader->field_2_num_frames);
    for (s32 i = 0; i < pAnimationHeader->field_2_num_frames; i++)
    {
        const FrameHeader* pFrameHeader = GetFrame(pAnimationHeader, i);
        myBins.emplace_back(-1, pFrameHeader->field_4_width, pFrameHeader->field_5_height);
    }

    mapbox::ShelfPack::PackOptions options;
    options.inPlace = true;
    mapbox::ShelfPack binPacker(allocTextureSize, allocTextureSize);
    auto packed = binPacker.pack(myBins, options);
    if (packed.size() != myBins.size())
    {
        // If this fails default to grid packing (yes somehow bin packing is worse than a grid sometimes! NP-Complete hell)
        LOG_WARNING("Its over for bin pack");
        for (s32 i = 0; i < pAnimationHeader->field_2_num_frames; i++)
        {
            const u32 baseX = ((i % spritesFitX) * biggestFrameSize.mMaxW);
            const u32 baseY = ((i / spritesFitX) * biggestFrameSize.mMaxH);
            myBins[i].x = baseX;
            myBins[i].y = baseY;
        }
    }
    else
    {
        // In many cases the next size down would fit so we try that as well
        AttemptHalfSpaceBinPack(myBins, packed, allocTextureSize);
    }

    std::vector<u8> spriteSheetBuffer(allocTextureSize * allocTextureSize);
    for (s32 i = 0; i < pAnimationHeader->field_2_num_frames; i++)
    {
        const FrameInfoHeader* pFrameInfoHeader = GetFrameInfoHeader(pAnimationHeader, i);
        const FrameHeader* pFrameHeader = GetFrame(pAnimationHeader, i);

        DecompressAnimFrame(decompressionBuffer, pFrameHeader, fileData);

        // Add frame to the sprite sheet
        const u32 imageWidth = CalcImageWidth(pFrameHeader);

        const u32 baseX = myBins[i].x;
        const u32 baseY = myBins[i].y;

        for (u32 x = 0; x < pFrameHeader->field_4_width; x++)
        {
            for (u32 y = 0; y < pFrameHeader->field_5_height; y++)
            {
                switch (pFirstFrame->field_6_colour_depth)
                {
                case 4: // 4bit indcies converted to 8bit in decompress frame
                case 8:
                {
                    const u32 srcIdx = (y * imageWidth) + x;
                    if (srcIdx > decompressionBuffer.size())
                    {
                        ALIVE_FATAL("decompression buffer out of bounds");
                    }

                    const u8 value = decompressionBuffer[srcIdx];

                    const u32 targetX = baseX + x;
                    const u32 targetY = baseY + y;

                    const u32 dstIdx = (targetY * allocTextureSize) + targetX;

                    if (dstIdx > spriteSheetBuffer.size())
                    {
                        ALIVE_FATAL("Sprite sheet buffer out of bounds");
                    }

                    spriteSheetBuffer[dstIdx] = value;

                    break;
                }

                case 16:
                    ALIVE_FATAL("16 bpp anims not present in OG, data corrupted?");
                    break;
                }
            }
        }


        perFrameInfos[i].mWidth = pFrameHeader->field_4_width;
        perFrameInfos[i].mHeight = pFrameHeader->field_5_height;

        if (mIsAoData)
        {
            perFrameInfos[i].mXOffset = PsxToPCX(pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x);
        }
        else
        {
            perFrameInfos[i].mXOffset = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x;
        }

        perFrameInfos[i].mYOffset = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y;

        perFrameInfos[i].mSpriteSheetX = baseX;
        perFrameInfos[i].mSpriteSheetY = baseY;

        perFrameInfos[i].mBoundMin.x = pFrameInfoHeader->field_8_data.offsetAndRect.mMin.x;
        perFrameInfos[i].mBoundMin.y = pFrameInfoHeader->field_8_data.offsetAndRect.mMin.y;

        perFrameInfos[i].mBoundMax.x = pFrameInfoHeader->field_8_data.offsetAndRect.mMax.x;
        perFrameInfos[i].mBoundMax.y = pFrameInfoHeader->field_8_data.offsetAndRect.mMax.y;


        if (pFrameInfoHeader->field_6_count > 2)
        {
            ALIVE_FATAL("No OG data should have more than 2 points");
        }

        u32 numPoints = pFrameInfoHeader->field_6_count;
        if (pFrameInfoHeader->field_6_count > 0)
        {
            // NOTE: Matches data on disk, size matters
            struct PointAndIndex final
            {
                u32 index;
                Point point[1]; // NOTE: Var length in 1 case only
            };
            auto pPointAndIndex = reinterpret_cast<const PointAndIndex*>(&pFrameInfoHeader->field_8_data.points[3]);

            for (s32 j = 0; j < pFrameInfoHeader->field_6_count; j++)
            {
                perFrameInfos[i].mPoints[j].mIndex = pPointAndIndex[j].index;
                perFrameInfos[i].mPoints[j].mPoint.x = pPointAndIndex[j].point[0].x;
                perFrameInfos[i].mPoints[j].mPoint.y = pPointAndIndex[j].point[0].y;

                switch (rec.mId)
                {
                case AnimId::Swinging_Ball_Fast:
                    [[fallthrough]];
                case AnimId::Swinging_Ball_Normal:
                    [[fallthrough]];
                case AnimId::Swinging_Ball_Slow:
                    // The only exception to the rule of a single point in the data is the ZBALLS
                    // so in this case only read 2 points by shoving them into the next points record
                    if (pFrameInfoHeader->field_6_count != 1)
                    {
                        ALIVE_FATAL("ZBALL has too many point entries");
                    }
                    // Yep there really isn't an index for the 2nd entry, its just 2 tightly packed points making a rect
                    perFrameInfos[i].mPoints[j + 1].mIndex = pPointAndIndex[j].index;
                    perFrameInfos[i].mPoints[j + 1].mPoint.x = pPointAndIndex[j].point[1].x;
                    perFrameInfos[i].mPoints[j + 1].mPoint.y = pPointAndIndex[j].point[1].y;

                    // Fix up the point count from 1 to 2
                    numPoints = 2;
                    break;

                default:
                    break;
                }
            }
        }
        perFrameInfos[i].mPointCount = numPoints;

        // Clear because the buffer is re-used to reduce memory allocs
        decompressionBuffer.clear();
        decompressionBuffer.resize(decompressionBufferSize);
    }

    PNGFile pngFile;
    pngFile.Save((outputFile.GetPath() + ".png").c_str(), pal, spriteSheetBuffer, allocTextureSize, allocTextureSize);

    // Write json file
    AnimAttributes attributes = {};
    // TODO: Current values are kind of nonsense, map to something sane

    attributes.mFrameRate = pAnimationHeader->field_0_fps;

    if ((!isAoData && rec.mId == AnimId::RockSack_Idle) || rec.mId == AnimId::BoneBag_Idle)
    {
        // Replicate OWI hack that was in the object ctor, we do it in the data conversion so
        // the game object doesn't need the hack.
        attributes.mFrameRate = 0;
    }

    attributes.mFlipX = (pAnimationHeader->field_6_flags & AnimationHeader::eFlipXFlag) ? true : false;
    attributes.mFlipY = (pAnimationHeader->field_6_flags & AnimationHeader::eFlipYFlag) ? true : false;
    attributes.mLoop = (pAnimationHeader->field_6_flags & AnimationHeader::eLoopFlag) ? true : false;
    attributes.mLoopStartFrame = pAnimationHeader->field_4_loop_start_frame;
    // TODO: Remove if not really needed (check after loader is impl'd)
    attributes.mMaxWidth = biggestFrameSize.mMaxW;
    attributes.mMaxHeight = biggestFrameSize.mMaxH;

    nlohmann::json animJsonInfo;
    animJsonInfo["attributes"] = attributes;
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

u16 AnimationConverter::ToTGAPixelFormat(u16 pixel)
{
    const u8 r = pixel & 0x1F;
    const u8 g = (pixel >> 5) & 0x1F;
    const u8 b = (pixel >> 10) & 0x1F;
    const u8 semiTrans = (pixel >> 15) & 0x1;

    //  color value: x[RRRRR][GG GGG][BBBBB] 1,5,5,5
    const u16 convertedPixel = (b) | (g << 5) | (r << 10) | (semiTrans << 15);
    return convertedPixel;
}

void AnimationConverter::ConvertPalToTGAFormat(const std::vector<u8>& fileData, u32 clutOffset, AnimationPal& pal)
{
    const u32 clutSize = *reinterpret_cast<const u32*>(fileData.data() + clutOffset);
    const u16* pClutData = reinterpret_cast<const u16*>(fileData.data() + clutOffset + sizeof(u32));

    for (u32 i = 0; i < clutSize; i++)
    {
        RGBA32 pixel = RGBConversion::RGBA555ToRGBA888Components(pClutData[i]);

        pal.mPal[i].r = pixel.r;
        pal.mPal[i].g = pixel.g;
        pal.mPal[i].b = pixel.b;
        pal.mPal[i].a = pixel.a;
    }
}

void AnimationConverter::DecompressAnimFrame(std::vector<u8>& decompressionBuffer, const FrameHeader* pFrameHeader, const std::vector<u8>& fileData)
{
    switch (pFrameHeader->field_7_compression_type)
    {
        case CompressionType::eType_0_NoCompression:
        {
            // AE_Ropes doesn't have all of the frame data without +3
            std::size_t lenToCopy = (pFrameHeader->field_4_width + 3) * pFrameHeader->field_5_height;
            if (pFrameHeader->field_6_colour_depth == 4)
            {
                lenToCopy = lenToCopy / 2;
            }

            if (reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2) + lenToCopy > fileData.data() + fileData.size())
            {
                ALIVE_FATAL("Decompression type 0 read out of bounds");
            }

            if (lenToCopy > decompressionBuffer.size())
            {
                ALIVE_FATAL("Decompression type 0 write out of bounds");
            }

            memcpy(decompressionBuffer.data(), reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), lenToCopy);
            break;
        }

        case CompressionType::eType_2_ThreeToFourBytes:
            CompressionType2_Decompress(
                reinterpret_cast<const u8*>(&pFrameHeader[1]),
                decompressionBuffer.data(),
                CalcWidthAdjustedForBPP(pFrameHeader) * pFrameHeader->field_5_height * 2);
            // AnimFlagsToBitDepth(mFlags)
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
                CompressionType_3Ae_Decompress(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), decompressionBuffer.data());
            }
            // renderer.Upload(AnimFlagsToBitDepth(mFlags), *mDbuf);

            break;

        case CompressionType::eType_4_RLE:
        case CompressionType::eType_5_RLE:
           // ALIVE_FATAL("todo");
            CompressionType_4Or5_Decompress(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), decompressionBuffer.data());
            // renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, *mDbuf);
            break;

        case CompressionType::eType_6_RLE:
            //ALIVE_FATAL("todo");
            CompressionType6Ae_Decompress(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), decompressionBuffer.data());
            // renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, *mDbuf);
            break;

        case CompressionType::eType_7_NotUsed:
        case CompressionType::eType_8_NotUsed:
            ALIVE_FATAL("Decompression 7 and 8 never expected to be used");
            break;

        default:
            ALIVE_FATAL("Unknown compression type %d", static_cast<s32>(pFrameHeader->field_7_compression_type));
            break;
    }
    
    // Convert 4bit to 8bit pal indecies
    if (pFrameHeader->field_6_colour_depth == 4)
    {
        std::vector<u8> newData(decompressionBuffer.size() * 2);

        // Expand 4bit to 8bit
        std::size_t src = 0;
        std::size_t dst = 0;
        while (dst < newData.size())
        {
            newData[dst++] = (decompressionBuffer[src] & 0xF);
            newData[dst++] = ((decompressionBuffer[src++] & 0xF0) >> 4);
        }
        decompressionBuffer = newData;
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
    auto pFrameInfoHeader = reinterpret_cast<const FrameInfoHeader*>(&mFileData[frameOffset]);
    return pFrameInfoHeader;
}

const FrameHeader* AnimationConverter::GetFrame(const AnimationHeader* pAnimationHeader, u32 idx)
{
    const FrameInfoHeader* pFrameInfoHeader = GetFrameInfoHeader(pAnimationHeader, idx);
    auto pFrameHeader = reinterpret_cast<const FrameHeader*>(&mFileData[pFrameInfoHeader->field_0_frame_header_offset]);
    return pFrameHeader;
}
