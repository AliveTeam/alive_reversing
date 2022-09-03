#include "stdafx.h"
#include "FlyingSlig.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "MusicController.hpp"
#include "DDCheat.hpp"
#include "Slig.hpp"
#include "Grenade.hpp"
#include "Dove.hpp"
#include "CrawlingSlig.hpp"
#include "Particle.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "GameSpeak.hpp"
#include "Explosion.hpp"
#include "Bullet.hpp"
#include "ParticleBurst.hpp"
#include "Lever.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"
#include "Function.hpp"
#include "AnimationCallBacks.hpp"

// Warning, index is saved, order matters here
ALIVE_ARY(1, 0x5523A0, TFlyingSligBrainFn, 26, sFlyingSlig_motion_table_5523A0,
          {
              &FlyingSlig::M_Idle_0_4385E0,
              &FlyingSlig::M_HorizontalMovement_1_4386A0,
              &FlyingSlig::M_IdleToTurn_2_4388B0,
              &FlyingSlig::M_DownMovement_3_438AA0,
              &FlyingSlig::M_DownMovementToTurn_4_438CC0,
              &FlyingSlig::M_UpMovement_5_438DD0,
              &FlyingSlig::M_UpMovementToTurn_6_439030,
              &FlyingSlig::M_LeverPull_7_439150,
              &FlyingSlig::M_GameSpeak_8_4391D0,
              &FlyingSlig::M_Possession_9_434290,
              &FlyingSlig::M_EndHorizontalMovement_10_4387D0,
              &FlyingSlig::M_BeginUpMovement_11_438E40,
              &FlyingSlig::M_HorizontalToDownMovement_12_438B10,
              &FlyingSlig::M_UpToHorizontalMovement_13_438F60,
              &FlyingSlig::M_DownToHorizontalMovement_14_438BF0,
              &FlyingSlig::M_QuickTurn_15_4387F0,
              &FlyingSlig::M_IdleToHorizontalMovement_16_438730,
              &FlyingSlig::M_BeginDownMovement_17_438B80,
              &FlyingSlig::M_EndDownMovement_18_438C90,
              &FlyingSlig::M_DownKnockback_19_4390D0,
              &FlyingSlig::M_UpKnockback_20_439110,
              &FlyingSlig::M_EndUpMovement_21_438EB0,
              &FlyingSlig::M_InstantUpXTurn_22_438EE0,
              &FlyingSlig::M_InstantDownXTurn_23_438F20,
              &FlyingSlig::M_HorizontalToUpMovement_24_438D60,
              &FlyingSlig::M_TurnToHorizontalMovement_25_4389E0,
          });

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
    field_29C_brain_state = fn;
}

bool FlyingSlig::BrainIs(TFlyingSligBrainFn fn)
{
    return field_29C_brain_state == fn;
}

FlyingSlig::FlyingSlig(relive::Path_FlyingSlig* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(9)
{
    field_178_unused = 0;
    field_164_unused = -1;
    field_17A_unused = -1;

    if (tlvId != Guid{})
    {
        mBaseGameObjectTlvInfo = tlvId;
    }

    SetType(ReliveTypes::eFlyingSlig);

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kFlySligResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, TRUE, FALSE));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, TRUE, FALSE));
    field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, TRUE, FALSE));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, TRUE, FALSE));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, TRUE, FALSE));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, TRUE, FALSE));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kVaporResID, TRUE, FALSE));
    field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, TRUE, FALSE));

    const AnimRecord& rec = AnimRec(AnimId::FlyingSlig_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::FlyingSlig_Idle, ppRes);

    field_15E_useless = 0;

    mAnim.mFnPtrArray = kFlyingSlig_Anim_Frames_Fns_55EFC4;

    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    field_14C_timer = 0;
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    field_15C_voice_pitch_min = 45 * ((Math_NextRandom() % 5) - 2);

    field_150_grenade_delay = 0;
    field_154_collision_reaction_timer = 0;

    field_17E_flags.Clear(Flags_17E::eBit1_Speaking_flag1);
    field_17E_flags.Clear(Flags_17E::eBit5_Throw);
    field_17E_flags.Clear(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
    field_17E_flags.Clear(Flags_17E::eBit7_DoAction);
    field_17E_flags.Clear(Flags_17E::eBit8_Unused);
    field_17E_flags.Clear(Flags_17E::eBit9_Chanting);
    field_17E_flags.Clear(Flags_17E::eBit10_Speaking_flag2);

    field_158_obj_id = Guid{};

    field_288_unused = 0;
    field_290_bobbing_values_index = 0;
    field_284_bobbing_value = FP_FromInteger(0);
    field_28C_bobbing_values_table_index = 0;
    field_1E8_unused = 0;
    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    field_118_data = *pTlv;

    field_148_tlvInfo = tlvId;

    SetBrain(&FlyingSlig::Brain_0_Inactive);

    mCurrentMotion = eFlyingSligMotions::M_Idle_0_4385E0;

    if (field_118_data.mSpawnDelayState == relive::Path_FlyingSlig::SpawnDelayStates::eUseCustomSpawnMoveDelay)
    {
        field_14C_timer = sGnFrame + field_118_data.mSpawnMoveDelay;
    }
    else if (field_118_data.mSpawnDelayState == relive::Path_FlyingSlig::SpawnDelayStates::eMoveImmediately)
    {
        field_14C_timer = sGnFrame + 1;
    }

    field_2A8_max_x_speed = FP_FromInteger(field_118_data.mMaxVelocity) * mSpriteScale;
    field_2AC_up_vel = FP_FromInteger(-field_118_data.mMaxVelocity) * mSpriteScale;
    field_2B0_down_vel = FP_FromInteger(field_118_data.mMaxVelocity) * mSpriteScale;
    field_2B4_max_slow_down = FP_FromDouble(0.4) * mSpriteScale;
    field_2B8_max_speed_up = FP_FromDouble(0.4) * mSpriteScale;

    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, field_118_data.mFacing == relive::reliveXDirection::eLeft);

    if (field_118_data.mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFartsBat_Half_14;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFartsBats_33;
        mScale = Scale::Fg;
    }

    field_17E_flags.Set(Flags_17E::eBit13_Persistant, field_118_data.mPersistant == relive::reliveChoice::eYes);

    field_17C_launch_switch_id |= field_118_data.mLaunchGrenadeSwitchId;

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    FP hitX = {};
    FP hitY = {};
    sCollisions->Raycast(
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

    field_1D8_unused = 0;
    field_1DC_unused = 0;
    field_1E0_unused = 0;
    field_1E4_unused = 0;

    mShadow = relive_new Shadow();
}

const AnimId sFlyingSligAnimIdTable[28] = {
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

s32 FlyingSlig::CreateFromSaveState(const u8* pBuffer)
{
    auto pSaveState = reinterpret_cast<const FlyingSlig_State*>(pBuffer);

    auto pTlv = static_cast<relive::Path_FlyingSlig*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->field_3C_tlvInfo));
    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kFlySligResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("FLYSLIG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBLOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("GRENADE.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SMEXP.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("METAL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("BIGFLASH.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kVaporResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("VAPOR.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("DOGBLOW.BAN", nullptr);
    }

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
        pFlyingSlig->mCurrentLevel = MapWrapper::FromAESaveData(pSaveState->field_16_lvl_number);
        pFlyingSlig->mSpriteScale = pSaveState->field_18_sprite_scale;

        pFlyingSlig->field_27C_r = pSaveState->field_1C_oldr;
        pFlyingSlig->field_27E_g = pSaveState->field_1E_oldg;
        pFlyingSlig->field_280_b = pSaveState->field_20_oldb;

        pFlyingSlig->mRGB.SetRGB(pSaveState->field_1C_oldr, pSaveState->field_1E_oldg, pSaveState->field_20_oldb);

        pFlyingSlig->mCurrentMotion = pSaveState->field_24_current_state;

        u8** ppRes = pFlyingSlig->ResBlockForMotion_4350F0(pSaveState->field_24_current_state);
        pFlyingSlig->mAnim.Set_Animation_Data(sFlyingSligAnimIdTable[pFlyingSlig->mCurrentMotion], ppRes);

        pFlyingSlig->mAnim.mCurrentFrame = pSaveState->field_26_current_frame;

        pFlyingSlig->mAnim.mFrameChangeCounter = pSaveState->field_28_frame_change_counter;

        pFlyingSlig->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pSaveState->field_2A_bAnimRender & 1);
        pFlyingSlig->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, pSaveState->field_22_bAnimFlipX & 1);
        pFlyingSlig->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pSaveState->field_2B_bDrawable & 1);

        if (IsLastFrame(&pFlyingSlig->mAnim))
        {
            pFlyingSlig->mAnim.mFlags.Set(AnimFlags::eBit18_IsLastFrame);
        }

        pFlyingSlig->mHealth = pSaveState->field_2C_current_health;
        pFlyingSlig->mCurrentMotion = pSaveState->field_30_current_state;
        pFlyingSlig->mNextMotion = pSaveState->field_32_delayed_state;
        pFlyingSlig->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pSaveState->field_34_lastLineYPos);
        pFlyingSlig->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
        pFlyingSlig->BaseAliveGameObjectCollisionLineType = -1;

        if (pSaveState->field_36_line_idx != -1)
        {
            pFlyingSlig->BaseAliveGameObjectCollisionLineType = pSaveState->field_36_line_idx;
        }

        if (pSaveState->field_3A.Get(FlyingSlig_State::eBit1_bPossessed))
        {
            sControlledCharacter = pFlyingSlig;
            pFlyingSlig->field_2A8_max_x_speed = (FP_FromDouble(5.5) * pFlyingSlig->mSpriteScale);
            pFlyingSlig->field_2AC_up_vel = (FP_FromDouble(-5.5) * pFlyingSlig->mSpriteScale);
            pFlyingSlig->field_2B0_down_vel = (FP_FromDouble(5.5) * pFlyingSlig->mSpriteScale);
            pFlyingSlig->field_2B4_max_slow_down = (FP_FromDouble(0.25) * pFlyingSlig->mSpriteScale);
            pFlyingSlig->field_2B8_max_speed_up = (FP_FromDouble(0.7) * pFlyingSlig->mSpriteScale);
        }

        pFlyingSlig->field_17C_launch_switch_id = pSaveState->field_38_launch_switch_id;


        pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit5_Throw, pSaveState->field_3A.Get(FlyingSlig_State::eBit2_Throw));
        pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit6_bAlertedAndNotFacingAbe, pSaveState->field_3A.Get(FlyingSlig_State::eBit3_bAlertedAndNotFacingAbe));
        pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit7_DoAction, pSaveState->field_3A.Get(FlyingSlig_State::eBit4_DoAction));
        pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit9_Chanting, pSaveState->field_3A.Get(FlyingSlig_State::eBit5_Chanting));
        pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit10_Speaking_flag2, pSaveState->field_3A.Get(FlyingSlig_State::eBit6_Speaking_flag2));
        pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit1_Speaking_flag1, pSaveState->field_3A.Get(FlyingSlig_State::eBit7_Speaking_flag1));
        pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit2_bLastLine, pSaveState->field_3A.Get(FlyingSlig_State::eBit8_bLastLine));
        pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit3, pSaveState->field_3A.Get(FlyingSlig_State::eBit9));
        pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit4, pSaveState->field_3A.Get(FlyingSlig_State::eBit10));


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
        pFlyingSlig->field_1D8_unused = pSaveState->field_78_unused;
        pFlyingSlig->field_1DC_unused = pSaveState->field_7C_unused;
        pFlyingSlig->field_1E0_unused = pSaveState->field_80_unused;
        pFlyingSlig->field_1E4_unused = pSaveState->field_84_unused;
        pFlyingSlig->field_294_nextXPos = pSaveState->field_88_nextXPos;
        pFlyingSlig->field_298_nextYPos = pSaveState->field_8C_nextYPos;
        pFlyingSlig->SetBrain(sFlyingSligBrainTable[pSaveState->field_90_fns1_idx]);
        pFlyingSlig->field_1E8_unused = pSaveState->field_98_unused;

        pFlyingSlig->field_2A0_abe_level = MapWrapper::FromAESaveData(pSaveState->field_9A_abe_level);
        pFlyingSlig->field_2A2_abe_path = pSaveState->field_9C_abe_path;
        pFlyingSlig->field_2A4_abe_camera = pSaveState->field_9E_abe_camera;

        pFlyingSlig->field_290_bobbing_values_index = pSaveState->field_A4_bobbing_values_index;
        pFlyingSlig->field_284_bobbing_value = pSaveState->field_A8_bobbing_value;
        pFlyingSlig->field_28C_bobbing_values_table_index = pSaveState->field_A0_bobbing_values_table_index;
    }

    return sizeof(FlyingSlig_State);
}

s32 FlyingSlig::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<FlyingSlig_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eFlyingSlig_54;

    pState->field_4_xpos = mXPos;
    pState->field_8_ypos = mYPos;
    pState->field_C_velx = mVelX;
    pState->field_10_vely = mVelY;

    pState->field_14_path_number = mCurrentPath;
    pState->field_16_lvl_number = MapWrapper::ToAE(mCurrentLevel);
    pState->field_18_sprite_scale = mSpriteScale;

    pState->field_1C_oldr = mRGB.r;
    pState->field_1E_oldg = mRGB.g;
    pState->field_20_oldb = mRGB.b;

    pState->field_22_bAnimFlipX = mAnim.mFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_24_current_state = mCurrentMotion;
    pState->field_26_current_frame = mAnim.mCurrentFrame;
    pState->field_28_frame_change_counter = mAnim.mFrameChangeCounter;

    pState->field_2B_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2A_bAnimRender = mAnim.mFlags.Get(AnimFlags::eBit3_Render);
    pState->field_2C_current_health = mHealth;
    pState->field_30_current_state = mCurrentMotion;
    pState->field_32_delayed_state = mNextMotion;

    pState->field_34_lastLineYPos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);

    pState->field_36_line_idx = -1; // OG bug - actually becomes 0 due to impossible case removed below ?

    if (BaseAliveGameObjectCollisionLine)
    {
        pState->field_36_line_idx = static_cast<s16>(BaseAliveGameObjectCollisionLine - sCollisions->field_0_pArray);
    }

    pState->field_38_launch_switch_id = field_17C_launch_switch_id;

    pState->field_3A.Set(FlyingSlig_State::eBit1_bPossessed, this == sControlledCharacter);
    pState->field_3A.Set(FlyingSlig_State::eBit2_Throw, field_17E_flags.Get(Flags_17E::eBit5_Throw));
    pState->field_3A.Set(FlyingSlig_State::eBit3_bAlertedAndNotFacingAbe, field_17E_flags.Get(Flags_17E::eBit6_bAlertedAndNotFacingAbe));
    pState->field_3A.Set(FlyingSlig_State::eBit4_DoAction, field_17E_flags.Get(Flags_17E::eBit7_DoAction));
    pState->field_3A.Set(FlyingSlig_State::eBit5_Chanting, field_17E_flags.Get(Flags_17E::eBit9_Chanting));
    pState->field_3A.Set(FlyingSlig_State::eBit6_Speaking_flag2, field_17E_flags.Get(Flags_17E::eBit10_Speaking_flag2));
    pState->field_3A.Set(FlyingSlig_State::eBit7_Speaking_flag1, field_17E_flags.Get(Flags_17E::eBit1_Speaking_flag1));
    pState->field_3A.Set(FlyingSlig_State::eBit8_bLastLine, field_17E_flags.Get(Flags_17E::eBit2_bLastLine));
    pState->field_3A.Set(FlyingSlig_State::eBit9, field_17E_flags.Get(Flags_17E::eBit3));
    pState->field_3A.Set(FlyingSlig_State::eBit10, field_17E_flags.Get(Flags_17E::eBit4));

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
    pState->field_78_unused = field_1D8_unused;
    pState->field_7C_unused = field_1DC_unused;
    pState->field_80_unused = field_1E0_unused;
    pState->field_84_unused = field_1E4_unused;
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

    pState->field_98_unused = field_1E8_unused;

    pState->field_9A_abe_level = MapWrapper::ToAE(field_2A0_abe_level);
    pState->field_9C_abe_path = field_2A2_abe_path;
    pState->field_9E_abe_camera = field_2A4_abe_camera;

    pState->field_A4_bobbing_values_index = field_290_bobbing_values_index;
    pState->field_A8_bobbing_value = field_284_bobbing_value;
    pState->field_A0_bobbing_values_table_index = field_28C_bobbing_values_table_index;

    return sizeof(FlyingSlig_State);
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
                field_2A0_abe_level,
                field_2A2_abe_path,
                field_2A4_abe_camera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    relive::Path_TLV* pTlv = sPathInfo->TLV_From_Offset_Lvl_Cam(field_148_tlvInfo);
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
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mOverlayId != gMap.GetOverlayId() || (gMap.mCurrentPath != gMap.mNextPath && (this != sControlledCharacter || field_17E_flags.Get(Flags_17E::eBit13_Persistant))))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void FlyingSlig::VUpdate()
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);

        if (!IsPossessed())
        {
            if (BaseAliveGameObjectCollisionLineType != -1)
            {
                const FP savedNextXPos = field_294_nextXPos;
                const FP savedNextYPos = field_298_nextYPos;
                // Everything else uses this a a line type, but of course MrSlig uses it as an index
                BaseAliveGameObjectCollisionLine = sCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLineType);
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        mXPos = field_294_nextXPos;
        mYPos = field_298_nextYPos;

        (this->*(field_29C_brain_state))();

        (this->*(sFlyingSlig_motion_table_5523A0)[mCurrentMotion])();

        Movement();
    }
}

void FlyingSlig::VRender(PrimHeader** ot)
{
    const s16 eyeIndices[] = {60, 62};
    renderWithGlowingEyes(ot, this, &field_1F4_pPalAlloc[0], ALIVE_COUNTOF(field_1F4_pPalAlloc),
                          &field_274_pal_rect, field_27C_r, field_27E_g, field_280_b, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

void FlyingSlig::sub_4348A0()
{
    field_17E_flags.Clear(Flags_17E::eBit3);
    field_17E_flags.Clear(Flags_17E::eBit2_bLastLine);
    field_298_nextYPos = mYPos;
    field_294_nextXPos = mXPos;
    field_18C = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);
    sub_437C70(BaseAliveGameObjectCollisionLine);
    const s16 v5 = FP_GetExponent(mYPos - field_1A4_rect.y);
    const s16 v6 = FP_GetExponent(mXPos - field_1A4_rect.x);
    field_194 = FP_FromInteger(Math_SquareRoot_Int_496E70(v5 * v5 + v6 * v6));
    field_17E_flags.Set(Flags_17E::eBit4, field_118_data.mFacing == relive::reliveXDirection::eLeft);
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



ALIVE_ARY(1, 0x55257C, const s32*, 4, sBobbingValuesTables_55257C, {nullptr, sBobbingValuesHorizontalMovement_552500, sBobbingValuesIdle_552524, sBobbingValuesTurning_552550});

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
        const s32 newVel = Math_SquareRoot_Int_496E70(FP_GetExponent((mVelY * mVelY) + (mVelX * mVelX)));
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

        const FP v15 = FP_FromInteger(Math_SquareRoot_Int_496E70(FP_GetExponent((mVelY * mVelY) + (mVelX * mVelX))));
        if (v15 > field_2A8_max_x_speed)
        {
            mVelX = ((mVelX / v15) * field_2A8_max_x_speed);
            mVelY = ((mVelY / v15) * field_2A8_max_x_speed);
        }

        if (mVelY < FP_FromInteger(0))
        {
            if (CollisionUp_43A640(mVelY))
            {
                mVelY = -mVelY;
            }
        }

        if (mVelY > FP_FromInteger(0))
        {
            if (CollisionDown_43A9E0(mVelY))
            {
                mVelY = -mVelY;
            }
        }

        if (mVelX != FP_FromInteger(0))
        {
            if (CollisionLeftRight_43AC80(mVelX))
            {
                mVelX = -mVelX;
            }
        }
    }

    mXPos += mVelX;
    mYPos += mVelY;

    SetActiveCameraDelayedFromDir();

    if (field_17E_flags.Get(Flags_17E::eBit5_Throw))
    {
        if (static_cast<s32>(sGnFrame) > field_150_grenade_delay && 
            (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit4_bPossesed) || SwitchStates_Get(field_17C_launch_switch_id) ||
             mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit4_bPossesed)) && CanThrowGrenade_43A490())
        {
            ThrowGrenade_43A1E0();
        }
    }

    if (field_17E_flags.Get(Flags_17E::eBit10_Speaking_flag2))
    {
        VSetMotion(eFlyingSligMotions::M_GameSpeak_8_4391D0);
    }

    if (field_17E_flags.Get(Flags_17E::eBit6_bAlertedAndNotFacingAbe))
    {
        VSetMotion(eFlyingSligMotions::M_IdleToTurn_2_4388B0);
    }

    if (field_17E_flags.Get(Flags_17E::eBit7_DoAction))
    {
        if (mCurrentMotion == eFlyingSligMotions::M_Idle_0_4385E0)
        {
            TryPullLever_439DB0();
        }
    }

    field_294_nextXPos = mXPos;
    field_298_nextYPos = mYPos;

    mYPos += mSpriteScale * FP_FromInteger(20);

    if (field_28C_bobbing_values_table_index)
    {
        if (field_28C_bobbing_values_table_index > 3)
        {
            ALIVE_FATAL("FlyingSlig array out of bounds");
        }
        const FP* pTable = reinterpret_cast<const FP*>(sBobbingValuesTables_55257C[field_28C_bobbing_values_table_index]); // TODO: Type conversion !!
        field_288_unused = pTable;
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
        Slig_SoundEffect_4BFFE0((Math_NextRandom() % 3) ? SligSfx::ePropeller2_10 : SligSfx::ePropeller1_9, this);
    }

    field_17E_flags.Clear(Flags_17E::eBit5_Throw);
    field_17E_flags.Clear(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
    field_17E_flags.Clear(Flags_17E::eBit7_DoAction);
    field_17E_flags.Clear(Flags_17E::eBit10_Speaking_flag2);

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    field_190 = FP_FromInteger(0);
}

s16 FlyingSlig::VTakeDamage(BaseGameObject* pFrom)
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
                    pTlv = sPathInfo->TlvGetAt(pTlv,
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
                                return 0;
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
                return 1;
            }
            BlowUp_436510();
            auto pExplosion = relive_new Explosion(mXPos, mYPos - (mSpriteScale * FP_FromInteger(5)), mSpriteScale, 1);
            if (!pExplosion)
            {
                return 1;
            }
            return 1;
        }

        case ReliveTypes::eElectricWall:
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_15C_voice_pitch_min, this);
            break;

        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eExplosion:
            if (!BrainIs(&FlyingSlig::Brain_1_Death))
            {
                BlowUp_436510();
            }
            break;

        case ReliveTypes::eElectrocute:
            if (!BrainIs(&FlyingSlig::Brain_1_Death))
            {
                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
                mHealth = FP_FromInteger(0);
                SetBrain(&FlyingSlig::Brain_1_Death);
                field_14C_timer = sGnFrame;
            }
            break;

        default:
            return 1;
    }
    return 1;
}

void FlyingSlig::Brain_0_Inactive()
{
    if (static_cast<s32>(sGnFrame) >= field_14C_timer)
    {
        ToMoving_435720();
    }
}

void FlyingSlig::Brain_1_Death()
{
    if (static_cast<s32>(sGnFrame) >= field_14C_timer)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        EventBroadcast(kEventMudokonComfort, this);
    }
}

void FlyingSlig::Brain_2_Moving()
{
    if (!sub_436730() && sub_4374A0(1) == 1)
    {
        field_17E_flags.Toggle(Flags_17E::eBit4);
        PatrolDelay_435860();
    }
}

void FlyingSlig::Brain_3_GetAlerted()
{
    if (mCurrentMotion != eFlyingSligMotions::M_Idle_0_4385E0)
    {
        field_14C_timer++;
    }
    else if (VIsFacingMe(sControlledCharacter))
    {
        if (!sub_436730() && static_cast<s32>(sGnFrame) >= field_14C_timer)
        {
            ToMoving_435720();
        }
    }
    else
    {
        field_17E_flags.Set(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
        field_14C_timer++;
    }
}

void FlyingSlig::Brain_4_ChasingEnemy()
{
    field_17E_flags.Clear(Flags_17E::eBit3);

    if (EventGet(kEventHeroDying) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        ToAbeDead_436010();
        return;
    }

    if (EventGet(kEventResetting) || sControlledCharacter->mSpriteScale != mSpriteScale || IsInInvisibleZone(sControlledCharacter) || sControlledCharacter->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) || (!IsWallBetween_43A550(this, sControlledCharacter) && (!IsActiveHero(sControlledCharacter) || sActiveHero->mCurrentMotion != eAbeMotions::Motion_65_LedgeAscend_4548E0) && sControlledCharacter->Type() != ReliveTypes::eMineCar))
    {
        PatrolDelay_435860();
        return;
    }

    if (sub_436C60(&BaseAliveGameObjectCollisionLine->mRect, 1))
    {
        if (FP_Abs(field_194 - field_1C4) < (FP_FromInteger(15) * mSpriteScale))
        {
            ToLaunchingGrenade_435F50();
            return;
        }

        field_17E_flags.Set(Flags_17E::eBit4, field_1C4 > field_194);
        field_17E_flags.Set(Flags_17E::eBit3);

        if (static_cast<s32>(sGnFrame) > field_150_grenade_delay && CanThrowGrenade_43A490())
        {
            if (VIsFacingMe(sControlledCharacter))
            {
                if (!(Math_NextRandom() & 15))
                {
                    ToLaunchingGrenade_435F50();
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
        ToMoving_435720();
    }
}

void FlyingSlig::Brain_6_GameSpeakToMoving()
{
    if (mCurrentMotion != eFlyingSligMotions::M_GameSpeak_8_4391D0)
    {
        ToMoving_435720();
    }
}

void FlyingSlig::Brain_7_PanicMoving()
{
    if (CanChase_436850(sControlledCharacter))
    {
        ToChase_435E10();
        return;
    }

    if (!IsEventInRange(kEventAbeOhm, mXPos, mYPos, AsEventScale(mScale)))
    {
        ToMoving_435720();
        return;
    }

    if (static_cast<s32>(sGnFrame) < field_14C_timer)
    {
        if (sub_4374A0(0) != 1)
        {
            return;
        }
        field_17E_flags.Toggle(Flags_17E::eBit4);
    }

    ToPanicIdle_435B50();
}

void FlyingSlig::Brain_8_PanicIdle()
{
    if (CanChase_436850(sControlledCharacter))
    {
        ToChase_435E10();
    }
    else if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, AsEventScale(mScale)))
    {
        if (static_cast<s32>(sGnFrame) >= field_14C_timer && mCurrentMotion != eFlyingSligMotions::M_GameSpeak_8_4391D0)
        {
            ToPanicMoving_435A50();
        }
    }
    else
    {
        ToMoving_435720();
    }
}

void FlyingSlig::Brain_9_SpottedEnemy()
{
    if (static_cast<s32>(sGnFrame) >= field_14C_timer)
    {
        ToChase_435E10();
    }
}

void FlyingSlig::Brain_10_LaunchingGrenade()
{
    if (VIsFacingMe(sControlledCharacter))
    {
        field_17E_flags.Set(Flags_17E::eBit5_Throw);
    }
    ToChase_435E10();
}

void FlyingSlig::Brain_11_AbeDead()
{
    if (static_cast<s32>(sGnFrame) == field_14C_timer - 45)
    {
        Say_436A50(SligSpeak ::eGotYa_12, 0);
    }
    else if (static_cast<s32>(sGnFrame) == field_14C_timer)
    {
        Say_436A50(SligSpeak::eLaugh_3, 0);
    }
}

void FlyingSlig::Brain_12_Possessed()
{
    if (sControlledCharacter == this && mHealth > FP_FromInteger(0))
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    if (!field_17E_flags.Get(Flags_17E::eBit9_Chanting))
    {
        if (Input_IsChanting_45F260())
        {
            ToChantShake_436270();
        }
    }

    if (!Input_IsChanting_45F260())
    {
        field_17E_flags.Clear(Flags_17E::eBit9_Chanting);
    }

    HandlePlayerControls_439340();
}

void FlyingSlig::Brain_13_Possession()
{
    if (static_cast<s32>(sGnFrame) > field_14C_timer)
    {
        Say_436A50(SligSpeak ::eHi_0, 0x1FF);
        ToPlayerControlled_4360C0();
    }
}

void FlyingSlig::Brain_14_DePossession()
{
    if (!Input_IsChanting_45F260())
    {
        ToPlayerControlled_4360C0();
        return;
    }

    if (static_cast<s32>(sGnFrame) <= field_14C_timer)
    {
        if (!(static_cast<s32>(sGnFrame) % 4))
        {
            const FP xOff = (mSpriteScale * FP_FromInteger(Math_RandomRange(-20, 20) + (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) ? -10 : 10)));
            const FP yOff = (mSpriteScale * FP_FromInteger(Math_RandomRange(-20, 10)));
            New_TintChant_Particle(
                xOff + mXPos,
                yOff + mYPos,
                mSpriteScale,
                Layer::eLayer_0);
        }
    }
    else
    {
        BlowUp_436510();
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
            VSetMotion(eFlyingSligMotions::M_LeverPull_7_439150);
        }
    }
    else if (mCurrentMotion == eFlyingSligMotions::M_Idle_0_4385E0)
    {
        ToPlayerControlled_4360C0();
    }
}

void FlyingSlig::Brain_17_FromCrawlingSlig()
{
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
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
        PatrolDelay_435860();
    }
    else
    {
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromDouble(-4.5);
        field_184_xSpeed = FP_FromInteger(0);
        field_188_ySpeed = FP_FromDouble(-0.5);
    }
}

void FlyingSlig::M_Idle_0_4385E0()
{
    if (!field_28C_bobbing_values_table_index)
    {
        field_28C_bobbing_values_table_index = 2;
        field_290_bobbing_values_index = 0;
    }

    if (field_184_xSpeed != FP_FromInteger(0))
    {
        if (IsFacingMovementDirection_43A510())
        {
            VSetMotion(eFlyingSligMotions::M_IdleToHorizontalMovement_16_438730);
            field_28C_bobbing_values_table_index = 1;
            field_290_bobbing_values_index = 0;
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_IdleToTurn_2_4388B0);
        }
    }
    else
    {
        if (field_188_ySpeed > FP_FromInteger(0))
        {
            if (field_188_ySpeed >= FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_BeginDownMovement_17_438B80);
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_BeginUpMovement_11_438E40);
            }
        }
    }
}

void FlyingSlig::M_HorizontalMovement_1_4386A0()
{
    if (field_188_ySpeed < FP_FromInteger(0))
    {
        VSetMotion(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
    }
    else if (field_188_ySpeed > FP_FromInteger(0))
    {
        VSetMotion(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
    }
    else if (field_184_xSpeed == FP_FromInteger(0))
    {
        VSetMotion(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
    }
    else if (!IsFacingMovementDirection_43A510())
    {
        VSetMotion(eFlyingSligMotions::M_QuickTurn_15_4387F0);
        field_28C_bobbing_values_table_index = 3;
        field_290_bobbing_values_index = 0;
    }
}

void FlyingSlig::M_IdleToTurn_2_4388B0()
{
    if (mAnim.mCurrentFrame == 4)
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
            if (field_188_ySpeed >= FP_FromInteger(0))
            {
                if (field_188_ySpeed <= FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection_43A510())
                    {
                        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
                        VSetMotion(eFlyingSligMotions::M_TurnToHorizontalMovement_25_4389E0);
                    }
                    else
                    {
                        VSetMotion(eFlyingSligMotions::M_QuickTurn_15_4387F0);
                    }
                }
                else
                {
                    VSetMotion(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                    field_28C_bobbing_values_table_index = 0;
                }
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
                field_28C_bobbing_values_table_index = 0;
            }
        }
    }
    else if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_BeginDownMovement_17_438B80);
            }
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_BeginUpMovement_11_438E40);
        }
    }
}

void FlyingSlig::M_DownMovement_3_438AA0()
{
    if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection_43A510())
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            if (field_184_xSpeed != FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0);
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            }
        }
    }
    else
    {
        VSetMotion(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
    }
}

void FlyingSlig::M_DownMovementToTurn_4_438CC0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection_43A510())
        {
            if (field_188_ySpeed > FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_DownMovement_3_438AA0);
            }
            else if (field_184_xSpeed != FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0);
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            }
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
        }
    }
}

void FlyingSlig::M_UpMovement_5_438DD0()
{
    if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection_43A510())
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_184_xSpeed != FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60);
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            }
        }
    }
    else
    {
        VSetMotion(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
    }
}

void FlyingSlig::M_UpMovementToTurn_6_439030()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection_43A510())
        {
            if (field_188_ySpeed < FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_UpMovement_5_438DD0);
            }
            else if (field_184_xSpeed != FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60);
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            }
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
        }
    }
}

void FlyingSlig::M_LeverPull_7_439150()
{
    if (mAnim.mCurrentFrame)
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
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

void FlyingSlig::M_GameSpeak_8_4391D0()
{
    if (mAnim.mCurrentFrame == 1 && field_17E_flags.Get(Flags_17E::eBit1_Speaking_flag1))
    {
        field_17E_flags.Clear(Flags_17E::eBit1_Speaking_flag1);

        if (BrainIs(&FlyingSlig::Brain_12_Possessed))
        {
            switch (field_17D_next_speak)
            {
                case SligSpeak::eHi_0:
                    pEventSystem_5BC11C->PushEvent(GameSpeakEvents::Slig_Hi_27);
                    break;
                case SligSpeak::eHereBoy_1:
                    pEventSystem_5BC11C->PushEvent(GameSpeakEvents::Slig_HereBoy_28);
                    break;
                case SligSpeak::eGetHim_2:
                    pEventSystem_5BC11C->PushEvent(GameSpeakEvents::Slig_GetEm_29);
                    break;
                case SligSpeak::eLaugh_3:
                    pEventSystem_5BC11C->PushEvent(GameSpeakEvents::Slig_Laugh_8);
                    break;
                case SligSpeak::eBullshit_5:
                    pEventSystem_5BC11C->PushEvent(GameSpeakEvents::Slig_BS_5);
                    break;
                case SligSpeak::eLookOut_6:
                    pEventSystem_5BC11C->PushEvent(GameSpeakEvents::Slig_LookOut_6);
                    break;
                case SligSpeak::eBullshit2_7:
                    pEventSystem_5BC11C->PushEvent(GameSpeakEvents::Slig_BS2_7);
                    break;
                case SligSpeak::eFreeze_8:
                    pEventSystem_5BC11C->PushEvent(GameSpeakEvents::Slig_Freeze_31);
                    break;
                default:
                    break;
            }
        }
        Slig_GameSpeak_SFX_4C04F0(field_17D_next_speak, 0, field_160_voice_pitch_min, this);
        EventBroadcast(kEventSpeaking, this);
    }
    else if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        this->VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_Possession_9_434290()
{
    
}

void FlyingSlig::M_EndHorizontalMovement_10_4387D0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_BeginUpMovement_11_438E40()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184_xSpeed > FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection_43A510())
            {
                VSetMotion(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
                return;
            }
        }
        else if (field_188_ySpeed >= FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            return;
        }
        VSetMotion(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_HorizontalToDownMovement_12_438B10()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection_43A510())
            {
                VSetMotion(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
                return;
            }
        }
        else if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            return;
        }
        VSetMotion(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_UpToHorizontalMovement_13_438F60()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
        }
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (field_188_ySpeed > FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                field_28C_bobbing_values_table_index = 0;
            }
            else if (field_188_ySpeed < FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
                field_28C_bobbing_values_table_index = 0;
            }
            else if (field_184_xSpeed == FP_FromInteger(0))
            {
                VSetMotion(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
            }
            else if (IsFacingMovementDirection_43A510())
            {
                VSetMotion(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_QuickTurn_15_4387F0);
            }
        }
    }
}

void FlyingSlig::M_DownToHorizontalMovement_14_438BF0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                if (field_184_xSpeed > FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection_43A510())
                    {
                        VSetMotion(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
                    }
                    else
                    {
                        VSetMotion(eFlyingSligMotions::M_QuickTurn_15_4387F0);
                    }
                }
                else
                {
                    VSetMotion(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
                }
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                field_28C_bobbing_values_table_index = 0;
            }
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C_bobbing_values_table_index = 0;
        }
    }
}

void FlyingSlig::M_QuickTurn_15_4387F0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_188_ySpeed < FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_188_ySpeed > FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_184_xSpeed == FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
        }
        else if (!IsFacingMovementDirection_43A510())
        {
            VSetMotion(eFlyingSligMotions::M_QuickTurn_15_4387F0);
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
    }
}

void FlyingSlig::M_IdleToHorizontalMovement_16_438730()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed < FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_188_ySpeed > FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_184_xSpeed == FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
        }
        else if (IsFacingMovementDirection_43A510())
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_QuickTurn_15_4387F0);
        }
    }
}

void FlyingSlig::M_BeginDownMovement_17_438B80()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection_43A510())
            {
                VSetMotion(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
                return;
            }
        }
        else if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            return;
        }
        VSetMotion(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_EndDownMovement_18_438C90()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_BeginUpMovement_11_438E40);
        }
    }
}

void FlyingSlig::M_DownKnockback_19_4390D0()
{
    if (FP_Abs(field_184_xSpeed) > FP_FromDouble(0.1) || field_188_ySpeed < FP_FromDouble(0.1))
    {
        VSetMotion(eFlyingSligMotions::M_EndDownMovement_18_438C90);
    }
}

void FlyingSlig::M_UpKnockback_20_439110()
{
    if (FP_Abs(field_184_xSpeed) > FP_FromDouble(0.1) || field_188_ySpeed > FP_FromDouble(-0.1))
    {
        VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_EndUpMovement_21_438EB0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_BeginDownMovement_17_438B80);
        }
    }
}

void FlyingSlig::M_InstantUpXTurn_22_438EE0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
        VSetMotion(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_InstantDownXTurn_23_438F20()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
        VSetMotion(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_HorizontalToUpMovement_24_438D60()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection_43A510())
            {
                VSetMotion(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
                return;
            }
        }
        else if (field_188_ySpeed >= FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            return;
        }
        VSetMotion(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_TurnToHorizontalMovement_25_4389E0()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                if (field_184_xSpeed != FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection_43A510())
                    {
                        VSetMotion(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
                    }
                    else
                    {
                        VSetMotion(eFlyingSligMotions::M_QuickTurn_15_4387F0);
                    }
                }
                else
                {
                    VSetMotion(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
                }
            }
            else
            {
                VSetMotion(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                field_28C_bobbing_values_table_index = 0;
            }
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C_bobbing_values_table_index = 0;
        }
    }
}

s16 FlyingSlig::IsFacingMovementDirection_43A510()
{
    return ((field_184_xSpeed > FP_FromInteger(0) && !(mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)))
            || (field_184_xSpeed < FP_FromInteger(0) && mAnim.mFlags.Get(AnimFlags::eBit5_FlipX)));
}

void FlyingSlig::ToPlayerControlled_4360C0()
{
    VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
    SetBrain(&FlyingSlig::Brain_12_Possessed);
}

void FlyingSlig::ToMoving_435720()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    SetBrain(&FlyingSlig::Brain_2_Moving);
}

void FlyingSlig::ToPanicIdle_435B50()
{
    Say_436A50(SligSpeak::eHelp_10, 0);
    field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + field_118_data.mPanicDelay;
    SetBrain(&FlyingSlig::Brain_8_PanicIdle);
}

void FlyingSlig::ToChase_435E10()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    SetBrain(&FlyingSlig::Brain_4_ChasingEnemy);
}

s16 FlyingSlig::IsPossessed()
{
    return BrainIs(&FlyingSlig::Brain_12_Possessed) || BrainIs(&FlyingSlig::Brain_15_FlyingSligSpawn) || BrainIs(&FlyingSlig::Brain_17_FromCrawlingSlig) || BrainIs(&FlyingSlig::Brain_13_Possession) || BrainIs(&FlyingSlig::Brain_14_DePossession);
}

s16 FlyingSlig::CanChase_436850(BaseAliveGameObject* pObj)
{
    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) || !gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0) || EventGet(kEventResetting) || IsAbeEnteringDoor_43B030(pObj) || sActiveHero->mSpriteScale != mSpriteScale || !IsWallBetween_43A550(this, pObj))
    {
        return 0;
    }

    if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, AsEventScale(mScale)))
    {
        return 1;
    }

    if (VIsFacingMe(pObj) && !IsInInvisibleZone(pObj) && !pObj->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        return 1;
    }

    return 0;
}

void FlyingSlig::Say_436A50(SligSpeak speak, s16 pitch)
{
    if (mCurrentMotion != eFlyingSligMotions::M_GameSpeak_8_4391D0)
    {
        field_17E_flags.Set(Flags_17E::eBit1_Speaking_flag1);
        field_17E_flags.Set(Flags_17E::eBit10_Speaking_flag2);
        field_17D_next_speak = speak;
        field_160_voice_pitch_min = pitch;
    }
}

s16 FlyingSlig::sub_4374A0(s16 a2)
{
    field_17E_flags.Clear(Flags_17E::eBit2_bLastLine);

    if (field_17E_flags.Get(Flags_17E::eBit4))
    {
        if (!field_17E_flags.Get(Flags_17E::eBit12_bNoNextLine) && (field_182_bound1 == ReliveTypes::eNone || !a2))
        {
            if (!field_17E_flags.Get(Flags_17E::eBit3))
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
                if (width == FP_FromInteger(0) && !field_17E_flags.Get(Flags_17E::eBit12_bNoNextLine))
                {
                    field_190 = field_2B8_max_speed_up;
                }
                else
                {
                    field_17E_flags.Set(Flags_17E::eBit2_bLastLine);

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
        if (!field_17E_flags.Get(Flags_17E::eBit11_bNoPrevLine) && (field_180_bound2 == ReliveTypes::eNone || !a2))
        {
            if (!field_17E_flags.Get(Flags_17E::eBit3))
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
                if (width == FP_FromInteger(0) && !field_17E_flags.Get(Flags_17E::eBit11_bNoPrevLine))
                {
                    field_190 = -field_2B8_max_speed_up;
                }
                else
                {
                    field_17E_flags.Set(Flags_17E::eBit2_bLastLine);
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

s16 FlyingSlig::IsAbeEnteringDoor_43B030(BaseAliveGameObject* pThis)
{
    return Slig::IsAbeEnteringDoor_4BB990(pThis);
}

bool FlyingSlig::IsWallBetween_43A550(BaseAliveGameObject* pThis, BaseAliveGameObject* pObj)
{
    // TODO: Duplicated like IsAbeEnteringDoor_4BB990 ??
    const PSX_RECT bRect = pObj->VGetBoundingRect();

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions->Raycast(
               pThis->mXPos,
               pThis->mYPos,
               pObj->mXPos,
               FP_FromInteger((bRect.y + bRect.h) / 2),
               &pLine,
               &hitX,
               &hitY,
               pThis->mScale == Scale::Fg ? kFgFloorWallOrCeiling : kBgFloorWallOrCeiling)
        != 1;
}

void FlyingSlig::ThrowGrenade_43A1E0()
{
    FP grenadeXVel = (FP_FromInteger(Math_RandomRange(50, 64)) / FP_FromInteger(10) * mSpriteScale);
    const FP grenadeYVel = (FP_FromInteger(-6) * mSpriteScale);

    FP grenadeXPos = (FP_FromInteger(0) * mSpriteScale);
    const FP grenadeYPos = (FP_FromInteger(-5) * mSpriteScale);

    const FP xpos = (FP_FromInteger(0) * mSpriteScale);
    const FP ypos = (FP_FromInteger(-20) * mSpriteScale);

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        grenadeXPos = -grenadeXPos;
        grenadeXVel = -grenadeXVel;
    }

    auto pGrenade = relive_new Grenade(grenadeXPos + mXPos, grenadeYPos + mYPos, 0, 1, 0, this);
    if (pGrenade)
    {
        pGrenade->mSpriteScale = mSpriteScale;
        pGrenade->mScale = mScale;
        pGrenade->VThrow(grenadeXVel, grenadeYVel);
    }

    New_ShootingFire_Particle(xpos + mXPos, ypos + mYPos, mAnim.mFlags.Get(AnimFlags::eBit5_FlipX), mSpriteScale);
    Slig_SoundEffect_4BFFE0(SligSfx::eThrowGrenade_8, this);
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
        Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHereBoy_1, 0, field_15C_voice_pitch_min, this);
    }
}

void FlyingSlig::BlowUp_436510()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);

    relive_new Gibs(GibType::Slig_1, mXPos, mYPos, mVelX, mVelY, mSpriteScale, 0);

    relive_new Blood(mXPos, mYPos - (FP_FromInteger(30) * mSpriteScale), FP_FromInteger(0), FP_FromInteger(0), mSpriteScale, 20);

    New_Smoke_Particles(mXPos, mYPos - (FP_FromInteger(30) * mSpriteScale), mSpriteScale, 3, 128u, 128u, 128u);
    SfxPlayMono(relive::SoundEffects::KillEffect, 128, mSpriteScale);
    SfxPlayMono(relive::SoundEffects::FallingItemHit, 90, mSpriteScale);

    mAnim.mFlags.Clear(AnimFlags::eBit2_Animate);
    mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

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
    if (CanChase_436850(sControlledCharacter))
    {
        ToSpottedEnemy_435E70();
        return 1;
    }
    else if (CanHearAbe_4369C0() && !BrainIs(&FlyingSlig::Brain_3_GetAlerted))
    {
        ToAlerted_4357E0();
        return 1;
    }
    else if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, AsEventScale(mScale)))
    {
        ToPanicMoving_435A50();
        return 1;
    }
    else
    {
        if (sActiveHero && sActiveHero->mHealth < FP_FromInteger(0) && FP_Abs(mXPos - sActiveHero->mXPos) < FP_FromInteger(640) && FP_Abs(mYPos - sActiveHero->mYPos) < FP_FromInteger(240))
        {
            ToAbeDead_436010();
            return 1;
        }
        return 0;
    }
}

s16 FlyingSlig::CanHearAbe_4369C0()
{
    return IsActiveHero(IsEventInRange(kEventSuspiciousNoise, mXPos, mYPos, AsEventScale(mScale))) || IsActiveHero(IsEventInRange(kEventSpeaking, mXPos, mYPos, AsEventScale(mScale)));
}

void FlyingSlig::ToSpottedEnemy_435E70()
{
    if (field_118_data.mPrechaseDelay)
    {
        Say_436A50(SligSpeak ::eFreeze_8, 0);
        field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + field_118_data.mPrechaseDelay;
        SetBrain(&FlyingSlig::Brain_9_SpottedEnemy);
    }
    else
    {
        Slig_GameSpeak_SFX_4C04F0(SligSpeak::eFreeze_8, 0, field_15C_voice_pitch_min, this);
        ToChase_435E10();
    }
}

void FlyingSlig::ToAbeDead_436010()
{
    SetBrain(&FlyingSlig::Brain_11_AbeDead);
    field_14C_timer = sGnFrame + 70;
}

void FlyingSlig::ToAlerted_4357E0()
{
    Say_436A50(SligSpeak ::eWhat_9, 0);
    SetBrain(&FlyingSlig::Brain_3_GetAlerted);
    field_14C_timer = sGnFrame + field_118_data.mAlertedListenTime;
}

void FlyingSlig::ToPanicMoving_435A50()
{
    MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + 12;
    SetBrain(&FlyingSlig::Brain_7_PanicMoving);
}

s16 FlyingSlig::IsTurning_436AE0()
{
    return mCurrentMotion == eFlyingSligMotions::M_IdleToTurn_2_4388B0 || mCurrentMotion == eFlyingSligMotions::M_QuickTurn_15_4387F0 || mCurrentMotion == eFlyingSligMotions::M_DownMovementToTurn_4_438CC0 || mCurrentMotion == eFlyingSligMotions::M_UpMovementToTurn_6_439030;
}

u8** FlyingSlig::ResBlockForMotion_4350F0(s32 /*motion*/)
{
    if (field_15E_useless == 0)
    {
        return field_10_resources_array.ItemAt(0);
    }
    else
    {
        field_15E_useless = 0;
        return field_10_resources_array.ItemAt(0);
    }
}

void FlyingSlig::ToChantShake_436270()
{
    SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
    VSetMotion(eFlyingSligMotions::M_Possession_9_434290);
    SetBrain(&FlyingSlig::Brain_14_DePossession);
    field_14C_timer = sGnFrame + 45;
}

void FlyingSlig::ToPossesed_436130()
{
    Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_15C_voice_pitch_min, this);
    VSetMotion(eFlyingSligMotions::M_Possession_9_434290);
    SetBrain(&FlyingSlig::Brain_13_Possession);
    field_14C_timer = sGnFrame + 35;
}

void FlyingSlig::vUpdateAnimRes_4350A0()
{
    u8** ppRes = ResBlockForMotion_4350F0(mCurrentMotion);
    mAnim.Set_Animation_Data(sFlyingSligAnimIdTable[mCurrentMotion], ppRes);
}

void FlyingSlig::PatrolDelay_435860()
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

s16 FlyingSlig::CanThrowGrenade_43A490()
{
    return mCurrentMotion == eFlyingSligMotions::M_Idle_0_4385E0 || mCurrentMotion == eFlyingSligMotions::M_HorizontalMovement_1_4386A0 || mCurrentMotion == eFlyingSligMotions::M_DownMovement_3_438AA0 || mCurrentMotion == eFlyingSligMotions::M_UpMovement_5_438DD0 || mCurrentMotion == eFlyingSligMotions::M_IdleToHorizontalMovement_16_438730 || mCurrentMotion == eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0 || mCurrentMotion == eFlyingSligMotions::M_BeginUpMovement_11_438E40 || mCurrentMotion == eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60 || mCurrentMotion == eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10 || mCurrentMotion == eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0 || mCurrentMotion == eFlyingSligMotions::M_EndDownMovement_18_438C90 || mCurrentMotion == eFlyingSligMotions::M_BeginDownMovement_17_438B80;
}

void FlyingSlig::ToLaunchingGrenade_435F50()
{
    if (!VIsFacingMe(sControlledCharacter) && !IsTurning_436AE0())
    {
        if (field_18C == FP_FromInteger(0))
        {
            field_17E_flags.Set(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
        }
        field_17E_flags.Toggle(Flags_17E::eBit4);
    }
    SetBrain(&FlyingSlig::Brain_10_LaunchingGrenade);
}

void FlyingSlig::HandlePlayerControls_439340()
{
    /* TODO OG issue - these local statics are never used ??
    if (!(byte_5BC5F4 & 1))
    {
        byte_5BC5F4 |= 1u;
        dword_5BC5F8 = Math_FixedPoint_Divide_496B70(0x800000, field_2B8);
    }
    if (!(byte_5BC5F4 & 2))
    {
        dword_5BC5F0 = 0x3F0000;
        byte_5BC5F4 |= 2u;
    }
    */

    field_17E_flags.Clear(Flags_17E::eBit5_Throw);
    field_17E_flags.Clear(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
    field_17E_flags.Clear(Flags_17E::eBit7_DoAction);

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    if (Input().isHeld(InputCommands::Enum::eThrowItem))
    {
        field_17E_flags.Set(Flags_17E::eBit5_Throw);
    }

    if (Input().isHeld(InputCommands::Enum::eDoAction))
    {
        field_17E_flags.Set(Flags_17E::eBit7_DoAction);
    }

    if (!field_17E_flags.Get(Flags_17E::eBit1_Speaking_flag1))
    {
        field_17E_flags.Set(Flags_17E::eBit1_Speaking_flag1);
        field_17E_flags.Set(Flags_17E::eBit10_Speaking_flag2);

        field_160_voice_pitch_min = 0;

        if (Input().isHeld(sInputKey_GameSpeak1))
        {
            field_17D_next_speak = SligSpeak::eHi_0;
        }
        else if (Input().isHeld(sInputKey_GameSpeak3))
        {
            field_17D_next_speak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak::eGetHim_2 : SligSpeak::eFreeze_8;
        }
        else if (Input().isHeld(sInputKey_GameSpeak4))
        {
            field_17D_next_speak = Input().Is_Demo_Playing_45F220() != 0 ? SligSpeak::eFreeze_8 : SligSpeak::eGetHim_2;
        }
        else if (Input().isHeld(sInputKey_GameSpeak2))
        {
            field_17D_next_speak = SligSpeak::eHereBoy_1;
        }
        else if (Input().isHeld(sInputKey_GameSpeak6))
        {
            field_17D_next_speak = SligSpeak::eBullshit_5;
        }
        else if (Input().isHeld(sInputKey_GameSpeak7))
        {
            field_17D_next_speak = SligSpeak::eLookOut_6;
        }
        else if (Input().isHeld(sInputKey_GameSpeak5))
        {
            field_17D_next_speak = SligSpeak::eBullshit2_7;
        }
        else if (Input().isHeld(sInputKey_GameSpeak8))
        {
            field_17D_next_speak = SligSpeak::eLaugh_3;
        }
        else
        {
            field_17E_flags.Clear(Flags_17E::eBit1_Speaking_flag1);
            field_17E_flags.Clear(Flags_17E::eBit10_Speaking_flag2);
        }
    }

    if (Input().isPressed(InputCommands::Enum::eUp))
    {
        if (Input().isPressed(InputCommands::Enum::eLeft))
        {
            field_184_xSpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            return;
        }
        if (Input().isPressed(InputCommands::Enum::eRight))
        {
            field_184_xSpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            return;
        }
        field_184_xSpeed = FP_FromInteger(0);
        field_188_ySpeed = -field_2B8_max_speed_up;
    }
    else if (Input().isPressed(InputCommands::Enum::eDown))
    {
        if (Input().isPressed(InputCommands::Enum::eLeft))
        {
            field_184_xSpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
        }
        else if (Input().isPressed(InputCommands::Enum::eRight))
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
    else if (Input().isPressed(InputCommands::Enum::eLeft))
    {
        field_188_ySpeed = FP_FromInteger(0);
        field_184_xSpeed = -field_2B8_max_speed_up;
    }
    else if (Input().isPressed(InputCommands::Enum::eRight))
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


    field_1EC_pNextLine = sCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLine->field_C_next);
    field_1F0_pPrevLine = sCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLine->field_A_previous);

    field_198_line_length = FP_FromInteger(BaseAliveGameObjectCollisionLine->field_12_line_length);

    field_17E_flags.Set(Flags_17E::eBit11_bNoPrevLine, field_1F0_pPrevLine == nullptr);
    field_17E_flags.Set(Flags_17E::eBit12_bNoNextLine, field_1EC_pNextLine == nullptr);

    field_182_bound1 = FindLeftOrRightBound_43B0A0(field_1A4_rect.w, field_1A4_rect.h);
    field_180_bound2 = FindLeftOrRightBound_43B0A0(field_1A4_rect.x, field_1A4_rect.y);
    field_1BC = Math_Tan_496F70(field_1A4_rect.y - field_1A4_rect.h, field_1A4_rect.w - field_1A4_rect.x);

    field_1C0 = field_1BC + FP_FromInteger(128);

    if (field_1C0 >= FP_FromInteger(256))
    {
        field_1C0 -= FP_FromInteger(256);
    }
    return 1;
}

ReliveTypes FlyingSlig::FindLeftOrRightBound_43B0A0(FP xOrY, FP wOrH)
{
    const FP kGridSize = ScaleToGridSize(mSpriteScale);

    const FP left = xOrY - kGridSize;
    const FP top = wOrH - kGridSize;
    const FP right = xOrY + kGridSize;
    const FP bottom = wOrH + kGridSize;

    // TODO: Check left is really Abs'd.
    ReliveTypes found_type = ReliveTypes::eNone;

    if (sPathInfo->TLV_Get_At_4DB4B0(FP_GetExponent(FP_Abs(left)), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), ReliveTypes::eSligBoundLeft))
    {
        found_type = ReliveTypes::eSligBoundLeft;
    }
    else if (sPathInfo->TLV_Get_At_4DB4B0(FP_GetExponent(left), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), ReliveTypes::eSligBoundRight))
    {
        found_type = ReliveTypes::eSligBoundRight;
    }
    return found_type;
}

void FlyingSlig::VPossessed()
{
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit4_bPossesed);
    field_17E_flags.Set(Flags_17E::eBit1_Speaking_flag1);

    field_2A0_abe_level = gMap.mCurrentLevel;
    field_2A2_abe_path = gMap.mCurrentPath;
    field_2A4_abe_camera = gMap.mCurrentCamera;

    field_2A8_max_x_speed = FP_FromDouble(5.5) * mSpriteScale;
    field_2AC_up_vel = FP_FromDouble(-5.5) * mSpriteScale;
    field_2B0_down_vel = FP_FromDouble(5.5) * mSpriteScale;
    field_2B4_max_slow_down = FP_FromDouble(0.25) * mSpriteScale;
    field_2B8_max_speed_up = FP_FromDouble(0.7) * mSpriteScale;

    ToPossesed_436130();
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
        const FP k80Scaled = (FP_FromInteger(80) * mSpriteScale);

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
            if ((sControlledCharacter->mYPos - (FP_FromInteger(60) * mSpriteScale) - yOff1) > (FP_FromInteger(35) * mSpriteScale))
            {
                FP hitX = {};
                FP hitY = {};
                PathLine* pLine = nullptr;

                if (sCollisions->Raycast(
                        sControlledCharacter->mXPos,
                        yOff1,
                        sControlledCharacter->mXPos,
                        yOff1 + (FP_FromInteger(35) * mSpriteScale),
                        &pLine,
                        &hitX,
                        &hitY,
                        mScale == Scale::Fg ? kFgFloor : kBgFloor)
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

            if ((sControlledCharacter->mYPos - (FP_FromInteger(60) * mSpriteScale) - yOff2) > (FP_FromInteger(35) * mSpriteScale))
            {
                if (sCollisions->Raycast(
                        sControlledCharacter->mXPos,
                        yOff2,
                        sControlledCharacter->mXPos,
                        yOff2 + (FP_FromInteger(35) * mSpriteScale),
                        &pLine,
                        &hitX,
                        &hitY,
                        mScale == Scale::Fg ? kFgFloor : kBgFloor)
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
                sqrt1 = FP_FromInteger(Math_SquareRoot_Int_496E70(
                    FP_GetExponent(yOff1 - field_1A4_rect.y) * (FP_GetExponent(yOff1 - field_1A4_rect.y)) + FP_GetExponent(rRight - field_1A4_rect.x) * (FP_GetExponent(rRight - field_1A4_rect.x))));
            }

            if (bLeftInRect)
            {
                const s32 sqrt2_int = Math_SquareRoot_Int_496E70(
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
        const FP k40Scaled = (FP_FromInteger(40) * mSpriteScale);
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

        if (FP_Abs(sControlledCharacter->mXPos - FP_FromInteger(pRect->x)) > (FP_FromInteger(120) * mSpriteScale))
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

    PathLine* pNextLine = sCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLine->field_C_next);
    if (pNextLine)
    {
        while (pNextLine != BaseAliveGameObjectCollisionLine)
        {
            if (sub_436C60(&pNextLine->mRect, 0))
            {
                field_17E_flags.Set(Flags_17E::eBit4);
                pLastNextOrPrevLine = pNextLine;
                if (pNextLine)
                {
                    lastNextSegmentLength = totalNextSegmentLength;
                }
                break;
            }
            else
            {
                totalNextSegmentLength += FP_FromInteger(pNextLine->field_12_line_length);
                pNextLine = sCollisions->Get_Line_At_Idx(pNextLine->field_C_next);
                if (!pNextLine)
                {
                    break;
                }
            }
        }
    }

    FP totalPrevSegmentLength = {};
    PathLine* pPrevLine = sCollisions->Get_Line_At_Idx(BaseAliveGameObjectCollisionLine->field_A_previous);
    if (pPrevLine)
    {
        while (pPrevLine != BaseAliveGameObjectCollisionLine)
        {
            if (sub_436C60(&pPrevLine->mRect, 0))
            {
                if (totalPrevSegmentLength < lastNextSegmentLength)
                {
                    field_17E_flags.Clear(Flags_17E::eBit4);
                    pLastNextOrPrevLine = pPrevLine;
                }
                break;
            }
            else
            {
                totalPrevSegmentLength += FP_FromInteger(pPrevLine->field_12_line_length);
                pPrevLine = sCollisions->Get_Line_At_Idx(pPrevLine->field_A_previous);
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
    const FP calc = Math_Tan_496F70(mYPos - sControlledCharacter->mYPos, sControlledCharacter->mXPos - mXPos);
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
        field_17E_flags.Clear(Flags_17E::eBit4);
    }
    else
    {
        field_17E_flags.Set(Flags_17E::eBit4);
    }
}

void FlyingSlig::VSetMotion(s16 state)
{
    vSetMotion_43B1B0(state);
}

void FlyingSlig::vSetMotion_43B1B0(s16 newMotion)
{
    mCurrentMotion = newMotion;
    vUpdateAnimRes_4350A0();
}

s16 FlyingSlig::CollisionUp_43A640(FP velY)
{
    const FP y2 = mYPos - (mSpriteScale * FP_FromInteger(20)) + velY;

    FP xOff1 = {};
    FP xOff2 = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff1 = (mSpriteScale * FP_FromInteger(17));
        xOff2 = ((mSpriteScale * FP_FromInteger(17)) / FP_FromInteger(3));
    }
    else
    {
        xOff2 = (mSpriteScale * FP_FromInteger(17));
        xOff1 = ((mSpriteScale * FP_FromInteger(17)) / FP_FromInteger(3));
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = sCollisions->Raycast(
        mXPos - xOff1,
        mYPos,
        mXPos - xOff1,
        y2,
        &pLine,
        &hitX,
        &hitY,
        mScale == Scale::Fg ? CollisionMask(eFloor_0, eDynamicCollision_32, eWallLeft_1, eWallRight_2, eFlyingSligCeiling_17) : CollisionMask(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundFlyingSligCeiling_18));
    
    if (!bCollision)
    {
        bCollision = sCollisions->Raycast(
            xOff2 + mXPos,
            mYPos,
            xOff2 + mXPos,
            y2,
            &pLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ?
            CollisionMask(eFloor_0, eDynamicCollision_32, eWallLeft_1, eWallRight_2, eFlyingSligCeiling_17) :
            CollisionMask(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundFlyingSligCeiling_18));
    }

    if (bCollision)
    {
        if (FP_Abs(field_184_xSpeed) < FP_FromDouble(0.1) && mCurrentMotion != eFlyingSligMotions::M_UpKnockback_20_439110)
        {
            VSetMotion(eFlyingSligMotions::M_UpKnockback_20_439110);
        }

        if (static_cast<s32>(sGnFrame) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX_4C04F0(sGnFrame & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame + 10;
            relive_new ParticleBurst(
                mXPos,
                hitY + (FP_FromInteger(7) * mSpriteScale),
                5u,
                mSpriteScale,
                BurstType::eSmallPurpleSparks_6,
                9);
        }

        Slig_SoundEffect_4BFFE0(sGnFrame & 1 ? SligSfx::eCollideWithWall1_12 : SligSfx::eCollideWithWall2_13, this);

        mYPos += velY + hitY - y2;
        return 1;
    }
    return 0;
}

s16 FlyingSlig::CollisionDown_43A9E0(FP velY)
{
    const FP y2 = (mSpriteScale * FP_FromInteger(10)) + mYPos + velY;

    FP xOff1 = {};
    FP xOff2 = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff1 = (mSpriteScale * FP_FromInteger(17));
        xOff2 = ((mSpriteScale * FP_FromInteger(17)) / FP_FromInteger(3));
    }
    else
    {
        xOff1 = ((mSpriteScale * FP_FromInteger(17)) / FP_FromInteger(3));
        xOff2 = (mSpriteScale * FP_FromInteger(17));
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = sCollisions->Raycast(
        mXPos - xOff1,
        mYPos,
        mXPos - xOff1,
        y2,
        &pLine,
        &hitX,
        &hitY,
        mScale == Scale::Fg ? 
        CollisionMask(eFloor_0, eDynamicCollision_32, eFlyingSligCeiling_17) : 
        CollisionMask(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundFlyingSligCeiling_18));

    if (!bCollision)
    {
        bCollision = sCollisions->Raycast(
            xOff2 + mXPos,
            mYPos,
            xOff2 + mXPos,
            y2,
            &pLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? 
            CollisionMask(eFloor_0, eDynamicCollision_32, eFlyingSligCeiling_17) : 
            CollisionMask(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundFlyingSligCeiling_18));

    }

    if (bCollision)
    {
        if (FP_Abs(field_184_xSpeed) < FP_FromDouble(0.1) && mCurrentMotion != eFlyingSligMotions::M_DownKnockback_19_4390D0)
        {
            VSetMotion(eFlyingSligMotions::M_DownKnockback_19_4390D0);
        }

        if (static_cast<s32>(sGnFrame) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX_4C04F0(sGnFrame & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame + 10;
        }

        mYPos += velY + hitY - y2;
        return 1;
    }
    return 0;
}

s16 FlyingSlig::CollisionLeftRight_43AC80(FP velX)
{
    FP newX = {};
    if (velX <= FP_FromInteger(0))
    {
        newX = mXPos - (mSpriteScale * FP_FromInteger(17));
    }
    else
    {
        newX = (mSpriteScale * FP_FromInteger(17)) + mXPos;
    }

    const FP xOff = velX + newX;
    const FP yOff2 = mYPos - (mSpriteScale * FP_FromInteger(20));
    const FP yOff1 = (mSpriteScale * FP_FromInteger(20));

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = sCollisions->Raycast(
        mXPos,
        mYPos - yOff1,
        xOff,
        yOff2,
        &pLine,
        &hitX,
        &hitY,
        mScale == Scale::Fg ? CollisionMask(eWallLeft_1, eWallRight_2, eFlyingSligCeiling_17) : CollisionMask(eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundFlyingSligCeiling_18));

    FP sparkX = {};
    if (bCollision)
    {
        FP k25Directed = {};
        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
        {
            k25Directed = FP_FromInteger(-25);
        }
        else
        {
            k25Directed = FP_FromInteger(25);
        }
        sparkX = (k25Directed * mSpriteScale) + mXPos;
        Slig_SoundEffect_4BFFE0(sGnFrame & 1 ? SligSfx::eCollideWithWall1_12 : SligSfx::eCollideWithWall2_13, this);
    }
    else
    {
        bCollision = sCollisions->Raycast(
            mXPos,
            (mSpriteScale * FP_FromInteger(10)) + mYPos,
            xOff,
            (mSpriteScale * FP_FromInteger(10)) + mYPos,
            &pLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? CollisionMask(eWallLeft_1, eWallRight_2, eFlyingSligCeiling_17) : CollisionMask(eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundFlyingSligCeiling_18));

        if (bCollision)
        {
            FP k25Directed = {};
            if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
            {
                k25Directed = FP_FromInteger(-25);
            }
            else
            {
                k25Directed = FP_FromInteger(25);
            }
            sparkX = (k25Directed * mSpriteScale) + mXPos;
        }
    }

    if (bCollision)
    {
        if (static_cast<s32>(sGnFrame) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX_4C04F0(sGnFrame & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame + 10;
            relive_new ParticleBurst(sparkX, hitY + (FP_FromInteger(16) * mSpriteScale), 5u, mSpriteScale, BurstType::eSmallPurpleSparks_6, 9);
        }
        mXPos += velX + hitX - xOff;
        return 1;
    }

    return 0;
}

void FlyingSlig::PullLever_436450()
{
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    if ((FP_Abs(mXPos - field_1C8_lever_pull_range_xpos) < FP_FromInteger(1)) &&
        (FP_Abs(mYPos - field_1CC_lever_pull_range_ypos) < FP_FromInteger(1)))
    {
        VSetMotion(eFlyingSligMotions::M_LeverPull_7_439150);
    }
    SetBrain(&FlyingSlig::Brain_15_FlyingSligSpawn);
}

s16 FlyingSlig::TryPullLever_439DB0()
{
    if (BrainIs(&FlyingSlig::Brain_15_FlyingSligSpawn))
    {
        return 0;
    }

    FP kGridSizeDirected = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        kGridSizeDirected = -ScaleToGridSize(mSpriteScale);
    }
    else
    {
        kGridSizeDirected = ScaleToGridSize(mSpriteScale);
    }

    const FP k15Scaled = FP_FromInteger(15) * mSpriteScale;
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
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            const PSX_RECT bObjRect = pAliveObj->VGetBoundingRect();
            if (rect_w <= bObjRect.w && rect_x >= bObjRect.x && rect_y >= bObjRect.y && rect_h <= bObjRect.h)
            {
                if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (mXPos < pAliveObj->mXPos)
                    {
                        return FALSE;
                    }
                    field_1C8_lever_pull_range_xpos = (FP_FromInteger(45) * mSpriteScale) + pAliveObj->mXPos;
                }
                else
                {
                    if (mXPos > pAliveObj->mXPos)
                    {
                        return FALSE;
                    }
                    field_1C8_lever_pull_range_xpos = pAliveObj->mXPos - (FP_FromInteger(47) * mSpriteScale);
                }

                field_1CC_lever_pull_range_ypos = pAliveObj->mYPos - (FP_FromInteger(23) * mSpriteScale);
                field_158_obj_id = pAliveObj->mBaseGameObjectId;
                PullLever_436450();
                return TRUE;
            }
        }
    }
    return FALSE;
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
            if (!field_17E_flags.Get(Flags_17E::eBit2_bLastLine) && field_1EC_pNextLine)
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
            if (!field_17E_flags.Get(Flags_17E::eBit2_bLastLine) && field_1F0_pPrevLine)
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
