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


#include "nlohmann/json.hpp"
#include "../../Tools/relive_api/LvlReaderWriter.hpp"
#include "../../Tools/relive_api/file_api.hpp"
#include "../../Tools/relive_api/CamConverter.hpp"
#include "../../Tools/relive_api/PathCamerasEnumerator.hpp"
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


static void to_json(nlohmann::json& j, const PathLineAO& p)
{
    j = nlohmann::json{
        {"x", p.mRect.x},
        {"y", p.mRect.y},
        {"w", p.mRect.w},
        {"h", p.mRect.h},
        {"next", p.field_10_next},
        {"previous", p.field_C_previous},
        {"type", p.mLineType},
    };
}


static void ConvertPathCollisions(nlohmann::json& j, const CollisionInfo& info, const std::vector<u8>& pathResource)
{
    const u8* pData = pathResource.data();
    const u8* pStart = pData + info.field_C_collision_offset;

    auto pCollisions = reinterpret_cast<const PathLineAO*>(pStart);
    for (u32 i = 0; i < info.field_10_num_collision_items; i++)
    {

        // TODO: Use AE format lines
        j.push_back(pCollisions[i]);
    }
}

template <typename converter_type, typename tlv_src_type>
static void convert_tlv(nlohmann::json& j, const AO::Path_TLV& tlv)
{
    j.push_back(converter_type::From(static_cast<const tlv_src_type&>(tlv)));
}

template <typename converter_type, typename tlv_src_type>
static void convert_tlv(nlohmann::json& j, const ::Path_TLV& tlv)
{
    j.push_back(converter_type::From(static_cast<const tlv_src_type&>(tlv)));
}


static void ConvertTLV(nlohmann::json& j, const ::Path_TLV& tlv)
{
    if (0) { ConvertTLV(j, tlv); } // stop annoying me compiler

    switch (tlv.mTlvType32.mType)
    {
        case ::TlvTypes::ContinuePoint_0:
            convert_tlv<relive::Path_ContinuePoint_Converter, ::Path_ContinuePoint>(j, tlv);
            break;
        case ::TlvTypes::PathTransition_1:
            convert_tlv<relive::Path_PathTransition_Converter, ::Path_PathTransition>(j, tlv);
            break;
        case ::TlvTypes::Hoist_2:
            convert_tlv<relive::Path_Hoist_Converter, ::Path_Hoist>(j, tlv);
            break;
        case ::TlvTypes::Edge_3:
            convert_tlv<relive::Path_Edge_Converter, ::Path_Edge>(j, tlv);
            break;
        case ::TlvTypes::DeathDrop_4:
            convert_tlv<relive::Path_DeathDrop_Converter, ::Path_DeathDrop>(j, tlv);
            break;
        case ::TlvTypes::Door_5:
            convert_tlv<relive::Path_Door_Converter, ::Path_Door>(j, tlv);
            break;
        case ::TlvTypes::ShadowZone_6:
            convert_tlv<relive::Path_ShadowZone_Converter, ::Path_ShadowZone>(j, tlv);
            break;
        case ::TlvTypes::LiftPoint_7:
            convert_tlv<relive::Path_LiftPoint_Converter, ::Path_LiftPoint>(j, tlv);
            break;
        case ::TlvTypes::LocalWell_8:
            convert_tlv<relive::Path_WellLocal_Converter, ::Path_WellLocal>(j, tlv);
            break;
        case ::TlvTypes::Dove_9:
            convert_tlv<relive::Path_Dove_Converter, ::Path_Dove>(j, tlv);
            break;
        case ::TlvTypes::RockSack_10:
            convert_tlv<relive::Path_RockSack_Converter, ::Path_RockSack>(j, tlv);
            break;
        case ::TlvTypes::FallingItem_11:
            convert_tlv<relive::Path_FallingItem_Converter, ::Path_FallingItem>(j, tlv);
            break;
        case ::TlvTypes::PullRingRope_12:
            convert_tlv<relive::Path_PullRingRope_Converter, ::Path_PullRingRope>(j, tlv);
            break;
        case ::TlvTypes::BackgroundAnimation_13:
            convert_tlv<relive::Path_BackgroundAnimation_Converter, ::Path_BackgroundAnimation>(j, tlv);
            break;
        case ::TlvTypes::TimedMine_14:
            convert_tlv<relive::Path_TimedMine_Converter, ::Path_TimedMine>(j, tlv);
            break;
        case ::TlvTypes::Slig_15:
            convert_tlv<relive::Path_Slig_Converter, ::Path_Slig>(j, tlv);
            break;
        case ::TlvTypes::Slog_16:
            convert_tlv<relive::Path_Slog_Converter, ::Path_Slog>(j, tlv);
            break;
        case ::TlvTypes::Lever_17:
            convert_tlv<relive::Path_Lever_Converter, ::Path_Lever>(j, tlv);
            break;
        case ::TlvTypes::Null_18:
            return;
        case ::TlvTypes::SecurityOrb_19:
            convert_tlv<relive::Path_SecurityOrb_Converter, ::Path_SecurityOrb>(j, tlv);
            break;
        case ::TlvTypes::Null_20:
            return;
        case ::TlvTypes::Pulley_21:
            convert_tlv<relive::Path_Pulley_Converter, ::Path_Pulley>(j, tlv);
            break;
        case ::TlvTypes::AbeStart_22:
            convert_tlv<relive::Path_AbeStart_Converter, ::Path_AbeStart>(j, tlv);
            break;
        case ::TlvTypes::WellExpress_23:
            convert_tlv<relive::Path_WellExpress_Converter, ::Path_WellExpress>(j, tlv);
            break;
        case ::TlvTypes::Mine_24:
            convert_tlv<relive::Path_Mine_Converter, ::Path_Mine>(j, tlv);
            break;
        case ::TlvTypes::UXB_25:
            convert_tlv<relive::Path_UXB_Converter, ::Path_UXB>(j, tlv);
            break;
        case ::TlvTypes::Paramite_26:
            convert_tlv<relive::Path_Paramite_Converter, ::Path_Paramite>(j, tlv);
            break;
        case ::TlvTypes::MovieHandStone_27:
            convert_tlv<relive::Path_MovieStone_Converter, ::Path_MovieStone>(j, tlv);
            break;
        case ::TlvTypes::BirdPortal_28:
            convert_tlv<relive::Path_BirdPortal_Converter, ::Path_BirdPortal>(j, tlv);
            break;
        case ::TlvTypes::BirdPortalExit_29:
            convert_tlv<relive::Path_BirdPortalExit_Converter, ::Path_BirdPortalExit>(j, tlv);
            break;
        case ::TlvTypes::TrapDoor_30:
            convert_tlv<relive::Path_TrapDoor_Converter, ::Path_TrapDoor>(j, tlv);
            break;
        case ::TlvTypes::RollingBall_31: // not implemented in relive AE
            return;
        case ::TlvTypes::SligBoundLeft_32:
            convert_tlv<relive::Path_SligBoundLeft_Converter, ::Path_SligBoundLeft>(j, tlv);
            break;
        case ::TlvTypes::InvisibleZone_33:
            convert_tlv<relive::Path_InvisibleZone_Converter, ::Path_InvisibleZone>(j, tlv);
            break;
        case ::TlvTypes::FootSwitch_34:
            convert_tlv<relive::Path_FootSwitch_Converter, ::Path_FootSwitch>(j, tlv);
            break;
        case ::TlvTypes::SecurityClaw_35: // TODO: this tlv is the same as the orb tlv
            convert_tlv<relive::Path_SecurityClaw_Converter, ::Path_SecurityClaw>(j, tlv);
            break;
        case ::TlvTypes::MotionDetector_36:
            convert_tlv<relive::Path_MotionDetector_Converter, ::Path_MotionDetector>(j, tlv);
            break;
        case ::TlvTypes::SligSpawner_37:
            convert_tlv<relive::Path_SligSpawner_Converter, ::Path_SligSpawner>(j, tlv);
            break;
        case ::TlvTypes::ElectricWall_38:
            convert_tlv<relive::Path_ElectricWall_Converter, ::Path_ElectricWall>(j, tlv);
            break;
        case ::TlvTypes::LiftMover_39:
            convert_tlv<relive::Path_LiftMover_Converter, ::Path_LiftMover>(j, tlv);
            break;
        case ::TlvTypes::MeatSack_40:
            convert_tlv<relive::Path_MeatSack_Converter, ::Path_MeatSack>(j, tlv);
            break;
        case ::TlvTypes::Scrab_41:
            convert_tlv<relive::Path_Scrab_Converter, ::Path_Scrab>(j, tlv);
            break;
        case ::TlvTypes::Null_42:
            return;
        case ::TlvTypes::ScrabBoundLeft_43:
            convert_tlv<relive::Path_ScrabBoundLeft_Converter, ::Path_ScrabBoundLeft>(j, tlv);
            break;
        case ::TlvTypes::ScrabBoundRight_44:
            convert_tlv<relive::Path_ScrabBoundRight_Converter, ::Path_ScrabBoundRight>(j, tlv);
            break;
        case ::TlvTypes::SligBoundRight_45:
            convert_tlv<relive::Path_SligBoundRight_Converter, ::Path_SligBoundRight>(j, tlv);
            break;
        case ::TlvTypes::SligPersist_46:
            convert_tlv<relive::Path_SligPersist_Converter, ::Path_SligPersist>(j, tlv);
            break;
        case ::TlvTypes::EnemyStopper_47:
            convert_tlv<relive::Path_EnemyStopper_Converter, ::Path_EnemyStopper>(j, tlv);
            break;
        case ::TlvTypes::InvisibleSwitch_48:
            convert_tlv<relive::Path_InvisibleSwitch_Converter, ::Path_InvisibleSwitch>(j, tlv);
            break;
        case ::TlvTypes::Mudokon_49:
            convert_tlv<relive::Path_Mudokon_Converter, ::Path_Mudokon>(j, tlv);
            break;
        case ::TlvTypes::ZSligCover_50:
            convert_tlv<relive::Path_ZSligCover_Converter, ::Path_ZSligCover>(j, tlv);
            break;
        case ::TlvTypes::DoorFlame_51:
            convert_tlv<relive::Path_DoorFlame_Converter, ::Path_DoorFlame>(j, tlv);
            break;
        case ::TlvTypes::MovingBomb_52:
            convert_tlv<relive::Path_MovingBomb_Converter, ::Path_MovingBomb>(j, tlv);
            break;
        case ::TlvTypes::MovingBombStopper_53:
            convert_tlv<relive::Path_MovingBombStopper_Converter, ::Path_MovingBombStopper>(j, tlv);
            break;
        case ::TlvTypes::MainMenuController_54:
            convert_tlv<relive::Path_MainMenuController_Converter, ::Path_MainMenuController>(j, tlv);
            break;
        case ::TlvTypes::Unknown_55:
            LOG_INFO("unknown tlv 55");
            return;
        case ::TlvTypes::Null_56:
            return;
        case ::TlvTypes::TimerTrigger_57:
            convert_tlv<relive::Path_TimerTrigger_Converter, ::Path_TimerTrigger>(j, tlv);
            break;
        case ::TlvTypes::SecurityDoor_58:
            convert_tlv<relive::Path_SecurityDoor_Converter, ::Path_SecurityDoor>(j, tlv);
            break;
        case ::TlvTypes::BoomMachine_59:
            convert_tlv<relive::Path_BoomMachine_Converter, ::Path_BoomMachine>(j, tlv);
            break;
        case ::TlvTypes::LCDScreen_60:
            convert_tlv<relive::Path_LCDScreen_Converter, ::Path_LCDScreen>(j, tlv);
            break;
        case ::TlvTypes::HandStone_61:
            convert_tlv<relive::Path_HandStone_Converter, ::Path_HandStone>(j, tlv);
            break;
        case ::TlvTypes::CreditsController_62:
            convert_tlv<relive::Path_CreditsController_Converter, ::Path_CreditsController>(j, tlv);
            break;
        case ::TlvTypes::Null_63:
            return;
        case ::TlvTypes::LCDStatusBoard_64:
            convert_tlv<relive::Path_LCDStatusBoard_Converter, ::Path_LCDStatusBoard>(j, tlv);
            break;
        case ::TlvTypes::WheelSyncer_65:
            convert_tlv<relive::Path_WheelSyncer_Converter, ::Path_WheelSyncer>(j, tlv);
            return;
        case ::TlvTypes::MusicTrigger_66:
            convert_tlv<relive::Path_MusicTrigger_Converter, ::Path_MusicTrigger>(j, tlv);
            break;
        case ::TlvTypes::Light_67: // not implemented in AE
            return;
        case ::TlvTypes::SlogSpawner_68:
            convert_tlv<relive::Path_SlogSpawner_Converter, ::Path_SlogSpawner>(j, tlv);
            break;
        case ::TlvTypes::GasCountdown_69:
            convert_tlv<relive::Path_GasCountDown_Converter, ::Path_GasCountDown>(j, tlv);
            break;
        case ::TlvTypes::Unknown_70:
            ALIVE_FATAL("unknown tlv type 70 unused?");
            return;
        case ::TlvTypes::GasEmitter_71:
            convert_tlv<relive::Path_GasEmitter_Converter, ::Path_GasEmitter>(j, tlv);
            break;
        case ::TlvTypes::ZzzSpawner_72:
            convert_tlv<relive::Path_ZzzSpawner_Converter, ::Path_ZzzSpawner>(j, tlv);
            break;
        case ::TlvTypes::Glukkon_73:
            convert_tlv<relive::Path_Glukkon_Converter, ::Path_Glukkon>(j, tlv);
            break;
        case ::TlvTypes::KillUnsavedMudokons_74: // AO only
            return;
        case ::TlvTypes::SoftLanding_75:
            convert_tlv<relive::Path_SoftLanding_Converter, ::Path_SoftLanding>(j, tlv);
            break;
        case ::TlvTypes::ResetPath_76:
            convert_tlv<relive::Path_ResetPath_Converter, ::Path_ResetPath>(j, tlv);
            break;
        case ::TlvTypes::Water_77:
            convert_tlv<relive::Path_Water_Converter, ::Path_Water>(j, tlv);
            break;
        case ::TlvTypes::Null_78:
            return;
        case ::TlvTypes::WorkWheel_79:
            convert_tlv<relive::Path_WorkWheel_Converter, ::Path_WorkWheel>(j, tlv);
            break;
        case ::TlvTypes::Null_80:
            return;
        case ::TlvTypes::LaughingGas_81:
            convert_tlv<relive::Path_LaughingGas_Converter, ::Path_LaughingGas>(j, tlv);
            break;
        case ::TlvTypes::FlyingSlig_82:
            convert_tlv<relive::Path_FlyingSlig_Converter, ::Path_FlyingSlig>(j, tlv);
            break;
        case ::TlvTypes::Fleech_83:
            convert_tlv<relive::Path_Fleech_Converter, ::Path_Fleech>(j, tlv);
            break;
        case ::TlvTypes::Slurg_84:
            convert_tlv<relive::Path_Slurg_Converter, ::Path_Slurg>(j, tlv);
            break;
        case ::TlvTypes::SlamDoor_85:
            convert_tlv<relive::Path_SlamDoor_Converter, ::Path_SlamDoor>(j, tlv);
            break;
        case ::TlvTypes::LevelLoader_86:
            convert_tlv<relive::Path_LevelLoader_Converter, ::Path_LevelLoader>(j, tlv);
            break;
        case ::TlvTypes::DemoSpawnPoint_87:
            convert_tlv<relive::Path_DemoSpawnPoint_Converter, ::Path_DemoSpawnPoint>(j, tlv);
            break;
        case ::TlvTypes::Teleporter_88:
            convert_tlv<relive::Path_Teleporter_Converter, ::Path_Teleporter>(j, tlv);
            break;
        case ::TlvTypes::SlurgSpawner_89:
            convert_tlv<relive::Path_SlurgSpawner_Converter, ::Path_SlurgSpawner>(j, tlv);
            break;
        case ::TlvTypes::Drill_90:
            convert_tlv<relive::Path_Drill_Converter, ::Path_Drill>(j, tlv);
            break;
        case ::TlvTypes::ColourfulMeter_91:
            convert_tlv<relive::Path_ColourfulMeter_Converter, ::Path_ColourfulMeter>(j, tlv);
            break;
        case ::TlvTypes::FlyingSligSpawner_92:
            convert_tlv<relive::Path_FlyingSligSpawner_Converter, ::Path_FlyingSligSpawner>(j, tlv);
            break;
        case ::TlvTypes::MineCar_93:
            convert_tlv<relive::Path_MineCar_Converter, ::Path_MineCar>(j, tlv);
            break;
        case ::TlvTypes::BoneBag_94:
            convert_tlv<relive::Path_BoneBag_Converter, ::Path_BoneBag>(j, tlv);
            break;
        case ::TlvTypes::ExplosionSet_95:
            convert_tlv<relive::Path_ExplosionSet_Converter, ::Path_ExplosionSet>(j, tlv);
            break;
        case ::TlvTypes::MultiSwitchController_96:
            convert_tlv<relive::Path_MultiSwitchController_Converter, ::Path_MultiSwitchController>(j, tlv);
            break;
        case ::TlvTypes::StatusLight_97:
            convert_tlv<relive::Path_StatusLight_Converter, ::Path_StatusLight>(j, tlv);
            break;
        case ::TlvTypes::SlapLock_98:
            convert_tlv<relive::Path_SlapLock_Converter, ::Path_SlapLock>(j, tlv);
            break;
        case ::TlvTypes::ParamiteWebLine_99:
            convert_tlv<relive::Path_ParamiteWebLine_Converter, ::Path_ParamiteWebLine>(j, tlv);
            break;
        case ::TlvTypes::Alarm_100:
            convert_tlv<relive::Path_Alarm_Converter, ::Path_Alarm>(j, tlv);
            break;
        case ::TlvTypes::BrewMachine_101:
            convert_tlv<relive::Path_BrewMachine_Converter, ::Path_BrewMachine>(j, tlv);
            break;
        case ::TlvTypes::ScrabSpawner_102:
            convert_tlv<relive::Path_ScrabSpawner_Converter, ::Path_ScrabSpawner>(j, tlv);
            break;
        case ::TlvTypes::CrawlingSlig_103:
            convert_tlv<relive::Path_CrawlingSlig_Converter, ::Path_CrawlingSlig>(j, tlv);
            break;
        case ::TlvTypes::SligGetPants_104:
            convert_tlv<relive::Path_SligGetPants_Converter, ::Path_SligGetPants>(j, tlv);
            break;
        case ::TlvTypes::SligGetWings_105:
            convert_tlv<relive::Path_SligGetWings_Converter, ::Path_SligGetWings>(j, tlv);
            break;
        case ::TlvTypes::Greeter_106:
            convert_tlv<relive::Path_Greeter_Converter, ::Path_Greeter>(j, tlv);
            break;
        case ::TlvTypes::CrawlingSligButton_107:
            convert_tlv<relive::Path_CrawlingSligButton_Converter, ::Path_CrawlingSligButton>(j, tlv);
            break;
        case ::TlvTypes::GlukkonSwitch_108:
            convert_tlv<relive::Path_GlukkonSwitch_Converter, ::Path_GlukkonSwitch>(j, tlv);
            break;
        case ::TlvTypes::DoorBlocker_109:
            convert_tlv<relive::Path_DoorBlocker_Converter, ::Path_DoorBlocker>(j, tlv);
            break;
        case ::TlvTypes::TorturedMudokon_110:
            convert_tlv<relive::Path_TorturedMudokon_Converter, ::Path_TorturedMudokon>(j, tlv);
            break;
        case ::TlvTypes::TrainDoor_111:
            convert_tlv<relive::Path_TrainDoor_Converter, ::Path_TrainDoor>(j, tlv);
            break;
        default:
            ALIVE_FATAL("TLV conversion for this type not implemented");
    }
}


static void ConvertTLV(nlohmann::json& j, const AO::Path_TLV& tlv)
{
    switch (tlv.mTlvType32.mType)
    {
        case AO::TlvTypes::ContinuePoint_0:
            convert_tlv<relive::Path_ContinuePoint_Converter, AO::Path_ContinuePoint>(j, tlv);
            break;
        case AO::TlvTypes::PathTransition_1:
            convert_tlv<relive::Path_PathTransition_Converter, AO::Path_PathTransition>(j, tlv);
            break;
        case AO::TlvTypes::ContinueZone_2: // dead tlv
            return;
        case AO::TlvTypes::Hoist_3:
            convert_tlv<relive::Path_Hoist_Converter, AO::Path_Hoist>(j, tlv);
            break;
        case AO::TlvTypes::Edge_4:
            convert_tlv<relive::Path_Edge_Converter, AO::Path_Edge>(j, tlv);
            break;
        case AO::TlvTypes::DeathDrop_5:
            convert_tlv<relive::Path_DeathDrop_Converter, AO::Path_DeathDrop>(j, tlv);
            break;
        case AO::TlvTypes::Door_6:
            convert_tlv<relive::Path_Door_Converter, AO::Path_Door>(j, tlv);
            break;
        case AO::TlvTypes::ShadowZone_7:
            convert_tlv<relive::Path_ShadowZone_Converter, AO::Path_ShadowZone>(j, tlv);
            break;
        case AO::TlvTypes::LiftPoint_8:
            convert_tlv<relive::Path_LiftPoint_Converter, AO::Path_LiftPoint>(j, tlv);
            break;
        case AO::TlvTypes::WellLocal_11:
            convert_tlv<relive::Path_WellLocal_Converter, AO::Path_WellLocal>(j, tlv);
            break;
        case AO::TlvTypes::Dove_12:
            convert_tlv<relive::Path_Dove_Converter, AO::Path_Dove>(j, tlv);
            break;
        case AO::TlvTypes::RockSack_13:
            convert_tlv<relive::Path_RockSack_Converter, AO::Path_RockSack>(j, tlv);
            break;
        case AO::TlvTypes::ZBall_14:
            convert_tlv<relive::Path_ZBall_Converter, AO::Path_ZBall>(j, tlv);
            break;
        case AO::TlvTypes::FallingItem_15:
            convert_tlv<relive::Path_FallingItem_Converter, AO::Path_FallingItem>(j, tlv);
            break;
        case AO::TlvTypes::PullRingRope_18:
            convert_tlv<relive::Path_PullRingRope_Converter, AO::Path_PullRingRope>(j, tlv);
            break;
        case AO::TlvTypes::BackgroundAnimation_19:
            convert_tlv<relive::Path_BackgroundAnimation_Converter, AO::Path_BackgroundAnimation>(j, tlv);
            break;
        case AO::TlvTypes::Honey_20:
            convert_tlv<relive::Path_Honey_Converter, AO::Path_Honey>(j, tlv);
            break;
        case AO::TlvTypes::TimedMine_22:
            convert_tlv<relive::Path_TimedMine_Converter, AO::Path_TimedMine>(j, tlv);
            break;
        case AO::TlvTypes::Slig_24:
            convert_tlv<relive::Path_Slig_Converter, AO::Path_Slig>(j, tlv);
            break;
        case AO::TlvTypes::Slog_25:
            convert_tlv<relive::Path_Slog_Converter, AO::Path_Slog>(j, tlv);
            break;
        case AO::TlvTypes::Lever_26:
            convert_tlv<relive::Path_Lever_Converter, AO::Path_Lever>(j, tlv);
            break;
        case AO::TlvTypes::BellHammer_27:
            convert_tlv<relive::Path_BellHammer_Converter, AO::Path_BellHammer>(j, tlv);
            break;
        case AO::TlvTypes::StartController_28:
            convert_tlv<relive::Path_StartController_Converter, AO::Path_StartController>(j, tlv);
            break;
        case AO::TlvTypes::SecurityOrb_29:
            convert_tlv<relive::Path_SecurityOrb_Converter, AO::Path_SecurityOrb>(j, tlv);
            break;
        case AO::TlvTypes::LiftMudokon_32:
            convert_tlv<relive::Path_LiftMudokon_Converter, AO::Path_LiftMudokon>(j, tlv);
            break;
        case AO::TlvTypes::BeeSwarmHole_34:
            convert_tlv<relive::Path_BeeSwarmHole_Converter, AO::Path_BeeSwarmHole>(j, tlv);
            break;
        case AO::TlvTypes::Pulley_35:
            convert_tlv<relive::Path_Pulley_Converter, AO::Path_Pulley>(j, tlv);
            break;
        case AO::TlvTypes::HoneySack_36:
            convert_tlv<relive::Path_HoneySack_Converter, AO::Path_HoneySack>(j, tlv);
            break;
        case AO::TlvTypes::AbeStart_37: // dead tlv in AO
            return;
        case AO::TlvTypes::ElumStart_38:
            convert_tlv<relive::Path_ElumStart_Converter, AO::Path_ElumStart>(j, tlv);
            break;
        case AO::TlvTypes::ElumWall_40:
            convert_tlv<relive::Path_ElumWall_Converter, AO::Path_ElumWall>(j, tlv);
            break;
        case AO::TlvTypes::SlingMudokon_41:
            convert_tlv<relive::Path_SlingMudokon_Converter, AO::Path_SlingMudokon>(j, tlv);
            break;
        case AO::TlvTypes::HoneyDripTarget_42:
            convert_tlv<relive::Path_HoneyDripTarget_Converter, AO::Path_HoneyDripTarget>(j, tlv);
            break;
        case AO::TlvTypes::Bees_43:
            convert_tlv<relive::Path_Bees_Converter, AO::Path_Bees>(j, tlv);
            break;
        case AO::TlvTypes::WellExpress_45:
            convert_tlv<relive::Path_WellExpress_Converter, AO::Path_WellExpress>(j, tlv);
            break;
        case AO::TlvTypes::Mine_46:
            convert_tlv<relive::Path_Mine_Converter, AO::Path_Mine>(j, tlv);
            break;
        case AO::TlvTypes::UXB_47:
            convert_tlv<relive::Path_UXB_Converter, AO::Path_UXB>(j, tlv);
            break;
        case AO::TlvTypes::Paramite_48:
            convert_tlv<relive::Path_Paramite_Converter, AO::Path_Paramite>(j, tlv);
            break;
        case AO::TlvTypes::Bat_49:
            convert_tlv<relive::Path_Bat_Converter, AO::Path_Bat>(j, tlv);
            break;
        case AO::TlvTypes::RingMudokon_50:
            convert_tlv<relive::Path_RingMudokon_Converter, AO::Path_RingMudokon>(j, tlv);
            break;
        case AO::TlvTypes::MovieStone_51:
            convert_tlv<relive::Path_MovieStone_Converter, AO::Path_MovieStone>(j, tlv);
            break;
        case AO::TlvTypes::BirdPortal_52:
            convert_tlv<relive::Path_BirdPortal_Converter, AO::Path_BirdPortal>(j, tlv);
            break;
        case AO::TlvTypes::BirdPortalExit_53:
            convert_tlv<relive::Path_BirdPortalExit_Converter, AO::Path_BirdPortalExit>(j, tlv);
            break;
        case AO::TlvTypes::BellSongStone_54:
            convert_tlv<relive::Path_BellsongStone_Converter, AO::Path_BellsongStone>(j, tlv);
            break;
        case AO::TlvTypes::TrapDoor_55:
            convert_tlv<relive::Path_TrapDoor_Converter, AO::Path_TrapDoor>(j, tlv);
            break;
        case AO::TlvTypes::RollingBall_56:
            convert_tlv<relive::Path_RollingBall_Converter, AO::Path_RollingBall>(j, tlv);
            break;
        case AO::TlvTypes::eSligBoundLeft_57:
            convert_tlv<relive::Path_SligBoundLeft_Converter, AO::Path_SligBoundLeft>(j, tlv);
            break;
        case AO::TlvTypes::InvisibleZone_58:
            convert_tlv<relive::Path_InvisibleZone_Converter, AO::Path_InvisibleZone>(j, tlv);
            break;
        case AO::TlvTypes::RollingBallStopper_59:
            convert_tlv<relive::Path_RollingBallStopper_Converter, AO::Path_RollingBallStopper>(j, tlv);
            break;
        case AO::TlvTypes::FootSwitch_60:
            convert_tlv<relive::Path_FootSwitch_Converter, AO::Path_FootSwitch>(j, tlv);
            break;
        case AO::TlvTypes::SecurityClaw_61:
            convert_tlv<relive::Path_SecurityClaw_Converter, AO::Path_SecurityClaw>(j, tlv);
            break;
        case AO::TlvTypes::MotionDetector_62:
            convert_tlv<relive::Path_MotionDetector_Converter, AO::Path_MotionDetector>(j, tlv);
            break;
        case AO::TlvTypes::SligSpawner_66:
            convert_tlv<relive::Path_SligSpawner_Converter, AO::Path_SligSpawner>(j, tlv);
            break;
        case AO::TlvTypes::ElectricWall_67:
            convert_tlv<relive::Path_ElectricWall_Converter, AO::Path_ElectricWall>(j, tlv);
            break;
        case AO::TlvTypes::LiftMover_68:
            convert_tlv<relive::Path_LiftMover_Converter, AO::Path_LiftMover>(j, tlv);
            break;
        case AO::TlvTypes::ChimeLock_69:
            convert_tlv<relive::Path_ChimeLock_Converter, AO::Path_ChimeLock>(j, tlv);
            break;
        case AO::TlvTypes::MeatSack_71:
            convert_tlv<relive::Path_MeatSack_Converter, AO::Path_MeatSack>(j, tlv);
            break;
        case AO::TlvTypes::Scrab_72:
            convert_tlv<relive::Path_Scrab_Converter, AO::Path_Scrab>(j, tlv);
            break;
        case AO::TlvTypes::FlintLockFire_73:
            convert_tlv<relive::Path_FlintLockFire_Converter, AO::Path_FlintLockFire>(j, tlv);
            break;
        case AO::TlvTypes::ScrabLeftBound_74:
            convert_tlv<relive::Path_ScrabBoundLeft_Converter, AO::Path_ScrabBoundLeft>(j, tlv);
            break;
        case AO::TlvTypes::ScrabRightBound_75:
            convert_tlv<relive::Path_ScrabBoundRight_Converter, AO::Path_ScrabBoundRight>(j, tlv);
            break;
        case AO::TlvTypes::eSligBoundRight_76:
            convert_tlv<relive::Path_SligBoundRight_Converter, AO::Path_SligBoundRight>(j, tlv);
            break;
        case AO::TlvTypes::eSligPersist_77:
            convert_tlv<relive::Path_SligPersist_Converter, AO::Path_SligPersist>(j, tlv);
            break;
        case AO::TlvTypes::EnemyStopper_79:
            convert_tlv<relive::Path_EnemyStopper_Converter, AO::Path_EnemyStopper>(j, tlv);
            break;
        case AO::TlvTypes::InvisibleSwitch_81:
            convert_tlv<relive::Path_InvisibleSwitch_Converter, AO::Path_InvisibleSwitch>(j, tlv);
            break;
        case AO::TlvTypes::Mudokon_82:
            convert_tlv<relive::Path_Mudokon_Converter, AO::Path_Mudokon>(j, tlv);
            break;
        case AO::TlvTypes::ZSligCover_83:
            convert_tlv<relive::Path_ZSligCover_Converter, AO::Path_ZSligCover>(j, tlv);
            break;
        case AO::TlvTypes::DoorFlame_84:
            convert_tlv<relive::Path_DoorFlame_Converter, AO::Path_DoorFlame>(j, tlv);
            break;
        case AO::TlvTypes::MovingBomb_86:
            convert_tlv<relive::Path_MovingBomb_Converter, AO::Path_MovingBomb>(j, tlv);
            break;
        case AO::TlvTypes::MovingBombStopper_87:
            convert_tlv<relive::Path_MovingBombStopper_Converter, AO::Path_MovingBombStopper>(j, tlv);
            break;
        case AO::TlvTypes::MeatSaw_88:
            convert_tlv<relive::Path_MeatSaw_Converter, AO::Path_MeatSaw>(j, tlv);
            break;
        case AO::TlvTypes::MudokonPathTrans_89:
            convert_tlv<relive::Path_MudokonPathTrans_Converter, AO::Path_MudokonPathTrans>(j, tlv);
            break;
        case AO::TlvTypes::MainMenuController_90:
            convert_tlv<relive::Path_MainMenuController_Converter, AO::Path_MainMenuController>(j, tlv);
            break;
        case AO::TlvTypes::HintFly_92:
            convert_tlv<relive::Path_HintFly_Converter, AO::Path_HintFly>(j, tlv);
            break;
        case AO::TlvTypes::ScrabNoFall_93:
            convert_tlv<relive::Path_ScrabNoFall_Converter, AO::Path_ScrabNoFall>(j, tlv);
            break;
        case AO::TlvTypes::TimerTrigger_94:
            convert_tlv<relive::Path_TimerTrigger_Converter, AO::Path_TimerTrigger>(j, tlv);
            break;
        case AO::TlvTypes::SecurityDoor_95:
            convert_tlv<relive::Path_SecurityDoor_Converter, AO::Path_SecurityDoor>(j, tlv);
            break;
        case AO::TlvTypes::DemoPlaybackStone_96:
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " is unused");
            return;
        case AO::TlvTypes::BoomMachine_97:
            convert_tlv<relive::Path_BoomMachine_Converter, AO::Path_BoomMachine>(j, tlv);
            break;
        case AO::TlvTypes::LCDScreen_98:
            convert_tlv<relive::Path_LCDScreen_Converter, AO::Path_LCDScreen>(j, tlv);
            break;
        case AO::TlvTypes::ElumPathTrans_99:
            convert_tlv<relive::Path_ElumPathTrans_Converter, AO::Path_ElumPathTrans>(j, tlv);
            break;
        case AO::TlvTypes::HandStone_100:
            convert_tlv<relive::Path_HandStone_Converter, AO::Path_HandStone>(j, tlv);
            break;
        case AO::TlvTypes::CreditsController_101:
            convert_tlv<relive::Path_CreditsController_Converter, AO::Path_CreditsController>(j, tlv);
            break;
        case AO::TlvTypes::Preloader_102: // dead tlv
            return;
        case AO::TlvTypes::LCDStatusBoard_103:
            convert_tlv<relive::Path_LCDStatusBoard_Converter, AO::Path_LCDStatusBoard>(j, tlv);
            break;
        case AO::TlvTypes::MusicTrigger_105:
            convert_tlv<relive::Path_MusicTrigger_Converter, AO::Path_MusicTrigger>(j, tlv);
            break;
        case AO::TlvTypes::LightEffect_106:
            convert_tlv<relive::Path_LightEffect_Converter, AO::Path_LightEffect>(j, tlv);
            break;
        case AO::TlvTypes::SlogSpawner_107:
            convert_tlv<relive::Path_SlogSpawner_Converter, AO::Path_SlogSpawner>(j, tlv);
            break;
        case AO::TlvTypes::GasCountDown_108:
            convert_tlv<relive::Path_GasCountDown_Converter, AO::Path_GasCountDown>(j, tlv);
            break;
        case AO::TlvTypes::RingCancel_109:
            convert_tlv<relive::Path_RingCancel_Converter, AO::Path_RingCancel>(j, tlv);
            break;
        case AO::TlvTypes::GasEmitter_110:
            convert_tlv<relive::Path_GasEmitter_Converter, AO::Path_GasEmitter>(j, tlv);
            break;
        case AO::TlvTypes::ZzzSpawner_111:
            convert_tlv<relive::Path_ZzzSpawner_Converter, AO::Path_ZzzSpawner>(j, tlv);
            break;
        case AO::TlvTypes::BackgroundGlukkon_112:
            convert_tlv<relive::Path_BackgroundGlukkon_Converter, AO::Path_BackgroundGlukkon>(j, tlv);
            break;
        case AO::TlvTypes::KillUnsavedMuds_113:
            convert_tlv<relive::Path_KillUnsavedMuds_Converter, AO::Path_KillUnsavedMuds>(j, tlv);
            break;
        case AO::TlvTypes::SoftLanding_114:
            convert_tlv<relive::Path_SoftLanding_Converter, AO::Path_SoftLanding>(j, tlv);
            break;
        case AO::TlvTypes::ResetPath_115:
            convert_tlv<relive::Path_ResetPath_Converter, AO::Path_ResetPath>(j, tlv);
            break;

        default:
            ALIVE_FATAL("TLV conversion for this type not implemented");
    }
}

static void ConvertPathTLVs(nlohmann::json& j, const AO::PathData& info, const std::vector<u8>& pathResource, u32 indexTableIndex)
{
    const u8* pData = pathResource.data();
    const s32* pIndexTable = reinterpret_cast<const s32*>(pData + info.field_18_object_index_table_offset);

    const s32 indexTableValue = pIndexTable[indexTableIndex];
    if (indexTableValue == -1)
    {
        return;
    }

    const u8* pStart = pathResource.data() + (indexTableValue + info.field_14_object_offset);
   
    auto pPathTLV = reinterpret_cast<const AO::Path_TLV*>(pStart);
    while (pPathTLV)
    {
        // Convert TLV to ReliveTLV
        ConvertTLV(j, *pPathTLV);

        // End of TLVs for given camera
        if (pPathTLV->mTlvFlags.Get(AO::TlvFlags::eBit3_End_TLV_List))
        {
            break;
        }

        // Skip length bytes to get to the start of the next TLV
        const u8* ptr = reinterpret_cast<const u8*>(pPathTLV);
        const u8* pNext = ptr + pPathTLV->mLength;
        pPathTLV = reinterpret_cast<const AO::Path_TLV*>(pNext);
    }
}

static void to_json(nlohmann::json& j, const CameraEntry& p)
{
    j = nlohmann::json{
        {"x", p.mX},
        {"y", p.mY},
        {"id", p.mId}, // TODO: Can probably get rid of this in the future
        {"name", p.mName},
    };
}

static void SaveJson(const nlohmann::json& j, FileSystem& fs, const FileSystem::Path& path)
{
    std::string jsonStr = j.dump(4);

    std::vector<u8> data;
    data.resize(jsonStr.length());
    data.assign(jsonStr.begin(), jsonStr.end());
    fs.Save(path, data);
}

static void ConvertPath(FileSystem& fs, const FileSystem::Path& path, const ReliveAPI::LvlFileChunk& pathBndChunk, EReliveLevelIds reliveLvl)
{
    const AO::PathBlyRec* pBlyRec = AO::Path_Get_Bly_Record_434650(reliveLvl, static_cast<u16>(pathBndChunk.Id()));

    // Save cameras and map objects
    const s32 width = (pBlyRec->field_4_pPathData->field_8_bTop - pBlyRec->field_4_pPathData->field_4_bLeft) / pBlyRec->field_4_pPathData->field_C_grid_width;
    const s32 height = (pBlyRec->field_4_pPathData->field_A_bBottom - pBlyRec->field_4_pPathData->field_6_bRight) / pBlyRec->field_4_pPathData->field_E_grid_height;

    nlohmann::json camerasArray = nlohmann::json::array();
    PathCamerasEnumerator cameraEnumerator(width, height, pathBndChunk.Data());
    cameraEnumerator.Enumerate([&](const CameraEntry& tmpCamera)
                               {
        // Save map objects
        nlohmann::json mapObjectsArray = nlohmann::json::array();
        const u32 indexTableIdx = To1dIndex(width, tmpCamera.mX, tmpCamera.mY);
        LOG_INFO(indexTableIdx);
        ConvertPathTLVs(mapObjectsArray, *pBlyRec->field_4_pPathData, pathBndChunk.Data(), indexTableIdx);

        // Its possible to have a camera with no objects (-1 index table)
        // But its also possible to have a blank camera with objects (blank camera name, non -1 index table)
        if (!tmpCamera.mName.empty() || !mapObjectsArray.empty())
        { 
            nlohmann::json camJson;
            to_json(camJson, tmpCamera);
            camJson["map_objects"] = mapObjectsArray;
            camerasArray.push_back(camJson);
        } });

    // Save collisions
    nlohmann::json collisionsArray = nlohmann::json::array();
    ConvertPathCollisions(collisionsArray, *pBlyRec->field_8_pCollisionData, pathBndChunk.Data());


    nlohmann::json j = {
        {"cameras", camerasArray},
        {"collisions", collisionsArray},
    };

    SaveJson(j, fs, path);
    LOG_INFO("converted path " << pathBndChunk.Id() << " level " << ToString(MapWrapper::ToAO(reliveLvl)));
}

void TestTlvConversion()
{
    nlohmann::json j;

    AO::Path_ShadowZone shadowTlv = {};
    shadowTlv.mTlvType32.mType = AO::TlvTypes::ContinuePoint_0;
    ConvertTLV(j, shadowTlv);

    // TODO: Check from AE tlv

    LOG_INFO(j.dump(4));
}

static void SaveLevelInfoJson(EReliveLevelIds /*reliveLvl*/, AO::LevelIds lvlIdxAsLvl, FileSystem& fs, const ReliveAPI::ChunkedLvlFile& pathBndFile)
{
    FileSystem::Path pathDir;
    pathDir.Append("relive_data").Append("ao").Append(ToString(lvlIdxAsLvl)).Append("paths");
    fs.CreateDirectory(pathDir);

    FileSystem::Path pathJsonFile = pathDir;
    pathJsonFile.Append("level_info.json");

    nlohmann::json jsonPathFilesArray;

    // Convert hard coded path data json
    for (u32 j = 0; j < pathBndFile.ChunkCount(); j++)
    {
        const ReliveAPI::LvlFileChunk& pathBndChunk = pathBndFile.ChunkAt(j);
        if (pathBndChunk.Header().mResourceType == AO::ResourceManager::Resource_Path)
        {
            // Write out what paths exist so the game knows what files to load
            jsonPathFilesArray.push_back(std::to_string(pathBndChunk.Header().field_C_id) + ".json");
        }
    }

    // TODO: Misc info, back ground music id, reverb etc

    // TODO: Colour tints per object in this lvl

    // TODO: Music info - incl each SEQ name
    /*
    AO::SoundBlockInfo* pSoundBlock = AO::Path_Get_MusicInfo(reliveLvl);
    pSoundBlock->
    */

    nlohmann::json j;
    j["paths"] = jsonPathFilesArray;

    SaveJson(j, fs, pathJsonFile);
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
    // For each LVL
    for (s32 lvlIdx = 0; lvlIdx < AO::Path_Get_Paths_Count(); lvlIdx++)
    {
        // Skip entries that have no data
        const AO::LevelIds lvlIdxAsLvl = static_cast<AO::LevelIds>(lvlIdx);
        if (lvlIdxAsLvl == AO::LevelIds::eRemoved_7 || lvlIdxAsLvl == AO::LevelIds::eRemoved_11)
        {
            continue;
        }

        // Open the LVL file
        const EReliveLevelIds reliveLvl = MapWrapper::FromAO(lvlIdxAsLvl);
        ReliveAPI::FileIO fileIo;
        ReliveAPI::LvlReader lvlReader(fileIo, (std::string(AO::Path_Get_Lvl_Name(reliveLvl)) + ".LVL").c_str());

        if (!lvlReader.IsOpen())
        {
            // Fatal, missing LVL file
            ALIVE_FATAL("Couldn't open lvl file");
        }

        // Convert animations that exist in this LVL
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

        // Iterate and convert spceific file types in the LVL
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
                    // TODO: For BSQ convert back to the original SEQ name for each entry

                    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);

                    FileSystem::Path filePath;
                    filePath.Append("relive_data").Append("ao").Append(ToString(lvlIdxAsLvl));
                    fs.CreateDirectory(filePath);
                    filePath.Append(fileName);


                    fs.Save(filePath, fileBuffer);
                }
                else if (endsWith(fileName, "PATH.BND"))
                {
                    FileSystem::Path pathDir;
                    pathDir.Append("relive_data").Append("ao").Append(ToString(lvlIdxAsLvl)).Append("paths");
                    fs.CreateDirectory(pathDir);

                    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);
                    ReliveAPI::ChunkedLvlFile pathBndFile(fileBuffer);
                    for (u32 j = 0; j < pathBndFile.ChunkCount(); j++)
                    {
                        const ReliveAPI::LvlFileChunk& pathBndChunk = pathBndFile.ChunkAt(j);
                        if (pathBndChunk.Header().mResourceType == AO::ResourceManager::Resource_Path)
                        {
                            FileSystem::Path pathJsonFile = pathDir;
                            pathJsonFile.Append(std::to_string(pathBndChunk.Header().field_C_id) + ".json");
                            ConvertPath(fs, pathJsonFile, pathBndChunk, reliveLvl);
                        }
                    }

                    SaveLevelInfoJson(reliveLvl, lvlIdxAsLvl, fs, pathBndFile);
                }
            }
        }
    }
}
