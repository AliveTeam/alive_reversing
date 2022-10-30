#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"

class Bone;

namespace relive
{
    struct Path_Slog;
}

enum class SlogSound : s32
{
    Padding_0 = 0,
    Padding_1 = 1,
    IdleWoof_2 = 2,
    IdleGrrr_3 = 3,
    IdleGrrAlt_4 = 4,
    CautiousWoof_5 = 5,
    JumpBite_6 = 6,
    Padding_7 = 7,
    AttackGrowl_8 = 8,
    DeathWhine_9 = 9,
    YawnStart_10 = 10,
    YawnEnd_11 = 11,
    Padding_12 = 12,
    HungryYip_13 = 13,
    Skid_14 = 14,
    Padding_15 = 15,
    Landing_16 = 16,
    FastStep_17 = 17,
    SlowStep_18 = 18
};

enum class eSlogMotions
{
    m2 = -2,
    m1 = -1,
    Motion_0_Idle,
    Motion_1_Walk,
    Motion_2_Run,
    Motion_3_TurnAround,
    Motion_4_Fall,
    Motion_5_MoveHeadUpwards,
    Motion_6_StopRunning,
    Motion_7_SlideTurn,
    Motion_8_StartWalking,
    Motion_9_EndWalking,
    Motion_10_Land,
    Motion_11_Unused,
    Motion_12_StartFastBarking,
    Motion_13_EndFastBarking,
    Motion_14_AngryBark,
    Motion_15_Sleeping,
    Motion_16_MoveHeadDownwards,
    Motion_17_WakeUp,
    Motion_18_JumpForwards,
    Motion_19_JumpUpwards,
    Motion_20_Eating,
    Motion_21_Dying,
    Motion_22_Scratch,
    Motion_23_Growl
};

struct Slog_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    Guid field_4_objectId;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    FP field_1C_sprite_scale;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
    s16 field_26_bAnimFlipX;
    s16 field_28_current_motion;
    s16 field_2A_anim_cur_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_bRender;
    s8 field_2F_bDrawable;
    FP field_30_health;
    s16 field_34_current_motion;
    s16 field_36_next_motion;
    s16 field_38_last_line_ypos;
    s16 field_3A_line_type;
    Guid field_3C_id;
    Guid field_40_tlvInfo;
    Guid field_44_obj_id;
    s16 field_48_state_idx;
    s16 field_4A_brain_state_result;
    s32 field_4C_timer;
    FP field_50_falling_velx_scale_factor;
    Guid field_54_obj_id;
    s16 field_58_has_woofed;
    s16 field_5A_waiting_counter;
    s16 field_5C_response_index;
    s16 field_5E_response_part;
    s16 field_60_anger_level;
    s16 field_62_jump_counter;
    s32 field_64_scratch_timer;
    s32 field_68_growl_timer;
    Guid field_6C_bone_id;
    s16 field_70_jump_delay;
    u8 field_72_slog_random_index;
    u8 field_73_padding;

    enum Flags_74
    {
        eBit1_BitingTarget = 0x1,
        eBit2_Possessed = 0x2,
        eBit3_Asleep = 0x4,
        eBit4_MovedOffScreen = 0x8,
        eBit5_StopRunning = 0x10,
        eBit6_Shot = 0x20,
        eBit7_Hungry = 0x40,
        eBit8_CommandedToAttack = 0x80,
        eBit9_HitByAbilityRing = 0x100,
        eBit10_ListenToSligs = 0x200,
    };
    BitField16<Flags_74> field_74_flags;
    s16 field_76_padding;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(Slog_State, 0x78);

class Slog;
using TSlogBrainFn = s16 (Slog::*)();
using TSlogMotionFn = void (Slog::*)();

class Slog final : public BaseAliveGameObject
{
public:
    Slog(FP xpos, FP ypos, FP scale, s16 bListenToSligs, s16 jumpDelay);
    Slog(relive::Path_Slog* pTlv, const Guid& tlvId);
    ~Slog();

    void LoadAnimations();
    virtual void VUpdate() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

public:
    // Motions
    void Motion_0_Idle();
    void Motion_1_Walk();
    void Motion_2_Run();
    void Motion_3_TurnAround();
    void Motion_4_Fall();
    void Motion_5_MoveHeadUpwards();
    void Motion_6_StopRunning();
    void Motion_7_SlideTurn();
    void Motion_8_StartWalking();
    void Motion_9_EndWalking();
    void Motion_10_Land();
    void Motion_11_Unused();
    void Motion_12_StartFastBarking();
    void Motion_13_EndFastBarking();
    void Motion_14_AngryBark();
    void Motion_15_Sleeping();
    void Motion_16_MoveHeadDownwards();
    void Motion_17_WakeUp();
    void Motion_18_JumpForwards();
    void Motion_19_JumpUpwards();
    void Motion_20_Eating();
    void Motion_21_Dying();
    void Motion_22_Scratch();
    void Motion_23_Growl();

public:
    s16 Brain_0_ListeningToSlig();

    s16 Brain_ListeningToSlig_State_0_Init();
    s16 Brain_ListeningToSlig_State_1_Idle(const FP xpos1GridAHead);
    s16 Brain_ListeningToSlig_State_2_Listening(const FP xpos1GridAHead, IBaseAliveGameObject* pObj);
    s16 Brain_ListeningToSlig_State_3_Walking(const FP xpos1GridAHead);
    s16 Brain_ListeningToSlig_State_4_Running(const FP xpos1GridAHead);
    s16 Brain_ListeningToSlig_State_5_Waiting();
    s16 Brain_ListeningToSlig_State_6_Responding();

    s16 Brain_1_Idle();

    s16 Brain_2_ChasingAbe();
    s16 Brain_ChasingAbe_State_0_Init();
    s16 Brain_ChasingAbe_State_1_Waiting();
    s16 Brain_ChasingAbe_State_2_Thinking(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_3_GrowlOrScratch(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_4_LungingAtTarget(IBaseAliveGameObject* pTarget);


    s16 Brain_ChasingAbe_State_7_EatingTarget(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_8_ToIdle();
    s16 Brain_ChasingAbe_State_9_Falling();
    s16 Brain_ChasingAbe_State_10_HungryForBone();
    s16 Brain_ChasingAbe_State_11_ChasingAfterBone();
    s16 Brain_ChasingAbe_State_12_WalkingToBone();
    s16 Brain_ChasingAbe_State_13_EatingBone();
    s16 Brain_ChasingAbe_State_14_CheckingIfBoneNearby();
    s16 Brain_ChasingAbe_State_15_ChasingAfterTarget(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_16_JumpingUpwards();
    s16 Brain_ChasingAbe_State_17_WaitingToChase(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_18_WaitingToJump(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_19_AboutToCollide(IBaseAliveGameObject* pTarget);
    s16 Brain_ChasingAbe_State_20_Collided(IBaseAliveGameObject* pTarget);

    s16 Brain_3_Death();

private:
    u8** ResBlockForMotion(s16 motion);
    void SetAnimFrame();
    void Init();

private:
    eSlogMotions GetNextMotion() const
    {
        return static_cast<eSlogMotions>(mNextMotion);
    }
    eSlogMotions GetCurrentMotion() const
    {
        return static_cast<eSlogMotions>(mCurrentMotion);
    }

    void ToIdle();
    void Sfx(SlogSound effectId);
    void ToJump();
    s16 ToNextMotion();
    bool CollisionCheck(FP hitY, FP hitX);
    void MoveOnLine();
    Bone* FindBone();
    IBaseAliveGameObject* FindTarget(s16 bKillSligs, s16 bLookingUp);
    s16 PlayerOrNakedSligNear();
    void DelayedResponse(s16 responseIdx);
    s16 HandleEnemyStopper();
    s16 Facing(FP xpos);

public:
    Guid field_118_target_id;
    s16 field_11C_biting_target = 0;

private:
    u16 field_120_brain_state_idx = 0;
    s16 field_122_brain_state_result = 0;
    s32 field_124_timer = 0;
    FP field_128_falling_velx_scale_factor = {};
    Guid field_12C_tlvInfo;
    s16 field_130_motion_resource_block_index = 0;
    s16 field_132_has_woofed = 0;
    s32 field_134_last_event_index = 0;
    Guid field_138_listening_to_slig_id;
    s16 field_13C_waiting_counter = 0;
    s16 field_13E_response_index = 0;
    s16 field_140_response_part = 0;
    s16 field_142_anger_level = 0;
    s16 field_144_wake_up_anger = 0;
    s16 field_146_total_anger = 0;
    s16 field_148_chase_anger = 0;
    s32 field_14C_scratch_timer = 0;
    s32 field_150_growl_timer = 0;
    s16 field_154_anger_switch_id = 0;
    s16 field_156_bone_eating_time = 0;
    s16 field_158_chase_delay = 0;
    s16 field_15A_jump_counter = 0;
    Guid field_15C_bone_id;
    enum Flags_160 : s16
    {
        eBit1_StopRunning = 0x1,
        eBit2_ListenToSligs = 0x2,
        eBit3_Shot = 0x4,
        eBit4_Hungry = 0x8,
        eBit5_CommandedToAttack = 0x10,
        eBit6_HitByAbilityRing = 0x20,
        eBit7_Asleep = 0x40,
        eBit8_Asleep = 0x80,
        eBit9_MovedOffScreen = 0x100,
        eBit10_Padding = 0x200,
        eBit11_Padding = 0x400,
        eBit12_Padding = 0x800,
        eBit13_Unused = 0x1000,
        eBit14_Padding = 0x2000,
        eBit15_Padding = 0x4000,
    };
    BitField16<Flags_160> field_160_flags = {};
};

extern s16 sSlogCount;
