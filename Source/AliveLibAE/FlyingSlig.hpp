#pragma once

#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/SaveStateBase.hpp"
#include "../relive_lib/FatalError.hpp"

enum class LevelIds : s16;

class FlyingSlig;

using TFlyingSligBrainFn = void (FlyingSlig::*)(void);

#define FLYING_SLIG_MOTIONS_ENUM(ENTRY)         \
    ENTRY(Motion_0_Idle)                      \
    ENTRY(Motion_1_HorizontalMovement)        \
    ENTRY(Motion_2_IdleToTurn)                \
    ENTRY(Motion_3_DownMovement)              \
    ENTRY(Motion_4_DownMovementToTurn)        \
    ENTRY(Motion_5_UpMovement)                \
    ENTRY(Motion_6_UpMovementToTurn)          \
    ENTRY(Motion_7_LeverPull)                 \
    ENTRY(Motion_8_GameSpeak)                 \
    ENTRY(Motion_9_Possession)                \
    ENTRY(Motion_10_EndHorizontalMovement)    \
    ENTRY(Motion_11_BeginUpMovement)          \
    ENTRY(Motion_12_HorizontalToDownMovement) \
    ENTRY(Motion_13_UpToHorizontalMovement)   \
    ENTRY(Motion_14_DownToHorizontalMovement) \
    ENTRY(Motion_15_QuickTurn)                \
    ENTRY(Motion_16_IdleToHorizontalMovement) \
    ENTRY(Motion_17_BeginDownMovement)        \
    ENTRY(Motion_18_EndDownMovement)          \
    ENTRY(Motion_19_DownKnockback)            \
    ENTRY(Motion_20_UpKnockback)              \
    ENTRY(Motion_21_EndUpMovement)            \
    ENTRY(Motion_22_InstantUpXTurn)           \
    ENTRY(Motion_23_InstantDownXTurn)         \
    ENTRY(Motion_24_HorizontalToUpMovement)   \
    ENTRY(Motion_25_TurnToHorizontalMovement)

#define MAKE_ENUM(VAR) VAR,
enum class eFlyingSligMotions
{
    None_m1 = -1,
    FLYING_SLIG_MOTIONS_ENUM(MAKE_ENUM)
};

enum class SligSpeak : s8;

struct FlyingSligSaveState final : public SaveStateBase
{
    FlyingSligSaveState()
        : SaveStateBase(ReliveTypes::eFlyingSlig, sizeof(*this))
    { }
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_vely;
    s16 field_14_path_number;
    EReliveLevelIds field_16_lvl_number;
    FP field_18_sprite_scale;
    s16 field_1C_oldr;
    s16 field_1E_oldg;
    s16 field_20_oldb;
    s16 field_22_bAnimFlipX;
    eFlyingSligMotions field_24_current_state;
    s16 field_26_current_frame;
    s16 field_28_frame_change_counter;
    u8 field_2A_bAnimRender;
    u8 field_2B_bDrawable;
    FP field_2C_current_health;
    eFlyingSligMotions field_30_current_state;
    eFlyingSligMotions field_32_delayed_state;
    s16 field_34_lastLineYPos;
    s16 field_36_line_idx;
    u8 field_38_launch_switch_id;
    bool mPossessed;
    bool mThrowGrenade;
    bool mAlertedAndNotFacingAbe;
    bool mDoAction;
    bool mChanting;
    bool mLastLine;
    bool mSpeaking1;
    bool mSpeaking2;
    bool mUnknown1;
    bool mUnknown2;
    Guid field_3C_tlvInfo;
    s32 field_40_timer;
    s32 field_44_grenade_delay;
    s32 field_48_collision_reaction_timer;
    FP field_4C_xSpeed;
    FP field_50_ySpeed;
    SligSpeak field_54_next_speak;
    s16 field_56_voice_pitch_min;
    Guid field_58_obj_id;
    FP field_5C;
    FP field_60;
    FP field_64;
    FP field_68_line_length;
    FP field_6C;
    FP field_70_lever_pull_range_xpos;
    FP field_74_lever_pull_range_ypos;
    FP field_88_nextXPos;
    FP field_8C_nextYPos;
    s32 field_90_fns1_idx;
    EReliveLevelIds field_9A_abe_level;
    s16 field_9C_abe_path;
    s16 field_9E_abe_camera;
    s16 field_A0_bobbing_values_table_index;
    s32 field_A4_bobbing_values_index;
    FP field_A8_bobbing_value;
};

class FlyingSlig final : public BaseAliveGameObject
{
public:
    static constexpr AnimId sFlyingSligAnimIdTable[26] = {
        AnimId::FlyingSlig_Idle,
        AnimId::FlyingSlig_MoveHorizontal,
        AnimId::FlyingSlig_IdleTurnAround,
        AnimId::FlyingSlig_MoveDown,
        AnimId::FlyingSlig_MoveDownTurnAround,
        AnimId::FlyingSlig_MoveUp,
        AnimId::FlyingSlig_MoveUpTurnAround,
        AnimId::FlyingSlig_PullLever,
        AnimId::FlyingSlig_Speak,
        AnimId::FlyingSlig_Possession,
        AnimId::FlyingSlig_MoveHorizontalEnd,
        AnimId::FlyingSlig_MoveUpStart,
        AnimId::FlyingSlig_MoveHorizontalToDown,
        AnimId::FlyingSlig_MoveUpToHorizontal,
        AnimId::FlyingSlig_MoveDownToHorizontal,
        AnimId::FlyingSlig_TurnQuick,
        AnimId::FlyingSlig_IdleToHorizontal,
        AnimId::FlyingSlig_BeginDownMovement,
        AnimId::FlyingSlig_EndDownMovement,
        AnimId::FlyingSlig_DownKnockback,
        AnimId::FlyingSlig_UpKnockback,
        AnimId::FlyingSlig_EndUpMovement,
        AnimId::FlyingSlig_InstantUpXTurn,
        AnimId::FlyingSlig_InstantDownXTurn,
        AnimId::FlyingSlig_HorizontalToUpMovement,
        AnimId::FlyingSlig_TurnToHorizontalMovement,
    };

    FlyingSlig(relive::Path_FlyingSlig* pTlv, const Guid& tlvId);
    ~FlyingSlig();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VPossessed() override;
    void VSetMotion(eFlyingSligMotions newMotion);
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
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

    static void CreateFromSaveState(SerializedObjectData& pBuffer);
    void ToPlayerControlled();
    void SetBrain(TFlyingSligBrainFn fn);
    bool BrainIs(TFlyingSligBrainFn fn);

    void Brain_0_Inactive();
    void Brain_1_Death();
    void Brain_2_Moving();
    void Brain_3_GetAlerted();
    void Brain_4_ChasingEnemy();
    void Brain_5_Idle();
    void Brain_6_GameSpeakToMoving();
    void Brain_7_PanicMoving();
    void Brain_8_PanicIdle();
    void Brain_9_SpottedEnemy();
    void Brain_10_LaunchingGrenade();
    void Brain_11_AbeDead();
    void Brain_12_Possessed();
    void Brain_13_Possession();
    void Brain_14_DePossession();
    void Brain_15_FlyingSligSpawn();
    void Brain_17_FromCrawlingSlig();

    void Motion_0_Idle();
    void Motion_1_HorizontalMovement();
    void Motion_2_IdleToTurn();
    void Motion_3_DownMovement();
    void Motion_4_DownMovementToTurn();
    void Motion_5_UpMovement();
    void Motion_6_UpMovementToTurn();
    void Motion_7_LeverPull();
    void Motion_8_GameSpeak();
    void Motion_9_Possession();
    void Motion_10_EndHorizontalMovement();
    void Motion_11_BeginUpMovement();
    void Motion_12_HorizontalToDownMovement();
    void Motion_13_UpToHorizontalMovement();
    void Motion_14_DownToHorizontalMovement();
    void Motion_15_QuickTurn();
    void Motion_16_IdleToHorizontalMovement();
    void Motion_17_BeginDownMovement();
    void Motion_18_EndDownMovement();
    void Motion_19_DownKnockback();
    void Motion_20_UpKnockback();
    void Motion_21_EndUpMovement();
    void Motion_22_InstantUpXTurn();
    void Motion_23_InstantDownXTurn();
    void Motion_24_HorizontalToUpMovement();
    void Motion_25_TurnToHorizontalMovement();

private:
    inline void SetMotionHelper(eFlyingSligMotions motion)
    {
        VSetMotion(motion);
    }

    void LoadAnimations();
    void sub_4348A0();
    void ToMoving();
    void ToPanicIdle();
    void ToChase();
    void ToChantShake();
    void ToPossesed();
    void vUpdateAnimRes_4350A0();
    void PatrolDelay();
    s16 CanThrowGrenade();
    void ToLaunchingGrenade();
    void HandlePlayerControls();
    ReliveTypes FindLeftOrRightBound(FP xOrY, FP wOrH);
    s16 CanHearAbe();
    void ToSpottedEnemy();
    void ToAbeDead();
    void ToAlerted();
    void ToPanicMoving();
    s16 IsTurning();
    s16 CollisionUp(FP velY);
    s16 CollisionDown(FP velY);
    s16 CollisionLeftRight(FP velX);
    void PullLever();
    s16 TryPullLever();
    s16 CanChase(IBaseAliveGameObject* pObj);
    void Say(SligSpeak speak, s16 pitch);
    static s16 IsAbeEnteringDoor(IBaseAliveGameObject* pThis);
    static bool IsWallBetween(IBaseAliveGameObject* pThis, IBaseAliveGameObject* pObj);
    void ThrowGrenade();
    void BlowUp();
    s16 IsPossessed();
    void Movement();
    s16 IsFacingMovementDirection();

    // TODO: names
    s16 sub_4374A0(s16 a2);
    s16 sub_436730();
    s16 sub_437C70(PathLine* pLine);
    s16 sub_436C60(PSX_RECT* pRect, s16 op1);
    bool sub_436B20();
    void sub_4373B0();
    void sub_437AC0(FP a2, FP_Point* pPoint);

    // TODO: remove these later
    void SetPreviousMotion(eFlyingSligMotions motion)
    {
        mPreviousMotion = motion;
    }

    void SetCurrentMotion(eFlyingSligMotions motion)
    {
        mCurrentMotion = motion;
    }

    void SetNextMotion(eFlyingSligMotions motion)
    {
        mNextMotion = motion;
    }

private:
    relive::Path_FlyingSlig field_118_data;
    Guid field_148_tlvInfo;
    s32 field_14C_timer = 0;
    s32 field_150_grenade_delay = 0;
    s32 field_154_collision_reaction_timer = 0;
    Guid field_158_obj_id;
    s16 field_15C_voice_pitch_min = 0;
    s16 field_160_voice_pitch_min = 0;
    u8 field_17C_launch_switch_id = 0;
    SligSpeak field_17D_next_speak = SligSpeak::eHi_0;
    bool mThrowGrenade = false;
    bool mAlertedAndNotFacingAbe = false;
    bool mDoAction = false;
    bool mChanting = false;
    bool mLastLine = false;
    bool mSpeaking1 = false;
    bool mSpeaking2 = false;
    bool mNoPrevLine = false;
    bool mNoNextLine = false;
    bool mPersistant = false;
    bool mUnknown1 = false;
    bool mUnknown2 = false;
    ReliveTypes field_180_bound2 = ReliveTypes::eNone;
    ReliveTypes field_182_bound1 = ReliveTypes::eNone;
    FP field_184_xSpeed = {};
    FP field_188_ySpeed = {};
    FP field_18C = {};
    FP field_190 = {};
    FP field_194 = {};
    FP field_198_line_length = {};
    FP_Rect field_1A4_rect = {};
    FP field_1BC = {};
    FP field_1C0 = {};
    FP field_1C4 = {};
    FP field_1C8_lever_pull_range_xpos = {};
    FP field_1CC_lever_pull_range_ypos = {};
    PathLine* field_1EC_pNextLine = nullptr;
    PathLine* field_1F0_pPrevLine = nullptr;
    std::shared_ptr<AnimationPal> field_1F4_mPal;
    s16 field_27C_r = 0;
    s16 field_27E_g = 0;
    s16 field_280_b = 0;
    FP field_284_bobbing_value = {};
    s16 field_28C_bobbing_values_table_index = 0;
    s32 field_290_bobbing_values_index = 0;
    eFlyingSligMotions mPreviousMotion = eFlyingSligMotions::Motion_0_Idle;
    eFlyingSligMotions mCurrentMotion = eFlyingSligMotions::Motion_0_Idle;
    eFlyingSligMotions mNextMotion = eFlyingSligMotions::Motion_0_Idle;
    bool mbMotionChanged = false;

public:
    FP field_294_nextXPos = {};
    FP field_298_nextYPos = {};
    TFlyingSligBrainFn mBrainState = nullptr;
    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;
    FP field_2A8_max_x_speed = {};
    FP field_2AC_up_vel = {};
    FP field_2B0_down_vel = {};
    FP field_2B4_max_slow_down = {};
    FP field_2B8_max_speed_up = {};
};
