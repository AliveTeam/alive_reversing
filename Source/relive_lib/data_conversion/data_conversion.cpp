#include "data_conversion.hpp"
#include "file_system.hpp"
#include "camera_converter.hpp"
#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/LvlArchive.hpp"
#include "../../AliveLibCommon/AnimResources.hpp"
#include "../../AliveLibCommon/BaseGameAutoPlayer.hpp"
#include "../Animation.hpp"
#include "../Compression.hpp"
#include "../MapWrapper.hpp"
#include <vector>

constexpr u32 kDataVersion = 1;

extern const CombinedAnimRecord kAnimRecords[915];
extern const AnimDetails kNullAnimDetails;




class TgaFile final
{
public:
    void Save(const char_type* pFileName, const u16* pal256, const std::vector<u8>& pixelData, u32 width, u32 height)
    {
        // The TGA header uses a var length id string which means we can't just use
        // a struct to represent it since the alignment is not fixed until after this field.
        AutoFILE f;
        f.Open(pFileName, "wb", false);

        u8 mIdLength = 0;
        u8 mColourMapType = 1;  // Pal based TGA
        u8 mImageType = 1;      // Pal based TGA
        u8 pad = 0;
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
        u8 mDescriptor = 0;
        f.Write(mXOrigin);
        f.Write(mYOrigin);
        f.Write(mWidth);
        f.Write(mHeight);
        f.Write(mBitsPerPixel);
        f.Write(mDescriptor);

        f.Write(reinterpret_cast<const u8*>(&pal256[0]), sizeof(u16) * 256);

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
    {AnimId::Abe_Body_Gib, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false}};

struct AnimRecNames final
{
    AnimId mAnimId;
    char_type* mAnimName;
};

constexpr AnimRecNames kAnimRecNames[] = {
    {AnimId::Abe_Arm_Gib, "arm_gib"},
    {AnimId::Abe_Body_Gib, "body_gib"}
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

class AnimationConverter final
{
public:
    AnimationConverter(const AnimRecord& rec, const std::vector<u8>& fileData, bool isAoData)
        : mFileData(fileData)
        , mIsAoData(isAoData)
    {
        const auto pAnimationFileHeader = reinterpret_cast<const AnimationFileHeader*>(&mFileData[kResHeaderSize]);

        // Get the CLUT/pal
        u16 pal[256] = {};
        for (u32 i = 0; i < pAnimationFileHeader->mClutSize; i++)
        {
            // TODO: Convert to the RGB format that TGA wants
            pal[i] = pAnimationFileHeader->mClutData[i];
        }

        const auto pAnimationHeader = reinterpret_cast<const AnimationHeader*>(&mFileData[rec.mFrameTableOffset + kResHeaderSize]);

        // Get the size required to decompres a single frame
        const u32 decompressionBufferSize = CalcDecompressionBufferSize(rec, pAnimationFileHeader, GetFrame(pAnimationHeader, 0));

        std::vector<u8> decompressionBuffer(decompressionBufferSize);

        // Add each frame
        for (s32 i = 0; i < pAnimationHeader->field_2_num_frames; i++)
        {
            // 36x23 depth 8
            const FrameHeader* pFrameHeader = GetFrame(pAnimationHeader, i);
            DecompressAnimFrame(decompressionBuffer, pFrameHeader);

            TgaFile tgaFile;
            std::string fileName = AnimBaseName(rec.mId) + std::string("_") + std::to_string(i) + ".tga";
            tgaFile.Save(fileName.c_str(), pal, decompressionBuffer, CalcImageWidth(pFrameHeader), pFrameHeader->field_5_height);
        }
    }

private:
    void DecompressAnimFrame(std::vector<u8>& decompressionBuffer, const FrameHeader* pFrameHeader)
    {
        switch (pFrameHeader->field_7_compression_type)
        {
            case CompressionType::eType_0_NoCompression:
                // reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2)
                break;

            case CompressionType::eType_2_ThreeToFourBytes:
                /*
                CompressionType2_Decompress_40AA50(
                    reinterpret_cast<const u8*>(&pFrameHeader[1]),
                    *mDbuf,
                    width_bpp_adjusted * pFrameHeader->field_5_height * 2);
                // AnimFlagsToBitDepth(mFlags)
                */
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
                // CompressionType_4Or5_Decompress_4ABAB0(reinterpret_cast<const u8*>(&pFrameHeader->field_8_width2), *mDbuf);
                // renderer.Upload(AnimFlagsToBitDepth(mFlags), vram_rect, *mDbuf);
                break;

            case CompressionType::eType_6_RLE:
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

    u32 CalcDecompressionBufferSize(const AnimRecord& rec, const AnimationFileHeader* animFileHeader, const FrameHeader* pFrameHeader)
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

    const FrameHeader* GetFrame(const AnimationHeader* pAnimationHeader, u32 idx)
    {
        const u32 frameOffset = pAnimationHeader->mFrameOffsets[idx];
        auto pFrame = reinterpret_cast<const FrameInfoHeader*>(&mFileData[frameOffset + kResHeaderSize]);

        auto pFrameHeader = reinterpret_cast<const FrameHeader*>(&mFileData[pFrame->field_0_frame_header_offset + kResHeaderSize]);
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
            return "menu";
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

static void ReadLvlFileInto(AO::LvlArchive& archive, const char_type* fileName, std::vector<u8>& fileBuffer)
{
    auto banFile = archive.Find_File_Record(fileName);

    fileBuffer.resize(banFile->field_10_num_sectors * 2048);
    archive.Read_File(banFile, fileBuffer.data());
    fileBuffer.resize(banFile->field_14_file_size);
}

void DataConversion::ConvertData()
{
    // TODO: Check existing data version, if any

    FileSystem fs;
    //CameraConverter cc;

    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    fs.CreateDirectory(dataDir);
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

        AO::LvlArchive archive;
        const EReliveLevelIds reliveLvl = MapWrapper::FromAO(lvlIdxAsLvl);
        if (archive.OpenArchive(AO::CdLvlName(reliveLvl), 0))
        {
            for (auto& rec : kAnimRecConversionInfo)
            {
                // Animation is in this LVL and not yet converted
                if (!rec.mConverted && rec.mAoLvl == reliveLvl)
                {
                    FileSystem::Path filePath;
                    filePath.Append("relive_data").Append("ao").Append(ToString(lvlIdxAsLvl));
                    fs.CreateDirectory(filePath);

                    // e.g "abe"
                    filePath.Append(ToString(rec.mGroup));

                    // e.g "arm_gib"
                    filePath.Append(AnimRecName(rec.mAnimId));

                    const auto& animDetails = AO::AnimRec(rec.mAnimId);

                    ReadLvlFileInto(archive, animDetails.mBanName, fileBuffer);
                    AnimationConverter animationConverter(animDetails, fileBuffer, true);

                    // Track what is converted so we know what is missing at the end
                    rec.mConverted = true;
                }
            }

            for (u32 i = 0; i < archive.FileCount(); i++)
            {
                auto pFileRec = archive.FileAt(i);
                if (pFileRec->field_0_file_name)
                {
                    std::string fileName(pFileRec->field_0_file_name, strnlen(pFileRec->field_0_file_name, 12));

                    ReadLvlFileInto(archive, fileName.c_str(), fileBuffer);

                    FileSystem::Path filePath;
                    filePath.Append("relive_data").Append("ao").Append(ToString(lvlIdxAsLvl));
                    fs.CreateDirectory(filePath);
                    filePath.Append(fileName);


                    fs.Save(filePath, fileBuffer);
                }
            }

        }
        else
        {
            // Fatal, missing LVL file
        }
    }
}
