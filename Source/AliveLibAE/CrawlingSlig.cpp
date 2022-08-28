#include "stdafx.h"
#include "CrawlingSlig.hpp"
#include "Function.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "MusicController.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "SwitchStates.hpp"
#include "SnoozeParticle.hpp"
#include "Input.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "ScreenShake.hpp"
#include "Sound/Midi.hpp"
#include "Slig.hpp"
#include "FlyingSlig.hpp"
#include "CrawlingSligButton.hpp"
#include "Sfx.hpp"
#include "SlamDoor.hpp"
#include "Grid.hpp"

const TintEntry kCrawlingSligTints_5514B8[18] = {
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

const AnimId sCrawlingSligAnimIdTable[18] = {
    AnimId::CrawlingSlig_Idle,
    AnimId::CrawlingSlig_UsingButton,
    AnimId::CrawlingSlig_WakingUp,
    AnimId::CrawlingSlig_Crawling,
    AnimId::CrawlingSlig_StartFalling,
    AnimId::CrawlingSlig_Falling,
    AnimId::CrawlingSlig_Landing,
    AnimId::CrawlingSlig_ToShakingToIdle,
    AnimId::CrawlingSlig_Speaking,
    AnimId::CrawlingSlig_Snoozing,
    AnimId::CrawlingSlig_PushingWall,
    AnimId::CrawlingSlig_TurnAround,
    AnimId::CrawlingSlig_Shaking,
    AnimId::CrawlingSlig_Empty,
    AnimId::CrawlingSlig_ShakingToIdle,
    AnimId::CrawlingSlig_EndCrawling,
    AnimId::CrawlingSlig_IdleToPushingWall,
    AnimId::CrawlingSlig_EndPushingWall 
};

ALIVE_ARY(1, 0x551428, TCrawlingSligMotionFn, 18, sCrawlingSlig_motions_551428,
          {&CrawlingSlig::Motion_0_Idle,
           &CrawlingSlig::Motion_1_UsingButton,
           &CrawlingSlig::Motion_2_WakingUp,
           &CrawlingSlig::Motion_3_Crawling,
           &CrawlingSlig::Motion_4_StartFalling,
           &CrawlingSlig::Motion_5_Falling,
           &CrawlingSlig::Motion_6_Landing,
           &CrawlingSlig::Motion_7_ToShakingToIdle,
           &CrawlingSlig::Motion_8_Speaking,
           &CrawlingSlig::Motion_9_Snoozing,
           &CrawlingSlig::Motion_10_PushingWall,
           &CrawlingSlig::Motion_11_TurnAround,
           &CrawlingSlig::Motion_12_Shaking,
           &CrawlingSlig::Motion_13_Empty,
           &CrawlingSlig::Motion_14_ShakingToIdle,
           &CrawlingSlig::Motion_15_EndCrawling,
           &CrawlingSlig::Motion_16_IdleToPushingWall,
           &CrawlingSlig::Motion_17_EndPushingWall});


const static TCrawlingSligBrainFn sCrawlingSligBrainTable[] = {
    &CrawlingSlig::Brain_0_Sleeping,
    &CrawlingSlig::Brain_1_Idle,
    &CrawlingSlig::Brain_2_PanicGetALocker,
    &CrawlingSlig::Brain_3_Possessed,
    &CrawlingSlig::Brain_4_GetKilled,
    &CrawlingSlig::Brain_5_Transformed,
};

enum Brain_2_PanicGetALocker
{
    eBrain2_DetermineCrawlDirection_0 = 0,
    eBrain2_Falling_1 = 1,
    eBrain2_SearchLocker_2 = 2,
    eBrain2_TurnAroundForLocker_3 = 3,
    eBrain2_SearchLockerOrTurnAround_4 = 4,
    eBrain2_GetPantsOrWings_5 = 5,
    eBrain2_UsingButton_6 = 6,
    eBrain2_TurnAround_7 = 7,
    eBrain2_Crawling_8 = 8,
    eBrain2_CheckIfWallHit_9 = 9,
    eBrain2_BeatBySlig_10 = 10
};

enum Brain_2_Possessed
{
    eBrain3_StartPossession_0 = 0,
    eBrain3_Possessed_1 = 1,
    eBrain3_Unpossessing_2 = 2,
    eBrain3_BeatBySlig_3 = 3
};

enum Brain_4_GetKilled
{
    eBrain4_Unknown_0 = 0,
    eBrain4_Vaporize_1 = 1,
    eBrain4_GibsDeath_2 = 2,
    eBrain4_SetDead_3 = 3,
    eBrain4_DeathBySlog_4 = 4,
    eBrain4_DeathDrop_5 = 5
};

CrawlingSlig::CrawlingSlig(relive::Path_CrawlingSlig* pTlv, const TLVUniqueId& tlvId)
    : BaseAliveGameObject(2)
{
    field_1DC_unused = -1;
    SetType(ReliveTypes::eCrawlingSlig);

    const AnimRecord& rec = AnimRec(AnimId::CrawlingSlig_Idle);
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, 1, 0));
    Animation_Init(AnimId::CrawlingSlig_Idle, field_10_resources_array.ItemAt(0));

    SetTint(&kCrawlingSligTints_5514B8[0], gMap.mCurrentLevel);

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);

    field_1CC_unused = 0;
    field_1AC_timer = 0;
    field_1C8_say_help_timer = 0;
    field_1B8_bChanting = 0;
    field_1C4_unused_counter = 0;
    field_1C0_speak = SligSpeak::eNone;
    field_1D0_slig_button_id = -1;
    field_1D4_obj_id = -1;
    field_1D8_obj_id = -1;
    field_1E4_pPantsOrWingsTlv = 0;

    mShadow = relive_new Shadow();

    field_118_tlvInfo = tlvId;
    field_1E8_tlv = *pTlv;

    if (field_1E8_tlv.mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_8;
        mScale = Scale::Bg;
    }
    else if (field_1E8_tlv.mScale == relive::reliveScale::eFull)
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_27;
        mScale = Scale::Fg;
    }

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (field_1E8_tlv.mStartState == relive::Path_CrawlingSlig::StartState::eAwake)
    {
        Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, TRUE);
        SetBrain(&CrawlingSlig::Brain_1_Idle);
    }
    else
    {
        if (mSpriteScale == FP_FromInteger(1))
        {
            mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        }
        else
        {
            mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        }
        Set_AnimAndMotion(CrawlingSligMotion::Motion_9_Snoozing, TRUE);
        SetBrain(&CrawlingSlig::Brain_0_Sleeping);
    }

    field_208_brain_sub_state = 0;
    if (field_1E8_tlv.mCrawlDirection == relive::Path_CrawlingSlig::CrawlDirection::eRandom)
    {
        field_1E0_crawl_direction = NextRandom() ? relive::Path_CrawlingSlig::CrawlDirection::eRight : relive::Path_CrawlingSlig::CrawlDirection::eLeft;
    }
    else
    {
        field_1E0_crawl_direction = field_1E8_tlv.mCrawlDirection;
    }

    field_1C6_unused = field_1E8_tlv.mPanicSwitchId;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(37),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }
}

void CrawlingSlig::VRender(PrimHeader** ot)
{
    const s16 eyeIndices[] = {60, 62};
    renderWithGlowingEyes(ot, this, &field_11C_pPalAlloc[0], ALIVE_COUNTOF(field_11C_pPalAlloc),
                          &field_19C_pal_rect, field_1A4_r, field_1A6_g, field_1A8_b, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

s32 CrawlingSlig::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const CrawlingSlig_State*>(pBuffer);

    auto pTlv = static_cast<relive::Path_CrawlingSlig*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_44_tlvInfo));
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligResID_449, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("CRAWLSLG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBLOW.BAN", nullptr);
    }

    auto pCrawlingSlig = relive_new CrawlingSlig(pTlv, pState->field_44_tlvInfo);
    if (pCrawlingSlig)
    {
        pCrawlingSlig->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

        if (pState->field_40_bIsControlled)
        {
            sControlledCharacter = pCrawlingSlig;
        }

        pCrawlingSlig->BaseAliveGameObjectPathTLV = nullptr;
        pCrawlingSlig->BaseAliveGameObjectCollisionLine = nullptr;

        pCrawlingSlig->mXPos = pState->field_8_xpos;
        pCrawlingSlig->mYPos = pState->field_C_ypos;
        pCrawlingSlig->mVelX = pState->field_10_velx;
        pCrawlingSlig->mVelY = pState->field_14_vely;

        pCrawlingSlig->field_1B0_velx_scale_factor = pState->field_58_velx_scale_factor;

        pCrawlingSlig->mCurrentPath = pState->field_18_path_number;
        pCrawlingSlig->mCurrentLevel = MapWrapper::FromAESaveData(pState->field_1A_lvl_number);
        pCrawlingSlig->mSpriteScale = pState->field_1C_sprite_scale;

        pCrawlingSlig->field_1A4_r = pState->mRingRed;
        pCrawlingSlig->field_1A6_g = pState->mRingGreen;
        pCrawlingSlig->field_1A8_b = pState->mRingBlue;

        pCrawlingSlig->mRGB.SetRGB(pState->mRingRed, pState->mRingGreen, pState->mRingBlue);

        pCrawlingSlig->mCurrentMotion = pState->field_28_current_motion;

        pCrawlingSlig->mAnim.Set_Animation_Data(sCrawlingSligAnimIdTable[pState->field_28_current_motion], nullptr);
        pCrawlingSlig->mAnim.mCurrentFrame = pState->field_2A_anim_cur_frame;

        pCrawlingSlig->mAnim.mFrameChangeCounter = pState->field_2C_anim_frame_change_counter;

        pCrawlingSlig->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, pState->field_26_bFlipX & 1);
        pCrawlingSlig->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->field_2E_bRender & 1);

        pCrawlingSlig->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_bDrawable & 1);

        if (IsLastFrame(&pCrawlingSlig->mAnim))
        {
            pCrawlingSlig->mAnim.mFlags.Set(AnimFlags::eBit18_IsLastFrame);
        }

        pCrawlingSlig->mHealth = pState->field_30_health;
        pCrawlingSlig->mCurrentMotion = pState->field_34_cur_motion;
        pCrawlingSlig->mNextMotion = pState->field_36_next_motion;
        pCrawlingSlig->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_38_last_line_ypos);
        pCrawlingSlig->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
        pCrawlingSlig->field_1AC_timer = pState->field_54_timer;
        pCrawlingSlig->BaseAliveGameObjectCollisionLineType = pState->field_3A_line_type;
        pCrawlingSlig->field_118_tlvInfo = pState->field_44_tlvInfo;
        pCrawlingSlig->SetBrain(sCrawlingSligBrainTable[pState->field_48_brain_idx]);
        pCrawlingSlig->field_208_brain_sub_state = pState->field_50_brain_sub_state;
        pCrawlingSlig->field_1B8_bChanting = pState->field_5E_bChanting;
        pCrawlingSlig->field_1BA_prev_level = MapWrapper::FromAESaveData(pState->field_60_prev_leve);
        pCrawlingSlig->field_1BC_prev_path = pState->field_62_prev_path;
        pCrawlingSlig->field_1BE_prev_camera = pState->field_64_prev_camera;
        pCrawlingSlig->field_1B4_unused = pState->field_68_unused;
        pCrawlingSlig->field_1D0_slig_button_id = pState->field_6C_slig_button_id;
        pCrawlingSlig->field_1D4_obj_id = pState->field_70_obj_id;
        pCrawlingSlig->field_1D8_obj_id = pState->field_74_obj_id;
        pCrawlingSlig->field_1C0_speak = pState->field_78_speak;
        pCrawlingSlig->field_1C2_pitch = pState->field_66_pitch;
        pCrawlingSlig->field_1C4_unused_counter = pState->field_7A_unused_counter;
        pCrawlingSlig->field_1C8_say_help_timer = pState->field_7C_say_help_timer;
    }

    return sizeof(CrawlingSlig_State);
}

s32 CrawlingSlig::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<CrawlingSlig_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eCrawlingSlig_26;
    pState->field_4_obj_id = mBaseGameObjectTlvInfo;

    pState->field_8_xpos = mXPos;
    pState->field_C_ypos = mYPos;
    pState->field_10_velx = mVelX;
    pState->field_14_vely = mVelY;

    pState->field_58_velx_scale_factor = field_1B0_velx_scale_factor;

    pState->field_18_path_number = mCurrentPath;
    pState->field_1A_lvl_number = MapWrapper::ToAE(mCurrentLevel);
    pState->field_1C_sprite_scale = mSpriteScale;

    pState->mRingRed = mRGB.r;
    pState->mRingGreen = mRGB.g;
    pState->mRingBlue = mRGB.b;
    pState->field_26_bFlipX = mAnim.mFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_28_current_motion = mCurrentMotion;
    pState->field_2A_anim_cur_frame = mAnim.mCurrentFrame;
    pState->field_2C_anim_frame_change_counter = mAnim.mFrameChangeCounter;
    pState->field_2F_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2E_bRender = mAnim.mFlags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = mHealth;
    pState->field_34_cur_motion = mCurrentMotion;
    pState->field_36_next_motion = mNextMotion;
    pState->field_3A_line_type = -1;

    // TODO: Check correct
    pState->field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_3A_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }

    pState->field_40_bIsControlled = (this == sControlledCharacter);
    pState->field_54_timer = field_1AC_timer;
    pState->field_44_tlvInfo = field_118_tlvInfo;
    pState->field_48_brain_idx = 0;

    s32 idx = 0;
    for (const auto& fn : sCrawlingSligBrainTable)
    {
        if (BrainIs(fn))
        {
            pState->field_48_brain_idx = idx;
            break;
        }
        idx++;
    }

    pState->field_50_brain_sub_state = field_208_brain_sub_state;
    pState->field_5E_bChanting = field_1B8_bChanting;
    pState->field_60_prev_leve = MapWrapper::ToAE(field_1BA_prev_level);
    pState->field_62_prev_path = field_1BC_prev_path;
    pState->field_64_prev_camera = field_1BE_prev_camera;
    pState->field_66_pitch = field_1C2_pitch;
    pState->field_68_unused = field_1B4_unused;
    pState->field_6C_slig_button_id = field_1D0_slig_button_id;
    pState->field_70_obj_id = field_1D4_obj_id;
    pState->field_74_obj_id = field_1D8_obj_id;
    pState->field_78_speak = field_1C0_speak;
    pState->field_7A_unused_counter = field_1C4_unused_counter;
    pState->field_7C_say_help_timer = field_1C8_say_help_timer;
    return sizeof(CrawlingSlig_State);
}

void CrawlingSlig::VPossessed()
{
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
    field_1B8_bChanting = TRUE;
    Set_AnimAndMotion(CrawlingSligMotion::Motion_12_Shaking, TRUE);
    SetBrain(&CrawlingSlig::Brain_3_Possessed);
    field_208_brain_sub_state = 0;
    field_1AC_timer = sGnFrame + 35;
    field_1BA_prev_level = gMap.mCurrentLevel;
    field_1BC_prev_path = gMap.mCurrentPath;
    field_1BE_prev_camera = gMap.mCurrentCamera;
}

void CrawlingSlig::Set_AnimAndMotion(CrawlingSligMotion currentMotion, s16 bClearNextMotion)
{
    mAnim.Set_Animation_Data(sCrawlingSligAnimIdTable[static_cast<s16>(currentMotion)], nullptr);
    mCurrentMotion = static_cast<s16>(currentMotion);

    UpdateAnimBlock();

    if (bClearNextMotion)
    {
        mNextMotion = -1;
    }
}

void CrawlingSlig::UpdateAnimBlock()
{
    u8** ppRes = GetAnimBlock(mCurrentMotion);
    if (!ppRes)
    {
        mCurrentMotion = 0;
        ppRes = GetAnimBlock(0);
    }
    mAnim.Set_Animation_Data(sCrawlingSligAnimIdTable[mCurrentMotion], ppRes);
}

u8** CrawlingSlig::GetAnimBlock(s32 /*currentMotion*/)
{
    if (field_1CC_unused)
    {
        field_1CC_unused = 0;
    }
    return field_10_resources_array.ItemAt(0);
}

s16 CrawlingSlig::NextRandom()
{
    return (Math_NextRandom() & 1) == 0;
}

void CrawlingSlig::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
        {
            mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
            if (BaseAliveGameObjectCollisionLineType == -1)
            {
                BaseAliveGameObjectCollisionLine = 0;
            }
            else
            {
                sCollisions->Raycast(
                    mXPos,
                    mYPos - FP_FromInteger(20),
                    mXPos,
                    mYPos + FP_FromInteger(20),
                    &BaseAliveGameObjectCollisionLine,
                    &mXPos,
                    &mYPos,
                    CollisionMask(static_cast<eLineTypes>(BaseAliveGameObjectCollisionLineType)));
            }
            BaseAliveGameObjectCollisionLineType = 0;
            field_1D4_obj_id = RefreshId(field_1D4_obj_id);
            field_1D8_obj_id = RefreshId(field_1D8_obj_id);
            field_1D0_slig_button_id = RefreshId(field_1D0_slig_button_id);
        }

        if (!Input_IsChanting_45F260())
        {
            field_1B8_bChanting = 0;
        }

        field_208_brain_sub_state = (this->*field_204_brain_state)();

        const FP oldX = mXPos;
        const FP oldY = mYPos;

        (this->*sCrawlingSlig_motions_551428[mCurrentMotion])();

        if (oldX != mXPos || oldY != mYPos)
        {
            auto pTlv = sPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);

            VOnTlvCollision(pTlv);
        }
    }
}

s16 CrawlingSlig::HandleEnemyStopper(FP /*velX*/)
{
    FP gridSizeDirected = ScaleToGridSize(mSpriteScale);
    relive::Path_EnemyStopper::StopDirection direction = relive::Path_EnemyStopper::StopDirection::Both;
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        direction = relive::Path_EnemyStopper::StopDirection::Left;
        gridSizeDirected = -gridSizeDirected;
    }
    else
    {
        direction = relive::Path_EnemyStopper::StopDirection::Right;
    }

    if (WallHit(mSpriteScale * FP_FromInteger(30), gridSizeDirected * FP_FromDouble(1.5)))
    {
        return 1;
    }

    const FP gridSize = ScaleToGridSize(mSpriteScale);
    auto pSlamDoor = static_cast<relive::Path_SlamDoor*>(sPathInfo->TLV_Get_At_4DB4B0(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos + gridSizeDirected),
        FP_GetExponent(mYPos - gridSize),
        ReliveTypes::eSlamDoor));
    BaseAliveGameObjectPathTLV = pSlamDoor;

    if (pSlamDoor && ((pSlamDoor->mStartClosed == relive::reliveChoice::eYes && !SwitchStates_Get(pSlamDoor->mSwitchId)) || (pSlamDoor->mStartClosed == relive::reliveChoice::eNo && SwitchStates_Get(pSlamDoor->mSwitchId))))
    {
        return 1;
    }

    auto pStopper = static_cast<relive::Path_EnemyStopper*>(sPathInfo->TLV_Get_At_4DB4B0(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos + gridSizeDirected),
        FP_GetExponent(mYPos - gridSize),
        ReliveTypes::eEnemyStopper));
    BaseAliveGameObjectPathTLV = pStopper;

    return pStopper && (pStopper->mStopDirection == direction || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && SwitchStates_Get(pStopper->mSwitchId);
}

relive::Path_TLV* CrawlingSlig::FindPantsOrWings()
{
    relive::Path_TLV* pTlvIter = sPathInfo->TlvGetAt(nullptr, mXPos, mYPos, mXPos, mYPos);
    while (pTlvIter)
    {
        if (pTlvIter->mTlvType == ReliveTypes::eSligGetPants || pTlvIter->mTlvType == ReliveTypes::eSligGetWings)
        {
            return pTlvIter;
        }
        pTlvIter = sPathInfo->TlvGetAt(pTlvIter, mXPos, mYPos, mXPos, mYPos);
    }
    return nullptr;
}

BaseGameObject* CrawlingSlig::FindSligButton()
{
    return FindObjectOfType(ReliveTypes::eSligButton, mXPos, mYPos - (FP_FromInteger(30) * mSpriteScale));
}

void CrawlingSlig::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = -1;
        Set_AnimAndMotion(CrawlingSligMotion::Motion_4_StartFalling, TRUE);
    }
}

void CrawlingSlig::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_DeathDrop_5;
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                EventBroadcast(kEventMudokonComfort, this);
                Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_1C2_pitch, this);
                field_1AC_timer = sGnFrame + 60;
            }
        }

        pTlv = sPathInfo->TlvGetAt(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

s16 CrawlingSlig::VTakeDamage(BaseGameObject* pFrom)
{
    if (!BrainIs(&CrawlingSlig::Brain_5_Transformed))
    {
        switch (pFrom->Type())
        {
            case ReliveTypes::eBullet:
            case ReliveTypes::eDrill:
            case ReliveTypes::eBaseBomb:
            case ReliveTypes::eRockSpawner:
            case ReliveTypes::eMeatSaw:
            case ReliveTypes::eMineCar:
            //case ReliveTypes::eNeverSet:
            case ReliveTypes::eExplosion:
                if (!BrainIs(&CrawlingSlig::Brain_4_GetKilled))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_GibsDeath_2;
                    EventBroadcast(kEventMudokonComfort, this);
                }
                return 1;

            case ReliveTypes::eElectricWall:
                Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, 0, this);
                return 1;

            case ReliveTypes::eSlig:
            {
                // Take a BEATING
                field_1AC_timer = sGnFrame + 20;
                mHealth -= FP_FromDouble(0.13);

                if (mHealth <= FP_FromInteger(0))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_GibsDeath_2;
                    return 0;
                }

                Set_AnimAndMotion(CrawlingSligMotion::Motion_7_ToShakingToIdle, TRUE);
                Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, 0, this);

                if (BrainIs(&CrawlingSlig::Brain_2_PanicGetALocker))
                {
                    field_208_brain_sub_state = Brain_2_PanicGetALocker::eBrain2_BeatBySlig_10;
                }
                else if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
                {
                    field_208_brain_sub_state = Brain_2_Possessed::eBrain3_BeatBySlig_3;
                }
                else
                {
                    SetBrain(&CrawlingSlig::Brain_2_PanicGetALocker);
                    field_208_brain_sub_state = Brain_2_PanicGetALocker::eBrain2_BeatBySlig_10;
                }
            }
                return 1;

            case ReliveTypes::eSlog:
                if (!BrainIs(&CrawlingSlig::Brain_4_GetKilled))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_DeathBySlog_4;
                    mVelY = FP_FromInteger(0);
                    mVelX = FP_FromInteger(0);
                    mHealth = FP_FromInteger(0);
                    MapFollowMe(TRUE);
                    field_1AC_timer = sGnFrame + 15;
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_13_Empty, TRUE);
                    EventBroadcast(kEventMudokonComfort, this);
                }
                return 1;

            case ReliveTypes::eElectrocute:
                if (!BrainIs(&CrawlingSlig::Brain_4_GetKilled))
                {
                    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
                    mHealth = FP_FromInteger(0);
                    field_1AC_timer = sGnFrame + 1;
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_SetDead_3;
                    EventBroadcast(kEventMudokonComfort, this);
                }
                return 1;

            default:
                return 1;
        }
    }
    return 1;
}

void CrawlingSlig::SetBrain(TCrawlingSligBrainFn fn)
{
    field_204_brain_state = fn;
}

bool CrawlingSlig::BrainIs(TCrawlingSligBrainFn fn)
{
    return field_204_brain_state == fn;
}

CrawlingSlig::~CrawlingSlig()
{
    if (sControlledCharacter == this)
    {
        sControlledCharacter = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
        if (gMap.mNextLevel != EReliveLevelIds::eMenu)
        {
            gMap.SetActiveCam(
                field_1BA_prev_level,
                field_1BC_prev_path,
                field_1BE_prev_camera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }
    if (mHealth > FP_FromInteger(0) || field_1E8_tlv.mRespawnOnDeath == relive::reliveChoice::eYes)
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 1);
    }
}

bool CrawlingSlig::PanicOn()
{
    return SwitchStates_Get(field_1E8_tlv.mPanicSwitchId) != 0;
}

void CrawlingSlig::ToIdle()
{
    field_1B0_velx_scale_factor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, FALSE);
    MapFollowMe(TRUE);
}

enum Brain_0_Sleeping
{
    eBrain0_Sleeping_0 = 0,
    eBrain0_WakingUp_1 = 1,
    eBrain0_IsAwake_2 = 2
};

s16 CrawlingSlig::Brain_0_Sleeping()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    if (field_208_brain_sub_state == Brain_0_Sleeping::eBrain0_Sleeping_0)
    {
        if (!(static_cast<s32>(sGnFrame) % 16))
        {
            if (field_1C4_unused_counter > 0)
            {
                field_1C4_unused_counter--;
            }
        }

        if (PanicOn())
        {
            field_1AC_timer = sGnFrame + Math_RandomRange(15, 45);
            return Brain_0_Sleeping::eBrain0_WakingUp_1;
        }
        else
        {
            return field_208_brain_sub_state;
        }
    }

    if (field_208_brain_sub_state == Brain_0_Sleeping::eBrain0_WakingUp_1)
    {
        if (static_cast<s32>(sGnFrame) <= field_1AC_timer)
        {
            return field_208_brain_sub_state;
        }

        SetNextMotion(CrawlingSligMotion::Motion_2_WakingUp);
        return Brain_0_Sleeping::eBrain0_IsAwake_2;
    }

    if (field_208_brain_sub_state != Brain_0_Sleeping::eBrain0_IsAwake_2 ||
        GetCurrentMotion() != CrawlingSligMotion::Motion_2_WakingUp ||
        !(mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return field_208_brain_sub_state;
    }

    if (mSpriteScale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_27;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_8;
    }

    ToIdle();
    SetBrain(&CrawlingSlig::Brain_2_PanicGetALocker);
    return 0;
}

s16 CrawlingSlig::Brain_1_Idle()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    if (PanicOn())
    {
        SetBrain(&CrawlingSlig::Brain_2_PanicGetALocker);
    }
    return 0;
}

s16 CrawlingSlig::Brain_2_PanicGetALocker()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    if (!BaseAliveGameObjectCollisionLine && field_208_brain_sub_state != Brain_2_PanicGetALocker::eBrain2_Falling_1)
    {
        field_208_brain_sub_state = Brain_2_PanicGetALocker::eBrain2_Falling_1;
    }

    switch (field_208_brain_sub_state)
    {
        case Brain_2_PanicGetALocker::eBrain2_DetermineCrawlDirection_0:
            if ((field_1E0_crawl_direction != relive::Path_CrawlingSlig::CrawlDirection::eRight || !(mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))) && (field_1E0_crawl_direction != relive::Path_CrawlingSlig::CrawlDirection::eLeft || mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)))
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                return Brain_2_PanicGetALocker::eBrain2_SearchLocker_2;
            }
            SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;

        case Brain_2_PanicGetALocker::eBrain2_Falling_1:
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_0_Idle)
            {
                return field_208_brain_sub_state;
            }
            return Brain_2_PanicGetALocker::eBrain2_DetermineCrawlDirection_0;

        case Brain_2_PanicGetALocker::eBrain2_SearchLocker_2:
            if (HandleEnemyStopper(mVelX))
            {
                field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame + 30;
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                return Brain_2_PanicGetALocker::eBrain2_TurnAround_7;
            }

            field_1E4_pPantsOrWingsTlv = FindPantsOrWings();
            if (field_1E4_pPantsOrWingsTlv)
            {
                ToIdle();
                return Brain_2_PanicGetALocker::eBrain2_GetPantsOrWings_5;
            }
            else
            {
                if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= field_1C8_say_help_timer)
                {
                    return field_208_brain_sub_state;
                }
                SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
                field_1C8_say_help_timer = sGnFrame + 60;
                field_1C0_speak = SligSpeak::eHelp_10;
                return Brain_2_PanicGetALocker::eBrain2_SearchLockerOrTurnAround_4;
            }
            break;

        case Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3:
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_11_TurnAround || !(mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }
            SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
            return Brain_2_PanicGetALocker::eBrain2_SearchLocker_2;

        case Brain_2_PanicGetALocker::eBrain2_SearchLockerOrTurnAround_4:
        {
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_8_Speaking || !(mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }

            FP gridScale = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridScale = -ScaleToGridSize(mSpriteScale);
            }
            else
            {
                gridScale = ScaleToGridSize(mSpriteScale);
            }

            if (!WallHit(FP_FromInteger(35), gridScale))
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                return Brain_2_PanicGetALocker::eBrain2_SearchLocker_2;
            }
            field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame + 30;
            SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            return Brain_2_PanicGetALocker::eBrain2_TurnAround_7;
        }

        case Brain_2_PanicGetALocker::eBrain2_GetPantsOrWings_5:
            if (!field_1E4_pPantsOrWingsTlv)
            {
                field_1E4_pPantsOrWingsTlv = FindPantsOrWings();
            }

            if (GetCurrentMotion() != CrawlingSligMotion::Motion_0_Idle || (field_1E4_pPantsOrWingsTlv && field_1E4_pPantsOrWingsTlv->mTlvSpecificMeaning))
            {
                if (Math_NextRandom() & 1)
                {
                    field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame + 30;
                    return Brain_2_PanicGetALocker::eBrain2_Crawling_8;
                }
                else
                {
                    field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame + 30;
                    SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                    return Brain_2_PanicGetALocker::eBrain2_TurnAround_7;
                }
            }
            else
            {
                field_1E4_pPantsOrWingsTlv->mTlvSpecificMeaning &= 0xFF;
                field_1E4_pPantsOrWingsTlv->mTlvSpecificMeaning |= 1;

                SetNextMotion(CrawlingSligMotion::Motion_1_UsingButton);
                field_1AC_timer = sGnFrame + 20;
                return Brain_2_PanicGetALocker::eBrain2_UsingButton_6;
            }
            break;

        case Brain_2_PanicGetALocker::eBrain2_UsingButton_6:
            if (field_1E4_pPantsOrWingsTlv)
            {
                return field_208_brain_sub_state;
            }
            field_1E4_pPantsOrWingsTlv = FindPantsOrWings();
            return field_208_brain_sub_state;

        case Brain_2_PanicGetALocker::eBrain2_TurnAround_7:
            if (GetCurrentMotion() == CrawlingSligMotion::Motion_11_TurnAround && mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                return Brain_2_PanicGetALocker::eBrain2_Crawling_8;
            }
            return field_208_brain_sub_state;

        case Brain_2_PanicGetALocker::eBrain2_Crawling_8:
            if (HandleEnemyStopper(mVelX))
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;
            }

            if (static_cast<s32>(sGnFrame) > field_1AC_timer)
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;
            }

            if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= field_1C8_say_help_timer)
            {
                return field_208_brain_sub_state;
            }

            SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
            field_1C8_say_help_timer = sGnFrame + 60;
            field_1C0_speak = SligSpeak::eHelp_10;
            return Brain_2_PanicGetALocker::eBrain2_CheckIfWallHit_9;

        case Brain_2_PanicGetALocker::eBrain2_CheckIfWallHit_9:
        {
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_8_Speaking || !(mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }

            FP gridScale = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridScale = -ScaleToGridSize(mSpriteScale);
            }
            else
            {
                gridScale = ScaleToGridSize(mSpriteScale);
            }

            if (WallHit(FP_FromInteger(35), gridScale))
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
                return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;
            }
            else
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
                return Brain_2_PanicGetALocker::eBrain2_Crawling_8;
            }
        }
        break;

        case Brain_2_PanicGetALocker::eBrain2_BeatBySlig_10:
            if (GetCurrentMotion() == CrawlingSligMotion::Motion_14_ShakingToIdle)
            {
                if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, TRUE);
                    field_208_brain_sub_state = Brain_2_PanicGetALocker::eBrain2_DetermineCrawlDirection_0;
                    return field_208_brain_sub_state;
                }
            }
            else if (static_cast<s32>(sGnFrame) > field_1AC_timer)
            {
                Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, TRUE);
            }
            return field_208_brain_sub_state;

        default:
            return field_208_brain_sub_state;
    }
}

s16 CrawlingSlig::Brain_3_Possessed()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    switch (field_208_brain_sub_state)
    {
        case Brain_2_Possessed::eBrain3_StartPossession_0:
            if (static_cast<s32>(sGnFrame) <= field_1AC_timer)
            {
                return field_208_brain_sub_state;
            }
            Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, TRUE);
            return Brain_2_Possessed::eBrain3_Possessed_1;

        case Brain_2_Possessed::eBrain3_Possessed_1:
            if (!Input_IsChanting_45F260() || field_1B8_bChanting)
            {
                return field_208_brain_sub_state;
            }
            field_1AC_timer = sGnFrame + 30;
            SfxPlayMono(SoundEffect::PossessEffect_17, 0);
            Set_AnimAndMotion(CrawlingSligMotion::Motion_12_Shaking, TRUE);
            return Brain_2_Possessed::eBrain3_Unpossessing_2;

        case Brain_2_Possessed::eBrain3_Unpossessing_2:
            if (Input_IsChanting_45F260())
            {
                if (!(static_cast<s32>(sGnFrame) % 4))
                {
                    New_TintChant_Particle(
                        (mSpriteScale * FP_FromInteger(Math_RandomRange(-20, 20))) + mXPos,
                        mYPos - (mSpriteScale * FP_FromInteger(Math_RandomRange(0, 30))),
                        mSpriteScale,
                        Layer::eLayer_0);
                }

                if (static_cast<s32>(sGnFrame) <= field_1AC_timer && sActiveHero->mHealth > FP_FromInteger(0))
                {
                    return field_208_brain_sub_state;
                }

                sControlledCharacter = sActiveHero;
                mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
                gMap.SetActiveCam(field_1BA_prev_level, field_1BC_prev_path, field_1BE_prev_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
                SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
                return field_208_brain_sub_state;
            }
            else
            {
                Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, TRUE);
                return Brain_2_Possessed::eBrain3_Possessed_1;
            }
            break;

        case Brain_2_Possessed::eBrain3_BeatBySlig_3:
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_14_ShakingToIdle)
            {
                if (static_cast<s32>(sGnFrame) > field_1AC_timer)
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, TRUE);
                }
                return field_208_brain_sub_state;
            }

            if (!(mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }

            Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, 1);
            field_208_brain_sub_state = Brain_2_Possessed::eBrain3_Possessed_1;
            return field_208_brain_sub_state;

        default:
            break;
    }
    return field_208_brain_sub_state;
}

s16 CrawlingSlig::Brain_4_GetKilled()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (field_208_brain_sub_state)
    {
        case Brain_4_GetKilled::eBrain4_Unknown_0:
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_7_ToShakingToIdle || !(mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }
            return Brain_4_GetKilled::eBrain4_Vaporize_1;

        case Brain_4_GetKilled::eBrain4_Vaporize_1:
            if (field_1AC_timer < static_cast<s32>((sGnFrame + 80)))
            {
                mSpriteScale -= FP_FromDouble(0.008);
                mRGB.r -= 2;
                mRGB.g -= 2;
                mRGB.b -= 2;
            }

            if (static_cast<s32>(sGnFrame) < field_1AC_timer - 24)
            {
                DeathSmokeEffect(true);
            }

            if (field_1AC_timer < static_cast<s32>(sGnFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return field_208_brain_sub_state;

        case Brain_4_GetKilled::eBrain4_GibsDeath_2:
        {
            relive_new Gibs(
                GibType::Slig_1,
                mXPos,
                mYPos,
                mVelX,
                mVelY,
                mSpriteScale,
                0);

            relive_new Blood(
                mXPos,
                mYPos - (FP_FromInteger(30) * mSpriteScale),
                FP_FromInteger(0),
                FP_FromInteger(0),
                mSpriteScale,
                20);

            New_Smoke_Particles(
                mXPos,
                mYPos - (FP_FromInteger(30) * mSpriteScale),
                mSpriteScale,
                3,
                128u,
                128u,
                128u);

            SfxPlayMono(SoundEffect::KillEffect_64, 128, mSpriteScale);
            SfxPlayMono(SoundEffect::FallingItemHit_47, 90, mSpriteScale);

            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
            mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);

            Set_AnimAndMotion(CrawlingSligMotion::Motion_12_Shaking, TRUE);
            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mHealth = FP_FromInteger(0);
            field_1AC_timer = sGnFrame + 40;
            return Brain_4_GetKilled::eBrain4_SetDead_3;
        }

        case Brain_4_GetKilled::eBrain4_SetDead_3:
            if (static_cast<s32>(sGnFrame) > field_1AC_timer)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return field_208_brain_sub_state;

        case Brain_4_GetKilled::eBrain4_DeathBySlog_4:
            if (static_cast<s32>(sGnFrame) <= field_1AC_timer)
            {
                return field_208_brain_sub_state;
            }
            field_1AC_timer = sGnFrame + 90;
            return Brain_4_GetKilled::eBrain4_Vaporize_1;

        case Brain_4_GetKilled::eBrain4_DeathDrop_5:
            if (static_cast<s32>(sGnFrame) < field_1AC_timer)
            {
                if (!(static_cast<s32>(field_1AC_timer - sGnFrame) % 15))
                {
                    Slig_GameSpeak_SFX_4C04F0(
                        SligSpeak::eHelp_10,
                        // TODO: revisit the logic below
                        static_cast<s16>(2 * (field_1AC_timer & (0xFFFF - sGnFrame))),
                        field_1C2_pitch,
                        this);
                }

                if (static_cast<s32>(sGnFrame) == field_1AC_timer - 6)
                {
                    SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
                }
                return field_208_brain_sub_state;
            }
            else
            {
                Environment_SFX_457A40(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);
                relive_new ScreenShake(0, 0);
                field_1AC_timer = sGnFrame + 30;
                return Brain_4_GetKilled::eBrain4_SetDead_3;
            }
            break;

        default:
            return field_208_brain_sub_state;
    }
}

s16 CrawlingSlig::Brain_5_Transformed()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(field_1D8_obj_id);
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    if (!pObj || pObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 0;
}

void CrawlingSlig::Motion_0_Idle()
{
    HandleCommon();
}

void CrawlingSlig::Motion_1_UsingButton()
{
    // Check for using a slig button which is a button than can trigger an id
    auto pSligButton = static_cast<CrawlingSligButton*>(sObjectIds.Find_Impl(field_1D0_slig_button_id));
    if (pSligButton && mAnim.mCurrentFrame == 8)
    {
        pSligButton->UseButton();
        field_1D0_slig_button_id = -1;
    }
    // If not using a button check if we are on a locker to get pants or wings
    else if (field_1E4_pPantsOrWingsTlv)
    {
        if (mAnim.mCurrentFrame == 7)
        {
            SfxPlayMono(SoundEffect::CrawlingSligTransformStart_93, 0);
        }

        if (static_cast<s32>(sGnFrame) == field_1AC_timer - 1)
        {
            New_DestroyOrCreateObject_Particle(
                mXPos,
                (mSpriteScale * FP_FromInteger(45)) + mYPos,
                mSpriteScale);
        }
        else if (static_cast<s32>(sGnFrame) > field_1AC_timer)
        {
            if (field_1E4_pPantsOrWingsTlv->mTlvType == ReliveTypes::eSligGetPants && ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlgbasicResID, 0, 0))
            {
                // Transform to a walking slig

                SfxPlayMono(SoundEffect::SligSpawn_114, 0);

                auto pWalkingSlig = relive_new Slig(static_cast<relive::Path_Slig*>(field_1E4_pPantsOrWingsTlv), sPathInfo->TLVInfo_From_TLVPtr(field_1E4_pPantsOrWingsTlv));
                if (pWalkingSlig)
                {
                    field_1D8_obj_id = pWalkingSlig->mBaseGameObjectId;

                    pWalkingSlig->mSpriteScale = mSpriteScale;

                    pWalkingSlig->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, mAnim.mFlags.Get(AnimFlags::eBit5_FlipX));

                    if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
                    {
                        pWalkingSlig->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
                        pWalkingSlig->field_146_return_level = field_1BA_prev_level;
                        pWalkingSlig->field_148_return_path = field_1BC_prev_path;
                        pWalkingSlig->field_14A_return_camera = field_1BE_prev_camera;
                        pWalkingSlig->SetBrain(&Slig::Brain_Possessed_2_4BBCF0);
                        pWalkingSlig->field_11C_brain_sub_state = 4;
                        sControlledCharacter = pWalkingSlig;
                    }
                }

                mHealth = FP_FromInteger(0);
            }
            else if (field_1E4_pPantsOrWingsTlv->mTlvType == ReliveTypes::eSligGetWings && ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kFlySligResID, 0, 0))
            {
                // Transform to a flying slig

                SfxPlayMono(SoundEffect::FlyingSligSpawn_113, 0);

                auto pFlyingSlig = relive_new FlyingSlig(static_cast<relive::Path_FlyingSlig*>(field_1E4_pPantsOrWingsTlv), sPathInfo->TLVInfo_From_TLVPtr(field_1E4_pPantsOrWingsTlv));
                if (pFlyingSlig)
                {
                    field_1D8_obj_id = pFlyingSlig->mBaseGameObjectId;
                    pFlyingSlig->mXPos = mXPos;
                    pFlyingSlig->mYPos = mYPos - FP_FromInteger(15);
                    pFlyingSlig->field_294_nextXPos = mXPos;
                    pFlyingSlig->field_298_nextYPos = pFlyingSlig->mYPos;
                    pFlyingSlig->mSpriteScale = mSpriteScale;
                    pFlyingSlig->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, mAnim.mFlags.Get(AnimFlags::eBit5_FlipX));

                    if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
                    {
                        pFlyingSlig->ToPlayerControlled_4360C0();
                        pFlyingSlig->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
                        pFlyingSlig->field_2A0_abe_level = field_1BA_prev_level;
                        pFlyingSlig->field_2A2_abe_path = field_1BC_prev_path;
                        pFlyingSlig->field_2A4_abe_camera = field_1BE_prev_camera;
                        sControlledCharacter = pFlyingSlig;
                        pFlyingSlig->field_2A8_max_x_speed = (FP_FromDouble(5.5) * mSpriteScale);
                        pFlyingSlig->field_2AC_up_vel = (-FP_FromDouble(5.5) * mSpriteScale);
                        pFlyingSlig->field_2B0_down_vel = (FP_FromDouble(5.5) * mSpriteScale);
                        pFlyingSlig->field_2B4_max_slow_down = (FP_FromDouble(0.3) * mSpriteScale);
                        pFlyingSlig->field_2B8_max_speed_up = (FP_FromDouble(0.8) * mSpriteScale);
                    }
                    else
                    {
                        pFlyingSlig->SetBrain(&FlyingSlig::Brain_17_FromCrawlingSlig);
                    }
                }
                mHealth = FP_FromInteger(0);
            }
            else
            {
                // Impossible case - slig will go away and not transform into anything ??
            }

            // Final transform
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            SetBrain(&CrawlingSlig::Brain_5_Transformed);
            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            Set_AnimAndMotion(CrawlingSligMotion::Motion_0_Idle, TRUE);
            mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
            mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
            SetType(ReliveTypes::eNone);
        }
    }
    else
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            HandleCommon();
        }
    }
}

void CrawlingSlig::Motion_2_WakingUp()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eSnoozeParticle)
        {
            static_cast<SnoozeParticle*>(pObj)->mState = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon();
    }
}

void CrawlingSlig::Motion_3_Crawling()
{
    if (CanCrawl())
    {
        if (mAnim.mCurrentFrame == 3 || mAnim.mCurrentFrame == 6)
        {
            Slig_SoundEffect_4BFFE0(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
        }
        else if (mAnim.mCurrentFrame == 11)
        {
            if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (GetNextMotion() == CrawlingSligMotion::Motion_0_Idle || GetNextMotion() == CrawlingSligMotion::Motion_11_TurnAround || GetNextMotion() == CrawlingSligMotion::Motion_7_ToShakingToIdle || GetNextMotion() == CrawlingSligMotion::Motion_8_Speaking)
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_15_EndCrawling, FALSE);
                }
            }
            else
            {
                if ((mVelX > FP_FromInteger(0) && Input().isPressed(InputCommands::Enum::eLeft)) || (mVelX < FP_FromInteger(0) && Input().isPressed(InputCommands::Enum::eRight)) || !(Input().isPressed(InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_15_EndCrawling, TRUE);
                }
            }
            MapFollowMe(TRUE);
        }
    }
}

void CrawlingSlig::Motion_4_StartFalling()
{
    Motion_5_Falling();
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Set_AnimAndMotion(CrawlingSligMotion::Motion_5_Falling, TRUE);
    }
}

void CrawlingSlig::Motion_5_Falling()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (mSpriteScale * field_1B0_velx_scale_factor);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (mSpriteScale * field_1B0_velx_scale_factor) + mVelX;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
    {
        SetActiveCameraDelayedFromDir();
    }

    if (bCollision)
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = pLine;
                PlatformCollide();
                mYPos = hitY;
                mXPos = hitX;
                MapFollowMe(TRUE);
                if ((hitY - BaseAliveGameObjectLastLineYPos) > (ScaleToGridSize(mSpriteScale) * FP_FromInteger(5)))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_GibsDeath_2;
                }
                else
                {
                    Set_AnimAndMotion(CrawlingSligMotion::Motion_6_Landing, TRUE);
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
                mVelX = -mVelX / FP_FromInteger(2);
                break;

            default:
                return;
        }
    }
}

void CrawlingSlig::Motion_6_Landing()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, TRUE);
    }
}

void CrawlingSlig::Motion_7_ToShakingToIdle()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (GetNextMotion() == CrawlingSligMotion::Motion_14_ShakingToIdle)
        {
            Set_AnimAndMotion(CrawlingSligMotion::Motion_14_ShakingToIdle, TRUE);
        }
    }
}

void CrawlingSlig::Motion_8_Speaking()
{
    if (mAnim.mCurrentFrame == 2 && field_1C0_speak != SligSpeak::eNone)
    {
        if (gMap.mCurrentPath == mCurrentPath && gMap.mCurrentLevel == mCurrentLevel && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            Slig_GameSpeak_SFX_4C04F0(field_1C0_speak, 0, 0, this);
        }
        field_1C0_speak = SligSpeak::eNone;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon();
    }
}

void CrawlingSlig::Motion_9_Snoozing()
{
    if (GetNextMotion() == CrawlingSligMotion::Motion_2_WakingUp)
    {
        Set_AnimAndMotion(CrawlingSligMotion::Motion_2_WakingUp, TRUE);
    }
    else if (!(sGnFrame & 31))
    {
        if ((sGnFrame / 25) & 1)
        {
            Slig_SoundEffect_4BFFE0(SligSfx::eSnooze1_5, this);
        }
        else
        {
            Slig_SoundEffect_4BFFE0(SligSfx::eSnooze2_4, this);
        }

        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            FP xOff = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -(mSpriteScale * FP_FromInteger(-10));
            }
            else
            {
                xOff = (mSpriteScale * FP_FromInteger(-10));
            }

            const FP yOff = (mSpriteScale * FP_FromInteger(-10));
            relive_new SnoozeParticle(
                mXPos + xOff,
                mYPos + yOff,
                mAnim.mRenderLayer,
                mAnim.field_14_scale);
        }
    }
}

void CrawlingSlig::Motion_10_PushingWall()
{
    MoveOnLine();

    if (mAnim.mCurrentFrame == 4 || mAnim.mCurrentFrame == 8)
    {
        MapFollowMe(TRUE);
        mVelX = FP_FromInteger(0);
        Slig_SoundEffect_4BFFE0(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
    }

    if (BrainIs(&CrawlingSlig::Brain_3_Possessed))
    {
        const bool flipX = mAnim.mFlags.Get(AnimFlags::eBit5_FlipX);
        if ((!flipX && Input().isPressed(InputCommands::Enum::eLeft)) || (flipX && Input().isPressed(InputCommands::Enum::eRight)) || !(Input().isPressed(InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
        {
            Set_AnimAndMotion(CrawlingSligMotion::Motion_17_EndPushingWall, TRUE);
        }
        else
        {
            FP yPos = {};
            if (flipX)
            {
                yPos = -ScaleToGridSize(mSpriteScale);
            }
            else
            {
                yPos = ScaleToGridSize(mSpriteScale);
            }

            if (!WallHit(mSpriteScale * FP_FromInteger(30), yPos))
            {
                Set_AnimAndMotion(CrawlingSligMotion::Motion_3_Crawling, TRUE);
            }
        }
    }
    else
    {
        if (GetNextMotion() == CrawlingSligMotion::Motion_0_Idle || GetNextMotion() == CrawlingSligMotion::Motion_1_UsingButton || GetNextMotion() == CrawlingSligMotion::Motion_11_TurnAround || GetNextMotion() == CrawlingSligMotion::Motion_7_ToShakingToIdle || GetNextMotion() == CrawlingSligMotion::Motion_8_Speaking)
        {
            Set_AnimAndMotion(CrawlingSligMotion::Motion_17_EndPushingWall, FALSE);
        }
    }
}

void CrawlingSlig::Motion_11_TurnAround()
{
    if (mAnim.mCurrentFrame == 8)
    {
        Slig_SoundEffect_4BFFE0(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
        MapFollowMe(TRUE);
        HandleCommon();
    }
}

void CrawlingSlig::Motion_12_Shaking()
{
    // Do nothing
}

void CrawlingSlig::Motion_13_Empty()
{
    
}

void CrawlingSlig::Motion_14_ShakingToIdle()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mHealth > FP_FromInteger(0))
        {
            mHealth = FP_FromInteger(1);
        }
        ToIdle();
    }
}

void CrawlingSlig::Motion_15_EndCrawling()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon();
    }
}

void CrawlingSlig::Motion_16_IdleToPushingWall()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(Math_RandomRange(static_cast<s32>(SligSpeak::eOuch1_13), static_cast<s32>(SligSpeak::eOuch2_14))), 0, 0, this);
        Set_AnimAndMotion(CrawlingSligMotion::Motion_10_PushingWall, TRUE);
    }
}

void CrawlingSlig::Motion_17_EndPushingWall()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void CrawlingSlig::HandleCommon()
{
    MapFollowMe(TRUE);

    if (BrainIs(&CrawlingSlig::Brain_3_Possessed) && field_208_brain_sub_state == Brain_2_Possessed::eBrain3_Possessed_1)
    {
        if (Input().isPressed(InputCommands::Enum::eRight))
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            }
            else
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
            }
        }
        else if (Input().isPressed(InputCommands::Enum::eLeft))
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                SetNextMotion(CrawlingSligMotion::Motion_3_Crawling);
            }
            else
            {
                SetNextMotion(CrawlingSligMotion::Motion_11_TurnAround);
            }
        }
        else if (Input().isHeld(InputCommands::Enum::eUp))
        {
            field_1E4_pPantsOrWingsTlv = FindPantsOrWings();
            if (field_1E4_pPantsOrWingsTlv)
            {
                if (!field_1E4_pPantsOrWingsTlv->mTlvSpecificMeaning)
                {
                    field_1E4_pPantsOrWingsTlv->mTlvSpecificMeaning &= 0xFF;
                    field_1E4_pPantsOrWingsTlv->mTlvSpecificMeaning |= 1;
                    SetNextMotion(CrawlingSligMotion::Motion_1_UsingButton);
                    field_1AC_timer = sGnFrame + 20;
                }
            }
            else
            {
                auto pSligButton = FindSligButton();
                if (pSligButton)
                {
                    SetNextMotion(CrawlingSligMotion::Motion_1_UsingButton);
                    field_1D0_slig_button_id = pSligButton->mBaseGameObjectId;
                }
                else
                {
                    SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
                    field_1C0_speak = SligSpeak::eWhat_9;
                }
            }
        }
        if (Input().isHeld(InputCommands::Enum::eGameSpeak1))
        {
            field_1C0_speak = SligSpeak::eHi_0;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak3))
        {
            field_1C0_speak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak::eGetHim_2 : SligSpeak ::eFreeze_8;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak4))
        {
            field_1C0_speak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak ::eFreeze_8 : SligSpeak::eGetHim_2;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak2))
        {
            field_1C0_speak = SligSpeak::eHereBoy_1;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak6))
        {
            field_1C0_speak = SligSpeak::eBullshit_5;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak7))
        {
            field_1C0_speak = SligSpeak::eLookOut_6;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak5))
        {
            field_1C0_speak = SligSpeak::eBullshit2_7;
        }
        else if (Input().isHeld(InputCommands::Enum::eGameSpeak8))
        {
            field_1C0_speak = SligSpeak::eLaugh_3;
        }

        if (field_1C0_speak != SligSpeak::eNone)
        {
            SetNextMotion(CrawlingSligMotion::Motion_8_Speaking);
        }
    }

    switch (GetNextMotion())
    {
        case CrawlingSligMotion::None: // TODO: None constant
            if (GetCurrentMotion() != CrawlingSligMotion::Motion_0_Idle)
            {
                SetNextMotion(CrawlingSligMotion::Motion_0_Idle);
                Set_AnimAndMotion(GetNextMotion(), TRUE);
                break;
            }
            break;

        case CrawlingSligMotion::Motion_0_Idle:
        case CrawlingSligMotion::Motion_1_UsingButton:
        case CrawlingSligMotion::Motion_7_ToShakingToIdle:
        case CrawlingSligMotion::Motion_8_Speaking:
        case CrawlingSligMotion::Motion_11_TurnAround:
            Set_AnimAndMotion(GetNextMotion(), TRUE);
            break;

        case CrawlingSligMotion::Motion_3_Crawling:
        {
            FP gridScale = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridScale = -ScaleToGridSize(mSpriteScale);
            }
            else
            {
                gridScale = ScaleToGridSize(mSpriteScale);
            }

            if (WallHit(mSpriteScale * FP_FromInteger(30), gridScale))
            {
                if (sControlledCharacter == this)
                {
                    SetNextMotion(CrawlingSligMotion::Motion_16_IdleToPushingWall);
                }
            }
            Set_AnimAndMotion(GetNextMotion(), TRUE);
            break;
        }

        default:
            return;
    }
}

const FP sCrawlingSligXVels_54471C[15] = {
    FP_FromDouble(0),
    FP_FromDouble(0),
    FP_FromDouble(0),
    FP_FromDouble(1.17),
    FP_FromDouble(2.56),
    FP_FromDouble(2.85),
    FP_FromDouble(2.23),
    FP_FromDouble(1.87),
    FP_FromDouble(3.47),
    FP_FromDouble(3.61),
    FP_FromDouble(3.96),
    FP_FromDouble(2.69),
    FP_FromDouble(0),
    FP_FromDouble(0),
    FP_FromDouble(0)};

s16 CrawlingSlig::CanCrawl()
{
    mVelX = sCrawlingSligXVels_54471C[mAnim.mCurrentFrame];

    FP gridScale = ScaleToGridSize(mSpriteScale);
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mVelX = -mVelX;
        gridScale = -gridScale;
    }

    mVelX = (mVelX * mSpriteScale);

    if (sControlledCharacter == this && WallHit(mSpriteScale * FP_FromInteger(30), gridScale))
    {
        field_1B0_velx_scale_factor = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
        Set_AnimAndMotion(CrawlingSligMotion::Motion_10_PushingWall, TRUE);
        const s32 snappedX = SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos));
        mVelX = ((FP_FromInteger(snappedX) - mXPos) / FP_FromInteger(4));
        Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(Math_RandomRange(static_cast<s32>(SligSpeak::eOuch1_13), static_cast<s32>(SligSpeak::eOuch2_14))), 0, 0, this);
        return FALSE;
    }
    else
    {
        MoveOnLine();
        return BaseAliveGameObjectCollisionLine != nullptr;
    }
}

void CrawlingSlig::MoveOnLine()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObject_PlatformId = -1;
                }
            }
            else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PlatformCollide();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            BaseAliveGameObjectLastLineYPos = mYPos;
            if (GetCurrentMotion() == CrawlingSligMotion::Motion_3_Crawling)
            {
                Set_AnimAndMotion(CrawlingSligMotion::Motion_4_StartFalling, TRUE);
            }
            field_1B0_velx_scale_factor = FP_FromInteger(1);
            mXPos = mXPos + mVelX;
        }
    }
    else
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        Set_AnimAndMotion(CrawlingSligMotion::Motion_5_Falling, TRUE);
    }
}

void CrawlingSlig::PlatformCollide()
{
    const PSX_RECT bRect = VGetBoundingRect();

    PSX_Point xy = {bRect.x, bRect.y};
    PSX_Point wh = {bRect.w, bRect.h};
    xy.y += 5;
    wh.y += 5;

    VOnCollisionWith(
        xy,
        wh,
        ObjList_5C1B78, 1,
        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
}
