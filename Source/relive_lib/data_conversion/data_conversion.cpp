#include "data_conversion.hpp"
#include "file_system.hpp"
#include "camera_converter.hpp"
#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/LvlArchive.hpp"
#include "../../AliveLibCommon/AnimResources.hpp"

#include "../MapWrapper.hpp"
#include <vector>

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
