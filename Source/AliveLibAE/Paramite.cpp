#include "stdafx.h"
#include "Paramite.hpp"
#include "Function.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "MusicController.hpp"
#include "Sound/Midi.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "Throwable.hpp"
#include "Map.hpp"
#include "Meat.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "PlatformBase.hpp"
#include "SwitchStates.hpp"
#include "PullRingRope.hpp"
#include "Sfx.hpp"
#include "Slurg.hpp"
#include "../relive_lib/Particle.hpp"
#include "GameSpeak.hpp"
#include "ParamiteWeb.hpp"
#include "ParamiteWebLine.hpp"
#include "ScreenShake.hpp"
#include "Grid.hpp"
#include "Path.hpp"

const AnimId sParamiteMotionAnimIds[44] = {
    AnimId::Paramite_Idle,
    AnimId::Paramite_WalkBegin,
    AnimId::Paramite_Walking,
    AnimId::Paramite_Running,
    AnimId::Paramite_Turn,
    AnimId::Paramite_Hop,
    AnimId::Paramite_Hop,// HACK fix unused not converting // AnimId::Paramite_Unused,
    AnimId::Paramite_WalkRunTransition,
    AnimId::Paramite_WalkEnd,
    AnimId::Paramite_RunBegin,
    AnimId::Paramite_RunEnd,
    AnimId::Paramite_Falling,
    AnimId::Paramite_JumpUpBegin,
    AnimId::Paramite_JumpUpMidair,
    AnimId::Paramite_JumpUpLand,
    AnimId::Paramite_RopePull,
    AnimId::Paramite_CloseAttack,
    AnimId::Paramite_Landing,
    AnimId::Paramite_Unused2,
    AnimId::Paramite_Knockback,
    AnimId::Paramite_GameSpeakBegin,
    AnimId::Paramite_PreHiss,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_AllOYaGameSpeakBegin,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_PostHiss,
    AnimId::Paramite_GameSpeakEnd,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_PostHiss,
    AnimId::Paramite_RunningAttack,
    AnimId::Paramite_SurpriseWeb,
    AnimId::Paramite_SurpriseWeb,
    AnimId::Paramite_WebLeaveDown,
    AnimId::Paramite_WebIdle,
    AnimId::Paramite_WebGoingUp,
    AnimId::Paramite_WebGoingDown,
    AnimId::Paramite_WebGrab,
    AnimId::Paramite_WebLeaveUp,
    AnimId::Paramite_Eating,
    AnimId::Paramite_Death,
    AnimId::Paramite_Squawk,
    AnimId::Paramite_Attack };

const TintEntry kParamiteTints_55D73C[24] = {
    {EReliveLevelIds::eMudomoVault, 105u, 105u, 105u},
    {EReliveLevelIds::eMudomoVault_Ender, 105u, 105u, 105u},
    {EReliveLevelIds::eNone, 105u, 105u, 105u}};


const TParamiteMotionFn sParamiteMotionTable[44] = {
#define MAKE_FN(VAR) &Paramite::VAR,
    PARAMITE_MOTIONS_ENUM_AE(MAKE_FN)
#undef MAKE_FN
};

const TParamiteBrainFn sParamiteBrainTable[10] = {
    &Paramite::Brain_0_Patrol,
    &Paramite::Brain_1_Death,
    &Paramite::Brain_2_ChasingAbe,
    &Paramite::Brain_3_SurpriseWeb,
    &Paramite::Brain_4_Unused,
    &Paramite::Brain_5_SpottedMeat,
    &Paramite::Brain_6_Possessed,
    &Paramite::Brain_7_DeathDrop,
    &Paramite::Brain_8_ControlledByGameSpeak,
    &Paramite::Brain_9_ParamiteSpawn};


void Paramite::SetBrain(TParamiteBrainFn fn)
{
    mBrainState = fn;
}

bool Paramite::BrainIs(TParamiteBrainFn fn)
{
    return mBrainState == fn;
}

void Paramite::LoadAnimations()
{
    for (auto& animId : sParamiteMotionAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Paramite::Paramite(relive::Path_Paramite* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(16)
{
    field_160_last_event_index = -1;

    SetType(ReliveTypes::eParamite);

    mBaseGameObjectTlvInfo = tlvId;

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Paramite_Idle));

    SetTint(&kParamiteTints_55D73C[0], gMap.mCurrentLevel);

    mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed);
    mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanSetOffExplosives);

    BaseAliveGameObject_PlatformId = Guid{};
    field_158_next_brain_ret = -1;
    mParamiteGamespeakNextMotion = -1;
    mWebGuid = Guid{};
    mMeatGuid = Guid{};
    mTargetGuid = Guid{};
    mPullRingRopeGuid = Guid{};
    field_130_timer = 0;
    mBrainSubState = 0;
    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    SetCurrentMotion(eParamiteMotions::Motion_0_Idle);
    field_154_input = 0;
    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
        SetScale(Scale::Bg);
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
        SetScale(Scale::Fg);
    }

    if (!VIsFacingMe(sActiveHero))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
    }

    switch (pTlv->mEntranceType)
    {
        case relive::Path_Paramite::EntranceType::eSurpriseWeb:
            mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eCanBePossessed);
            SetBrain(&Paramite::Brain_3_SurpriseWeb);
            break;

        case relive::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb:
            SetBrain(&Paramite::Brain_9_ParamiteSpawn);
            mYPos -= FP_FromInteger(20);
            GetAnimation().mFlags.Clear(AnimFlags::eRender);
            mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eCanBePossessed);
            break;

        /*case relive::Path_Paramite::EntranceType::eUnused_ScaleToLeftGridSize:
            SetBrain(&Paramite::Brain_9_ParamiteSpawn);
            mXPos -= ScaleToGridSize(mSpriteScale);
            mAnim.mFlags.Clear(AnimFlags::eFlipX);
            mAnim.mFlags.Clear(AnimFlags::eRender);
            mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eCanBePossessed);
            break;

        case relive::Path_Paramite::EntranceType::eUnused_ScaleToRightGridSize:
            SetBrain(&Paramite::Brain_9_ParamiteSpawn);
            mXPos += ScaleToGridSize(mSpriteScale);
            mAnim.mFlags.Clear(AnimFlags::eRender);
            mAnim.mFlags.Set(AnimFlags::eFlipX);
            mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eCanBePossessed);
            break;*/

        default:
            SetBrain(&Paramite::Brain_0_Patrol);
            break;
    }

    mAloneChaseDelay = pTlv->mAloneChaseDelay;
    mSurpriseWebDelayTimer = pTlv->mSurpriseWebDelayTimer;
    mMeatEatingTime = pTlv->mMeatEatingTime;
    mGroupChaseDelay = pTlv->mGroupChaseDelay;
    mSurpriseWebSwitchId = pTlv->mSurpriseWebSwitchId;

    field_178_flags.Set(Flags_178::eBit1_hiss_before_attack, pTlv->mHissBeforeAttack == relive::reliveChoice::eYes);
    field_178_flags.Clear(Flags_178::eBit2_running);
    field_178_flags.Set(Flags_178::eBit4_out_of_sight, pTlv->mDeleteWhenOutOfSight == relive::reliveChoice::eYes);
    field_178_flags.Clear(Flags_178::eBit5_prevent_depossession);
    field_178_flags.Set(Flags_178::eBit8_bAttack_fleeches, pTlv->mAttackFleeches == relive::reliveChoice::eYes);
    field_178_flags.Clear(Flags_178::eBit6_spawned);
    field_178_flags.Clear(Flags_178::eBit7_alerted);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        BaseAliveGameObjectLastLineYPos = hitY;
        mYPos = hitY;
        SetCurrentMotion(eParamiteMotions::Motion_0_Idle);
        CheckForPlatform();
    }

    BaseAliveGameObjectLastLineYPos = mYPos;
    field_140_tlvInfo = tlvId;
    MapFollowMe(TRUE);

    VStackOnObjectsOfType(ReliveTypes::eParamite);

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    field_15C_paramite_xOffset = mXOffset;

    CreateShadow();
}

s32 Paramite::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Paramite_State*>(pBuffer);
    auto pTlv = static_cast<relive::Path_Paramite*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_3C_tlvInfo));

    auto pParamite = relive_new Paramite(pTlv, pState->field_3C_tlvInfo);

    if (pState->field_76_flags.Get(Paramite_State::eBit1_unused))
    {
        sControlledCharacter = pParamite;
    }

    pParamite->BaseAliveGameObjectPathTLV = nullptr;
    pParamite->BaseAliveGameObjectCollisionLine = nullptr;

    pParamite->mXPos = pState->field_4_xpos;
    pParamite->mYPos = pState->field_8_ypos;

    pParamite->mVelX = pState->field_C_velx;
    pParamite->mVelY = pState->field_10_vely;

    pParamite->field_13C_velx_offset = pState->field_64_velx_offset;
    pParamite->mCurrentPath = pState->field_14_path_number;
    pParamite->mCurrentLevel = MapWrapper::FromAESaveData(pState->field_16_lvl_number);
    pParamite->SetSpriteScale(pState->field_18_sprite_scale);

    pParamite->mRGB.SetRGB(pState->field_1C_r, pState->field_1E_g, pState->field_20_b);

    pParamite->SetCurrentMotion(pState->field_24_current_motion);
    pParamite->GetAnimation().Set_Animation_Data(pParamite->GetAnimRes(sParamiteMotionAnimIds[pParamite->mCurrentMotion]));

    pParamite->GetAnimation().SetCurrentFrame(pState->field_26_anim_current_frame);
    pParamite->GetAnimation().SetFrameChangeCounter(pState->field_28_frame_change_counter);

    pParamite->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2B_drawable & 1);

    pParamite->GetAnimation().mFlags.Set(AnimFlags::eFlipX, pState->field_22_flip_x & 1);
    pParamite->GetAnimation().mFlags.Set(AnimFlags::eRender, pState->field_2A_render & 1);

    if (IsLastFrame(&pParamite->GetAnimation()))
    {
        pParamite->GetAnimation().mFlags.Set(AnimFlags::eIsLastFrame);
    }

    pParamite->mHealth = pState->field_2C_health;
    pParamite->SetCurrentMotion(pState->field_30_current_motion);
    pParamite->SetNextMotion(pState->field_32_next_motion);
    pParamite->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_34_last_line_ypos);
    pParamite->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eRestoredFromQuickSave);
    pParamite->BaseAliveGameObjectCollisionLineType = pState->field_36_line_type;

    pParamite->mMeatGuid = pState->field_40_meat_id;
    pParamite->mWebGuid = pState->field_44_web_id;
    pParamite->mTargetGuid = pState->field_48_obj_id;
    pParamite->mPullRingRopeGuid = pState->field_4C_pull_ring_rope_id;

    pParamite->SetBrain(sParamiteBrainTable[pState->field_50_brain_idx]);

    pParamite->mBrainSubState = pState->field_58_brain_ret;
    pParamite->field_130_timer = pState->field_5C_timer;
    pParamite->field_138_depossession_timer = pState->field_60_depossession_timer;
    pParamite->field_13C_velx_offset = pState->field_64_velx_offset;

    pParamite->field_140_tlvInfo = pState->field_3C_tlvInfo;
    pParamite->field_148_timer = pState->field_68_timer;

    pParamite->mAbeLevel = MapWrapper::FromAESaveData(pState->field_6C_return_level);
    pParamite->mAbePath = pState->field_6E_return_path;
    pParamite->mAbeCamera = pState->field_70_return_camera;

    pParamite->field_154_input = InputObject::PsxButtonsToKeyboardInput_45EE40(pState->field_72_input);
    pParamite->field_158_next_brain_ret = pState->field_74_next_brain_ret;

    pParamite->field_178_flags.Set(Flags_178::eBit2_running, pState->field_76_flags.Get(Paramite_State::eBit2_running));
    pParamite->field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen, pState->field_76_flags.Get(Paramite_State::eBit3_hissed_or_left_screen));
    pParamite->field_178_flags.Set(Flags_178::eBit5_prevent_depossession, pState->field_76_flags.Get(Paramite_State::eBit4_prevent_depossession));
    pParamite->field_178_flags.Set(Flags_178::eBit6_spawned, pState->field_76_flags.Get(Paramite_State::eBit5_spawned));
    pParamite->field_178_flags.Set(Flags_178::eBit7_alerted, pState->field_76_flags.Get(Paramite_State::eBit6_alerted));
    pParamite->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed, pState->field_76_flags.Get(Paramite_State::eBit7_can_be_possessed));

    return sizeof(Paramite_State);
}

static Guid ResolveId(Guid objId)
{
    if (objId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(objId);
        if (pObj)
        {
            return pObj->mBaseGameObjectTlvInfo;
        }
    }
    return Guid{};
}

s32 Paramite::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Paramite_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eParamite_96;
    pState->field_4_xpos = mXPos;
    pState->field_8_ypos = mYPos;
    pState->field_C_velx = mVelX;
    pState->field_10_vely = mVelY;

    pState->field_64_velx_offset = field_13C_velx_offset;

    pState->field_14_path_number = mCurrentPath;
    pState->field_16_lvl_number = MapWrapper::ToAE(mCurrentLevel);
    pState->field_18_sprite_scale = GetSpriteScale();

    pState->field_1C_r = mRGB.r;
    pState->field_1E_g = mRGB.g;
    pState->field_20_b = mRGB.b;

    pState->field_22_flip_x = GetAnimation().mFlags.Get(AnimFlags::eFlipX);
    pState->field_24_current_motion = mCurrentMotion;
    pState->field_26_anim_current_frame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    pState->field_28_frame_change_counter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    pState->field_2B_drawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2A_render = GetAnimation().mFlags.Get(AnimFlags::eRender);
    pState->field_2C_health = mHealth;
    pState->field_30_current_motion = mCurrentMotion;
    pState->field_32_next_motion = mNextMotion;

    pState->field_34_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_36_line_type = BaseAliveGameObjectCollisionLine->mLineType;
    }
    else
    {
        pState->field_36_line_type = -1;
    }

    pState->field_76_flags.Clear();
    pState->field_76_flags.Set(Paramite_State::eBit1_unused, this == sControlledCharacter);

    pState->field_3C_tlvInfo = field_140_tlvInfo;
    pState->field_40_meat_id = ResolveId(mMeatGuid);
    pState->field_44_web_id = ResolveId(mWebGuid);
    pState->field_48_obj_id = ResolveId(mTargetGuid);
    pState->field_4C_pull_ring_rope_id = ResolveId(mPullRingRopeGuid);

    pState->field_50_brain_idx = 0;

    s32 idx = 0;
    for (auto& fn : sParamiteBrainTable)
    {
        if (BrainIs(fn))
        {
            pState->field_50_brain_idx = idx;
            break;
        }
        idx++;
    }

    pState->field_58_brain_ret = mBrainSubState;
    pState->field_5C_timer = field_130_timer;

    pState->field_60_depossession_timer = field_138_depossession_timer;
    pState->field_64_velx_offset = field_13C_velx_offset;

    pState->field_3C_tlvInfo = field_140_tlvInfo;
    pState->field_68_timer = field_148_timer;

    pState->field_6C_return_level = MapWrapper::ToAE(mAbeLevel);
    pState->field_6E_return_path = mAbePath;
    pState->field_70_return_camera = mAbeCamera;

    pState->field_72_input = InputObject::KeyboardInputToPsxButtons_45EF70(field_154_input);
    pState->field_74_next_brain_ret = field_158_next_brain_ret;

    pState->field_76_flags.Set(Paramite_State::eBit2_running, field_178_flags.Get(Flags_178::eBit2_running));
    pState->field_76_flags.Set(Paramite_State::eBit3_hissed_or_left_screen, field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen));
    pState->field_76_flags.Set(Paramite_State::eBit4_prevent_depossession, field_178_flags.Get(Flags_178::eBit5_prevent_depossession));
    pState->field_76_flags.Set(Paramite_State::eBit5_spawned, field_178_flags.Get(Flags_178::eBit6_spawned));
    pState->field_76_flags.Set(Paramite_State::eBit6_alerted, field_178_flags.Get(Flags_178::eBit7_alerted));
    pState->field_76_flags.Set(Paramite_State::eBit7_can_be_possessed, mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eCanBePossessed));

    return sizeof(Paramite_State);
}

const eParamiteMotions sParamitePatrolMotionTable[6] = {
    eParamiteMotions::Motion_22_Hiss1,
    eParamiteMotions::Motion_4_Turn,
    eParamiteMotions::Motion_22_Hiss1,
    eParamiteMotions::Motion_22_Hiss1,
    eParamiteMotions::Motion_4_Turn,
    eParamiteMotions::Motion_0_Idle};

namespace ParamiteEnums {

enum Brain_0_Patrol
{
    eBrain0_Inactive_0 = 0,
    eBrain0_IdleForAbe_1 = 1,
    eBrain0_FearingAbe_2 = 2,
    eBrain0_RunningFromAbe_3 = 3,
    eBrain0_ApproachingAbe_4 = 4,
    eBrain0_StopApproachingAbe_5 = 5,
    eBrain0_TurningForAbe_6 = 6,
    eBrain0_HittingAbe_7 = 7,
    eBrain0_StuckToWall_8 = 8,
    eBrain0_Attacking_9 = 9,
    eBrain0_LostInvisibleAbeSight_10 = 10,
    eBrain0_TurningAfterLoosingSight_11 = 11,
    eBrain0_Idle_12 = 12,
    eBrain0_IdleAnimation_13 = 13,
    eBrain0_Turning_14 = 14,
    eBrain0_LookingForInvisibleAbe_15 = 15,
    eBrain0_Panic_16 = 16
};

enum Brain_2_ChasingAbe
{
    eBrain2_Inactive_0 = 0,
    eBrain2_Attacking_1 = 1,
    eBrain2_ToWarning_2 = 2,
    eBrain2_Warning_3 = 3,
    eBrain2_CloseAttack_4 = 4,
    eBrain2_ToChasing_5 = 5,
    eBrain2_QuickAttack_6 = 6,
    eBrain2_Chasing_7 = 7,
    eBrain2_Jumping_8 = 8,
    eBrain2_TurningWhileChasing_9 = 9,
    eBrain2_Turning_10 = 10,
    eBrain2_Walking_11 = 11,
    eBrain2_WalkingToHop_12 = 12,
    eBrain2_Eating_13 = 13,
    eBrain2_Idle_14 = 14,
    eBrain2_Panic_15 = 15
};

enum Brain_3_SurpriseWeb
{
    eBrain3_Loading_0 = 0,
    eBrain3_Appearing_1 = 1,
    eBrain3_StartAnimation_2 = 2,
    eBrain3_StateLoop1_3 = 3,
    eBrain3_StateLoop2_4 = 4
};

enum Brain_5_SpottedMeat
{
    eBrain5_Idle_0 = 0,
    eBrain5_Running_1 = 1,
    eBrain5_Walking_2 = 2,
    eBrain5_Jumping_3 = 3,
    eBrain5_Turning_4 = 4,
    eBrain5_AttentiveToMeat_5 = 5,
    eBrain5_Eating_6 = 6
};

enum Brain_8_ControlledByGameSpeak
{
    eBrain8_Inactive_0 = 0,
    eBrain8_FollowingIdle_1 = 1,
    eBrain8_Falling_2 = 2,
    eBrain8_Jumping_3 = 3,
    eBrain8_FollowingIdleToWalking_4 = 4,
    eBrain8_FollowingWalking_5 = 5,
    eBrain8_FollowingRunning_6 = 6,
    eBrain8_IdleToWorking_7 = 7,
    eBrain8_Working_8 = 8,
    eBrain8_Idle_9 = 9,
    eBrain8_RecievedCommand_10 = 10,
    eBrain8_Speaking_11 = 11
};

enum Brain_9_ParamiteSpawn
{
    eBrain9_Inactive_0 = 0,
    eBrain9_PreSpawn_1 = 1,
    eBrain9_SlowDescend_2 = 2,
    eBrain9_SlowerDescend_3 = 3,
    eBrain9_DescendLoop1_4 = 4,
    eBrain9_DescendLoop2_5 = 5
};

} // namespace ParamiteEnums

s16 Paramite::Brain_0_Patrol()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!pObj)
    {
        mTargetGuid = Guid{};
    }

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0:
            if (gNumCamSwappers > 0)
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1:
            return Brain_Patrol_State_1_IdleForAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_FearingAbe_2:
            return Brain_Patrol_State_2_FearingAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_RunningFromAbe_3:
            return Brain_Patrol_State_3_RunningFromAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_ApproachingAbe_4:
            return Brain_Patrol_State_4_ApproachingAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_StopApproachingAbe_5:
            return Brain_Patrol_State_5_StopApproachingAbe(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_TurningForAbe_6:
            if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7:
            if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn || !(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_20_GameSpeakBegin);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_StuckToWall_8;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_StuckToWall_8:
            return Brain_Patrol_State_8_StuckToWall(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_Attacking_9:
            if (GetCurrentMotion() != eParamiteMotions::Motion_31_RunningAttack || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            if (pObj->mHealth > FP_FromInteger(0))
            {
                return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }
            SetBrain(&Paramite::Brain_2_ChasingAbe);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10:
            if (field_130_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_TurningAfterLoosingSight_11;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_TurningAfterLoosingSight_11:
            if (field_130_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12:
            return Brain_Patrol_State_12_Idle(pObj);

        case ParamiteEnums::Brain_0_Patrol::eBrain0_IdleAnimation_13:
            if (GetCurrentMotion() != eParamiteMotions::Motion_21_PreHiss || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            field_138_depossession_timer = sGnFrame + Math_RandomRange(45, 135);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_Turning_14:
            if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn || !(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            field_138_depossession_timer = sGnFrame + Math_RandomRange(45, 135);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_LookingForInvisibleAbe_15:
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;

        case ParamiteEnums::Brain_0_Patrol::eBrain0_Panic_16:
            if (!EventGet(kEventAbeOhm))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;
            }
            SetNextMotion(eParamiteMotions::Motion_22_Hiss1);
            return mBrainSubState;

        default:
            return mBrainSubState;
    }
}

s16 Paramite::Brain_Patrol_State_12_Idle(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    auto pMeat = FindMeat();
    if (pMeat)
    {
        SetBrain(&Paramite::Brain_5_SpottedMeat);
        mMeatGuid = pMeat->mBaseGameObjectId;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }

    if (pObj || FindTarget())
    {
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (EventGet(kEventAbeOhm))
    {
        SetNextMotion(eParamiteMotions::Motion_22_Hiss1);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Panic_16;
    }

    const auto pEventNoise = IsEventInRange(kEventNoise, mXPos, mYPos, AsEventScale(GetScale()));
    const auto pEventSpeaking = IsEventInRange(kEventSpeaking, mXPos, mYPos, AsEventScale(GetScale()));

    if (IsActiveHero(pEventNoise) || IsActiveHero(pEventSpeaking))
    {
        if (sActiveHero->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
        {
            SetNextMotion(sParamitePatrolMotionTable[Math_RandomRange(0, 4)]);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_LookingForInvisibleAbe_15;
        }
    }

    FP gridBlockAHead = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        gridBlockAHead = mXPos - kGridSize;
    }
    else
    {
        gridBlockAHead = kGridSize + mXPos;
    }

    auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eFleech, gridBlockAHead, mYPos));
    if (pFleech && VIsFacingMe(pFleech))
    {
        SetNextMotion(eParamiteMotions::Motion_43_Attack);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LookingForInvisibleAbe_15;
    }

    const GameSpeakEvents lastSpeak = LastSpeak();
    if (lastSpeak == GameSpeakEvents::Paramite_Howdy_48)
    {
        if (sControlledCharacter->Type() == ReliveTypes::eParamite && sControlledCharacter->mHealth > FP_FromInteger(0) && sControlledCharacter->GetSpriteScale() == GetSpriteScale())
        {
            if (CanIAcceptAGameSpeakCommand())
            {
                field_178_flags.Set(Flags_178::eBit7_alerted);
                SetBrain(&Paramite::Brain_8_ControlledByGameSpeak);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Inactive_0;
            }
        }
    }
    else if (lastSpeak == GameSpeakEvents::Paramite_AllYa_52)
    {
        if (sControlledCharacter->Type() == ReliveTypes::eParamite && sControlledCharacter->mHealth > FP_FromInteger(0) && sControlledCharacter->GetSpriteScale() == GetSpriteScale())
        {
            SetBrain(&Paramite::Brain_8_ControlledByGameSpeak);
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Inactive_0;
        }
    }

    if (VOnSameYLevel(sControlledCharacter))
    {
        if (sControlledCharacter->Type() == ReliveTypes::eParamite)
        {
            const auto controlledParamiteMotion = static_cast<eParamiteMotions>(sControlledCharacter->mCurrentMotion);
            if (controlledParamiteMotion == eParamiteMotions::Motion_43_Attack ||
                controlledParamiteMotion == eParamiteMotions::Motion_31_RunningAttack)
            {
                auto sControlledParamite = static_cast<Paramite*>(sControlledCharacter);
                if (sControlledParamite->FindTarget())
                {
                    mTargetGuid = sControlledParamite->mTargetGuid;
                    Sound(ParamiteSpeak::Howdy_5, 0);
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    SetBrain(&Paramite::Brain_2_ChasingAbe);
                    field_148_timer = sGnFrame + mGroupChaseDelay;
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
                }
            }
        }
    }

    if (field_138_depossession_timer > static_cast<s32>(sGnFrame))
    {
        if (!field_178_flags.Get(Flags_178::eBit6_spawned) || gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
        {
            return mBrainSubState;
        }
        else
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return mBrainSubState;
        }
    }

    if (Math_NextRandom() >= 6u)
    {
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Turning_14;
    }
    else
    {
        SetNextMotion(eParamiteMotions::Motion_20_GameSpeakBegin);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleAnimation_13;
    }
}

s16 Paramite::Brain_Patrol_State_5_StopApproachingAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    if (!pObj || !VOnSameYLevel(pObj) || GetSpriteScale() != pObj->GetSpriteScale())
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
    {
        mTargetGuid = Guid{};
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        field_130_timer = sGnFrame + Math_RandomRange(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (AnotherParamiteNear())
    {
        Sound(ParamiteSpeak::Howdy_5, 0);
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        SetBrain(&Paramite::Brain_2_ChasingAbe);
        field_148_timer = sGnFrame + mGroupChaseDelay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (mVelX < FP_FromInteger(0))
    {
        if (HandleEnemyStopper(-2))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
        if (WallHit(GetSpriteScale() * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine(1, 3))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
    }

    if (mVelX > FP_FromInteger(0))
    {
        if (HandleEnemyStopper(2))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine(0, 3))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
    }

    if (!VIsObj_GettingNear_On_X(pObj))
    {
        return mBrainSubState;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(8), pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (!VIsObjNearby(kGridSize * FP_FromInteger(10), pObj))
    {
        return mBrainSubState;
    }
    SetNextMotion(eParamiteMotions::Motion_2_Walking);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_ApproachingAbe_4;
}

s16 Paramite::Brain_Patrol_State_4_ApproachingAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    if (!pObj || !VOnSameYLevel(pObj) || GetSpriteScale() != pObj->GetSpriteScale())
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
    {
        mTargetGuid = Guid{};
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        field_130_timer = sGnFrame + Math_RandomRange(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (AnotherParamiteNear())
    {
        Sound(ParamiteSpeak::Howdy_5, 0);
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        SetBrain(&Paramite::Brain_2_ChasingAbe);
        field_148_timer = sGnFrame + mGroupChaseDelay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (mVelX < FP_FromInteger(0))
    {
        if (HandleEnemyStopper(-2))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine(1, 2))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
    }

    if (mVelX > FP_FromInteger(0))
    {
        if (HandleEnemyStopper(2))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine(0, 2))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
        }
    }

    if (VIsObj_GettingNear_On_X(pObj) || (VIsObjNearby(kGridSize * FP_FromInteger(10), pObj)))
    {
        if (!VIsObj_GettingNear_On_X(pObj))
        {
            return mBrainSubState;
        }
        if (!VIsObjNearby(kGridSize * FP_FromInteger(8), pObj))
        {
            return mBrainSubState;
        }
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    SetNextMotion(eParamiteMotions::Motion_3_Running);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_StopApproachingAbe_5;
}

s16 Paramite::Brain_Patrol_State_3_RunningFromAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    if (!pObj || !VOnSameYLevel(pObj) || GetSpriteScale() != pObj->GetSpriteScale())
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
    {
        mTargetGuid = Guid{};
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        field_130_timer = sGnFrame + Math_RandomRange(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (mVelX < FP_FromInteger(0))
    {
        if ((WallHit(GetSpriteScale() * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine(1, 3)))
        {
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
    }

    if (mVelX > FP_FromInteger(0))
    {
        if ((WallHit(GetSpriteScale() * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine(0, 3)))
        {
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
    }

    if (AnotherParamiteNear())
    {
        Sound(ParamiteSpeak::Howdy_5, 0);
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        SetBrain(&Paramite::Brain_2_ChasingAbe);
        field_148_timer = sGnFrame + mGroupChaseDelay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (VIsObj_GettingNear_On_X(pObj))
    {
        return mBrainSubState;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(7), pObj))
    {
        return mBrainSubState;
    }

    SetNextMotion(eParamiteMotions::Motion_2_Walking);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_FearingAbe_2;
}

s16 Paramite::Brain_Patrol_State_8_StuckToWall(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    auto pMeat = FindMeat();
    if (pMeat)
    {
        SetBrain(&Paramite::Brain_5_SpottedMeat);
        mMeatGuid = pMeat->mBaseGameObjectId;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }

    if (!pObj || !VOnSameYLevel(pObj) || GetSpriteScale() != pObj->GetSpriteScale())
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
    {
        mTargetGuid = Guid{};
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        field_130_timer = sGnFrame + Math_RandomRange(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(3), pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_31_RunningAttack);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Attacking_9;
    }

    if (VIsObj_GettingNear_On_X(pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_22_Hiss1);
    }
    else
    {
        if (!VIsObjNearby(kGridSize * FP_FromInteger(8), pObj))
        {
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_ApproachingAbe_4;
        }
    }

    if (GetCurrentMotion() != eParamiteMotions::Motion_21_PreHiss)
    {
        return mBrainSubState;
    }

    if (Math_NextRandom() >= 6u)
    {
        return mBrainSubState;
    }

    SetNextMotion(eParamiteMotions::Motion_22_Hiss1);
    return mBrainSubState;
}

s16 Paramite::Brain_Patrol_State_1_IdleForAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    auto pFoundMeat = FindMeat();
    if (pFoundMeat)
    {
        SetBrain(&Paramite::Brain_5_SpottedMeat);
        mMeatGuid = pFoundMeat->mBaseGameObjectId;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }

    if (!pObj || !VOnSameYLevel(pObj) || GetSpriteScale() != pObj->GetSpriteScale())
    {
        if (!FindTarget())
        {
            field_138_depossession_timer = sGnFrame + Math_RandomRange(45, 135);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Idle_12;
        }
        pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    }

    if (pObj && pObj->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
    {
        mTargetGuid = Guid{};
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        field_130_timer = sGnFrame + Math_RandomRange(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (EventGet(kEventAbeOhm) && VIsFacingMe(pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_22_Hiss1);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Panic_16;
    }

    if (HandleEnemyStopper(0))
    {
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if ((AnotherParamiteNear() || pObj->mHealth <= FP_FromInteger(0)) && GetSpriteScale() == pObj->GetSpriteScale())
    {
        Sound(ParamiteSpeak::Howdy_5, 0);
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        SetBrain(&Paramite::Brain_2_ChasingAbe);
        field_148_timer = sGnFrame + mGroupChaseDelay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    // mid range - warn abe if cornered or wait for new movement
    if (VIsObjNearby(kGridSize * FP_FromInteger(6), pObj))
    {
        if (VIsFacingMe(pObj))
        {
            if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine(1, 2))
                {
                    SetNextMotion(eParamiteMotions::Motion_20_GameSpeakBegin);
                    return ParamiteEnums::Brain_0_Patrol::eBrain0_StuckToWall_8;
                }
            }
            else if (Check_IsOnEndOfLine(0, 2))
            {
                SetNextMotion(eParamiteMotions::Motion_20_GameSpeakBegin);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_StuckToWall_8;
            }
            Sound(ParamiteSpeak::Howdy_5, 0);
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_TurningForAbe_6;
        }

        // close range - run away or attack if cornered
        if (VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
        {
            if ((!GetAnimation().mFlags.Get(AnimFlags::eFlipX) || !Check_IsOnEndOfLine(1, 2)) && (GetAnimation().mFlags.Get(AnimFlags::eFlipX) || !Check_IsOnEndOfLine(0, 2)))
            {
                SetNextMotion(eParamiteMotions::Motion_3_Running);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_RunningFromAbe_3;
            }
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
        else
        {
            if ((GetAnimation().mFlags.Get(AnimFlags::eFlipX) && Check_IsOnEndOfLine(1, 2)) || (!GetAnimation().mFlags.Get(AnimFlags::eFlipX) && Check_IsOnEndOfLine(0, 2)))
            {
                SetNextMotion(eParamiteMotions::Motion_4_Turn);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
            }
            else
            {
                SetNextMotion(eParamiteMotions::Motion_2_Walking);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_FearingAbe_2;
            }
        }
    }

    // far range - follow abe
    if (!VIsObjNearby(kGridSize * FP_FromInteger(8), pObj))
    {
        if (!VIsFacingMe(pObj))
        {
            if ((GetAnimation().mFlags.Get(AnimFlags::eFlipX) && !Check_IsOnEndOfLine(0, 2)) || (!GetAnimation().mFlags.Get(AnimFlags::eFlipX) && !Check_IsOnEndOfLine(1, 2)))
            {
                SetNextMotion(eParamiteMotions::Motion_4_Turn);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_TurningForAbe_6;
            }
        }
        else if ((GetAnimation().mFlags.Get(AnimFlags::eFlipX) && !Check_IsOnEndOfLine(1, 2)) || (!GetAnimation().mFlags.Get(AnimFlags::eFlipX) && !Check_IsOnEndOfLine(0, 2)))
        {
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_ApproachingAbe_4;
        }
    }

    if (VIsFacingMe(pObj))
    {
        return mBrainSubState;
    }
    SetNextMotion(eParamiteMotions::Motion_4_Turn);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_TurningForAbe_6;
}

s16 Paramite::Brain_Patrol_State_2_FearingAbe(BaseAliveGameObject* pObj)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    if (!pObj || !VOnSameYLevel(pObj) || GetSpriteScale() != pObj->GetSpriteScale())
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
    }

    if (pObj->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible))
    {
        mTargetGuid = Guid{};
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        field_130_timer = sGnFrame + Math_RandomRange(45, 60);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_LostInvisibleAbeSight_10;
    }

    if (mVelX < FP_FromInteger(0))
    {
        if ((WallHit(GetSpriteScale() * FP_FromInteger(20), -kGridSize) || Check_IsOnEndOfLine(1, 2)))
        {
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
    }

    if (mVelX > FP_FromInteger(0))
    {
        if ((WallHit(GetSpriteScale() * FP_FromInteger(20), kGridSize) || Check_IsOnEndOfLine(0, 2)))
        {
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_HittingAbe_7;
        }
    }

    if (AnotherParamiteNear())
    {
        Sound(ParamiteSpeak::Howdy_5, 0);
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        SetBrain(&Paramite::Brain_2_ChasingAbe);
        field_148_timer = sGnFrame + mGroupChaseDelay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (VIsObj_GettingNear_On_X(pObj) && VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_3_Running);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_RunningFromAbe_3;
    }

    if (VIsObj_GettingNear_On_X(pObj))
    {
        return mBrainSubState;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(7), pObj))
    {
        return mBrainSubState;
    }

    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_IdleForAbe_1;
}

s16 Paramite::Brain_1_Death()
{
    if (!BaseAliveGameObjectCollisionLine || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eTrackLine_8)
    {
        PathLine* ppPathLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        if (!InAirCollision(&ppPathLine, &hitX, &hitY, FP_FromDouble(1.8)))
        {
            return 100;
        }

        switch (ppPathLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
                BaseAliveGameObjectCollisionLine = ppPathLine;
                mXPos = hitX;
                mYPos = hitY;
                CheckForPlatform();
                break;
            default:
                break;
        }
    }

    if (field_130_timer <= static_cast<s32>(sGnFrame) || field_130_timer >= static_cast<s32>(sGnFrame + 80))
    {
        if (!GetAnimation().mFlags.Get(AnimFlags::eRender))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.01));
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;

        if (GetSpriteScale() >= FP_FromDouble(0.3))
        {
            DeathSmokeEffect(true);
        }
    }

    if (sControlledCharacter == this)
    {
        if (field_130_timer < static_cast<s32>(sGnFrame))
        {
            sControlledCharacter = sActiveHero;
            gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
    }

    if (sControlledCharacter != this)
    {
        if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    if (GetSpriteScale() <= FP_FromInteger(0) || field_130_timer < static_cast<s32>(sGnFrame))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}

s16 Paramite::Brain_2_ChasingAbe()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (pObj && !pObj->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible) && (pObj->Type() != ReliveTypes::eFleech || pObj->mHealth > FP_FromInteger(0)))
    {
        if (field_148_timer > static_cast<s32>(sGnFrame) || (VOnSameYLevel(pObj) && GetSpriteScale() == pObj->GetSpriteScale()))
        {
            if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
            {
                MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
            }

            if (EventGet(kEventAbeOhm))
            {
                SetNextMotion(eParamiteMotions::Motion_22_Hiss1);
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Panic_15;
            }

            switch (mBrainSubState)
            {
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0:
                    return Brain_ChasingAbe_State_0_Inactive(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Attacking_1:
                    return Brain_ChasingAbe_State_1_Attacking(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToWarning_2:
                    return Brain_ChasingAbe_State_2_ToWarning();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Warning_3:
                    return Brain_ChasingAbe_State_3_Warning();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_CloseAttack_4:
                    return Brain_ChasingAbe_State_4_CloseAttack();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5:
                    return Brain_ChasingAbe_State_5_ToChasing(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_QuickAttack_6:
                    return Brain_ChasingAbe_State_6_QuickAttack();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Chasing_7:
                    return Brain_ChasingAbe_State_7_Chasing(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8:
                    return Brain_ChasingAbe_State_8_Jumping();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_TurningWhileChasing_9:
                    return Brain_ChasingAbe_State_9_TurningWhileChasing();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10:
                    return Brain_ChasingAbe_State_10_Turning(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11:
                    return Brain_ChasingAbe_State_11_Walking(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_WalkingToHop_12:
                    return Brain_ChasingAbe_State_12_WalkingToHop(pObj);
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13:
                    return Brain_ChasingAbe_State_13_Eating();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Idle_14:
                    return Brain_ChasingAbe_State_14_Idle();
                case ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Panic_15:
                    return Brain_ChasingAbe_State_15_Panic();
                default:
                    return mBrainSubState;
            }
        }
    }
    else
    {
        mTargetGuid = Guid{};
    }

    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    SetBrain(&Paramite::Brain_0_Patrol);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
}

s16 Paramite::Brain_ChasingAbe_State_15_Panic()
{
    if (EventGet(kEventAbeOhm))
    {
        return mBrainSubState;
    }
    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
}

s16 Paramite::Brain_ChasingAbe_State_14_Idle()
{
    if (field_130_timer > static_cast<s32>(sGnFrame))
    {
        return mBrainSubState;
    }
    SetNextMotion(eParamiteMotions::Motion_40_Eating);
    field_130_timer = sGnFrame + 28;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
}

s16 Paramite::Brain_ChasingAbe_State_9_TurningWhileChasing()
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn || !(GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame)))
    {
        return mBrainSubState;
    }
    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
}

s16 Paramite::Brain_ChasingAbe_State_13_Eating()
{
    if (field_130_timer > static_cast<s32>(sGnFrame))
    {
        return mBrainSubState;
    }
    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    field_130_timer = sGnFrame + 15;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Idle_14;
}

s16 Paramite::Brain_ChasingAbe_State_8_Jumping()
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
}

s16 Paramite::Brain_ChasingAbe_State_6_QuickAttack()
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        return mBrainSubState;
    }

    if (field_178_flags.Get(Flags_178::eBit1_hiss_before_attack))
    {
        SetNextMotion(eParamiteMotions::Motion_22_Hiss1);
    }
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
}

s16 Paramite::Brain_ChasingAbe_State_3_Warning()
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_22_Hiss1)
    {
        return mBrainSubState;
    }
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
}

s16 Paramite::Brain_ChasingAbe_State_4_CloseAttack()
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        return mBrainSubState;
    }

    if (field_178_flags.Get(Flags_178::eBit1_hiss_before_attack))
    {
        field_130_timer = sGnFrame + Math_RandomRange(0, 6);
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToWarning_2;
    }
    else
    {
        field_130_timer = sGnFrame + mAloneChaseDelay;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
    }
}

s16 Paramite::Brain_ChasingAbe_State_2_ToWarning()
{
    if (field_130_timer > static_cast<s32>(sGnFrame))
    {
        return mBrainSubState;
    }
    SetNextMotion(eParamiteMotions::Motion_22_Hiss1);
    field_130_timer = sGnFrame + mAloneChaseDelay;
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Warning_3;
}

s16 Paramite::Brain_ChasingAbe_State_11_Walking(BaseAliveGameObject* pObj)
{
    if (!VIsFacingMe(pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10;
    }

    if (!VOnSameYLevel(pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    if (VIsObjNearby(kGridSize, pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_40_Eating);
        field_130_timer = sGnFrame + 28;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
    }

    const FP xSnapped = FP_FromInteger(SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos)));

    if (mVelX > FP_FromInteger(0))
    {
        if (FP_Abs(mXPos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine(0, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_WalkingToHop_12;
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        if (FP_Abs(mXPos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine(1, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_WalkingToHop_12;
        }
    }
    return mBrainSubState;
}

s16 Paramite::Brain_ChasingAbe_State_7_Chasing(BaseAliveGameObject* pObj)
{
    if (!VOnSameYLevel(pObj) || WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }
    else
    {
        if (!VIsFacingMe(pObj))
        {
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_TurningWhileChasing_9;
        }

        const FP xSnapped = FP_FromInteger(SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos)));
        if (mVelX < FP_FromInteger(0))
        {
            if (HandleEnemyStopper(-2))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
            }

            if (FP_Abs(mXPos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine(1, 1))
            {
                ToHop();
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
            }
            else
            {
                // Drop through to idle/attack case
            }
        }
        else if (mVelX > FP_FromInteger(0))
        {
            if (HandleEnemyStopper(2))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
            }

            if (FP_Abs(mXPos - xSnapped) < FP_FromInteger(6) && Check_IsOnEndOfLine(0, 1))
            {
                ToHop();
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
            }
            else
            {
                // Drop through to idle/attack case
            }
        }

        const FP kGridSize = ScaleToGridSize(GetSpriteScale());

        if (!VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
        {
            return mBrainSubState;
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
        }
        else
        {
            SetNextMotion(eParamiteMotions::Motion_31_RunningAttack);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Attacking_1;
        }
    }
}

s16 Paramite::Brain_ChasingAbe_State_12_WalkingToHop(BaseAliveGameObject* pObj)
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }

    if (VIsFacingMe(pObj))
    {
        const FP kGridSize = ScaleToGridSize(GetSpriteScale());

        if (VIsObjNearby(kGridSize, pObj))
        {
            SetNextMotion(eParamiteMotions::Motion_40_Eating);
            field_130_timer = sGnFrame + 28;
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
        }
        else
        {
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
    }
    else
    {
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10;
    }
}

s16 Paramite::Brain_ChasingAbe_State_10_Turning(BaseAliveGameObject* pObj)
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        return mBrainSubState;
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    if (VIsObjNearby(kGridSize, pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_40_Eating);
        field_130_timer = sGnFrame + 28;
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
    }

    if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (!Check_IsOnEndOfLine(0, 1))
        {
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
    }
    else if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (!Check_IsOnEndOfLine(1, 1))
        {
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
    }

    SetNextMotion(eParamiteMotions::Motion_5_Hop);
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
}

s16 Paramite::Brain_ChasingAbe_State_5_ToChasing(BaseAliveGameObject* pObj)
{
    if (!VOnSameYLevel(pObj))
    {
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (!VIsFacingMe(pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_QuickAttack_6;
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (HandleEnemyStopper(-2))
        {
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
        }
    }
    else if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (HandleEnemyStopper(2))
        {
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
        }
    }

    if (VIsObjNearby(kGridSize, pObj))
    {
        SetNextMotion(eParamiteMotions::Motion_16_CloseAttack);
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
        }
        else
        {
            SetNextMotion(eParamiteMotions::Motion_31_RunningAttack);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Attacking_1;
        }
    }

    if (field_130_timer > static_cast<s32>(sGnFrame) && field_178_flags.Get(Flags_178::eBit1_hiss_before_attack))
    {
        return mBrainSubState;
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (Check_IsOnEndOfLine(1, 1))
        {
            SetNextMotion(eParamiteMotions::Motion_5_Hop);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
        }
    }
    else if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (Check_IsOnEndOfLine(0, 1))
        {
            SetNextMotion(eParamiteMotions::Motion_5_Hop);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
        }
    }

    SetNextMotion(eParamiteMotions::Motion_3_Running);
    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Chasing_7;
}

s16 Paramite::Brain_ChasingAbe_State_1_Attacking(BaseAliveGameObject* pObj)
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_31_RunningAttack)
    {
        if (GetNextMotion() != eParamiteMotions::Motion_31_RunningAttack)
        {
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
        }

        if (GetCurrentMotion() != eParamiteMotions::Motion_31_RunningAttack)
        {
            return mBrainSubState;
        }
    }

    if (!GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        return mBrainSubState;
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    if (pObj->mHealth > FP_FromInteger(0))
    {
        if (VIsFacingMe(pObj))
        {
            if (VIsObjNearby(kGridSize * FP_FromInteger(4), pObj))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
                }
                else
                {
                    SetNextMotion(eParamiteMotions::Motion_31_RunningAttack);
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Attacking_1;
                }
            }

            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Hop);
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Hop);
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }

            SetNextMotion(eParamiteMotions::Motion_3_Running);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Chasing_7;
        }
        else
        {
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_QuickAttack_6;
        }
    }
    else
    {
        if (VIsFacingMe(pObj))
        {
            if (VIsObjNearby(kGridSize, pObj))
            {
                SetNextMotion(eParamiteMotions::Motion_40_Eating);
                field_130_timer = sGnFrame + 28;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
            }

            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Hop);
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Hop);
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
        else
        {
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10;
        }
    }
}

s16 Paramite::Brain_ChasingAbe_State_0_Inactive(BaseAliveGameObject* pObj)
{
    if (!VOnSameYLevel(pObj))
    {
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
    {
        return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
    }

    if (pObj->mHealth > FP_FromInteger(0))
    {
        if (VIsFacingMe(pObj))
        {
            if (field_178_flags.Get(Flags_178::eBit1_hiss_before_attack))
            {
                field_130_timer = sGnFrame + Math_RandomRange(0, 6);
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToWarning_2;
            }
            else
            {
                field_130_timer = sGnFrame + mAloneChaseDelay;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_ToChasing_5;
            }
        }
        else
        {
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_CloseAttack_4;
        }
    }
    else
    {
        if (VIsFacingMe(pObj))
        {
            const FP kGridSize = ScaleToGridSize(GetSpriteScale());

            if (VIsObjNearby(kGridSize, pObj))
            {
                SetNextMotion(eParamiteMotions::Motion_40_Eating);
                field_130_timer = sGnFrame + 28;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Eating_13;
            }

            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Hop);
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Hop);
                    return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Jumping_8;
                }
            }
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Walking_11;
        }
        else
        {
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Turning_10;
        }
    }
}

s16 Paramite::Brain_3_SurpriseWeb()
{
    auto pExistingWeb = static_cast<ParamiteWeb*>(sObjectIds.Find_Impl(mWebGuid));
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (mBrainSubState)
    {
        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_Loading_0:
            if (gNumCamSwappers <= 0)
            {
                SetCurrentMotion(eParamiteMotions::Motion_0_Idle);
                MapFollowMe(TRUE);
            }
            return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_Appearing_1;

        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_Appearing_1:
            if (mSurpriseWebSwitchId != 0 && SwitchStates_Get(mSurpriseWebSwitchId))
            {
                mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed);
                GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
                field_130_timer = sGnFrame + mSurpriseWebDelayTimer;
                auto pNewWeb = relive_new ParamiteWeb(mXPos, FP_GetExponent(mYPos) - 20, FP_GetExponent(mYPos) - 10, GetSpriteScale());
                if (pNewWeb)
                {
                    mWebGuid = pNewWeb->mBaseGameObjectId;
                }
                return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StartAnimation_2;
            }
            break;

        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StartAnimation_2:
            if (field_130_timer <= static_cast<s32>(sGnFrame))
            {
                mVelY = FP_FromInteger(0);
                SetCurrentMotion(eParamiteMotions::Motion_33_SurpriseWeb);
                return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop1_3;
            }
            break;

        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop1_3:
            pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(mYPos)) - 10;
            pExistingWeb->mYPos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
            if (GetCurrentMotion() == eParamiteMotions::Motion_0_Idle)
            {
                auto pWeb = static_cast<ParamiteWeb*>(sObjectIds.Find_Impl(mWebGuid));
                pWeb->field_104_bEnabled = TRUE;
                mWebGuid = Guid{};
                SetBrain(&Paramite::Brain_0_Patrol);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
            }

            if (mVelY < (GetSpriteScale() * FP_FromInteger(8)))
            {
                mVelY = (GetSpriteScale() * FP_FromDouble(0.5)) + mVelY;
                return mBrainSubState;
            }
            return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop2_4;

        case ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop2_4:
            pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(mYPos)) - 10;
            pExistingWeb->mYPos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
            if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
            {
                if (mVelY <= (GetSpriteScale() * FP_FromInteger(-1)))
                {
                    return ParamiteEnums::Brain_3_SurpriseWeb::eBrain3_StateLoop1_3;
                }
                else
                {
                    mVelY = mVelY - (GetSpriteScale() * FP_FromInteger(1));
                    return mBrainSubState;
                }
            }
            else
            {
                auto pWeb = static_cast<ParamiteWeb*>(sObjectIds.Find_Impl(mWebGuid));
                pWeb->field_104_bEnabled = TRUE;
                mWebGuid = Guid{};
                SetBrain(&Paramite::Brain_0_Patrol);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
            }
            break;

        default:
            break;
    }

    return mBrainSubState;
}

s16 Paramite::Brain_4_Unused()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    // NOTE: Some unused code removed from OG here - looked like paramite would have
    // done a game speak before going back to partol.

    if (GetCurrentMotion() == eParamiteMotions::Motion_4_Turn)
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
    }
    else
    {
        SetCurrentMotion(eParamiteMotions::Motion_0_Idle);
        SetNextMotion(-1);
    }

    SetBrain(&Paramite::Brain_0_Patrol);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
}

s16 Paramite::Brain_5_SpottedMeat()
{
    auto pMeat = static_cast<Meat*>(sObjectIds.Find_Impl(mMeatGuid));

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!pMeat || pMeat->VIsFalling())
    {
        Sound(ParamiteSpeak::DetectedMeat_7, 0);
        mMeatGuid = Guid{};
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        SetBrain(&Paramite::Brain_0_Patrol);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
    }

    if (pMeat->field_130_pLine)
    {
        if (FP_Abs(pMeat->mYPos - mYPos) > FP_FromInteger(20))
        {
            mMeatGuid = Guid{};
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            SetBrain(&Paramite::Brain_0_Patrol);
            return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
        }
    }

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0:
            return Brain_SpottedMeat_State_0_Idle(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Running_1:
            return Brain_SpottedMeat_State_1_Running(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2:
            return Brain_SpottedMeat_State_2_Walking(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3:
            return Brain_SpottedMeat_State_3_Jumping();
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4:
            return Brain_SpottedMeat_State_4_Turning(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5:
            return Brain_SpottedMeat_State_5_AttentiveToMeat(pMeat);
        case ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6:
            return Brain_SpottedMeat_State_6_Eating(pMeat);
        default:
            return mBrainSubState;
    }
}

s16 Paramite::Brain_SpottedMeat_State_6_Eating(Meat* pMeat)
{
    if (!VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), pMeat))
    {
        SetNextMotion(eParamiteMotions::Motion_2_Walking);
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
    }

    if (!VIsFacingMe(pMeat))
    {
        if (FP_Abs(pMeat->mXPos - mXPos) > FP_FromInteger(5))
        {
            Sound(ParamiteSpeak::DetectedMeat_7, 0);
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
        }
    }

    if (field_130_timer > static_cast<s32>(sGnFrame))
    {
        return mBrainSubState;
    }

    pMeat->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    mMeatGuid = Guid{};
    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    SetBrain(&Paramite::Brain_0_Patrol);
    return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
}

s16 Paramite::Brain_SpottedMeat_State_5_AttentiveToMeat(Meat* pMeat)
{
    if (!VIsFacingMe(pMeat))
    {
        Sound(ParamiteSpeak::DetectedMeat_7, 0);
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
    }

    if (VIsObj_GettingNear_On_X(pMeat))
    {
        return mBrainSubState;
    }


    if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (Check_IsOnEndOfLine(0, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }
    else if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (Check_IsOnEndOfLine(1, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    if (!VIsObjNearby(kGridSize * FP_FromInteger(3), pMeat))
    {
        SetNextMotion(eParamiteMotions::Motion_3_Running);
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Running_1;
    }

    if (!VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), pMeat))
    {
        SetNextMotion(eParamiteMotions::Motion_2_Walking);
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
    }

    if (!pMeat->field_130_pLine || !BaseAliveGameObjectCollisionLine)
    {
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
    }

    SetNextMotion(eParamiteMotions::Motion_40_Eating);
    field_130_timer = sGnFrame + mMeatEatingTime;
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6;
}

s16 Paramite::Brain_SpottedMeat_State_4_Turning(Meat* pMeat)
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn || !GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        return mBrainSubState;
    }

    if (!VIsFacingMe(pMeat))
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }

    if (FP_Abs(pMeat->mXPos - mXPos) > FP_FromInteger(5))
    {
        Sound(ParamiteSpeak::DetectedMeat_7, 0);
        SetNextMotion(eParamiteMotions::Motion_4_Turn);
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
    }
    else
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
    }
}

s16 Paramite::Brain_SpottedMeat_State_3_Jumping()
{
    if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
    {
        return mBrainSubState;
    }
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Idle_0;
}

s16 Paramite::Brain_SpottedMeat_State_2_Walking(Meat* pMeat)
{
    const s32 xPos_int = FP_GetExponent(mXPos);
    const s32 xSnapped = SnapToXGrid(GetSpriteScale(), xPos_int);

    if (mVelX < FP_FromInteger(0))
    {
        if (std::abs(xPos_int - xSnapped) < 6 && Check_IsOnEndOfLine(1, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (mVelX > FP_FromInteger(0))
    {
        if (std::abs(xPos_int - xSnapped) < 6 && Check_IsOnEndOfLine(0, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (!VIsFacingMe(pMeat))
    {
        if (FP_Abs(pMeat->mXPos - mXPos) > FP_FromInteger(5))
        {
            Sound(ParamiteSpeak::DetectedMeat_7, 0);
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
        }
    }

    if (!VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), pMeat))
    {
        return mBrainSubState;
    }

    if (pMeat->field_130_pLine)
    {
        SetNextMotion(eParamiteMotions::Motion_40_Eating);
        field_130_timer = sGnFrame + mMeatEatingTime;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6;
    }
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
}

s16 Paramite::Brain_SpottedMeat_State_1_Running(Meat* pMeat)
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    const s32 xAsInt = FP_GetExponent(mXPos);
    const s32 xSnapped = SnapToXGrid(GetSpriteScale(), xAsInt);

    if (mVelX < FP_FromInteger(0))
    {
        if (std::abs(xAsInt - xSnapped) < 6 && Check_IsOnEndOfLine(1, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (mVelX > FP_FromInteger(0))
    {
        if (std::abs(xAsInt - xSnapped) < 6 && Check_IsOnEndOfLine(0, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (!VIsFacingMe(pMeat))
    {
        if (FP_Abs(pMeat->mXPos - mXPos) > FP_FromInteger(5))
        {
            Sound(ParamiteSpeak::DetectedMeat_7, 0);
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
        }
    }

    if (VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), pMeat))
    {
        if (pMeat->field_130_pLine)
        {
            SetNextMotion(eParamiteMotions::Motion_40_Eating);
            field_130_timer = sGnFrame + mMeatEatingTime;
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6;
        }
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Running_1;
    }

    if (!VIsObjNearby(kGridSize * FP_FromInteger(3), pMeat))
    {
        return mBrainSubState;
    }

    SetNextMotion(eParamiteMotions::Motion_2_Walking);
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
}

s16 Paramite::Brain_SpottedMeat_State_0_Idle(Meat* pMeat)
{
    if (!VIsFacingMe(pMeat))
    {
        if (FP_Abs(pMeat->mXPos - mXPos) > FP_FromInteger(5))
        {
            Sound(ParamiteSpeak::DetectedMeat_7, 0);
            SetNextMotion(eParamiteMotions::Motion_4_Turn);
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Turning_4;
        }
    }

    if (VIsObj_GettingNear_On_X(pMeat))
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (Check_IsOnEndOfLine(0, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }
    else if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        if (Check_IsOnEndOfLine(1, 1))
        {
            ToHop();
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Jumping_3;
        }
    }

    if (!VIsObjNearby(kGridSize * FP_FromInteger(3), pMeat))
    {
        SetNextMotion(eParamiteMotions::Motion_3_Running);
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Running_1;
    }

    if (VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), pMeat))
    {
        if (mXPos == pMeat->mXPos)
        {
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
        }

        if (!pMeat->field_130_pLine || !BaseAliveGameObjectCollisionLine)
        {
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
        }

        SetNextMotion(eParamiteMotions::Motion_40_Eating);
        field_130_timer = sGnFrame + mMeatEatingTime;
        return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Eating_6;
    }

    SetNextMotion(eParamiteMotions::Motion_2_Walking);
    return ParamiteEnums::Brain_5_SpottedMeat::eBrain5_Walking_2;
}

s16 Paramite::Brain_6_Possessed()
{
    if (mBrainSubState == 1)
    {
        if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
        }

        if (EventGet(kEventDeathReset))
        {
            if (sControlledCharacter != this)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
        }

        if (mHealth <= FP_FromInteger(0))
        {
            SetBrain(&Paramite::Brain_1_Death);
            field_130_timer = sGnFrame + 90;
            return mBrainSubState;
        }
    }
    else if (mBrainSubState == 0)
    {
        if (static_cast<s32>(sGnFrame) >= field_130_timer)
        {
            mBrainSubState = 1;
            mHealth = FP_FromInteger(1);
        }
    }

    return mBrainSubState;
}

s16 Paramite::Brain_7_DeathDrop()
{
    if (mBrainSubState == 0)
    {
        field_130_timer = sGnFrame + 60;
        return 1;
    }
    else if (mBrainSubState == 1)
    {
        if (static_cast<s32>(sGnFrame) < field_130_timer)
        {
            if (static_cast<s32>(sGnFrame) == field_130_timer - 6)
            {
                SND_SEQ_Play(SeqId::HitBottomOfDeathPit_9, 1, 65, 65);
            }
            return mBrainSubState;
        }

        Environment_SFX_457A40(EnvironmentSfx::eFallingDeathScreamHitGround_15, 0, 0x7FFF, this);

        relive_new ScreenShake(0, 0);
        field_130_timer = sGnFrame + 30;
        return 2;
    }
    else
    {
        if (mBrainSubState != 2 || static_cast<s32>(sGnFrame) <= field_130_timer)
        {
            return mBrainSubState;
        }

        if (sControlledCharacter == this)
        {
            sControlledCharacter = sActiveHero;
            gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }

        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return mBrainSubState;
    }
}

s16 Paramite::Brain_8_ControlledByGameSpeak()
{
    if (sControlledCharacter->Type() != ReliveTypes::eParamite || sControlledCharacter->mHealth <= FP_FromInteger(0) || sControlledCharacter->GetSpriteScale() != GetSpriteScale())
    {
        SetBrain(&Paramite::Brain_0_Patrol);
        return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
    }
    if (VOnSameYLevel(sControlledCharacter))
    {
        const auto controlledParamiteMotion = static_cast<eParamiteMotions>(sControlledCharacter->mCurrentMotion);
        if (controlledParamiteMotion == eParamiteMotions::Motion_43_Attack ||
            controlledParamiteMotion == eParamiteMotions::Motion_31_RunningAttack)
        {
            if (static_cast<Paramite*>(sControlledCharacter)->FindTarget())
            {
                mTargetGuid = static_cast<Paramite*>(sControlledCharacter)->mTargetGuid;
                Sound(ParamiteSpeak::Howdy_5, 0);
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                SetBrain(&Paramite::Brain_2_ChasingAbe);
                field_148_timer = sGnFrame + mGroupChaseDelay;
                return ParamiteEnums::Brain_2_ChasingAbe::eBrain2_Inactive_0;
            }
        }
    }

    switch (mBrainSubState)
    {
        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Inactive_0:
        {
            field_178_flags.Clear(Flags_178::eBit7_alerted);
            s16 result = VIsFacingMe(sControlledCharacter);
            if (result)
            {
                SetNextMotion(eParamiteMotions::Motion_24_Empty);
                field_158_next_brain_ret = 9;
                result = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11;
            }
            else
            {
                if (GetCurrentMotion() == eParamiteMotions::Motion_4_Turn)
                {
                    result = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Inactive_0;
                }
                else
                {
                    SetNextMotion(eParamiteMotions::Motion_4_Turn);
                }
            }
            return result;
        }

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1:
            if (VIsFacingMe(sControlledCharacter))
            {
                FP gridBlock = {};
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    gridBlock = mXPos - ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    gridBlock = ScaleToGridSize(GetSpriteScale()) + mXPos;
                }

                auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eFleech, gridBlock, mYPos));
                if (pFleech && VIsFacingMe(pFleech))
                {
                    SetNextMotion(eParamiteMotions::Motion_43_Attack);
                    field_158_next_brain_ret = 1;
                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11;
                }

                const GameSpeakEvents lastSpeak = LastSpeak();
                switch (lastSpeak)
                {
                    case GameSpeakEvents::Paramite_Stay_49:
                        SetGamespeakNextMotion(eParamiteMotions::Motion_22_Hiss1);
                        field_130_timer = sGnFrame + StableDelay();
                        field_158_next_brain_ret = 9;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_Howdy_48:
                    case GameSpeakEvents::Paramite_AllYa_52:
                        SetGamespeakNextMotion(eParamiteMotions::Motion_24_Empty);
                        field_130_timer = sGnFrame + StableDelay();
                        field_158_next_brain_ret = 1;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_DoIt_51:
                        SetGamespeakNextMotion(eParamiteMotions::Motion_26_Hiss3);
                        field_130_timer = sGnFrame + StableDelay();
                        field_158_next_brain_ret = 7;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    default:
                        if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), sControlledCharacter))
                        {
                            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                            {
                                if (!WallHit(GetSpriteScale() * FP_FromInteger(20), -ScaleToGridSize(GetSpriteScale())))
                                {
                                    if (!Check_IsOnEndOfLine(1, 1) && GetCurrentMotion() == eParamiteMotions::Motion_0_Idle)
                                    {
                                        field_130_timer = sGnFrame + StableDelay();
                                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdleToWalking_4;
                                    }
                                    if (!Check_IsOnEndOfLine(1, 4) && VOnSameYLevel(sControlledCharacter))
                                    {
                                        SetNextMotion(eParamiteMotions::Motion_5_Hop);
                                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Jumping_3;
                                    }
                                }
                            }
                            else if (!GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                            {
                                if (!WallHit(GetSpriteScale() * FP_FromInteger(20), ScaleToGridSize(GetSpriteScale())))
                                {
                                    if (!Check_IsOnEndOfLine(0, 1) && GetCurrentMotion() == eParamiteMotions::Motion_0_Idle)
                                    {
                                        field_130_timer = sGnFrame + StableDelay();
                                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdleToWalking_4;
                                    }
                                }

                                if (!Check_IsOnEndOfLine(0, 4) && VOnSameYLevel(sControlledCharacter))
                                {
                                    SetNextMotion(eParamiteMotions::Motion_5_Hop);
                                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Jumping_3;
                                }
                            }
                        }

                        const auto controlledParamiteMotion = static_cast<eParamiteMotions>(sControlledCharacter->mCurrentMotion);
                        if (lastSpeak == GameSpeakEvents::Paramite_CMon_or_Attack_50)
                        {
                            SetGamespeakNextMotion(eParamiteMotions::Motion_23_Hiss2);
                            field_130_timer = sGnFrame + StableDelay();
                            field_158_next_brain_ret = 2;
                            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;
                        }
                        else if (controlledParamiteMotion != eParamiteMotions::Motion_43_Attack)
                        {
                            return mBrainSubState;
                        }
                        else
                        {
                            SetGamespeakNextMotion(eParamiteMotions::Motion_43_Attack);
                            field_158_next_brain_ret = 1;
                            field_130_timer = sGnFrame + 10;
                            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;
                        }
                        break;
                }
            }
            else
            {
                if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn)
                {
                    SetNextMotion(eParamiteMotions::Motion_4_Turn);
                }
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            break;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Falling_2:
            if (!VIsFacingMe(sControlledCharacter))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }

            if (GetCurrentMotion() != eParamiteMotions::Motion_2_Walking)
            {
                SetNextMotion(eParamiteMotions::Motion_2_Walking);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Falling_2;
            }

            if (mVelX < FP_FromInteger(0) && (WallHit(GetSpriteScale() * FP_FromInteger(20), -ScaleToGridSize(GetSpriteScale())) || Check_IsOnEndOfLine(1, 2)))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            else if (mVelX > FP_FromInteger(0) && (WallHit(GetSpriteScale() * FP_FromInteger(20), ScaleToGridSize(GetSpriteScale())) || Check_IsOnEndOfLine(0, 2)))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            else if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()), sControlledCharacter))
            {
                return mBrainSubState;
            }
            else
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            break;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Jumping_3:
            if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdleToWalking_4:
            if (field_130_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingWalking_5;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingWalking_5:
            if (!VIsFacingMe(sControlledCharacter))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(20), -ScaleToGridSize(GetSpriteScale())) || Check_IsOnEndOfLine(1, 2))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
            }
            else if (mVelX > FP_FromInteger(0))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(20), ScaleToGridSize(GetSpriteScale())) || Check_IsOnEndOfLine(0, 2))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
            }

            if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), sControlledCharacter))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }

            if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5), sControlledCharacter))
            {
                return mBrainSubState;
            }

            if (!(GetAnimation().mFlags.Get(AnimFlags::eFlipX)))
            {
                if (Check_IsOnEndOfLine(0, 3))
                {
                    return mBrainSubState;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(1, 3))
                {
                    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                    {
                        return mBrainSubState;
                    }

                    if (Check_IsOnEndOfLine(0, 3))
                    {
                        return mBrainSubState;
                    }
                }
            }
            SetNextMotion(eParamiteMotions::Motion_3_Running);
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingRunning_6;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingRunning_6:
            if (!VIsFacingMe(sControlledCharacter))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            if (mVelX < FP_FromInteger(0) && (WallHit(GetSpriteScale() * FP_FromInteger(20), -ScaleToGridSize(GetSpriteScale())) || Check_IsOnEndOfLine(1, 3)))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
            }
            else
            {
                if (mVelX > FP_FromInteger(0) && ((WallHit(GetSpriteScale() * FP_FromInteger(20), ScaleToGridSize(GetSpriteScale()))) || Check_IsOnEndOfLine(0, 3)))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
                else
                {
                    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), sControlledCharacter))
                    {
                        SetNextMotion(eParamiteMotions::Motion_0_Idle);
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                    }
                    else
                    {
                        if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(5), sControlledCharacter))
                        {
                            return mBrainSubState;
                        }
                        SetNextMotion(eParamiteMotions::Motion_2_Walking);
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingWalking_5;
                    }
                }
            }
            break;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_IdleToWorking_7:
            if (GetCurrentMotion() == eParamiteMotions::Motion_12_JumpUpBegin)
            {
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Working_8;
            }
            SetNextMotion(eParamiteMotions::Motion_12_JumpUpBegin);
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_IdleToWorking_7;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Working_8:
            if (GetCurrentMotion() == eParamiteMotions::Motion_0_Idle)
            {
                SetBrain(&Paramite::Brain_0_Patrol);
                return ParamiteEnums::Brain_0_Patrol::eBrain0_Inactive_0;
            }
            else
            {
                if (GetCurrentMotion() == eParamiteMotions::Motion_15_RopePull)
                {
                    SetNextMotion(eParamiteMotions::Motion_11_Falling);
                }
                return mBrainSubState;
            }
            break;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Idle_9:
            if (VIsFacingMe(sControlledCharacter))
            {
                const GameSpeakEvents lastSpeak = LastSpeak();
                switch (lastSpeak)
                {
                    case GameSpeakEvents::Paramite_CMon_or_Attack_50:
                        SetGamespeakNextMotion(eParamiteMotions::Motion_23_Hiss2);
                        field_130_timer = sGnFrame + StableDelay();
                        field_158_next_brain_ret = 1;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_Stay_49:
                        SetGamespeakNextMotion(eParamiteMotions::Motion_22_Hiss1);
                        field_130_timer = sGnFrame + StableDelay();
                        field_158_next_brain_ret = 9;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_Howdy_48:
                    case GameSpeakEvents::Paramite_AllYa_52:
                        SetGamespeakNextMotion(eParamiteMotions::Motion_24_Empty);
                        field_130_timer = sGnFrame + StableDelay();
                        field_158_next_brain_ret = 9;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    case GameSpeakEvents::Paramite_DoIt_51:
                        SetGamespeakNextMotion(eParamiteMotions::Motion_26_Hiss3);
                        field_130_timer = sGnFrame + StableDelay();
                        field_158_next_brain_ret = 7;
                        return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10;

                    default:
                        return mBrainSubState;
                }
            }
            else
            {
                if (GetCurrentMotion() != eParamiteMotions::Motion_4_Turn)
                {
                    SetNextMotion(eParamiteMotions::Motion_4_Turn);
                }
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Idle_9;
            }
            break;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_RecievedCommand_10:
            if (field_130_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            else
            {
                SetNextMotion(mParamiteGamespeakNextMotion);
                mParamiteGamespeakNextMotion = -1;
            }
            return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11;

        case ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11:
            if (GetCurrentMotion() == eParamiteMotions::Motion_23_Hiss2 || GetCurrentMotion() == eParamiteMotions::Motion_22_Hiss1 || GetCurrentMotion() == eParamiteMotions::Motion_26_Hiss3)
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
            }

            if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
            {
                return ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_Speaking_11;
            }
            else
            {
                return field_158_next_brain_ret;
            }
            break;

        default:
            return mBrainSubState;
    }
}

s16 Paramite::Brain_9_ParamiteSpawn()
{
    auto pExistingWeb = static_cast<ParamiteWeb*>(sObjectIds.Find_Impl(mWebGuid));
    switch (mBrainSubState)
    {
        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_Inactive_0:
            if (mSurpriseWebSwitchId)
            {
                mBrainSubState = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_PreSpawn_1;
            }
            else
            {
                if (BaseAliveGameObjectCollisionLine != nullptr)
                {
                    mBrainSubState = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_SlowDescend_2;
                }
                else
                {
                    mBrainSubState = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_SlowerDescend_3;
                }
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_PreSpawn_1:
            if (SwitchStates_Get(mSurpriseWebSwitchId))
            {
                field_178_flags.Set(Flags_178::eBit6_spawned);
                SfxPlayMono(relive::SoundEffects::ParamiteSpawn, 0);
                mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eCanBePossessed);
                GetAnimation().mFlags.Set(AnimFlags::eRender);

                if (!BaseAliveGameObjectCollisionLine)
                {
                    mVelY = FP_FromInteger(0);
                    SetCurrentMotion(eParamiteMotions::Motion_33_SurpriseWeb);
                    auto pWeb = relive_new ParamiteWeb(mXPos, FP_GetExponent(mYPos) - 20,
                                                    FP_GetExponent(mYPos) - 10,
                                                    GetSpriteScale());
                    if (pWeb)
                    {
                        mWebGuid = pWeb->mBaseGameObjectId;
                    }
                    mBrainSubState = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop1_4;
                }
                else
                {
                    SetBrain(&Paramite::Brain_8_ControlledByGameSpeak);
                    mBrainSubState = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
            }
            else
            {
                mBrainSubState = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_PreSpawn_1;
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_SlowDescend_2:
            if (field_160_last_event_index != gEventSystem->field_28_last_event_index)
            {
                field_160_last_event_index = gEventSystem->field_28_last_event_index;
                if (gEventSystem->field_20_last_event == GameSpeakEvents::Paramite_Howdy_48)
                {
                    GetAnimation().mFlags.Set(AnimFlags::eRender);
                    SetBrain(&Paramite::Brain_8_ControlledByGameSpeak);
                    mBrainSubState = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                }
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_SlowerDescend_3:
            if (field_160_last_event_index != gEventSystem->field_28_last_event_index)
            {
                field_160_last_event_index = gEventSystem->field_28_last_event_index;

                if (gEventSystem->field_20_last_event == GameSpeakEvents::Paramite_Howdy_48)
                {
                    GetAnimation().mFlags.Set(AnimFlags::eRender);
                    mVelY = FP_FromInteger(0);
                    SetCurrentMotion(eParamiteMotions::Motion_33_SurpriseWeb);
                    auto pWeb = relive_new ParamiteWeb(mXPos, FP_GetExponent(mYPos) - 20,
                                                    FP_GetExponent(mYPos) - 10,
                                                    GetSpriteScale());
                    if (pWeb)
                    {
                        mWebGuid = pWeb->mBaseGameObjectId;
                    }
                    mBrainSubState = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop1_4;
                }
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop1_4:
            pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(mYPos)) - 10;
            pExistingWeb->mYPos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
            if (GetCurrentMotion() == eParamiteMotions::Motion_0_Idle)
            {
                mBrainSubState = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                pExistingWeb->field_104_bEnabled = 1;
                mWebGuid = Guid{};
                SetBrain(&Paramite::Brain_8_ControlledByGameSpeak);
            }
            else
            {
                if (mVelY < (GetSpriteScale() * FP_FromInteger(8)))
                {
                    mVelY = (GetSpriteScale() * FP_FromDouble(0.5)) + mVelY;
                    return mBrainSubState;
                }
                mBrainSubState = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop2_5;
            }
            break;

        case ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop2_5:
            pExistingWeb->field_FA_ttl_remainder = FP_GetExponent(FP_Abs(mYPos)) - 10;
            pExistingWeb->mYPos = FP_FromInteger(pExistingWeb->field_FA_ttl_remainder);
            if (GetCurrentMotion() == eParamiteMotions::Motion_0_Idle)
            {
                mBrainSubState = ParamiteEnums::Brain_8_ControlledByGameSpeak::eBrain8_FollowingIdle_1;
                pExistingWeb->field_104_bEnabled = 1;
                mWebGuid = Guid{};
                SetBrain(&Paramite::Brain_8_ControlledByGameSpeak);
            }
            else
            {
                if (mVelY > (GetSpriteScale() * FP_FromInteger(-1)))
                {
                    mVelY = mVelY - (GetSpriteScale() * FP_FromInteger(1));
                    return mBrainSubState;
                }
                mBrainSubState = ParamiteEnums::Brain_9_ParamiteSpawn::eBrain9_DescendLoop1_4;
            }
            break;

        default:
            break;
    }
    return mBrainSubState;
}

void Paramite::Motion_0_Idle()
{
    if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
    {
        ToNextMotion();
        return;
    }

    if (Input_IsChanting_45F260() && !field_178_flags.Get(Flags_178::eBit5_prevent_depossession))
    {
        SetCurrentMotion(eParamiteMotions::Motion_29_GetDepossessedBegin);
        field_138_depossession_timer = sGnFrame + 30;
        SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
    }
    else if (Input().isHeld(sInputKey_GameSpeak1))
    {
        SetCurrentMotion(eParamiteMotions::Motion_20_GameSpeakBegin);
        SetNextMotion(eParamiteMotions::Motion_24_Empty);
    }
    else if (Input().isHeld(sInputKey_GameSpeak2))
    {
        SetCurrentMotion(eParamiteMotions::Motion_20_GameSpeakBegin);
        SetNextMotion(eParamiteMotions::Motion_23_Hiss2);
    }
    else if (Input().isHeld(sInputKey_GameSpeak3))
    {
        SetCurrentMotion(eParamiteMotions::Motion_20_GameSpeakBegin);
        SetNextMotion(eParamiteMotions::Motion_22_Hiss1);
    }
    else if (Input().isHeld(sInputKey_GameSpeak4))
    {
        SetCurrentMotion(eParamiteMotions::Motion_20_GameSpeakBegin);
        SetNextMotion(eParamiteMotions::Motion_26_Hiss3);
    }
    else if (Input().isHeld(sInputKey_GameSpeak6))
    {
        SetCurrentMotion(eParamiteMotions::Motion_25_AllOYaGameSpeakBegin);
    }
    else
    {
        ToNextMotion();
    }
}

const FP sWalkTable_5464BC[3] = {
    FP_FromInteger(0),
    FP_FromDouble(1.402099609375),
    FP_FromDouble(9.025436401367188)};

void Paramite::Motion_1_WalkBegin()
{
    FP frameVelX = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelX = -sWalkTable_5464BC[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sWalkTable_5464BC[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * frameVelX);

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            SetCurrentMotion(eParamiteMotions::Motion_2_Walking);
        }
    }
}

const FP sWalkTable_546484[14] = {
    FP_FromDouble(2.03741455078125),
    FP_FromDouble(4.02911376953125),
    FP_FromDouble(4.46575927734375),
    FP_FromDouble(3.3473663330078125),
    FP_FromDouble(3.1887054443359375),
    FP_FromDouble(4.937652587890625),
    FP_FromDouble(3.126708984375),
    FP_FromDouble(2.50823974609375),
    FP_FromDouble(4.440032958984375),
    FP_FromDouble(3.9687652587890625),
    FP_FromDouble(2.9050140380859375),
    FP_FromDouble(3.6402130126953125),
    FP_FromDouble(4.701416015625),
    FP_FromDouble(2.72283935546875)};


void Paramite::Motion_2_Walking()
{
    FP frameVelX = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelX = -sWalkTable_546484[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sWalkTable_546484[GetAnimation().GetCurrentFrame()];
    }

    mVelX = GetSpriteScale() * frameVelX;

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
        return;
    }

    MoveOnLine();

    if (GetCurrentMotion() == eParamiteMotions::Motion_2_Walking)
    {
        field_154_input |= Input().mPads[sCurrentControllerIndex].mHeld;
        switch (GetAnimation().GetCurrentFrame())
        {
            case 0:
            case 7:
            {
                if (GetAnimation().GetCurrentFrame() == 0)
                {
                    Sound(ParamiteSpeak::SlightStep_4, 0);
                }
                else
                {
                    Sound(ParamiteSpeak::LoudStep_3, 0);
                }

                FP gridBlock = {};
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    gridBlock = -ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    gridBlock = ScaleToGridSize(GetSpriteScale());
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(20), gridBlock * FP_FromInteger(1)))
                {
                    SetCurrentMotion(eParamiteMotions::Motion_8_WalkEnd);
                    return;
                }

                if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
                {
                    HandleStopWalking();
                    field_154_input = 0;
                    return;
                }

                if (GetNextMotion() == eParamiteMotions::Motion_0_Idle ||
                    GetNextMotion() == eParamiteMotions::Motion_20_GameSpeakBegin ||
                    GetNextMotion() == eParamiteMotions::Motion_4_Turn ||
                    GetNextMotion() == eParamiteMotions::Motion_42_Squawk ||
                    GetNextMotion() == eParamiteMotions::Motion_22_Hiss1 ||
                    GetNextMotion() == eParamiteMotions::Motion_40_Eating)
                {
                    SetCurrentMotion(eParamiteMotions::Motion_8_WalkEnd);
                }
                return;
            }

            case 3:
            case 10:
                if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
                {
                    if (Input().isPressed(sInputKey_Run))
                    {
                        field_178_flags.Set(Flags_178::eBit2_running);
                        SetPreviousMotion(eParamiteMotions::Motion_3_Running);
                        mBaseAliveGameObjectLastAnimFrame = 11;
                    }

                    field_154_input = 0;

                    if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
                    {
                        UpdateSlurgWatchPoints();
                        MapFollowMe(TRUE);
                        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
                    }
                    return;
                }

                if (GetNextMotion() == eParamiteMotions::Motion_3_Running)
                {
                    field_178_flags.Set(Flags_178::eBit2_running);
                    SetPreviousMotion(eParamiteMotions::Motion_3_Running);
                    mBaseAliveGameObjectLastAnimFrame = 11;
                    SetNextMotion(-1);
                    if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
                    {
                        UpdateSlurgWatchPoints();
                        MapFollowMe(TRUE);
                        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
                    }
                    return;
                }
                else if (GetNextMotion() == eParamiteMotions::Motion_31_RunningAttack)
                {
                    SetCurrentMotion(eParamiteMotions::Motion_31_RunningAttack);
                    SetNextMotion(-1);

                    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                    {
                        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    }
                    else
                    {
                        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                    }
                }

                if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
                {
                    UpdateSlurgWatchPoints();
                    MapFollowMe(TRUE);
                    field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
                }
                return;

            default:
                field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
                return;
        }
    }
}

const FP sRunningTable_5464E8[14] = {
    FP_FromDouble(5.1657867431640625),
    FP_FromDouble(5.624664306640625),
    FP_FromDouble(3.328460693359375),
    FP_FromDouble(3.532440185546875),
    FP_FromDouble(5.26165771484375),
    FP_FromDouble(10.463729858398438),
    FP_FromDouble(9.87750244140625),
    FP_FromDouble(8.148956298828125),
    FP_FromDouble(8.294830322265625),
    FP_FromDouble(7.1818389892578125),
    FP_FromDouble(6.7950592041015625),
    FP_FromDouble(8.932144165039062),
    FP_FromDouble(11.460769653320312),
    FP_FromDouble(5.9268646240234375)};

void Paramite::Motion_3_Running()
{
    FP frameVelX = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelX = -sRunningTable_5464E8[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sRunningTable_5464E8[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * frameVelX);
    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
        return;
    }

    MoveOnLine();

    if (GetCurrentMotion() == eParamiteMotions::Motion_3_Running)
    {
        field_154_input = Input().mPads[sCurrentControllerIndex].mHeld | field_154_input;

        if (GetAnimation().GetCurrentFrame() == 3 || GetAnimation().GetCurrentFrame() == 10)
        {
            if (GetAnimation().GetCurrentFrame() == 3)
            {
                SFX_Play_Pitch(relive::SoundEffects::PickupItem, 45, -600);
            }
            else
            {
                Sound(ParamiteSpeak::LoudStep_3, 0);
                Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);
            }

            if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (GetNextMotion() != eParamiteMotions::Motion_2_Walking)
                {
                    if (GetNextMotion() != eParamiteMotions::Motion_0_Idle &&
                        GetNextMotion() != eParamiteMotions::Motion_4_Turn &&
                        GetNextMotion() != eParamiteMotions::Motion_40_Eating &&
                        GetNextMotion() != eParamiteMotions::Motion_42_Squawk &&
                        GetNextMotion() != eParamiteMotions::Motion_22_Hiss1)
                    {
                        if (GetNextMotion() == eParamiteMotions::Motion_31_RunningAttack)
                        {
                            SetCurrentMotion(eParamiteMotions::Motion_31_RunningAttack);
                            SetNextMotion(-1);

                            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                            {
                                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                            }
                            else
                            {
                                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                            }
                        }
                        else
                        {
                            if (GetNextMotion() == eParamiteMotions::Motion_5_Hop)
                            {
                                ToHop();
                            }
                        }
                    }
                    else
                    {
                        SetCurrentMotion(eParamiteMotions::Motion_10_RunEnd);
                    }
                }
                else
                {
                    SetCurrentMotion(eParamiteMotions::Motion_7_WalkRunTransition);
                    SetNextMotion(-1);
                }

                if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
                {
                    UpdateSlurgWatchPoints();
                    MapFollowMe(TRUE);
                    field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
                }
            }
            else
            {
                HandleInputRunning();
                field_154_input = 0;
            }

            if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
            {
                UpdateSlurgWatchPoints();
                MapFollowMe(TRUE);
                field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
            }
            return;
        }

        if (GetAnimation().GetCurrentFrame() != 6 && GetAnimation().GetCurrentFrame() != 13)
        {
            field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
            return;
        }

        if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
        {
            if (field_154_input & sInputKey_Hop)
            {
                field_154_input = 0;
                ToHop();
            }
        }

        if (!field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
        {
            UpdateSlurgWatchPoints();
            MapFollowMe(TRUE);
            field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
            return;
        }
    }
}

void Paramite::Motion_4_Turn()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sound(ParamiteSpeak::SlightStep_4, 0);
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);

        if (ToNextMotion())
        {
            GetAnimation().Set_Animation_Data(GetAnimation().mAnimRes);
        }
        else
        {
            ToIdle();
        }
    }
}

const FP sHopTable_546544[14] = {
    FP_FromDouble(6.478485107421875),
    FP_FromDouble(6.952423095703125),
    FP_FromDouble(4.5809173583984375),
    FP_FromDouble(4.7915191650390625),
    FP_FromDouble(6.5775146484375),
    FP_FromDouble(12.131378173828125),
    FP_FromDouble(11.163650512695312),
    FP_FromDouble(6.437164306640625),
    FP_FromDouble(6.45751953125),
    FP_FromDouble(5.377593994140625),
    FP_FromDouble(5.9170379638671875),
    FP_FromDouble(7.3821563720703125),
    FP_FromDouble(6.7065582275390625),
    FP_FromDouble(9.063491821289062)};

void Paramite::Motion_5_Hop()
{
    FP frameVelX = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelX = -sHopTable_546544[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sHopTable_546544[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * frameVelX);

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        BaseAliveGameObjectCollisionLine = WebCollision(FP_FromInteger(0), mVelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            mVelX = FP_FromInteger(0);
            SetCurrentMotion(eParamiteMotions::Motion_38_WebGrab);
            auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType(ReliveTypes::eWebLine, mXPos, mYPos));
            if (pWeb)
            {
                pWeb->Wobble(FP_GetExponent(mYPos));
            }
        }
        else
        {
            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;
            const auto bHit = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(0.9));

            if (sControlledCharacter == this)
            {
                SetActiveCameraDelayedFromDir();
            }

            if (bHit)
            {
                switch (pLine->mLineType)
                {
                    case eLineTypes::eFloor_0:
                    case eLineTypes::eBackgroundFloor_4:
                    case eLineTypes::eDynamicCollision_32:
                    case eLineTypes::eBackgroundDynamicCollision_36:
                        BaseAliveGameObjectCollisionLine = pLine;
                        mXPos = hitX;
                        mYPos = hitY;
                        ToIdle();
                        CheckForPlatform();
                        UpdateSlurgWatchPoints();
                        return;

                    case eLineTypes::eWallLeft_1:
                    case eLineTypes::eWallRight_2:
                        mXPos = hitX - mVelX;
                        MapFollowMe(TRUE);
                        mVelX = FP_FromInteger(0);
                        mYPos = hitY;
                        return;

                    default:
                        break;
                }
            }

            if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(5))
            {
                field_13C_velx_offset = FP_FromDouble(0.75);
                SetCurrentMotion(eParamiteMotions::Motion_11_Falling);
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    mVelX = (GetSpriteScale() * FP_FromInteger(-5));
                }
                else
                {
                    mVelX = (GetSpriteScale() * FP_FromInteger(5));
                }
            }

            if (GetAnimation().GetCurrentFrame() == 3)
            {
                SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -500);
            }
            else if (GetAnimation().GetCurrentFrame() == 12)
            {
                Sound(ParamiteSpeak::LoudStep_3, 0);
                Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 55, 500, 0);
            }
        }
    }
}

const FP M_Unused_6_VelTable_5464D4[5] = {
    FP_FromDouble(2.2522430419921875),
    FP_FromDouble(5.6306304931640625),
    FP_FromDouble(7.3198089599609375),
    FP_FromDouble(7.3198089599609375),
    FP_FromDouble(5.6306304931640625)};

void Paramite::Motion_6_Unused()
{
    FP frameVelX = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelX = -M_Unused_6_VelTable_5464D4[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = M_Unused_6_VelTable_5464D4[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * frameVelX);

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        field_178_flags.Set(Flags_178::eBit2_running);
        SetPreviousMotion(eParamiteMotions::Motion_3_Running);
        mBaseAliveGameObjectLastAnimFrame = 2;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
    }
}

const FP sWalkToRunTable_546538[3] = {
    FP_FromDouble(5.3964691162109375),
    FP_FromDouble(5.396453857421875),
    FP_FromDouble(5.396484375)};

void Paramite::Motion_7_WalkRunTransition()
{
    FP frameVelX = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelX = -sWalkToRunTable_546538[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sWalkToRunTable_546538[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * frameVelX);

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_2_Walking);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
    }
}

const FP sWalkEndTable_5464C8[3] = {
    FP_FromDouble(2.3350982666015625),
    FP_FromDouble(5.0355224609375),
    FP_FromDouble(6.70159912109375)};

void Paramite::Motion_8_WalkEnd()
{
    FP frameVelX = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelX = -sWalkEndTable_5464C8[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sWalkEndTable_5464C8[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * frameVelX);

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            ToIdle();
        }
    }
}

const FP sRunBeginTable_546520[3] = {
    FP_FromDouble(1.8789520263671875),
    FP_FromDouble(2.15924072265625),
    FP_FromDouble(3.330413818359375)};

void Paramite::Motion_9_RunBegin()
{
    field_154_input |= Input().mPads[sCurrentControllerIndex].mHeld;

    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = (GetSpriteScale() * -sRunBeginTable_546520[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sRunBeginTable_546520[GetAnimation().GetCurrentFrame()]);
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        field_178_flags.Set(Flags_178::eBit2_running);
        SetPreviousMotion(eParamiteMotions::Motion_3_Running);
        mBaseAliveGameObjectLastAnimFrame = 2;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
    }
}

const FP sRunEndTable_54652C[3] = {
    FP_FromDouble(3.11126708984375),
    FP_FromDouble(3.063446044921875),
    FP_FromDouble(3.43255615234375)};

void Paramite::Motion_10_RunEnd()
{
    FP frameVelX = {};
    if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelX = -sRunEndTable_54652C[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sRunEndTable_54652C[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * frameVelX);

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
    }
}

void Paramite::Motion_11_Falling()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (GetSpriteScale() * field_13C_velx_offset);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (GetSpriteScale() * field_13C_velx_offset) + mVelX;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    BaseAliveGameObjectCollisionLine = WebCollision(FP_FromInteger(0), mVelX);
    if (BaseAliveGameObjectCollisionLine)
    {
        mVelX = FP_FromInteger(0);
        SetCurrentMotion(eParamiteMotions::Motion_38_WebGrab);
        auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType(ReliveTypes::eWebLine, mXPos, mYPos));
        if (pWeb)
        {
            pWeb->Wobble(FP_GetExponent(mYPos));
        }
    }
    else
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        const auto bCollision = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

        if (sControlledCharacter == this)
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
                    mYPos = hitY;
                    mXPos = hitX;
                    SetCurrentMotion(eParamiteMotions::Motion_17_Landing);

                    CheckForPlatform();
                    UpdateSlurgWatchPoints();

                    if (mYPos - BaseAliveGameObjectLastLineYPos > (GetSpriteScale() * FP_FromInteger(180)))
                    {
                        EventBroadcast(kScrabOrParamiteDied, this);
                        mHealth = FP_FromInteger(0);
                        SetBrain(&Paramite::Brain_1_Death);
                        SetCurrentMotion(eParamiteMotions::Motion_41_Death);
                        field_130_timer = sGnFrame + 90;

                        relive_new Blood(mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), GetSpriteScale(), 50);
                    }
                    break;

                case eLineTypes::eWallLeft_1:
                case eLineTypes::eWallRight_2:
                case eLineTypes::eBackgroundWallLeft_5:
                case eLineTypes::eBackgroundWallRight_6:
                    mXPos = hitX - mVelX;
                    mXPos = FP_FromInteger(SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos)));
                    mYPos = hitY;
                    mVelX = FP_FromInteger(0);
                    MapFollowMe(TRUE);
                    break;
                default:
                    return;
            }
        }
    }
}

void Paramite::Motion_12_JumpUpBegin()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(-10) * GetSpriteScale();
        mYPos += mVelY;
        VOnTrapDoorOpen();
        SetCurrentMotion(eParamiteMotions::Motion_13_JumpUpMidair);
        BaseAliveGameObjectCollisionLine = nullptr;
    }
    else if (GetAnimation().GetCurrentFrame() == 3)
    {
        SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -500);
    }
}

void Paramite::Motion_13_JumpUpMidair()
{
    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        ToKnockBack();
        return;
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* ppPathLine = nullptr;
    const auto bHit = InAirCollision(&ppPathLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (sControlledCharacter == this)
    {
        SetActiveCameraDelayedFromDir();
    }

    if (bHit)
    {
        switch (ppPathLine->mLineType)
        {
            case 0u:
            case 4u:
            case 32u:
            case 36u:
                BaseAliveGameObjectCollisionLine = ppPathLine;
                mXPos = hitX;
                mYPos = hitY;
                ToIdle();
                CheckForPlatform();
                UpdateSlurgWatchPoints();
                SetCurrentMotion(eParamiteMotions::Motion_14_JumpUpLand);
                GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
                return;

            case 3u:
            case 7u:
                if (mVelY < FP_FromInteger(0))
                {
                    mVelY = FP_FromInteger(0);
                    mYPos = hitY;
                }
                break;

            default:
                break;
        }
    }
    else
    {
        PullRingRope* pRope = FindPullRope();
        if (pRope)
        {
            if (pRope->VPull(this))
            {
                SetCurrentMotion(eParamiteMotions::Motion_15_RopePull);
                mPullRingRopeGuid = pRope->mBaseGameObjectId;
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return;
            }
            mPullRingRopeGuid = Guid{};
        }

        if (sControlledCharacter == this)
        {
            FP gridBlock = {};
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                gridBlock = -ScaleToGridSize(GetSpriteScale());
            }
            else
            {
                gridBlock = ScaleToGridSize(GetSpriteScale());
            }


            if (mVelY > FP_FromInteger(0))
            {
                BaseAliveGameObjectCollisionLine = WebCollision(FP_FromInteger(0), gridBlock);
                if (BaseAliveGameObjectCollisionLine)
                {
                    mVelX = FP_FromInteger(0);
                    SetCurrentMotion(eParamiteMotions::Motion_38_WebGrab);

                    auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType(ReliveTypes::eWebLine, mXPos, mYPos));
                    if (pWeb)
                    {
                        pWeb->Wobble(FP_GetExponent(mYPos));
                    }
                    // Don't try to go to falling
                    return;
                }
            }
            else if (FP_Abs(mVelY) < FP_FromInteger(3))
            {
                auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eFleech, mXPos, mYPos - FP_FromInteger(20)));
                if (pFleech)
                {
                    pFleech->VTakeDamage(this);
                    pFleech->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }
        }
    }

    if ((mYPos - BaseAliveGameObjectLastLineYPos) > FP_FromInteger(5))
    {
        field_13C_velx_offset = FP_FromDouble(0.75);
        SetCurrentMotion(eParamiteMotions::Motion_11_Falling);
    }
}

void Paramite::Motion_14_JumpUpLand()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }
    else if (GetAnimation().GetCurrentFrame() == 1)
    {
        Sound(ParamiteSpeak::LoudStep_3, 0);
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 55, 500, 0);
    }
}

void Paramite::Motion_15_RopePull()
{
    auto pPullRingRope = static_cast<PullRingRope*>(sObjectIds.Find_Impl(mPullRingRopeGuid));
    if (!pPullRingRope || ((pPullRingRope && pPullRingRope->VIsNotBeingPulled()) && (sControlledCharacter == this || GetNextMotion() == eParamiteMotions::Motion_11_Falling)))
    {
        if (pPullRingRope)
        {
            pPullRingRope->VMarkAsPulled();
        }
        mPullRingRopeGuid = Guid{};
        mVelY = FP_FromInteger(0);
        SetCurrentMotion(eParamiteMotions::Motion_13_JumpUpMidair);
        SetNextMotion(-1);
        GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
    }
}

void Paramite::Motion_16_CloseAttack()
{
    if (GetAnimation().GetCurrentFrame() == 10)
    {
        Sound(ParamiteSpeak::CMon_or_Attack_0, 0);

        BaseAliveGameObject* pObj = nullptr;
        if (sControlledCharacter == this)
        {
            if (FindTarget())
            {
                pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
                mTargetGuid = Guid{};
            }
        }
        else
        {
            pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
        }

        if (pObj)
        {
            const PSX_RECT otherRect = pObj->VGetBoundingRect();
            const PSX_RECT ourRect = VGetBoundingRect();

            s16 right = 0;
            s16 left = 0;
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                right = ourRect.x;
                left = (ourRect.x + ourRect.w) / 2;
            }
            else
            {
                left = ourRect.w;
                right = (ourRect.x + ourRect.w) / 2;
            }

            if (otherRect.x <= left && otherRect.w >= right && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
            {
                if (GetSpriteScale() == pObj->GetSpriteScale())
                {
                    if (!WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }
}

void Paramite::Motion_17_Landing()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }
}

void Paramite::Motion_18_Unused()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }
}

void Paramite::Motion_19_Knockback()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_11_Falling();
        if (GetCurrentMotion() == eParamiteMotions::Motion_41_Death || GetCurrentMotion() == eParamiteMotions::Motion_17_Landing)
        {
            SetCurrentMotion(eParamiteMotions::Motion_19_Knockback);
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, -200, this);
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (BaseAliveGameObjectCollisionLine || !GetAnimation().mFlags.Get(AnimFlags::eRender))
        {
            if (mHealth > FP_FromInteger(0))
            {
                ToIdle();
            }
            else
            {
                SetCurrentMotion(eParamiteMotions::Motion_41_Death);
                SetBrain(&Paramite::Brain_1_Death);
                field_130_timer = sGnFrame + 90;
            }
        }
    }
}

void Paramite::Motion_20_GameSpeakBegin()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    if (pObj)
    {
        const PSX_RECT otherRect = pObj->VGetBoundingRect();
        const PSX_RECT ourRect = VGetBoundingRect();

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear_On_X(pObj))
            {
                if (GetSpriteScale() == pObj->GetSpriteScale())
                {
                    if (!WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                    {
                        if (pObj->mHealth > FP_FromInteger(0))
                        {
                            SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                            Mudokon_SFX(MudSounds::eHurt2_9, 0, 0, pObj);
                        }
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_21_PreHiss);
    }
}

void Paramite::Motion_21_PreHiss()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    if (pObj)
    {
        const PSX_RECT otherRect = pObj->VGetBoundingRect();
        const PSX_RECT ourRect = VGetBoundingRect();

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear_On_X(pObj))
            {
                if (GetSpriteScale() == pObj->GetSpriteScale())
                {
                    if (!WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }

    switch (GetNextMotion())
    {
        case eParamiteMotions::Motion_22_Hiss1:
        case eParamiteMotions::Motion_23_Hiss2:
        case eParamiteMotions::Motion_26_Hiss3:
            SetCurrentMotion(mNextMotion);
            SetNextMotion(-1);
            break;

        case eParamiteMotions::Motion_24_Empty:
            SetCurrentMotion(eParamiteMotions::Motion_28_GameSpeakEnd);
            SetNextMotion(-1);
            Sound(ParamiteSpeak::Howdy_5, 0);

            if (sControlledCharacter == this)
            {
                gEventSystem->PushEvent(GameSpeakEvents::Paramite_Howdy_48);
            }
            break;

        case eParamiteMotions::Motion_31_RunningAttack:
            SetCurrentMotion(eParamiteMotions::Motion_31_RunningAttack);
            SetNextMotion(-1);

            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }

            break;

        default:
            if (mNextMotion != -1 || sControlledCharacter == this)
            {
                SetCurrentMotion(eParamiteMotions::Motion_28_GameSpeakEnd);
            }
            break;
    }
}

void Paramite::Motion_22_Hiss1()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    if (pObj)
    {
        const PSX_RECT otherRect = pObj->VGetBoundingRect();
        const PSX_RECT ourRect = VGetBoundingRect();

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear_On_X(pObj))
            {
                if (GetSpriteScale() == pObj->GetSpriteScale())
                {
                    if (!WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }

    if (GetAnimation().GetCurrentFrame() != 2 || field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
    {
        field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
    }
    else
    {
        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
        EventBroadcast(kEventLoudNoise, this);
        Sound(ParamiteSpeak::Stay_1, 0);

        if (sControlledCharacter == this)
        {
            gEventSystem->PushEvent(GameSpeakEvents::Paramite_Stay_49);
        }
    }

    if (GetNextMotion() == eParamiteMotions::Motion_31_RunningAttack)
    {
        SetCurrentMotion(eParamiteMotions::Motion_31_RunningAttack);
        SetNextMotion(-1);

        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }
        else
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }
    }
    else if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_27_PostHiss);
    }
}

void Paramite::Motion_23_Hiss2()
{
    if (GetAnimation().GetCurrentFrame() != 2 || field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
    {
        field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
    }
    else
    {
        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
        Sound(ParamiteSpeak::CMon_or_Attack_0, 0);
        if (sControlledCharacter == this)
        {
            gEventSystem->PushEvent(GameSpeakEvents::Paramite_CMon_or_Attack_50);
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_27_PostHiss);
    }
}

void Paramite::Motion_24_Empty()
{
    // Empty
}

void Paramite::Motion_25_AllOYaGameSpeakBegin()
{
    if (GetAnimation().GetCurrentFrame() == 7)
    {
        Sound(ParamiteSpeak::AllYa_9, 0);
        if (sControlledCharacter == this)
        {
            gEventSystem->PushEvent(GameSpeakEvents::Paramite_AllYa_52);
        }
    }
    else if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_28_GameSpeakEnd);
    }
}

void Paramite::Motion_26_Hiss3()
{
    if (GetAnimation().GetCurrentFrame() != 2 || field_178_flags.Get(Flags_178::eBit3_hissed_or_left_screen))
    {
        field_178_flags.Clear(Flags_178::eBit3_hissed_or_left_screen);
    }
    else
    {
        field_178_flags.Set(Flags_178::eBit3_hissed_or_left_screen);
        EventBroadcast(kEventLoudNoise, this);
        Sound(ParamiteSpeak::DoIt_2, 0);
        if (sControlledCharacter == this)
        {
            gEventSystem->PushEvent(GameSpeakEvents::Paramite_DoIt_51);
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_27_PostHiss);
    }
}

void Paramite::Motion_27_PostHiss()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    if (pObj)
    {
        const PSX_RECT otherRect = pObj->VGetBoundingRect();
        const PSX_RECT ourRect = VGetBoundingRect();

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear_On_X(pObj))
            {
                if (GetSpriteScale() == pObj->GetSpriteScale())
                {
                    if (!WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_21_PreHiss);
    }
}

void Paramite::Motion_28_GameSpeakEnd()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    if (pObj)
    {
        const PSX_RECT otherRect = pObj->VGetBoundingRect();
        const PSX_RECT ourRect = VGetBoundingRect();

        if (otherRect.x <= ourRect.w && otherRect.w >= ourRect.x && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (VIsObj_GettingNear_On_X(pObj))
            {
                if (GetSpriteScale() == pObj->GetSpriteScale())
                {
                    if (!WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (!ToNextMotion())
        {
            ToIdle();
        }
    }
}

void Paramite::Motion_29_GetDepossessedBegin()
{
    if (sControlledCharacter == this)
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            if (!Input_IsChanting_45F260())
            {
                SetCurrentMotion(eParamiteMotions::Motion_30_GetDepossessedEnd);
            }
        }

        if (!(static_cast<s32>(sGnFrame) % 4))
        {
            New_TintChant_Particle(
                (GetSpriteScale() * FP_FromInteger(Math_RandomRange(-20, 20))) + mXPos,
                mYPos - (GetSpriteScale() * FP_FromInteger(Math_RandomRange(20, 50))),
                GetSpriteScale(),
                Layer::eLayer_0);
        }

        if (static_cast<s32>(sGnFrame) > field_138_depossession_timer)
        {
            sControlledCharacter = sActiveHero;
            mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::ePossessed);
            SetCurrentMotion(eParamiteMotions::Motion_30_GetDepossessedEnd);
            SetBrain(&Paramite::Brain_0_Patrol);
            mBrainSubState = 0;
            gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
            if (mAbeCamera != gMap.mCurrentCamera)
            {
                if (field_178_flags.Get(Flags_178::eBit6_spawned))
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
            }
        }
    }
}

void Paramite::Motion_30_GetDepossessedEnd()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }
}

void Paramite::Motion_31_RunningAttack()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));

    if (WallHit(GetSpriteScale() * FP_FromInteger(20), mVelX))
    {
        BaseAliveGameObjectCollisionLine = nullptr;
        BaseAliveGameObjectLastLineYPos = mYPos;
        ToKnockBack();
        return;
    }
    else if (sControlledCharacter == this)
    {
        auto pWebLine = WebCollision(FP_FromInteger(0), mVelX);
        if (pWebLine)
        {
            mVelX = FP_FromInteger(0);
            SetCurrentMotion(eParamiteMotions::Motion_38_WebGrab);
            BaseAliveGameObjectCollisionLine = pWebLine;
            return;
        }
    }

    MoveOnLine();

    if (GetCurrentMotion() == eParamiteMotions::Motion_31_RunningAttack)
    {
        if (sControlledCharacter == this)
        {
            SetActiveCameraDelayedFromDir();
            if (FindTarget())
            {
                pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
                mTargetGuid = Guid{};
            }
        }

        if (pObj)
        {
            const PSX_RECT otherRect = pObj->VGetBoundingRect();
            const PSX_RECT ourRect = VGetBoundingRect();

            s16 right = 0;
            s16 left = 0;
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                right = ourRect.x;
                left = (ourRect.x + ourRect.w) / 2;
            }
            else
            {
                left = ourRect.w;
                right = (ourRect.x + ourRect.w) / 2;
            }

            if (otherRect.x <= left && otherRect.w >= right && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
            {
                if (GetSpriteScale() == pObj->GetSpriteScale())
                {
                    if (!WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }

        if (GetAnimation().GetCurrentFrame() == 3)
        {
            Sound(ParamiteSpeak::CMon_or_Attack_0, 0);
        }

        if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
        {
            ToIdle();
        }
    }
}

void Paramite::Motion_32_Empty()
{
    
}

void Paramite::Motion_33_SurpriseWeb()
{
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mVelY + mYPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
        BaseAliveGameObjectLastLineYPos = hitY;
        SetCurrentMotion(eParamiteMotions::Motion_34_WebLeaveDown);
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            CheckForPlatform();
        }
    }
    else
    {
        mYPos += mVelY;
    }

    if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 3)
    {
        Sound(ParamiteSpeak::ClimbingWeb_6, 0);
    }
}

void Paramite::Motion_34_WebLeaveDown()
{
    if (GetAnimation().GetCurrentFrame() == 2)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);
        mXOffset = field_15C_paramite_xOffset;
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }
}

void Paramite::Motion_35_WebIdle()
{
    // Go down web
    if (Input().isPressed(sInputKey_Down))
    {
        SetCurrentMotion(eParamiteMotions::Motion_37_WebGoingDown);
    }

    // Go up web
    if (Input().isPressed(sInputKey_Up))
    {
        SetCurrentMotion(eParamiteMotions::Motion_36_WebGoingUp);
    }

    // Try to leave to the left
    if (Input().isPressed(sInputKey_Left))
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (sCollisions->Raycast(
                mXPos - (GetSpriteScale() * FP_FromDouble(0.5)),
                mYPos - FP_FromInteger(10),
                mXPos - (GetSpriteScale() * FP_FromDouble(0.5)),
                mYPos + FP_FromInteger(10),
                &pLine,
                &hitX,
                &hitY,
                GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
        {
            auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType(ReliveTypes::eWebLine, mXPos, mYPos));
            if (pWeb)
            {
                pWeb->Wobble(FP_GetExponent(mYPos));
            }
            GetAnimation().mFlags.Set(AnimFlags::eFlipX);
            mXPos = hitX;
            mYPos = hitY;
            BaseAliveGameObjectCollisionLine = pLine;
            SetCurrentMotion(eParamiteMotions::Motion_34_WebLeaveDown);
        }
    }

    // Try to leave to the right
    if (Input().isPressed(sInputKey_Right))
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (sCollisions->Raycast(
                mXPos + (GetSpriteScale() * FP_FromDouble(0.5)),
                mYPos - FP_FromInteger(10),
                mXPos + (GetSpriteScale() * FP_FromDouble(0.5)),
                mYPos + FP_FromInteger(10),
                &pLine,
                &hitX,
                &hitY,
                GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
        {
            auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType(ReliveTypes::eWebLine, mXPos, mYPos));
            if (pWeb)
            {
                pWeb->Wobble(FP_GetExponent(mYPos));
            }
            GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
            mXPos = hitX;
            mYPos = hitY;
            BaseAliveGameObjectCollisionLine = pLine;
            SetCurrentMotion(eParamiteMotions::Motion_34_WebLeaveDown);
        }
    }
}

void Paramite::Motion_36_WebGoingUp()
{
    if (Input().isPressed(sInputKey_Up))
    {
        mVelY = -(GetSpriteScale() * FP_FromInteger(4));
    }
    else
    {
        mVelY = FP_FromInteger(0);
        SetCurrentMotion(eParamiteMotions::Motion_35_WebIdle);
    }

    const auto oldYPos = mYPos;
    const auto pOldLine = BaseAliveGameObjectCollisionLine;

    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelY);
    }

    if (sControlledCharacter == this)
    {
        SetActiveCameraDelayedFromDir();
    }

    if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 3)
    {
        Sound(ParamiteSpeak::ClimbingWeb_6, 0);
    }

    if (!BaseAliveGameObjectCollisionLine || !((1 << BaseAliveGameObjectCollisionLine->mLineType) & 0x100) || FP_GetExponent(mYPos - (GetSpriteScale() * FP_FromInteger(20))) < BaseAliveGameObjectCollisionLine->mRect.y)
    {
        BaseAliveGameObjectCollisionLine = nullptr;

        const auto kHalfGrid = (ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(0.5));

        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;

        if (!sCollisions->Raycast(
                mXPos - FP_FromInteger(10),
                mYPos - FP_FromInteger(30),
                mXPos,
                mYPos - FP_FromInteger(30),
                &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgWalls : kBgWalls))
        {
            if (sCollisions->Raycast(
                    mXPos - kHalfGrid,
                    mYPos - FP_FromInteger(30),
                    mXPos - kHalfGrid,
                    mYPos + FP_FromInteger(10),
                    &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
                mXPos = hitX;
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
                SetCurrentMotion(eParamiteMotions::Motion_39_WebLeaveUp);

                auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType(ReliveTypes::eWebLine, mXPos, mYPos));
                if (pWeb)
                {
                    pWeb->Wobble(FP_GetExponent(mYPos));
                }
                return;
            }

            if (!sCollisions->Raycast(
                    mXPos + FP_FromInteger(10),
                    mYPos - FP_FromInteger(30),
                    mXPos,
                    mYPos - FP_FromInteger(30),
                    &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgWalls : kBgWalls))
            {
                if (sCollisions->Raycast(
                        kHalfGrid + mXPos,
                        mYPos - FP_FromInteger(30),
                        kHalfGrid + mXPos,
                        mYPos + FP_FromInteger(10),
                        &pLine, &hitX, &hitY, GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
                {
                    GetAnimation().mFlags.Set(AnimFlags::eFlipX);
                    mXPos = hitX;
                    mYPos = hitY;
                    BaseAliveGameObjectCollisionLine = pLine;
                    SetCurrentMotion(eParamiteMotions::Motion_39_WebLeaveUp);
                }
                else
                {
                    SetCurrentMotion(eParamiteMotions::Motion_11_Falling);
                    BaseAliveGameObjectLastLineYPos = mYPos;
                    mXOffset = field_15C_paramite_xOffset;
                }

                auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType(ReliveTypes::eWebLine, mXPos, mYPos));
                if (pWeb)
                {
                    pWeb->Wobble(FP_GetExponent(mYPos));
                }
                return;
            }
        }

        mYPos = oldYPos;
        BaseAliveGameObjectCollisionLine = pOldLine;
    }
}

void Paramite::Motion_37_WebGoingDown()
{
    if (Input().isPressed(sInputKey_Down))
    {
        mVelY = (GetSpriteScale() * FP_FromInteger(4));
    }
    else
    {
        mVelY = FP_FromInteger(0);
        SetCurrentMotion(eParamiteMotions::Motion_35_WebIdle);
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelY);
    }

    if (sControlledCharacter == this)
    {
        SetActiveCameraDelayedFromDir();
    }

    if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 3)
    {
        Sound(ParamiteSpeak::ClimbingWeb_6, 0);
    }

    if (!BaseAliveGameObjectCollisionLine || !((1 << BaseAliveGameObjectCollisionLine->mLineType) & 0x100))
    {
        auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType(ReliveTypes::eWebLine, mXPos, mYPos));
        if (pWeb)
        {
            pWeb->Wobble(FP_GetExponent(mYPos));
        }
        BaseAliveGameObjectCollisionLine = nullptr;
        BaseAliveGameObjectLastLineYPos = mYPos;
        SetCurrentMotion(eParamiteMotions::Motion_11_Falling);

        FP gridSize = {};
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            gridSize = ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSize = -ScaleToGridSize(GetSpriteScale());
        }

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        if (sCollisions->Raycast(
                gridSize + mXPos,
                mYPos,
                gridSize + mXPos,
                (GetSpriteScale() * FP_FromInteger(50)) + mYPos,
                &pLine,
                &hitX,
                &hitY,
                GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
        {
            mXPos = (gridSize * FP_FromDouble(0.5)) + mXPos;
            BaseAliveGameObjectCollisionLine = pLine;
            mYPos = hitY;
            SetCurrentMotion(eParamiteMotions::Motion_34_WebLeaveDown);
            GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
        }
        else
        {
            const FP invertedGridSize = -gridSize;
            if (sCollisions->Raycast(
                    invertedGridSize + mXPos,
                    mYPos,
                    invertedGridSize + mXPos,
                    (GetSpriteScale() * FP_FromInteger(50)) + mYPos,
                    &pLine,
                    &hitX,
                    &hitY,
                    GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mXPos = (invertedGridSize * FP_FromDouble(0.5)) + mXPos;
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
                SetCurrentMotion(eParamiteMotions::Motion_34_WebLeaveDown);
            }
            else
            {
                GetAnimation().mFlags.Toggle(AnimFlags::eFlipX);
                mXPos -= (invertedGridSize * FP_FromDouble(0.5));
                mXOffset = field_15C_paramite_xOffset;
            }
        }
    }
}

void Paramite::Motion_38_WebGrab()
{
    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mXOffset = 0;
        SetCurrentMotion(eParamiteMotions::Motion_35_WebIdle);
        auto pWeb = static_cast<ParamiteWebLine*>(FindObjectOfType(ReliveTypes::eWebLine, mXPos, mYPos));
        if (pWeb)
        {
            pWeb->Wobble(FP_GetExponent(mYPos));
        }
    }
}

void Paramite::Motion_39_WebLeaveUp()
{
    Motion_34_WebLeaveDown();
}

void Paramite::Motion_40_Eating()
{
    if (GetAnimation().GetCurrentFrame() == 5)
    {
        FP gridBlock = {};
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            gridBlock = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridBlock = ScaleToGridSize(GetSpriteScale());
        }

        if (sControlledCharacter == this)
        {
            auto pFleech = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eFleech, gridBlock + mXPos, mYPos - FP_FromInteger(5)));
            if (pFleech)
            {
                pFleech->VTakeDamage(this);
                pFleech->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else
            {
                auto pSlurg = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eSlurg, gridBlock + mXPos, mYPos));
                if (pSlurg)
                {
                    relive_new Blood(pSlurg->mXPos, pSlurg->mYPos, FP_FromInteger(0), FP_FromInteger(5), GetSpriteScale(), 30);
                    pSlurg->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
                else
                {
                    SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Eating1, relive::SoundEffects::Eating2), 0);
                }
            }
        }
        else
        {
            SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Eating1, relive::SoundEffects::Eating2), 0);
        }
    }

    if (sControlledCharacter == this && GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (Input().isPressed(sInputKey_Down))
        {
            return;
        }
        else
        {
            ToIdle();
            return;
        }
    }

    if (mNextMotion != -1 && !ToNextMotion())
    {
        ToIdle();
        return;
    }
}

void Paramite::Motion_41_Death()
{
    
}

void Paramite::Motion_42_Squawk()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sound(ParamiteSpeak::DoIt_2, 0);
    }
}

void Paramite::Motion_43_Attack()
{
    BaseAliveGameObject* pObj = nullptr;

    if (GetAnimation().GetCurrentFrame() == 4)
    {
        pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
        Sound(ParamiteSpeak::CMon_or_Attack_0, 0);

        if (sControlledCharacter == this)
        {
            if (FindTarget())
            {
                pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
                mTargetGuid = Guid{};
            }
        }
        else if (field_178_flags.Get(Flags_178::eBit8_bAttack_fleeches))
        {
            if (!pObj)
            {
                FP gridBlock = {};
                if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
                {
                    gridBlock = mXPos - ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    gridBlock = ScaleToGridSize(GetSpriteScale()) + mXPos;
                }
                pObj = static_cast<BaseAliveGameObject*>(FindObjectOfType(ReliveTypes::eFleech, gridBlock, mYPos));
            }
        }
    }

    if (pObj)
    {
        const PSX_RECT otherRect = pObj->VGetBoundingRect();
        const PSX_RECT ourRect = VGetBoundingRect();

        s16 right = 0;
        s16 left = 0;
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            right = ourRect.x;
            left = (ourRect.x + ourRect.w) / 2;
        }
        else
        {
            left = ourRect.w;
            right = (ourRect.x + ourRect.w) / 2;
        }
        if (otherRect.x <= left && otherRect.w >= right && otherRect.h >= ourRect.y && otherRect.y <= ourRect.h)
        {
            if (GetSpriteScale() == pObj->GetSpriteScale())
            {
                if (!WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                {
                    pObj->VTakeDamage(this);
                }
            }
        }
    }

    if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }
}

Paramite::~Paramite()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(mWebGuid);
    if (pObj)
    {
        pObj->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        mWebGuid = Guid{};
    }

    mMeatGuid = Guid{};
    VOnTrapDoorOpen();

    if (mHealth > FP_FromInteger(0) || field_178_flags.Get(Flags_178::eBit6_spawned))
    {
        Path::TLV_Reset(field_140_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_140_tlvInfo, -1, 0, 1);
    }

    SND_SEQ_Stop(SeqId::ParamiteNearby_25);
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (sControlledCharacter == this)
    {
        sControlledCharacter = sActiveHero;
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
}

void Paramite::HandleDDCheat()
{
    VOnTrapDoorOpen();
    SetCurrentMotion(eParamiteMotions::Motion_11_Falling);
    BaseAliveGameObjectLastLineYPos = mYPos;
    BaseAliveGameObjectCollisionLine = nullptr;

    static s32 sArray1_5C929C[8] = {};
    static s32 sArray2_5C92BC[8] = {};

    static s8 byte_5C9298 = 0;

    if (!(byte_5C9298 & 1))
    {
        byte_5C9298 |= 1u;
        sArray1_5C929C[0] = 4;
        sArray1_5C929C[1] = 4;
        sArray1_5C929C[2] = 0;
        sArray1_5C929C[3] = -4;
        sArray1_5C929C[4] = -4;
        sArray1_5C929C[5] = -4;
        sArray1_5C929C[6] = 0;
        sArray1_5C929C[7] = 4;
    }
    else if (!(byte_5C9298 & 2))
    {
        byte_5C9298 = byte_5C9298 | 2;
        sArray2_5C92BC[0] = 0;
        sArray2_5C92BC[1] = -4;
        sArray2_5C92BC[2] = -4;
        sArray2_5C92BC[3] = -4;
        sArray2_5C92BC[4] = 0;
        sArray2_5C92BC[5] = 4;
        sArray2_5C92BC[6] = 4;
        sArray2_5C92BC[7] = 4;
    }

    // TODO: InputCommand constants
    if (Input().mPads[sCurrentControllerIndex].mPressed & 0xF)
    {
        mVelX = FP_FromInteger(sArray1_5C929C[Input().mPads[sCurrentControllerIndex].mDir >> 5]);
        mVelY = FP_FromInteger(sArray2_5C92BC[Input().mPads[sCurrentControllerIndex].mDir >> 5]);

        if (sInputKey_Run & Input().mPads[sCurrentControllerIndex].mPressed)
        {
            mVelX += FP_FromInteger(sArray1_5C929C[Input().mPads[sCurrentControllerIndex].mDir >> 5]);
            mVelX += FP_FromInteger(sArray1_5C929C[Input().mPads[sCurrentControllerIndex].mDir >> 5]);
            mVelY += FP_FromInteger(sArray2_5C92BC[Input().mPads[sCurrentControllerIndex].mDir >> 5]);
        }

        mXPos += mVelX;
        mYPos += mVelY;

        // Keep in map bounds
        PSX_Point mapBounds = {};
        gMap.Get_map_size(&mapBounds);
        if (mXPos < FP_FromInteger(0))
        {
            mXPos = FP_FromInteger(0);
        }

        if (mXPos >= FP_FromInteger(mapBounds.x))
        {
            mXPos = FP_FromInteger(mapBounds.x) - FP_FromInteger(1);
        }

        if (mYPos < FP_FromInteger(0))
        {
            mYPos = FP_FromInteger(0);
        }

        if (mYPos >= FP_FromInteger(mapBounds.y))
        {
            mYPos = FP_FromInteger(mapBounds.y) - FP_FromInteger(1);
        }
    }
    else
    {
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
    }
    SetActiveCameraDelayedFromDir();
}

void Paramite::HandleBrainsAndMotions()
{
    const auto oldMotion = mCurrentMotion;
    mBrainSubState = (this->*mBrainState)();

    if (sDDCheat_ShowAI_Info)
    {
        DDCheat::DebugStr(
            "Paramite %d %d %d %d\n",
            mBrainSubState,
            field_130_timer,
            mCurrentMotion,
            mNextMotion);
    }

    const FP oldXPos = mXPos;
    const FP oldYPos = mYPos;
    (this->*sParamiteMotionTable[mCurrentMotion])();

    if (oldXPos != mXPos || oldYPos != mYPos)
    {
        BaseAliveGameObjectPathTLV = sPathInfo->TlvGetAt(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
        VOnTlvCollision(BaseAliveGameObjectPathTLV);
    }

    if (mbGotShot)
    {
        ToKnockBack();
        mbGotShot = false;
        SetCurrentMotion(mNextMotion);
        SetNextMotion(-1);
    }

    if (oldMotion == mCurrentMotion)
    {
        if (field_178_flags.Get(Flags_178::eBit2_running))
        {
            SetCurrentMotion(mPreviousMotion);
            vUpdateAnim();
            GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
            field_178_flags.Clear(Flags_178::eBit2_running);
        }
    }
    else
    {
        vUpdateAnim();
    }
}

void Paramite::VUpdate()
{
    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eRestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eRestoredFromQuickSave);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
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
        mMeatGuid = BaseGameObject::RefreshId(mMeatGuid);
        mTargetGuid = BaseGameObject::RefreshId(mTargetGuid);
        mPullRingRopeGuid = BaseGameObject::RefreshId(mPullRingRopeGuid);

        if (mWebGuid != Guid{})
        {
            auto pWeb = relive_new ParamiteWeb(mXPos,
                                            FP_GetExponent(mYPos) - 20,
                                            FP_GetExponent(mYPos) - 10,
                                            GetSpriteScale());
            if (pWeb)
            {
                mWebGuid = pWeb->mBaseGameObjectId;
            }
        }
    }

    auto pMeat = static_cast<BaseThrowable*>(sObjectIds.Find_Impl(mMeatGuid));
    if (mHealth > FP_FromInteger(0)
        && gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        SND_SEQ_PlaySeq(SeqId::ParamiteNearby_25, 1, 0);
    }
    else if (!Find_Paramite())
    {
        SND_SEQ_Stop(SeqId::ParamiteNearby_25);
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const FP xDelta = FP_Abs(mXPos - sControlledCharacter->mXPos);
    const FP yDelta = FP_Abs(mYPos - sControlledCharacter->mYPos);

    if (xDelta > FP_FromInteger(750) || yDelta > FP_FromInteger(390))
    {
        if (field_178_flags.Get(Flags_178::eBit4_out_of_sight) && field_178_flags.Get(Flags_178::eBit6_spawned))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        else if (pMeat)
        {
            if (pMeat->VIsFalling() || pMeat->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                mMeatGuid = Guid{};
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                SetBrain(&Paramite::Brain_0_Patrol);
                mBrainSubState = 0;
            }
        }
        GetAnimation().mFlags.Clear(AnimFlags::eAnimate);
        GetAnimation().mFlags.Clear(AnimFlags::eRender);
    }
    else
    {
        if (mHealth > FP_FromInteger(0))
        {
            if (!BrainIs(&Paramite::Brain_9_ParamiteSpawn) || field_178_flags.Get(Flags_178::eBit6_spawned))
            {
                GetAnimation().mFlags.Set(AnimFlags::eRender);
            }
            GetAnimation().mFlags.Set(AnimFlags::eAnimate);
        }

        if (!Input_IsChanting_45F260())
        {
            field_178_flags.Clear(Flags_178::eBit5_prevent_depossession);
        }


        if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter == this)
        {
            HandleDDCheat();
        }
        else
        {
            HandleBrainsAndMotions();
        }
    }
}

s16 Paramite::Find_Paramite()
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eParamite && pObj != this && gMap.Is_Point_In_Current_Camera(pObj->mCurrentLevel, pObj->mCurrentPath, pObj->mXPos, pObj->mYPos, 0))
        {
            return 1;
        }
    }
    return 0;
}

void Paramite::vUpdateAnim()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sParamiteMotionAnimIds[mCurrentMotion]));
}

Meat* Paramite::FindMeat()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eMeat)
        {
            auto pMeat = static_cast<Meat*>(pObj);
            if (pMeat->VCanEatMe())
            {
                if (gMap.Is_Point_In_Current_Camera(pMeat->mCurrentLevel, pMeat->mCurrentPath, pMeat->mXPos, pMeat->mYPos, 0) && !WallHit(mYPos, pMeat->mXPos - mXPos))
                {
                    if (!pMeat->field_130_pLine)
                    {
                        return pMeat;
                    }

                    if (FP_Abs(pMeat->mYPos - mYPos) <= FP_FromInteger(20))
                    {
                        return pMeat;
                    }
                }
            }
        }
    }
    return nullptr;
}

s16 Paramite::IsNear(Paramite* pOther)
{
    return FP_Abs(pOther->mYPos - mYPos) < GetSpriteScale() * FP_FromInteger(100);
}

s16 Paramite::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    if (pOther)
    {
        const PSX_RECT bRect = pOther->VGetBoundingRect();

        if ((FP_Abs(mYPos - FP_FromInteger(bRect.h)) < GetSpriteScale() * FP_FromInteger(40)) || (pOther->Type() == ReliveTypes::eParamite && static_cast<Paramite*>(pOther)->GetCurrentMotion() == eParamiteMotions::Motion_13_JumpUpMidair))
        {
            return 1;
        }
    }
    return 0;
}

void Paramite::VUnPosses()
{
    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::ePossessed);
    field_130_timer = sGnFrame + 180;
    SetBrain(&Paramite::Brain_0_Patrol);
    mBrainSubState = 0;
}

void Paramite::VPossessed()
{
    mBaseAliveGameObjectFlags.Set(AliveObjectFlags::ePossessed);
    field_178_flags.Set(Flags_178::eBit5_prevent_depossession);
    SetBrain(&Paramite::Brain_6_Possessed);
    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    mBrainSubState = 0;
    field_130_timer = sGnFrame + 30;
    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;
}

s16 Paramite::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return 1;
    }

    mMeatGuid = Guid{};

    switch (pFrom->Type())
    {
        case ReliveTypes::eDrill:
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eAirExplosion:
        {
            EventBroadcast(kScrabOrParamiteDied, this);
            relive_new Gibs(GibType::Slog_2, mXPos, mYPos, mVelX, mVelY, GetSpriteScale(), 0);
            mHealth = FP_FromInteger(0);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            GetAnimation().mFlags.Clear(AnimFlags::eRender);
            if (sControlledCharacter != this)
            {
                return 1;
            }
            SND_SEQ_Play(SeqId::DeathDrums_29, 1, 127, 127);
        }
            return 1;

        case ReliveTypes::eFleech:
        {
            mHealth -= FP_FromDouble(0.15);
            if (mHealth < FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
            }
            if (sControlledCharacter != this)
            {
                if (mTargetGuid == Guid{} || mTargetGuid == sActiveHero->mBaseGameObjectId)
                {
                    if (BrainIs(&Paramite::Brain_0_Patrol) || BrainIs(&Paramite::Brain_8_ControlledByGameSpeak))
                    {
                        SetBrain(&Paramite::Brain_2_ChasingAbe);
                        mBrainSubState = 0;
                        field_148_timer = sGnFrame + mGroupChaseDelay;
                        mTargetGuid = pFrom->mBaseGameObjectId;
                    }
                }
            }

            if (mHealth > FP_FromInteger(0))
            {
                return 1;
            }

            EventBroadcast(kScrabOrParamiteDied, this);
            SetBrain(&Paramite::Brain_1_Death);
            field_130_timer = sGnFrame + 90;
            SetCurrentMotion(eParamiteMotions::Motion_41_Death);
            vUpdateAnim();

            relive_new Blood(mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), GetSpriteScale(), 50);

            if (sControlledCharacter == this)
            {
                SND_SEQ_Play(SeqId::DeathDrums_29, 1, 127, 127);
            }
        }
            return 0;

        case ReliveTypes::eAbilityRing:
            return 0;

        default:
        {
            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            EventBroadcast(kScrabOrParamiteDied, this);
            mHealth = FP_FromInteger(0);
            SetBrain(&Paramite::Brain_1_Death);
            field_130_timer = sGnFrame + 90;
            SetCurrentMotion(eParamiteMotions::Motion_41_Death);
            vUpdateAnim();

            relive_new Blood(mXPos, mYPos, FP_FromInteger(0), FP_FromInteger(5), GetSpriteScale(), 50);

            if (sControlledCharacter != this)
            {
                return 1;
            }

            SND_SEQ_Play(SeqId::DeathDrums_29, 1, 127, 127);
            return 1;
        }
    }
}

u8** Paramite::ResBlockForMotion(s16 motion)
{
    const auto paramiteMotion = static_cast<eParamiteMotions>(motion);
    if (paramiteMotion < eParamiteMotions::Motion_20_GameSpeakBegin)
    {
        return field_10_resources_array.ItemAt(0);
    }
    if (paramiteMotion < eParamiteMotions::Motion_31_RunningAttack)
    {
        return field_10_resources_array.ItemAt(5);
    }
    if (paramiteMotion < eParamiteMotions::Motion_32_Empty)
    {
        return field_10_resources_array.ItemAt(14);
    }
    if (paramiteMotion < eParamiteMotions::Motion_40_Eating)
    {
        return field_10_resources_array.ItemAt(4);
    }
    if (paramiteMotion < eParamiteMotions::Motion_41_Death)
    {
        return field_10_resources_array.ItemAt(1);
    }
    if (paramiteMotion < eParamiteMotions::Motion_42_Squawk)
    {
        return field_10_resources_array.ItemAt(15);
    }
    if (paramiteMotion >= eParamiteMotions::Motion_43_Attack)
    {
        return field_10_resources_array.ItemAt(motion >= 44 ? 0 : 9);
    }
    return field_10_resources_array.ItemAt(10);
}

void Paramite::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            if (mHealth > FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
                mBrainSubState = 0;
                SetBrain(&Paramite::Brain_7_DeathDrop);
                mVelY = FP_FromInteger(0);
                mVelX = FP_FromInteger(0);
                SetCurrentMotion(eParamiteMotions::Motion_41_Death);
                EventBroadcast(kScrabOrParamiteDied, this);
            }
        }
        pTlv = sPathInfo->TlvGetAt(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

s16 Paramite::AnotherParamiteNear()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eParamite && pObj != this)
        {
            auto pOther = static_cast<Paramite*>(pObj);
            if (pOther->GetSpriteScale() == GetSpriteScale() && gMap.Is_Point_In_Current_Camera(pOther->mCurrentLevel, pOther->mCurrentPath, pOther->mXPos, pOther->mYPos, 0) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) && IsNear(pOther))
            {
                if (pOther->BrainIs(&Paramite::Brain_0_Patrol) || pOther->BrainIs(&Paramite::Brain_2_ChasingAbe))
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

PathLine* Paramite::WebCollision(FP yOff, FP xOff)
{
    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (!sCollisions->Raycast(
            mXPos,
            mYPos + yOff,
            mXPos + xOff,
            mYPos + yOff, &pLine, &hitX, &hitY, CollisionMask(eTrackLine_8)))
    {
        return 0;
    }

    mXPos = hitX;
    mYPos = hitY;
    return pLine;
}

void Paramite::VRender(PrimHeader** ppOt)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Paramite::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        SetCurrentMotion(eParamiteMotions::Motion_11_Falling);
    }
}

void Paramite::ToHop()
{
    VOnTrapDoorOpen();
    SetCurrentMotion(eParamiteMotions::Motion_5_Hop);
    SetNextMotion(-1);
    BaseAliveGameObjectLastLineYPos = mYPos;
    mVelY = FP_FromDouble(-6.3) * GetSpriteScale();
    mYPos += mVelY;
    BaseAliveGameObjectCollisionLine = nullptr;
}

s16 Paramite::CanIAcceptAGameSpeakCommand()
{
    if (sControlledCharacter->Type() != ReliveTypes::eParamite)
    {
        return 0;
    }

    const s32 distToPlayer = Math_Distance(
        FP_GetExponent(sControlledCharacter->mXPos),
        FP_GetExponent(sControlledCharacter->mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos));

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find another paramite on the same layer/scale
        if (pObj != this && pObj != sControlledCharacter && pObj->GetSpriteScale() == sControlledCharacter->GetSpriteScale() && pObj->Type() == ReliveTypes::eParamite)
        {
            auto pParamite = static_cast<Paramite*>(pObj);

            if (pParamite->field_178_flags.Get(Flags_178::eBit7_alerted))
            {
                return 0;
            }

            if (!pParamite->BrainIs(&Paramite::Brain_8_ControlledByGameSpeak) && gMap.Is_Point_In_Current_Camera(pParamite->mCurrentLevel, pParamite->mCurrentPath, pParamite->mXPos, pParamite->mYPos, 0))
            {
                if (sControlledCharacter->VIsFacingMe(pParamite) && !sControlledCharacter->VIsFacingMe(this))
                {
                    return 0;
                }

                if (distToPlayer > Math_Distance(
                        FP_GetExponent(sControlledCharacter->mXPos),
                        FP_GetExponent(sControlledCharacter->mYPos),
                        FP_GetExponent(pParamite->mXPos),
                        FP_GetExponent(pParamite->mYPos)))
                {
                    if (sControlledCharacter->VIsFacingMe(pParamite) == sControlledCharacter->VIsFacingMe(this))
                    {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

s16 Paramite::HandleEnemyStopper(s16 numGridBlocks)
{
    auto pEnemyStopper = static_cast<relive::Path_EnemyStopper*>(sPathInfo->TLV_Get_At_4DB4B0(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos + (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(numGridBlocks))),
        FP_GetExponent(mYPos),
        ReliveTypes::eEnemyStopper));

    // No stopper or its disabled
    if (!pEnemyStopper || !SwitchStates_Get(pEnemyStopper->mSwitchId))
    {
        return 0;
    }

    // We have a stopper that applies to any direction
    if (pEnemyStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
    {
        return 1;
    }

    // Does the stopper direction match the animation direction?
    if ((pEnemyStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && GetAnimation().mFlags.Get(AnimFlags::eFlipX)) || (pEnemyStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && !(GetAnimation().mFlags.Get(AnimFlags::eFlipX))))
    {
        return 1;
    }

    return 0;
}

PullRingRope* Paramite::FindPullRope()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::ePullRingRope)
        {
            auto pRope = static_cast<PullRingRope*>(pObj);

            if (pRope->GetSpriteScale() == GetSpriteScale())
            {
                const PSX_RECT bRect = pRope->VGetBoundingRect();
                if ((mYPos - (GetSpriteScale() * FP_FromInteger(40))) <= pRope->mYPos && mYPos > pRope->mYPos)
                {
                    if (mXPos > FP_FromInteger(bRect.x) && mXPos < FP_FromInteger(bRect.w))
                    {
                        return pRope;
                    }
                }
            }
        }
    }
    return nullptr;
}

s16 Paramite::NextPlayerInputMotion()
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    if (Input().isPressed(sInputKey_Right))
    {
        if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
        {
            SetCurrentMotion(eParamiteMotions::Motion_4_Turn);
            return 1;
        }
        else
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(20), kGridSize))
            {
                return 0;
            }

            if (Input().isPressed(sInputKey_Run))
            {
                mVelX = (kGridSize / FP_FromDouble(3.5));
                SetCurrentMotion(eParamiteMotions::Motion_9_RunBegin);
            }
            else
            {
                mVelX = (kGridSize / FP_FromInteger(7));
                SetCurrentMotion(eParamiteMotions::Motion_1_WalkBegin);
            }
            return 1;
        }
    }
    else if (Input().isPressed(sInputKey_Left))
    {
        if (!(GetAnimation().mFlags.Get(AnimFlags::eFlipX)))
        {
            SetCurrentMotion(eParamiteMotions::Motion_4_Turn);
            return 1;
        }
        else
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(20), -kGridSize))
            {
                return 0;
            }

            if (Input().isPressed(sInputKey_Run))
            {
                mVelX = -(kGridSize / FP_FromDouble(3.5));
                SetCurrentMotion(eParamiteMotions::Motion_9_RunBegin);
            }
            else
            {
                mVelX = -(kGridSize / FP_FromInteger(7));
                SetCurrentMotion(eParamiteMotions::Motion_1_WalkBegin);
            }
            return 1;
        }
    }
    else
    {
        if (Input().isHeld(sInputKey_Up))
        {
            SetCurrentMotion(eParamiteMotions::Motion_12_JumpUpBegin);
            return 1;
        }
        else if (Input().isPressed(sInputKey_Down))
        {
            FP xCheck = {};
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                xCheck = -kGridSize;
            }
            else
            {
                xCheck = kGridSize;
            }

            PathLine* pLine = WebCollision(GetSpriteScale() * FP_FromInteger(20), xCheck);
            if (pLine)
            {
                SetCurrentMotion(eParamiteMotions::Motion_38_WebGrab);
                BaseAliveGameObjectCollisionLine = pLine;
            }
            else
            {
                SetCurrentMotion(eParamiteMotions::Motion_40_Eating);
            }
            return 1;
        }
        else
        {
            if (Input().isPressed(sInputKey_ThrowItem | sInputKey_DoAction | sInputKey_GameSpeak5))
            {
                if (FindTarget())
                {
                    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
                    if (pTarget)
                    {
                        mTargetGuid = Guid{};
                        if (VIsObjNearby(kGridSize * FP_FromDouble(0.5), pTarget))
                        {
                            SetCurrentMotion(eParamiteMotions::Motion_16_CloseAttack);
                            return 1;
                        }
                    }
                }

                SetCurrentMotion(eParamiteMotions::Motion_43_Attack);
                return 1;
            }
            else
            {
                if (Input().isPressed(sInputKey_Hop))
                {
                    ToHop();
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
}

s16 Paramite::FindTarget()
{
    if (sControlledCharacter == this)
    {
        BaseAliveGameObject* pBestTarget = nullptr;
        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if ((pObj->Type() == ReliveTypes::eFleech || pObj->Type() == ReliveTypes::eSlig || pObj->Type() == ReliveTypes::eSlurg || pObj->Type() == ReliveTypes::eMudokon) && pObj->mHealth > FP_FromInteger(0))
            {
                if (VOnSameYLevel(pObj))
                {
                    if (GetSpriteScale() == pObj->GetSpriteScale())
                    {
                        if (!WallHit(GetSpriteScale() * FP_FromInteger(20), pObj->mXPos - mXPos))
                        {
                            if (FP_GetExponent(pObj->mXPos) / 375 == FP_GetExponent(mXPos) / 375 && FP_GetExponent(pObj->mYPos) / 260 == FP_GetExponent(mYPos) / 260)
                            {
                                if (!pBestTarget)
                                {
                                    pBestTarget = pObj;
                                }
                                else
                                {
                                    if (FP_Abs(mXPos - pObj->mXPos) < FP_Abs(mXPos - pBestTarget->mXPos))
                                    {
                                        pBestTarget = pObj;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (pBestTarget)
        {
            mTargetGuid = pBestTarget->mBaseGameObjectId;
            return 1;
        }
    }

    if (VOnSameYLevel(sActiveHero) && !sActiveHero->mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eInvisible) && GetSpriteScale() == sActiveHero->GetSpriteScale() && !WallHit((sActiveHero->GetSpriteScale() * FP_FromInteger(20)), sActiveHero->mXPos - mXPos))
    {
        mTargetGuid = sActiveHero->mBaseGameObjectId;
        return 1;
    }
    else
    {
        mTargetGuid = Guid{};
        return 0;
    }
}

s16 Paramite::ToNextMotion()
{
    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        return NextPlayerInputMotion();
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    switch (GetNextMotion())
    {
        case eParamiteMotions::Motion_4_Turn:
        case eParamiteMotions::Motion_40_Eating:
        case eParamiteMotions::Motion_12_JumpUpBegin:
        case eParamiteMotions::Motion_42_Squawk:
        case eParamiteMotions::Motion_43_Attack:
        case eParamiteMotions::Motion_16_CloseAttack:
        case eParamiteMotions::Motion_20_GameSpeakBegin:
            SetCurrentMotion(mNextMotion);
            SetNextMotion(-1);
            return 1;

        case eParamiteMotions::Motion_3_Running:
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(kGridSize / FP_FromDouble(3.5));
                SetCurrentMotion(eParamiteMotions::Motion_9_RunBegin);
            }
            else
            {
                mVelX = (kGridSize / FP_FromDouble(3.5));
                SetCurrentMotion(eParamiteMotions::Motion_9_RunBegin);
            }
            SetNextMotion(-1);
            return 1;

        case eParamiteMotions::Motion_2_Walking:
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(kGridSize / FP_FromInteger(7));
            }
            else
            {
                mVelX = (kGridSize / FP_FromInteger(7));
            }
            SetCurrentMotion(eParamiteMotions::Motion_1_WalkBegin);
            SetNextMotion(-1);
            return 1;

        case eParamiteMotions::Motion_22_Hiss1:
        case eParamiteMotions::Motion_24_Empty:
        case eParamiteMotions::Motion_23_Hiss2:
        case eParamiteMotions::Motion_26_Hiss3:
            SetCurrentMotion(eParamiteMotions::Motion_20_GameSpeakBegin);
            return 1;

        case eParamiteMotions::Motion_25_AllOYaGameSpeakBegin:
            SetCurrentMotion(eParamiteMotions::Motion_25_AllOYaGameSpeakBegin);
            SetNextMotion(-1);
            return 1;

        case eParamiteMotions::Motion_31_RunningAttack:
            SetCurrentMotion(eParamiteMotions::Motion_31_RunningAttack);
            SetNextMotion(-1);
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(kGridSize / FP_FromInteger(4));
            }
            else
            {
                mVelX = (kGridSize / FP_FromInteger(4));
            }
            return 1;

        case eParamiteMotions::Motion_5_Hop:
            ToHop();
            return 1;

        default:
            return 0;
    }
}

void Paramite::ToIdle()
{
    field_13C_velx_offset = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    SetCurrentMotion(eParamiteMotions::Motion_0_Idle);
    MapFollowMe(TRUE);
    field_154_input = 0;
}

void Paramite::ToKnockBack()
{
    mXPos -= mVelX;
    field_154_input = 0;

    MapFollowMe(TRUE);

    if (mVelY < FP_FromInteger(0))
    {
        mVelY = FP_FromInteger(0);
    }

    SetCurrentMotion(eParamiteMotions::Motion_19_Knockback);
}

void Paramite::MoveOnLine()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectLastLineYPos = mYPos;
        SetCurrentMotion(eParamiteMotions::Motion_11_Falling);
        return;
    }

    BaseGameObject* pPlatform = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    const FP oldXPos = mXPos;

    // As we move on the line it might change to another one
    BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
    if (BaseAliveGameObjectCollisionLine)
    {
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eTrackLine_8)
        {
            mVelX = FP_FromInteger(0);
            mXPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.x);
            mYPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y) + (GetSpriteScale() * FP_FromInteger(20));
            SetCurrentMotion(eParamiteMotions::Motion_38_WebGrab);
        }
        else if (pPlatform)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
            {
                const auto savedMotion = mCurrentMotion;
                VOnTrapDoorOpen();
                SetCurrentMotion(savedMotion);
            }
        }
        else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            CheckForPlatform();
        }
    }
    else
    {
        // Not on a line so we are falling

        if (sControlledCharacter == this)
        {
            // Check if we hit a web while falling and grab it if so
            BaseAliveGameObjectCollisionLine = WebCollision(GetSpriteScale() * FP_FromInteger(20), mVelX);
            if (BaseAliveGameObjectCollisionLine)
            {
                mVelX = FP_FromInteger(0);
                SetCurrentMotion(eParamiteMotions::Motion_38_WebGrab);
                return;
            }
        }

        // Otherwise just fall
        VOnTrapDoorOpen();
        SetCurrentMotion(eParamiteMotions::Motion_11_Falling);
        BaseAliveGameObjectLastLineYPos = mYPos;
        field_13C_velx_offset = FP_FromDouble(0.3);
        mXPos = oldXPos + mVelX;
    }
}

void Paramite::CheckForPlatform()
{
    PSX_Point xy = {};
    PSX_Point wh = {};

    xy.x = FP_GetExponent(mXPos - FP_FromInteger(5));
    xy.y = FP_GetExponent(mYPos - FP_FromInteger(5));

    wh.x = FP_GetExponent(mXPos + FP_FromInteger(5));
    wh.y = FP_GetExponent(mYPos + FP_FromInteger(5));

    OnCollisionWith(xy, wh, gPlatformsArray);
}

void Paramite::HandleStopWalking()
{
    // Pressing opposite direction of movement or not pressing any direction
    if ((mVelX > FP_FromInteger(0) && Input().isPressed(sInputKey_Left)) || (mVelX < FP_FromInteger(0) && Input().isPressed(sInputKey_Right)) || !Input().isPressed(sInputKey_Left | sInputKey_Right))
    {
        SetCurrentMotion(eParamiteMotions::Motion_8_WalkEnd);
    }
}

void Paramite::HandleInputRunning()
{
    if ((mVelX > FP_FromInteger(0) && Input().isPressed(sInputKey_Left)) || (mVelX < FP_FromInteger(0) && Input().isPressed(sInputKey_Right)) || !Input().isPressed(sInputKey_Left | sInputKey_Right))
    {
        SetCurrentMotion(eParamiteMotions::Motion_10_RunEnd);
    }
    else if (Input().isPressed(sInputKey_Run))
    {
        if (sInputKey_Hop & field_154_input)
        {
            ToHop();
        }
        else if ((sInputKey_ThrowItem | sInputKey_DoAction) & field_154_input)
        {
            SetCurrentMotion(eParamiteMotions::Motion_31_RunningAttack);
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
        }
    }
    else
    {
        SetCurrentMotion(eParamiteMotions::Motion_7_WalkRunTransition);
    }
}

s16 sParamiteDelayIdx_5C92EC = 0;

const s16 sParamiteDelayTable_55D7B8[4] = {0, 5, 10, 0};

s16 Paramite::StableDelay()
{
    if (sParamiteDelayIdx_5C92EC >= 3u)
    {
        sParamiteDelayIdx_5C92EC = 0;
    }
    return sParamiteDelayTable_55D7B8[sParamiteDelayIdx_5C92EC++];
}

// TODO: repetition with `MainMenu.cpp`
const relive::SfxDefinition paramite_stru_55D7C0[12] = {
    {0u, 5u, 60u, 90u, -520, -520},
    {0u, 5u, 61u, 75u, -520, -520},
    {0u, 5u, 62u, 50u, -520, -520},
    {0u, 5u, 64u, 40u, -520, -520},
    {0u, 5u, 65u, 40u, -520, -520},
    {0u, 5u, 66u, 30u, -520, -520},
    {0u, 5u, 66u, 50u, 0, 0},
    {0u, 5u, 67u, 40u, -520, -520},
    {0u, 5u, 68u, 30u, -520, -520},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

void Paramite::Sound(ParamiteSpeak soundId, s16 pitch_min)
{
    const CameraPos direction = gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos);

    s16 volRight = 0;
    if (soundId == ParamiteSpeak::Howdy_5)
    {
        volRight = paramite_stru_55D7C0[5].field_C_default_volume;
        if (sControlledCharacter == this)
        {
            volRight *= 3;
        }
    }
    else if (soundId == ParamiteSpeak::AllYa_9)
    {
        volRight = sSeqData_558D50.mSeqs[20].field_9_volume;
    }
    else
    {
        volRight = paramite_stru_55D7C0[static_cast<s32>(soundId)].field_C_default_volume;
    }

    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        volRight = (2 * volRight) / 3;
    }

    PSX_RECT pRect = {};
    gMap.Get_Camera_World_Rect(direction, &pRect);

    s16 volLeft = 0;
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
            volLeft = volRight;
            break;

        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            const s16 v12 = FP_GetExponent(FP_FromRaw(paramite_stru_55D7C0[static_cast<s32>(soundId)].field_C_default_volume) / FP_FromInteger(3));
            volLeft = v12;
            volRight = v12;
            break;
        }

        case CameraPos::eCamLeft_3:
        {
            const FP v9 = (FP_FromInteger(pRect.w) - mXPos) / FP_FromInteger(368);
            volLeft = volRight - FP_GetExponent((v9 * FP_FromInteger((volRight - 1) * (volRight / 3))));
            const FP v10 = (v9 * FP_FromInteger(volRight));
            volRight = volRight - FP_GetExponent(v10);
            break;
        }

        case CameraPos::eCamRight_4:
        {
            const FP v11 = (mXPos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
            volLeft = volRight - FP_GetExponent(v11 * FP_FromInteger(volRight));
            const FP v10 = (v11 * FP_FromInteger((volRight - 1) * (volRight / 3)));
            volRight = volRight - FP_GetExponent(v10);
            break;
        }

        default:
            return;
    }

    if (soundId == ParamiteSpeak::AllYa_9)
    {
        SND_SEQ_Play(SeqId::MainMenuParamiteAttack_20, 1, volLeft, volRight);
    }
    else if (pitch_min > 0)
    {
        SFX_SfxDefinition_Play_Stereo(paramite_stru_55D7C0[static_cast<s32>(soundId)], volLeft, volRight, pitch_min, pitch_min);
    }
    else
    {
        SFX_SfxDefinition_Play_Stereo(paramite_stru_55D7C0[static_cast<s32>(soundId)], volLeft, volRight, -520, -520);
    }
}

void Paramite::UpdateSlurgWatchPoints()
{
    const s8 count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
    if (count < 5)
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
            pPoints->mPoints[count].x = FP_GetExponent(mXPos);
            pPoints->mPoints[count].y = BaseAliveGameObjectCollisionLine->mRect.y - 5;
            sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
        }
    }
}

GameSpeakEvents Paramite::LastSpeak()
{
    GameSpeakEvents ret = GameSpeakEvents::eNone_m1;
    if (field_160_last_event_index == gEventSystem->field_28_last_event_index)
    {
        if (gEventSystem->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            ret = GameSpeakEvents::eNone_m1;
        }
        else
        {
            ret = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_160_last_event_index = gEventSystem->field_28_last_event_index;
        ret = gEventSystem->field_20_last_event;
    }

    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 1))
    {
        return ret;
    }

    return GameSpeakEvents::eNone_m1;
}
