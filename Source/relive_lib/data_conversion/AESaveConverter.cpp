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
    j["world_data"].push_back(AEData::Quicksave::From(*reinterpret_cast<const AEData::Quicksave*>(pSavedWorldData)));

    while (*reinterpret_cast<const u32*>(pSavedObjStates) != 0)
    {
        nlohmann::json saveState;

        // Maps to AETypes
        pSavedObjStates += ConvertObjectSaveStateData(saveState, static_cast<AETypes>(*pSavedObjStates), reinterpret_cast<const u8*>(pSavedObjStates)) / sizeof(u16);
        j["object_states"].push_back(saveState);
        // TODO: Add the read state as json
    }

    auto paths = ResourceManagerWrapper::LoadPaths(MapWrapper::FromAE(pSavedWorldData->field_204_world_info.field_4_level));
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

s32 AESaveConverter::ConvertObjectSaveStateData(nlohmann::json& j, AETypes type, const u8* pData)
{
    switch (type)
    {
        case ::AETypes::eSligSpawner_2:
            to_json(j, AEData::SligSpawnerSaveState::From(*reinterpret_cast<const AEData::SligSpawnerSaveState*>(pData)));
            return sizeof(AEData::SligSpawnerSaveState);
            break;

        case ::AETypes::eLiftMover_9:
            to_json(j, AEData::LiftMoverSaveState::From(*reinterpret_cast<const AEData::LiftMoverSaveState*>(pData)));
            return sizeof(AEData::LiftMoverSaveState);
            break;

        case ::AETypes::eBone_11:
            to_json(j, AEData::BoneSaveState::From(*reinterpret_cast<const AEData::BoneSaveState*>(pData)));
            return sizeof(AEData::BoneSaveState);
            break;

        case ::AETypes::eMinesAlarm_25:
            to_json(j, AEData::MinesAlarmSaveState::From(*reinterpret_cast<const AEData::MinesAlarmSaveState*>(pData)));
            return sizeof(AEData::MinesAlarmSaveState);
            break;

        case ::AETypes::eCrawlingSlig_26:
            to_json(j, AEData::CrawlingSligSaveState::From(*reinterpret_cast<const AEData::CrawlingSligSaveState*>(pData)));
            return sizeof(AEData::CrawlingSligSaveState);
            break;

        case ::AETypes::eDrill_30:
            to_json(j, AEData::DrillSaveState::From(*reinterpret_cast<const AEData::DrillSaveState*>(pData)));
            return sizeof(AEData::DrillSaveState);
            break;

        case ::AETypes::eEvilFart_45:
            to_json(j, AEData::EvilFartSaveState::From(*reinterpret_cast<const AEData::EvilFartSaveState*>(pData)));
            return sizeof(AEData::EvilFartSaveState);
            break;

        case ::AETypes::eFleech_50:
            to_json(j, AEData::FleechSaveState::From(*reinterpret_cast<const AEData::FleechSaveState*>(pData)));
            return sizeof(AEData::FleechSaveState);
            break;

        case ::AETypes::eFlyingSlig_54:
            to_json(j, AEData::FlyingSligSaveState::From(*reinterpret_cast<const AEData::FlyingSligSaveState*>(pData)));
            return sizeof(AEData::FlyingSligSaveState);
            break;

        case ::AETypes::eFlyingSligSpawner_55:
            to_json(j, AEData::FlyingSligSpawnerSaveState::From(*reinterpret_cast<const AEData::FlyingSligSpawnerSaveState*>(pData)));
            return sizeof(AEData::FlyingSligSpawnerSaveState);
            break;

        case ::AETypes::eGameEnderController_57:
            to_json(j, AEData::GameEnderControllerSaveState::From(*reinterpret_cast<const AEData::GameEnderControllerSaveState*>(pData)));
            return sizeof(AEData::GameEnderControllerSaveState);
            break;

        case ::AETypes::eSlapLock_OrbWhirlWind_60:
            to_json(j, AEData::SlapLockWhirlWindSaveState::From(*reinterpret_cast<const AEData::SlapLockWhirlWindSaveState*>(pData)));
            return sizeof(AEData::SlapLockWhirlWindSaveState);
            break;

        case ::AETypes::eSlapLock_61:
            to_json(j, AEData::SlapLockSaveState::From(*reinterpret_cast<const AEData::SlapLockSaveState*>(pData)));
            return sizeof(AEData::SlapLockSaveState);
            break;

        case ::AETypes::eGreeter_64:
            to_json(j, AEData::GreeterSaveState::From(*reinterpret_cast<const AEData::GreeterSaveState*>(pData)));
            return sizeof(AEData::GreeterSaveState);
            break;

        case ::AETypes::eGrenade_65:
            to_json(j, AEData::GrenadeSaveState::From(*reinterpret_cast<const AEData::GrenadeSaveState*>(pData)));
            return sizeof(AEData::GrenadeSaveState);
            break;

        case ::AETypes::eGlukkon_67:
            to_json(j, AEData::GlukkonSaveState::From(*reinterpret_cast<const AEData::GlukkonSaveState*>(pData)));
            return sizeof(AEData::GlukkonSaveState);
            break;

        case ::AETypes::eAbe_69:
            to_json(j, AEData::AbeSaveState::From(*reinterpret_cast<const AEData::AbeSaveState*>(pData)));
            return sizeof(AEData::AbeSaveState);
            break;

        case ::AETypes::eLiftPoint_78:
            to_json(j, AEData::LiftPointSaveState::From(*reinterpret_cast<const AEData::LiftPointSaveState*>(pData)));
            return sizeof(AEData::LiftPointSaveState);
            break;

        case ::AETypes::eMudokon_110:
        case ::AETypes::eRingOrLiftMud_81:
            to_json(j, AEData::MudokonSaveState::From(*reinterpret_cast<const AEData::MudokonSaveState*>(pData)));
            return sizeof(AEData::MudokonSaveState);
            break;

        case ::AETypes::eMeat_84:
            to_json(j, AEData::MeatSaveState::From(*reinterpret_cast<const AEData::MeatSaveState*>(pData)));
            return sizeof(AEData::MeatSaveState);
            break;

        case ::AETypes::eMineCar_89:
            to_json(j, AEData::MineCarSaveState::From(*reinterpret_cast<const AEData::MineCarSaveState*>(pData)));
            return sizeof(AEData::MineCarSaveState);
            break;

        case ::AETypes::eParamite_96:
            to_json(j, AEData::ParamiteSaveState::From(*reinterpret_cast<const AEData::ParamiteSaveState*>(pData)));
            return sizeof(AEData::ParamiteSaveState);
            break;

        case ::AETypes::eBirdPortal_99:
            to_json(j, AEData::BirdPortalSaveState::From(*reinterpret_cast<const AEData::BirdPortalSaveState*>(pData)));
            return sizeof(AEData::BirdPortalSaveState);
            break;

        case ::AETypes::eThrowableArray_102:
            to_json(j, AEData::ThrowableArraySaveState::From(*reinterpret_cast<const AEData::ThrowableArraySaveState*>(pData)));
            return sizeof(AEData::ThrowableArraySaveState);
            break;

        case ::AETypes::eAbilityRing_104:
            to_json(j, AEData::AbilityRingSaveState::From(*reinterpret_cast<const AEData::AbilityRingSaveState*>(pData)));
            return sizeof(AEData::AbilityRingSaveState);
            break;

        case ::AETypes::eRock_105:
            to_json(j, AEData::RockSaveState::From(*reinterpret_cast<const AEData::RockSaveState*>(pData)));
            return sizeof(AEData::RockSaveState);
            break;

        case ::AETypes::eScrab_112:
            to_json(j, AEData::ScrabSaveState::From(*reinterpret_cast<const AEData::ScrabSaveState*>(pData)));
            return sizeof(AEData::ScrabSaveState);
            break;

        case ::AETypes::eScrabSpawner_113:
            to_json(j, AEData::ScrabSpawnerSaveState::From(*reinterpret_cast<const AEData::ScrabSpawnerSaveState*>(pData)));
            return sizeof(AEData::ScrabSpawnerSaveState);
            break;

        case ::AETypes::eSlamDoor_122:
        {
            to_json(j, AEData::SlamDoorSaveState::From(*reinterpret_cast<const AEData::SlamDoorSaveState*>(pData)));
            return sizeof(AEData::SlamDoorSaveState);
        }

        case ::AETypes::eSlig_125:
            to_json(j, AEData::SligSaveState::From(*reinterpret_cast<const AEData::SligSaveState*>(pData)));
            return sizeof(AEData::SligSaveState);
            break;

        case ::AETypes::eSlog_126:
            to_json(j, AEData::SlogSaveState::From(*reinterpret_cast<const AEData::SlogSaveState*>(pData)));
            return sizeof(AEData::SlogSaveState);
            break;

        case ::AETypes::eSlurg_129:
            to_json(j, AEData::SlurgSaveState::From(*reinterpret_cast<const AEData::SlurgSaveState*>(pData)));
            return sizeof(AEData::SlurgSaveState);
            break;

        case ::AETypes::eTimerTrigger_136:
            to_json(j, AEData::TimerTriggerSaveState::From(*reinterpret_cast<const AEData::TimerTriggerSaveState*>(pData)));
            return sizeof(AEData::TimerTriggerSaveState);
            break;

        case ::AETypes::eTrapDoor_142:
            to_json(j, AEData::TrapDoorSaveState::From(*reinterpret_cast<const AEData::TrapDoorSaveState*>(pData)));
            return sizeof(AEData::TrapDoorSaveState);
            break;

        case ::AETypes::eUXB_143:
            to_json(j, AEData::UXBSaveState::From(*reinterpret_cast<const AEData::UXBSaveState*>(pData)));
            return sizeof(AEData::UXBSaveState);
            break;

        case ::AETypes::eWorkWheel_148:
            to_json(j, AEData::WorkWheelSaveState::From(*reinterpret_cast<const AEData::WorkWheelSaveState*>(pData)));
            return sizeof(AEData::WorkWheelSaveState);
            break;

        default:
            ALIVE_FATAL("No create save state for type %d", static_cast<s32>(type));
    }
}
