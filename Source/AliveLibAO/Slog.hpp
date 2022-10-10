#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

void Slog_ForceLink();

namespace AO {

#define SLOG_STATES_ENUM(ENTRY)             \
    ENTRY(Motion_0_Idle_4742E0)              \
    ENTRY(Motion_1_Walk_4743F0)              \
    ENTRY(Motion_2_Run_4749A0)               \
    ENTRY(Motion_3_TurnAround_474C70)        \
    ENTRY(Motion_4_Fall_4750C0)              \
    ENTRY(Motion_5_Unknown_474070)           \
    ENTRY(Motion_6_MoveHeadUpwards_474220)   \
    ENTRY(Motion_7_SlideTurn_474DB0)         \
    ENTRY(Motion_8_StopRunning_474EC0)       \
    ENTRY(Motion_9_StartWalking_474690)      \
    ENTRY(Motion_10_EndWalking_4747D0)       \
    ENTRY(Motion_11_Land_475AB0)             \
    ENTRY(Motion_12_Unknown_475B50)          \
    ENTRY(Motion_13_StartFastBarking_475B70) \
    ENTRY(Motion_14_EndFastBarking_475BB0)   \
    ENTRY(Motion_15_AngryBark_475290)            \
    ENTRY(Motion_16_Sleeping_4752E0)         \
    ENTRY(Motion_17_MoveHeadDownwards_475510)       \
    ENTRY(Motion_18_WakeUp_475460)           \
    ENTRY(Motion_19_JumpForwards_475610)     \
    ENTRY(Motion_20_JumpUpwards_475890)      \
    ENTRY(Motion_21_Eating_475900)           \
    ENTRY(Motion_22_Dying_475A90)            \
    ENTRY(Motion_23_Scratch_475550)          \
    ENTRY(Motion_24_Growl_475590)

#define MAKE_ENUM(VAR) VAR,
enum eSlogMotions : s32
{
    SLOG_STATES_ENUM(MAKE_ENUM)
};

class Slog final : public BaseAliveGameObject
{
public:
    Slog(relive::Path_Slog* pTlv, const Guid& tlvId);
    Slog(FP xpos, FP ypos, FP scale);
    ~Slog();

    void LoadAnimations();

    s16 HandleEnemyStopper();
    void ToJump();
    void Sfx(s32 idx);
    s16 IsPlayerNear();
    BaseAliveGameObject* FindAbeMudOrSlig();
    void Init();
    s16 ToNextMotion();
    void ToIdle();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUpdate() override;
    
    void SetAnimFrame();
    void MoveOnLine();

    // Motions
    void Motion_0_Idle_4742E0();
    void Motion_1_Walk_4743F0();
    void Motion_2_Run_4749A0();
    void Motion_3_TurnAround_474C70();
    void Motion_4_Fall_4750C0();
    void Motion_5_Unknown_474070();
    void Motion_6_MoveHeadUpwards_474220();
    void Motion_7_SlideTurn_474DB0();
    void Motion_8_StopRunning_474EC0();
    void Motion_9_StartWalking_474690();
    void Motion_10_EndWalking_4747D0();
    void Motion_11_Land_475AB0();
    void Motion_12_Unknown_475B50();
    void Motion_13_StartFastBarking_475B70();
    void Motion_14_EndFastBarking_475BB0();
    void Motion_15_AngryBark_475290();
    void Motion_16_Sleeping_4752E0();
    void Motion_17_MoveHeadDownwards_475510();
    void Motion_18_WakeUp_475460();
    void Motion_19_JumpForwards_475610();
    void Motion_20_JumpUpwards_475890();
    void Motion_21_Eating_475900();
    void Motion_22_Dying_475A90();
    void Motion_23_Scratch_475550();
    void Motion_24_Growl_475590();

    // Brains
    s16 Brain_0_ListeningToSlig_472450();
    s16 Brain_1_Idle_4719C0();
    s16 Brain_2_ChasingAbe_470F50();
    s16 Brain_3_Dead_4721B0();

    // Inlined
    s16 Facing(FP xpos);

    // Inlined
    void DelayedResponse(s16 responseIdx);

    BaseAliveGameObject* field_10C_pTarget = nullptr;
    s16 field_110 = 0;
    s16 field_112 = 0;
    s16 field_114_brain_idx = 0;
    s16 field_116_brain_sub_state = 0;
    s16 field_118_always_zero = 0;
    s16 field_11A = 0;
    s32 field_11C_timer = 0;
    s32 field_120 = 0;
    s16 field_124 = 0;
    s16 field_126_movedOffScreen = 0;
    s32 field_128 = 0;
    FP field_12C = {};
    s32 field_130 = 0;
    s16 field_134 = 0;
    s16 field_136 = 0;
    Guid field_138_tlvInfo;
    s16 field_13C_res_idx = 0;
    s16 field_13E = 0;
    s32 field_140 = 0;
    s32 field_144 = 0;
    s32 field_148 = -1;
    BaseAliveGameObject* field_14C_pSlig = nullptr;
    s16 field_150_waiting_counter = 0;
    s16 field_152_response_index = 0;
    s16 field_154_response_part = 0;
    s16 field_156 = 0;
    s16 field_158_wake_up_anger = 0;
    s16 field_15A_total_anger = 0;
    s16 field_15C_chase_anger = 0;
    s16 field_15E = 0;
    s32 field_160 = 0;
    s32 field_164_timer = 0;
    s16 mAngerSwitchId = 0;
    s16 field_16A = 0;
    BaseAliveGameObject* field_16C_pUnknown = nullptr;
    s16 field_170 = 0;
    s16 field_172 = 0;
    s16 field_174 = 0;
    s16 field_176 = 0;
    s16 field_178_bShot = 0; // flag in AE
    s16 field_17A = 0;
    s16 field_17C_res = 0;
    relive::reliveChoice mAsleep = relive::reliveChoice::eNo;
    s16 field_180 = 0;
   // u8** field_184_resources[5] = {};
};

extern s16 gNumSlogs_9F11C8;

} // namespace AO
