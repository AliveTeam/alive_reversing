#include "AESaveConverter.hpp"
#include "BinaryPath.hpp"
#include "AESaveSerialization.hpp"
#include "data_conversion/data_conversion.hpp"
#include "nlohmann/json.hpp"

// TODO: Add new relive only types for anything that was copy pasted into here
// and is still used after the engine merging

enum class LevelIds : s16;

bool AESaveConverter::Convert(const std::vector<u8>& savData, const char_type* pFileName)
{
    auto pSavedWorldData = reinterpret_cast<const AEData::Quicksave*>(savData.data());

    const u16* pSavedObjStates = reinterpret_cast<const u16*>(pSavedWorldData->field_55C_objects_state_data);

    nlohmann::json j;
    j["world_data"] = AEData::Quicksave::From(*reinterpret_cast<const AEData::Quicksave*>(pSavedWorldData));

    while (*reinterpret_cast<const u32*>(pSavedObjStates) != 0)
    {
        // Maps to AETypes
        pSavedObjStates += ConvertObjectSaveStateData(j, static_cast<AETypes>(*pSavedObjStates), reinterpret_cast<const u8*>(pSavedObjStates)) / sizeof(u16);
        // TODO: Add the read state as json
    }

    auto paths = ResourceManagerWrapper::LoadPaths(MapWrapper::FromAE(pSavedWorldData->field_204_world_info.mLevel));
    // Skip the 2 zero entries, the saved flag words come after the object save state data
    const u8* pSrcFlags = reinterpret_cast<const u8*>(pSavedObjStates + 2);
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
                    //Guid::NewGuidFromTlvInfo(pTlv->);

                    const bool terminatorFlagOn = pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List);
                    pTlv->mTlvFlags.Raw().all = *pSrcFlags;
                    pSrcFlags++;
                    if (terminatorFlagOn && !pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
                    {
                        LOG_WARNING("Save data removed list terminator flag, putting it back");
                        pTlv->mTlvFlags.Set(relive::TlvFlags::eBit3_End_TLV_List);
                    }

                    pTlv->mTlvSpecificMeaning = *pSrcFlags;
                    pSrcFlags++;

                    // TODO: Add an entry to the json with the object guid and the flags/specific meaning data
                }
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }

    FileSystem fs;
    return SaveJson(j, fs, pFileName);
}

template <typename save_state>
static void convert_save(nlohmann::json& j, const u8* pData)
{
    j["object_states"].push_back(save_state::From(*reinterpret_cast<const save_state*>(pData)));
}

s32 AESaveConverter::ConvertObjectSaveStateData(nlohmann::json& j, AETypes type, const u8* pData)
{
    switch (type)
    {
        case ::AETypes::eSligSpawner_2:
            convert_save<AEData::SligSpawnerSaveState>(j, pData);
            return sizeof(AEData::SligSpawnerSaveState);

        case ::AETypes::eLiftMover_9:
            convert_save<AEData::LiftMoverSaveState>(j, pData);
            return sizeof(AEData::LiftMoverSaveState);

        case ::AETypes::eBone_11:
            convert_save<AEData::BoneSaveState>(j, pData);
            return sizeof(AEData::BoneSaveState);

        case ::AETypes::eMinesAlarm_25:
            convert_save<AEData::MinesAlarmSaveState>(j, pData);
            return sizeof(AEData::MinesAlarmSaveState);

        case ::AETypes::eCrawlingSlig_26:
            convert_save<AEData::CrawlingSligSaveState>(j, pData);
            return sizeof(AEData::CrawlingSligSaveState);

        case ::AETypes::eDrill_30:
            convert_save<AEData::DrillSaveState>(j, pData);
            return sizeof(AEData::DrillSaveState);

        case ::AETypes::eEvilFart_45:
            convert_save<AEData::EvilFartSaveState>(j, pData);
            return sizeof(AEData::EvilFartSaveState);

        case ::AETypes::eFleech_50:
            convert_save<AEData::FleechSaveState>(j, pData);
            return sizeof(AEData::FleechSaveState);

        case ::AETypes::eFlyingSlig_54:
            convert_save<AEData::FlyingSligSaveState>(j, pData);
            return sizeof(AEData::FlyingSligSaveState);

        case ::AETypes::eFlyingSligSpawner_55:
            convert_save<AEData::FlyingSligSpawnerSaveState>(j, pData);
            return sizeof(AEData::FlyingSligSpawnerSaveState);

        case ::AETypes::eGameEnderController_57:
            convert_save<AEData::GameEnderControllerSaveState>(j, pData);
            return sizeof(AEData::GameEnderControllerSaveState);

        case ::AETypes::eSlapLock_OrbWhirlWind_60:
            convert_save<AEData::SlapLockWhirlWindSaveState>(j, pData);
            return sizeof(AEData::SlapLockWhirlWindSaveState);

        case ::AETypes::eSlapLock_61:
            convert_save<AEData::SlapLockSaveState>(j, pData);
            return sizeof(AEData::SlapLockSaveState);

        case ::AETypes::eGreeter_64:
            convert_save<AEData::GreeterSaveState>(j, pData);
            return sizeof(AEData::GreeterSaveState);

        case ::AETypes::eGrenade_65:
            convert_save<AEData::GrenadeSaveState>(j, pData);
            return sizeof(AEData::GrenadeSaveState);

        case ::AETypes::eGlukkon_67:
            convert_save<AEData::GlukkonSaveState>(j, pData);
            return sizeof(AEData::GlukkonSaveState);

        case ::AETypes::eAbe_69:
            convert_save<AEData::AbeSaveState>(j, pData);
            return sizeof(AEData::AbeSaveState);

        case ::AETypes::eLiftPoint_78:
            convert_save<AEData::LiftPointSaveState>(j, pData);
            return sizeof(AEData::LiftPointSaveState);

        case ::AETypes::eMudokon_110:
        case ::AETypes::eRingOrLiftMud_81:
            convert_save<AEData::MudokonSaveState>(j, pData);
            return sizeof(AEData::MudokonSaveState);

        case ::AETypes::eMeat_84:
            convert_save<AEData::MeatSaveState>(j, pData);
            return sizeof(AEData::MeatSaveState);

        case ::AETypes::eMineCar_89:
            convert_save<AEData::MineCarSaveState>(j, pData);
            return sizeof(AEData::MineCarSaveState);

        case ::AETypes::eParamite_96:
            convert_save<AEData::ParamiteSaveState>(j, pData);
            return sizeof(AEData::ParamiteSaveState);

        case ::AETypes::eBirdPortal_99:
            convert_save<AEData::BirdPortalSaveState>(j, pData);
            return sizeof(AEData::BirdPortalSaveState);

        case ::AETypes::eThrowableArray_102:
            convert_save<AEData::ThrowableArraySaveState>(j, pData);
            return sizeof(AEData::ThrowableArraySaveState);

        case ::AETypes::eAbilityRing_104:
            convert_save<AEData::AbilityRingSaveState>(j, pData);
            return sizeof(AEData::AbilityRingSaveState);

        case ::AETypes::eRock_105:
            convert_save<AEData::RockSaveState>(j, pData);
            return sizeof(AEData::RockSaveState);

        case ::AETypes::eScrab_112:
            convert_save<AEData::ScrabSaveState>(j, pData);
            return sizeof(AEData::ScrabSaveState);

        case ::AETypes::eScrabSpawner_113:
            convert_save<AEData::ScrabSpawnerSaveState>(j, pData);
            return sizeof(AEData::ScrabSpawnerSaveState);

        case ::AETypes::eSlamDoor_122:
            convert_save<AEData::SlamDoorSaveState>(j, pData);
            return sizeof(AEData::SlamDoorSaveState);

        case ::AETypes::eSlig_125:
            convert_save<AEData::SligSaveState>(j, pData);
            return sizeof(AEData::SligSaveState);

        case ::AETypes::eSlog_126:
            convert_save<AEData::SlogSaveState>(j, pData);
            return sizeof(AEData::SlogSaveState);

        case ::AETypes::eSlurg_129:
            convert_save<AEData::SlurgSaveState>(j, pData);
            return sizeof(AEData::SlurgSaveState);

        case ::AETypes::eTimerTrigger_136:
            convert_save<AEData::TimerTriggerSaveState>(j, pData);
            return sizeof(AEData::TimerTriggerSaveState);

        case ::AETypes::eTrapDoor_142:
            convert_save<AEData::TrapDoorSaveState>(j, pData);;
            return sizeof(AEData::TrapDoorSaveState);

        case ::AETypes::eUXB_143:
            convert_save<AEData::UXBSaveState>(j, pData);
            return sizeof(AEData::UXBSaveState);

        case ::AETypes::eWorkWheel_148:
            convert_save<AEData::WorkWheelSaveState>(j, pData);
            return sizeof(AEData::WorkWheelSaveState);

        default:
            ALIVE_FATAL("No create save state for type %d", static_cast<s32>(type));
    }
}
