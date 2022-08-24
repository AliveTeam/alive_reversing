#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"
#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"

struct Path_CrawlingSlig final : public Path_TLV
{
    Scale_short mScale;
    XDirection_short mFacing; // unused

    enum class StartState : s16
    {
        eSleeping_0 = 0,
        eSleeping_1 = 1,
        eAwake_2 = 2,
    };
    StartState mStartState;

    enum class CrawlDirection : s16
    {
        eLeft_0 = 0,
        eRight_1 = 1,
        eRandom_2 = 2,
    };
    CrawlDirection mCrawlDirection;
    s16 mPanicSwitchId;
    Choice_short mRespawnOnDeath;
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
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
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

enum class CrawlingSligMotion : s16
{
    None = -1,
    Motion_0_Idle = 0,
    Motion_1_UsingButton = 1,
    Motion_2_WakingUp = 2,
    Motion_3_Crawling = 3,
    Motion_4_StartFalling = 4,
    Motion_5_Falling = 5,
    Motion_6_Landing = 6,
    Motion_7_ToShakingToIdle = 7,
    Motion_8_Speaking = 8,
    Motion_9_Snoozing = 9,
    Motion_10_PushingWall = 10,
    Motion_11_TurnAround = 11,
    Motion_12_Shaking = 12,
    Motion_13_Empty = 13,
    Motion_14_ShakingToIdle = 14,
    Motion_15_EndCrawling = 15,
    Motion_16_IdleToPushingWall = 16,
    Motion_17_EndPushingWall = 17
};

class CrawlingSlig final : public BaseAliveGameObject
{
public:
    CrawlingSlig(Path_CrawlingSlig* pTlv, s32 tlvInfo);
    ~CrawlingSlig();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VPossessed() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VOnTlvCollision(Path_TLV* pTlv) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    CrawlingSligMotion GetNextMotion() const
    {
        return static_cast<CrawlingSligMotion>(mNextMotion);
    }
    CrawlingSligMotion GetCurrentMotion() const
    {
        return static_cast<CrawlingSligMotion>(mCurrentMotion);
    }

    void Set_AnimAndMotion(CrawlingSligMotion currentMotion, s16 bClearNextMotion);
    void UpdateAnimBlock();
    u8** GetAnimBlock(s32 currentMotion);
    static s16 NextRandom();
    s16 HandleEnemyStopper(FP velX);
    Path_TLV* FindPantsOrWings();
    BaseGameObject* FindSligButton();

    void SetBrain(TCrawlingSligBrainFn fn);
    bool BrainIs(TCrawlingSligBrainFn fn);

    bool PanicOn();
    void ToIdle();
    void HandleCommon();
    s16 CanCrawl();
    void MoveOnLine();
    void PlatformCollide();

public:
    s16 Brain_0_Sleeping();
    s16 Brain_1_Idle();
    s16 Brain_2_PanicGetALocker();
    s16 Brain_3_Possessed();
    s16 Brain_4_GetKilled();
    s16 Brain_5_Transformed();


    void Motion_0_Idle();
    void Motion_1_UsingButton();
    void Motion_2_WakingUp();
    void Motion_3_Crawling();
    void Motion_4_StartFalling();
    void Motion_5_Falling();
    void Motion_6_Landing();
    void Motion_7_ToShakingToIdle();
    void Motion_8_Speaking();
    void Motion_9_Snoozing();
    void Motion_10_PushingWall();
    void Motion_11_TurnAround();
    void Motion_12_Shaking();
    void Motion_13_Empty();
    void Motion_14_ShakingToIdle();
    void Motion_15_EndCrawling();
    void Motion_16_IdleToPushingWall();
    void Motion_17_EndPushingWall();

private:
    s32 field_118_tlvInfo = 0;
    s16 field_11C_pPalAlloc[64] = {};
    PSX_RECT field_19C_pal_rect = {};
    s16 field_1A4_r = 0;
    s16 field_1A6_g = 0;
    s16 field_1A8_b = 0;

    s32 field_1AC_timer = 0;
    FP field_1B0_velx_scale_factor = {}; // TODO: Not sure if this is an accurate name, but can't think of anything better.
    s32 field_1B4_unused = 0;

    s16 field_1B8_bChanting = 0;

    EReliveLevelIds field_1BA_prev_level = EReliveLevelIds::eNone;
    s16 field_1BC_prev_path = 0;
    s16 field_1BE_prev_camera = 0;

    SligSpeak field_1C0_speak = SligSpeak::eHi_0;
    s16 field_1C2_pitch = 0;
    s16 field_1C4_unused_counter = 0;
    s16 field_1C6_unused = 0;
    s32 field_1C8_say_help_timer = 0;
    s16 field_1CC_unused = 0;
    s32 field_1D0_slig_button_id = 0;
    s32 field_1D4_obj_id = 0;
    s32 field_1D8_obj_id = 0;
    s32 field_1DC_unused = 0;
    Path_CrawlingSlig::CrawlDirection field_1E0_crawl_direction = Path_CrawlingSlig::CrawlDirection::eLeft_0;
    Path_TLV* field_1E4_pPantsOrWingsTlv = nullptr;
    Path_CrawlingSlig field_1E8_tlv = {};
    TCrawlingSligBrainFn field_204_brain_state = nullptr;
    s16 field_208_brain_sub_state = 0;
};
ALIVE_ASSERT_SIZEOF(CrawlingSlig, 0x20C);
