#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Abe.hpp"

void Scrab_ForceLink();

START_NS_AO

#define SCRAB_STATES_ENUM(ENTRY) \
    ENTRY(State_0_Empty_45E3D0) \
    ENTRY(State_1_Stand_45E620) \
    ENTRY(State_2_Walk_45E730) \
    ENTRY(State_3_Run_45EAB0) \
    ENTRY(State_4_Turn_45EF30) \
    ENTRY(State_5_RunToStand_45ED90) \
    ENTRY(State_6_HopBegin_45F3C0) \
    ENTRY(State_7_HopMidair_45F1A0) \
    ENTRY(State_8_Land_45F500) \
    ENTRY(State_9_JumpToFall_45EFD0) \
    ENTRY(State_10_StandToWalk_45E670) \
    ENTRY(State_11_StandToRun_45E9F0) \
    ENTRY(State_12_WalkToStand_45E930) \
    ENTRY(State_13_RunJumpBegin_45F5D0) \
    ENTRY(State_14_RunJumpEnd_45F850) \
    ENTRY(State_15_ToFall_45F180) \
    ENTRY(State_16_Stamp_45F920) \
    ENTRY(State_17_Empty_45F9C0) \
    ENTRY(State_18_GetEaten_45FF70) \
    ENTRY(State_19_Unused_45F9D0) \
    ENTRY(State_20_HowlBegin_45FA60) \
    ENTRY(State_21_HowlEnd_45FAF0) \
    ENTRY(State_22_Shriek_45FB00) \
    ENTRY(State_23_ScrabBattleAnim_45FBA0) \
    ENTRY(State_24_FeedToGulp_45FC30) \
    ENTRY(State_25_ToFeed_45FCE0) \
    ENTRY(State_26_Feed_45FDA0) \
    ENTRY(State_27_AttackLunge_45FDF0) \
    ENTRY(State_28_LegKick_45FF60) \
    ENTRY(State_29_DeathBegin_45FFA0)

#define MAKE_ENUM(VAR) VAR,
enum eScrabStates : int
{
    SCRAB_STATES_ENUM(MAKE_ENUM)
};

struct Path_Scrab : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_attack_delay;
    __int16 field_1C_patrol_type;
    __int16 field_1E_left_min_delay;
    __int16 field_20_left_max_delay;
    __int16 field_22_right_min_delay;
    __int16 field_24_right_max_delay;
    __int16 field_26_attack_duration;
    __int16 field_28_disable_resources;
    __int16 field_2A_roar_randomly;
};
ALIVE_ASSERT_SIZEOF(Path_Scrab, 0x2C);

enum class ScrabSounds : unsigned __int8
{
    eHowl_0 = 0,
    eDeathHowl_1 = 1,
    eEmpty_2 = 2,
    eEmpty_3 = 3,
    eHitCollision_4 = 4,
    eShredding_5 = 5,
    eWalk1_6 = 6,
    eWalk2_7 = 7, // unused in AO?
    eYell_8 = 8,
};

class Scrab : public BaseAliveGameObject
{
public:
    EXPORT Scrab* ctor_45B5F0(Path_Scrab* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_45BA50();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VRender(int** pOrderingTable) override;

    EXPORT void VRender_45BBF0(int** ppOt);

    EXPORT Scrab* Vdtor_45C310(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_45B360();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT __int16 VTakeDamage_45BC10(BaseGameObject* pFrom);

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    EXPORT void VOn_TLV_Collision_45BDC0(Path_TLV* pTlv);

    virtual void VScreenChanged() override;

    EXPORT __int16 CanSeeAbe_45C100(BaseAliveGameObject* pObj);

    EXPORT void VScreenChanged_45C290();

    EXPORT void ToStand_45E310();

    EXPORT void vUpdateAnim_45B330();

    EXPORT BYTE** ResBlockForMotion_45BB30(__int16 motion);

    EXPORT void PlatformCollide_45E580();

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_45E5E0();

    EXPORT __int16 ToNextMotion_45DFB0();

    void ToStand();

    EXPORT int Scrab_SFX_460B80(ScrabSounds soundId, int vol, int pitch, __int16 applyDirection);

    EXPORT void ToJump_45E340();

    EXPORT void MoveOnLine_45E450();

    virtual __int16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;

    EXPORT __int16 VOnSameYLevel_45C180(BaseAnimatedWithPhysicsGameObject* pObj);

    EXPORT Scrab* FindScrabToFight_45BE30();

    EXPORT __int16 FindAbeOrMud_45BEF0();
    
    __int16 HandleRunning();

    __int16 GetMotionForPatrolType(__int16 patrolType);

    // States
    EXPORT void State_0_Empty_45E3D0();
    EXPORT void State_1_Stand_45E620();
    EXPORT void State_2_Walk_45E730();
    EXPORT void State_3_Run_45EAB0();
    EXPORT void State_4_Turn_45EF30();
    EXPORT void State_5_RunToStand_45ED90();
    EXPORT void State_6_HopBegin_45F3C0();
    EXPORT void State_7_HopMidair_45F1A0();
    EXPORT void State_8_Land_45F500();
    EXPORT void State_9_JumpToFall_45EFD0();
    EXPORT void State_10_StandToWalk_45E670();
    EXPORT void State_11_StandToRun_45E9F0();
    EXPORT void State_12_WalkToStand_45E930();
    EXPORT void State_13_RunJumpBegin_45F5D0();
    EXPORT void State_14_RunJumpEnd_45F850();
    EXPORT void State_15_ToFall_45F180();
    EXPORT void State_16_Stamp_45F920();
    EXPORT void State_17_Empty_45F9C0();
    EXPORT void State_18_GetEaten_45FF70();
    EXPORT void State_19_Unused_45F9D0();
    EXPORT void State_20_HowlBegin_45FA60();
    EXPORT void State_21_HowlEnd_45FAF0();
    EXPORT void State_22_Shriek_45FB00();
    EXPORT void State_23_ScrabBattleAnim_45FBA0();
    EXPORT void State_24_FeedToGulp_45FC30();
    EXPORT void State_25_ToFeed_45FCE0();
    EXPORT void State_26_Feed_45FDA0();
    EXPORT void State_27_AttackLunge_45FDF0();
    EXPORT void State_28_LegKick_45FF60();
    EXPORT void State_29_DeathBegin_45FFA0();

    // Brains
    EXPORT __int16 Brain_Fighting_45C370();

    EXPORT __int16 Brain_BatDeath_45CA60();

    EXPORT __int16 Brain_Death_45CB80();

    EXPORT __int16 Brain_ChasingEnemy_45CC90();

    EXPORT __int16 Brain_Patrol_460020();

    EXPORT __int16 Brain_460D80();

    using TBrainType = decltype(&Scrab::Brain_Fighting_45C370);

    void SetBrain(TBrainType fn);

    bool BrainIs(TBrainType fn);


    TBrainType field_10C_fn;
    __int16 field_110_brain_ret;
    __int16 field_112;
    __int16 field_114_attack_delay;
    __int16 field_116_patrol_type;
    int field_118_timer;
    Scrab* field_11C_pFight_target;
    BaseAliveGameObject* field_120_pTarget;
    int field_124;
    FP field_128;
    FP field_12C;
    __int16 field_130;
    __int16 field_132_res_block_idx;
    int field_134_tlvInfo;
    int field_138_attack_duration;
    int field_13C;
    int field_140;
    __int16 field_144_left_min_delay;
    __int16 field_146_left_max_delay;
    __int16 field_148_right_min_delay;
    __int16 field_14A_right_max_delay;
    int field_14C;
    BYTE** field_150_resources[14];
    __int16 field_188_flags;
    __int16 field_18A;
};
ALIVE_ASSERT_SIZEOF(Scrab, 0x18C);

END_NS_AO

