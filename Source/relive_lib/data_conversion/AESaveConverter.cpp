#include "AESaveConverter.hpp"
#include "../../AliveLibCommon/FatalError.hpp"

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
            //return SligSpawner::CreateFromSaveState(pData);
            break;

        case ::AETypes::eLiftMover_9:
            //return LiftMover::CreateFromSaveState(pData);
            break;

        case ::AETypes::eBone_11:
            //return Bone::CreateFromSaveState(pData);
            break;

        case ::AETypes::eMinesAlarm_25:
            //return MinesAlarm::CreateFromSaveState(pData);
            break;

        case ::AETypes::eCrawlingSlig_26:
            //return CrawlingSlig::CreateFromSaveState(pData);
            break;

        case ::AETypes::eDrill_30:
            //return Drill::CreateFromSaveState(pData);
            break;

        case ::AETypes::eEvilFart_45:
            //return EvilFart::CreateFromSaveState(pData);
            break;

        case ::AETypes::eFleech_50:
            //return Fleech::CreateFromSaveState(pData);
            break;

        case ::AETypes::eFlyingSlig_54:
            //return FlyingSlig::CreateFromSaveState(pData);
            break;

        case ::AETypes::eFlyingSligSpawner_55:
            //return FlyingSligSpawner::CreateFromSaveState(pData);
            break;

        case ::AETypes::eGameEnderController_57:
            //return GameEnderController::CreateFromSaveState(pData);
            break;

        case ::AETypes::eSlapLock_OrbWhirlWind_60:
            //return SlapLockWhirlWind::CreateFromSaveState(pData);
            break;

        case ::AETypes::eLockedSoul_61:
            //return SlapLock::CreateFromSaveState(pData);
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
            //return LiftPoint::CreateFromSaveState(pData);
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
            //return BirdPortal::CreateFromSaveState(pData);
            break;

        case ::AETypes::eThrowableArray_102:
            //return ThrowableArray::CreateFromSaveState(pData);
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
            //return ScrabSpawner::CreateFromSaveState(pData);
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
            //return Slurg::CreateFromSaveState(pData);
            break;

        case ::AETypes::eTimerTrigger_136:
            //return TimerTrigger::CreateFromSaveState(pData);
            break;

        case ::AETypes::eTrapDoor_142:
            //return TrapDoor::CreateFromSaveState(pData);
            break;

        case ::AETypes::eUXB_143:
            //return UXB::CreateFromSaveState(pData);
            break;

        case ::AETypes::eWheel_148:
            //return WorkWheel::CreateFromSaveState(pData);
            break;

        default:
            ALIVE_FATAL("No create save state for type %d", static_cast<s32>(type));
    }
}
};