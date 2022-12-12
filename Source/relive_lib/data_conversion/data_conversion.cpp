#include "stdafx.h"
#include "data_conversion.hpp"
#include "file_system.hpp"
#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/Path.hpp"
#include "../../AliveLibAE/Font.hpp"
#include "../../AliveLibCommon/AnimResources.hpp"
#include "../Animation.hpp"
#include "../MapWrapper.hpp"
#include <algorithm>
#include <cctype>

#include "nlohmann/json.hpp"
#include "../../Tools/relive_api/LvlReaderWriter.hpp"
#include "../../Tools/relive_api/file_api.hpp"
#include "../../Tools/relive_api/CamConverter.hpp"
#include "../../Tools/relive_api/PathCamerasEnumerator.hpp"
#include "../../Tools/relive_api/ApiFG1Reader.hpp"
#include "../Collisions.hpp"
#include "AnimationConverter.hpp"
#include "relive_tlvs_conversion.hpp"
#include "Collisions.hpp"
#include "AnimConversionInfo.hpp"
#include "PNGFile.hpp"
#include "AESaveConverter.hpp"
#include "../BinaryPath.hpp"
#include "../../AliveLibAE/ResourceManager.hpp"
#include "fmv_converter.hpp"

// Bump this if any data format breaks are made so that OG/mod data is re-converted/upgraded
const u32 DataConversion::kVersion = 2;

static bool ReadLvlFileInto(ReliveAPI::LvlReader& archive, const char_type* fileName, std::vector<u8>& fileBuffer)
{
    if (!archive.ReadFileInto(fileBuffer, fileName))
    {
        fileBuffer.clear();
        return false;
    }
    return true;
}

static bool endsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static void ToPathLine(PathLine& dst, const PathLineAO& src)
{
    dst.mRect = src.mRect;
    dst.mLineType = src.mLineType;
    dst.field_A_previous = static_cast<s16>(src.field_C_previous);
    dst.field_C_next = static_cast<s16>(src.field_10_next);
    dst.field_12_line_length = 0; // TODO: Calculate for AO in the future
}

static void ToPathLine(PathLine& dst, const PathLineAE& src)
{
    dst.mRect = src.mRect;
    dst.mLineType = src.mLineType;
    dst.field_A_previous = src.field_A_previous;
    dst.field_C_next = src.field_C_next;
    dst.field_12_line_length = src.field_12_line_length;
}

static void ConvertPathCollisions(nlohmann::json& j, const CollisionInfo& info, const std::vector<u8>& pathResource, bool isAo)
{
    const u8* pData = pathResource.data();
    const u8* pStart = pData + info.field_C_collision_offset;

    if (isAo)
    {
        auto pCollisions = reinterpret_cast<const PathLineAO*>(pStart);
        for (u32 i = 0; i < info.field_10_num_collision_items; i++)
        {
            PathLine tmp;
            ToPathLine(tmp, pCollisions[i]);
            j.push_back(tmp);
        }
    }
    else
    {
        auto pCollisions = reinterpret_cast<const PathLineAE*>(pStart);
        for (u32 i = 0; i < info.field_10_num_collision_items; i++)
        {
            PathLine tmp;
            ToPathLine(tmp, pCollisions[i]);
            j.push_back(tmp);
        }
    }
}

template <typename converter_type, typename tlv_src_type>
static void convert_tlv(nlohmann::json& j, const AO::Path_TLV& tlv, const Guid& tlvId)
{
    j.push_back(converter_type::From(static_cast<const tlv_src_type&>(tlv), tlvId));
}

template <typename converter_type, typename tlv_src_type>
static void convert_tlv(nlohmann::json& j, const ::Path_TLV& tlv, const Guid& tlvId)
{
    j.push_back(converter_type::From(static_cast<const tlv_src_type&>(tlv), tlvId));
}


static void ConvertTLV(nlohmann::json& j, const ::Path_TLV& tlv, const Guid& tlvId, ::LevelIds /*lvlId*/, u32 /*pathId*/)
{
    switch (tlv.mTlvType32.mType)
    {
        case ::TlvTypes::ContinuePoint_0:
            convert_tlv<relive::Path_ContinuePoint_Converter, ::Path_ContinuePoint>(j, tlv, tlvId);
            break;
        case ::TlvTypes::PathTransition_1:
            convert_tlv<relive::Path_PathTransition_Converter, ::Path_PathTransition>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Hoist_2:
            convert_tlv<relive::Path_Hoist_Converter, ::Path_Hoist>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Edge_3:
            convert_tlv<relive::Path_Edge_Converter, ::Path_Edge>(j, tlv, tlvId);
            break;
        case ::TlvTypes::DeathDrop_4:
            convert_tlv<relive::Path_DeathDrop_Converter, ::Path_DeathDrop>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Door_5:
            convert_tlv<relive::Path_Door_Converter, ::Path_Door>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ShadowZone_6:
            convert_tlv<relive::Path_ShadowZone_Converter, ::Path_ShadowZone>(j, tlv, tlvId);
            break;
        case ::TlvTypes::LiftPoint_7:
            convert_tlv<relive::Path_LiftPoint_Converter, ::Path_LiftPoint>(j, tlv, tlvId);
            break;
        case ::TlvTypes::LocalWell_8:
            convert_tlv<relive::Path_WellLocal_Converter, ::Path_WellLocal>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Dove_9:
            convert_tlv<relive::Path_Dove_Converter, ::Path_Dove>(j, tlv, tlvId);
            break;
        case ::TlvTypes::RockSack_10:
            convert_tlv<relive::Path_RockSack_Converter, ::Path_RockSack>(j, tlv, tlvId);
            break;
        case ::TlvTypes::FallingItem_11:
            convert_tlv<relive::Path_FallingItem_Converter, ::Path_FallingItem>(j, tlv, tlvId);
            break;
        case ::TlvTypes::PullRingRope_12:
            convert_tlv<relive::Path_PullRingRope_Converter, ::Path_PullRingRope>(j, tlv, tlvId);
            break;
        case ::TlvTypes::BackgroundAnimation_13:
            convert_tlv<relive::Path_BackgroundAnimation_Converter, ::Path_BackgroundAnimation>(j, tlv, tlvId);
            break;
        case ::TlvTypes::TimedMine_14:
            convert_tlv<relive::Path_TimedMine_Converter, ::Path_TimedMine>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Slig_15:
            convert_tlv<relive::Path_Slig_Converter, ::Path_Slig>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Slog_16:
            convert_tlv<relive::Path_Slog_Converter, ::Path_Slog>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Lever_17:
            convert_tlv<relive::Path_Lever_Converter, ::Path_Lever>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Null_18:
            return;
        case ::TlvTypes::SecurityOrb_19:
            convert_tlv<relive::Path_SecurityOrb_Converter, ::Path_SecurityOrb>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Null_20:
            return;
        case ::TlvTypes::Pulley_21:
            convert_tlv<relive::Path_Pulley_Converter, ::Path_Pulley>(j, tlv, tlvId);
            break;
        case ::TlvTypes::AbeStart_22:
            convert_tlv<relive::Path_AbeStart_Converter, ::Path_AbeStart>(j, tlv, tlvId);
            break;
        case ::TlvTypes::WellExpress_23:
            convert_tlv<relive::Path_WellExpress_Converter, ::Path_WellExpress>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Mine_24:
            convert_tlv<relive::Path_Mine_Converter, ::Path_Mine>(j, tlv, tlvId);
            break;
        case ::TlvTypes::UXB_25:
            convert_tlv<relive::Path_UXB_Converter, ::Path_UXB>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Paramite_26:
            convert_tlv<relive::Path_Paramite_Converter, ::Path_Paramite>(j, tlv, tlvId);
            break;
        case ::TlvTypes::MovieHandStone_27:
            convert_tlv<relive::Path_MovieStone_Converter, ::Path_MovieStone>(j, tlv, tlvId);
            break;
        case ::TlvTypes::BirdPortal_28:
            convert_tlv<relive::Path_BirdPortal_Converter, ::Path_BirdPortal>(j, tlv, tlvId);
            break;
        case ::TlvTypes::BirdPortalExit_29:
            convert_tlv<relive::Path_BirdPortalExit_Converter, ::Path_BirdPortalExit>(j, tlv, tlvId);
            break;
        case ::TlvTypes::TrapDoor_30:
            convert_tlv<relive::Path_TrapDoor_Converter, ::Path_TrapDoor>(j, tlv, tlvId);
            break;
        case ::TlvTypes::RollingBall_31: // not implemented in relive AE
            return;
        case ::TlvTypes::SligBoundLeft_32:
            convert_tlv<relive::Path_SligBoundLeft_Converter, ::Path_SligBoundLeft>(j, tlv, tlvId);
            break;
        case ::TlvTypes::InvisibleZone_33:
            convert_tlv<relive::Path_InvisibleZone_Converter, ::Path_InvisibleZone>(j, tlv, tlvId);
            break;
        case ::TlvTypes::FootSwitch_34:
            convert_tlv<relive::Path_FootSwitch_Converter, ::Path_FootSwitch>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SecurityClaw_35: // TODO: this tlv is the same as the orb tlv
            convert_tlv<relive::Path_SecurityClaw_Converter, ::Path_SecurityClaw>(j, tlv, tlvId);
            break;
        case ::TlvTypes::MotionDetector_36:
            convert_tlv<relive::Path_MotionDetector_Converter, ::Path_MotionDetector>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SligSpawner_37:
            convert_tlv<relive::Path_SligSpawner_Converter, ::Path_SligSpawner>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ElectricWall_38:
            convert_tlv<relive::Path_ElectricWall_Converter, ::Path_ElectricWall>(j, tlv, tlvId);
            break;
        case ::TlvTypes::LiftMover_39:
            convert_tlv<relive::Path_LiftMover_Converter, ::Path_LiftMover>(j, tlv, tlvId);
            break;
        case ::TlvTypes::MeatSack_40:
            convert_tlv<relive::Path_MeatSack_Converter, ::Path_MeatSack>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Scrab_41:
            convert_tlv<relive::Path_Scrab_Converter, ::Path_Scrab>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Null_42:
            return;
        case ::TlvTypes::ScrabBoundLeft_43:
            convert_tlv<relive::Path_ScrabBoundLeft_Converter, ::Path_ScrabBoundLeft>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ScrabBoundRight_44:
            convert_tlv<relive::Path_ScrabBoundRight_Converter, ::Path_ScrabBoundRight>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SligBoundRight_45:
            convert_tlv<relive::Path_SligBoundRight_Converter, ::Path_SligBoundRight>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SligPersist_46:
            convert_tlv<relive::Path_SligPersist_Converter, ::Path_SligPersist>(j, tlv, tlvId);
            break;
        case ::TlvTypes::EnemyStopper_47:
            convert_tlv<relive::Path_EnemyStopper_Converter, ::Path_EnemyStopper>(j, tlv, tlvId);
            break;
        case ::TlvTypes::InvisibleSwitch_48:
            convert_tlv<relive::Path_InvisibleSwitch_Converter, ::Path_InvisibleSwitch>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Mudokon_49:
            convert_tlv<relive::Path_Mudokon_Converter, ::Path_Mudokon>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ZSligCover_50:
            convert_tlv<relive::Path_ZSligCover_Converter, ::Path_ZSligCover>(j, tlv, tlvId);
            break;
        case ::TlvTypes::DoorFlame_51:
            convert_tlv<relive::Path_DoorFlame_Converter, ::Path_DoorFlame>(j, tlv, tlvId);
            break;
        case ::TlvTypes::MovingBomb_52:
            convert_tlv<relive::Path_MovingBomb_Converter, ::Path_MovingBomb>(j, tlv, tlvId);
            break;
        case ::TlvTypes::MovingBombStopper_53:
            convert_tlv<relive::Path_MovingBombStopper_Converter, ::Path_MovingBombStopper>(j, tlv, tlvId);
            break;
        case ::TlvTypes::MainMenuController_54:
            convert_tlv<relive::Path_MainMenuController_Converter, ::Path_MainMenuController>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Unknown_55:
            LOG_INFO("unknown tlv 55");
            return;
        case ::TlvTypes::Null_56:
            return;
        case ::TlvTypes::TimerTrigger_57:
            convert_tlv<relive::Path_TimerTrigger_Converter, ::Path_TimerTrigger>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SecurityDoor_58:
            convert_tlv<relive::Path_SecurityDoor_Converter, ::Path_SecurityDoor>(j, tlv, tlvId);
            break;
        case ::TlvTypes::BoomMachine_59:
            convert_tlv<relive::Path_BoomMachine_Converter, ::Path_BoomMachine>(j, tlv, tlvId);
            break;
        case ::TlvTypes::LCDScreen_60:
            convert_tlv<relive::Path_LCDScreen_Converter, ::Path_LCDScreen>(j, tlv, tlvId);
            break;
        case ::TlvTypes::HandStone_61:
            convert_tlv<relive::Path_HandStone_Converter, ::Path_HandStone>(j, tlv, tlvId);
            break;
        case ::TlvTypes::CreditsController_62:
            convert_tlv<relive::Path_CreditsController_Converter, ::Path_CreditsController>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Null_63:
            return;
        case ::TlvTypes::LCDStatusBoard_64:
            convert_tlv<relive::Path_LCDStatusBoard_Converter, ::Path_LCDStatusBoard>(j, tlv, tlvId);
            break;
        case ::TlvTypes::WheelSyncer_65:
            convert_tlv<relive::Path_WheelSyncer_Converter, ::Path_WheelSyncer>(j, tlv, tlvId);
            return;
        case ::TlvTypes::MusicTrigger_66:
            convert_tlv<relive::Path_MusicTrigger_Converter, ::Path_MusicTrigger>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Light_67: // not implemented in AE
            return;
        case ::TlvTypes::SlogSpawner_68:
            convert_tlv<relive::Path_SlogSpawner_Converter, ::Path_SlogSpawner>(j, tlv, tlvId);
            break;
        case ::TlvTypes::GasCountdown_69:
            convert_tlv<relive::Path_GasCountDown_Converter, ::Path_GasCountDown>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Unknown_70:
            ALIVE_FATAL("unknown tlv type 70 unused?");
            return;
        case ::TlvTypes::GasEmitter_71:
            convert_tlv<relive::Path_GasEmitter_Converter, ::Path_GasEmitter>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ZzzSpawner_72:
            convert_tlv<relive::Path_ZzzSpawner_Converter, ::Path_ZzzSpawner>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Glukkon_73:
            convert_tlv<relive::Path_Glukkon_Converter, ::Path_Glukkon>(j, tlv, tlvId);
            break;
        case ::TlvTypes::KillUnsavedMudokons_74: // AO only
            return;
        case ::TlvTypes::SoftLanding_75:
            convert_tlv<relive::Path_SoftLanding_Converter, ::Path_SoftLanding>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ResetPath_76:
            convert_tlv<relive::Path_ResetPath_Converter, ::Path_ResetPath>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Water_77:
            convert_tlv<relive::Path_Water_Converter, ::Path_Water>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Null_78:
            return;
        case ::TlvTypes::WorkWheel_79:
            convert_tlv<relive::Path_WorkWheel_Converter, ::Path_WorkWheel>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Null_80:
            return;
        case ::TlvTypes::LaughingGas_81:
            convert_tlv<relive::Path_LaughingGas_Converter, ::Path_LaughingGas>(j, tlv, tlvId);
            break;
        case ::TlvTypes::FlyingSlig_82:
            convert_tlv<relive::Path_FlyingSlig_Converter, ::Path_FlyingSlig>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Fleech_83:
            convert_tlv<relive::Path_Fleech_Converter, ::Path_Fleech>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Slurg_84:
            convert_tlv<relive::Path_Slurg_Converter, ::Path_Slurg>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SlamDoor_85:
            convert_tlv<relive::Path_SlamDoor_Converter, ::Path_SlamDoor>(j, tlv, tlvId);
            break;
        case ::TlvTypes::LevelLoader_86:
            convert_tlv<relive::Path_LevelLoader_Converter, ::Path_LevelLoader>(j, tlv, tlvId);
            break;
        case ::TlvTypes::DemoSpawnPoint_87:
            convert_tlv<relive::Path_DemoSpawnPoint_Converter, ::Path_DemoSpawnPoint>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Teleporter_88:
            convert_tlv<relive::Path_Teleporter_Converter, ::Path_Teleporter>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SlurgSpawner_89:
            convert_tlv<relive::Path_SlurgSpawner_Converter, ::Path_SlurgSpawner>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Drill_90:
            convert_tlv<relive::Path_Drill_Converter, ::Path_Drill>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ColourfulMeter_91:
            convert_tlv<relive::Path_ColourfulMeter_Converter, ::Path_ColourfulMeter>(j, tlv, tlvId);
            break;
        case ::TlvTypes::FlyingSligSpawner_92:
            convert_tlv<relive::Path_FlyingSligSpawner_Converter, ::Path_FlyingSligSpawner>(j, tlv, tlvId);
            break;
        case ::TlvTypes::MineCar_93:
            convert_tlv<relive::Path_MineCar_Converter, ::Path_MineCar>(j, tlv, tlvId);
            break;
        case ::TlvTypes::BoneBag_94:
            convert_tlv<relive::Path_BoneBag_Converter, ::Path_BoneBag>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ExplosionSet_95:
            convert_tlv<relive::Path_ExplosionSet_Converter, ::Path_ExplosionSet>(j, tlv, tlvId);
            break;
        case ::TlvTypes::MultiSwitchController_96:
            convert_tlv<relive::Path_MultiSwitchController_Converter, ::Path_MultiSwitchController>(j, tlv, tlvId);
            break;
        case ::TlvTypes::StatusLight_97:
            convert_tlv<relive::Path_StatusLight_Converter, ::Path_StatusLight>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SlapLock_98:
            convert_tlv<relive::Path_SlapLock_Converter, ::Path_SlapLock>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ParamiteWebLine_99:
            convert_tlv<relive::Path_ParamiteWebLine_Converter, ::Path_ParamiteWebLine>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Alarm_100:
            convert_tlv<relive::Path_Alarm_Converter, ::Path_Alarm>(j, tlv, tlvId);
            break;
        case ::TlvTypes::BrewMachine_101:
            convert_tlv<relive::Path_BrewMachine_Converter, ::Path_BrewMachine>(j, tlv, tlvId);
            break;
        case ::TlvTypes::ScrabSpawner_102:
            convert_tlv<relive::Path_ScrabSpawner_Converter, ::Path_ScrabSpawner>(j, tlv, tlvId);
            break;
        case ::TlvTypes::CrawlingSlig_103:
            convert_tlv<relive::Path_CrawlingSlig_Converter, ::Path_CrawlingSlig>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SligGetPants_104:
            convert_tlv<relive::Path_SligGetPants_Converter, ::Path_SligGetPants>(j, tlv, tlvId);
            break;
        case ::TlvTypes::SligGetWings_105:
            convert_tlv<relive::Path_SligGetWings_Converter, ::Path_SligGetWings>(j, tlv, tlvId);
            break;
        case ::TlvTypes::Greeter_106:
            convert_tlv<relive::Path_Greeter_Converter, ::Path_Greeter>(j, tlv, tlvId);
            break;
        case ::TlvTypes::CrawlingSligButton_107:
            convert_tlv<relive::Path_CrawlingSligButton_Converter, ::Path_CrawlingSligButton>(j, tlv, tlvId);
            break;
        case ::TlvTypes::GlukkonSwitch_108:
            convert_tlv<relive::Path_GlukkonSwitch_Converter, ::Path_GlukkonSwitch>(j, tlv, tlvId);
            break;
        case ::TlvTypes::DoorBlocker_109:
            convert_tlv<relive::Path_DoorBlocker_Converter, ::Path_DoorBlocker>(j, tlv, tlvId);
            break;
        case ::TlvTypes::TorturedMudokon_110:
            convert_tlv<relive::Path_TorturedMudokon_Converter, ::Path_TorturedMudokon>(j, tlv, tlvId);
            break;
        case ::TlvTypes::TrainDoor_111:
            convert_tlv<relive::Path_TrainDoor_Converter, ::Path_TrainDoor>(j, tlv, tlvId);
            break;
        default:
            ALIVE_FATAL("TLV conversion for this type not implemented");
    }
}


static void ConvertTLV(nlohmann::json& j, const AO::Path_TLV& tlv, const Guid& tlvId, AO::LevelIds lvlId, u32 pathId)
{
    switch (tlv.mTlvType32.mType)
    {
        case AO::TlvTypes::ContinuePoint_0:
            convert_tlv<relive::Path_ContinuePoint_Converter, AO::Path_ContinuePoint>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::PathTransition_1:
            convert_tlv<relive::Path_PathTransition_Converter, AO::Path_PathTransition>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ContinueZone_2: // dead tlv
            return;
        case AO::TlvTypes::Hoist_3:
            convert_tlv<relive::Path_Hoist_Converter, AO::Path_Hoist>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Edge_4:
            convert_tlv<relive::Path_Edge_Converter, AO::Path_Edge>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::DeathDrop_5:
            convert_tlv<relive::Path_DeathDrop_Converter, AO::Path_DeathDrop>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Door_6:
            convert_tlv<relive::Path_Door_Converter, AO::Path_Door>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ShadowZone_7:
            convert_tlv<relive::Path_ShadowZone_Converter, AO::Path_ShadowZone>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::LiftPoint_8:
            convert_tlv<relive::Path_LiftPoint_Converter, AO::Path_LiftPoint>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::WellLocal_11:
            convert_tlv<relive::Path_WellLocal_Converter, AO::Path_WellLocal>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Dove_12:
            convert_tlv<relive::Path_Dove_Converter, AO::Path_Dove>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::RockSack_13:
            convert_tlv<relive::Path_RockSack_Converter, AO::Path_RockSack>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ZBall_14:
            convert_tlv<relive::Path_ZBall_Converter, AO::Path_ZBall>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::FallingItem_15:
            convert_tlv<relive::Path_FallingItem_Converter, AO::Path_FallingItem>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::PullRingRope_18:
            convert_tlv<relive::Path_PullRingRope_Converter, AO::Path_PullRingRope>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::BackgroundAnimation_19:
            convert_tlv<relive::Path_BackgroundAnimation_Converter, AO::Path_BackgroundAnimation>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Honey_20:
            convert_tlv<relive::Path_Honey_Converter, AO::Path_Honey>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::TimedMine_22:
            convert_tlv<relive::Path_TimedMine_Converter, AO::Path_TimedMine>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Slig_24:
            convert_tlv<relive::Path_Slig_Converter, AO::Path_Slig>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Slog_25:
            convert_tlv<relive::Path_Slog_Converter, AO::Path_Slog>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Lever_26:
            convert_tlv<relive::Path_Lever_Converter, AO::Path_Lever>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::BellHammer_27:
            convert_tlv<relive::Path_BellHammer_Converter, AO::Path_BellHammer>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::StartController_28:
            convert_tlv<relive::Path_StartController_Converter, AO::Path_StartController>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::SecurityOrb_29:
            convert_tlv<relive::Path_SecurityOrb_Converter, AO::Path_SecurityOrb>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::LiftMudokon_32:
            convert_tlv<relive::Path_LiftMudokon_Converter, AO::Path_LiftMudokon>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::BeeSwarmHole_34:
            convert_tlv<relive::Path_BeeSwarmHole_Converter, AO::Path_BeeSwarmHole>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Pulley_35:
            convert_tlv<relive::Path_Pulley_Converter, AO::Path_Pulley>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::HoneySack_36:
            convert_tlv<relive::Path_HoneySack_Converter, AO::Path_HoneySack>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::AbeStart_37:
            convert_tlv<relive::Path_AbeStart_Converter, AO::Path_AbeStart>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ElumStart_38:
            convert_tlv<relive::Path_ElumStart_Converter, AO::Path_ElumStart>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ElumWall_40:
            convert_tlv<relive::Path_ElumWall_Converter, AO::Path_ElumWall>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::SlingMudokon_41:
            convert_tlv<relive::Path_SlingMudokon_Converter, AO::Path_SlingMudokon>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::HoneyDripTarget_42:
            convert_tlv<relive::Path_HoneyDripTarget_Converter, AO::Path_HoneyDripTarget>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::BeeNest_43:
            convert_tlv<relive::Path_BeeNest_Converter, AO::Path_BeeNest>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::WellExpress_45:
            convert_tlv<relive::Path_WellExpress_Converter, AO::Path_WellExpress>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Mine_46:
            convert_tlv<relive::Path_Mine_Converter, AO::Path_Mine>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::UXB_47:
            convert_tlv<relive::Path_UXB_Converter, AO::Path_UXB>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Paramite_48:
            convert_tlv<relive::Path_Paramite_Converter, AO::Path_Paramite>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Bat_49:
            convert_tlv<relive::Path_Bat_Converter, AO::Path_Bat>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::RingMudokon_50:
            convert_tlv<relive::Path_RingMudokon_Converter, AO::Path_RingMudokon>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::MovieStone_51:
            convert_tlv<relive::Path_MovieStone_Converter, AO::Path_MovieStone>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::BirdPortal_52:
            convert_tlv<relive::Path_BirdPortal_Converter, AO::Path_BirdPortal>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::BirdPortalExit_53:
            convert_tlv<relive::Path_BirdPortalExit_Converter, AO::Path_BirdPortalExit>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::BellSongStone_54:
            convert_tlv<relive::Path_BellsongStone_Converter, AO::Path_BellsongStone>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::TrapDoor_55:
            convert_tlv<relive::Path_TrapDoor_Converter, AO::Path_TrapDoor>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::RollingBall_56:
            convert_tlv<relive::Path_RollingBall_Converter, AO::Path_RollingBall>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::eSligBoundLeft_57:
            convert_tlv<relive::Path_SligBoundLeft_Converter, AO::Path_SligBoundLeft>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::InvisibleZone_58:
            convert_tlv<relive::Path_InvisibleZone_Converter, AO::Path_InvisibleZone>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::RollingBallStopper_59:
            convert_tlv<relive::Path_RollingBallStopper_Converter, AO::Path_RollingBallStopper>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::FootSwitch_60:
            convert_tlv<relive::Path_FootSwitch_Converter, AO::Path_FootSwitch>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::SecurityClaw_61:
            convert_tlv<relive::Path_SecurityClaw_Converter, AO::Path_SecurityClaw>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::MotionDetector_62:
            convert_tlv<relive::Path_MotionDetector_Converter, AO::Path_MotionDetector>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::SligSpawner_66:
            convert_tlv<relive::Path_SligSpawner_Converter, AO::Path_SligSpawner>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ElectricWall_67:
            convert_tlv<relive::Path_ElectricWall_Converter, AO::Path_ElectricWall>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::LiftMover_68:
            convert_tlv<relive::Path_LiftMover_Converter, AO::Path_LiftMover>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ChimeLock_69:
            convert_tlv<relive::Path_ChimeLock_Converter, AO::Path_ChimeLock>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Elum_70:
            ALIVE_FATAL("TODO Elum_70");
            break;
        case AO::TlvTypes::MeatSack_71:
            convert_tlv<relive::Path_MeatSack_Converter, AO::Path_MeatSack>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Scrab_72:
            convert_tlv<relive::Path_Scrab_Converter, AO::Path_Scrab>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::FlintLockFire_73:
            convert_tlv<relive::Path_FlintLockFire_Converter, AO::Path_FlintLockFire>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ScrabLeftBound_74:
            convert_tlv<relive::Path_ScrabBoundLeft_Converter, AO::Path_ScrabBoundLeft>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ScrabRightBound_75:
            convert_tlv<relive::Path_ScrabBoundRight_Converter, AO::Path_ScrabBoundRight>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::eSligBoundRight_76:
            convert_tlv<relive::Path_SligBoundRight_Converter, AO::Path_SligBoundRight>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::eSligPersist_77:
            convert_tlv<relive::Path_SligPersist_Converter, AO::Path_SligPersist>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::EnemyStopper_79:
            convert_tlv<relive::Path_EnemyStopper_Converter, AO::Path_EnemyStopper>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::InvisibleSwitch_81:
            convert_tlv<relive::Path_InvisibleSwitch_Converter, AO::Path_InvisibleSwitch>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Mudokon_82:
            convert_tlv<relive::Path_Mudokon_Converter, AO::Path_Mudokon>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ZSligCover_83:
            convert_tlv<relive::Path_ZSligCover_Converter, AO::Path_ZSligCover>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::DoorFlame_84:
            convert_tlv<relive::Path_DoorFlame_Converter, AO::Path_DoorFlame>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::MovingBomb_86:
            convert_tlv<relive::Path_MovingBomb_Converter, AO::Path_MovingBomb>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::MovingBombStopper_87:
            convert_tlv<relive::Path_MovingBombStopper_Converter, AO::Path_MovingBombStopper>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::MeatSaw_88:
            convert_tlv<relive::Path_MeatSaw_Converter, AO::Path_MeatSaw>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::MudokonPathTrans_89:
            convert_tlv<relive::Path_MudokonPathTrans_Converter, AO::Path_MudokonPathTrans>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::MainMenuController_90:
            convert_tlv<relive::Path_MainMenuController_Converter, AO::Path_MainMenuController>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::HintFly_92:
            convert_tlv<relive::Path_HintFly_Converter, AO::Path_HintFly>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ScrabNoFall_93:
            convert_tlv<relive::Path_ScrabNoFall_Converter, AO::Path_ScrabNoFall>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::TimerTrigger_94:
            convert_tlv<relive::Path_TimerTrigger_Converter, AO::Path_TimerTrigger>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::SecurityDoor_95:
            convert_tlv<relive::Path_SecurityDoor_Converter, AO::Path_SecurityDoor>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::DemoPlaybackStone_96:
            LOG_WARNING("tlv of type %d is unused", static_cast<s16>(tlv.mTlvType32.mType));
            return;
        case AO::TlvTypes::BoomMachine_97:
            convert_tlv<relive::Path_BoomMachine_Converter, AO::Path_BoomMachine>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::LCDScreen_98:
            convert_tlv<relive::Path_LCDScreen_Converter, AO::Path_LCDScreen>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ElumPathTrans_99:
            convert_tlv<relive::Path_ElumPathTrans_Converter, AO::Path_ElumPathTrans>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::HandStone_100:
            convert_tlv<relive::Path_HandStone_Converter, AO::Path_HandStone>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::CreditsController_101:
            convert_tlv<relive::Path_CreditsController_Converter, AO::Path_CreditsController>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::Preloader_102: // dead tlv
            return;
        case AO::TlvTypes::LCDStatusBoard_103:
            convert_tlv<relive::Path_LCDStatusBoard_Converter, AO::Path_LCDStatusBoard>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::MusicTrigger_105:
            convert_tlv<relive::Path_MusicTrigger_Converter, AO::Path_MusicTrigger>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::LightEffect_106:
            convert_tlv<relive::Path_LightEffect_Converter, AO::Path_LightEffect>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::SlogSpawner_107:
            convert_tlv<relive::Path_SlogSpawner_Converter, AO::Path_SlogSpawner>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::GasCountDown_108:
            convert_tlv<relive::Path_GasCountDown_Converter, AO::Path_GasCountDown>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::RingCancel_109:
            // Special case due to broken OG data
            j.push_back(relive::Path_RingCancel_Converter::From(static_cast<const AO::Path_RingCancel&>(tlv), tlvId, lvlId, pathId));
            break;
        case AO::TlvTypes::GasEmitter_110:
            convert_tlv<relive::Path_GasEmitter_Converter, AO::Path_GasEmitter>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ZzzSpawner_111:
            convert_tlv<relive::Path_ZzzSpawner_Converter, AO::Path_ZzzSpawner>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::BackgroundGlukkon_112:
            convert_tlv<relive::Path_BackgroundGlukkon_Converter, AO::Path_BackgroundGlukkon>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::KillUnsavedMuds_113:
            convert_tlv<relive::Path_KillUnsavedMuds_Converter, AO::Path_KillUnsavedMuds>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::SoftLanding_114:
            convert_tlv<relive::Path_SoftLanding_Converter, AO::Path_SoftLanding>(j, tlv, tlvId);
            break;
        case AO::TlvTypes::ResetPath_115:
            convert_tlv<relive::Path_ResetPath_Converter, AO::Path_ResetPath>(j, tlv, tlvId);
            break;

        default:
            ALIVE_FATAL("TLV conversion for this type not implemented");
    }
}

static bool IsTlvEnd(const AO::Path_TLV* Tlv)
{
    return Tlv->mTlvFlags.Get(AO::TlvFlags::eBit3_End_TLV_List);
}

static bool IsTlvEnd(const ::Path_TLV* Tlv)
{
    return Tlv->mTlvFlags.Get(::TlvFlags::eBit3_End_TLV_List);
}

template <typename TlvType, typename LevelIdType>
static void ConvertPathTLVs(nlohmann::json& j, u32 pathId, LevelIdType levelId, s32 indexTableOffset, s32 objectOffset, const std::vector<u8>& pathResource, u32 indexTableIndex)
{
    const u8* pData = pathResource.data();
    const s32* pIndexTable = reinterpret_cast<const s32*>(pData + indexTableOffset);

    const s32 indexTableValue = pIndexTable[indexTableIndex];
    if (indexTableValue == -1)
    {
        return;
    }

    const u8* pStart = pathResource.data() + (indexTableValue + objectOffset);
   
    auto pPathTLV = reinterpret_cast<const TlvType*>(pStart);
    u32 tlvOffset = indexTableValue + objectOffset;
    while (pPathTLV)
    {
        union TlvInfo
        {
            Guid::TlvOffsetLevelIdPathId mTlvInfo;
            u32 mData;
        };
        TlvInfo tlvInfoUnion;
        tlvInfoUnion.mTlvInfo.tlvOffset = static_cast<u16>(tlvOffset);
        tlvInfoUnion.mTlvInfo.pathId = static_cast<u8>(pathId);
        tlvInfoUnion.mTlvInfo.levelId = static_cast<u8>(levelId);

        // Convert TLV to ReliveTLV
        ConvertTLV(j, *pPathTLV, Guid::NewGuidFromTlvInfo(tlvInfoUnion.mData), levelId, pathId);

        // End of TLVs for given camera
        if (IsTlvEnd(pPathTLV))
        {
            break;
        }

        // Skip length bytes to get to the start of the next TLV
        const u8* ptr = reinterpret_cast<const u8*>(pPathTLV);
        const u8* pNext = ptr + pPathTLV->mLength;
        tlvOffset += pPathTLV->mLength;
        pPathTLV = reinterpret_cast<const TlvType*>(pNext);
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

static void to_json(nlohmann::json& j, const PathSoundInfo& p)
{
    j = nlohmann::json{
        {"vh_file", p.mVhFile},
        {"vb_file", p.mVbFile},
        {"seq_files", p.mSeqFiles},
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


namespace AO { 
extern OpenSeqHandle g_SeqTable_4C9E70[165];
}
extern SeqHandleTable sSeqData;

static void HashBSQTable(OpenSeqHandle* pTable, s32 size)
{
    for (s32 i = 0; i < size; i++)
    {
        if (pTable[i].field_0_mBsqName)
        {
            pTable[i].field_4_generated_res_id = ResourceManager::SEQ_HashName_49BE30(pTable[i].field_0_mBsqName);
        }
    }
}

static const char_type* FileNameFromSEQId(OpenSeqHandle* pTable, s32 size, s32 id)
{
    for (s32 i = 0; i < size; i++)
    {
        if (pTable[i].field_4_generated_res_id == id)
        {
            return pTable[i].field_0_mBsqName;
        }
    }
    ALIVE_FATAL("Unknown SEQ id");
}

static std::vector<std::string> ConvertBSQ(const FileSystem::Path& dataDir, const char_type* pBSQName, ReliveAPI::LvlReader& lvlReader, bool isAo)
{
    auto bsqData = lvlReader.ReadFile(pBSQName);
    if (!bsqData)
    {
        ALIVE_FATAL("Missing BSQ");
    }

    static bool seqNamesHashed = false;
    if (!seqNamesHashed)
    {
        if (isAo)
        {
            HashBSQTable(AO::g_SeqTable_4C9E70, ALIVE_COUNTOF(AO::g_SeqTable_4C9E70));
        }
        else
        {
            HashBSQTable(sSeqData.mSeqs, ALIVE_COUNTOF(sSeqData.mSeqs));
        }
        seqNamesHashed = true;
    }

    std::vector<std::string> seqs;
    ReliveAPI::ChunkedLvlFile bsq(*bsqData);
    for (u32 i = 0u; i < bsq.ChunkCount(); i++)
    {
        const ReliveAPI::LvlFileChunk& chunk = bsq.ChunkAt(i);
        if (chunk.Header().mResourceType == ResourceManagerWrapper::Resource_Seq)
        {
            const char_type* pSeqName = nullptr;
            if (isAo)
            {
                pSeqName = FileNameFromSEQId(AO::g_SeqTable_4C9E70, ALIVE_COUNTOF(AO::g_SeqTable_4C9E70), chunk.Header().field_C_id);
            }
            else
            {
                pSeqName = FileNameFromSEQId(sSeqData.mSeqs, ALIVE_COUNTOF(sSeqData.mSeqs), chunk.Header().field_C_id);
            }
            seqs.push_back(pSeqName);

            FileSystem::Path filePath = dataDir;
            filePath.Append(pSeqName);

            FileSystem fs;
            fs.Save(filePath, chunk.Data());
        }
    }
    return seqs;
}

template <typename LevelIdType>
static void SaveFileFromLvlDirect(const char_type* pFileName, const FileSystem::Path& dataDir, ReliveAPI::LvlReader& lvlReader, LevelIdType lvlIdxAsLvl, std::vector<u8>& fileBuffer)
{
    ReadLvlFileInto(lvlReader, pFileName, fileBuffer);

    FileSystem::Path filePath = dataDir;
    filePath.Append(ToString(lvlIdxAsLvl));
    
    FileSystem fs;
    fs.CreateDirectory(filePath);
    filePath.Append(pFileName);

    fs.Save(filePath, fileBuffer);
}

template <typename TlvType, typename LevelIdType>
static void ConvertPath(FileSystem& fs, const FileSystem::Path& path, const ReliveAPI::LvlFileChunk& pathBndChunk, EReliveLevelIds reliveLvl, LevelIdType lvlIdx, ReliveAPI::LvlReader& lvlReader, std::vector<u8>& fileBuffer, bool isAo)
{
    auto level = (isAo ? ToString(MapWrapper::ToAO(reliveLvl)) : ToString(MapWrapper::ToAE(reliveLvl)));
    LOG_INFO("Converting: %s; path %d", level, pathBndChunk.Id());

    s32 width = 0;
    s32 height = 0;
    CollisionInfo* pCollisionInfo = nullptr;
    s32 indexTableOffset = 0;
    s32 objectOffset = 0;

    if (isAo)
    {
        const AO::PathBlyRec* pBlyRec = AO::Path_Get_Bly_Record(reliveLvl, static_cast<u16>(pathBndChunk.Id()));

        // Save cameras and map objects
        width = (pBlyRec->field_4_pPathData->field_8_bTop - pBlyRec->field_4_pPathData->field_4_bLeft) / pBlyRec->field_4_pPathData->field_C_grid_width;
        height = (pBlyRec->field_4_pPathData->field_A_bBottom - pBlyRec->field_4_pPathData->field_6_bRight) / pBlyRec->field_4_pPathData->field_E_grid_height;
        pCollisionInfo = pBlyRec->field_8_pCollisionData;
        indexTableOffset = pBlyRec->field_4_pPathData->field_18_object_index_table_offset;
        objectOffset = pBlyRec->field_4_pPathData->field_14_object_offset;
    }
    else
    {
        const ::PathBlyRec* pBlyRec = ::Path_Get_Bly_Record(reliveLvl, static_cast<u16>(pathBndChunk.Id()));

        // Save cameras and map objects
        width = (pBlyRec->field_4_pPathData->field_4_bTop - pBlyRec->field_4_pPathData->field_0_bLeft) / pBlyRec->field_4_pPathData->field_A_grid_width;
        height = (pBlyRec->field_4_pPathData->field_6_bBottom - pBlyRec->field_4_pPathData->field_2_bRight) / pBlyRec->field_4_pPathData->field_C_grid_height;
        pCollisionInfo = pBlyRec->field_8_pCollisionData;
        indexTableOffset = pBlyRec->field_4_pPathData->field_16_object_indextable_offset;
        objectOffset = pBlyRec->field_4_pPathData->field_12_object_offset;
    }

    nlohmann::json camerasArray = nlohmann::json::array();
    PathCamerasEnumerator cameraEnumerator(width, height, pathBndChunk.Data());
    cameraEnumerator.Enumerate([&](const CameraEntry& tmpCamera)
                               {
        // Save map objects
        nlohmann::json mapObjectsArray = nlohmann::json::array();
        const u32 indexTableIdx = To1dIndex(width, tmpCamera.mX, tmpCamera.mY);
        //LOG_INFO(indexTableIdx);
        ConvertPathTLVs<TlvType>(mapObjectsArray, pathBndChunk.Id(), lvlIdx, indexTableOffset, objectOffset, pathBndChunk.Data(), indexTableIdx);

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
    ConvertPathCollisions(collisionsArray, *pCollisionInfo, pathBndChunk.Data(), isAo);

    FileSystem::Path seqsDir = path;
    seqsDir.Append(ToString(lvlIdx));
    fs.CreateDirectory(seqsDir);

    // Save sound info (per path rather than per LVL)
    PathSoundInfo soundInfo;
    if (isAo)
    {
        const AO::SoundBlockInfo* pSoundBlock = AO::Path_Get_MusicInfo(reliveLvl);
        soundInfo.mVhFile = pSoundBlock->field_0_vab_header_name;
        soundInfo.mVbFile = pSoundBlock->field_4_vab_body_name;
        soundInfo.mSeqFiles = ConvertBSQ(seqsDir, AO::Path_Get_BsqFileName(reliveLvl), lvlReader, isAo);
    }
    else
    {
        const SoundBlockInfo* pSoundBlock = Path_Get_MusicInfo(reliveLvl);

        // TODO: Convert to AO format instead of using sounds.dat for now (in the vh/vb/bsq copy)
        soundInfo.mVhFile = pSoundBlock->field_0_vab_header_name;
        soundInfo.mVbFile = pSoundBlock->field_4_vab_body_name;

        soundInfo.mSeqFiles = ConvertBSQ(seqsDir, Path_Get_BsqFileName(reliveLvl), lvlReader, isAo);

        // TODO
        //Path_Get_BackGroundMusicId(reliveLvl);
        //Path_Get_Reverb(reliveLvl);

        // TODO: Makes more sense to calculate this on loading by summing up the mud count(s)?
        //Path_GetMudsInLevel(reliveLvl, pathBndChunk.Id());
    }

    SaveFileFromLvlDirect(soundInfo.mVhFile.c_str(), path, lvlReader, lvlIdx, fileBuffer);
    SaveFileFromLvlDirect(soundInfo.mVbFile.c_str(), path, lvlReader, lvlIdx, fileBuffer);

    nlohmann::json j = {
        {"id", pathBndChunk.Id()}, // TODO: shouldn't be required once everything is converted and not using path id numbers
        {"cameras", camerasArray},
        {"collisions", collisionsArray},
        {"sound_info", soundInfo},
    };

    FileSystem::Path pathJsonFile = path;
    pathJsonFile.Append(ToString(lvlIdx)).Append("paths");
    fs.CreateDirectory(pathJsonFile);
    pathJsonFile.Append(std::to_string(pathBndChunk.Header().field_C_id) + ".json");
    SaveJson(j, fs, pathJsonFile);
}


template <typename LevelIdType>
static void SaveLevelInfoJson(const FileSystem::Path& dataDir, EReliveLevelIds /*reliveLvl*/, LevelIdType lvlIdxAsLvl, FileSystem& fs, const ReliveAPI::ChunkedLvlFile& pathBndFile)
{
    FileSystem::Path pathDir = dataDir;
    pathDir.Append(ToString(lvlIdxAsLvl)).Append("paths");
    fs.CreateDirectory(pathDir);

    FileSystem::Path pathJsonFile = pathDir;
    pathJsonFile.Append("level_info.json");

    nlohmann::json jsonPathFilesArray;

    // TODO: Handle conversion of path extension blocks from custom lvls

    /*
    // AO data
 if (pHeader->mLineType == Resource_Pxtd)
                    {
                        auto pChunkData = reinterpret_cast<u8*>(&pHeader[1]);
                        auto pExt = reinterpret_cast<const PerPathExtension*>(pChunkData);
                        if (pExt->mSize != sizeof(PerPathExtension))
                        {
                            LOG_INFO("Skip " << pExt->mBlyName << " expected size " << sizeof(PerPathExtension) << " but got " << pExt->mSize);
                            pChunkData += pExt->mSize;
                        }
                        else
                        {
                            LOG_INFO("Applying " << pExt->mBlyName);

                            pChunkData += sizeof(PerPathExtension);

                            // Apply LCD Screen messages
                            auto pLCDScreenMsgs = reinterpret_cast<StringTable*>(pChunkData);
                            pChunkData = StringTable::MakeTable(pLCDScreenMsgs);
                            SetLcdMessagesForLvl(*pLCDScreenMsgs, static_cast<LevelIds>(lvlIdx), pExt->mPathId);

                            // Apply hint fly messages
                            auto pHintFlyMsgs = reinterpret_cast<StringTable*>(pChunkData);
                            pChunkData = StringTable::MakeTable(pHintFlyMsgs);
                            SetHintFlyMessagesForLvl(*pHintFlyMsgs, static_cast<LevelIds>(lvlIdx), pExt->mPathId);

                            // Apply the data
                            PathRoot& rPath = *Path_Get_PathRoot(lvlIdx);
                            rPath.field_0_pBlyArrayPtr[pExt->mPathId].field_0_blyName = pExt->mBlyName;

                            PathBlyRec& rBlyRec = rPath.field_0_pBlyArrayPtr[pExt->mPathId];
                            if (!rBlyRec.field_4_pPathData)
                            {
                                rBlyRec.field_4_pPathData = &GetPathData(lvlIdx)[pExt->mPathId];
                                // HACK: Set throwable type in this path to grenades
                                rBlyRec.mOverlayId = 3;
                                rPath.field_18_num_paths++;
                            }
                            PathData& rPathData = *rBlyRec.field_4_pPathData;

                            rPathData.field_0 = sub_402560;
                            rPathData.field_4_bLeft = 0;
                            rPathData.field_6_bRight = 0;
                            SetAndLog("top", rPathData.field_8_bTop, static_cast<s16>(pExt->mXSize * pExt->mGridWidth));
                            SetAndLog("bottom", rPathData.field_A_bBottom, static_cast<s16>(pExt->mYSize * pExt->mGridHeight));

                            SetAndLog("grid width", rPathData.field_C_grid_width, static_cast<s16>(pExt->mGridWidth));
                            SetAndLog("grid height", rPathData.field_E_grid_height, static_cast<s16>(pExt->mGridHeight));

                            SetAndLog("field_10", rPathData.field_10, static_cast<s16>(pExt->mGridWidth));
                            SetAndLog("field_12", rPathData.field_12, static_cast<s16>(pExt->mGridHeight));

                            SetAndLog<s32>("object offset", rPathData.field_14_object_offset, pExt->mObjectOffset);
                            SetAndLog<s32>("index table offset", rPathData.field_18_object_index_table_offset, pExt->mIndexTableOffset);

                           // rPathData.field_1C_object_funcs = kObjectFactory;

                            rBlyRec.field_8_pCollisionData = &GetCollisions(lvlIdx)[pExt->mPathId];

                            CollisionInfo& rColInfo = *rBlyRec.field_8_pCollisionData;
                            rColInfo.field_4_left = 0;
                            rColInfo.field_6_right = 0;
                            SetAndLog("top", rColInfo.field_8_top, static_cast<s16>(pExt->mXSize * pExt->mGridWidth));
                            SetAndLog("bottom", rColInfo.field_A_bottom, static_cast<s16>(pExt->mYSize * pExt->mGridHeight));
                            SetAndLog("collision offset", rColInfo.field_C_collision_offset, pExt->mCollisionOffset);
                            SetAndLog("num collision items", rColInfo.field_10_num_collision_items, pExt->mNumCollisionLines);
                            SetAndLog<u32>("grid width", rColInfo.field_14_grid_width, pExt->mGridWidth);
                            SetAndLog<u32>("grid height", rColInfo.field_18_grid_height, pExt->mGridHeight);

                            if (pExt->mTotalMuds != 0)
                            {
                                SetAndLog("sTotalMuds", sMudExtData[lvlIdx][pExt->mPathId].mTotal, pExt->mTotalMuds);
                            }

                            if (pExt->mBadEndingMuds != 0)
                            {
                                SetAndLog("sBadEndingMuds", sMudExtData[lvlIdx][pExt->mPathId].mBadEnding, pExt->mBadEndingMuds);
                            }

                            if (pExt->mGoodEndingMuds)
                            {
                                SetAndLog("sGoodEndingMuds", sMudExtData[lvlIdx][pExt->mPathId].mGoodEnding, pExt->mGoodEndingMuds);
                            }
                        }
                    }
    */

    /*
     // AE data
     if (pHeader->mResourceType == Resource_Pxtd)
                    {
                        auto pChunkData = reinterpret_cast<u8*>(&pHeader[1]);
                        auto pExt = reinterpret_cast<const PerPathExtension*>(pChunkData);
                        if (pExt->mSize != sizeof(PerPathExtension))
                        {
                            LOG_INFO("Skip " << pExt->mBlyName << " expected size " << sizeof(PerPathExtension) << " but got " << pExt->mSize);
                            pChunkData += pExt->mSize;
                        }
                        else
                        {
                            LOG_INFO("Applying " << pExt->mBlyName);

                            pChunkData += sizeof(PerPathExtension);

                            // Apply LCD Screen messages
                            auto pLCDScreenMsgs = reinterpret_cast<StringTable*>(pChunkData);
                            pChunkData = StringTable::MakeTable(pLCDScreenMsgs);
                            SetLcdMessagesForLvl(*pLCDScreenMsgs, static_cast<LevelIds>(lvlIdx), pExt->mPathId);

                            // Will be empty for AE
                            auto pHintFlyMsgs = reinterpret_cast<StringTable*>(pChunkData);
                            pChunkData = StringTable::MakeTable(pHintFlyMsgs);

                            // Apply the data
                            PathRoot& rPath = *Path_Get_PathRoot(lvlIdx);
                            rPath.field_0_pBlyArrayPtr[pExt->mPathId].field_0_blyName = pExt->mBlyName;

                            PathBlyRec& rBlyRec = rPath.field_0_pBlyArrayPtr[pExt->mPathId];
                            if (!rBlyRec.field_4_pPathData)
                            {
                                rBlyRec.field_4_pPathData = &GetPathData(lvlIdx)[pExt->mPathId];
                                // HACK: Set throwable type in this path to grenades
                                rBlyRec.mOverlayId = 100;
                                rPath.field_18_num_paths++;
                            }
                            PathData& rPathData = *rBlyRec.field_4_pPathData;

                            rPathData.field_0_bLeft = 0;
                            rPathData.field_2_bRight = 0;
                            SetAndLog("top", rPathData.field_4_bTop, static_cast<s16>(pExt->mXSize * pExt->mGridWidth));
                            SetAndLog("bottom", rPathData.field_6_bBottom, static_cast<s16>(pExt->mYSize * pExt->mGridHeight));

                            SetAndLog("grid width", rPathData.field_A_grid_width, static_cast<s16>(pExt->mGridWidth));
                            SetAndLog("grid height", rPathData.field_C_grid_height, static_cast<s16>(pExt->mGridHeight));

                            SetAndLog("width", rPathData.field_E_width, static_cast<s16>(pExt->mGridWidth));
                            SetAndLog("height", rPathData.field_10_height, static_cast<s16>(pExt->mGridHeight));

                            SetAndLog("object offset", rPathData.field_12_object_offset, pExt->mObjectOffset);
                            SetAndLog("index table offset", rPathData.field_16_object_indextable_offset, pExt->mIndexTableOffset);

                            SetAndLog("abe start xpos", rPathData.field_1A_abe_start_xpos, static_cast<s16>(pExt->mAbeStartXPos));
                            SetAndLog("abe start ypos", rPathData.field_1C_abe_start_ypos, static_cast<s16>(pExt->mAbeStartYPos));

                            rBlyRec.field_8_pCollisionData = &GetCollisions(lvlIdx)[pExt->mPathId];

                            CollisionInfo& rColInfo = *rBlyRec.field_8_pCollisionData;
                            rColInfo.field_4_left = 0;
                            rColInfo.field_6_right = 0;
                            SetAndLog("top", rColInfo.field_8_top, static_cast<s16>(pExt->mXSize * pExt->mGridWidth));
                            SetAndLog("bottom", rColInfo.field_A_bottom, static_cast<s16>(pExt->mYSize * pExt->mGridHeight));
                            SetAndLog("collision offset", rColInfo.field_C_collision_offset, pExt->mCollisionOffset);
                            SetAndLog("num collision items", rColInfo.field_10_num_collision_items, pExt->mNumCollisionLines);
                            SetAndLog<u32>("grid width", rColInfo.field_14_grid_width, pExt->mGridWidth);
                            SetAndLog<u32>("grid height", rColInfo.field_18_grid_height, pExt->mGridHeight);

                            if (pExt->mTotalMuds != 0)
                            {
                                SetAndLog("sTotalMuds", sMudExtData[lvlIdx][pExt->mPathId].mTotal, pExt->mTotalMuds);
                            }

                            if (pExt->mBadEndingMuds != 0)
                            {
                                SetAndLog("sBadEndingMuds", sMudExtData[lvlIdx][pExt->mPathId].mBadEnding, pExt->mBadEndingMuds);
                            }

                            if (pExt->mGoodEndingMuds)
                            {
                                SetAndLog("sGoodEndingMuds", sMudExtData[lvlIdx][pExt->mPathId].mGoodEnding, pExt->mGoodEndingMuds);
                            }

                            if (pExt->mNumMudsInPath != 0)
                            {
                                if (pExt->mNumMudsInPath != Path_GetMudsInLevel(MapWrapper::FromAE(static_cast<LevelIds>(lvlIdx)), pExt->mPathId))
                                {
                                    LOG_INFO("Set muds in lvl count to " << pExt->mNumMudsInPath);
                                    Path_SetMudsInLevel(MapWrapper::FromAE(static_cast<LevelIds>(lvlIdx)), pExt->mPathId, pExt->mNumMudsInPath);
                                }
                            }
                        }
                    }

    */


    // Convert hard coded path data json
    for (u32 j = 0; j < pathBndFile.ChunkCount(); j++)
    {
        const ReliveAPI::LvlFileChunk& pathBndChunk = pathBndFile.ChunkAt(j);
        if (pathBndChunk.Header().mResourceType == ResourceManagerWrapper::Resource_Path)
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

extern PalRecConversionInfo kPalConversionInfo[17];

static void ConvertPal(const FileSystem::Path& dataDir, const char* pFileName, const u16* pData, u32 len);

static void LogNonConvertedPals(bool isAo)
{
    for (auto& rec : kPalConversionInfo)
    {
        if (!rec.mConverted)
        {
            const auto palDetails = isAo ? AO::PalRec(rec.mPalId) : PalRec(rec.mPalId);
            if (palDetails.mBanName)
            {
                LOG_INFO("MISSING PAL: %s", ToString(rec.mPalId));
            }
        }
    }
}

static void ConvertPals(const FileSystem::Path& dataDir, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, bool isAo)
{
    for (auto& rec : kPalConversionInfo)
    {
        if (!rec.mConverted)
        {
            const auto palDetails = isAo ? AO::PalRec(rec.mPalId) : PalRec(rec.mPalId);
            if (palDetails.mBanName)
            {
                // Not every file is in every LVL - we might get it from a later LVL
                if (ReadLvlFileInto(lvlReader, palDetails.mBanName, fileBuffer))
                {
                    // A BAN/BND can have multiple chunks, make sure we pick the right one
                    ReliveAPI::ChunkedLvlFile palFile(fileBuffer);
                    for (u32 i = 0; i < palFile.ChunkCount(); i++)
                    {
                        const auto& res = palFile.ChunkAt(i);
                        if (res.Header().mResourceType == ResourceManagerWrapper::Resource_Palt && static_cast<s32>(res.Header().field_C_id) == palDetails.mResourceId)
                        {
                            LOG_INFO("Converting PAL: %s", ToString(rec.mPalId));

                            const auto& palData = res.Data();
                            u32 palLen = *reinterpret_cast<const u32*>(palData.data());
                            const u16* pPalData = reinterpret_cast<const u16*>(palData.data()) + 2;

                            ConvertPal(dataDir, ToString(rec.mPalId), pPalData, palLen);
                            rec.mConverted = true;
                        }
                    }
                }
            }
        }
    }
}


static void ConvertAnimations(const FileSystem::Path& dataDir, FileSystem& fs, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, bool isAo)
{
    // Convert animations that exist in this LVL
    for (auto& rec : kAnimRecConversionInfo)
    {
        const auto animDetails = isAo ? AO::AnimRec(rec.mAnimId) : AnimRec(rec.mAnimId);

        // Skip animations that are only present in AO or AE
        if ((isAo && rec.mAoLvl == EReliveLevelIds::eNone) || (!isAo && rec.mAeLvl == EReliveLevelIds::eNone))
        {
            continue;
        }

        // Animation is in this LVL and not yet converted
        if (!rec.mConverted)
        {
            if ((isAo && rec.mAoLvl == reliveLvl) || (!isAo && rec.mAeLvl == reliveLvl))
            {
                // Not every file is in every LVL - we might get it from a later LVL
                if (ReadLvlFileInto(lvlReader, animDetails.mBanName, fileBuffer))
                {
                    // A BAN/BND can have multiple chunks, make sure we pick the right one
                    ReliveAPI::ChunkedLvlFile animFile(fileBuffer);
                    auto res = animFile.ChunkById(animDetails.mResourceId);
                    if (res)
                    {
                        const char_type* animName = AnimRecName(rec.mAnimId);
                        const char_type* groupName = AnimRecGroupName(rec.mAnimId);

                        LOG_INFO("Converting: %s", animName);

                        FileSystem::Path filePath = dataDir;
                        filePath.Append("animations");

                        // e.g "abe"
                        filePath.Append(groupName);

                        // Ensure the containing directory exists
                        fs.CreateDirectory(filePath);

                        // e.g "arm_gib"
                        filePath.Append(animName);

                        AnimationConverter animationConverter(filePath, animDetails, res->Data(), isAo);

                        // Track what is converted so we know what is missing at the end
                        rec.mConverted = true;
                    }
                }
            }
        }
    }
}

static void LogNonConvertedAnims(bool isAo)
{
    // Log skippeed/missing anims
    for (auto& rec : kAnimRecConversionInfo)
    {
        const auto animDetails = isAo ? AO::AnimRec(rec.mAnimId) : AnimRec(rec.mAnimId);
        if (animDetails.mFrameTableOffset != 0)
        {
            if (!rec.mConverted)
            {
                LOG_INFO("Didn't convert %s", AnimRecName(rec.mAnimId));
            }
        }
    }
}

template <typename LevelIdType, typename TlvType>
static void ConvertPathBND(const FileSystem::Path& dataDir, const std::string& fileName, FileSystem& fs, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, LevelIdType lvlIdxAsLvl, EReliveLevelIds reliveLvl, bool isAo)
{
    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);
    ReliveAPI::ChunkedLvlFile pathBndFile(fileBuffer);
    for (u32 j = 0; j < pathBndFile.ChunkCount(); j++)
    {
        const ReliveAPI::LvlFileChunk& pathBndChunk = pathBndFile.ChunkAt(j);
        if (pathBndChunk.Header().mResourceType == ResourceManagerWrapper::Resource_Path)
        {
            ConvertPath<TlvType, LevelIdType>(fs, dataDir, pathBndChunk, reliveLvl, lvlIdxAsLvl, lvlReader, fileBuffer, isAo);
        }
    }

    SaveLevelInfoJson(dataDir, reliveLvl, lvlIdxAsLvl, fs, pathBndFile);
}

/*static*/ void SaveCameraJsonManifest(const std::string& baseName, ReliveAPI::ApiFG1Reader& reader, const FileSystem::Path& dataDir, u32 fg1ResBlockCount)
{
    nlohmann::json camManifest;
    nlohmann::json layersArray;
    for (u32 i = 0; i < 4; i++)
    {
        if (reader.LayerUsed(i))
        {
            layersArray.push_back(baseName + reader.NameForLayer(i) + ".png");
        }
    }
    camManifest["layers"] = layersArray;
    if (layersArray.empty())
    {
        // The FG1 blocks result in no actual FG1 data, add yet another fake/dummy
        // block so the FG1 object count matches OG for playbacks
        fg1ResBlockCount++;
    }
    camManifest["fg1_block_count"] = fg1ResBlockCount;

    FileSystem fs;
    SaveJson(camManifest, fs, dataDir);
}

template <typename LevelIdType>
static void ConvertCamera(const FileSystem::Path& dataDir, const std::string& fileName, FileSystem& fs, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, LevelIdType lvlIdxAsLvl)
{
    LOG_INFO("%s", fileName.c_str());
    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);

    ReliveAPI::ChunkedLvlFile camFile(fileBuffer);

    std::string camBaseName = fileName.substr(0, fileName.length() - 4); // chop off .CAM
    camBaseName = camBaseName.substr(2); // Chop off the 2 letter lvl prefix

    FileSystem::Path pathDir = dataDir;

    pathDir.Append(ToString(lvlIdxAsLvl));
    fs.CreateDirectory(pathDir);

    FileSystem::Path jsonFileName = pathDir;
    jsonFileName.Append(camBaseName + ".json");

    pathDir.Append(camBaseName); 

    // Convert camera images and FG layers
    ReliveAPI::CamConverter cc;
    auto fg1ReaderAndBlockCount = cc.Convert(camFile, pathDir.GetPath());
    if (fg1ReaderAndBlockCount.first)
    {
        SaveCameraJsonManifest(camBaseName, *fg1ReaderAndBlockCount.first, jsonFileName, fg1ReaderAndBlockCount.second);
    }

    // TODO: Convert any BgAnims in this camera
}

static void ConvertFont(const FileSystem::Path& dataDir, const std::string& fileName, ReliveAPI::LvlReader& lvlReader, std::vector<u8>& fileBuffer, bool isPauseMenuFont)
{
    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);

    ReliveAPI::ChunkedLvlFile camFile(fileBuffer);

     std::optional<ReliveAPI::LvlFileChunk> font = camFile.ChunkByType(ResourceManager::Resource_Font);
     if (!font)
     {
         ALIVE_FATAL("Font missing");
     }

     auto fontFile = reinterpret_cast<const File_Font*>(font->Data().data());

     FileSystem::Path path = dataDir;
     if (isPauseMenuFont)
     {
         path.Append("pause_menu_font.png");
     }
     else
     {
         path.Append("lcd_font.png");
     }
     PNGFile png;
     AnimationPal pal = {};

     for (s32 i = 0; i < 16; i++)
     {
         RGBA32 pixel = RGBConversion::RGBA555ToRGBA888Components(fontFile->mPalette[i]);

         pal.mPal[i].r = pixel.r;
         pal.mPal[i].g = pixel.g;
         pal.mPal[i].b = pixel.b;
         pal.mPal[i].a = pixel.a;
     }

     std::vector<u8> newData((fontFile->mWidth / 2) * fontFile->mHeight * 2);

     // Expand 4bit to 8bit
     std::size_t src = 0;
     std::size_t dst = 0;
     while (dst < newData.size())
     {
         newData[dst++] = (fontFile->mPixelBuffer[src] & 0xF);
         newData[dst++] = ((fontFile->mPixelBuffer[src++] & 0xF0) >> 4);
     }

     if (!isPauseMenuFont)
     {
         // Make it a square texture
         std::vector<u8> newDataTmp(256 * 256);
         for (s32 x = 0; x < fontFile->mWidth; x++)
         {
             for (s32 y = 0; y < fontFile->mHeight; y++)
             {
                 const u32 srcIdx = (y * fontFile->mWidth) + x;
                 const u32 dstIdx = (y * 256) + x;
                 newDataTmp[dstIdx] = newData[srcIdx];
             }
         }

         png.Save(path.GetPath().c_str(), pal, newDataTmp, 256, 256);
     }
     else
     {
         png.Save(path.GetPath().c_str(), pal, newData, fontFile->mWidth, fontFile->mHeight);
     }

     // TODO: Dump out the atlas for each char
}

template<typename LevelIdType, typename TlvType>
static void ConvertFilesInLvl(const FileSystem::Path& dataDir, FileSystem& fs, ReliveAPI::LvlReader& lvlReader, std::vector<u8>& fileBuffer, LevelIdType lvlIdxAsLvl, EReliveLevelIds reliveLvl, bool isAo, bool onlySaves)
{
    // Iterate and convert specific file types in the LVL
    for (s32 i = 0; i < lvlReader.FileCount(); i++)
    {
        auto fileName = lvlReader.FileNameAt(i);
        if (!fileName.empty())
        {
            if (onlySaves)
            {
                if (endsWith(fileName, ".SAV"))
                {
                    if (ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer))
                    {
                        if (!isAo)
                        {
                            // Remove the resource header
                            fileBuffer.erase(fileBuffer.begin(), fileBuffer.begin() + 16);

                            // TODO: Actually convert at some later point
                            AESaveConverter saveConverter;
                            saveConverter.Convert(fileBuffer, (fileName + ".json").c_str());
                        }
                    }
                }
            }
            else
            {
                if (endsWith(fileName, ".FNT"))
                {
                    if (fileName == "LCDFONT.FNT")
                    {
                        ConvertFont(dataDir, fileName, lvlReader, fileBuffer, false);
                    }
                }
                else if (endsWith(fileName, ".CAM"))
                {
                    if (fileName == "S1P01C01.CAM" || fileName == "STP01C06.CAM")
                    {
                        ConvertFont(dataDir, fileName, lvlReader, fileBuffer, true);
                    }

                     ConvertCamera(dataDir, fileName, fs, fileBuffer, lvlReader, lvlIdxAsLvl);
                }
                else if (endsWith(fileName, ".JOY"))
                {
                    // TODO: Actually convert at some later point
                    SaveFileFromLvlDirect(fileName.c_str(), dataDir, lvlReader, lvlIdxAsLvl, fileBuffer);
                }
                else if (endsWith(fileName, "PATH.BND"))
                {
                    ConvertPathBND<LevelIdType, TlvType>(dataDir, fileName, fs, fileBuffer, lvlReader, lvlIdxAsLvl, reliveLvl, isAo);
                }
            }
        }
    }
}


static void SavePal(const AnimationPal& pal, const FileSystem::Path& fileName)
{
    AutoFILE f;
    f.Open(fileName.GetPath().c_str(), "wb", false);
    f.Write(pal.mPal);
}

static void ConvertPal(const FileSystem::Path& dataDir, const char* pFileName, const u16* pData, u32 len)
{
    FileSystem::Path palFilePath = dataDir;
    palFilePath.Append(pFileName);

    AnimationPal pal;
    for (u32 i = 0; i < len; i++)
    {
        RGBA32 pixel = RGBConversion::RGBA555ToRGBA888Components(pData[i]);

        pal.mPal[i].r = pixel.r;
        pal.mPal[i].g = pixel.g;
        pal.mPal[i].b = pixel.b;
        pal.mPal[i].a = pixel.a;
    }
    SavePal(pal, palFilePath);
}

static void ConvertHardcodedPals(const FileSystem::Path& dataDir)
{
    const static u8 mainMenuFontPal[] = {
        0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
        0xA5, 0x94, 0xE7, 0x9C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
        0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
        0x73, 0x4E};

    const static u8 pauseMenuFontPal[] = {
        0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
        0xA5, 0x14, 0xE7, 0x1C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
        0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
        0x73, 0x4E};

    const static u8 pal_ColourfulMeter[32] = {
        0u, 0u, 1u, 128u, 1u, 132u, 32u, 132u, 33u,
        128u, 32u, 132u, 33u, 132u, 101u, 206u, 101u, 140u,
        140u, 177u, 19u, 148u, 100u, 206u, 101u, 206u, 215u,
        152u, 20u, 161u, 24u, 216u};

    const static u8 sLCDScreen_Palette[] = {
        0x00, 0x00, 0x01, 0x80, 0x01, 0x84, 0x20, 0x84, 0x21, 0x80,
        0x20, 0x84, 0x21, 0x84, 0x65, 0xCE, 0x65, 0x8C, 0x8C, 0xB1,
        0x60, 0x8E, 0x64, 0xCE, 0x65, 0xCE, 0xD7, 0x98, 0x14, 0xA1,
        0x18, 0xD8};

    const static u8 sLCDScreen_Palette2[] = {
        0x00, 0x00, 0x01, 0x80, 0x01, 0x84, 0x20, 0x84, 0x21, 0x80,
        0x20, 0x84, 0x21, 0x84, 0x05, 0x84, 0x65, 0x8C, 0x8C, 0xB1,
        0x13, 0x94, 0x64, 0xCE, 0x65, 0xCE, 0xD7, 0x98, 0x14, 0xA1,
        0x18, 0xD8};

    const static u8 pal_LCDStatusBoard[] = {
        0x00, 0x00, 0x01, 0x80, 0x01, 0x84, 0x20, 0x84, 0x21, 0x80,
        0x20, 0x84, 0x21, 0x84, 0x65, 0xCE, 0x65, 0x8C, 0x8C, 0xB1,
        0x13, 0x94, 0x64, 0xCE, 0x65, 0xCE, 0xD7, 0x98, 0x14, 0xA1,
        0x18, 0xD8};


    ConvertPal(dataDir, ToString(PalId::MainMenuFont_MainMenu), reinterpret_cast<const u16*>(mainMenuFontPal), ALIVE_COUNTOF(mainMenuFontPal) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::MainMenuFont_PauseMenu), reinterpret_cast<const u16*>(pauseMenuFontPal), ALIVE_COUNTOF(pauseMenuFontPal) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::LedFont_ColourfulMeter), reinterpret_cast<const u16*>(pal_ColourfulMeter), ALIVE_COUNTOF(pal_ColourfulMeter) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::LedFont_1), reinterpret_cast<const u16*>(sLCDScreen_Palette), ALIVE_COUNTOF(sLCDScreen_Palette) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::LedFont_2), reinterpret_cast<const u16*>(sLCDScreen_Palette2), ALIVE_COUNTOF(sLCDScreen_Palette2) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::LedFont_Red), reinterpret_cast<const u16*>(pal_LCDStatusBoard), ALIVE_COUNTOF(pal_LCDStatusBoard) / sizeof(u16));
}

template<typename FnOnLvl>
static void IterateAELvls(FnOnLvl fnOnLvl)
{
    for (s32 lvlIdx = 0; lvlIdx < Path_Get_Paths_Count(); lvlIdx++)
    {
        // Skip entries that have no data
        const ::LevelIds lvlIdxAsLvl = static_cast<::LevelIds>(lvlIdx);
        if (lvlIdxAsLvl == ::LevelIds::eTestLevel_15)
        {
            continue;
        }

        // Open the LVL file
        const EReliveLevelIds reliveLvl = MapWrapper::FromAE(lvlIdxAsLvl);
        ReliveAPI::FileIO fileIo;

        auto lvName = std::string(::Path_Get_Lvl_Name(reliveLvl)) + ".LVL";
        ReliveAPI::LvlReader lvlReader(fileIo, lvName.c_str());

        if (!lvlReader.IsOpen())
        {
            // Fatal, missing LVL file
            ALIVE_FATAL("Couldn't open lvl file");
        }
        fnOnLvl(lvlReader, reliveLvl, lvlIdxAsLvl);
    }
}

template<typename FnOnLvl>
static void IterateAOLvls(FnOnLvl fnOnLvl)
{
    for (s32 lvlIdx = 0; lvlIdx < AO::Path_Get_Paths_Count(); lvlIdx++)
    {
        // Skip entries that have no data
        const AO::LevelIds lvlIdxAsLvl = static_cast<AO::LevelIds>(lvlIdx);
        if (lvlIdxAsLvl == AO::LevelIds::eRemoved_7 || lvlIdxAsLvl == AO::LevelIds::eRemoved_11)
        {
            continue;
        }

        const EReliveLevelIds reliveLvl = MapWrapper::FromAO(lvlIdxAsLvl);
        ReliveAPI::FileIO fileIo;
        ReliveAPI::LvlReader lvlReader(fileIo, (std::string(AO::Path_Get_Lvl_Name(reliveLvl)) + ".LVL").c_str());

        if (!lvlReader.IsOpen())
        {
            // Fatal, missing LVL file
            ALIVE_FATAL("Couldn't open lvl file");
        }

        fnOnLvl(lvlReader, reliveLvl, lvlIdxAsLvl);
    }
}

const char_type kDataVersionFileName[] = "data_version.json";

static void WriteDataVersion(const FileSystem::Path& path, u32 version)
{
    auto j = nlohmann::json{{"data_version", version}};
    FileSystem fs;
    auto fileName = path;
    fileName.Append(kDataVersionFileName);
    SaveJson(j, fs, fileName);
}

static u32 data_version_from_json(const nlohmann::json& j)
{
    u32 version = 0;
    j.at("data_version").get_to(version);
    return version;
}

static u32 data_version_from_path(const FileSystem::Path& dataDir)
{
    auto path = dataDir;
    path.Append(kDataVersionFileName);

    FileSystem fs;
    if (fs.FileExists(path.GetPath().c_str()))
    {
        const std::string jsonStr = fs.LoadToString(path);
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return data_version_from_json(j);
    }
    return 0;
}

u32 DataConversion::DataVersionAO()
{
    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ao");
    return data_version_from_path(dataDir);
}

u32 DataConversion::DataVersionAE()
{
    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ae");
    return data_version_from_path(dataDir);
}

void DataConversion::ConvertDataAO()
{
    FileSystem fs;

    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ao");
    fs.CreateDirectory(dataDir);

    ConvertFMVs(dataDir, true);

    // TODO: Prob diff data in AO, check me
    ConvertHardcodedPals(dataDir);

    std::vector<u8> fileBuffer;
    IterateAOLvls([&](ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, AO::LevelIds lvlIdxAsLvl)
    {
        ConvertAnimations(dataDir, fs, fileBuffer, lvlReader, reliveLvl, true);

        ConvertPals(dataDir, fileBuffer, lvlReader, true);

        ConvertFilesInLvl<AO::LevelIds, AO::Path_TLV>(dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, true, false);
    });

    IterateAOLvls([&](ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, AO::LevelIds lvlIdxAsLvl)
    {
        ConvertFilesInLvl<AO::LevelIds, AO::Path_TLV>(dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, true, true);
    });

    WriteDataVersion(dataDir, DataConversion::kVersion);

    LogNonConvertedAnims(true);
    LogNonConvertedPals(true);
}

void DataConversion::ConvertDataAE()
{
    FileSystem fs;

    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ae");
    fs.CreateDirectory(dataDir);

    ConvertFMVs(dataDir, false);
    ConvertHardcodedPals(dataDir);

    std::vector<u8> fileBuffer;
    IterateAELvls([&](ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, LevelIds lvlIdxAsLvl) 
    {
        ConvertAnimations(dataDir, fs, fileBuffer, lvlReader, reliveLvl, false);

        ConvertPals(dataDir, fileBuffer, lvlReader, false);

        ConvertFilesInLvl<::LevelIds, ::Path_TLV>(dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, false, false);
    });

    IterateAELvls([&](ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, LevelIds lvlIdxAsLvl) 
    { 
        ConvertFilesInLvl<::LevelIds, ::Path_TLV>(dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, false, true);
    });

    WriteDataVersion(dataDir, DataConversion::kVersion);

    LogNonConvertedAnims(false);
}
