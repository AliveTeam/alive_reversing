#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"
#include "BaseAliveGameObject.hpp"

struct Path_CrawlingSlig final : public Path_TLV
{
    Scale_short field_10_scale;
    XDirection_short field_12_direction;

    enum class State : s16
    {
        eSleeping_0 = 0,
        eSleeping_1 = 1,
        eAwake_2 = 2,
    };
    State field_14_state;

    enum class CrawlDirection : s16
    {
        eLeft_0 = 0,
        eRight_1 = 1,
        eRandom_2 = 2,
    };
    CrawlDirection field_16_crawl_direction;
    s16 field_18_panic_switch_id;
    Choice_short field_1A_respawn_on_death;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_CrawlingSlig, 0x1C);

class CrawlingSlig;

using TCrawlingSligBrainFn = s16 (CrawlingSlig::*)();
using TCrawlingSligMotionFn = void (CrawlingSlig::*)();

enum class LevelIds : s16;
enum class SligSpeak : s8;

struct CrawlingSlig_State final
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
    s16 field_20_r;
    s16 field_22_g;
    s16 field_24_b;
    s16 field_26_bFlipX;
    s16 field_28_current_motion;
    s16 field_2A_anim_cur_frame;
    s16 field_2C_anim_frame_change_counter;
    s8 field_2E_bRender;
    s8 field_2F_bDrawable;
    FP field_30_health;
    s16 field_34_cur_motion;
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
    s16 field_50_brain_sub_state;
    s16 field_52_padding;
    s32 field_54_timer;
    FP field_58_velx_scale_factor;
    s16 field_5C_padding;
    s16 field_5E_bChanting;
    LevelIds field_60_prev_leve;
    s16 field_62_prev_path;
    s16 field_64_prev_camera;
    s16 field_66_pitch;
    s32 field_68_unused;
    s32 field_6C_slig_button_id;
    s32 field_70_obj_id;
    s32 field_74_obj_id;
    SligSpeak field_78_speak;
    s8 field_79_padding;
    s16 field_7A_unused_counter;
    s32 field_7C_say_help_timer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(CrawlingSlig_State, 0x80);

enum CrawlingSligMotion : s16
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

class CrawlingSlig final : public BaseAliveGameObject
{
public:
    CrawlingSlig(Path_CrawlingSlig* pTlv, s32 tlvInfo);
    ~CrawlingSlig();
    

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

    virtual void VPossessed_408F70() override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState_41AE80(const u8* pBuffer);

private:
    s32 vGetSaveState_41C9A0(CrawlingSlig_State* pState);

    void vPossessed_4195F0();

    void Set_AnimAndMotion_419890(s16 currentMotion, s16 bClearNextMotion);

    void UpdateAnimBlock_419900();

    u8** GetAnimBlock_419950(s32 currentMotion);

    static s16 NextRandom_4197D0();

    void vUpdate_419100();

    void vRender_419990(PrimHeader** ot);

    s16 HandleEnemyStopper_41C740(FP velX);

    Path_TLV* FindPantsOrWings_419750();

    BaseGameObject* FindSligButton_419840();

    void vOnTrapDoorOpen_41C580();

    void vOn_TLV_Collision_419680(Path_TLV* pTlv);

    s16 vTakeDamage_4192B0(BaseGameObject* pFrom);

private:
    void SetBrain(TCrawlingSligBrainFn fn);
    bool BrainIs(TCrawlingSligBrainFn fn);

    Bool32 PanicOn_419810();

    void ToIdle_41C070();

    void HandleCommon_41C0B0();

    s16 CanCrawl_41C5D0();

    void MoveOnLine_41C3D0();

    void PlatformCollide_41C500();

public:
    s16 Brain_0_Sleeping_419DE0();
    s16 Brain_1_Idle_419F60();
    s16 Brain_2_PanicGetALocker_419FE0();
    s16 Brain_3_Possessed_41A5B0();
    s16 Brain_4_GetKilled_41A880();
    s16 Brain_5_Transformed_41ADF0();


    void M_Idle_0_41B260();
    void M_UsingButton_1_41B890();
    void M_WakingUp_2_41BF00();
    void M_Crawling_3_41B280();
    void M_StartFalling_4_41B620();
    void M_Falling_5_41B650();
    void M_Landing_6_41B870();
    void M_ToShakingToIdle_7_41C010();
    void M_Speaking_8_41BF70();
    void M_Snoozing_9_41BD80();
    void M_PushingWall_10_41B400();
    void M_TurnAround_11_41B590();
    void M_Shaking_12_418C30();
    void M_Empty_13_418C50();
    void M_ShakingToIdle_14_41C040();
    void M_EndCrawling_15_41B600();
    void M_IdleToPushingWall_16_41B3C0();
    void M_EndPushingWall_17_41B3A0();

private:
    s32 field_118_tlvInfo;
    s16 field_11C_pPalAlloc[64];
    PSX_RECT field_19C_pal_rect;
    s16 field_1A4_r;
    s16 field_1A6_g;
    s16 field_1A8_b;

    s16 field_1AA_padding;
    s32 field_1AC_timer;
    FP field_1B0_velx_scale_factor; // TODO: Not sure if this is an accurate name, but can't think of anything better.
    s32 field_1B4_unused;

    s16 field_1B8_bChanting;

    LevelIds field_1BA_prev_level;
    s16 field_1BC_prev_path;
    s16 field_1BE_prev_camera;

    SligSpeak field_1C0_speak;
    s8 field_1C1_padding;
    s16 field_1C2_pitch;
    s16 field_1C4_unused_counter;
    s16 field_1C6_unused;
    s32 field_1C8_say_help_timer;
    s16 field_1CC_unused;
    s16 field_1CE_padding;
    s32 field_1D0_slig_button_id;
    s32 field_1D4_obj_id;
    s32 field_1D8_obj_id;
    s32 field_1DC_unused;
    Path_CrawlingSlig::CrawlDirection field_1E0_crawl_direction;
    s16 field_1E2_padding;
    Path_TLV* field_1E4_pPantsOrWingsTlv;
    Path_CrawlingSlig field_1E8_tlv;
    TCrawlingSligBrainFn field_204_brain_state;
    s16 field_208_brain_sub_state;
    s16 field_20A_padding;
};
ALIVE_ASSERT_SIZEOF(CrawlingSlig, 0x20C);
