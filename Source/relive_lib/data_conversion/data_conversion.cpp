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
#include "Collisions.hpp"

constexpr u32 kDataVersion = 1;

extern const CombinedAnimRecord kAnimRecords[915];
extern const AnimDetails kNullAnimDetails;



enum class EAnimGroup
{
    Ungrouped,
    Gibs,
    Ropes,
    Throwables,
    Abe,
    Lights,
    Bat,
    Fleech,
    FlyingSlig,
    Glukkons,
    Greeter,
    MineCar,
    Paramite,
    Scrab,
    CrawlingSlig,
    Slig,
    Shrykull,
    Icons,
    MenuHighlight,
    MenuAbeSpeak,
    MenuGlukkonSpeak,
    MenuScrabSpeak,
    MenuParamiteSpeak,
    MenuSligSpeak,
    Slog,
    Slurg,
    Elum,
    BirdPortal,
    PullRingRope,
    BoneBag,
    Drill,
    Chime,
    HoistRocks,
    ExplosionRocks,
    ExplosionSticks,
    CrawlingSligLocker,
    Lifts,
    Doors,
    Dove,
    ElectricWall,
    Explosions,
    FallingItems,
    FootSwitches,
    Grenade,
    HoneySack,
    MeatSack,
    MeatSaw,
    MotionDetector,
    MovingBomb,
    RockSack,
    SecurityClaw,
    SecurityDoor,
    SecurityOrb,
    SlamDoors,
    SlapLock,
    StatusLight,
    SwingingBall,
    Lever,
    TimedMine,
    TorturedMudokon,
    TrapDoors,
    UXB,
    WorkWheel,
    ZapLine,
    FlintLock,
    BoomMachine,
    CrawlingSligButton,
    BellHammer,
    DoorLock,
};

static const char* ToString(EAnimGroup animGroup)
{
    switch (animGroup)
    {
        case EAnimGroup::Ungrouped:
            return "ungrouped";
        case EAnimGroup::Gibs:
            return "gibs";
        case EAnimGroup::Ropes:
            return "ropes";
        case EAnimGroup::Throwables:
            return "throwables";
        case EAnimGroup::Abe:
            return "abe";
        case EAnimGroup::Lights:
            return "lights";
        case EAnimGroup::Bat:
            return "bat";
        case EAnimGroup::Fleech:
            return "fleech";
        case EAnimGroup::FlyingSlig:
            return "flying_slig";
        case EAnimGroup::Glukkons:
            return "glukkons";
        case EAnimGroup::Greeter:
            return "greeter";
        case EAnimGroup::MineCar:
            return "mine_car";
        case EAnimGroup::Paramite:
            return "paramtie";
        case EAnimGroup::Scrab:
            return "scrab";
        case EAnimGroup::CrawlingSlig:
            return "crawling_slig";
        case EAnimGroup::Slig:
            return "slig";
        case EAnimGroup::Shrykull:
            return "shrykull";
        case EAnimGroup::Icons:
            return "icons";
        case EAnimGroup::MenuHighlight:
            return "menu_highlight";
        case EAnimGroup::MenuAbeSpeak:
            return "menu_abe_speak";
        case EAnimGroup::MenuGlukkonSpeak:
            return "menu_glukkon_speak";
        case EAnimGroup::MenuScrabSpeak:
            return "menu_Scrab_speak";
        case EAnimGroup::MenuParamiteSpeak:
            return "menu_paramite_speak";
        case EAnimGroup::MenuSligSpeak:
            return "menu_slig_speak";
        case EAnimGroup::Slog:
            return "slog";
        case EAnimGroup::Slurg:
            return "slurg";
        case EAnimGroup::Elum:
            return "elum";
        case EAnimGroup::BirdPortal:
            return "bird_portal";
        case EAnimGroup::PullRingRope:
            return "pull_ring_rope";
        case EAnimGroup::BoneBag:
            return "bone_bag";
        case EAnimGroup::Drill:
            return "drill";
        case EAnimGroup::Chime:
            return "chime";
        case EAnimGroup::HoistRocks:
            return "hoist_rocks";
        case EAnimGroup::ExplosionRocks:
            return "explosion_rocks";
        case EAnimGroup::ExplosionSticks:
            return "explosion_sticks";
        case EAnimGroup::CrawlingSligLocker:
            return "crawling_slig_locker";
        case EAnimGroup::Lifts:
            return "lifts";
        case EAnimGroup::Doors:
            return "doors";
        case EAnimGroup::Dove:
            return "dove";
        case EAnimGroup::ElectricWall:
            return "electric_wall";
        case EAnimGroup::Explosions:
            return "explosions";
        case EAnimGroup::FallingItems:
            return "falling_items";
        case EAnimGroup::FootSwitches:
            return "foot_switches";
        case EAnimGroup::Grenade:
            return "grenade";
        case EAnimGroup::HoneySack:
            return "honey_sack";
        case EAnimGroup::MeatSack:
            return "meat_sack";
        case EAnimGroup::MeatSaw:
            return "meat_saw";
        case EAnimGroup::MotionDetector:
            return "motion_detector";
        case EAnimGroup::MovingBomb:
            return "moving_bomb";
        case EAnimGroup::RockSack:
            return "rock_sack";
        case EAnimGroup::SecurityClaw:
            return "security_claw";
        case EAnimGroup::SecurityDoor:
            return "security_door";
        case EAnimGroup::SecurityOrb:
            return "security_orb";
        case EAnimGroup::SlamDoors:
            return "slam_doors";
        case EAnimGroup::SlapLock:
            return "slap_lock";
        case EAnimGroup::StatusLight:
            return "status_light";
        case EAnimGroup::SwingingBall:
            return "swinging_ball";
        case EAnimGroup::Lever:
            return "lever";
        case EAnimGroup::TimedMine:
            return "timed_mine";
        case EAnimGroup::TorturedMudokon:
            return "tortured_mudokon";
        case EAnimGroup::TrapDoors:
            return "trap_doors";
        case EAnimGroup::UXB:
            return "uxb";
        case EAnimGroup::WorkWheel:
            return "work_wheel";
        case EAnimGroup::ZapLine:
            return "zap_line";
        case EAnimGroup::FlintLock:
            return "flint_lock";
        case EAnimGroup::BoomMachine:
            return "boom_machine";
        case EAnimGroup::CrawlingSligButton:
            return "crawling_slig_button";
        case EAnimGroup::BellHammer:
            return "bell_hammer";
        case EAnimGroup::DoorLock:
            return "door_lock";
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
    {AnimId::Abe_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Abe_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Abe_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::BlindMud_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::BlindMud_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::BlindMud_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    { AnimId::Aslik_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Aslik_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Aslik_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Dripik_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Dripik_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Dripik_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Phleg_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Phleg_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Phleg_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Elum_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Meat_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Metal_Gib, EAnimGroup::Gibs, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Rock_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Slig_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Arm_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slog_Head_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Body_Gib, EAnimGroup::Gibs, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Stick_Gib, EAnimGroup::Gibs, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },

    {AnimId::Mudokon_RunToRoll, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_CrouchSpeak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_ToCrouchSpeak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_FallLandDie, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_PoisonGasDeath, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_ZShotRolling, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_ZShot, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_DoorEnter, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_DoorExit, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_WalkOffEdge, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RunOffEdge, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_SneakOffEdge, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_HandstoneEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_GrenadeMachineUse, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_HandstoneBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_LedgeHang, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_LedgeDescend, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RingRopePullHang, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_LedgeHangWobble, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_LedgeAscend, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_HopMid, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_HopLand, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_HopBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_PickupItem, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RollingKnockback, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RollingKnockForward, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_LiftGrabIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_LiftUseDown, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_AO_LiftUse, EAnimGroup::Abe, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_LiftGrabEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_LiftGrabBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_LiftUseUp, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_MineCarEnter, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::Mudokon_MineCarExit, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::Mudokon_PushWall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RockThrowStandingHold, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RockThrowStandingEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RockThrowCrouchingHold, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RockThrowCrouchingThrow, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RockThrowStandingThrow, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_Rolling, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RollingEnd_Unused, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RollBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_DunnoEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_EndShrykull, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_ToShrykull, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_SlapBomb, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_Sorry, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::Mudokon_AfterSorry, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::Mudokon_ToSpeak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_Null, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RunJumpLand, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RunTurnToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RunJumpLandRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RunJumpLandWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_TurnToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_HopToFall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RunJumpToFall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_RollOffEdge, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_Well_Idle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_WellBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_JumpIntoWell, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},

    // Should probably make another group called Mudokon and/or remove the Abe group since they share
    // the same animations
    {AnimId::Mudokon_Chant, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    {AnimId::Mudokon_ChantEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false},
    { AnimId::Mudokon_Chisel, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ToDuck, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchToStand, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchTurn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Duck, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DuckToCrouch, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Fall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_KnockbackGetUp, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SlapOwnHead, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_HoistIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LandSoft, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_HoistBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Idle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_KnockForward, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Knockback, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_LeverUse, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Run, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunSlideStop, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunSlideTurn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunJumpMid, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunJumpBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchScrub, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DunnoBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Punch, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_Idle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_Angry, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_Speak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_ShootStart, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_ShootEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sling_AngryToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubLoop, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubLoopToPause, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubPauseToLoop, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubPause, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_IdleToStandScrub, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandScrubToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Struggle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StruggleToCrouchChant, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchChant, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchChantToStruggle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DunnoMid, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_M_15_Null, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchSpeak1, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_CrouchSpeak2, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumWalkLoop, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunSlideStop, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunTurn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_M_106_Null, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumHopBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumHopMid, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumHopLand, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunJumpBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunJumpMid, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunJumpLand, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumTurn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunLoop, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumSpeak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_M_116_Null, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumWalkBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_M_119_Null, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumMidRunToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunTurnToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumRunTurnToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumWalkEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumMidWalkEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumBeesStruggling, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumMountEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumUnmountBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumUnmountEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumMountBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumFallOffEdge, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumFall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumLand, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumJumpToFall, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_ElumKnockback, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Idle_RubEyes, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Idle_StretchArms, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Idle_Yawn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_ToShrykull, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_ShrykullEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_HoistBegin_Long, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_LandSoft_Long, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sneak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Sneak_End, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_WalkToSneak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandToCrouch, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Speak1, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Speak3, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Speak2, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SpeakFart, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandingTurn, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_WalkBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_WalkToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidWalkToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_AO_Null, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StartChisel, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StopChisel, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_WalkToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidWalkToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidRunToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_RunTurnToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SneakToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidWalkToSneak, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidSneakToWalk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SneakBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_SneakToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_MidSneakToIdle, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_StandToRun, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_FallingFromGrab, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_DuckKnockback, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_Walk, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_TurnWheel, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_TurnWheelEnd, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mudokon_TurnWheelBegin, EAnimGroup::Abe, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    {AnimId::Bat_Flying, EAnimGroup::Bat, EReliveLevelIds::eNone, EReliveLevelIds::eStockYards, false},
    {AnimId::Bat, EAnimGroup::Bat, EReliveLevelIds::eNone, EReliveLevelIds::eStockYards, false},
    {AnimId::Bat_Unknown, EAnimGroup::Bat, EReliveLevelIds::eNone, EReliveLevelIds::eStockYards, false},

    {AnimId::Fleech_Climb, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_SettleOnGround, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Consume, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Fall, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Sleeping, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Idle, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_StopCrawling, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_StopMidCrawlCycle, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Land, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_SleepingWithTongue, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_PatrolCry, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_RaiseHead, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_WakingUp, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Unused, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Knockback, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_ExtendTongueFromEnemy, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_RetractTongueFromEnemey, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_DeathByFalling, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},
    {AnimId::Fleech_Crawl, EAnimGroup::Fleech, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false},

    {AnimId::FlyingSlig_Idle, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveHorizontal, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_IdleTurnAround, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveDown, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveDownTurnAround, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveUp, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveUpTurnAround, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_PullLever, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_Speak, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_Possession, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    {AnimId::FlyingSlig_MoveHorizontalEnd, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false},
    { AnimId::FlyingSlig_MoveUpStart, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_MoveHorizontalToDown, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_MoveUpToHorizontal, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_MoveDownToHorizontal, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_TurnQuick, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_IdleToHorizontal, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_BeginDownMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_EndDownMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_DownKnockback, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_UpKnockback, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_EndUpMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_InstantUpXTurn, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_InstantDownXTurn, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_HorizontalToUpMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::FlyingSlig_TurnToHorizontalMovement, EAnimGroup::FlyingSlig, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Glukkon_Aslik_KnockBack, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Jump, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Land, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_LongLaugh, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_ChantShake, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_GetShot, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Speak1, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Speak2, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Speak3, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_KnockBackStandBegin, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_KnockBackStandEnd, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Turn, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_JumpToFall, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_WalkToFall, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Fall, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_DeathFall, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_BeginWalk, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_EndWalk, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_StandToJump, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_JumpToStand, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_WalkToJump, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_JumpToWalk, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_EndSingleStep, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Aslik_Walk, EAnimGroup::Glukkons, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },

    { AnimId::Glukkon_Dripik_KnockBack, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Jump, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Land, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_LongLaugh, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_ChantShake, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_GetShot, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Speak1, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Speak2, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Speak3, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_KnockBackStandBegin, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_KnockBackStandEnd, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Turn, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_JumpToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_WalkToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Fall, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_DeathFall, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_BeginWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_EndWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_StandToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_JumpToStand, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_WalkToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_JumpToWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_EndSingleStep, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Dripik_Walk, EAnimGroup::Glukkons, EReliveLevelIds::eBarracks_Ender, EReliveLevelIds::eNone, false },

    { AnimId::Glukkon_Normal_KnockBack, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Jump, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Land, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_LongLaugh, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_ChantShake, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_GetShot, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Speak1, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Speak2, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Speak3, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_KnockBackStandBegin, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_KnockBackStandEnd, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Turn, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_JumpToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_WalkToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Fall, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_DeathFall, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_BeginWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_EndWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_StandToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_JumpToStand, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_WalkToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_JumpToWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_EndSingleStep, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Normal_Walk, EAnimGroup::Glukkons, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },

    { AnimId::Glukkon_Phleg_KnockBack, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Jump, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Land, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_LongLaugh, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_ChantShake, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_GetShot, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Speak1, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Speak2, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Speak3, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_KnockBackStandBegin, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_KnockBackStandEnd, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Turn, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_JumpToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_WalkToFall, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Fall, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_DeathFall, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_BeginWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_EndWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_StandToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_JumpToStand, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_WalkToJump, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_JumpToWalk, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_EndSingleStep, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },
    { AnimId::Glukkon_Phleg_Walk, EAnimGroup::Glukkons, EReliveLevelIds::eBonewerkz_Ender, EReliveLevelIds::eNone, false },

    { AnimId::Greeter_Chase, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Falling, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Hit, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Moving, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Speak, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Greeter_Turn, EAnimGroup::Greeter, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::Mine_Car_Closed, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Open, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Shake_A, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Shake_B, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Tread_Idle, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Tread_Move_A, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Mine_Car_Tread_Move_B, EAnimGroup::MineCar, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Paramite_Attack, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Struggle, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_AO_M_7_Unknown, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_AO_M_22_Unknown, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WebGoingDown, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WebLeaveDown, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WebLeaveUp, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WebIdle, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WebGrab, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WebGoingUp, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Eating, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Death, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Idle, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WalkBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_JumpUpMidair, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_JumpUpLand, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_JumpUpBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Walking, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_RunningAttack, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Running, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Hop, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_PreHiss, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Squawk, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_AllOYaGameSpeakBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_GameSpeakEnd, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_GameSpeakBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Turn, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Unused, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WalkRunTransition, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_WalkEnd, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_RunBegin, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_RunEnd, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Falling, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_RopePull, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_CloseAttack, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Landing, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Unused2, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Knockback, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_SurpriseWeb, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_Hiss, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::Paramite_PostHiss, EAnimGroup::Paramite, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },

    { AnimId::Scrab_FeedToGulp, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Stamp, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_DeathEnd, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Feed, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_StandToFeed, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Idle, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Jump, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_LegKick, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_HopBegin, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Landing, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Knockback, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Shriek, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Run, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_RunToStand, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_HowlBegin, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_HowlEnd, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Turn, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_JumpAndRunToFall, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_StandToWalk, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_StandToRun, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_WalkToStand, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_WalkToFall, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_WalkToRun, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_RunToWalk, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_GetEaten, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Empty, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_GulpToStand, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_AttackLunge, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_DeathBegin, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_AO_ToFall, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_AO_ToFeed, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_AO_M_19_Unused, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_Walk, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_AttackSpin, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Scrab_ScrabBattleAnim, EAnimGroup::Scrab, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eDesertTemple, false },

    { AnimId::CrawlingSlig_Idle, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_UsingButton, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_WakingUp, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Crawling, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_StartFalling, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Falling, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Landing, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_ToShakingToIdle, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Speaking, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Snoozing, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_PushingWall, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_TurnAround, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Shaking, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_Empty, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_ShakingToIdle, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_EndCrawling, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_IdleToPushingWall, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSlig_EndPushingWall, EAnimGroup::CrawlingSlig, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },

    { AnimId::Slig_Beat, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_OutToFall, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LandingFatal, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Smash, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LandingSoft, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_FallingInitiate, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_KnockbackToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Idle, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_ShootToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Knockback, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_PullLever, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LiftGripping, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LiftUp, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LiftDown, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LiftGrip, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_LiftUngrip, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_PossessShake, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_ReloadGun, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Running, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_StandToRun, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SlidingToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SlidingTurn, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Shoot, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Recoil, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_ShootZ, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_ShootZtoStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SteppingToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Sleeping, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Speak1, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Speak2, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Speak3, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Speak4, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_TurnAroundStanding, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Walking, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_StandToWalk, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SlidingTurnToWalk, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SlidingTurnToRun, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_StandingToStep, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_Gamespeak, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_WalkToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Slig_SleepingToStand, EAnimGroup::Slig, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::CrawlingSligButton, EAnimGroup::CrawlingSligButton, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSligButtonUse, EAnimGroup::CrawlingSligButton, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },

    { AnimId::ShrykullStart, EAnimGroup::Shrykull, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::ShrykullTransform, EAnimGroup::Shrykull, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::ShrykullDetransform, EAnimGroup::Shrykull, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },

    { AnimId::NormalMudIcon, EAnimGroup::Icons, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::AngryMudIcon, EAnimGroup::Icons, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::HappyMudIcon, EAnimGroup::Icons, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::MenuHighlight_ButtonRemapSquare, EAnimGroup::MenuHighlight, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuHighlight_Circle, EAnimGroup::MenuHighlight, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuHighlight_Triangle, EAnimGroup::MenuHighlight, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuHighlight_Square, EAnimGroup::MenuHighlight, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },

    { AnimId::MenuAbeSpeak_Laugh, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_WhistleHigh, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_WhistleLow, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Fart, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Hello, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Idle, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_IdleBlink, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Ok, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_FollowMe, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Wait, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Work, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Anger, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_AllYa, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Sympathy, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_StopIt, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Chant, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_ChantEnd, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::MenuAbeSpeak_Goodbye, EAnimGroup::MenuAbeSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },

    { AnimId::MenuGlukkonSpeak_Idle, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_IdleVariation, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_Hey, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_Commere, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_StayHere, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_DoIt, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_KillEm, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_AllOYa, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_Help, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuGlukkonSpeak_Laugh, EAnimGroup::MenuGlukkonSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },

    { AnimId::MenuScrabSpeak_Idle, EAnimGroup::MenuScrabSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuScrabSpeak_IdleVariation, EAnimGroup::MenuScrabSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuScrabSpeak_ShredPower, EAnimGroup::MenuScrabSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuScrabSpeak_Howl, EAnimGroup::MenuScrabSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },

    { AnimId::MenuParamiteSpeak_Howdy, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_Idle, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_IdleVariation, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_Cmon, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_Stay, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_DoIt, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_Attack, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuParamiteSpeak_AllAYa, EAnimGroup::MenuParamiteSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },

    { AnimId::MenuSligSpeak_Idle, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_Hi, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_HereBoy, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_Freeze, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_GetEm, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_SmoBs, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_Bs, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_LookOut, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },
    { AnimId::MenuSligSpeak_Laugh, EAnimGroup::MenuSligSpeak, EReliveLevelIds::eMenu, EReliveLevelIds::eNone, false },

    { AnimId::Slog_MoveHeadUpwards, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_StartFastBarking, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_AngryBark, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_BROKEN_J, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Dying, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Eating, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Fall, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Idle, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Growl, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_AO_M_12_Unknown, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_JumpUpwards, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_JumpForwards, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Run, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_StopRunning, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_SlideTurn, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Scratch, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Sleeping, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_MoveHeadDownwards, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_WakeUp, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_TurnAround, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_StartWalking, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_EndWalking, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Land, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Unused, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_EndFastBarking, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::Slog_Walk, EAnimGroup::Slog, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },

    { AnimId::Slurg_Burst, EAnimGroup::Slurg, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Slurg_Move, EAnimGroup::Slurg, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Slurg_Turn_Around, EAnimGroup::Slurg, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::Background_Glukkon_Idle, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },
    { AnimId::Background_Glukkon_Laugh, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },
    { AnimId::Background_Glukkon_KillHim1, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },
    { AnimId::Background_Glukkon_KillHim2, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },
    { AnimId::Background_Glukkon_Dying, EAnimGroup::Glukkons, EReliveLevelIds::eNone, EReliveLevelIds::eBoardRoom, false },

    { AnimId::BellHammer_Idle, EAnimGroup::BellHammer, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::BellHammer_Smashing, EAnimGroup::BellHammer, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },

    { AnimId::Elum_Idle, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_WalkLoop, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Turn, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_WalkToIdle, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_MidWalkToIdle, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_IdleToWalk2, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_IdleToWalk1, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ToYell, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Yell, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Unknown1, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunTurn, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunTurnToWalk, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Speak, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Land, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunOffEdge, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_WalkOffEdge, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_JumpToFall, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_LickingHoney, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_LickingToStruggle, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_AbeMountingEnd, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_AbeUnmountingEnd, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_BeesStruggling, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_HopBegin, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_HopMid, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_HopLand, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunJumpBegin, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunJumpMid, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunJumpLand, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunLoop, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunSlideStop, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunTurnToRun, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_IdleToRun, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_WalkToRun, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_MidWalkToRun, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_RunToWalk, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_MidRunToWalk, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ScratchBegin, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ScratchLoop, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ScratchEnd, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_ScratchUnknown, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_MountUnmountBegin, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Elum_Knockback, EAnimGroup::Elum, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },

    { AnimId::BirdPortal_TerminatorGrow, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BirdPortal_Sparks, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BirdPortal_Flash, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BirdPortal_TerminatorShrink, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BirdPortal_TerminatorIdle, EAnimGroup::BirdPortal, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::PullRingRope_Idle, EAnimGroup::PullRingRope, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::PullRingRope_UseEnd, EAnimGroup::PullRingRope, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::PullRingRope_UseBegin, EAnimGroup::PullRingRope, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Pullring_Desert_Idle, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Pullring_Desert_UseBegin, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Pullring_Desert_UseEnd, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::Pullring_Farms_Idle, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Pullring_Farms_UseBegin, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Pullring_Farms_UseEnd, EAnimGroup::PullRingRope, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Bone, EAnimGroup::Throwables, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::BoneBag_Idle, EAnimGroup::BoneBag, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::BoneBag_HardHit, EAnimGroup::BoneBag, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::BoneBag_SoftHit, EAnimGroup::BoneBag, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },

    { AnimId::Rock, EAnimGroup::Throwables, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::RockSack_Idle, EAnimGroup::RockSack, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::RockSack_SoftHit, EAnimGroup::RockSack, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::RockSack_HardHit, EAnimGroup::RockSack, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Security_Claw_Lower_Open, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Security_Claw_Lower_Close, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Security_Claw_Lower_Idle, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Security_Claw_Upper_Rotating, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Security_Claw_Upper_NoRotation, EAnimGroup::SecurityClaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Security_Door, EAnimGroup::SecurityDoor, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarmsReturn, false },
    { AnimId::Security_Door_Idle, EAnimGroup::SecurityDoor, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarmsReturn, false },
    { AnimId::Security_Door_Speak, EAnimGroup::SecurityDoor, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarmsReturn, false },

    { AnimId::Security_Orb, EAnimGroup::SecurityOrb, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },

    { AnimId::Slam_Door_Industrial_Closed, EAnimGroup::SlamDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Industrial_Closing, EAnimGroup::SlamDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Industrial_Opening, EAnimGroup::SlamDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Vault_Closed, EAnimGroup::SlamDoors, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Vault_Closing, EAnimGroup::SlamDoors, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Slam_Door_Vault_Opening, EAnimGroup::SlamDoors, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },

    { AnimId::SlapLock_Initiate, EAnimGroup::SlapLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::SlapLock_Punched, EAnimGroup::SlapLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::SlapLock_Shaking, EAnimGroup::SlapLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::Status_Light_Green, EAnimGroup::StatusLight, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Status_Light_Red, EAnimGroup::StatusLight, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Swinging_Ball_Fast, EAnimGroup::SwingingBall, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Swinging_Ball_Normal, EAnimGroup::SwingingBall, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Swinging_Ball_Slow, EAnimGroup::SwingingBall, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },

    { AnimId::Lever_Idle, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Lever_Pull_Left, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Lever_Pull_Release_Left, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Lever_Pull_Right, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Lever_Pull_Release_Right, EAnimGroup::Lever, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::RuptureFarms_Lever_Idle, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_Lever_Pull_Left, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_Lever_Pull_Release_Left, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_Lever_Pull_Right, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_Lever_Pull_Release_Right, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Lines_Lever_Idle, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_Lever_Pull_Left, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_Lever_Pull_Release_Left, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_Lever_Pull_Right, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_Lever_Pull_Release_Right, EAnimGroup::Lever, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },

    { AnimId::TimedMine_Activated, EAnimGroup::TimedMine, EReliveLevelIds::eMines, EReliveLevelIds::eForest, false },
    { AnimId::TimedMine_Idle, EAnimGroup::TimedMine, EReliveLevelIds::eMines, EReliveLevelIds::eForest, false },

    { AnimId::Tortured_Mudokon, EAnimGroup::TorturedMudokon, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Tortured_Mudokon_Released, EAnimGroup::TorturedMudokon, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Tortured_Mudokon_Zap, EAnimGroup::TorturedMudokon, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Tortured_Mudokon_Tears, EAnimGroup::TorturedMudokon, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },

    { AnimId::Trap_Door_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Trap_Door_Tribal_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Tribal_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Tribal_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Trap_Door_Tribal_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },

    { AnimId::R1_TrapDoor_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::R1_TrapDoor_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::R1_TrapDoor_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::R1_TrapDoor_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Lines_TrapDoor_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_TrapDoor_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_TrapDoor_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Lines_TrapDoor_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },

    { AnimId::Desert_TrapDoor_Open, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertEscape, false },
    { AnimId::Desert_TrapDoor_Closed, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertEscape, false },
    { AnimId::Desert_TrapDoor_Opening, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertEscape, false },
    { AnimId::Desert_TrapDoor_Closing, EAnimGroup::TrapDoors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertEscape, false },

    { AnimId::UXB_Active, EAnimGroup::UXB, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::UXB_Disabled, EAnimGroup::UXB, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::UXB_Toggle, EAnimGroup::UXB, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Work_Wheel_Idle, EAnimGroup::WorkWheel, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Work_Wheel_Turning, EAnimGroup::WorkWheel, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Zap_Line_Blue, EAnimGroup::ZapLine, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Zap_Line_Red, EAnimGroup::ZapLine, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Drill_Horizontal_Off, EAnimGroup::Drill, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Drill_Horizontal_On, EAnimGroup::Drill, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Drill_Vertical_Off, EAnimGroup::Drill, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Drill_Vertical_On, EAnimGroup::Drill, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::BigChime, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::BigChime_Moving, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::MediumChime, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::MediumChime_Moving, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::SmallChime, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::SmallChime_Moving, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Chime_Ball, EAnimGroup::Chime, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },

    { AnimId::HoistRock1, EAnimGroup::HoistRocks, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::HoistRock2, EAnimGroup::HoistRocks, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::HoistRock3, EAnimGroup::HoistRocks, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::AO_HoistRock1, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::AO_HoistRock2, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::AO_HoistRock3, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::RuptureFarms_HoistRock1, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_HoistRock2, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::RuptureFarms_HoistRock3, EAnimGroup::HoistRocks, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Explosion_Rocks, EAnimGroup::ExplosionRocks, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Explosion_Sticks, EAnimGroup::ExplosionRocks, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },

    { AnimId::CrawlingSligLocker_Closed, EAnimGroup::CrawlingSligLocker, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::CrawlingSligLocker_Open, EAnimGroup::CrawlingSligLocker, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },

    { AnimId::FlintLock_Hammers_Activating, EAnimGroup::FlintLock, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::FlintLock_Hammers_Disabled, EAnimGroup::FlintLock, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::FlintLock_Gourd, EAnimGroup::FlintLock, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },

    { AnimId::GreenDoorLight, EAnimGroup::Lights, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarmsReturn, false },
    { AnimId::GreenHubLight, EAnimGroup::Lights, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarmsReturn, false },
    { AnimId::RedDoorLight, EAnimGroup::Lights, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarmsReturn, false },
    { AnimId::RedHubLight, EAnimGroup::Lights, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarmsReturn, false },

    { AnimId::LiftPlatform_Mines, EAnimGroup::Lifts, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::LiftBottomWheel_Mines, EAnimGroup::Lifts, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::LiftTopWheel_Mines, EAnimGroup::Lifts, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::LiftPlatform_Necrum, EAnimGroup::Lifts, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::LiftBottomWheel_Necrum, EAnimGroup::Lifts, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::LiftTopWheel_Necrum, EAnimGroup::Lifts, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::LiftPlatform_Forest, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },
    { AnimId::LiftBottomWheel_Forest, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },
    { AnimId::LiftTopWheel_Forest, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false },

    { AnimId::LiftPlatform_Desert, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::LiftBottomWheel_Desert, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::LiftTopWheel_Desert, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },

    { AnimId::LiftPlatform_Desert2, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::LiftBottomWheel_Desert2, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::LiftTopWheel_Desert2, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },

    { AnimId::LiftPlatform_RuptureFarms, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::LiftBottomWheel_RuptureFarms, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::LiftTopWheel_RuptureFarms, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::LiftPlatform_Lines, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::LiftBottomWheel_Lines, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::LiftTopWheel_Lines, EAnimGroup::Lifts, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },

    { AnimId::Door_Barracks_Closed, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_Barracks_Open, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_BarracksMetal_Closed, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_BarracksMetal_Open, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_Bonewerkz_Closed, EAnimGroup::Doors, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::Door_Bonewerkz_Open, EAnimGroup::Doors, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::Door_Brewery_Closed, EAnimGroup::Doors, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Door_Brewery_Open, EAnimGroup::Doors, EReliveLevelIds::eBrewery, EReliveLevelIds::eNone, false },
    { AnimId::Door_Feeco_Closed, EAnimGroup::Doors, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Door_Feeco_Open, EAnimGroup::Doors, EReliveLevelIds::eFeeCoDepot, EReliveLevelIds::eNone, false },
    { AnimId::Door_Mines_Closed, EAnimGroup::Doors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Door_Mines_Open, EAnimGroup::Doors, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Door_Temple_Closed, EAnimGroup::Doors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Door_Temple_Open, EAnimGroup::Doors, EReliveLevelIds::eMudancheeVault, EReliveLevelIds::eNone, false },
    { AnimId::Door_Train_Closed, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_Train_Closing, EAnimGroup::Doors, EReliveLevelIds::eBarracks, EReliveLevelIds::eNone, false },
    { AnimId::Door_RuptureFarms_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Door_RuptureFarms_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Door_Lines_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Door_Lines_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Door_Forest_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Door_Forest_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::HubDoor_Forest_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::HubDoor_Forest_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::FinalTestDoor_Forest_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::FinalTestDoor_Forest_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Door_Desert_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Door_Desert_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::HubDoor_Desert_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::HubDoor_Desert_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::FinalTestDoor_Desert_Open, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },
    { AnimId::FinalTestDoor_Desert_Closed, EAnimGroup::Doors, EReliveLevelIds::eNone, EReliveLevelIds::eDesertTemple, false },

    { AnimId::Rope_Lines, EAnimGroup::Ropes, EReliveLevelIds::eNone, EReliveLevelIds::eLines, false },
    { AnimId::Rope_R1, EAnimGroup::Ropes, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::AE_Rope, EAnimGroup::Ropes, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Electric_Wall, EAnimGroup::ElectricWall, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Explosion, EAnimGroup::Explosions, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Explosion_Mine, EAnimGroup::Explosions, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Explosion_Small, EAnimGroup::Explosions, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::FallingCrate_Falling, EAnimGroup::FallingItems, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::FallingCrate_Waiting, EAnimGroup::FallingItems, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },

    { AnimId::AE_FallingRock_Falling, EAnimGroup::FallingItems, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::AE_FallingRock_Waiting, EAnimGroup::FallingItems, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::AO_FallingRock_Falling, EAnimGroup::FallingItems, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },
    { AnimId::AO_FallingRock_Waiting, EAnimGroup::FallingItems, EReliveLevelIds::eNone, EReliveLevelIds::eDesert, false },

    { AnimId::FallingMeat_Waiting, EAnimGroup::FallingItems, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::FallingMeat_Falling, EAnimGroup::FallingItems, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Foot_Switch_Bonewerkz_Idle, EAnimGroup::FootSwitches, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },
    { AnimId::Foot_Switch_Bonewerkz_Pressed, EAnimGroup::FootSwitches, EReliveLevelIds::eBonewerkz, EReliveLevelIds::eNone, false },

    { AnimId::Foot_Switch_Industrial_Idle, EAnimGroup::FootSwitches, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Foot_Switch_Industrial_Pressed, EAnimGroup::FootSwitches, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },

    { AnimId::Foot_Switch_Temple, EAnimGroup::FootSwitches, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },
    { AnimId::Foot_Switch_Temple_Pressed, EAnimGroup::FootSwitches, EReliveLevelIds::eNone, EReliveLevelIds::eForestTemple, false },

    { AnimId::Foot_Switch_Vault_Idle, EAnimGroup::FootSwitches, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },
    { AnimId::Foot_Switch_Vault_Pressed, EAnimGroup::FootSwitches, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eNone, false },

    { AnimId::Grenade, EAnimGroup::Throwables, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BoomMachine_Button_Off, EAnimGroup::BoomMachine, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BoomMachine_Button_On, EAnimGroup::BoomMachine, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BoomMachine_Nozzle_Idle, EAnimGroup::BoomMachine, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::BoomMachine_Nozzle_DropGrenade, EAnimGroup::BoomMachine, EReliveLevelIds::eBarracks, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::Honey_Drip, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false }, // or forest chase?
    { AnimId::HoneySack_Hanging, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false }, // or forest chase?
    { AnimId::HoneySack_Falling, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false }, // or forest chase?
    { AnimId::HoneySack_FallingToSmashed, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false }, // or forest chase?
    { AnimId::HoneySack_OnGround, EAnimGroup::HoneySack, EReliveLevelIds::eNone, EReliveLevelIds::eForest, false }, // or forest chase?

    { AnimId::Meat, EAnimGroup::Throwables, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::MeatSack_Idle, EAnimGroup::MeatSack, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },
    { AnimId::MeatSack_Hit, EAnimGroup::MeatSack, EReliveLevelIds::eMudomoVault, EReliveLevelIds::eForestTemple, false },

    { AnimId::MeatSaw_Idle, EAnimGroup::MeatSaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::MeatSaw_Moving, EAnimGroup::MeatSaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::MeatSawMotor, EAnimGroup::MeatSaw, EReliveLevelIds::eNone, EReliveLevelIds::eRuptureFarms, false },

    { AnimId::MotionDetector_Flare, EAnimGroup::MotionDetector, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },
    { AnimId::MotionDetector_Laser, EAnimGroup::MotionDetector, EReliveLevelIds::eMines, EReliveLevelIds::eStockYards, false },

    { AnimId::Door_Lock_Idle, EAnimGroup::DoorLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },
    { AnimId::Door_Lock_Open, EAnimGroup::DoorLock, EReliveLevelIds::eNecrum, EReliveLevelIds::eNone, false },

    { AnimId::Mine_Flash, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::WaterDrop, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::BrewMachine_Button, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eNone, false },
    { AnimId::Circular_Fade, EAnimGroup::Ungrouped, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::DeathFlare_1, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::DeathFlare_2, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Bullet_Shell, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Blood, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::MenuDoor, EAnimGroup::Ungrouped, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::AbeIntro, EAnimGroup::Ungrouped, EReliveLevelIds::eMenu, EReliveLevelIds::eMenu, false },
    { AnimId::BloodDrop, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Scoopz, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::FlintGlow, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::GreenGlow, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::GoldGlow, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::ChantOrb_Particle_Small, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Well_Leaf, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Bomb_RedGreenTick, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Bomb_Flash, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Stone_Ball, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Stone_Ball_Stopper, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Star, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::AE_ZapSpark, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::ParamiteWeb, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::MovingBomb, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Mine, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Honey, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::HintFly, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Fart, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::WaterSplash, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::ShootingZFire_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::OptionChantOrb_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::SquibSmoke_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::ChantOrb_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::ShootingFire_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Vaporize_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Loading_Icon2, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Loading_Icon, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Dust_Particle, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::ObjectShadow, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::SpotLight, EAnimGroup::Ungrouped, EReliveLevelIds::eMines, EReliveLevelIds::eRuptureFarms, false },
    { AnimId::Door_FireBackgroundGlow, EAnimGroup::Ungrouped, EReliveLevelIds::eNecrum, EReliveLevelIds::eDesertTemple, false },
    { AnimId::Fire, EAnimGroup::Ungrouped, EReliveLevelIds::eNecrum, EReliveLevelIds::eDesertTemple, false },
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

const char_type* AnimRecName(AnimId anim)
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
            LOG_WARNING("tlv of type " << static_cast<s16>(tlv.mTlvType32.mType) << " is unused");
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

static void SaveJson(const nlohmann::json& j, FileSystem& fs, const FileSystem::Path& path)
{
    std::string jsonStr = j.dump(4);

    std::vector<u8> data;
    data.resize(jsonStr.length());
    data.assign(jsonStr.begin(), jsonStr.end());
    fs.Save(path, data);
}

template <typename TlvType, typename LevelIdType>
static void ConvertPath(FileSystem& fs, const FileSystem::Path& path, const ReliveAPI::LvlFileChunk& pathBndChunk, EReliveLevelIds reliveLvl, LevelIdType lvlIdx, bool isAo)
{
    s32 width = 0;
    s32 height = 0;
    CollisionInfo* pCollisionInfo = nullptr;
    s32 indexTableOffset = 0;
    s32 objectOffset = 0;

    if (isAo)
    {
        const AO::PathBlyRec* pBlyRec = AO::Path_Get_Bly_Record_434650(reliveLvl, static_cast<u16>(pathBndChunk.Id()));

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


    nlohmann::json j = {
        {"id", pathBndChunk.Id()}, // TODO: shouldn't be required once everything is converted and not using path id numbers
        {"cameras", camerasArray},
        {"collisions", collisionsArray},
    };

    SaveJson(j, fs, path);
    LOG_INFO("converted path " << pathBndChunk.Id() << " level " << (isAo ? ToString(MapWrapper::ToAO(reliveLvl)) : ToString(MapWrapper::ToAE(reliveLvl))));
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

static void ConvertAnimations(const FileSystem::Path& dataDir, FileSystem& fs, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, bool isAo)
{
    // Convert animations that exist in this LVL
    for (auto& rec : kAnimRecConversionInfo)
    {
        // Animation is in this LVL and not yet converted
        if (!rec.mConverted)
        {
            if ((isAo && rec.mAoLvl == reliveLvl) || (!isAo && rec.mAeLvl == reliveLvl))
            {
                FileSystem::Path filePath = dataDir;
                filePath.Append("animations");

                // e.g "abe"
                filePath.Append(ToString(rec.mGroup));

                // Ensure the containing directory exists
                fs.CreateDirectory(filePath);

                const auto& animDetails = isAo ? AO::AnimRec(rec.mAnimId) : AnimRec(rec.mAnimId);

                // e.g "arm_gib"
                filePath.Append(AnimBaseName(rec.mAnimId));

                ReadLvlFileInto(lvlReader, animDetails.mBanName, fileBuffer);
                AnimationConverter animationConverter(filePath, animDetails, fileBuffer, isAo);

                // Track what is converted so we know what is missing at the end
                rec.mConverted = true;
            }
        }
    }
}

template <typename LevelIdType, typename TlvType>
static void ConvertPathBND(const FileSystem::Path& dataDir, const std::string& fileName, FileSystem& fs, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, LevelIdType lvlIdxAsLvl, EReliveLevelIds reliveLvl, bool isAo)
{
    FileSystem::Path pathDir = dataDir;
    pathDir.Append(ToString(lvlIdxAsLvl)).Append("paths");
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
            ConvertPath<TlvType, LevelIdType>(fs, pathJsonFile, pathBndChunk, reliveLvl, lvlIdxAsLvl, isAo);
        }
    }

    SaveLevelInfoJson(dataDir, reliveLvl, lvlIdxAsLvl, fs, pathBndFile);
}

template <typename LevelIdType>
static void ConvertCamera(const FileSystem::Path& dataDir, const std::string& fileName, FileSystem& fs, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, LevelIdType lvlIdxAsLvl)
{
    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);

    ReliveAPI::ChunkedLvlFile camFile(fileBuffer);

    std::string camBaseName = fileName.substr(0, fileName.length() - 4); // chop off .CAM
    FileSystem::Path pathDir = dataDir;
    pathDir.Append(ToString(lvlIdxAsLvl));
    fs.CreateDirectory(pathDir);
    pathDir.Append(camBaseName);

    // Convert camera images and FG layers
    ReliveAPI::CamConverter cc(camFile, pathDir.GetPath());

    // TODO: Convert any BgAnims in this camera
}

template <typename LevelIdType>
static void ConvertSound(const FileSystem::Path& dataDir, const std::string& fileName, FileSystem& fs, ReliveAPI::LvlReader& lvlReader, std::vector<u8>& fileBuffer, LevelIdType lvlIdxAsLvl)
{
    // TODO: For BSQ convert back to the original SEQ name for each entry

    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);

    FileSystem::Path filePath = dataDir;
    filePath.Append(ToString(lvlIdxAsLvl));
    fs.CreateDirectory(filePath);
    filePath.Append(fileName);


    fs.Save(filePath, fileBuffer);
}

template<typename LevelIdType, typename TlvType>
static void ConvertFilesInLvl(const FileSystem::Path& dataDir, FileSystem& fs, ReliveAPI::LvlReader& lvlReader, std::vector<u8>& fileBuffer, LevelIdType lvlIdxAsLvl, EReliveLevelIds reliveLvl, bool isAo)
{
    // Iterate and convert specific file types in the LVL
    for (s32 i = 0; i < lvlReader.FileCount(); i++)
    {
        auto fileName = lvlReader.FileNameAt(i);
        if (!fileName.empty())
        {
            if (endsWith(fileName, ".CAM"))
            {
                //ConvertCamera(dataDir, fileName, fs, fileBuffer, lvlReader, lvlIdxAsLvl);
            }
            // TODO: Seek these out instead of converting everything we see since the names are fixed per LVL
            else if (endsWith(fileName, ".VB") || endsWith(fileName, ".VH") || endsWith(fileName, ".BSQ"))
            {
                ConvertSound(dataDir, fileName, fs, lvlReader, fileBuffer, lvlIdxAsLvl);
            }
            else if (endsWith(fileName, "PATH.BND"))
            {
                ConvertPathBND<LevelIdType, TlvType>(dataDir, fileName, fs, fileBuffer, lvlReader, lvlIdxAsLvl, reliveLvl, isAo);
            }
        }
    }
}

void DataConversion::ConvertDataAO()
{
    // TODO: Check existing data version, if any

    FileSystem fs;

    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ao");
    fs.CreateDirectory(dataDir);

    std::vector<u8> fileBuffer;
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

        ConvertAnimations(dataDir, fs, fileBuffer, lvlReader, reliveLvl, true);

        ConvertFilesInLvl<AO::LevelIds, AO::Path_TLV>(dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, true);
    }
}

void DataConversion::ConvertDataAE()
{
    FileSystem fs;

    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ae");
    fs.CreateDirectory(dataDir);

    std::vector<u8> fileBuffer;
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
        ReliveAPI::LvlReader lvlReader(fileIo, (std::string(::Path_Get_Lvl_Name(reliveLvl)) + ".LVL").c_str());

        if (!lvlReader.IsOpen())
        {
            // Fatal, missing LVL file
            ALIVE_FATAL("Couldn't open lvl file");
        }

        ConvertAnimations(dataDir, fs, fileBuffer, lvlReader, reliveLvl, false);

        ConvertFilesInLvl<::LevelIds, ::Path_TLV>(dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, false);
    }
}
