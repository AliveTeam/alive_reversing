#pragma once

#include "FunctionFwd.hpp"
#include "Psx.hpp"
#include "BitField.hpp"
#include "FixedPoint.hpp"
#include <type_traits>

struct PathData;

enum TLV_Flags
{
    eBit1_Created = 0x1,
    eBit2_Unknown = 0x2,
    eBit3_End_TLV_List = 0x4,
};

enum class TlvTypes : s16
{
    None_m1 = -1,
    ContinuePoint_0 = 0,
    PathTransition_1 = 1,
    Hoist_2 = 2,
    Edge_3 = 3,
    DeathDrop_4 = 4,
    Door_5 = 5,
    ShadowZone_6 = 6,
    LiftPoint_7 = 7,
    LocalWell_8 = 8,
    Dove_9 = 9,
    RockSack_10 = 10,
    FallingItem_11 = 11,
    PullRingRope_12 = 12,
    BackgroundAnimation_13 = 13,
    TimedMine_14 = 14,
    Slig_15 = 15,
    Slog_16 = 16,
    Switch_17 = 17,
    Null_18 = 18,
    SecurityOrb_19 = 19,
    Null_20 = 20,
    Pulley_21 = 21,
    AbeStart_22 = 22,
    WellExpress_23 = 23,
    Mine_24 = 24,
    UXB_25 = 25,
    Paramite_26 = 26,
    MovieHandStone_27 = 27,
    BirdPortal_28 = 28,
    BirdPortalExit_29 = 29,
    TrapDoor_30 = 30,
    RollingRock_31 = 31,
    SligBoundLeft_32 = 32,
    InvisibleZone_33 = 33,
    FootSwitch_34 = 34,
    SecurityClaw_35 = 35,
    MotionDetector_36 = 36,
    SligSpawner_37 = 37,
    ElectricWall_38 = 38,
    LiftMover_39 = 39,
    MeatSack_40 = 40,
    Scrab_41 = 41,
    Null_42 = 42,
    ScrabLeftBound_43 = 43,
    ScrabRightBound_44 = 44,
    SligBoundRight_45 = 45,
    SligPersist_46 = 46,
    EnemyStopper_47 = 47,
    InvisibleSwitch_48 = 48,
    Mudokon_49 = 49,
    ZSligCover_50 = 50,
    DoorFlame_51 = 51,
    MovingBomb_52 = 52,
    MovingBombStopper_53 = 53,
    MainMenuController_54 = 54,
    Unknown_55 = 55,
    Null_56 = 56,
    TimerTrigger_57 = 57,
    SecurityDoor_58 = 58,
    BoomMachine_59 = 59,
    LCD_60 = 60,
    HandStone_61 = 61,
    CreditsController_62 = 62,
    Null_63 = 63,
    LCDStatusBoard_64 = 64,
    WheelSyncer_65 = 65,
    MusicTrigger_66 = 66,
    Light_67 = 67,
    SlogSpawner_68 = 68,
    GasCountdown_69 = 69,
    Unknown_70 = 70,
    GasEmitter_71 = 71,
    SlogHut_72 = 72,
    Glukkon_73 = 73,
    KillUnsavedMudokons_74 = 74,
    SoftLanding_75 = 75,
    ResetSwitchRange_76 = 76,
    Water_77 = 77,
    Null_78 = 78,
    WorkWheel_79 = 79,
    Null_80 = 80,
    LaughingGas_81 = 81,
    FlyingSlig_82 = 82,
    Fleech_83 = 83,
    Slurg_84 = 84,
    SlamDoor_85 = 85,
    LevelLoader_86 = 86,
    DemoSpawnPoint_87 = 87,
    Teleporter_88 = 88,
    SlurgSpawner_89 = 89,
    Grinder_90 = 90,
    ColourfulMeter_91 = 91,
    FlyingSligSpawner_92 = 92,
    MineCar_93 = 93,
    BoneBag_94 = 94,
    ExplosionSet_95 = 95,
    MultiSwitchController_96 = 96,
    StatusLight_97 = 97,
    SlapLock_98 = 98,
    ParamiteWebLine_99 = 99,
    Alarm_100 = 100,
    FartMachine_101 = 101,
    ScrabSpawner_102 = 102,
    NakedSlig_103 = 103,
    SligGetPants_104 = 104,
    SligGetWings_105 = 105,
    Greeter_106 = 106,
    NakedSligButton_107 = 107,
    GlukkonSwitch_108 = 108,
    DoorBlocker_109 = 109,
    TorturedMudokon_110 = 110,
    TrainDoor_111 = 111,
};

// ABI fix to allow using the enum as a 32bit type
struct TlvTypes32
{
    TlvTypes mType;
    s16 padTo32Bits;

    bool operator==(TlvTypes type) const
    {
        return mType == type;
    }

    TlvTypes32& operator=(TlvTypes type)
    {
        mType = type;
        padTo32Bits = 0;
        return *this;
    }
};
static_assert(std::is_pod<TlvTypes32>::value, "TlvTypes32 must be pod");
ALIVE_ASSERT_SIZEOF(TlvTypes32, 4);

enum class Scale_short : s16
{
    eFull_0 = 0,
    eHalf_1 = 1,
};

enum class Scale_int : s32
{
    eFull_0 = 0,
    eHalf_1 = 1,
};

enum class Direction
{
    eLeft = 0,
    eRight = 1,
};

enum class XDirection_short : s16
{
    eLeft_0 = 0,
    eRight_1 = 1,
};

enum class Choice_short : s16
{
    eNo_0 = 0,
    eYes_1 = 1,
};

struct Path_TLV
{
    BitField8<TLV_Flags> field_0_flags;
    u8 field_1_tlv_state;
    s16 field_2_length;
    TlvTypes32 field_4_type;
    PSX_Point field_8_top_left;
    PSX_Point field_C_bottom_right;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TLV, 0x10);


struct Path_EnemyStopper : public Path_TLV
{
    enum class StopDirection : s16
    {
        Left_0 = 0,
        Right_1 = 1,
        Both_2 = 2,
    };
    StopDirection field_10_stop_direction;
    s16 field_12_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_EnemyStopper, 0x14);


enum class LevelIds : s16;
enum class CameraPos : s16;
enum class LoadMode : s16;

class Path
{
public:
    EXPORT void ctor_4DB170();
    EXPORT void dtor_4DB1A0();
    EXPORT void Free_4DB1C0();
    EXPORT void Init_4DB200(const PathData* pPathData, LevelIds level, s16 path, s16 cameraId, u8** ppPathRes);


    EXPORT void Loader_4DB800(s16 xpos, s16 ypos, LoadMode loadMode, TlvTypes typeToLoad);

    EXPORT Path_TLV* Get_First_TLV_For_Offsetted_Camera_4DB610(s16 cam_x_idx, s16 cam_y_idx);
    EXPORT static Path_TLV* CCSTD Next_TLV_4DB6A0(Path_TLV* pTlv);
    EXPORT Path_TLV* TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes objectType, s16 camX);
    EXPORT Path_TLV* TLV_Get_At_4DB4B0(s16 xpos, s16 ypos, s16 width, s16 height, TlvTypes objectType);
    EXPORT Path_TLV* TLV_Get_At_4DB290(Path_TLV* pTlv, FP xpos, FP ypos, FP w, FP h);
    EXPORT Path_TLV* TLV_From_Offset_Lvl_Cam_4DB770(u32 tlvOffset_levelId_PathId);

    EXPORT u32 TLVInfo_From_TLVPtr_4DB7C0(Path_TLV* pTlv);

    EXPORT static Path_TLV* CCSTD TLV_Next_Of_Type_4DB720(Path_TLV* pTlv, TlvTypes type);
    EXPORT static void CCSTD TLV_Reset_4DB8E0(u32 tlvOffset_levelId_PathId, s16 hiFlags, s8 bSetCreated, s8 bBit2);
    EXPORT static void CC Start_Sounds_For_Objects_In_Camera_4CBAF0(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx);

    EXPORT static void CCSTD Reset_TLVs_4DBCF0(u16 pathId);

    LevelIds field_0_levelId;
    u16 field_2_pathId;
    s16 field_4_cameraId;
    u16 field_6_cams_on_x;
    u16 field_8_cams_on_y;
    s16 field_A_padding;
    const PathData* field_C_pPathData;
    u8** field_10_ppRes;
};
ALIVE_ASSERT_SIZEOF(Path, 0x14);

enum class CameraPos : s16;

EXPORT void CC Stop_slig_sounds_4CBA70(CameraPos direction, s8 kZero);

struct Path_Teleporter_Data
{
    s16 field_10_id;
    s16 field_12_target_id;
    s16 field_14_camera;
    s16 field_16_path;
    LevelIds field_18_level;
    s16 field_1A_trigger_id;
    Scale_short field_1C_scale;
    s16 field_1E_cam_swap_effect;
    s16 field_20_movie_number;
    s16 field_22_eletric_x;
    s16 field_24_electric_y;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Teleporter_Data, 0x16);

struct Path_Teleporter : public Path_TLV
{
    Path_Teleporter_Data field_10_data;
    s16 field_26_padding; // Actually padding here as the game won't copy these 2 bytes, but its included in the TLV length
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Teleporter, 0x28); // 0x10 for base

struct Path_Hoist : public Path_TLV
{
    enum class Type : s16
    {
        eNextFloor = 0,
        eNextEdge = 1,
        eOffScreen = 2,
    };
    Type field_10_type;

    enum class GrabDirection : s16
    {
        eLeft = 0,
        eRight = 1,
        eBoth = 2,
    };
    GrabDirection field_12_grab_direction;

    u8 field_14_id; // never used
    // pad

    Scale_short field_16_scale;
};
// TODO: Size

struct Path_Edge : public Path_TLV
{
    enum class GrabDirection : s16
    {
        eLeft = 0,
        eRight = 1,
        eBoth = 2,
    };
    GrabDirection field_10_grab_direction;
    s16 field_12_bCan_grab;

    Scale_int field_14_scale;
};
// TODO: Size

struct Path_SoftLanding : public Path_TLV
{
    s32 field_10_id;
};
// TODO: Size


struct Path_WellBase : public Path_TLV
{
    Scale_short field_0_scale;
    s16 field_2_trigger_id;
    s16 field_4_well_id;
    s16 field_6_res_id;
};
// TODO: Size

struct Path_WellLocal : public Path_WellBase
{
    s16 field_18_off_dx;
    s16 field_1A_off_dy;
    s16 field_1C_on_dx;
    s16 field_1E_on_dy;
    Choice_short field_20_bEmit_leaves;
    s16 field_22_leaf_x;
    s16 field_24_leaf_y;
    s16 field_26_padding;
};
// TODO: Size

struct Path_WellExpress : public Path_WellBase
{
    s16 field_18_exit_x;
    s16 field_1A_exit_y;
    LevelIds field_1C_disabled_well_level;
    s16 field_1E_disabled_well_path;
    s16 field_20_disabled_well_camera;
    s16 field_22_disabled_well_id;
    LevelIds field_24_enabled_well_level;
    s16 field_26_enabled_well_path;
    s16 field_28_enabled_well_camera;
    s16 field_2A_enabled_well_id;
    Choice_short field_2C_bEmit_leaves;
    s16 field_2E_leaf_x;
    s16 field_30_leaf_y;
    s16 field_32_movie_id;
};
// TODO: Size

struct Path_Alarm : public Path_TLV
{
    s16 field_10_id;
    u16 field_12_duration;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Alarm, 0x14);

struct Path_MovieStone : public Path_TLV
{
    s16 field_10_movie_number;
    Scale_short field_12_scale;
    s32 field_14_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovieStone, 0x18);

struct Path_HandStone : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_camera_id1;
    s16 field_12_camera_id2;
    s16 field_12_camera_id3;
    s32 field_18_trigger_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_HandStone, 0x1C);

struct Path_AbeStart : public Path_TLV
{
    s16 field_10_xpos;
    s16 field_12_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_AbeStart, 0x14);

ALIVE_VAR_EXTERN(Path*, sPath_dword_BB47C0);
