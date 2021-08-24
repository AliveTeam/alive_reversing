#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "GameSpeak.hpp"

enum class RingTypes : s16;

enum class Mud_State : s16
{
    eChisle_0 = 0,
    eScrub_1 = 1,
    eAngryWorker_2 = 2,
    eDamageRingGiver_3 = 3,
    eHealthRingGiver_4 = 4,
};

enum class Mud_TLV_Emotion : s16
{
    eNormal_0 = 0,
    eAngry_1 = 1,
    eSad_2 = 2,
    eWired_3 = 3,
    eSick_4 = 4
};

struct Path_Mudokon final : public Path_TLV
{
    Scale_short field_10_scale;
    Mud_State field_12_state;
    XDirection_short field_14_direction;
    s16 field_16_voice_pitch;
    s16 field_18_rescue_id;
    Choice_short field_1A_bDeaf;
    s16 field_1C_disabled_resources;
    Choice_short field_1E_reset_pos_on_screen_change;
    Mud_TLV_Emotion field_20_emotion;
    Choice_short field_22_bBlind;
    s16 field_24_angry_trigger;
    Choice_short field_26_stop_turning_work_wheel;
    Choice_short field_28_bGets_depressed;
    s16 field_2A_ring_timeout;
    Choice_short field_2C_bGive_ring_without_password;
    s16 field_2E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Mudokon, 0x30);

enum class Mud_Emotion : s16
{
    eNormal_0 = 0,
    eAngry_1 = 1,
    eAggressive_2 = 2,
    eSad_3 = 3,
    eSuicidal_4 = 4,
    eHappy_5 = 5, // TODO: Not used ??
    eWired_6 = 6,
    eSick_7 = 7
};

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


#define MUD_BRAIN_STATES_ENUM(ENTRY)        \
    ENTRY(Brain_0_GiveRings_470C10)         \
    ENTRY(Brain_1_Chisel_47C5F0)            \
    ENTRY(Brain_2_Scrub_47D270)             \
    ENTRY(Brain_3_TurnWheel_47E0D0)         \
    ENTRY(Brain_4_ListeningToAbe_477B40)    \
    ENTRY(Brain_5_ShrivelDeath_4714A0)      \
    ENTRY(Brain_6_Escape_47A560)            \
    ENTRY(Brain_7_FallAndSmackDeath_471600) \
    ENTRY(Brain_8_AngryWorker_47E910)       \
    ENTRY(Brain_9_Sick_47A910)

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
enum eMudMotions : u16
{
    M_Idle_0_4724E0,
    M_WalkLoop_1_4728B0,
    M_StandingTurn_2_472BF0,

    // NOTE: 3-6 are all the same entry, added to avoid compile issue mentioned above.
    M_Speak_3_472FA0,
    M_Speak_4_472FA0,
    M_Speak_5_472FA0,
    M_Speak_6_472FA0,

    M_WalkBegin_7_472AB0,
    M_WalkToIdle_8_472B30,
    M_MidWalkToIdle_9_472BD0,
    M_LeverUse_10_473020,
    M_Chisel_11_4732D0,
    M_StartChisel_12_473500,
    M_StopChisel_13_473530,
    M_CrouchScrub_14_473560,
    M_CrouchIdle_15_474040,
    M_CrouchTurn_16_4740E0,
    M_StandToCrouch_17_474120,
    M_CrouchToStand_18_474150,
    M_WalkToRun_19_473600,
    M_MidWalkToRun_20_4736D0,
    M_RunLoop_21_473720,
    M_RunToWalk_22_4738E0,
    M_MidRunToWalk_23_4739B0,
    M_RunSlideStop_24_473A00,
    M_RunSlideTurn_25_473AA0,
    M_RunTurnToRun_26_473BB0,
    M_SneakLoop_27_473C40,
    M_MidWalkToSneak_28_473D60,
    M_SneakToWalk_29_473E20,
    M_WalkToSneak_30_473EE0,
    M_MidSneakToWalk_31_473F30,
    M_SneakBegin_32_473F80,
    M_SneakToIdle_33_473FF0,
    M_MidSneakToIdle_34_474020,
    M_RunJumpBegin_35_474460,
    M_RunJumpMid_36_474570,
    M_StandToRun_37_4749A0,
    M_Punch_38_474AA0,
    M_HoistBegin_39_4748E0,
    M_HoistLand_40_474780,
    M_LandSoft1_41_474960,
    M_LandSoft2_42_4743F0,
    M_DunnoBegin_43_472790,
    M_DunnoEnd_44_4727B0,
    M_KnockForward_45_474180,
    M_Knockback_46_4742A0,
    M_KnockbackGetUp_47_474380,
    M_WalkOffEdge_48_4743C0,
    M_Fall_49_472C60,
    M_Chant_50_473040,
    M_ChantEnd_51_4730D0,
    M_ToDuck_52_474A20,
    M_Duck_53_474A40,
    M_DuckToCrouch_54_474A80,
    M_DuckKnockback_55_474250,
    M_SlapOwnHead_56_4727D0,
    M_TurnWheelBegin_57_474C00,
    M_TurnWheelLoop_58_474CC0,
    M_TurnWheelEnd_59_474D30
};

enum class MudAction : s16;
enum class MudSounds : s16;

class BirdPortal;

struct Mudokon_State final
{
    AETypes field_0_type;
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
    s16 field_24_current_motion;
    s16 field_26_anim_current_frame;
    s16 field_28_anim_frame_change_counter;
    s8 field_2A_bAnimRender;
    s8 field_2B_bDrawable;
    FP field_2C_health;
    s16 field_30_current_motion;
    s16 field_32_next_motion;
    s16 field_34_lastLineYPos;
    s16 field_36_line_type;
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
    s32 field_54_savedfield124; //TODO: Find out what field_124 is. It appears to be some sort of timer. -- Nemin (5/7/2020)
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
        eBit5_save_state = 0x10,
        eBit6_alerted = 0x20,
        eBit7_blind = 0x40,
        eBit8_following = 0x80,
        eBit9_standing_for_sad_or_angry = 0x100,
        eBit10_stopped_at_wheel = 0x200,
        eBit11_do_angry = 0x400,
        eBit12_seen_while_sick = 0x800,
        eBit13_stop_trigger = 0x1000,
        eBit14_unused = 0x2000,
        eBit15_return_to_previous_motion = 0x4000,
        eBit16_get_depressed = 0x8000
    };
    BitField16<Flags_6A> field_6C;

    enum Flags_6E
    {
        e6E_Bit1_alert_enemies = 0x1,
        e6E_Bit2 = 0x2,
        e6E_Bit3_make_sad_noise = 0x4,
        e6E_Bit4_ring_timeout = 0x8,
        e6E_Bit5 = 0x10,
        e6E_Bit6 = 0x20
    };
    BitField16<Flags_6E> field_6E;

    s16 field_70_brain_sub_state2;
    s16 field_72_stand_idle_timer;
    MudAction field_74_delayed_speak;
    Mud_Emotion field_76_emo_tlb;
    GameSpeakEvents field_78;
    eMudMotions field_7A_motion;
    Mud_Brain_State field_7C_brain_state;
    s16 field_7E_brain_sub_state;
    s32 field_80_timer;
    s32 field_84_response_entry_idx;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Mudokon_State, 0x88);

class Mudokon final : public BaseAliveGameObject
{
public:
    EXPORT Mudokon* ctor_474F30(Path_Mudokon* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override
    {
        return vGetSaveState_47B080(reinterpret_cast<Mudokon_State*>(pSaveBuffer));
    }

    virtual void VPossessed_408F70() override
    {
        vPossessed_4774F0();
    }

    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override
    {
        return vTakeDamage_476270(pFrom);
    }

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override
    {
        vOnTlvCollision_476EA0(pTlv);
    }

    virtual void VOnTrapDoorOpen() override
    {
        vOnTrapDoorOpen_472350();
    }

    // New virtuals
    virtual void VUpdateAnimRes_474D80()
    {
        vUpdateAnimRes_474D80();
    }

    EXPORT static s32 CC CreateFromSaveState_4717C0(const u8* pBuffer);


private:
    EXPORT s32 vGetSaveState_47B080(Mudokon_State* pState);


    EXPORT void vUpdate_4757A0();

    EXPORT void SetPal_4772D0(Mud_Emotion emotion);

    EXPORT void vOnTrapDoorOpen_472350();

    EXPORT void vOnTlvCollision_476EA0(Path_TLV* pTlv);

    EXPORT s16 FacingTarget_473140(BirdPortal* pTarget);

    EXPORT Mudokon* vdtor_475770(s32 flags);

    EXPORT void dtor_475B60();

    EXPORT void vScreenChanged_476F30();

    EXPORT void vPossessed_4774F0();

    EXPORT s16 vTakeDamage_476270(BaseGameObject* pFrom);

    s16 TurningWheelHelloOrAllYaResponse();

public:
    EXPORT void vUpdateAnimRes_474D80();

public: // Brains
    EXPORT s16 Brain_0_GiveRings_470C10();
    EXPORT s16 Brain_1_Chisel_47C5F0();
    EXPORT s16 Brain_2_Scrub_47D270();
    EXPORT s16 Brain_3_TurnWheel_47E0D0();

    EXPORT s16 Brain_4_ListeningToAbe_477B40();

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

    EXPORT s16 Brain_5_ShrivelDeath_4714A0();
    EXPORT s16 Brain_6_Escape_47A560();
    EXPORT s16 Brain_7_FallAndSmackDeath_471600();
    EXPORT s16 Brain_8_AngryWorker_47E910();
    EXPORT s16 Brain_9_Sick_47A910();

public: // Motions
    EXPORT void M_Idle_0_4724E0();
    EXPORT void M_WalkLoop_1_4728B0();
    EXPORT void M_StandingTurn_2_472BF0();
    EXPORT void M_Speak_472FA0();
    EXPORT void M_WalkBegin_7_472AB0();
    EXPORT void M_WalkToIdle_8_472B30();
    EXPORT void M_MidWalkToIdle_9_472BD0();
    EXPORT void M_LeverUse_10_473020();
    EXPORT void M_Chisel_11_4732D0();
    EXPORT void M_StartChisel_12_473500();
    EXPORT void M_StopChisel_13_473530();
    EXPORT void M_CrouchScrub_14_473560();
    EXPORT void M_CrouchIdle_15_474040();
    EXPORT void M_CrouchTurn_16_4740E0();
    EXPORT void M_StandToCrouch_17_474120();
    EXPORT void M_CrouchToStand_18_474150();
    EXPORT void M_WalkToRun_19_473600();
    EXPORT void M_MidWalkToRun_20_4736D0();
    EXPORT void M_RunLoop_21_473720();
    EXPORT void M_RunToWalk_22_4738E0();
    EXPORT void M_MidRunToWalk_23_4739B0();
    EXPORT void M_RunSlideStop_24_473A00();
    EXPORT void M_RunSlideTurn_25_473AA0();
    EXPORT void M_RunTurnToRun_26_473BB0();
    EXPORT void M_SneakLoop_27_473C40();
    EXPORT void M_MidWalkToSneak_28_473D60();
    EXPORT void M_SneakToWalk_29_473E20();
    EXPORT void M_WalkToSneak_30_473EE0();
    EXPORT void M_MidSneakToWalk_31_473F30();
    EXPORT void M_SneakBegin_32_473F80();
    EXPORT void M_SneakToIdle_33_473FF0();
    EXPORT void M_MidSneakToIdle_34_474020();
    EXPORT void M_RunJumpBegin_35_474460();
    EXPORT void M_RunJumpMid_36_474570();
    EXPORT void M_StandToRun_37_4749A0();
    EXPORT void M_Punch_38_474AA0();
    EXPORT void M_HoistBegin_39_4748E0();
    EXPORT void M_HoistLand_40_474780();
    EXPORT void M_LandSoft1_41_474960();
    EXPORT void M_LandSoft2_42_4743F0();
    EXPORT void M_DunnoBegin_43_472790();
    EXPORT void M_DunnoEnd_44_4727B0();
    EXPORT void M_KnockForward_45_474180();
    EXPORT void M_Knockback_46_4742A0();
    EXPORT void M_KnockbackGetUp_47_474380();
    EXPORT void M_WalkOffEdge_48_4743C0();
    EXPORT void M_Fall_49_472C60();
    EXPORT void M_Chant_50_473040();
    EXPORT void M_ChantEnd_51_4730D0();
    EXPORT void M_ToDuck_52_474A20();
    EXPORT void M_Duck_53_474A40();
    EXPORT void M_DuckToCrouch_54_474A80();
    EXPORT void M_DuckKnockback_55_474250();
    EXPORT void M_SlapOwnHead_56_4727D0();
    EXPORT void M_TurnWheelBegin_57_474C00();
    EXPORT void M_TurnWheelLoop_58_474CC0();
    EXPORT void M_TurnWheelEnd_59_474D30();

private:
    EXPORT s16 StableDelay_477570();
    EXPORT s16 CheckForPortal_4775E0();
    EXPORT GameSpeakEvents LastGameSpeak_476FF0();
    EXPORT s16 LaughingGasInCurrentScreen_4774A0();
    EXPORT void Sound_475EC0(MudSounds idx);
    EXPORT s16 CanRespond_4770B0();

    EXPORT u8** AnimBlockForMotion_474DC0(s16 motion);

    EXPORT void CheckFloorGone_472320();
    EXPORT void ToStand_4724A0();

    EXPORT s16 FindWheel_4777B0(FP xpos, FP ypos);
    EXPORT s16 GetBrainSubStateResponse_477AF0(MudAction action);
    EXPORT s16 StopAtWheel_477880();
    EXPORT void ToKnockback_473190();
    EXPORT s16 IsMotionUnknown_4730F0();
    EXPORT void MoveOnLine_4720D0();

    EXPORT static const struct MudEmotionTableEntry* CC ResponseTo_471730(Mud_Emotion emotion, MudAction action);

    void AddAlerted();

    void RemoveAlerted();
    s16 BrainStartWheelTurning();
    bool NeedToTurnAround();

    EXPORT void ReduceXVelocityBy_472260(FP velXScaleBy);

    EXPORT void CheckKnockedOntoABomb_4723C0();

    EXPORT void HurtSound_475DB0();

    EXPORT void TakeASlap_476090(BaseGameObject* pFrom);

    EXPORT s32 GetResponseEntryIdx_471760();

    EXPORT static const MudEmotionTableEntry* CCSTD GetResponseEntry_471790(s32 idx);

private:
    s32 field_118_tlvInfo;
    s32 field_11C_bird_portal_id;
    s16 field_120_angry_trigger;
    s16 field_122_padding;
    s32 field_124;
    s32 field_128_angry_timer;
    s32 field_12C_unused;
    s16 field_130_unused;
    s16 field_132_padding;
    FP field_134_xVelSlowBy;
    s32 field_138_unused;
    s16 field_13C_voice_pitch;
    s16 field_13E_padding;
    s32 field_140_last_event_index;
    s32 field_144_padding;
    s16 field_148_padding;
    s16 field_14A_padding;
    s16 field_14C_padding;
    s16 field_14E_padding;
    s16 field_150_padding;
    s16 field_152_padding;
    s16 field_154_unused;
    s16 field_156_unused;
    s32 field_158_wheel_id;
    s32 field_15C_unused;
    MudSounds field_160_delayed_speak;
    s16 field_162_maxXOffset;
    s32 field_164_ring_timeout;
    RingTypes field_168_ring_type;

    enum Flags_16A
    {
        eBit1_not_rescued = 0x1,
        eBit2_reset_pos_on_screen_change = 0x2,
        eBit3_alerted = 0x4,
        eBit4_blind = 0x8,
        eBit5_following = 0x10,
        eBit6_standing_for_sad_or_angry = 0x20,
        eBit7_stopped_at_wheel = 0x40,
        eBit8_do_angry = 0x80,
        eBit9_seen_while_sick = 0x100,
        eBit10_stop_turning_work_wheel = 0x200,
        eBit11_get_depressed = 0x400,
        eBit12_alert_enemies = 0x800,
        eBit13 = 0x1000, // TODO: It's only unset until it's checked, then set. Relates to MudAction::eUnknown_15 and MudAction::eUnknown_16. It's only set when a Glukkon makes a loud noise on the same screen as the Mudokon.
        eBit14_make_sad_noise = 0x2000,
        eBit15_ring_timeout = 0x4000,
        eBit16_give_ring_without_password = 0x8000,
        eBit17_padding = 0x10000,
        eBit18_padding = 0x20000,
        eBit19_padding = 0x40000,
        eBit20_padding = 0x80000,
        eBit21_padding = 0x100000,
        eBit22_padding = 0x200000,
        eBit23_padding = 0x400000,
        eBit24_padding = 0x800000,
        eBit25_padding = 0x1000000,
    };
    BitField16<Flags_16A> field_16A_flags;

    enum Flags_16C
    {
        eBit1_Unknown = 0x1,
        eBit2_Unknown = 0x2,
        eBit3_Unknown = 0x4,
    };
    BitField16<Flags_16C> field_16C_flags;
    s16 field_16E_padding;
    s16 field_170_padding;
    s16 field_172_padding;
    s16 field_174_padding;
    s16 field_176_padding;
    s16 field_178_brain_sub_state2;
    s16 field_17A_rescue_id;
    s16 field_17C_stand_idle_timer;
    MudAction field_17E_delayed_speak;
    Mud_Emotion field_180_emo_tbl;
    GameSpeakEvents field_182;
    eMudMotions field_184_next_motion2;
    s16 field_186_padding;
    const struct MudEmotionTableEntry* field_188_pTblEntry;
    s16 field_18C_unused;

public:
    Mud_Brain_State field_18E_brain_state;

private:
    s16 field_190_brain_sub_state;
    s16 field_192_return_to_previous_motion;
    s32 field_194_timer;
    s32 field_198_turning_wheel_timer;
};
ALIVE_ASSERT_SIZEOF(Mudokon, 0x19C);
