#pragma once

#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/FatalError.hpp"

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

enum class eParamiteMotions
{
    None_m1 = -1,
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

class Paramite final : public ::BaseAliveGameObject
{
public:
    static constexpr AnimId sParamiteMotionAnimIds[26] = {
        AnimId::Paramite_Idle,
        AnimId::Paramite_WalkBegin,
        AnimId::Paramite_Walking,
        AnimId::Paramite_Running,
        AnimId::Paramite_Idle,
        AnimId::Paramite_Turn,
        AnimId::Paramite_Hop,
        AnimId::Paramite_Hop, // HACK fix unused not converting // AnimId::Paramite_AO_M_7_Unknown,
        AnimId::Paramite_WalkRunTransition,
        AnimId::Paramite_WalkEnd,
        AnimId::Paramite_RunBegin,
        AnimId::Paramite_RunEnd,
        AnimId::Paramite_Falling,
        AnimId::Paramite_GameSpeakBegin,
        AnimId::Paramite_PreHiss,
        AnimId::Paramite_Hiss,
        AnimId::Paramite_PostHiss,
        AnimId::Paramite_GameSpeakEnd,
        AnimId::Paramite_RunningAttack,
        AnimId::Paramite_SurpriseWeb,
        AnimId::Paramite_SurpriseWeb,
        AnimId::Paramite_WebLeaveDown,
        AnimId::Paramite_AO_M_22_Unknown,
        AnimId::Paramite_Eating,
        AnimId::Paramite_Death,
        AnimId::Paramite_Struggle};

    Paramite(relive::Path_Paramite* pTlv, const Guid& tlvId);
    ~Paramite();

    virtual void VRender(OrderingTable& ot) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VScreenChanged() override;
    virtual bool VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUpdate() override;
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
    bool mHissBeforeAttack = false;
    s16 field_140_use_prev_motion = 0;
    s16 field_142_bSnapped = 0;
    bool mDeleteWhenOutOfSight = false;
    Guid mMeat = {};
    Guid mParamiteWeb = {};
    eParamiteMotions mPreviousMotion = eParamiteMotions::Motion_0_Idle;
    eParamiteMotions mCurrentMotion = eParamiteMotions::Motion_0_Idle;
    eParamiteMotions mNextMotion = eParamiteMotions::Motion_0_Idle;
};

} // namespace AO
