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
    case AO::TlvTypes::ContinuePoint_0:
            j.push_back({ "continue_point", relive::Path_ContinuePoint_Converter::From(static_cast<const AO::Path_ContinuePoint&>(tlv)) });
            break;
        case AO::TlvTypes::PathTransition_1:
            j.push_back({ "path_transition", relive::Path_PathTransition_Converter::From(static_cast<const AO::Path_PathTransition&>(tlv)) });
            break;
        case AO::TlvTypes::ContinueZone_2: // dead tlv
            return;
        case AO::TlvTypes::Hoist_3:
            j.push_back({ "hoist", relive::Path_Hoist_Converter::From(static_cast<const AO::Path_Hoist&>(tlv)) });
            break;
        case AO::TlvTypes::Edge_4:
            j.push_back({ "edge", relive::Path_Edge_Converter::From(static_cast<const AO::Path_Edge&>(tlv)) });
            break;
        case AO::TlvTypes::DeathDrop_5:
            j.push_back({ "death_drop", relive::Path_DeathDrop_Converter::From(static_cast<const AO::Path_DeathDrop&>(tlv)) });
            break;
        case AO::TlvTypes::Door_6:
            j.push_back({ "door", relive::Path_Door_Converter::From(static_cast<const AO::Path_Door&>(tlv)) });
            break;
        case AO::TlvTypes::ShadowZone_7:
            j.push_back({"shadow_zone", relive::Path_ShadowZone_Converter::From(static_cast<const AO::Path_ShadowZone&>(tlv))});
            break;
        case AO::TlvTypes::LiftPoint_8:
            j.push_back({ "lift_point", relive::Path_LiftPoint_Converter::From(static_cast<const AO::Path_LiftPoint&>(tlv)) });
            break;
        case AO::TlvTypes::WellLocal_11:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::Dove_12:
            j.push_back({ "dove", relive::Path_Dove_Converter::From(static_cast<const AO::Path_Dove&>(tlv)) });
            break;
        case AO::TlvTypes::RockSack_13:
            j.push_back({ "rock_sack", relive::Path_RockSack_Converter::From(static_cast<const AO::Path_RockSack&>(tlv)) });
            break;
        case AO::TlvTypes::ZBall_14:
            j.push_back({ "z_ball", relive::Path_ZBall_Converter::From(static_cast<const AO::Path_ZBall&>(tlv)) });
            break;
        case AO::TlvTypes::FallingItem_15:
            j.push_back({ "falling_item", relive::Path_FallingItem_Converter::From(static_cast<const AO::Path_FallingItem&>(tlv)) });
            break;
        case AO::TlvTypes::PullRingRope_18:
            j.push_back({ "pull_ring_rope", relive::Path_PullRingRope_Converter::From(static_cast<const AO::Path_PullRingRope&>(tlv)) });
            break;
        case AO::TlvTypes::BackgroundAnimation_19:
            j.push_back({ "background_animation", relive::Path_BackgroundAnimation_Converter::From(static_cast<const AO::Path_BackgroundAnimation&>(tlv)) });
            break;
        case AO::TlvTypes::Honey_20:
            j.push_back({ "honey", relive::Path_Honey_Converter::From(static_cast<const AO::Path_Honey&>(tlv)) });
            break;
        case AO::TlvTypes::TimedMine_22:
            j.push_back({ "timed_mine", relive::Path_TimedMine_Converter::From(static_cast<const AO::Path_TimedMine&>(tlv)) });
            break;
        case AO::TlvTypes::Slig_24:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::Slog_25:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::Lever_26:
            j.push_back({ "lever", relive::Path_Lever_Converter::From(static_cast<const AO::Path_Lever&>(tlv)) });
            break;
        case AO::TlvTypes::BellHammer_27:
            j.push_back({ "bell_hammer", relive::Path_BellHammer_Converter::From(static_cast<const AO::Path_BellHammer&>(tlv)) });
            break;
        case AO::TlvTypes::StartController_28:
            j.push_back({ "start_controller", relive::Path_StartController_Converter::From(static_cast<const AO::Path_StartController&>(tlv)) });
            break;
        case AO::TlvTypes::SecurityOrb_29:
            j.push_back({"security_orb", relive::Path_SecurityOrb_Converter::From(static_cast<const AO::Path_SecurityOrb&>(tlv))});
            break;
        case AO::TlvTypes::LiftMudokon_32:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::BeeSwarmHole_34:
            j.push_back({ "bee_swarm_hole", relive::Path_BeeSwarmHole_Converter::From(static_cast<const AO::Path_BeeSwarmHole&>(tlv)) });
            break;
        case AO::TlvTypes::Pulley_35:
            j.push_back({ "pulley", relive::Path_Pulley_Converter::From(static_cast<const AO::Path_Pulley&>(tlv)) });
            break;
        case AO::TlvTypes::HoneySack_36:
            j.push_back({ "honey_sack", relive::Path_HoneySack_Converter::From(static_cast<const AO::Path_HoneySack&>(tlv)) });
            break;
        case AO::TlvTypes::AbeStart_37: // dead tlv in AO
            return;
        case AO::TlvTypes::ElumStart_38:
            j.push_back({ "elum_start", relive::Path_ElumStart_Converter::From(static_cast<const AO::Path_ElumStart&>(tlv)) });
            break;
        case AO::TlvTypes::ElumWall_40:
            j.push_back({ "elum_wall", relive::Path_ElumWall_Converter::From(static_cast<const AO::Path_ElumWall&>(tlv)) });
            break;
        case AO::TlvTypes::SlingMudokon_41:
            j.push_back({ "sling_mudokon", relive::Path_SlingMudokon_Converter::From(static_cast<const AO::Path_SlingMudokon&>(tlv)) });
            break;
        case AO::TlvTypes::HoneyDripTarget_42:
            j.push_back({ "honey_drip_target", relive::Path_HoneyDripTarget_Converter::From(static_cast<const AO::Path_HoneyDripTarget&>(tlv)) });
            break;
        case AO::TlvTypes::Bees_43:
            j.push_back({ "bees", relive::Path_Bees_Converter::From(static_cast<const AO::Path_Bees&>(tlv)) });
            break;
        case AO::TlvTypes::WellExpress_45:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::Mine_46:
            j.push_back({ "mine", relive::Path_Mine_Converter::From(static_cast<const AO::Path_Mine&>(tlv)) });
            break;
        case AO::TlvTypes::UXB_47:
            j.push_back({ "uxb", relive::Path_UXB_Converter::From(static_cast<const AO::Path_UXB&>(tlv)) });
            break;
        case AO::TlvTypes::Paramite_48:
            j.push_back({ "paramite", relive::Path_Paramite_Converter::From(static_cast<const AO::Path_Paramite&>(tlv)) });
            break;
        case AO::TlvTypes::Bat_49:
            j.push_back({ "bat", relive::Path_Bat_Converter::From(static_cast<const AO::Path_Bat&>(tlv)) });
            break;
        case AO::TlvTypes::RingMudokon_50:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::MovieStone_51:
            j.push_back({ "movie_stone", relive::Path_MovieStone_Converter::From(static_cast<const AO::Path_MovieStone&>(tlv)) });
            break;
        case AO::TlvTypes::BirdPortal_52:
            j.push_back({ "bird_portal", relive::Path_BirdPortal_Converter::From(static_cast<const AO::Path_BirdPortal&>(tlv)) });
            break;
        case AO::TlvTypes::BirdPortalExit_53:
            j.push_back({ "bird_portal_exit", relive::Path_BirdPortalExit_Converter::From(static_cast<const AO::Path_BirdPortalExit&>(tlv)) });
            break;
        case AO::TlvTypes::BellSongStone_54:
            j.push_back({ "bell_song_stone", relive::Path_BellsongStone_Converter::From(static_cast<const AO::Path_BellsongStone&>(tlv)) });
            break;
        case AO::TlvTypes::TrapDoor_55:
            j.push_back({ "trap_door", relive::Path_TrapDoor_Converter::From(static_cast<const AO::Path_TrapDoor&>(tlv)) });
            break;
        case AO::TlvTypes::RollingBall_56:
            j.push_back({ "rolling_ball", relive::Path_RollingBall_Converter::From(static_cast<const AO::Path_RollingBall&>(tlv)) });
            break;
        case AO::TlvTypes::eSligBoundLeft_57:
            j.push_back({ "slig_bound_left", relive::Path_SligBound_Converter::From(static_cast<const AO::Path_SligBound&>(tlv)) });
            break;
        case AO::TlvTypes::InvisibleZone_58:
            j.push_back({ "invisible_zone", relive::Path_InvisibleZone_Converter::From(static_cast<const AO::Path_InvisibleZone&>(tlv)) });
            break;
        case AO::TlvTypes::RollingBallStopper_59:
            j.push_back({ "rolling_ball_stopper", relive::Path_RollingBallStopper_Converter::From(static_cast<const AO::Path_RollingBallStopper&>(tlv)) });
            break;
        case AO::TlvTypes::FootSwitch_60:
            j.push_back({ "foot_switch", relive::Path_FootSwitch_Converter::From(static_cast<const AO::Path_FootSwitch&>(tlv)) });
            break;
        case AO::TlvTypes::SecurityClaw_61:
            j.push_back({ "security_claw", relive::Path_SecurityClaw_Converter::From(static_cast<const AO::Path_SecurityClaw&>(tlv)) });
            break;
        case AO::TlvTypes::MotionDetector_62:
            j.push_back({ "motion_detector", relive::Path_MotionDetector_Converter::From(static_cast<const AO::Path_MotionDetector&>(tlv)) });
            break;
        case AO::TlvTypes::SligSpawner_66:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::ElectricWall_67:
            j.push_back({ "electric_wall", relive::Path_ElectricWall_Converter::From(static_cast<const AO::Path_ElectricWall&>(tlv)) });
            break;
        case AO::TlvTypes::LiftMover_68:
            j.push_back({ "lift_mover", relive::Path_LiftMover_Converter::From(static_cast<const AO::Path_LiftMover&>(tlv)) });
            break;
        case AO::TlvTypes::ChimeLock_69:
            j.push_back({ "chime_lock", relive::Path_ChimeLock_Converter::From(static_cast<const AO::Path_ChimeLock&>(tlv)) });
            break;
        case AO::TlvTypes::MeatSack_71:
            j.push_back({ "meat_sack", relive::Path_MeatSack_Converter::From(static_cast<const AO::Path_MeatSack&>(tlv)) });
            break;
        case AO::TlvTypes::Scrab_72:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::FlintLockFire_73:
            j.push_back({ "flint_lock_fire", relive::Path_FlintLockFire_Converter::From(static_cast<const AO::Path_FlintLockFire&>(tlv)) });
            break;
        case AO::TlvTypes::ScrabLeftBound_74:
            j.push_back({ "scrab_left_bound", relive::Path_ScrabLeftBound_Converter::From(static_cast<const AO::Path_ScrabLeftBound&>(tlv)) });
            break;
        case AO::TlvTypes::ScrabRightBound_75:
            j.push_back({ "scrab_right_bound", relive::Path_ScrabRightBound_Converter::From(static_cast<const AO::Path_ScrabRightBound&>(tlv)) });
            break;
        case AO::TlvTypes::eSligBoundRight_76:
            j.push_back({ "slig_bound_right", relive::Path_SligBound_Converter::From(static_cast<const AO::Path_SligBound&>(tlv)) });
            break;
        case AO::TlvTypes::eSligPersist_77:
            j.push_back({ "slig_persist", relive::Path_SligBound_Converter::From(static_cast<const AO::Path_SligBound&>(tlv)) });
            break;
        case AO::TlvTypes::EnemyStopper_79:
            j.push_back({ "enemy_stopper", relive::Path_EnemyStopper_Converter::From(static_cast<const AO::Path_EnemyStopper&>(tlv)) });
            break;
        case AO::TlvTypes::InvisibleSwitch_81:
            j.push_back({ "invisible_switch", relive::Path_InvisibleSwitch_Converter::From(static_cast<const AO::Path_InvisibleSwitch&>(tlv)) });
            break;
        case AO::TlvTypes::Mudokon_82:
            j.push_back({ "mudokon", relive::Path_Mudokon_Converter::From(static_cast<const AO::Path_Mudokon&>(tlv)) });
            break;
        case AO::TlvTypes::ZSligCover_83:
            j.push_back({ "zslig_cover", relive::Path_ZSligCover_Converter::From(static_cast<const AO::Path_ZSligCover&>(tlv)) });
            break;
        case AO::TlvTypes::DoorFlame_84:
            j.push_back({ "door_flame", relive::Path_DoorFlame_Converter::From(static_cast<const AO::Path_DoorFlame&>(tlv)) });
            break;
        case AO::TlvTypes::MovingBomb_86:
            j.push_back({ "moving_bomb", relive::Path_MovingBomb_Converter::From(static_cast<const AO::Path_MovingBomb&>(tlv)) });
            break;
        case AO::TlvTypes::MovingBombStopper_87:
            j.push_back({ "moving_bomb_stopper", relive::Path_MovingBombStopper_Converter::From(static_cast<const AO::Path_MovingBombStopper&>(tlv)) });
            break;
        case AO::TlvTypes::MeatSaw_88:
            j.push_back({ "meat_saw", relive::Path_MeatSaw_Converter::From(static_cast<const AO::Path_MeatSaw&>(tlv)) });
            break;
        case AO::TlvTypes::MudokonPathTrans_89:
            j.push_back({ "mudokon_path_trans", relive::Path_MudokonPathTrans_Converter::From(static_cast<const AO::Path_MudokonPathTrans&>(tlv)) });
            break;
        case AO::TlvTypes::MainMenuController_90: // no props
            j.push_back({ "main_menu_controller", relive::Path_MainMenuController_Converter::From(static_cast<const AO::Path_MainMenuController&>(tlv)) });
            break;
        case AO::TlvTypes::HintFly_92:
            j.push_back({ "hint_fly", relive::Path_HintFly_Converter::From(static_cast<const AO::Path_HintFly&>(tlv)) });
            break;
        case AO::TlvTypes::ScrabNoFall_93:
            j.push_back({ "scrab_no_fall", relive::Path_ScrabNoFall_Converter::From(static_cast<const AO::Path_ScrabNoFall&>(tlv)) });
            break;
        case AO::TlvTypes::TimerTrigger_94:
            j.push_back({ "timer_trigger", relive::Path_TimerTrigger_Converter::From(static_cast<const AO::Path_TimerTrigger&>(tlv)) });
            break;
        case AO::TlvTypes::SecurityDoor_95:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::DemoPlaybackStone_96:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::BoomMachine_97:
            j.push_back({ "boom_machine", relive::Path_BoomMachine_Converter::From(static_cast<const AO::Path_BoomMachine&>(tlv)) });
            break;
        case AO::TlvTypes::LCDScreen_98:
            j.push_back({ "lcd_screen", relive::Path_LCDScreen_Converter::From(static_cast<const AO::Path_LCDScreen&>(tlv)) });
            break;
        case AO::TlvTypes::ElumPathTrans_99:
            j.push_back({ "elum_path_trans", relive::Path_ElumPathTrans_Converter::From(static_cast<const AO::Path_ElumPathTrans&>(tlv)) });
            break;
        case AO::TlvTypes::HandStone_100:
            j.push_back({ "hand_stone", relive::Path_HandStone_Converter::From(static_cast<const AO::Path_HandStone&>(tlv)) });
            break;
        case AO::TlvTypes::CreditsController_101:
            j.push_back({ "credits_controller", relive::Path_CreditsController_Converter::From(static_cast<const AO::Path_CreditsController&>(tlv)) });
            break;
        case AO::TlvTypes::Preloader_102: // dead tlv
            return;
        case AO::TlvTypes::LCDStatusBoard_103:
            j.push_back({ "lcd_status_board", relive::Path_LCDStatusBoard_Converter::From(static_cast<const AO::Path_LCDStatusBoard&>(tlv)) });
            break;
        case AO::TlvTypes::MusicTrigger_105:
            j.push_back({ "music_trigger", relive::Path_MusicTrigger_Converter::From(static_cast<const AO::Path_MusicTrigger&>(tlv)) });
            break;
        case AO::TlvTypes::LightEffect_106:
            j.push_back({ "light_effect", relive::Path_LightEffect_Converter::From(static_cast<const AO::Path_LightEffect&>(tlv)) });
            break;
        case AO::TlvTypes::SlogSpawner_107:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::GasCountDown_108:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::RingCancel_109:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::GasEmitter_110:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::ZzzSpawner_111:
            j.push_back({ "zzz_spawner", relive::Path_ZzzSpawner_Converter::From(static_cast<const AO::Path_ZzzSpawner&>(tlv)) });
            break;
        case AO::TlvTypes::BackgroundGlukkon_112:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " not implemented");
            return;
        case AO::TlvTypes::KillUnsavedMuds_113:
            j.push_back({ "kill_unsaved_muds", relive::Path_KillUnsavedMuds_Converter::From(static_cast<const AO::Path_KillUnsavedMuds&>(tlv)) });
            break;
        case AO::TlvTypes::SoftLanding_114:
            j.push_back({ "soft_landing", relive::Path_SoftLanding_Converter::From(static_cast<const AO::Path_SoftLanding&>(tlv)) });
            break;
        case AO::TlvTypes::ResetPath_115:
            j.push_back({ "reset_path", relive::Path_ResetPath_Converter::From(static_cast<const AO::Path_ResetPath&>(tlv)) });
            break;

        default:
            ALIVE_FATAL("TLV conversion for this type not implemented");
    }
    //LOG_INFO(j.dump(4));
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
    LOG_INFO("Converted all TLV's for level " << ToString(MapWrapper::ToAO(reliveLvl)));
}

void TestTlvConversion()
{
    AO::Path_ShadowZone shadowTlv = {};
    shadowTlv.mTlvType32.mType = AO::TlvTypes::ContinuePoint_0;
    ConvertTLV(shadowTlv);

    // TODO: Check from AE tlv


}

void DataConversion::ConvertData()
{
    // TODO: Check existing data version, if any
    //TestTlvConversion(); // test hax to test a specific tlv

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
