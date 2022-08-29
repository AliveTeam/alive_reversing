#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Psx.hpp"
#include "../AliveLibCommon/BitField.hpp"
#include "FixedPoint.hpp"
#include <type_traits>
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Factory.hpp"

struct PathData;

enum TlvFlags
{
    eBit1_Created = 0x1,
    eBit2_Destroyed = 0x2,
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
    Lever_17 = 17,
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
    RollingBall_31 = 31,
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
    ScrabBoundLeft_43 = 43,
    ScrabBoundRight_44 = 44,
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
    LCDScreen_60 = 60,
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
    ZzzSpawner_72 = 72,
    Glukkon_73 = 73,
    KillUnsavedMudokons_74 = 74,
    SoftLanding_75 = 75,
    ResetPath_76 = 76,
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
    Drill_90 = 90,
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
    BrewMachine_101 = 101,
    ScrabSpawner_102 = 102,
    CrawlingSlig_103 = 103,
    SligGetPants_104 = 104,
    SligGetWings_105 = 105,
    Greeter_106 = 106,
    CrawlingSligButton_107 = 107,
    GlukkonSwitch_108 = 108,
    DoorBlocker_109 = 109,
    TorturedMudokon_110 = 110,
    TrainDoor_111 = 111,
};

// ABI fix to allow using the enum as a 32bit type
struct TlvTypes32 final
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


// NOTE: enum created for kPathChangeEffectToInternalScreenChangeEffect_55D55C
enum ScreenChangeEffects : s16
{
    ePlay1FMV_0 = 0,
    eRightToLeft_1 = 1,
    eLeftToRight_2 = 2,
    eBottomToTop_3 = 3,
    eTopToBottom_4 = 4,
    eBoxOut_5 = 5,
    eVerticalSplit_6 = 6,
    eHorizontalSplit_7 = 7,
    eUnknown_8 = 8,
    eInstantChange_9 = 9,
};

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

enum class XDirection_int : s32
{
    eLeft_0 = 0,
    eRight_1 = 1,
};

enum class XDirection_short : s16
{
    eLeft_0 = 0,
    eRight_1 = 1,
};

enum class Choice_int : s32
{
    eNo_0 = 0,
    eYes_1 = 1,
};

enum class Choice_short : s16
{
    eNo_0 = 0,
    eYes_1 = 1,
};

struct Path_TLV
{
    BitField8<TlvFlags> mTlvFlags;
    u8 mTlvSpecificMeaning;
    s16 mLength;
    TlvTypes32 mTlvType32;
    PSX_Point mTopLeft;
    PSX_Point mBottomRight;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TLV, 0x10);


struct Path_EnemyStopper final : public Path_TLV
{
    enum class StopDirection : s16
    {
        Left_0 = 0,
        Right_1 = 1,
        Both_2 = 2,
    };
    StopDirection mStopDirection;
    s16 mSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_EnemyStopper, 0x14);


enum class LevelIds : s16;
enum class CameraPos : s16;
enum class LoadMode : s16;

class Path
{
public:
    Path();
    ~Path();
    void Free();
    void Init(const PathData* pPathData, EReliveLevelIds level, s16 path, s16 cameraId, u8** ppPathRes);


    void Loader_4DB800(s16 xpos, s16 ypos, LoadMode loadMode, ReliveTypes typeToLoad);

    relive::Path_TLV* Get_First_TLV_For_Offsetted_Camera(s16 cam_x_idx, s16 cam_y_idx);
    static relive::Path_TLV* Next_TLV(relive::Path_TLV* pTlv);

    // note: inline as used by the API
    static relive::Path_TLV* Next_TLV_Impl(relive::Path_TLV* pTlv)
    {
        if (pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
        {
            return nullptr;
        }

        // Skip length bytes to get to the start of the next TLV
        u8* ptr = reinterpret_cast<u8*>(pTlv);
        u8* pNext = ptr + pTlv->mLength;
        return reinterpret_cast<relive::Path_TLV*>(pNext);
    }

    relive::Path_TLV* TLV_First_Of_Type_In_Camera(ReliveTypes objectType, s16 camX);
    relive::Path_TLV* TLV_Get_At_4DB4B0(s16 xpos, s16 ypos, s16 width, s16 height, ReliveTypes objectType);
    relive::Path_TLV* TlvGetAt(relive::Path_TLV* pTlv, FP xpos, FP ypos, FP w, FP h);
    relive::Path_TLV* TLV_From_Offset_Lvl_Cam(const Guid& tlvId);

    Guid TLVInfo_From_TLVPtr(relive::Path_TLV* pTlv);

    static relive::Path_TLV* TLV_Next_Of_Type(relive::Path_TLV* pTlv, ReliveTypes type);
    static void TLV_Reset(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed);
    static void Start_Sounds_For_Objects_In_Camera(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx);

    static void Reset_TLVs(u16 pathId);

    EReliveLevelIds mLevelId = EReliveLevelIds::eNone;
    u16 mPathId = 0;
    s16 mCameraId = 0;
    u16 mCamsOnX = 0;
    u16 mCamsOnY = 0;
    const PathData* mPathData = nullptr;
    u8** field_10_ppRes = nullptr;
};
ALIVE_ASSERT_SIZEOF(Path, 0x14);

enum class CameraPos : s16;

void Stop_slig_sounds(CameraPos direction, s8 kZero);

struct Path_Teleporter_Data
{
    s16 mTeleporterId;
    s16 mOtherTeleporterId;
    s16 mDestCamera;
    s16 mDestPath;
    LevelIds mDestLevel;
    s16 mSwitchId;
    Scale_short mScale;
    ScreenChangeEffects mWipeEffect;
    s16 mMovieId;
    s16 mElectricX;
    s16 mElectricY;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Teleporter_Data, 0x16);

struct Path_Teleporter final : public Path_TLV
{
    Path_Teleporter_Data mData;
    s16 field_26_padding; // Actually padding here as the game won't copy these 2 bytes, but its included in the TLV length
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Teleporter, 0x28); // 0x10 for base

struct Path_Hoist final : public Path_TLV
{
    enum class Type : s16
    {
        eNextFloor = 0,
        eNextEdge = 1,
        eOffScreen = 2,
    };
    Type mHoistType;

    enum class GrabDirection : s16
    {
        eFacingLeft = 0,
        eFacingRight = 1,
        eFacingAnyDirection = 2,
    };
    GrabDirection mGrabDirection;
    u8 field_14_switch_id; // never used
    // pad

    Scale_short field_16_scale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Hoist, 0x18);

struct Path_Edge final : public Path_TLV
{
    enum class GrabDirection : s16
    {
        eFacingLeft = 0,
        eFacingRight = 1,
        eFacingAnyDirection = 2,
    };
    GrabDirection mGrabDirection;
    Choice_short mCanGrab;

    Scale_int mScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Edge, 0x18);

struct Path_SoftLanding final : public Path_TLV
{
    s32 mSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SoftLanding, 0x14);


struct Path_WellBase : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mOtherWellId;
    s16 mAnimId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WellBase, 0x18);

struct Path_WellLocal final : public Path_WellBase
{
    s16 field_18_off_dx;
    s16 field_1A_off_dy;
    s16 mOnDestX;
    s16 mOnDestY;
    Choice_short mEmitLeaves;
    s16 mLeafX;
    s16 mLeafY;
    s16 field_26_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WellLocal, 0x28);

struct Path_WellExpress final : public Path_WellBase
{
    s16 field_18_exit_x;
    s16 field_1A_exit_y;
    LevelIds field_1C_disabled_well_level;
    s16 field_1E_disabled_well_path;
    s16 mOffDestCamera;
    s16 mOffOtherWellId;
    LevelIds mOnDestLevel;
    s16 mOnDestPath;
    s16 mOnDestCamera;
    s16 mOnOtherWellId;
    Choice_short mEmitLeaves;
    s16 mLeafX;
    s16 mLeafY;
    s16 mMovieId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WellExpress, 0x34);

struct Path_Alarm final : public Path_TLV
{
    s16 mSwitchId;
    u16 mAlarmDuration;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Alarm, 0x14);

struct Path_MovieStone final : public Path_TLV
{
    s16 mMovieId;
    Scale_short mScale;
    s32 mTriggerSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MovieStone, 0x18);

struct Path_HandStone final : public Path_TLV
{
    Scale_short mScale;
    s16 mCameraId1;
    s16 mCameraId2;
    s16 mCameraId3;
    s32 mTriggerSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_HandStone, 0x1C);

struct Path_AbeStart final : public Path_TLV
{
    s16 field_10_xpos;
    s16 field_12_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_AbeStart, 0x14);

ALIVE_VAR_EXTERN(Path*, sPathInfo);
