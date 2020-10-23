#pragma once

#include "FunctionFwd.hpp"
#include "Path.hpp"
#include "BaseAliveGameObject.hpp"

struct Path_NakedSlig : public Path_TLV
{
    enum class Scale : __int16
    {
        Full_0 = 0,
        Half_1 = 1,
    };
    Scale field_10_scale;

    enum class Direction : __int16
    {
        Left_0 = 0,
        Right_1 = 1,
    };
    Direction field_12_direction;

    enum class State : __int16
    {
        Awake_0 = 0,
        Sleep_1 = 1,
        State_2 = 2, // ??
    };
    State field_14_state;

    enum class LockerDirection : __int16
    {
        Left_0 = 0,
        Right_1 = 1,
        Search_2 = 2,
    };
    LockerDirection field_16_locker_direction;

    __int16 field_18_panic_id;

    enum class ResetOnDeath : __int16
    {
        Yes_1 = 1,
        No_0 = 0,
    };
    ResetOnDeath field_1A_reset_on_death;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_NakedSlig, 0x1C);

class NakedSlig;

using TNakedSligAIFn = __int16 (NakedSlig::*)();
using TNakedSligMotionFn = void (NakedSlig::*)();

enum class LevelIds : __int16;
enum class SligSpeak : signed char;

struct NakedSlig_State
{
    Types field_0_type;
    __int16 field_2_padding;
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
    __int16 field_26_bFlipX;
    __int16 field_28_current_motion;
    __int16 field_2A_anim_cur_frame;
    __int16 field_2C_anim_frame_change_counter;
    char field_2E_bRender;
    char field_2F_bDrawable;
    FP field_30_health;
    __int16 field_34_cur_motion;
    __int16 field_36_next_motion;
    __int16 field_38_last_line_ypos;
    __int16 field_3A_line_type;
    __int16 field_3C_padding;
    __int16 field_3E_padding;
    char field_40_bIsControlled;
    char field_41_padding;
    __int16 field_42_padding;
    int field_44_tlvInfo;
    int field_48_ai_idx;
    __int16 field_4C_padding;
    __int16 field_4E_padding;
    __int16 field_50_brain_sub_state;
    __int16 field_52_padding;
    int field_54_timer;
    FP field_58_velx_scale_factor;
    __int16 field_5C_padding;
    __int16 field_5E_bChanting;
    LevelIds field_60_prev_leve;
    __int16 field_62_prev_path;
    __int16 field_64_prev_camera;
    __int16 field_66_pitch;
    int field_68_unused;
    int field_6C_slig_button_id;
    int field_70_obj_id;
    int field_74_obj_id;
    SligSpeak field_78_speak;
    char field_79_padding;
    __int16 field_7A_unused_counter;
    int field_7C_say_help_timer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(NakedSlig_State, 0x80);

enum NakedSligMotion : __int16
{
    M_Idle_0_41B260 = 0,
    M_UsingButton_1_41B890 = 1,
    M_WakingUp_2_41BF00 = 2,
    M_Crawling_3_41B280 = 3,
    M_StartFalling_4_41B620 = 4,
    M_Falling_5_41B650 = 5,
    M_Landing_6_41B870 = 6,
    M_ToShakingToIdle_7_41C010 = 7,
    M_Speaking_8_41BF70 = 8,
    M_Snoozing_9_41BD80 = 9,
    M_PushingWall_10_41B400 = 10,
    M_TurnAround_11_41B590 = 11,
    M_Shaking_12_418C30 = 12,
    M_Empty_13_418C50 = 13,
    M_ShakingToIdle_14_41C040 = 14,
    M_EndCrawling_15_41B600 = 15,
    M_IdleToPushingWall_16_41B3C0 = 16,
    M_EndPushingWall_17_41B3A0 = 17
};

class NakedSlig : public BaseAliveGameObject
{
public:
    EXPORT NakedSlig* ctor_418C70(Path_NakedSlig* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VRender(int** pOrderingTable) override;

    virtual void VPossessed_408F70() override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    EXPORT static int CC CreateFromSaveState_41AE80(const BYTE* pBuffer);

private:
    EXPORT int vGetSaveState_41C9A0(NakedSlig_State* pState);

    EXPORT void vPossessed_4195F0();

    EXPORT void Set_AnimAndMotion_419890(__int16 currentMotion, __int16 bClearNextMotion);

    EXPORT void UpdateAnimBlock_419900();

    EXPORT BYTE** GetAnimBlock_419950(int currentMotion);

    EXPORT static __int16 CC NextRandom_4197D0();

    EXPORT void vUpdate_419100();

    EXPORT void vRender_419990(int** ot);

    EXPORT __int16 HandleEnemyStopper_41C740(FP velX);

    EXPORT Path_TLV* FindPantsOrWings_419750();

    EXPORT BaseGameObject* FindSligButton_419840();

    EXPORT void vOnTrapDoorOpen_41C580();

    EXPORT void vOn_TLV_Collision_419680(Path_TLV* pTlv);

    EXPORT __int16 vTakeDamage_4192B0(BaseGameObject* pFrom);

private:
    void SetBrain(TNakedSligAIFn fn);
    bool BrainIs(TNakedSligAIFn fn);

    EXPORT void dtor_418FE0();

    EXPORT NakedSlig* vdtor_418FB0(signed int flags);

    EXPORT BOOL PanicOn_419810();

    EXPORT void ToIdle_41C070();

    EXPORT void HandleCommon_41C0B0();
   
    EXPORT __int16 CanCrawl_41C5D0();

    EXPORT void MoveOnLine_41C3D0();

    EXPORT void PlatformCollide_41C500();
public:
    EXPORT __int16 AI_0_Sleeping_419DE0();
    EXPORT __int16 AI_1_Idle_419F60();
    EXPORT __int16 AI_2_PanicGetALocker_419FE0();
    EXPORT __int16 AI_3_Possessed_41A5B0();
    EXPORT __int16 AI_4_GetKilled_41A880();
    EXPORT __int16 AI_5_Transformed_41ADF0();


    EXPORT void M_Idle_0_41B260();
    EXPORT void M_UsingButton_1_41B890();
    EXPORT void M_WakingUp_2_41BF00();
    EXPORT void M_Crawling_3_41B280();
    EXPORT void M_StartFalling_4_41B620();
    EXPORT void M_Falling_5_41B650();
    EXPORT void M_Landing_6_41B870();
    EXPORT void M_ToShakingToIdle_7_41C010();
    EXPORT void M_Speaking_8_41BF70();
    EXPORT void M_Snoozing_9_41BD80();
    EXPORT void M_PushingWall_10_41B400();
    EXPORT void M_TurnAround_11_41B590();
    EXPORT void M_Shaking_12_418C30();
    EXPORT void M_Empty_13_418C50();
    EXPORT void M_ShakingToIdle_14_41C040();
    EXPORT void M_EndCrawling_15_41B600();
    EXPORT void M_IdleToPushingWall_16_41B3C0();
    EXPORT void M_EndPushingWall_17_41B3A0();

private:
    int field_118_tlvInfo;
    __int16 field_11C_pPalAlloc[64];
    PSX_RECT field_19C_pal_rect;
    __int16 field_1A4_r;
    __int16 field_1A6_g;
    __int16 field_1A8_b;

    __int16 field_1AA_padding;
    int field_1AC_timer;
    FP field_1B0_velx_scale_factor; // TODO: Not sure if this is an accurate name, but can't think of anything better.
    int field_1B4_unused;

    __int16 field_1B8_bChanting;

    LevelIds field_1BA_prev_level;
    __int16 field_1BC_prev_path;
    __int16 field_1BE_prev_camera;

    SligSpeak field_1C0_speak;
    char field_1C1_padding;
    __int16 field_1C2_pitch;
    __int16 field_1C4_unused_counter;
    __int16 field_1C6_unused;
    int field_1C8_say_help_timer;
    __int16 field_1CC_unused;
    __int16 field_1CE_padding;
    int field_1D0_slig_button_id;
    int field_1D4_obj_id;
    int field_1D8_obj_id;
    int field_1DC_unused;
    Path_NakedSlig::LockerDirection field_1E0_locker_direction;
    __int16 field_1E2_padding;
    Path_TLV* field_1E4_pPantsOrWingsTlv;
    Path_NakedSlig field_1E8_tlv;
    TNakedSligAIFn field_204_brain_state;
    __int16 field_208_brain_sub_state;
    __int16 field_20A_padding;
};
ALIVE_ASSERT_SIZEOF(NakedSlig, 0x20C);
