#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

class Bone;

struct Path_Slog final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_direction;
    s16 field_14_asleep;
    s16 field_16_wake_up_anger;
    s16 field_18_bark_anger;
    s16 field_1A_chase_anger;
    s16 field_1C_jump_delay;
    s16 field_1E_disabled_resources;
    s16 field_20_angry_id;
    s16 field_22_bone_eating_time;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slog, 0x24);

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

struct Slog_State
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_objectId;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    s16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    FP field_1C_sprite_scale;
    s16 field_20_r;
    s16 field_22_g;
    s16 field_24_b;
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
    s32 field_3C_id;
    s32 field_40_tlvInfo;
    s32 field_44_obj_id;
    s16 field_48_state_idx;
    s16 field_4A_brain_state_result;
    s32 field_4C_timer;
    FP field_50_falling_velx_scale_factor;
    s32 field_54_obj_id;
    s16 field_58_has_woofed;
    s16 field_5A_waiting_counter;
    s16 field_5C_response_index;
    s16 field_5E_response_part;
    s16 field_60_anger_level;
    s16 field_62_jump_counter;
    s32 field_64_scratch_timer;
    s32 field_68_growl_timer;
    s32 field_6C_bone_id;
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
ALIVE_ASSERT_SIZEOF_ALWAYS(Slog_State, 0x78);

class Slog;
using TSlogAIFn = s16 (Slog::*)();
using TSlogMotionFn = void (Slog::*)();

class Slog : public BaseAliveGameObject
{
public:
    EXPORT Slog* ctor_4C4540(FP xpos, FP ypos, FP scale, s16 bListenToSligs, s16 jumpDelay);


    EXPORT Slog* ctor_4C42E0(Path_Slog* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    EXPORT static s32 CC CreateFromSaveState_4C54F0(const u8* pBuffer);

private:
    EXPORT s32 vGetSaveState_4C78F0(Slog_State* pState);


public:
    EXPORT void M_Idle_0_4C5F90();
    EXPORT void M_Walk_1_4C60C0();
    EXPORT void M_Run_2_4C6340();
    EXPORT void M_TurnAround_3_4C65C0();
    EXPORT void M_Fall_4_4C6930();
    EXPORT void M_MoveHeadUpwards_5_4C5F20();
    EXPORT void M_StopRunning_6_4C66C0();
    EXPORT void M_SlideTurn_7_4C6790();
    EXPORT void M_StartWalking_8_4C62E0();
    EXPORT void M_EndWalking_9_4C6310();
    EXPORT void M_Land_10_4C7820();
    EXPORT void M_Unused_11_4C7860();
    EXPORT void M_StartFastBarking_12_4C7880();
    EXPORT void M_EndFastBarking_13_4C78D0();
    EXPORT void M_AngryBark_14_4C6CF0();
    EXPORT void M_Sleeping_15_4C6D60();
    EXPORT void M_MoveHeadDownwards_16_4C70D0();
    EXPORT void M_Bark_17_4C7000();
    EXPORT void M_JumpForwards_18_4C7210();
    EXPORT void M_JumpUpwards_19_4C7470();
    EXPORT void M_Eating_20_4C75F0();
    EXPORT void M_Dying_21_4C77F0();
    EXPORT void M_Scratch_22_4C7120();
    EXPORT void M_Growl_23_4C7170();

public:
    EXPORT s16 AI_ListeningToSlig_0_4C3790();

    s16 AI_ListeningToSlig_State_6_Responding();

    s16 AI_ListeningToSlig_State_5_Waiting();

    s16 AI_ListeningToSlig_State_4_Running(const FP xpos1GridAHead);

    s16 AI_ListeningToSlig_State_3_Walking(const FP xpos1GridAHead);

    s16 AI_ListeningToSlig_State_2_Listening(const FP xpos1GridAHead, BaseAliveGameObject* pObj);

    s16 AI_ListeningToSlig_State_1_Idle(const FP xpos1GridAHead);

    s16 AI_ListeningToSlig_State_0_Init();

    EXPORT s16 AI_Idle_1_4C2830();
    EXPORT s16 AI_ChasingAbe_2_4C0A00();

    s16 AI_ChasingAbe_State_19_AboutToCollide(BaseAliveGameObject* pTarget);

    s16 AI_ChasingAbe_State_18_WaitingToJump(BaseAliveGameObject* pTarget);

    s16 AI_ChasingAbe_State_17_WaitingToChase(BaseAliveGameObject* pTarget);

    s16 AI_ChasingAbe_State_16_JumpingUpwards();

    s16 AI_ChasingAbe_State_15_ChasingAfterTarget(BaseAliveGameObject* pTarget);

    s16 AI_ChasingAbe_State_14_CheckingIfBoneNearby();

    s16 AI_ChasingAbe_State_13_EatingBone();

    s16 AI_ChasingAbe_State_12_WalkingToBone();

    s16 AI_ChasingAbe_State_11_ChasingAfterBone();

    s16 AI_ChasingAbe_State_20_Collided(BaseAliveGameObject* pTarget);

    s16 AI_ChasingAbe_State_10_HungryForBone();

    s16 AI_ChasingAbe_State_9_Falling();

    s16 AI_ChasingAbe_State_8_ToIdle();

    s16 AI_ChasingAbe_State_7_EatingTarget(BaseAliveGameObject* pTarget);

    s16 AI_ChasingAbe_State_4_LungingAtTarget(BaseAliveGameObject* pTarget);

    s16 AI_ChasingAbe_State_3_GrowlOrScratch(BaseAliveGameObject* pTarget);

    s16 AI_ChasingAbe_State_2_Thinking(BaseAliveGameObject* pTarget);

    s16 AI_ChasingAbe_State_1_Waiting();

    s16 AI_ChasingAbe_State_0_Init();

    EXPORT s16 AI_Death_3_4C3250();

private:
    EXPORT u8** ResBlockForMotion_4C4A80(s16 motion);


    EXPORT void SetAnimFrame_4C42A0();

    EXPORT void Init_4C46A0();

    EXPORT void vUpdate_4C50D0();

    EXPORT void dtor_4C49A0();

    EXPORT Slog* vdtor_4C4510(s32 flags);


private:
    EXPORT void ToIdle_4C5C10();

    EXPORT void Sfx_4C7D30(SlogSound effectId);

    EXPORT void ToJump_4C5C60();

    EXPORT s16 ToNextMotion_4C5A30();

    EXPORT BOOL CollisionCheck_4C5480(FP hitY, FP hitX);

    EXPORT void MoveOnLine_4C5DA0();

    EXPORT Bone* FindBone_4C25B0();

    EXPORT BaseAliveGameObject* FindTarget_4C33C0(s16 bKillSligs, s16 bLookingUp);

    EXPORT void vOn_TrapDoor_Open_4C5D50();

    EXPORT void vOn_Tlv_Collision_4C5060(Path_TLV* pTlv);

    EXPORT s16 vTakeDamage_4C4B80(BaseGameObject* pFrom);

    EXPORT void vOnThrowableHit_4C4B50(BaseGameObject* pFrom);

    EXPORT s16 PlayerOrNakedSligNear_4C26A0();

    EXPORT void DelayedResponse_4C3750(s16 responseIdx);

    EXPORT s16 HandleEnemyStopper_4C5340();

    EXPORT s16 Facing_4C4020(FP xpos);

public:
    s32 field_118_target_id;
    s16 field_11C_biting_target;

private:
    s16 field_11E_padding;
    u16 field_120_brain_state_idx;
    s16 field_122_brain_state_result;
    s32 field_124_timer;
    FP field_128_falling_velx_scale_factor;
    s32 field_12C_tlvInfo;
    s16 field_130_motion_resource_block_index;
    s16 field_132_has_woofed;
    s32 field_134_last_event_index;
    s32 field_138_listening_to_slig_id;
    s16 field_13C_waiting_counter;
    s16 field_13E_response_index;
    s16 field_140_response_part;
    s16 field_142_anger_level;
    s16 field_144_wake_up_anger;
    s16 field_146_total_anger;
    s16 field_148_chase_anger;
    s16 field_14A_padding;
    s32 field_14C_scratch_timer;
    s32 field_150_growl_timer;
    s16 field_154_angry_id;
    s16 field_156_bone_eating_time;
    s16 field_158_chase_delay;
    s16 field_15A_jump_counter;
    s32 field_15C_bone_id;
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
    BitField16<Flags_160> field_160_flags;
    s16 field_162_padding;
};
ALIVE_ASSERT_SIZEOF(Slog, 0x164);

ALIVE_VAR_EXTERN(s16, sSlogCount_BAF7F2);
