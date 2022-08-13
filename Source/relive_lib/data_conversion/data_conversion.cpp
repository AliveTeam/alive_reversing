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
#include "../MapWrapper.hpp"
#include <vector>
#include <algorithm>

#define JSON_DISABLE_ENUM_SERIALIZATION 1

#include "nlohmann/json.hpp"
#include "../../Tools/relive_api/LvlReaderWriter.hpp"
#include "../../Tools/relive_api/file_api.hpp"
#include "../../Tools/relive_api/CamConverter.hpp"
#include "../Collisions.hpp"
#include "AnimationConverter.hpp"
#include "relive_tlvs_conversion.hpp"

constexpr u32 kDataVersion = 1;

extern const CombinedAnimRecord kAnimRecords[915];
extern const AnimDetails kNullAnimDetails;



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

static void TestTlvConversion()
{
    AO::Path_ShadowZone shadowTlv = {};
    shadowTlv.mTlvType32.mType = AO::TlvTypes::ShadowZone_7;
    ConvertTLV(shadowTlv);

    // TODO: Check from AE tlv


}

void DataConversion::ConvertData()
{
    // TODO: Check existing data version, if any
    TestTlvConversion();

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
