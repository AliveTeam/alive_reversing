#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

void Slog_ForceLink();

namespace AO {

struct Path_Slog final : public Path_TLV
{
    Scale_short field_18_scale;
    XDirection_short field_1A_start_direction;
    Choice_short field_1C_asleep;
    s16 field_1E_wake_up_anger;
    s16 field_20_bark_anger;
    s16 field_22_chase_anger;
    s16 field_24_chase_delay;
    s16 field_26_disabled_resources;
    s16 field_28_anger_switch_id;
    s16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Slog, 0x2C);

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
    Slog(Path_Slog* pTlv, s32 tlvInfo);
    Slog(FP xpos, FP ypos, FP scale);
    ~Slog();

    s16 HandleEnemyStopper();
    void ToJump();
    void Sfx(s32 idx);
    s16 IsPlayerNear();
    BaseAliveGameObject* FindAbeMudOrSlig();
    void Init();
    s16 ToNextMotion();
    void ToIdle();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUpdate() override;
    
    void SetAnimFrame();
    u8** ResBlockForMotion(s16 motion);
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

    BaseAliveGameObject* field_10C_pTarget;
    s16 field_110;
    s16 field_112;
    s16 field_114_brain_idx;
    s16 field_116_brain_sub_state;
    s16 field_118_always_zero;
    s16 field_11A;
    s32 field_11C_timer;
    s32 field_120;
    s16 field_124;
    s16 field_126_movedOffScreen;
    s32 field_128;
    FP field_12C;
    s32 field_130;
    s16 field_134;
    s16 field_136;
    s32 field_138_tlvInfo;
    s16 field_13C_res_idx;
    s16 field_13E;
    s32 field_140;
    s32 field_144;
    s32 field_148;
    BaseAliveGameObject* field_14C_pSlig;
    s16 field_150_waiting_counter;
    s16 field_152_response_index;
    s16 field_154_response_part;
    s16 field_156;
    s16 field_158_wake_up_anger;
    s16 field_15A_total_anger;
    s16 field_15C_chase_anger;
    s16 field_15E;
    s32 field_160;
    s32 field_164_timer;
    s16 field_168_anger_switch_id;
    s16 field_16A;
    BaseAliveGameObject* field_16C_pUnknown;
    s16 field_170;
    s16 field_172;
    s16 field_174;
    s16 field_176;
    s16 field_178_bShot; // flag in AE
    s16 field_17A;
    s16 field_17C_res;
    Choice_short field_17E_asleep;
    s16 field_180;
    s16 field_182_pad;
    u8** field_184_resources[5];
};
ALIVE_ASSERT_SIZEOF(Slog, 0x198);

ALIVE_VAR_EXTERN(s16, gNumSlogs_9F11C8);

} // namespace AO
