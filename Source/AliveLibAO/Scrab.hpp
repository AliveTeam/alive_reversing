#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "../AliveLibAE/Path.hpp"

void Scrab_ForceLink();

namespace AO {

#define SCRAB_MOTIONS_ENUM(ENTRY)           \
    ENTRY(Motion_0_Empty_45E3D0)            \
    ENTRY(Motion_1_Stand_45E620)            \
    ENTRY(Motion_2_Walk_45E730)             \
    ENTRY(Motion_3_Run_45EAB0)              \
    ENTRY(Motion_4_Turn_45EF30)             \
    ENTRY(Motion_5_RunToStand_45ED90)       \
    ENTRY(Motion_6_HopBegin_45F3C0)         \
    ENTRY(Motion_7_HopMidair_45F1A0)        \
    ENTRY(Motion_8_HopLand_45F500)             \
    ENTRY(Motion_9_JumpToFall_45EFD0)       \
    ENTRY(Motion_10_StandToWalk_45E670)     \
    ENTRY(Motion_11_StandToRun_45E9F0)      \
    ENTRY(Motion_12_WalkToStand_45E930)     \
    ENTRY(Motion_13_RunJumpBegin_45F5D0)    \
    ENTRY(Motion_14_RunJumpEnd_45F850)      \
    ENTRY(Motion_15_ToFall_45F180)          \
    ENTRY(Motion_16_Stamp_45F920)           \
    ENTRY(Motion_17_Empty_45F9C0)           \
    ENTRY(Motion_18_GetEaten_45FF70)        \
    ENTRY(Motion_19_Unused_45F9D0)          \
    ENTRY(Motion_20_HowlBegin_45FA60)       \
    ENTRY(Motion_21_HowlEnd_45FAF0)         \
    ENTRY(Motion_22_Shriek_45FB00)          \
    ENTRY(Motion_23_ScrabBattleAnim_45FBA0) \
    ENTRY(Motion_24_FeedToGulp_45FC30)      \
    ENTRY(Motion_25_ToFeed_45FCE0)          \
    ENTRY(Motion_26_Feed_45FDA0)            \
    ENTRY(Motion_27_AttackLunge_45FDF0)     \
    ENTRY(Motion_28_LegKick_45FF60)         \
    ENTRY(Motion_29_DeathBegin_45FFA0)

#define MAKE_ENUM(VAR) VAR,
enum eScrabMotions : s32
{
    SCRAB_MOTIONS_ENUM(MAKE_ENUM)
};

enum class ScrabPatrolType : s16
{
    eWalk_0 = 0,
    eRunOrWalk192_1 = 1,
    eRunOrWalk128_2 = 2,
    eRunOrWalk64_3 = 3,
    eRun_4 = 4
};

struct Path_Scrab final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_attack_delay;
    ScrabPatrolType field_1C_patrol_type;
    s16 field_1E_left_min_delay;
    s16 field_20_left_max_delay;
    s16 field_22_right_min_delay;
    s16 field_24_right_max_delay;
    s16 field_26_spotting_abe_delay;
    s16 field_28_disable_resources;
    Choice_short field_2A_roar_randomly;
};
ALIVE_ASSERT_SIZEOF(Path_Scrab, 0x2C);

enum class ScrabSounds : u8
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

class Scrab final : public BaseAliveGameObject
{
public:
    EXPORT Scrab* ctor_45B5F0(Path_Scrab* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_45BA50();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_45BBF0(PrimHeader** ppOt);

    EXPORT Scrab* Vdtor_45C310(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_45B360();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_45BC10(BaseGameObject* pFrom);

    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    EXPORT void VOn_TLV_Collision_45BDC0(Path_TLV* pTlv);

    virtual void VScreenChanged() override;

    EXPORT s16 CanSeeAbe_45C100(BaseAliveGameObject* pObj);

    EXPORT void VScreenChanged_45C290();

    EXPORT void ToStand_45E310();

    EXPORT void vUpdateAnim_45B330();

    EXPORT u8** ResBlockForMotion_45BB30(s16 motion);

    EXPORT void PlatformCollide_45E580();

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_45E5E0();

    EXPORT s16 ToNextMotion_45DFB0();

    void ToStand();

    EXPORT s32 Scrab_SFX_460B80(ScrabSounds soundId, s32 vol, s32 pitch, s16 applyDirection);

    EXPORT void ToJump_45E340();

    EXPORT void MoveOnLine_45E450();

    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;

    EXPORT s16 VOnSameYLevel_45C180(BaseAnimatedWithPhysicsGameObject* pObj);

    EXPORT Scrab* FindScrabToFight_45BE30();

    EXPORT s16 FindAbeOrMud_45BEF0();

    s16 HandleRunning();

    s16 GetMotionForPatrolType(ScrabPatrolType ScrabPatrolType);

    // Motions
    EXPORT void Motion_0_Empty_45E3D0();
    EXPORT void Motion_1_Stand_45E620();
    EXPORT void Motion_2_Walk_45E730();
    EXPORT void Motion_3_Run_45EAB0();
    EXPORT void Motion_4_Turn_45EF30();
    EXPORT void Motion_5_RunToStand_45ED90();
    EXPORT void Motion_6_HopBegin_45F3C0();
    EXPORT void Motion_7_HopMidair_45F1A0();
    EXPORT void Motion_8_HopLand_45F500();
    EXPORT void Motion_9_JumpToFall_45EFD0();
    EXPORT void Motion_10_StandToWalk_45E670();
    EXPORT void Motion_11_StandToRun_45E9F0();
    EXPORT void Motion_12_WalkToStand_45E930();
    EXPORT void Motion_13_RunJumpBegin_45F5D0();
    EXPORT void Motion_14_RunJumpEnd_45F850();
    EXPORT void Motion_15_ToFall_45F180();
    EXPORT void Motion_16_Stamp_45F920();
    EXPORT void Motion_17_Empty_45F9C0();
    EXPORT void Motion_18_GetEaten_45FF70();
    EXPORT void Motion_19_Unused_45F9D0();
    EXPORT void Motion_20_HowlBegin_45FA60();
    EXPORT void Motion_21_HowlEnd_45FAF0();
    EXPORT void Motion_22_Shriek_45FB00();
    EXPORT void Motion_23_ScrabBattleAnim_45FBA0();
    EXPORT void Motion_24_FeedToGulp_45FC30();
    EXPORT void Motion_25_ToFeed_45FCE0();
    EXPORT void Motion_26_Feed_45FDA0();
    EXPORT void Motion_27_AttackLunge_45FDF0();
    EXPORT void Motion_28_LegKick_45FF60();
    EXPORT void Motion_29_DeathBegin_45FFA0();

    // Brains
    EXPORT s16 Brain_Fighting_45C370();

    EXPORT s16 Brain_BatDeath_45CA60();

    EXPORT s16 Brain_Death_45CB80();

    EXPORT s16 Brain_ChasingEnemy_Real_45CC90();

    EXPORT s16 Brain_ChasingEnemy_45CC90();

    EXPORT s16 Brain_Patrol_460020();

    EXPORT s16 Brain_WalkAround_460D80();

    using TBrainType = decltype(&Scrab::Brain_Fighting_45C370);

    void SetBrain(TBrainType fn);

    bool BrainIs(TBrainType fn);

    void SetFightTarget(Scrab* pTarget);
    void SetTarget(BaseAliveGameObject* pTarget);

    TBrainType field_10C_fn;
    s16 field_110_brain_sub_state;
    s16 field_112;
    s16 field_114_attack_delay;
    ScrabPatrolType field_116_patrol_type;
    s32 field_118_timer;
    Scrab* field_11C_pFight_target;
    BaseAliveGameObject* field_120_pTarget;
    s32 field_124;
    FP field_128;
    FP field_12C;
    s16 field_130_unused;
    s16 field_132_res_block_idx;
    s32 field_134_tlvInfo;
    s32 field_138_spotting_abe_delay;
    s32 field_13C_spotting_timer;
    s32 field_140_last_shriek_timer;
    s16 field_144_left_min_delay;
    s16 field_146_left_max_delay;
    s16 field_148_right_min_delay;
    s16 field_14A_right_max_delay;
    s32 field_14C;
    u8** field_150_resources[14];
    s16 field_188_flags;
    s16 field_18A;
};
ALIVE_ASSERT_SIZEOF(Scrab, 0x18C);

} // namespace AO
