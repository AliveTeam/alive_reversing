#pragma once

#include "FunctionFwd.hpp"
#include "Factory.hpp"
#include "AddPointer.hpp"

namespace AO {

enum class LevelIds : s16
{
    eNone = -1,
    eMenu_0 = 0,
    eRuptureFarms_1 = 1,
    eLines_2 = 2,
    eForest_3 = 3,
    eForestTemple_4 = 4,
    eStockYards_5 = 5,
    eStockYardsReturn_6 = 6,
    eRemoved_7 = 7,
    eDesert_8 = 8,
    eDesertTemple_9 = 9,
    eCredits_10 = 10,
    eRemoved_11 = 11,
    eBoardRoom_12 = 12,
    eRuptureFarmsReturn_13 = 13,
    eForestChase = 14,
    eDesertEscape = 15,
};

enum class TlvTypes : s16
{
    None_m1 = -1,
    ContinuePoint_0 = 0,
    PathTransition_1 = 1,
    ContinueZone_2 = 2, // TODO: Temp - will be removed later because never used by the game (editor api support)
    Hoist_3 = 3,
    Edge_4 = 4,
    DeathDrop_5 = 5,
    Door_6 = 6,
    ShadowZone_7 = 7,
    LiftPoint_8 = 8,

    WellLocal_11 = 11, // TODO: check if WellLocal and WellExpress are not swapped
    Dove_12 = 12,
    RockSack_13 = 13,
    ZBall_14 = 14,
    FallingItem_15 = 15,
    PullRingRope_18 = 18,
    BackgroundAnimation_19 = 19,
    Honey_20 = 20,

    TimedMine_22 = 22,

    Slig_24 = 24,
    Slog_25 = 25,
    Switch_26 = 26,
    BellHammer_27 = 27,
    StartController_28 = 28,
    SecurityOrb_29 = 29,

    LiftMudokon_32 = 32,

    BeeSwarmHole_34 = 34,
    Pulley_35 = 35,
    HoneySack_36 = 36,
    AbeStart_37 = 37,
    ElumStart_38 = 38,

    ElumWall_40 = 40,
    SlingMudokon_41 = 41,
    HoneyDripTarget_42 = 42,
    Bees_43 = 43,

    WellExpress_45 = 45, // TODO: check if WellLocal and WellExpress are not swapped
    Mine_46 = 46,
    UXB_47 = 47,
    Paramite_48 = 48,
    Bat_49 = 49,
    RingMudokon_50 = 50,
    MovieStone_51 = 51,
    BirdPortal_52 = 52,
    BirdPortalExit_53 = 53,
    BellSongStone_54 = 54,
    TrapDoor_55 = 55,
    RollingBall_56 = 56,
    eSligBoundLeft_57 = 57,
    InvisibleZone_58 = 58,
    RollingBallStopper_59 = 59,
    FootSwitch_60 = 60,
    SecurityClaw_61 = 61,
    MotionDetector_62 = 62,

    SligSpawner_66 = 66,
    ElectricWall_67 = 67,
    LiftMover_68 = 68,
    ChimeLock_69 = 69,

    MeatSack_71 = 71,
    Scrab_72 = 72,
    FlintLockFire_73 = 73,
    ScrabLeftBound_74 = 74,
    ScrabRightBound_75 = 75,
    eSligBoundRight_76 = 76,
    eSligPersist_77 = 77,
    EnemyStopper_79 = 79,

    InvisibleSwitch_81 = 81,
    Mudokon_82 = 82,
    ZSligCover_83 = 83,
    DoorFlame_84 = 84,

    MovingBomb_86 = 86,
    MovingBombStopper_87 = 87,
    MeatSaw_88 = 88,
    MudokonPathTrans_89 = 89,
    MenuController_90 = 90,

    HintFly_92 = 92,
    ScrabNoFall_93 = 93,
    IdSplitter_94 = 94,
    SecurityDoor_95 = 95,
    DemoPlaybackStone_96 = 96,
    BoomMachine_97 = 97,
    LCDScreen_98 = 98,
    ElumPathTrans_99 = 99,
    HandStone_100 = 100,
    CreditsController_101 = 101,
    Preloader_102 = 102,
    LCDStatusBoard_103 = 103,

    MusicTrigger_105 = 105,
    LightEffect_106 = 106,
    SlogSpawner_107 = 107,
    DeathClock_108 = 108,
    RingCancel_109 = 109,
    GasEmitter_110 = 110,
    SlogHut_111 = 111,
    Glukkon_112 = 112,
    KillUnsavedMuds_113 = 113,
    SoftLanding_114 = 114,
    ResetPath_115 = 115,
};

struct Path_TLV;
class Map;

using TTempFn = s32(CC*)();

struct PathData
{
    TTempFn field_0;
    s16 field_4_bLeft;
    s16 field_6_bRight;
    s16 field_8_bTop;
    s16 field_A_bBottom;
    s16 field_C_grid_width;
    s16 field_E_grid_height;
    s16 field_10;
    s16 field_12;
    s32 field_14_object_offset;
    s32 field_18_object_index_table_offset;
    PathFunctionTable field_1C_object_funcs;
};

struct CollisionInfo;

using TCollisionsFactory = AddPointer_t<void(const CollisionInfo* pCollisionInfo, const u8* pPathData)>;

struct CollisionInfo
{
    TCollisionsFactory field_0_fn_ptr;
    s16 field_4_left;
    s16 field_6_right;
    s16 field_8_top;
    s16 field_A_bottom;
    u32 field_C_collision_offset;
    u32 field_10_num_collision_items;
    u32 field_14_grid_width;
    u32 field_18_grid_height;
};

struct PathBlyRec
{
    const char* field_0_blyName;
    const PathData* field_4_pPathData;
    const CollisionInfo* field_8_pCollisionData;
    WORD field_C_overlay_id;
    WORD field_E;
};

struct FmvInfo
{
    const char* field_0_pName;
    u16 field_4_id;
    s16 field_6;
    s16 field_8_stop_music;
    s16 field_A;
    s16 field_C_volume;
    s16 field_E;
};

struct SoundBlockInfo
{
    const char* field_0_vab_header_name;
    const char* field_4_vab_body_name;
    s32 field_8_vab_id;
    u8* field_C_pVabHeader;
};

struct PathRoot
{
    const PathBlyRec* field_0_pBlyArrayPtr;
    FmvInfo* field_4_pFmvArray;
    SoundBlockInfo* field_8_pMusicInfo;
    const char* field_C_bsq_file_name;
    s16 field_10_reverb;
    s16 field_12_bg_music_id;
    const char* field_14_lvl_name;
    s16 field_18_num_paths;
    s16 field_1A_unused; // message to display to change cd ??
    s32 field_1C_overlay_idx;
    const char* field_20_lvl_name_cd;
    s32 field_24;
    const char* field_28_ovl_name_cd;
    s32 field_2C;
    const char* field_30_mov_name_cd;
    const char* field_34_idx_name;
    const char* field_38_bnd_name;
};

struct PathRootContainer
{
    PathRoot paths[16];
};

EXPORT const PathBlyRec* CC Path_Get_Bly_Record_434650(LevelIds level, u16 path);

EXPORT FmvInfo* CC Path_Get_FMV_Record_434680(LevelIds levelId, u16 fmvId);

EXPORT s32 CC Path_Format_CameraName_4346B0(char* pNameBuffer, LevelIds level, s16 path, s16 camera);

extern PathRootContainer gMapData_4CAB58;

}
