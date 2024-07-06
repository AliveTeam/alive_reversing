#pragma once

#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/GameObjects/AbilityRing.hpp"
#include "MudokonEnums.hpp"
#include "../relive_lib/SaveStateBase.hpp"
#include "../relive_lib/FatalError.hpp"

namespace relive
{
    struct Path_Mudokon;
}

#define MUD_BRAIN_STATES_ENUM(ENTRY)        \
    ENTRY(Brain_0_GiveRings)         \
    ENTRY(Brain_1_Chisel)            \
    ENTRY(Brain_2_CrouchScrub)             \
    ENTRY(Brain_3_TurnWheel)         \
    ENTRY(Brain_4_ListeningToAbe)    \
    ENTRY(Brain_5_ShrivelDeath)      \
    ENTRY(Brain_6_Escape)            \
    ENTRY(Brain_7_FallAndSmackDeath) \
    ENTRY(Brain_8_AngryWorker)       \
    ENTRY(Brain_9_Sick)

#define MAKE_STRINGS(VAR) #VAR,
const char_type* const sMudBrainStateNames[10] = {
    MUD_BRAIN_STATES_ENUM(MAKE_STRINGS)};

#define MAKE_ENUM(VAR) VAR,
enum Mud_Brain_State : u16
{
    MUD_BRAIN_STATES_ENUM(MAKE_ENUM)
};


// Note can't use the macro magic here because there are duplicate names because
// speak generic appears 3 times :(
enum class eMudMotions
{
    None_m1 = -1,
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

enum class MudAction : s16;
enum eLineTypes : s16;

class BirdPortal;

struct MudokonSaveState final : public SaveStateBase
{
    MudokonSaveState()
        : SaveStateBase(ReliveTypes::eMudokon, sizeof(*this))
    { }
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    EReliveLevelIds field_16_lvl_number;
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
    Guid field_40_tlvInfo;
    FP field_44_velx_slow_by;
    Guid field_4C_portal_id;
    s16 field_50_angry_trigger;
    s16 field_52_padding;
    s32 field_54_laugh_and_crouch_timer;
    s32 field_58_angry_timer;
    s16 field_5E_voice_pitch;
    Guid field_60_wheel_id;
    MudSounds field_68;
    s16 field_6A_maxXOffset;
    bool mNotRescued;
    bool mPersistAndResetOffscreen;
    bool mAlerted;
    bool mBlind;
    bool mFollowingAbe;
    bool mStandingForSadOrAngry;
    bool mStoppedAtWheel;
    bool mDoAngry;
    bool mSeenWhileSick;
    bool mWorkAfterTurningWheel;
    bool mReturnToPreviousMotion;
    bool mGetDepressed;
    bool mAlertEnemies;
    bool mNoiseUnknown;
    bool mMakeSadNoise;
    bool mAbeHasRing;
    bool mRingAndAngryMudTimeout;
    bool mIsMudStandingUp2;
    s16 field_70_brain_sub_state2;
    s16 field_72_stand_idle_timer;
    MudAction field_74_delayed_speak;
    Mud_Emotion field_76_emo_tlb;
    GameSpeakEvents field_78_speak_event;
    eMudMotions field_7A_motion;
    Mud_Brain_State field_7C_brain_state;
    s16 field_7E_brain_sub_state;
    s32 field_80_timer;
    s32 field_84_response_entry_idx;
};

class Mudokon final : public BaseAliveGameObject
{
public:
    static constexpr AnimId kMudMotionAnimIds[60] = {
        AnimId::Mudokon_Idle,
        AnimId::Mudokon_Walk,
        AnimId::Mudokon_StandingTurn,
        AnimId::Mudokon_Speak1,
        AnimId::Mudokon_Speak3,
        AnimId::Mudokon_Speak2,
        AnimId::Mudokon_SpeakFart,
        AnimId::Mudokon_WalkBegin,
        AnimId::Mudokon_WalkToIdle,
        AnimId::Mudokon_MidWalkToIdle,
        AnimId::Mudokon_LeverUse,
        AnimId::Mudokon_Chisel,
        AnimId::Mudokon_StartChisel,
        AnimId::Mudokon_StopChisel,
        AnimId::Mudokon_CrouchScrub,
        AnimId::Mudokon_CrouchIdle,
        AnimId::Mudokon_CrouchTurn,
        AnimId::Mudokon_StandToCrouch,
        AnimId::Mudokon_CrouchToStand,
        AnimId::Mudokon_WalkToRun,
        AnimId::Mudokon_MidWalkToRun,
        AnimId::Mudokon_Run,
        AnimId::Mudokon_RunToWalk,
        AnimId::Mudokon_MidRunToWalk,
        AnimId::Mudokon_RunSlideStop,
        AnimId::Mudokon_RunSlideTurn,
        AnimId::Mudokon_RunTurnToRun,
        AnimId::Mudokon_Sneak,
        AnimId::Mudokon_WalkToSneak,
        AnimId::Mudokon_SneakToWalk,
        AnimId::Mudokon_MidWalkToSneak,
        AnimId::Mudokon_MidSneakToWalk,
        AnimId::Mudokon_SneakBegin,
        AnimId::Mudokon_SneakToIdle,
        AnimId::Mudokon_MidSneakToIdle,
        AnimId::Mudokon_RunJumpBegin,
        AnimId::Mudokon_RunJumpMid,
        AnimId::Mudokon_StandToRun,
        AnimId::Mudokon_Punch,
        AnimId::Mudokon_HoistBegin,
        AnimId::Mudokon_HoistIdle,
        AnimId::Mudokon_LandSoft,
        AnimId::Mudokon_LandSoft,
        AnimId::Mudokon_DunnoBegin,
        AnimId::Mudokon_DunnoEnd, // duplicates: 269928
        AnimId::Mudokon_KnockForward,
        AnimId::Mudokon_Knockback,
        AnimId::Mudokon_KnockbackGetUp,
        AnimId::Mudokon_WalkOffEdge,
        AnimId::Mudokon_FallingFromGrab,
        AnimId::Mudokon_Chant,
        AnimId::Mudokon_ChantEnd,
        AnimId::Mudokon_ToDuck,
        AnimId::Mudokon_Duck,
        AnimId::Mudokon_DuckToCrouch,
        AnimId::Mudokon_DuckKnockback,
        AnimId::Mudokon_SlapOwnHead,
        AnimId::Mudokon_TurnWheelBegin,
        AnimId::Mudokon_TurnWheel,
        AnimId::Mudokon_TurnWheelEnd};

    Mudokon(relive::Path_Mudokon* pTlv, const Guid& tlvId);
    ~Mudokon();
    
    void LoadAnimations();

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;

    virtual void VPossessed() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VOnTrapDoorOpen() override;

    // New virtuals
    virtual void VUpdateResBlock();
    virtual s16 VGetMotion(eMotionType motionType) override
    {
        switch (motionType)
        {
            case eMotionType::ePreviousMotion:
                return static_cast<s16>(mPreviousMotion);
            case eMotionType::eCurrentMotion:
                return static_cast<s16>(mCurrentMotion);
            case eMotionType::eNextMotion:
                return static_cast<s16>(mNextMotion);
            default:
                ALIVE_FATAL("Invalid motion type %d", static_cast<s32>(motionType));
        }
    }

    static void CreateFromSaveState(SerializedObjectData& pBuffer);


private:

    void SetPal(Mud_Emotion emotion);
    s16 FacingBirdPortal(BirdPortal* pTarget);
    s16 TurningWheelHelloOrAllYaResponse();

public: // Brains
    s16 Brain_0_GiveRings();
    s16 Brain_1_Chisel();
    s16 Brain_2_CrouchScrub();
    s16 Brain_3_TurnWheel();

    s16 Brain_4_ListeningToAbe();

    s16 Brain_ListeningToAbe_State_0();
    s16 Brain_ListeningToAbe_State_1();
    s16 Brain_ListeningToAbe_State_2();
    s16 Brain_ListeningToAbe_State_3();
    s16 Brain_ListeningToAbe_State_4();
    s16 Brain_ListeningToAbe_State_5();
    s16 Brain_ListeningToAbe_State_6();
    s16 Brain_ListeningToAbe_State_7();
    s16 Brain_ListeningToAbe_State_8();
    s16 Brain_ListeningToAbe_State_9();
    s16 Brain_ListeningToAbe_State_10();
    s16 Brain_ListeningToAbe_State_11();
    s16 Brain_ListeningToAbe_State_12();
    s16 Brain_ListeningToAbe_State_13();
    s16 Brain_ListeningToAbe_State_14();
    s16 Brain_ListeningToAbe_State_15();
    s16 Brain_ListeningToAbe_State_16();
    s16 Brain_ListeningToAbe_State_17();
    s16 Brain_ListeningToAbe_State_18();
    s16 Brain_ListeningToAbe_State_19();
    s16 Brain_ListeningToAbe_State_20();
    s16 Brain_ListeningToAbe_State_21();
    s16 Brain_ListeningToAbe_State_22();

    s16 Brain_5_ShrivelDeath();
    s16 Brain_6_Escape();
    s16 Brain_7_FallAndSmackDeath();
    s16 Brain_8_AngryWorker();
    s16 Brain_9_Sick();

public: // Motions
    void Motion_0_Idle();
    void Motion_1_WalkLoop();
    void Motion_2_StandingTurn();
    void Motion_Speak();
    void Motion_7_WalkBegin();
    void Motion_8_WalkToIdle();
    void Motion_9_MidWalkToIdle();
    void Motion_10_LeverUse();
    void Motion_11_Chisel();
    void Motion_12_StartChisel();
    void Motion_13_StopChisel();
    void Motion_14_CrouchScrub();
    void Motion_15_CrouchIdle();
    void Motion_16_CrouchTurn();
    void Motion_17_StandToCrouch();
    void Motion_18_CrouchToStand();
    void Motion_19_WalkToRun();
    void Motion_20_MidWalkToRun();
    void Motion_21_RunLoop();
    void Motion_22_RunToWalk();
    void Motion_23_MidRunToWalk();
    void Motion_24_RunSlideStop();
    void Motion_25_RunSlideTurn();
    void Motion_26_RunTurnToRun();
    void Motion_27_SneakLoop();
    void Motion_28_MidWalkToSneak();
    void Motion_29_SneakToWalk();
    void Motion_30_WalkToSneak();
    void Motion_31_MidSneakToWalk();
    void Motion_32_SneakBegin();
    void Motion_33_SneakToIdle();
    void Motion_34_MidSneakToIdle();
    void Motion_35_RunJumpBegin();
    void Motion_36_RunJumpMid();
    void Motion_37_StandToRun();
    void Motion_38_Punch();
    void Motion_39_HoistBegin();
    void Motion_40_HoistLand();
    void Motion_41_LandSoft1(); // TODO: better names
    void Motion_42_LandSoft2(); // ^
    void Motion_43_DunnoBegin();
    void Motion_44_DunnoEnd();
    void Motion_45_KnockForward();
    void Motion_46_Knockback();
    void Motion_47_KnockbackGetUp();
    void Motion_48_WalkOffEdge();
    void Motion_49_Fall();
    void Motion_50_Chant();
    void Motion_51_ChantEnd();
    void Motion_52_ToDuck();
    void Motion_53_Duck();
    void Motion_54_DuckToCrouch();
    void Motion_55_DuckKnockback();
    void Motion_56_SlapOwnHead();
    void Motion_57_TurnWheelBegin();
    void Motion_58_TurnWheelLoop();
    void Motion_59_TurnWheelEnd();

private:
    s16 MudResponseDelay();
    s16 FindBirdPortal();
    GameSpeakEvents LastGameSpeak();
    s16 LaughingGasInCurrentScreen();
    void MudEmotionSound(MudSounds idx);
    s16 IAmNearestToAbe();

    u8** GetResBlockForMotion(s16 motion);

    void CheckFloorGone();
    void ToStand();

    s16 FindWheel(FP xpos, FP ypos);
    s16 GetBrainSubStateResponse(MudAction action);
    s16 StopAtWheel();
    void ToKnockback();
    s16 CanAbeSneak();
    void MoveOnLine();

    static const struct MudEmotionTableEntry* ResponseTo_471730(Mud_Emotion emotion, MudAction action);
    void AddAlerted();
    void RemoveAlerted();
    s16 BrainStartWheelTurning();
    bool NeedToTurnAround();
    void ReduceXVelocityBy(FP velXScaleBy);
    void CheckKnockedOntoABomb();
    void HurtSoundPitchedToHealth();
    void TakeASlap(BaseGameObject* pFrom);
    s32 GetResponseEntryIdx();
    static const MudEmotionTableEntry* GetResponseEntry(s32 idx);

private:
    Guid field_118_tlvInfo;
    Guid field_11C_bird_portal_id;
    s16 field_120_angry_switch_id = 0;
    s32 field_124_laugh_and_crouch_timer = 0;
    s32 field_128_angry_timer = 0;
    FP field_134_xVelSlowBy = {};
    s16 field_13C_voice_pitch = 0;
    GameSpeakListener mListener;
    Guid field_158_wheel_id;
    MudSounds field_160_delayed_speak = MudSounds::eEmpty_0;
    s16 field_162_maxXOffset = 0;
    s32 field_164_ring_pulse_interval = 0;
    RingTypes field_168_ring_type = RingTypes::eExplosive_Pulse_0;
    bool mNotRescued = false;
    bool mPersistAndResetOffscreen = false;
    bool mAlerted = false;
    bool mBlind = false;
    bool mFollowingAbe = false;
    bool mStandingForSadOrAngry = false;
    bool mStoppedAtWheel = false;
    bool mDoAngry = false;
    bool mSeenWhileSick = false;
    bool mWorkAfterTurningWheel = false;
    bool mGetDepressed = false;
    bool mAlertEnemies = false;
    bool mNoiseUnknown = false;
    bool mMakeSadNoise = false;
    bool mGiveRingWithoutPassword = false;
    bool mRingAndAngryMudTimeout = false;
    bool mAbeHasRing = false;
    bool mIsMudStandingUp2 = false;
    bool mEscaping = false;
    s16 field_178_brain_sub_state2 = 0;
    s16 field_17A_rescue_switch_id = 0;
    s16 field_17C_stand_idle_timer = 0;
    MudAction field_17E_delayed_speak = MudAction::eHelloOrAllYa_0;
    Mud_Emotion field_180_emo_tbl = Mud_Emotion::eNormal_0;
    GameSpeakEvents field_182_speak_event = GameSpeakEvents::eUnknown_0;
    eMudMotions field_184_next_motion2 = eMudMotions::Motion_0_Idle;
    const struct MudEmotionTableEntry* field_188_pTblEntry = nullptr;


public:
    Mud_Brain_State mBrainState = Mud_Brain_State::Brain_0_GiveRings;
    eMudMotions mPreviousMotion = eMudMotions::Motion_0_Idle;
    eMudMotions mCurrentMotion = eMudMotions::Motion_0_Idle;
    eMudMotions mNextMotion = eMudMotions::Motion_0_Idle;
    bool mbMotionChanged = false;

private:
    s16 mBrainSubState = 0;
    bool mReturnToPreviousMotion = false;
    s32 field_194_timer = 0;
    s32 field_198_turning_wheel_timer = 0;
    bool mChiselMud = false;
};
