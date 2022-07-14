#include "data_conversion.hpp"
#include "file_system.hpp"
#include "camera_converter.hpp"
#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/LvlArchive.hpp"
#include "../../AliveLibAO/Path.hpp"
#include "../../AliveLibAO/ShadowZone.hpp"
#include "../../AliveLibAE/ShadowZone.hpp"
#include "../../AliveLibAO/ResourceManager.hpp"
#include "../../AliveLibCommon/AnimResources.hpp"
#include "../../AliveLibCommon/BaseGameAutoPlayer.hpp"
#include "../Animation.hpp"
#include "../Compression.hpp"
#include "../MapWrapper.hpp"
#include <vector>
#include <algorithm>
#include "nlohmann/json.hpp"
#include "../../Tools/relive_api/LvlReaderWriter.hpp"
#include "../../Tools/relive_api/file_api.hpp"
#include "../../Tools/relive_api/CamConverter.hpp"
#include "../Collisions.hpp"

constexpr u32 kDataVersion = 1;

extern const CombinedAnimRecord kAnimRecords[915];
extern const AnimDetails kNullAnimDetails;



struct AnimationPal final
{
    u16 mPal[256] = {};
};


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
        u8 mColourMapType = 1;  // Pal based TGA
        u8 mImageType = 1;      // Pal based TGA

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

enum class EAnimGroup
{
    Abe,
    Elum,
    Slig,
    Doors,
    Lights,
    Lifts,
    TrapDoors
};

static const char* ToString(EAnimGroup animGroup)
{
    switch (animGroup)
    {
        case EAnimGroup::Abe:
            return "abe";
        default:
            ALIVE_FATAL("unknown group");
    }
}

struct AnimRecConversionInfo final
{
    AnimId mAnimId;         // which anim?
    EAnimGroup mGroup;      // abe, doors etc
    EReliveLevelIds mAeLvl; // LVL this anim exists in for AE
    EReliveLevelIds mAoLvl; // LVL this anim exists in for AO
    bool mConverted;
};

AnimRecConversionInfo kAnimRecConversionInfo[] = {
    {AnimId::Abe_Arm_Gib, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Abe_Body_Gib, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_StandingTurn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false}

};

struct AnimRecNames final
{
    AnimId mAnimId;
    char_type* mAnimName;
};

constexpr AnimRecNames kAnimRecNames[] = {
    {AnimId::Abe_Arm_Gib, "arm_gib"},
    {AnimId::Abe_Body_Gib, "body_gib"},
    {AnimId::Mudokon_StandingTurn, "standing_turn"}

};

const char_type* AnimBaseName(AnimId id)
{
    for (auto& rec : kAnimRecNames)
    {
        if (rec.mAnimId == id)
        {
            return rec.mAnimName;
        }
    }
    ALIVE_FATAL("No name in mapping table");
}


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
    j = nlohmann::json
    {
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

class AnimationConverter final
{
public:
    struct MaxWH final
    {
        u32 mMaxW = 0;
        u32 mMaxH = 0;
    };

    AnimationConverter(const FileSystem::Path& outputFile, const AnimRecord& rec, const std::vector<u8>& fileData, bool isAoData)
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
        nlohmann::json animJsonInfo = 
        {
            // TODO: Current values are kind of nonsense, map to something sane
            {"frame_rate", pAnimationHeader->field_0_fps},
            {"flip_x", (pAnimationHeader->field_6_flags & AnimationHeader::eFlipXFlag) ? true : false},
            {"flip_y", (pAnimationHeader->field_6_flags & AnimationHeader::eFlipYFlag) ? true : false},
            {"loop", (pAnimationHeader->field_6_flags & AnimationHeader::eLoopFlag) ? true : false},
            {"loop_start_frame", pAnimationHeader->field_4_loop_start_frame},
            //{"number_of_frames", pAnimationHeader->field_2_num_frames},
            // TODO: Remove if not really needed (check after loader is impl'd)
            {"max_width", bestMaxSize.mMaxW},
            {"max_height", bestMaxSize.mMaxH}
        };
        
        animJsonInfo["frames"] = perFrameInfos;

        const std::string animJsonInfoString = animJsonInfo.dump(4);
        AutoFILE jsonFile;
        jsonFile.Open((outputFile.GetPath() + ".json").c_str(), "wb", false);
        jsonFile.Write(reinterpret_cast<const u8*>(animJsonInfoString.c_str()), static_cast<u32>(animJsonInfoString.size()));
    }

private:

    // Calc the max width and height because the ones in the header are often way too big for some reason
    // at least for a single animation in a BAN file
    MaxWH CalcMaxWH(const AnimationHeader* pAnimationHeader)
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

    void ConvertPalToTGAFormat(const AnimationFileHeader* pAnimationFileHeader, AnimationPal& pal)
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

    void DecompressAnimFrame(std::vector<u8>& decompressionBuffer, const FrameHeader* pFrameHeader)
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
                    //CompressionType_3Ae_Decompress_40A6A0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *mDbuf);
                    ALIVE_FATAL("todo");
                }
                //renderer.Upload(AnimFlagsToBitDepth(mFlags), *mDbuf);
               
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

    static constexpr u32 kResHeaderSize = 16;

    u32 CalcWidthAdjustedForBPP(const FrameHeader* pFrameHeader)
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

    u32 CalcImageWidth(const FrameHeader* pFrameHeader)
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

    u32 CalcDecompressionBufferSize(const AnimRecord& rec, const FrameHeader* pFrameHeader)
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

    const FrameInfoHeader* GetFrameInfoHeader(const AnimationHeader* pAnimationHeader, u32 idx)
    {
        const u32 frameOffset = pAnimationHeader->mFrameOffsets[idx];
        auto pFrameInfoHeader = reinterpret_cast<const FrameInfoHeader*>(&mFileData[frameOffset + kResHeaderSize]);
        return pFrameInfoHeader;
    }

    const FrameHeader* GetFrame(const AnimationHeader* pAnimationHeader, u32 idx)
    {
        const FrameInfoHeader* pFrameInfoHeader = GetFrameInfoHeader(pAnimationHeader, idx);
        auto pFrameHeader = reinterpret_cast<const FrameHeader*>(&mFileData[pFrameInfoHeader->field_0_frame_header_offset + kResHeaderSize]);
        return pFrameHeader;
    }

    const std::vector<u8>& mFileData;
    bool mIsAoData = false;
};

const char* AnimRecName(AnimId anim)
{
    for (const auto& rec : kAnimRecNames)
    {
        if (rec.mAnimId == anim)
        {
            return rec.mAnimName;
        }
    }
    ALIVE_FATAL("Unknown anim id");
}

static const char* ToString(AO::LevelIds lvlId)
{
    switch (lvlId)
    {
        case AO::LevelIds::eMenu_0:
            return "menu";
        case AO::LevelIds::eRuptureFarms_1:
            return "rupture_farms";
        case AO::LevelIds::eLines_2:
            return "monsaic_lines";
        case AO::LevelIds::eForest_3:
            return "paramonia";
        case AO::LevelIds::eForestTemple_4:
            return "paramonia_temple";
        case AO::LevelIds::eStockYards_5:
            return "stock_yards";
        case AO::LevelIds::eStockYardsReturn_6:
            return "stock_yards_return";
        case AO::LevelIds::eDesert_8:
            return "scrabania";
        case AO::LevelIds::eDesertTemple_9:
            return "scrabania_temple";
        case AO::LevelIds::eCredits_10:
            return "credits";
        case AO::LevelIds::eBoardRoom_12:
            return "board_room";
        case AO::LevelIds::eRuptureFarmsReturn_13:
            return "rupture_farms_return";
        case AO::LevelIds::eForestChase_14:
            return "paramonia_temple_escape";
        case AO::LevelIds::eDesertEscape_15:
            return "scrabania_temple_escape";
        default:
            return "unknown";
    }
}

static void ReadLvlFileInto(ReliveAPI::LvlReader& archive, const char_type* fileName, std::vector<u8>& fileBuffer)
{
    if (!archive.ReadFileInto(fileBuffer, fileName))
    {
        // TODO
    }
}

static bool endsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static void ConvertPathCollisions(const CollisionInfo& info, const std::vector<u8>& pathResource)
{
    const u8* pData = pathResource.data();
    const u8* pStart = pData + info.field_C_collision_offset;

    auto pCollisionIter = reinterpret_cast<const PathLineAO*>(pStart);
    for (u32 i = 0; i < info.field_10_num_collision_items; i++)
    {
        // TODO: Use AE format lines
        if (pCollisionIter[i].mLineType == eLineTypes::eBackgroundCeiling_7)
        {

        }
    }
}


void to_json(nlohmann::json& j, const RGB16& p)
{
    j = nlohmann::json{
        {"r", p.r},
        {"g", p.g},
        {"b", p.b}};
}

void from_json(const nlohmann::json& j, RGB16& p)
{
    j.at("r").get_to(p.r);
    j.at("g").get_to(p.g);
    j.at("b").get_to(p.b);
}

namespace relive {

class Path_ShadowZone
{
public:
    enum class Scale : s16
    {
        eBoth,
        eHalf,
        eFull,
    };

    RGB16 mRGB;
    Scale mScale = Scale::eFull;
};

NLOHMANN_JSON_SERIALIZE_ENUM(Path_ShadowZone::Scale, {
    {Path_ShadowZone::Scale::eBoth, "both"},
    {Path_ShadowZone::Scale::eFull, "full"},
    {Path_ShadowZone::Scale::eHalf, "half"},
})


void to_json(nlohmann::json& j, const Path_ShadowZone& p)
{
    j = nlohmann::json{
        {"rgb", p.mRGB},
        {"scale", p.mScale},
    };
}


void from_json(const nlohmann::json& j, Path_ShadowZone& p)
{
    j.at("rgb").get_to(p.mRGB);
    j.at("scale").get_to(p.mScale);
}


class Path_ShadowZone_Converter
{
public:
    static Path_ShadowZone From(const AO::Path_ShadowZone& tlv)
    {
        Path_ShadowZone r;
        r.mRGB.SetRGB(tlv.field_1C_r, tlv.field_1E_g, tlv.field_20_b);
        r.mScale = From(tlv.field_24_scale);
        return r;
    }

    static Path_ShadowZone From(const ::Path_ShadowZone& tlv)
    {
        Path_ShadowZone r;
        r.mRGB.SetRGB(tlv.field_14_r, tlv.field_16_g, tlv.field_18_b);
        r.mScale = From(tlv.field_1C_scale);
        return r;
    }

private:
    static Path_ShadowZone::Scale From(::ShadowZoneScale aeScale)
    {
        switch (aeScale)
        {
            case ::ShadowZoneScale::eHalf_1:
                return Path_ShadowZone::Scale::eHalf;
            case ::ShadowZoneScale::eFull_2:
                return Path_ShadowZone::Scale::eFull;
            case ::ShadowZoneScale::eBoth_0:
                return Path_ShadowZone::Scale::eBoth;
        }
        ALIVE_FATAL("Bad AE shadow scale");
    }

    static Path_ShadowZone::Scale From(AO::ShadowZoneScale aoScale)
    {
        switch (aoScale)
        {
            case AO::ShadowZoneScale::eHalf_1:
                return Path_ShadowZone::Scale::eHalf;
            case AO::ShadowZoneScale::eFull_2:
                return Path_ShadowZone::Scale::eFull;
            case AO::ShadowZoneScale::eBoth_0:
                return Path_ShadowZone::Scale::eBoth;
        }
        ALIVE_FATAL("Bad AO shadow scale");
    }
};

} // namespace relive

static void ConvertTLV(const AO::Path_TLV& tlv)
{
    nlohmann::json j;
    switch (tlv.mTlvType32.mType)
    {
        case AO::TlvTypes::ShadowZone_7:
            j.push_back({"shadow", relive::Path_ShadowZone_Converter::From(static_cast<const AO::Path_ShadowZone&>(tlv))});
            break;

        default:
            ALIVE_FATAL("TLV conversion for this type not implemented");
    }
    LOG_INFO(j.dump(4));
}

static void ConvertPathTLVs(const AO::PathData& info, const std::vector<u8>& pathResource)
{
    const u8* pData = pathResource.data();
    const u8* pStart = pData + info.field_14_object_offset;
    const u8* pEnd = pData + info.field_18_object_index_table_offset;

    auto pPathTLV = reinterpret_cast<const AO::Path_TLV*>(pStart);
    while (pPathTLV && reinterpret_cast<const u8*>(pPathTLV) < pEnd)
    {
        // TODO: Convert TLV to ReliveTLV
        ConvertTLV(*pPathTLV);

        // Skip length bytes to get to the start of the next TLV
        const u8* ptr = reinterpret_cast<const u8*>(pPathTLV);
        const u8* pNext = ptr + pPathTLV->mLength;
        pPathTLV = reinterpret_cast<const AO::Path_TLV*>(pNext);
    }
}

static void ConvertPath(const ReliveAPI::LvlFileChunk& pathBndChunk, EReliveLevelIds reliveLvl)
{
    const AO::PathBlyRec* pBlyRec = AO::Path_Get_Bly_Record_434650(reliveLvl, static_cast<u16>(pathBndChunk.Id()));

    ConvertPathCollisions(*pBlyRec->field_8_pCollisionData, pathBndChunk.Data());
    ConvertPathTLVs(*pBlyRec->field_4_pPathData, pathBndChunk.Data());
}

static void ConvertPaths(const ReliveAPI::ChunkedLvlFile& pathBnd, EReliveLevelIds reliveLvl)
{
    for (u32 i = 0; i < pathBnd.ChunkCount(); i++)
    {
        const ReliveAPI::LvlFileChunk& pathBndChunk = pathBnd.ChunkAt(i);
        if (pathBndChunk.Header().mResourceType == AO::ResourceManager::Resource_Path)
        {
            ConvertPath(pathBndChunk, reliveLvl);
        }
    }
}

void DataConversion::ConvertData()
{
    // TODO: Check existing data version, if any

    AO::Path_ShadowZone shadowTlv = {};

    FileSystem fs;
    //CameraConverter cc;

    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ao");
    fs.CreateDirectory(dataDir);

    std::vector<u8> fileBuffer;
    for (s32 lvlIdx = 0; lvlIdx < AO::Path_Get_Paths_Count(); lvlIdx++)
    {
        const AO::LevelIds lvlIdxAsLvl = static_cast<AO::LevelIds>(lvlIdx);
        if (lvlIdxAsLvl == AO::LevelIds::eRemoved_7 || lvlIdxAsLvl == AO::LevelIds::eRemoved_11)
        {
            continue;
        }

        const EReliveLevelIds reliveLvl = MapWrapper::FromAO(lvlIdxAsLvl);
        ReliveAPI::FileIO fileIo;
        ReliveAPI::LvlReader lvlReader(fileIo, (std::string(AO::Path_Get_Lvl_Name(reliveLvl)) + ".LVL").c_str());

        if (lvlReader.IsOpen())
        {
            for (auto& rec : kAnimRecConversionInfo)
            {
                // Animation is in this LVL and not yet converted
                if (!rec.mConverted && rec.mAoLvl == reliveLvl)
                {
                    FileSystem::Path filePath;
                    filePath.Append("relive_data").Append("ao").Append("animations");

                    // e.g "abe"
                    filePath.Append(ToString(rec.mGroup));

                    // Ensure the containing directory exists
                    fs.CreateDirectory(filePath);

                    const auto& animDetails = AO::AnimRec(rec.mAnimId);

                    // e.g "arm_gib"
                    filePath.Append(AnimBaseName(rec.mAnimId));

                    ReadLvlFileInto(lvlReader, animDetails.mBanName, fileBuffer);
                    AnimationConverter animationConverter(filePath, animDetails, fileBuffer, true);

                    // Track what is converted so we know what is missing at the end
                    rec.mConverted = true;
                }
            }

            for (s32 i = 0; i < lvlReader.FileCount(); i++)
            {
                auto fileName = lvlReader.FileNameAt(i);
                if (!fileName.empty())
                {
                    if (endsWith(fileName, ".CAM"))
                    {
                        ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);

                        ReliveAPI::ChunkedLvlFile camFile(fileBuffer);
                  
                        FileSystem::Path pathDir;
                        std::string camBaseName = fileName.substr(0, fileName.length() - 4); // chop off .CAM
                        pathDir.Append("relive_data").Append("ao").Append(ToString(lvlIdxAsLvl));
                        fs.CreateDirectory(pathDir);
                        pathDir.Append(camBaseName);

                        // Convert camera images and FG layers
                        ReliveAPI::CamConverter cc(camFile, pathDir.GetPath());

                        // TODO: Convert any BgAnims in this camera
                    }
                    // TODO: Seek these out instead of converting everything we see since the names are fixed per LVL
                    else if (endsWith(fileName, ".VB") || endsWith(fileName, ".VH") || endsWith(fileName, ".BSQ"))
                    {
                        ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);

                        FileSystem::Path filePath;
                        filePath.Append("relive_data").Append("ao").Append(ToString(lvlIdxAsLvl));
                        fs.CreateDirectory(filePath);
                        filePath.Append(fileName);


                        fs.Save(filePath, fileBuffer);
                    }
                    else if (endsWith(fileName, "PATH.BND"))
                    {
                        // TODO: Path conversion
                        ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);
                        ReliveAPI::ChunkedLvlFile pathBndFile(fileBuffer);
                        ConvertPaths(pathBndFile, reliveLvl);
                    }
                }
            }

        }
        else
        {
            // Fatal, missing LVL file
            ALIVE_FATAL("Couldn't open lvl file");
        }
    }
}
