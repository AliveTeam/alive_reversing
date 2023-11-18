#include "stdafx.h"
#include "Scrab.hpp"
#include "../relive_lib/Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "MusicController.hpp"
#include "Abe.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "GameSpeak.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "LiftPoint.hpp"
#include "Slurg.hpp"
#include "Grid.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "QuikSave.hpp"

static const TintEntry sScrabTints_560260[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
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
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

// TODO: repetition with `MainMenu.cpp`
static const relive::SfxDefinition scrab_sScrabSfx_560330[9] = {
    {0u, 4u, 60u, 55u, 0, 0},
    {0u, 4u, 61u, 70u, 0, 0},
    {0u, 4u, 62u, 80u, 0, 0},
    {0u, 4u, 63u, 80u, 0, 0},
    {0u, 4u, 64u, 60u, -127, 127},
    {0u, 4u, 66u, 90u, 0, 0},
    {0u, 4u, 67u, 50u, -511, 0},
    {0u, 4u, 67u, 50u, 0, 511},
    {0u, 4u, 68u, 110u, -1791, -1791},
};


static const TScrabMotionFn sScrabMotionTable[40] = {
#define MAKE_FN(VAR) &Scrab::VAR,
    SCRAB_MOTIONS_ENUM_AE(MAKE_FN)
#undef MAKE_FN
};

static const TScrabBrainFn sScrabBrainTable[6] = {
    &Scrab::Brain_0_Patrol,
    &Scrab::Brain_1_ChasingEnemy,
    &Scrab::Brain_2_Fighting,
    &Scrab::Brain_3_Death,
    &Scrab::Brain_4_ShrinkDeath,
    &Scrab::Brain_5_Possessed};

void Scrab::SetBrain(TScrabBrainFn fn)
{
    mBrainState = fn;
}

bool Scrab::BrainIs(TScrabBrainFn fn)
{
    return mBrainState == fn;
}

void Scrab::LoadAnimations()
{
    for (auto& animId : sScrabMotionAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Scrab::Scrab(relive::Path_Scrab* pTlv, const Guid& tlvId, relive::Path_ScrabSpawner::SpawnDirection spawnDirection)
    : BaseAliveGameObject(14)
{
    field_17C_last_event = -1;

    SetType(ReliveTypes::eScrab);

    if (tlvId != Guid{})
    {
        mBaseGameObjectTlvInfo = tlvId;
    }

    mFightTargetId = Guid{};
    mTargetGuid = Guid{};

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Scrab_Idle));

    field_140_motion_resource_block_index = 0;
    field_12C_timer = 0;
    SetNextMotion(eScrabMotions::Motion_0_Stand);
    BaseAliveGameObject_PlatformId = Guid{};
    SetCurrentMotion(eScrabMotions::Motion_0_Stand);
    field_11E_return_to_previous_motion = 0;

    mAttacking = false;
    mForceUpdateAnimation = false;

    SetCanBePossessed(true);
    SetCanSetOffExplosives(true);
    SetRestoredFromQuickSave(false);

    mPreventDepossession = 0;
    field_16C_input = 0;
    field_1A2_speak_counter = 0;
    mShredPowerActive = 0;
    field_154_movement_timer = 0;

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

    if (spawnDirection == relive::Path_ScrabSpawner::SpawnDirection::eLeft)
    {
        mXPos -= ScaleToGridSize(GetSpriteScale());
    }
    else if (spawnDirection == relive::Path_ScrabSpawner::SpawnDirection::eRight)
    {
        mXPos += ScaleToGridSize(GetSpriteScale());
    }

    mAttackDelay = pTlv->mAttackDelay;
    mPatrolTypeRunOrWalkChance = pTlv->mPatrolTypeRunOrWalkChance;
    mPauseLeftMin = pTlv->mPauseLeftMin;
    mPauseLeftMax = pTlv->mPauseLeftMax;
    mPauseRightMin = pTlv->mPauseRightMin;
    mPauseRightMax = pTlv->mPauseRightMax;
    mPauseAfterChaseTime = pTlv->mPauseAfterChaseTime;
    mPossessedMaxWhirlAttackDuration = pTlv->mPossessedMaxWhirlAttackDuration;
    mKillEnemy = pTlv->mKillEnemy == relive::reliveChoice::eYes ? Choice_short::eYes_1 : Choice_short::eNo_0;

    field_1A0_speak_max = 3;

    mRoarRandomly = pTlv->mRoarRandomly == relive::reliveChoice::eYes ? true : false;
    mPersistant = pTlv->mPersistant == relive::reliveChoice::eYes ? true : false;

    if (!OnFloor())
    {
        SetCurrentMotion(eScrabMotions::Motion_8_JumpToFall);
        vUpdateAnim();
        SetUpdateDelay(30);
    }

    SetTint(&sScrabTints_560260[0], gMap.mCurrentLevel);

    SetDoPurpleLightEffect(true);

    field_14C_pause_after_chase_timer = 0;
    field_150_attack_delay_timer = 0;
    field_144_tlvInfo = tlvId;
    mBrainSubState = 0;

    ToPatrol();

    CreateShadow();
}

void Scrab::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv != nullptr)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            Scrab_SFX(ScrabSounds::eYell_8, 127, -1000, 0);
            SetDead(true);
            mHealth = FP_FromInteger(0);
        }
        else if (pTlv->mTlvType == ReliveTypes::eEnemyStopper)
        {
            const auto enemyStopperPath = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV); //TODO it should probably be pTlv, instead - OG bug?
            const relive::Path_EnemyStopper::StopDirection stopDirection = enemyStopperPath->mStopDirection;
            if ((stopDirection == relive::Path_EnemyStopper::StopDirection::Left && mXPos < field_198_max_xpos) || (stopDirection == relive::Path_EnemyStopper::StopDirection::Right && mXPos > field_198_max_xpos) || stopDirection == relive::Path_EnemyStopper::StopDirection::Both)
            {
                if (SwitchStates_Get(enemyStopperPath->mSwitchId))
                {
                    if (sControlledCharacter != this)
                    {
                        mXPos = field_198_max_xpos;
                    }
                }
            }
        }
        pTlv = gPathInfo->TlvGetAt(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

void Scrab::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<ScrabSaveState>();

    auto pTlv = static_cast<relive::Path_Scrab*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_44_tlvInfo));

    auto pScrab = relive_new Scrab(pTlv, pState->field_44_tlvInfo, relive::Path_ScrabSpawner::SpawnDirection::eNone);
    if (pScrab)
    {
        pScrab->mBaseGameObjectTlvInfo = pState->field_4_obj_id;

        if (pState->mIsControlled)
        {
            sControlledCharacter = pScrab;
        }

        pScrab->BaseAliveGameObjectPathTLV = nullptr;
        pScrab->BaseAliveGameObjectCollisionLine = nullptr;

        pScrab->mXPos = pState->mXPos;
        pScrab->mYPos = pState->mYPos;
        pScrab->mVelX = pState->mVelX;
        pScrab->mVelY = pState->mVelY;

        pScrab->field_134_falling_velx_scale_factor = pState->field_64_falling_velx_scale_factor;
        pScrab->mCurrentPath = pState->mCurrentPath;
        pScrab->mCurrentLevel = pState->mCurrentLevel;
        pScrab->SetSpriteScale(pState->mSpriteScale);
        pScrab->mRGB.SetRGB(pState->mRingRed, pState->mRingGreen, pState->mRingBlue);
        pScrab->SetCurrentMotion(pState->field_28_current_motion);

        const u32 idx = static_cast<u32>(pState->field_28_current_motion);
        pScrab->GetAnimation().Set_Animation_Data(pScrab->GetAnimRes(sScrabMotionAnimIds[idx]));

        pScrab->GetAnimation().SetCurrentFrame(pState->mCurrentFrame);
        pScrab->GetAnimation().SetFrameChangeCounter(pState->mFrameChangeCounter);

        pScrab->SetDrawable(pState->mDrawable & 1);

        pScrab->GetAnimation().SetFlipX(pState->mFlipX & 1);
        pScrab->GetAnimation().SetRender(pState->mAnimRender & 1);

        if (IsLastFrame(&pScrab->GetAnimation()))
        {
            pScrab->GetAnimation().SetIsLastFrame(true);
        }

        pScrab->mHealth = pState->mHealth;
        pScrab->SetCurrentMotion(pState->field_34_current_motion);
        pScrab->SetNextMotion(pState->field_36_next_motion);
        pScrab->BaseAliveGameObjectLastLineYPos = FP_FromInteger(pState->field_38_last_line_ypos);
        pScrab->field_130_depossession_timer = pState->field_60_depossession_timer;
        pScrab->SetRestoredFromQuickSave(true);
        pScrab->field_12C_timer = pState->field_5C_timer;
        pScrab->BaseAliveGameObjectCollisionLineType = pState->mLineType;
        pScrab->field_144_tlvInfo = pState->field_44_tlvInfo;

        pScrab->SetBrain(sScrabBrainTable[pState->field_48_brain_idx]);

        pScrab->mBrainSubState = pState->mBrainSubState;
        pScrab->mTargetGuid = pState->field_54_obj_id;
        pScrab->mFightTargetId = pState->field_58_target_obj_id;

        pScrab->field_140_motion_resource_block_index = pState->field_68_motion_resource_block_index;
        pScrab->field_14C_pause_after_chase_timer = pState->field_6C_spotting_abe_timer;
        pScrab->field_150_attack_delay_timer = pState->field_70_attack_delay_timer;
        pScrab->field_154_movement_timer = pState->field_74_movement_timer;
        pScrab->field_160_sfx_bitmask = pState->field_78_sfx_bitmask;
        pScrab->mPreventDepossession = pState->mPreventDepossession;

        pScrab->mAbeLevel = pState->mAbeLevel;
        pScrab->mAbePath = pState->mAbePath;
        pScrab->mAbeCamera = pState->mAbeCamera;
        pScrab->field_16C_input = InputObject::PsxButtonsToKeyboardInput_45EE40(pState->field_84_input);
        pScrab->mShredPowerActive = pState->field_8C_shred_power_active;
        pScrab->field_194_speak = pState->field_8E_speak;
        pScrab->field_198_max_xpos = pState->field_90_max_xpos;
        pScrab->field_19C_max_ypos = pState->field_94_max_ypos;
        pScrab->field_1A2_speak_counter = pState->field_98_speak_counter;

        pScrab->mAttacking = pState->mAttacking;
        pScrab->mForceUpdateAnimation = pState->mForceUpdateAnimation;
        pScrab->mRoarRandomly = pState->mRoarRandomly;
        pScrab->mPersistant = pState->mPersistant;
    }
}

void Scrab::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return;
    }

    ScrabSaveState data = {};

    data.mType = ReliveTypes::eScrab;
    data.field_4_obj_id = mBaseGameObjectTlvInfo;

    data.mXPos = mXPos;
    data.mYPos = mYPos;
    data.mVelX = mVelX;
    data.mVelY = mVelY;

    data.field_64_falling_velx_scale_factor = field_134_falling_velx_scale_factor;

    data.mCurrentPath = mCurrentPath;
    data.mCurrentLevel = mCurrentLevel;
    data.mSpriteScale = GetSpriteScale();

    data.mRingRed = mRGB.r;
    data.mRingGreen = mRGB.g;
    data.mRingBlue = mRGB.b;

    data.mFlipX = GetAnimation().GetFlipX();
    data.field_28_current_motion = GetCurrentMotion();
    data.mCurrentFrame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    data.mFrameChangeCounter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    data.mDrawable = GetDrawable();
    data.mAnimRender = GetAnimation().GetRender();
    data.mHealth = mHealth;
    data.field_34_current_motion = GetCurrentMotion();
    data.field_36_next_motion = GetNextMotion();
    data.mLineType = eLineTypes::eNone_m1;

    // TODO: Check correctness
    data.field_38_last_line_ypos = FP_GetExponent(BaseAliveGameObjectLastLineYPos);
    if (BaseAliveGameObjectCollisionLine)
    {
        data.mLineType = BaseAliveGameObjectCollisionLine->mLineType;
    }

    data.mIsControlled = (this == sControlledCharacter);
    data.field_60_depossession_timer = field_130_depossession_timer;
    data.field_5C_timer = field_12C_timer;
    data.field_44_tlvInfo = field_144_tlvInfo;
    data.field_48_brain_idx = 0;

    s32 idx = 0;
    for (const auto& fn : sScrabBrainTable)
    {
        if (BrainIs(fn))
        {
            data.field_48_brain_idx = idx;
        }
        idx++;
    }

    data.field_54_obj_id = Guid{};
    data.mBrainSubState = mBrainSubState;

    if (mTargetGuid != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mTargetGuid);
        if (pObj)
        {
            data.field_54_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    data.field_58_target_obj_id = Guid{};
    if (mFightTargetId != Guid{})
    {
        BaseGameObject* pObj = sObjectIds.Find_Impl(mFightTargetId);
        if (pObj)
        {
            data.field_58_target_obj_id = pObj->mBaseGameObjectTlvInfo;
        }
    }

    data.field_68_motion_resource_block_index = field_140_motion_resource_block_index;
    data.field_6C_spotting_abe_timer = field_14C_pause_after_chase_timer;
    data.field_70_attack_delay_timer = field_150_attack_delay_timer;
    data.field_74_movement_timer = field_154_movement_timer;
    data.field_78_sfx_bitmask = field_160_sfx_bitmask;
    data.mPreventDepossession = mPreventDepossession;
    data.mAbeLevel = mAbeLevel;
    data.mAbePath = mAbePath;
    data.mAbeCamera = mAbeCamera;
    data.field_84_input = InputObject::KeyboardInputToPsxButtons_45EF70(field_16C_input);
    data.field_8C_shred_power_active = mShredPowerActive;
    data.field_8E_speak = field_194_speak;
    data.field_90_max_xpos = field_198_max_xpos;
    data.field_94_max_ypos = field_19C_max_ypos;
    data.field_98_speak_counter = field_1A2_speak_counter;

    data.mAttacking = mAttacking;
    data.mForceUpdateAnimation = mForceUpdateAnimation;
    data.mRoarRandomly = mRoarRandomly;
    data.mPersistant = mPersistant;

    pSaveBuffer.Write(data);
}

Scrab::~Scrab()
{
    mTargetGuid = Guid{};
    mFightTargetId = Guid{};

    VOnTrapDoorOpen();

    if (mHealth <= FP_FromInteger(0))
    {
        Path::TLV_Reset(field_144_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_144_tlvInfo, -1, 0, 0);
    }

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

void Scrab::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        mForceUpdateAnimation = true;
        SetCurrentMotion(eScrabMotions::Motion_15_RunToFall);
    }
}

void Scrab::vUpdateAnim()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sScrabMotionAnimIds[mCurrentMotion]));
}

s16 Scrab::OnFloor()
{
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(30),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
    {
        mYPos = hitY;
        ToStand();
        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            PlatformCollide();
        }
        return true;
    }
    return false;
}

const FP velx_input_entries_546D84[8] = {
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4)};

const FP vely_input_entries_546DA4[11] = {
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)};


void Scrab::HandleDDCheat()
{
    BaseAliveGameObjectCollisionLine = nullptr;

    if (Input().IsAnyHeld(InputCommands::eUp | InputCommands::eDown | InputCommands::eLeft | InputCommands::eRight))
    {
        // TODO: InputCommand constants
        mVelX = velx_input_entries_546D84[Input().mPads[sCurrentControllerIndex].mDir >> 5];
        mVelY = vely_input_entries_546DA4[Input().mPads[sCurrentControllerIndex].mDir >> 5];

        if (Input().IsAnyHeld(InputCommands::eRun))
        {
            mVelX += velx_input_entries_546D84[Input().mPads[sCurrentControllerIndex].mDir >> 5];
            mVelX += velx_input_entries_546D84[Input().mPads[sCurrentControllerIndex].mDir >> 5];
            mVelY += vely_input_entries_546DA4[Input().mPads[sCurrentControllerIndex].mDir >> 5];
        }

        mXPos += mVelX;
        mYPos += mVelY;

        // Keep in map bounds
        PSX_Point point = {};
        gMap.Get_map_size(&point);
        if (mXPos < FP_FromInteger(0))
        {
            mXPos = FP_FromInteger(0);
        }

        if (mXPos >= FP_FromInteger(point.x))
        {
            mXPos = FP_FromInteger(point.x) - FP_FromInteger(1);
        }

        if (mYPos < FP_FromInteger(0))
        {
            mYPos = FP_FromInteger(0);
        }

        if (mYPos >= FP_FromInteger(point.y))
        {
            mYPos = FP_FromInteger(point.y) - FP_FromInteger(1);
        }
    }
    else
    {
        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);
    }

    SetActiveCameraDelayedFromDir();
    BaseAliveGameObjectLastLineYPos = mYPos;
}

void Scrab::VUpdate()
{
    if (GetRestoredFromQuickSave())
    {
        SetRestoredFromQuickSave(false);
        if (BaseAliveGameObjectCollisionLineType == -1)
        {
            BaseAliveGameObjectCollisionLine = nullptr;
        }
        else
        {
            gCollisions->Raycast(
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
        mTargetGuid = BaseGameObject::RefreshId(mTargetGuid);
        mFightTargetId = BaseGameObject::RefreshId(mFightTargetId);
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
        return;
    }

    const FP xDelta = FP_Abs(mXPos - sControlledCharacter->mXPos);
    if (xDelta <= FP_FromInteger(750))
    {
        const FP yDelta = FP_Abs(mYPos - sControlledCharacter->mYPos);
        if (yDelta <= FP_FromInteger(520))
        {
            if (mHealth > FP_FromInteger(0))
            {
                GetAnimation().SetAnimate(true);
                GetAnimation().SetRender(true);
            }

            if (!Input_IsChanting())
            {
                mPreventDepossession = 0;
            }

            if (gDDCheat_FlyingEnabled && sControlledCharacter == this)
            {
                HandleDDCheat();
                return;
            }

            if (!gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    1))
            {
                field_1A2_speak_counter = 0;
            }

            const auto oldMotion = mCurrentMotion;

            mBrainSubState = (this->*mBrainState)();

            if (gDDCheat_ShowAI_Info)
            {
                DDCheat::DebugStr(
                    "Scrab %d %d %d %d\n",
                    mBrainSubState,
                    field_12C_timer,
                    mCurrentMotion,
                    mNextMotion);
            }

            field_19C_max_ypos = mYPos;
            field_198_max_xpos = mXPos;

            InvokeMemberFunction(this, sScrabMotionTable, mCurrentMotion);

            if (field_198_max_xpos != mXPos || field_19C_max_ypos != mYPos)
            {
                BaseAliveGameObjectPathTLV = gPathInfo->TlvGetAt(
                    nullptr,
                    mXPos,
                    mYPos,
                    mXPos,
                    mYPos);
                VOnTlvCollision(BaseAliveGameObjectPathTLV);
            }

            if (oldMotion != mCurrentMotion || mForceUpdateAnimation)
            {
                mForceUpdateAnimation = false;
                vUpdateAnim();
            }
            else if (field_11E_return_to_previous_motion)
            {
                // NOTE: This never gets set to true, so this branch will never be executed.
                mCurrentMotion = mPreviousMotion;
                vUpdateAnim();
                GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                field_11E_return_to_previous_motion = 0;
            }

            Update_Slurg_Step_Watch_Points();

            if (mShredPowerActive)
            {
                if (sControlledCharacter != this)
                {
                    field_13C_last_ypos = mYPos;
                    return;
                }

                if (mHealth > FP_FromInteger(0) && !(static_cast<s32>(sGnFrame) % 4))
                {
                    // Draw green particle
                    s16 particleXRand = Math_RandomRange(40, 50);
                    const s16 particleYRand = Math_RandomRange(45, 55);
                    FP particleVelX = (FP_FromInteger(-7) * GetSpriteScale());
                    FP particleVelY = (FP_FromDouble(0.3) * GetSpriteScale());

                    if (Math_NextRandom() & 1)
                    {
                        particleVelX = -particleVelX;
                        particleXRand = -particleXRand;
                    }

                    if (Math_NextRandom() & 1)
                    {
                        particleVelY = -particleVelY;
                    }

                    const FP particleYPos = mYPos - (GetSpriteScale() * FP_FromInteger(particleYRand));
                    const FP particleXPos = mXPos + (GetSpriteScale() * FP_FromInteger(particleXRand));

                    New_Orb_Particle(
                        particleXPos,
                        particleYPos,
                        particleVelX,
                        particleVelY,
                        GetSpriteScale(),
                        Layer::eLayer_0,
                        RGB16{255, 0, 0});
                }
            }

            if (sControlledCharacter == this && BaseAliveGameObject_PlatformId != Guid{})
            {
                mVelY = mYPos - field_13C_last_ypos;
                SetActiveCameraDelayedFromDir();
            }
            field_13C_last_ypos = mYPos;
            return;
        }
    }

    if (mPersistant)
    {
        GetAnimation().SetAnimate(false);
        GetAnimation().SetRender(false);
    }
    else
    {
        SetDead(true);
    }
}

void Scrab::Update_Slurg_Step_Watch_Points()
{
    if (GetCurrentMotion() == eScrabMotions::Motion_1_Walk || GetCurrentMotion() == eScrabMotions::Motion_2_Run || GetCurrentMotion() == eScrabMotions::Motion_21_Stamp || GetCurrentMotion() == eScrabMotions::Motion_32_AttackSpin)
    {
        if (sGnFrame & 1)
        {
            const s8 count = gSlurgStepWatchPointsCount[gSlurgStepWatchPointsIdx];
            if (count < 5)
            {
                Slurg_Step_Watch_Points* pPoints = &gSlurgStepWatchPoints[gSlurgStepWatchPointsIdx];
                pPoints->mPoints[count].x = FP_GetExponent(mXPos);
                pPoints->mPoints[count].y = BaseAliveGameObjectCollisionLine->mRect.y - 5;
                gSlurgStepWatchPointsCount[gSlurgStepWatchPointsIdx] = count + 1;
            }
        }
    }
}

enum Scrab_Brain_0_Patrol
{
    eBrain0_ToMoving_0 = 0,
    eBrain0_Moving_1 = 1,
    eBrain0_Turning_2 = 2,
    eBrain0_Idle_3 = 3,
    eBrain0_Howling_4 = 4,
    eBrain0_Shriek_5 = 5,
    eBrain0_OnLift_6 = 6,
    eBrain0_KickFleech_7 = 7,
    eBrain0_ToSpeak_8 = 8,
    eBrain0_UsingInvisibility_9 = 9,
    eBrain0_PostInvisibilitySmash_10 = 10,
    eBrain0_BeingSpawned_11 = 11,
};

enum Brain_1_ChasingEnemy
{
    eBrain1_Inactive_0 = 0,
    eBrain1_Idle_1 = 1,
    eBrain1_Running_2 = 2,
    eBrain1_Turning_3 = 3,
    eBrain1_Panic_4 = 4,
    eBrain1_Falling_5 = 5,
    eBrain1_ToIdle_6 = 6,
    eBrain1_Jumping_7 = 7,
    eBrain1_Attacking_8 = 8,
    eBrain1_Walking_9 = 9,
    eBrain1_EnemyDead_10 = 10,
    eBrain1_SmashingEnemy_11 = 11,
    eBrain1_Eating_12 = 12,
    eBrain1_Howl_13 = 13,
    eBrain1_Shriek_14 = 14,
    eBrain1_PreparingToHowlOrShriek_15 = 15,
    eBrain1_KilledPossessedScrab_16 = 16
};

enum Brain_2_Fighting
{
    eBrain2_LookingForOpponent_0 = 0,
    eBrain2_SpottedOpponent_1 = 1,
    eBrain2_Turning_2 = 2,
    eBrain2_Walking_3 = 3,
    eBrain2_SetInPosition_4 = 4,
    eBrain2_Idle_5 = 5,
    eBrain2_Yelling_6 = 6,
    eBrain2_Shriek_7 = 7,
    eBrain2_ToRun_8 = 8,
    eBrain2_Running_9 = 9,
    eBrain2_Battling_10 = 10,
    eBrain2_Victorious_11 = 11,
    eBrain2_SmashingOpponent_12 = 12,
    eBrain2_VictoryYell_13 = 13,
    eBrain2_InterruptVictoryStates_14 = 14,
    eBrain2_WaitingForBattle_15 = 15
};

static const s32 kPatrolTypeRunOrWalkChance_546FD8[6] = {256, 192, 128, 64, 0, 0};

s16 Scrab::Brain_0_Patrol()
{
    Scrab* pFighter = FindScrabToFight();
    if (pFighter)
    {
        SetBrain(&Scrab::Brain_2_Fighting);
        mFightTargetId = pFighter->mBaseGameObjectId;
        SetNextMotion(eScrabMotions::Motion_0_Stand);
        return Brain_2_Fighting::eBrain2_LookingForOpponent_0;
    }

    if (FindAbeOrMud())
    {
        auto pOtherScrab = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
        SetBrain(&Scrab::Brain_1_ChasingEnemy);
        if (pOtherScrab->Type() == ReliveTypes::eScrab && pOtherScrab->GetPossessed())
        {
            SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
            field_150_attack_delay_timer = MakeTimer(90);
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;
        }

        SetNextMotion(eScrabMotions::Motion_0_Stand);
        return Brain_1_ChasingEnemy::eBrain1_Inactive_0;
    }

    if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, EventScale::Both) && !sActiveHero->GetInvisible())
    {
        SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
        return Scrab_Brain_0_Patrol::eBrain0_Howling_4;
    }

    auto pSwitch = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pSwitch && pSwitch->Type() == ReliveTypes::eLiftPoint && !(static_cast<LiftPoint*>(pSwitch)->vOnAnyFloor() || mBrainSubState != Scrab_Brain_0_Patrol::eBrain0_OnLift_6))
    {
        SetNextMotion(eScrabMotions::Motion_0_Stand);
        return Scrab_Brain_0_Patrol::eBrain0_OnLift_6;
    }

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    if (GetCurrentMotion() == eScrabMotions::Motion_8_JumpToFall && mBrainSubState != Scrab_Brain_0_Patrol::eBrain0_BeingSpawned_11)
    {
        mBrainSubState = Scrab_Brain_0_Patrol::eBrain0_BeingSpawned_11;
    }

    switch (mBrainSubState)
    {
        case Scrab_Brain_0_Patrol::eBrain0_ToMoving_0:
            if (GetCurrentMotion() == eScrabMotions::Motion_0_Stand)
            {
                field_194_speak = LastSpeak();
                if (field_1A2_speak_counter < field_1A0_speak_max && (field_194_speak == GameSpeakEvents::eScrab_Howl || field_194_speak == GameSpeakEvents::eScrab_Shriek))
                {
                    return Scrab_Brain_0_Patrol::eBrain0_ToSpeak_8;
                }
            }

            if (Check_IsOnEndOfLine(
                    GetAnimation().GetFlipX(),
                    GetCurrentMotion() != eScrabMotions::Motion_2_Run ? 1 : 3)
                || Handle_SlamDoor_or_EnemyStopper(mVelX, 1))
            {
                SetNextMotion(eScrabMotions::Motion_3_Turn);
                return Scrab_Brain_0_Patrol::eBrain0_Turning_2;
            }

            if (Math_NextRandom() >= kPatrolTypeRunOrWalkChance_546FD8[mPatrolTypeRunOrWalkChance])
            {
                SetNextMotion(eScrabMotions::Motion_2_Run);
            }
            else
            {
                SetNextMotion(eScrabMotions::Motion_1_Walk);
            }
            return Scrab_Brain_0_Patrol::eBrain0_Moving_1;

        case Scrab_Brain_0_Patrol::eBrain0_Moving_1:
            field_194_speak = Scrab::LastSpeak();
            if (field_1A2_speak_counter >= field_1A0_speak_max || (field_194_speak != GameSpeakEvents::eScrab_Howl && field_194_speak != GameSpeakEvents::eScrab_Shriek))
            {
                if (Find_Fleech())
                {
                    SetNextMotion(eScrabMotions::Motion_38_LegKick);
                    return Scrab_Brain_0_Patrol::eBrain0_KickFleech_7;
                }

                if (Check_IsOnEndOfLine(
                        GetAnimation().GetFlipX(),
                        GetCurrentMotion() != eScrabMotions::Motion_2_Run ? 1 : 3)
                    || // TODO: check order is correct
                    Handle_SlamDoor_or_EnemyStopper(mVelX, 1))
                {
                    SetNextMotion(eScrabMotions::Motion_3_Turn);
                    return Scrab_Brain_0_Patrol::eBrain0_Turning_2;
                }

                if (Math_NextRandom() >= 3u || static_cast<s32>(sGnFrame - field_154_movement_timer) <= 150)
                {
                    return mBrainSubState;
                }

                SetNextMotion(eScrabMotions::Motion_30_Shriek);
                field_154_movement_timer = sGnFrame;
                return Scrab_Brain_0_Patrol::eBrain0_Shriek_5;
            }
            else
            {
                SetNextMotion(eScrabMotions::Motion_0_Stand);
                return Scrab_Brain_0_Patrol::eBrain0_ToSpeak_8;
            }
            break;

        case Scrab_Brain_0_Patrol::eBrain0_Turning_2:
        {
            if (GetCurrentMotion() != eScrabMotions::Motion_3_Turn || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            SetNextMotion(eScrabMotions::Motion_0_Stand);

            s16 maxTimer = 0;
            s16 minTimer = 0;
            if (GetAnimation().GetFlipX())
            {
                maxTimer = mPauseLeftMax;
                minTimer = mPauseLeftMin;
            }
            else
            {
                maxTimer = mPauseRightMax;
                minTimer = mPauseRightMin;
            }
            field_12C_timer = MakeTimer(Math_RandomRange(minTimer, maxTimer));
            return Scrab_Brain_0_Patrol::eBrain0_Idle_3;
        }

        case Scrab_Brain_0_Patrol::eBrain0_Idle_3:
            field_194_speak = LastSpeak();
            if (field_1A2_speak_counter >= field_1A0_speak_max || (field_194_speak != GameSpeakEvents::eScrab_Howl && field_194_speak != GameSpeakEvents::eScrab_Shriek))
            {
                if (Find_Fleech())
                {
                    SetNextMotion(eScrabMotions::Motion_38_LegKick);
                    return Scrab_Brain_0_Patrol::eBrain0_KickFleech_7;
                }
                else
                {
                    if (static_cast<s32>(sGnFrame) <= field_12C_timer)
                    {
                        return mBrainSubState;
                    }

                    if (Math_NextRandom() >= 30u || static_cast<s32>(sGnFrame - field_154_movement_timer) <= 150)
                    {
                        return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
                    }
                    SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
                    field_154_movement_timer = sGnFrame;
                    return Scrab_Brain_0_Patrol::eBrain0_Howling_4;
                }
            }
            else
            {
                SetNextMotion(eScrabMotions::Motion_0_Stand);
                return Scrab_Brain_0_Patrol::eBrain0_ToSpeak_8;
            }
            break;

        case Scrab_Brain_0_Patrol::eBrain0_Howling_4:
            if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, EventScale::Both))
            {
                return mBrainSubState;
            }
            if (GetCurrentMotion() == eScrabMotions::Motion_26_HowlBegin && GetAnimation().GetIsLastFrame())
            {
                SetNextMotion(eScrabMotions::Motion_0_Stand);
                return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            return mBrainSubState;

        case Scrab_Brain_0_Patrol::eBrain0_Shriek_5:
            if (GetCurrentMotion() == eScrabMotions::Motion_30_Shriek && GetAnimation().GetIsLastFrame())
            {
                SetNextMotion(eScrabMotions::Motion_0_Stand);
                return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            return mBrainSubState;

        case Scrab_Brain_0_Patrol::eBrain0_OnLift_6:
            if (pSwitch)
            {
                if (!static_cast<LiftPoint*>(pSwitch)->vOnAnyFloor())
                {
                    return mBrainSubState;
                }
                return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            else
            {
                BaseAliveGameObject_PlatformId = Guid{};
                return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            break;

        case Scrab_Brain_0_Patrol::eBrain0_KickFleech_7:
            if (GetCurrentMotion() == eScrabMotions::Motion_38_LegKick && GetAnimation().GetIsLastFrame())
            {
                SetNextMotion(eScrabMotions::Motion_0_Stand);
                return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            return mBrainSubState;


        case Scrab_Brain_0_Patrol::eBrain0_ToSpeak_8:
            if (GetCurrentMotion() != eScrabMotions::Motion_0_Stand)
            {
                return mBrainSubState;
            }

            field_1A2_speak_counter++;

            if (field_194_speak == GameSpeakEvents::eScrab_Shriek)
            {
                SetNextMotion(eScrabMotions::Motion_30_Shriek);
                field_154_movement_timer = sGnFrame;
                return Scrab_Brain_0_Patrol::eBrain0_Shriek_5;
            }
            else if (field_194_speak == GameSpeakEvents::eScrab_Howl)
            {
                SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
                return Scrab_Brain_0_Patrol::eBrain0_Howling_4;
            }
            else
            {
                return mBrainSubState;
            }
            break;

        case Scrab_Brain_0_Patrol::eBrain0_UsingInvisibility_9:
            if (GetCurrentMotion() != eScrabMotions::Motion_26_HowlBegin || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_21_Stamp);
            field_12C_timer = MakeTimer(30);
            return Scrab_Brain_0_Patrol::eBrain0_PostInvisibilitySmash_10;

        case Scrab_Brain_0_Patrol::eBrain0_PostInvisibilitySmash_10:
            if (GetAnimation().GetIsLastFrame() && static_cast<s32>(sGnFrame) > field_12C_timer)
            {
                SetNextMotion(eScrabMotions::Motion_0_Stand);
                return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            return mBrainSubState;

        case Scrab_Brain_0_Patrol::eBrain0_BeingSpawned_11:
            if (GetCurrentMotion() != eScrabMotions::Motion_0_Stand)
            {
                return mBrainSubState;
            }
            return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;

        default:
            return mBrainSubState;
    }
}

s16 Scrab::Brain_1_ChasingEnemy()
{
    Scrab* pScrabToFight = FindScrabToFight();
    if (pScrabToFight)
    {
        SetBrain(&Scrab::Brain_2_Fighting);
        mFightTargetId = pScrabToFight->mBaseGameObjectId;
        SetNextMotion(eScrabMotions::Motion_0_Stand);
        return Brain_2_Fighting::eBrain2_LookingForOpponent_0;
    }

    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    if (!pObj || GetDead() || (static_cast<s32>(sGnFrame) > field_14C_pause_after_chase_timer && !CanSeeAbe(pObj)))
    {
        mTargetGuid = Guid{};
        SetNextMotion(eScrabMotions::Motion_0_Stand);
        ToPatrol();
        return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
    }

    if (pObj->GetInvisible())
    {
        mTargetGuid = Guid{};
        SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
        ToPatrol();
        return Scrab_Brain_0_Patrol::eBrain0_UsingInvisibility_9;
    }

    if (CanSeeAbe(pObj))
    {
        field_14C_pause_after_chase_timer = MakeTimer(mPauseAfterChaseTime);
    }

    if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, EventScale::Both) && mBrainSubState != 26) //TODO OG bug? mNextMotion instead of field_11C_sub_state
    {
        SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
        return Brain_1_ChasingEnemy::eBrain1_Panic_4;
    }

    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pLiftPoint && pLiftPoint->Type() != ReliveTypes::eLiftPoint)
    {
        pLiftPoint = nullptr; //OG bug fix: Before it could use the pointer as a LiftPoint even if it, in fact, wasn't one
    }
    if (pLiftPoint && !pLiftPoint->vOnAnyFloor() && mBrainSubState != Brain_1_ChasingEnemy::eBrain1_Panic_4)
    {
        SetNextMotion(eScrabMotions::Motion_0_Stand);
        return Brain_1_ChasingEnemy::eBrain1_Panic_4;
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case Brain_1_ChasingEnemy::eBrain1_Inactive_0:
        {
            field_150_attack_delay_timer = MakeTimer(mAttackDelay);
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;
        }
        case Brain_1_ChasingEnemy::eBrain1_Idle_1:
        {
            if (GetCurrentMotion() == eScrabMotions::Motion_0_Stand)
            {
                field_194_speak = LastSpeak();
                if (field_1A2_speak_counter < field_1A0_speak_max
                    && (LastSpeak() == GameSpeakEvents::eScrab_Howl
                        || LastSpeak() == GameSpeakEvents::eScrab_Shriek))
                {
                    return Brain_1_ChasingEnemy::eBrain1_PreparingToHowlOrShriek_15;
                }
            }

            if (!VIsFacingMe(pObj))
            {
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                {
                    SetNextMotion(eScrabMotions::Motion_3_Turn);
                }
                else
                {
                    SetCurrentMotion(eScrabMotions::Motion_3_Turn);
                    mNextMotion = -1;
                    MapFollowMe(true);
                }
                return Brain_1_ChasingEnemy::eBrain1_Turning_3;
            }

            if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(1.5), pObj)
                && VOnSameYLevel(pObj))
            {
                if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()), pObj))
                {
                    SetNextMotion(eScrabMotions::Motion_37_AttackLunge);
                    return Brain_1_ChasingEnemy::eBrain1_Attacking_8;
                }
                SetNextMotion(eScrabMotions::Motion_38_LegKick);
                return Brain_1_ChasingEnemy::eBrain1_Attacking_8;
            }
            else if (CanSeeAbe(pObj) && static_cast<s32>(sGnFrame) >= field_150_attack_delay_timer)
            {
                if (Handle_SlamDoor_or_EnemyStopper(mVelX, 0))
                {
                    return mBrainSubState;
                }
                SetNextMotion(eScrabMotions::Motion_2_Run);
                return Brain_1_ChasingEnemy::eBrain1_Running_2;
            }
            else
            {
                if (Math_NextRandom() >= 16u || static_cast<s32>(sGnFrame - field_154_movement_timer) <= 60)
                {
                    return Brain_1_ChasingEnemy::eBrain1_Idle_1;
                }
                SetNextMotion(eScrabMotions::Motion_30_Shriek);
                field_154_movement_timer = sGnFrame;
                return Brain_1_ChasingEnemy::eBrain1_Shriek_14;
            }
        }
        case Brain_1_ChasingEnemy::eBrain1_Running_2:
            return Brain_ChasingEnemy_State_2_Running(pObj);

        case Brain_1_ChasingEnemy::eBrain1_Turning_3:
            if (GetCurrentMotion() == eScrabMotions::Motion_3_Turn)
            {
                if (!GetAnimation().GetIsLastFrame())
                {
                    return mBrainSubState;
                }
                SetNextMotion(eScrabMotions::Motion_0_Stand);
                return Brain_1_ChasingEnemy::eBrain1_Idle_1;
            }

            if (GetNextMotion() != eScrabMotions::Motion_3_Turn)
            {
                SetNextMotion(eScrabMotions::Motion_3_Turn);
            }

            if (GetCurrentMotion() != eScrabMotions::Motion_30_Shriek)
            {
                return mBrainSubState;
            }

            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Panic_4:
            if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()), pObj)
                && VOnSameYLevel(pObj))
            {
                SetNextMotion(eScrabMotions::Motion_38_LegKick);
                return Brain_1_ChasingEnemy::eBrain1_Attacking_8;
            }

            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor())
                {
                    return mBrainSubState;
                }
            }
            else
            {
                BaseAliveGameObject_PlatformId = Guid{};
            }
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Falling_5:
        case Brain_1_ChasingEnemy::eBrain1_ToIdle_6:
            if (mCurrentMotion)
            {
                return mBrainSubState;
            }
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Jumping_7:
            if (GetCurrentMotion() != eScrabMotions::Motion_13_RunJumpEnd)
            {
                return mBrainSubState;
            }
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Attacking_8:
            if ((GetCurrentMotion() != eScrabMotions::Motion_37_AttackLunge
                 && GetCurrentMotion() != eScrabMotions::Motion_38_LegKick)
                || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (pObj->mHealth > FP_FromInteger(0))
            {
                return Brain_1_ChasingEnemy::eBrain1_Idle_1;
                break;
            }

            if (VIsFacingMe(pObj))
            {
                if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()), pObj))
                {
                    if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()), pObj))
                    {
                        SetNextMotion(eScrabMotions::Motion_1_Walk);
                    }
                }
                return Brain_1_ChasingEnemy::eBrain1_Walking_9;
            }
            else
            {
                SetNextMotion(eScrabMotions::Motion_3_Turn);
                return Brain_1_ChasingEnemy::eBrain1_EnemyDead_10;
            }

        case Brain_1_ChasingEnemy::eBrain1_Walking_9:
            if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()), pObj))
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_21_Stamp);
            field_12C_timer = MakeTimer(30);
            return Brain_1_ChasingEnemy::eBrain1_SmashingEnemy_11;

        case Brain_1_ChasingEnemy::eBrain1_EnemyDead_10:
            if (GetCurrentMotion() != eScrabMotions::Motion_3_Turn || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()), pObj))
            {
                SetNextMotion(eScrabMotions::Motion_1_Walk);
            }
            return Brain_1_ChasingEnemy::eBrain1_Walking_9;

        case Brain_1_ChasingEnemy::eBrain1_SmashingEnemy_11:
            if (static_cast<s32>(sGnFrame) <= field_12C_timer)
            {
                return mBrainSubState;
            }

            if (pObj->mYPos - mYPos < FP_FromInteger(0))
            {
                pObj->mYPos - mYPos = -(pObj->mYPos - mYPos);
            }

            if (pObj->mYPos - mYPos >= FP_FromInteger(5))
            {
                SetNextMotion(eScrabMotions::Motion_30_Shriek);
                return Brain_1_ChasingEnemy::eBrain1_Shriek_14;
            }
            else
            {
                SetNextMotion(eScrabMotions::Motion_35_StandToFeed);
                return Brain_1_ChasingEnemy::eBrain1_Eating_12;
            }

        case Brain_1_ChasingEnemy::eBrain1_Eating_12:
            if (GetCurrentMotion() == eScrabMotions::Motion_33_FeedToGulp)
            {
                if (GetAnimation().GetIsLastFrame())
                {
                    SetNextMotion(eScrabMotions::Motion_35_StandToFeed);
                }
            }
            return mBrainSubState;

        case Brain_1_ChasingEnemy::eBrain1_Howl_13:
            if (IsEventInRange(kEventAbeOhm, mXPos, mYPos, EventScale::Both))
            {
                return mBrainSubState;
            }

            SetCurrentMotion(eScrabMotions::Motion_26_HowlBegin);
            if (GetCurrentMotion() != eScrabMotions::Motion_30_Shriek)
            {
                return mBrainSubState;
            }
            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Shriek_14:
            if (GetCurrentMotion() != eScrabMotions::Motion_30_Shriek)
            {
                return mBrainSubState;
            }

            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_PreparingToHowlOrShriek_15:
            if (mCurrentMotion)
            {
                return mBrainSubState;
            }

            field_1A2_speak_counter++;
            if (field_194_speak == GameSpeakEvents::eScrab_Shriek)
            {
                SetNextMotion(eScrabMotions::Motion_30_Shriek);
                field_154_movement_timer = sGnFrame;
                return Brain_1_ChasingEnemy::eBrain1_Shriek_14;
            }

            if (field_194_speak != GameSpeakEvents::eScrab_Howl)
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
            return Brain_1_ChasingEnemy::eBrain1_Howl_13;

        case Brain_1_ChasingEnemy::eBrain1_KilledPossessedScrab_16:
            if (GetCurrentMotion() == eScrabMotions::Motion_8_JumpToFall)
            {
                return Brain_1_ChasingEnemy::eBrain1_Falling_5;
            }
            else if (GetCurrentMotion() == eScrabMotions::Motion_32_AttackSpin)
            {
                return mBrainSubState;
            }
            else
            {
                return Brain_1_ChasingEnemy::eBrain1_Idle_1;
            }
        default:
            return mBrainSubState;
    }
}

s16 Scrab::Brain_ChasingEnemy_State_2_Running(BaseAliveGameObject* pObj)
{
    field_194_speak = LastSpeak();
    if (field_1A2_speak_counter < field_1A0_speak_max
        && (LastSpeak() == GameSpeakEvents::eScrab_Howl
            || LastSpeak() == GameSpeakEvents::eScrab_Shriek))
    {
        SetNextMotion(eScrabMotions::Motion_0_Stand);
        return Brain_1_ChasingEnemy::eBrain1_PreparingToHowlOrShriek_15;
    }

    if (Handle_SlamDoor_or_EnemyStopper(mVelX, 0))
    {
        SetNextMotion(eScrabMotions::Motion_0_Stand);
        return Brain_1_ChasingEnemy::eBrain1_Idle_1;
    }

    s32 xPosition = abs(FP_GetExponent(mXPos) - SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos)));

    //Jump at the end of the ledge
    FP xOffset = (mVelX >= FP_FromInteger(0)) ? ScaleToGridSize(GetSpriteScale()) : -ScaleToGridSize(GetSpriteScale());
    if (xPosition < 6
        && Check_IsOnEndOfLine(mVelX < FP_FromInteger(0), 1)
        && ((pObj->mYPos - mYPos < FP_FromInteger(5))
            || gPathInfo->TLV_Get_At(
                FP_GetExponent(mXPos + xOffset),
                FP_GetExponent(mYPos + FP_FromInteger(10)),
                FP_GetExponent(mXPos + xOffset),
                FP_GetExponent(mYPos + FP_FromInteger(10)),
                ReliveTypes::eElectricWall))
        && !Check_IsOnEndOfLine(mVelX < FP_FromInteger(0), 3))
    {
        ToJump();
        mNextMotion = -1;
        return Brain_1_ChasingEnemy::eBrain1_Jumping_7;
    }

    if (VIsFacingMe(pObj))
    {
        if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(7), pObj)
            && pObj->GetPossessed()
            && pObj->Type() == ReliveTypes::eScrab)
        {
            if (!mShredPowerActive)
            {
                SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
                return Brain_1_ChasingEnemy::eBrain1_Howl_13;
            }
            if (!Math_NextRandom())
            {
                SetNextMotion(eScrabMotions::Motion_32_AttackSpin);
                return Brain_1_ChasingEnemy::eBrain1_KilledPossessedScrab_16;
            }
        }

        if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pObj)
            && GetCurrentMotion() == eScrabMotions::Motion_2_Run
            && VOnSameYLevel(pObj))
        {
            if (WallHit(GetSpriteScale() * FP_FromInteger(45), pObj->mXPos - mXPos))
            {
                SetNextMotion(eScrabMotions::Motion_0_Stand);
                return Brain_1_ChasingEnemy::eBrain1_Idle_1;
            }
            else
            {
                SetNextMotion(eScrabMotions::Motion_37_AttackLunge);
                return Brain_1_ChasingEnemy::eBrain1_Attacking_8;
            }
        }
        else
        {
            if (GetCurrentMotion() != eScrabMotions::Motion_8_JumpToFall)
            {
                //A patch workaround: This situation should probably never happen in the first place, but since it does...
                if (GetCurrentMotion() == eScrabMotions::Motion_0_Stand && mNextMotion == -1)
                {
                    SetNextMotion(eScrabMotions::Motion_2_Run);
                }
                return mBrainSubState;
            }
            return Brain_1_ChasingEnemy::eBrain1_Falling_5;
        }
    }
    else
    {
        if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
        {
            SetNextMotion(eScrabMotions::Motion_3_Turn);
        }
        else
        {
            SetCurrentMotion(eScrabMotions::Motion_3_Turn);
            mNextMotion = -1;
            MapFollowMe(true);
        }
        return Brain_1_ChasingEnemy::eBrain1_Turning_3;
    }
}

s16 Scrab::Brain_2_Fighting()
{
    auto pTarget = static_cast<Scrab*>(sObjectIds.Find_Impl(mFightTargetId));
    if (mBrainSubState != Brain_2_Fighting::eBrain2_Battling_10 && mBrainSubState != Brain_2_Fighting::eBrain2_Victorious_11 && mBrainSubState != Brain_2_Fighting::eBrain2_SmashingOpponent_12 && mBrainSubState != Brain_2_Fighting::eBrain2_VictoryYell_13 && (!pTarget || pTarget->GetDead() || (WallHit(GetSpriteScale() * FP_FromInteger(45), pTarget->mXPos - mXPos)) || !VOnSameYLevel(pTarget)))
    {
        SetNextMotion(eScrabMotions::Motion_0_Stand);
        ToPatrol();
        return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
    }

    if (pTarget && pTarget->GetPossessed() && mHealth > FP_FromInteger(0) && pTarget->mHealth > FP_FromInteger(0))
    {
        mTargetGuid = mFightTargetId;
        mFightTargetId = Guid{};
        SetBrain(&Scrab::Brain_1_ChasingEnemy);
        field_150_attack_delay_timer = MakeTimer(90);
        SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
        return Brain_2_Fighting::eBrain2_SpottedOpponent_1;
    }

    if (EventGet(kEventAbeOhm))
    {
        if (mBrainSubState != Brain_2_Fighting::eBrain2_Running_9 && mBrainSubState != Brain_2_Fighting::eBrain2_Battling_10)
        {
            if (mBrainSubState != Brain_2_Fighting::eBrain2_Victorious_11 && mBrainSubState != Brain_2_Fighting::eBrain2_SmashingOpponent_12 && mBrainSubState != Brain_2_Fighting::eBrain2_VictoryYell_13)
            {
                SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
                ToPatrol();
                return Scrab_Brain_0_Patrol::eBrain0_Howling_4;
            }

            if (pTarget->mBrainSubState != Brain_2_Fighting::eBrain2_Running_9 && mNextMotion == -1)
            {
                mBrainSubState = Brain_2_Fighting::eBrain2_InterruptVictoryStates_14;
                SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
            }
        }
    }

    if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    switch (mBrainSubState)
    {
        case Brain_2_Fighting::eBrain2_LookingForOpponent_0:
            if (mCurrentMotion)
            {
                if (GetNextMotion() != eScrabMotions::Motion_0_Stand)
                {
                    SetNextMotion(eScrabMotions::Motion_0_Stand);
                }
                return Brain_2_Fighting::eBrain2_LookingForOpponent_0;
            }

            mAttacking = false;
            mNextMotion = -1;
            if (pTarget->mFightTargetId == Guid{} || pTarget->mFightTargetId == mBaseGameObjectId)
            {
                if (VIsFacingMe(pTarget))
                {
                    if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(8), pTarget))
                    {
                        if (pTarget->GetCurrentMotion() == eScrabMotions::Motion_26_HowlBegin)
                        {
                            return mBrainSubState;
                        }
                        SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
                        return Brain_2_Fighting::eBrain2_Yelling_6;
                    }
                    SetNextMotion(eScrabMotions::Motion_3_Turn);
                    return Brain_2_Fighting::eBrain2_Turning_2;
                }
                else
                {
                    SetNextMotion(eScrabMotions::Motion_3_Turn);
                    return Brain_2_Fighting::eBrain2_SpottedOpponent_1;
                }
            }
            else
            {
                mFightTargetId = Guid{};
                SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
                return Brain_2_Fighting::eBrain2_WaitingForBattle_15;
            }
            break;

        case Brain_2_Fighting::eBrain2_SpottedOpponent_1:
            if (GetCurrentMotion() != eScrabMotions::Motion_3_Turn || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(8), pTarget))
            {
                SetNextMotion(eScrabMotions::Motion_3_Turn);
                return Brain_2_Fighting::eBrain2_Turning_2;
            }
            else
            {
                SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
                return Brain_2_Fighting::eBrain2_Yelling_6;
            }
            break;

        case Brain_2_Fighting::eBrain2_Turning_2:
            if (GetCurrentMotion() != eScrabMotions::Motion_3_Turn || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_1_Walk);
            return Brain_2_Fighting::eBrain2_Walking_3;

        case Brain_2_Fighting::eBrain2_Walking_3:
        {
            auto xPos = mXPos;
            s16 sIsFlipped = 0;
            if (GetAnimation().GetFlipX())
            {
                sIsFlipped = 1;
                xPos -= FP_FromInteger(50);
            }
            else
            {
                xPos += FP_FromInteger(50);
            }
            if (!Handle_SlamDoor_or_EnemyStopper(mVelX, 0) && !Check_IsOnEndOfLine(sIsFlipped, 2) && VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(10), pTarget))
            {
                if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, xPos, mYPos, 0))
                {
                    return mBrainSubState;
                }
            }
            SetNextMotion(eScrabMotions::Motion_3_Turn);
            return Brain_2_Fighting::eBrain2_SetInPosition_4;
        }
        case Brain_2_Fighting::eBrain2_SetInPosition_4:
            if (GetCurrentMotion() != eScrabMotions::Motion_3_Turn)
            {
                return mBrainSubState;
            }
            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
            return Brain_2_Fighting::eBrain2_Yelling_6;

        case Brain_2_Fighting::eBrain2_Idle_5:
            if (static_cast<s32>(sGnFrame) <= field_12C_timer)
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_30_Shriek);
            return Brain_2_Fighting::eBrain2_Shriek_7;

        case Brain_2_Fighting::eBrain2_Yelling_6:
            if (GetCurrentMotion() != eScrabMotions::Motion_26_HowlBegin || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            field_12C_timer = MakeTimer(20);
            return Brain_2_Fighting::eBrain2_Idle_5;

        case Brain_2_Fighting::eBrain2_Shriek_7:
            if (GetCurrentMotion() != eScrabMotions::Motion_30_Shriek || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            mAttacking = true;
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            return Brain_2_Fighting::eBrain2_ToRun_8;

        case Brain_2_Fighting::eBrain2_ToRun_8:
            if (!pTarget->mAttacking)
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_2_Run);
            return Brain_2_Fighting::eBrain2_Running_9;

        case Brain_2_Fighting::eBrain2_Running_9:
            if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()), pTarget) || !BaseAliveGameObjectCollisionLine || !pTarget->BaseAliveGameObjectCollisionLine)
            {
                return mBrainSubState;
            }

            if (pTarget->GetAnimation().GetRender())
            {
                GetAnimation().SetRender(false);
                mHealth = FP_FromInteger(0);
            }

            MapFollowMe(true);
            SetCurrentMotion(eScrabMotions::Motion_31_ScrabBattleAnim);
            field_12C_timer = MakeTimer(40);
            return Brain_2_Fighting::eBrain2_Battling_10;

        case Brain_2_Fighting::eBrain2_Battling_10:
            if (static_cast<s32>(sGnFrame) <= field_12C_timer)
            {
                return mBrainSubState;
            }

            SND_Stop_Channels_Mask(field_160_sfx_bitmask);
            field_160_sfx_bitmask = 0;
            Scrab_SFX(ScrabSounds::eDeathHowl_1, 0, -1571, 1);
            Scrab_SFX(ScrabSounds::eYell_8, 0, -1571, 1);
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, -383, 0);

            if (!GetAnimation().GetRender())
            {
                SetBrain(&Scrab::Brain_3_Death);
                mXPos = pTarget->mXPos;
                SetCurrentMotion(eScrabMotions::Motion_39_DeathBegin);
                GetAnimation().SetRender(true);
                field_12C_timer = MakeTimer(90);
                return mBrainSubState;
            }
            SetCurrentMotion(eScrabMotions::Motion_0_Stand);
            field_12C_timer = MakeTimer(5);
            return Brain_2_Fighting::eBrain2_Victorious_11;

        case Brain_2_Fighting::eBrain2_Victorious_11:
            if (static_cast<s32>(sGnFrame) <= field_12C_timer)
            {
                return mBrainSubState;
            }
            field_12C_timer = 5;
            SetNextMotion(eScrabMotions::Motion_21_Stamp);
            return Brain_2_Fighting::eBrain2_SmashingOpponent_12;

        case Brain_2_Fighting::eBrain2_SmashingOpponent_12:
            if (GetCurrentMotion() != eScrabMotions::Motion_21_Stamp)
            {
                return mBrainSubState;
            }

            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            field_12C_timer--;
            if (field_12C_timer > 0)
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_26_HowlBegin);
            return Brain_2_Fighting::eBrain2_VictoryYell_13;

        case Brain_2_Fighting::eBrain2_VictoryYell_13:
            if (GetCurrentMotion() != eScrabMotions::Motion_26_HowlBegin || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            mFightTargetId = Guid{};
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            ToPatrol();
            return Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;

        case Brain_2_Fighting::eBrain2_InterruptVictoryStates_14:
            if (EventGet(kEventAbeOhm))
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            return Brain_2_Fighting::eBrain2_LookingForOpponent_0;

        case Brain_2_Fighting::eBrain2_WaitingForBattle_15:
        {
            if (GetCurrentMotion() != eScrabMotions::Motion_26_HowlBegin)
            {
                return mBrainSubState;
            }
            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            auto pFoundTarget = FindScrabToFight();
            if (!pFoundTarget)
            {
                return mBrainSubState;
            }
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            mFightTargetId = pFoundTarget->mBaseGameObjectId;
            return Brain_2_Fighting::eBrain2_LookingForOpponent_0;
        }

        default:
            return mBrainSubState;
    }
}

s16 Scrab::Brain_3_Death()
{
    if (field_12C_timer < static_cast<s32>((sGnFrame + 80)))
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_12C_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (field_12C_timer < static_cast<s32>(sGnFrame))
    {
        SetDead(true);
    }

    return 0;
}

s16 Scrab::Brain_4_ShrinkDeath()
{
    if (GetScale() == Scale::Fg)
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.06));
    }
    else
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.03));
    }

    if (field_12C_timer < static_cast<s32>(sGnFrame))
    {
        SetDead(true);
    }

    return 0;
}

s16 Scrab::Brain_5_Possessed()
{
    // Abe is dead, go back to patrolling
    if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        sControlledCharacter = sActiveHero;
        SetPossessed(false);
        field_1A2_speak_counter = 0;
        MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
        ToPatrol();
        mBrainSubState = Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
        gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
        return mBrainSubState;
    }

    if (mBrainSubState == 0)
    {
        if (static_cast<s32>(sGnFrame) <= field_12C_timer)
        {
            return mBrainSubState;
        }
        SetNextMotion(eScrabMotions::Motion_0_Stand);
        return 1;
    }
    else
    {
        if (mBrainSubState != 1 || gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) == CameraPos::eCamInvalid_m1)
        {
            return mBrainSubState;
        }
        MusicController::static_PlayMusic(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
        return mBrainSubState;
    }
}

void Scrab::Motion_0_Stand()
{
    if (BrainIs(&Scrab::Brain_5_Possessed))
    {
        if (mBrainSubState != 1)
        {
            return;
        }

        if (Input_IsChanting() && !mPreventDepossession)
        {
            field_130_depossession_timer = MakeTimer(30);
            SfxPlayMono(relive::SoundEffects::PossessEffect, 0);
            SetCurrentMotion(eScrabMotions::Motion_28_GetDepossessedBegin);
            return;
        }

        if (Input().IsAnyPressed(InputCommands::eGameSpeak2))
        {
            SetCurrentMotion(eScrabMotions::Motion_30_Shriek);
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            return;
        }

        if (Input().IsAnyPressed(InputCommands::eGameSpeak1))
        {
            SetCurrentMotion(eScrabMotions::Motion_26_HowlBegin);
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            return;
        }

        if (Input().IsAnyHeld(InputCommands::eThrowItem | InputCommands::eDoAction) && mShredPowerActive)
        {
            SetCurrentMotion(eScrabMotions::Motion_32_AttackSpin);
            mNextMotion = -1;
            field_12C_timer = MakeTimer(mPossessedMaxWhirlAttackDuration);
            return;
        }

        if (Input().IsAnyHeld(InputCommands::eThrowItem | InputCommands::eDoAction))
        {
            SetCurrentMotion(eScrabMotions::Motion_21_Stamp);
            mNextMotion = -1;
            return;
        }

        if (Input().IsAnyHeld(InputCommands::eHop))
        {
            const FP k45Scaled = (GetSpriteScale() * FP_FromInteger(45));
            const FP kGridSize = ScaleToGridSize(GetSpriteScale());

            if (GetAnimation().GetFlipX())
            {
                if (!WallHit(k45Scaled, -kGridSize))
                {
                    SetCurrentMotion(eScrabMotions::Motion_5_HopBegin);
                    mNextMotion = -1;
                    return;
                }
            }
            else
            {
                if (!WallHit(k45Scaled, kGridSize))
                {
                    SetCurrentMotion(eScrabMotions::Motion_5_HopBegin);
                    mNextMotion = -1;
                    return;
                }
            }
        }
    }
    ToNextMotion();
}

const FP sScrabWalkXVels_546E54[22] = {
    FP_FromDouble(1.570587158203125),
    FP_FromDouble(1.5780487060546875),
    FP_FromDouble(1.5256805419921875),
    FP_FromDouble(1.5651397705078125),
    FP_FromDouble(1.5966339111328125),
    FP_FromDouble(3.694122314453125),
    FP_FromDouble(3.9574737548828125),
    FP_FromDouble(3.9715423583984375),
    FP_FromDouble(3.3073272705078125),
    FP_FromDouble(2.629669189453125),
    FP_FromDouble(2.1679229736328125),
    FP_FromDouble(2.0220794677734375),
    FP_FromDouble(1.804046630859375),
    FP_FromDouble(1.6065216064453125),
    FP_FromDouble(2.6418609619140625),
    FP_FromDouble(2.685821533203125),
    FP_FromDouble(2.673095703125),
    FP_FromDouble(2.691070556640625),
    FP_FromDouble(1.3966827392578125),
    FP_FromDouble(1.376983642578125),
    FP_FromDouble(1.3767242431640625),
    FP_FromDouble(2.1646728515625)};

void Scrab::Motion_1_Walk()
{
    mVelX = GetSpriteScale() * sScrabWalkXVels_546E54[GetAnimation().GetCurrentFrame()];

    if (GetAnimation().GetFlipX())
    {
        mVelX = -mVelX;
    }

    MoveOnLine();

    if (GetCurrentMotion() != eScrabMotions::Motion_1_Walk)
    {
        return;
    }

    FP kGridSize = {};
    if (GetAnimation().GetFlipX())
    {
        kGridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        kGridSize = ScaleToGridSize(GetSpriteScale());
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), kGridSize * FP_FromDouble(0.5)))
    {
        KnockBack();
        return;
    }

    switch (GetAnimation().GetCurrentFrame())
    {
        case 3:
        case 13:
            Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
            return;

        case 5:
        case 15:
            if (WallHit(GetSpriteScale() * FP_FromInteger(45), kGridSize * FP_FromInteger(1)))
            {
                SetCurrentMotion(eScrabMotions::Motion_11_WalkToStand);
            }
            else if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (GetNextMotion() == eScrabMotions::Motion_0_Stand || GetNextMotion() == eScrabMotions::Motion_3_Turn || GetNextMotion() == eScrabMotions::Motion_21_Stamp || GetNextMotion() == eScrabMotions::Motion_25_Empty || GetNextMotion() == eScrabMotions::Motion_30_Shriek || GetNextMotion() == eScrabMotions::Motion_26_HowlBegin || GetNextMotion() == eScrabMotions::Motion_6_HopMidair || GetNextMotion() == eScrabMotions::Motion_37_AttackLunge || GetNextMotion() == eScrabMotions::Motion_38_LegKick)
                {
                    SetCurrentMotion(eScrabMotions::Motion_11_WalkToStand);
                }
            }
            else
            {
                if ((mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)) || !Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
                {
                    SetCurrentMotion(eScrabMotions::Motion_11_WalkToStand);
                }
            }
            return;

        case 7:
        case 18:
            Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
            if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
            {
                if (GetNextMotion() != eScrabMotions::Motion_2_Run)
                {
                    MapFollowMe(true);
                    return;
                }
                SetCurrentMotion(eScrabMotions::Motion_16_WalkToRun);
                mNextMotion = -1;
                MapFollowMe(true);
                return;
            }

            if (Input().IsAnyHeld(InputCommands::eThrowItem | InputCommands::eDoAction) && mShredPowerActive)
            {
                SetCurrentMotion(eScrabMotions::Motion_32_AttackSpin);
                field_12C_timer = MakeTimer(mPossessedMaxWhirlAttackDuration);
                mNextMotion = -1;
                MapFollowMe(true);
                return;
            }

            if (Input().IsAnyHeld(InputCommands::eRun))
            {
                SetCurrentMotion(eScrabMotions::Motion_16_WalkToRun);
                mNextMotion = -1;
                MapFollowMe(true);
                return;
            }

            if (Input().IsAnyHeld(InputCommands::eHop))
            {
                SetCurrentMotion(eScrabMotions::Motion_5_HopBegin);
                mNextMotion = -1;
                MapFollowMe(true);
                return;
            }

            MapFollowMe(true);
            return;

        default:
            return;
    }
}

static const FP sScrabRunVelX_546EC4[14] = {
    FP_FromDouble(9.498733520507812),
    FP_FromDouble(9.733749389648438),
    FP_FromDouble(9.584152221679688),
    FP_FromDouble(9.141021728515625),
    FP_FromDouble(9.099105834960938),
    FP_FromDouble(9.576034545898438),
    FP_FromDouble(9.327011108398438),
    FP_FromDouble(8.654891967773438),
    FP_FromDouble(5.2911224365234375),
    FP_FromDouble(4.2801666259765625),
    FP_FromDouble(3.446380615234375),
    FP_FromDouble(3.3974456787109375),
    FP_FromDouble(3.8137969970703125),
    FP_FromDouble(5.15692138671875)};

void Scrab::Motion_2_Run()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    mVelX = GetSpriteScale() * sScrabRunVelX_546EC4[GetAnimation().GetCurrentFrame()];

    if (GetAnimation().GetFlipX())
    {
        mVelX = -mVelX;
    }

    FP gridSize = {};
    if (GetAnimation().GetFlipX())
    {
        gridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        gridSize = ScaleToGridSize(GetSpriteScale());
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), gridSize * FP_FromDouble(0.5)))
    {
        KnockBack();
        return;
    }

    MoveOnLine();

    if (GetCurrentMotion() == eScrabMotions::Motion_2_Run)
    {
        KillTarget(pTarget);
        switch (GetAnimation().GetCurrentFrame())
        {
            case 0:
            case 5:
            case 7:
            case 12:
                Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
                return;

            case 3:
            case 10:
                if (sControlledCharacter != this || mHealth <= FP_FromInteger(0))
                {
                    if (GetNextMotion() == eScrabMotions::Motion_1_Walk)
                    {
                        SetCurrentMotion(eScrabMotions::Motion_17_RunToWalk);
                        mNextMotion = -1;
                        MapFollowMe(true);
                        return;
                    }

                    if (GetNextMotion() != eScrabMotions::Motion_0_Stand && GetNextMotion() != eScrabMotions::Motion_3_Turn && GetNextMotion() != eScrabMotions::Motion_6_HopMidair && GetNextMotion() != eScrabMotions::Motion_37_AttackLunge && GetNextMotion() != eScrabMotions::Motion_25_Empty && GetNextMotion() != eScrabMotions::Motion_26_HowlBegin && GetNextMotion() != eScrabMotions::Motion_38_LegKick && GetNextMotion() != eScrabMotions::Motion_30_Shriek)
                    {
                        if (GetNextMotion() == eScrabMotions::Motion_32_AttackSpin)
                        {
                            SetCurrentMotion(eScrabMotions::Motion_32_AttackSpin);
                            MapFollowMe(true);
                            return;
                        }

                        if (GetNextMotion() == eScrabMotions::Motion_31_ScrabBattleAnim)
                        {
                            ToStand();
                            SetCurrentMotion(eScrabMotions::Motion_31_ScrabBattleAnim);
                            mNextMotion = -1;
                            MapFollowMe(true);
                            return;
                        }

                        MapFollowMe(true);
                        return;
                    }
                }
                else
                {
                    if ((mVelX <= FP_FromInteger(0) || !Input().IsAnyHeld(InputCommands::eLeft)) && (mVelX >= FP_FromInteger(0) || !Input().IsAnyHeld(InputCommands::eRight)))
                    {
                        if (Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight))
                        {
                            if (!Input().IsAnyHeld(InputCommands::eRun))
                            {
                                SetCurrentMotion(eScrabMotions::Motion_17_RunToWalk);
                                MapFollowMe(true);
                                return;
                            }

                            if (Input().IsAnyHeld(InputCommands::eHop))
                            {
                                ToJump();
                                MapFollowMe(true);
                                return;
                            }

                            if (Input().IsAnyHeld(InputCommands::eThrowItem | InputCommands::eDoAction) && mShredPowerActive)
                            {
                                field_12C_timer = MakeTimer(mPossessedMaxWhirlAttackDuration);
                                SetCurrentMotion(eScrabMotions::Motion_32_AttackSpin);
                                mNextMotion = -1;
                                MapFollowMe(true);
                                return;
                            }

                            MapFollowMe(true);
                            return;
                        }
                    }
                }
                SetCurrentMotion(eScrabMotions::Motion_4_RunToStand);
                MapFollowMe(true);
                return;

            default:
                return;
        }
    }
}

void Scrab::Motion_3_Turn()
{
    if (GetAnimation().GetCurrentFrame() == 5 || GetAnimation().GetCurrentFrame() == 9 || GetAnimation().GetCurrentFrame() == 11)
    {
        Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();
        TryMoveOrStand();
    }
}

const FP dword_546EFC[10] = {
    {490908},
    {453112},
    {254902},
    {155230},
    {49871},
    {42004},
    {46393},
    {50715},
    {47541},
    {47764}};

void Scrab::Motion_4_RunToStand()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    mVelX = GetSpriteScale() * dword_546EFC[GetAnimation().GetCurrentFrame()]; // TODO: check size
    if (GetAnimation().GetFlipX())
    {
        mVelX = -mVelX;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        KnockBack();
    }
    else
    {
        MoveOnLine();

        if (GetCurrentMotion() == eScrabMotions::Motion_4_RunToStand)
        {
            KillTarget(pTarget);

            if (GetAnimation().GetIsLastFrame())
            {
                MapFollowMe(true);
                TryMoveOrStand();
            }
        }
    }
}

const FP sScrabHopBeginVelX_546F24[4] = {
    FP_FromDouble(-1.0283050537109375),
    FP_FromDouble(-1.8401947021484375),
    FP_FromDouble(-0.2052764892578125),
    FP_FromDouble(2.77862548828125)};

void Scrab::Motion_5_HopBegin()
{
    EventBroadcast(kEventNoise, this);

    FP frameVelX = {};
    if (GetAnimation().GetFlipX())
    {
        frameVelX = -sScrabHopBeginVelX_546F24[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sScrabHopBeginVelX_546F24[GetAnimation().GetCurrentFrame()];
    }

    mVelX = GetSpriteScale() * frameVelX;

    FP gridSize = {};
    if (GetAnimation().GetFlipX())
    {
        gridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        gridSize = ScaleToGridSize(GetSpriteScale());
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), gridSize * FP_FromDouble(0.75)))
    {
        KnockBack();
        SetCurrentMotion(eScrabMotions::Motion_8_JumpToFall);
    }
    else
    {
        mXPos += mVelX;
        if (GetAnimation().GetIsLastFrame())
        {
            SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -800);
            BaseAliveGameObjectLastLineYPos = mYPos;
            if (GetAnimation().GetFlipX())
            {
                mVelX = GetSpriteScale() * FP_FromDouble(-5.67);
            }
            else
            {
                mVelX = GetSpriteScale() * FP_FromDouble(5.67);
            }
            mVelY = GetSpriteScale() * FP_FromDouble(-9.8);
            mYPos += mVelY;
            VOnTrapDoorOpen();
            SetCurrentMotion(eScrabMotions::Motion_6_HopMidair);
            BaseAliveGameObjectCollisionLine = nullptr;
        }
    }
}

const FP sScrabHopMidAirVelX_546F34[8] = {
    FP_FromDouble(5.00537109375),
    FP_FromDouble(6.8809356689453125),
    FP_FromDouble(15.796859741210938),
    FP_FromDouble(18.309295654296875),
    FP_FromDouble(15.494369506835938),
    FP_FromDouble(15.187164306640625),
    FP_FromDouble(6.1830902099609375),
    FP_FromDouble(5.00537109375)};

void Scrab::Motion_6_HopMidair()
{
    EventBroadcast(kEventNoise, this);

    FP frameVelX = {};
    if (GetAnimation().GetFlipX())
    {
        frameVelX = -sScrabHopMidAirVelX_546F34[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sScrabHopMidAirVelX_546F34[GetAnimation().GetCurrentFrame()];
    }

    mVelX = GetSpriteScale() * frameVelX;

    FP kGridSize = {};
    if (GetAnimation().GetFlipX())
    {
        kGridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        kGridSize = ScaleToGridSize(GetSpriteScale());
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), kGridSize * FP_FromDouble(0.75)))
    {
        KnockBack();
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
                    ToStand();
                    SetCurrentMotion(eScrabMotions::Motion_7_HopLand);
                    PlatformCollide();
                    mXPos = hitX;
                    mYPos = hitY;
                    return;

                case eLineTypes::eWallLeft_1:
                case eLineTypes::eWallRight_2:
                    mVelX = (-mVelX / FP_FromInteger(2));
                    return;
            }
        }

        if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(5))
        {
            field_134_falling_velx_scale_factor = FP_FromDouble(1.25);
            SetCurrentMotion(eScrabMotions::Motion_8_JumpToFall);
        }
    }
}

const FP sScrabHopLandVelX_546F54[4] = {
    FP_FromDouble(2.77862548828125),
    FP_FromDouble(0.9429779052734375),
    FP_FromDouble(0.4394683837890625),
    FP_FromDouble(0.388397216796875)};

void Scrab::Motion_7_HopLand()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, 400, this);
        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
    }

    EventBroadcast(kEventNoise, this);

    FP frameVelX = {};
    if (GetAnimation().GetFlipX())
    {
        frameVelX = -sScrabHopLandVelX_546F54[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelX = sScrabHopLandVelX_546F54[GetAnimation().GetCurrentFrame()];
    }
    mVelX = (GetSpriteScale() * frameVelX);

    FP gridSize = {};
    if (GetAnimation().GetFlipX())
    {
        gridSize = -ScaleToGridSize(GetSpriteScale());
    }
    else
    {
        gridSize = ScaleToGridSize(GetSpriteScale());
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), gridSize * FP_FromDouble(0.75)))
    {
        KnockBack();
    }
    else
    {
        MoveOnLine();
        if (GetAnimation().GetIsLastFrame())
        {
            ToStand();
        }
    }
}

void Scrab::Motion_8_JumpToFall()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX = mVelX - (GetSpriteScale() * field_134_falling_velx_scale_factor);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX = (GetSpriteScale() * field_134_falling_velx_scale_factor) + mVelX;
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bHit = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
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
                SetCurrentMotion(eScrabMotions::Motion_20_Fall);
                PlatformCollide();
                mYPos = hitY;
                mXPos = hitX;
                MapFollowMe(true);
                break;
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
                mVelX = (-mVelX / FP_FromInteger(2));
                break;
            default:
                return;
        }
    }
}

const FP sStandToWalkVels_546E48[3] = {
    FP_FromDouble(1.12),
    FP_FromDouble(2.37),
    FP_FromDouble(3.20)};

void Scrab::Motion_9_StandToWalk()
{
    mVelX = (GetSpriteScale() * sStandToWalkVels_546E48[GetAnimation().GetCurrentFrame() & 3]); // TODO: Check size of 3 is correct
    if (GetAnimation().GetFlipX())
    {
        mVelX = -mVelX;
    }

    MoveOnLine();

    if (GetCurrentMotion() == eScrabMotions::Motion_9_StandToWalk)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            SetCurrentMotion(eScrabMotions::Motion_1_Walk);
        }
    }
}

const FP sStandToRunVels_546EB8[3] = {
    FP_FromDouble(2.29),
    FP_FromDouble(2.69),
    FP_FromDouble(3.88)};

void Scrab::Motion_10_StandToRun()
{
    mVelX = GetSpriteScale() * sStandToRunVels_546EB8[GetAnimation().GetCurrentFrame() & 3]; // TODO: Check size of 3 is correct
    if (GetAnimation().GetFlipX())
    {
        mVelX = -mVelX;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        KnockBack();
    }
    else
    {
        MoveOnLine();

        if (GetCurrentMotion() == eScrabMotions::Motion_10_StandToRun)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                SetCurrentMotion(eScrabMotions::Motion_2_Run);
            }
        }
    }
}

const FP sWalkToStandVels_546EAC[3] = {
    FP_FromDouble(1.04),
    FP_FromDouble(3.29),
    FP_FromDouble(2.86)};

void Scrab::Motion_11_WalkToStand()
{
    mVelX = (GetSpriteScale() * sWalkToStandVels_546EAC[GetAnimation().GetCurrentFrame() & 3]); // TODO: Check size of 3 is correct
    if (GetAnimation().GetFlipX())
    {
        mVelX = -mVelX;
    }

    MoveOnLine();

    if (GetCurrentMotion() == eScrabMotions::Motion_11_WalkToStand)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            TryMoveOrStand();
        }
    }
}

const FP dword_546F64[8] = {
    FP_FromDouble(11.25),
    FP_FromDouble(10.49),
    FP_FromDouble(9.67),
    FP_FromDouble(9.01),
    FP_FromDouble(7.47),
    FP_FromDouble(7.03),
    FP_FromDouble(6.69),
    FP_FromDouble(6.44)};

void Scrab::Motion_12_RunJumpBegin()
{
    if (GetAnimation().GetCurrentFrame() == 1)
    {
        SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -800);
    }

    EventBroadcast(kEventNoise, this);

    FP velX = {};
    if (GetAnimation().GetFlipX())
    {
        velX = -dword_546F64[GetAnimation().GetCurrentFrame() & 8]; // TODO: Check size
    }
    else
    {
        velX = dword_546F64[GetAnimation().GetCurrentFrame() & 8]; // TODO: Check size
    }

    mVelX = GetSpriteScale() * velX;

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        KnockBack();
    }
    else
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        const auto bHit = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
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
                    ToStand();
                    SetCurrentMotion(eScrabMotions::Motion_13_RunJumpEnd);
                    PlatformCollide();
                    mXPos = hitX;
                    mYPos = hitY;
                    return;

                case eLineTypes::eWallLeft_1:
                case eLineTypes::eWallRight_2:
                    mVelX = (-mVelX / FP_FromInteger(2));
                    return;
                default:
                    break;
            }
        }

        if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(5))
        {
            field_134_falling_velx_scale_factor = FP_FromDouble(1.25);
            SetCurrentMotion(eScrabMotions::Motion_8_JumpToFall);
        }
    }
}

const FP sEndRunJumpVels_546F84[21] = {
    FP_FromDouble(2.68),
    FP_FromDouble(2.65),
    FP_FromDouble(2.72),
    FP_FromDouble(2.89),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)};


void Scrab::Motion_13_RunJumpEnd()
{
    EventBroadcast(kEventNoise, this);
    mVelX = (GetSpriteScale() * sEndRunJumpVels_546F84[GetAnimation().GetCurrentFrame() & 21]); // TODO: Check size
    if (GetAnimation().GetFlipX())
    {
        mVelX = -mVelX;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        KnockBack();
    }
    else
    {
        MoveOnLine();
        if (GetAnimation().GetIsLastFrame())
        {
            Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
            if (!ToNextMotion())
            {
                ToStand();
            }
        }
    }
}

void Scrab::Motion_14_WalkToFall()
{
    Motion_8_JumpToFall();
    if (GetCurrentMotion() == eScrabMotions::Motion_0_Stand)
    {
        SetCurrentMotion(eScrabMotions::Motion_7_HopLand);
    }
}

void Scrab::Motion_15_RunToFall()
{
    Motion_8_JumpToFall();
    if (GetCurrentMotion() == eScrabMotions::Motion_0_Stand)
    {
        SetCurrentMotion(eScrabMotions::Motion_7_HopLand);
    }
}

void Scrab::Motion_16_WalkToRun()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eScrabMotions::Motion_2_Run);
        mNextMotion = -1;
    }
}

void Scrab::Motion_17_RunToWalk()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eScrabMotions::Motion_1_Walk);
        mNextMotion = -1;
    }
}

void Scrab::Motion_18_Knockback()
{
    if (BaseAliveGameObjectCollisionLine)
    {
        MoveOnLine();
    }
    else
    {
        Motion_8_JumpToFall();
    }

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, 400, this);
    }

    else if (GetAnimation().GetCurrentFrame() == 2)
    {
        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (BaseAliveGameObjectCollisionLine)
        {
            ToStand();
        }
        else
        {
            SetCurrentMotion(eScrabMotions::Motion_8_JumpToFall);
        }
    }
}

void Scrab::Motion_19_GetEaten()
{
    if (GetAnimation().GetCurrentFrame() == 3)
    {
        SFX_Play_Pitch(relive::SoundEffects::PickupItem, 120, -1000);
    }
}

void Scrab::Motion_20_Fall()
{
    if (GetAnimation().GetIsLastFrame())
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 80, 400, this);
        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
        ToStand();
        mNextMotion = -1;
    }
}

void Scrab::Motion_21_Stamp()
{
    if (GetAnimation().GetCurrentFrame() == 9)
    {
        if (BrainIs(&Scrab::Brain_5_Possessed))
        {
            KillTarget(nullptr);
        }

        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
        SFX_Play_Pitch(relive::SoundEffects::KillEffect, 60, Math_RandomRange(-255, 255));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        Scrab_SFX(ScrabSounds::eHowl_0, 60, 511, 1);

        if (BrainIs(&Scrab::Brain_5_Possessed))
        {
            ToStand();
        }
        else
        {
            ToNextMotion();
        }
    }
}

void Scrab::Motion_22_GetPossessed()
{
    if (GetAnimation().GetCurrentFrame() == 9)
    {
        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (mNextMotion)
        {
            mForceUpdateAnimation = true;
            SetCurrentMotion(eScrabMotions::Motion_22_GetPossessed);
        }
        else
        {
            ToStand();
        }
    }
}

void Scrab::Motion_23_Empty()
{
    
}

void Scrab::Motion_24_DeathEnd()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        mVelX = FP_FromInteger(0);
        Motion_8_JumpToFall();
        if (GetCurrentMotion() != eScrabMotions::Motion_24_DeathEnd)
        {
            SetCurrentMotion(eScrabMotions::Motion_24_DeathEnd);
        }
    }
}

void Scrab::Motion_25_Empty()
{
    
}

void Scrab::Motion_26_HowlBegin()
{
    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (sGnFrame & 1)
        {
            EventBroadcast(kEventLoudNoise, this);
            Scrab_SFX(ScrabSounds::eYell_8, 0, Math_RandomRange(-1600, -900), 1);
            if (BrainIs(&Scrab::Brain_5_Possessed))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eScrab_Howl);
            }
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mShredPowerActive = 1;

        if (mNextMotion == -1)
        {
            ToStand();
        }
        else
        {
            SetCurrentMotion(eScrabMotions::Motion_27_HowlEnd);
        }
    }
}

void Scrab::Motion_27_HowlEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (BrainIs(&Scrab::Brain_5_Possessed))
        {
            if (!ToNextMotion())
            {
                ToStand();
            }
        }
        else
        {
            ToNextMotion();
        }
    }
}

void Scrab::Motion_28_GetDepossessedBegin()
{
    mShredPowerActive = 0;

    if (sControlledCharacter == this)
    {
        if (GetAnimation().GetCurrentFrame() == 2)
        {
            Scrab_SFX(ScrabSounds::eYell_8, 0, Math_RandomRange(-1600, -900), 1);
        }

        if (GetAnimation().GetIsLastFrame())
        {
            if (!Input_IsChanting())
            {
                SetCurrentMotion(eScrabMotions::Motion_29_GetDepossessedEnd);
            }
        }

        if (!(static_cast<s32>(sGnFrame) % 4))
        {
            const FP xRnd = FP_FromInteger(Math_RandomRange(-20, 20));
            const FP yRnd = FP_FromInteger(Math_RandomRange(20, 50));
            const FP ypos = mYPos - (GetSpriteScale() * yRnd);
            const FP xpos = (GetSpriteScale() * xRnd) + mXPos;
            New_TintChant_Particle(xpos, ypos, GetSpriteScale(), Layer::eLayer_0);
        }

        if (static_cast<s32>(sGnFrame) > field_130_depossession_timer || sActiveHero->mHealth <= FP_FromInteger(0))
        {
            sControlledCharacter = sActiveHero;
            SetPossessed(false);
            field_1A2_speak_counter = 0;
            MusicController::static_PlayMusic(MusicController::MusicTypes::eNone_0, this, 0, 0);
            SetCurrentMotion(eScrabMotions::Motion_29_GetDepossessedEnd);
            ToPatrol();
            mBrainSubState = Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
            gMap.SetActiveCam(mAbeLevel, mAbePath, mAbeCamera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
    }
}

void Scrab::Motion_29_GetDepossessedEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Scrab::Motion_30_Shriek()
{
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        if (sGnFrame & 1)
        {
            EventBroadcast(kEventLoudNoise, this);
            Scrab_SFX(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
            if (BrainIs(&Scrab::Brain_5_Possessed))
            {
                gEventSystem->PushEvent(GameSpeakEvents::eScrab_Shriek);
            }
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (!ToNextMotion())
        {
            ToStand();
        }
    }
}

void Scrab::Motion_31_ScrabBattleAnim()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        field_160_sfx_bitmask = Scrab_SFX(ScrabSounds::eShredding_5, 100, Math_RandomRange(-600, 200), 1);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToNextMotion();
    }
}

void Scrab::Motion_32_AttackSpin()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));

    mShredPowerActive = 0;
    if (Expired(field_12C_timer))
    {
        SetCurrentMotion(eScrabMotions::Motion_2_Run);
        return;
    }

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        field_160_sfx_bitmask = Scrab_SFX(ScrabSounds::eShredding_5, 100, Math_RandomRange(-600, 200), 1);
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    if (sControlledCharacter != this)
    {
        if (pObj)
        {
            if (pObj->mXPos <= mXPos)
            {
                mVelX = -(kGridSize / FP_FromDouble(3.5));
            }
            else
            {
                mVelX = (kGridSize / FP_FromDouble(3.5));
            }
        }
        else
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else
    {
        if (Input().IsAnyHeld(InputCommands::eThrowItem | InputCommands::eDoAction))
        {
            if (!Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight))
            {
                mVelX = FP_FromInteger(0);
            }

            if (Input().IsAnyHeld(InputCommands::eRun))
            {
                if (Input().IsAnyHeld(InputCommands::eLeft))
                {
                    mVelX = -(kGridSize / FP_FromDouble(3.5));
                }

                if (Input().IsAnyHeld(InputCommands::eRight))
                {
                    mVelX = (kGridSize / FP_FromDouble(3.5));
                }
            }
            else
            {
                if (Input().IsAnyHeld(InputCommands::eLeft))
                {
                    mVelX = -(kGridSize / FP_FromInteger(7));
                }

                if (Input().IsAnyHeld(InputCommands::eRight))
                {
                    mVelX = (kGridSize / FP_FromInteger(7));
                }
            }
        }
        else
        {
            SetCurrentMotion(eScrabMotions::Motion_2_Run);
        }
    }

    MapFollowMe(false);

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), mVelX))
    {
        mVelX = (-mVelX / FP_FromInteger(2));
    }
    else
    {
        MoveOnLine();
        if (GetCurrentMotion() == eScrabMotions::Motion_32_AttackSpin)
        {
            KillTarget(pObj);
        }
    }
}

void Scrab::Motion_33_FeedToGulp()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (GetNextMotion() == eScrabMotions::Motion_35_StandToFeed)
        {
            SetCurrentMotion(eScrabMotions::Motion_35_StandToFeed);
        }
        else
        {
            SetCurrentMotion(eScrabMotions::Motion_34_GulpToStand);
        }
        mNextMotion = -1;
    }
}

void Scrab::Motion_34_GulpToStand()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Scrab::Motion_35_StandToFeed()
{
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eScrabMotions::Motion_36_Feed);
    }
}

void Scrab::Motion_36_Feed()
{
    if (GetAnimation().GetCurrentFrame())
    {
        if (GetAnimation().GetIsLastFrame())
        {
            SetCurrentMotion(eScrabMotions::Motion_33_FeedToGulp);
        }
    }
    else
    {
        SfxPlayMono((Math_NextRandom() & 1) ? relive::SoundEffects::Eating2 : relive::SoundEffects::Eating1, 0);
    }
}

void Scrab::Motion_37_AttackLunge()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetGuid));
    KillTarget(pTarget);

    if (GetAnimation().GetCurrentFrame() == 4)
    {
        Scrab_SFX(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Scrab::Motion_38_LegKick()
{
    Motion_37_AttackLunge();
}

void Scrab::Motion_39_DeathBegin()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        mVelX = FP_FromInteger(0);
        Motion_8_JumpToFall();
        if (GetCurrentMotion() != eScrabMotions::Motion_39_DeathBegin)
        {
            SetCurrentMotion(eScrabMotions::Motion_39_DeathBegin);
        }
    }
    if (GetAnimation().GetIsLastFrame())
    {
        SetCurrentMotion(eScrabMotions::Motion_24_DeathEnd);
    }
}

bool Scrab::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    const PSX_RECT ourRect = VGetBoundingRect();
    const PSX_RECT otherRect = pOther->VGetBoundingRect();

    const FP k10Scaled = (FP_FromInteger(10) * GetSpriteScale());
    if (FP_FromInteger(ourRect.y) <= (FP_FromInteger(otherRect.h) - k10Scaled) && ourRect.y >= otherRect.y)
    {
        return true;
    }

    if (ourRect.h <= otherRect.h)
    {
        if (FP_FromInteger(ourRect.h) >= (k10Scaled + FP_FromInteger(otherRect.y)))
        {
            return true;
        }
    }

    if (ourRect.y >= otherRect.y)
    {
        if (ourRect.h <= otherRect.h)
        {
            return true;
        }

        if (ourRect.y > otherRect.y)
        {
            return false;
        }
    }

    if (ourRect.h < otherRect.h)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Scrab::ToPatrol()
{
    SetBrain(&Scrab::Brain_0_Patrol);
}

void Scrab::ToStand()
{
    field_134_falling_velx_scale_factor = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    SetCurrentMotion(eScrabMotions::Motion_0_Stand);
    MapFollowMe(true);
}

void Scrab::MoveOnLine()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    const FP oldXPos = mXPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(&mXPos, &mYPos, mVelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (pObj)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 &&
                    BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const auto oldMotion = mCurrentMotion;
                    VOnTrapDoorOpen();
                    mCurrentMotion = oldMotion;
                }
            }
            else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 ||
                     BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PlatformCollide();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            BaseAliveGameObjectLastLineYPos = mYPos;
            field_134_falling_velx_scale_factor = FP_FromInteger(1);
            mXPos = oldXPos + mVelX;
            if (GetCurrentMotion() == eScrabMotions::Motion_1_Walk)
            {
                SetCurrentMotion(eScrabMotions::Motion_14_WalkToFall);
            }
            else
            {
                SetCurrentMotion(eScrabMotions::Motion_15_RunToFall);
            }
        }
    }
    else
    {
        SetCurrentMotion(eScrabMotions::Motion_8_JumpToFall);
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
}

void Scrab::PlatformCollide()
{
    const PSX_RECT bRect = VGetBoundingRect();
    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
    const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
    OnCollisionWith(xy, wh, gPlatformsArray);
}

BaseAliveGameObject* Scrab::Find_Fleech()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eFleech)
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (pAliveObj->mHealth > FP_FromInteger(0))
            {
                if (pAliveObj->VOnSameYLevel(pAliveObj))
                {
                    if (pAliveObj->VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(3), pAliveObj))
                    {
                        if (pAliveObj->VIsFacingMe(pAliveObj))
                        {
                            if (!WallHit(GetSpriteScale() * FP_FromInteger(45), pAliveObj->mXPos - mXPos) && gMap.Is_Point_In_Current_Camera(pAliveObj->mCurrentLevel, pAliveObj->mCurrentPath, pAliveObj->mXPos, pAliveObj->mYPos, 0) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                            {
                                return pAliveObj;
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

void Scrab::VPossessed()
{
    SetPossessed(true);
    mPreventDepossession = 1;
    mShredPowerActive = 0;
    SetCurrentMotion(eScrabMotions::Motion_22_GetPossessed);
    mNextMotion = -1;
    vUpdateAnim();
    SetBrain(&Scrab::Brain_5_Possessed);
    mBrainSubState = 0;
    mTargetGuid = Guid{};
    mFightTargetId = Guid{};
    field_12C_timer = MakeTimer(35);
    mAbeLevel = gMap.mCurrentLevel;
    mAbePath = gMap.mCurrentPath;
    mAbeCamera = gMap.mCurrentCamera;
}

u8** Scrab::ResBlockForMotion(s16 /*motion*/)
{
    /*if (motion < eScrabMotions::Motion_21_Stamp)
    {
        field_140_motion_resource_block_index = 0;
    }
    else if (motion < eScrabMotions::Motion_23_Empty)
    {
        field_140_motion_resource_block_index = 6;
    }
    else if (motion < eScrabMotions::Motion_25_Empty)
    {
        field_140_motion_resource_block_index = 8;
    }
    else if (motion < eScrabMotions::Motion_26_HowlBegin)
    {
        field_140_motion_resource_block_index = 10;
    }
    else if (motion < eScrabMotions::Motion_30_Shriek)
    {
        field_140_motion_resource_block_index = 5;
    }
    else if (motion < eScrabMotions::Motion_31_ScrabBattleAnim)
    {
        field_140_motion_resource_block_index = 9;
    }
    else if (motion < eScrabMotions::Motion_33_FeedToGulp)
    {
        field_140_motion_resource_block_index = 4;
    }
    else if (motion < eScrabMotions::Motion_35_StandToFeed)
    {
        field_140_motion_resource_block_index = 11;
    }
    else if (motion < eScrabMotions::Motion_37_AttackLunge)
    {
        field_140_motion_resource_block_index = 1;
    }
    else if (motion < eScrabMotions::Motion_38_LegKick)
    {
        field_140_motion_resource_block_index = 2;
    }
    else if (motion >= eScrabMotions::Motion_39_DeathBegin)
    {
        field_140_motion_resource_block_index = motion >= 40 ? 0 : 13; // 40 == max motions ??
    }
    else
    {
        field_140_motion_resource_block_index = 3;
    }
    return mBaseGameObjectResArray.ItemAt(field_140_motion_resource_block_index);*/
    return nullptr;
}

void Scrab::VScreenChanged()
{
    BaseGameObject* pChaseTarget = sObjectIds.Find_Impl(mTargetGuid);

    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
    else if (pChaseTarget)
    {
        if (pChaseTarget->GetDead())
        {
            mTargetGuid = Guid{};
            SetNextMotion(eScrabMotions::Motion_0_Stand);
            ToPatrol();
            mBrainSubState = Scrab_Brain_0_Patrol::eBrain0_ToMoving_0;
        }
    }
}

void Scrab::VRender(OrderingTable& ot)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void Scrab::TryMoveOrStand()
{
    if (!ToNextMotion())
    {
        ToStand();
    }
}

s16 Scrab::ToNextMotion()
{
    MapFollowMe(true);

    if (sControlledCharacter == this && mHealth > FP_FromInteger(0) && mBrainSubState != 0)
    {
        return PlayerControlled();
    }

    if (GetNextMotion() == eScrabMotions::Motion_3_Turn || GetNextMotion() == eScrabMotions::Motion_21_Stamp || GetNextMotion() == eScrabMotions::Motion_25_Empty || GetNextMotion() == eScrabMotions::Motion_26_HowlBegin || GetNextMotion() == eScrabMotions::Motion_30_Shriek || GetNextMotion() == eScrabMotions::Motion_31_ScrabBattleAnim || GetNextMotion() == eScrabMotions::Motion_37_AttackLunge || GetNextMotion() == eScrabMotions::Motion_38_LegKick || GetNextMotion() == eScrabMotions::Motion_35_StandToFeed)
    {
        mCurrentMotion = mNextMotion;
        mNextMotion = -1;
        return 1;
    }

    const FP k45Scaled = (GetSpriteScale() * FP_FromInteger(45));
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    // Check if going to run into a wall.
    if (GetNextMotion() == eScrabMotions::Motion_2_Run)
    {
        if (GetAnimation().GetFlipX())
        {
            if (WallHit(k45Scaled, -kGridSize))
            {
                return 0;
            }
            else
            {
                mVelX = -(kGridSize / FP_FromDouble(3.5));
                SetCurrentMotion(eScrabMotions::Motion_10_StandToRun);
                mNextMotion = -1;
                return 1;
            }
        }
        else
        {
            if (WallHit(k45Scaled, kGridSize))
            {
                return 0;
            }
            else
            {
                mVelX = (kGridSize / FP_FromDouble(3.5));
                SetCurrentMotion(eScrabMotions::Motion_10_StandToRun);
                mNextMotion = -1;
                return 1;
            }
        }
    }

    if (GetNextMotion() != eScrabMotions::Motion_1_Walk)
    {
        if (GetNextMotion() == eScrabMotions::Motion_0_Stand)
        {
            ToStand();
            return 1;
        }

        if (GetNextMotion() != eScrabMotions::Motion_6_HopMidair)
        {
            return 0;
        }

        if (GetAnimation().GetFlipX())
        {
            if (WallHit(k45Scaled, -kGridSize))
            {
                return 0;
            }
            else
            {
                SetCurrentMotion(eScrabMotions::Motion_5_HopBegin);
                mNextMotion = -1;
                return 1;
            }
        }
        else
        {
            if (WallHit(k45Scaled, kGridSize))
            {
                return 0;
            }
            else
            {
                SetCurrentMotion(eScrabMotions::Motion_5_HopBegin);
                mNextMotion = -1;
                return 1;
            }
        }
    }
    else
    {
        if (GetAnimation().GetFlipX())
        {
            if (WallHit(k45Scaled, -kGridSize))
            {
                return 0;
            }
            else
            {
                SetCurrentMotion(eScrabMotions::Motion_9_StandToWalk);
                mVelX = -(kGridSize / FP_FromInteger(7));
                mNextMotion = -1;
                return 1;
            }
        }
        else
        {
            if (WallHit(k45Scaled, kGridSize))
            {
                return 0;
            }
            else
            {
                SetCurrentMotion(eScrabMotions::Motion_9_StandToWalk);
                mVelX = (kGridSize / FP_FromInteger(7));
                mNextMotion = -1;
                return 1;
            }
        }
    }
}

s16 Scrab::PlayerControlled()
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP k45Scaled = (GetSpriteScale() * FP_FromInteger(45));

    if (Input().IsAnyHeld(InputCommands::eRight))
    {
        if (GetAnimation().GetFlipX())
        {
            SetCurrentMotion(eScrabMotions::Motion_3_Turn);
            return 1;
        }
        else
        {
            if (WallHit(k45Scaled, kGridSize))
            {
                return 0;
            }

            if (Input().IsAnyHeld(InputCommands::eRun))
            {
                mVelX = (kGridSize / FP_FromDouble(3.5));
                SetCurrentMotion(eScrabMotions::Motion_10_StandToRun);
                return 1;
            }
            else
            {
                mVelX = (kGridSize / FP_FromInteger(7));
                SetCurrentMotion(eScrabMotions::Motion_9_StandToWalk);
                return 1;
            }
        }
    }
    else if (Input().IsAnyHeld(InputCommands::eLeft))
    {
        if (!GetAnimation().GetFlipX())
        {
            SetCurrentMotion(eScrabMotions::Motion_3_Turn);
            return 1;
        }
        else
        {
            if (WallHit(k45Scaled, -kGridSize))
            {
                return 0;
            }

            if (Input().IsAnyHeld(InputCommands::eRun))
            {
                mVelX = -(kGridSize / FP_FromDouble(3.5));
                SetCurrentMotion(eScrabMotions::Motion_10_StandToRun);
                return 1;
            }
            else
            {
                mVelX = -(kGridSize / FP_FromInteger(7));
                SetCurrentMotion(eScrabMotions::Motion_9_StandToWalk);
                return 1;
            }
        }
    }
    else
    {
        if (Input().IsAnyHeld(InputCommands::eUp))
        {
            SetCurrentMotion(eScrabMotions::Motion_35_StandToFeed);
            return 1;
        }

        if (Input().IsAnyHeld(InputCommands::eDown))
        {
            SetCurrentMotion(eScrabMotions::Motion_21_Stamp);
            return 1;
        }

        return 0;
    }
}

void Scrab::ToJump()
{
    BaseAliveGameObjectLastLineYPos = mYPos;

    if (GetAnimation().GetFlipX())
    {
        mVelX = GetSpriteScale() * -FP_FromDouble(-5.67);
    }
    else
    {
        mVelX = GetSpriteScale() * -FP_FromDouble(5.67);
    }

    mVelY = GetSpriteScale() * FP_FromDouble(-9.6);
    mYPos += mVelY;

    VOnTrapDoorOpen();

    SetCurrentMotion(eScrabMotions::Motion_12_RunJumpBegin);
    BaseAliveGameObjectCollisionLine = nullptr;
}

bool Scrab::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return false;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eFleech:
            mHealth = mHealth - FP_FromDouble(0.13);
            if (mHealth < FP_FromInteger(0))
            {
                mHealth = FP_FromInteger(0);
            }

            if (mHealth > FP_FromInteger(0))
            {
                return true;
            }

            if (static_cast<BaseAliveGameObject*>(pFrom)->mXPos >= mXPos)
            {
                GetAnimation().SetFlipX(false);
            }
            else
            {
                GetAnimation().SetFlipX(true);
            }

            EventBroadcast(kScrabOrParamiteDied, this);
            SetBrain(&Scrab::Brain_4_ShrinkDeath);
            SetCurrentMotion(eScrabMotions::Motion_19_GetEaten);
            field_12C_timer = MakeTimer(90);
            vUpdateAnim();
            if (sControlledCharacter == this)
            {
                SND_SEQ_Play(SeqId::DeathDrums_29, 1, 127, 127);
            }
            return false;

        case ReliveTypes::eAbilityRing:
            return false;

        case ReliveTypes::eBullet:
        //case ReliveTypes::eNeverSet:
        case ReliveTypes::eScrab:
            break;

        default:
            SfxPlayMono(relive::SoundEffects::KillEffect, 127);
            SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
            break;
    }

    EventBroadcast(kScrabOrParamiteDied, this);
    mHealth = FP_FromInteger(0);
    SetBrain(&Scrab::Brain_3_Death);
    field_12C_timer = MakeTimer(90);
    SetCurrentMotion(eScrabMotions::Motion_39_DeathBegin);
    vUpdateAnim();

    if (sControlledCharacter == this)
    {
        SND_SEQ_Play(SeqId::DeathDrums_29, 1, 127, 127);
    }

    return true;
}

void Scrab::KnockBack()
{
    field_134_falling_velx_scale_factor = FP_FromDouble(0.35);

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    FP v4 = {};
    if (GetAnimation().GetFlipX())
    {
        v4 = (kGridSize / FP_FromDouble(3.5));
    }
    else
    {
        v4 = -(kGridSize / FP_FromDouble(3.5));
    }
    mVelX = (v4 / FP_FromInteger(2));

    MapFollowMe(true);

    if (mVelY < FP_FromInteger(0))
    {
        mVelY = FP_FromInteger(0);
    }

    SetCurrentMotion(eScrabMotions::Motion_18_Knockback);
    field_130_depossession_timer = MakeTimer(10);
}

const relive::SfxDefinition& getSfxDef(ScrabSounds effectId)
{
    return scrab_sScrabSfx_560330[static_cast<s32>(effectId)];
}

s32 Scrab::Scrab_SFX(ScrabSounds soundId, s32 vol, s32 pitch, s16 applyDirection)
{
    s16 volumeLeft = 0;
    s16 volumeRight = 0;
    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    const relive::SfxDefinition& effectDef = getSfxDef(soundId);
    const s16 defaultSndIdxVol = effectDef.mDefaultVolume;

    volumeRight = static_cast<s16>(vol);
    if (vol <= 0)
    {
        volumeRight = defaultSndIdxVol;
    }

    if (GetSpriteScale() == FP_FromDouble(0.5))
    {
        volumeRight /= 2;
    }

    if (applyDirection)
    {
        PSX_RECT pRect = {};
        gMap.Get_Camera_World_Rect(direction, &pRect);
        switch (direction)
        {
            case CameraPos::eCamCurrent_0:
                volumeLeft = volumeRight;
                break;
            case CameraPos::eCamTop_1:
            case CameraPos::eCamBottom_2:
            {
                volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
                volumeRight = volumeLeft;
            }
            break;
            case CameraPos::eCamLeft_3:
            {
                const FP percentHowFar = (FP_FromInteger(pRect.w) - mXPos) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
                volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            }
            break;
            case CameraPos::eCamRight_4:
            {
                const FP percentHowFar = (mXPos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
                volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            }
            break;
            default:
                return 0;
        }
    }
    else
    {
        volumeLeft = volumeRight;
    }

    return SFX_SfxDefinition_Play_Stereo(
        effectDef,
        volumeLeft,
        volumeRight,
        static_cast<s16>(pitch),
        static_cast<s16>(pitch));
}

void Scrab::KillTarget(BaseAliveGameObject* pTarget)
{
    bool bKilledTarget = false;
    bool bKillSpecific = false;

    if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
    {
        if (!BrainIs(&Scrab::Brain_2_Fighting))
        {
            if (!BrainIs(&Scrab::Brain_5_Possessed) || GetCurrentMotion() == eScrabMotions::Motion_38_LegKick || GetCurrentMotion() == eScrabMotions::Motion_21_Stamp || GetCurrentMotion() == eScrabMotions::Motion_32_AttackSpin)
            {
                const PSX_RECT bOurRect = VGetBoundingRect();
                IBaseAliveGameObject* pObj = pTarget;

                s32 list_idx = 0;
                if (pTarget)
                {
                    bKillSpecific = true;
                }
                else if (gBaseAliveGameObjects->mUsedSize > 0)
                {
                    list_idx = 1;
                    pObj = gBaseAliveGameObjects->ItemAt(0);
                }
                else
                {
                    pObj = nullptr;
                }

                do
                {
                    if (pObj->GetIsBaseAliveGameObject())
                    {
                        if (pObj != this)
                        {
                            if ((pObj->Type() == ReliveTypes::eAbe ||
                                pObj->Type() == ReliveTypes::eRingOrLiftMud ||
                                pObj->Type() == ReliveTypes::eMudokon ||
                                //pObj->Type() == ReliveTypes::eNevetSet ||
                                pObj->Type() == ReliveTypes::eFleech ||
                                pObj->Type() == ReliveTypes::eScrab) &&
                                GetScale() == pObj->GetScale() && pObj->mHealth > FP_FromInteger(0))
                            {
                                const FP xDist = pObj->mXPos - mXPos;
                                if (!WallHit(GetSpriteScale() * FP_FromInteger(45), xDist))
                                {
                                    if (!pObj->GetInvisible())
                                    {
                                        if (pObj->Type() != ReliveTypes::eScrab ||
                                            !pObj->GetPossessed() ||
                                            (pObj->mCurrentMotion != 32 /*eScrabMotions::Motion_32_AttackSpin*/ &&
                                            (pObj->Type() != ReliveTypes::eFleech || BrainIs(&Scrab::Brain_5_Possessed) || mKillEnemy == Choice_short::eYes_1)))

                                        {
                                            const PSX_RECT objRect = pObj->VGetBoundingRect();

                                            if (PSX_Rects_overlap_no_adjustment(&objRect, &bOurRect))
                                            {
                                                if (pObj->VTakeDamage(this))
                                                {
                                                    bKilledTarget = true;
                                                    SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                                                    if (pObj->Type() == ReliveTypes::eAbe)
                                                    {
                                                        Mudokon_SFX(MudSounds::eHurt2_9, 0, 0, sActiveHero);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (bKillSpecific)
                    {
                        break;
                    }

                    if (bKilledTarget)
                    {
                        break;
                    }

                    if (list_idx >= gBaseAliveGameObjects->Size())
                    {
                        break;
                    }

                    pObj = gBaseAliveGameObjects->ItemAt(list_idx++);
                }
                while (pObj);
            }
        }
    }
}

s16 Scrab::FindAbeOrMud()
{
    if (CanSeeAbe(sActiveHero) && sActiveHero->mHealth > FP_FromInteger(0) && sActiveHero->GetSpriteScale() == GetSpriteScale() && !sActiveHero->GetInvisible())
    {
        if (!WallHit(GetSpriteScale() * FP_FromInteger(45), sActiveHero->mXPos - mXPos))
        {
            mTargetGuid = sActiveHero->mBaseGameObjectId;
            return true;
        }
    }

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetIsBaseAliveGameObject())
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if ((pAliveObj->Type() == ReliveTypes::eRingOrLiftMud || pAliveObj->Type() == ReliveTypes::eMudokon || pAliveObj->Type() == ReliveTypes::eScrab) && (pAliveObj->Type() != ReliveTypes::eScrab || pAliveObj->GetPossessed()) && CanSeeAbe(pAliveObj) && pAliveObj->mHealth > FP_FromInteger(0) && pAliveObj->GetSpriteScale() == GetSpriteScale())
            {
                if (!WallHit(GetSpriteScale() * FP_FromInteger(45), pAliveObj->mXPos - mXPos))
                {
                    mTargetGuid = pAliveObj->mBaseGameObjectId;
                    return true;
                }
            }
        }
    }
    return false;
}

s16 Scrab::CanSeeAbe(BaseAliveGameObject* pObj)
{
    if (pObj->GetSpriteScale() != GetSpriteScale())
    {
        return 0;
    }

    if (IsActiveHero(pObj))
    {
        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_67_LedgeHang_454E20 || sActiveHero->mCurrentMotion == eAbeMotions::Motion_69_LedgeHangWobble_454EF0)
        {
            return VOnSameYLevel(pObj);
        }
    }

    if (pObj->mYPos <= (mYPos - (GetSpriteScale() * FP_FromInteger(45))) || !LineOfSightTo(this, pObj))
    {
        return 0;
    }

    if (VOnSameYLevel(pObj))
    {
        FP offX = {};
        if (GetAnimation().GetFlipX())
        {
            offX = -ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            offX = ScaleToGridSize(GetSpriteScale());
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(45), offX))
        {
            return 0;
        }
    }

    return 1;
}


bool Scrab::LineOfSightTo(Scrab* pThis, BaseAliveGameObject* pObj)
{
    const PSX_RECT objRect = pObj->VGetBoundingRect();
    const PSX_RECT bRect = pThis->VGetBoundingRect();

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return gCollisions->Raycast(
               pThis->mXPos,
               FP_FromInteger((bRect.y + bRect.h) / 2),
               pObj->mXPos,
               FP_FromInteger((objRect.y + objRect.h) / 2),
               &pLine,
               &hitX,
               &hitY,
               pThis->GetScale() == Scale::Fg ? kFgFloorWallOrCeiling
               : kBgFloorWallOrCeiling)
        != 1;
} 

Scrab* Scrab::FindScrabToFight()
{
    Scrab* pScrabIAmFightingAlready = nullptr;
    Scrab* pScrabNotInAFight = nullptr;
    Scrab* pScrabInFightWithSomeoneElse = nullptr;

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eScrab)
        {
            auto pScrab = static_cast<Scrab*>(pObj);

            if (pScrab != this && !pScrab->GetPossessed() && !BrainIs(&Scrab::Brain_3_Death))
            {
                if (VOnSameYLevel(pScrab))
                {
                    if (!WallHit(GetSpriteScale() * FP_FromInteger(45), pScrab->mXPos - mXPos) && gMap.Is_Point_In_Current_Camera(pScrab->mCurrentLevel, pScrab->mCurrentPath, pScrab->mXPos, pScrab->mYPos, 0) && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
                    {
                        if (pScrab->mFightTargetId == Guid{})
                        {
                            pScrabNotInAFight = pScrab;
                        }
                        else
                        {
                            if (pScrab->mFightTargetId == mBaseGameObjectId)
                            {
                                pScrabIAmFightingAlready = pScrab;
                            }
                            else
                            {
                                pScrabInFightWithSomeoneElse = pScrab;
                            }
                        }
                    }
                }
            }
        }
    }

    if (pScrabIAmFightingAlready)
    {
        return pScrabIAmFightingAlready;
    }

    if (pScrabNotInAFight)
    {
        return pScrabNotInAFight;
    }

    // This can be nullptr
    return pScrabInFightWithSomeoneElse;
}

s16 Scrab::Handle_SlamDoor_or_EnemyStopper(FP velX, s16 bCheckLeftRightBounds)
{
    if (velX < FP_FromInteger(0))
    {
        velX = -velX;
    }

    FP gridSize = ScaleToGridSize(GetSpriteScale());
    if (velX > FP_FromInteger(5))
    {
        gridSize = (gridSize * FP_FromInteger(2));
    }

    ReliveTypes objectType = {};
    relive::Path_EnemyStopper::StopDirection stopDirection = relive::Path_EnemyStopper::StopDirection::Both;
    if (GetAnimation().GetFlipX())
    {
        objectType = ReliveTypes::eScrabLeftBound;
        stopDirection = relive::Path_EnemyStopper::StopDirection::Left;
        gridSize = -gridSize;
    }
    else
    {
        objectType = ReliveTypes::eScrabRightBound;
        stopDirection = relive::Path_EnemyStopper::StopDirection::Right;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(45), gridSize * FP_FromInteger(1)))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(FP_Abs(mYPos)),
        FP_GetExponent(mXPos + gridSize),
        FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
        ReliveTypes::eSlamDoor);

    auto pSlamDoorTlv = static_cast<relive::Path_SlamDoor*>(BaseAliveGameObjectPathTLV);
    if (pSlamDoorTlv && ((pSlamDoorTlv->mStartClosed == relive::reliveChoice::eYes && !SwitchStates_Get(pSlamDoorTlv->mSwitchId)) || (pSlamDoorTlv->mStartClosed == relive::reliveChoice::eNo && SwitchStates_Get(pSlamDoorTlv->mSwitchId))))
    {
        return 1;
    }

    BaseAliveGameObjectPathTLV = gPathInfo->TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos + gridSize),
        FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
        ReliveTypes::eEnemyStopper);

    auto pPathEnemyStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
    if (pPathEnemyStopper && (pPathEnemyStopper->mStopDirection == stopDirection || pPathEnemyStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && SwitchStates_Get(pPathEnemyStopper->mSwitchId))
    {
        return 1;
    }

    if (bCheckLeftRightBounds)
    {
        if (gPathInfo->TLV_Get_At(
                FP_GetExponent(mXPos),
                FP_GetExponent(FP_Abs(mYPos)),
                FP_GetExponent(mXPos + gridSize),
                FP_GetExponent(mYPos - ScaleToGridSize(GetSpriteScale())),
                objectType))
        {
            return 1;
        }
    }
    return 0;
}

GameSpeakEvents Scrab::LastSpeak()
{
    if (!gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 1))
    {
        return GameSpeakEvents::eNone;
    }

    if (field_17C_last_event == gEventSystem->mLastEventIndex)
    {
        if (gEventSystem->mLastEvent == GameSpeakEvents::eNone)
        {
            return GameSpeakEvents::eNone;
        }
        else
        {
            return GameSpeakEvents::eSameAsLast;
        }
    }
    else
    {
        field_17C_last_event = gEventSystem->mLastEventIndex;
        return gEventSystem->mLastEvent;
    }
}
