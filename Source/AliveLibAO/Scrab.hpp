#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/FatalError.hpp"

namespace AO {

#define SCRAB_MOTIONS_ENUM_AO(ENTRY)           \
    ENTRY(Motion_0_Empty)            \
    ENTRY(Motion_1_Stand)            \
    ENTRY(Motion_2_Walk)             \
    ENTRY(Motion_3_Run)              \
    ENTRY(Motion_4_Turn)             \
    ENTRY(Motion_5_RunToStand)       \
    ENTRY(Motion_6_HopBegin)         \
    ENTRY(Motion_7_HopMidair)        \
    ENTRY(Motion_8_HopLand)          \
    ENTRY(Motion_9_JumpToFall)       \
    ENTRY(Motion_10_StandToWalk)     \
    ENTRY(Motion_11_StandToRun)      \
    ENTRY(Motion_12_WalkToStand)     \
    ENTRY(Motion_13_RunJumpBegin)    \
    ENTRY(Motion_14_RunJumpEnd)      \
    ENTRY(Motion_15_ToFall)          \
    ENTRY(Motion_16_Stamp)           \
    ENTRY(Motion_17_Empty)           \
    ENTRY(Motion_18_GetEaten)        \
    ENTRY(Motion_19_Unused)          \
    ENTRY(Motion_20_HowlBegin)       \
    ENTRY(Motion_21_HowlEnd)         \
    ENTRY(Motion_22_Shriek)          \
    ENTRY(Motion_23_ScrabBattleAnim) \
    ENTRY(Motion_24_FeedToGulp)      \
    ENTRY(Motion_25_ToFeed)          \
    ENTRY(Motion_26_Feed)            \
    ENTRY(Motion_27_AttackLunge)     \
    ENTRY(Motion_28_LegKick)         \
    ENTRY(Motion_29_DeathBegin)

#define MAKE_ENUM(VAR) VAR,
enum class eScrabMotions
{
    None_m1 = -1,
    SCRAB_MOTIONS_ENUM_AO(MAKE_ENUM)
};

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
    static constexpr AnimId sScrabMotionAnimIds[30] = {
        AnimId::Scrab_Idle,
        AnimId::Scrab_Idle,
        AnimId::Scrab_Walk,
        AnimId::Scrab_Run,
        AnimId::Scrab_Turn,
        AnimId::Scrab_RunToStand,
        AnimId::Scrab_HopBegin,
        AnimId::Scrab_Jump,
        AnimId::Scrab_Landing,
        AnimId::Scrab_Idle,
        AnimId::Scrab_StandToWalk,
        AnimId::Scrab_StandToRun,
        AnimId::Scrab_WalkToStand,
        AnimId::Scrab_Jump,
        AnimId::Scrab_Landing,
        AnimId::Scrab_AO_ToFall,
        AnimId::Scrab_Stamp,
        AnimId::Scrab_GetEaten,
        AnimId::Scrab_GetEaten,
        AnimId::Scrab_AO_M_19_Unused,
        AnimId::Scrab_HowlBegin,
        AnimId::Scrab_HowlEnd,
        AnimId::Scrab_Shriek,
        AnimId::Scrab_ScrabBattleAnim,
        AnimId::Scrab_FeedToGulp,
        AnimId::Scrab_AO_ToFeed,
        AnimId::Scrab_Feed,
        AnimId::Scrab_AttackLunge,
        AnimId::Scrab_LegKick,
        AnimId::Scrab_DeathBegin};

    Scrab(relive::Path_Scrab* pTlv, const Guid& tlvId);
    ~Scrab();

    virtual void VRender(OrderingTable& ot) override;
    virtual void VUpdate() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VScreenChanged() override;
    virtual void VOnTrapDoorOpen() override;
    virtual bool VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;
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

    s16 CanSeeAbe(BaseAliveGameObject* pObj);
    void vUpdateAnim();
    void PlatformCollide();
    s16 ToNextMotion();
    void ToStand();
    s32 Scrab_SFX(ScrabSounds soundId, s32 vol, s32 pitch, s16 applyDirection);
    void ToJump();
    void MoveOnLine();


    Scrab* FindScrabToFight();
    bool FindAbeOrMud();
    s16 HandleRunning(); // returns the brain sub state
    eScrabMotions GetMotionForPatrolType(relive::Path_Scrab::ScrabPatrolType ScrabPatrolType);

    // Motions
    void Motion_0_Empty();
    void Motion_1_Stand();
    void Motion_2_Walk();
    void Motion_3_Run();
    void Motion_4_Turn();
    void Motion_5_RunToStand();
    void Motion_6_HopBegin();
    void Motion_7_HopMidair();
    void Motion_8_HopLand();
    void Motion_9_JumpToFall();
    void Motion_10_StandToWalk();
    void Motion_11_StandToRun();
    void Motion_12_WalkToStand();
    void Motion_13_RunJumpBegin();
    void Motion_14_RunJumpEnd();
    void Motion_15_ToFall();
    void Motion_16_Stamp();
    void Motion_17_Empty();
    void Motion_18_GetEaten();
    void Motion_19_Unused();
    void Motion_20_HowlBegin();
    void Motion_21_HowlEnd();
    void Motion_22_Shriek();
    void Motion_23_ScrabBattleAnim();
    void Motion_24_FeedToGulp();
    void Motion_25_ToFeed();
    void Motion_26_Feed();
    void Motion_27_AttackLunge();
    void Motion_28_LegKick();
    void Motion_29_DeathBegin();

    // Brains
    s16 Brain_Fighting();
    s16 Brain_BatDeath();
    s16 Brain_Death();
    s16 Brain_ChasingEnemy();
    s16 Brain_Patrol();
    s16 Brain_WalkAround();

    using TBrainType = decltype(&Scrab::Brain_Fighting);

    void SetBrain(TBrainType fn);

    bool BrainIs(TBrainType fn);

    void SetFightTarget(Scrab* pTarget);
    void SetTarget(BaseAliveGameObject* pTarget);

    TBrainType mBrainState = nullptr;
    s16 mBrainSubState = 0;
    s16 mAttackDelay = 0;
    relive::Path_Scrab::ScrabPatrolType mPatrolType = relive::Path_Scrab::ScrabPatrolType::eWalk;
    s32 field_118_timer = 0;
    Scrab* mScrabTarget = nullptr;
    BaseAliveGameObject* mAbeOrMudTarget = nullptr;
    FP field_128 = {};
    FP field_12C = {};
    Guid field_134_tlvInfo;
    s32 field_138_spotting_abe_delay = 0;
    s32 field_13C_spotting_timer = 0;
    s32 field_140_last_shriek_timer = 0;
    s16 mPauseLeftMin = 0;
    s16 mPauseLeftMax = 0;
    s16 mPauseRightMin = 0;
    s16 mPauseRightMax = 0;
    s32 field_14C = 0; // scrab sfx channel mask
    s16 field_188_flags = 0;
    eScrabMotions mPreviousMotion = eScrabMotions::Motion_0_Empty;
    eScrabMotions mCurrentMotion = eScrabMotions::Motion_0_Empty;
    eScrabMotions mNextMotion = eScrabMotions::Motion_0_Empty;
    bool mbMotionChanged = false;
};

} // namespace AO
