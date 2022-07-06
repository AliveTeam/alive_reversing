#pragma once

#include "../AliveLibCommon/BitField.hpp"
#include "../AliveLibCommon/Psx_common.hpp"

namespace AO {




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
    Lever_26 = 26,
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
    TimerTrigger_94 = 94,
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
    GasCountDown_108 = 108,
    RingCancel_109 = 109,
    GasEmitter_110 = 110,
    ZzzSpawner_111 = 111,
    BackgroundGlukkon_112 = 112,
    KillUnsavedMuds_113 = 113,
    SoftLanding_114 = 114,
    ResetPath_115 = 115,
};


enum TlvFlags
{
    eBit1_Created = 0x1,
    eBit2_Destroyed = 0x2,
    eBit3_End_TLV_List = 0x4,
};

// ABI fix to allow using the enum as a 32bit type
struct TlvTypes32 final
{
    TlvTypes mType;
    s16 padto32Bits;

    bool operator==(TlvTypes type) const
    {
        return mType == type;
    }
};
static_assert(std::is_pod<TlvTypes32>::value, "TlvTypes32 must be pod");
ALIVE_ASSERT_SIZEOF(TlvTypes32, 4);

struct Path_TLV
{
    BitField8<TlvFlags> mTlvFlags;
    s8 field_1_unknown;
    s16 mLength;
    TlvTypes32 mTlvType32;
    s32 field_8;                 // only ever used as some sort of hacky memory overwrite check, always 0 in the Path data
    PSX_Point field_C_sound_pos; // a completely pointless copy of mTopLeft, the data is always the same in all released Paths
    PSX_Point mTopLeft;
    PSX_Point mBottomRight;

    // Note: Part of Path object in AE
    static Path_TLV* Next_446460(Path_TLV* pTlv);

    // Note: must be inlined as its used by the api
    static Path_TLV* Next(Path_TLV* pTlv)
    {
        if (pTlv->mTlvFlags.Get(TlvFlags::eBit3_End_TLV_List))
        {
            return nullptr;
        }

        return Next_NoCheck(pTlv);
    }

    // Note: must be inlined as its used by the api
    static Path_TLV* Next_NoCheck(Path_TLV* pTlv)
    {
        // Skip length bytes to get to the start of the next TLV
        u8* ptr = reinterpret_cast<u8*>(pTlv);
        u8* pNext = ptr + pTlv->mLength;
        return reinterpret_cast<Path_TLV*>(pNext);
    }

    static Path_TLV* TLV_Next_Of_Type_446500(Path_TLV* pTlv, TlvTypes type);

    // Some strange self terminate check that is inlined everywhere
    void RangeCheck()
    {
        if (mLength < 24u || mLength > 480u)
        {
            mTlvFlags.Set(eBit3_End_TLV_List);
        }
    }
};
ALIVE_ASSERT_SIZEOF(Path_TLV, 0x18);

class Path final
{
public:
    static void TLV_Reset(u32 tlvOffset_levelId_PathId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed);
};
} // namespace AO
