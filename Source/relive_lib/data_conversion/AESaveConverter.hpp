#pragma once

#include "guid.hpp"
#include "../ObjectTypes.hpp"
#include "../FixedPoint.hpp"
#include "../BitField.hpp"
#include "../../AliveLibAE/PathData.hpp"
#include "../FatalError.hpp"

#include "relive_tlvs.hpp"

#include "../../AliveLibAE/SlamDoor.hpp"
#include "../../AliveLibAE/SligSpawner.hpp"
#include "../../AliveLibAE/LiftMover.hpp"
#include "../../AliveLibAE/Bone.hpp"
#include "../../AliveLibAE/MinesAlarm.hpp"
#include "../../AliveLibAE/CrawlingSlig.hpp"
#include "../../AliveLibAE/Drill.hpp"
#include "../../AliveLibAE/EvilFart.hpp"
#include "../../AliveLibAE/GameEnderController.hpp"
#include "../../AliveLibAE/SlapLockWhirlWind.hpp"
#include "../../AliveLibAE/SlapLock.hpp"
#include "../../AliveLibAE/BirdPortal.hpp"
#include "../../AliveLibAE/ThrowableArray.hpp"
#include "../../AliveLibAE/ScrabSpawner.hpp"
#include "../../AliveLibAE/TimerTrigger.hpp"
#include "../../AliveLibAE/TrapDoor.hpp"
#include "../../AliveLibAE/UXB.hpp"
#include "../../AliveLibAE/WorkWheel.hpp"
#include "../../AliveLibAE/Slurg.hpp"
#include "../../AliveLibAE/LiftPoint.hpp"
#include "../../AliveLibAE/Slog.hpp"
#include "../../AliveLibAE/Rock.hpp"
#include "../../AliveLibAE/AbilityRing.hpp"
#include "../../AliveLibAE/Paramite.hpp"
#include "../../AliveLibAE/MineCar.hpp"
#include "../../AliveLibAE/Meat.hpp"
#include "../../AliveLibAE/Grenade.hpp"
#include "../../AliveLibAE/Greeter.hpp"
#include "../../AliveLibAE/Glukkon.hpp"
#include "../../AliveLibAE/Fleech.hpp"
#include "../../AliveLibAE/FlyingSlig.hpp"
#include "../../AliveLibAE/FlyingSligSpawner.hpp"
#include "../../AliveLibAE/Abe.hpp"
#include "../../AliveLibAE/Mudokon.hpp"
#include "../../AliveLibAE/Scrab.hpp"

#include "../../AliveLibAE/QuikSave.hpp"

#include "nlohmann/json_fwd.hpp"

// Any enum/struct in the AEData namespace is related to OG data and can't ever be changed
// otherwise interpreting the OG data will break.
namespace AEData 
{ 
struct PSX_RECT final
{
    s16 x, y, w, h;
};
ALIVE_ASSERT_SIZEOF(PSX_RECT, 8);

static ::PSX_RECT From(const PSX_RECT& data)
{
    ::PSX_RECT d;
    d.x = data.x;
    d.y = data.y;
    d.w = data.w;
    d.h = data.h;
    return d;
}

struct TlvOffsetLevelIdPathId final
{
    u16 tlvOffset;
    u8 levelId;
    u8 pathId;
};

struct TlvOffsetCombinedLevelIdPathId final
{
    u16 tlvOffset;
    u16 levelIdPathId;
};

union TlvItemInfoUnion
{
    u32 all;
    TlvOffsetCombinedLevelIdPathId combined;
    TlvOffsetLevelIdPathId parts;
};
ALIVE_ASSERT_SIZEOF(TlvItemInfoUnion, 4);

enum eMudMotions : s16
{
    Motion_0_Idle,
    Motion_1_WalkLoop,
    Motion_2_StandingTurn,

    // NOTE: 3-6 are all the same entry, added to avoid compile issue mentioned above.
    M_Speak_3_472FA0,
    M_Speak_4_472FA0,
    M_Speak_5_472FA0,
    M_Speak_6_472FA0,

    Motion_7_WalkBegin,
    Motion_8_WalkToIdle,
    Motion_9_MidWalkToIdle,
    Motion_10_LeverUse,
    Motion_11_Chisel,
    Motion_12_StartChisel,
    Motion_13_StopChisel,
    Motion_14_CrouchScrub,
    Motion_15_CrouchIdle,
    Motion_16_CrouchTurn,
    Motion_17_StandToCrouch,
    Motion_18_CrouchToStand,
    Motion_19_WalkToRun,
    Motion_20_MidWalkToRun,
    Motion_21_RunLoop,
    Motion_22_RunToWalk,
    Motion_23_MidRunToWalk,
    Motion_24_RunSlideStop,
    Motion_25_RunSlideTurn,
    Motion_26_RunTurnToRun,
    Motion_27_SneakLoop,
    Motion_28_MidWalkToSneak,
    Motion_29_SneakToWalk,
    Motion_30_WalkToSneak,
    Motion_31_MidSneakToWalk,
    Motion_32_SneakBegin,
    Motion_33_SneakToIdle,
    Motion_34_MidSneakToIdle,
    Motion_35_RunJumpBegin,
    Motion_36_RunJumpMid,
    Motion_37_StandToRun,
    Motion_38_Punch,
    Motion_39_HoistBegin,
    Motion_40_HoistLand,
    Motion_41_LandSoft1,
    Motion_42_LandSoft2,
    Motion_43_DunnoBegin,
    Motion_44_DunnoEnd,
    Motion_45_KnockForward,
    Motion_46_Knockback,
    Motion_47_KnockbackGetUp,
    Motion_48_WalkOffEdge,
    Motion_49_Fall,
    Motion_50_Chant,
    Motion_51_ChantEnd,
    Motion_52_ToDuck,
    Motion_53_Duck,
    Motion_54_DuckToCrouch,
    Motion_55_DuckKnockback,
    Motion_56_SlapOwnHead,
    Motion_57_TurnWheelBegin,
    Motion_58_TurnWheelLoop,
    Motion_59_TurnWheelEnd
};

enum class GameSpeakEvents : s16
{
    eSameAsLast_m2 = -2,
    eNone_m1 = -1,
    eUnknown_0 = 0,
    eUnknown_1 = 1,
    eUnknown_2 = 2,
    eFart_3 = 3,
    eUnknown_4 = 4,
    Slig_BS_5 = 5,
    Slig_LookOut_6 = 6,
    Slig_BS2_7 = 7,
    Slig_Laugh_8 = 8,
    eHello_9 = 9,
    eFollowMe_10 = 10,
    eAnger_11 = 11,
    eWait_12 = 12,
    eUnknown_13 = 13,
    eUnknown_14 = 14,
    eUnknown_15 = 15,
    eUnknown_16 = 16,
    eUnknown_17 = 17,
    eUnknown_18 = 18,
    eUnknown_19 = 19,
    eUnknown_20 = 20,
    eWork_21 = 21,
    eStopIt_22 = 22,
    eAllYa_23 = 23,
    eSorry_24 = 24,
    eUnknown_25 = 25,
    eUnknown_26 = 26,
    Slig_Hi_27 = 27,
    Slig_HereBoy_28 = 28,
    Slig_GetEm_29 = 29,
    eUnknown_30 = 30,
    Slig_Freeze_31 = 31,
    eUnknown_32 = 32,
    eUnknown_33 = 33,
    eUnknown_34 = 34,
    eUnknown_35 = 35, // Glukkon_None_35?
    Glukkon_Hey_36 = 36,
    Glukkon_DoIt_37 = 37,
    Glukkon_StayHere_38 = 38,
    Glukkon_Commere_39 = 39,
    Glukkon_AllOYa_40 = 40,
    Glukkon_Heh_41 = 41,
    Glukkon_Help_42 = 42,
    Glukkon_Laugh_43 = 43,
    Glukkon_KillEm_44 = 44,
    Glukkon_Unknown_45 = 45,
    Glukkon_Unknown_46 = 46,
    Glukkon_What_47 = 47,

    Paramite_Howdy_48 = 48,
    Paramite_Stay_49 = 49,
    Paramite_CMon_or_Attack_50 = 50,
    Paramite_DoIt_51 = 51,
    Paramite_AllYa_52 = 52,

    Scrab_Howl_53 = 53,
    Scrab_Shriek_54 = 54,
};

enum eLineTypes : s16
{
    eNone_m1 = -1,
    eFloor_0 = 0,
    eWallLeft_1 = 1,
    eWallRight_2 = 2,
    eCeiling_3 = 3,
    eBackgroundFloor_4 = 4,
    eBackgroundWallLeft_5 = 5,
    eBackgroundWallRight_6 = 6,
    eBackgroundCeiling_7 = 7,
    eTrackLine_8 = 8,
    eArt_9 = 9,
    eBulletWall_10 = 10,
    eMineCarFloor_11 = 11,
    eMineCarWall_12 = 12,
    eMineCarCeiling_13 = 13,
    eBackgroundMineCarFloor_14 = 14,
    eBackgroundMineCarWall_15 = 15,
    eBackgroundMineCarCeiling_16 = 16,
    eFlyingObjectWall_17 = 17,
    eBackgroundFlyingObjectWall_18 = 18,

    eDynamicCollision_32 = 32,
    eBackgroundDynamicCollision_36 = 36,
};

static ::eLineTypes From(const eLineTypes type)
{
    switch (type)
    {
        case eLineTypes::eNone_m1:
            return ::eLineTypes::eNone_m1;
        case eLineTypes::eFloor_0:
            return ::eLineTypes::eFloor_0;
        case eLineTypes::eWallLeft_1:
            return ::eLineTypes::eWallLeft_1;
        case eLineTypes::eWallRight_2:
            return ::eLineTypes::eWallRight_2;
        case eLineTypes::eCeiling_3:
            return ::eLineTypes::eCeiling_3;
        case eLineTypes::eBackgroundFloor_4:
            return ::eLineTypes::eBackgroundFloor_4;
        case eLineTypes::eBackgroundWallLeft_5:
            return ::eLineTypes::eBackgroundWallLeft_5;
        case eLineTypes::eBackgroundWallRight_6:
            return ::eLineTypes::eBackgroundWallRight_6;
        case eLineTypes::eBackgroundCeiling_7:
            return ::eLineTypes::eBackgroundCeiling_7;
        case eLineTypes::eTrackLine_8:
            return ::eLineTypes::eTrackLine_8;
        case eLineTypes::eArt_9:
            return ::eLineTypes::eArt_9;
        case eLineTypes::eBulletWall_10:
            return ::eLineTypes::eBulletWall_10;
        case eLineTypes::eMineCarFloor_11:
            return ::eLineTypes::eMineCarFloor_11;
        case eLineTypes::eMineCarWall_12:
            return ::eLineTypes::eMineCarWall_12;
        case eLineTypes::eMineCarCeiling_13:
            return ::eLineTypes::eMineCarCeiling_13;
        case eLineTypes::eBackgroundMineCarFloor_14:
            return ::eLineTypes::eBackgroundMineCarFloor_14;
        case eLineTypes::eBackgroundMineCarWall_15:
            return ::eLineTypes::eBackgroundMineCarWall_15;
        case eLineTypes::eBackgroundMineCarCeiling_16:
            return ::eLineTypes::eBackgroundMineCarCeiling_16;
        case eLineTypes::eFlyingObjectWall_17:
            return ::eLineTypes::eFlyingObjectWall_17;
        case eLineTypes::eBackgroundFlyingObjectWall_18:
            return ::eLineTypes::eBackgroundFlyingObjectWall_18;
        case eLineTypes::eDynamicCollision_32:
            return ::eLineTypes::eDynamicCollision_32;
        case eLineTypes::eBackgroundDynamicCollision_36:
            return ::eLineTypes::eBackgroundDynamicCollision_36;
    }
    ALIVE_FATAL("Bad line type value");
}

static ::GameSpeakEvents From(const GameSpeakEvents event)
{
    switch (event)
    {
        case GameSpeakEvents::eSameAsLast_m2:
            return ::GameSpeakEvents::eSameAsLast_m2;
        case GameSpeakEvents::eNone_m1:
            return ::GameSpeakEvents::eNone_m1;
        case GameSpeakEvents::eUnknown_0:
            return ::GameSpeakEvents::eUnknown_0;
        case GameSpeakEvents::eUnknown_1:
            return ::GameSpeakEvents::eUnknown_1;
        case GameSpeakEvents::eUnknown_2:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eFart_3:
            return ::GameSpeakEvents::eAbe_Fart_3;
        case GameSpeakEvents::eUnknown_4:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::Slig_BS_5:
            return ::GameSpeakEvents::Slig_BS_5;
        case GameSpeakEvents::Slig_LookOut_6:
            return ::GameSpeakEvents::Slig_LookOut_6;
        case GameSpeakEvents::Slig_BS2_7:
            return ::GameSpeakEvents::Slig_BS2_7;
        case GameSpeakEvents::Slig_Laugh_8:
            return ::GameSpeakEvents::Slig_Laugh_8;
        case GameSpeakEvents::eHello_9:
            return ::GameSpeakEvents::eAbe_Hello_9;
        case GameSpeakEvents::eFollowMe_10:
            return ::GameSpeakEvents::eAbe_FollowMe_10;
        case GameSpeakEvents::eAnger_11:
            return ::GameSpeakEvents::eAbe_Anger_11;
        case GameSpeakEvents::eWait_12:
            return ::GameSpeakEvents::eAbe_Wait_12;
        case GameSpeakEvents::eUnknown_13:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_14:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_15:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_16:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_17:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_18:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_19:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_20:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eWork_21:
            return ::GameSpeakEvents::eAbe_Work_21;
        case GameSpeakEvents::eStopIt_22:
            return ::GameSpeakEvents::eAbe_StopIt_22;
        case GameSpeakEvents::eAllYa_23:
            return ::GameSpeakEvents::eAbe_AllYa_23;
        case GameSpeakEvents::eSorry_24:
            return ::GameSpeakEvents::eAbe_Sorry_24;
        case GameSpeakEvents::eUnknown_25:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_26:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::Slig_Hi_27:
            return ::GameSpeakEvents::eSlig_Hi_27;
        case GameSpeakEvents::Slig_HereBoy_28:
            return ::GameSpeakEvents::eSlig_HereBoy_28;
        case GameSpeakEvents::Slig_GetEm_29:
            return ::GameSpeakEvents::eSlig_GetEm_29;
        case GameSpeakEvents::eUnknown_30:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::Slig_Freeze_31:
            return ::GameSpeakEvents::eSlig_Freeze_31;
        case GameSpeakEvents::eUnknown_32:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_33:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_34:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::eUnknown_35:
            return ::GameSpeakEvents::eGlukkon_None_35;
        case GameSpeakEvents::Glukkon_Hey_36:
            return ::GameSpeakEvents::eGlukkon_Hey_36;
        case GameSpeakEvents::Glukkon_DoIt_37:
            return ::GameSpeakEvents::eGlukkon_DoIt_37;
        case GameSpeakEvents::Glukkon_StayHere_38:
            return ::GameSpeakEvents::eGlukkon_StayHere_38;
        case GameSpeakEvents::Glukkon_Commere_39:
            return ::GameSpeakEvents::eGlukkon_Commere_39;
        case GameSpeakEvents::Glukkon_AllOYa_40:
            return ::GameSpeakEvents::eGlukkon_AllOYa_40;
        case GameSpeakEvents::Glukkon_Heh_41:
            return ::GameSpeakEvents::eGlukkon_Heh_41;
        case GameSpeakEvents::Glukkon_Help_42:
            return ::GameSpeakEvents::eGlukkon_Help_42;
        case GameSpeakEvents::Glukkon_Laugh_43:
            return ::GameSpeakEvents::eGlukkon_Laugh_43;
        case GameSpeakEvents::Glukkon_KillEm_44:
            return ::GameSpeakEvents::eGlukkon_KillEm_44;
        case GameSpeakEvents::Glukkon_Unknown_45:
            return ::GameSpeakEvents::eGlukkon_Unknown_45;
        case GameSpeakEvents::Glukkon_Unknown_46:
            ALIVE_FATAL("GameSpeakEvents %d isn't used", static_cast<s32>(event));
        case GameSpeakEvents::Glukkon_What_47:
            return ::GameSpeakEvents::eGlukkon_What_47;
        case GameSpeakEvents::Paramite_Howdy_48:
            return ::GameSpeakEvents::eParamite_Howdy_48;
        case GameSpeakEvents::Paramite_Stay_49:
            return ::GameSpeakEvents::eParamite_Stay_49;
        case GameSpeakEvents::Paramite_CMon_or_Attack_50:
            return ::GameSpeakEvents::eParamite_CMon_or_Attack_50;
        case GameSpeakEvents::Paramite_DoIt_51:
            return ::GameSpeakEvents::eParamite_DoIt_51;
        case GameSpeakEvents::Paramite_AllYa_52:
            return ::GameSpeakEvents::eParamite_AllYa_52;
        case GameSpeakEvents::Scrab_Howl_53:
            return ::GameSpeakEvents::eScrab_Howl_53;
        case GameSpeakEvents::Scrab_Shriek_54:
            return ::GameSpeakEvents::eScrab_Shriek_54;
        default:
            // For bad OG save data
            LOG_WARNING("Bad gamespeak event value %d", static_cast<u32>(event));
            return ::GameSpeakEvents::eNone_m1;
    }
}

#include "../AliveLibAE/MudokonEnums.hpp"

static ::Mud_Emotion From(const Mud_Emotion emo)
{
    switch (emo)
    {
        case Mud_Emotion::eNormal_0:
            return ::Mud_Emotion::eNormal_0;
        case Mud_Emotion::eAngry_1:
            return ::Mud_Emotion::eAngry_1;
        case Mud_Emotion::eAggressive_2:
            return ::Mud_Emotion::eAggressive_2;
        case Mud_Emotion::eSad_3:
            return ::Mud_Emotion::eSad_3;
        case Mud_Emotion::eSuicidal_4:
            return ::Mud_Emotion::eSuicidal_4;
        case Mud_Emotion::eHappy_5:
            return ::Mud_Emotion::eHappy_5;
        case Mud_Emotion::eWired_6:
            return ::Mud_Emotion::eWired_6;
        case Mud_Emotion::eSick_7:
            return ::Mud_Emotion::eSick_7;
    }
    ALIVE_FATAL("Bad mudokon emotion value");
}

enum class Scale : s16
{
    Bg = 0,
    Fg = 1,
};

static ::Scale From(const Scale scale)
{
    switch (scale)
    {
        case Scale::Bg:
            return ::Scale::Bg;
        case Scale::Fg:
            return ::Scale::Fg;
    }
    ALIVE_FATAL("Bad scale value");
}

// Shared enum for sligs
enum class SligSpeak : s8
{
    eNone = -1,
    eHi_0 = 0,
    eHereBoy_1 = 1,
    eGetHim_2 = 2,
    eLaugh_3 = 3,
    eStay_4 = 4,
    eBullshit_5 = 5,
    eLookOut_6 = 6,
    eBullshit2_7 = 7,
    eFreeze_8 = 8,
    eWhat_9 = 9,
    eHelp_10 = 10,
    eBlurgh_11 = 11,
    eGotYa_12 = 12,
    eOuch1_13 = 13,
    eOuch2_14 = 14,
};

static ::SligSpeak From(const SligSpeak speak)
{
    switch (speak)
    {
        case SligSpeak::eNone:
            return ::SligSpeak::eNone;
        case SligSpeak::eHi_0:
            return ::SligSpeak::eHi_0;
        case SligSpeak::eHereBoy_1:
            return ::SligSpeak::eHereBoy_1;
        case SligSpeak::eGetHim_2:
            return ::SligSpeak::eGetHim_2;
        case SligSpeak::eLaugh_3:
            return ::SligSpeak::eLaugh_3;
        case SligSpeak::eStay_4:
            return ::SligSpeak::eStay_4;
        case SligSpeak::eBullshit_5:
            return ::SligSpeak::eBullshit_5;
        case SligSpeak::eLookOut_6:
            return ::SligSpeak::eLookOut_6;
        case SligSpeak::eBullshit2_7:
            return ::SligSpeak::eBullshit2_7;
        case SligSpeak::eFreeze_8:
            return ::SligSpeak::eFreeze_8;
        case SligSpeak::eWhat_9:
            return ::SligSpeak::eWhat_9;
        case SligSpeak::eHelp_10:
            return ::SligSpeak::eHelp_10;
        case SligSpeak::eBlurgh_11:
            return ::SligSpeak::eBlurgh_11;
        case SligSpeak::eGotYa_12:
            return ::SligSpeak::eGotYa_12;
        case SligSpeak::eOuch1_13:
            return ::SligSpeak::eOuch1_13;
        case SligSpeak::eOuch2_14:
            return ::SligSpeak::eOuch2_14;
        default:
            LOG_WARNING("Bad slig speak value %d", static_cast<u32>(speak));
            return ::SligSpeak::eNone;
    }
}

enum class MudSounds : s16
{
    eNone = -1,
    eEmpty_0 = 0,
    eEmpty_1 = 1,
    eEmpty_2 = 2,
    eHelloNeutral_3 = 3,
    eFollowMe_4 = 4,
    eAnger_5 = 5,
    eWait_6 = 6,
    eFart_7 = 7,
    eGiggle_8 = 8,
    eHurt2_9 = 9,
    eLaugh_10 = 10,
    eGoodbye_11 = 11,
    eOkay_12 = 12,
    eNuhUh_13 = 13,
    eOops_14 = 14,
    eDeathDropScream_15 = 15,
    eHurt1_16 = 16,
    eAllOYa_17 = 17,
    eHiAngry_18 = 18,
    eHiHappy_19 = 19,
    eHiSad_20 = 20,
    eNoAngry_21 = 21,
    eNoSad_22 = 22,
    eFartPuh_23 = 23,
    eSick_24 = 24,
    eWork_25 = 25,
    eStopIt_26 = 26,
    eSorry_27 = 27,
    eSadUgh_28 = 28,
};

static ::MudSounds From(const MudSounds snd)
{
    switch (snd)
    {
        case MudSounds::eNone:
            return ::MudSounds::eNone;
        case MudSounds::eEmpty_0:
            return ::MudSounds::eEmpty_0;
        case MudSounds::eEmpty_1:
            return ::MudSounds::eEmpty_1;
        case MudSounds::eEmpty_2:
            return ::MudSounds::eEmpty_2;
        case MudSounds::eHelloNeutral_3:
            return ::MudSounds::eHelloNeutral_3;
        case MudSounds::eFollowMe_4:
            return ::MudSounds::eFollowMe_4;
        case MudSounds::eAnger_5:
            return ::MudSounds::eAnger_5;
        case MudSounds::eWait_6:
            return ::MudSounds::eWait_6;
        case MudSounds::eFart_7:
            return ::MudSounds::eFart_7;
        case MudSounds::eGiggle_8:
            return ::MudSounds::eGiggle_8;
        case MudSounds::eHurt2_9:
            return ::MudSounds::eHurt2_9;
        case MudSounds::eLaugh_10:
            return ::MudSounds::eLaugh_10;
        case MudSounds::eGoodbye_11:
            return ::MudSounds::eGoodbye_11;
        case MudSounds::eOkay_12:
            return ::MudSounds::eOkay_12;
        case MudSounds::eNuhUh_13:
            return ::MudSounds::eNuhUh_13;
        case MudSounds::eOops_14:
            return ::MudSounds::eOops_14;
        case MudSounds::eDeathDropScream_15:
            return ::MudSounds::eDeathDropScream_15;
        case MudSounds::eHurt1_16:
            return ::MudSounds::eHurt1_16;
        case MudSounds::eAllOYa_17:
            return ::MudSounds::eAllOYa_17;
        case MudSounds::eHiAngry_18:
            return ::MudSounds::eHiAngry_18;
        case MudSounds::eHiHappy_19:
            return ::MudSounds::eHiHappy_19;
        case MudSounds::eHiSad_20:
            return ::MudSounds::eHiSad_20;
        case MudSounds::eNoAngry_21:
            return ::MudSounds::eNoAngry_21;
        case MudSounds::eNoSad_22:
            return ::MudSounds::eNoSad_22;
        case MudSounds::eFartPuh_23:
            return ::MudSounds::eFartPuh_23;
        case MudSounds::eSick_24:
            return ::MudSounds::eSick_24;
        case MudSounds::eWork_25:
            return ::MudSounds::eWork_25;
        case MudSounds::eStopIt_26:
            return ::MudSounds::eStopIt_26;
        case MudSounds::eSorry_27:
            return ::MudSounds::eSorry_27;
        case MudSounds::eSadUgh_28:
            return ::MudSounds::eSadUgh_28;
    }
    ALIVE_FATAL("Bad mudokon sound value");
}

struct SligSpawnerSaveState final
{
    AETypes mType;
    s16 padding1;
    s32 mTlvInfo;
    enum class SpawnerStates : s16
    {
        eInactive_0 = 0,
        eSligSpawned_1 = 1,
    };
    SpawnerStates mState;
    s16 padding2;
    s32 mSpawnedSligId;

    static ::SligSpawnerSaveState From(const SligSpawnerSaveState& data)
    {
        ::SligSpawnerSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        d.mState = From(data.mState);
        d.mSpawnedSligId = Guid::NewGuidFromTlvInfo(data.mSpawnedSligId);
        return d;
    }

    static ::SpawnerStates From(const SpawnerStates state)
    {
        switch (state)
        {
            case SpawnerStates::eInactive_0:
                return ::SpawnerStates::eInactive_0;
            case SpawnerStates::eSligSpawned_1:
                return ::SpawnerStates::eSligSpawned_1;
        }
        ALIVE_FATAL("Bad slig spawner state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SligSpawnerSaveState, 0x10);

struct LiftMoverSaveState final
{
    AETypes mType;
    s16 field_2_padding;
    s32 mTlvInfo;
    enum class LiftMoverStates : s16
    {
        eInactive_0 = 0,
        eStartMovingDown_1 = 1,
        eMovingDown_2 = 2,
        eStartMovingUp_3 = 3,
        eMovingUp_4 = 4,
        eMovingDone_5 = 5,
    };
    LiftMoverStates mState;

    static ::LiftMoverSaveState From(const LiftMoverSaveState& data)
    {
        ::LiftMoverSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        d.mState = From(data.mState);
        return d;
    }

    static ::LiftMoverStates From(const LiftMoverStates state)
    {
        switch (state)
        {
            case LiftMoverStates::eInactive_0:
                return ::LiftMoverStates::eInactive_0;
            case LiftMoverStates::eStartMovingDown_1:
                return ::LiftMoverStates::eStartMovingDown_1;
            case LiftMoverStates::eMovingDown_2:
                return ::LiftMoverStates::eMovingDown_2;
            case LiftMoverStates::eStartMovingUp_3:
                return ::LiftMoverStates::eStartMovingUp_3;
            case LiftMoverStates::eMovingUp_4:
                return ::LiftMoverStates::eMovingUp_4;
            case LiftMoverStates::eMovingDone_5:
                return ::LiftMoverStates::eMovingDone_5;
        }
        ALIVE_FATAL("Bad lift mover state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(LiftMoverSaveState, 0xC);

struct BoneSaveState final
{
    AETypes mType;
    s16 field_2_padding;
    s32 mBaseTlvId;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    FP mSpriteScale;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;

    enum BoneStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8,
        eBit5_bHitObject = 0x10,
    };

    BitField16<BoneStateFlags> field_20_flags;
    s16 field_22_padding;
    s32 mPlatformTlvInfo;
    eLineTypes mCollisionLineType;
    s16 mBaseThrowableCount;
    enum class BoneStates : s16
    {
        eSpawned_0 = 0,
        eAirborne_1 = 1,
        eCollided_2 = 2,
        eOnGround_3 = 3,
        eEdible_4 = 4,
        eFalling_5 = 5
    };
    BoneStates mState;
    s16 mVolumeModifier;
    FP mInitialXPos;
    FP mInitialYPos;
    s32 mTimeToLiveTimer;

    static ::BoneSaveState From(const BoneSaveState& data)
    {
        ::BoneSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mBaseTlvId = Guid::NewGuidFromTlvInfo(data.mBaseTlvId);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mSpriteScale = data.mSpriteScale;
        d.mCurrentPath = data.mCurrentPath;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.mCurrentLevel);
        d.mRender = data.field_20_flags.Get(BoneStateFlags::eBit1_bRender);
        d.mDrawable = data.field_20_flags.Get(BoneStateFlags::eBit2_bDrawable);
        d.mLoop = data.field_20_flags.Get(BoneStateFlags::eBit3_bLoop);
        d.mInteractive = data.field_20_flags.Get(BoneStateFlags::eBit4_bInteractive);
        d.mHitObject = data.field_20_flags.Get(BoneStateFlags::eBit5_bHitObject);
        d.mPlatformId = Guid::NewGuidFromTlvInfo(data.mPlatformTlvInfo);
        d.mCollisionLineType = AEData::From(data.mCollisionLineType);
        d.mThrowableCount = data.mBaseThrowableCount;
        d.mState = From(data.mState);
        d.mBounceCount = data.mVolumeModifier;
        d.mPreviousXPos = data.mInitialXPos;
        d.mPreviousYPos = data.mInitialYPos;
        d.mTimeToLiveTimer = data.mTimeToLiveTimer;
        return d;
    }

    static ::BoneStates From(const BoneStates state)
    {
        switch (state)
        {
            case BoneStates::eSpawned_0:
                return ::BoneStates::eSpawned_0;
            case BoneStates::eAirborne_1:
                return ::BoneStates::eAirborne_1;
            case BoneStates::eCollided_2:
                return ::BoneStates::eCollided_2;
            case BoneStates::eOnGround_3:
                return ::BoneStates::eOnGround_3;
            case BoneStates::eEdible_4:
                return ::BoneStates::eEdible_4;
            case BoneStates::eFalling_5:
                return ::BoneStates::eFalling_5;
        }
        ALIVE_FATAL("Bad bone states value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(BoneSaveState, 0x3C);

struct MinesAlarmSaveState final
{
    AETypes mType;
    s16 field_2_pad;
    s32 mExplosionTimer;

    static ::MinesAlarmSaveState From(const MinesAlarmSaveState& data)
    {
        ::MinesAlarmSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mExplosionTimer = data.mExplosionTimer;
        return d;
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MinesAlarmSaveState, 0x8);

struct CrawlingSligSaveState final
{
    enum class CrawlingSligMotion : s16
    {
        None = -1,
        Motion_0_Idle = 0,
        Motion_1_UsingButton = 1,
        Motion_2_WakingUp = 2,
        Motion_3_Crawling = 3,
        Motion_4_StartFalling = 4,
        Motion_5_Falling = 5,
        Motion_6_Landing = 6,
        Motion_7_ToShakingToIdle = 7,
        Motion_8_Speaking = 8,
        Motion_9_Snoozing = 9,
        Motion_10_PushingWall = 10,
        Motion_11_TurnAround = 11,
        Motion_12_Shaking = 12,
        Motion_13_Empty = 13,
        Motion_14_ShakingToIdle = 14,
        Motion_15_EndCrawling = 15,
        Motion_16_IdleToPushingWall = 16,
        Motion_17_EndPushingWall = 17
    };

    enum eCrawlingSligBrains : s32
    {
        Brain_0_Sleeping = 0,
        Brain_1_Idle = 1,
        Brain_2_PanicGetALocker = 2,
        Brain_3_Possessed = 3,
        Brain_4_GetKilled = 4,
        Brain_5_Transformed = 5
    };

    AETypes mType;
    s16 field_2_padding;
    s32 mBaseTlvId;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;
    FP mSpriteScale;
    s16 mR;
    s16 mG;
    s16 mB;
    s16 mFlipX;
    CrawlingSligMotion mCurrentMotion;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mRender;
    s8 mDrawable;
    FP mHealth;
    CrawlingSligMotion mCurrentMotion2;
    CrawlingSligMotion mNextMotion;
    s16 mLastLineYPos;
    eLineTypes mCollisionLineType;
    s16 field_3C_padding;
    s16 field_3E_padding;
    s8 mControlled;
    s8 field_41_padding;
    s16 field_42_padding;
    s32 mCrawlingSligTlvInfo;
    eCrawlingSligBrains mBrainState;
    s16 field_4C_padding;
    s16 field_4E_padding;
    s16 mBrainSubState;
    s16 field_52_padding;
    s32 mMultiUseTimer;
    FP mVelxScaleFactor;
    s16 field_5C_padding;
    s16 mChanting;
    LevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    s16 field_66_unused;
    s32 field_68_unused;
    s32 mSligButtonTlvInfo;
    s32 field_70_obj_id;
    s32 mTransformedSligTlvInfo;
    SligSpeak field_78_speak;
    s8 field_79_padding;
    s16 field_7A_unused_counter;
    s32 mSayHelpTimer;

    static ::CrawlingSligSaveState From(const CrawlingSligSaveState& data)
    {
        ::CrawlingSligSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mBaseTlvId = Guid::NewGuidFromTlvInfo(data.mBaseTlvId);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mCurrentPath = data.mCurrentPath;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.mCurrentLevel);
        d.mSpriteScale = data.mSpriteScale;
        d.mR = data.mR;
        d.mG = data.mG;
        d.mB = data.mB;
        d.mFlipX = data.mFlipX;
        d.mCurrentMotion = From(data.mCurrentMotion);
        d.mCurrentFrame = data.mCurrentFrame;
        d.mFrameChangeCounter = data.mFrameChangeCounter;
        d.mRender = data.mRender;
        d.mDrawable = data.mDrawable;
        d.mHealth = data.mHealth;
        d.mCurrentMotion2 = From(data.mCurrentMotion2);
        d.mNextMotion = From(data.mNextMotion);
        d.mLastLineYPos = data.mLastLineYPos;
        d.mCollisionLineType = AEData::From(data.mCollisionLineType);
        d.mControlled = data.mControlled;
        d.mCrawlingSligTlvId = Guid::NewGuidFromTlvInfo(data.mCrawlingSligTlvInfo);
        d.mBrainType = From(data.mBrainState);
        d.mBrainSubState = data.mBrainSubState; // TODO: convert enums
        d.mMultiUseTimer = data.mMultiUseTimer;
        d.mVelxScaleFactor = data.mVelxScaleFactor;
        d.mChanting = data.mChanting;
        d.mAbeLevel = MapWrapper::FromAESaveData(data.mAbeLevel);
        d.mAbePath = data.mAbePath;
        d.mAbeCamera = data.mAbeCamera;
        d.mSligButtonTlvId = Guid::NewGuidFromTlvInfo(data.mSligButtonTlvInfo);
        d.field_70_obj_id = Guid::NewGuidFromTlvInfo(data.field_70_obj_id); // TODO: seems unused
        d.mTransformedSligId = Guid::NewGuidFromTlvInfo(data.mTransformedSligTlvInfo);
        d.mSpeak = AEData::From(data.field_78_speak);
        d.mSayHelpTimer = data.mSayHelpTimer;
        return d;
    }

    static ::CrawlingSligMotion From(const CrawlingSligMotion& motion)
    {
        switch (motion)
        {
            case CrawlingSligMotion::None:
                return ::CrawlingSligMotion::None;
            case CrawlingSligMotion::Motion_0_Idle:
                return ::CrawlingSligMotion::Motion_0_Idle;
            case CrawlingSligMotion::Motion_1_UsingButton:
                return ::CrawlingSligMotion::Motion_1_UsingButton;
            case CrawlingSligMotion::Motion_2_WakingUp:
                return ::CrawlingSligMotion::Motion_2_WakingUp;
            case CrawlingSligMotion::Motion_3_Crawling:
                return ::CrawlingSligMotion::Motion_3_Crawling;
            case CrawlingSligMotion::Motion_4_StartFalling:
                return ::CrawlingSligMotion::Motion_4_StartFalling;
            case CrawlingSligMotion::Motion_5_Falling:
                return ::CrawlingSligMotion::Motion_5_Falling;
            case CrawlingSligMotion::Motion_6_Landing:
                return ::CrawlingSligMotion::Motion_6_Landing;
            case CrawlingSligMotion::Motion_7_ToShakingToIdle:
                return ::CrawlingSligMotion::Motion_7_ToShakingToIdle;
            case CrawlingSligMotion::Motion_8_Speaking:
                return ::CrawlingSligMotion::Motion_8_Speaking;
            case CrawlingSligMotion::Motion_9_Snoozing:
                return ::CrawlingSligMotion::Motion_9_Snoozing;
            case CrawlingSligMotion::Motion_10_PushingWall:
                return ::CrawlingSligMotion::Motion_10_PushingWall;
            case CrawlingSligMotion::Motion_11_TurnAround:
                return ::CrawlingSligMotion::Motion_11_TurnAround;
            case CrawlingSligMotion::Motion_12_Shaking:
                return ::CrawlingSligMotion::Motion_12_Shaking;
            case CrawlingSligMotion::Motion_13_Empty:
                return ::CrawlingSligMotion::Motion_13_Empty;
            case CrawlingSligMotion::Motion_14_ShakingToIdle:
                return ::CrawlingSligMotion::Motion_14_ShakingToIdle;
            case CrawlingSligMotion::Motion_15_EndCrawling:
                return ::CrawlingSligMotion::Motion_15_EndCrawling;
            case CrawlingSligMotion::Motion_16_IdleToPushingWall:
                return ::CrawlingSligMotion::Motion_16_IdleToPushingWall;
            case CrawlingSligMotion::Motion_17_EndPushingWall:
                return ::CrawlingSligMotion::Motion_17_EndPushingWall;
            default:
                ALIVE_FATAL("Bad crawling slig motion %d", static_cast<s32>(motion));
        }
    }

    static ::ISligBrain::EBrainTypes From(const eCrawlingSligBrains& brain)
    {
        switch (brain)
        {
            case eCrawlingSligBrains::Brain_0_Sleeping:
                return ::ISligBrain::EBrainTypes::Sleeping;
            case eCrawlingSligBrains::Brain_1_Idle:
                return ::ISligBrain::EBrainTypes::Idle;
            case eCrawlingSligBrains::Brain_2_PanicGetALocker:
                return ::ISligBrain::EBrainTypes::PanicGetALocker;
            case eCrawlingSligBrains::Brain_3_Possessed:
                return ::ISligBrain::EBrainTypes::Possessed;
            case eCrawlingSligBrains::Brain_4_GetKilled:
                return ::ISligBrain::EBrainTypes::GetKilled;
            case eCrawlingSligBrains::Brain_5_Transformed:
                return ::ISligBrain::EBrainTypes::Transformed;
            default:
                ALIVE_FATAL("Bad crawling slig brain %d", static_cast<s32>(brain));
        }
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(CrawlingSligSaveState, 0x80);

struct DrillSaveState final
{
    AETypes mType;
    s16 field_2_padding;
    s32 field_4_padding;
    s32 mDrillTlvInfo;
    s32 mOffTimer;
    enum class DrillStates : s16
    {
        State_0_Restart_Cycle = 0,
        State_1_Going_Down = 1,
        State_2_GoingUp = 2,
    };
    DrillStates mState;
    s16 mXYOff;

    static ::DrillSaveState From(const DrillSaveState& data)
    {
        ::DrillSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mDrillTlvId = Guid::NewGuidFromTlvInfo(data.mDrillTlvInfo);
        d.mOffTimer = data.mOffTimer;
        d.mState = From(data.mState);
        d.mXYOff = data.mXYOff;
        return d;
    }

    static ::DrillStates From(const DrillStates state)
    {
        switch (state)
        {
            case DrillStates::State_0_Restart_Cycle:
                return ::DrillStates::eRestartCycle_0;
            case DrillStates::State_1_Going_Down:
                return ::DrillStates::eGoingDown_1;
            case DrillStates::State_2_GoingUp:
                return ::DrillStates::eGoingUp_2;
        }
        ALIVE_FATAL("Bad drill states value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(DrillSaveState, 0x14);

struct EvilFartSaveState final
{
    AETypes mType;
    s16 mRed;
    s16 mGreen;
    s16 mBlue;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    FP mSpriteScale;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mAnimRender;
    s8 mDrawable;
    LevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;

    enum Flags_2C
    {
        eBit1_bControlled = 0x1,
        eBit2_FartExploded = 0x2,
    };
    BitField16<Flags_2C> field_2C;

    s16 mPossessedAliveTimer;
    enum class FartStates : s16
    {
        eIdle_0 = 0,
        eFlying_1 = 1,
        eDechanting_2 = 2
    };
    FartStates mState;
    s16 field_32_padding;
    s32 mUnpossessionTimer;
    s32 mBackToAbeTimer;

    static ::EvilFartSaveState From(const EvilFartSaveState& data)
    {
        ::EvilFartSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mRed = data.mRed;
        d.mGreen = data.mGreen;
        d.mBlue = data.mBlue;
        d.mCurrentPath = data.mCurrentPath;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.mCurrentLevel);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mSpriteScale = data.mSpriteScale;
        d.mCurrentFrame = data.mCurrentFrame;
        d.mFrameChangeCounter = data.mFrameChangeCounter;
        d.mAnimRender = data.mAnimRender;
        d.mDrawable = data.mDrawable;
        d.mAbeLevel = MapWrapper::FromAESaveData(data.mAbeLevel);
        d.mAbePath = data.mAbePath;
        d.mAbeCamera = data.mAbeCamera;
        d.mControlled = data.field_2C.Get(Flags_2C::eBit1_bControlled);
        d.mFartExploded = data.field_2C.Get(Flags_2C::eBit2_FartExploded);
        d.mPossessedAliveTimer = data.mPossessedAliveTimer;
        d.mState = From(data.mState);
        d.mUnpossessionTimer = data.mUnpossessionTimer;
        d.mBackToAbeTimer = data.mBackToAbeTimer;
        return d;
    }

    static ::FartStates From(const FartStates state)
    {
        switch (state)
        {
            case FartStates::eIdle_0:
                return ::FartStates::eIdle_0;
            case FartStates::eFlying_1:
                return ::FartStates::eFlying_1;
            case FartStates::eDechanting_2:
                return ::FartStates::eDechanting_2;
        }
        ALIVE_FATAL("Bad fart states value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(EvilFartSaveState, 60);

struct FleechSaveState final
{
    enum class eFleechMotions : s16
    {
        eNone_m1 = -1,
        Motion_0_Sleeping,
        Motion_1_WakingUp,
        Motion_2_Unknown,
        Motion_3_Idle,
        Motion_4_Crawl,
        Motion_5_PatrolCry,
        Motion_6_Knockback,
        Motion_7_StopCrawling,
        Motion_8_StopMidCrawlCycle,
        Motion_9_Fall,
        Motion_10_Land,
        Motion_11_RaiseHead,
        Motion_12_Climb,
        Motion_13_SettleOnGround,
        Motion_14_ExtendTongueFromEnemy,
        Motion_15_RetractTongueFromEnemey,
        Motion_16_DeathByFalling,
        Motion_17_SleepingWithTongue,
        Motion_18_Consume
    };

    enum class eFleechBrains : s16
    {
        eBrain_0_Patrol = 0,
        eBrain_1_ChasingAbe = 1,
        eBrain_2_Scared = 2,
        eBrain_3_Death = 3
    };

    AETypes mType;
    s16 field_2;
    s32 field_4_obj_id;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mPathNumber;
    LevelIds mLvlNumber;
    FP mSpriteScale;
    s16 mRed;
    s16 mGreen;
    s16 mBlue;
    s16 field_26_bFlipX;
    eFleechMotions field_28_current_motion;
    s16 field_2A_anim_current_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_bRender;
    s8 field_2F_bDrawable;
    FP mHealth;
    eFleechMotions mCurrentMotion;
    eFleechMotions mNextMotion;
    s16 mLastLineYPos;
    eLineTypes mCollisionLineType;
    s32 mPlatformId;
    s32 mTlvInfo;
    s32 field_44_obj_id;
    s16 field_48_unused;
    s16 mTongueState;
    s16 mTongueSubState;
    s16 mEnemyXPos;
    s16 mEnemyYPos;
    s16 mTongueOriginX;
    s16 mTongueOriginY;
    s16 mTongueDestinationX;
    s16 mTongueDestinationY;
    s16 field_5A;
    s8 field_5C_tongue_active_flag;
    s8 field_5D_render_flag;
    eFleechBrains mBrainState;
    s16 mBrainSubState;
    s16 mReturnToPreviousMotion;
    s32 field_64_shrivel_timer;
    s8 field_68_fleech_random_idx;
    s8 field_69;
    s16 field_6A_bDidMapFollowMe;
    s32 field_6C_unused;
    FP field_70_velx_factor;
    s16 field_74_unused;
    s16 field_76_current_anger;
    s16 field_78_max_anger;
    s16 field_7A_attack_anger;
    s16 field_7C_wakeup_id;
    s16 field_7E_wake_up_switch_anger_value;
    s16 field_80_wake_up_switch_value;
    s16 field_82_can_wake_up_id;
    s16 field_84_EventXPos;
    s16 field_86_ScrabParamiteEventXPos;
    s16 field_88_patrol_range;
    s16 field_8A_old_xpos;
    s16 field_8C;
    s16 field_8E_rnd_crawl;
    s16 field_90_chase_delay;
    s16 field_92_chase_timer;
    s16 field_94_lost_target_timeout;
    s16 field_96_lost_target_timer;
    s16 field_98_hoistX;
    s16 field_9A_hoistY;
    s16 field_9C_always_0;
    s8 field_9E_angle;
    s8 field_9F;
    FP field_A0_hoistY_distance;
    FP field_A4_hoistX_distance;
    s32 field_A8;
    s32 field_AC_obj_id;

    enum FleechStateFlags
    {
        eHoistDone = 0x1,
        eChasingOrScaredCrawlingLeft = 0x2,
        eShrivelDeath = 0x4,
        eScaredSound = 0x8,
        eAsleep = 0x10,
        eGoesToSleep = 0x20,
        ePersistant = 0x40,
    };

    BitField16<FleechStateFlags> mFleechStateFlags;
    s16 field_B2;

    static ::FleechSaveState From(const FleechSaveState& data)
    {
        ::FleechSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_4_obj_id = Guid::NewGuidFromTlvInfo(data.field_4_obj_id);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mPathNumber = data.mPathNumber;
        d.mLvlNumber = MapWrapper::FromAESaveData(data.mLvlNumber);
        d.mSpriteScale = data.mSpriteScale;
        d.mRed = data.mRed;
        d.mGreen = data.mGreen;
        d.mBlue = data.mBlue;
        d.mFlipX = data.field_26_bFlipX;
        d.field_28_current_motion = From(data.field_28_current_motion);
        d.field_2A_anim_current_frame = data.field_2A_anim_current_frame;
        d.field_2C_frame_change_counter = data.field_2C_frame_change_counter;
        d.mRender = data.field_2E_bRender;
        d.mDrawable = data.field_2F_bDrawable;
        d.mHealth = data.mHealth;
        d.mCurrentMotion = From(data.mCurrentMotion);
        d.mNextMotion = From(data.mNextMotion);
        d.mLastLineYPos = data.mLastLineYPos;
        d.mCollisionLineType = AEData::From(data.mCollisionLineType);
        d.mPlatformId = Guid::NewGuidFromTlvInfo(data.mPlatformId);
        d.mTlvInfo = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        d.field_44_obj_id = Guid::NewGuidFromTlvInfo(data.field_44_obj_id);
        d.mTongueState = data.mTongueState;
        d.mTongueSubState = data.mTongueSubState;
        d.mEnemyXPos = data.mEnemyXPos;
        d.mEnemyYPos = data.mEnemyYPos;
        d.mTongueOriginX = data.mTongueOriginX;
        d.mTongueOriginY = data.mTongueOriginY;
        d.mTongueDestinationX = data.mTongueDestinationX;
        d.mTongueDestinationY = data.mTongueDestinationY;
        d.field_5A = data.field_5A;
        d.mTongueActive = data.field_5C_tongue_active_flag;
        d.mRenderTongue = data.field_5D_render_flag;
        d.mBrainState = From(data.mBrainState);
        d.mBrainSubState = data.mBrainSubState;
        d.mReturnToPreviousMotion = data.mReturnToPreviousMotion;
        d.field_64_shrivel_timer = data.field_64_shrivel_timer;
        d.field_68_fleech_random_idx = data.field_68_fleech_random_idx;
        d.field_69 = data.field_69;
        d.field_6A_bDidMapFollowMe = data.field_6A_bDidMapFollowMe;
        d.field_70_velx_factor = data.field_70_velx_factor;
        d.field_76_current_anger = data.field_76_current_anger;
        d.field_78_max_anger = data.field_78_max_anger;
        d.field_7A_attack_anger = data.field_7A_attack_anger;
        d.field_7C_wakeup_id = data.field_7C_wakeup_id;
        d.field_7E_wake_up_switch_anger_value = data.field_7E_wake_up_switch_anger_value;
        d.field_80_wake_up_switch_value = data.field_80_wake_up_switch_value;
        d.field_82_can_wake_up_id = data.field_82_can_wake_up_id;
        d.field_84_EventXPos = data.field_84_EventXPos;
        d.field_86_ScrabParamiteEventXPos = data.field_86_ScrabParamiteEventXPos;
        d.field_88_patrol_range = data.field_88_patrol_range;
        d.field_8A_old_xpos = data.field_8A_old_xpos;
        d.field_8C = data.field_8C;
        d.field_8E_rnd_crawl = data.field_8E_rnd_crawl;
        d.field_90_chase_delay = data.field_90_chase_delay;
        d.field_92_chase_timer = data.field_92_chase_timer;
        d.field_94_lost_target_timeout = data.field_94_lost_target_timeout;
        d.field_96_lost_target_timer = data.field_96_lost_target_timer;
        d.field_98_hoistX = data.field_98_hoistX;
        d.field_9A_hoistY = data.field_9A_hoistY;
        d.field_9E_angle = data.field_9E_angle;
        d.field_9F = data.field_9F;
        d.field_A0_hoistY_distance = data.field_A0_hoistY_distance;
        d.field_A4_hoistX_distance = data.field_A4_hoistX_distance;
        d.field_A8 = Guid::NewGuidFromTlvInfo(data.field_A8);
        d.field_AC_obj_id = Guid::NewGuidFromTlvInfo(data.field_AC_obj_id);
        d.mHoistDone = data.mFleechStateFlags.Get(FleechStateFlags::eHoistDone);
        d.mChasingOrScaredCrawlingLeft = data.mFleechStateFlags.Get(FleechStateFlags::eChasingOrScaredCrawlingLeft);
        d.mShrivelDeath = data.mFleechStateFlags.Get(FleechStateFlags::eShrivelDeath);
        d.mScaredSound = data.mFleechStateFlags.Get(FleechStateFlags::eScaredSound);
        d.mAsleep = data.mFleechStateFlags.Get(FleechStateFlags::eAsleep);
        d.mGoesToSleep = data.mFleechStateFlags.Get(FleechStateFlags::eGoesToSleep);
        d.mPersistant = data.mFleechStateFlags.Get(FleechStateFlags::ePersistant);
        d.field_B2 = data.field_B2;
        return d;
    }

    static ::eFleechMotions From(const eFleechMotions motion)
    {
        switch (motion)
        {
            case eFleechMotions::eNone_m1:
                return ::eFleechMotions::eNone_m1;
            case eFleechMotions::Motion_0_Sleeping:
                return ::eFleechMotions::Motion_0_Sleeping;
            case eFleechMotions::Motion_1_WakingUp:
                return ::eFleechMotions::Motion_1_WakingUp;
            case eFleechMotions::Motion_2_Unknown:
                return ::eFleechMotions::Motion_2_Unknown;
            case eFleechMotions::Motion_3_Idle:
                return ::eFleechMotions::Motion_3_Idle;
            case eFleechMotions::Motion_4_Crawl:
                return ::eFleechMotions::Motion_4_Crawl;
            case eFleechMotions::Motion_5_PatrolCry:
                return ::eFleechMotions::Motion_5_PatrolCry;
            case eFleechMotions::Motion_6_Knockback:
                return ::eFleechMotions::Motion_6_Knockback;
            case eFleechMotions::Motion_7_StopCrawling:
                return ::eFleechMotions::Motion_7_StopCrawling;
            case eFleechMotions::Motion_8_StopMidCrawlCycle:
                return ::eFleechMotions::Motion_8_StopMidCrawlCycle;
            case eFleechMotions::Motion_9_Fall:
                return ::eFleechMotions::Motion_9_Fall;
            case eFleechMotions::Motion_10_Land:
                return ::eFleechMotions::Motion_10_Land;
            case eFleechMotions::Motion_11_RaiseHead:
                return ::eFleechMotions::Motion_11_RaiseHead;
            case eFleechMotions::Motion_12_Climb:
                return ::eFleechMotions::Motion_12_Climb;
            case eFleechMotions::Motion_13_SettleOnGround:
                return ::eFleechMotions::Motion_13_SettleOnGround;
            case eFleechMotions::Motion_14_ExtendTongueFromEnemy:
                return ::eFleechMotions::Motion_14_ExtendTongueFromEnemy;
            case eFleechMotions::Motion_15_RetractTongueFromEnemey:
                return ::eFleechMotions::Motion_15_RetractTongueFromEnemey;
            case eFleechMotions::Motion_16_DeathByFalling:
                return ::eFleechMotions::Motion_16_DeathByFalling;
            case eFleechMotions::Motion_17_SleepingWithTongue:
                return ::eFleechMotions::Motion_17_SleepingWithTongue;
            case eFleechMotions::Motion_18_Consume:
                return ::eFleechMotions::Motion_18_Consume;
            default:
                LOG_WARNING("Bad fleech motion value %d", static_cast<u32>(motion));
                return ::eFleechMotions::Motion_0_Sleeping;
        }
    }

    static ::eFleechBrains From(const eFleechBrains brain)
    {
        switch (brain)
        {
            case eFleechBrains::eBrain_0_Patrol:
                return ::eFleechBrains::eBrain_0_Patrol;
            case eFleechBrains::eBrain_1_ChasingAbe:
                return ::eFleechBrains::eBrain_1_ChasingAbe;
            case eFleechBrains::eBrain_2_Scared:
                return ::eFleechBrains::eBrain_2_Scared;
            case eFleechBrains::eBrain_3_Death:
                return ::eFleechBrains::eBrain_3_Death;
        }
        ALIVE_FATAL("Bad fleech brain value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FleechSaveState, 0xB4);

struct FlyingSligSaveState final
{
    AETypes mType;
    s16 field_2;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    s16 field_1C_oldr;
    s16 field_1E_oldg;
    s16 field_20_oldb;
    s16 field_22_bAnimFlipX;
    s16 field_24_current_state;
    s16 field_26_current_frame;
    s16 field_28_frame_change_counter;
    u8 field_2A_bAnimRender;
    u8 field_2B_bDrawable;
    FP field_2C_current_health;
    s16 field_30_current_state;
    s16 field_32_delayed_state;
    s16 field_34_lastLineYPos;
    s16 field_36_line_idx;
    u8 field_38_launch_switch_id;
    u8 field_39_padding;
    enum Flags_3A
    {
        eBit1_bPossessed = 0x1,
        eBit2_Throw_grenade = 0x2,
        eBit3_bAlertedAndNotFacingAbe = 0x4,
        eBit4_DoAction = 0x8,
        eBit5_Chanting = 0x10,
        eBit6_Speaking_flag2 = 0x20,
        eBit7_Speaking_flag1 = 0x40,
        eBit8_bLastLine = 0x80,
        eBit9_unknown1 = 0x100,
        eBit10_unknown2 = 0x200,
        eBit11_padding = 0x400,
    };
    BitField16<Flags_3A> field_3A;
    s32 field_3C_tlvInfo;
    s32 field_40_timer;
    s32 field_44_grenade_delay;
    s32 field_48_collision_reaction_timer;
    FP field_4C_xSpeed;
    FP field_50_ySpeed;
    SligSpeak field_54_next_speak;
    s8 field_55_padding;
    s16 field_56_voice_pitch_min;
    s32 field_58_obj_id;
    FP field_5C;
    FP field_60;
    FP field_64;
    FP field_68_line_length;
    FP field_6C;
    FP field_70_lever_pull_range_xpos;
    FP field_74_lever_pull_range_ypos;
    s32 field_78_unused;
    s32 field_7C_unused;
    s32 field_80_unused;
    s32 field_84_unused;
    FP field_88_nextXPos;
    FP field_8C_nextYPos;
    s32 field_90_fns1_idx;
    s16 field_94_padding;
    s16 field_96_padding;
    s16 field_98_unused;
    LevelIds field_9A_abe_level;
    s16 field_9C_abe_path;
    s16 field_9E_abe_camera;
    s16 field_A0_bobbing_values_table_index;
    s16 field_A2_padding;
    s32 field_A4_bobbing_values_index;
    FP field_A8_bobbing_value;

    static ::FlyingSligSaveState From(const FlyingSligSaveState& data)
    {
        ::FlyingSligSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_4_xpos = data.field_4_xpos;
        d.field_8_ypos = data.field_8_ypos;
        d.field_C_velx = data.field_C_velx;
        d.field_10_vely = data.field_10_vely;
        d.field_14_path_number = data.field_14_path_number;
        d.field_16_lvl_number = MapWrapper::FromAESaveData(data.field_16_lvl_number);
        d.field_18_sprite_scale = data.field_18_sprite_scale;
        d.field_1C_oldr = data.field_1C_oldr;
        d.field_1E_oldg = data.field_1E_oldg;
        d.field_20_oldb = data.field_20_oldb;
        d.field_22_bAnimFlipX = data.field_22_bAnimFlipX;
        d.field_24_current_state = data.field_24_current_state;
        d.field_26_current_frame = data.field_26_current_frame;
        d.field_28_frame_change_counter = data.field_28_frame_change_counter;
        d.field_2A_bAnimRender = data.field_2A_bAnimRender;
        d.field_2B_bDrawable = data.field_2B_bDrawable;
        d.field_2C_current_health = data.field_2C_current_health;
        d.field_30_current_state = data.field_30_current_state;
        d.field_32_delayed_state = data.field_32_delayed_state;
        d.field_34_lastLineYPos = data.field_34_lastLineYPos;
        d.field_36_line_idx = data.field_36_line_idx;
        d.field_38_launch_switch_id = data.field_38_launch_switch_id;
        d.mPossessed = data.field_3A.Get(Flags_3A::eBit1_bPossessed);
        d.mThrowGrenade = data.field_3A.Get(Flags_3A::eBit2_Throw_grenade);
        d.mAlertedAndNotFacingAbe = data.field_3A.Get(Flags_3A::eBit3_bAlertedAndNotFacingAbe);
        d.mDoAction = data.field_3A.Get(Flags_3A::eBit4_DoAction);
        d.mChanting = data.field_3A.Get(Flags_3A::eBit5_Chanting);
        d.mSpeaking2 = data.field_3A.Get(Flags_3A::eBit6_Speaking_flag2);
        d.mSpeaking1 = data.field_3A.Get(Flags_3A::eBit7_Speaking_flag1);
        d.mLastLine = data.field_3A.Get(Flags_3A::eBit8_bLastLine);
        d.mUnknown1 = data.field_3A.Get(Flags_3A::eBit9_unknown1);
        d.mUnknown2 = data.field_3A.Get(Flags_3A::eBit10_unknown2);
        d.field_3C_tlvInfo = Guid::NewGuidFromTlvInfo(data.field_3C_tlvInfo);
        d.field_40_timer = data.field_40_timer;
        d.field_44_grenade_delay = data.field_44_grenade_delay;
        d.field_48_collision_reaction_timer = data.field_48_collision_reaction_timer;
        d.field_4C_xSpeed = data.field_4C_xSpeed;
        d.field_50_ySpeed = data.field_50_ySpeed;
        d.field_54_next_speak = AEData::From(data.field_54_next_speak);
        d.field_56_voice_pitch_min = data.field_56_voice_pitch_min;
        d.field_58_obj_id = Guid::NewGuidFromTlvInfo(data.field_58_obj_id);
        d.field_5C = data.field_5C;
        d.field_60 = data.field_60;
        d.field_64 = data.field_64;
        d.field_68_line_length = data.field_68_line_length;
        d.field_6C = data.field_6C;
        d.field_70_lever_pull_range_xpos = data.field_70_lever_pull_range_xpos;
        d.field_74_lever_pull_range_ypos = data.field_74_lever_pull_range_ypos;
        d.field_88_nextXPos = data.field_88_nextXPos;
        d.field_8C_nextYPos = data.field_8C_nextYPos;
        d.field_90_fns1_idx = data.field_90_fns1_idx;
        d.field_9A_abe_level = MapWrapper::FromAESaveData(data.field_9A_abe_level);
        d.field_9C_abe_path = data.field_9C_abe_path;
        d.field_9E_abe_camera = data.field_9E_abe_camera;
        d.field_A0_bobbing_values_table_index = data.field_A0_bobbing_values_table_index;
        d.field_A4_bobbing_values_index = data.field_A4_bobbing_values_index;
        d.field_A8_bobbing_value = data.field_A8_bobbing_value;
        return d;
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSligSaveState, 0xAC);

struct FlyingSligSpawnerSaveState final
{
    AETypes mType;
    s16 field_2_pad;
    s32 field_4_tlvInfo;
    s32 field_8_bSpawned;
    s32 field_C_spawned_slig_obj_id;

    static ::FlyingSligSpawnerSaveState From(const FlyingSligSpawnerSaveState& data)
    {
        ::FlyingSligSpawnerSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_4_tlvInfo = Guid::NewGuidFromTlvInfo(data.field_4_tlvInfo);
        d.field_8_bSpawned = data.field_8_bSpawned;
        d.field_C_spawned_slig_obj_id = Guid::NewGuidFromTlvInfo(data.field_C_spawned_slig_obj_id);
        return d;
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSligSpawnerSaveState, 0x10);

struct GameEnderControllerSaveState final
{
    AETypes mType;
    s16 field_2_padding;
    s32 field_4_obj_id;
    s32 field_8_timer;
    enum class GameEnderControllerStates : s16
    {
        eInit_0 = 0,
        eDetermineEnding_1 = 1,
        eFinish_2 = 2,
        eBadEnding_3 = 3,
        eGoodEnding_4 = 4,
        eAngelicEnding_5 = 5,
        eAngelicEndingCredits_6 = 6,
        ePadding_7 = 7,
        ePadding_8 = 8,
        ePadding_9 = 9,
    };
    GameEnderControllerStates field_C_state;
    s16 field_E_padding;

    static ::GameEnderControllerSaveState From(const GameEnderControllerSaveState& data)
    {
        ::GameEnderControllerSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mObjId = Guid::NewGuidFromTlvInfo(data.field_4_obj_id);
        d.mTimer = data.field_8_timer;
        d.mState = From(data.field_C_state);
        return d;
    }

    static ::GameEnderControllerStates From(const GameEnderControllerStates state)
    {
        switch (state)
        {
            case GameEnderControllerStates::eInit_0:
                return ::GameEnderControllerStates::eInit_0;
            case GameEnderControllerStates::eDetermineEnding_1:
                return ::GameEnderControllerStates::eDetermineEnding_1;
            case GameEnderControllerStates::eFinish_2:
                return ::GameEnderControllerStates::eFinish_2;
            case GameEnderControllerStates::eBadEnding_3:
                return ::GameEnderControllerStates::eBadEnding_3;
            case GameEnderControllerStates::eGoodEnding_4:
                return ::GameEnderControllerStates::eGoodEnding_4;
            case GameEnderControllerStates::eAngelicEnding_5:
                return ::GameEnderControllerStates::eAngelicEnding_5;
            case GameEnderControllerStates::eAngelicEndingCredits_6:
                return ::GameEnderControllerStates::eAngelicEndingCredits_6;
        }
        ALIVE_FATAL("Bad game ender controller state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(GameEnderControllerSaveState, 0x10);

struct SlapLockWhirlWindSaveState final
{
    AETypes mType;
    s16 mSwitchId;

    static ::SlapLockWhirlWindSaveState From(const SlapLockWhirlWindSaveState& data)
    {
        ::SlapLockWhirlWindSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mSwitchId = data.mSwitchId;
        return d;
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLockWhirlWindSaveState, 0x4);

struct SlapLockSaveState final
{
    AETypes mType;
    s16 mAnimRender;
    s32 mTlvInfo;
    s8 mTlvState;
    s8 padding;
    enum class SlapLockStates : s16
    {
        eShaking_0 = 0,
        eIdle_1 = 1,
        eSlapped_2 = 2,
        eBroken_3 = 3,
        eEmitInvisibilityPowerupRing_4 = 4,
        eFlickerHero_5 = 5,
        eGiveInvisibilityFromFlicker_6 = 6,
        eGiveInvisibility_7 = 7,
    };
    SlapLockStates mState;
    s32 mTimer1;
    s32 mAbilityRingId;
    s32 mShinyParticleTimer;

    static ::SlapLockSaveState From(const SlapLockSaveState& data)
    {
        ::SlapLockSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mAnimRender = data.mAnimRender;
        d.mTlvInfo = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        d.mTlvState = data.mTlvState;
        d.mState = From(data.mState);
        d.mTimer1 = data.mTimer1;
        d.mAbilityRingId = Guid::NewGuidFromTlvInfo(data.mAbilityRingId);
        d.mShinyParticleTimer = data.mShinyParticleTimer;
        return d;
    }

    static ::SlapLockStates From(const SlapLockStates state)
    {
        switch (state)
        {
            case SlapLockStates::eShaking_0:
                return ::SlapLockStates::eShaking_0;
            case SlapLockStates::eIdle_1:
                return ::SlapLockStates::eIdle_1;
            case SlapLockStates::eSlapped_2:
                return ::SlapLockStates::eSlapped_2;
            case SlapLockStates::eBroken_3:
                return ::SlapLockStates::eBroken_3;
            case SlapLockStates::eEmitInvisibilityPowerupRing_4:
                return ::SlapLockStates::eEmitInvisibilityPowerupRing_4;
            case SlapLockStates::eFlickerHero_5:
                return ::SlapLockStates::eFlickerHero_5;
            case SlapLockStates::eGiveInvisibilityFromFlicker_6:
                return ::SlapLockStates::eGiveInvisibilityFromFlicker_6;
            case SlapLockStates::eGiveInvisibility_7:
                return ::SlapLockStates::eGiveInvisibility_7;
        }
        ALIVE_FATAL("Bad slap lock state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLockSaveState, 0x18);

struct GreeterSaveState final
{
    AETypes mType;
    s16 field_2_r;
    s16 field_4_g;
    s16 field_6_b;
    s16 field_8_path_number;
    LevelIds field_A_lvl_number;
    FP field_C_xpos;
    FP field_10_ypos;
    FP field_14_velx;
    FP field_18_vely;
    FP field_1C_sprite_scale;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mAnimRender;
    s8 mDrawable;
    s8 field_26_padding;
    s8 field_27_padding;
    s32 field_28_tlvInfo;
    s32 field_2C_unused;
    s32 field_30_last_turn_time;
    s32 field_34_timer;
    s16 mTimesShot;
    s16 field_3A_bDontSetDestroyed;
    s16 mChasing;
    s16 field_3E_padding;
    FP field_40_speed;
    enum class GreeterBrainStates : s16
    {
        eBrain_0_Patrol,
        eBrain_1_PatrolTurn,
        eBrain_2_Speak,
        eBrain_3_ChaseSpeak,
        eBrain_4_Chase,
        eBrain_5_Knockback,
        eBrain_6_ToChase,
        eBrain_7_Fall
    };
    GreeterBrainStates mBrainState;
    s16 field_46_targetOnLeft;
    s16 field_48_targetOnRight;
    s16 field_4A_padding;
    FP mMotionLaserXPos;

    static ::GreeterSaveState From(const GreeterSaveState& data)
    {
        ::GreeterSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_2_r = data.field_2_r;
        d.field_4_g = data.field_4_g;
        d.field_6_b = data.field_6_b;
        d.field_8_path_number = data.field_8_path_number;
        d.field_A_lvl_number = MapWrapper::FromAESaveData(data.field_A_lvl_number);
        d.field_C_xpos = data.field_C_xpos;
        d.field_10_ypos = data.field_10_ypos;
        d.field_14_velx = data.field_14_velx;
        d.field_18_vely = data.field_18_vely;
        d.field_1C_sprite_scale = data.field_1C_sprite_scale;
        d.mCurrentFrame = data.mCurrentFrame;
        d.mFrameChangeCounter = data.mFrameChangeCounter;
        d.mAnimRender = data.mAnimRender;
        d.mDrawable = data.mDrawable;
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.field_28_tlvInfo);
        d.field_30_last_turn_time = data.field_30_last_turn_time;
        d.field_34_timer = data.field_34_timer;
        d.mTimesShot = data.mTimesShot;
        d.field_3A_bDontSetDestroyed = data.field_3A_bDontSetDestroyed;
        d.mChasing = data.mChasing;
        d.field_40_speed = data.field_40_speed;
        d.mBrainState = From(data.mBrainState);
        d.field_46_targetOnLeft = data.field_46_targetOnLeft;
        d.field_48_targetOnRight = data.field_48_targetOnRight;
        d.mMotionLaserXPos = data.mMotionLaserXPos;
        return d;
    }

    static ::GreeterBrainStates From(const GreeterBrainStates state)
    {
        switch (state)
        {
            case GreeterBrainStates::eBrain_0_Patrol:
                return ::GreeterBrainStates::eBrain_0_Patrol;
            case GreeterBrainStates::eBrain_1_PatrolTurn:
                return ::GreeterBrainStates::eBrain_1_PatrolTurn;
            case GreeterBrainStates::eBrain_2_Speak:
                return ::GreeterBrainStates::eBrain_2_Speak;
            case GreeterBrainStates::eBrain_3_ChaseSpeak:
                return ::GreeterBrainStates::eBrain_3_ChaseSpeak;
            case GreeterBrainStates::eBrain_4_Chase:
                return ::GreeterBrainStates::eBrain_4_Chase;
            case GreeterBrainStates::eBrain_5_Knockback:
                return ::GreeterBrainStates::eBrain_5_Knockback;
            case GreeterBrainStates::eBrain_6_ToChase:
                return ::GreeterBrainStates::eBrain_6_ToChase;
            case GreeterBrainStates::eBrain_7_Fall:
                return ::GreeterBrainStates::eBrain_7_Fall;
        }
        ALIVE_FATAL("Bad greeter brain state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(GreeterSaveState, 0x50);

struct GrenadeSaveState final
{
    AETypes mType;
    s16 field_2_pad;
    s32 mTlvInfo;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    FP mSpriteScale;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;

    enum Flags_20
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8,
        eBit5_unused = 0x10,
        eBit6_bExplodeNow = 0x20,
        eBit7_bBlowUpOnCollision = 0x40,
    };
    BitField16<Flags_20> field_20_flags;
    s16 field_22_padding;
    s32 mPlatformId;
    eLineTypes mCollisionLineType;
    s16 mThrowableCount;
    enum class GrenadeStates : s16
    {
        eFallingToBeCollected_0 = 0,
        eWaitToBeCollected_1 = 1,
        eDoesNothing_2 = 2,
        eCountingDown_3 = 3,
        eFalling_4 = 4,
        eHitGround_5 = 5,
        eWaitForExplodeEnd_6 = 6,
        eExploded_7 = 7,
        eDoesNothing_8 = 8,
        eFallingBlowUpOnGround_9 = 9,
    };
    GrenadeStates mState;
    s16 mBounceCount;
    s16 mExplodeCountdown;
    s16 field_32_padding;
    FP mPreviousXPos;
    FP mPreviousYPos;

    static ::GrenadeSaveState From(const GrenadeSaveState& data)
    {
        ::GrenadeSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mTlvInfo = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mSpriteScale = data.mSpriteScale;
        d.mCurrentPath = data.mCurrentPath;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.mCurrentLevel);
        d.mRender = data.field_20_flags.Get(Flags_20::eBit1_bRender);
        d.mDrawable = data.field_20_flags.Get(Flags_20::eBit2_bDrawable);
        d.mLoop = data.field_20_flags.Get(Flags_20::eBit3_bLoop);
        d.mInteractive = data.field_20_flags.Get(Flags_20::eBit4_bInteractive);
        d.mExplodeNow = data.field_20_flags.Get(Flags_20::eBit6_bExplodeNow);
        d.mBlowUpOnCollision = data.field_20_flags.Get(Flags_20::eBit7_bBlowUpOnCollision);
        d.mPlatformId = Guid::NewGuidFromTlvInfo(data.mPlatformId);
        d.mCollisionLineType = AEData::From(data.mCollisionLineType);
        d.mThrowableCount = data.mThrowableCount;
        d.mState = From(data.mState);
        d.mBounceCount = data.mBounceCount;
        d.mExplodeCountdown = data.mExplodeCountdown;
        d.mPreviousXPos = data.mPreviousXPos;
        d.mPreviousYPos = data.mPreviousYPos;
        return d;
    }

    static ::GrenadeStates From(const GrenadeStates state)
    {
        switch (state)
        {
            case GrenadeStates::eFallingToBeCollected_0:
                return ::GrenadeStates::eFallingToBeCollected_0;
            case GrenadeStates::eWaitToBeCollected_1:
                return ::GrenadeStates::eWaitToBeCollected_1;
            case GrenadeStates::eDoesNothing_2:
                return ::GrenadeStates::eDoesNothing_2;
            case GrenadeStates::eCountingDown_3:
                return ::GrenadeStates::eCountingDown_3;
            case GrenadeStates::eFalling_4:
                return ::GrenadeStates::eFalling_4;
            case GrenadeStates::eHitGround_5:
                return ::GrenadeStates::eHitGround_5;
            case GrenadeStates::eWaitForExplodeEnd_6:
                return ::GrenadeStates::eWaitForExplodeEnd_6;
            case GrenadeStates::eExploded_7:
                return ::GrenadeStates::eExploded_7;
            case GrenadeStates::eDoesNothing_8:
                return ::GrenadeStates::eDoesNothing_8;
            case GrenadeStates::eFallingBlowUpOnGround_9:
                return ::GrenadeStates::eFallingBlowUpOnGround_9;
        }
        ALIVE_FATAL("Bad grenade state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(GrenadeSaveState, 0x3C);

struct GlukkonSaveState final
{
    enum class eGlukkonMotions : s16
    {
        eNone_m1 = -1,
        Motion_0_Idle = 0,
        Motion_1_Walk = 1,
        Motion_2_Turn = 2,
        Motion_3_KnockBack = 3,
        Motion_4_Jump = 4,
        Motion_5_JumpToFall = 5,
        Motion_6_WalkToFall = 6,
        Motion_7_Fall = 7,
        Motion_8_DeathFall = 8,
        Motion_9_Land = 9,
        Motion_10_ChantShake = 10,
        Motion_11_Speak1 = 11,
        Motion_12_Speak2 = 12,
        Motion_13_LongLaugh = 13,
        Motion_14_BeginWalk = 14,
        Motion_15_EndWalk = 15,
        Motion_16_StandToJump = 16,
        Motion_17_JumpToStand = 17,
        Motion_18_WalkToJump = 18,
        Motion_19_JumpToWalk = 19,
        Motion_20_KnockBackStandBegin = 20,
        Motion_21_GetShot = 21,
        Motion_22_KnockBackStandEnd = 22,
        Motion_23_Speak3 = 23,
        Motion_24_EndSingleStep = 24
    };

    AETypes mType;
    s16 field_2_padding;
    s32 field_4_object_id;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;
    FP mSpriteScale;
    u16 mRed;
    u16 mGreen;
    u16 mBlue;
    u16 mFlipX;
    eGlukkonMotions mCurrentMotion;
    u16 mCurrentFrame;
    u16 mFrameChangeCounter;
    u8 mRender;
    u8 mDrawable;
    FP mHealth;
    eGlukkonMotions mCurrentMotion2;
    eGlukkonMotions mNextMotion;
    s16 field_38_last_line_ypos;
    eLineTypes mLineType;
    s32 field_3C_padding;
    u16 mIsActiveChar;
    s16 field_42_padding;
    s32 mTlvId;
    s32 mBrainStateIdx;
    s32 field_4C_padding;
    s16 mBrainSubState;
    s16 field_52_padding;
    s32 field_54_timer;
    FP mFallingVelXScaleFactor;
    s16 field_5C_padding;
    s16 mPreventDepossession;
    LevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    enum class GlukkonSpeak : s8
    {
        None = -1,
        Hey_0 = 0,
        DoIt_1 = 1,
        StayHere_2 = 2,
        Commere_3 = 3,
        AllOYa_4 = 4,
        Heh_5 = 5,
        Help_6 = 6,
        Laugh_7 = 7,
        KillEm_8 = 8,
        Unused_9 = 9,
        Unused_10 = 10,
        What_11 = 11,
        Unused_12 = 12,
        Unused_13 = 13,
        Unused_14 = 14
    };
    GlukkonSpeak mSpeak;
    s16 mGamespeakPitch;
    s16 field_6A_padding;
    FP mPreviousYPos;
    s32 mRandomishSpeakTimer;
    s32 mTurnOrHelpTimer;
    s32 mPanicTimer;
    s16 field_7C;
    s16 field_7E_padding;
    s32 mKnockbackDelayAfterGettingShotTimer;
    s32 mGettingShotTimer;
    s32 mFadeId;
    s16 mCanBePossessed;
    AETypes mCurrentType;

    static ::GlukkonSaveState From(const GlukkonSaveState& data)
    {
        ::GlukkonSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_4_object_id = Guid::NewGuidFromTlvInfo(data.field_4_object_id);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mCurrentPath = data.mCurrentPath;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.mCurrentLevel);
        d.mSpriteScale = data.mSpriteScale;
        d.mRed = data.mRed;
        d.mGreen = data.mGreen;
        d.mBlue = data.mBlue;
        d.mFlipX = data.mFlipX;
        d.mCurrentMotion = From(data.mCurrentMotion);
        d.mCurrentFrame = data.mCurrentFrame;
        d.mFrameChangeCounter = data.mFrameChangeCounter;
        d.mRender = data.mRender;
        d.mDrawable = data.mDrawable;
        d.mHealth = data.mHealth;
        d.mCurrentMotion2 = From(data.mCurrentMotion2);
        d.mNextMotion = From(data.mNextMotion);
        d.field_38_last_line_ypos = data.field_38_last_line_ypos;
        d.mLineType = AEData::From(data.mLineType);
        d.mIsActiveChar = data.mIsActiveChar;
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.mTlvId);
        d.mBrainStateIdx = data.mBrainStateIdx; // TODO: convert
        d.mBrainSubState = data.mBrainSubState; // dito
        d.field_54_timer = data.field_54_timer;
        d.mFallingVelXScaleFactor = data.mFallingVelXScaleFactor;
        d.mPreventDepossession = data.mPreventDepossession;
        d.mAbeLevel = MapWrapper::FromAESaveData(data.mAbeLevel);
        d.mAbePath = data.mAbePath;
        d.mAbeCamera = data.mAbeCamera;
        d.mSpeak = From(data.mSpeak);
        d.mGamespeakPitch = data.mGamespeakPitch;
        d.mPreviousYPos = data.mPreviousYPos;
        d.mRandomishSpeakTimer = data.mRandomishSpeakTimer;
        d.mTurnOrHelpTimer = data.mTurnOrHelpTimer;
        d.mPanicTimer = data.mPanicTimer;
        d.field_7C = data.field_7C;
        d.mKnockbackDelayAfterGettingShotTimer = data.mKnockbackDelayAfterGettingShotTimer;
        d.mGettingShotTimer = data.mGettingShotTimer;
        d.mFadeId = Guid::NewGuidFromTlvInfo(data.mFadeId);
        d.mCanBePossessed = data.mCanBePossessed;
        d.mCurrentType = BaseGameObject::FromAE(data.mCurrentType);
        return d;
    }

    static ::eGlukkonMotions From(const eGlukkonMotions& motion)
    {
        switch (motion)
        {
            case eGlukkonMotions::eNone_m1:
                return ::eGlukkonMotions::eNone_m1;
            case eGlukkonMotions::Motion_0_Idle:
                return ::eGlukkonMotions::Motion_0_Idle;
            case eGlukkonMotions::Motion_1_Walk:
                return ::eGlukkonMotions::Motion_1_Walk;
            case eGlukkonMotions::Motion_2_Turn:
                return ::eGlukkonMotions::Motion_2_Turn;
            case eGlukkonMotions::Motion_3_KnockBack:
                return ::eGlukkonMotions::Motion_3_KnockBack;
            case eGlukkonMotions::Motion_4_Jump:
                return ::eGlukkonMotions::Motion_4_Jump;
            case eGlukkonMotions::Motion_5_JumpToFall:
                return ::eGlukkonMotions::Motion_5_JumpToFall;
            case eGlukkonMotions::Motion_6_WalkToFall:
                return ::eGlukkonMotions::Motion_6_WalkToFall;
            case eGlukkonMotions::Motion_7_Fall:
                return ::eGlukkonMotions::Motion_7_Fall;
            case eGlukkonMotions::Motion_8_DeathFall:
                return ::eGlukkonMotions::Motion_8_DeathFall;
            case eGlukkonMotions::Motion_9_Land:
                return ::eGlukkonMotions::Motion_9_Land;
            case eGlukkonMotions::Motion_10_ChantShake:
                return ::eGlukkonMotions::Motion_10_ChantShake;
            case eGlukkonMotions::Motion_11_Speak1:
                return ::eGlukkonMotions::Motion_11_Speak1;
            case eGlukkonMotions::Motion_12_Speak2:
                return ::eGlukkonMotions::Motion_12_Speak2;
            case eGlukkonMotions::Motion_13_LongLaugh:
                return ::eGlukkonMotions::Motion_13_LongLaugh;
            case eGlukkonMotions::Motion_14_BeginWalk:
                return ::eGlukkonMotions::Motion_14_BeginWalk;
            case eGlukkonMotions::Motion_15_EndWalk:
                return ::eGlukkonMotions::Motion_15_EndWalk;
            case eGlukkonMotions::Motion_16_StandToJump:
                return ::eGlukkonMotions::Motion_16_StandToJump;
            case eGlukkonMotions::Motion_17_JumpToStand:
                return ::eGlukkonMotions::Motion_17_JumpToStand;
            case eGlukkonMotions::Motion_18_WalkToJump:
                return ::eGlukkonMotions::Motion_18_WalkToJump;
            case eGlukkonMotions::Motion_19_JumpToWalk:
                return ::eGlukkonMotions::Motion_19_JumpToWalk;
            case eGlukkonMotions::Motion_20_KnockBackStandBegin:
                return ::eGlukkonMotions::Motion_20_KnockBackStandBegin;
            case eGlukkonMotions::Motion_21_GetShot:
                return ::eGlukkonMotions::Motion_21_GetShot;
            case eGlukkonMotions::Motion_22_KnockBackStandEnd:
                return ::eGlukkonMotions::Motion_22_KnockBackStandEnd;
            case eGlukkonMotions::Motion_23_Speak3:
                return ::eGlukkonMotions::Motion_23_Speak3;
            case eGlukkonMotions::Motion_24_EndSingleStep:
                return ::eGlukkonMotions::Motion_24_EndSingleStep;
            default:
                ALIVE_FATAL("Bad glukkon motion value %d", static_cast<s32>(motion));
        }
    }

    static ::GlukkonSpeak From(const GlukkonSpeak speak)
    {
        switch (speak)
        {
            case GlukkonSpeak::None:
                return ::GlukkonSpeak::None;
            case GlukkonSpeak::Hey_0:
                return ::GlukkonSpeak::Hey_0;
            case GlukkonSpeak::DoIt_1:
                return ::GlukkonSpeak::DoIt_1;
            case GlukkonSpeak::StayHere_2:
                return ::GlukkonSpeak::StayHere_2;
            case GlukkonSpeak::Commere_3:
                return ::GlukkonSpeak::Commere_3;
            case GlukkonSpeak::AllOYa_4:
                return ::GlukkonSpeak::AllOYa_4;
            case GlukkonSpeak::Heh_5:
                return ::GlukkonSpeak::Heh_5;
            case GlukkonSpeak::Help_6:
                return ::GlukkonSpeak::Help_6;
            case GlukkonSpeak::Laugh_7:
                return ::GlukkonSpeak::Laugh_7;
            case GlukkonSpeak::KillEm_8:
                return ::GlukkonSpeak::KillEm_8;
            case GlukkonSpeak::Unused_9:
                return ::GlukkonSpeak::Unused_9;
            case GlukkonSpeak::Unused_10:
                ALIVE_FATAL("GlukkonSpeak %d is unused", static_cast<s32>(speak));
            case GlukkonSpeak::What_11:
                return ::GlukkonSpeak::What_11;
            case GlukkonSpeak::Unused_12:
                ALIVE_FATAL("GlukkonSpeak %d is unused", static_cast<s32>(speak));
            case GlukkonSpeak::Unused_13:
                ALIVE_FATAL("GlukkonSpeak %d is unused", static_cast<s32>(speak));
            case GlukkonSpeak::Unused_14:
                ALIVE_FATAL("GlukkonSpeak %d is unused", static_cast<s32>(speak));
        }
        ALIVE_FATAL("Bad glukkon speak value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(GlukkonSaveState, 144);

struct AbeSaveState final
{
    AETypes mType;
    s16 field_2_padding;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    u16 mCurrentPath;
    LevelIds mCurrentLevel;
    FP mSpriteScale;
    Scale mScale;
    u16 mRed;
    u16 mGreen;
    u16 mBlue;
    u16 bAnimFlipX;
    u16 mCurrentMotion;
    u16 mCurrentFrame;
    u16 mFrameChangeCounter;
    s8 mRenderLayer;
    s8 mAnimRender;
    s8 mIsDrawable;
    s8 field_2F_padding;
    FP mHealth;
    u16 mCurrentMotion2; // the same as mCurrentMotion
    u16 mNextMotion;
    u16 mLastLineYPos;
    eLineTypes mCollisionLineType;
    u32 mPlatformId;
    u16 mIsElectrocuted;
    u16 mIsInvisible;
    s8 mIsAbeControlled;
    s8 field_45_padding;
    s16 field_46_padding;
    FP field_48_x_vel_slow_by;
    u32 field_4C_unused;
    u32 field_50_state;
    u32 field_54_timer;
    u32 field_58_abe_timer;
    u32 mRegenHealthTimer;
    Mud_Emotion mMood;
    MudSounds mSay;
    u32 mAutoSayTimer;
    u32 mRingPulseTimer;
    s8 mBaseThrowableCount;
    s8 bShrivel;
    s8 mHaveShrykull;
    s8 bHaveInvisiblity;
    u16 mPrevInput;
    u16 mReleasedButtons;
    u16 mKnockdownMotion;
    s16 field_76_padding;
    u32 mRollingMotionTimer;
    u32 mDeathFadeOutId;
    u32 mCircularFadeId;
    u32 mOrbWhirlWindId;
    u32 mPossessedObjectId;
    u32 mThrowableId;
    u32 mPullRingRopeId;
    u32 mSlappableOrPickupId;
    u32 mWorkWheelId;
    u32 mInvisibilityTimer;
    u16 field_A0_unused;
    u16 mInvisibilityDuration;
    s8 mHandStoneCamIdx;
    s8 field_A5_padding;
    s16 field_A6_padding;
    TlvTypes32 mHandStoneType;
    u16 mFmvId;
    u16 mHandStoneCam1;
    u16 mHandStoneCam2;
    u16 mHandStoneCam3;
    u16 field_B4_unused;
    u16 field_B6_unused;
    u16 field_B8_unused;
    u16 field_BA_unused;
    u16 field_BC_unused;
    u16 field_BE_unused;
    u16 mHasEvilFart;
    LevelIds mDstWellLevel;
    u16 mDstWellPath;
    u16 mDstWellCamera;
    u16 door_id;
    s8 mThrowDirection;
    s8 field_CB_padding;
    u16 mBirdPortalSubState;
    s16 field_CE_padding;
    u32 mBirdPortalId;
    enum Flags_D4
    {
        eD4_Bit1_lift_point_dead_while_using_lift = 0x1, // unused
        eD4_Bit2_return_to_previous_motion = 0x2,
        eD4_Bit3_WalkToRun = 0x4, // unused
        eD4_Bit4_unused = 0x8,
        eD4_Bit5_prevent_chanting = 0x10,
        eD4_Bit6_land_softly = 0x20,
        eD4_Bit7_unused = 0x40,
        eD4_Bit8_laugh_at_chant_end = 0x80,
        eD4_Bit9_unused = 0x100,
        eD4_Bit10_play_ledge_grab_sounds = 0x200,
        eD4_Bit11_unused = 0x400,
        eD4_Bit12_have_healing = 0x800,
        eD4_eBit13_teleporting = 0x1000,
        eD4_eBit14_mudanchee_done = 0x2000,
        eD4_eBit15_mudomo_done = 0x4000,
        eD4_eBit16_shadow_enabled = 0x8000,
    };
    BitField16<Flags_D4> field_D4_flags;

    enum Flags_D6
    {
        eD6_Bit1_shadow_at_bottom = 0x1
    };
    BitField16<Flags_D6> field_D6_flags;

    static ::AbeSaveState From(const AbeSaveState& data)
    {
        ::AbeSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mCurrentPath = data.mCurrentPath;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.mCurrentLevel);
        d.mSpriteScale = data.mSpriteScale;
        d.mScale = AEData::From(data.mScale);
        d.mRed = data.mRed;
        d.mGreen = data.mGreen;
        d.mBlue = data.mBlue;
        d.bAnimFlipX = data.bAnimFlipX;
        d.mCurrentMotion = data.mCurrentMotion;
        d.mCurrentFrame = data.mCurrentFrame;
        d.mFrameChangeCounter = data.mFrameChangeCounter;
        d.mRenderLayer = data.mRenderLayer;
        d.mAnimRender = data.mAnimRender;
        d.mIsDrawable = data.mIsDrawable;
        d.mHealth = data.mHealth;
        d.mCurrentMotion2 = data.mCurrentMotion2;
        d.mNextMotion = data.mNextMotion;
        d.mLastLineYPos = data.mLastLineYPos;
        d.mCollisionLineType = AEData::From(data.mCollisionLineType);
        d.mPlatformId = Guid::NewGuidFromTlvInfo(data.mPlatformId);
        d.mIsElectrocuted = data.mIsElectrocuted;
        d.mIsInvisible = data.mIsInvisible;
        d.mIsAbeControlled = data.mIsAbeControlled;
        d.field_48_x_vel_slow_by = data.field_48_x_vel_slow_by;
        d.field_50_state = data.field_50_state;
        d.field_54_timer = data.field_54_timer;
        d.field_58_abe_timer = data.field_58_abe_timer;
        d.mRegenHealthTimer = data.mRegenHealthTimer;
        d.mMood = AEData::From(data.mMood);
        d.mSay = AEData::From(data.mSay);
        d.mAutoSayTimer = data.mAutoSayTimer;
        d.mRingPulseTimer = data.mRingPulseTimer;
        d.mBaseThrowableCount = data.mBaseThrowableCount;
        d.mShrivel = data.bShrivel;
        d.mHaveShrykull = data.mHaveShrykull;
        d.bHaveInvisiblity = data.bHaveInvisiblity;
        d.mPrevInput = data.mPrevInput;
        d.mReleasedButtons = data.mReleasedButtons;
        d.mKnockdownMotion = data.mKnockdownMotion;
        d.mRollingMotionTimer = data.mRollingMotionTimer;
        d.mDeathFadeOutId = Guid::NewGuidFromTlvInfo(data.mDeathFadeOutId);
        d.mCircularFadeId = Guid::NewGuidFromTlvInfo(data.mCircularFadeId);
        d.mOrbWhirlWindId = Guid::NewGuidFromTlvInfo(data.mOrbWhirlWindId);
        d.mPossessedObjectId = Guid::NewGuidFromTlvInfo(data.mPossessedObjectId);
        d.mThrowableId = Guid::NewGuidFromTlvInfo(data.mThrowableId);
        d.mPullRingRopeId = Guid::NewGuidFromTlvInfo(data.mPullRingRopeId);
        d.mSlappableOrPickupId = Guid::NewGuidFromTlvInfo(data.mSlappableOrPickupId);
        d.mWorkWheelId = Guid::NewGuidFromTlvInfo(data.mWorkWheelId);
        d.mInvisibilityTimer = data.mInvisibilityTimer;
        d.mInvisibilityDuration = data.mInvisibilityDuration;
        d.mHandStoneCamIdx = data.mHandStoneCamIdx;
        d.mHandStoneType = From(data.mHandStoneType.mType);
        d.mFmvId = data.mFmvId;
        d.mHandStoneCam1 = data.mHandStoneCam1;
        d.mHandStoneCam2 = data.mHandStoneCam2;
        d.mHandStoneCam3 = data.mHandStoneCam3;
        d.mHasEvilFart = data.mHasEvilFart;
        d.mDstWellLevel = MapWrapper::FromAESaveData(data.mDstWellLevel);
        d.mDstWellPath = data.mDstWellPath;
        d.mDstWellCamera = data.mDstWellCamera;
        d.door_id = data.door_id;
        d.mThrowDirection = data.mThrowDirection;
        d.mBirdPortalSubState = data.mBirdPortalSubState;
        d.mBirdPortalId = Guid::NewGuidFromTlvInfo(data.mBirdPortalId);

        if (data.field_D4_flags.Get(Flags_D4::eD4_Bit9_unused))
        {
            ALIVE_FATAL("never expected Flags_D4::eD4_Bit9_unused to be set");
        }

        d.mReturnToPreviousMotion = data.field_D4_flags.Get(Flags_D4::eD4_Bit2_return_to_previous_motion);
        d.mPreventChanting = data.field_D4_flags.Get(Flags_D4::eD4_Bit5_prevent_chanting);
        d.mLandSoftly = data.field_D4_flags.Get(Flags_D4::eD4_Bit6_land_softly);
        d.mLaughAtChantEnd = data.field_D4_flags.Get(Flags_D4::eD4_Bit8_laugh_at_chant_end);
        d.mPlayLedgeGrabSounds = data.field_D4_flags.Get(Flags_D4::eD4_Bit10_play_ledge_grab_sounds);
        d.mHaveHealing = data.field_D4_flags.Get(Flags_D4::eD4_Bit12_have_healing);
        d.mTeleporting = data.field_D4_flags.Get(Flags_D4::eD4_eBit13_teleporting);
        d.mMudancheeDone = data.field_D4_flags.Get(Flags_D4::eD4_eBit14_mudanchee_done);
        d.mMudomoDone = data.field_D4_flags.Get(Flags_D4::eD4_eBit15_mudomo_done);
        d.mShadowEnabled = data.field_D4_flags.Get(Flags_D4::eD4_eBit16_shadow_enabled);
        d.mShadowAtBottom = data.field_D6_flags.Get(Flags_D6::eD6_Bit1_shadow_at_bottom);
        return d;
    }

    // Abe is the only one who uses the TlvTypes enum to store HandStone types
    // so there should be no need to convert more values.
    static ::ReliveTypes From(const TlvTypes type)
    {
        switch (type)
        {
            case TlvTypes::None_m1:
                return ::ReliveTypes::eNone;
            case TlvTypes::HandStone_61:
                return ::ReliveTypes::eHandStone;
            case TlvTypes::MovieHandStone_27:
                return ::ReliveTypes::eMovieHandStone;
            default:
                // To handle OG saves with random data
                LOG_WARNING("Bad handstone tlv type value %d", static_cast<u32>(type));
                return ::ReliveTypes::eNone;
        }
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(AbeSaveState, 216);

struct LiftPointSaveState final
{
    AETypes mType;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    s32 field_C_tlvInfo;
    s32 field_10_pTlv;
    FP field_14_floorYLevel;
    enum class LiftPointStopType : s16
    {
        eTopFloor_0 = 0,
        eBottomFloor_1 = 1,
        eMiddleFloor_2 = 2,
        eMiddleLockFloor_3 = 3,
        eStartPointOnly_4 = 4,
    };
    LiftPointStopType field_18_lift_point_stop_type;
    enum Flags
    {
        eBit1_bMoving = 0x1,
        eBit2_bTopFloor = 0x2,
        eBit3_bMiddleFloor = 0x4,
        eBit4_bBottomFloor = 0x8,
        eBit5_bMoveToFloorLevel = 0x10,
        eBit6_unused = 0x20,
        eBit7_KeepOnMiddleFloor = 0x40,
    };
    BitField16<Flags> field_1A;

    static ::LiftPointSaveState From(const LiftPointSaveState& data)
    {
        ::LiftPointSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mXPos = data.field_4_xpos;
        d.mYPos = data.field_8_ypos;
        d.mPlatformId = Guid::NewGuidFromTlvInfo(data.field_C_tlvInfo);
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.field_10_pTlv);
        d.mFloorLevelY = data.field_14_floorYLevel;
        d.mLiftPointStopType = From(data.field_18_lift_point_stop_type);
        d.mMoving = data.field_1A.Get(Flags::eBit1_bMoving);
        d.mTopFloor = data.field_1A.Get(Flags::eBit2_bTopFloor);
        d.mMiddleFloor = data.field_1A.Get(Flags::eBit3_bMiddleFloor);
        d.mBottomFloor = data.field_1A.Get(Flags::eBit4_bBottomFloor);
        d.mMoveToFloorLevel = data.field_1A.Get(Flags::eBit5_bMoveToFloorLevel);
        d.mKeepOnMiddleFloor = data.field_1A.Get(Flags::eBit7_KeepOnMiddleFloor);
        return d;
    }

    static relive::Path_LiftPoint::LiftPointStopType From(const LiftPointStopType state)
    {
        switch (state)
        {
            case LiftPointStopType::eTopFloor_0:
                return relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
            case LiftPointStopType::eBottomFloor_1:
                return relive::Path_LiftPoint::LiftPointStopType::eBottomFloor;
            case LiftPointStopType::eMiddleFloor_2:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor;
            case LiftPointStopType::eMiddleLockFloor_3:
                return relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor;
            case LiftPointStopType::eStartPointOnly_4:
                return relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
        }
        ALIVE_FATAL("Bad lift point stop type value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(LiftPointSaveState, 0x1C);

struct MudokonSaveState final
{
    AETypes mType;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_bFlipX;
    eMudMotions field_24_current_motion;
    s16 field_26_anim_current_frame;
    s16 field_28_anim_frame_change_counter;
    s8 field_2A_bAnimRender;
    s8 field_2B_bDrawable;
    FP field_2C_health;
    eMudMotions field_30_current_motion;
    eMudMotions field_32_next_motion;
    s16 field_34_lastLineYPos;
    eLineTypes field_36_line_type;
    s16 field_38_padding;
    s16 field_3A_padding;
    s8 field_3C_can_be_possessed;
    s8 field_3D_bIsPlayer;
    s16 field_3E_padding;
    s32 field_40_tlvInfo;
    FP field_44_velx_slow_by;
    s32 field_48_unused;
    s32 field_4C_portal_id;
    s16 field_50_angry_trigger;
    s16 field_52_padding;
    s32 field_54_laugh_and_crouch_timer;
    s32 field_58_angry_timer;
    s16 field_5C_unused;
    s16 field_5E_voice_pitch;
    s32 field_60_wheel_id;
    s32 field_64_unused;
    MudSounds field_68;
    s16 field_6A_maxXOffset;

    enum Flags_6A
    {
        eBit1_padding = 0x1,
        eBit2_unused = 0x2,
        eBit3_padding = 0x4,
        eBit4_not_rescued = 0x8,
        eBit5_persist_and_reset_offscreen = 0x10,
        eBit6_alerted = 0x20,
        eBit7_blind = 0x40,
        eBit8_following = 0x80,
        eBit9_standing_for_sad_or_angry = 0x100,
        eBit10_stopped_at_wheel = 0x200,
        eBit11_do_angry = 0x400,
        eBit12_seen_while_sick = 0x800,
        eBit13_work_after_turning_wheel = 0x1000,
        eBit14_unused = 0x2000,
        eBit15_return_to_previous_motion = 0x4000,
        eBit16_get_depressed = 0x8000
    };
    BitField16<Flags_6A> field_6C;

    enum Flags_6E
    {
        e6E_Bit1_alert_enemies = 0x1,
        e6E_Bit2_noise_unknown = 0x2,
        e6E_Bit3_make_sad_noise = 0x4,
        e6E_Bit4_ring_and_angry_mud_timeout = 0x8,
        e6E_Bit5_abe_has_ring = 0x10,
        e6E_Bit6_is_mud_standing_up_2 = 0x20
    };
    BitField16<Flags_6E> field_6E;

    s16 field_70_brain_sub_state2;
    s16 field_72_stand_idle_timer;
    enum class MudAction : s16
    {
        eHelloOrAllYa_0 = 0,
        eFollowMe_1 = 1,
        eWait_2 = 2,
        eUnused_3 = 3,
        eStopIt_4 = 4,
        eUnused_5 = 5,
        eFart_6 = 6,
        eSlapOrWater_7 = 7,
        eSorry_8 = 8,
        eMudAbuse_9 = 9,
        eComfort_10 = 10,
        eSmackIntoWall_11 = 11,
        eLaugh_12 = 12,
        eDuck_13 = 13,
        eMudDied_14 = 14,
        eUnknown_15 = 15,
        eUnknown_16 = 16,
        eNone_17 = 17,
    };
    MudAction field_74_delayed_speak;
    Mud_Emotion field_76_emo_tlb;
    GameSpeakEvents field_78_speak_event;
    eMudMotions field_7A_motion;
    enum Mud_Brain_State : s16
    {
        Brain_0_GiveRings,
        Brain_1_Chisel,
        Brain_2_CrouchScrub,
        Brain_3_TurnWheel,
        Brain_4_ListeningToAbe,
        Brain_5_ShrivelDeath,
        Brain_6_Escape,
        Brain_7_FallAndSmackDeath,
        Brain_8_AngryWorker,
        Brain_9_Sick
    };
    Mud_Brain_State field_7C_brain_state;
    s16 field_7E_brain_sub_state;
    s32 field_80_timer;
    s32 field_84_response_entry_idx;

    static ::MudokonSaveState From(const MudokonSaveState& data)
    {
        ::MudokonSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_4_xpos = data.field_4_xpos;
        d.field_8_ypos = data.field_8_ypos;
        d.field_C_velx = data.field_C_velx;
        d.field_10_vely = data.field_10_vely;
        d.field_14_path_number = data.field_14_path_number;
        d.field_16_lvl_number = MapWrapper::FromAESaveData(data.field_16_lvl_number);
        d.field_18_sprite_scale = data.field_18_sprite_scale;
        d.field_1C_r = data.field_1C_r;
        d.field_1E_g = data.field_1E_g;
        d.field_20_b = data.field_20_b;
        d.field_22_bFlipX = data.field_22_bFlipX;
        d.field_24_current_motion = From(data.field_24_current_motion);
        d.field_26_anim_current_frame = data.field_26_anim_current_frame;
        d.field_28_anim_frame_change_counter = data.field_28_anim_frame_change_counter;
        d.field_2A_bAnimRender = data.field_2A_bAnimRender;
        d.field_2B_bDrawable = data.field_2B_bDrawable;
        d.field_2C_health = data.field_2C_health;
        d.field_30_current_motion = From(data.field_30_current_motion);
        d.field_32_next_motion = From(data.field_32_next_motion);
        d.field_34_lastLineYPos = data.field_34_lastLineYPos;
        d.field_36_line_type = AEData::From(data.field_36_line_type);
        d.field_3C_can_be_possessed = data.field_3C_can_be_possessed;
        d.field_3D_bIsPlayer = data.field_3D_bIsPlayer;
        d.field_40_tlvInfo = Guid::NewGuidFromTlvInfo(data.field_40_tlvInfo);
        d.field_44_velx_slow_by = data.field_44_velx_slow_by;
        d.field_4C_portal_id = Guid::NewGuidFromTlvInfo(data.field_4C_portal_id);
        d.field_50_angry_trigger = data.field_50_angry_trigger;
        d.field_54_laugh_and_crouch_timer = data.field_54_laugh_and_crouch_timer;
        d.field_58_angry_timer = data.field_58_angry_timer;
        d.field_5E_voice_pitch = data.field_5E_voice_pitch;
        d.field_60_wheel_id = Guid::NewGuidFromTlvInfo(data.field_60_wheel_id);
        d.field_68 = AEData::From(data.field_68);
        d.field_6A_maxXOffset = data.field_6A_maxXOffset;
        d.mNotRescued = data.field_6C.Get(Flags_6A::eBit4_not_rescued);
        d.mPersistAndResetOffscreen = data.field_6C.Get(Flags_6A::eBit5_persist_and_reset_offscreen);
        d.mAlerted = data.field_6C.Get(Flags_6A::eBit6_alerted);
        d.mBlind = data.field_6C.Get(Flags_6A::eBit7_blind);
        d.mFollowingAbe = data.field_6C.Get(Flags_6A::eBit8_following);
        d.mStandingForSadOrAngry = data.field_6C.Get(Flags_6A::eBit9_standing_for_sad_or_angry);
        d.mStoppedAtWheel = data.field_6C.Get(Flags_6A::eBit10_stopped_at_wheel);
        d.mDoAngry = data.field_6C.Get(Flags_6A::eBit11_do_angry);
        d.mSeenWhileSick = data.field_6C.Get(Flags_6A::eBit12_seen_while_sick);
        d.mWorkAfterTurningWheel = data.field_6C.Get(Flags_6A::eBit13_work_after_turning_wheel);
        d.mReturnToPreviousMotion = data.field_6C.Get(Flags_6A::eBit15_return_to_previous_motion);
        d.mGetDepressed = data.field_6C.Get(Flags_6A::eBit16_get_depressed);
        d.mAlertEnemies = data.field_6E.Get(Flags_6E::e6E_Bit1_alert_enemies);
        d.mNoiseUnknown = data.field_6E.Get(Flags_6E::e6E_Bit2_noise_unknown);
        d.mMakeSadNoise = data.field_6E.Get(Flags_6E::e6E_Bit3_make_sad_noise);
        d.mRingAndAngryMudTimeout = data.field_6E.Get(Flags_6E::e6E_Bit4_ring_and_angry_mud_timeout);
        d.mAbeHasRing = data.field_6E.Get(Flags_6E::e6E_Bit5_abe_has_ring);
        d.mIsMudStandingUp2 = data.field_6E.Get(Flags_6E::e6E_Bit6_is_mud_standing_up_2);
        d.field_70_brain_sub_state2 = data.field_70_brain_sub_state2;
        d.field_72_stand_idle_timer = data.field_72_stand_idle_timer;
        d.field_74_delayed_speak = From(data.field_74_delayed_speak);
        d.field_76_emo_tlb = AEData::From(data.field_76_emo_tlb);
        d.field_78_speak_event = AEData::From(data.field_78_speak_event);
        d.field_7A_motion = From(data.field_7A_motion);
        d.field_7C_brain_state = From(data.field_7C_brain_state);
        d.field_7E_brain_sub_state = data.field_7E_brain_sub_state;
        d.field_80_timer = data.field_80_timer;
        d.field_84_response_entry_idx = data.field_84_response_entry_idx;
        return d;
    }

    static ::MudAction From(const MudAction action)
    {
        switch (action)
        {
            case MudAction::eHelloOrAllYa_0:
                return ::MudAction::eHelloOrAllYa_0;
            case MudAction::eFollowMe_1:
                return ::MudAction::eFollowMe_1;
            case MudAction::eWait_2:
                return ::MudAction::eWait_2;
            case MudAction::eUnused_3:
                return ::MudAction::eUnused_3;
            case MudAction::eStopIt_4:
                return ::MudAction::eStopIt_4;
            case MudAction::eUnused_5:
                return ::MudAction::eUnused_5;
            case MudAction::eFart_6:
                return ::MudAction::eFart_6;
            case MudAction::eSlapOrWater_7:
                return ::MudAction::eSlapOrWater_7;
            case MudAction::eSorry_8:
                return ::MudAction::eSorry_8;
            case MudAction::eMudAbuse_9:
                return ::MudAction::eMudAbuse_9;
            case MudAction::eComfort_10:
                return ::MudAction::eComfort_10;
            case MudAction::eSmackIntoWall_11:
                return ::MudAction::eSmackIntoWall_11;
            case MudAction::eLaugh_12:
                return ::MudAction::eLaugh_12;
            case MudAction::eDuck_13:
                return ::MudAction::eDuck_13;
            case MudAction::eMudDied_14:
                return ::MudAction::eMudDied_14;
            case MudAction::eUnknown_15:
                return ::MudAction::eUnknown_15;
            case MudAction::eUnknown_16:
                return ::MudAction::eUnknown_16;
            case MudAction::eNone_17:
                return ::MudAction::eNone_17;
        }
        ALIVE_FATAL("Bad mudokon action value");
    }

    static ::Mud_Brain_State From(const Mud_Brain_State state)
    {
        switch (state)
        {
            case Mud_Brain_State::Brain_0_GiveRings:
                return ::Mud_Brain_State::Brain_0_GiveRings;
            case Mud_Brain_State::Brain_1_Chisel:
                return ::Mud_Brain_State::Brain_1_Chisel;
            case Mud_Brain_State::Brain_2_CrouchScrub:
                return ::Mud_Brain_State::Brain_2_CrouchScrub;
            case Mud_Brain_State::Brain_3_TurnWheel:
                return ::Mud_Brain_State::Brain_3_TurnWheel;
            case Mud_Brain_State::Brain_4_ListeningToAbe:
                return ::Mud_Brain_State::Brain_4_ListeningToAbe;
            case Mud_Brain_State::Brain_5_ShrivelDeath:
                return ::Mud_Brain_State::Brain_5_ShrivelDeath;
            case Mud_Brain_State::Brain_6_Escape:
                return ::Mud_Brain_State::Brain_6_Escape;
            case Mud_Brain_State::Brain_7_FallAndSmackDeath:
                return ::Mud_Brain_State::Brain_7_FallAndSmackDeath;
            case Mud_Brain_State::Brain_8_AngryWorker:
                return ::Mud_Brain_State::Brain_8_AngryWorker;
            case Mud_Brain_State::Brain_9_Sick:
                return ::Mud_Brain_State::Brain_9_Sick;
        }
        ALIVE_FATAL("Bad mudokon brain state value");
    }

    static ::eMudMotions From(const eMudMotions motion)
    {
        switch (motion)
        {
            case eMudMotions::Motion_0_Idle:
                return ::eMudMotions::Motion_0_Idle;
            case eMudMotions::Motion_1_WalkLoop:
                return ::eMudMotions::Motion_1_WalkLoop;
            case eMudMotions::Motion_2_StandingTurn:
                return ::eMudMotions::Motion_2_StandingTurn;
            case eMudMotions::M_Speak_3_472FA0:
                return ::eMudMotions::M_Speak_3_472FA0;
            case eMudMotions::M_Speak_4_472FA0:
                return ::eMudMotions::M_Speak_4_472FA0;
            case eMudMotions::M_Speak_5_472FA0:
                return ::eMudMotions::M_Speak_5_472FA0;
            case eMudMotions::M_Speak_6_472FA0:
                return ::eMudMotions::M_Speak_6_472FA0;
            case eMudMotions::Motion_7_WalkBegin:
                return ::eMudMotions::Motion_7_WalkBegin;
            case eMudMotions::Motion_8_WalkToIdle:
                return ::eMudMotions::Motion_8_WalkToIdle;
            case eMudMotions::Motion_9_MidWalkToIdle:
                return ::eMudMotions::Motion_9_MidWalkToIdle;
            case eMudMotions::Motion_10_LeverUse:
                return ::eMudMotions::Motion_10_LeverUse;
            case eMudMotions::Motion_11_Chisel:
                return ::eMudMotions::Motion_11_Chisel;
            case eMudMotions::Motion_12_StartChisel:
                return ::eMudMotions::Motion_12_StartChisel;
            case eMudMotions::Motion_13_StopChisel:
                return ::eMudMotions::Motion_13_StopChisel;
            case eMudMotions::Motion_14_CrouchScrub:
                return ::eMudMotions::Motion_14_CrouchScrub;
            case eMudMotions::Motion_15_CrouchIdle:
                return ::eMudMotions::Motion_15_CrouchIdle;
            case eMudMotions::Motion_16_CrouchTurn:
                return ::eMudMotions::Motion_16_CrouchTurn;
            case eMudMotions::Motion_17_StandToCrouch:
                return ::eMudMotions::Motion_17_StandToCrouch;
            case eMudMotions::Motion_18_CrouchToStand:
                return ::eMudMotions::Motion_18_CrouchToStand;
            case eMudMotions::Motion_19_WalkToRun:
                return ::eMudMotions::Motion_19_WalkToRun;
            case eMudMotions::Motion_20_MidWalkToRun:
                return ::eMudMotions::Motion_20_MidWalkToRun;
            case eMudMotions::Motion_21_RunLoop:
                return ::eMudMotions::Motion_21_RunLoop;
            case eMudMotions::Motion_22_RunToWalk:
                return ::eMudMotions::Motion_22_RunToWalk;
            case eMudMotions::Motion_23_MidRunToWalk:
                return ::eMudMotions::Motion_23_MidRunToWalk;
            case eMudMotions::Motion_24_RunSlideStop:
                return ::eMudMotions::Motion_24_RunSlideStop;
            case eMudMotions::Motion_25_RunSlideTurn:
                return ::eMudMotions::Motion_25_RunSlideTurn;
            case eMudMotions::Motion_26_RunTurnToRun:
                return ::eMudMotions::Motion_26_RunTurnToRun;
            case eMudMotions::Motion_27_SneakLoop:
                return ::eMudMotions::Motion_27_SneakLoop;
            case eMudMotions::Motion_28_MidWalkToSneak:
                return ::eMudMotions::Motion_28_MidWalkToSneak;
            case eMudMotions::Motion_29_SneakToWalk:
                return ::eMudMotions::Motion_29_SneakToWalk;
            case eMudMotions::Motion_30_WalkToSneak:
                return ::eMudMotions::Motion_30_WalkToSneak;
            case eMudMotions::Motion_31_MidSneakToWalk:
                return ::eMudMotions::Motion_31_MidSneakToWalk;
            case eMudMotions::Motion_32_SneakBegin:
                return ::eMudMotions::Motion_32_SneakBegin;
            case eMudMotions::Motion_33_SneakToIdle:
                return ::eMudMotions::Motion_33_SneakToIdle;
            case eMudMotions::Motion_34_MidSneakToIdle:
                return ::eMudMotions::Motion_34_MidSneakToIdle;
            case eMudMotions::Motion_35_RunJumpBegin:
                return ::eMudMotions::Motion_35_RunJumpBegin;
            case eMudMotions::Motion_36_RunJumpMid:
                return ::eMudMotions::Motion_36_RunJumpMid;
            case eMudMotions::Motion_37_StandToRun:
                return ::eMudMotions::Motion_37_StandToRun;
            case eMudMotions::Motion_38_Punch:
                return ::eMudMotions::Motion_38_Punch;
            case eMudMotions::Motion_39_HoistBegin:
                return ::eMudMotions::Motion_39_HoistBegin;
            case eMudMotions::Motion_40_HoistLand:
                return ::eMudMotions::Motion_40_HoistLand;
            case eMudMotions::Motion_41_LandSoft1:
                return ::eMudMotions::Motion_41_LandSoft1;
            case eMudMotions::Motion_42_LandSoft2:
                return ::eMudMotions::Motion_42_LandSoft2;
            case eMudMotions::Motion_43_DunnoBegin:
                return ::eMudMotions::Motion_43_DunnoBegin;
            case eMudMotions::Motion_44_DunnoEnd:
                return ::eMudMotions::Motion_44_DunnoEnd;
            case eMudMotions::Motion_45_KnockForward:
                return ::eMudMotions::Motion_45_KnockForward;
            case eMudMotions::Motion_46_Knockback:
                return ::eMudMotions::Motion_46_Knockback;
            case eMudMotions::Motion_47_KnockbackGetUp:
                return ::eMudMotions::Motion_47_KnockbackGetUp;
            case eMudMotions::Motion_48_WalkOffEdge:
                return ::eMudMotions::Motion_48_WalkOffEdge;
            case eMudMotions::Motion_49_Fall:
                return ::eMudMotions::Motion_49_Fall;
            case eMudMotions::Motion_50_Chant:
                return ::eMudMotions::Motion_50_Chant;
            case eMudMotions::Motion_51_ChantEnd:
                return ::eMudMotions::Motion_51_ChantEnd;
            case eMudMotions::Motion_52_ToDuck:
                return ::eMudMotions::Motion_52_ToDuck;
            case eMudMotions::Motion_53_Duck:
                return ::eMudMotions::Motion_53_Duck;
            case eMudMotions::Motion_54_DuckToCrouch:
                return ::eMudMotions::Motion_54_DuckToCrouch;
            case eMudMotions::Motion_55_DuckKnockback:
                return ::eMudMotions::Motion_55_DuckKnockback;
            case eMudMotions::Motion_56_SlapOwnHead:
                return ::eMudMotions::Motion_56_SlapOwnHead;
            case eMudMotions::Motion_57_TurnWheelBegin:
                return ::eMudMotions::Motion_57_TurnWheelBegin;
            case eMudMotions::Motion_58_TurnWheelLoop:
                return ::eMudMotions::Motion_58_TurnWheelLoop;
            case eMudMotions::Motion_59_TurnWheelEnd:
                return ::eMudMotions::Motion_59_TurnWheelEnd;
            default:
                // For bad OG save data
                LOG_WARNING("Bad mudokon motion value %d", static_cast<u32>(motion));
                return ::eMudMotions::Motion_0_Idle;
        }
        
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MudokonSaveState, 0x88);

struct MeatSaveState final
{
    AETypes mType;
    s16 field_2_pad;
    s32 mTlvId;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    FP mSpriteScale;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;
    enum MeatStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8
    };
    BitField16<MeatStateFlags> field_20_flags;
    s16 field_22_pad;
    s32 mPlatformId;
    eLineTypes mLineType;
    s16 mThrowableCount;
    enum class MeatStates : s16
    {
        eCreated_0 = 0,
        eIdle_1 = 1,
        eBeingThrown_2 = 2,
        eBecomeAPickUp_3 = 3,
        eWaitForPickUp_4 = 4,
        eFall_5 = 5,
    };
    MeatStates mState;
    s16 field_2E_pad;
    FP mPreviousXPos;
    FP mPreviousYPos;
    s32 mDeadTimer;

    static ::MeatSaveState From(const MeatSaveState& data)
    {
        ::MeatSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.mTlvId);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mSpriteScale = data.mSpriteScale;
        d.mCurrentPath = data.mCurrentPath;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.mCurrentLevel);
        d.mRender = data.field_20_flags.Get(MeatStateFlags::eBit1_bRender);
        d.mDrawable = data.field_20_flags.Get(MeatStateFlags::eBit2_bDrawable);
        d.mLoop = data.field_20_flags.Get(MeatStateFlags::eBit3_bLoop);
        d.mInteractive = data.field_20_flags.Get(MeatStateFlags::eBit4_bInteractive);
        d.mPlatformId = Guid::NewGuidFromTlvInfo(data.mPlatformId);
        d.mLineType = AEData::From(data.mLineType);
        d.mThrowableCount = data.mThrowableCount;
        d.mState = From(data.mState);
        d.mPreviousXPos = data.mPreviousXPos;
        d.mPreviousYPos = data.mPreviousYPos;
        d.mDeadTimer = data.mDeadTimer;
        return d;
    }

    static ::MeatStates From(const MeatStates state)
    {
        switch (state)
        {
            case MeatStates::eCreated_0:
                return ::MeatStates::eCreated_0;
            case MeatStates::eIdle_1:
                return ::MeatStates::eIdle_1;
            case MeatStates::eBeingThrown_2:
                return ::MeatStates::eBeingThrown_2;
            case MeatStates::eBecomeAPickUp_3:
                return ::MeatStates::eBecomeAPickUp_3;
            case MeatStates::eWaitForPickUp_4:
                return ::MeatStates::eWaitForPickUp_4;
            case MeatStates::eFall_5:
                return ::MeatStates::eFall_5;
        }
        ALIVE_FATAL("Bad meat state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MeatSaveState, 60);

struct MineCarSaveState final
{
    AETypes mType;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    FP field_14_sprite_scale;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    s16 field_1C_r;
    s16 field_1E_g;
    s16 field_20_b;
    s16 field_22_xFlip;
    s32 field_24_frame_table;
    s16 field_28_current_motion;
    s16 field_2A_current_anim_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_render;
    s8 field_2F_drawable;
    s8 field_30_unused;
    s8 field_31_padding;
    s16 field_32_unused;
    s16 field_34_unused;
    s16 field_36_unused;
    s32 field_38_frame_table_offset2;
    FP field_3C_health;
    s16 field_40_current_motion;
    s16 field_42_next_motion;
    s16 field_44_last_line_ypos;
    eLineTypes field_46_collision_line_type;
    s16 field_48_padding;
    s16 field_4A_padding;
    s32 field_4C_tlvInfo;
    enum class MineCarStates : s16
    {
        eParkedWithoutAbe_0 = 0,
        eParkedWithAbe_1 = 1,
        eMoving_2 = 2,
        eFalling_3 = 3,
    };
    MineCarStates field_50_state;
    enum class MineCarDirs : s16
    {
        eDown_0 = 0,
        eRight_1 = 1,
        eLeft_2 = 2,
        eUp_3 = 3,
    };
    MineCarDirs field_52_turn_direction;
    s16 field_54_unused;
    s16 field_56_unused;
    s16 field_58_falling_counter;
    s16 field_5A_bAbeInCar;
    s32 field_5C_frame_mod_16;
    s16 field_60_spawned_path;
    s16 field_62_spawned_camera;
    s16 field_64_throw_item_key1;
    s16 field_66_continue_move_input;

    static ::MineCarSaveState From(const MineCarSaveState& data)
    {
        ::MineCarSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_4_xpos = data.field_4_xpos;
        d.field_8_ypos = data.field_8_ypos;
        d.field_C_velx = data.field_C_velx;
        d.field_10_vely = data.field_10_vely;
        d.field_14_sprite_scale = data.field_14_sprite_scale;
        d.field_18_path_number = data.field_18_path_number;
        d.field_1A_lvl_number = MapWrapper::FromAESaveData(data.field_1A_lvl_number);
        d.field_1C_r = data.field_1C_r;
        d.field_1E_g = data.field_1E_g;
        d.field_20_b = data.field_20_b;
        d.field_22_xFlip = data.field_22_xFlip;
        d.field_24_frame_table = data.field_24_frame_table;
        d.field_28_current_motion = data.field_28_current_motion;
        d.field_2A_current_anim_frame = data.field_2A_current_anim_frame;
        d.field_2C_frame_change_counter = data.field_2C_frame_change_counter;
        d.field_2E_render = data.field_2E_render;
        d.field_2F_drawable = data.field_2F_drawable;
        d.field_38_frame_table_offset2 = data.field_38_frame_table_offset2;
        d.field_3C_health = data.field_3C_health;
        d.field_40_current_motion = data.field_40_current_motion;
        d.field_42_next_motion = data.field_42_next_motion;
        d.field_44_last_line_ypos = data.field_44_last_line_ypos;
        d.field_46_collision_line_type = AEData::From(data.field_46_collision_line_type);
        d.field_4C_tlvInfo = Guid::NewGuidFromTlvInfo(data.field_4C_tlvInfo);
        d.field_50_state = From(data.field_50_state);
        d.field_52_turn_direction = From(data.field_52_turn_direction);
        d.field_58_falling_counter = data.field_58_falling_counter;
        d.field_5A_bAbeInCar = data.field_5A_bAbeInCar;
        d.field_5C_frame_mod_16 = data.field_5C_frame_mod_16;
        d.field_60_spawned_path = data.field_60_spawned_path;
        d.field_62_spawned_camera = data.field_62_spawned_camera;
        d.field_64_throw_item_key1 = data.field_64_throw_item_key1;
        d.field_66_continue_move_input = data.field_66_continue_move_input;
        return d;
    }

    static ::MineCarStates From(const MineCarStates state)
    {
        switch (state)
        {
            case MineCarStates::eParkedWithoutAbe_0:
                return ::MineCarStates::eParkedWithoutAbe_0;
            case MineCarStates::eParkedWithAbe_1:
                return ::MineCarStates::eParkedWithAbe_1;
            case MineCarStates::eMoving_2:
                return ::MineCarStates::eMoving_2;
            case MineCarStates::eFalling_3:
                return ::MineCarStates::eFalling_3;
        }
        ALIVE_FATAL("Bad mine car state value");
    }

    static ::MineCarDirs From(const MineCarDirs dir)
    {
        switch (dir)
        {
            case MineCarDirs::eDown_0:
                return ::MineCarDirs::eDown_0;
            case MineCarDirs::eRight_1:
                return ::MineCarDirs::eRight_1;
            case MineCarDirs::eLeft_2:
                return ::MineCarDirs::eLeft_2;
            case MineCarDirs::eUp_3:
                return ::MineCarDirs::eUp_3;
        }
        ALIVE_FATAL("Bad mine car dir value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MineCarSaveState, 0x68);

struct ParamiteSaveState final
{
    enum class eParamiteMotions : s16
    {
        eNone_m1 = -1,
        Motion_0_Idle,
        Motion_1_WalkBegin,
        Motion_2_Walking,
        Motion_3_Running,
        Motion_4_Turn,
        Motion_5_Hop,
        Motion_6_Unused,
        Motion_7_WalkRunTransition,
        Motion_8_WalkEnd,
        Motion_9_RunBegin,
        Motion_10_RunEnd,
        Motion_11_Falling,
        Motion_12_JumpUpBegin,
        Motion_13_JumpUpMidair,
        Motion_14_JumpUpLand,
        Motion_15_RopePull,
        Motion_16_CloseAttack,
        Motion_17_Landing,
        Motion_18_Unused,
        Motion_19_Knockback,
        Motion_20_GameSpeakBegin,
        Motion_21_PreHiss,
        Motion_22_Hiss1,
        Motion_23_Hiss2,
        Motion_24_Empty,
        Motion_25_AllOYaGameSpeakBegin,
        Motion_26_Hiss3,
        Motion_27_PostHiss,
        Motion_28_GameSpeakEnd,
        Motion_29_GetDepossessedBegin,
        Motion_30_GetDepossessedEnd,
        Motion_31_RunningAttack,
        Motion_32_Empty,
        Motion_33_SurpriseWeb,
        Motion_34_WebLeaveDown,
        Motion_35_WebIdle,
        Motion_36_WebGoingUp,
        Motion_37_WebGoingDown,
        Motion_38_WebGrab,
        Motion_39_WebLeaveUp,
        Motion_40_Eating,
        Motion_41_Death,
        Motion_42_Squawk,
        Motion_43_Attack
    };

    AETypes mType;
    s16 field_2_pad;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;
    FP mSpriteScale;
    s16 mR;
    s16 mG;
    s16 mB;
    s16 mFlipX;
    eParamiteMotions field_24_current_motion;
    s16 mAnimCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mRender;
    s8 mDrawable;
    FP mHealth;
    eParamiteMotions field_30_current_motion;
    eParamiteMotions field_32_next_motion;
    s16 field_34_last_line_ypos;
    eLineTypes field_36_line_type;
    s16 field_38_padding;
    s16 field_3A_padding;
    s32 field_3C_tlvInfo;
    s32 field_40_meat_id;
    s32 field_44_web_id;
    s32 field_48_obj_id;
    s32 field_4C_pull_ring_rope_id;
    s32 mBrainIdx;
    s16 field_54_padding;
    s16 field_56_padding;
    s16 mBrainSubState;
    s16 field_5A_padding;
    s32 field_5C_timer;
    s32 field_60_depossession_timer;
    FP field_64_velx_offset;
    s32 field_68_timer;
    LevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    s16 field_72_input;
    s16 field_74_next_brain_ret;

    enum Flags_76 : s16
    {
        eBit1_controlled = 0x1,
        eBit2_running = 0x2,
        eBit3_hissed_or_left_screen = 0x4,
        eBit4_prevent_depossession = 0x8,
        eBit5_spawned = 0x10,
        eBit6_alerted = 0x20,
        eBit7_can_be_possessed = 0x40,
    };

    BitField16<Flags_76> field_76_flags;

    static ::ParamiteSaveState From(const ParamiteSaveState& data)
    {
        ::ParamiteSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mCurrentPath = data.mCurrentPath;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.mCurrentLevel);
        d.mSpriteScale = data.mSpriteScale;
        d.mR = data.mR;
        d.mG = data.mG;
        d.mB = data.mB;
        d.mFlipX = data.mFlipX;
        d.field_24_current_motion = From(data.field_24_current_motion);
        d.mAnimCurrentFrame = data.mAnimCurrentFrame;
        d.mFrameChangeCounter = data.mFrameChangeCounter;
        d.mRender = data.mRender;
        d.mDrawable = data.mDrawable;
        d.mHealth = data.mHealth;
        d.field_30_current_motion = From(data.field_30_current_motion);
        d.field_32_next_motion = From(data.field_32_next_motion);
        d.field_34_last_line_ypos = data.field_34_last_line_ypos;
        d.field_36_line_type = AEData::From(data.field_36_line_type);
        d.field_3C_tlvInfo = Guid::NewGuidFromTlvInfo(data.field_3C_tlvInfo);
        d.field_40_meat_id = Guid::NewGuidFromTlvInfo(data.field_40_meat_id);
        d.field_44_web_id = Guid::NewGuidFromTlvInfo(data.field_44_web_id);
        d.field_48_obj_id = Guid::NewGuidFromTlvInfo(data.field_48_obj_id);
        d.field_4C_pull_ring_rope_id = Guid::NewGuidFromTlvInfo(data.field_4C_pull_ring_rope_id);
        d.mBrainIdx = data.mBrainIdx;
        d.mBrainSubState = data.mBrainSubState;
        d.field_5C_timer = data.field_5C_timer;
        d.field_60_depossession_timer = data.field_60_depossession_timer;
        d.field_64_velx_offset = data.field_64_velx_offset;
        d.field_68_timer = data.field_68_timer;
        d.mAbeLevel = MapWrapper::FromAESaveData(data.mAbeLevel);
        d.mAbePath = data.mAbePath;
        d.mAbeCamera = data.mAbeCamera;
        d.field_72_input = data.field_72_input;
        d.field_74_next_brain_ret = data.field_74_next_brain_ret;
        d.mControlled = data.field_76_flags.Get(Flags_76::eBit1_controlled);
        d.mRunning = data.field_76_flags.Get(Flags_76::eBit2_running);
        d.mHissedOrLeftScreen = data.field_76_flags.Get(Flags_76::eBit3_hissed_or_left_screen);
        d.mPreventDepossession = data.field_76_flags.Get(Flags_76::eBit4_prevent_depossession);
        d.mSpawned = data.field_76_flags.Get(Flags_76::eBit5_spawned);
        d.mAlerted = data.field_76_flags.Get(Flags_76::eBit6_alerted);
        d.mCanBePossessed = data.field_76_flags.Get(Flags_76::eBit7_can_be_possessed);
        return d;
    }

    static ::eParamiteMotions From(const eParamiteMotions motion)
    {
        switch (motion)
        {
            case eParamiteMotions::eNone_m1:
                return ::eParamiteMotions::eNone_m1;
            case eParamiteMotions::Motion_0_Idle:
                return ::eParamiteMotions::Motion_0_Idle;
            case eParamiteMotions::Motion_1_WalkBegin:
                return ::eParamiteMotions::Motion_1_WalkBegin;
            case eParamiteMotions::Motion_2_Walking:
                return ::eParamiteMotions::Motion_2_Walking;
            case eParamiteMotions::Motion_3_Running:
                return ::eParamiteMotions::Motion_3_Running;
            case eParamiteMotions::Motion_4_Turn:
                return ::eParamiteMotions::Motion_4_Turn;
            case eParamiteMotions::Motion_5_Hop:
                return ::eParamiteMotions::Motion_5_Hop;
            case eParamiteMotions::Motion_6_Unused:
                return ::eParamiteMotions::Motion_6_Unused;
            case eParamiteMotions::Motion_7_WalkRunTransition:
                return ::eParamiteMotions::Motion_7_WalkRunTransition;
            case eParamiteMotions::Motion_8_WalkEnd:
                return ::eParamiteMotions::Motion_8_WalkEnd;
            case eParamiteMotions::Motion_9_RunBegin:
                return ::eParamiteMotions::Motion_9_RunBegin;
            case eParamiteMotions::Motion_10_RunEnd:
                return ::eParamiteMotions::Motion_10_RunEnd;
            case eParamiteMotions::Motion_11_Falling:
                return ::eParamiteMotions::Motion_11_Falling;
            case eParamiteMotions::Motion_12_JumpUpBegin:
                return ::eParamiteMotions::Motion_12_JumpUpBegin;
            case eParamiteMotions::Motion_13_JumpUpMidair:
                return ::eParamiteMotions::Motion_13_JumpUpMidair;
            case eParamiteMotions::Motion_14_JumpUpLand:
                return ::eParamiteMotions::Motion_14_JumpUpLand;
            case eParamiteMotions::Motion_15_RopePull:
                return ::eParamiteMotions::Motion_15_RopePull;
            case eParamiteMotions::Motion_16_CloseAttack:
                return ::eParamiteMotions::Motion_16_CloseAttack;
            case eParamiteMotions::Motion_17_Landing:
                return ::eParamiteMotions::Motion_17_Landing;
            case eParamiteMotions::Motion_18_Unused:
                return ::eParamiteMotions::Motion_18_Unused;
            case eParamiteMotions::Motion_19_Knockback:
                return ::eParamiteMotions::Motion_19_Knockback;
            case eParamiteMotions::Motion_20_GameSpeakBegin:
                return ::eParamiteMotions::Motion_20_GameSpeakBegin;
            case eParamiteMotions::Motion_21_PreHiss:
                return ::eParamiteMotions::Motion_21_PreHiss;
            case eParamiteMotions::Motion_22_Hiss1:
                return ::eParamiteMotions::Motion_22_Hiss1;
            case eParamiteMotions::Motion_23_Hiss2:
                return ::eParamiteMotions::Motion_23_Hiss2;
            case eParamiteMotions::Motion_24_Empty:
                return ::eParamiteMotions::Motion_24_Empty;
            case eParamiteMotions::Motion_25_AllOYaGameSpeakBegin:
                return ::eParamiteMotions::Motion_25_AllOYaGameSpeakBegin;
            case eParamiteMotions::Motion_26_Hiss3:
                return ::eParamiteMotions::Motion_26_Hiss3;
            case eParamiteMotions::Motion_27_PostHiss:
                return ::eParamiteMotions::Motion_27_PostHiss;
            case eParamiteMotions::Motion_28_GameSpeakEnd:
                return ::eParamiteMotions::Motion_28_GameSpeakEnd;
            case eParamiteMotions::Motion_29_GetDepossessedBegin:
                return ::eParamiteMotions::Motion_29_GetDepossessedBegin;
            case eParamiteMotions::Motion_30_GetDepossessedEnd:
                return ::eParamiteMotions::Motion_30_GetDepossessedEnd;
            case eParamiteMotions::Motion_31_RunningAttack:
                return ::eParamiteMotions::Motion_31_RunningAttack;
            case eParamiteMotions::Motion_32_Empty:
                return ::eParamiteMotions::Motion_32_Empty;
            case eParamiteMotions::Motion_33_SurpriseWeb:
                return ::eParamiteMotions::Motion_33_SurpriseWeb;
            case eParamiteMotions::Motion_34_WebLeaveDown:
                return ::eParamiteMotions::Motion_34_WebLeaveDown;
            case eParamiteMotions::Motion_35_WebIdle:
                return ::eParamiteMotions::Motion_35_WebIdle;
            case eParamiteMotions::Motion_36_WebGoingUp:
                return ::eParamiteMotions::Motion_36_WebGoingUp;
            case eParamiteMotions::Motion_37_WebGoingDown:
                return ::eParamiteMotions::Motion_37_WebGoingDown;
            case eParamiteMotions::Motion_38_WebGrab:
                return ::eParamiteMotions::Motion_38_WebGrab;
            case eParamiteMotions::Motion_39_WebLeaveUp:
                return ::eParamiteMotions::Motion_39_WebLeaveUp;
            case eParamiteMotions::Motion_40_Eating:
                return ::eParamiteMotions::Motion_40_Eating;
            case eParamiteMotions::Motion_41_Death:
                return ::eParamiteMotions::Motion_41_Death;
            case eParamiteMotions::Motion_42_Squawk:
                return ::eParamiteMotions::Motion_42_Squawk;
            case eParamiteMotions::Motion_43_Attack:
                return ::eParamiteMotions::Motion_43_Attack;
            default:
                ALIVE_FATAL("Bad paramite motion value %d", static_cast<s16>(motion));
        }
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(ParamiteSaveState, 0x78);

struct BirdPortalSaveState final
{
    enum class PortalStates : u8
    {
        CreatePortal_0 = 0,
        IdlePortal_1 = 1,
        JoinDovesInCenter_2 = 2,
        KillDoves_3 = 3,
        CreateTerminators_4 = 4,
        ExpandTerminators_5 = 5,
        ActivePortal_6 = 6,
        ShrykullGetDoves_7 = 7,
        Unused_8 = 8,
        GetShrykull_9 = 9,
        CollapseTerminators_10 = 10,
        StopSound_11 = 11,
        CreateFlash1_12 = 12,
        CreateFlash2_13 = 13,
        CreateFlash3_14 = 14,
        KillPortal_15 = 15,
        AbeInsidePortal_16 = 16,
        PortalExit_SetPosition_17 = 17,
        PortalExit_CreateTerminators_18 = 18,
        PortalExit_ExpandTerminators_19 = 19,
        PortalExit_AbeExitting_20 = 20,
        KillPortalClipper_21 = 21,
        FadeoutTerminators_22 = 22,
    };

    AETypes mType;
    PortalStates mState;
    u8 mMudCountForShrykull;
    s32 mTlvInfo;

    static ::BirdPortalSaveState From(const BirdPortalSaveState& data)
    {
        ::BirdPortalSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mState = From(data.mState);
        d.mMudCountForShrykull = data.mMudCountForShrykull;
        d.mTlvInfo = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        return d;
    }

    static ::PortalStates From(const PortalStates& state)
    {
        switch (state)
        {
            case PortalStates::CreatePortal_0:
                return ::PortalStates::CreatePortal_0;
            case PortalStates::IdlePortal_1:
                return ::PortalStates::IdlePortal_1;
            case PortalStates::JoinDovesInCenter_2:
                return ::PortalStates::JoinDovesInCenter_2;
            case PortalStates::KillDoves_3:
                return ::PortalStates::KillDoves_3;
            case PortalStates::CreateTerminators_4:
                return ::PortalStates::CreateTerminators_4;
            case PortalStates::ExpandTerminators_5:
                return ::PortalStates::ExpandTerminators_5;
            case PortalStates::ActivePortal_6:
                return ::PortalStates::ActivePortal_6;
            case PortalStates::ShrykullGetDoves_7:
                return ::PortalStates::ShrykullGetDoves_7;
            case PortalStates::Unused_8:
                ALIVE_FATAL("Never expected PortalStates::Unused_8 to be used");
                //return ::PortalStates::Unused_8;
            case PortalStates::GetShrykull_9:
                return ::PortalStates::GetShrykull_9;
            case PortalStates::CollapseTerminators_10:
                return ::PortalStates::CollapseTerminators_10;
            case PortalStates::StopSound_11:
                return ::PortalStates::StopSound_11;
            case PortalStates::CreateFlash1_12:
                return ::PortalStates::CreateFlash1_12;
            case PortalStates::CreateFlash2_13:
                return ::PortalStates::CreateFlash2_13;
            case PortalStates::CreateFlash3_14:
                return ::PortalStates::CreateFlash3_14;
            case PortalStates::KillPortal_15:
                return ::PortalStates::KillPortal_15;
            case PortalStates::AbeInsidePortal_16:
                return ::PortalStates::AbeInsidePortal_16;
            case PortalStates::PortalExit_SetPosition_17:
                return ::PortalStates::PortalExit_SetPosition_17;
            case PortalStates::PortalExit_CreateTerminators_18:
                return ::PortalStates::PortalExit_CreateTerminators_18;
            case PortalStates::PortalExit_ExpandTerminators_19:
                return ::PortalStates::PortalExit_ExpandTerminators_19;
            case PortalStates::PortalExit_AbeExitting_20:
                return ::PortalStates::PortalExit_AbeExitting_20;
            case PortalStates::KillPortalClipper_21:
                return ::PortalStates::KillPortalClipper_21;
            case PortalStates::FadeoutTerminators_22:
                return ::PortalStates::FadeoutTerminators_22;
            default:
                ALIVE_FATAL("Bad portal state value %d", static_cast<s32>(state));
        }
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(BirdPortalSaveState, 8);

struct ThrowableArraySaveState final
{
    s16 field_0_unused;
    s16 field_2_item_count;

    static ::ThrowableArraySaveState From(const ThrowableArraySaveState& data)
    {
        ::ThrowableArraySaveState d;
        d.mCount = data.field_2_item_count;
        return d;
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(ThrowableArraySaveState, 0x4);


enum class RingTypes : s16
{
    // Red rings
    eExplosive_Pulse_0 = 0,
    eExplosive_Emit_1 = 1,        // Actually blows stuff up
    eExplosive_Emit_Effect_2 = 2, // Graphical effect only
    eExplosive_Give_3 = 3,

    // Blue rings
    eShrykull_Pulse_Small_4 = 4,
    eShrykull_Pulse_Large_5 = 5,
    eShrykull_Pulse_Orange_6 = 6, // Never used?

    // Green rings
    eInvisible_Pulse_Small_7 = 7,
    eInvisible_Pulse_Large_8 = 8,
    eInvisible_Pulse_Emit_9 = 9,
    eInvisible_Pulse_Give_10 = 10,

    // Yellow rings
    eHealing_Emit_Effect_11 = 11, // Graphical effect only
    eHealing_Emit_12 = 12,        // Actually heal muds
    eHealing_Give_13 = 13,
    eHealing_Pulse_14 = 14,
};

struct AbilityRingSaveState final
{
    AETypes mRingObjectType;
    FP mRingXPos;
    FP mRingYPos;
    RingTypes mRingType;
    FP mRingScale;
    s32 mRingTlvInfo;
    FP mRingRight;
    s32 mRingCount;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;

    static ::AbilityRingSaveState From(const AbilityRingSaveState& data)
    {
        ::AbilityRingSaveState d;
        d.mRingObjectType = BaseGameObject::FromAE(data.mRingObjectType);
        d.mRingXPos = data.mRingXPos;
        d.mRingYPos = data.mRingYPos;
        d.mRingType = From(data.mRingType);
        d.mRingScale = data.mRingScale;
        d.mRingTlvInfo = Guid::NewGuidFromTlvInfo(data.mRingTlvInfo);
        d.mRingRight = data.mRingRight;
        d.mRingCount = data.mRingCount;
        d.mRingRed = data.mRingRed;
        d.mRingGreen = data.mRingGreen;
        d.mRingBlue = data.mRingBlue;
        return d;
    }

    static ::RingTypes From(const RingTypes type)
    {
        switch (type)
        {
            case RingTypes::eExplosive_Pulse_0:
                return ::RingTypes::eExplosive_Pulse_0;
            case RingTypes::eExplosive_Emit_1:
                return ::RingTypes::eExplosive_Emit_1;
            case RingTypes::eExplosive_Emit_Effect_2:
                return ::RingTypes::eExplosive_Emit_Effect_2;
            case RingTypes::eExplosive_Give_3:
                return ::RingTypes::eExplosive_Give_3;
            case RingTypes::eShrykull_Pulse_Small_4:
                return ::RingTypes::eShrykull_Pulse_Small_4;
            case RingTypes::eShrykull_Pulse_Large_5:
                return ::RingTypes::eShrykull_Pulse_Large_5;
            case RingTypes::eShrykull_Pulse_Orange_6:
                return ::RingTypes::eShrykull_Pulse_Orange_6;
            case RingTypes::eInvisible_Pulse_Small_7:
                return ::RingTypes::eInvisible_Pulse_Small_7;
            case RingTypes::eInvisible_Pulse_Large_8:
                return ::RingTypes::eInvisible_Pulse_Large_8;
            case RingTypes::eInvisible_Pulse_Emit_9:
                return ::RingTypes::eInvisible_Pulse_Emit_9;
            case RingTypes::eInvisible_Pulse_Give_10:
                return ::RingTypes::eInvisible_Pulse_Give_10;
            case RingTypes::eHealing_Emit_Effect_11:
                return ::RingTypes::eHealing_Emit_Effect_11;
            case RingTypes::eHealing_Emit_12:
                return ::RingTypes::eHealing_Emit_12;
            case RingTypes::eHealing_Give_13:
                return ::RingTypes::eHealing_Give_13;
            case RingTypes::eHealing_Pulse_14:
                return ::RingTypes::eHealing_Pulse_14;
        }
        ALIVE_FATAL("Bad ability ring type value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(AbilityRingSaveState, 0x28);

struct RockSaveState final
{
    AETypes mType;
    s16 field_2_padding;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    FP field_18_sprite_scale;
    s16 field_1C_path_number;
    LevelIds field_1E_lvl_number;
    enum RockStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8
    };
    BitField16<RockStateFlags> field_20_flags;
    s16 field_22_padding;
    s32 field_24_id;
    eLineTypes field_28_line_type;
    s16 field_2A_count;
    enum class RockStates : s16
    {
        eNone_0 = 0,
        eFallingOutOfRockSack_1 = 1,
        eRolling_2 = 2,
        eOnGround_3 = 3,
        eBouncing_4 = 4,
        eFallingOutOfWorld_5 = 5,
    };
    RockStates field_2C_state;
    s16 field_2E_volume;
    FP field_30_xpos;
    FP field_34_ypos;

    static ::RockSaveState From(const RockSaveState& data)
    {
        ::RockSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.field_4_obj_id);
        d.mXPos = data.field_8_xpos;
        d.mYPos = data.field_C_ypos;
        d.mVelX = data.field_10_velx;
        d.mVelY = data.field_14_vely;
        d.mSpriteScale = data.field_18_sprite_scale;
        d.mCurrentPath = data.field_1C_path_number;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.field_1E_lvl_number);
        d.mRender = data.field_20_flags.Get(RockStateFlags::eBit1_bRender);
        d.mDrawable = data.field_20_flags.Get(RockStateFlags::eBit2_bDrawable);
        d.mLoop = data.field_20_flags.Get(RockStateFlags::eBit3_bLoop);
        d.mInteractive = data.field_20_flags.Get(RockStateFlags::eBit4_bInteractive);
        d.mPlatformId = Guid::NewGuidFromTlvInfo(data.field_24_id);
        d.mCollisionLineType = AEData::From(data.field_28_line_type);
        d.mThrowableCount = data.field_2A_count;
        d.mState = From(data.field_2C_state);
        d.mBounceCount = data.field_2E_volume;
        d.mPreviousXPos = data.field_30_xpos;
        d.mPreviousYPos = data.field_34_ypos;
        return d;
    }

    static ::RockStates From(const RockStates scale)
    {
        switch (scale)
        {
            case RockStates::eNone_0:
                return ::RockStates::eNone_0;
            case RockStates::eFallingOutOfRockSack_1:
                return ::RockStates::eFallingOutOfRockSack_1;
            case RockStates::eRolling_2:
                return ::RockStates::eRolling_2;
            case RockStates::eOnGround_3:
                return ::RockStates::eOnGround_3;
            case RockStates::eBouncing_4:
                return ::RockStates::eBouncing_4;
            case RockStates::eFallingOutOfWorld_5:
                return ::RockStates::eFallingOutOfWorld_5;
        }
        ALIVE_FATAL("Bad rock state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(RockSaveState, 0x38);

struct ScrabSaveState final
{
    enum class eScrabMotions : s16
    {
        eNone_m1 = -1,
        Motion_0_Stand,
        Motion_1_Walk,
        Motion_2_Run,
        Motion_3_Turn,
        Motion_4_RunToStand,
        Motion_5_HopBegin,
        Motion_6_HopMidair,
        Motion_7_HopLand,
        Motion_8_JumpToFall,
        Motion_9_StandToWalk,
        Motion_10_StandToRun,
        Motion_11_WalkToStand,
        Motion_12_RunJumpBegin,
        Motion_13_RunJumpEnd,
        Motion_14_WalkToFall,
        Motion_15_RunToFall,
        Motion_16_WalkToRun,
        Motion_17_RunToWalk,
        Motion_18_Knockback,
        Motion_19_GetEaten,
        Motion_20_Fall,
        Motion_21_Stamp,
        Motion_22_GetPossessed,
        Motion_23_Empty,
        Motion_24_DeathEnd,
        Motion_25_Empty,
        Motion_26_HowlBegin,
        Motion_27_HowlEnd,
        Motion_28_GetDepossessedBegin,
        Motion_29_GetDepossessedEnd,
        Motion_30_Shriek,
        Motion_31_ScrabBattleAnim,
        Motion_32_AttackSpin,
        Motion_33_FeedToGulp,
        Motion_34_GulpToStand,
        Motion_35_StandToFeed,
        Motion_36_Feed,
        Motion_37_AttackLunge,
        Motion_38_LegKick,
        Motion_39_DeathBegin
    };

    AETypes mType;
    s16 field_2_padding;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    FP field_1C_sprite_scale;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
    s16 field_26_bAnimFlipX;
    eScrabMotions field_28_current_motion;
    s16 field_2A_current_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_bAnimRender;
    s8 field_2F_bDrawable;
    FP field_30_health;
    eScrabMotions field_34_current_motion;
    eScrabMotions field_36_next_motion;
    s16 field_38_last_line_ypos;
    eLineTypes field_3A_line_type;
    s16 field_3C_padding;
    s16 field_3E_padding;
    s8 field_40_bIsControlled;
    s8 field_41_padding;
    s16 field_42_padding;
    s32 field_44_tlvInfo;
    s32 field_48_brain_idx;
    s16 field_4C_padding;
    s16 field_4E_padding;
    s16 field_50_sub_state;
    s16 field_52_padding;
    s32 field_54_obj_id;
    s32 field_58_target_obj_id;
    s32 field_5C_timer;
    s32 field_60_depossession_timer;
    FP field_64_falling_velx_scale_factor;
    s16 field_68_motion_resource_block_index;
    s16 field_6A_padding;
    s32 field_6C_spotting_abe_timer;
    s32 field_70_attack_delay_timer;
    s32 field_74_movement_timer;
    s32 field_78_sfx_bitmask;
    s16 field_7C_prevent_depossession;
    LevelIds field_7E_return_level;
    s16 field_80_return_path;
    s16 field_82_return_camera;
    s16 field_84_input;
    s16 field_86_padding;
    s32 field_88_unused;
    s16 field_8C_shred_power_active;
    GameSpeakEvents field_8E_speak;
    FP field_90_max_xpos;
    FP field_94_max_ypos;
    s16 field_98_speak_counter;
    s16 field_9A_unused;
    s16 field_9C_unused;

    enum Flags_9E
    {
        eBit1_attacking = 0x1,
        eBit2_unused = 0x2,
        eBit3_unused = 0x4,
        eBit4_force_update_animation = 0x8,
        eBit5_roar_randomly = 0x10,
        eBit6_persistant = 0x20,
    };
    BitField16<Flags_9E> field_9E_flags;

    static ::ScrabSaveState From(const ScrabSaveState& data)
    {
        ::ScrabSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_4_obj_id = Guid::NewGuidFromTlvInfo(data.field_4_obj_id);
        d.mXPos = data.field_8_xpos;
        d.mYPos = data.field_C_ypos;
        d.mVelX = data.field_10_velx;
        d.mVelY = data.field_14_vely;
        d.mCurrentPath = data.field_18_path_number;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.field_1A_lvl_number);
        d.mSpriteScale = data.field_1C_sprite_scale;
        d.mRingRed = data.mRingRed;
        d.mRingGreen = data.mRingGreen;
        d.mRingBlue = data.mRingBlue;
        d.mFlipX = data.field_26_bAnimFlipX;
        d.field_28_current_motion = From(data.field_28_current_motion);
        d.mCurrentFrame = data.field_2A_current_frame;
        d.mFrameChangeCounter = data.field_2C_frame_change_counter;
        d.mAnimRender = data.field_2E_bAnimRender;
        d.mDrawable = data.field_2F_bDrawable;
        d.mHealth = data.field_30_health;
        d.field_34_current_motion = From(data.field_34_current_motion);
        d.field_36_next_motion = From(data.field_36_next_motion);
        d.field_38_last_line_ypos = data.field_38_last_line_ypos;
        d.mLineType = AEData::From(data.field_3A_line_type);
        d.mIsControlled = data.field_40_bIsControlled;
        d.field_44_tlvInfo = Guid::NewGuidFromTlvInfo(data.field_44_tlvInfo);
        d.field_48_brain_idx = data.field_48_brain_idx;
        d.mBrainSubState = data.field_50_sub_state;
        d.field_54_obj_id = Guid::NewGuidFromTlvInfo(data.field_54_obj_id);
        d.field_58_target_obj_id = Guid::NewGuidFromTlvInfo(data.field_58_target_obj_id);
        d.field_5C_timer = data.field_5C_timer;
        d.field_60_depossession_timer = data.field_60_depossession_timer;
        d.field_64_falling_velx_scale_factor = data.field_64_falling_velx_scale_factor;
        d.field_68_motion_resource_block_index = data.field_68_motion_resource_block_index;
        d.field_6C_spotting_abe_timer = data.field_6C_spotting_abe_timer;
        d.field_70_attack_delay_timer = data.field_70_attack_delay_timer;
        d.field_74_movement_timer = data.field_74_movement_timer;
        d.field_78_sfx_bitmask = data.field_78_sfx_bitmask;
        d.mPreventDepossession = data.field_7C_prevent_depossession;
        d.mAbeLevel = MapWrapper::FromAESaveData(data.field_7E_return_level);
        d.mAbePath = data.field_80_return_path;
        d.mAbeCamera = data.field_82_return_camera;
        d.field_84_input = data.field_84_input;
        d.field_8C_shred_power_active = data.field_8C_shred_power_active;
        d.field_8E_speak = AEData::From(data.field_8E_speak);
        d.field_90_max_xpos = data.field_90_max_xpos;
        d.field_94_max_ypos = data.field_94_max_ypos;
        d.field_98_speak_counter = data.field_98_speak_counter;
        d.mAttacking = data.field_9E_flags.Get(Flags_9E::eBit1_attacking);
        d.mForceUpdateAnimation = data.field_9E_flags.Get(Flags_9E::eBit4_force_update_animation);
        d.mRoarRandomly = data.field_9E_flags.Get(Flags_9E::eBit5_roar_randomly);
        d.mPersistant = data.field_9E_flags.Get(Flags_9E::eBit6_persistant);
        return d;
    }

    static ::eScrabMotions From(const eScrabMotions motion)
    {
        switch (motion)
        {
            case eScrabMotions::eNone_m1:
                return ::eScrabMotions::eNone_m1;
            case eScrabMotions::Motion_0_Stand:
                return ::eScrabMotions::Motion_0_Stand;
            case eScrabMotions::Motion_1_Walk:
                return ::eScrabMotions::Motion_1_Walk;
            case eScrabMotions::Motion_2_Run:
                return ::eScrabMotions::Motion_2_Run;
            case eScrabMotions::Motion_3_Turn:
                return ::eScrabMotions::Motion_3_Turn;
            case eScrabMotions::Motion_4_RunToStand:
                return ::eScrabMotions::Motion_4_RunToStand;
            case eScrabMotions::Motion_5_HopBegin:
                return ::eScrabMotions::Motion_5_HopBegin;
            case eScrabMotions::Motion_6_HopMidair:
                return ::eScrabMotions::Motion_6_HopMidair;
            case eScrabMotions::Motion_7_HopLand:
                return ::eScrabMotions::Motion_7_HopLand;
            case eScrabMotions::Motion_8_JumpToFall:
                return ::eScrabMotions::Motion_8_JumpToFall;
            case eScrabMotions::Motion_9_StandToWalk:
                return ::eScrabMotions::Motion_9_StandToWalk;
            case eScrabMotions::Motion_10_StandToRun:
                return ::eScrabMotions::Motion_10_StandToRun;
            case eScrabMotions::Motion_11_WalkToStand:
                return ::eScrabMotions::Motion_11_WalkToStand;
            case eScrabMotions::Motion_12_RunJumpBegin:
                return ::eScrabMotions::Motion_12_RunJumpBegin;
            case eScrabMotions::Motion_13_RunJumpEnd:
                return ::eScrabMotions::Motion_13_RunJumpEnd;
            case eScrabMotions::Motion_14_WalkToFall:
                return ::eScrabMotions::Motion_14_WalkToFall;
            case eScrabMotions::Motion_15_RunToFall:
                return ::eScrabMotions::Motion_15_RunToFall;
            case eScrabMotions::Motion_16_WalkToRun:
                return ::eScrabMotions::Motion_16_WalkToRun;
            case eScrabMotions::Motion_17_RunToWalk:
                return ::eScrabMotions::Motion_17_RunToWalk;
            case eScrabMotions::Motion_18_Knockback:
                return ::eScrabMotions::Motion_18_Knockback;
            case eScrabMotions::Motion_19_GetEaten:
                return ::eScrabMotions::Motion_19_GetEaten;
            case eScrabMotions::Motion_20_Fall:
                return ::eScrabMotions::Motion_20_Fall;
            case eScrabMotions::Motion_21_Stamp:
                return ::eScrabMotions::Motion_21_Stamp;
            case eScrabMotions::Motion_22_GetPossessed:
                return ::eScrabMotions::Motion_22_GetPossessed;
            case eScrabMotions::Motion_23_Empty:
                return ::eScrabMotions::Motion_23_Empty;
            case eScrabMotions::Motion_24_DeathEnd:
                return ::eScrabMotions::Motion_24_DeathEnd;
            case eScrabMotions::Motion_25_Empty:
                return ::eScrabMotions::Motion_25_Empty;
            case eScrabMotions::Motion_26_HowlBegin:
                return ::eScrabMotions::Motion_26_HowlBegin;
            case eScrabMotions::Motion_27_HowlEnd:
                return ::eScrabMotions::Motion_27_HowlEnd;
            case eScrabMotions::Motion_28_GetDepossessedBegin:
                return ::eScrabMotions::Motion_28_GetDepossessedBegin;
            case eScrabMotions::Motion_29_GetDepossessedEnd:
                return ::eScrabMotions::Motion_29_GetDepossessedEnd;
            case eScrabMotions::Motion_30_Shriek:
                return ::eScrabMotions::Motion_30_Shriek;
            case eScrabMotions::Motion_31_ScrabBattleAnim:
                return ::eScrabMotions::Motion_31_ScrabBattleAnim;
            case eScrabMotions::Motion_32_AttackSpin:
                return ::eScrabMotions::Motion_32_AttackSpin;
            case eScrabMotions::Motion_33_FeedToGulp:
                return ::eScrabMotions::Motion_33_FeedToGulp;
            case eScrabMotions::Motion_34_GulpToStand:
                return ::eScrabMotions::Motion_34_GulpToStand;
            case eScrabMotions::Motion_35_StandToFeed:
                return ::eScrabMotions::Motion_35_StandToFeed;
            case eScrabMotions::Motion_36_Feed:
                return ::eScrabMotions::Motion_36_Feed;
            case eScrabMotions::Motion_37_AttackLunge:
                return ::eScrabMotions::Motion_37_AttackLunge;
            case eScrabMotions::Motion_38_LegKick:
                return ::eScrabMotions::Motion_38_LegKick;
            case eScrabMotions::Motion_39_DeathBegin:
                return ::eScrabMotions::Motion_39_DeathBegin;
            default:
                ALIVE_FATAL("Bad scrab motion value %d", static_cast<s16>(motion));
        }
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(ScrabSaveState, 0xA0);

struct ScrabSpawnerSaveState final
{
    AETypes mType;
    s16 field_2_pad;
    s32 field_4_tlvInfo;
    enum class ScrabSpawnerStates : s32
    {
        eInactive_0 = 0,
        eScrabSpawned_1 = 1
    };
    ScrabSpawnerStates field_8_state;
    s32 field_C_spawned_scrab_id;

    static ::ScrabSpawnerSaveState From(const ScrabSpawnerSaveState& data)
    {
        ::ScrabSpawnerSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_4_tlvInfo = Guid::NewGuidFromTlvInfo(data.field_4_tlvInfo);
        d.field_8_state = From(data.field_8_state);
        d.field_C_spawned_scrab_id = Guid::NewGuidFromTlvInfo(data.field_C_spawned_scrab_id);
        return d;
    }

    static ::ScrabSpawnerStates From(const ScrabSpawnerStates state)
    {
        switch (state)
        {
            case ScrabSpawnerStates::eInactive_0:
                return ::ScrabSpawnerStates::eInactive_0;
            case ScrabSpawnerStates::eScrabSpawned_1:
                return ::ScrabSpawnerStates::eScrabSpawned_1;
        }
        ALIVE_FATAL("Bad scrab spawner state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(ScrabSpawnerSaveState, 0x10);

struct SlamDoorSaveState final
{
    AETypes mType;
    TlvItemInfoUnion mTlvInfo;

    static ::SlamDoorSaveState From(const SlamDoorSaveState& data)
    {
        ::SlamDoorSaveState d;
        d.mTlvInfo = Guid::NewGuidFromTlvInfo(data.mTlvInfo.all);
        return d;
    }

};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlamDoorSaveState, 8);

struct SligSaveState final
{
    enum class eSligMotions : s16
    {
        eNone_m1 = -1,
        Motion_0_StandIdle = 0,
        Motion_1_StandToWalk = 1,
        Motion_2_Walking = 2,
        Motion_3_StandToRun = 3,
        Motion_4_Running = 4,
        Motion_5_TurnAroundStanding = 5,
        Motion_6_Shoot = 6,
        Motion_7_Falling = 7,
        Motion_8_SlidingToStand = 8,
        Motion_9_SlidingTurn = 9,
        Motion_10_SlidingTurnToWalk = 10,
        Motion_11_SlidingTurnToRun = 11,
        Motion_12_ReloadGun = 12,
        Motion_13_ShootToStand = 13,
        Motion_14_SteppingToStand = 14,
        Motion_15_StandingToStep = 15,
        Motion_16_DepossessingAbort = 16,
        Motion_17_GameSpeak = 17,
        Motion_18_WalkToStand = 18,
        Motion_19_Recoil = 19,
        Motion_20_SpeakHereBoy = 20,
        Motion_21_SpeakHi = 21,
        Motion_22_SpeakFreeze = 22,
        Motion_23_SpeakGetHim = 23,
        Motion_24_SpeakLaugh = 24,
        Motion_25_SpeakBullshit1 = 25,
        Motion_26_SpeakLookOut = 26,
        Motion_27_SpeakBullshit2 = 27,
        Motion_28_SpeakPanic = 28,
        Motion_29_SpeakWhat = 29,
        Motion_30_SpeakAIFreeze = 30,
        Motion_31_Blurgh = 31,
        Motion_32_Sleeping = 32,
        Motion_33_SleepingToStand = 33,
        Motion_34_Knockback = 34,
        Motion_35_KnockbackToStand = 35,
        Motion_36_Depossessing = 36,
        Motion_37_Possess = 37,
        Motion_38_OutToFall = 38,
        Motion_39_FallingInitiate = 39,
        Motion_40_LandingSoft = 40,
        Motion_41_LandingFatal = 41,
        Motion_42_ShootZ = 42,
        Motion_43_ShootZtoStand = 43,
        Motion_44_Smash = 44,
        Motion_45_PullLever = 45,
        Motion_46_LiftGrip = 46,
        Motion_47_LiftUngrip = 47,
        Motion_48_LiftGripping = 48,
        Motion_49_LiftUp = 49,
        Motion_50_LiftDown = 50,
        Motion_51_Beat = 51
    };

    AETypes mType;
    s16 field_2_padding;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    LevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    Scale field_1C_scale;
    s16 field_1E_r;
    s16 field_20_g;
    s16 field_22_b;
    s16 field_24_bFlipX;
    eSligMotions field_26_current_motion;
    s16 field_28_current_frame;
    s16 field_2A_frame_change_counter;
    s8 field_2C_bRender;
    s8 field_2D_bDrawable;
    s16 field_2E_padding;
    FP field_30_health;
    eSligMotions field_34_current_motion;
    eSligMotions field_36_next_motion;
    s16 field_38_last_line_ypos;
    eLineTypes field_3A_collision_line_type;
    s16 field_3C_padding;
    s16 field_3E_padding;
    s8 field_40_bActiveChar;
    s8 field_41_padding;
    s16 field_42_brain_sub_state;
    s16 field_44_pitch_min;
    s16 field_46_padding;
    s32 field_48_timer;
    s16 field_4C_return_to_previous_motion;
    s16 field_4E_checked_if_off_screen;
    s16 field_50_input;
    s16 field_52_padding;
    s32 field_54_timer;
    FP field_58_falling_velx_scale_factor;
    s32 field_5C_tlvInfo;
    s16 field_60_res_idx;
    eSligMotions field_62_shot_motion;
    PSX_RECT field_64_zone_rect;
    s16 field_6C_unused;
    s16 field_6E_unused;
    s16 field_70_unused;
    LevelIds field_72_return_level;
    s16 field_74_return_path;
    s16 field_76_return_camera;
    s32 field_78_death_by_being_shot_timer;
    s32 field_7C_explode_timer;
    s32 field_80_brain_state_idx;
    s16 field_84_padding;
    s16 field_86_padding;
    s32 field_88_unused;
    s32 field_8C_num_times_to_shoot;
    s16 field_90_force_alive_state;
    s16 field_92_spotted_possessed_slig;
    s32 field_94_glukkon_id;
    s16 field_98_state_after_speak;
    s16 field_9A_attention_timeout;
    s16 field_9C_unused;
    s16 field_9E_next_command_arg1;
    s16 field_A0_cmd_idx;
    enum Flags_A2
    {
        eBit1_Padding = 0x1,
        eBit2_FollowGlukkon = 0x2,
        eBit3_StoppedForLeverOrLift = 0x4,
        eBit4_GlukkonCalledAllOYa = 0x8,
        eBit5_HeardGlukkon = 0x10,
    };
    BitField16<Flags_A2> field_A2_flags;

    static ::SligSaveState From(const SligSaveState& data)
    {
        ::SligSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.field_4_xpos = data.field_4_xpos;
        d.field_8_ypos = data.field_8_ypos;
        d.field_C_velx = data.field_C_velx;
        d.field_10_vely = data.field_10_vely;
        d.field_14_path_number = data.field_14_path_number;
        d.field_16_lvl_number = MapWrapper::FromAESaveData(data.field_16_lvl_number);
        d.field_18_sprite_scale = data.field_18_sprite_scale;
        d.field_1C_scale = AEData::From(data.field_1C_scale);
        d.field_1E_r = data.field_1E_r;
        d.field_20_g = data.field_20_g;
        d.field_22_b = data.field_22_b;
        d.field_24_bFlipX = data.field_24_bFlipX;
        d.field_26_current_motion = From(data.field_26_current_motion);
        d.field_28_current_frame = data.field_28_current_frame;
        d.field_2A_frame_change_counter = data.field_2A_frame_change_counter;
        d.field_2C_bRender = data.field_2C_bRender;
        d.field_2D_bDrawable = data.field_2D_bDrawable;
        d.field_30_health = data.field_30_health;
        d.field_34_current_motion = From(data.field_34_current_motion);
        d.field_36_next_motion = From(data.field_36_next_motion);
        d.field_38_last_line_ypos = data.field_38_last_line_ypos;
        d.field_3A_collision_line_type = AEData::From(data.field_3A_collision_line_type);
        d.field_40_bActiveChar = data.field_40_bActiveChar;
        d.field_42_brain_sub_state = data.field_42_brain_sub_state;
        d.field_44_pitch_min = data.field_44_pitch_min;
        d.field_48_timer = data.field_48_timer;
        d.mReturnToPreviousMotion = data.field_4C_return_to_previous_motion;
        d.mCheckedIfOffScreen = data.field_4E_checked_if_off_screen;
        d.field_50_input = data.field_50_input;
        d.field_54_timer = data.field_54_timer;
        d.mFallingVelxScaleFactor = data.field_58_falling_velx_scale_factor;
        d.field_5C_tlvInfo = Guid::NewGuidFromTlvInfo(data.field_5C_tlvInfo);
        d.field_60_res_idx = data.field_60_res_idx;
        d.field_62_shot_motion = From(data.field_62_shot_motion);
        d.field_64_zone_rect = AEData::From(data.field_64_zone_rect);
        d.field_72_return_level = MapWrapper::FromAESaveData(data.field_72_return_level);
        d.field_74_return_path = data.field_74_return_path;
        d.field_76_return_camera = data.field_76_return_camera;
        d.field_78_death_by_being_shot_timer = data.field_78_death_by_being_shot_timer;
        d.field_7C_explode_timer = data.field_7C_explode_timer;
        d.field_80_brain_state_idx = data.field_80_brain_state_idx;
        d.field_8C_num_times_to_shoot = data.field_8C_num_times_to_shoot;
        d.field_90_force_alive_state = data.field_90_force_alive_state;
        d.field_92_spotted_possessed_slig = data.field_92_spotted_possessed_slig;
        d.field_94_glukkon_id = Guid::NewGuidFromTlvInfo(data.field_94_glukkon_id);
        d.field_98_state_after_speak = data.field_98_state_after_speak;
        d.field_9A_attention_timeout = data.field_9A_attention_timeout;
        d.field_9E_next_command_arg1 = data.field_9E_next_command_arg1;
        d.field_A0_cmd_idx = data.field_A0_cmd_idx;
        d.mFollowGlukkon = data.field_A2_flags.Get(Flags_A2::eBit2_FollowGlukkon);
        d.mStoppedForLeverOrLift = data.field_A2_flags.Get(Flags_A2::eBit3_StoppedForLeverOrLift);
        d.mGlukkonCalledAllOYa = data.field_A2_flags.Get(Flags_A2::eBit4_GlukkonCalledAllOYa);
        d.mHeardGlukkon = data.field_A2_flags.Get(Flags_A2::eBit5_HeardGlukkon);
        return d;
    }

    static ::eSligMotions From(const eSligMotions motion)
    {
        switch (motion)
        {
            case eSligMotions::eNone_m1:
                return ::eSligMotions::eNone_m1;
            case eSligMotions::Motion_0_StandIdle:
                return ::eSligMotions::Motion_0_StandIdle;
            case eSligMotions::Motion_1_StandToWalk:
                return ::eSligMotions::Motion_1_StandToWalk;
            case eSligMotions::Motion_2_Walking:
                return ::eSligMotions::Motion_2_Walking;
            case eSligMotions::Motion_3_StandToRun:
                return ::eSligMotions::Motion_3_StandToRun;
            case eSligMotions::Motion_4_Running:
                return ::eSligMotions::Motion_4_Running;
            case eSligMotions::Motion_5_TurnAroundStanding:
                return ::eSligMotions::Motion_5_TurnAroundStanding;
            case eSligMotions::Motion_6_Shoot:
                return ::eSligMotions::Motion_6_Shoot;
            case eSligMotions::Motion_7_Falling:
                return ::eSligMotions::Motion_7_Falling;
            case eSligMotions::Motion_8_SlidingToStand:
                return ::eSligMotions::Motion_8_SlidingToStand;
            case eSligMotions::Motion_9_SlidingTurn:
                return ::eSligMotions::Motion_9_SlidingTurn;
            case eSligMotions::Motion_10_SlidingTurnToWalk:
                return ::eSligMotions::Motion_10_SlidingTurnToWalk;
            case eSligMotions::Motion_11_SlidingTurnToRun:
                return ::eSligMotions::Motion_11_SlidingTurnToRun;
            case eSligMotions::Motion_12_ReloadGun:
                return ::eSligMotions::Motion_12_ReloadGun;
            case eSligMotions::Motion_13_ShootToStand:
                return ::eSligMotions::Motion_13_ShootToStand;
            case eSligMotions::Motion_14_SteppingToStand:
                return ::eSligMotions::Motion_14_SteppingToStand;
            case eSligMotions::Motion_15_StandingToStep:
                return ::eSligMotions::Motion_15_StandingToStep;
            case eSligMotions::Motion_16_DepossessingAbort:
                return ::eSligMotions::Motion_16_DepossessingAbort;
            case eSligMotions::Motion_17_GameSpeak:
                return ::eSligMotions::Motion_17_GameSpeak;
            case eSligMotions::Motion_18_WalkToStand:
                return ::eSligMotions::Motion_18_WalkToStand;
            case eSligMotions::Motion_19_Recoil:
                return ::eSligMotions::Motion_19_Recoil;
            case eSligMotions::Motion_20_SpeakHereBoy:
                return ::eSligMotions::Motion_20_SpeakHereBoy;
            case eSligMotions::Motion_21_SpeakHi:
                return ::eSligMotions::Motion_21_SpeakHi;
            case eSligMotions::Motion_22_SpeakFreeze:
                return ::eSligMotions::Motion_22_SpeakFreeze;
            case eSligMotions::Motion_23_SpeakGetHim:
                return ::eSligMotions::Motion_23_SpeakGetHim;
            case eSligMotions::Motion_24_SpeakLaugh:
                return ::eSligMotions::Motion_24_SpeakLaugh;
            case eSligMotions::Motion_25_SpeakBullshit1:
                return ::eSligMotions::Motion_25_SpeakBullshit1;
            case eSligMotions::Motion_26_SpeakLookOut:
                return ::eSligMotions::Motion_26_SpeakLookOut;
            case eSligMotions::Motion_27_SpeakBullshit2:
                return ::eSligMotions::Motion_27_SpeakBullshit2;
            case eSligMotions::Motion_28_SpeakPanic:
                return ::eSligMotions::Motion_28_SpeakPanic;
            case eSligMotions::Motion_29_SpeakWhat:
                return ::eSligMotions::Motion_29_SpeakWhat;
            case eSligMotions::Motion_30_SpeakAIFreeze:
                return ::eSligMotions::Motion_30_SpeakAIFreeze;
            case eSligMotions::Motion_31_Blurgh:
                return ::eSligMotions::Motion_31_Blurgh;
            case eSligMotions::Motion_32_Sleeping:
                return ::eSligMotions::Motion_32_Sleeping;
            case eSligMotions::Motion_33_SleepingToStand:
                return ::eSligMotions::Motion_33_SleepingToStand;
            case eSligMotions::Motion_34_Knockback:
                return ::eSligMotions::Motion_34_Knockback;
            case eSligMotions::Motion_35_KnockbackToStand:
                return ::eSligMotions::Motion_35_KnockbackToStand;
            case eSligMotions::Motion_36_Depossessing:
                return ::eSligMotions::Motion_36_Depossessing;
            case eSligMotions::Motion_37_Possess:
                return ::eSligMotions::Motion_37_Possess;
            case eSligMotions::Motion_38_OutToFall:
                return ::eSligMotions::Motion_38_OutToFall;
            case eSligMotions::Motion_39_FallingInitiate:
                return ::eSligMotions::Motion_39_FallingInitiate;
            case eSligMotions::Motion_40_LandingSoft:
                return ::eSligMotions::Motion_40_LandingSoft;
            case eSligMotions::Motion_41_LandingFatal:
                return ::eSligMotions::Motion_41_LandingFatal;
            case eSligMotions::Motion_42_ShootZ:
                return ::eSligMotions::Motion_42_ShootZ;
            case eSligMotions::Motion_43_ShootZtoStand:
                return ::eSligMotions::Motion_43_ShootZtoStand;
            case eSligMotions::Motion_44_Smash:
                return ::eSligMotions::Motion_44_Smash;
            case eSligMotions::Motion_45_PullLever:
                return ::eSligMotions::Motion_45_PullLever;
            case eSligMotions::Motion_46_LiftGrip:
                return ::eSligMotions::Motion_46_LiftGrip;
            case eSligMotions::Motion_47_LiftUngrip:
                return ::eSligMotions::Motion_47_LiftUngrip;
            case eSligMotions::Motion_48_LiftGripping:
                return ::eSligMotions::Motion_48_LiftGripping;
            case eSligMotions::Motion_49_LiftUp:
                return ::eSligMotions::Motion_49_LiftUp;
            case eSligMotions::Motion_50_LiftDown:
                return ::eSligMotions::Motion_50_LiftDown;
            case eSligMotions::Motion_51_Beat:
                return ::eSligMotions::Motion_51_Beat;
            default:
                ALIVE_FATAL("Bad slig motion value %d", static_cast<s32>(motion));
        }
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SligSaveState, 0xA4);

struct SlogSaveState final
{
    enum class eSlogMotions : s16
    {
        m2 = -2,
        m1 = -1,
        Motion_0_Idle,
        Motion_1_Walk,
        Motion_2_Run,
        Motion_3_TurnAround,
        Motion_4_Fall,
        Motion_5_MoveHeadUpwards,
        Motion_6_StopRunning,
        Motion_7_SlideTurn,
        Motion_8_StartWalking,
        Motion_9_EndWalking,
        Motion_10_Land,
        Motion_11_Unused,
        Motion_12_StartFastBarking,
        Motion_13_EndFastBarking,
        Motion_14_AngryBark,
        Motion_15_Sleeping,
        Motion_16_MoveHeadDownwards,
        Motion_17_WakeUp,
        Motion_18_JumpForwards,
        Motion_19_JumpUpwards,
        Motion_20_Eating,
        Motion_21_Dying,
        Motion_22_Scratch,
        Motion_23_Growl
    };

    enum class eSlogBrains : s16
    {
        Brain_0_ListeningToSlig,
        Brain_1_Idle,
        Brain_2_ChasingAbe,
        Brain_3_Death
    };

    AETypes mType;
    s16 field_2_padding;
    s32 mBaseTlvInfo;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;
    FP mSpriteScale;
    s16 mR;
    s16 mG;
    s16 mB;
    s16 mFlipX;
    eSlogMotions mCurrentMotion;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mRender;
    s8 mDrawable;
    FP mHealth;
    eSlogMotions mCurrentMotion2;
    eSlogMotions mNextMotion;
    s16 mLastLineYPos;
    eLineTypes mCollisionLineType;
    s32 mPlatformTlvInfo;
    s32 mSlogTlvInfo;
    s32 mTargetId;
    eSlogBrains mBrainState;
    s16 mBrainSubState;
    s32 mMultiUseTimer;
    FP mFallingVelxScaleFactor;
    s32 mListeningToSligId;
    s16 mHasWoofed;
    s16 mWaitingCounter;
    s16 mResponseIdx;
    s16 mResponsePart;
    s16 mAngerLevel;
    s16 mJumpCounter;
    s32 mScratchTimer;
    s32 mGrowlTimer;
    s32 mBoneId;
    s16 mChaseDelay;
    u8 mSlogRandomIdx;
    u8 field_73_padding;

    enum Flags_74
    {
        eBit1_BitingTarget = 0x1,
        eBit2_Possessed = 0x2, // unused
        eBit3_Asleep = 0x4,
        eBit4_MovedOffScreen = 0x8,
        eBit5_StopRunning = 0x10,
        eBit6_Shot = 0x20,
        eBit7_Hungry = 0x40,
        eBit8_CommandedToAttack = 0x80,
        eBit9_HitByAbilityRing = 0x100,
        eBit10_ListenToSligs = 0x200,
    };
    BitField16<Flags_74> field_74_flags;
    s16 field_76_padding;

    static ::SlogSaveState From(const SlogSaveState& data)
    {
        ::SlogSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mBaseTlvId = Guid::NewGuidFromTlvInfo(data.mBaseTlvInfo);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mVelY = data.mVelY;
        d.mCurrentPath = data.mCurrentPath;
        d.mCurrentLevel = MapWrapper::FromAESaveData(data.mCurrentLevel);
        d.mSpriteScale = data.mSpriteScale;
        d.mR = data.mR;
        d.mG = data.mG;
        d.mB = data.mB;
        d.mFlipX = data.mFlipX;
        d.mCurrentMotion = From(data.mCurrentMotion);
        d.mCurrentFrame = data.mCurrentFrame;
        d.mFrameChangeCounter = data.mFrameChangeCounter;
        d.mRender = data.mRender;
        d.mDrawable = data.mDrawable;
        d.mHealth = data.mHealth;
        d.mCurrentMotion2 = From(data.mCurrentMotion2);
        d.mNextMotion = From(data.mNextMotion);
        d.mLastLineYPos = data.mLastLineYPos;
        d.mCollisionLineType = AEData::From(data.mCollisionLineType);
        d.mPlatformId = Guid::NewGuidFromTlvInfo(data.mPlatformTlvInfo);
        d.mSlogTlvId = Guid::NewGuidFromTlvInfo(data.mSlogTlvInfo);
        d.mTargetId = Guid::NewGuidFromTlvInfo(data.mTargetId);
        d.mBrainState = From(data.mBrainState);
        d.mBrainSubState = data.mBrainSubState;
        d.mMultiUseTimer = data.mMultiUseTimer;
        d.mFallingVelxScaleFactor = data.mFallingVelxScaleFactor;
        d.mListeningToSligId = Guid::NewGuidFromTlvInfo(data.mListeningToSligId);
        d.mHasWoofed = data.mHasWoofed;
        d.mWaitingCounter = data.mWaitingCounter;
        d.mResponseIdx = data.mResponseIdx;
        d.mResponsePart = data.mResponsePart;
        d.mAngerLevel = data.mAngerLevel;
        d.mJumpCounter = data.mJumpCounter;
        d.mScratchTimer = data.mScratchTimer;
        d.mGrowlTimer = data.mGrowlTimer;
        d.mBoneId = Guid::NewGuidFromTlvInfo(data.mBoneId);
        d.mChaseDelay = data.mChaseDelay;
        d.mSlogRandomIdx = data.mSlogRandomIdx;
        d.mBitingTarget = data.field_74_flags.Get(Flags_74::eBit1_BitingTarget);
        d.eBit3_Asleep = data.field_74_flags.Get(Flags_74::eBit3_Asleep);
        d.mMovedOffScreen = data.field_74_flags.Get(Flags_74::eBit4_MovedOffScreen);
        d.mStopRunning = data.field_74_flags.Get(Flags_74::eBit5_StopRunning);
        d.mShot = data.field_74_flags.Get(Flags_74::eBit6_Shot);
        d.mHungry = data.field_74_flags.Get(Flags_74::eBit7_Hungry);
        d.mCommandedToAttack = data.field_74_flags.Get(Flags_74::eBit8_CommandedToAttack);
        d.mHitByAbilityRing = data.field_74_flags.Get(Flags_74::eBit9_HitByAbilityRing);
        d.mListenToSligs = data.field_74_flags.Get(Flags_74::eBit10_ListenToSligs);
        return d;
    }

    static ::eSlogMotions From(const eSlogMotions motion)
    {
        switch (motion)
        {
            case eSlogMotions::m2:
                return ::eSlogMotions::m2;
            case eSlogMotions::m1:
                return ::eSlogMotions::m1;
            case eSlogMotions::Motion_0_Idle:
                return ::eSlogMotions::Motion_0_Idle;
            case eSlogMotions::Motion_1_Walk:
                return ::eSlogMotions::Motion_1_Walk;
            case eSlogMotions::Motion_2_Run:
                return ::eSlogMotions::Motion_2_Run;
            case eSlogMotions::Motion_3_TurnAround:
                return ::eSlogMotions::Motion_3_TurnAround;
            case eSlogMotions::Motion_4_Fall:
                return ::eSlogMotions::Motion_4_Fall;
            case eSlogMotions::Motion_5_MoveHeadUpwards:
                return ::eSlogMotions::Motion_5_MoveHeadUpwards;
            case eSlogMotions::Motion_6_StopRunning:
                return ::eSlogMotions::Motion_6_StopRunning;
            case eSlogMotions::Motion_7_SlideTurn:
                return ::eSlogMotions::Motion_7_SlideTurn;
            case eSlogMotions::Motion_8_StartWalking:
                return ::eSlogMotions::Motion_8_StartWalking;
            case eSlogMotions::Motion_9_EndWalking:
                return ::eSlogMotions::Motion_9_EndWalking;
            case eSlogMotions::Motion_10_Land:
                return ::eSlogMotions::Motion_10_Land;
            case eSlogMotions::Motion_11_Unused:
                return ::eSlogMotions::Motion_11_Unused;
            case eSlogMotions::Motion_12_StartFastBarking:
                return ::eSlogMotions::Motion_12_StartFastBarking;
            case eSlogMotions::Motion_13_EndFastBarking:
                return ::eSlogMotions::Motion_13_EndFastBarking;
            case eSlogMotions::Motion_14_AngryBark:
                return ::eSlogMotions::Motion_14_AngryBark;
            case eSlogMotions::Motion_15_Sleeping:
                return ::eSlogMotions::Motion_15_Sleeping;
            case eSlogMotions::Motion_16_MoveHeadDownwards:
                return ::eSlogMotions::Motion_16_MoveHeadDownwards;
            case eSlogMotions::Motion_17_WakeUp:
                return ::eSlogMotions::Motion_17_WakeUp;
            case eSlogMotions::Motion_18_JumpForwards:
                return ::eSlogMotions::Motion_18_JumpForwards;
            case eSlogMotions::Motion_19_JumpUpwards:
                return ::eSlogMotions::Motion_19_JumpUpwards;
            case eSlogMotions::Motion_20_Eating:
                return ::eSlogMotions::Motion_20_Eating;
            case eSlogMotions::Motion_21_Dying:
                return ::eSlogMotions::Motion_21_Dying;
            case eSlogMotions::Motion_22_Scratch:
                return ::eSlogMotions::Motion_22_Scratch;
            case eSlogMotions::Motion_23_Growl:
                return ::eSlogMotions::Motion_23_Growl;
            default:
                ALIVE_FATAL("Bad slog motion value %d", static_cast<s16>(motion));
        }
    }

    static ::eSlogBrains From(const eSlogBrains brain)
    {
        switch (brain)
        {
            case eSlogBrains::Brain_0_ListeningToSlig:
                return ::eSlogBrains::Brain_0_ListeningToSlig;
            case eSlogBrains::Brain_1_Idle:
                return ::eSlogBrains::Brain_1_Idle;
            case eSlogBrains::Brain_2_ChasingAbe:
                return ::eSlogBrains::Brain_2_ChasingAbe;
            case eSlogBrains::Brain_3_Death:
                return ::eSlogBrains::Brain_3_Death;
            default:
                ALIVE_FATAL("Bad slog brain value %d", static_cast<s16>(brain));
        }
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlogSaveState, 0x78);

struct SlurgSaveState final
{
    AETypes mType;
    s16 padding1;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mSlurgSpriteScale;
    s16 mFlipX;
    s16 mCurrentMotion;
    s16 mAnimCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mDrawable;
    s8 mRender;
    s16 padding2;
    s32 mFrameTableOffset;
    s32 mTlvInfo;
    enum class SlurgStates : s16
    {
        eMoving_0 = 0,
        ePausing_1 = 1,
        eBurst_2 = 2
    };
    SlurgStates mSlurgState;
    enum SlurgFlags
    {
        eGoingRight = 0x1,
        eMoving = 0x2,
    };
    BitField16<SlurgFlags> mSlurgFlags;

    static ::SlurgSaveState From(const SlurgSaveState& data)
    {
        ::SlurgSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mXPos = data.mXPos;
        d.mYPos = data.mYPos;
        d.mVelX = data.mVelX;
        d.mSlurgSpriteScale = data.mSlurgSpriteScale;
        d.mFlipX = data.mFlipX;
        d.mCurrentMotion = data.mCurrentMotion;
        d.mAnimCurrentFrame = data.mAnimCurrentFrame;
        d.mFrameChangeCounter = data.mFrameChangeCounter;
        d.mDrawable = data.mDrawable;
        d.mRender = data.mRender;
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        d.mSlurgState = From(data.mSlurgState);
        d.mGoingRight = data.mSlurgFlags.Get(SlurgFlags::eGoingRight);
        d.mMoving = data.mSlurgFlags.Get(SlurgFlags::eMoving);
        return d;
    }

    static ::SlurgStates From(const SlurgStates state)
    {
        switch (state)
        {
            case SlurgStates::eMoving_0:
                return ::SlurgStates::eMoving_0;
            case SlurgStates::ePausing_1:
                return ::SlurgStates::ePausing_1;
            case SlurgStates::eBurst_2:
                return ::SlurgStates::eBurst_2;
        }
        ALIVE_FATAL("Bad slurg state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlurgSaveState, 0x2C);

struct TimerTriggerSaveState final
{
    AETypes mType;
    s32 mTlvInfo;
    s32 mActivationDelayTimer;
    enum class TimerTriggerStates : s16
    {
        eWaitForEnabled_0 = 0,
        eWaitForFirstTrigger_1 = 1,
        eCheckForStartAgain_2 = 2,
        eWaitForSecondTrigger_3 = 3,
    };
    TimerTriggerStates mState;
    s16 mStartingSwitchState;

    static ::TimerTriggerSaveState From(const TimerTriggerSaveState& data)
    {
        ::TimerTriggerSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        d.mActivationDelayTimer = data.mActivationDelayTimer;
        d.mState = From(data.mState);
        d.mStartingSwitchState = data.mStartingSwitchState;
        return d;
    }

    static ::TimerTriggerStates From(const TimerTriggerStates state)
    {
        switch (state)
        {
            case TimerTriggerStates::eWaitForEnabled_0:
                return ::TimerTriggerStates::eWaitForEnabled_0;
            case TimerTriggerStates::eWaitForFirstTrigger_1:
                return ::TimerTriggerStates::eWaitForFirstTrigger_1;
            case TimerTriggerStates::eCheckForStartAgain_2:
                return ::TimerTriggerStates::eCheckForStartAgain_2;
            case TimerTriggerStates::eWaitForSecondTrigger_3:
                return ::TimerTriggerStates::eWaitForSecondTrigger_3;
        }
        ALIVE_FATAL("Bad timer trigger state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TimerTriggerSaveState, 0x10);

struct TrapDoorSaveState final
{
    AETypes mType;
    enum class TrapDoorState : s16
    {
        eClosed_0 = 0,
        eOpening_1 = 1,
        eOpen_2 = 2,
        eClosing_3 = 3,
    };
    TrapDoorState mState;
    s32 mOpenTime;
    s32 mTlvInfo;

    static ::TrapDoorSaveState From(const TrapDoorSaveState& data)
    {
        ::TrapDoorSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mState = From(data.mState);
        d.mOpenTime = data.mOpenTime;
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        return d;
    }

    static ::TrapDoorState From(const TrapDoorState state)
    {
        switch (state)
        {
            case TrapDoorState::eClosed_0:
                return ::TrapDoorState::eClosed_0;
            case TrapDoorState::eOpening_1:
                return ::TrapDoorState::eOpening_1;
            case TrapDoorState::eOpen_2:
                return ::TrapDoorState::eOpen_2;
            case TrapDoorState::eClosing_3:
                return ::TrapDoorState::eClosing_3;
        }
        ALIVE_FATAL("Bad trap door state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TrapDoorSaveState, 0xC);

struct UXBSaveState final
{
    AETypes mType;
    s16 field_2_padding;
    TlvItemInfoUnion mTlvInfo;
    u32 mNextStateTimer;
    enum class UXBState : u16
    {
        eDelay = 0,
        eActive = 1,
        eExploding = 2,
        eDeactivated = 3
    };
    UXBState mCurrentState;
    UXBState mStartingState;
    u16 mDisabledResources;
    u16 mPatternIndex;
    u16 mRedBlinkCount;
    u16 mIsRed;

    static ::UXBSaveState From(const UXBSaveState& data)
    {
        ::UXBSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mTlvInfo = Guid::NewGuidFromTlvInfo(data.mTlvInfo.all);
        d.mNextStateTimer = data.mNextStateTimer;
        d.mCurrentState = From(data.mCurrentState);
        d.mStartingState = From(data.mStartingState);
        d.mPatternIndex = data.mPatternIndex;
        d.mRedBlinkCount = data.mRedBlinkCount;
        d.mIsRed = data.mIsRed;
        return d;
    }

    static ::UXBState From(const UXBState state)
    {
        switch (state)
        {
            case UXBState::eDelay:
                return ::UXBState::eDelay;
            case UXBState::eActive:
                return ::UXBState::eActive;
            case UXBState::eExploding:
                return ::UXBState::eExploding;
            case UXBState::eDeactivated:
                return ::UXBState::eDeactivated;
        }
        ALIVE_FATAL("Bad UXB state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(UXBSaveState, 24);

struct WorkWheelSaveState final
{
    AETypes mType;
    s16 padding_1;
    s32 mTlvInfo;
    s16 mTurningTime;
    s16 padding_2;
    enum class WheelStates : s16
    {
        eIdle_0 = 0,
        eTurning_1 = 1,
    };
    WheelStates mState;
    s16 padding_3;

    static ::WorkWheelSaveState From(const WorkWheelSaveState& data)
    {
        ::WorkWheelSaveState d;
        d.mType = BaseGameObject::FromAE(data.mType);
        d.mTlvId = Guid::NewGuidFromTlvInfo(data.mTlvInfo);
        d.mTurningTime = data.mTurningTime;
        d.mState = From(data.mState);
        return d;
    }

    static ::WheelStates From(const WheelStates state)
    {
        switch (state)
        {
            case WheelStates::eIdle_0:
                return ::WheelStates::eIdle_0;
            case WheelStates::eTurning_1:
                return ::WheelStates::eTurning_1;
        }
        ALIVE_FATAL("Bad wheel state value");
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(WorkWheelSaveState, 0x10);

struct SwitchStates final
{
    s8 mData[256];

    static ::SwitchStates From(const SwitchStates& data)
    {
        ::SwitchStates d = {};
        for (s16 i = 0; i < ALIVE_COUNTOF(data.mData); i++)
        {
            d.mData[i] = data.mData[i];
        }
        return d;
    }
};

struct Quicksave_PSX_Header final
{
    u8 field_0_frame_1_name[128];
    s8 field_80_frame_2_padding[128];
    s8 field_100_frame_3_padding[128];
    s8 field_180_frame_4_padding[128];
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Quicksave_PSX_Header, 0x200);

struct Quicksave_WorldInfo final
{
    s32 mGnFrame;
    LevelIds mLevel;
    s16 mPath;
    s16 mCam;
    s16 mSaveFileId;
    s16 mControlledCharX;
    s16 mControlledCharY;
    s16 mControlledCharScale;
    s16 mRescuedMudokons;
    s16 mKilledMudokons;
    s8 field_16_muds_in_area;
    s8 field_17_last_saved_killed_muds_per_path;
    s8 field_18_saved_killed_muds_per_zulag[20];
    s8 field_2C_current_zulag_number;
    s8 mTotalMeterBars;
    s16 field_2E_use_alt_save_header;
    s16 field_30_bDrawMeterCountDown;
    s16 mVisitedBonewerkz;
    s16 mVisitedBarracks;
    s16 mVisitedFeecoEnder;
    s32 mGasTimer;
    s16 mAbeInvincible;
    s16 field_3E_padding;

    static ::Quicksave_WorldInfo From(const Quicksave_WorldInfo& data)
    {
        ::Quicksave_WorldInfo d;
        d.mGnFrame = data.mGnFrame;
        d.mLevel = data.mLevel;
        d.mPath = data.mPath;
        d.mCam = data.mCam;
        d.mSaveFileId = data.mSaveFileId;
        d.mControlledCharX = data.mControlledCharX;
        d.mControlledCharY = data.mControlledCharY;
        d.mControlledCharScale = data.mControlledCharScale;
        d.mRescuedMudokons = data.mRescuedMudokons;
        d.mKilledMudokons = data.mKilledMudokons;
        d.field_16_muds_in_area = data.field_16_muds_in_area;
        d.field_17_last_saved_killed_muds_per_path = data.field_17_last_saved_killed_muds_per_path;
        for (s16 i = 0; i < ALIVE_COUNTOF(field_18_saved_killed_muds_per_zulag); i++)
        {
            d.field_18_saved_killed_muds_per_zulag[i] = data.field_18_saved_killed_muds_per_zulag[i];
        }
        d.field_2C_current_zulag_number = data.field_2C_current_zulag_number;
        d.mTotalMeterBars = data.mTotalMeterBars;
        d.field_2E_use_alt_save_header = data.field_2E_use_alt_save_header;
        d.field_30_bDrawMeterCountDown = data.field_30_bDrawMeterCountDown;
        d.mVisitedBonewerkz = data.mVisitedBonewerkz;
        d.mVisitedBarracks = data.mVisitedBarracks;
        d.mVisitedFeecoEnder = data.mVisitedFeecoEnder;
        d.mGasTimer = data.mGasTimer;
        d.mAbeInvincible = data.mAbeInvincible;
        return d;
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Quicksave_WorldInfo, 0x40);

struct Quicksave final
{
    Quicksave_PSX_Header field_0_header;
    s32 field_200_accumulated_obj_count;
    Quicksave_WorldInfo field_204_world_info;
    Quicksave_WorldInfo field_244_restart_path_world_info;
    AbeSaveState field_284_restart_path_abe_state;
    SwitchStates field_35C_restart_path_switch_states;
    SwitchStates field_45C_switch_states;
    u8 field_55C_objects_state_data[6820];

    static ::Quicksave From(const Quicksave& data)
    {
        ::Quicksave d;
        d.mAccumulatedObjCount = data.field_200_accumulated_obj_count;
        d.mWorldInfo = Quicksave_WorldInfo::From(data.field_204_world_info);
        d.mRestartPathWorldInfo = Quicksave_WorldInfo::From(data.field_244_restart_path_world_info);
        d.mRestartPathAbeState = AbeSaveState::From(data.field_284_restart_path_abe_state);
        d.mRestartPathSwitchStates = SwitchStates::From(data.field_35C_restart_path_switch_states);
        d.mSwitchStates = SwitchStates::From(data.field_45C_switch_states);
        return d;
    }
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Quicksave, 0x2000);

} // namespace AEData

class AESaveConverter final
{
public:
    bool Convert(const std::vector<u8>& savData, const char_type* pFileName);

private:

    void AddObjectState(nlohmann::json j, const ::SligSpawnerSaveState& d);

    s32 ConvertObjectSaveStateData(nlohmann::json& j, AETypes type, const u8* pData);
};
