#include "stdafx.h"
#include "FlyingSlig.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "MusicController.hpp"
#include "Slig.hpp"
#include "Grenade.hpp"
#include "Dove.hpp"
#include "CrawlingSlig.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "GameSpeak.hpp"
#include "AirExplosion.hpp"
#include "Bullet.hpp"
#include "ParticleBurst.hpp"
#include "Lever.hpp"
#include "Grid.hpp"
#include "../relive_lib/Function.hpp"
#include "AnimationCallBacks.hpp"
#include "../relive_lib/FatalError.hpp"
#include <algorithm>
#include "Input.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Path.hpp"

// Warning, index is saved, order matters here
const static TFlyingSligBrainFn sFlyingSligMotionTable[26] =
{
    &FlyingSlig::Motion_0_Idle,
    &FlyingSlig::Motion_1_HorizontalMovement,
    &FlyingSlig::Motion_2_IdleToTurn,
    &FlyingSlig::Motion_3_DownMovement,
    &FlyingSlig::Motion_4_DownMovementToTurn,
    &FlyingSlig::Motion_5_UpMovement,
    &FlyingSlig::Motion_6_UpMovementToTurn,
    &FlyingSlig::Motion_7_LeverPull,
    &FlyingSlig::Motion_8_GameSpeak,
    &FlyingSlig::Motion_9_Possession,
    &FlyingSlig::Motion_10_EndHorizontalMovement,
    &FlyingSlig::Motion_11_BeginUpMovement,
    &FlyingSlig::Motion_12_HorizontalToDownMovement,
    &FlyingSlig::Motion_13_UpToHorizontalMovement,
    &FlyingSlig::Motion_14_DownToHorizontalMovement,
    &FlyingSlig::Motion_15_QuickTurn,
    &FlyingSlig::Motion_16_IdleToHorizontalMovement,
    &FlyingSlig::Motion_17_BeginDownMovement,
    &FlyingSlig::Motion_18_EndDownMovement,
    &FlyingSlig::Motion_19_DownKnockback,
    &FlyingSlig::Motion_20_UpKnockback,
    &FlyingSlig::Motion_21_EndUpMovement,
    &FlyingSlig::Motion_22_InstantUpXTurn,
    &FlyingSlig::Motion_23_InstantDownXTurn,
    &FlyingSlig::Motion_24_HorizontalToUpMovement,
    &FlyingSlig::Motion_25_TurnToHorizontalMovement,
};

// Warning, index is saved, order matters here
const static TFlyingSligBrainFn sFlyingSligBrainTable[18] = 
{
    &FlyingSlig::Brain_0_Inactive,
    &FlyingSlig::Brain_1_Death,
    &FlyingSlig::Brain_2_Moving,
    &FlyingSlig::Brain_3_GetAlerted,
    &FlyingSlig::Brain_4_ChasingEnemy,
    &FlyingSlig::Brain_5_Idle,
    &FlyingSlig::Brain_6_GameSpeakToMoving,
    &FlyingSlig::Brain_7_PanicMoving,
    &FlyingSlig::Brain_8_PanicIdle,
    &FlyingSlig::Brain_9_SpottedEnemy,
    &FlyingSlig::Brain_10_LaunchingGrenade,
    &FlyingSlig::Brain_11_AbeDead,
    &FlyingSlig::Brain_12_Possessed,
    &FlyingSlig::Brain_13_Possession,
    &FlyingSlig::Brain_14_DePossession,
    &FlyingSlig::Brain_15_FlyingSligSpawn,
    &FlyingSlig::Brain_1_Death,
    &FlyingSlig::Brain_17_FromCrawlingSlig,
};

void FlyingSlig::SetBrain(TFlyingSligBrainFn fn)
{
    mBrainState = fn;
}

bool FlyingSlig::BrainIs(TFlyingSligBrainFn fn)
{
    return mBrainState == fn;
}

const static AnimId sFlyingSligAnimIdTable[26] = {
    AnimId::FlyingSlig_Idle,
    AnimId::FlyingSlig_MoveHorizontal,
    AnimId::FlyingSlig_IdleTurnAround,
    AnimId::FlyingSlig_MoveDown,
    AnimId::FlyingSlig_MoveDownTurnAround,
    AnimId::FlyingSlig_MoveUp,
    AnimId::FlyingSlig_MoveUpTurnAround,
    AnimId::FlyingSlig_PullLever,
    AnimId::FlyingSlig_Speak,
    AnimId::FlyingSlig_Possession,
    AnimId::FlyingSlig_MoveHorizontalEnd,
    AnimId::FlyingSlig_MoveUpStart,
    AnimId::FlyingSlig_MoveHorizontalToDown,
    AnimId::FlyingSlig_MoveUpToHorizontal,
    AnimId::FlyingSlig_MoveDownToHorizontal,
    AnimId::FlyingSlig_TurnQuick,
    AnimId::FlyingSlig_IdleToHorizontal,
    AnimId::FlyingSlig_BeginDownMovement,
    AnimId::FlyingSlig_EndDownMovement,
    AnimId::FlyingSlig_DownKnockback,
    AnimId::FlyingSlig_UpKnockback,
    AnimId::FlyingSlig_EndUpMovement,
    AnimId::FlyingSlig_InstantUpXTurn,
    AnimId::FlyingSlig_InstantDownXTurn,
    AnimId::FlyingSlig_HorizontalToUpMovement,
    AnimId::FlyingSlig_TurnToHorizontalMovement,
};

void FlyingSlig::LoadAnimations()
{
    for (auto& animId : sFlyingSligAnimIdTable)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }

    // used in Animation_OnFrame_FlyingSlig
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Vaporize_Particle));
}

FlyingSlig::FlyingSlig(relive::Path_FlyingSlig* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(9)
{
    LoadAnimations();

    if (tlvId != Guid{})
    {
        mBaseGameObjectTlvInfo = tlvId;
    }

    SetType(ReliveTypes::eFlyingSlig);

    Animation_Init(GetAnimRes(AnimId::FlyingSlig_Idle));

    field_1F4_mPal = std::make_shared<AnimationPal>();

    GetAnimation().SetFnPtrArray(gFlyingSlig_Anim_Frames_Fns);

    SetCanBePossessed(true);
    SetCanSetOffExplosives(true);

    SetCanExplode(true);

    field_14C_timer = 0;
    SetDoPurpleLightEffect(true);

    field_15C_voice_pitch_min = 45 * ((Math_NextRandom() % 5) - 2);

    field_150_grenade_delay = 0;
    field_154_collision_reaction_timer = 0;

    mSpeaking1 = false;
    mThrowGrenade = false;
    mAlertedAndNotFacingAbe = false;
    mDoAction = false;
    mChanting = false;
    mSpeaking2 = false;

    field_158_obj_id = Guid{};

    field_290_bobbing_values_index = 0;
    field_284_bobbing_value = FP_FromInteger(0);
    field_28C_bobbing_values_table_index = 0;
    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    field_118_data = *pTlv;

    field_148_tlvInfo = tlvId;

    SetBrain(&FlyingSlig::Brain_0_Inactive);

    SetCurrentMotion(eFlyingSligMotions::Motion_0_Idle);

    if (field_118_data.mSpawnDelayState == relive::Path_FlyingSlig::SpawnDelayStates::eUseCustomSpawnMoveDelay)
    {
        field_14C_timer = sGnFrame + field_118_data.mSpawnMoveDelay;
    }
    else if (field_118_data.mSpawnDelayState == relive::Path_FlyingSlig::SpawnDelayStates::eMoveImmediately)
    {
        field_14C_timer = sGnFrame + 1;
    }

    field_2A8_max_x_speed = FP_FromInteger(field_118_data.mMaxVelocity) * GetSpriteScale();
    field_2AC_up_vel = FP_FromInteger(-field_118_data.mMaxVelocity) * GetSpriteScale();
    field_2B0_down_vel = FP_FromInteger(field_118_data.mMaxVelocity) * GetSpriteScale();
    field_2B4_max_slow_down = FP_FromDouble(0.4) * GetSpriteScale();
    field_2B8_max_speed_up = FP_FromDouble(0.4) * GetSpriteScale();

    GetAnimation().SetFlipX(field_118_data.mFacing == relive::reliveXDirection::eLeft);

    if (field_118_data.mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBat_Half_14);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBats_33);
        SetScale(Scale::Fg);
    }

    mPersistant = field_118_data.mPersistant == relive::reliveChoice::eYes;

    field_17C_launch_switch_id |= field_118_data.mLaunchGrenadeSwitchId;

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    FP hitX = {};
    FP hitY = {};
    gCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeftX),
        FP_FromInteger(pTlv->mTopLeftY),
        FP_FromInteger(pTlv->mBottomRightX),
        FP_FromInteger(pTlv->mBottomRightY),
        &BaseAliveGameObjectCollisionLine,
        &hitX,
        &hitY,
        CollisionMask(eTrackLine_8));

    if (BaseAliveGameObjectCollisionLine)
    {
        mXPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.x);
        mYPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
        sub_4348A0();
        field_194 = FP_FromInteger(0);
    }

    CreateShadow();
}

s32 FlyingSlig::CreateFromSaveState(const u8* pBuffer)
{
    auto pSaveState = reinterpret_cast<const FlyingSligSaveState*>(pBuffer);

    auto pTlv = static_cast<relive::Path_FlyingSlig*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->field_3C_tlvInfo));

    auto pFlyingSlig = relive_new FlyingSlig(pTlv, pSaveState->field_3C_tlvInfo);
    if (pFlyingSlig)
    {
        pFlyingSlig->BaseAliveGameObjectPathTLV = nullptr;
        pFlyingSlig->BaseAliveGameObjectCollisionLine = nullptr;

        pFlyingSlig->mXPos = pSaveState->field_4_xpos;
        pFlyingSlig->mYPos = pSaveState->field_8_ypos;

        pFlyingSlig->mVelX = pSaveState->field_C_velx;
        pFlyingSlig->mVelY = pSaveState->field_10_vely;

        pFlyingSlig->mCurrentPath = pSaveState->field_14_path_number;
        pFlyingSlig->mCurrentLevel = pSaveState->field_16_lvl_number;
        pFlyingSlig->SetSpriteScale(pSaveState->field_18_sprite_scale);

        pFlyingSlig->field_27C_r = pSaveState->field_1C_oldr;
        pFlyingSlig->field_27E_g = pSaveState->field_1E_oldg;
        pFlyingSlig->field_280_b = pSaveState->field_20_oldb;

        pFlyingSlig->mRGB.SetRGB(pSaveState->field_1C_oldr, pSaveState->field_1E_oldg, pSaveState->field_20_oldb);

        pFlyingSlig->SetCurrentMotion(pSaveState->field_24_current_state);

        pFlyingSlig->GetAnimation().Set_Animation_Data(pFlyingSlig->GetAnimRes(sFlyingSligAnimIdTable[pFlyingSlig->mCurrentMotion]));

        pFlyingSlig->GetAnimation().SetCurrentFrame(pSaveState->field_26_current_frame);

        pFlyingSlig->GetAnimation().SetFrameChangeCounter(pSaveState->field_28_frame_change_counter);

        pFlyingSlig->GetAnimation().SetRender(pSaveState->field_2A_bAnimRender & 1);
        pFlyingSlig->GetAnimation().SetFlipX(pSaveState->field_22_bAnimFlipX & 1);
        pFlyingSlig->SetDrawable(pSaveState->field_2B_bDrawable & 1);

        if (IsLastFrame(&pFlyingSlig->GetAnimation()))
        {
            pFlyingSlig->GetAnimation().SetIsLastFrame(true);
        }

        pFlyingSlig->mHealth = pSaveState->field_2C_current_health;
        pFlyingSlig->SetCurrentMotion(pSaveState->field_30_current_state);
        pFlyingSlig->mNextMotion = pSaveState->field_32_delayed_state;
        pFlyingSlig->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pSaveState->field_34_lastLineYPos);
        pFlyingSlig->SetRestoredFromQuickSave(true);
        pFlyingSlig->BaseAliveGameObjectCollisionLineType = -1;

        if (pSaveState->field_36_line_idx != -1)
        {
            pFlyingSlig->BaseAliveGameObjectCollisionLineType = pSaveState->field_36_line_idx;
        }

        if (pSaveState->mPossessed)
        {
            sControlledCharacter = pFlyingSlig;
            pFlyingSlig->field_2A8_max_x_speed = (FP_FromDouble(5.5) * pFlyingSlig->GetSpriteScale());
            pFlyingSlig->field_2AC_up_vel = (FP_FromDouble(-5.5) * pFlyingSlig->GetSpriteScale());
            pFlyingSlig->field_2B0_down_vel = (FP_FromDouble(5.5) * pFlyingSlig->GetSpriteScale());
            pFlyingSlig->field_2B4_max_slow_down = (FP_FromDouble(0.25) * pFlyingSlig->GetSpriteScale());
            pFlyingSlig->field_2B8_max_speed_up = (FP_FromDouble(0.7) * pFlyingSlig->GetSpriteScale());
        }

        pFlyingSlig->field_17C_launch_switch_id = pSaveState->field_38_launch_switch_id;


        pFlyingSlig->mThrowGrenade = pSaveState->mThrowGrenade;
        pFlyingSlig->mAlertedAndNotFacingAbe = pSaveState->mAlertedAndNotFacingAbe;
        pFlyingSlig->mDoAction = pSaveState->mDoAction;
        pFlyingSlig->mChanting = pSaveState->mChanting;
        pFlyingSlig->mSpeaking2 = pSaveState->mSpeaking2;
        pFlyingSlig->mSpeaking1 = pSaveState->mSpeaking1;
        pFlyingSlig->mLastLine = pSaveState->mLastLine;
        pFlyingSlig->mUnknown1 = pSaveState->mUnknown1;
        pFlyingSlig->mUnknown2 = pSaveState->mUnknown2;


        pFlyingSlig->field_14C_timer = pSaveState->field_40_timer;
        pFlyingSlig->field_150_grenade_delay = pSaveState->field_44_grenade_delay;
        pFlyingSlig->field_154_collision_reaction_timer = pSaveState->field_48_collision_reaction_timer;
        pFlyingSlig->field_184_xSpeed = pSaveState->field_4C_xSpeed;
        pFlyingSlig->field_188_ySpeed = pSaveState->field_50_ySpeed;
        pFlyingSlig->field_17D_next_speak = pSaveState->field_54_next_speak;
        pFlyingSlig->field_160_voice_pitch_min = pSaveState->field_56_voice_pitch_min;
        pFlyingSlig->field_158_obj_id = pSaveState->field_58_obj_id;
        pFlyingSlig->field_18C = pSaveState->field_5C;
        pFlyingSlig->field_190 = pSaveState->field_60;
        pFlyingSlig->field_194 = pSaveState->field_64;
        pFlyingSlig->field_198_line_length = pSaveState->field_68_line_length;
        pFlyingSlig->field_1C4 = pSaveState->field_6C;
        pFlyingSlig->field_1C8_lever_pull_range_xpos = pSaveState->field_70_lever_pull_range_xpos;
        pFlyingSlig->field_1CC_lever_pull_range_ypos = pSaveState->field_74_lever_pull_range_ypos;
        pFlyingSlig->field_294_nextXPos = pSaveState->field_88_nextXPos;
        pFlyingSlig->field_298_nextYPos = pSaveState->field_8C_nextYPos;
        pFlyingSlig->SetBrain(sFlyingSligBrainTable[pSaveState->field_90_fns1_idx]);

        pFlyingSlig->mAbeLevel = pSaveState->field_9A_abe_level;
        pFlyingSlig->mAbePath = pSaveState->field_9C_abe_path;
        pFlyingSlig->mAbeCamera = pSaveState->field_9E_abe_camera;

        pFlyingSlig->field_290_bobbing_values_index = pSaveState->field_A4_bobbing_values_index;
        pFlyingSlig->field_284_bobbing_value = pSaveState->field_A8_bobbing_value;
        pFlyingSlig->field_28C_bobbing_values_table_index = pSaveState->field_A0_bobbing_values_table_index;
    }

    return sizeof(FlyingSligSaveState);
}

s32 FlyingSlig::VGetSaveState(u8* pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return 0;
    }

    auto pState = reinterpret_cast<FlyingSligSaveState*>(pSaveBuffer);

    pState->field_0_type = ReliveTypes::eFlyingSlig;

    pState->field_4_xpos = mXPos;
    pState->field_8_ypos = mYPos;
    pState->field_C_velx = mVelX;
    pState->field_10_vely = mVelY;

    pState->field_14_path_number = mCurrentPath;
    pState->field_16_lvl_number = mCurrentLevel;
    pState->field_18_sprite_scale = GetSpriteScale();

    pState->field_1C_oldr = mRGB.r;
    pState->field_1E_oldg = mRGB.g;
    pState->field_20_oldb = mRGB.b;

    pState->field_22_bAnimFlipX = GetAnimation().GetFlipX();
    pState->field_24_current_state = mCurrentMotion;
    pState->field_26_current_frame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    pState->field_28_frame_change_counter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());

    pState->field_2B_bDrawable = GetDrawable();
    pState->field_2A_bAnimRender = GetAnimation().GetRender();
    pState->field_2C_current_health = mHealth;
    pState->field_30_current_state = mCurrentMotion;
    pState->field_32_delayed_state = mNextMotion;

    pState->field_34_lastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    pState->field_36_line_idx = -1; // OG bug - actually becomes 0 due to impossible case removed below ?

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_36_line_idx = static_cast<s16>(BaseAliveGameObjectCollisionLine - gCollisions->mCollisionArray);
    }

    pState->field_38_launch_switch_id = field_17C_launch_switch_id;

    pState->mPossessed = this == sControlledCharacter;
    pState->mThrowGrenade = mThrowGrenade;
    pState->mAlertedAndNotFacingAbe = mAlertedAndNotFacingAbe;
    pState->mDoAction = mDoAction;
    pState->mChanting = mChanting;
    pState->mSpeaking2 = mSpeaking2;
    pState->mSpeaking1 = mSpeaking1;
    pState->mLastLine = mLastLine;
    pState->mUnknown1 = mUnknown1;
    pState->mUnknown2 = mUnknown2;

    pState->field_3C_tlvInfo = field_148_tlvInfo;
    pState->field_40_timer = field_14C_timer;
    pState->field_44_grenade_delay = field_150_grenade_delay;
    pState->field_48_collision_reaction_timer = field_154_collision_reaction_timer;

    pState->field_4C_xSpeed = field_184_xSpeed;
    pState->field_50_ySpeed = field_188_ySpeed;

    pState->field_54_next_speak = field_17D_next_speak;
    pState->field_56_voice_pitch_min = field_160_voice_pitch_min;

    pState->field_58_obj_id = Guid{};
    if (field_158_obj_id != Guid{})
    {
        auto pObj = sObjectIds.Find_Impl(field_158_obj_id);
        if (pObj)
        {
            pState->field_58_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    pState->field_5C = field_18C;
    pState->field_60 = field_190;
    pState->field_64 = field_194;
    pState->field_68_line_length = field_198_line_length;
    pState->field_6C = field_1C4;
    pState->field_70_lever_pull_range_xpos = field_1C8_lever_pull_range_xpos;
    pState->field_74_lever_pull_range_ypos = field_1CC_lever_pull_range_ypos;
    pState->field_88_nextXPos = field_294_nextXPos;
    pState->field_8C_nextYPos = field_298_nextYPos;
    pState->field_90_fns1_idx = 0;

    s32 idx = 0;
    for (const auto& fn : sFlyingSligBrainTable)
    {
        if (BrainIs(fn))
        {
            pState->field_90_fns1_idx = idx;
            break;
        }
        idx++;
    }

    pState->field_9A_abe_level = mAbeLevel;
    pState->field_9C_abe_path = mAbePath;
    pState->field_9E_abe_camera = mAbeCamera;

    pState->field_A4_bobbing_values_index = field_290_bobbing_values_index;
    pState->field_A8_bobbing_value = field_284_bobbing_value;
    pState->field_A0_bobbing_values_table_index = field_28C_bobbing_values_table_index;

    return sizeof(FlyingSligSaveState);
}

FlyingSlig::~FlyingSlig()
{
    if (sControlledCharacter == this)
    {
        sControlledCharacter = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
        if (gMap.mNextLevel != EReliveLevelIds::eMenu)
        {
            gMap.SetActiveCam(
                mAbeLevel,
                mAbePath,
                mAbeCamera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    relive::Path_TLV* pTlv = gPathInfo->TLV_From_Offset_Lvl_Cam(field_148_tlvInfo);
    if (pTlv)
    {
        if (pTlv->mTlvType != ReliveTypes::eSligGetWings && pTlv->mTlvType != ReliveTypes::eFlyingSligSpawner)
        {
            if (mHealth <= FP_FromInteger(0))
            {
                Path::TLV_Reset(field_148_tlvInfo, -1, 0, 1);
            }
            else
            {
                Path::TLV_Reset(field_148_tlvInfo, -1, 0, 0);
            }
        }
    }
}

void FlyingSlig::VScreenChanged()
{
    if (gMap.LevelChanged() || (gMap.PathChanged() && (this != sControlledCharacter || mPersistant)))
    {
        SetDead(true);
    }
}

void FlyingSlig::VUpdate()
{
    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);

        if (!IsPossessed())
        {
            if (BaseAliveGameObjectCollisionLineType != -1)
            {
                const FP savedNextXPos = field_294_nextXPos;
                const FP savedNextYPos = field_298_nextYPos;
                // Everything else uses this a a line type, but of course MrSlig uses it as an index
                BaseAliveGameObjectCollisionLine = gCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLineType);
                sub_4348A0();
                field_294_nextXPos = savedNextXPos;
                field_298_nextYPos = savedNextYPos;
            }
        }

        if (field_158_obj_id != Guid{})
        {
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->mBaseGameObjectTlvInfo == field_158_obj_id)
                {
                    field_158_obj_id = pObj->mBaseGameObjectId;
                    break;
                }
            }
        }
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
    else
    {
        mXPos = field_294_nextXPos;
        mYPos = field_298_nextYPos;

        (this->*(mBrainState))();

        InvokeMemberFunction(this, sFlyingSligMotionTable, mCurrentMotion);

        Movement();
    }
}

void FlyingSlig::VRender(PrimHeader** ot)
{
    const s16 eyeIndices[] = {60, 62};
    renderWithGlowingEyes(ot, this, field_1F4_mPal, 64, field_27C_r, field_27E_g, field_280_b, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

void FlyingSlig::sub_4348A0()
{
    mUnknown1 = false;
    mLastLine = false;
    field_298_nextYPos = mYPos;
    field_294_nextXPos = mXPos;
    field_18C = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);
    sub_437C70(BaseAliveGameObjectCollisionLine);
    const s16 v5 = FP_GetExponent(mYPos - field_1A4_rect.y);
    const s16 v6 = FP_GetExponent(mXPos - field_1A4_rect.x);
    field_194 = FP_FromInteger(Math_SquareRoot_Int(v5 * v5 + v6 * v6));
    mUnknown2 = field_118_data.mFacing == relive::reliveXDirection::eLeft;
}

const s32 sBobbingValuesHorizontalMovement_552500[9] = {
    -163840,
    -245760,
    -286720,
    -327680,
    -286720,
    -245760,
    -163840,
    -65536,
    65535999};

const s32 sBobbingValuesIdle_552524[11] = {
    74785,
    85101,
    79944,
    59312,
    23209,
    -34563,
    -82422,
    -98373,
    -82422,
    -34563,
    65535999};

const s32 sBobbingValuesTurning_552550[11] = {
    38603,
    84930,
    84928,
    38605,
    -187040,
    -344791,
    -178048,
    70049,
    100043,
    38273,
    65535999};



const s32* sBobbingValuesTables_55257C[4] = {nullptr, sBobbingValuesHorizontalMovement_552500, sBobbingValuesIdle_552524, sBobbingValuesTurning_552550};

void FlyingSlig::Movement()
{
    if (!IsPossessed())
    {
        if (field_190 > FP_FromInteger(0))
        {
            if (field_18C + field_190 > field_2A8_max_x_speed)
            {
                field_18C = field_2A8_max_x_speed;
            }
            else
            {
                field_18C += field_190;
            }
        }
        else if (field_190 < FP_FromInteger(0))
        {
            if (field_18C + field_190 < -field_2A8_max_x_speed)
            {
                field_18C = -field_2A8_max_x_speed;
            }
            else
            {
                field_18C += field_190;
            }
        }
        else if (field_190 == FP_FromInteger(0))
        {
            if (field_18C >= -field_2B4_max_slow_down)
            {
                if (field_18C <= field_2B4_max_slow_down)
                {
                    field_18C = FP_FromInteger(0);
                }
                else
                {
                    field_18C -= field_2B4_max_slow_down;
                }
            }
            else
            {
                field_18C += field_2B4_max_slow_down;
            }
        }

        FP_Point posXY = {};
        sub_437AC0(field_18C, &posXY);
        mVelX = posXY.x - mXPos;
        mVelY = posXY.y - mYPos;
    }
    else
    {
        const s32 newVel = Math_SquareRoot_Int(FP_GetExponent((mVelY * mVelY) + (mVelX * mVelX)));
        if (FP_Abs(FP_FromInteger(newVel)) > FP_FromDouble(0.05))
        {
            mVelX = mVelX - ((mVelX / FP_FromInteger(newVel)) * field_2B4_max_slow_down) + field_184_xSpeed;
            mVelY = mVelY - ((mVelY / FP_FromInteger(newVel)) * field_2B4_max_slow_down) + field_188_ySpeed;
        }
        else
        {
            if (field_184_xSpeed != FP_FromInteger(0) || field_188_ySpeed != FP_FromInteger(0))
            {
                mVelX += field_184_xSpeed;
                mVelY = mVelY + field_188_ySpeed;
            }
            else
            {
                mVelX = FP_FromInteger(0);
                mVelY = FP_FromInteger(0);
            }
        }

        const FP v15 = FP_FromInteger(Math_SquareRoot_Int(FP_GetExponent((mVelY * mVelY) + (mVelX * mVelX))));
        if (v15 > field_2A8_max_x_speed)
        {
            mVelX = ((mVelX / v15) * field_2A8_max_x_speed);
            mVelY = ((mVelY / v15) * field_2A8_max_x_speed);
        }

        if (mVelY < FP_FromInteger(0))
        {
            if (CollisionUp(mVelY))
            {
                mVelY = -mVelY;
            }
        }

        if (mVelY > FP_FromInteger(0))
        {
            if (CollisionDown(mVelY))
            {
                mVelY = -mVelY;
            }
        }

        if (mVelX != FP_FromInteger(0))
        {
            if (CollisionLeftRight(mVelX))
            {
                mVelX = -mVelX;
            }
        }
    }

    mXPos += mVelX;
    mYPos += mVelY;

    SetActiveCameraDelayedFromDir();

    if (mThrowGrenade)
    {
        if (static_cast<s32>(sGnFrame) > field_150_grenade_delay && 
            (GetPossessed() || SwitchStates_Get(field_17C_launch_switch_id) ||
             GetPossessed()) && CanThrowGrenade())
        {
            ThrowGrenade();
        }
    }

    if (mSpeaking2)
    {
        SetMotionHelper(eFlyingSligMotions::Motion_8_GameSpeak);
    }

    if (mAlertedAndNotFacingAbe)
    {
        SetMotionHelper(eFlyingSligMotions::Motion_2_IdleToTurn);
    }

    if (mDoAction)
    {
        if (GetCurrentMotion() == eFlyingSligMotions::Motion_0_Idle)
        {
            TryPullLever();
        }
    }

    field_294_nextXPos = mXPos;
    field_298_nextYPos = mYPos;

    mYPos += GetSpriteScale() * FP_FromInteger(20);

    if (field_28C_bobbing_values_table_index)
    {
        if (field_28C_bobbing_values_table_index > 3)
        {
            ALIVE_FATAL("FlyingSlig array out of bounds");
        }
        const FP* pTable = reinterpret_cast<const FP*>(sBobbingValuesTables_55257C[field_28C_bobbing_values_table_index]); // TODO: Type conversion !!
        field_284_bobbing_value = pTable[field_290_bobbing_values_index];
        if (field_284_bobbing_value <= FP_FromInteger(990))
        {
            field_290_bobbing_values_index++;
        }
        else
        {
            field_284_bobbing_value = FP_FromInteger(0);
            field_28C_bobbing_values_table_index = 0;
            field_290_bobbing_values_index = 0;
        }
        mYPos = mYPos + field_284_bobbing_value;
    }
    else
    {
        if (field_284_bobbing_value != FP_FromInteger(0))
        {
            if (field_284_bobbing_value <= FP_FromInteger(0))
            {
                if (field_284_bobbing_value <= FP_FromInteger(-1))
                {
                    field_284_bobbing_value = field_284_bobbing_value + FP_FromInteger(1);
                }
                else
                {
                    field_284_bobbing_value = FP_FromInteger(0);
                }
            }
            else if (field_284_bobbing_value < FP_FromInteger(1))
            {
                field_284_bobbing_value = FP_FromInteger(0);
            }
            else
            {
                field_284_bobbing_value = field_284_bobbing_value - FP_FromInteger(1);
            }
            mYPos = mYPos + field_284_bobbing_value;
        }
    }

    s32 v37 = {};
    if (Math_Distance(0, 0, FP_GetExponent(mVelX), FP_GetExponent(mVelY)) >= 0)
    {
        v37 = Math_Distance(0, 0, FP_GetExponent(mVelX), FP_GetExponent(mVelY));
    }
    else
    {
        v37 = -Math_Distance(0, 0, FP_GetExponent(mVelX), FP_GetExponent(mVelY));
    }

    const s32 v38 = static_cast<s32>(sGnFrame) % ((FP_FromInteger(v37) < field_2A8_max_x_speed) + 2);
    if (!v38 && mHealth > FP_FromInteger(0))
    {
        Slig_SoundEffect((Math_NextRandom() % 3) ? SligSfx::ePropeller2_10 : SligSfx::ePropeller1_9, this);
    }

    mThrowGrenade = false;
    mAlertedAndNotFacingAbe = false;
    mDoAction = false;
    mSpeaking2 = false;

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    field_190 = FP_FromInteger(0);
}

bool FlyingSlig::VTakeDamage(BaseGameObject* pFrom)
{
    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        {
            if (static_cast<Bullet*>(pFrom)->mBulletType == BulletType::eZBullet_3)
            {
                const PSX_RECT bRect = VGetBoundingRect();
                relive::Path_TLV* pTlv = nullptr;
                do
                {
                    pTlv = gPathInfo->TlvGetAt(pTlv,
                                                                 mXPos,
                                                                 FP_FromInteger(bRect.y),
                                                                 mXPos,
                                                                 FP_FromInteger(bRect.y));
                    if (!pTlv)
                    {
                        break;
                    }
                    if (pTlv->mTlvType == ReliveTypes::eZSligCover)
                    {
                        // Left/right in cover
                        if (bRect.x >= pTlv->mTopLeftX && bRect.x <= pTlv->mBottomRightX && bRect.y >= pTlv->mTopLeftY && bRect.y <= pTlv->mBottomRightY)
                        {
                            // Top/bottom in cover
                            if (bRect.w >= pTlv->mTopLeftX && bRect.w <= pTlv->mBottomRightX && bRect.h >= pTlv->mTopLeftY && bRect.h <= pTlv->mBottomRightY)
                            {
                                return false;
                            }
                        }
                    }
                }
                while (pTlv);
            }
            // Not in Z-Cover, fall through and be shot
            [[fallthrough]];
        }
        case ReliveTypes::eDrill:
        case ReliveTypes::eRockSpawner:
        case ReliveTypes::eAbe:
        case ReliveTypes::eMeatSaw:
        case ReliveTypes::eMineCar:
        case ReliveTypes::eSlog:
        {
            if (BrainIs(&FlyingSlig::Brain_1_Death))
            {
                return true;
            }
            BlowUp();
            auto pExplosion = relive_new AirExplosion(mXPos, mYPos - (GetSpriteScale() * FP_FromInteger(5)), GetSpriteScale(), 1);
            if (!pExplosion)
            {
                return true;
            }
            return true;
        }

        case ReliveTypes::eElectricWall:
            Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, field_15C_voice_pitch_min, this);
            break;

        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eAirExplosion:
            if (!BrainIs(&FlyingSlig::Brain_1_Death))
            {
                BlowUp();
            }
            break;

        case ReliveTypes::eElectrocute:
            if (!BrainIs(&FlyingSlig::Brain_1_Death))
            {
                GetAnimation().SetRender(false);
                mHealth = FP_FromInteger(0);
                SetBrain(&FlyingSlig::Brain_1_Death);
                field_14C_timer = sGnFrame;
            }
            break;

        default:
            return true;
    }
    return true;
}

void FlyingSlig::Brain_0_Inactive()
{
    if (static_cast<s32>(sGnFrame) >= field_14C_timer)
    {
        ToMoving();
    }
}

void FlyingSlig::Brain_1_Death()
{
    if (static_cast<s32>(sGnFrame) >= field_14C_timer)
    {
        SetDead(true);
        EventBroadcast(kEventMudokonComfort, this);
    }
}

void FlyingSlig::Brain_2_Moving()
{
    if (!sub_436730() && sub_4374A0(1) == 1)
    {
        mUnknown2 = !mUnknown2;
        PatrolDelay();
    }
}

void FlyingSlig::Brain_3_GetAlerted()
{
    if (GetCurrentMotion() != eFlyingSligMotions::Motion_0_Idle)
    {
        field_14C_timer++;
    }
    else if (VIsFacingMe(sControlledCharacter))
    {
        if (!sub_436730() && static_cast<s32>(sGnFrame) >= field_14C_timer)
        {
            ToMoving();
        }
    }
    else
    {
        mAlertedAndNotFacingAbe = true;
        field_14C_timer++;
    }
}

void FlyingSlig::Brain_4_ChasingEnemy()
{
    mUnknown1 = false;

    if (EventGet(kEventHeroDying) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        ToAbeDead();
        return;
    }

    if (EventGet(kEventResetting) || sControlledCharacter->GetSpriteScale() != GetSpriteScale() || IsInInvisibleZone(sControlledCharacter) || sControlledCharacter->GetInvisible() || (!IsWallBetween(this, sControlledCharacter) && (!IsActiveHero(sControlledCharacter) || sActiveHero->mCurrentMotion != eAbeMotions::Motion_65_LedgeAscend_4548E0) && sControlledCharacter->Type() != ReliveTypes::eMineCar))
    {
        PatrolDelay();
        return;
    }

    if (sub_436C60(&BaseAliveGameObjectCollisionLine->mRect, 1))
    {
        if (FP_Abs(field_194 - field_1C4) < (FP_FromInteger(15) * GetSpriteScale()))
        {
            ToLaunchingGrenade();
            return;
        }

        mUnknown2 = field_1C4 > field_194;
        mUnknown1 = true;

        if (static_cast<s32>(sGnFrame) > field_150_grenade_delay && CanThrowGrenade())
        {
            if (VIsFacingMe(sControlledCharacter))
            {
                if (!(Math_NextRandom() & 15))
                {
                    ToLaunchingGrenade();
                    return;
                }
            }
        }
    }
    else if (!sub_436B20())
    {
        sub_4373B0();
    }

    sub_4374A0(0);
}

void FlyingSlig::Brain_5_Idle()
{
    if (!sub_436730() && static_cast<s32>(sGnFrame) >= field_14C_timer)
    {
        ToMoving();
    }
}

void FlyingSlig::Brain_6_GameSpeakToMoving()
{
    if (GetCurrentMotion() != eFlyingSligMotions::Motion_8_GameSpeak)
    {
        ToMoving();
    }
}

void FlyingSlig::Brain_7_PanicMoving()
{
    if (CanChase(sControlledCharacter))
    {
        ToChase();
        return;
    }

    if (!IsEventInRange(kEventAbeOhm, mXPos, mYPos, AsEventScale(GetScale())))
    {
        ToMoving();
        return;
    }

    if (static_cast<s32>(sGnFrame) < field_14C_timer)
    {
        if (sub_4374A0(0) != 1)
        {
            return;
        }
        mUnknown2 = !mUnknown2;
    }

    ToPanicIdle();
}

void FlyingSlig::Brain_8_PanicIdle()
{
    if (CanChase(sControlledCharacter))
    {
        ToChase();
    }
    else if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, AsEventScale(GetScale())))
    {
        if (static_cast<s32>(sGnFrame) >= field_14C_timer && GetCurrentMotion() != eFlyingSligMotions::Motion_8_GameSpeak)
        {
            ToPanicMoving();
        }
    }
    else
    {
        ToMoving();
    }
}

void FlyingSlig::Brain_9_SpottedEnemy()
{
    if (static_cast<s32>(sGnFrame) >= field_14C_timer)
    {
        ToChase();
    }
}

void FlyingSlig::Brain_10_LaunchingGrenade()
{
    if (VIsFacingMe(sControlledCharacter))
    {
        mThrowGrenade = true;
    }
    ToChase();
}

void FlyingSlig::Brain_11_AbeDead()
{
    if (static_cast<s32>(sGnFrame) == field_14C_timer - 45)
    {
        Say(SligSpeak ::eGotYa_12, 0);
    }
    else if (static_cast<s32>(sGnFrame) == field_14C_timer)
    {
        Say(SligSpeak::eLaugh_3, 0);
    }
}

void FlyingSlig::Brain_12_Possessed()
{
    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    if (!mChanting)
    {
        if (Input_IsChanting())
        {
            ToChantShake();
        }
    }

    if (!Input_IsChanting())
    {
        mChanting = false;
    }

    HandlePlayerControls();
}

void FlyingSlig::Brain_13_Possession()
{
    if (static_cast<s32>(sGnFrame) > field_14C_timer)
    {
        Say(SligSpeak ::eHi_0, 0x1FF);
        ToPlayerControlled();
    }
}

void FlyingSlig::Brain_14_DePossession()
{
    if (!Input_IsChanting())
    {
        ToPlayerControlled();
        return;
    }

    if (static_cast<s32>(sGnFrame) <= field_14C_timer)
    {
        if (!(static_cast<s32>(sGnFrame) % 4))
        {
            const FP xOff = (GetSpriteScale() * FP_FromInteger(Math_RandomRange(-20, 20) + (GetAnimation().GetFlipX() ? -10 : 10)));
            const FP yOff = (GetSpriteScale() * FP_FromInteger(Math_RandomRange(-20, 10)));
            New_TintChant_Particle(
                xOff + mXPos,
                yOff + mYPos,
                GetSpriteScale(),
                Layer::eLayer_0);
        }
    }
    else
    {
        BlowUp();
    }
}

void FlyingSlig::Brain_15_FlyingSligSpawn()
{
    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    if (FP_Abs(mXPos - field_1C8_lever_pull_range_xpos) >= FP_FromInteger(1) || FP_Abs(mYPos - field_1CC_lever_pull_range_ypos) >= FP_FromInteger(1))
    {
        const FP delta1 = field_1C8_lever_pull_range_xpos - mXPos;
        const FP delta2 = field_1CC_lever_pull_range_ypos - mYPos;

        if (FP_Abs(field_1C8_lever_pull_range_xpos - mXPos) <= FP_FromInteger(2))
        {
            mXPos = field_1C8_lever_pull_range_xpos;
        }
        else if (delta1 >= FP_FromInteger(2))
        {
            mXPos += FP_FromInteger(2);
        }
        else
        {
            mXPos -= FP_FromInteger(2);
        }

        if (FP_Abs(field_1CC_lever_pull_range_ypos - mYPos) <= FP_FromInteger(2))
        {
            mYPos = field_1CC_lever_pull_range_ypos;
        }
        else if (delta2 >= FP_FromInteger(2))
        {
            mYPos += FP_FromInteger(2);
        }
        else
        {
            mYPos -= FP_FromInteger(2);
        }

        if ((FP_Abs(mXPos - field_1C8_lever_pull_range_xpos) < FP_FromInteger(1)) && (FP_Abs(mYPos - field_1CC_lever_pull_range_ypos) < FP_FromInteger(1)))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_7_LeverPull);
        }
    }
    else if (GetCurrentMotion() == eFlyingSligMotions::Motion_0_Idle)
    {
        ToPlayerControlled();
    }
}

void FlyingSlig::Brain_17_FromCrawlingSlig()
{
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos - FP_FromInteger(5),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            CollisionMask(eTrackLine_8))
        == 1)
    {
        mXPos = hitX;
        mYPos = hitY;
        sub_4348A0();
        mVelY = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);
        field_188_ySpeed = FP_FromInteger(0);
        field_184_xSpeed = FP_FromInteger(0);
        PatrolDelay();
    }
    else
    {
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromDouble(-4.5);
        field_184_xSpeed = FP_FromInteger(0);
        field_188_ySpeed = FP_FromDouble(-0.5);
    }
}

void FlyingSlig::Motion_0_Idle()
{
    if (!field_28C_bobbing_values_table_index)
    {
        field_28C_bobbing_values_table_index = 2;
        field_290_bobbing_values_index = 0;
    }

    if (field_184_xSpeed != FP_FromInteger(0))
    {
        if (IsFacingMovementDirection())
        {
            SetMotionHelper(eFlyingSligMotions::Motion_16_IdleToHorizontalMovement);
            field_28C_bobbing_values_table_index = 1;
            field_290_bobbing_values_index = 0;
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_2_IdleToTurn);
        }
    }
    else
    {
        if (field_188_ySpeed > FP_FromInteger(0))
        {
            if (field_188_ySpeed >= FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_17_BeginDownMovement);
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_11_BeginUpMovement);
            }
        }
    }
}

void FlyingSlig::Motion_1_HorizontalMovement()
{
    if (field_188_ySpeed < FP_FromInteger(0))
    {
        SetMotionHelper(eFlyingSligMotions::Motion_24_HorizontalToUpMovement);
    }
    else if (field_188_ySpeed > FP_FromInteger(0))
    {
        SetMotionHelper(eFlyingSligMotions::Motion_12_HorizontalToDownMovement);
    }
    else if (field_184_xSpeed == FP_FromInteger(0))
    {
        SetMotionHelper(eFlyingSligMotions::Motion_10_EndHorizontalMovement);
    }
    else if (!IsFacingMovementDirection())
    {
        SetMotionHelper(eFlyingSligMotions::Motion_15_QuickTurn);
        field_28C_bobbing_values_table_index = 3;
        field_290_bobbing_values_index = 0;
    }
}

void FlyingSlig::Motion_2_IdleToTurn()
{
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            GetAnimation().ToggleFlipX();
            if (field_188_ySpeed >= FP_FromInteger(0))
            {
                if (field_188_ySpeed <= FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection())
                    {
                        GetAnimation().ToggleFlipX();
                        SetMotionHelper(eFlyingSligMotions::Motion_25_TurnToHorizontalMovement);
                    }
                    else
                    {
                        SetMotionHelper(eFlyingSligMotions::Motion_15_QuickTurn);
                    }
                }
                else
                {
                    SetMotionHelper(eFlyingSligMotions::Motion_12_HorizontalToDownMovement);
                    field_28C_bobbing_values_table_index = 0;
                }
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_24_HorizontalToUpMovement);
                field_28C_bobbing_values_table_index = 0;
            }
        }
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_0_Idle);
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_17_BeginDownMovement);
            }
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_11_BeginUpMovement);
        }
    }
}

void FlyingSlig::Motion_3_DownMovement()
{
    if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection())
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            if (field_184_xSpeed != FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_14_DownToHorizontalMovement);
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_18_EndDownMovement);
            }
        }
    }
    else
    {
        SetMotionHelper(eFlyingSligMotions::Motion_4_DownMovementToTurn);
    }
}

void FlyingSlig::Motion_4_DownMovementToTurn()
{
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();

        if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection())
        {
            if (field_188_ySpeed > FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_3_DownMovement);
            }
            else if (field_184_xSpeed != FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_14_DownToHorizontalMovement);
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_18_EndDownMovement);
            }
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_4_DownMovementToTurn);
        }
    }
}

void FlyingSlig::Motion_5_UpMovement()
{
    if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection())
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_184_xSpeed != FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_13_UpToHorizontalMovement);
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_21_EndUpMovement);
            }
        }
    }
    else
    {
        SetMotionHelper(eFlyingSligMotions::Motion_6_UpMovementToTurn);
    }
}

void FlyingSlig::Motion_6_UpMovementToTurn()
{
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();

        if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection())
        {
            if (field_188_ySpeed < FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_5_UpMovement);
            }
            else if (field_184_xSpeed != FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_13_UpToHorizontalMovement);
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_21_EndUpMovement);
            }
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_6_UpMovementToTurn);
        }
    }
}

void FlyingSlig::Motion_7_LeverPull()
{
    if (GetAnimation().GetCurrentFrame())
    {
        if (GetAnimation().GetIsLastFrame())
        {
            SetMotionHelper(eFlyingSligMotions::Motion_0_Idle);
        }
    }
    else
    {
        auto pSwitch = static_cast<Lever*>(sObjectIds.Find_Impl(field_158_obj_id));
        if (pSwitch)
        {
            pSwitch->VPull(mXPos < pSwitch->mXPos);
        }
        field_158_obj_id = Guid{};
    }
}

void FlyingSlig::Motion_8_GameSpeak()
{
    if (GetAnimation().GetCurrentFrame() == 1 && mSpeaking1)
    {
        mSpeaking1 = false;

        if (BrainIs(&FlyingSlig::Brain_12_Possessed))
        {
            switch (field_17D_next_speak)
            {
                case SligSpeak::eHi_0:
                    gEventSystem->PushEvent(GameSpeakEvents::eSlig_Hi_27);
                    break;
                case SligSpeak::eHereBoy_1:
                    gEventSystem->PushEvent(GameSpeakEvents::eSlig_HereBoy_28);
                    break;
                case SligSpeak::eGetHim_2:
                    gEventSystem->PushEvent(GameSpeakEvents::eSlig_GetEm_29);
                    break;
                case SligSpeak::eLaugh_3:
                    gEventSystem->PushEvent(GameSpeakEvents::Slig_Laugh_8);
                    break;
                case SligSpeak::eBullshit_5:
                    gEventSystem->PushEvent(GameSpeakEvents::Slig_BS_5);
                    break;
                case SligSpeak::eLookOut_6:
                    gEventSystem->PushEvent(GameSpeakEvents::Slig_LookOut_6);
                    break;
                case SligSpeak::eBullshit2_7:
                    gEventSystem->PushEvent(GameSpeakEvents::Slig_BS2_7);
                    break;
                case SligSpeak::eFreeze_8:
                    gEventSystem->PushEvent(GameSpeakEvents::eSlig_Freeze_31);
                    break;
                default:
                    break;
            }
        }
        Slig_GameSpeak_SFX(field_17D_next_speak, 0, field_160_voice_pitch_min, this);
        EventBroadcast(kEventSpeaking, this);
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        this->SetMotionHelper(eFlyingSligMotions::Motion_0_Idle);
    }
}

void FlyingSlig::Motion_9_Possession()
{
    
}

void FlyingSlig::Motion_10_EndHorizontalMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetMotionHelper(eFlyingSligMotions::Motion_0_Idle);
    }
}

void FlyingSlig::Motion_11_BeginUpMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (field_184_xSpeed > FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection())
            {
                SetMotionHelper(eFlyingSligMotions::Motion_6_UpMovementToTurn);
                return;
            }
        }
        else if (field_188_ySpeed >= FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_21_EndUpMovement);
            return;
        }
        SetMotionHelper(eFlyingSligMotions::Motion_5_UpMovement);
    }
}

void FlyingSlig::Motion_12_HorizontalToDownMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection())
            {
                SetMotionHelper(eFlyingSligMotions::Motion_4_DownMovementToTurn);
                return;
            }
        }
        else if (field_188_ySpeed <= FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_18_EndDownMovement);
            return;
        }
        SetMotionHelper(eFlyingSligMotions::Motion_3_DownMovement);
    }
}

void FlyingSlig::Motion_13_UpToHorizontalMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_1_HorizontalMovement);
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_12_HorizontalToDownMovement);
        }
        if (GetAnimation().GetIsLastFrame())
        {
            if (field_188_ySpeed > FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_12_HorizontalToDownMovement);
                field_28C_bobbing_values_table_index = 0;
            }
            else if (field_188_ySpeed < FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_24_HorizontalToUpMovement);
                field_28C_bobbing_values_table_index = 0;
            }
            else if (field_184_xSpeed == FP_FromInteger(0))
            {
                SetMotionHelper(eFlyingSligMotions::Motion_10_EndHorizontalMovement);
            }
            else if (IsFacingMovementDirection())
            {
                SetMotionHelper(eFlyingSligMotions::Motion_1_HorizontalMovement);
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_15_QuickTurn);
            }
        }
    }
}

void FlyingSlig::Motion_14_DownToHorizontalMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                if (field_184_xSpeed > FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection())
                    {
                        SetMotionHelper(eFlyingSligMotions::Motion_1_HorizontalMovement);
                    }
                    else
                    {
                        SetMotionHelper(eFlyingSligMotions::Motion_15_QuickTurn);
                    }
                }
                else
                {
                    SetMotionHelper(eFlyingSligMotions::Motion_10_EndHorizontalMovement);
                }
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_12_HorizontalToDownMovement);
                field_28C_bobbing_values_table_index = 0;
            }
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_24_HorizontalToUpMovement);
            field_28C_bobbing_values_table_index = 0;
        }
    }
}

void FlyingSlig::Motion_15_QuickTurn()
{
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();

        if (field_188_ySpeed < FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_24_HorizontalToUpMovement);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_188_ySpeed > FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_12_HorizontalToDownMovement);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_184_xSpeed == FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_10_EndHorizontalMovement);
        }
        else if (!IsFacingMovementDirection())
        {
            SetMotionHelper(eFlyingSligMotions::Motion_15_QuickTurn);
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_1_HorizontalMovement);
        }
    }
}

void FlyingSlig::Motion_16_IdleToHorizontalMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (field_188_ySpeed < FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_24_HorizontalToUpMovement);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_188_ySpeed > FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_12_HorizontalToDownMovement);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_184_xSpeed == FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_10_EndHorizontalMovement);
        }
        else if (IsFacingMovementDirection())
        {
            SetMotionHelper(eFlyingSligMotions::Motion_1_HorizontalMovement);
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_15_QuickTurn);
        }
    }
}

void FlyingSlig::Motion_17_BeginDownMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection())
            {
                SetMotionHelper(eFlyingSligMotions::Motion_4_DownMovementToTurn);
                return;
            }
        }
        else if (field_188_ySpeed <= FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_18_EndDownMovement);
            return;
        }
        SetMotionHelper(eFlyingSligMotions::Motion_3_DownMovement);
    }
}

void FlyingSlig::Motion_18_EndDownMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_0_Idle);
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_11_BeginUpMovement);
        }
    }
}

void FlyingSlig::Motion_19_DownKnockback()
{
    if (FP_Abs(field_184_xSpeed) > FP_FromDouble(0.1) || field_188_ySpeed < FP_FromDouble(0.1))
    {
        SetMotionHelper(eFlyingSligMotions::Motion_18_EndDownMovement);
    }
}

void FlyingSlig::Motion_20_UpKnockback()
{
    if (FP_Abs(field_184_xSpeed) > FP_FromDouble(0.1) || field_188_ySpeed > FP_FromDouble(-0.1))
    {
        SetMotionHelper(eFlyingSligMotions::Motion_0_Idle);
    }
}

void FlyingSlig::Motion_21_EndUpMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_0_Idle);
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_17_BeginDownMovement);
        }
    }
}

void FlyingSlig::Motion_22_InstantUpXTurn()
{
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        SetMotionHelper(eFlyingSligMotions::Motion_5_UpMovement);
    }
}

void FlyingSlig::Motion_23_InstantDownXTurn()
{
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        SetMotionHelper(eFlyingSligMotions::Motion_3_DownMovement);
    }
}

void FlyingSlig::Motion_24_HorizontalToUpMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection())
            {
                SetMotionHelper(eFlyingSligMotions::Motion_6_UpMovementToTurn);
                return;
            }
        }
        else if (field_188_ySpeed >= FP_FromInteger(0))
        {
            SetMotionHelper(eFlyingSligMotions::Motion_21_EndUpMovement);
            return;
        }
        SetMotionHelper(eFlyingSligMotions::Motion_5_UpMovement);
    }
}

void FlyingSlig::Motion_25_TurnToHorizontalMovement()
{
    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();

        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                if (field_184_xSpeed != FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection())
                    {
                        SetMotionHelper(eFlyingSligMotions::Motion_1_HorizontalMovement);
                    }
                    else
                    {
                        SetMotionHelper(eFlyingSligMotions::Motion_15_QuickTurn);
                    }
                }
                else
                {
                    SetMotionHelper(eFlyingSligMotions::Motion_10_EndHorizontalMovement);
                }
            }
            else
            {
                SetMotionHelper(eFlyingSligMotions::Motion_12_HorizontalToDownMovement);
                field_28C_bobbing_values_table_index = 0;
            }
        }
        else
        {
            SetMotionHelper(eFlyingSligMotions::Motion_24_HorizontalToUpMovement);
            field_28C_bobbing_values_table_index = 0;
        }
    }
}

s16 FlyingSlig::IsFacingMovementDirection()
{
    return ((field_184_xSpeed > FP_FromInteger(0) && !(GetAnimation().GetFlipX()))
            || (field_184_xSpeed < FP_FromInteger(0) && GetAnimation().GetFlipX()));
}

void FlyingSlig::ToPlayerControlled()
{
    SetMotionHelper(eFlyingSligMotions::Motion_0_Idle);
    SetBrain(&FlyingSlig::Brain_12_Possessed);
}

void FlyingSlig::ToMoving()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    SetBrain(&FlyingSlig::Brain_2_Moving);
}

void FlyingSlig::ToPanicIdle()
{
    Say(SligSpeak::eHelp_10, 0);
    field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + field_118_data.mPanicDelay;
    SetBrain(&FlyingSlig::Brain_8_PanicIdle);
}

void FlyingSlig::ToChase()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    SetBrain(&FlyingSlig::Brain_4_ChasingEnemy);
}

s16 FlyingSlig::IsPossessed()
{
    return BrainIs(&FlyingSlig::Brain_12_Possessed) || BrainIs(&FlyingSlig::Brain_15_FlyingSligSpawn) || BrainIs(&FlyingSlig::Brain_17_FromCrawlingSlig) || BrainIs(&FlyingSlig::Brain_13_Possession) || BrainIs(&FlyingSlig::Brain_14_DePossession);
}

s16 FlyingSlig::CanChase(IBaseAliveGameObject* pObj)
{
    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) || EventGet(kEventResetting) || IsAbeEnteringDoor(pObj) || sActiveHero->GetSpriteScale() != GetSpriteScale() || !IsWallBetween(this, pObj))
    {
        return 0;
    }

    if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, AsEventScale(GetScale())))
    {
        return 1;
    }

    if (VIsFacingMe(pObj) && !IsInInvisibleZone(pObj) && !pObj->GetInvisible())
    {
        return 1;
    }

    return 0;
}

void FlyingSlig::Say(SligSpeak speak, s16 pitch)
{
    if (GetCurrentMotion() != eFlyingSligMotions::Motion_8_GameSpeak)
    {
        mSpeaking1 = true;
        mSpeaking2 = true;
        field_17D_next_speak = speak;
        field_160_voice_pitch_min = pitch;
    }
}

s16 FlyingSlig::sub_4374A0(s16 a2)
{
    mLastLine = false;

    if (mUnknown2)
    {
        if (!mNoNextLine && (field_182_bound1 == ReliveTypes::eNone || !a2))
        {
            if (!mUnknown1)
            {
                field_190 = field_2B8_max_speed_up;
            }
            else
            {
                const FP v64 = FP_Abs(((field_18C * field_18C) / field_2B4_max_slow_down) * FP_FromDouble(0.5));
                if (v64 < FP_Abs(field_1C4 - field_194))
                {
                    field_190 = field_2B8_max_speed_up;
                }
            }
        }
        else
        {
            FP directedXMaxSpeedUp = field_2B8_max_speed_up + field_18C;
            if (directedXMaxSpeedUp > field_2A8_max_x_speed)
            {
                directedXMaxSpeedUp = field_2A8_max_x_speed;
            }

            FP left = {};
            FP right = {};
            if (directedXMaxSpeedUp <= FP_FromInteger(0))
            {
                left = field_1A4_rect.x;
                right = field_1A4_rect.w;
            }
            else
            {
                left = field_1A4_rect.w;
                right = field_1A4_rect.x;
            }

            const FP width = left - right;
            if (width > FP_FromInteger(0) && field_182_bound1 == ReliveTypes::eSligBoundLeft)
            {
                field_190 = field_2B8_max_speed_up;
            }
            else if (width < FP_FromInteger(0) && field_182_bound1 == ReliveTypes::eSligBoundRight)
            {
                field_190 = field_2B8_max_speed_up;
            }
            else
            {
                if (width == FP_FromInteger(0) && !mNoNextLine)
                {
                    field_190 = field_2B8_max_speed_up;
                }
                else
                {
                    mLastLine = true;

                    const FP v65 = FP_Abs((((field_18C * field_18C) / field_2B4_max_slow_down) * FP_FromDouble(0.5)));
                    const FP v27 = field_198_line_length - field_194;
                    if (v27 < field_2A8_max_x_speed && field_18C == FP_FromInteger(0))
                    {
                        return 1;
                    }

                    if (v65 < v27)
                    {
                        field_190 = field_2B8_max_speed_up;
                    }
                }
            }
        }
    }
    else
    {
        if (!mNoPrevLine && (field_180_bound2 == ReliveTypes::eNone || !a2))
        {
            if (!mUnknown1)
            {
                field_190 = -field_2B8_max_speed_up;
            }
            else
            {
                const FP v66 = FP_Abs(((field_18C * field_18C) / field_2B4_max_slow_down) * FP_FromDouble(0.5));
                if (v66 < FP_Abs(field_1C4 - field_194))
                {
                    field_190 = -field_2B8_max_speed_up;
                }
            }
        }
        else
        {
            FP directedXMaxSpeed = -field_2A8_max_x_speed;
            if (field_18C - field_2B8_max_speed_up >= directedXMaxSpeed)
            {
                directedXMaxSpeed = field_18C - field_2B8_max_speed_up;
            }

            FP left = {};
            FP right = {};
            if (directedXMaxSpeed <= FP_FromInteger(0))
            {
                left = field_1A4_rect.x;
                right = field_1A4_rect.w;
            }
            else
            {
                left = field_1A4_rect.w;
                right = field_1A4_rect.x;
            }

            const FP width = left - right;
            if (width > FP_FromInteger(0) && field_180_bound2 == ReliveTypes::eSligBoundLeft)
            {
                field_190 = -field_2B8_max_speed_up;
            }
            else if (width < FP_FromInteger(0) && field_180_bound2 == ReliveTypes::eSligBoundRight)
            {
                field_190 = -field_2B8_max_speed_up;
            }
            else
            {
                if (width == FP_FromInteger(0) && !mNoPrevLine)
                {
                    field_190 = -field_2B8_max_speed_up;
                }
                else
                {
                    mLastLine = true;
                    const FP v2 = FP_Abs(((field_18C * field_18C) / field_2B4_max_slow_down) * FP_FromDouble(0.5));
                    if (field_194 < field_2A8_max_x_speed && field_18C == FP_FromInteger(0))
                    {
                        return 1;
                    }

                    if (v2 < field_194)
                    {
                        field_190 = -field_2B8_max_speed_up;
                    }
                }
            }
        }
    }

    if (field_190 > FP_FromInteger(0))
    {
        field_184_xSpeed = field_1A4_rect.w - field_1A4_rect.x;
        field_188_ySpeed = field_1A4_rect.h - field_1A4_rect.y;
    }
    else if (field_190 < FP_FromInteger(0))
    {
        field_184_xSpeed = field_1A4_rect.x - field_1A4_rect.w;
        field_188_ySpeed = field_1A4_rect.y - field_1A4_rect.h;
    }

    return 0;
}

s16 FlyingSlig::IsAbeEnteringDoor(IBaseAliveGameObject* pThis)
{
    return Slig::IsAbeEnteringDoor(pThis);
}

bool FlyingSlig::IsWallBetween(IBaseAliveGameObject* pThis, IBaseAliveGameObject* pObj)
{
    // TODO: Duplicated like IsAbeEnteringDoor ??
    const PSX_RECT bRect = pObj->VGetBoundingRect();

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return gCollisions->Raycast(
               pThis->mXPos,
               pThis->mYPos,
               pObj->mXPos,
               FP_FromInteger((bRect.y + bRect.h) / 2),
               &pLine,
               &hitX,
               &hitY,
               pThis->GetScale() == Scale::Fg ? kFgFloorWallOrCeiling : kBgFloorWallOrCeiling)
        != 1;
}

void FlyingSlig::ThrowGrenade()
{
    FP grenadeXVel = (FP_FromInteger(Math_RandomRange(50, 64)) / FP_FromInteger(10) * GetSpriteScale());
    const FP grenadeYVel = (FP_FromInteger(-6) * GetSpriteScale());

    FP grenadeXPos = (FP_FromInteger(0) * GetSpriteScale());
    const FP grenadeYPos = (FP_FromInteger(-5) * GetSpriteScale());

    const FP xpos = (FP_FromInteger(0) * GetSpriteScale());
    const FP ypos = (FP_FromInteger(-20) * GetSpriteScale());

    if (GetAnimation().GetFlipX())
    {
        grenadeXPos = -grenadeXPos;
        grenadeXVel = -grenadeXVel;
    }

    auto pGrenade = relive_new Grenade(grenadeXPos + mXPos, grenadeYPos + mYPos, 0, 1, this);
    if (pGrenade)
    {
        pGrenade->SetSpriteScale(GetSpriteScale());
        pGrenade->SetScale(GetScale());
        pGrenade->VThrow(grenadeXVel, grenadeYVel);
    }

    New_ShootingFire_Particle(xpos + mXPos, ypos + mYPos, GetAnimation().GetFlipX(), GetSpriteScale());
    Slig_SoundEffect(SligSfx::eThrowGrenade_8, this);
    EventBroadcast(kEventShooting, this);
    EventBroadcast(kEventLoudNoise, this);

    Dove::All_FlyAway(0);

    s32 randomisedGrenadeDelay = field_118_data.mGrenadeDelay + (Math_NextRandom() & 7);
    if (randomisedGrenadeDelay < 20)
    {
        randomisedGrenadeDelay = 20;
    }
    field_150_grenade_delay = randomisedGrenadeDelay + sGnFrame;

    if (IsPossessed() == 0 && Math_NextRandom() < 168u)
    {
        Slig_GameSpeak_SFX(SligSpeak::eHereBoy_1, 0, field_15C_voice_pitch_min, this);
    }
}

void FlyingSlig::BlowUp()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    relive_new Gibs(GibType::Slig_1, mXPos, mYPos, mVelX, mVelY, GetSpriteScale(), 0);

    relive_new Blood(mXPos, mYPos - (FP_FromInteger(30) * GetSpriteScale()), FP_FromInteger(0), FP_FromInteger(0), GetSpriteScale(), 20);

    New_Smoke_Particles(mXPos, mYPos - (FP_FromInteger(30) * GetSpriteScale()), GetSpriteScale(), 3, RGB16{ 128, 128, 128 });
    SfxPlayMono(relive::SoundEffects::KillEffect, 128, GetSpriteScale());
    SfxPlayMono(relive::SoundEffects::FallingItemHit, 90, GetSpriteScale());

    GetAnimation().SetAnimate(false);
    GetAnimation().SetRender(false);

    field_18C = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);
    field_184_xSpeed = FP_FromInteger(0);
    mHealth = FP_FromInteger(0);
    SetBrain(&FlyingSlig::Brain_1_Death);
    field_14C_timer = sGnFrame + 40;
}

s16 FlyingSlig::sub_436730()
{
    if (CanChase(sControlledCharacter))
    {
        ToSpottedEnemy();
        return 1;
    }
    else if (CanHearAbe() && !BrainIs(&FlyingSlig::Brain_3_GetAlerted))
    {
        ToAlerted();
        return 1;
    }
    else if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, AsEventScale(GetScale())))
    {
        ToPanicMoving();
        return 1;
    }
    else
    {
        if (sActiveHero && sActiveHero->mHealth < FP_FromInteger(0) && FP_Abs(mXPos - sActiveHero->mXPos) < FP_FromInteger(640) && FP_Abs(mYPos - sActiveHero->mYPos) < FP_FromInteger(240))
        {
            ToAbeDead();
            return 1;
        }
        return 0;
    }
}

s16 FlyingSlig::CanHearAbe()
{
    return IsActiveHero(IsEventInRange(kEventSuspiciousNoise, mXPos, mYPos, AsEventScale(GetScale()))) || IsActiveHero(IsEventInRange(kEventSpeaking, mXPos, mYPos, AsEventScale(GetScale())));
}

void FlyingSlig::ToSpottedEnemy()
{
    if (field_118_data.mPrechaseDelay)
    {
        Say(SligSpeak ::eFreeze_8, 0);
        field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + field_118_data.mPrechaseDelay;
        SetBrain(&FlyingSlig::Brain_9_SpottedEnemy);
    }
    else
    {
        Slig_GameSpeak_SFX(SligSpeak::eFreeze_8, 0, field_15C_voice_pitch_min, this);
        ToChase();
    }
}

void FlyingSlig::ToAbeDead()
{
    SetBrain(&FlyingSlig::Brain_11_AbeDead);
    field_14C_timer = sGnFrame + 70;
}

void FlyingSlig::ToAlerted()
{
    Say(SligSpeak ::eWhat_9, 0);
    SetBrain(&FlyingSlig::Brain_3_GetAlerted);
    field_14C_timer = sGnFrame + field_118_data.mAlertedListenTime;
}

void FlyingSlig::ToPanicMoving()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + 12;
    SetBrain(&FlyingSlig::Brain_7_PanicMoving);
}

s16 FlyingSlig::IsTurning()
{
    return GetCurrentMotion() == eFlyingSligMotions::Motion_2_IdleToTurn ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_15_QuickTurn ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_4_DownMovementToTurn ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_6_UpMovementToTurn;
}

void FlyingSlig::ToChantShake()
{
    SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
    SetMotionHelper(eFlyingSligMotions::Motion_9_Possession);
    SetBrain(&FlyingSlig::Brain_14_DePossession);
    field_14C_timer = sGnFrame + 45;
}

void FlyingSlig::ToPossesed()
{
    Slig_GameSpeak_SFX(SligSpeak::eHelp_10, 0, field_15C_voice_pitch_min, this);
    SetMotionHelper(eFlyingSligMotions::Motion_9_Possession);
    SetBrain(&FlyingSlig::Brain_13_Possession);
    field_14C_timer = sGnFrame + 35;
}

void FlyingSlig::vUpdateAnimRes_4350A0()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sFlyingSligAnimIdTable[mCurrentMotion]));
}

void FlyingSlig::PatrolDelay()
{
    if (BrainIs(&FlyingSlig::Brain_4_ChasingEnemy))
    {
        field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + field_118_data.mGiveUpChaseDelay;
        SetBrain(&FlyingSlig::Brain_5_Idle);
        return;
    }

    field_14C_timer = field_118_data.mPatrolPauseMin + sGnFrame;
    if (field_118_data.mPatrolPauseMin <= field_118_data.mPatrolPauseMax)
    {
        SetBrain(&FlyingSlig::Brain_5_Idle);
        return;
    }

    field_14C_timer += Math_NextRandom() % (field_118_data.mPatrolPauseMin - field_118_data.mPatrolPauseMax);
    SetBrain(&FlyingSlig::Brain_5_Idle);
}

s16 FlyingSlig::CanThrowGrenade()
{
    return GetCurrentMotion() == eFlyingSligMotions::Motion_0_Idle ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_1_HorizontalMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_3_DownMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_5_UpMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_16_IdleToHorizontalMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_10_EndHorizontalMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_11_BeginUpMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_13_UpToHorizontalMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_12_HorizontalToDownMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_14_DownToHorizontalMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_18_EndDownMovement ||
        GetCurrentMotion() == eFlyingSligMotions::Motion_17_BeginDownMovement;
}

void FlyingSlig::ToLaunchingGrenade()
{
    if (!VIsFacingMe(sControlledCharacter) && !IsTurning())
    {
        if (field_18C == FP_FromInteger(0))
        {
            mAlertedAndNotFacingAbe = true;
        }
        mUnknown2 = !mUnknown2;
    }
    SetBrain(&FlyingSlig::Brain_10_LaunchingGrenade);
}

void FlyingSlig::HandlePlayerControls()
{
    /* TODO OG issue - these local statics are never used ??
    if (!(byte_5BC5F4 & 1))
    {
        byte_5BC5F4 |= 1u;
        dword_5BC5F8 = Math_FixedPoint_Divide(0x800000, field_2B8);
    }
    if (!(byte_5BC5F4 & 2))
    {
        dword_5BC5F0 = 0x3F0000;
        byte_5BC5F4 |= 2u;
    }
    */

    mThrowGrenade = false;
    mAlertedAndNotFacingAbe = false;
    mDoAction = false;

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    if (Input().IsAnyPressed(InputCommands::eThrowItem))
    {
        mThrowGrenade = true;
    }

    if (Input().IsAnyPressed(InputCommands::eDoAction))
    {
        mDoAction = true;
    }

    if (!mSpeaking1)
    {
        mSpeaking1 = true;
        mSpeaking2 = true;

        field_160_voice_pitch_min = 0;

        if (Input().IsAnyPressed(InputCommands::eGameSpeak1))
        {
            field_17D_next_speak = SligSpeak::eHi_0;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak3))
        {
            field_17D_next_speak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak::eGetHim_2 : SligSpeak::eFreeze_8;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak4))
        {
            field_17D_next_speak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak::eFreeze_8 : SligSpeak::eGetHim_2;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak2))
        {
            field_17D_next_speak = SligSpeak::eHereBoy_1;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak6))
        {
            field_17D_next_speak = SligSpeak::eBullshit_5;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak7))
        {
            field_17D_next_speak = SligSpeak::eLookOut_6;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak5))
        {
            field_17D_next_speak = SligSpeak::eBullshit2_7;
        }
        else if (Input().IsAnyPressed(InputCommands::eGameSpeak8))
        {
            field_17D_next_speak = SligSpeak::eLaugh_3;
        }
        else
        {
            mSpeaking1 = false;
            mSpeaking2 = false;
        }
    }

    if (Input().IsAnyHeld(InputCommands::eUp))
    {
        if (Input().IsAnyHeld(InputCommands::eLeft))
        {
            field_184_xSpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            return;
        }
        if (Input().IsAnyHeld(InputCommands::eRight))
        {
            field_184_xSpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            return;
        }
        field_184_xSpeed = FP_FromInteger(0);
        field_188_ySpeed = -field_2B8_max_speed_up;
    }
    else if (Input().IsAnyHeld(InputCommands::eDown))
    {
        if (Input().IsAnyHeld(InputCommands::eLeft))
        {
            field_184_xSpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
        }
        else if (Input().IsAnyHeld(InputCommands::eRight))
        {
            field_184_xSpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
        }
        else
        {
            field_184_xSpeed = FP_FromInteger(0);
            field_188_ySpeed = field_2B8_max_speed_up;
        }
    }
    else if (Input().IsAnyHeld(InputCommands::eLeft))
    {
        field_188_ySpeed = FP_FromInteger(0);
        field_184_xSpeed = -field_2B8_max_speed_up;
    }
    else if (Input().IsAnyHeld(InputCommands::eRight))
    {
        field_188_ySpeed = FP_FromInteger(0);
        field_184_xSpeed = field_2B8_max_speed_up;
    }
}

s16 FlyingSlig::sub_437C70(PathLine* pLine)
{
    BaseAliveGameObjectCollisionLine = pLine;
    if (!BaseAliveGameObjectCollisionLine)
    {
        return 0;
    }

    field_1A4_rect.x = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.x);
    field_1A4_rect.y = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
    field_1A4_rect.w = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.w);
    field_1A4_rect.h = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.h);


    field_1EC_pNextLine = gCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLine->mNext);
    field_1F0_pPrevLine = gCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLine->mPrevious);

    field_198_line_length = FP_FromInteger(BaseAliveGameObjectCollisionLine->mLineLength);

    mNoPrevLine = field_1F0_pPrevLine == nullptr;
    mNoNextLine = field_1EC_pNextLine == nullptr;

    field_182_bound1 = FindLeftOrRightBound(field_1A4_rect.w, field_1A4_rect.h);
    field_180_bound2 = FindLeftOrRightBound(field_1A4_rect.x, field_1A4_rect.y);
    field_1BC = Math_Tan(field_1A4_rect.y - field_1A4_rect.h, field_1A4_rect.w - field_1A4_rect.x);

    field_1C0 = field_1BC + FP_FromInteger(128);

    if (field_1C0 >= FP_FromInteger(256))
    {
        field_1C0 -= FP_FromInteger(256);
    }
    return 1;
}

ReliveTypes FlyingSlig::FindLeftOrRightBound(FP xOrY, FP wOrH)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    const FP left = xOrY - kGridSize;
    const FP top = wOrH - kGridSize;
    const FP right = xOrY + kGridSize;
    const FP bottom = wOrH + kGridSize;

    // TODO: Check left is really Abs'd.
    ReliveTypes found_type = ReliveTypes::eNone;

    if (gPathInfo->TLV_Get_At(FP_GetExponent(FP_Abs(left)), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), ReliveTypes::eSligBoundLeft))
    {
        found_type = ReliveTypes::eSligBoundLeft;
    }
    else if (gPathInfo->TLV_Get_At(FP_GetExponent(left), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), ReliveTypes::eSligBoundRight))
    {
        found_type = ReliveTypes::eSligBoundRight;
    }
    return found_type;
}

void FlyingSlig::VPossessed()
{
    SetPossessed(true);
    mSpeaking1 = true;

    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;

    field_2A8_max_x_speed = FP_FromDouble(5.5) * GetSpriteScale();
    field_2AC_up_vel = FP_FromDouble(-5.5) * GetSpriteScale();
    field_2B0_down_vel = FP_FromDouble(5.5) * GetSpriteScale();
    field_2B4_max_slow_down = FP_FromDouble(0.25) * GetSpriteScale();
    field_2B8_max_speed_up = FP_FromDouble(0.7) * GetSpriteScale();

    ToPossesed();
}

s16 FlyingSlig::sub_436C60(PSX_RECT* pRect, s16 arg_4)
{
    if (!pRect)
    {
        return 0;
    }

    FP v1 = {};
    if (FP_FromInteger(std::abs(pRect->w - pRect->x)) < FP_FromInteger(1))
    {
        // Set to a huge value if near 0.
        v1 = FP_FromInteger(9999);
    }
    else
    {
        const s32 a1 = pRect->h - pRect->y;
        const s32 a2 = pRect->w - pRect->x;
        v1 = FP_Abs(FP_FromInteger((a1) / a2));
    }

    if (v1 < FP_FromInteger(4))
    {
        const FP k80Scaled = (FP_FromInteger(80) * GetSpriteScale());

        const FP rLeft = sControlledCharacter->mXPos - k80Scaled;
        const FP rRight = sControlledCharacter->mXPos + k80Scaled;

        const FP r_x = FP_FromInteger(std::min(pRect->x, pRect->w));
        const FP r_w = FP_FromInteger(std::max(pRect->w, pRect->x));

        const FP r_y = FP_FromInteger(std::min(pRect->y, pRect->h));
        const FP r_h = FP_FromInteger(std::max(pRect->h, pRect->y) + 150);

        s32 bLeftInRect = 0;
        if (rLeft < r_x || rLeft > r_w || sControlledCharacter->mYPos < r_y || sControlledCharacter->mYPos > r_h)
        {
            bLeftInRect = 0;
        }
        else
        {
            bLeftInRect = 1;
        }

        s32 bRightInRect = 0;
        if (rRight < r_x || rRight > r_w || sControlledCharacter->mYPos < r_y || sControlledCharacter->mYPos > r_h)
        {
            bRightInRect = 0;
        }
        else
        {
            bRightInRect = 1;
        }

        if (!bLeftInRect && !bRightInRect)
        {
            return 0;
        }

        if (!arg_4 && (bRightInRect || bLeftInRect))
        {
            return 1;
        }

        const FP v2 = (field_1A4_rect.h - field_1A4_rect.y) / (field_1A4_rect.w - field_1A4_rect.x);
        const FP v3 = field_1A4_rect.y - (v2 * field_1A4_rect.x);

        FP yOff1 = {};
        if (bRightInRect)
        {
            yOff1 = v3 + (v2 * rRight);
            if ((sControlledCharacter->mYPos - (FP_FromInteger(60) * GetSpriteScale()) - yOff1) > (FP_FromInteger(35) * GetSpriteScale()))
            {
                FP hitX = {};
                FP hitY = {};
                PathLine* pLine = nullptr;

                if (gCollisions->Raycast(
                        sControlledCharacter->mXPos,
                        yOff1,
                        sControlledCharacter->mXPos,
                        yOff1 + (FP_FromInteger(35) * GetSpriteScale()),
                        &pLine,
                        &hitX,
                        &hitY,
                        GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
                    == 1)
                {
                    bRightInRect = 0;
                }
            }
        }

        FP yOff2 = {};
        if (bLeftInRect)
        {
            yOff2 = (v2 * rLeft) + v3;

            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;

            if ((sControlledCharacter->mYPos - (FP_FromInteger(60) * GetSpriteScale()) - yOff2) > (FP_FromInteger(35) * GetSpriteScale()))
            {
                if (gCollisions->Raycast(
                        sControlledCharacter->mXPos,
                        yOff2,
                        sControlledCharacter->mXPos,
                        yOff2 + (FP_FromInteger(35) * GetSpriteScale()),
                        &pLine,
                        &hitX,
                        &hitY,
                        GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
                    == 1)
                {
                    bLeftInRect = 0;
                }
            }
        }

        if (bRightInRect || bLeftInRect)
        {
            FP sqrt1 = {};
            FP sqrt2 = {};
            if (bRightInRect)
            {
                sqrt1 = FP_FromInteger(Math_SquareRoot_Int(
                    FP_GetExponent(yOff1 - field_1A4_rect.y) * (FP_GetExponent(yOff1 - field_1A4_rect.y)) + FP_GetExponent(rRight - field_1A4_rect.x) * (FP_GetExponent(rRight - field_1A4_rect.x))));
            }

            if (bLeftInRect)
            {
                const s32 sqrt2_int = Math_SquareRoot_Int(
                    FP_GetExponent(yOff2 - field_1A4_rect.y) * (FP_GetExponent(yOff2 - field_1A4_rect.y)) + FP_GetExponent(rLeft - field_1A4_rect.x) * (FP_GetExponent(rLeft - field_1A4_rect.x)));

                sqrt2 = FP_FromInteger(sqrt2_int);
            }

            if (bRightInRect && bLeftInRect)
            {
                if (FP_Abs(sqrt2 - field_194) >= FP_Abs(sqrt1 - field_194))
                {
                    field_1C4 = sqrt1;
                    return 1;
                }
                else
                {
                    field_1C4 = sqrt2;
                    return 1;
                }
            }
            else if (bRightInRect)
            {
                field_1C4 = sqrt1;
                return 1;
            }
            else if (bLeftInRect)
            {
                field_1C4 = sqrt2;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        const FP k40Scaled = (FP_FromInteger(40) * GetSpriteScale());
        const FP yTop = sControlledCharacter->mYPos - k40Scaled;
        if (pRect->y >= pRect->h)
        {
            if (yTop < FP_FromInteger(pRect->h))
            {
                return 0;
            }

            if (yTop > FP_FromInteger(pRect->y))
            {
                return 0;
            }
        }
        else
        {
            if (yTop < FP_FromInteger(pRect->y))
            {
                return 0;
            }

            if (yTop > FP_FromInteger(pRect->h))
            {
                return 0;
            }
        }

        if (FP_Abs(sControlledCharacter->mXPos - FP_FromInteger(pRect->x)) > (FP_FromInteger(120) * GetSpriteScale()))
        {
            return 0;
        }

        if (arg_4)
        {
            field_1C4 = FP_Abs(yTop - field_1A4_rect.y);
        }

        return 1;
    }
}

bool FlyingSlig::sub_436B20()
{
    PathLine* pLastNextOrPrevLine = nullptr;

    FP lastNextSegmentLength = FP_FromInteger(9999);
    FP totalNextSegmentLength = {};

    PathLine* pNextLine = gCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLine->mNext);
    if (pNextLine)
    {
        while (pNextLine != BaseAliveGameObjectCollisionLine)
        {
            if (sub_436C60(&pNextLine->mRect, 0))
            {
                mUnknown2 = true;
                pLastNextOrPrevLine = pNextLine;
                if (pNextLine)
                {
                    lastNextSegmentLength = totalNextSegmentLength;
                }
                break;
            }
            else
            {
                totalNextSegmentLength += FP_FromInteger(pNextLine->mLineLength);
                pNextLine = gCollisions->Get_Line_At_Idx(pNextLine->mNext);
                if (!pNextLine)
                {
                    break;
                }
            }
        }
    }

    FP totalPrevSegmentLength = {};
    PathLine* pPrevLine = gCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLine->mPrevious);
    if (pPrevLine)
    {
        while (pPrevLine != BaseAliveGameObjectCollisionLine)
        {
            if (sub_436C60(&pPrevLine->mRect, 0))
            {
                if (totalPrevSegmentLength < lastNextSegmentLength)
                {
                    mUnknown2 = false;
                    pLastNextOrPrevLine = pPrevLine;
                }
                break;
            }
            else
            {
                totalPrevSegmentLength += FP_FromInteger(pPrevLine->mLineLength);
                pPrevLine = gCollisions->Get_Line_At_Idx(pPrevLine->mPrevious);
                if (!pPrevLine)
                {
                    break;
                }
            }
        }
    }

    return pLastNextOrPrevLine != nullptr;
}

void FlyingSlig::sub_4373B0()
{
    const FP calc = Math_Tan(mYPos - sControlledCharacter->mYPos, sControlledCharacter->mXPos - mXPos);
    FP value1 = FP_Abs(field_1BC - calc);
    if (value1 > FP_FromInteger(128))
    {
        value1 = FP_FromInteger(256) - value1;
    }

    FP value2 = FP_Abs(field_1C0 - calc);
    if (value2 > FP_FromInteger(128))
    {
        value2 = FP_FromInteger(256) - value2;
    }

    if (value1 >= value2)
    {
        mUnknown2 = false;
    }
    else
    {
        mUnknown2 = true;
    }
}

void FlyingSlig::VSetMotion(s16 newMotion)
{
    SetCurrentMotion(newMotion);
    vUpdateAnimRes_4350A0();
}

s16 FlyingSlig::CollisionUp(FP velY)
{
    const FP y2 = mYPos - (GetSpriteScale() * FP_FromInteger(20)) + velY;

    FP xOff1 = {};
    FP xOff2 = {};
    if (GetAnimation().GetFlipX())
    {
        xOff1 = (GetSpriteScale() * FP_FromInteger(17));
        xOff2 = ((GetSpriteScale() * FP_FromInteger(17)) / FP_FromInteger(3));
    }
    else
    {
        xOff2 = (GetSpriteScale() * FP_FromInteger(17));
        xOff1 = ((GetSpriteScale() * FP_FromInteger(17)) / FP_FromInteger(3));
    }


    const CollisionMask fgMask = CollisionMask(eDynamicCollision_32, eFlyingObjectWall_17, eCeiling_3);
    const CollisionMask bgMask = CollisionMask(eBackgroundDynamicCollision_36, eBackgroundFlyingObjectWall_18, eBackgroundCeiling_7);

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = gCollisions->Raycast(
        mXPos - xOff1,
        mYPos,
        mXPos - xOff1,
        y2,
        &pLine,
        &hitX,
        &hitY,
        GetScale() == Scale::Fg ? fgMask : bgMask);
    
    if (!bCollision)
    {
        bCollision = gCollisions->Raycast(
            xOff2 + mXPos,
            mYPos,
            xOff2 + mXPos,
            y2,
            &pLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? fgMask : bgMask);
    }

    if (bCollision)
    {
        if (FP_Abs(field_184_xSpeed) < FP_FromDouble(0.1) && GetCurrentMotion() != eFlyingSligMotions::Motion_20_UpKnockback)
        {
            SetMotionHelper(eFlyingSligMotions::Motion_20_UpKnockback);
        }

        if (static_cast<s32>(sGnFrame) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX(sGnFrame & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame + 10;
            relive_new ParticleBurst(
                mXPos,
                hitY + (FP_FromInteger(7) * GetSpriteScale()),
                5u,
                GetSpriteScale(),
                BurstType::eSmallPurpleSparks_6,
                9);
        }

        Slig_SoundEffect(sGnFrame & 1 ? SligSfx::eCollideWithWall1_12 : SligSfx::eCollideWithWall2_13, this);

        mYPos += velY + hitY - y2;
        return 1;
    }
    return 0;
}

s16 FlyingSlig::CollisionDown(FP velY)
{
    const FP y2 = (GetSpriteScale() * FP_FromInteger(10)) + mYPos + velY;

    FP xOff1 = {};
    FP xOff2 = {};
    if (GetAnimation().GetFlipX())
    {
        xOff1 = (GetSpriteScale() * FP_FromInteger(17));
        xOff2 = ((GetSpriteScale() * FP_FromInteger(17)) / FP_FromInteger(3));
    }
    else
    {
        xOff1 = ((GetSpriteScale() * FP_FromInteger(17)) / FP_FromInteger(3));
        xOff2 = (GetSpriteScale() * FP_FromInteger(17));
    }

    const CollisionMask fgMask = CollisionMask(eFloor_0, eDynamicCollision_32, eFlyingObjectWall_17);
    const CollisionMask bgMask = CollisionMask(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundFlyingObjectWall_18);

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = gCollisions->Raycast(
        mXPos - xOff1,
        mYPos,
        mXPos - xOff1,
        y2,
        &pLine,
        &hitX,
        &hitY,
        GetScale() == Scale::Fg ? fgMask : bgMask);

    if (!bCollision)
    {
        bCollision = gCollisions->Raycast(
            xOff2 + mXPos,
            mYPos,
            xOff2 + mXPos,
            y2,
            &pLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? fgMask : bgMask);
    }

    if (bCollision)
    {
        if (FP_Abs(field_184_xSpeed) < FP_FromDouble(0.1) && GetCurrentMotion() != eFlyingSligMotions::Motion_19_DownKnockback)
        {
            SetMotionHelper(eFlyingSligMotions::Motion_19_DownKnockback);
        }

        if (static_cast<s32>(sGnFrame) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX(sGnFrame & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame + 10;
        }

        mYPos += velY + hitY - y2;
        return 1;
    }
    return 0;
}

s16 FlyingSlig::CollisionLeftRight(FP velX)
{
    FP newX = {};
    if (velX <= FP_FromInteger(0))
    {
        newX = mXPos - (GetSpriteScale() * FP_FromInteger(17));
    }
    else
    {
        newX = (GetSpriteScale() * FP_FromInteger(17)) + mXPos;
    }

    const FP xOff = velX + newX;
    const FP yOff2 = mYPos - (GetSpriteScale() * FP_FromInteger(20));
    const FP yOff1 = (GetSpriteScale() * FP_FromInteger(20));

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = gCollisions->Raycast(
        mXPos,
        mYPos - yOff1,
        xOff,
        yOff2,
        &pLine,
        &hitX,
        &hitY,
        GetScale() == Scale::Fg ? CollisionMask(eWallLeft_1, eWallRight_2, eFlyingObjectWall_17) : CollisionMask(eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundFlyingObjectWall_18));

    FP sparkX = {};
    if (bCollision)
    {
        FP k25Directed = {};
        if (GetAnimation().GetFlipX())
        {
            k25Directed = FP_FromInteger(-25);
        }
        else
        {
            k25Directed = FP_FromInteger(25);
        }
        sparkX = (k25Directed * GetSpriteScale()) + mXPos;
        Slig_SoundEffect(sGnFrame & 1 ? SligSfx::eCollideWithWall1_12 : SligSfx::eCollideWithWall2_13, this);
    }
    else
    {
        bCollision = gCollisions->Raycast(
            mXPos,
            (GetSpriteScale() * FP_FromInteger(10)) + mYPos,
            xOff,
            (GetSpriteScale() * FP_FromInteger(10)) + mYPos,
            &pLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? CollisionMask(eWallLeft_1, eWallRight_2, eFlyingObjectWall_17) : CollisionMask(eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundFlyingObjectWall_18));

        if (bCollision)
        {
            FP k25Directed = {};
            if (GetAnimation().GetFlipX())
            {
                k25Directed = FP_FromInteger(-25);
            }
            else
            {
                k25Directed = FP_FromInteger(25);
            }
            sparkX = (k25Directed * GetSpriteScale()) + mXPos;
        }
    }

    if (bCollision)
    {
        if (static_cast<s32>(sGnFrame) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX(sGnFrame & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame + 10;
            relive_new ParticleBurst(sparkX, hitY + (FP_FromInteger(16) * GetSpriteScale()), 5u, GetSpriteScale(), BurstType::eSmallPurpleSparks_6, 9);
        }
        mXPos += velX + hitX - xOff;
        return 1;
    }

    return 0;
}

void FlyingSlig::PullLever()
{
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    if ((FP_Abs(mXPos - field_1C8_lever_pull_range_xpos) < FP_FromInteger(1)) &&
        (FP_Abs(mYPos - field_1CC_lever_pull_range_ypos) < FP_FromInteger(1)))
    {
        SetMotionHelper(eFlyingSligMotions::Motion_7_LeverPull);
    }
    SetBrain(&FlyingSlig::Brain_15_FlyingSligSpawn);
}

s16 FlyingSlig::TryPullLever()
{
    if (BrainIs(&FlyingSlig::Brain_15_FlyingSligSpawn))
    {
        return 0;
    }

    FP kGridSizeDirected = {};
    if (GetAnimation().GetFlipX())
    {
        kGridSizeDirected = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        kGridSizeDirected = ScaleToGridSize(GetSpriteScale());
    }

    const FP k15Scaled = FP_FromInteger(15) * GetSpriteScale();
    const FP k2ScaledDirected = (kGridSizeDirected * FP_FromInteger(2));

    // TODO: These can be replaced with std::min/std::max.
    FP rect_w_fp = {};
    if (mXPos + kGridSizeDirected <= mXPos + k2ScaledDirected)
    {
        rect_w_fp = mXPos + kGridSizeDirected;
    }
    else
    {
        rect_w_fp = mXPos + k2ScaledDirected;
    }

    FP rect_x_fp = {};
    if (mXPos + k2ScaledDirected <= mXPos + kGridSizeDirected)
    {
        rect_x_fp = mXPos + kGridSizeDirected;
    }
    else
    {
        rect_x_fp = (k2ScaledDirected) + mXPos;
    }

    FP rect_h_fp = {};
    if (mYPos - k15Scaled <= k15Scaled + mYPos)
    {
        rect_h_fp = mYPos - k15Scaled;
    }
    else
    {
        rect_h_fp = k15Scaled + mYPos;
    }

    FP rect_y_fp = {};
    if (k15Scaled + mYPos <= mYPos - k15Scaled)
    {
        rect_y_fp = mYPos - k15Scaled;
    }
    else
    {
        rect_y_fp = k15Scaled + mYPos;
    }

    // TODO: Can be replaced with FP_Rect and PSX_RECT.
    const s16 rect_w = FP_GetExponent(rect_w_fp);
    const s16 rect_x = FP_GetExponent(rect_x_fp);
    const s16 rect_h = FP_GetExponent(rect_h_fp);
    const s16 rect_y = FP_GetExponent(rect_y_fp);

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eLever)
        {
            auto pAliveObj = static_cast<Lever*>(pObj);

            const PSX_RECT bObjRect = pAliveObj->VGetBoundingRect();
            if (rect_w <= bObjRect.w && rect_x >= bObjRect.x && rect_y >= bObjRect.y && rect_h <= bObjRect.h)
            {
                if (GetAnimation().GetFlipX())
                {
                    if (mXPos < pAliveObj->mXPos)
                    {
                        return false;
                    }
                    field_1C8_lever_pull_range_xpos = (FP_FromInteger(45) * GetSpriteScale()) + pAliveObj->mXPos;
                }
                else
                {
                    if (mXPos > pAliveObj->mXPos)
                    {
                        return false;
                    }
                    field_1C8_lever_pull_range_xpos = pAliveObj->mXPos - (FP_FromInteger(47) * GetSpriteScale());
                }

                field_1CC_lever_pull_range_ypos = pAliveObj->mYPos - (FP_FromInteger(23) * GetSpriteScale());
                field_158_obj_id = pAliveObj->mBaseGameObjectId;
                PullLever();
                return true;
            }
        }
    }
    return false;
}

void FlyingSlig::sub_437AC0(FP arg1, FP_Point* pPoint)
{
    FP unknown = arg1;
    if (arg1 == FP_FromInteger(0))
    {
        pPoint->x = mXPos;
        pPoint->y = mYPos;
        return;
    }

    if (arg1 > FP_FromInteger(0))
    {
        if (field_194 + arg1 > field_198_line_length)
        {
            if (!mLastLine && field_1EC_pNextLine)
            {
                unknown = field_194 + arg1 - field_198_line_length;
                if (sub_437C70(field_1EC_pNextLine))
                {
                    field_194 = FP_FromInteger(0);
                }
                else
                {
                    unknown = FP_FromInteger(0);
                    field_194 = field_198_line_length;
                }
            }
            else
            {
                unknown = FP_FromInteger(0);
                field_194 = field_198_line_length;
            }
        }
    }
    else
    {
        if (field_194 + arg1 < FP_FromInteger(0))
        {
            if (!mLastLine && field_1F0_pPrevLine)
            {
                unknown = field_194 + arg1;
                if (sub_437C70(field_1F0_pPrevLine))
                {
                    field_194 = field_198_line_length;
                }
                else
                {
                    unknown = FP_FromInteger(0);
                    field_194 = FP_FromInteger(0);
                }
            }
            else
            {
                unknown = FP_FromInteger(0);
                field_194 = FP_FromInteger(0);
            }
        }
    }

    field_194 += unknown;
    pPoint->x = (field_194 * ((field_1A4_rect.w - field_1A4_rect.x) / field_198_line_length)) + field_1A4_rect.x;
    pPoint->y = (field_194 * ((field_1A4_rect.h - field_1A4_rect.y) / field_198_line_length)) + field_1A4_rect.y;
}
