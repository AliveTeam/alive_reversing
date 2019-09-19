#pragma once

#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "BaseAliveGameObject.hpp"

#define GLUKKON_MOTIONS_ENUM(ENTRY) \
    ENTRY(M_Idle_0_442D10) \
    ENTRY(M_Walk_1_442D30) \
    ENTRY(M_Turn_2_442F10) \
    ENTRY(M_KnockBack_3_442F40) \
    ENTRY(M_Jump_4_443030) \
    ENTRY(M_JumpToFall_5_4434C0) \
    ENTRY(M_WalkToFall_6_4434E0) \
    ENTRY(M_Fall_7_443510) \
    ENTRY(M_DeathFall_8_443760) \
    ENTRY(M_Land_9_443790) \
    ENTRY(M_ChantShake_10_443B50) \
    ENTRY(M_Speak1_11_4437D0) \
    ENTRY(M_Speak2_12_4438F0) \
    ENTRY(M_LongLaugh_13_443930) \
    ENTRY(M_BeginWalk_14_443950) \
    ENTRY(M_EndWalk_15_443970) \
    ENTRY(M_StandToJump_16_4439B0) \
    ENTRY(M_JumpToStand_17_4439D0) \
    ENTRY(M_WalkToJump_18_443A00) \
    ENTRY(M_JumpToWalk_19_443A30) \
    ENTRY(M_KnockBackStandBegin_20_442FC0) \
    ENTRY(M_GetShot_21_443A60) \
    ENTRY(M_KnockBackStandEnd_22_443010) \
    ENTRY(M_Speak3_23_443910) \
    ENTRY(M_EndSingleStep_24_443990)

#define MAKE_ENUM(VAR) VAR,
enum eGlukkonMotions : int
{
    GLUKKON_MOTIONS_ENUM(MAKE_ENUM)
};

enum class GlukkonTypes : __int16
{
    Normal_0 = 0,
    Aslik_1 = 1,
    Drpik_2 = 2,
    Phleg_3 = 3,
    Normal_4 = 4,
    Normal_5 = 5,
};

enum class GlukkonSpeak : char
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

struct Path_Glukkon : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_start_direction;
    __int16 field_14_default_behaviour;
    __int16 field_16_pre_alarmed_delay;
    __int16 field_18_switch_id;
    __int16 field_1A_post_alarm_delay;
    __int16 field_1C_spawn_id;
    __int16 field_1E_spawn_direction;
    __int16 field_20_spawn_delay;
    GlukkonTypes field_22_glukkon_type;
    __int16 field_24_start_gas_id;
    __int16 field_26_play_movie_id;
    __int16 field_28_movie_to_play_id;
    __int16 field_2A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Glukkon, 0x2C);

enum class LevelIds : __int16;

class Glukkon;
using TGlukkonAIFn = __int16 (Glukkon::*)();
using TGlukkonMotionFn = void (Glukkon::*)();

struct Glukkon_SaveState
{
    Types field_0_id;
    __int16 field_2_pad;
    __int32 field_4_object_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_xvel;
    FP field_14_yvel;
    __int16 field_18_path;
    LevelIds field_1A_level;
    FP field_1C_sprite_scale;
    WORD field_20_r;
    WORD field_22_g;
    WORD field_24_b;
    WORD field_26_flipX;
    WORD field_28_current_motion;
    WORD field_2A_current_frame;
    WORD field_2C_frame_change_counter;
    BYTE field_2E;
    BYTE field_2F;
    FP field_30_health;
    WORD field_34_current_motion;
    WORD field_36_next_motion;
    __int16 field_38;
    char field_3A_line_type;
    BYTE field_3B_pad;
    __int32 field_3C_pad;
    WORD field_40_bIsActiveChar;
    __int16 field_42_pad;
    __int32 field_44_tlvInfo;
    __int32 field_48_brain_state_idx;
    __int32 field_4C_pad;
    __int16 field_50;
    __int16 field_52_pad;
    __int32 field_54;
    FP field_58;
    __int16 field_5C_pad;
    __int16 field_5E;
    LevelIds field_60_level;
    __int16 field_62_path;
    __int16 field_64_camera;
    GlukkonSpeak field_66_speak;
    __int16 field_68;
    __int16 field_6A_pad;
    FP field_6C;
    __int32 field_70;
    __int32 field_74;
    __int32 field_78;
    __int16 field_7C;
    __int16 field_7E_pad;
    __int32 field_80;
    __int32 field_84;
    __int32 field_88_obj_id;
    __int16 field_8C;
    Types field_8E;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Glukkon_SaveState, 144);

class Glukkon : public BaseAliveGameObject
{
public:
    EXPORT Glukkon* ctor_43F030(Path_Glukkon* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT static signed int CC CreateFromSaveState_442830(const BYTE* pBuffer);

    virtual void VUpdate() override;

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_4406C0(pOrderingTable);
    }

    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    virtual void VScreenChanged() override;

    virtual void VPossessed_408F70() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual void VOnTrapDoorOpen() override;

    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

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
    EXPORT __int16 AI_0_Calm_WalkAround_440B40();
    EXPORT __int16 AI_1_Panic_4412F0();
    EXPORT __int16 AI_2_Slapped_441720();
    EXPORT __int16 AI_3_PlayerControlled_441A30();
    EXPORT __int16 AI_4_Death_442010();
    EXPORT __int16 AI_5_WaitToSpawn_442490();

public:
    EXPORT static void CC PlaySound_GameSpeak_444AF0(GlukkonSpeak sndIdx, __int16 volume, __int16 pitch, Glukkon* pGlukkon);

private:
    EXPORT void Init_43F260();

    EXPORT Glukkon* vdtor_43F230(signed int flags);

    EXPORT void dtor_43F570();

    EXPORT void vUpdate_43F770();

    EXPORT void vPossessed_440160();

    EXPORT void Update_Slurg_WatchPoints_440600();

    EXPORT void SetAnim_43F9C0(__int16 currentMotion, __int16 bClearNextMotion = FALSE);

    EXPORT void Speak_444640(GlukkonSpeak speak);

    EXPORT void HandleInput_443BB0();

    EXPORT __int16 ShouldPanic_440200(__int16 panicEvenIfNotFacingMe);

    EXPORT __int16 PathBlocked_4442F0(FP a2, __int16 checkBounds);

    EXPORT void SpeakRandomish_4405D0();

    EXPORT void vRender_4406C0(int** ot);

    EXPORT __int16 DoMovement_444190();

    EXPORT void FollowLine_443EB0();

    EXPORT void GetOnPlatforms_444060();

    EXPORT static void CC PlaySound_4447D0(int sndIdx, Glukkon* pGlukkon);

    EXPORT void ToDead_43F640();

    EXPORT void vOn_TLV_Collision_4404A0(Path_TLV* pTlv);

    EXPORT static BOOL CCSTD IsLineOfSightBetween_4403B0(Glukkon* pGlukkon, BaseAliveGameObject* pOther);

    EXPORT void ToStand_443B70();

    EXPORT void SlowDown_444700(FP speed);

    EXPORT void vScreenChanged_440110();

    EXPORT int vGetSaveState_444B90(BYTE* pSaveBuffer);

    EXPORT void vOnTrapDoorOpen_444120();

    EXPORT __int16 vTakeDamage_43FA40(BaseGameObject* pFrom);
    
    
    void SetBrain(TGlukkonAIFn fn);
    bool BrainIs(TGlukkonAIFn fn);

private:
    __int16 field_118_pPalAlloc[64];
    PSX_RECT field_198;
    __int16 field_1A0_red;
    __int16 field_1A2_green;
    __int16 field_1A4_blue;
    __int16 field_1A6;
    Path_Glukkon field_1A8_tlvData;
    __int32 field_1D4_timer;
    FP field_1D8;
    FP field_1DC;
    __int16 field_1E0;
    __int16 field_1E2_bUnknown;
    LevelIds field_1E4_level;
    __int16 field_1E6_path;
    __int16 field_1E8_camera;
    GlukkonSpeak field_1EA_speak;
    char field_1EB_pad;
    int field_1EC;
    int field_1F0;
    int field_1F4;
    int field_1F8;
    __int16 field_1FC;
    __int16 field_1FE;
    int field_200;
    int field_204;
    int field_208_obj_id;
    TGlukkonAIFn field_20C_brain_state_fn;
    __int16 field_210;
    __int16 field_212_currentWalkPitch;
    __int32 field_214_tlv_info;
};
ALIVE_ASSERT_SIZEOF(Glukkon, 0x218);
