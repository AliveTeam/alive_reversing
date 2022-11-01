#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class CrawlingSlig;

using TCrawlingSligBrainFn = s16 (CrawlingSlig::*)();
using TCrawlingSligMotionFn = void (CrawlingSlig::*)();

enum class LevelIds : s16;
enum class SligSpeak : s8;

struct CrawlingSligSaveState final
{
    ReliveTypes field_0_type;
    s16 field_2_padding;
    Guid field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    s16 field_18_path_number;
    EReliveLevelIds field_1A_lvl_number;
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
    Guid field_44_tlvInfo;
    s32 field_48_brain_idx;
    s16 field_4C_padding;
    s16 field_4E_padding;
    s16 field_50_brain_sub_state;
    s16 field_52_padding;
    s32 field_54_timer;
    FP field_58_velx_scale_factor;
    s16 field_5C_padding;
    s16 field_5E_bChanting;
    EReliveLevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    s16 field_66_unused;
    s32 field_68_unused;
    Guid field_6C_slig_button_id;
    Guid field_70_obj_id;
    Guid field_74_obj_id;
    SligSpeak field_78_speak;
    s8 field_79_padding;
    s16 field_7A_unused_counter;
    s32 field_7C_say_help_timer;
};

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
    CrawlingSlig(relive::Path_CrawlingSlig* pTlv, const Guid& guid);
    ~CrawlingSlig();


    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VPossessed() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

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
    void LoadAnimations();

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
    relive::Path_TLV* FindPantsOrWings();
    BaseGameObject* FindSligButton();

    void SetBrain(TCrawlingSligBrainFn fn);
    bool BrainIs(TCrawlingSligBrainFn fn);

    bool PanicOn();
    void ToIdle();
    void HandleCommon();
    s16 CanCrawl();
    void MoveOnLine();
    void PlatformCollide();

private:
    Guid mGuid;
    s16 field_11C_pPalAlloc[64] = {};
    s16 field_1A4_r = 0;
    s16 field_1A6_g = 0;
    s16 field_1A8_b = 0;

    s32 field_1AC_timer = 0;
    FP field_1B0_velx_scale_factor = {}; // TODO: Not sure if this is an accurate name, but can't think of anything better.

    bool mChanting = false;

    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbePath = 0;
    s16 mAbeCamera = 0;

    SligSpeak mSpeak = SligSpeak::eNone;
    s32 mSayHelpTimer = 0;
    Guid field_1D0_slig_button_id = Guid{};
    Guid field_1D4_obj_id = Guid{};
    Guid field_1D8_obj_id = Guid{};
    relive::Path_CrawlingSlig::CrawlDirection mCrawlDirection = relive::Path_CrawlingSlig::CrawlDirection::eLeft;
    relive::Path_TLV* mTlvHeader = nullptr;
    relive::Path_CrawlingSlig mTlv = {};
    TCrawlingSligBrainFn mBrainState = nullptr;
    s16 mBrainSubState = 0;
};
