#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "GameSpeak.hpp"

#define SCRAB_MOTIONS_ENUM_AE(ENTRY)          \
    ENTRY(M_Stand_0_4A8220)                \
    ENTRY(M_Walk_1_4A84D0)                 \
    ENTRY(M_Run_2_4A89C0)                  \
    ENTRY(M_Turn_3_4A91A0)                 \
    ENTRY(M_RunToStand_4_4A90C0)           \
    ENTRY(M_HopBegin_5_4A96C0)             \
    ENTRY(M_HopMidair_6_4A9490)            \
    ENTRY(M_HopLand_7_4A9890)              \
    ENTRY(M_JumpToFall_8_4A9220)           \
    ENTRY(M_StandToWalk_9_4A8450)          \
    ENTRY(M_StandToRun_10_4A8900)          \
    ENTRY(M_WalkToStand_11_4A8880)         \
    ENTRY(M_RunJumpBegin_12_4A99C0)        \
    ENTRY(M_RunJumpEnd_13_4A9BE0)          \
    ENTRY(M_WalkToFall_14_4A9460)          \
    ENTRY(M_RunToFall_15_4A9430)           \
    ENTRY(M_WalkToRun_16_4A8D60)           \
    ENTRY(M_RunToWalk_17_4A8D90)           \
    ENTRY(M_Knockback_18_4AA490)           \
    ENTRY(M_GetEaten_19_4AA3E0)            \
    ENTRY(M_Fall_20_4A93E0)                \
    ENTRY(M_Stamp_21_4A9CC0)               \
    ENTRY(M_GetPossessed_22_4AA420)        \
    ENTRY(M_Empty_23_4A9D80)               \
    ENTRY(M_DeathEnd_24_4AA140)            \
    ENTRY(M_Empty_25_4A34D0)               \
    ENTRY(M_HowlBegin_26_4A9DA0)           \
    ENTRY(M_HowlEnd_27_4A9E60)             \
    ENTRY(M_GetDepossessedBegin_28_4AA200) \
    ENTRY(M_GetDepossessedEnd_29_4AA3C0)   \
    ENTRY(M_Shriek_30_4A9EA0)              \
    ENTRY(M_ScrabBattleAnim_31_4A9F30)     \
    ENTRY(M_AttackSpin_32_4A8DC0)          \
    ENTRY(M_FeedToGulp_33_4A9FA0)          \
    ENTRY(M_GulpToStand_34_4A9FF0)         \
    ENTRY(M_StandToFeed_35_4AA010)         \
    ENTRY(M_Feed_36_4AA030)                \
    ENTRY(M_AttackLunge_37_4AA0B0)         \
    ENTRY(M_LegKick_38_4AA120)             \
    ENTRY(M_DeathBegin_39_4AA190)

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

enum class ScrabSpawnDirection : s16
{
    eNone_0 = 0,
    eLeft_1 = 1,
    eRight_2 = 2,
};

struct Path_ScrabBoundLeft final : public Path_TLV
{
    // Empty
};

struct Path_ScrabBoundRight final : public Path_TLV
{
    // Empty
};

struct Path_Scrab : public Path_TLV
{
    Scale_short mScale;
    s16 mAttackDelay;
    s16 mPatrolTypeRunOrWalkChance;
    s16 mPauseLeftMin;
    s16 mPauseLeftMax;
    s16 mPauseRightMin;
    s16 mPauseRightMax;
    u16 mPauseAfterChaseTime;
    s16 mDisabledResources;
    Choice_short mRoarRandomly;
    Choice_short mPersistant;
    s16 mPossessedMaxWhirlAttackDuration;
    s16 field_28_unused;
    Choice_short mKillEnemy;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Scrab, 0x2C);

enum class LevelIds : s16;
enum class GameSpeakEvents : s16;

struct Scrab_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_obj_id;
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
    s32 field_44_tlvInfo;
    s32 field_48_brain_idx;
    s16 field_4C_padding;
    s16 field_4E_padding;
    s16 field_50_sub_state;
    s16 field_52_padding;
    s32 field_54_obj_id;
    s32 field_58_target_obj_id;
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
ALIVE_ASSERT_SIZEOF_ALWAYS(Scrab_State, 0xA0);

class Scrab;
using TScrabBrainFn = s16 (Scrab::*)();
using TScrabMotionFn = void (Scrab::*)();

class Scrab final : public BaseAliveGameObject
{
public:
    Scrab(relive::Path_Scrab* pTlv, s32 tlvInfo, ScrabSpawnDirection spawnDirection);
    ~Scrab();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VPossessed() override;
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(Path_TLV* pTlv) override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther) override;
    virtual void VOnTrapDoorOpen() override;

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    void vUpdateAnim();
    s16 OnFloor();
    void Update_Slurg_Step_Watch_Points();

public:
    s16 Brain_0_Patrol_4AA630();

    s16 Brain_ChasingEnemy_State_2_Running(BaseAliveGameObject* pObj);

    s16 Brain_1_ChasingEnemy_4A6470();
    s16 Brain_2_Fighting_4A5840();
    s16 Brain_3_Death_4A62B0();
    s16 Brain_4_ShrinkDeath_4A6420();
    s16 Brain_5_Possessed_4A6180();

    void SetBrain(TScrabBrainFn fn);
    bool BrainIs(TScrabBrainFn fn);

public:
    void M_Stand_0_4A8220();
    void M_Walk_1_4A84D0();
    void M_Run_2_4A89C0();
    void M_Turn_3_4A91A0();
    void M_RunToStand_4_4A90C0();
    void M_HopBegin_5_4A96C0();
    void M_HopMidair_6_4A9490();
    void M_HopLand_7_4A9890();
    void M_JumpToFall_8_4A9220();
    void M_StandToWalk_9_4A8450();
    void M_StandToRun_10_4A8900();
    void M_WalkToStand_11_4A8880();
    void M_RunJumpBegin_12_4A99C0();
    void M_RunJumpEnd_13_4A9BE0();
    void M_WalkToFall_14_4A9460();
    void M_RunToFall_15_4A9430();
    void M_WalkToRun_16_4A8D60();
    void M_RunToWalk_17_4A8D90();
    void M_Knockback_18_4AA490();
    void M_GetEaten_19_4AA3E0();
    void M_Fall_20_4A93E0();
    void M_Stamp_21_4A9CC0();
    void M_GetPossessed_22_4AA420();
    void M_Empty_23_4A9D80();
    void M_DeathEnd_24_4AA140();
    void M_Empty_25_4A34D0();
    void M_HowlBegin_26_4A9DA0();
    void M_HowlEnd_27_4A9E60();
    void M_GetDepossessedBegin_28_4AA200();
    void M_GetDepossessedEnd_29_4AA3C0();
    void M_Shriek_30_4A9EA0();
    void M_ScrabBattleAnim_31_4A9F30();
    void M_AttackSpin_32_4A8DC0();
    void M_FeedToGulp_33_4A9FA0();
    void M_GulpToStand_34_4A9FF0();
    void M_StandToFeed_35_4AA010();
    void M_Feed_36_4AA030();
    void M_AttackLunge_37_4AA0B0();
    void M_LegKick_38_4AA120();
    void M_DeathBegin_39_4AA190();


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
    TScrabBrainFn field_118_brain_state = nullptr;
    s16 field_11C_brain_sub_state = 0;
    s16 field_11E_return_to_previous_motion = 0;
    s32 field_120_obj_id = 0;
    s32 field_124_fight_target_obj_id = 0;
    s16 field_128_attack_delay = 0;
    s16 field_12A_patrol_type_run_or_walk_chance = 0;
    s32 field_12C_timer = 0;
    s32 field_130_depossession_timer = 0;
    FP field_134_falling_velx_scale_factor = {};
    FP field_138_unused = {};
    FP field_13C_last_ypos = {};
    s16 field_140_motion_resource_block_index = 0;
    s32 field_144_tlvInfo = 0;
    s32 field_148_pause_after_chase_delay = 0;
    s32 field_14C_pause_after_chase_timer = 0;
    s32 field_150_attack_delay_timer = 0;
    s32 field_154_movement_timer = 0;
    s16 field_158_left_min_delay = 0;
    s16 field_15A_left_max_delay = 0;
    s16 field_15C_right_min_delay = 0;
    s16 field_15E_right_max_delay = 0;
    s32 field_160_sfx_bitmask = 0;
    s16 field_164_prevent_depossession = 0;
    EReliveLevelIds field_166_return_level = EReliveLevelIds::eNone;
    s16 field_168_return_path = 0;
    s16 field_16A_return_camera = 0;
    s32 field_16C_input = 0;
    s32 field_170_unused = 0;
    s16 field_174_possessed_max_whirl_attack_duration = 0;
    s16 field_176_unused = 0;
    s16 field_178_shred_power_active = 0;
    s32 field_17C_last_event = 0;
    s16 field_190_unused = 0;
    s16 field_192_unused = 0;
    GameSpeakEvents field_194_speak = GameSpeakEvents::eUnknown_0;
    FP field_198_max_xpos = {};
    FP field_19C_max_ypos = {};
    s16 field_1A0_speak_max = 0;
    s16 field_1A2_speak_counter = 0;
    s16 field_1A4_unused = 0;
    s16 field_1A6_unused = 0;
    Choice_short field_1A8_bKill_enemy = Choice_short::eNo_0;

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
//ALIVE_ASSERT_SIZEOF(Scrab, 0x1AC);
