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
    s32 field_0_gnFrame;
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
    s16 field_32_visited_bonewerks;
    s16 field_34_visited_barracks;
    s16 field_36_visited_feeco_ender;
    s32 field_38_gas_timer;
    s16 field_3C_bBulletProof;
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

class AESaveConverter final
{
public:

private:
    template<typename T>
    void AddObjectState(const T&)
    {

    }

    s32 ConvertObjectSaveStateData(AETypes type, const u8* pData)
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
            //return Fleech::CreateFromSaveState(pData);
            break;

        case ::AETypes::eFlyingSlig_54:
            //return FlyingSlig::CreateFromSaveState(pData);
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
            //return Greeter::CreateFromSaveState(pData);
            break;

        case ::AETypes::eGrenade_65:
            //return Grenade::CreateFromSaveState(pData);
            break;

        case ::AETypes::eGlukkon_67:
            //return Glukkon::CreateFromSaveState(pData);
            break;

        case ::AETypes::eAbe_69:
            //return Abe::CreateFromSaveState(pData);
            break;

        case ::AETypes::eLiftPoint_78:
            AddObjectState(AEData::LiftPointSaveState::From(*reinterpret_cast<const AEData::LiftPointSaveState*>(pData)));
            return sizeof(AEData::LiftPointSaveState);
            break;

        case ::AETypes::eMudokon_110:
        case ::AETypes::eRingOrLiftMud_81:
            //return Mudokon::CreateFromSaveState(pData);
            break;

        case ::AETypes::eMeat_84:
            //return Meat::CreateFromSaveState(pData);
            break;

        case ::AETypes::eMineCar_89:
            //return MineCar::CreateFromSaveState(pData);
            break;

        case ::AETypes::eParamite_96:
            //return Paramite::CreateFromSaveState(pData);
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
            //return AbilityRing::CreateFromSaveState(pData);
            break;

        case ::AETypes::eRock_105:
            //return Rock::CreateFromSaveState(pData);
            break;

        case ::AETypes::eScrab_112:
            //return Scrab::CreateFromSaveState(pData);
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
            //return Slig::CreateFromSaveState(pData);
            break;

        case ::AETypes::eSlog_126:
            //return Slog::CreateFromSaveState(pData);
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

        case ::AETypes::eWheel_148:
            AddObjectState(AEData::WorkWheelSaveState::From(*reinterpret_cast<const AEData::WorkWheelSaveState*>(pData)));
            return sizeof(AEData::WorkWheelSaveState);
            break;

        default:
            ALIVE_FATAL("No create save state for type %d", static_cast<s32>(type));
    }
}
};