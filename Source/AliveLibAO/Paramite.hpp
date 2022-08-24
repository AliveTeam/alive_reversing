#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

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

struct Path_Paramite final : public Path_TLV
{
    Scale_short mScale;
    Choice_short mEnterFromWeb;
    s16 mAloneChaseDelay;
    s16 mSurpriseWebDelayTimer;
    s16 mMeatEatingTime;
    s16 mGroupChaseDelay;
    s16 field_24_disabled_resources; // Unused
    s16 mSurpriseWebSwitchId;
    Choice_short mHissBeforeAttack;
    Choice_short mDeleteWhenOutOfSight;
};
ALIVE_ASSERT_SIZEOF(Path_Paramite, 0x2C);

class Paramite final : public BaseAliveGameObject
{
public:
    Paramite(relive::Path_Paramite* pTlv, s32 tlvInfo);
    ~Paramite();

    eParamiteMotions GetNextMotion() const
    {
        return static_cast<eParamiteMotions>(mNextMotion);
    }
    eParamiteMotions GetCurrentMotion() const
    {
        return static_cast<eParamiteMotions>(mCurrentMotion);
    }

    u8** ResBlockForMotion(s16 motion);

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


    TParamiteBrain field_10C_fn = nullptr;
    s16 field_110_brain_sub_state = 0;
    s16 field_112_surprise_web_delay_timer = 0;
    s32 field_114_timer = 0;
    s32 field_118_not_used = 0;
    s16 field_11C_meat_eating_time = 0;
    s16 field_11E_alone_chase_delay = 0;
    s32 field_120_wait_timer = 0;
    FP field_124_XSpeed = {};
    s16 field_128_never_read = 0;
    s16 field_12A_res_idx = 0;
    s32 field_12C_tlvInfo = 0;
    s32 field_130_not_used = 0;
    s32 field_134_group_chase_delay = 0;
    s32 field_138_attack_timer = 0;
    s16 field_13C_surprise_web_switch_id = 0;
    relive::reliveChoice mHissBeforeAttack = relive::reliveChoice::eNo;
    s16 field_140_use_prev_motion = 0;
    s16 field_142_bSnapped = 0;
    relive::reliveChoice mDeleteWhenOutOfSight = relive::reliveChoice::eNo;
    s16 field_146_not_used = 0;
    Meat* field_148_pMeat = 0;
    ParamiteWeb* field_14C_pWeb = nullptr;
    u8** field_150_resources[16] = {};
};
ALIVE_ASSERT_SIZEOF(Paramite, 0x190);

} // namespace AO
