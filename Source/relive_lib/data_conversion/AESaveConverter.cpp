#include "AESaveConverter.hpp"

// TODO: Add new relive only types for anything that was copy pasted into here
// and is still used after the engine merging

struct SwitchStates final
{
    s8 mData[256];
};

struct Quicksave_PSX_Header final
{
    u8 field_0_frame_1_name[128];
    s8 field_80_frame_2_padding[128];
    s8 field_100_frame_3_padding[128];
    s8 field_180_frame_4_padding[128];
};
ALIVE_ASSERT_SIZEOF(Quicksave_PSX_Header, 0x200);

enum class LevelIds : s16;

struct Quicksave_WorldInfo final
{
    s32 mGnFrame;
    LevelIds field_4_level;
    s16 field_6_path;
    s16 field_8_cam;
    s16 mSaveFileId;
    s16 field_C_controlled_x;
    s16 field_E_controlled_y;
    s16 field_10_controlled_scale;
    s16 field_12_saved_muds;
    s16 field_14_killed_muds;
    s8 field_16_muds_in_area;
    s8 field_17_last_saved_killed_muds_per_path;
    s8 field_18_saved_killed_muds_per_zulag[20];
    s8 field_2C_current_zulag_number;
    s8 field_2D_total_meter_bars;
    s16 field_2E_use_alt_save_header;
    s16 field_30_bDrawMeterCountDown;
    s16 mVisitedBonewerkz;
    s16 mVisitedBarracks;
    s16 mVisitedFeecoEnder;
    s32 mGasTimer;
    s16 mAbeInvincible;
    s16 field_3E_padding;
};
ALIVE_ASSERT_SIZEOF(Quicksave_WorldInfo, 0x40);

struct Quicksave final
{
    Quicksave_PSX_Header field_0_header;
    s32 field_200_accumulated_obj_count;
    Quicksave_WorldInfo field_204_world_info;
    Quicksave_WorldInfo field_244_restart_path_world_info;
    u8 field_284_restart_path_abe_state[216];
    SwitchStates field_35C_restart_path_switch_states;
    SwitchStates field_45C_switch_states;
    u8 field_55C_objects_state_data[6820];
};
ALIVE_ASSERT_SIZEOF(Quicksave, 0x2000);


bool AESaveConverter::Convert(const std::vector<u8>& savData, const char_type* /*pFileName*/)
{
    auto pSaveData = reinterpret_cast<const Quicksave*>(savData.data());

    const u16* pSaveData2 = reinterpret_cast<const u16*>(pSaveData->field_55C_objects_state_data);

    // TODO: Add the required world info to the json

    while (*reinterpret_cast<const u32*>(pSaveData2) != 0)
    {
        // Maps to AETypes
        pSaveData2 += ConvertObjectSaveStateData(static_cast<AETypes>(*pSaveData2), reinterpret_cast<const u8*>(pSaveData2)) / sizeof(u16);

        // TODO: Add the read state as json
    }

    auto paths = ResourceManagerWrapper::LoadPaths(MapWrapper::FromAE(pSaveData->field_204_world_info.field_4_level));
    // Skip the 2 zero entries, the saved flag words come after the object save state data
    const u8* pSrcFlags = reinterpret_cast<const u8*>(pSaveData2 + 2);
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
    return true;
}

s32 AESaveConverter::ConvertObjectSaveStateData(AETypes type, const u8* pData)
{
    switch (type)
    {
        case ::AETypes::eSligSpawner_2:
            AddObjectState(AEData::SligSpawnerSaveState::From(*reinterpret_cast<const AEData::SligSpawnerSaveState*>(pData)));
            return sizeof(AEData::SligSpawnerSaveState);
            break;

        case ::AETypes::eLiftMover_9:
            AddObjectState(AEData::LiftMoverSaveState::From(*reinterpret_cast<const AEData::LiftMoverSaveState*>(pData)));
            return sizeof(AEData::LiftMoverSaveState);
            break;

        case ::AETypes::eBone_11:
            AddObjectState(AEData::BoneSaveState::From(*reinterpret_cast<const AEData::BoneSaveState*>(pData)));
            return sizeof(AEData::BoneSaveState);
            break;

        case ::AETypes::eMinesAlarm_25:
            AddObjectState(AEData::MinesAlarmSaveState::From(*reinterpret_cast<const AEData::MinesAlarmSaveState*>(pData)));
            return sizeof(AEData::MinesAlarmSaveState);
            break;

        case ::AETypes::eCrawlingSlig_26:
            AddObjectState(AEData::CrawlingSligSaveState::From(*reinterpret_cast<const AEData::CrawlingSligSaveState*>(pData)));
            return sizeof(AEData::CrawlingSligSaveState);
            break;

        case ::AETypes::eDrill_30:
            AddObjectState(AEData::DrillSaveState::From(*reinterpret_cast<const AEData::DrillSaveState*>(pData)));
            return sizeof(AEData::DrillSaveState);
            break;

        case ::AETypes::eEvilFart_45:
            AddObjectState(AEData::EvilFartSaveState::From(*reinterpret_cast<const AEData::EvilFartSaveState*>(pData)));
            return sizeof(AEData::EvilFartSaveState);
            break;

        case ::AETypes::eFleech_50:
            AddObjectState(AEData::FleechSaveState::From(*reinterpret_cast<const AEData::FleechSaveState*>(pData)));
            return sizeof(AEData::FleechSaveState);
            break;

        case ::AETypes::eFlyingSlig_54:
            AddObjectState(AEData::FlyingSligSaveState::From(*reinterpret_cast<const AEData::FlyingSligSaveState*>(pData)));
            return sizeof(AEData::FlyingSligSaveState);
            break;

        case ::AETypes::eFlyingSligSpawner_55:
            AddObjectState(AEData::FlyingSligSpawnerSaveState::From(*reinterpret_cast<const AEData::FlyingSligSpawnerSaveState*>(pData)));
            return sizeof(AEData::FlyingSligSpawnerSaveState);
            break;

        case ::AETypes::eGameEnderController_57:
            AddObjectState(AEData::GameEnderControllerSaveState::From(*reinterpret_cast<const AEData::GameEnderControllerSaveState*>(pData)));
            return sizeof(AEData::GameEnderControllerSaveState);
            break;

        case ::AETypes::eSlapLock_OrbWhirlWind_60:
            AddObjectState(AEData::SlapLockWhirlWindSaveState::From(*reinterpret_cast<const AEData::SlapLockWhirlWindSaveState*>(pData)));
            return sizeof(AEData::SlapLockWhirlWindSaveState);
            break;

        case ::AETypes::eSlapLock_61:
            AddObjectState(AEData::SlapLockSaveState::From(*reinterpret_cast<const AEData::SlapLockSaveState*>(pData)));
            return sizeof(AEData::SlapLockSaveState);
            break;

        case ::AETypes::eGreeter_64:
            AddObjectState(AEData::GreeterSaveState::From(*reinterpret_cast<const AEData::GreeterSaveState*>(pData)));
            return sizeof(AEData::GreeterSaveState);
            break;

        case ::AETypes::eGrenade_65:
            AddObjectState(AEData::GrenadeSaveState::From(*reinterpret_cast<const AEData::GrenadeSaveState*>(pData)));
            return sizeof(AEData::GrenadeSaveState);
            break;

        case ::AETypes::eGlukkon_67:
            AddObjectState(AEData::GlukkonSaveState::From(*reinterpret_cast<const AEData::GlukkonSaveState*>(pData)));
            return sizeof(AEData::GlukkonSaveState);
            break;

        case ::AETypes::eAbe_69:
            AddObjectState(AEData::AbeSaveState::From(*reinterpret_cast<const AEData::AbeSaveState*>(pData)));
            return sizeof(AEData::AbeSaveState);
            break;

        case ::AETypes::eLiftPoint_78:
            AddObjectState(AEData::LiftPointSaveState::From(*reinterpret_cast<const AEData::LiftPointSaveState*>(pData)));
            return sizeof(AEData::LiftPointSaveState);
            break;

        case ::AETypes::eMudokon_110:
        case ::AETypes::eRingOrLiftMud_81:
            AddObjectState(AEData::MudokonSaveState::From(*reinterpret_cast<const AEData::MudokonSaveState*>(pData)));
            return sizeof(AEData::MudokonSaveState);
            break;

        case ::AETypes::eMeat_84:
            AddObjectState(AEData::MeatSaveState::From(*reinterpret_cast<const AEData::MeatSaveState*>(pData)));
            return sizeof(AEData::MeatSaveState);
            break;

        case ::AETypes::eMineCar_89:
            AddObjectState(AEData::MineCarSaveState::From(*reinterpret_cast<const AEData::MineCarSaveState*>(pData)));
            return sizeof(AEData::MineCarSaveState);
            break;

        case ::AETypes::eParamite_96:
            AddObjectState(AEData::ParamiteSaveState::From(*reinterpret_cast<const AEData::ParamiteSaveState*>(pData)));
            return sizeof(AEData::ParamiteSaveState);
            break;

        case ::AETypes::eBirdPortal_99:
            AddObjectState(AEData::BirdPortalSaveState::From(*reinterpret_cast<const AEData::BirdPortalSaveState*>(pData)));
            return sizeof(AEData::BirdPortalSaveState);
            break;

        case ::AETypes::eThrowableArray_102:
            AddObjectState(AEData::ThrowableArraySaveState::From(*reinterpret_cast<const AEData::ThrowableArraySaveState*>(pData)));
            return sizeof(AEData::ThrowableArraySaveState);
            break;

        case ::AETypes::eAbilityRing_104:
            AddObjectState(AEData::AbilityRingSaveState::From(*reinterpret_cast<const AEData::AbilityRingSaveState*>(pData)));
            return sizeof(AEData::AbilityRingSaveState);
            break;

        case ::AETypes::eRock_105:
            AddObjectState(AEData::RockSaveState::From(*reinterpret_cast<const AEData::RockSaveState*>(pData)));
            return sizeof(AEData::RockSaveState);
            break;

        case ::AETypes::eScrab_112:
            AddObjectState(AEData::ScrabSaveState::From(*reinterpret_cast<const AEData::ScrabSaveState*>(pData)));
            return sizeof(AEData::ScrabSaveState);
            break;

        case ::AETypes::eScrabSpawner_113:
            AddObjectState(AEData::ScrabSpawnerSaveState::From(*reinterpret_cast<const AEData::ScrabSpawnerSaveState*>(pData)));
            return sizeof(AEData::ScrabSpawnerSaveState);
            break;

        case ::AETypes::eSlamDoor_122:
        {
            AddObjectState(AEData::SlamDoorSaveState::From(*reinterpret_cast<const AEData::SlamDoorSaveState*>(pData)));
            return sizeof(AEData::SlamDoorSaveState);
        }

        case ::AETypes::eSlig_125:
            AddObjectState(AEData::SligSaveState::From(*reinterpret_cast<const AEData::SligSaveState*>(pData)));
            return sizeof(AEData::SligSaveState);
            break;

        case ::AETypes::eSlog_126:
            AddObjectState(AEData::SlogSaveState::From(*reinterpret_cast<const AEData::SlogSaveState*>(pData)));
            return sizeof(AEData::SlogSaveState);
            break;

        case ::AETypes::eSlurg_129:
            AddObjectState(AEData::SlurgSaveState::From(*reinterpret_cast<const AEData::SlurgSaveState*>(pData)));
            return sizeof(AEData::SlurgSaveState);
            break;

        case ::AETypes::eTimerTrigger_136:
            AddObjectState(AEData::TimerTriggerSaveState::From(*reinterpret_cast<const AEData::TimerTriggerSaveState*>(pData)));
            return sizeof(AEData::TimerTriggerSaveState);
            break;

        case ::AETypes::eTrapDoor_142:
            AddObjectState(AEData::TrapDoorSaveState::From(*reinterpret_cast<const AEData::TrapDoorSaveState*>(pData)));
            return sizeof(AEData::TrapDoorSaveState);
            break;

        case ::AETypes::eUXB_143:
            AddObjectState(AEData::UXBSaveState::From(*reinterpret_cast<const AEData::UXBSaveState*>(pData)));
            return sizeof(AEData::UXBSaveState);
            break;

        case ::AETypes::eWorkWheel_148:
            AddObjectState(AEData::WorkWheelSaveState::From(*reinterpret_cast<const AEData::WorkWheelSaveState*>(pData)));
            return sizeof(AEData::WorkWheelSaveState);
            break;

        default:
            ALIVE_FATAL("No create save state for type %d", static_cast<s32>(type));
    }
}
