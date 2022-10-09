#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

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

enum class LevelIds : s16;

class Glukkon;
using TGlukkonBrainFn = s16 (Glukkon::*)();
using TGlukkonMotionFn = void (Glukkon::*)();

struct Glukkon_SaveState final
{
    AETypes field_0_id;
    s16 field_2_padding;
    Guid field_4_object_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_xvel;
    FP field_14_yvel;
    s16 field_18_path;
    LevelIds field_1A_level;
    FP field_1C_sprite_scale;
    u16 mRingRed;
    u16 mRingGreen;
    u16 mRingBlue;
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
    Guid field_44_tlvInfo;
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
    Guid field_88_obj_id;
    s16 field_8C_can_be_possessed;
    AETypes field_8E_type_id;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(Glukkon_SaveState, 144);

class Glukkon final : public BaseAliveGameObject
{
public:
    Glukkon(relive::Path_Glukkon* pTlv, const Guid& tlvId);
    ~Glukkon();

    void LoadAnimations();

    static s32 CreateFromSaveState(const u8* pBuffer);

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VScreenChanged() override;
    virtual void VPossessed() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual void VOnTrapDoorOpen() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

public:
    void M_Idle_0_442D10();
    void M_Walk_1_442D30();
    void M_Turn_2_442F10();
    void M_KnockBack_3_442F40();
    void M_Jump_4_443030();

    void JumpHelper();

    void M_JumpToFall_5_4434C0();
    void M_WalkToFall_6_4434E0();
    void M_Fall_7_443510();
    void M_DeathFall_8_443760();
    void M_Land_9_443790();
    void M_ChantShake_10_443B50();
    void M_Speak1_11_4437D0();
    void M_Speak2_12_4438F0();
    void M_LongLaugh_13_443930();
    void M_BeginWalk_14_443950();
    void M_EndWalk_15_443970();
    void M_StandToJump_16_4439B0();
    void M_JumpToStand_17_4439D0();
    void M_WalkToJump_18_443A00();
    void M_JumpToWalk_19_443A30();
    void M_KnockBackStandBegin_20_442FC0();
    void M_GetShot_21_443A60();
    void M_KnockBackStandEnd_22_443010();
    void M_Speak3_23_443910();
    void M_EndSingleStep_24_443990();

public:
    s16 Brain_0_Calm_WalkAround_440B40();
    s16 Brain_1_Panic_4412F0();
    s16 Brain_2_Slapped_441720();
    s16 Brain_3_PlayerControlled_441A30();
    s16 Brain_4_Death_442010();
    s16 Brain_5_WaitToSpawn_442490();

public:
    static void PlaySound_GameSpeak(GlukkonSpeak sndIdx, s16 volume, s16 pitch, Glukkon* pGlukkon);

private:
    void Init();
    void Update_Slurg_WatchPoints();
    void SetAnim(s16 currentMotion, s16 bClearNextMotion = FALSE);
    void Speak(GlukkonSpeak speak);
    void HandleInput();
    s16 ShouldPanic(s16 panicEvenIfNotFacingMe);
    s16 PathBlocked(FP a2, s16 checkBounds);
    void SpeakRandomish();
    s16 DoMovement();
    void FollowLine();
    void GetOnPlatforms();
    static void PlaySound(s32 sndIdx, Glukkon* pGlukkon);
    void ToDead();
    static bool IsLineOfSightBetween(Glukkon* pGlukkon, BaseAliveGameObject* pOther);
    void ToStand();
    void SlowDown(FP speed);
    void SetBrain(TGlukkonBrainFn fn);
    bool BrainIs(TGlukkonBrainFn fn);

private:
    s16 field_118_pPalAlloc[64] = {};
    s16 field_1A0_red = 0;
    s16 field_1A2_green = 0;
    s16 field_1A4_blue = 0;
    relive::Path_Glukkon field_1A8_tlvData;
    s32 field_1D4_timer = 0;
    FP field_1D8_falling_velx_scale_factor = {};
    FP field_1DC_previous_ypos = {};
    s16 field_1E0_gamespeak_pitch = 0;
    s16 mPreventDepossession = 0;
    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;
    GlukkonSpeak field_1EA_speak = GlukkonSpeak::Hey_0;
    s32 field_1EC_unused = 0;
    s32 field_1F0_randomish_speak_timer = 0;
    s32 field_1F4_turn_or_help_timer = 0;
    s32 field_1F8_panic_timer = 0;
    s16 field_1FC = 0; // When set to true, broadcasts event kEventGlukkonUnknown.
    s32 field_200_knockback_delay_after_getting_shot_timer = 0;
    s32 field_204_getting_shot_timer = 0;
    Guid field_208_obj_id;
    TGlukkonBrainFn mBrainState = nullptr;
    s16 mBrainSubState = 0;
    s16 field_212_currentWalkPitch = 0;
    Guid field_214_tlv_info;
};
ALIVE_ASSERT_SIZEOF(Glukkon, 0x218);
