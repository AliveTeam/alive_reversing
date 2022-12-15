#pragma once

#include "BaseAliveGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

#define SLOG_STATES_ENUM_AO(ENTRY)             \
    ENTRY(Motion_0_Idle)              \
    ENTRY(Motion_1_Walk)              \
    ENTRY(Motion_2_Run)               \
    ENTRY(Motion_3_TurnAround)        \
    ENTRY(Motion_4_Fall)              \
    ENTRY(Motion_5_Unknown)           \
    ENTRY(Motion_6_MoveHeadUpwards)   \
    ENTRY(Motion_7_SlideTurn)         \
    ENTRY(Motion_8_StopRunning)       \
    ENTRY(Motion_9_StartWalking)      \
    ENTRY(Motion_10_EndWalking)       \
    ENTRY(Motion_11_Land)             \
    ENTRY(Motion_12_Unknown)          \
    ENTRY(Motion_13_StartFastBarking) \
    ENTRY(Motion_14_EndFastBarking)   \
    ENTRY(Motion_15_AngryBark)            \
    ENTRY(Motion_16_Sleeping)         \
    ENTRY(Motion_17_MoveHeadDownwards)       \
    ENTRY(Motion_18_WakeUp)           \
    ENTRY(Motion_19_JumpForwards)     \
    ENTRY(Motion_20_JumpUpwards)      \
    ENTRY(Motion_21_Eating)           \
    ENTRY(Motion_22_Dying)            \
    ENTRY(Motion_23_Scratch)          \
    ENTRY(Motion_24_Growl)

#define MAKE_ENUM(VAR) VAR,
enum class eSlogMotions : s32
{
    None_m2 = -2,
    None_m1 = -1,
    SLOG_STATES_ENUM_AO(MAKE_ENUM)
};

class Slog final : public BaseAliveGameObject
{
public:
    Slog(relive::Path_Slog* pTlv, const Guid& tlvId);
    Slog(FP xpos, FP ypos, FP scale);
    ~Slog();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUpdate() override;

    // Motions
    void Motion_0_Idle();
    void Motion_1_Walk();
    void Motion_2_Run();
    void Motion_3_TurnAround();
    void Motion_4_Fall();
    void Motion_5_Unknown();
    void Motion_6_MoveHeadUpwards();
    void Motion_7_SlideTurn();
    void Motion_8_StopRunning();
    void Motion_9_StartWalking();
    void Motion_10_EndWalking();
    void Motion_11_Land();
    void Motion_12_Unknown();
    void Motion_13_StartFastBarking();
    void Motion_14_EndFastBarking();
    void Motion_15_AngryBark();
    void Motion_16_Sleeping();
    void Motion_17_MoveHeadDownwards();
    void Motion_18_WakeUp();
    void Motion_19_JumpForwards();
    void Motion_20_JumpUpwards();
    void Motion_21_Eating();
    void Motion_22_Dying();
    void Motion_23_Scratch();
    void Motion_24_Growl();

    // Brains
    s16 Brain_0_ListeningToSlig();
    s16 Brain_1_Idle();
    s16 Brain_2_ChasingAbe();
    s16 Brain_3_Dead();

private:
    void LoadAnimations();

    s16 HandleEnemyStopper();
    void ToJump();
    void Sfx(s32 idx);
    s16 IsPlayerNear();
    IBaseAliveGameObject* FindAbeMudOrSlig();
    void Init();
    s16 ToNextMotion();
    void ToIdle();

    void SetAnimFrame();
    void MoveOnLine();

    // Inlined
    s16 Facing(FP xpos);

    // Inlined
    void DelayedResponse(s16 responseIdx);
    eSlogMotions GetCurrentMotion() const
    {
        return static_cast<eSlogMotions>(mCurrentMotion);
    }

    eSlogMotions GetNextMotion() const
    {
        return static_cast<eSlogMotions>(mNextMotion);
    }

public:
    IBaseAliveGameObject* field_10C_pTarget = nullptr;
    s16 field_110 = 0;

private:
    s16 field_114_brain_idx = 0;
    s16 field_116_brain_sub_state = 0;
    s32 field_11C_timer = 0;
    s16 field_126_movedOffScreen = 0;
    Guid field_138_tlvInfo;
    s16 field_13C_res_idx = 0;
    s32 field_144 = 0;
    s32 field_148 = -1;
    IBaseAliveGameObject* field_14C_pSlig = nullptr;
    s16 field_150_waiting_counter = 0;
    s16 field_152_response_index = 0;
    s16 field_154_response_part = 0;
    s16 field_156 = 0;
    s16 field_158_wake_up_anger = 0;
    s16 field_15A_total_anger = 0;
    s16 field_15C_chase_anger = 0;
    s32 field_160 = 0;
    s32 field_164_timer = 0;
    s16 mAngerSwitchId = 0;
    IBaseAliveGameObject* field_16C_pUnknown = nullptr;
    s16 mChaseDelay = 0;
    s16 field_172 = 0;
    s16 field_174 = 0; // slog moving left?
    s16 field_176 = 0; // set to 1 in the tlv slog ctor but set to 0 in the other slog ctor
    s16 field_178_bShot = 0; // flag in AE
    s16 field_17C_res = 0; // skips playing a sound after taking damage from a slig
    relive::reliveChoice mAsleep = relive::reliveChoice::eNo;
   // u8** field_184_resources[5] = {};
};

extern s16 gNumSlogs_9F11C8;

} // namespace AO
