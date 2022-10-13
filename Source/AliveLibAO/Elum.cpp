#include "stdafx_ao.h"
#include "Function.hpp"
#include "Elum.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "LiftPoint.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Sfx.hpp"
#include "Gibs.hpp"
#include "MusicTrigger.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Midi.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "Honey.hpp"
#include "GameSpeak.hpp"
#include "Math.hpp"
#include "Grid.hpp"
#include "../AliveLibAE/Sound/Midi.hpp"

namespace AO {

Elum* gElum = nullptr;

using TElumStateFunction = decltype(&Elum::Motion_0_Respawn_414C60);

const TElumStateFunction sElum_motion_table_4C5148[] = {
    &Elum::Motion_0_Respawn_414C60,
    &Elum::Motion_1_Idle_412990,
    &Elum::Motion_2_Unknown_412C30,
    &Elum::Motion_3_WalkLoop_412C90,
    &Elum::Motion_4_Turn_4140F0,
    &Elum::Motion_5_WalkToIdle_4132D0,
    &Elum::Motion_6_MidWalkToIdle_4133F0,
    &Elum::Motion_7_IdleToWalk2_413200,
    &Elum::Motion_8_IdleToWalk1_413270,
    &Elum::Motion_9_ToYell_415890,
    &Elum::Motion_10_Yell_4158E0,
    &Elum::Motion_11_Unknown_4159A0,
    &Elum::Motion_12_RunTurn_414520,
    &Elum::Motion_13_RunTurnToWalk_4147C0,
    &Elum::Motion_14_Speak_414860,
    &Elum::Motion_15_Speak_4148F0,
    &Elum::Motion_16_Speak_414980,
    &Elum::Motion_17_Unknown_413620,
    &Elum::Motion_18_Unknown_4136A0,
    &Elum::Motion_19_Dead_415F90,
    &Elum::Motion_20_Fall_415F70,
    &Elum::Motion_21_Land_414A20,
    &Elum::Motion_22_RunOffEdge_415810,
    &Elum::Motion_23_WalkOffEdge_415E90,
    &Elum::Motion_24_JumpToFall_415ED0,
    &Elum::Motion_25_LickingHoney_415B50,
    &Elum::Motion_26_LickingToStruggling_415AC0,
    &Elum::Motion_27_AbeMountingEnd_415CA0,
    &Elum::Motion_28_AbeUnmountingEnd_415D60,
    &Elum::Motion_29_BeesStruggling_412A90,
    &Elum::Motion_30_HopBegin_414E30,
    &Elum::Motion_31_HopMid_414C70,
    &Elum::Motion_32_HopLand_415140,
    &Elum::Motion_33_RunJumpBegin_415400,
    &Elum::Motion_34_RunJumpMid_415240,
    &Elum::Motion_35_RunJumpLand_415580,
    &Elum::Motion_36_RunLoop_413720,
    &Elum::Motion_37_RunSlideStop_4142E0,
    &Elum::Motion_38_RunTurnToRun_414810,
    &Elum::Motion_39_IdleToRun_413B00,
    &Elum::Motion_40_WalkToRun_4134B0,
    &Elum::Motion_41_MidWalkToRun_413560,
    &Elum::Motion_42_RunToWalk_413B60,
    &Elum::Motion_43_MidRunToWalk_413E20,
    &Elum::Motion_44_ScratchBegin_412730,
    &Elum::Motion_45_ScratchLoop_4127B0,
    &Elum::Motion_46_ScratchEnd_412800,
    &Elum::Motion_47_Unknown_415A30,
    &Elum::Motion_48_AbeMoutingBegin_415C40,
    &Elum::Motion_49_AbeUnmountingBegin_415D00,
    &Elum::Motion_50_Knockback_415DC0};

using TBrainStateFunc = decltype(&Elum::Brain_0_WithoutAbe_416190);

const TBrainStateFunc sElum_brain_table_4C52E8[] = {
    &Elum::Brain_0_WithoutAbe_416190,
    &Elum::Brain_1_HoneyAddiction_411730,
};

const AnimId gElumMotionAnimIds[52] = {
    AnimId::Elum_Idle,
    AnimId::Elum_Idle,
    AnimId::Elum_Idle,
    AnimId::Elum_WalkLoop,
    AnimId::Elum_Turn,
    AnimId::Elum_WalkToIdle,
    AnimId::Elum_MidWalkToIdle,
    AnimId::Elum_IdleToWalk2,
    AnimId::Elum_IdleToWalk1,
    AnimId::Elum_ToYell,
    AnimId::Elum_Yell,
    AnimId::Elum_Unknown1,
    AnimId::Elum_RunTurn,
    AnimId::Elum_RunTurnToWalk,
    AnimId::Elum_Speak,
    AnimId::Elum_Speak,
    AnimId::Elum_Speak,
    AnimId::Elum_WalkToIdle,
    AnimId::Elum_MidWalkToIdle,
    AnimId::Elum_Idle,
    AnimId::Elum_Idle,
    AnimId::Elum_Land,
    AnimId::Elum_RunOffEdge,
    AnimId::Elum_WalkOffEdge,
    AnimId::Elum_JumpToFall,
    AnimId::Elum_LickingHoney,
    AnimId::Elum_LickingToStruggle,
    AnimId::Elum_AbeMountingEnd,
    AnimId::Elum_AbeUnmountingEnd,
    AnimId::Elum_BeesStruggling,
    AnimId::Elum_HopBegin,
    AnimId::Elum_HopMid,
    AnimId::Elum_HopLand,
    AnimId::Elum_RunJumpBegin,
    AnimId::Elum_RunJumpMid,
    AnimId::Elum_RunJumpLand,
    AnimId::Elum_RunLoop,
    AnimId::Elum_RunSlideStop,
    AnimId::Elum_RunTurnToRun,
    AnimId::Elum_IdleToRun,
    AnimId::Elum_WalkToRun,
    AnimId::Elum_MidWalkToRun,
    AnimId::Elum_RunToWalk,
    AnimId::Elum_MidRunToWalk,
    AnimId::Elum_ScratchBegin,
    AnimId::Elum_ScratchLoop,
    AnimId::Elum_ScratchEnd,
    AnimId::Elum_ScratchUnknown,
    AnimId::Elum_MountUnmountBegin,
    AnimId::Elum_MountUnmountBegin,
    AnimId::Elum_Knockback,
    AnimId::None};

Elum::~Elum()
{
    gElum = nullptr;
    VOnTrapDoorOpen();
}

void Elum::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eContinuePoint)
        {
            auto pContinueTlv = static_cast<relive::Path_ContinuePoint*>(pTlv);
            if (mContinueZoneNumber != pContinueTlv->mZoneNumber && pContinueTlv->mZoneNumber > field_142_zone_number && pContinueTlv->mElumRestarts == relive::reliveChoice::eYes)
            {
                mContinueZoneNumber = pContinueTlv->mZoneNumber;
                mContinueRect.x = pContinueTlv->mTopLeftX;
                mContinueRect.y = pContinueTlv->mTopLeftY;
                mContinueRect.w = pContinueTlv->mBottomRightX;
                mContinueRect.h = pContinueTlv->mBottomRightY;

                mContinuePath = gMap.mCurrentPath;
                mContinueCamera = gMap.mCurrentCamera;
                mContinueLevel = gMap.mCurrentLevel;
                mContinueSpriteScale = mSpriteScale;
                mRespawnOnDead = 1;
            }
        }
        else if (pTlv->mTlvType == ReliveTypes::eDeathDrop && mHealth > FP_FromInteger(0))
        {
            if (sControlledCharacter != this)
            {
                mDontFollowAbe = 0;
            }
            Elum_SFX_416E10(ElumSounds::eHowl_2, this);
            mAnim.mFlags.Clear(AnimFlags::eRender);
            mCurrentMotion = eElumMotions::Motion_20_Fall_415F70;
            mHealth = FP_FromInteger(0);
        }

        pTlv = gMap.TLV_Get_At(
            pTlv,
            mXPos,
            mYPos,
            mXPos,
            mYPos);
    }
}

s16 Elum::VTakeDamage(BaseGameObject* pFrom)
{
    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eExplosion:
            if (mHealth > FP_FromInteger(0))
            {
                Elum_SFX_416E10(ElumSounds::eExploding_7, 0);
                SfxPlayMono(relive::SoundEffects::KillEffect, 75);

                if (sActiveHero->mHealth > FP_FromInteger(0))
                {
                    mCurrentMotion = eElumMotions::Motion_20_Fall_415F70;
                }
                else
                {
                    mCurrentMotion = eElumMotions::Motion_19_Dead_415F90;
                }

                relive_new Gibs(
                    GibType::Elum_3,
                    mXPos,
                    mYPos,
                    mVelX,
                    mVelY,
                    mSpriteScale);

                mHealth = FP_FromInteger(0);

                if (sControlledCharacter != this)
                {
                    mDontFollowAbe = 0;
                }

                mAnim.mFlags.Clear(AnimFlags::eRender);
                mAnim.Set_Animation_Data(GetAnimRes(gElumMotionAnimIds[mCurrentMotion]));
            }
            return 1;

        case ReliveTypes::eAbilityRing:
            return 0;

        case ReliveTypes::eBeeSwarm:
            if (sControlledCharacter != this)
            {
                field_170_flags.Set(Elum::Flags_170::eStrugglingWithBees_Bit1);
            }
            break;

        default:
            return 1;
    }
    return 1;
}

void Elum::ToKnockback()
{
    mVelX = FP_FromInteger(0);
    mCurrentMotion = eElumMotions::Motion_50_Knockback_415DC0;

    if (BaseAliveGameObjectCollisionLine)
    {
        MoveOnLine(0);
    }
    MapFollowMe(TRUE);
    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 95, -200, this);
}

void Elum::VOnTrapDoorOpen()
{
    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
    }
}

void Elum::Vsub_416120()
{
    ToIdle();

    mAnim.Set_Animation_Data(GetAnimRes(gElumMotionAnimIds[mCurrentMotion]));
}

void Elum::MidWalkToNextMotion_412FA0()
{
    if (mVelX <= FP_FromInteger(0))
    {
        if (WallHit(mSpriteScale * FP_FromInteger(40), -ScaleToGridSize(mSpriteScale)))
        {
            mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
            return;
        }
    }
    else
    {
        if (WallHit(mSpriteScale * FP_FromInteger(40), ScaleToGridSize(mSpriteScale)))
        {
            mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
            return;
        }
    }

    if ((mVelX > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left)) || (mVelX < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right)) || !Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
    {
        mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
    }
    else
    {
        if (field_10E_pressed & sInputKey_Hop)
        {
            mCurrentMotion = eElumMotions::Motion_18_MidWalkToHop_4136A0;
        }
    }
    field_10E_pressed = 0;
}

void Elum::WalkToNextMotion_4130D0()
{
    if (mVelX <= FP_FromInteger(0))
    {
        if (WallHit(mSpriteScale * FP_FromInteger(40), -ScaleToGridSize(mSpriteScale)))
        {
            mCurrentMotion = eElumMotions::Motion_5_WalkToIdle_4132D0;
            return;
        }
    }
    else
    {
        if (WallHit(mSpriteScale * FP_FromInteger(40), ScaleToGridSize(mSpriteScale)))
        {
            mCurrentMotion = eElumMotions::Motion_5_WalkToIdle_4132D0;
            return;
        }
    }

    if ((mVelX > FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Left)) || (mVelX < FP_FromInteger(0) && Input().IsAnyPressed(sInputKey_Right)) || !Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
    {
        mCurrentMotion = eElumMotions::Motion_5_WalkToIdle_4132D0;
        field_10E_pressed = 0;
    }
    else
    {
        if (field_10E_pressed & sInputKey_Hop)
        {
            mCurrentMotion = eElumMotions::Motion_17_WalkToHop_413620;
        }
        field_10E_pressed = 0;
    }
}

void Elum::SlowOnX_414210(FP amount)
{
    if (FP_GetExponent(mVelX))
    {
        MoveOnLine(0);

        if (mVelX <= FP_FromInteger(0))
        {
            if (mVelX < FP_FromInteger(0))
            {
                mVelX += (mSpriteScale * amount);
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = FP_FromInteger(0);
                }
            }
        }
        else
        {
            mVelX -= (mSpriteScale * amount);
            if (mVelX < FP_FromInteger(0))
            {
                mVelX = FP_FromInteger(0);
            }
        }
    }
    else
    {
        CheckLiftPointGoneAndSetCamera();
    }
}

void Elum::CheckLiftPointGoneAndSetCamera()
{
    if (mLiftPoint)
    {
        if (mLiftPoint->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            VOnTrapDoorOpen();
            field_170_flags.Set(Elum::Flags_170::eFalling_Bit3);
        }

        if (sControlledCharacter == this)
        {
            SetActiveCameraDelayedFromDir_401C90();
        }
    }
}

void Elum::MoveOnLine(s16 xLookAhead)
{
    CheckLiftPointGoneAndSetCamera();

    const FP oldX = mXPos;
    FP xpos_off_fp = mXPos + FP_FromInteger(xLookAhead);
    BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
        &xpos_off_fp,
        &mYPos,
        mVelX);

    if (BaseAliveGameObjectCollisionLine)
    {
        mXPos += mVelX;
        if (mLiftPoint)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32)
            {
                VOnTrapDoorOpen();
            }
        }
        else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjListPlatforms_50766C,
                (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
        }
    }
    else
    {
        VOnTrapDoorOpen();
        mCurrentMotion = eElumMotions::Motion_23_WalkOffEdge_415E90;
        BaseAliveGameObjectLastLineYPos = mYPos;
        mXPos = mVelX + oldX;
    }
}

void Elum::SetAbeAsPlayer(s16 abeMotion)
{
    // Back to Abe
    if (sControlledCharacter == this)
    {
        sControlledCharacter = sActiveHero;
        sActiveHero->mNextMotion = abeMotion;
    }

    /*
    // re-load some elum resources
    field_104_pending_resource_count += 2;

    ResourceManager::LoadResourceFile(
        "ELMALONE.BAN",
        BaseAliveGameObject::OnResourceLoaded_4019A0,
        this);

    ResourceManager::LoadResourceFile(
        "ELMPRMNT.BAN",
        BaseAliveGameObject::OnResourceLoaded_4019A0,
        this);
    */
}

s16 Elum::ToNextMotion_4120F0()
{
    if (sControlledCharacter == this)
    {
        return ToNextMotionAbeControlled_411E40();
    }

    switch (mNextMotion)
    {
        case eElumMotions::Motion_3_WalkLoop_412C90:
            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (WallHit(mSpriteScale * FP_FromInteger(25), ScaleToGridSize(mSpriteScale)))
                {
                    return 0;
                }
                mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
            }
            else
            {
                if (WallHit(mSpriteScale * FP_FromInteger(25), ScaleToGridSize(mSpriteScale)))
                {
                    return 0;
                }
                mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
            }

            if (field_124_bShould_IdleToWalk1)
            {
                mCurrentMotion = eElumMotions::Motion_7_IdleToWalk1_413200;
                field_124_bShould_IdleToWalk1 = field_124_bShould_IdleToWalk1 == 0;
            }
            else
            {
                mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2_413270;
                field_124_bShould_IdleToWalk1 = 1;
            }

            mNextMotion = -1;
            return 1;

        case eElumMotions::Motion_29_BeesStruggling_412A90:
            Elum_SFX_416E10(ElumSounds::eBeesStruggle_3, 0);
            mCurrentMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
            field_110_timer = sGnFrame + 25;
            mNextMotion = -1;
            return 1;

        case eElumMotions::Motion_4_Turn_4140F0:
        case eElumMotions::Motion_14_Speak_414860:
        case eElumMotions::Motion_15_Speak_4148F0:
        case eElumMotions::Motion_16_Speak_414980:
            mCurrentMotion = mNextMotion;
            mNextMotion = -1;
            return 1;

        case eElumMotions::Motion_10_Yell_4158E0:
            mCurrentMotion = eElumMotions::Motion_9_ToYell_415890;
            mNextMotion = -1;
            return 1;

        case eElumMotions::Motion_45_ScratchLoop_4127B0:
            mCurrentMotion = eElumMotions::Motion_44_ScratchBegin_412730;
            mNextMotion = -1;
            return 1;

        default:
            return 0;
    }
}

s16 Elum::ToNextMotionAbeControlled_411E40()
{
    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
    if (pLiftPoint)
    {
        if (pLiftPoint->field_10C == 1)
        {
            if (!pLiftPoint->OnAnyFloor())
            {
                return 0;
            }
        }
    }

    if (Input().IsAnyPressed(sInputKey_Left | sInputKey_Right))
    {
        FP gridSize = {};
        if (Input().IsAnyPressed(sInputKey_Right))
        {
            gridSize = ScaleToGridSize(mSpriteScale);

            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mCurrentMotion = eElumMotions::Motion_4_Turn_4140F0;
                return 1;
            }
        }
        else if (Input().IsAnyPressed(sInputKey_Left))
        {
            gridSize = -ScaleToGridSize(mSpriteScale);

            if (!mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mCurrentMotion = eElumMotions::Motion_4_Turn_4140F0;
                return 1;
            }
        }

        if (WallHit(mSpriteScale * FP_FromInteger(40), gridSize))
        {
            return 0;
        }

        if (Input().IsAnyPressed(sInputKey_Run))
        {
            mVelX = gridSize / FP_FromInteger(4);
            mCurrentMotion = eElumMotions::Motion_39_IdleToRun_413B00;
        }
        else
        {
            mVelX = gridSize / FP_FromInteger(9);
            mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2_413270;
        }
        return 1;
    }
    else
    {
        if (Input().IsAnyPressed(sInputKey_LeftGameSpeakEnabler | sInputKey_RightGameSpeakEnabler)
            || !(Input().IsAnyPressed(sInputKey_Hop)))
        {
            return 0;
        }

        if (mAnim.mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = mSpriteScale * FP_FromInteger(-15);
        }
        else
        {
            mVelX = mSpriteScale * FP_FromInteger(15);
        }

        mCurrentMotion = eElumMotions::Motion_30_HopBegin_414E30;
        return 1;
    }
}

void Elum::HandleElumPathTrans_411460()
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);

    if (sActiveHero->mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mXPos = ScaleToGridSize(mSpriteScale) + FP_FromInteger(camCoords.x + XGrid_Index_To_XPos(mSpriteScale, MaxGridBlocks_41FA10(mSpriteScale)));
    }
    else
    {
        mXPos = FP_FromInteger(camCoords.x + XGrid_Index_To_XPos(mSpriteScale, 0)) - ScaleToGridSize(mSpriteScale);
    }

    if (sActiveHero->BaseAliveGameObjectCollisionLine)
    {
        mYPos = sActiveHero->mYPos;
    }
    else
    {
        mYPos = FP_FromInteger(camCoords.y + FP_GetExponent(mYPos) % 480);
    }

    if (BaseAliveGameObjectCollisionLine && BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32)
    {
        mLiftPoint = nullptr;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos - FP_FromInteger(40),
            mXPos,
            mYPos + FP_FromInteger(40),
            &pLine,
            &hitX,
            &hitY,
            mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
    {
        mYPos = hitY;
        BaseAliveGameObjectCollisionLine = pLine;
    }
    else
    {
        BaseAliveGameObjectCollisionLine = nullptr;
    }

    mCurrentLevel = gMap.mCurrentLevel;
    mCurrentPath = gMap.mCurrentPath;

    SetUpdateDelay(20);
}

const relive::SfxDefinition sElumSfx_4C5398[12] = {
    {0, 4, 36, 50, -127, 0},
    {0, 4, 37, 50, -127, 0},
    {0, 4, 38, 50, -127, 0},
    {0, 5, 61, 118, -1, 1},
    {0, 5, 67, 80, -630, -630},
    {0, 5, 63, 110, 0, 0},
    {0, 18, 46, 127, -1700, -1700},
    {0, 3, 59, 90, -600, -600},
    {0, 5, 64, 60, -1, 1},
    {0, 5, 65, 60, -1, 1},
    {0, 5, 66, 60, -1, 1},
    {0, 21, 64, 60, -1, 1}};

void Elum::Elum_SFX_416E10(ElumSounds soundId, BaseAliveGameObject* pObj)
{
    switch (soundId)
    {
        case ElumSounds::eWalkingFootstep_0:
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[Math_RandomRange(0, 2)], Math_RandomRange(54, 58), -700, -573);
            break;

        case ElumSounds::eRunningFootstep_1:
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[Math_RandomRange(0, 2)], Math_RandomRange(66, 70), -573, -446);
            break;

        case ElumSounds::eHowl_2:
        {
            CameraPos dir = CameraPos::eCamCurrent_0;
            if (pObj)
            {
                dir = gMap.GetDirection(
                    pObj->mCurrentLevel,
                    pObj->mCurrentPath,
                    pObj->mXPos,
                    pObj->mYPos);
            }

            s32 volLeft = 0;
            s32 volRight = 0;
            switch (dir)
            {
                case CameraPos::eCamCurrent_0:
                    volLeft = 118;
                    volRight = 118;
                    break;

                case CameraPos::eCamRight_4:
                    volLeft = 26;
                    volRight = 80;
                    break;

                case CameraPos::eCamLeft_3:
                    volLeft = 80;
                    volRight = 26;
                    break;

                default:
                    volLeft = 80;
                    volRight = 80;
                    break;
            }
            SFX_SfxDefinition_Play_477330(sElumSfx_4C5398[3], (s16) volLeft, (s16) volRight, 0, 0);
            break;
        }

        case ElumSounds::eBeesStruggle_3:
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[4], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eHitGroundSoft_4:
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[7], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eRunSlide_5:
            Environment_SFX_42A220(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, 0);
            break;

        case ElumSounds::eSpottedHoney_6:
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[5], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eExploding_7:
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[6], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eLickingHoney_8:
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[Math_RandomRange(8, 10)], 0, 0x7FFF, 0x7FFF);
            break;

        default:
            return;
    }
}

void Elum::FindHoney_411600()
{
    if (mCurrentMotion != eElumMotions::Motion_25_LickingHoney_415B50)
    {
        field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);

        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);

            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->Type() == ReliveTypes::eHoney)
            {
                auto pHoney = static_cast<Honey*>(pObjIter);
                if (gMap.Is_Point_In_Current_Camera(
                        pHoney->mCurrentLevel,
                        pHoney->mCurrentPath,
                        pHoney->mXPos,
                        pHoney->mYPos,
                        0)
                    && gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    field_12C_honey_xpos = FP_GetExponent(pHoney->mXPos);
                    field_12E_honey_ypos = FP_GetExponent(pHoney->mYPos);
                    field_170_flags.Set(Elum::Flags_170::eFoundHoney_Bit4);
                    break;
                }
            }
        }
    }
}

s16 Elum::NearHoney_411DA0()
{
    if (field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4))
    {
        auto pLiftPoint = static_cast<LiftPoint*>(mLiftPoint);
        if (pLiftPoint && pLiftPoint->field_10C == 1 && !pLiftPoint->OnAnyFloor())
        {
            // We're on a lift that isn't on a floor
            return 0;
        }

        return abs(FP_GetExponent(mYPos) - field_12E_honey_ypos) <= 24 ? 1 : 0;
    }
    return 0;
}

s16 Elum::Brain_0_WithoutAbe_416190()
{
    const s32 last_event_index = gEventSystem->field_18_last_event_index;

    GameSpeakEvents last_event_v1;
    GameSpeakEvents last_event_v2;

    if (field_158_last_event_idx == last_event_index)
    {
        const bool bLastEventMinus1 = gEventSystem->field_10_last_event == GameSpeakEvents::eNone_m1;
        last_event_v2 = bLastEventMinus1 ? GameSpeakEvents::eNone_m1 : GameSpeakEvents::eSameAsLast_m2;
        last_event_v1 = bLastEventMinus1 ? GameSpeakEvents::eNone_m1 : GameSpeakEvents::eSameAsLast_m2;
    }
    else
    {
        field_158_last_event_idx = last_event_index;
        last_event_v1 = gEventSystem->field_10_last_event;
        last_event_v2 = gEventSystem->field_10_last_event;
    }

    if (field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
    {
        mNextMotion = eElumMotions::Motion_1_Idle_412990;
        field_128_brain_idx = 1;
        return 0;
    }

    const FP kGridSize = ScaleToGridSize(mSpriteScale);

    // TODO: A lot of duplicated logic below and a lot of simplifaction
    // can be done also

    switch (field_12A_brain_sub_state)
    {
        case 0:
            return 2;

        case 1:
        {
            const FP xd = sActiveHero->mXPos - mXPos;
            if (FP_Abs(xd) < (kGridSize * FP_FromInteger(2)))
            {
                mNextMotion = eElumMotions::Motion_1_Idle_412990;
                return 2;
            }

            if (xd > FP_FromInteger(0))
            {
                if (mVelX < FP_FromInteger(0))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }
            else if (xd < FP_FromInteger(0))
            {
                if (mVelX > FP_FromInteger(0))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }
            else if (mVelX > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }
            return field_12A_brain_sub_state;
        }

        case 2:
        {
            if (NearHoney_411DA0())
            {
                field_128_brain_idx = 1;
                mDontFollowAbe = 1;
                return 0;
            }

            const FP xd = sActiveHero->mXPos - mXPos;
            if (xd > (kGridSize / FP_FromInteger(2)) && mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                return 4;
            }
            else if (xd < -(kGridSize / FP_FromInteger(2)) && !mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                return 4;
            }

            if (mLiftPoint)
            {
                if (mLiftPoint->field_10C == 1)
                {
                    auto pLift = static_cast<LiftPoint*>(mLiftPoint);
                    if (!pLift->OnAnyFloor()) // TODO: Check logic
                    {
                        if (mXPos == sActiveHero->mXPos)
                        {
                            return 2;
                        }

                        if (VIsFacingMe(sActiveHero))
                        {
                            mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
                            return 3;
                        }

                        mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                        return 4;
                    }
                }
            }

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_139_ElumMountBegin)
            {
                mCurrentMotion = eElumMotions::Motion_48_AbeMoutingBegin_415C40;
                return 16;
            }

            if (last_event_v1 == GameSpeakEvents::eFollowMe_10)
            {
                if (gMap.mCurrentLevel == mCurrentLevel
                    && gMap.mCurrentPath == mCurrentPath)
                {
                    if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                    {
                        if (FP_Abs(xd) > (kGridSize / FP_FromInteger(2)))
                        {
                            field_114_respond_timer = sGnFrame + 14;
                            return 12;
                        }
                    }
                }
            }

            if (last_event_v1 == GameSpeakEvents::eHello_9
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    field_114_respond_timer = sGnFrame + 14;
                    return 9;
                }
            }

            if (last_event_v1 == GameSpeakEvents::eFart_3
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    field_114_respond_timer = sGnFrame + 28;
                    return 11;
                }
            }

            if (last_event_v1 == GameSpeakEvents::eWait_12
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath
                && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                mDontFollowAbe = 1;
                field_114_respond_timer = sGnFrame + 14;
                return 5;
            }

            if (FP_Abs(xd) > (kGridSize * FP_FromInteger(3)) && VOnSameYLevel(sActiveHero))
            {
                if (xd < FP_FromInteger(0))
                {
                    if (!Check_IsOnEndOfLine(1, 1))
                    {
                        if (!WallHit(mSpriteScale * FP_FromInteger(25), -kGridSize))
                        {
                            mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
                            return 1;
                        }
                    }
                }
                else if (xd > FP_FromInteger(0))
                {
                    if (!Check_IsOnEndOfLine(0, 1))
                    {
                        if (!WallHit(mSpriteScale * FP_FromInteger(25), kGridSize))
                        {
                            mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
                            return 1;
                        }
                    }
                }
            }
            return field_12A_brain_sub_state;
        }

        case 3:
            if (mCurrentMotion == eElumMotions::Motion_3_WalkLoop_412C90)
            {
                mNextMotion = eElumMotions::Motion_1_Idle_412990;
            }

            if (mCurrentMotion != eElumMotions::Motion_1_Idle_412990)
            {
                return field_12A_brain_sub_state;
            }

            if (mXPos == sActiveHero->mXPos)
            {
                return 2;
            }

            mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
            return field_12A_brain_sub_state;

        case 4:
            if (mCurrentMotion != eElumMotions::Motion_4_Turn_4140F0 || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return field_12A_brain_sub_state;
            }
            return 2;

        case 5:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            mNextMotion = eElumMotions::Motion_16_Speak_414980;
            return 6;

        case 6:
        {
            if (NearHoney_411DA0())
            {
                field_128_brain_idx = 1;
                mDontFollowAbe = 1;
                return 0;
            }

            const FP xd_1 = sActiveHero->mXPos - mXPos;
            if (xd_1 > (kGridSize / FP_FromInteger(2)) && mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                return 7;
            }
            else if (xd_1 < -(kGridSize / FP_FromInteger(2)) && !mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                return 7;
            }

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_139_ElumMountBegin)
            {
                mCurrentMotion = eElumMotions::Motion_48_AbeMoutingBegin_415C40;
                return 16;
            }

            if (last_event_v2 == GameSpeakEvents::eFollowMe_10 && gMap.mCurrentLevel == mCurrentLevel && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    mDontFollowAbe = 0;
                    if (FP_Abs(xd_1) < (kGridSize / FP_FromInteger(2)))
                    {
                        return 2;
                    }

                    field_114_respond_timer = sGnFrame + 14;
                    return 12;
                }
            }

            if (last_event_v2 == GameSpeakEvents::eWait_12
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    mDontFollowAbe = 1;
                    field_114_respond_timer = sGnFrame + 14;
                    return 5;
                }
            }

            if (last_event_v2 == GameSpeakEvents::eHello_9
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    field_114_respond_timer = sGnFrame + 14;
                    return 8;
                }
            }

            if (last_event_v2 == GameSpeakEvents::eFart_3
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath
                && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                field_114_respond_timer = sGnFrame + 28;
                return 10;
            }

            if (EventGet(kEventDeathReset) == 0)
            {
                return field_12A_brain_sub_state;
            }
            mNextMotion = eElumMotions::Motion_1_Idle_412990;
            return 2;
        }

        case 7:
            if (mCurrentMotion != eElumMotions::Motion_1_Idle_412990)
            {
                return field_12A_brain_sub_state;
            }
            mDontFollowAbe = 1;
            return 6;

        case 8:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            mNextMotion = eElumMotions::Motion_14_Speak_414860;
            return 6;

        case 9:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            mNextMotion = eElumMotions::Motion_14_Speak_414860;
            return 2;

        case 10:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            mNextMotion = eElumMotions::Motion_15_Speak_4148F0;
            return 6;

        case 11:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            mNextMotion = eElumMotions::Motion_15_Speak_4148F0;
            return 2;

        case 12:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return field_12A_brain_sub_state;
            }
            mNextMotion = eElumMotions::Motion_14_Speak_414860;
            return 13;

        case 13:
        {
            if (mCurrentMotion == eElumMotions::Motion_1_Idle_412990)
            {
                mNextMotion = eElumMotions::Motion_14_Speak_414860;
            }

            if (mCurrentMotion != eElumMotions::Motion_14_Speak_414860 || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return field_12A_brain_sub_state;
            }

            const FP xd = sActiveHero->mXPos - mXPos;
            if (xd >= FP_FromInteger(0))
            {
                if (xd >= (kGridSize / FP_FromInteger(2)))
                {
                    mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
                    return 14;
                }
            }
            else
            {
                if (-xd >= (kGridSize / FP_FromInteger(2)))
                {
                    mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
                    return 14;
                }
            }
            mNextMotion = eElumMotions::Motion_1_Idle_412990;
            return 2;
        }

        case 14:
        {
            if (last_event_v2 == GameSpeakEvents::eWait_12)
            {
                mDontFollowAbe = 1;
                mNextMotion = eElumMotions::Motion_1_Idle_412990;
                field_114_respond_timer = sGnFrame + 14;
                return 5;
            }

            const FP xd = sActiveHero->mXPos - mXPos;
            if (FP_Abs(xd) < (kGridSize / FP_FromInteger(2)))
            {
                mNextMotion = eElumMotions::Motion_1_Idle_412990;
                return 2;
            }

            if (xd > FP_FromInteger(0))
            {
                if (mVelX < FP_FromInteger(0))
                {
                    mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                    return 15;
                }
            }
            else if (xd < FP_FromInteger(0))
            {
                if (mVelX > FP_FromInteger(0))
                {
                    mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                    return 15;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle_412990;
                    return 2;
                }
            }
            return field_12A_brain_sub_state;
        }

        case 15:
        {
            if (mCurrentMotion != eElumMotions::Motion_4_Turn_4140F0 || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return field_12A_brain_sub_state;
            }

            if (FP_Abs(sActiveHero->mXPos - mXPos) < (kGridSize / FP_FromInteger(2)))
            {
                mNextMotion = eElumMotions::Motion_1_Idle_412990;
                return 2;
            }

            mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
            return 14;
        }

        case 16:
            if (NearHoney_411DA0())
            {
                field_128_brain_idx = 1;
                mDontFollowAbe = 1;
                return 0;
            }

            if (sControlledCharacter == this
                || mCurrentMotion == eElumMotions::Motion_48_AbeMoutingBegin_415C40
                || mCurrentMotion == eElumMotions::Motion_27_AbeMountingEnd_415CA0)
            {
                return field_12A_brain_sub_state;
            }
            mDontFollowAbe = 0;
            return 2;

        default:
            return field_12A_brain_sub_state;
    }
}

s16 Elum::Brain_1_HoneyAddiction_411730()
{
    const FP kGridSize = ScaleToGridSize(mSpriteScale);

    switch (field_12A_brain_sub_state)
    {
        case 0:
        {
            if (field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
            {
                field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);
                mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                return 5;
            }

            const FP honey_xd = mXPos - FP_FromInteger(field_12C_honey_xpos);
            if (honey_xd >= FP_FromInteger(0))
            {
                if (mAnim.mFlags.Get(AnimFlags::eFlipX))
                {
                    if (honey_xd >= (kGridSize * FP_FromInteger(2)))
                    {
                        mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
                        Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                        return 2;
                    }
                    else
                    {
                        Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                        mNextMotion = eElumMotions::Motion_25_LickingHoney_415B50;
                        if (sControlledCharacter == this)
                        {
                            SetAbeAsPlayer(eAbeMotions::Motion_128_KnockForward);
                        }
                        return 3;
                    }
                }
                mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                return 1;
            }
            else
            {
                if (!mAnim.mFlags.Get(AnimFlags::eFlipX))
                {
                    if (honey_xd <= -(kGridSize * FP_FromInteger(2)))
                    {
                        mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
                        Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                        return 2;
                    }
                    else
                    {
                        Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                        mNextMotion = eElumMotions::Motion_25_LickingHoney_415B50;
                        if (sControlledCharacter == this)
                        {
                            SetAbeAsPlayer(eAbeMotions::Motion_128_KnockForward);
                        }
                        return 3;
                    }
                }
                mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                return 1;
            }
            break;
        }

        case 1:
        {
            if (mCurrentMotion != eElumMotions::Motion_4_Turn_4140F0 || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return field_12A_brain_sub_state;
            }

            const FP honey_xd_1 = mXPos - FP_FromInteger(field_12C_honey_xpos);

            if (honey_xd_1 >= FP_FromInteger(0))
            {
                if (honey_xd_1 < (kGridSize * FP_FromInteger(2)))
                {
                    Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                    mNextMotion = eElumMotions::Motion_25_LickingHoney_415B50;
                    if (sControlledCharacter == this)
                    {
                        SetAbeAsPlayer(eAbeMotions::Motion_128_KnockForward);
                    }
                    return 3;
                }
            }
            else
            {
                if (honey_xd_1 > -(kGridSize * FP_FromInteger(2)))
                {
                    Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
                    mNextMotion = eElumMotions::Motion_25_LickingHoney_415B50;
                    if (sControlledCharacter == this)
                    {
                        SetAbeAsPlayer(eAbeMotions::Motion_128_KnockForward);
                    }
                    return 3;
                }
            }
            mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
            Elum_SFX_416E10(ElumSounds::eSpottedHoney_6, 0);
            return 2;
        }

        case 2:
            if (!NearHoney_411DA0())
            {
                mNextMotion = eElumMotions::Motion_1_Idle_412990;
                field_128_brain_idx = 0;
                return 0;
            }

            if ((mVelX <= FP_FromInteger(0) && mXPos - FP_FromInteger(field_12C_honey_xpos) >= (kGridSize * FP_FromInteger(2))) ||
                (mVelX >= FP_FromInteger(0) && FP_FromInteger(field_12C_honey_xpos) - mXPos >= (kGridSize * FP_FromInteger(2))))
            {
                return field_12A_brain_sub_state;
            }

            if (sControlledCharacter == this)
            {
                SetAbeAsPlayer(eAbeMotions::Motion_128_KnockForward);
            }

            mNextMotion = eElumMotions::Motion_25_LickingHoney_415B50;
            return 3;

        case 3:
            if (!BaseAliveGameObjectCollisionLine)
            {
                mNextMotion = eElumMotions::Motion_1_Idle_412990;
                field_128_brain_idx = 0;
                return 0;
            }

            if (!field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
            {
                return field_12A_brain_sub_state;
            }

            field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);
            field_170_flags.Set(Elum::Flags_170::eStungByBees_Bit2);

            field_146_honey_ypos = gMap.mCurrentCamera;

            mDontFollowAbe = 0;

            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mNextMotion = eElumMotions::Motion_4_Turn_4140F0;
                return 4;
            }
            else
            {
                mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;

                field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                return 5;
            }
            break;

        case 4:
            if (mCurrentMotion == eElumMotions::Motion_4_Turn_4140F0 && mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                return 5;
            }
            return field_12A_brain_sub_state;

        case 5:
            if (mCurrentMotion != eElumMotions::Motion_29_BeesStruggling_412A90 || field_110_timer > static_cast<s32>(sGnFrame))
            {
                return field_12A_brain_sub_state;
            }
            mNextMotion = eElumMotions::Motion_44_ScratchBegin_412730;
            return 6;

        case 6:
        {
            if (mCurrentMotion != eElumMotions::Motion_46_ScratchEnd_412800 || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return field_12A_brain_sub_state;
            }

            if (!field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
            {
                mNextMotion = eElumMotions::Motion_1_Idle_412990;
                field_128_brain_idx = 0;
                return 0;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine(0, 2))
                {
                    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    return 5;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(1, 2))
                {
                    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    return 5;
                }
            }

            FP gridSizeDirected = {};
            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                gridSizeDirected = -kGridSize;
            }
            else
            {
                gridSizeDirected = kGridSize;
            }

            if (WallHit(mSpriteScale * FP_FromInteger(25), gridSizeDirected))
            {
                field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                return 5;
            }
            else
            {
                field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
                return 7;
            }
            break;
        }

        case 7:
            if (mVelX > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = sGnFrame + 40;
                    return 5;
                }
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = sGnFrame + 40;
                    return 5;
                }
            }

            if (mCurrentMotion == eElumMotions::Motion_3_WalkLoop_412C90 && mAnim.mCurrentFrame == 11)
            {
                mNextMotion = eElumMotions::Motion_44_ScratchBegin_412730;
                return 9;
            }

            if (mCurrentMotion != eElumMotions::Motion_1_Idle_412990)
            {
                return field_12A_brain_sub_state;
            }

            mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
            field_110_timer = sGnFrame + 40;
            return 5;

        case 8:
            if (field_110_timer > static_cast<s32>(sGnFrame))
            {
                return field_12A_brain_sub_state;
            }

            if (!field_170_flags.Get(Elum::Flags_170::eStrugglingWithBees_Bit1))
            {
                mNextMotion = eElumMotions::Motion_1_Idle_412990;
                field_128_brain_idx = 0;
                return 0;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine(0, 2))
                {
                    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                    mNextMotion = eElumMotions::Motion_44_ScratchBegin_412730;
                    return 9;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(1, 2))
                {
                    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
                    mNextMotion = eElumMotions::Motion_44_ScratchBegin_412730;
                    return 9;
                }
            }
            field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
            mNextMotion = eElumMotions::Motion_3_WalkLoop_412C90;
            return 10;

        case 9:
            if (mCurrentMotion != eElumMotions::Motion_46_ScratchEnd_412800 || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return field_12A_brain_sub_state;
            }
            mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
            return 8;

        case 10:
            if (mVelX > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = sGnFrame + 40;
                    return 8;
                }
            }
            else if (mVelX < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = sGnFrame + 40;
                    return 8;
                }
            }

            if (mCurrentMotion != eElumMotions::Motion_3_WalkLoop_412C90 || mAnim.mCurrentFrame != 11)
            {
                if (mCurrentMotion == eElumMotions::Motion_1_Idle_412990)
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
                    field_110_timer = sGnFrame + 40;
                    return 8;
                }
                else
                {
                    return field_12A_brain_sub_state;
                }
            }

            mNextMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
            return 5;

        default:
            return field_12A_brain_sub_state;
    }
}

void Elum::Motion_0_Respawn_414C60()
{
    Motion_21_Land_414A20();
}

void Elum::Motion_1_Idle_412990()
{
    CheckLiftPointGoneAndSetCamera();

    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_115_ElumSpeak && !ToNextMotion_4120F0())
    {
        if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_150_Chant && sControlledCharacter == sActiveHero)
        {
            mCurrentMotion = eElumMotions::Motion_9_ToYell_415890;
        }
        else if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_137_ElumUnmountBegin)
        {
            mCurrentMotion = eElumMotions::Motion_49_AbeUnmountingBegin_415D00;
        }
        else if (sGnFrame - field_110_timer > 200 && sControlledCharacter != this)
        {
            mCurrentMotion = eElumMotions::Motion_44_ScratchBegin_412730;
        }
    }
}

void Elum::Motion_2_Unknown_412C30()
{
    ToIdle();
}

void Elum::Motion_3_WalkLoop_412C90()
{
    field_10E_pressed |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (mCurrentMotion == eElumMotions::Motion_3_WalkLoop_412C90)
    {
        if (mAnim.mCurrentFrame == 2)
        {
            if (mNextMotion == eElumMotions::Motion_25_LickingHoney_415B50)
            {
                mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
            }
            else
            {
                FP offX_directed = {};
                if (mAnim.mFlags.Get(AnimFlags::eFlipX))
                {
                    offX_directed = -ScaleToGridSize(mSpriteScale);
                }
                else
                {
                    offX_directed = ScaleToGridSize(mSpriteScale);
                }

                if (WallHit(mSpriteScale * FP_FromInteger(25), offX_directed))
                {
                    mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
                }
                else if (sControlledCharacter == this && !field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4))
                {
                    MidWalkToNextMotion_412FA0();
                }
                /*
                else if (field_104_pending_resource_count > 0)
                {
                    mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
                }
                */
                else
                {
                    if (mNextMotion == eElumMotions::Motion_1_Idle_412990)
                    {
                        mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
                        mNextMotion = -1;
                    }
                    else if (mNextMotion == eElumMotions::Motion_4_Turn_4140F0)
                    {
                        mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
                    }
                }
            }
        }
        else if (mAnim.mCurrentFrame == 5)
        {
            if (!field_10C_bFootStep2)
            {
                field_10C_bFootStep2 = 1;
                MapFollowMe(1);
            }

            if (sControlledCharacter != this)
            {
                Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
                return;
            }

            if (Input().IsAnyPressed(sInputKey_Run))
            {
                mCurrentMotion = eElumMotions::Motion_41_MidWalkToRun_413560;
            }

            field_10E_pressed = 0;
            Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
        }
        else if (mAnim.mCurrentFrame == 11)
        {
            if (mNextMotion == eElumMotions::Motion_25_LickingHoney_415B50)
            {
                mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle_4133F0;
            }
            else
            {
                FP offX_directed = {};
                if (mAnim.mFlags.Get(AnimFlags::eFlipX))
                {
                    offX_directed = -ScaleToGridSize(mSpriteScale);
                }
                else
                {
                    offX_directed = ScaleToGridSize(mSpriteScale);
                }

                if (WallHit(mSpriteScale * FP_FromInteger(25), offX_directed))
                {
                    mCurrentMotion = eElumMotions::Motion_5_WalkToIdle_4132D0;
                }
                else if (sControlledCharacter == this && !field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4))
                {
                    WalkToNextMotion_4130D0();
                }
                /*
                else if (field_104_pending_resource_count)
                {
                    mCurrentMotion = eElumMotions::Motion_5_WalkToIdle_4132D0;
                }
                */
                else
                {
                    if (mNextMotion == eElumMotions::Motion_1_Idle_412990)
                    {
                        mCurrentMotion = eElumMotions::Motion_5_WalkToIdle_4132D0;
                        mNextMotion = -1;
                    }
                    else if (mNextMotion == eElumMotions::Motion_4_Turn_4140F0
                             || mNextMotion == eElumMotions::Motion_44_ScratchBegin_412730
                             || mNextMotion == eElumMotions::Motion_29_BeesStruggling_412A90)
                    {
                        mCurrentMotion = eElumMotions::Motion_5_WalkToIdle_4132D0;
                    }
                }
            }
        }
        else if (mAnim.mCurrentFrame == 14)
        {
            if (!field_10C_bFootStep2)
            {
                field_10C_bFootStep2 = 1;
                MapFollowMe(1);
            }

            if (sControlledCharacter != this)
            {
                Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
                return;
            }

            if (Input().IsAnyPressed(sInputKey_Run))
            {
                mCurrentMotion = eElumMotions::Motion_40_WalkToRun_4134B0;
            }

            field_10E_pressed = 0;
            Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
        }
        else
        {
            field_10C_bFootStep2 = 0;
        }
    }
}

void Elum::Motion_4_Turn_4140F0()
{
    CheckLiftPointGoneAndSetCamera();

    if (!mAnim.mCurrentFrame)
    {
        Environment_SFX_42A220(EnvironmentSfx::eRollingNoise_8, 0, 0x7FFF, this);
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MapFollowMe(TRUE);
        mAnim.mFlags.Toggle(AnimFlags::eFlipX);

        if (mNextMotion == eElumMotions::Motion_29_BeesStruggling_412A90)
        {
            Elum_SFX_416E10(ElumSounds::eBeesStruggle_3, 0);
            mCurrentMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
            mNextMotion = -1;
            field_110_timer = sGnFrame + 25;
        }
        else if (ToNextMotion_4120F0())
        {
            mAnim.Set_Animation_Data(mAnim.mAnimRes);
            if (sControlledCharacter == this)
            {
                sActiveHero->SyncToElum(mCurrentMotion);
            }
        }
        else
        {
            ToIdle();
        }
    }
}

void Elum::Motion_5_WalkToIdle_4132D0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (mAnim.mCurrentFrame == 0)
    {
        Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
    }
    else if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MapFollowMe(1);

        if (mNextMotion == eElumMotions::Motion_29_BeesStruggling_412A90)
        {
            Elum_SFX_416E10(ElumSounds::eBeesStruggle_3, 0);
            mCurrentMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
            field_110_timer = sGnFrame + 25;
            mNextMotion = -1;
        }
        else if (mNextMotion == eElumMotions::Motion_44_ScratchBegin_412730)
        {
            mCurrentMotion = eElumMotions::Motion_44_ScratchBegin_412730;
            mNextMotion = -1;
        }
        else
        {
            ToIdle();
            /*
            if (field_104_pending_resource_count > 0)
            {
                mCurrentMotion = eElumMotions::Motion_2_Unknown_412C30;
            }
            else
            */
            {
                if (mNextMotion == eElumMotions::Motion_4_Turn_4140F0 || mNextMotion == eElumMotions::Motion_25_LickingHoney_415B50)
                {
                    mCurrentMotion = mNextMotion;
                    mNextMotion = -1;
                }
            }
        }
    }
}

void Elum::Motion_6_MidWalkToIdle_4133F0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (mAnim.mCurrentFrame == 0)
    {
        Elum_SFX_416E10(ElumSounds::eWalkingFootstep_0, 0);
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MapFollowMe(1);

        ToIdle();

        /*
        if (field_104_pending_resource_count)
        {
            mCurrentMotion = eElumMotions::Motion_2_Unknown_412C30;
        }
        else
        */
        {
            if (mNextMotion == eElumMotions::Motion_4_Turn_4140F0 || mNextMotion == eElumMotions::Motion_25_LickingHoney_415B50)
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = -1;
            }
        }
    }
}

void Elum::Motion_7_IdleToWalk2_413200()
{
    field_10E_pressed |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mPreviousMotion = 3;
        mBaseAliveGameObjectLastAnimFrame = eElumMotions::Motion_9_ToYell_415890;
        field_120_bUnknown = 1;
    }

    MoveOnLine(0);
}

void Elum::Motion_8_IdleToWalk1_413270()
{
    field_10E_pressed |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_3_WalkLoop_412C90;
    }

    MoveOnLine(0);
}

void Elum::Motion_9_ToYell_415890()
{
    CheckLiftPointGoneAndSetCamera();

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_10_Yell_4158E0;
    }
}

void Elum::Motion_10_Yell_4158E0()
{
    EventBroadcast(kEventNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (mAnim.mCurrentFrame || !field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }
    else
    {
        Elum_SFX_416E10(ElumSounds::eHowl_2, this);
        field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_11_Unknown_4159A0()
{
    Motion_47_Unknown_415A30();
}

void Elum::Motion_12_RunTurn_414520()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (!mAnim.mCurrentFrame)
    {
        Elum_SFX_416E10(ElumSounds::eRunSlide_5, 0);
    }

    const FP offY = (sControlledCharacter == this) ? mSpriteScale * FP_FromInteger(40) : mSpriteScale * FP_FromInteger(25);
    if (WallHit(offY, mVelX))
    {
        mCurrentMotion = eElumMotions::Motion_50_Knockback_415DC0;
        if (BaseAliveGameObjectCollisionLine)
        {
            mVelX = -mVelX;
            MoveOnLine(0);
        }
        else
        {
            mXPos -= mVelX;
        }
        mVelX = FP_FromInteger(0);
        MapFollowMe(TRUE);
        Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 95, -200, this);
        return;
    }

    SlowOnX_414210(FP_FromDouble(2.125));

    if (mCurrentMotion == eElumMotions::Motion_12_RunTurn_414520
        && mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MapFollowMe(TRUE);

        const FP gridSize = (mAnim.mFlags.Get(AnimFlags::eFlipX)) ? ScaleToGridSize(mSpriteScale) : -ScaleToGridSize(mSpriteScale);
        if (Input().IsAnyPressed(sInputKey_Run))
        {
            mVelX = gridSize / FP_FromInteger(4);
            mCurrentMotion = eElumMotions::Motion_38_RunTurnToRun_414810;
            return;
        }
        mVelX = gridSize / FP_FromInteger(9);
        mCurrentMotion = eElumMotions::Motion_13_RunTurnToWalk_4147C0;
    }
}

void Elum::Motion_13_RunTurnToWalk_4147C0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_3_WalkLoop_412C90;
        mAnim.mFlags.Toggle(AnimFlags::eFlipX);
    }
}

void Elum::Motion_14_Speak_414860()
{
    if (mAnim.mCurrentFrame == 3 && field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
    {
        Environment_SFX_42A220(EnvironmentSfx::eElumOkay_21, 75, 0, this);
        field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
    }
    else
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_15_Speak_4148F0()
{
    if (mAnim.mCurrentFrame == 3 && field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
    {
        Environment_SFX_42A220(EnvironmentSfx::eElumHowl_20, 75, 0, this);
        field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
    }
    else
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_16_Speak_414980()
{
    if (mAnim.mCurrentFrame == 3 && field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
    {
        Environment_SFX_42A220(EnvironmentSfx::eElumHowl_20, 75, 0, this);
        field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
    }
    else
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mNextMotion = eElumMotions::Motion_10_Yell_4158E0;
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_17_Unknown_413620()
{
    MoveOnLine(0);

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MapFollowMe(1);

        if (mAnim.mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = (mSpriteScale * FP_FromInteger(-15));
        }
        else
        {
            mVelX = (mSpriteScale * FP_FromInteger(15));
        }

        mCurrentMotion = eElumMotions::Motion_30_HopBegin_414E30;
    }
}

void Elum::Motion_18_Unknown_4136A0()
{
    Motion_17_Unknown_413620();
}

void Elum::Motion_19_Dead_415F90()
{
    if (sActiveHero->mHealth > FP_FromInteger(0))
    {
        if (!sActiveHero->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel) && sActiveHero->field_2A8_flags.Get(Flags_2A8::e2A8_Bit8_bLandSoft) /* && field_104_pending_resource_count == 0*/)
        {
            mXPos = FP_FromInteger(mContinueRect.x);
            mYPos = FP_FromInteger(mContinueRect.y);

            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);

            mCurrentLevel = mContinueLevel;
            mCurrentPath = mContinuePath;
            mSpriteScale = mContinueSpriteScale;

            field_128_brain_idx = 0;
            field_12A_brain_sub_state = 6;
            mDontFollowAbe = 1;

            if (!gMap.Is_Point_In_Current_Camera(mContinueLevel, mContinuePath, mXPos, mYPos, 0))
            {
                Elum_SFX_416E10(ElumSounds::eHowl_2, this);
            }

            field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
            field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);
            field_110_timer = sGnFrame;

            mAnim.mFlags.Set(AnimFlags::eFlipX, sActiveHero->mAnim.mFlags.Get(AnimFlags::eFlipX));

            if (field_170_flags.Get(Elum::Flags_170::eStungByBees_Bit2))
            {
                if (mContinueCamera < field_146_honey_ypos)
                {
                    field_170_flags.Clear(Elum::Flags_170::eStungByBees_Bit2);
                }
            }

            mAnim.mFlags.Set(AnimFlags::eRender);

            BaseAliveGameObjectLastLineYPos = mYPos;
            mCurrentMotion = 0;
            mNextMotion = -1;
            BaseAliveGameObjectCollisionLine = nullptr;
            mHealth = FP_FromInteger(1);

            /*
            if (field_174_resources.res[20])
            {
                ResourceManager::FreeResource_455550(field_174_resources.res[20]);
                field_174_resources.res[20] = nullptr;
            }

            if (!field_174_resources.res[30])
            {
                VLoadUnmountedResources_411260();
            }*/
        }
    }
}

void Elum::Motion_20_Fall_415F70()
{
    if (sActiveHero->mHealth <= FP_FromInteger(0))
    {
        mCurrentMotion = eElumMotions::Motion_19_Dead_415F90;
    }
}

void Elum::Motion_21_Land_414A20()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (mSpriteScale * field_118_jump_velx);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (mSpriteScale * field_118_jump_velx);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const s16 bHit = InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

    if (sControlledCharacter == this)
    {
        SetActiveCameraDelayedFromDir_401C90();
    }

    if (bHit)
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                Elum_SFX_416E10(ElumSounds::eHitGroundSoft_4, 0);
                BaseAliveGameObjectCollisionLine = pLine;
                if (mYPos - BaseAliveGameObjectLastLineYPos >= FP_FromInteger(20))
                {
                    mCurrentMotion = eElumMotions::Motion_22_RunOffEdge_415810;
                }
                else
                {
                    mCurrentMotion = eElumMotions::Motion_1_Idle_412990;
                }

                mXPos = hitX;
                mYPos = hitY;
                MapFollowMe(TRUE);

                PSX_Point xy = {};
                xy.x = FP_GetExponent(mXPos - FP_FromInteger(10));
                xy.y = FP_GetExponent(mYPos - FP_FromInteger(10));

                PSX_Point wh = {};
                wh.x = FP_GetExponent(mXPos + FP_FromInteger(10));
                wh.y = FP_GetExponent(mYPos + FP_FromInteger(10));

                VOnCollisionWith(
                    xy,
                    wh,
                    ObjListPlatforms_50766C,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                break;
            }

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackgroundWallLeft_5:
            case eLineTypes::eBackgroundWallRight_6:
                if (bHit)
                {
                    mVelX = (-mVelX / FP_FromInteger(2));
                }
                break;

            default:
                break;
        }
    }
}

void Elum::Motion_22_RunOffEdge_415810()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MapFollowMe(1);
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_23_WalkOffEdge_415E90()
{
    Motion_24_JumpToFall_415ED0();
}

void Elum::Motion_24_JumpToFall_415ED0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_21_Land_414A20;
    }

    Motion_21_Land_414A20();
}

void Elum::Motion_25_LickingHoney_415B50()
{
    EventBroadcast(kEventNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (mAnim.mCurrentFrame == 6)
    {
        if (gMap.GetDirection(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos)
            == CameraPos::eCamCurrent_0)
        {
            if (field_170_flags.Get(Elum::Flags_170::eCanSpeak_Bit6))
            {
                Elum_SFX_416E10(ElumSounds::eLickingHoney_8, 0);
                field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);
            }
        }
    }
    else
    {
        field_170_flags.Set(Elum::Flags_170::eCanSpeak_Bit6);
    }

    if (mAnim.mFlags.Get(AnimFlags::eForwardLoopCompleted))
    {
        if (mNextMotion == eElumMotions::Motion_4_Turn_4140F0 || mNextMotion == eElumMotions::Motion_29_BeesStruggling_412A90)
        {
            mCurrentMotion = eElumMotions::Motion_26_LickingToStruggling_415AC0;
            /*
            if (!field_174_resources.res[30])
            {
                VLoadUnmountedResources_411260();
            }*/
        }
    }
}

void Elum::Motion_26_LickingToStruggling_415AC0()
{
    Motion_47_Unknown_415A30();
}

void Elum::Motion_27_AbeMountingEnd_415CA0()
{
    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_136_ElumMountEnd)
    {
        ToIdle();
    }
}

void Elum::Motion_28_AbeUnmountingEnd_415D60()
{
    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_138_ElumUnmountEnd)
    {
        ToIdle();
    }
}

void Elum::Motion_29_BeesStruggling_412A90()
{
    EventBroadcast(kEventNoise, this);

    if (mNextMotion == eElumMotions::Motion_44_ScratchBegin_412730)
    {
        mCurrentMotion = eElumMotions::Motion_44_ScratchBegin_412730;
        mNextMotion = -1;
        return;
    }

    if (mNextMotion != eElumMotions::Motion_3_WalkLoop_412C90)
    {
        if (mNextMotion == eElumMotions::Motion_1_Idle_412990)
        {
            ToIdle();
            mNextMotion = -1;
        }
        return;
    }

    if (WallHit(
            mSpriteScale * FP_FromInteger(25),
            ScaleToGridSize(mSpriteScale) * FP_FromInteger(mAnim.mFlags.Get(AnimFlags::eFlipX) != 0 ? -1 : 1)))
    {
        ToIdle();
        mNextMotion = -1;
        return;
    }

    mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2_413270;
    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mNextMotion = -1;
        mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }
    else
    {
        mNextMotion = -1;
        mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }
}

void Elum::Motion_30_HopBegin_414E30()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckLiftPointGoneAndSetCamera();

    const FP xpos = mAnim.mFlags.Get(AnimFlags::eFlipX) ? mSpriteScale * FP_FromInteger(-56) : mSpriteScale * FP_FromInteger(56);
    const FP velX = mAnim.mFlags.Get(AnimFlags::eFlipX) ? mSpriteScale * FP_FromDouble(-9.85) : mSpriteScale * FP_FromDouble(9.85);
    const FP offX = mAnim.mFlags.Get(AnimFlags::eFlipX) ? FP_FromInteger(-56) : FP_FromInteger(56);

    if (WallHit(mSpriteScale * FP_FromInteger(40), offX))
    {
        ToKnockback();
        return;
    }

    if (!(mAnim.mFlags.Get(AnimFlags::eIsLastFrame)))
    {
        return;
    }

    mXPos += xpos;
    mVelX = velX;

    mVelY = mSpriteScale * FP_FromDouble(-2.7);
    BaseAliveGameObjectLastLineYPos = mYPos;

    if (WallHit(mSpriteScale * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
        return;
    }

    mXPos += mVelX;
    mYPos += mVelY;

    VOnTrapDoorOpen();
    mCurrentMotion = eElumMotions::Motion_31_HopMid_414C70;
    BaseAliveGameObjectCollisionLine = nullptr;
}

void Elum::RunJumpMidAndHopMid(MidType midType)
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(mSpriteScale * FP_FromInteger(40), mVelX))
    {
        RunSlideStopKnockback();
    }
    else
    {
        FP hitX = {};
        FP hitY = {};

        const FP velY = midType == MidType::eRunJumpMid ? FP_FromDouble(0.8) : FP_FromDouble(0.9);
        const FP jump_velx = midType == MidType::eRunJumpMid ? FP_FromDouble(1.1) : FP_FromDouble(2.15);

        const auto InAirCollision = InAirCollision_4019C0(&BaseAliveGameObjectCollisionLine, &hitX, &hitY, velY);

        if (sControlledCharacter == this)
        {
            SetActiveCameraDelayedFromDir_401C90();
        }

        if (InAirCollision)
        {
            switch (BaseAliveGameObjectCollisionLine->mLineType)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eDynamicCollision_32:
                case eLineTypes::eBackgroundDynamicCollision_36:
                    Elum_SFX_416E10(ElumSounds::eHitGroundSoft_4, 0);

                    switch (midType)
                    {
                        case MidType::eRunJumpMid:
                            mXPos = hitX;
                            mYPos = hitY;
                            mCurrentMotion = eElumMotions::Motion_35_RunJumpLand_415580;
                            MapFollowMe(TRUE);
                            break;

                        case MidType::eHopMid:
                            mVelY = FP_FromInteger(0);
                            mVelX = FP_FromInteger(0);
                            mCurrentMotion = eElumMotions::Motion_32_HopLand_415140;
                            mXPos = hitX;
                            mYPos = hitY;
                            break;

                        default:
                            break;
                    }

                    return;
                default:
                    break;
            }
        }

        if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(2))
        {
            field_118_jump_velx = jump_velx;
            mCurrentMotion = eElumMotions::Motion_24_JumpToFall_415ED0;
        }
    }
}

void Elum::Motion_31_HopMid_414C70()
{
    RunJumpMidAndHopMid(MidType::eHopMid);
}

void Elum::Motion_32_HopLand_415140()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        const PSX_RECT bRect = VGetBoundingRect();

        VOnCollisionWith(
            {bRect.x, static_cast<s16>(bRect.y + 5)},
            {bRect.w, static_cast<s16>(bRect.h + 5)},
            ObjListPlatforms_50766C,
            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

        MapFollowMe(TRUE);

        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_33_RunJumpBegin_415400()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (WallHit(mSpriteScale * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
    }
    else if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        FP velX = {};
        if (mAnim.mFlags.Get(AnimFlags::eFlipX))
        {
            velX = mSpriteScale * FP_FromDouble(-11.43);
        }
        else
        {
            velX = mSpriteScale * FP_FromDouble(11.43);
        }

        mVelX = velX;

        BaseAliveGameObjectLastLineYPos = mYPos;
        mVelY = mSpriteScale * FP_FromInteger(-4);

        mXPos += mVelX;
        mYPos += mSpriteScale * FP_FromInteger(-4);

        VOnTrapDoorOpen();
        mCurrentMotion = eElumMotions::Motion_34_RunJumpMid_415240;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Elum::Motion_34_RunJumpMid_415240()
{
    RunJumpMidAndHopMid(MidType::eRunJumpMid);
}

void Elum::Motion_35_RunJumpLand_415580()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        const PSX_RECT bRect = VGetBoundingRect();

        VOnCollisionWith(
            {bRect.x, static_cast<s16>(bRect.y + 5)},
            {bRect.w, static_cast<s16>(bRect.h + 5)},
            ObjListPlatforms_50766C,
            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

        if (sControlledCharacter == this)
        {
            FP offX = {};
            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                offX = -(mSpriteScale * FP_FromDouble(18.75));
            }
            else
            {
                offX = mSpriteScale * FP_FromDouble(18.75);
            }

            if (WallHit(mSpriteScale * FP_FromInteger(40), offX))
            {
                ToKnockback();
            }
            else
            {
                mPreviousMotion = eElumMotions::Motion_36_RunLoop_413720;
                mBaseAliveGameObjectLastAnimFrame = 4;
                field_120_bUnknown = 1;
                if (mAnim.mFlags.Get(AnimFlags::eFlipX))
                {
                    mXPos -= mSpriteScale * FP_FromDouble(18.75);
                    mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
                }
                else
                {
                    mXPos += mSpriteScale * FP_FromDouble(18.75);
                    mVelX = ScaleToGridSize(mSpriteScale) / FP_FromInteger(4);
                }
            }
        }
        else
        {
            mVelX = (mVelX >= FP_FromInteger(0)) ? FP_FromDouble(13.3) : FP_FromDouble(-13.3);
            mCurrentMotion = eElumMotions::Motion_12_RunTurn_414520;
        }
    }
}

void Elum::Motion_36_RunLoop_413720()
{
    field_10E_pressed |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(mSpriteScale * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
        return;
    }

    s16 gridSizeDirected = 0;
    if (mVelX <= FP_FromInteger(0))
    {
        gridSizeDirected = FP_GetExponent(ScaleToGridSize(mSpriteScale));
    }
    else
    {
        gridSizeDirected = -FP_GetExponent(ScaleToGridSize(mSpriteScale));
    }

    MoveOnLine(gridSizeDirected);

    if (mCurrentMotion == eElumMotions::Motion_36_RunLoop_413720)
    {
        if (mAnim.mCurrentFrame != 0 && mAnim.mCurrentFrame != 4)
        {
            field_10C_bFootStep2 = 0;
            return;
        }

        Elum_SFX_416E10(ElumSounds::eRunningFootstep_1, 0);

        if (!field_10C_bFootStep2)
        {
            field_10C_bFootStep2 = 1;
            MapFollowMe(TRUE);
        }

        if (sControlledCharacter == this)
        {
            if (sInputKey_Hop & field_10E_pressed)
            {
                mCurrentMotion = eElumMotions::Motion_33_RunJumpBegin_415400;
                field_10E_pressed = 0;
            }
            else if (Input().IsAnyPressed(sInputKey_Right) && mVelX < FP_FromInteger(0))
            {
                mCurrentMotion = eElumMotions::Motion_12_RunTurn_414520;
                mVelX = mSpriteScale * FP_FromDouble(-13.3);
            }
            else if (Input().IsAnyPressed(sInputKey_Left) && mVelX > FP_FromInteger(0))
            {
                mCurrentMotion = eElumMotions::Motion_12_RunTurn_414520;
                mVelX = mSpriteScale * FP_FromDouble(13.3);
            }
            else if (!Input().IsAnyPressed(sInputKey_Right | sInputKey_Left))
            {
                mCurrentMotion = eElumMotions::Motion_37_RunSlideStop_4142E0;

                if (mVelX >= FP_FromInteger(0))
                {
                    mVelX = (mSpriteScale * FP_FromDouble(13.3));
                }
                else
                {
                    mVelX = (mSpriteScale * FP_FromDouble(-13.3));
                }
            }
            else if (!Input().IsAnyPressed(sInputKey_Run))
            {
                if (mAnim.mCurrentFrame == 0)
                {
                    mCurrentMotion = eElumMotions::Motion_42_RunToWalk_413B60;
                }
                else
                {
                    mCurrentMotion = eElumMotions::Motion_43_MidRunToWalk_413E20;
                }
            }
            return;
        }
        else
        {
            mCurrentMotion = eElumMotions::Motion_12_RunTurn_414520;

            if (mVelX >= FP_FromInteger(0))
            {
                mVelX = (mSpriteScale * FP_FromDouble(13.3));
            }
            else
            {
                mVelX = (mSpriteScale * FP_FromDouble(-13.3));
            }
        }

        field_10E_pressed = 0;
    }
}

void Elum::RunSlideStopKnockback()
{
    mCurrentMotion = eElumMotions::Motion_50_Knockback_415DC0;
    if (BaseAliveGameObjectCollisionLine)
    {
        mVelX = -mVelX;
        MoveOnLine(FALSE);
    }
    else
    {
        mXPos -= mVelX;
    }
    mVelX = FP_FromInteger(0);
    MapFollowMe(TRUE);
    Environment_SFX_42A220(EnvironmentSfx::eKnockback_13, 95, -200, this);
}

void Elum::Motion_37_RunSlideStop_4142E0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mAnim.mCurrentFrame == 0)
    {
        Elum_SFX_416E10(ElumSounds::eRunSlide_5, 0);
    }

    const FP offY = (sControlledCharacter == this) ? mSpriteScale * FP_FromInteger(40) : mSpriteScale * FP_FromInteger(25);

    if (WallHit(offY, mVelX))
    {
        RunSlideStopKnockback();
        return;
    }

    SlowOnX_414210(FP_FromDouble(2.125));

    if (sControlledCharacter == this && mAnim.mCurrentFrame < 7)
    {
        if ((mAnim.mFlags.Get(AnimFlags::eFlipX) && Input().IsAnyPressed(sInputKey_Right)) || (!mAnim.mFlags.Get(AnimFlags::eFlipX) && Input().IsAnyPressed(sInputKey_Left)))
        {
            mPreviousMotion = eElumMotions::Motion_12_RunTurn_414520;
            mBaseAliveGameObjectLastAnimFrame = mAnim.mCurrentFrame;
            field_120_bUnknown = 1;
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        MapFollowMe(TRUE);
        if (!ToNextMotion_4120F0())
        {
            field_118_jump_velx = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            mCurrentMotion = eAbeMotions::Motion_1_WalkLoop;
            field_110_timer = sGnFrame;
            field_10E_pressed = 0;
            MapFollowMe(TRUE);
        }
    }
}

void Elum::Motion_38_RunTurnToRun_414810()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_36_RunLoop_413720;
        mAnim.mFlags.Toggle(AnimFlags::eFlipX);
    }
}

void Elum::Motion_39_IdleToRun_413B00()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10E_pressed |= Input().Pressed();

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_36_RunLoop_413720;
    }

    MoveOnLine(0);
}

void Elum::Motion_40_WalkToRun_4134B0()
{
    field_10E_pressed |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_36_RunLoop_413720;
    }

    MoveOnLine(0);
}

void Elum::Motion_41_MidWalkToRun_413560()
{
    field_10E_pressed |= Input().Pressed();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mPreviousMotion = eElumMotions::Motion_36_RunLoop_413720;
        mBaseAliveGameObjectLastAnimFrame = eElumMotions::Motion_4_Turn_4140F0;
        field_120_bUnknown = 1;
    }

    MoveOnLine(0);
}

void Elum::Motion_42_RunToWalk_413B60()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10E_pressed |= Input().Pressed();

    FP velX{};
    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        velX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }
    else
    {
        velX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }

    mVelX = velX;

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_3_WalkLoop_412C90;
        VCheckCollisionLineStillValid(10);

        PSX_Point xy{FP_GetExponent(mXPos - FP_FromInteger(10)), FP_GetExponent(mYPos - FP_FromInteger(10))};
        PSX_Point wh{FP_GetExponent(mXPos + FP_FromInteger(10)), FP_GetExponent(mYPos + FP_FromInteger(10))};
        VOnCollisionWith(
            xy,
            wh,
            ObjListPlatforms_50766C,
            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

        FP offX = {};
        if (mAnim.mFlags.Get(AnimFlags::eFlipX))
        {
            offX = FP_FromInteger(-2) * ScaleToGridSize(mSpriteScale);
        }
        else
        {
            offX = FP_FromInteger(2) * ScaleToGridSize(mSpriteScale);
        }

        if (WallHit(mSpriteScale * FP_FromInteger(40), offX))
        {
            ToKnockback();
            return;
        }
    }

    if (WallHit(mSpriteScale * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
        return;
    }

    MoveOnLine(0);
}

void Elum::Motion_43_MidRunToWalk_413E20()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10E_pressed |= Input().Pressed();

    FP velX = {};
    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        velX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }
    else
    {
        velX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
    }

    mVelX = velX;

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mPreviousMotion = 3;
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_120_bUnknown = 1;
        VCheckCollisionLineStillValid(10);

        PSX_Point xy{FP_GetExponent(mXPos - FP_FromInteger(10)), FP_GetExponent(mYPos - FP_FromInteger(10))};
        PSX_Point wh{FP_GetExponent(mXPos + FP_FromInteger(10)), FP_GetExponent(mYPos + FP_FromInteger(10))};

        VOnCollisionWith(
            xy,
            wh,
            ObjListPlatforms_50766C,
            (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

        FP offX = {};
        if (mAnim.mFlags.Get(AnimFlags::eFlipX))
        {
            offX = FP_FromInteger(-2) * ScaleToGridSize(mSpriteScale);
        }
        else
        {
            offX = FP_FromInteger(2) * ScaleToGridSize(mSpriteScale);
        }

        if (WallHit(mSpriteScale * FP_FromInteger(40), offX))
        {
            ToKnockback();
            return;
        }
    }

    if (WallHit(mSpriteScale * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
        return;
    }
    MoveOnLine(0);
}

void Elum::Motion_44_ScratchBegin_412730()
{
    CheckLiftPointGoneAndSetCamera();

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_45_ScratchLoop_4127B0;
        if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) == CameraPos::eCamCurrent_0)
        {
            SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_15, 1, 1);
        }
    }
}

void Elum::Motion_45_ScratchLoop_4127B0()
{
    CheckLiftPointGoneAndSetCamera();

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mCurrentMotion = eElumMotions::Motion_46_ScratchEnd_412800;
    }
}

void Elum::Motion_46_ScratchEnd_412800()
{
    CheckLiftPointGoneAndSetCamera();

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (mNextMotion == eElumMotions::Motion_3_WalkLoop_412C90)
        {
            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (!WallHit(mSpriteScale * FP_FromInteger(40), -ScaleToGridSize(mSpriteScale)))
                {
                    mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
                    mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2_413270;
                    mNextMotion = -1;
                    return;
                }
            }
            else
            {
                if (!WallHit(mSpriteScale * FP_FromInteger(40), ScaleToGridSize(mSpriteScale)))
                {
                    mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(9));
                    mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2_413270;
                    mNextMotion = -1;
                    return;
                }
            }
            mNextMotion = -1;
            return;
        }

        if (mNextMotion == eElumMotions::Motion_29_BeesStruggling_412A90)
        {
            Elum_SFX_416E10(ElumSounds::eBeesStruggle_3, 0);
            mCurrentMotion = eElumMotions::Motion_29_BeesStruggling_412A90;
            field_110_timer = sGnFrame + 25;
            mNextMotion = -1;
            return;
        }

        ToIdle();
    }
}

void Elum::ToIdle()
{
    field_118_jump_velx = FP_FromInteger(0);
    field_110_timer = sGnFrame;
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mCurrentMotion = eElumMotions::Motion_1_Idle_412990;
    field_10E_pressed = 0;
    MapFollowMe(TRUE);
}

void Elum::Motion_47_Unknown_415A30()
{
    EventBroadcast(kEventNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (!ToNextMotion_4120F0())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_48_AbeMoutingBegin_415C40()
{
    if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_136_ElumMountEnd)
    {
        mCurrentMotion = eElumMotions::Motion_27_AbeMountingEnd_415CA0;
    }
}

void Elum::Motion_49_AbeUnmountingBegin_415D00()
{
    if (sActiveHero->mCurrentMotion != eAbeMotions::Motion_137_ElumUnmountBegin)
    {
        mCurrentMotion = eElumMotions::Motion_28_AbeUnmountingEnd_415D60;
    }
}

void Elum::Motion_50_Knockback_415DC0()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_21_Land_414A20();
        if (mCurrentMotion == eElumMotions::Motion_1_Idle_412990)
        {
            mCurrentMotion = eElumMotions::Motion_50_Knockback_415DC0;
            field_110_timer = sGnFrame + 10;
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (field_110_timer <= static_cast<s32>(sGnFrame))
        {
            if (BaseAliveGameObjectCollisionLine)
            {
                ToIdle();
            }
        }
    }
}

void Elum::VUpdate()
{
    if (sDDCheat_FlyingEnabled_50771C && sControlledCharacter == this)
    {
        VOnTrapDoorOpen();

        mCurrentMotion = eElumMotions::Motion_21_Land_414A20;
        BaseAliveGameObjectCollisionLine = nullptr;

        static s32 sElum_velx_table_4FF968[] = {
            4,
            4,
            0,
            -4,
            -4,
            -4,
            0,
            4};

        static s32 sElum_vely_table_4FF988[] = {
            0,
            -4,
            -4,
            -4,
            0,
            4,
            4,
            4};

        if (Input().IsAnyPressed(0xF000u))
        {
            const s32 tableIdx = Input().Dir();
            mVelX = FP_FromRaw(sElum_velx_table_4FF968[tableIdx]);
            mVelY = FP_FromRaw(sElum_vely_table_4FF988[tableIdx]);

            if (Input().IsAnyPressed(sInputKey_Run))
            {
                mVelX += FP_FromRaw(sElum_velx_table_4FF968[tableIdx] * 2);
                mVelY += FP_FromRaw(sElum_vely_table_4FF988[tableIdx]);
            }

            mXPos += mVelX;
            mYPos += mVelY;

            PSX_Point map_size = {};
            gMap.Get_map_size(&map_size);

            if (mXPos < FP_FromInteger(0))
            {
                mXPos = FP_FromInteger(0);
            }

            if (mXPos >= FP_FromInteger(map_size.x))
            {
                mXPos = FP_FromInteger(map_size.x) - FP_FromInteger(1);
            }

            if (mYPos < FP_FromInteger(0))
            {
                mYPos = FP_FromInteger(0);
            }

            if (mYPos >= FP_FromInteger(map_size.y))
            {
                mYPos = FP_FromInteger(map_size.y) - FP_FromInteger(1);
            }
        }
        else
        {
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
        }

        SetActiveCameraDelayedFromDir_401C90();

        sActiveHero->mXPos = mXPos;
        sActiveHero->mYPos = mYPos;
        return;
    }

    if (!(field_170_flags.Get(Elum::Flags_170::eFoundHoney_Bit4) || field_170_flags.Get(Elum::Flags_170::eStungByBees_Bit2)) && field_128_brain_idx != 1)
    {
        FindHoney_411600();
    }

    if (field_170_flags.Get(Elum::Flags_170::eChangedPathNotMounted_Bit5))
    {
        HandleElumPathTrans_411460();
        field_170_flags.Clear(Elum::Flags_170::eChangedPathMounted_Bit7);
        field_170_flags.Clear(Elum::Flags_170::eChangedPathNotMounted_Bit5);
    }

    if (mCurrentMotion == eElumMotions::Motion_19_Dead_415F90 || mCurrentPath == gMap.mCurrentPath)
    {
        PathLine* pLine = nullptr;
        if (field_170_flags.Get(Elum::Flags_170::eChangedPathMounted_Bit7))
        {
            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mXPos,
                    mYPos - FP_FromInteger(40),
                    mXPos,
                    mYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
            }
            else
            {
                BaseAliveGameObjectCollisionLine = nullptr;
                mCurrentMotion = eElumMotions::Motion_21_Land_414A20;
            }
            field_170_flags.Clear(Elum::Flags_170::eChangedPathMounted_Bit7);
        }

        const auto oldMotion = mCurrentMotion;
        const FP old_x = mXPos;
        const FP old_y = mYPos;

        if (oldMotion != eElumMotions::Motion_19_Dead_415F90)
        {
            field_12A_brain_sub_state = (this->*sElum_brain_table_4C52E8[field_128_brain_idx])();
        }

        if (field_170_flags.Get(Elum::Flags_170::eFalling_Bit3))
        {
            VCheckCollisionLineStillValid(10);

            const PSX_RECT bRect = VGetBoundingRect();

            VOnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                ObjListPlatforms_50766C,
                (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

            if (mLiftPoint)
            {
                field_170_flags.Clear(Elum::Flags_170::eFalling_Bit3);
            }
        }

        (this->*sElum_motion_table_4C5148[mCurrentMotion])();

        if ((oldMotion != mCurrentMotion && oldMotion == 2) || oldMotion == 11 || oldMotion == 47)
        {
            LOG_INFO("old motion: " << oldMotion << " | new motion: " << mCurrentMotion);
        }

        if (old_x != mXPos || old_y != mYPos)
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);
            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (oldMotion == mCurrentMotion)
        {
            if (field_120_bUnknown)
            {
                mCurrentMotion = mPreviousMotion;

                mAnim.Set_Animation_Data(GetAnimRes(gElumMotionAnimIds[mCurrentMotion]));
                mAnim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
                field_120_bUnknown = 0;
                if (sControlledCharacter == this)
                {
                    sActiveHero->SyncToElum(mCurrentMotion);
                }
            }
        }
        else
        {
            mAnim.Set_Animation_Data(GetAnimRes(gElumMotionAnimIds[mCurrentMotion]));
            if (sControlledCharacter == this)
            {
                sActiveHero->SyncToElum(mCurrentMotion);
            }
        }

        if (EventGet(kEventDeathReset))
        {
            if (!field_154_bAbeForcedDownFromElum)
            {
                if (sActiveHero->mContinueZoneNumber != mContinueZoneNumber)
                {
                    field_128_brain_idx = 0;
                    field_12A_brain_sub_state = 6;
                    mDontFollowAbe = 1;

                    if (sControlledCharacter == this)
                    {
                        sActiveHero->mXPos = mXPos;
                        sActiveHero->mYPos = mYPos;
                        sActiveHero->mAnim.mFlags.Set(AnimFlags::eFlipX, mAnim.mFlags.Get(AnimFlags::eFlipX));
                    }
                    return;
                }
            }

            mAnim.mFlags.Clear(AnimFlags::eRender);
            mCurrentMotion = eElumMotions::Motion_19_Dead_415F90;
            VOnTrapDoorOpen();
        }

        if (sControlledCharacter == this)
        {
            sActiveHero->mXPos = mXPos;
            sActiveHero->mYPos = mYPos;
            sActiveHero->mAnim.mFlags.Set(AnimFlags::eFlipX, mAnim.mFlags.Get(AnimFlags::eFlipX));
        }
        return;
    }
}

void Elum::VRender(PrimHeader** ppOt)
{
    if (mCurrentLevel == gMap.mCurrentLevel
        && mCurrentPath == gMap.mCurrentPath
        && !UpdateDelay())
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Elum::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (gMap.mCurrentPath != gMap.mNextPath)
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eLines)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else if (gMap.mCurrentLevel == EReliveLevelIds::eDesert && gMap.mNextPath == 9)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            else if (mCurrentPath == gMap.mCurrentPath)
            {
                auto pElumPathTrans = static_cast<relive::Path_ElumPathTrans*>(gMap.TLV_Get_At(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eElumPathTrans));

                if (pElumPathTrans)
                {
                    if (mDontFollowAbe != 1 && sControlledCharacter != this && pElumPathTrans->mNextLevel == gMap.mNextLevel && pElumPathTrans->mNextPath == gMap.mNextPath)
                    {
                        field_170_flags.Set(Elum::Flags_170::eChangedPathNotMounted_Bit5);
                    }
                }
            }

            if (mLiftPoint)
            {
                VOnTrapDoorOpen();
                field_170_flags.Set(Elum::Flags_170::eFalling_Bit3);
            }

            if (!field_170_flags.Get(Elum::Flags_170::eChangedPathNotMounted_Bit5))
            {
                BaseAliveGameObjectCollisionLine = nullptr;
                field_170_flags.Set(Elum::Flags_170::eChangedPathMounted_Bit7);
            }
        }
    }
}

s16 word_4C52F0 = 0x88;
s16 word_4C52F2 = 0x38;
u32 dword_4C52F4 = 0x1C92C;
u32 dword_4C52F8 = 0xC8;
s16 word_4C52FA = 0;

void Elum::Spawn(const Guid& tlvInfo)
{
    relive_new Elum(tlvInfo);
}

void Elum::LoadAnimations()
{
    for (auto& animId : gElumMotionAnimIds)
    {
        if (animId != AnimId::None)
        {
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
        }
    }
}

Elum::Elum(const Guid& tlvInfo)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eElum);

    field_158_last_event_idx = -1;
    field_16C_never_read = 0;
    field_16E_never_read = -1;
    field_1F0_tlvInfo = tlvInfo;

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Elum_Land));

    mSpriteScale = sActiveHero->mSpriteScale;

    if (mSpriteScale == FP_FromInteger(1))
    {
        mScale = Scale::Fg;
        mAnim.mRenderLayer = Layer::eLayer_ZapLinesElumMuds_28;
    }
    else
    {
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_ZapLinesMudsElum_Half_9;
    }

    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);

    field_170_flags.Clear(Elum::Flags_170::eStungByBees_Bit2);
    field_170_flags.Clear(Elum::Flags_170::eFoundHoney_Bit4);
    field_170_flags.Clear(Elum::Flags_170::eChangedPathNotMounted_Bit5);
    field_170_flags.Clear(Elum::Flags_170::eChangedPathMounted_Bit7);

    mLiftPoint = nullptr;
    gElum = this;

    mCurrentMotion = eElumMotions::Motion_21_Land_414A20;
    field_120_bUnknown = 0;

    field_12E_honey_ypos = 0;
    field_12C_honey_xpos = 0;

    mRespawnOnDead = 0;
    field_110_timer = sGnFrame;

    mXPos = sActiveHero->mXPos - (ScaleToGridSize(mSpriteScale) * FP_FromInteger(2));
    mYPos = sActiveHero->mYPos - FP_FromInteger(5);

    mDontFollowAbe = 0;
    field_124_bShould_IdleToWalk1 = 1;
    field_10E_pressed = 0;
    field_EC = 2;
    mBaseAliveGameObjectLastAnimFrame = 0;
    field_130_unused = 0;

    mContinueZoneNumber = 0;
    field_142_zone_number = sActiveHero->mContinueZoneNumber;

    field_154_bAbeForcedDownFromElum = 0;
    field_128_brain_idx = 0;
    field_12A_brain_sub_state = 0;

    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit6);


    field_170_flags.Clear(Elum::Flags_170::eStrugglingWithBees_Bit1);
    field_170_flags.Clear(Elum::Flags_170::eFalling_Bit3);

    field_170_flags.Clear(Elum::Flags_170::eCanSpeak_Bit6);

    relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 30);

    mShadow = relive_new Shadow();
}

} // namespace AO
