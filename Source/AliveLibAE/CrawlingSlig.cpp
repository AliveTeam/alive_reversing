#include "stdafx.h"
#include "CrawlingSlig.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "MusicController.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "DDCheat.hpp"
#include "SwitchStates.hpp"
#include "SnoozeParticle.hpp"
#include "Input.hpp"
#include "ObjectIds.hpp"
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

const AnimId sCrawlingSligFrameTableOffsets_551470[18] = {
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
          {&CrawlingSlig::M_Idle_0_41B260,
           &CrawlingSlig::M_UsingButton_1_41B890,
           &CrawlingSlig::M_WakingUp_2_41BF00,
           &CrawlingSlig::M_Crawling_3_41B280,
           &CrawlingSlig::M_StartFalling_4_41B620,
           &CrawlingSlig::M_Falling_5_41B650,
           &CrawlingSlig::M_Landing_6_41B870,
           &CrawlingSlig::M_ToShakingToIdle_7_41C010,
           &CrawlingSlig::M_Speaking_8_41BF70,
           &CrawlingSlig::M_Snoozing_9_41BD80,
           &CrawlingSlig::M_PushingWall_10_41B400,
           &CrawlingSlig::M_TurnAround_11_41B590,
           &CrawlingSlig::M_Shaking_12_418C30,
           &CrawlingSlig::M_Empty_13_418C50,
           &CrawlingSlig::M_ShakingToIdle_14_41C040,
           &CrawlingSlig::M_EndCrawling_15_41B600,
           &CrawlingSlig::M_IdleToPushingWall_16_41B3C0,
           &CrawlingSlig::M_EndPushingWall_17_41B3A0});


const static BrainFunctionData<TCrawlingSligBrainFn> sCrawlingSligBrainTable[] = {
    {&CrawlingSlig::Brain_0_Sleeping_419DE0, 0x401D1B, "Brain_0_Sleeping"},
    {&CrawlingSlig::Brain_1_Idle_419F60, 0x40340E, "Brain_1_Idle"},
    {&CrawlingSlig::Brain_2_PanicGetALocker_419FE0, 0x419FE0, "Brain_2_PanicGetALocker"},
    {&CrawlingSlig::Brain_3_Possessed_41A5B0, 0x404539, "Brain_3_Possessed"},
    {&CrawlingSlig::Brain_4_GetKilled_41A880, 0x403265, "Brain_4_GetKilled"},
    {&CrawlingSlig::Brain_5_Transformed_41ADF0, 0x40484A, "Brain_5_Transformed"},
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

CrawlingSlig::CrawlingSlig(Path_CrawlingSlig* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(2)
{
    field_1DC_unused = -1;
    SetType(ReliveTypes::eCrawlingSlig);

    const AnimRecord& rec = AnimRec(AnimId::CrawlingSlig_Idle);
    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, 1, 0));
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, field_10_resources_array.ItemAt(0), 1, 1);

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

    mShadow = ae_new<Shadow>();

    field_118_tlvInfo = tlvInfo;
    field_1E8_tlv = *pTlv;

    if (field_1E8_tlv.field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.mRenderLayer = Layer::eLayer_8;
        field_D6_scale = 0;
    }
    else if (field_1E8_tlv.field_10_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.mRenderLayer = Layer::eLayer_27;
        field_D6_scale = 1;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (field_1E8_tlv.field_14_state == Path_CrawlingSlig::State::eAwake_2)
    {
        Set_AnimAndMotion_419890(CrawlingSligMotion::M_Idle_0_41B260, TRUE);
        SetBrain(&CrawlingSlig::Brain_1_Idle_419F60);
    }
    else
    {
        if (field_CC_sprite_scale == FP_FromInteger(1))
        {
            field_20_animation.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        }
        else
        {
            field_20_animation.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        }
        Set_AnimAndMotion_419890(CrawlingSligMotion::M_Snoozing_9_41BD80, TRUE);
        SetBrain(&CrawlingSlig::Brain_0_Sleeping_419DE0);
    }

    field_208_brain_sub_state = 0;
    if (field_1E8_tlv.field_16_crawl_direction == Path_CrawlingSlig::CrawlDirection::eRandom_2)
    {
        field_1E0_crawl_direction = NextRandom() ? Path_CrawlingSlig::CrawlDirection::eRight_1 : Path_CrawlingSlig::CrawlDirection::eLeft_0;
    }
    else
    {
        field_1E0_crawl_direction = field_1E8_tlv.field_16_crawl_direction;
    }

    field_1C6_unused = field_1E8_tlv.field_18_panic_switch_id;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(37),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 0x10)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
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

    auto pTlv = static_cast<Path_CrawlingSlig*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_44_tlvInfo));
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kCrawlingSligResID_449, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("CRAWLSLG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBLOW.BAN", nullptr);
    }

    auto pCrawlingSlig = ae_new<CrawlingSlig>(pTlv, pState->field_44_tlvInfo);
    if (pCrawlingSlig)
    {
        pCrawlingSlig->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

        if (pState->field_40_bIsControlled)
        {
            sControlledCharacter_5C1B8C = pCrawlingSlig;
        }

        pCrawlingSlig->BaseAliveGameObjectPathTLV = nullptr;
        pCrawlingSlig->BaseAliveGameObjectCollisionLine = nullptr;

        pCrawlingSlig->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_8_xpos;
        pCrawlingSlig->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_C_ypos;
        pCrawlingSlig->field_C4_velx = pState->field_10_velx;
        pCrawlingSlig->field_C8_vely = pState->field_14_vely;

        pCrawlingSlig->field_1B0_velx_scale_factor = pState->field_58_velx_scale_factor;

        pCrawlingSlig->field_C0_path_number = pState->field_18_path_number;
        pCrawlingSlig->field_C2_lvl_number = MapWrapper::FromAE(pState->field_1A_lvl_number);
        pCrawlingSlig->field_CC_sprite_scale = pState->field_1C_sprite_scale;

        pCrawlingSlig->field_1A4_r = pState->mRingRed;
        pCrawlingSlig->field_D0_r = pState->mRingRed;

        pCrawlingSlig->field_1A6_g = pState->mRingGreen;
        pCrawlingSlig->field_D2_g = pState->mRingGreen;

        pCrawlingSlig->field_1A8_b = pState->mRingBlue;
        pCrawlingSlig->field_D4_b = pState->mRingBlue;

        pCrawlingSlig->mCurrentMotion = pState->field_28_current_motion;

        const AnimRecord& rec = AnimRec(sCrawlingSligFrameTableOffsets_551470[pState->field_28_current_motion]);
        pCrawlingSlig->field_20_animation.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
        pCrawlingSlig->field_20_animation.field_92_current_frame = pState->field_2A_anim_cur_frame;

        pCrawlingSlig->field_20_animation.mFrameChangeCounter = pState->field_2C_anim_frame_change_counter;

        pCrawlingSlig->field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->field_26_bFlipX & 1);
        pCrawlingSlig->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_2E_bRender & 1);

        pCrawlingSlig->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_bDrawable & 1);

        if (IsLastFrame(&pCrawlingSlig->field_20_animation))
        {
            pCrawlingSlig->field_20_animation.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
        }

        pCrawlingSlig->mHealth = pState->field_30_health;
        pCrawlingSlig->mCurrentMotion = pState->field_34_cur_motion;
        pCrawlingSlig->mNextMotion = pState->field_36_next_motion;
        pCrawlingSlig->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_38_last_line_ypos);
        pCrawlingSlig->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
        pCrawlingSlig->field_1AC_timer = pState->field_54_timer;
        pCrawlingSlig->BaseAliveGameObjectCollisionLineType = pState->field_3A_line_type;
        pCrawlingSlig->field_118_tlvInfo = pState->field_44_tlvInfo;
        pCrawlingSlig->SetBrain(sCrawlingSligBrainTable[pState->field_48_brain_idx].mOurFn);
        pCrawlingSlig->field_208_brain_sub_state = pState->field_50_brain_sub_state;
        pCrawlingSlig->field_1B8_bChanting = pState->field_5E_bChanting;
        pCrawlingSlig->field_1BA_prev_level = MapWrapper::FromAE(pState->field_60_prev_leve);
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

    pState->field_8_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_C_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_58_velx_scale_factor = field_1B0_velx_scale_factor;

    pState->field_18_path_number = field_C0_path_number;
    pState->field_1A_lvl_number = MapWrapper::ToAE(field_C2_lvl_number);
    pState->field_1C_sprite_scale = field_CC_sprite_scale;

    pState->mRingRed = field_D0_r;
    pState->mRingGreen = field_D2_g;
    pState->mRingBlue = field_D4_b;
    pState->field_26_bFlipX = field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_28_current_motion = mCurrentMotion;
    pState->field_2A_anim_cur_frame = field_20_animation.field_92_current_frame;
    pState->field_2C_anim_frame_change_counter = field_20_animation.mFrameChangeCounter;
    pState->field_2F_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2E_bRender = field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = mHealth;
    pState->field_34_cur_motion = mCurrentMotion;
    pState->field_36_next_motion = mNextMotion;
    pState->field_3A_line_type = -1;

    // TODO: Check correct
    pState->field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_3A_line_type = BaseAliveGameObjectCollisionLine->field_8_type;
    }

    pState->field_40_bIsControlled = (this == sControlledCharacter_5C1B8C);
    pState->field_54_timer = field_1AC_timer;
    pState->field_44_tlvInfo = field_118_tlvInfo;
    pState->field_48_brain_idx = 0;

    s32 idx = 0;
    for (const auto& fn : sCrawlingSligBrainTable)
    {
        if (BrainIs(fn.mOurFn))
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
    Set_AnimAndMotion_419890(CrawlingSligMotion::M_Shaking_12_418C30, TRUE);
    SetBrain(&CrawlingSlig::Brain_3_Possessed_41A5B0);
    field_208_brain_sub_state = 0;
    field_1AC_timer = sGnFrame + 35;
    field_1BA_prev_level = gMap.mCurrentLevel;
    field_1BC_prev_path = gMap.mCurrentPath;
    field_1BE_prev_camera = gMap.mCurrentCamera;
}

void CrawlingSlig::Set_AnimAndMotion_419890(s16 currentMotion, s16 bClearNextMotion)
{
    const AnimRecord& rec = AnimRec(sCrawlingSligFrameTableOffsets_551470[currentMotion]);
    field_20_animation.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
    mCurrentMotion = currentMotion;

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
    const AnimRecord& rec = AnimRec(sCrawlingSligFrameTableOffsets_551470[mCurrentMotion]);
    field_20_animation.Set_Animation_Data(rec.mFrameTableOffset, ppRes);
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
    if (Event_Get(kEventDeathReset))
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
                sCollisions_DArray_5C1128->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
                    &BaseAliveGameObjectCollisionLine,
                    &mBaseAnimatedWithPhysicsGameObject_XPos,
                    &mBaseAnimatedWithPhysicsGameObject_YPos,
                    1 << BaseAliveGameObjectCollisionLineType);
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

        const FP oldX = mBaseAnimatedWithPhysicsGameObject_XPos;
        const FP oldY = mBaseAnimatedWithPhysicsGameObject_YPos;

        (this->*sCrawlingSlig_motions_551428[mCurrentMotion])();

        if (oldX != mBaseAnimatedWithPhysicsGameObject_XPos || oldY != mBaseAnimatedWithPhysicsGameObject_YPos)
        {
            auto pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos);

            VOn_TLV_Collision(pTlv);
        }
    }
}

s16 CrawlingSlig::HandleEnemyStopper(FP /*velX*/)
{
    FP gridSizeDirected = ScaleToGridSize(field_CC_sprite_scale);
    Path_EnemyStopper::StopDirection direction = Path_EnemyStopper::StopDirection::Both_2;
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        direction = Path_EnemyStopper::StopDirection::Left_0;
        gridSizeDirected = -gridSizeDirected;
    }
    else
    {
        direction = Path_EnemyStopper::StopDirection::Right_1;
    }

    if (WallHit(field_CC_sprite_scale * FP_FromInteger(30), gridSizeDirected * FP_FromDouble(1.5)))
    {
        return 1;
    }

    const FP gridSize = ScaleToGridSize(field_CC_sprite_scale);
    auto pSlamDoor = static_cast<Path_SlamDoor*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + gridSizeDirected),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - gridSize),
        TlvTypes::SlamDoor_85));
    BaseAliveGameObjectPathTLV = pSlamDoor;

    if (pSlamDoor && ((pSlamDoor->field_10_bStart_closed == Choice_short::eYes_1 && !SwitchStates_Get(pSlamDoor->field_14_switch_id)) || (pSlamDoor->field_10_bStart_closed == Choice_short::eNo_0 && SwitchStates_Get(pSlamDoor->field_14_switch_id))))
    {
        return 1;
    }

    auto pStopper = static_cast<Path_EnemyStopper*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + gridSizeDirected),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - gridSize),
        TlvTypes::EnemyStopper_47));
    BaseAliveGameObjectPathTLV = pStopper;

    return pStopper && (pStopper->field_10_stop_direction == direction || pStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Both_2) && SwitchStates_Get(pStopper->field_12_switch_id);
}

Path_TLV* CrawlingSlig::FindPantsOrWings()
{
    Path_TLV* pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(nullptr, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
    while (pTlvIter)
    {
        if (pTlvIter->field_4_type == TlvTypes::SligGetPants_104 || pTlvIter->field_4_type == TlvTypes::SligGetWings_105)
        {
            return pTlvIter;
        }
        pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlvIter, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
    }
    return nullptr;
}

BaseGameObject* CrawlingSlig::FindSligButton()
{
    return FindObjectOfType(ReliveTypes::eSligButton, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * field_CC_sprite_scale));
}

void CrawlingSlig::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObjectId = -1;
        Set_AnimAndMotion_419890(CrawlingSligMotion::M_StartFalling_4_41B620, TRUE);
    }
}

void CrawlingSlig::VOn_TLV_Collision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                SetBrain(&CrawlingSlig::Brain_4_GetKilled_41A880);
                field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_DeathDrop_5;
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                Event_Broadcast(kEventMudokonComfort, this);
                Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_1C2_pitch, this);
                field_1AC_timer = sGnFrame + 60;
            }
        }

        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            pTlv,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);
    }
}

s16 CrawlingSlig::VTakeDamage(BaseGameObject* pFrom)
{
    if (!BrainIs(&CrawlingSlig::Brain_5_Transformed_41ADF0))
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
                if (!BrainIs(&CrawlingSlig::Brain_4_GetKilled_41A880))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled_41A880);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_GibsDeath_2;
                    Event_Broadcast(kEventMudokonComfort, this);
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
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled_41A880);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_GibsDeath_2;
                    return 0;
                }

                Set_AnimAndMotion_419890(eSligMotions::M_Falling_7_4B42D0, TRUE);
                Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, 0, this);

                if (BrainIs(&CrawlingSlig::Brain_2_PanicGetALocker_419FE0))
                {
                    field_208_brain_sub_state = Brain_2_PanicGetALocker::eBrain2_BeatBySlig_10;
                }
                else if (BrainIs(&CrawlingSlig::Brain_3_Possessed_41A5B0))
                {
                    field_208_brain_sub_state = Brain_2_Possessed::eBrain3_BeatBySlig_3;
                }
                else
                {
                    SetBrain(&CrawlingSlig::Brain_2_PanicGetALocker_419FE0);
                    field_208_brain_sub_state = Brain_2_PanicGetALocker::eBrain2_BeatBySlig_10;
                }
            }
                return 1;

            case ReliveTypes::eSlog:
                if (!BrainIs(&CrawlingSlig::Brain_4_GetKilled_41A880))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled_41A880);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_DeathBySlog_4;
                    field_C8_vely = FP_FromInteger(0);
                    field_C4_velx = FP_FromInteger(0);
                    mHealth = FP_FromInteger(0);
                    MapFollowMe(TRUE);
                    field_1AC_timer = sGnFrame + 15;
                    Set_AnimAndMotion_419890(CrawlingSligMotion::M_Empty_13_418C50, TRUE);
                    Event_Broadcast(kEventMudokonComfort, this);
                }
                return 1;

            case ReliveTypes::eElectrocute:
                if (!BrainIs(&CrawlingSlig::Brain_4_GetKilled_41A880))
                {
                    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                    mHealth = FP_FromInteger(0);
                    field_1AC_timer = sGnFrame + 1;
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled_41A880);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_SetDead_3;
                    Event_Broadcast(kEventMudokonComfort, this);
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
    ::SetBrain(fn, field_204_brain_state, sCrawlingSligBrainTable);
}

bool CrawlingSlig::BrainIs(TCrawlingSligBrainFn fn)
{
    return ::BrainIs(fn, field_204_brain_state, sCrawlingSligBrainTable);
}

CrawlingSlig::~CrawlingSlig()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
        if (gMap.mLevel != EReliveLevelIds::eMenu)
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
    if (mHealth > FP_FromInteger(0) || field_1E8_tlv.field_1A_respawn_on_death == Choice_short::eYes_1)
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 1);
    }
}

Bool32 CrawlingSlig::PanicOn()
{
    return SwitchStates_Get(field_1E8_tlv.field_18_panic_switch_id) != 0;
}

void CrawlingSlig::ToIdle()
{
    field_1B0_velx_scale_factor = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    Set_AnimAndMotion_419890(CrawlingSligMotion::M_Idle_0_41B260, FALSE);
    MapFollowMe(TRUE);
}

enum Brain_0_Sleeping
{
    eBrain0_Sleeping_0 = 0,
    eBrain0_WakingUp_1 = 1,
    eBrain0_IsAwake_2 = 2
};

s16 CrawlingSlig::Brain_0_Sleeping_419DE0()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
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

        mNextMotion = CrawlingSligMotion::M_WakingUp_2_41BF00;
        return Brain_0_Sleeping::eBrain0_IsAwake_2;
    }

    if (field_208_brain_sub_state != Brain_0_Sleeping::eBrain0_IsAwake_2 ||
        mCurrentMotion != CrawlingSligMotion::M_WakingUp_2_41BF00 ||
        !(field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
    {
        return field_208_brain_sub_state;
    }

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.mRenderLayer = Layer::eLayer_27;
    }
    else
    {
        field_20_animation.mRenderLayer = Layer::eLayer_8;
    }

    ToIdle();
    SetBrain(&CrawlingSlig::Brain_2_PanicGetALocker_419FE0);
    return 0;
}

s16 CrawlingSlig::Brain_1_Idle_419F60()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    if (PanicOn())
    {
        SetBrain(&CrawlingSlig::Brain_2_PanicGetALocker_419FE0);
    }
    return 0;
}

s16 CrawlingSlig::Brain_2_PanicGetALocker_419FE0()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
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
            if ((field_1E0_crawl_direction != Path_CrawlingSlig::CrawlDirection::eRight_1 || !(field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))) && (field_1E0_crawl_direction != Path_CrawlingSlig::CrawlDirection::eLeft_0 || field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
            {
                mNextMotion = CrawlingSligMotion::M_Crawling_3_41B280;
                return Brain_2_PanicGetALocker::eBrain2_SearchLocker_2;
            }
            mNextMotion = CrawlingSligMotion::M_TurnAround_11_41B590;
            return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;

        case Brain_2_PanicGetALocker::eBrain2_Falling_1:
            if (mCurrentMotion != CrawlingSligMotion::M_Idle_0_41B260)
            {
                return field_208_brain_sub_state;
            }
            return Brain_2_PanicGetALocker::eBrain2_DetermineCrawlDirection_0;

        case Brain_2_PanicGetALocker::eBrain2_SearchLocker_2:
            if (HandleEnemyStopper(field_C4_velx))
            {
                field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame + 30;
                mNextMotion = CrawlingSligMotion::M_TurnAround_11_41B590;
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
                mNextMotion = CrawlingSligMotion::M_Speaking_8_41BF70;
                field_1C8_say_help_timer = sGnFrame + 60;
                field_1C0_speak = SligSpeak::eHelp_10;
                return Brain_2_PanicGetALocker::eBrain2_SearchLockerOrTurnAround_4;
            }
            break;

        case Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3:
            if (mCurrentMotion != CrawlingSligMotion::M_TurnAround_11_41B590 || !(field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }
            mNextMotion = CrawlingSligMotion::M_Crawling_3_41B280;
            return Brain_2_PanicGetALocker::eBrain2_SearchLocker_2;

        case Brain_2_PanicGetALocker::eBrain2_SearchLockerOrTurnAround_4:
        {
            if (mCurrentMotion != CrawlingSligMotion::M_Speaking_8_41BF70 || !(field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }

            FP gridScale = {};
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridScale = -ScaleToGridSize(field_CC_sprite_scale);
            }
            else
            {
                gridScale = ScaleToGridSize(field_CC_sprite_scale);
            }

            if (!WallHit(FP_FromInteger(35), gridScale))
            {
                mNextMotion = CrawlingSligMotion::M_Crawling_3_41B280;
                return Brain_2_PanicGetALocker::eBrain2_SearchLocker_2;
            }
            field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame + 30;
            mNextMotion = CrawlingSligMotion::M_TurnAround_11_41B590;
            return Brain_2_PanicGetALocker::eBrain2_TurnAround_7;
        }

        case Brain_2_PanicGetALocker::eBrain2_GetPantsOrWings_5:
            if (!field_1E4_pPantsOrWingsTlv)
            {
                field_1E4_pPantsOrWingsTlv = FindPantsOrWings();
            }

            if (mCurrentMotion != CrawlingSligMotion::M_Idle_0_41B260 || (field_1E4_pPantsOrWingsTlv && field_1E4_pPantsOrWingsTlv->field_1_tlv_state))
            {
                if (Math_NextRandom() & 1)
                {
                    field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame + 30;
                    return Brain_2_PanicGetALocker::eBrain2_Crawling_8;
                }
                else
                {
                    field_1AC_timer = (Math_NextRandom() & 15) + sGnFrame + 30;
                    mNextMotion = CrawlingSligMotion::M_TurnAround_11_41B590;
                    return Brain_2_PanicGetALocker::eBrain2_TurnAround_7;
                }
            }
            else
            {
                field_1E4_pPantsOrWingsTlv->field_1_tlv_state &= 0xFF;
                field_1E4_pPantsOrWingsTlv->field_1_tlv_state |= 1;

                mNextMotion = CrawlingSligMotion::M_UsingButton_1_41B890;
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
            if (mCurrentMotion == CrawlingSligMotion::M_TurnAround_11_41B590 && field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mNextMotion = CrawlingSligMotion::M_Crawling_3_41B280;
                return Brain_2_PanicGetALocker::eBrain2_Crawling_8;
            }
            return field_208_brain_sub_state;

        case Brain_2_PanicGetALocker::eBrain2_Crawling_8:
            if (HandleEnemyStopper(field_C4_velx))
            {
                mNextMotion = CrawlingSligMotion::M_TurnAround_11_41B590;
                return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;
            }

            if (static_cast<s32>(sGnFrame) > field_1AC_timer)
            {
                mNextMotion = CrawlingSligMotion::M_TurnAround_11_41B590;
                return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;
            }

            if (Math_NextRandom() >= 10u || static_cast<s32>(sGnFrame) <= field_1C8_say_help_timer)
            {
                return field_208_brain_sub_state;
            }

            mNextMotion = CrawlingSligMotion::M_Speaking_8_41BF70;
            field_1C8_say_help_timer = sGnFrame + 60;
            field_1C0_speak = SligSpeak::eHelp_10;
            return Brain_2_PanicGetALocker::eBrain2_CheckIfWallHit_9;

        case Brain_2_PanicGetALocker::eBrain2_CheckIfWallHit_9:
        {
            if (mCurrentMotion != CrawlingSligMotion::M_Speaking_8_41BF70 || !(field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }

            FP gridScale = {};
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridScale = -ScaleToGridSize(field_CC_sprite_scale);
            }
            else
            {
                gridScale = ScaleToGridSize(field_CC_sprite_scale);
            }

            if (WallHit(FP_FromInteger(35), gridScale))
            {
                mNextMotion = CrawlingSligMotion::M_TurnAround_11_41B590;
                return Brain_2_PanicGetALocker::eBrain2_TurnAroundForLocker_3;
            }
            else
            {
                mNextMotion = CrawlingSligMotion::M_Crawling_3_41B280;
                return Brain_2_PanicGetALocker::eBrain2_Crawling_8;
            }
        }
        break;

        case Brain_2_PanicGetALocker::eBrain2_BeatBySlig_10:
            if (mCurrentMotion == CrawlingSligMotion::M_ShakingToIdle_14_41C040)
            {
                if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    Set_AnimAndMotion_419890(CrawlingSligMotion::M_Idle_0_41B260, TRUE);
                    field_208_brain_sub_state = Brain_2_PanicGetALocker::eBrain2_DetermineCrawlDirection_0;
                    return field_208_brain_sub_state;
                }
            }
            else if (static_cast<s32>(sGnFrame) > field_1AC_timer)
            {
                Set_AnimAndMotion_419890(CrawlingSligMotion::M_ShakingToIdle_14_41C040, TRUE);
            }
            return field_208_brain_sub_state;

        default:
            return field_208_brain_sub_state;
    }
}

s16 CrawlingSlig::Brain_3_Possessed_41A5B0()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
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
            Set_AnimAndMotion_419890(CrawlingSligMotion::M_ShakingToIdle_14_41C040, TRUE);
            return Brain_2_Possessed::eBrain3_Possessed_1;

        case Brain_2_Possessed::eBrain3_Possessed_1:
            if (!Input_IsChanting_45F260() || field_1B8_bChanting)
            {
                return field_208_brain_sub_state;
            }
            field_1AC_timer = sGnFrame + 30;
            SFX_Play_Mono(SoundEffect::PossessEffect_17, 0);
            Set_AnimAndMotion_419890(CrawlingSligMotion::M_Shaking_12_418C30, TRUE);
            return Brain_2_Possessed::eBrain3_Unpossessing_2;

        case Brain_2_Possessed::eBrain3_Unpossessing_2:
            if (Input_IsChanting_45F260())
            {
                if (!(static_cast<s32>(sGnFrame) % 4))
                {
                    New_TintChant_Particle(
                        (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange(-20, 20))) + mBaseAnimatedWithPhysicsGameObject_XPos,
                        mBaseAnimatedWithPhysicsGameObject_YPos - (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange(0, 30))),
                        field_CC_sprite_scale,
                        Layer::eLayer_0);
                }

                if (static_cast<s32>(sGnFrame) <= field_1AC_timer && sActiveHero->mHealth > FP_FromInteger(0))
                {
                    return field_208_brain_sub_state;
                }

                sControlledCharacter_5C1B8C = sActiveHero;
                mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
                gMap.SetActiveCam(field_1BA_prev_level, field_1BC_prev_path, field_1BE_prev_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
                SetBrain(&CrawlingSlig::Brain_4_GetKilled_41A880);
                MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
                return field_208_brain_sub_state;
            }
            else
            {
                Set_AnimAndMotion_419890(CrawlingSligMotion::M_ShakingToIdle_14_41C040, TRUE);
                return Brain_2_Possessed::eBrain3_Possessed_1;
            }
            break;

        case Brain_2_Possessed::eBrain3_BeatBySlig_3:
            if (mCurrentMotion != CrawlingSligMotion::M_ShakingToIdle_14_41C040)
            {
                if (static_cast<s32>(sGnFrame) > field_1AC_timer)
                {
                    Set_AnimAndMotion_419890(CrawlingSligMotion::M_ShakingToIdle_14_41C040, TRUE);
                }
                return field_208_brain_sub_state;
            }

            if (!(field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }

            Set_AnimAndMotion_419890(0, 1);
            field_208_brain_sub_state = Brain_2_Possessed::eBrain3_Possessed_1;
            return field_208_brain_sub_state;

        default:
            break;
    }
    return field_208_brain_sub_state;
}

s16 CrawlingSlig::Brain_4_GetKilled_41A880()
{
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }

    switch (field_208_brain_sub_state)
    {
        case Brain_4_GetKilled::eBrain4_Unknown_0:
            if (mCurrentMotion != CrawlingSligMotion::M_ToShakingToIdle_7_41C010 || !(field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_208_brain_sub_state;
            }
            return Brain_4_GetKilled::eBrain4_Vaporize_1;

        case Brain_4_GetKilled::eBrain4_Vaporize_1:
            if (field_1AC_timer < static_cast<s32>((sGnFrame + 80)))
            {
                field_CC_sprite_scale -= FP_FromDouble(0.008);
                field_D0_r -= 2;
                field_D2_g -= 2;
                field_D4_b -= 2;
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
            ae_new<Gibs>(
                GibType::Slig_1,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                field_C4_velx,
                field_C8_vely,
                field_CC_sprite_scale,
                0);

            ae_new<Blood>(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * field_CC_sprite_scale),
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                20);

            New_Smoke_Particles(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * field_CC_sprite_scale),
                field_CC_sprite_scale,
                3,
                128u,
                128u,
                128u);

            SFX_Play_Mono(SoundEffect::KillEffect_64, 128, field_CC_sprite_scale);
            SFX_Play_Mono(SoundEffect::FallingItemHit_47, 90, field_CC_sprite_scale);

            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit2_Animate);

            Set_AnimAndMotion_419890(12, TRUE);
            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
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
                ae_new<ScreenShake>(0, 0);
                field_1AC_timer = sGnFrame + 30;
                return Brain_4_GetKilled::eBrain4_SetDead_3;
            }
            break;

        default:
            return field_208_brain_sub_state;
    }
}

s16 CrawlingSlig::Brain_5_Transformed_41ADF0()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(field_1D8_obj_id);
    if (gMap.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos)
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

void CrawlingSlig::M_Idle_0_41B260()
{
    HandleCommon();
}

void CrawlingSlig::M_UsingButton_1_41B890()
{
    // Check for using a slig button which is a button than can trigger an id
    auto pSligButton = static_cast<CrawlingSligButton*>(sObjectIds.Find_Impl(field_1D0_slig_button_id));
    if (pSligButton && field_20_animation.field_92_current_frame == 8)
    {
        pSligButton->UseButton();
        field_1D0_slig_button_id = -1;
    }
    // If not using a button check if we are on a locker to get pants or wings
    else if (field_1E4_pPantsOrWingsTlv)
    {
        if (field_20_animation.field_92_current_frame == 7)
        {
            SFX_Play_Mono(SoundEffect::CrawlingSligTransformStart_93, 0);
        }

        if (static_cast<s32>(sGnFrame) == field_1AC_timer - 1)
        {
            New_DestroyOrCreateObject_Particle(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                (field_CC_sprite_scale * FP_FromInteger(45)) + mBaseAnimatedWithPhysicsGameObject_YPos,
                field_CC_sprite_scale);
        }
        else if (static_cast<s32>(sGnFrame) > field_1AC_timer)
        {
            if (field_1E4_pPantsOrWingsTlv->field_4_type == TlvTypes::SligGetPants_104 && ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlgbasicResID, 0, 0))
            {
                // Transform to a walking slig

                SFX_Play_Mono(SoundEffect::SligSpawn_114, 0);

                auto pWalkingSlig = ae_new<Slig>(static_cast<Path_Slig*>(field_1E4_pPantsOrWingsTlv), sPath_dword_BB47C0->TLVInfo_From_TLVPtr(field_1E4_pPantsOrWingsTlv));
                if (pWalkingSlig)
                {
                    field_1D8_obj_id = pWalkingSlig->field_8_object_id;

                    pWalkingSlig->field_CC_sprite_scale = field_CC_sprite_scale;

                    pWalkingSlig->field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX, field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX));

                    if (BrainIs(&CrawlingSlig::Brain_3_Possessed_41A5B0))
                    {
                        pWalkingSlig->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
                        pWalkingSlig->field_146_level = field_1BA_prev_level;
                        pWalkingSlig->field_148_path = field_1BC_prev_path;
                        pWalkingSlig->field_14A_camera = field_1BE_prev_camera;
                        pWalkingSlig->SetBrain(&Slig::Brain_Possessed_2_4BBCF0);
                        pWalkingSlig->field_11C_brain_sub_state = 4;
                        sControlledCharacter_5C1B8C = pWalkingSlig;
                    }
                }

                mHealth = FP_FromInteger(0);
            }
            else if (field_1E4_pPantsOrWingsTlv->field_4_type == TlvTypes::SligGetWings_105 && ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kFlySligResID, 0, 0))
            {
                // Transform to a flying slig

                SFX_Play_Mono(SoundEffect::FlyingSligSpawn_113, 0);

                auto pFlyingSlig = ae_new<FlyingSlig>(static_cast<Path_FlyingSlig*>(field_1E4_pPantsOrWingsTlv), sPath_dword_BB47C0->TLVInfo_From_TLVPtr(field_1E4_pPantsOrWingsTlv));
                if (pFlyingSlig)
                {
                    field_1D8_obj_id = pFlyingSlig->field_8_object_id;
                    pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos;
                    pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_YPos = mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(15);
                    pFlyingSlig->field_294_nextXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
                    pFlyingSlig->field_298_nextYPos = pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_YPos;
                    pFlyingSlig->field_CC_sprite_scale = field_CC_sprite_scale;
                    pFlyingSlig->field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX, field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX));

                    if (BrainIs(&CrawlingSlig::Brain_3_Possessed_41A5B0))
                    {
                        pFlyingSlig->ToPlayerControlled_4360C0();
                        pFlyingSlig->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
                        pFlyingSlig->field_2A0_abe_level = field_1BA_prev_level;
                        pFlyingSlig->field_2A2_abe_path = field_1BC_prev_path;
                        pFlyingSlig->field_2A4_abe_camera = field_1BE_prev_camera;
                        sControlledCharacter_5C1B8C = pFlyingSlig;
                        pFlyingSlig->field_2A8_max_x_speed = (FP_FromDouble(5.5) * field_CC_sprite_scale);
                        pFlyingSlig->field_2AC_up_vel = (-FP_FromDouble(5.5) * field_CC_sprite_scale);
                        pFlyingSlig->field_2B0_down_vel = (FP_FromDouble(5.5) * field_CC_sprite_scale);
                        pFlyingSlig->field_2B4_max_slow_down = (FP_FromDouble(0.3) * field_CC_sprite_scale);
                        pFlyingSlig->field_2B8_max_speed_up = (FP_FromDouble(0.8) * field_CC_sprite_scale);
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
            SetBrain(&CrawlingSlig::Brain_5_Transformed_41ADF0);
            field_C8_vely = FP_FromInteger(0);
            field_C4_velx = FP_FromInteger(0);
            Set_AnimAndMotion_419890(0, TRUE);
            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
            field_20_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            SetType(ReliveTypes::eNone);
        }
    }
    else
    {
        if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            HandleCommon();
        }
    }
}

void CrawlingSlig::M_WakingUp_2_41BF00()
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
            static_cast<SnoozeParticle*>(pObj)->field_1E4_state = SnoozeParticle::SnoozeParticleState::eBlowingUp_2;
        }
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon();
    }
}

void CrawlingSlig::M_Crawling_3_41B280()
{
    if (CanCrawl())
    {
        if (field_20_animation.field_92_current_frame == 3 || field_20_animation.field_92_current_frame == 6)
        {
            Slig_SoundEffect_4BFFE0(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
        }
        else if (field_20_animation.field_92_current_frame == 11)
        {
            if (sControlledCharacter_5C1B8C != this || mHealth <= FP_FromInteger(0))
            {
                if (mNextMotion == CrawlingSligMotion::M_Idle_0_41B260 || mNextMotion == CrawlingSligMotion::M_TurnAround_11_41B590 || mNextMotion == CrawlingSligMotion::M_ToShakingToIdle_7_41C010 || mNextMotion == CrawlingSligMotion::M_Speaking_8_41BF70)
                {
                    Set_AnimAndMotion_419890(15, FALSE);
                }
            }
            else
            {
                if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft)) || (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight)) || !(sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
                {
                    Set_AnimAndMotion_419890(15, TRUE);
                }
            }
            MapFollowMe(TRUE);
        }
    }
}

void CrawlingSlig::M_StartFalling_4_41B620()
{
    M_Falling_5_41B650();
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Set_AnimAndMotion_419890(CrawlingSligMotion::M_Falling_5_41B650, TRUE);
    }
}

void CrawlingSlig::M_Falling_5_41B650()
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx = field_C4_velx - (field_CC_sprite_scale * field_1B0_velx_scale_factor);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx = (field_CC_sprite_scale * field_1B0_velx_scale_factor) + field_C4_velx;
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (BrainIs(&CrawlingSlig::Brain_3_Possessed_41A5B0))
    {
        SetActiveCameraDelayedFromDir();
    }

    if (bCollision)
    {
        switch (pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                BaseAliveGameObjectCollisionLine = pLine;
                PlatformCollide();
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
                MapFollowMe(TRUE);
                if ((hitY - BaseAliveGameObjectLastLineYPos) > (ScaleToGridSize(field_CC_sprite_scale) * FP_FromInteger(5)))
                {
                    SetBrain(&CrawlingSlig::Brain_4_GetKilled_41A880);
                    field_208_brain_sub_state = Brain_4_GetKilled::eBrain4_GibsDeath_2;
                }
                else
                {
                    Set_AnimAndMotion_419890(6, TRUE);
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
                field_C4_velx = -field_C4_velx / FP_FromInteger(2);
                break;

            default:
                return;
        }
    }
}

void CrawlingSlig::M_Landing_6_41B870()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Set_AnimAndMotion_419890(CrawlingSligMotion::M_ShakingToIdle_14_41C040, TRUE);
    }
}

void CrawlingSlig::M_ToShakingToIdle_7_41C010()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mNextMotion == CrawlingSligMotion::M_ShakingToIdle_14_41C040)
        {
            Set_AnimAndMotion_419890(CrawlingSligMotion::M_ShakingToIdle_14_41C040, TRUE);
        }
    }
}

void CrawlingSlig::M_Speaking_8_41BF70()
{
    if (field_20_animation.field_92_current_frame == 2 && field_1C0_speak != SligSpeak::eNone)
    {
        if (gMap.mCurrentPath == field_C0_path_number && gMap.mCurrentLevel == field_C2_lvl_number && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
        {
            Slig_GameSpeak_SFX_4C04F0(field_1C0_speak, 0, 0, this);
        }
        field_1C0_speak = SligSpeak::eNone;
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon();
    }
}

void CrawlingSlig::M_Snoozing_9_41BD80()
{
    if (mNextMotion == CrawlingSligMotion::M_WakingUp_2_41BF00)
    {
        Set_AnimAndMotion_419890(CrawlingSligMotion::M_WakingUp_2_41BF00, TRUE);
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

        if (gMap.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            FP xOff = {};
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = -(field_CC_sprite_scale * FP_FromInteger(-10));
            }
            else
            {
                xOff = (field_CC_sprite_scale * FP_FromInteger(-10));
            }

            const FP yOff = (field_CC_sprite_scale * FP_FromInteger(-10));
            ae_new<SnoozeParticle>(
                mBaseAnimatedWithPhysicsGameObject_XPos + xOff,
                mBaseAnimatedWithPhysicsGameObject_YPos + yOff,
                field_20_animation.mRenderLayer,
                field_20_animation.field_14_scale);
        }
    }
}

void CrawlingSlig::M_PushingWall_10_41B400()
{
    MoveOnLine();

    if (field_20_animation.field_92_current_frame == 4 || field_20_animation.field_92_current_frame == 8)
    {
        MapFollowMe(TRUE);
        field_C4_velx = FP_FromInteger(0);
        Slig_SoundEffect_4BFFE0(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
    }

    if (BrainIs(&CrawlingSlig::Brain_3_Possessed_41A5B0))
    {
        const bool flipX = field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
        if ((!flipX && sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft)) || (flipX && sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight)) || !(sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft | InputCommands::Enum::eRight)))
        {
            Set_AnimAndMotion_419890(CrawlingSligMotion::M_EndPushingWall_17_41B3A0, TRUE);
        }
        else
        {
            FP yPos = {};
            if (flipX)
            {
                yPos = -ScaleToGridSize(field_CC_sprite_scale);
            }
            else
            {
                yPos = ScaleToGridSize(field_CC_sprite_scale);
            }

            if (!WallHit(field_CC_sprite_scale * FP_FromInteger(30), yPos))
            {
                Set_AnimAndMotion_419890(CrawlingSligMotion::M_Crawling_3_41B280, TRUE);
            }
        }
    }
    else
    {
        if (mNextMotion == CrawlingSligMotion::M_Idle_0_41B260 || mNextMotion == CrawlingSligMotion::M_UsingButton_1_41B890 || mNextMotion == CrawlingSligMotion::M_TurnAround_11_41B590 || mNextMotion == CrawlingSligMotion::M_ToShakingToIdle_7_41C010 || mNextMotion == CrawlingSligMotion::M_Speaking_8_41BF70)
        {
            Set_AnimAndMotion_419890(CrawlingSligMotion::M_EndPushingWall_17_41B3A0, FALSE);
        }
    }
}

void CrawlingSlig::M_TurnAround_11_41B590()
{
    if (field_20_animation.field_92_current_frame == 8)
    {
        Slig_SoundEffect_4BFFE0(static_cast<SligSfx>(Math_RandomRange(14, 16)), this);
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        MapFollowMe(TRUE);
        HandleCommon();
    }
}

void CrawlingSlig::M_Shaking_12_418C30()
{
    // Do nothing
}

void CrawlingSlig::M_Empty_13_418C50()
{
    
}

void CrawlingSlig::M_ShakingToIdle_14_41C040()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mHealth > FP_FromInteger(0))
        {
            mHealth = FP_FromInteger(1);
        }
        ToIdle();
    }
}

void CrawlingSlig::M_EndCrawling_15_41B600()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        HandleCommon();
    }
}

void CrawlingSlig::M_IdleToPushingWall_16_41B3C0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Slig_GameSpeak_SFX_4C04F0(static_cast<SligSpeak>(Math_RandomRange(static_cast<s32>(SligSpeak::eOuch1_13), static_cast<s32>(SligSpeak::eOuch2_14))), 0, 0, this);
        Set_AnimAndMotion_419890(CrawlingSligMotion::M_PushingWall_10_41B400, TRUE);
    }
}

void CrawlingSlig::M_EndPushingWall_17_41B3A0()
{
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToIdle();
    }
}

void CrawlingSlig::HandleCommon()
{
    MapFollowMe(TRUE);

    if (BrainIs(&CrawlingSlig::Brain_3_Possessed_41A5B0) && field_208_brain_sub_state == Brain_2_Possessed::eBrain3_Possessed_1)
    {
        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight))
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mNextMotion = CrawlingSligMotion::M_TurnAround_11_41B590;
            }
            else
            {
                mNextMotion = CrawlingSligMotion::M_Crawling_3_41B280;
            }
        }
        else if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft))
        {
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                mNextMotion = CrawlingSligMotion::M_Crawling_3_41B280;
            }
            else
            {
                mNextMotion = CrawlingSligMotion::M_TurnAround_11_41B590;
            }
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eUp))
        {
            field_1E4_pPantsOrWingsTlv = FindPantsOrWings();
            if (field_1E4_pPantsOrWingsTlv)
            {
                if (!field_1E4_pPantsOrWingsTlv->field_1_tlv_state)
                {
                    field_1E4_pPantsOrWingsTlv->field_1_tlv_state &= 0xFF;
                    field_1E4_pPantsOrWingsTlv->field_1_tlv_state |= 1;
                    mNextMotion = 1;
                    field_1AC_timer = sGnFrame + 20;
                }
            }
            else
            {
                auto pSligButton = FindSligButton();
                if (pSligButton)
                {
                    mNextMotion = CrawlingSligMotion::M_UsingButton_1_41B890;
                    field_1D0_slig_button_id = pSligButton->field_8_object_id;
                }
                else
                {
                    mNextMotion = CrawlingSligMotion::M_Speaking_8_41BF70;
                    field_1C0_speak = SligSpeak::eWhat_9;
                }
            }
        }
        if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eGameSpeak1))
        {
            field_1C0_speak = SligSpeak::eHi_0;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eGameSpeak3))
        {
            field_1C0_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? SligSpeak::eGetHim_2 : SligSpeak ::eFreeze_8;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eGameSpeak4))
        {
            field_1C0_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? SligSpeak ::eFreeze_8 : SligSpeak::eGetHim_2;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eGameSpeak2))
        {
            field_1C0_speak = SligSpeak::eHereBoy_1;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eGameSpeak6))
        {
            field_1C0_speak = SligSpeak::eBullshit_5;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eGameSpeak7))
        {
            field_1C0_speak = SligSpeak::eLookOut_6;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eGameSpeak5))
        {
            field_1C0_speak = SligSpeak::eBullshit2_7;
        }
        else if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eGameSpeak8))
        {
            field_1C0_speak = SligSpeak::eLaugh_3;
        }

        if (field_1C0_speak != SligSpeak::eNone)
        {
            mNextMotion = CrawlingSligMotion::M_Speaking_8_41BF70;
        }
    }

    switch (mNextMotion)
    {
        case -1: // TODO: None constant
            if (mCurrentMotion != CrawlingSligMotion::M_Idle_0_41B260)
            {
                mNextMotion = CrawlingSligMotion::M_Idle_0_41B260;
                Set_AnimAndMotion_419890(mNextMotion, TRUE);
                break;
            }
            break;

        case CrawlingSligMotion::M_Idle_0_41B260:
        case CrawlingSligMotion::M_UsingButton_1_41B890:
        case CrawlingSligMotion::M_ToShakingToIdle_7_41C010:
        case CrawlingSligMotion::M_Speaking_8_41BF70:
        case CrawlingSligMotion::M_TurnAround_11_41B590:
            Set_AnimAndMotion_419890(mNextMotion, TRUE);
            break;

        case CrawlingSligMotion::M_Crawling_3_41B280:
        {
            FP gridScale = {};
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                gridScale = -ScaleToGridSize(field_CC_sprite_scale);
            }
            else
            {
                gridScale = ScaleToGridSize(field_CC_sprite_scale);
            }

            if (WallHit(field_CC_sprite_scale * FP_FromInteger(30), gridScale))
            {
                if (sControlledCharacter_5C1B8C == this)
                {
                    mNextMotion = CrawlingSligMotion::M_IdleToPushingWall_16_41B3C0;
                }
            }
            Set_AnimAndMotion_419890(mNextMotion, TRUE);
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
    field_C4_velx = sCrawlingSligXVels_54471C[field_20_animation.field_92_current_frame];

    FP gridScale = ScaleToGridSize(field_CC_sprite_scale);
    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
        gridScale = -gridScale;
    }

    field_C4_velx = (field_C4_velx * field_CC_sprite_scale);

    if (sControlledCharacter_5C1B8C == this && WallHit(field_CC_sprite_scale * FP_FromInteger(30), gridScale))
    {
        field_1B0_velx_scale_factor = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);
        Set_AnimAndMotion_419890(10, TRUE);
        const s32 snappedX = SnapToXGrid(field_CC_sprite_scale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos));
        field_C4_velx = ((FP_FromInteger(snappedX) - mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(4));
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
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObjectId));
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mBaseAnimatedWithPhysicsGameObject_XPos, &mBaseAnimatedWithPhysicsGameObject_YPos, field_C4_velx);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pPlatform)
            {
                if (BaseAliveGameObjectCollisionLine->field_8_type != 32 && BaseAliveGameObjectCollisionLine->field_8_type != 36)
                {
                    pPlatform->VRemove(this);
                    BaseAliveGameObjectId = -1;
                }
            }
            else if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_32 || BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eUnknown_36)
            {
                PlatformCollide();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
            if (mCurrentMotion == CrawlingSligMotion::M_Crawling_3_41B280)
            {
                Set_AnimAndMotion_419890(CrawlingSligMotion::M_StartFalling_4_41B620, TRUE);
            }
            field_1B0_velx_scale_factor = FP_FromInteger(1);
            mBaseAnimatedWithPhysicsGameObject_XPos = mBaseAnimatedWithPhysicsGameObject_XPos + field_C4_velx;
        }
    }
    else
    {
        BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
        Set_AnimAndMotion_419890(CrawlingSligMotion::M_Falling_5_41B650, TRUE);
    }
}

void CrawlingSlig::PlatformCollide()
{
    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    PSX_Point xy = {bRect.x, bRect.y};
    PSX_Point wh = {bRect.w, bRect.h};
    xy.field_2_y += 5;
    wh.field_2_y += 5;

    VOnCollisionWith(
        xy,
        wh,
        ObjList_5C1B78, 1,
        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
}
