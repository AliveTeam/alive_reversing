#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

void Paramite_ForceLink();

namespace AO {

// TODO: check me, enum taken from AE
enum class ParamiteSpeak : u8
{
    CMon_or_Attack_0 = 0,
    Stay_1 = 1,
    DoIt_2 = 2,
    LoudStep_3 = 3,
    SlightStep_4 = 4,
    Howdy_5 = 5,
    ClimbingWeb_6 = 6,
    DetectedMeat_7 = 7,
    None_8 = 8,
    AllYa_9 = 9,
    None_10 = 10,
    None_11 = 11,
};

enum class eParamiteMotions : s32
{
    Motion_0_Idle,
    Motion_1_WalkBegin,
    Motion_2_Walking,
    Motion_3_Running,
    Motion_4_Unknown,
    Motion_5_Turn,
    Motion_6_Hop,
    Motion_7_Unknown,
    Motion_8_WalkRunTransition,
    Motion_9_WalkEnd,
    Motion_10_RunBegin,
    Motion_11_RunEnd,
    Motion_12_Falling,
    Motion_13_GameSpeakBegin,
    Motion_14_PreHiss,
    Motion_15_Hiss,
    Motion_16_PostHiss,
    Motion_17_GameSpeakEnd,
    Motion_18_RunningAttack,
    Motion_19_Empty,
    Motion_20_SurpriseWeb,
    Motion_21_WebLeaveDown,
    Motion_22_Unknown,
    Motion_23_Eating,
    Motion_24_Struggle,
    Motion_25_Death
};

class ParamiteWeb;
class Meat;

class Paramite final : public BaseAliveGameObject
{
public:
    Paramite(relive::Path_Paramite* pTlv, const Guid& tlvId);
    ~Paramite();

    eParamiteMotions GetNextMotion() const
    {
        return static_cast<eParamiteMotions>(mNextMotion);
    }
    eParamiteMotions GetCurrentMotion() const
    {
        return static_cast<eParamiteMotions>(mCurrentMotion);
    }

    virtual void VRender(PrimHeader** ppOt) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VScreenChanged() override;
    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUpdate() override;

    void ToIdle();
    s16 ToNextMotion();
    void VUpdateAnimData();
    s16 AnotherParamiteNear();
    void ToKnockBack();
    Meat* FindMeat();
    void MoveOnLine();
    void Sound(ParamiteSpeak idx);
    void ToHop();
    void SetMusic();

    // Brains
    s16 Brain_0_Patrol();
    s16 Brain_1_SurpriseWeb();
    s16 Brain_2_Struggling();
    s16 Brain_3_Death();
    s16 Brain_4_ChasingAbe();
    s16 Brain_5_SpottedMeat();

    s16 IsBeeSwarmChasingMe_4022B0();

    using TParamiteBrain = decltype(&Paramite::Brain_0_Patrol);

    void SetBrain(TParamiteBrain fn);

    bool BrainIs(TParamiteBrain fn);

    enum class DirToCheck
    {
        Left,
        Right,
    };

    s16 HandleEnemyStopper(s16 numGridBlocks, relive::Path_EnemyStopper::StopDirection dir);

    // Motions
    void Motion_0_Idle();
    void Motion_1_WalkBegin();
    void Motion_2_Walking();
    void Motion_3_Running();
    void Motion_4_Unknown();
    void Motion_5_Turn();
    void Motion_6_Hop();
    void Motion_7_Unknown();
    void Motion_8_WalkRunTransition();
    void Motion_9_WalkEnd();
    void Motion_10_RunBegin();
    void Motion_11_RunEnd();
    void Motion_12_Falling();
    void Motion_13_GameSpeakBegin();
    void Motion_14_PreHiss();
    void Motion_15_Hiss();
    void Motion_16_PostHiss();
    void Motion_17_GameSpeakEnd();
    void Motion_18_RunningAttack();
    void Motion_19_Empty();
    void Motion_20_SurpriseWeb();
    void Motion_21_WebLeaveDown();
    void Motion_22_Unknown();
    void Motion_23_Eating();
    void Motion_24_Struggle();
    void Motion_25_Death();


    TParamiteBrain mBrainState = nullptr;
    s16 mBrainSubState = 0;
    s16 mSurpriseWebDelayTimer = 0;
    s32 field_114_timer = 0;
    s16 mMeatEatingTime = 0;
    s16 mAloneChaseDelay = 0;
    s32 mWaitTimer = 0;
    FP field_124_XSpeed = {};
    s16 field_12A_res_idx = 0;
    Guid field_12C_tlvInfo;
    s32 mGroupChaseDelay = 0;
    s32 mAttackTimer = 0;
    s16 mSurpriseWebSwitchId = 0;
    relive::reliveChoice mHissBeforeAttack = relive::reliveChoice::eNo;
    s16 field_140_use_prev_motion = 0;
    s16 field_142_bSnapped = 0;
    relive::reliveChoice mDeleteWhenOutOfSight = relive::reliveChoice::eNo;
    Meat* mMeat = nullptr;
    ParamiteWeb* mParamiteWeb = nullptr;
};

} // namespace AO
