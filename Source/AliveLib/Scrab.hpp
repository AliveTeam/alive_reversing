#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

#define SCRAB_MOTIONS_ENUM(ENTRY) \
    ENTRY(M_Stand_0_4A8220) \
    ENTRY(M_Walk_1_4A84D0) \
    ENTRY(M_Run_2_4A89C0) \
    ENTRY(M_Turn_3_4A91A0) \
    ENTRY(M_RunToStand_4_4A90C0) \
    ENTRY(M_HopBegin_5_4A96C0) \
    ENTRY(M_HopMidair_6_4A9490) \
    ENTRY(M_HopLand_7_4A9890) \
    ENTRY(M_JumpToFall_8_4A9220) \
    ENTRY(M_StandToWalk_9_4A8450) \
    ENTRY(M_StandToRun_10_4A8900) \
    ENTRY(M_WalkToStand_11_4A8880) \
    ENTRY(M_RunJumpBegin_12_4A99C0) \
    ENTRY(M_RunJumpEnd_13_4A9BE0) \
    ENTRY(M_WalkToFall_14_4A9460) \
    ENTRY(M_RunToFall_15_4A9430) \
    ENTRY(M_WalkToRun_16_4A8D60) \
    ENTRY(M_RunToWalk_17_4A8D90) \
    ENTRY(M_Knockback_18_4AA490) \
    ENTRY(M_GetEaten_19_4AA3E0) \
    ENTRY(M_Fall_20_4A93E0) \
    ENTRY(M_Stamp_21_4A9CC0) \
    ENTRY(M_GetPossessed_22_4AA420) \
    ENTRY(M_Empty_23_4A9D80) \
    ENTRY(M_DeathEnd_24_4AA140) \
    ENTRY(M_Empty_25_4A34D0) \
    ENTRY(M_HowlBegin_26_4A9DA0) \
    ENTRY(M_HowlEnd_27_4A9E60) \
    ENTRY(M_GetDepossessedBegin_28_4AA200) \
    ENTRY(M_GetDepossessedEnd_29_4AA3C0) \
    ENTRY(M_Shriek_30_4A9EA0) \
    ENTRY(M_ScrabBattleAnim_31_4A9F30) \
    ENTRY(M_AttackSpin_32_4A8DC0) \
    ENTRY(M_FeedToGulp_33_4A9FA0) \
    ENTRY(M_GulpToStand_34_4A9FF0) \
    ENTRY(M_StandToFeed_35_4AA010) \
    ENTRY(M_Feed_36_4AA030) \
    ENTRY(M_AttackLunge_37_4AA0B0) \
    ENTRY(M_LegKick_38_4AA120) \
    ENTRY(M_DeathBegin_39_4AA190)

#define MAKE_ENUM(VAR) VAR,
enum eScrabMotions : int
{
    SCRAB_MOTIONS_ENUM(MAKE_ENUM)
};

enum class ScrabSound : unsigned __int8
{
    Howl_0 = 0,
    DeathHowl_1 = 1,
    Empty_2 = 2,
    Empty_3 = 3,
    HitCollision_4 = 4,
    Shredding_5 = 5,
    Walk1_6 = 6,
    Walk2_7 = 7,
    Yell_8 = 8,
};

struct Path_Scrab : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_attack_delay;
    __int16 field_14_patrol_type;
    __int16 field_16_left_min_delay;
    __int16 field_18_left_max_delay;
    __int16 field_1A_right_min_delay;
    __int16 field_1C_right_max_delay;
    unsigned __int16 field_1E_attack_duration;
    __int16 field_20_disabled_resources;
    __int16 field_22_roar_randomly;
    __int16 field_24_persistant;
    __int16 field_26_whirl_attack_duration;
    __int16 field_28_whirl_attack_recharge;
    __int16 field_2A_kill_close_fleech;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Scrab, 0x2C);

enum class LevelIds : __int16;
enum class GameSpeakEvents : __int16;

struct Scrab_State
{
    Types field_0_type;
    __int16 field_2;
    int field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    __int16 field_18_path_number;
    LevelIds field_1A_lvl_number;
    FP field_1C_sprite_scale;
    __int16 field_20_r;
    __int16 field_22_g;
    __int16 field_24_b;
    __int16 field_26_bAnimFlipX;
    __int16 field_28_current_motion;
    __int16 field_2A_current_frame;
    __int16 field_2C_frame_change_counter;
    char field_2E_bAnimRender;
    char field_2F_bDrawable;
    FP field_30_health;
    __int16 field_34_current_motion;
    __int16 field_36_next_motion;
    __int16 field_38;
    __int16 field_3A_line_type;
    __int16 field_3C;
    __int16 field_3E;
    char field_40_bIsControlled;
    char field_41;
    __int16 field_42;
    int field_44_tlvInfo;
    int field_48_ai_idx;
    __int16 field_4C;
    __int16 field_4E;
    __int16 field_50_sub_state;
    __int16 field_52;
    int field_54_obj_id;
    int field_58_target_obj_id;
    int field_5C;
    int field_60;
    FP field_64;
    __int16 field_68;
    __int16 field_6A;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    __int16 field_7C;
    LevelIds field_7E_level;
    __int16 field_80_path;
    __int16 field_82_camera;
    __int16 field_84_input;
    __int16 field_86;
    int field_88;
    __int16 field_8C;
    GameSpeakEvents field_8E;
    FP field_90;
    FP field_94;
    __int16 field_98;
    __int16 field_9A;
    __int16 field_9C;

    enum Flags_9E
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
    };
    BitField16<Flags_9E> field_9E;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Scrab_State, 0xA0);

class Scrab;
using TScrabAIFn = __int16 (Scrab::*)();
using TScrabMotionFn = void (Scrab::*)();

class Scrab : public BaseAliveGameObject
{
public:
    EXPORT Scrab* ctor_4A3C40(Path_Scrab* pTlv, int tlvInfo, __int16 spawnedScale);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_4A45B0(pOrderingTable);
    }

    virtual void VPossessed_408F70() override
    {
        vPossesed_4A5620();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_4A5560();
    }

    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override
    {
        return vTakeDamage_4A45E0(pFrom);
    }

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override
    {
        return vOn_TLV_Collision_4A4B80(pTlv);
    }

    virtual int VGetSaveState(BYTE* pSaveBuffer) override
    {
        return vGetSaveState_4AB020(reinterpret_cast<Scrab_State*>(pSaveBuffer));
    }

    virtual __int16 vOnSameYLevel_425520(BaseAnimatedWithPhysicsGameObject* pOther) override
    {
        return vOnSameYLevel_4A5400(pOther);
    }

    virtual void VOnTrapDoorOpen() override
    {
        vOnTrapDoorOpen_4A7ED0();
    }

    EXPORT static int CC CreateFromSaveState_4A70A0(const BYTE* pBuffer);

private:
    EXPORT int vGetSaveState_4AB020(Scrab_State* pState);



    EXPORT Scrab* vdtor_4A41B0(signed int flags);
    EXPORT void dtor_4A42B0();

    EXPORT void vOnTrapDoorOpen_4A7ED0();


    EXPORT void vUpdateAnim_4A34F0();
    EXPORT __int16 OnFloor_4A41E0();

    EXPORT void vUpdate_4A3530();

    EXPORT void Update_Slurg_Step_Watch_Points_4A5780();

public:

    EXPORT __int16 AI_Patrol_0_4AA630();
    
    __int16 AI_ChasingEnemy_State_2(BaseAliveGameObject* pObj);

    EXPORT __int16 AI_ChasingEnemy_1_4A6470();
    EXPORT __int16 AI_Fighting_2_4A5840();
    EXPORT __int16 AI_Death_3_4A62B0();
    EXPORT __int16 AI_ShrinkDeath_4_4A6420();
    EXPORT __int16 AI_Possessed_5_4A6180();

    void SetBrain(TScrabAIFn fn);
    bool BrainIs(TScrabAIFn fn);
public:
    EXPORT void M_Stand_0_4A8220();
    EXPORT void M_Walk_1_4A84D0();
    EXPORT void M_Run_2_4A89C0();
    EXPORT void M_Turn_3_4A91A0();
    EXPORT void M_RunToStand_4_4A90C0();
    EXPORT void M_HopBegin_5_4A96C0();
    EXPORT void M_HopMidair_6_4A9490();
    EXPORT void M_HopLand_7_4A9890();
    EXPORT void M_JumpToFall_8_4A9220();
    EXPORT void M_StandToWalk_9_4A8450();
    EXPORT void M_StandToRun_10_4A8900();
    EXPORT void M_WalkToStand_11_4A8880();
    EXPORT void M_RunJumpBegin_12_4A99C0();
    EXPORT void M_RunJumpEnd_13_4A9BE0();
    EXPORT void M_WalkToFall_14_4A9460();
    EXPORT void M_RunToFall_15_4A9430();
    EXPORT void M_WalkToRun_16_4A8D60();
    EXPORT void M_RunToWalk_17_4A8D90();
    EXPORT void M_Knockback_18_4AA490();
    EXPORT void M_GetEaten_19_4AA3E0();
    EXPORT void M_Fall_20_4A93E0();
    EXPORT void M_Stamp_21_4A9CC0();
    EXPORT void M_GetPossessed_22_4AA420();
    EXPORT void M_Empty_23_4A9D80();
    EXPORT void M_DeathEnd_24_4AA140();
    EXPORT void M_Empty_25_4A34D0();
    EXPORT void M_HowlBegin_26_4A9DA0();
    EXPORT void M_HowlEnd_27_4A9E60();
    EXPORT void M_GetDepossessedBegin_28_4AA200();
    EXPORT void M_GetDepossessedEnd_29_4AA3C0();
    EXPORT void M_Shriek_30_4A9EA0();
    EXPORT void M_ScrabBattleAnim_31_4A9F30();
    EXPORT void M_AttackSpin_32_4A8DC0();
    EXPORT void M_FeedToGulp_33_4A9FA0();
    EXPORT void M_GulpToStand_34_4A9FF0();
    EXPORT void M_StandToFeed_35_4AA010();
    EXPORT void M_Feed_36_4AA030();
    EXPORT void M_AttackLunge_37_4AA0B0();
    EXPORT void M_LegKick_38_4AA120();
    EXPORT void M_DeathBegin_39_4AA190();


private:


    EXPORT __int16 vOnSameYLevel_4A5400(BaseAnimatedWithPhysicsGameObject* pOther);

    EXPORT void ToPatrol_4AA600();

    EXPORT void ToStand_4A75A0();

    EXPORT void MoveOnLine_4A7D20();

    EXPORT void PlatformCollide_4A7E50();

    EXPORT BaseAliveGameObject* Find_Fleech_4A4C90();

    EXPORT void vPossesed_4A5620();

    EXPORT BYTE** ResBlockForMotion_4A43E0(__int16 motion);

    EXPORT void vScreenChanged_4A5560();

    EXPORT void vRender_4A45B0(int** ot);

    EXPORT void TryMoveOrStand_4A7570();

    EXPORT __int16 ToNextMotion_4A7920();

    EXPORT __int16 PlayerControlled_4A76A0();

    EXPORT void ToJump_4A75E0();

    EXPORT __int16 vTakeDamage_4A45E0(BaseGameObject* pFrom);

    EXPORT void vOn_TLV_Collision_4A4B80(Path_TLV* pTlv);

    EXPORT void KnockBack_4AA530();

    EXPORT int Sound_4AADB0(ScrabSound soundId, int vol, int pitch, __int16 applyDirection);

    EXPORT void KillTarget_4A7F20(BaseAliveGameObject* pTarget);

    EXPORT __int16 FindAbeOrMud_4A4FD0();

    EXPORT __int16 CanSeeAbe_4A51A0(BaseAliveGameObject* pObj);

    EXPORT static BOOL CCSTD LineOfSightTo_4A52D0(Scrab* pThis, BaseAliveGameObject* pObj);

    EXPORT Scrab* FindScrabToFight_4A4E20();
  
    EXPORT __int16 Handle_SlamDoor_or_EnemyStopper_4A4830(FP velX, __int16 bCheckLeftRightBounds);

    EXPORT GameSpeakEvents LastSpeak_4A56F0();

private:
    TScrabAIFn field_118_brain_state;
    __int16 field_11C_sub_state;
    __int16 field_11E;
    int field_120_obj_id;
    int field_124_fight_target_obj_id;
    __int16 field_128_attack_delay;
    __int16 field_12A_patrol_type;
    int field_12C_timer;
    int field_130;
    FP field_134;
    FP field_138;
    FP field_13C_last_ypos;
    __int16 field_140;
    __int16 field_142;
    int field_144_tlvInfo;
    int field_148_attack_duration;
    int field_14C;
    int field_150;
    int field_154;
    __int16 field_158_left_min_delay;
    __int16 field_15A_left_max_delay;
    __int16 field_15C;
    __int16 field_15E_right_max_delay;
    int field_160;
    __int16 field_164;
    LevelIds field_166_level;
    __int16 field_168_path;
    __int16 field_16A_camera;
    int field_16C;
    int field_170;
    __int16 field_174_whirl_attack_duration;
    __int16 field_176_whirl_attack_recharge;
    __int16 field_178;
    __int16 field_17A;
    int field_17C_last_event;
    __int16 field_180;
    __int16 field_182;
    __int16 field_184;
    __int16 field_186;
    __int16 field_188;
    __int16 field_18A;
    __int16 field_18C;
    __int16 field_18E;
    __int16 field_190;
    __int16 field_192;
    GameSpeakEvents field_194_speak;
    __int16 field_196;
    FP field_198;
    FP field_19C;
    __int16 field_1A0;
    __int16 field_1A2;
    __int16 field_1A4;
    __int16 field_1A6;
    __int16 field_1A8_kill_close_fleech;

    enum Flags_1AA : __int16
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5_roar_randomly = 0x10,
        eBit6_persistant = 0x20,
        eBit7 = 0x40,
        eBit8 = 0x80,
        eBit9 = 0x100,
        eBit10 = 0x200,
        eBit11 = 0x400,
        eBit12 = 0x800,
        eBit13 = 0x1000,
        eBit14 = 0x2000,
        eBit15 = 0x4000,
    };
    BitField16<Flags_1AA> field_1AA_flags;
};
ALIVE_ASSERT_SIZEOF(Scrab, 0x1AC);
