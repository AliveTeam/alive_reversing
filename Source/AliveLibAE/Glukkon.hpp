#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Path.hpp"
#include "BaseAliveGameObject.hpp"

#define GLUKKON_MOTIONS_ENUM(ENTRY)        \
    ENTRY(M_Idle_0_442D10)                 \
    ENTRY(M_Walk_1_442D30)                 \
    ENTRY(M_Turn_2_442F10)                 \
    ENTRY(M_KnockBack_3_442F40)            \
    ENTRY(M_Jump_4_443030)                 \
    ENTRY(M_JumpToFall_5_4434C0)           \
    ENTRY(M_WalkToFall_6_4434E0)           \
    ENTRY(M_Fall_7_443510)                 \
    ENTRY(M_DeathFall_8_443760)            \
    ENTRY(M_Land_9_443790)                 \
    ENTRY(M_ChantShake_10_443B50)          \
    ENTRY(M_Speak1_11_4437D0)              \
    ENTRY(M_Speak2_12_4438F0)              \
    ENTRY(M_LongLaugh_13_443930)           \
    ENTRY(M_BeginWalk_14_443950)           \
    ENTRY(M_EndWalk_15_443970)             \
    ENTRY(M_StandToJump_16_4439B0)         \
    ENTRY(M_JumpToStand_17_4439D0)         \
    ENTRY(M_WalkToJump_18_443A00)          \
    ENTRY(M_JumpToWalk_19_443A30)          \
    ENTRY(M_KnockBackStandBegin_20_442FC0) \
    ENTRY(M_GetShot_21_443A60)             \
    ENTRY(M_KnockBackStandEnd_22_443010)   \
    ENTRY(M_Speak3_23_443910)              \
    ENTRY(M_EndSingleStep_24_443990)

#define MAKE_ENUM(VAR) VAR,
enum eGlukkonMotions : s32
{
    GLUKKON_MOTIONS_ENUM(MAKE_ENUM)
};

enum class GlukkonTypes : s16
{
    Normal_0 = 0,
    StoryAslik_1 = 1,
    StoryDripik_2 = 2,
    StoryPhleg_3 = 3,
    Normal_4 = 4,
    Normal_5 = 5,
};

enum class GlukkonSpeak : s8
{
    None = -1,
    Hey_0 = 0,
    DoIt_1 = 1,
    StayHere_2 = 2,
    Commere_3 = 3,
    AllOYa_4 = 4,
    Heh_5 = 5,
    Help_6 = 6,
    Laugh_7 = 7,
    KillEm_8 = 8,
    Unused_9 = 9,
    Unused_10 = 10,
    What_11 = 11,
    Unused_12 = 12,
    Unused_13 = 13,
    Unused_14 = 14
};

struct Path_Glukkon final : public Path_TLV
{
    Scale_short field_10_scale;
    enum class StartDirection : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1,
    };
    StartDirection field_12_start_direction;
    enum class Behavior : s16
    {
        eIgnoreWalls_0 = 0,
        eCheckForWalls_1 = 1
    };
    Behavior field_14_behaviour;
    s16 field_16_scream_help_delay;
    s16 field_18_help_switch_id;
    s16 field_1A_to_calm_delay;
    s16 field_1C_spawn_switch_id;
    enum class SpawnType : s16
    {
        eRegularSpawn_0 = 0,
        eFacingLeft_1 = 1,
        eFacingRight_2 = 2,
        eFullSpawnEffects_3 = 3
    };
    SpawnType field_1E_spawn_type;
    s16 field_20_spawn_delay;
    GlukkonTypes field_22_glukkon_type;
    s16 field_24_death_switch_id;
    s16 field_26_play_movie_switch_id;
    s16 field_28_movie_to_play_fmvID;
    s16 field_2A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Glukkon, 0x2C);

enum class LevelIds : s16;

class Glukkon;
using TGlukkonBrainFn = s16 (Glukkon::*)();
using TGlukkonMotionFn = void (Glukkon::*)();

struct Glukkon_SaveState final
{
    AETypes field_0_id;
    s16 field_2_padding;
    s32 field_4_object_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_xvel;
    FP field_14_yvel;
    s16 field_18_path;
    LevelIds field_1A_level;
    FP field_1C_sprite_scale;
    u16 field_20_r;
    u16 field_22_g;
    u16 field_24_b;
    u16 field_26_flipX;
    u16 field_28_current_motion;
    u16 field_2A_current_frame;
    u16 field_2C_frame_change_counter;
    u8 field_2E_render;
    u8 field_2F_drawable;
    FP field_30_health;
    u16 field_34_current_motion;
    u16 field_36_next_motion;
    s16 field_38_last_line_ypos;
    s8 field_3A_line_type;
    u8 field_3B_padding;
    s32 field_3C_padding;
    u16 field_40_bIsActiveChar;
    s16 field_42_padding;
    s32 field_44_tlvInfo;
    s32 field_48_brain_state_idx;
    s32 field_4C_padding;
    s16 field_50_brain_sub_state;
    s16 field_52_padding;
    s32 field_54_timer;
    FP field_58_falling_velx_scale_factor;
    s16 field_5C_padding;
    s16 field_5E_prevent_depossession;
    LevelIds field_60_level;
    s16 field_62_path;
    s16 field_64_camera;
    GlukkonSpeak field_66_speak;
    s16 field_68_gamespeak_pitch;
    s16 field_6A_padding;
    FP field_6C_previous_ypos;
    s32 field_70_randomish_speak_timer;
    s32 field_74_turn_or_help_timer;
    s32 field_78_panic_timer;
    s16 field_7C;
    s16 field_7E_padding;
    s32 field_80_knockback_delay_after_getting_shot_timer;
    s32 field_84_getting_shot_timer;
    s32 field_88_obj_id;
    s16 field_8C_can_be_possessed;
    AETypes field_8E_type_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Glukkon_SaveState, 144);

class Glukkon final : public BaseAliveGameObject
{
public:
    EXPORT Glukkon* ctor_43F030(Path_Glukkon* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT static s32 CC CreateFromSaveState_442830(const u8* pBuffer);

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    virtual void VScreenChanged() override;

    virtual void VPossessed_408F70() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual void VOnTrapDoorOpen() override;

    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;

public:
    EXPORT void M_Idle_0_442D10();
    EXPORT void M_Walk_1_442D30();
    EXPORT void M_Turn_2_442F10();
    EXPORT void M_KnockBack_3_442F40();
    EXPORT void M_Jump_4_443030();

    void JumpHelper();

    EXPORT void M_JumpToFall_5_4434C0();
    EXPORT void M_WalkToFall_6_4434E0();
    EXPORT void M_Fall_7_443510();
    EXPORT void M_DeathFall_8_443760();
    EXPORT void M_Land_9_443790();
    EXPORT void M_ChantShake_10_443B50();
    EXPORT void M_Speak1_11_4437D0();
    EXPORT void M_Speak2_12_4438F0();
    EXPORT void M_LongLaugh_13_443930();
    EXPORT void M_BeginWalk_14_443950();
    EXPORT void M_EndWalk_15_443970();
    EXPORT void M_StandToJump_16_4439B0();
    EXPORT void M_JumpToStand_17_4439D0();
    EXPORT void M_WalkToJump_18_443A00();
    EXPORT void M_JumpToWalk_19_443A30();
    EXPORT void M_KnockBackStandBegin_20_442FC0();
    EXPORT void M_GetShot_21_443A60();
    EXPORT void M_KnockBackStandEnd_22_443010();
    EXPORT void M_Speak3_23_443910();
    EXPORT void M_EndSingleStep_24_443990();

public:
    EXPORT s16 Brain_0_Calm_WalkAround_440B40();
    EXPORT s16 Brain_1_Panic_4412F0();
    EXPORT s16 Brain_2_Slapped_441720();
    EXPORT s16 Brain_3_PlayerControlled_441A30();
    EXPORT s16 Brain_4_Death_442010();
    EXPORT s16 Brain_5_WaitToSpawn_442490();

public:
    EXPORT static void CC PlaySound_GameSpeak_444AF0(GlukkonSpeak sndIdx, s16 volume, s16 pitch, Glukkon* pGlukkon);

private:
    EXPORT void Init_43F260();

    EXPORT Glukkon* vdtor_43F230(s32 flags);

    EXPORT void dtor_43F570();

    EXPORT void vUpdate_43F770();

    EXPORT void vPossessed_440160();

    EXPORT void Update_Slurg_WatchPoints_440600();

    EXPORT void SetAnim_43F9C0(s16 currentMotion, s16 bClearNextMotion = FALSE);

    EXPORT void Speak_444640(GlukkonSpeak speak);

    EXPORT void HandleInput_443BB0();

    EXPORT s16 ShouldPanic_440200(s16 panicEvenIfNotFacingMe);

    EXPORT s16 PathBlocked_4442F0(FP a2, s16 checkBounds);

    EXPORT void SpeakRandomish_4405D0();

    EXPORT void vRender_4406C0(PrimHeader** ot);

    EXPORT s16 DoMovement_444190();

    EXPORT void FollowLine_443EB0();

    EXPORT void GetOnPlatforms_444060();

    EXPORT static void CC PlaySound_4447D0(s32 sndIdx, Glukkon* pGlukkon);

    EXPORT void ToDead_43F640();

    EXPORT void vOn_TLV_Collision_4404A0(Path_TLV* pTlv);

    EXPORT static Bool32 CCSTD IsLineOfSightBetween_4403B0(Glukkon* pGlukkon, BaseAliveGameObject* pOther);

    EXPORT void ToStand_443B70();

    EXPORT void SlowDown_444700(FP speed);

    EXPORT void vScreenChanged_440110();

    EXPORT s32 vGetSaveState_444B90(u8* pSaveBuffer);

    EXPORT void vOnTrapDoorOpen_444120();

    EXPORT s16 vTakeDamage_43FA40(BaseGameObject* pFrom);


    void SetBrain(TGlukkonBrainFn fn);
    bool BrainIs(TGlukkonBrainFn fn);

private:
    s16 field_118_pPalAlloc[64];
    PSX_RECT field_198_pal_rect;
    s16 field_1A0_red;
    s16 field_1A2_green;
    s16 field_1A4_blue;
    s16 field_1A6_padding;
    Path_Glukkon field_1A8_tlvData;
    s32 field_1D4_timer;
    FP field_1D8_falling_velx_scale_factor;
    FP field_1DC_previous_ypos;
    s16 field_1E0_gamespeak_pitch;
    s16 field_1E2_prevent_depossession;
    LevelIds field_1E4_level;
    s16 field_1E6_path;
    s16 field_1E8_camera;
    GlukkonSpeak field_1EA_speak;
    s8 field_1EB_padding;
    s32 field_1EC_unused;
    s32 field_1F0_randomish_speak_timer;
    s32 field_1F4_turn_or_help_timer;
    s32 field_1F8_panic_timer;
    s16 field_1FC; // When set to true, broadcasts event kEventUnknown17.
    s16 field_1FE_padding;
    s32 field_200_knockback_delay_after_getting_shot_timer;
    s32 field_204_getting_shot_timer;
    s32 field_208_obj_id;
    TGlukkonBrainFn field_20C_brain_state_fn;
    s16 field_210_brain_sub_state;
    s16 field_212_currentWalkPitch;
    s32 field_214_tlv_info;
};
ALIVE_ASSERT_SIZEOF(Glukkon, 0x218);
