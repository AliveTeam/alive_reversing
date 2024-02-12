#include "AESaveConverter.hpp"
#include "BinaryPath.hpp"
#include "AESaveSerialization.hpp"
#include "data_conversion/data_conversion.hpp"
#include "nlohmann/json.hpp"
#include "file_system.hpp"

    // TODO: Add new relive only types for anything that was copy pasted into here
// and is still used after the engine merging

enum class LevelIds : s16;


static void ConvertOGBlyData(nlohmann::json& j, std::vector<std::unique_ptr<BinaryPath>>& paths, const u8* pSrcFlags)
{
    for (auto& binaryPath : paths)
    {
        for (auto& cam : binaryPath->GetCameras())
        {
            auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
            while (pTlv)
            {
                if (pTlv->mAttribute == relive::QuiksaveAttribute::eClearTlvFlags_1 || pTlv->mAttribute == relive::QuiksaveAttribute::eKeepTlvFlags_2) // Type 0 ignored - actually it should never be written here anyway
                {
                    // TODO: Obtain the guid
                    // Guid::NewGuidFromTlvInfo(pTlv->);

                    const bool terminatorFlagOn = pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List);
                    pTlv->mTlvFlags.Raw().all = *pSrcFlags;
                    j.push_back(pTlv->mTlvFlags.Raw().all);
                    pSrcFlags++;
                    
                    if (terminatorFlagOn && !pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
                    {
                        LOG_WARNING("Save data removed list terminator flag, putting it back");
                        pTlv->mTlvFlags.Set(relive::TlvFlags::eBit3_End_TLV_List);
                    }

                    pTlv->mTlvSpecificMeaning = *pSrcFlags;
                    j.push_back(pTlv->mTlvSpecificMeaning);
                    pSrcFlags++;

                    // TODO: Add an entry to the json with the object guid and the flags/specific meaning data
                }
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }
}


bool AESaveConverter::Convert(const std::vector<u8>& savData, const char_type* pFileName, AESaveConverter::PathsCache& cache)
{
    auto pSavedWorldData = reinterpret_cast<const AEData::Quicksave*>(savData.data());

    const u16* pSavedObjStates = reinterpret_cast<const u16*>(pSavedWorldData->field_55C_objects_state_data);

    nlohmann::json j;
    j = AEData::Quicksave::From(*reinterpret_cast<const AEData::Quicksave*>(pSavedWorldData));

    const PathBlyRec* pBlyRec = Path_Get_Bly_Record(MapWrapper::FromAE(pSavedWorldData->field_204_world_info.mLevel), pSavedWorldData->field_204_world_info.mPath);
    while (*reinterpret_cast<const u32*>(pSavedObjStates) != 0)
    {
        // Maps to AETypes
        pSavedObjStates += ConvertObjectSaveStateData(j, static_cast<AETypes>(*pSavedObjStates), pBlyRec->field_4_pPathData->field_12_object_offset, reinterpret_cast<const u8*>(pSavedObjStates)) / sizeof(u16);
        // TODO: Add the read state as json
    }

    const EReliveLevelIds reliveLvlId = MapWrapper::FromAE(pSavedWorldData->field_204_world_info.mLevel);
    if (cache.mLvlId != reliveLvlId)
    {
        // Reload cache
        cache.mLvlId = reliveLvlId;
        cache.mPaths = ResourceManagerWrapper::LoadPaths(reliveLvlId);
    }

    // Skip the u32 type 0 entry that marks the end of the object stave states data
    const u8* pSrcFlags = reinterpret_cast<const u8*>(pSavedObjStates + 2);
    nlohmann::json blyJson;
    ConvertOGBlyData(blyJson, cache.mPaths, pSrcFlags);
    j["object_bly_data"] = blyJson;

    FileSystem fs;
    return SaveJson(j, fs, pFileName);
}

template <typename OGBinarySaveStateStructure>
static void convert_save(nlohmann::json& j, u32 pathOffset, const u8* pOGSaveFileData)
{
    // Convert OG save structure to relive structure
    const auto& reliveFormatStructure = OGBinarySaveStateStructure::From(*reinterpret_cast<const OGBinarySaveStateStructure*>(pOGSaveFileData), pathOffset);

    // Convert relive structure to json
    j["object_states"].push_back(reliveFormatStructure);
}

s32 AESaveConverter::ConvertObjectSaveStateData(nlohmann::json& j, AETypes type, u32 pathOffset, const u8* pData)
{
    switch (type)
    {
        case ::AETypes::eSligSpawner_2:
            convert_save<AEData::SligSpawnerSaveState>(j, pathOffset, pData);
            return sizeof(AEData::SligSpawnerSaveState);

        case ::AETypes::eLiftMover_9:
            convert_save<AEData::LiftMoverSaveState>(j, pathOffset, pData);
            return sizeof(AEData::LiftMoverSaveState);

        case ::AETypes::eBone_11:
            convert_save<AEData::BoneSaveState>(j, pathOffset, pData);
            return sizeof(AEData::BoneSaveState);

        case ::AETypes::eMinesAlarm_25:
            convert_save<AEData::MinesAlarmSaveState>(j, pathOffset, pData);
            return sizeof(AEData::MinesAlarmSaveState);

        case ::AETypes::eCrawlingSlig_26:
            convert_save<AEData::CrawlingSligSaveState>(j, pathOffset, pData);
            return sizeof(AEData::CrawlingSligSaveState);

        case ::AETypes::eDrill_30:
            convert_save<AEData::DrillSaveState>(j, pathOffset, pData);
            return sizeof(AEData::DrillSaveState);

        case ::AETypes::eEvilFart_45:
            convert_save<AEData::EvilFartSaveState>(j, pathOffset, pData);
            return sizeof(AEData::EvilFartSaveState);

        case ::AETypes::eFleech_50:
            convert_save<AEData::FleechSaveState>(j, pathOffset, pData);
            return sizeof(AEData::FleechSaveState);

        case ::AETypes::eFlyingSlig_54:
            convert_save<AEData::FlyingSligSaveState>(j, pathOffset, pData);
            return sizeof(AEData::FlyingSligSaveState);

        case ::AETypes::eFlyingSligSpawner_55:
            convert_save<AEData::FlyingSligSpawnerSaveState>(j, pathOffset, pData);
            return sizeof(AEData::FlyingSligSpawnerSaveState);

        case ::AETypes::eGameEnderController_57:
            convert_save<AEData::GameEnderControllerSaveState>(j, pathOffset, pData);
            return sizeof(AEData::GameEnderControllerSaveState);

        case ::AETypes::eSlapLock_OrbWhirlWind_60:
            convert_save<AEData::SlapLockWhirlWindSaveState>(j, pathOffset, pData);
            return sizeof(AEData::SlapLockWhirlWindSaveState);

        case ::AETypes::eSlapLock_61:
            convert_save<AEData::SlapLockSaveState>(j, pathOffset, pData);
            return sizeof(AEData::SlapLockSaveState);

        case ::AETypes::eGreeter_64:
            convert_save<AEData::GreeterSaveState>(j, pathOffset, pData);
            return sizeof(AEData::GreeterSaveState);

        case ::AETypes::eGrenade_65:
            convert_save<AEData::GrenadeSaveState>(j, pathOffset, pData);
            return sizeof(AEData::GrenadeSaveState);

        case ::AETypes::eGlukkon_67:
            convert_save<AEData::GlukkonSaveState>(j, pathOffset, pData);
            return sizeof(AEData::GlukkonSaveState);

        case ::AETypes::eAbe_69:
            convert_save<AEData::AbeSaveState>(j, pathOffset, pData);
            return sizeof(AEData::AbeSaveState);

        case ::AETypes::eLiftPoint_78:
            convert_save<AEData::LiftPointSaveState>(j, pathOffset, pData);
            return sizeof(AEData::LiftPointSaveState);

        case ::AETypes::eMudokon_110:
        case ::AETypes::eRingOrLiftMud_81:
            convert_save<AEData::MudokonSaveState>(j, pathOffset, pData);
            return sizeof(AEData::MudokonSaveState);

        case ::AETypes::eMeat_84:
            convert_save<AEData::MeatSaveState>(j, pathOffset, pData);
            return sizeof(AEData::MeatSaveState);

        case ::AETypes::eMineCar_89:
            convert_save<AEData::MineCarSaveState>(j, pathOffset, pData);
            return sizeof(AEData::MineCarSaveState);

        case ::AETypes::eParamite_96:
            convert_save<AEData::ParamiteSaveState>(j, pathOffset, pData);
            return sizeof(AEData::ParamiteSaveState);

        case ::AETypes::eBirdPortal_99:
            convert_save<AEData::BirdPortalSaveState>(j, pathOffset, pData);
            return sizeof(AEData::BirdPortalSaveState);

        case ::AETypes::eThrowableArray_102:
            convert_save<AEData::ThrowableArraySaveState>(j, pathOffset, pData);
            return sizeof(AEData::ThrowableArraySaveState);

        case ::AETypes::eAbilityRing_104:
            convert_save<AEData::AbilityRingSaveState>(j, pathOffset, pData);
            return sizeof(AEData::AbilityRingSaveState);

        case ::AETypes::eRock_105:
            convert_save<AEData::RockSaveState>(j, pathOffset, pData);
            return sizeof(AEData::RockSaveState);

        case ::AETypes::eScrab_112:
            convert_save<AEData::ScrabSaveState>(j, pathOffset, pData);
            return sizeof(AEData::ScrabSaveState);

        case ::AETypes::eScrabSpawner_113:
            convert_save<AEData::ScrabSpawnerSaveState>(j, pathOffset, pData);
            return sizeof(AEData::ScrabSpawnerSaveState);

        case ::AETypes::eSlamDoor_122:
            convert_save<AEData::SlamDoorSaveState>(j, pathOffset, pData);
            return sizeof(AEData::SlamDoorSaveState);

        case ::AETypes::eSlig_125:
            convert_save<AEData::SligSaveState>(j, pathOffset, pData);
            return sizeof(AEData::SligSaveState);

        case ::AETypes::eSlog_126:
            convert_save<AEData::SlogSaveState>(j, pathOffset, pData);
            return sizeof(AEData::SlogSaveState);

        case ::AETypes::eSlurg_129:
            convert_save<AEData::SlurgSaveState>(j, pathOffset, pData);
            return sizeof(AEData::SlurgSaveState);

        case ::AETypes::eTimerTrigger_136:
            convert_save<AEData::TimerTriggerSaveState>(j, pathOffset, pData);
            return sizeof(AEData::TimerTriggerSaveState);

        case ::AETypes::eTrapDoor_142:
            convert_save<AEData::TrapDoorSaveState>(j, pathOffset, pData);;
            return sizeof(AEData::TrapDoorSaveState);

        case ::AETypes::eUXB_143:
            convert_save<AEData::UXBSaveState>(j, pathOffset, pData);
            return sizeof(AEData::UXBSaveState);

        case ::AETypes::eWorkWheel_148:
            convert_save<AEData::WorkWheelSaveState>(j, pathOffset, pData);
            return sizeof(AEData::WorkWheelSaveState);

        default:
            ALIVE_FATAL("No create save state for type %d", static_cast<s32>(type));
    }
}
