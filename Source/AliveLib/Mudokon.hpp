#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "GameSpeak.hpp"

enum class RingTypes : __int16;

enum class TLV_Scale : __int16
{
    Full_0 = 0,
    Half_1 = 1,
};

enum class Mud_State : __int16
{
    eChisle_0 = 0,
    eScrub_1 = 1,
    eAngryWorker_2 = 2,
    eDamageRingGiver_3 = 3,
    eHealthRingGiver_4 = 4,
};

enum class Mud_Direction : __int16
{
    eLeft_0 = 0,
    eRight_1 = 1,
};

enum class Mud_TLV_Emotion : __int16
{
    eNormal_0 = 0,
    eAngry_1 = 1,
    eSad_2 = 2,
    eWired_3 = 3,
    eSick_4 = 4
};

struct Path_Mudokon : public Path_TLV
{
    TLV_Scale field_10_scale;
    Mud_State field_12_state;
    Mud_Direction field_14_direction;
    __int16 field_16_voice_pitch;
    __int16 field_18_rescue_id;
    __int16 field_1A_bDeaf;
    __int16 field_1C_disabled_resources;
    __int16 field_1E_save_state;
    Mud_TLV_Emotion field_20_emotion;
    __int16 field_22_bBlind;
    __int16 field_24_angry_trigger;
    __int16 field_26_stop_trigger;
    __int16 field_28_bGets_depressed;
    __int16 field_2A_ring_timeout;
    __int16 field_2C_bInstant_power_up;
    __int16 field_2E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Mudokon, 0x30);

enum class Mud_Emotion : __int16
{
    eNormal_0 = 0,
    eAngry_1 = 1,
    eUnknown_2 = 2,
    eSad_3 = 3,
    eUnknown_4 = 4,
    eHappy_5 = 5, // TODO: Not used ??
    eWired_6 = 6,
    eSick_7 = 7
};

enum class MudAction : __int16
{
    eHelloOrAllYa_0 = 0,
    eFollowMe_1 = 1,
    eWait_2 = 2,
    eUnknown_3 = 3,
    eUnknown_4 = 4,
    eUnknown_5 = 5,
    eFart_6 = 6,
    eSlapOrWater_7 = 7,
    eSorry_8 = 8,
    eMudAbuse_9 = 9,
    eComfort_10 = 10,
    eUnknown_11 = 11,
    eLaugh_12 = 12,
    eDuck_13 = 13,
    eMudDied_14 = 14,
    eUnknown_15 = 15,
    eUnknown_16 = 16,
    eUnknown_17 = 17,
};


#define MUD_AI_STATES_ENUM(ENTRY) \
    ENTRY(AI_Give_rings_0_470C10) \
    ENTRY(AI_Chisel_1_47C5F0) \
    ENTRY(AI_Scrub_2_47D270) \
    ENTRY(AI_State_3_47E0D0) \
    ENTRY(AI_Wired_4_477B40) \
    ENTRY(AI_ShrivelDeath_5_4714A0) \
    ENTRY(AI_Escape_6_47A560) \
    ENTRY(AI_FallAndSmackDeath_7_471600) \
    ENTRY(AI_AngryWorker_8_47E910) \
    ENTRY(AI_Sick_9_47A910)

#define MAKE_STRINGS(VAR) #VAR,
const char* const sMudAiStateNames[10] =
{
    MUD_AI_STATES_ENUM(MAKE_STRINGS)
};

#define MAKE_ENUM(VAR) VAR,
enum Mud_AI_State : unsigned __int16
{
    MUD_AI_STATES_ENUM(MAKE_ENUM)
};


// Note can't use the macro magic here because there are duplicate names because
// speak generic appears 3 times :(
enum Mud_Motion : unsigned __int16
{
    StandIdle_0_4724E0,
    Walking_1_4728B0,
    TurnAroundStanding_2_472BF0,

    // NOTE: 3-6 are all the same entry, added to avoid compile issue mentioned above.
    Speak_Generic_3_472FA0,
    Speak_Generic_4_472FA0,
    Speak_Generic_5_472FA0,
    Speak_Generic_6_472FA0,

    StandToWalk_7_472AB0,
    WalkingToStand_8_472B30,
    jWalkingToStand_8_472BD0,
    PullLever_10_473020,
    Chisel_11_4732D0,
    StartChisel_12_473500,
    StopChisel_13_473530,
    CrouchScrub_14_473560,
    CrouchIdle_15_474040,
    CrouchTurn_16_4740E0,
    StandToCrouch_17_474120,
    CrouchToStand_18_474150,
    StandingToRun1_19_473600,
    StandingToRun2_20_4736D0,
    Running_21_473720,
    RunToWalk1_22_4738E0,
    RunToWalk2_23_4739B0,
    RunSlideStop_24_473A00,
    RunSlideTurn_25_473AA0,
    RunTurn_26_473BB0,
    Sneaking_27_473C40,
    WalkToSneak1_28_473D60,
    SneakingToWalk_29_473E20,
    WalkToSneak2_30_473EE0,
    SneakingToWalk2_31_473F30,
    StandToSneaking_32_473F80,
    SneakingToStand1_33_473FF0,
    SneakingToStand2_34_474020,
    JumpStart_35_474460,
    JumpMid_36_474570,
    WalkToRun_37_4749A0,
    Slap_38_474AA0,
    StartHoistJumpUp_39_4748E0,
    HoistFallToFloor_40_474780,
    HitFloorStanding1_41_474960,
    HitFloorStanding2_42_4743F0,
    StandToDunno_43_472790,
    DunnoToStand_44_4727B0,
    KnockForward_45_474180,
    StandToKnockBack_46_4742A0,
    StandUpFromKnockedBack_47_474380,
    FallLedgeBegin_48_4743C0,
    Fall_49_472C60,
    Chanting_50_473040,
    ChantToStand_51_4730D0,
    ToDuck_52_474A20,
    Duck_53_474A40,
    DuckToCrouch_54_474A80,
    DuckKnockBack_55_474250,
    SlapOwnHead_56_4727D0,
    TurnWheelBegin_57_474C00,
    TurnWheelLoop_58_474CC0,
    TurnWheelEnd_59_474D30
};

enum class MudAction : __int16;
enum class MudSounds : __int16;

class BirdPortal;

class Mudokon : public BaseAliveGameObject
{
public:
    EXPORT Mudokon* ctor_474F30(Path_Mudokon* pTlv, int tlvInfo);

    // TODO: Wire through other virtuals/vtable
    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual int VGetSaveState(BYTE* /*pSaveBuffer*/)
    {
        // TODO
        return 0;
    }

    virtual void VPossessed_408F70() override
    {
        vPossessed_4774F0();
    }
 
    virtual __int16 VTakeDamage_408730(BaseGameObject* /*pFrom*/) override
    {
        // TODO
        return 0;
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

private:
    EXPORT void vUpdate_4757A0();

    EXPORT void SetPal_4772D0(Mud_Emotion emotion);

    EXPORT void vOnTrapDoorOpen_472350();

    EXPORT void vOnTlvCollision_476EA0(Path_TLV* pTlv);

    EXPORT short FacingTarget_473140(BirdPortal* pTarget);

    EXPORT Mudokon* vdtor_475770(signed int flags);

    EXPORT void dtor_475B60();

    EXPORT void vScreenChanged_476F30();

    EXPORT void vPossessed_4774F0();

public:
    EXPORT void vUpdateAnimRes_474D80();
public: // AI states
    EXPORT __int16 AI_Give_rings_0_470C10();
    EXPORT __int16 AI_Chisel_1_47C5F0();
    EXPORT __int16 AI_Scrub_2_47D270();
    EXPORT __int16 AI_State_3_47E0D0();

    EXPORT __int16 AI_Wired_4_Real_477B40();
    EXPORT __int16 AI_Wired_4_477B40();

    __int16 BrainState4_State_0();
    __int16 BrainState4_State_1();
    __int16 BrainState4_State_2();
    __int16 BrainState4_State_3();
    __int16 BrainState4_State_4_FollowingAbe();
    __int16 BrainState4_State_5();
    __int16 BrainState4_State_6_RunAfterAbe();
    __int16 BrainState4_State_7_StandingForAbeCommand();
    __int16 BrainState4_State_8();
    __int16 BrainState4_State_9();
    __int16 BrainState4_State_10();
    __int16 BrainState4_State_11();
    __int16 BrainState4_State_12();
    __int16 BrainState4_State_13();
    __int16 BrainState4_State_14();
    __int16 BrainState4_State_15();
    __int16 BrainState4_State_16();
    __int16 BrainState4_State_17();
    __int16 BrainState4_State_18();
    __int16 BrainState4_State_19();
    __int16 BrainState4_State_20();
    __int16 BrainState4_State_21();
    __int16 BrainState4_State_22();

    EXPORT __int16 AI_ShrivelDeath_5_4714A0();
    EXPORT __int16 AI_Escape_6_47A560();
    EXPORT __int16 AI_FallAndSmackDeath_7_471600();
    EXPORT __int16 AI_AngryWorker_8_47E910();
    EXPORT __int16 AI_Sick_9_47A910();

public: // Motion states
   EXPORT void StandIdle_0_4724E0();
   EXPORT void Walking_1_4728B0();
   EXPORT void TurnAroundStanding_2_472BF0();
   EXPORT void Speak_Generic_472FA0();
   EXPORT void StandToWalk_7_472AB0();
   EXPORT void WalkingToStand_8_472B30();
   EXPORT void jWalkingToStand_8_472BD0();
   EXPORT void PullLever_10_473020();
   EXPORT void Chisel_11_4732D0();
   EXPORT void StartChisel_12_473500();
   EXPORT void StopChisel_13_473530();
   EXPORT void CrouchScrub_14_473560();
   EXPORT void CrouchIdle_15_474040();
   EXPORT void CrouchTurn_16_4740E0();
   EXPORT void StandToCrouch_17_474120();
   EXPORT void CrouchToStand_18_474150();
   EXPORT void StandingToRun1_19_473600();
   EXPORT void StandingToRun2_20_4736D0();
   EXPORT void Running_21_473720();
   EXPORT void RunToWalk1_22_4738E0();
   EXPORT void RunToWalk2_23_4739B0();
   EXPORT void RunSlideStop_24_473A00();
   EXPORT void RunSlideTurn_25_473AA0();
   EXPORT void RunTurn_26_473BB0();
   EXPORT void Sneaking_27_473C40();
   EXPORT void WalkToSneak1_28_473D60();
   EXPORT void SneakingToWalk_29_473E20();
   EXPORT void WalkToSneak2_30_473EE0();
   EXPORT void SneakingToWalk2_31_473F30();
   EXPORT void StandToSneaking_32_473F80();
   EXPORT void SneakingToStand1_33_473FF0();
   EXPORT void SneakingToStand2_34_474020();
   EXPORT void JumpStart_35_474460();
   EXPORT void JumpMid_36_474570();
   EXPORT void WalkToRun_37_4749A0();
   EXPORT void Slap_38_474AA0();
   EXPORT void StartHoistJumpUp_39_4748E0();
   EXPORT void HoistFallToFloor_40_474780();
   EXPORT void HitFloorStanding1_41_474960();
   EXPORT void HitFloorStanding2_42_4743F0();
   EXPORT void StandToDunno_43_472790();
   EXPORT void DunnoToStand_44_4727B0();
   EXPORT void KnockForward_45_474180();
   EXPORT void KnockedBackToStandUp_46_4742A0();
   EXPORT void StandUpFromKnockedBack_47_474380();
   EXPORT void FallLedgeBegin_48_4743C0();
   EXPORT void Fall_49_472C60();
   EXPORT void Chanting_50_473040();
   EXPORT void ChantToStand_51_4730D0();
   EXPORT void ToDuck_52_474A20();
   EXPORT void Duck_53_474A40();
   EXPORT void DuckToCrouch_54_474A80();
   EXPORT void DuckKnockBack_55_474250();
   EXPORT void SlapOwnHead_56_4727D0();
   EXPORT void TurnWheelBegin_57_474C00();
   EXPORT void TurnWheelLoop_58_474CC0();
   EXPORT void TurnWheelEnd_59_474D30();

private:
    EXPORT __int16 StableDelay_477570();
    EXPORT __int16 CheckForPortal_4775E0();
    EXPORT GameSpeakEvents LastGameSpeak_476FF0();
    EXPORT __int16 LaughingGasInCurrentScreen_4774A0();
    EXPORT void Sound_475EC0(MudSounds idx);
    EXPORT __int16 CanRespond_4770B0();

    EXPORT BYTE** AnimBlockForMotion_474DC0(short motion);

    EXPORT void ToFalling_472320();
    EXPORT void ToStand_4724A0();

    EXPORT __int16 FindWheel_4777B0(FP xpos, FP ypos);
    EXPORT __int16 sub_477AF0(MudAction action);
    EXPORT __int16 sub_477880();
    EXPORT void StandingKnockBack_473190();
    EXPORT __int16 IsMotionUnknown_4730F0();
    EXPORT void MoveOnLine_4720D0();

    EXPORT static const struct MudEmotionTableEntry* CC ResponseTo_471730(Mud_Emotion emotion, MudAction action);
    
    void AddAlerted();

    void RemoveAlerted();
    __int16 AIStartWheelTurning();
    bool NeedToTurnAround();

    EXPORT void ReduceXVelocityBy_472260(FP velXScaleBy);

    EXPORT void CheckKnockedOntoABomb_4723C0();

    EXPORT void HurtSound_475DB0();

    EXPORT void TakeASlap_476090(Abe *pFrom);

private:
    __int16 field_116;
    int field_118;
    int field_11C_bird_portal_id;
    __int16 field_120_angry_trigger;
    __int16 field_122;
    int field_124;
    int field_128_angry_timer;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
    FP field_134_xVelSlowBy;
    int field_138;
    __int16 field_13C_voice_pitch;
    __int16 field_13E;
    int field_140;
    int field_144;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    __int16 field_14E;
    __int16 field_150;
    __int16 field_152;
    __int16 field_154;
    __int16 field_156;
    int field_158_wheel_id;
    __int16 field_15C;
    __int16 field_15E;
    __int16 field_160_delayed_speak;
    __int16 field_162_maxXOffset;
    int field_164_ring_timeout;
    RingTypes field_168_ring_type;

    enum Flags
    {
        eBit1 = 0x1,
        eBit2_save_state = 0x2,
        eBit3_Alerted = 0x4,
        eBit4_blind = 0x8,
        eBit5 = 0x10,
        eBit6_StandingForSadOrAngry = 0x20,
        eBit7 = 0x40,
        eBit8_DoAngry = 0x80,
        eBit9 = 0x100,
        eBit10_stop_trigger = 0x200,
        eBit11_get_depressed = 0x400,
        eBit12 = 0x800,
        eBit13 = 0x1000,
        eBit14 = 0x2000,
        eBit15_ring_timeout = 0x4000,
        eBit16_instant_power_up = 0x8000,
        eBit17 = 0x10000,
        eBit18 = 0x20000,
        eBit19 = 0x40000,
        eBit20 = 0x80000,
        eBit21 = 0x100000,
        eBit22 = 0x200000,
        eBit23 = 0x400000,
        eBit24 = 0x800000,
        eBit25 = 0x1000000,
    };
    BitField16<Flags> field_16A_flags;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    __int16 field_174;
    __int16 field_176;
    __int16 field_178_sub_state2;
    __int16 field_17A_rescue_id;
    __int16 field_17C;
    MudAction field_17E_delayed_speak;
    Mud_Emotion field_180_emo_tbl;
    __int16 field_182;
    Mud_Motion field_184_next_motion2;
    __int16 field_186;
    const struct MudEmotionTableEntry* field_188_pTblEntry;
    __int16 field_18C;
    Mud_AI_State field_18E_ai_state;
    __int16 field_190_sub_state;
    __int16 field_192;
    int field_194_timer;
    int field_198;
};
ALIVE_ASSERT_SIZEOF(Mudokon, 0x19C);
