#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "GameSpeak.hpp"
#include "Path.hpp"

#define SCRAB_MOTIONS_ENUM_AE(ENTRY)          \
    ENTRY(Motion_0_Stand)                \
    ENTRY(Motion_1_Walk)                 \
    ENTRY(Motion_2_Run)                  \
    ENTRY(Motion_3_Turn)                 \
    ENTRY(Motion_4_RunToStand)           \
    ENTRY(Motion_5_HopBegin)             \
    ENTRY(Motion_6_HopMidair)            \
    ENTRY(Motion_7_HopLand)              \
    ENTRY(Motion_8_JumpToFall)           \
    ENTRY(Motion_9_StandToWalk)          \
    ENTRY(Motion_10_StandToRun)          \
    ENTRY(Motion_11_WalkToStand)         \
    ENTRY(Motion_12_RunJumpBegin)        \
    ENTRY(Motion_13_RunJumpEnd)          \
    ENTRY(Motion_14_WalkToFall)          \
    ENTRY(Motion_15_RunToFall)           \
    ENTRY(Motion_16_WalkToRun)           \
    ENTRY(Motion_17_RunToWalk)           \
    ENTRY(Motion_18_Knockback)           \
    ENTRY(Motion_19_GetEaten)            \
    ENTRY(Motion_20_Fall)                \
    ENTRY(Motion_21_Stamp)               \
    ENTRY(Motion_22_GetPossessed)        \
    ENTRY(Motion_23_Empty)               \
    ENTRY(Motion_24_DeathEnd)            \
    ENTRY(Motion_25_Empty)               \
    ENTRY(Motion_26_HowlBegin)           \
    ENTRY(Motion_27_HowlEnd)             \
    ENTRY(Motion_28_GetDepossessedBegin) \
    ENTRY(Motion_29_GetDepossessedEnd)   \
    ENTRY(Motion_30_Shriek)              \
    ENTRY(Motion_31_ScrabBattleAnim)     \
    ENTRY(Motion_32_AttackSpin)          \
    ENTRY(Motion_33_FeedToGulp)          \
    ENTRY(Motion_34_GulpToStand)         \
    ENTRY(Motion_35_StandToFeed)         \
    ENTRY(Motion_36_Feed)                \
    ENTRY(Motion_37_AttackLunge)         \
    ENTRY(Motion_38_LegKick)             \
    ENTRY(Motion_39_DeathBegin)

#define MAKE_ENUM(VAR) VAR,
enum eScrabMotions : s32
{
    SCRAB_MOTIONS_ENUM_AE(MAKE_ENUM)
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
    eWalk2_7 = 7,
    eYell_8 = 8,
};

enum class LevelIds : s16;
enum class GameSpeakEvents : s16;

struct ScrabSaveState final
{
    AETypes field_0_type;
    s16 field_2_padding;
    Guid field_4_obj_id;
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
    s16 field_2A_current_frame;
    s16 field_2C_frame_change_counter;
    s8 field_2E_bAnimRender;
    s8 field_2F_bDrawable;
    FP field_30_health;
    s16 field_34_current_motion;
    s16 field_36_next_motion;
    s16 field_38_last_line_ypos;
    s16 field_3A_line_type;
    s16 field_3C_padding;
    s16 field_3E_padding;
    s8 field_40_bIsControlled;
    s8 field_41_padding;
    s16 field_42_padding;
    Guid field_44_tlvInfo;
    s32 field_48_brain_idx;
    s16 field_4C_padding;
    s16 field_4E_padding;
    s16 field_50_sub_state;
    s16 field_52_padding;
    Guid field_54_obj_id;
    Guid field_58_target_obj_id;
    s32 field_5C_timer;
    s32 field_60_depossession_timer;
    FP field_64_falling_velx_scale_factor;
    s16 field_68_motion_resource_block_index;
    s16 field_6A_padding;
    s32 field_6C_spotting_abe_timer;
    s32 field_70_attack_delay_timer;
    s32 field_74_movement_timer;
    s32 field_78_sfx_bitmask;
    s16 field_7C_prevent_depossession;
    LevelIds field_7E_return_level;
    s16 field_80_return_path;
    s16 field_82_return_camera;
    s16 field_84_input;
    s16 field_86_padding;
    s32 field_88_unused;
    s16 field_8C_shred_power_active;
    GameSpeakEvents field_8E_speak;
    FP field_90_max_xpos;
    FP field_94_max_ypos;
    s16 field_98_speak_counter;
    s16 field_9A_unused;
    s16 field_9C_unused;

    enum Flags_9E
    {
        eBit1_attacking = 0x1,
        eBit2_unused = 0x2,
        eBit3_unused = 0x4,
        eBit4_force_update_animation = 0x8,
        eBit5_roar_randomly = 0x10,
        eBit6_persistant = 0x20,
    };
    BitField16<Flags_9E> field_9E_flags;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(ScrabSaveState, 0xA0);

class Scrab;
using TScrabBrainFn = s16 (Scrab::*)();
using TScrabMotionFn = void (Scrab::*)();

class Scrab final : public BaseAliveGameObject
{
public:
    Scrab(relive::Path_Scrab* pTlv, const Guid& tlvId, relive::Path_ScrabSpawner::SpawnDirection spawnDirection);
    ~Scrab();

    void LoadAnimations();
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VPossessed() override;
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual void VOnTrapDoorOpen() override;

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    void HandleDDCheat();
    void vUpdateAnim();
    s16 OnFloor();
    void Update_Slurg_Step_Watch_Points();

public:
    s16 Brain_0_Patrol();

    s16 Brain_ChasingEnemy_State_2_Running(BaseAliveGameObject* pObj);

    s16 Brain_1_ChasingEnemy();
    s16 Brain_2_Fighting();
    s16 Brain_3_Death();
    s16 Brain_4_ShrinkDeath();
    s16 Brain_5_Possessed();

    void SetBrain(TScrabBrainFn fn);
    bool BrainIs(TScrabBrainFn fn);

public:
    void Motion_0_Stand();
    void Motion_1_Walk();
    void Motion_2_Run();
    void Motion_3_Turn();
    void Motion_4_RunToStand();
    void Motion_5_HopBegin();
    void Motion_6_HopMidair();
    void Motion_7_HopLand();
    void Motion_8_JumpToFall();
    void Motion_9_StandToWalk();
    void Motion_10_StandToRun();
    void Motion_11_WalkToStand();
    void Motion_12_RunJumpBegin();
    void Motion_13_RunJumpEnd();
    void Motion_14_WalkToFall();
    void Motion_15_RunToFall();
    void Motion_16_WalkToRun();
    void Motion_17_RunToWalk();
    void Motion_18_Knockback();
    void Motion_19_GetEaten();
    void Motion_20_Fall();
    void Motion_21_Stamp();
    void Motion_22_GetPossessed();
    void Motion_23_Empty();
    void Motion_24_DeathEnd();
    void Motion_25_Empty();
    void Motion_26_HowlBegin();
    void Motion_27_HowlEnd();
    void Motion_28_GetDepossessedBegin();
    void Motion_29_GetDepossessedEnd();
    void Motion_30_Shriek();
    void Motion_31_ScrabBattleAnim();
    void Motion_32_AttackSpin();
    void Motion_33_FeedToGulp();
    void Motion_34_GulpToStand();
    void Motion_35_StandToFeed();
    void Motion_36_Feed();
    void Motion_37_AttackLunge();
    void Motion_38_LegKick();
    void Motion_39_DeathBegin();


private:
    void ToPatrol();
    void ToStand();
    void MoveOnLine();
    void PlatformCollide();
    BaseAliveGameObject* Find_Fleech();
    u8** ResBlockForMotion(s16 motion);
    void TryMoveOrStand();
    s16 ToNextMotion();
    s16 PlayerControlled();
    void ToJump();
    void KnockBack();
    s32 Scrab_SFX(ScrabSounds soundId, s32 vol, s32 pitch, s16 applyDirection);
    void KillTarget(BaseAliveGameObject* pTarget);
    s16 FindAbeOrMud();
    s16 CanSeeAbe(BaseAliveGameObject* pObj);
    static bool LineOfSightTo(Scrab* pThis, BaseAliveGameObject* pObj);
    Scrab* FindScrabToFight();
    s16 Handle_SlamDoor_or_EnemyStopper(FP velX, s16 bCheckLeftRightBounds);
    GameSpeakEvents LastSpeak();

private:
    TScrabBrainFn mBrainState = nullptr;
    s16 mBrainSubState = 0;
    s16 field_11E_return_to_previous_motion = 0;
    Guid mTargetGuid;
    Guid mFightTargetId;
    s16 mAttackDelay = 0;
    s16 mPatrolTypeRunOrWalkChance = 0;
    s32 field_12C_timer = 0;
    s32 field_130_depossession_timer = 0;
    FP field_134_falling_velx_scale_factor = {};
    FP field_13C_last_ypos = {};
    s16 field_140_motion_resource_block_index = 0;
    Guid field_144_tlvInfo;
    s32 mPauseAfterChaseTime = 0;
    s32 field_14C_pause_after_chase_timer = 0;
    s32 field_150_attack_delay_timer = 0;
    s32 field_154_movement_timer = 0;
    s16 mPauseLeftMin = 0;
    s16 mPauseLeftMax = 0;
    s16 mPauseRightMin = 0;
    s16 mPauseRightMax = 0;
    s32 field_160_sfx_bitmask = 0;
    s16 mPreventDepossession = 0;
    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;
    s32 field_16C_input = 0;
    s16 mPossessedMaxWhirlAttackDuration = 0;
    s16 mShredPowerActive = 0;
    s32 field_17C_last_event = 0;
    GameSpeakEvents field_194_speak = GameSpeakEvents::eUnknown_0;
    FP field_198_max_xpos = {};
    FP field_19C_max_ypos = {};
    s16 field_1A0_speak_max = 0;
    s16 field_1A2_speak_counter = 0;
    Choice_short mKillEnemy = Choice_short::eNo_0;

    enum Flags_1AA : s16
    {
        eBit1_attacking = 0x1,
        eBit2_unused = 0x2,
        eBit3_unused = 0x4,
        eBit4_force_update_animation = 0x8,
        eBit5_roar_randomly = 0x10,
        eBit6_persistant = 0x20,
        eBit7_padding = 0x40,
        eBit8_padding = 0x80,
        eBit9_padding = 0x100,
        eBit10_padding = 0x200,
        eBit11_padding = 0x400,
        eBit12_padding = 0x800,
        eBit13_padding = 0x1000,
        eBit14_padding = 0x2000,
        eBit15_padding = 0x4000,
    };
    BitField16<Flags_1AA> field_1AA_flags = {};
};
