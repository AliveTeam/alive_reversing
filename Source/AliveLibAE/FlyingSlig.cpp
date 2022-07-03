#include "stdafx.h"
#include "FlyingSlig.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Collisions.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "ObjectIds.hpp"
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

FlyingSlig::FlyingSlig(Path_FlyingSlig* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(9)
{
    field_178_unused = 0;
    field_164_unused = -1;
    field_17A_unused = -1;

    if (tlvInfo != -1)
    {
        mBaseGameObjectTlvInfo = tlvInfo;
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
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    //Animation_Init(116888, 107, 48u, field_10_resources_array.ItemAt(0), 1, 1u);

    field_15E_useless = 0;

    mBaseAnimatedWithPhysicsGameObject_Anim.mFnPtrArray = kFlyingSlig_Anim_Frames_Fns_55EFC4;

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

    field_158_obj_id = -1;

    field_288_unused = 0;
    field_290_bobbing_values_index = 0;
    field_284_bobbing_value = FP_FromInteger(0);
    field_28C_bobbing_values_table_index = 0;
    field_1E8_unused = 0;
    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    field_118_data = *pTlv;

    field_148_tlvInfo = tlvInfo;

    SetBrain(&FlyingSlig::Brain_0_Inactive);

    mCurrentMotion = eFlyingSligMotions::M_Idle_0_4385E0;

    if (field_118_data.field_10_data.field_2_state == Path_FlyingSlig_Data::SpawnDelayStates::eUseCustomSpawnMoveDelay_1)
    {
        field_14C_timer = sGnFrame + field_118_data.field_10_data.field_4_spawn_move_delay;
    }
    else if (field_118_data.field_10_data.field_2_state == Path_FlyingSlig_Data::SpawnDelayStates::eMoveImmediately_0)
    {
        field_14C_timer = sGnFrame + 1;
    }

    field_2A8_max_x_speed = FP_FromInteger(field_118_data.field_10_data.field_1A_max_velocity) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_2AC_up_vel = FP_FromInteger(-field_118_data.field_10_data.field_1A_max_velocity) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_2B0_down_vel = FP_FromInteger(field_118_data.field_10_data.field_1A_max_velocity) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_2B4_max_slow_down = FP_FromDouble(0.4) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_2B8_max_speed_up = FP_FromDouble(0.4) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, field_118_data.field_10_data.field_A_direction == XDirection_short::eLeft_0);

    if (field_118_data.field_10_data.field_0_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    field_17E_flags.Set(Flags_17E::eBit13_Persistant, field_118_data.field_10_data.field_1E_persistant == Choice_short::eYes_1);

    field_17C_launch_switch_id |= field_118_data.field_10_data.field_1C_launch_switch_id;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    FP hitX = {};
    FP hitY = {};
    sCollisions->Raycast(
        FP_FromInteger(pTlv->mTopLeft.x),
        FP_FromInteger(pTlv->mTopLeft.y),
        FP_FromInteger(pTlv->mBottomRight.x),
        FP_FromInteger(pTlv->mBottomRight.y),
        &BaseAliveGameObjectCollisionLine,
        &hitX,
        &hitY,
        CollisionMask(eTrackLine_8));

    if (BaseAliveGameObjectCollisionLine)
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.x);
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
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

    auto pTlv = static_cast<Path_FlyingSlig*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pSaveState->field_3C_tlvInfo));
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

        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_XPos = pSaveState->field_4_xpos;
        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_YPos = pSaveState->field_8_ypos;

        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_VelX = pSaveState->field_C_velx;
        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_VelY = pSaveState->field_10_vely;

        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_PathNumber = pSaveState->field_14_path_number;
        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAE(pSaveState->field_16_lvl_number);
        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale = pSaveState->field_18_sprite_scale;

        pFlyingSlig->field_27C_r = pSaveState->field_1C_oldr;
        pFlyingSlig->field_27E_g = pSaveState->field_1E_oldg;
        pFlyingSlig->field_280_b = pSaveState->field_20_oldb;

        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(pSaveState->field_1C_oldr, pSaveState->field_1E_oldg, pSaveState->field_20_oldb);

        pFlyingSlig->mCurrentMotion = pSaveState->field_24_current_state;

        u8** ppRes = pFlyingSlig->ResBlockForMotion_4350F0(pSaveState->field_24_current_state);
        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sFlyingSligAnimIdTable[pFlyingSlig->mCurrentMotion], ppRes);

        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame = pSaveState->field_26_current_frame;

        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pSaveState->field_28_frame_change_counter;

        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pSaveState->field_2A_bAnimRender & 1);
        pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pSaveState->field_22_bAnimFlipX & 1);
        pFlyingSlig->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pSaveState->field_2B_bDrawable & 1);

        if (IsLastFrame(&pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_Anim))
        {
            pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
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
            sControlledCharacter_5C1B8C = pFlyingSlig;
            pFlyingSlig->field_2A8_max_x_speed = (FP_FromDouble(5.5) * pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            pFlyingSlig->field_2AC_up_vel = (FP_FromDouble(-5.5) * pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            pFlyingSlig->field_2B0_down_vel = (FP_FromDouble(5.5) * pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            pFlyingSlig->field_2B4_max_slow_down = (FP_FromDouble(0.25) * pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            pFlyingSlig->field_2B8_max_speed_up = (FP_FromDouble(0.7) * pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_SpriteScale);
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

        pFlyingSlig->field_2A0_abe_level = pFlyingSlig->mBaseAnimatedWithPhysicsGameObject_LvlNumber; // always the same but set to junk in OG saves when using path skip cheat
        //pFlyingSlig->field_2A0_abe_level = MapWrapper::FromAE(pSaveState->field_9A_abe_level);
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

    pState->field_4_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_8_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pState->field_C_velx = mBaseAnimatedWithPhysicsGameObject_VelX;
    pState->field_10_vely = mBaseAnimatedWithPhysicsGameObject_VelY;

    pState->field_14_path_number = mBaseAnimatedWithPhysicsGameObject_PathNumber;
    pState->field_16_lvl_number = MapWrapper::ToAE(mBaseAnimatedWithPhysicsGameObject_LvlNumber);
    pState->field_18_sprite_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    pState->field_1C_oldr = mBaseAnimatedWithPhysicsGameObject_RGB.r;
    pState->field_1E_oldg = mBaseAnimatedWithPhysicsGameObject_RGB.g;
    pState->field_20_oldb = mBaseAnimatedWithPhysicsGameObject_RGB.b;

    pState->field_22_bAnimFlipX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_24_current_state = mCurrentMotion;
    pState->field_26_current_frame = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
    pState->field_28_frame_change_counter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;

    pState->field_2B_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2A_bAnimRender = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);
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

    pState->field_3A.Set(FlyingSlig_State::eBit1_bPossessed, this == sControlledCharacter_5C1B8C);
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

    pState->field_58_obj_id = -1;
    if (field_158_obj_id != -1)
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
    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
        if (gMap.mLevel != EReliveLevelIds::eMenu)
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

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_148_tlvInfo);
    if (pTlv)
    {
        if (pTlv->mTlvType32.mType != TlvTypes::SligGetWings_105 && pTlv->mTlvType32.mType != TlvTypes::FlyingSligSpawner_92)
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
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mOverlayId != gMap.GetOverlayId() || (gMap.mCurrentPath != gMap.mPath && (this != sControlledCharacter_5C1B8C || field_17E_flags.Get(Flags_17E::eBit13_Persistant))))
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

        if (field_158_obj_id != -1)
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
                    field_158_obj_id = pObj->field_8_object_id;
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
        mBaseAnimatedWithPhysicsGameObject_XPos = field_294_nextXPos;
        mBaseAnimatedWithPhysicsGameObject_YPos = field_298_nextYPos;

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
    field_298_nextYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
    field_294_nextXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    field_18C = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);
    sub_437C70(BaseAliveGameObjectCollisionLine);
    const s16 v5 = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - field_1A4_rect.y);
    const s16 v6 = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - field_1A4_rect.x);
    field_194 = FP_FromInteger(Math_SquareRoot_Int_496E70(v5 * v5 + v6 * v6));
    field_17E_flags.Set(Flags_17E::eBit4, field_118_data.field_10_data.field_A_direction == XDirection_short::eLeft_0);
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
        mBaseAnimatedWithPhysicsGameObject_VelX = posXY.x - mBaseAnimatedWithPhysicsGameObject_XPos;
        mBaseAnimatedWithPhysicsGameObject_VelY = posXY.y - mBaseAnimatedWithPhysicsGameObject_YPos;
    }
    else
    {
        const s32 newVel = Math_SquareRoot_Int_496E70(FP_GetExponent((mBaseAnimatedWithPhysicsGameObject_VelY * mBaseAnimatedWithPhysicsGameObject_VelY) + (mBaseAnimatedWithPhysicsGameObject_VelX * mBaseAnimatedWithPhysicsGameObject_VelX)));
        if (FP_Abs(FP_FromInteger(newVel)) > FP_FromDouble(0.05))
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX - ((mBaseAnimatedWithPhysicsGameObject_VelX / FP_FromInteger(newVel)) * field_2B4_max_slow_down) + field_184_xSpeed;
            mBaseAnimatedWithPhysicsGameObject_VelY = mBaseAnimatedWithPhysicsGameObject_VelY - ((mBaseAnimatedWithPhysicsGameObject_VelY / FP_FromInteger(newVel)) * field_2B4_max_slow_down) + field_188_ySpeed;
        }
        else
        {
            if (field_184_xSpeed != FP_FromInteger(0) || field_188_ySpeed != FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX += field_184_xSpeed;
                mBaseAnimatedWithPhysicsGameObject_VelY = mBaseAnimatedWithPhysicsGameObject_VelY + field_188_ySpeed;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
            }
        }

        const FP v15 = FP_FromInteger(Math_SquareRoot_Int_496E70(FP_GetExponent((mBaseAnimatedWithPhysicsGameObject_VelY * mBaseAnimatedWithPhysicsGameObject_VelY) + (mBaseAnimatedWithPhysicsGameObject_VelX * mBaseAnimatedWithPhysicsGameObject_VelX))));
        if (v15 > field_2A8_max_x_speed)
        {
            mBaseAnimatedWithPhysicsGameObject_VelX = ((mBaseAnimatedWithPhysicsGameObject_VelX / v15) * field_2A8_max_x_speed);
            mBaseAnimatedWithPhysicsGameObject_VelY = ((mBaseAnimatedWithPhysicsGameObject_VelY / v15) * field_2A8_max_x_speed);
        }

        if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0))
        {
            if (CollisionUp_43A640(mBaseAnimatedWithPhysicsGameObject_VelY))
            {
                mBaseAnimatedWithPhysicsGameObject_VelY = -mBaseAnimatedWithPhysicsGameObject_VelY;
            }
        }

        if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0))
        {
            if (CollisionDown_43A9E0(mBaseAnimatedWithPhysicsGameObject_VelY))
            {
                mBaseAnimatedWithPhysicsGameObject_VelY = -mBaseAnimatedWithPhysicsGameObject_VelY;
            }
        }

        if (mBaseAnimatedWithPhysicsGameObject_VelX != FP_FromInteger(0))
        {
            if (CollisionLeftRight_43AC80(mBaseAnimatedWithPhysicsGameObject_VelX))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -mBaseAnimatedWithPhysicsGameObject_VelX;
            }
        }
    }

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

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

    field_294_nextXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    field_298_nextYPos = mBaseAnimatedWithPhysicsGameObject_YPos;

    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20);

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
        mBaseAnimatedWithPhysicsGameObject_YPos = mBaseAnimatedWithPhysicsGameObject_YPos + field_284_bobbing_value;
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
            mBaseAnimatedWithPhysicsGameObject_YPos = mBaseAnimatedWithPhysicsGameObject_YPos + field_284_bobbing_value;
        }
    }

    s32 v37 = {};
    if (Math_Distance(0, 0, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelX), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelY)) >= 0)
    {
        v37 = Math_Distance(0, 0, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelX), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelY));
    }
    else
    {
        v37 = -Math_Distance(0, 0, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelX), FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_VelY));
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
            if (static_cast<Bullet*>(pFrom)->field_20_type == BulletType::eZBullet_3)
            {
                const PSX_RECT bRect = VGetBoundingRect();
                Path_TLV* pTlv = nullptr;
                do
                {
                    pTlv = sPath_dword_BB47C0->TlvGetAt(pTlv,
                                                                 mBaseAnimatedWithPhysicsGameObject_XPos,
                                                                 FP_FromInteger(bRect.y),
                                                                 mBaseAnimatedWithPhysicsGameObject_XPos,
                                                                 FP_FromInteger(bRect.y));
                    if (!pTlv)
                    {
                        break;
                    }
                    if (pTlv->mTlvType32 == TlvTypes::ZSligCover_50)
                    {
                        // Left/right in cover
                        if (bRect.x >= pTlv->mTopLeft.x && bRect.x <= pTlv->mBottomRight.x && bRect.y >= pTlv->mTopLeft.y && bRect.y <= pTlv->mBottomRight.y)
                        {
                            // Top/bottom in cover
                            if (bRect.w >= pTlv->mTopLeft.x && bRect.w <= pTlv->mBottomRight.x && bRect.h >= pTlv->mTopLeft.y && bRect.h <= pTlv->mBottomRight.y)
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
            auto pExplosion = relive_new Explosion(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5)), mBaseAnimatedWithPhysicsGameObject_SpriteScale, 1);
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
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
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
    else if (VIsFacingMe(sControlledCharacter_5C1B8C))
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

    if (EventGet(kEventHeroDying) && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        ToAbeDead_436010();
        return;
    }

    if (EventGet(kEventResetting) || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_SpriteScale != mBaseAnimatedWithPhysicsGameObject_SpriteScale || IsInInvisibleZone(sControlledCharacter_5C1B8C) || sControlledCharacter_5C1B8C->mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit8_bInvisible) || (!IsWallBetween_43A550(this, sControlledCharacter_5C1B8C) && (!IsActiveHero(sControlledCharacter_5C1B8C) || sActiveHero->mCurrentMotion != eAbeMotions::Motion_65_LedgeAscend_4548E0) && sControlledCharacter_5C1B8C->Type() != ReliveTypes::eMineCar))
    {
        PatrolDelay_435860();
        return;
    }

    if (sub_436C60(&BaseAliveGameObjectCollisionLine->mRect, 1))
    {
        if (FP_Abs(field_194 - field_1C4) < (FP_FromInteger(15) * mBaseAnimatedWithPhysicsGameObject_SpriteScale))
        {
            ToLaunchingGrenade_435F50();
            return;
        }

        field_17E_flags.Set(Flags_17E::eBit4, field_1C4 > field_194);
        field_17E_flags.Set(Flags_17E::eBit3);

        if (static_cast<s32>(sGnFrame) > field_150_grenade_delay && CanThrowGrenade_43A490())
        {
            if (VIsFacingMe(sControlledCharacter_5C1B8C))
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
    if (CanChase_436850(sControlledCharacter_5C1B8C))
    {
        ToChase_435E10();
        return;
    }

    if (!IsEventInRange(kEventAbeOhm, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)))
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
    if (CanChase_436850(sControlledCharacter_5C1B8C))
    {
        ToChase_435E10();
    }
    else if (IsEventInRange(kEventAbeOhm, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)))
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
    if (VIsFacingMe(sControlledCharacter_5C1B8C))
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
    if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
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
            const FP xOff = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(Math_RandomRange(-20, 20) + (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? -10 : 10)));
            const FP yOff = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(Math_RandomRange(-20, 10)));
            New_TintChant_Particle(
                xOff + mBaseAnimatedWithPhysicsGameObject_XPos,
                yOff + mBaseAnimatedWithPhysicsGameObject_YPos,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
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
    if (sControlledCharacter_5C1B8C == this && mHealth > FP_FromInteger(0))
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - field_1C8_lever_pull_range_xpos) >= FP_FromInteger(1) || FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - field_1CC_lever_pull_range_ypos) >= FP_FromInteger(1))
    {
        const FP delta1 = field_1C8_lever_pull_range_xpos - mBaseAnimatedWithPhysicsGameObject_XPos;
        const FP delta2 = field_1CC_lever_pull_range_ypos - mBaseAnimatedWithPhysicsGameObject_YPos;

        if (FP_Abs(field_1C8_lever_pull_range_xpos - mBaseAnimatedWithPhysicsGameObject_XPos) <= FP_FromInteger(2))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos = field_1C8_lever_pull_range_xpos;
        }
        else if (delta1 >= FP_FromInteger(2))
        {
            mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger(2);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_XPos -= FP_FromInteger(2);
        }

        if (FP_Abs(field_1CC_lever_pull_range_ypos - mBaseAnimatedWithPhysicsGameObject_YPos) <= FP_FromInteger(2))
        {
            mBaseAnimatedWithPhysicsGameObject_YPos = field_1CC_lever_pull_range_ypos;
        }
        else if (delta2 >= FP_FromInteger(2))
        {
            mBaseAnimatedWithPhysicsGameObject_YPos += FP_FromInteger(2);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_YPos -= FP_FromInteger(2);
        }

        if ((FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - field_1C8_lever_pull_range_xpos) < FP_FromInteger(1)) && (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - field_1CC_lever_pull_range_ypos) < FP_FromInteger(1)))
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
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(5),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            CollisionMask(eTrackLine_8))
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
        sub_4348A0();
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        field_188_ySpeed = FP_FromInteger(0);
        field_184_xSpeed = FP_FromInteger(0);
        PatrolDelay_435860();
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromDouble(-4.5);
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 4)
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
            if (field_188_ySpeed >= FP_FromInteger(0))
            {
                if (field_188_ySpeed <= FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection_43A510())
                    {
                        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);

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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);

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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
        }
    }
    else
    {
        auto pSwitch = static_cast<Lever*>(sObjectIds.Find_Impl(field_158_obj_id));
        if (pSwitch)
        {
            pSwitch->VPull(mBaseAnimatedWithPhysicsGameObject_XPos < pSwitch->mBaseAnimatedWithPhysicsGameObject_XPos);
        }
        field_158_obj_id = -1;
    }
}

void FlyingSlig::M_GameSpeak_8_4391D0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 1 && field_17E_flags.Get(Flags_17E::eBit1_Speaking_flag1))
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
    else if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        this->VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_Possession_9_434290()
{
    
}

void FlyingSlig::M_EndHorizontalMovement_10_4387D0()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        VSetMotion(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_BeginUpMovement_11_438E40()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
        else
        {
            VSetMotion(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
        }
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);

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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        VSetMotion(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_InstantDownXTurn_23_438F20()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
        VSetMotion(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_HorizontalToUpMovement_24_438D60()
{
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);

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
    return ((field_184_xSpeed > FP_FromInteger(0) && !(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
            || (field_184_xSpeed < FP_FromInteger(0) && mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)));
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
    field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + field_118_data.field_10_data.field_C_panic_delay;
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
    if (!gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) || !gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0) || EventGet(kEventResetting) || IsAbeEnteringDoor_43B030(pObj) || sActiveHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale != mBaseAnimatedWithPhysicsGameObject_SpriteScale || !IsWallBetween_43A550(this, pObj))
    {
        return 0;
    }

    if (IsEventInRange(kEventAbeOhm, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)))
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
        if (!field_17E_flags.Get(Flags_17E::eBit12_bNoNextLine) && (field_182_bound1 == TlvTypes::ContinuePoint_0 || !a2)) // todo: change to None when we can break abi
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
            if (width > FP_FromInteger(0) && field_182_bound1 == TlvTypes::SligBoundLeft_32)
            {
                field_190 = field_2B8_max_speed_up;
            }
            else if (width < FP_FromInteger(0) && field_182_bound1 == TlvTypes::SligBoundRight_45)
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
        if (!field_17E_flags.Get(Flags_17E::eBit11_bNoPrevLine) && (field_180_bound2 == TlvTypes::ContinuePoint_0 || !a2)) // todo: change to None when we can break abi
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
            if (width > FP_FromInteger(0) && field_180_bound2 == TlvTypes::SligBoundLeft_32)
            {
                field_190 = -field_2B8_max_speed_up;
            }
            else if (width < FP_FromInteger(0) && field_180_bound2 == TlvTypes::SligBoundRight_45)
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
               pThis->mBaseAnimatedWithPhysicsGameObject_XPos,
               pThis->mBaseAnimatedWithPhysicsGameObject_YPos,
               pObj->mBaseAnimatedWithPhysicsGameObject_XPos,
               FP_FromInteger((bRect.y + bRect.h) / 2),
               &pLine,
               &hitX,
               &hitY,
               pThis->mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloorWallOrCeiling : kBgFloorWallOrCeiling)
        != 1;
}

void FlyingSlig::ThrowGrenade_43A1E0()
{
    FP grenadeXVel = (FP_FromInteger(Math_RandomRange(50, 64)) / FP_FromInteger(10) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP grenadeYVel = (FP_FromInteger(-6) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    FP grenadeXPos = (FP_FromInteger(0) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP grenadeYPos = (FP_FromInteger(-5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    const FP xpos = (FP_FromInteger(0) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    const FP ypos = (FP_FromInteger(-20) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        grenadeXPos = -grenadeXPos;
        grenadeXVel = -grenadeXVel;
    }

    auto pGrenade = relive_new Grenade(grenadeXPos + mBaseAnimatedWithPhysicsGameObject_XPos, grenadeYPos + mBaseAnimatedWithPhysicsGameObject_YPos, 0, 1, 0, this);
    if (pGrenade)
    {
        pGrenade->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        pGrenade->mBaseAnimatedWithPhysicsGameObject_Scale = mBaseAnimatedWithPhysicsGameObject_Scale;
        pGrenade->VThrow(grenadeXVel, grenadeYVel);
    }

    New_ShootingFire_Particle(xpos + mBaseAnimatedWithPhysicsGameObject_XPos, ypos + mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX), mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    Slig_SoundEffect_4BFFE0(SligSfx::eThrowGrenade_8, this);
    EventBroadcast(kEventShooting, this);
    EventBroadcast(kEventLoudNoise, this);

    Dove::All_FlyAway(0);

    s32 randomisedGrenadeDelay = field_118_data.field_10_data.field_18_grenade_delay + (Math_NextRandom() & 7);
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

    relive_new Gibs(GibType::Slig_1, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_VelX, mBaseAnimatedWithPhysicsGameObject_VelY, mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0);

    relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * mBaseAnimatedWithPhysicsGameObject_SpriteScale), FP_FromInteger(0), FP_FromInteger(0), mBaseAnimatedWithPhysicsGameObject_SpriteScale, 20);

    New_Smoke_Particles(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(30) * mBaseAnimatedWithPhysicsGameObject_SpriteScale), mBaseAnimatedWithPhysicsGameObject_SpriteScale, 3, 128u, 128u, 128u);
    SfxPlayMono(SoundEffect::KillEffect_64, 128, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    SfxPlayMono(SoundEffect::FallingItemHit_47, 90, mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

    field_18C = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);
    field_184_xSpeed = FP_FromInteger(0);
    mHealth = FP_FromInteger(0);
    SetBrain(&FlyingSlig::Brain_1_Death);
    field_14C_timer = sGnFrame + 40;
}

s16 FlyingSlig::sub_436730()
{
    if (CanChase_436850(sControlledCharacter_5C1B8C))
    {
        ToSpottedEnemy_435E70();
        return 1;
    }
    else if (CanHearAbe_4369C0() && !BrainIs(&FlyingSlig::Brain_3_GetAlerted))
    {
        ToAlerted_4357E0();
        return 1;
    }
    else if (IsEventInRange(kEventAbeOhm, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)))
    {
        ToPanicMoving_435A50();
        return 1;
    }
    else
    {
        if (sActiveHero && sActiveHero->mHealth < FP_FromInteger(0) && FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - sActiveHero->mBaseAnimatedWithPhysicsGameObject_XPos) < FP_FromInteger(640) && FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - sActiveHero->mBaseAnimatedWithPhysicsGameObject_YPos) < FP_FromInteger(240))
        {
            ToAbeDead_436010();
            return 1;
        }
        return 0;
    }
}

s16 FlyingSlig::CanHearAbe_4369C0()
{
    return IsActiveHero(IsEventInRange(kEventSuspiciousNoise, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale))) || IsActiveHero(IsEventInRange(kEventSpeaking, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, AsEventScale(mBaseAnimatedWithPhysicsGameObject_Scale)));
}

void FlyingSlig::ToSpottedEnemy_435E70()
{
    if (field_118_data.field_10_data.field_10_prechase_delay)
    {
        Say_436A50(SligSpeak ::eFreeze_8, 0);
        field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + field_118_data.field_10_data.field_10_prechase_delay;
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
    field_14C_timer = sGnFrame + field_118_data.field_10_data.field_14_alerted_listen_time;
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
    SfxPlayMono(SoundEffect::PossessEffect_17, 0);
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
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(sFlyingSligAnimIdTable[mCurrentMotion], ppRes);
}

void FlyingSlig::PatrolDelay_435860()
{
    if (BrainIs(&FlyingSlig::Brain_4_ChasingEnemy))
    {
        field_14C_timer = (Math_NextRandom() & 7) + sGnFrame + field_118_data.field_10_data.field_E_give_up_chase_delay;
        SetBrain(&FlyingSlig::Brain_5_Idle);
        return;
    }

    field_14C_timer = field_118_data.field_10_data.field_6_patrol_pause_min + sGnFrame;
    if (field_118_data.field_10_data.field_6_patrol_pause_min <= field_118_data.field_10_data.field_8_patrol_pause_max)
    {
        SetBrain(&FlyingSlig::Brain_5_Idle);
        return;
    }

    field_14C_timer += Math_NextRandom() % (field_118_data.field_10_data.field_6_patrol_pause_min - field_118_data.field_10_data.field_8_patrol_pause_max);
    SetBrain(&FlyingSlig::Brain_5_Idle);
}

s16 FlyingSlig::CanThrowGrenade_43A490()
{
    return mCurrentMotion == eFlyingSligMotions::M_Idle_0_4385E0 || mCurrentMotion == eFlyingSligMotions::M_HorizontalMovement_1_4386A0 || mCurrentMotion == eFlyingSligMotions::M_DownMovement_3_438AA0 || mCurrentMotion == eFlyingSligMotions::M_UpMovement_5_438DD0 || mCurrentMotion == eFlyingSligMotions::M_IdleToHorizontalMovement_16_438730 || mCurrentMotion == eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0 || mCurrentMotion == eFlyingSligMotions::M_BeginUpMovement_11_438E40 || mCurrentMotion == eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60 || mCurrentMotion == eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10 || mCurrentMotion == eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0 || mCurrentMotion == eFlyingSligMotions::M_EndDownMovement_18_438C90 || mCurrentMotion == eFlyingSligMotions::M_BeginDownMovement_17_438B80;
}

void FlyingSlig::ToLaunchingGrenade_435F50()
{
    if (!VIsFacingMe(sControlledCharacter_5C1B8C) && !IsTurning_436AE0())
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

    if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eThrowItem))
    {
        field_17E_flags.Set(Flags_17E::eBit5_Throw);
    }

    if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eDoAction))
    {
        field_17E_flags.Set(Flags_17E::eBit7_DoAction);
    }

    if (!field_17E_flags.Get(Flags_17E::eBit1_Speaking_flag1))
    {
        field_17E_flags.Set(Flags_17E::eBit1_Speaking_flag1);
        field_17E_flags.Set(Flags_17E::eBit10_Speaking_flag2);

        field_160_voice_pitch_min = 0;

        if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak1_555104))
        {
            field_17D_next_speak = SligSpeak::eHi_0;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak3_555100))
        {
            field_17D_next_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? SligSpeak::eGetHim_2 : SligSpeak::eFreeze_8;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak4_5550FC))
        {
            field_17D_next_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? SligSpeak::eFreeze_8 : SligSpeak::eGetHim_2;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak2_5550F8))
        {
            field_17D_next_speak = SligSpeak::eHereBoy_1;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak6_555108))
        {
            field_17D_next_speak = SligSpeak::eBullshit_5;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak7_555114))
        {
            field_17D_next_speak = SligSpeak::eLookOut_6;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak5_55510C))
        {
            field_17D_next_speak = SligSpeak::eBullshit2_7;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak8_555110))
        {
            field_17D_next_speak = SligSpeak::eLaugh_3;
        }
        else
        {
            field_17E_flags.Clear(Flags_17E::eBit1_Speaking_flag1);
            field_17E_flags.Clear(Flags_17E::eBit10_Speaking_flag2);
        }
    }

    if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eUp))
    {
        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft))
        {
            field_184_xSpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            return;
        }
        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight))
        {
            field_184_xSpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            return;
        }
        field_184_xSpeed = FP_FromInteger(0);
        field_188_ySpeed = -field_2B8_max_speed_up;
    }
    else if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eDown))
    {
        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft))
        {
            field_184_xSpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
        }
        else if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight))
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
    else if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft))
    {
        field_188_ySpeed = FP_FromInteger(0);
        field_184_xSpeed = -field_2B8_max_speed_up;
    }
    else if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight))
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

TlvTypes FlyingSlig::FindLeftOrRightBound_43B0A0(FP xOrY, FP wOrH)
{
    const FP kGridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    const FP left = xOrY - kGridSize;
    const FP top = wOrH - kGridSize;
    const FP right = xOrY + kGridSize;
    const FP bottom = wOrH + kGridSize;

    // TODO: Check left is really Abs'd.
    TlvTypes found_type = {};

    if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(FP_GetExponent(FP_Abs(left)), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), TlvTypes::SligBoundLeft_32))
    {
        found_type = TlvTypes::SligBoundLeft_32;
    }
    else if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(FP_GetExponent(left), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), TlvTypes::SligBoundRight_45))
    {
        found_type = TlvTypes::SligBoundRight_45;
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

    field_2A8_max_x_speed = FP_FromDouble(5.5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_2AC_up_vel = FP_FromDouble(-5.5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_2B0_down_vel = FP_FromDouble(5.5) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_2B4_max_slow_down = FP_FromDouble(0.25) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    field_2B8_max_speed_up = FP_FromDouble(0.7) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

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
        const FP k80Scaled = (FP_FromInteger(80) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);

        const FP rLeft = sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos - k80Scaled;
        const FP rRight = sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos + k80Scaled;

        const FP r_x = FP_FromInteger(std::min(pRect->x, pRect->w));
        const FP r_w = FP_FromInteger(std::max(pRect->w, pRect->x));

        const FP r_y = FP_FromInteger(std::min(pRect->y, pRect->h));
        const FP r_h = FP_FromInteger(std::max(pRect->h, pRect->y) + 150);

        s32 bLeftInRect = 0;
        if (rLeft < r_x || rLeft > r_w || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos < r_y || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos > r_h)
        {
            bLeftInRect = 0;
        }
        else
        {
            bLeftInRect = 1;
        }

        s32 bRightInRect = 0;
        if (rRight < r_x || rRight > r_w || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos < r_y || sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos > r_h)
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
            if ((sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(60) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) - yOff1) > (FP_FromInteger(35) * mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                FP hitX = {};
                FP hitY = {};
                PathLine* pLine = nullptr;

                if (sCollisions->Raycast(
                        sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos,
                        yOff1,
                        sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos,
                        yOff1 + (FP_FromInteger(35) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                        &pLine,
                        &hitX,
                        &hitY,
                        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
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

            if ((sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(60) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) - yOff2) > (FP_FromInteger(35) * mBaseAnimatedWithPhysicsGameObject_SpriteScale))
            {
                if (sCollisions->Raycast(
                        sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos,
                        yOff2,
                        sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos,
                        yOff2 + (FP_FromInteger(35) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                        &pLine,
                        &hitX,
                        &hitY,
                        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
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
        const FP k40Scaled = (FP_FromInteger(40) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        const FP yTop = sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos - k40Scaled;
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

        if (FP_Abs(sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(pRect->x)) > (FP_FromInteger(120) * mBaseAnimatedWithPhysicsGameObject_SpriteScale))
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
    const FP calc = Math_Tan_496F70(mBaseAnimatedWithPhysicsGameObject_YPos - sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_YPos, sControlledCharacter_5C1B8C->mBaseAnimatedWithPhysicsGameObject_XPos - mBaseAnimatedWithPhysicsGameObject_XPos);
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
    const FP y2 = mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20)) + velY;

    FP xOff1 = {};
    FP xOff2 = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff1 = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17));
        xOff2 = ((mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17)) / FP_FromInteger(3));
    }
    else
    {
        xOff2 = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17));
        xOff1 = ((mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17)) / FP_FromInteger(3));
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = sCollisions->Raycast(
        mBaseAnimatedWithPhysicsGameObject_XPos - xOff1,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_XPos - xOff1,
        y2,
        &pLine,
        &hitX,
        &hitY,
        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eFloor_0, eDynamicCollision_32, eWallLeft_1, eWallRight_2, eFlyingSligCeiling_17) : CollisionMask(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundFlyingSligCeiling_18));
    
    if (!bCollision)
    {
        bCollision = sCollisions->Raycast(
            xOff2 + mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            xOff2 + mBaseAnimatedWithPhysicsGameObject_XPos,
            y2,
            &pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ?
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
                mBaseAnimatedWithPhysicsGameObject_XPos,
                hitY + (FP_FromInteger(7) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                5u,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                BurstType::eSmallPurpleSparks_6,
                9);
        }

        Slig_SoundEffect_4BFFE0(sGnFrame & 1 ? SligSfx::eCollideWithWall1_12 : SligSfx::eCollideWithWall2_13, this);

        mBaseAnimatedWithPhysicsGameObject_YPos += velY + hitY - y2;
        return 1;
    }
    return 0;
}

s16 FlyingSlig::CollisionDown_43A9E0(FP velY)
{
    const FP y2 = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10)) + mBaseAnimatedWithPhysicsGameObject_YPos + velY;

    FP xOff1 = {};
    FP xOff2 = {};
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff1 = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17));
        xOff2 = ((mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17)) / FP_FromInteger(3));
    }
    else
    {
        xOff1 = ((mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17)) / FP_FromInteger(3));
        xOff2 = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17));
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = sCollisions->Raycast(
        mBaseAnimatedWithPhysicsGameObject_XPos - xOff1,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_XPos - xOff1,
        y2,
        &pLine,
        &hitX,
        &hitY,
        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? 
        CollisionMask(eFloor_0, eDynamicCollision_32, eFlyingSligCeiling_17) : 
        CollisionMask(eBackgroundFloor_4, eBackgroundDynamicCollision_36, eBackgroundFlyingSligCeiling_18));

    if (!bCollision)
    {
        bCollision = sCollisions->Raycast(
            xOff2 + mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            xOff2 + mBaseAnimatedWithPhysicsGameObject_XPos,
            y2,
            &pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? 
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

        mBaseAnimatedWithPhysicsGameObject_YPos += velY + hitY - y2;
        return 1;
    }
    return 0;
}

s16 FlyingSlig::CollisionLeftRight_43AC80(FP velX)
{
    FP newX = {};
    if (velX <= FP_FromInteger(0))
    {
        newX = mBaseAnimatedWithPhysicsGameObject_XPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17));
    }
    else
    {
        newX = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(17)) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    const FP xOff = velX + newX;
    const FP yOff2 = mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20));
    const FP yOff1 = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20));

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = sCollisions->Raycast(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - yOff1,
        xOff,
        yOff2,
        &pLine,
        &hitX,
        &hitY,
        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eWallLeft_1, eWallRight_2, eFlyingSligCeiling_17) : CollisionMask(eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundFlyingSligCeiling_18));

    FP sparkX = {};
    if (bCollision)
    {
        FP k25Directed = {};
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
        {
            k25Directed = FP_FromInteger(-25);
        }
        else
        {
            k25Directed = FP_FromInteger(25);
        }
        sparkX = (k25Directed * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos;
        Slig_SoundEffect_4BFFE0(sGnFrame & 1 ? SligSfx::eCollideWithWall1_12 : SligSfx::eCollideWithWall2_13, this);
    }
    else
    {
        bCollision = sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10)) + mBaseAnimatedWithPhysicsGameObject_YPos,
            xOff,
            (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10)) + mBaseAnimatedWithPhysicsGameObject_YPos,
            &pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? CollisionMask(eWallLeft_1, eWallRight_2, eFlyingSligCeiling_17) : CollisionMask(eBackgroundWallLeft_5, eBackgroundWallRight_6, eBackgroundFlyingSligCeiling_18));

        if (bCollision)
        {
            FP k25Directed = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                k25Directed = FP_FromInteger(-25);
            }
            else
            {
                k25Directed = FP_FromInteger(25);
            }
            sparkX = (k25Directed * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mBaseAnimatedWithPhysicsGameObject_XPos;
        }
    }

    if (bCollision)
    {
        if (static_cast<s32>(sGnFrame) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX_4C04F0(sGnFrame & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame + 10;
            relive_new ParticleBurst(sparkX, hitY + (FP_FromInteger(16) * mBaseAnimatedWithPhysicsGameObject_SpriteScale), 5u, mBaseAnimatedWithPhysicsGameObject_SpriteScale, BurstType::eSmallPurpleSparks_6, 9);
        }
        mBaseAnimatedWithPhysicsGameObject_XPos += velX + hitX - xOff;
        return 1;
    }

    return 0;
}

void FlyingSlig::PullLever_436450()
{
    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    if ((FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - field_1C8_lever_pull_range_xpos) < FP_FromInteger(1)) &&
        (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - field_1CC_lever_pull_range_ypos) < FP_FromInteger(1)))
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
    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        kGridSizeDirected = -ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }
    else
    {
        kGridSizeDirected = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    }

    const FP k15Scaled = FP_FromInteger(15) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    const FP k2ScaledDirected = (kGridSizeDirected * FP_FromInteger(2));

    // TODO: These can be replaced with std::min/std::max.
    FP rect_w_fp = {};
    if (mBaseAnimatedWithPhysicsGameObject_XPos + kGridSizeDirected <= mBaseAnimatedWithPhysicsGameObject_XPos + k2ScaledDirected)
    {
        rect_w_fp = mBaseAnimatedWithPhysicsGameObject_XPos + kGridSizeDirected;
    }
    else
    {
        rect_w_fp = mBaseAnimatedWithPhysicsGameObject_XPos + k2ScaledDirected;
    }

    FP rect_x_fp = {};
    if (mBaseAnimatedWithPhysicsGameObject_XPos + k2ScaledDirected <= mBaseAnimatedWithPhysicsGameObject_XPos + kGridSizeDirected)
    {
        rect_x_fp = mBaseAnimatedWithPhysicsGameObject_XPos + kGridSizeDirected;
    }
    else
    {
        rect_x_fp = (k2ScaledDirected) + mBaseAnimatedWithPhysicsGameObject_XPos;
    }

    FP rect_h_fp = {};
    if (mBaseAnimatedWithPhysicsGameObject_YPos - k15Scaled <= k15Scaled + mBaseAnimatedWithPhysicsGameObject_YPos)
    {
        rect_h_fp = mBaseAnimatedWithPhysicsGameObject_YPos - k15Scaled;
    }
    else
    {
        rect_h_fp = k15Scaled + mBaseAnimatedWithPhysicsGameObject_YPos;
    }

    FP rect_y_fp = {};
    if (k15Scaled + mBaseAnimatedWithPhysicsGameObject_YPos <= mBaseAnimatedWithPhysicsGameObject_YPos - k15Scaled)
    {
        rect_y_fp = mBaseAnimatedWithPhysicsGameObject_YPos - k15Scaled;
    }
    else
    {
        rect_y_fp = k15Scaled + mBaseAnimatedWithPhysicsGameObject_YPos;
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
                if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (mBaseAnimatedWithPhysicsGameObject_XPos < pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos)
                    {
                        return FALSE;
                    }
                    field_1C8_lever_pull_range_xpos = (FP_FromInteger(45) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos;
                }
                else
                {
                    if (mBaseAnimatedWithPhysicsGameObject_XPos > pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos)
                    {
                        return FALSE;
                    }
                    field_1C8_lever_pull_range_xpos = pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos - (FP_FromInteger(47) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }

                field_1CC_lever_pull_range_ypos = pAliveObj->mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(23) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                field_158_obj_id = pAliveObj->field_8_object_id;
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
        pPoint->x = mBaseAnimatedWithPhysicsGameObject_XPos;
        pPoint->y = mBaseAnimatedWithPhysicsGameObject_YPos;
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
