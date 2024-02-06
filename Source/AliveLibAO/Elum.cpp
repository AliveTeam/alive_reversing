#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Elum.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "LiftPoint.hpp"
#include "Game.hpp"
#include "Input.hpp"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "MusicTrigger.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Midi.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "Honey.hpp"
#include "GameSpeak.hpp"
#include "Math.hpp"
#include "../relive_lib/Grid.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

Elum* gElum = nullptr;

using TElumStateFunction = decltype(&Elum::Motion_0_Respawn);

const TElumStateFunction sElumMotionTable[] = {
    &Elum::Motion_0_Respawn,
    &Elum::Motion_1_Idle,
    &Elum::Motion_2_Unknown,
    &Elum::Motion_3_WalkLoop,
    &Elum::Motion_4_Turn,
    &Elum::Motion_5_WalkToIdle,
    &Elum::Motion_6_MidWalkToIdle,
    &Elum::Motion_7_IdleToWalk2,
    &Elum::Motion_8_IdleToWalk1,
    &Elum::Motion_9_ToYell,
    &Elum::Motion_10_Yell,
    &Elum::Motion_11_Unknown,
    &Elum::Motion_12_RunTurn,
    &Elum::Motion_13_RunTurnToWalk,
    &Elum::Motion_14_Speak,
    &Elum::Motion_15_Speak,
    &Elum::Motion_16_Speak,
    &Elum::Motion_17_Unknown,
    &Elum::Motion_18_Unknown,
    &Elum::Motion_19_Dead,
    &Elum::Motion_20_Fall,
    &Elum::Motion_21_Land,
    &Elum::Motion_22_RunOffEdge,
    &Elum::Motion_23_WalkOffEdge,
    &Elum::Motion_24_JumpToFall,
    &Elum::Motion_25_LickingHoney,
    &Elum::Motion_26_LickingToStruggling,
    &Elum::Motion_27_AbeMountingEnd,
    &Elum::Motion_28_AbeUnmountingEnd,
    &Elum::Motion_29_BeesStruggling,
    &Elum::Motion_30_HopBegin,
    &Elum::Motion_31_HopMid,
    &Elum::Motion_32_HopLand,
    &Elum::Motion_33_RunJumpBegin,
    &Elum::Motion_34_RunJumpMid,
    &Elum::Motion_35_RunJumpLand,
    &Elum::Motion_36_RunLoop,
    &Elum::Motion_37_RunSlideStop,
    &Elum::Motion_38_RunTurnToRun,
    &Elum::Motion_39_IdleToRun,
    &Elum::Motion_40_WalkToRun,
    &Elum::Motion_41_MidWalkToRun,
    &Elum::Motion_42_RunToWalk,
    &Elum::Motion_43_MidRunToWalk,
    &Elum::Motion_44_ScratchBegin,
    &Elum::Motion_45_ScratchLoop,
    &Elum::Motion_46_ScratchEnd,
    &Elum::Motion_47_Unknown,
    &Elum::Motion_48_AbeMoutingBegin,
    &Elum::Motion_49_AbeUnmountingBegin,
    &Elum::Motion_50_Knockback};

using TBrainStateFunc = decltype(&Elum::Brain_0_WithoutAbe);

const TBrainStateFunc sElum_brain_table_4C52E8[] = {
    &Elum::Brain_0_WithoutAbe,
    &Elum::Brain_1_HoneyAddiction,
};

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
            if (mPreviousContinuePointZoneNumber != pContinueTlv->mZoneNumber &&
                pContinueTlv->mZoneNumber > mAbeZoneNumber &&
                pContinueTlv->mElumRestarts == relive::reliveChoice::eYes)
            {
                mPreviousContinuePointZoneNumber = pContinueTlv->mZoneNumber;
                mContinuePointRect.x = pContinueTlv->mTopLeftX;
                mContinuePointRect.y = pContinueTlv->mTopLeftY;
                mContinuePointRect.w = pContinueTlv->mBottomRightX;
                mContinuePointRect.h = pContinueTlv->mBottomRightY;

                mContinuePointPath = gMap.mCurrentPath;
                mContinueCamera = gMap.mCurrentCamera;
                mContinuePointLevel = gMap.mCurrentLevel;
                mContinuePointSpriteScale = GetSpriteScale();

                mRespawnOnDead = 1;
            }
        }
        else if (pTlv->mTlvType == ReliveTypes::eDeathDrop && mHealth > FP_FromInteger(0))
        {
            if (sControlledCharacter != this)
            {
                mDontFollowAbe = 0;
            }
            Elum_SFX(ElumSounds::eHowl_2, this);
            GetAnimation().SetRender(false);
            mCurrentMotion = eElumMotions::Motion_20_Fall;
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

bool Elum::VTakeDamage(BaseGameObject* pFrom)
{
    switch (pFrom->Type())
    {
        case ReliveTypes::eBullet:
        case ReliveTypes::eGroundExplosion:
        case ReliveTypes::eAirExplosion:
            if (mHealth > FP_FromInteger(0))
            {
                Elum_SFX(ElumSounds::eExploding_7, 0);
                SfxPlayMono(relive::SoundEffects::KillEffect, 75);

                if (gAbe->mHealth > FP_FromInteger(0))
                {
                    mCurrentMotion = eElumMotions::Motion_20_Fall;
                }
                else
                {
                    mCurrentMotion = eElumMotions::Motion_19_Dead;
                }

                relive_new Gibs(
                    GibType::eElum,
                    mXPos,
                    mYPos,
                    mVelX,
                    mVelY,
                    GetSpriteScale(),
                    false);

                mHealth = FP_FromInteger(0);

                if (sControlledCharacter != this)
                {
                    mDontFollowAbe = 0;
                }

                GetAnimation().SetRender(false);
                GetAnimation().Set_Animation_Data(GetAnimRes(gElumMotionAnimIds[static_cast<u32>(mCurrentMotion)]));
            }
            return true;

        case ReliveTypes::eAbilityRing:
            return false;

        case ReliveTypes::eBeeSwarm:
            if (sControlledCharacter != this)
            {
                mStrugglingWithBees = true;
            }
            break;

        default:
            return true;
    }
    return true;
}

void Elum::ToKnockback()
{
    mVelX = FP_FromInteger(0);
    mCurrentMotion = eElumMotions::Motion_50_Knockback;

    if (BaseAliveGameObjectCollisionLine)
    {
        MoveOnLine(0);
    }
    MapFollowMe(true);
    Environment_SFX(EnvironmentSfx::eKnockback_13, 95, -200, this);
}

void Elum::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
    }
}

void Elum::Vsub_416120()
{
    ToIdle();

    GetAnimation().Set_Animation_Data(GetAnimRes(gElumMotionAnimIds[static_cast<u32>(mCurrentMotion)]));
}

void Elum::MidWalkToNextMotion()
{
    if (mVelX <= FP_FromInteger(0))
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(40), -ScaleToGridSize(GetSpriteScale())))
        {
            mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle;
            return;
        }
    }
    else
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(40), ScaleToGridSize(GetSpriteScale())))
        {
            mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle;
            return;
        }
    }

    if ((mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)) || !Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
    {
        mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle;
    }
    else
    {
        if (field_10E_pressed & InputCommands::eHop)
        {
            mCurrentMotion = eElumMotions::Motion_18_MidWalkToHop;
        }
    }
    field_10E_pressed = 0;
}

void Elum::WalkToNextMotion()
{
    if (mVelX <= FP_FromInteger(0))
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(40), -ScaleToGridSize(GetSpriteScale())))
        {
            mCurrentMotion = eElumMotions::Motion_5_WalkToIdle;
            return;
        }
    }
    else
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(40), ScaleToGridSize(GetSpriteScale())))
        {
            mCurrentMotion = eElumMotions::Motion_5_WalkToIdle;
            return;
        }
    }

    if ((mVelX > FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eLeft)) || (mVelX < FP_FromInteger(0) && Input().IsAnyHeld(InputCommands::eRight)) || !Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
    {
        mCurrentMotion = eElumMotions::Motion_5_WalkToIdle;
        field_10E_pressed = 0;
    }
    else
    {
        if (field_10E_pressed & InputCommands::eHop)
        {
            mCurrentMotion = eElumMotions::Motion_17_WalkToHop;
        }
        field_10E_pressed = 0;
    }
}

void Elum::SlowOnX(FP amount)
{
    if (FP_GetExponent(mVelX))
    {
        MoveOnLine(0);

        if (mVelX <= FP_FromInteger(0))
        {
            if (mVelX < FP_FromInteger(0))
            {
                mVelX += (GetSpriteScale() * amount);
                if (mVelX > FP_FromInteger(0))
                {
                    mVelX = FP_FromInteger(0);
                }
            }
        }
        else
        {
            mVelX -= (GetSpriteScale() * amount);
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
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        if (pPlatform->GetDead())
        {
            VOnTrapDoorOpen();
            mFalling = true;
        }

        if (sControlledCharacter == this)
        {
            SetActiveCameraDelayedFromDir();
        }
    }
}

void Elum::MoveOnLine(s16 xLookAhead)
{
    CheckLiftPointGoneAndSetCamera();

    BaseGameObject* pPlatform = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    const FP oldX = mXPos;
    FP xpos_off_fp = mXPos + FP_FromInteger(xLookAhead);
    BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
        &xpos_off_fp,
        &mYPos,
        mVelX);

    if (BaseAliveGameObjectCollisionLine)
    {
        mXPos += mVelX;
        if (pPlatform)
        {
            if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32)
            {
                VOnTrapDoorOpen();
            }
        }
        else if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32)
        {
            const PSX_RECT bRect = VGetBoundingRect();
            OnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                gPlatformsArray);
        }
    }
    else
    {
        VOnTrapDoorOpen();
        mCurrentMotion = eElumMotions::Motion_23_WalkOffEdge;
        BaseAliveGameObjectLastLineYPos = mYPos;
        mXPos = mVelX + oldX;
    }
}

void Elum::KnockAbeOffElum()
{
    // Back to Abe
    if (sControlledCharacter == this)
    {
        sControlledCharacter = gAbe;
        gAbe->mNextMotion = eAbeMotions::Motion_128_KnockForward;
    }

    /*
    // re-load some elum resources
    field_104_pending_resource_count += 2;

    ResourceManager::LoadResourceFile(
        "ELMALONE.BAN",
        BaseAliveGameObject::OnResourceLoaded,
        this);

    ResourceManager::LoadResourceFile(
        "ELMPRMNT.BAN",
        BaseAliveGameObject::OnResourceLoaded,
        this);
    */
}

bool Elum::ToNextMotion()
{
    if (sControlledCharacter == this)
    {
        return ToNextMotionAbeControlled();
    }

    switch (mNextMotion)
    {
        case eElumMotions::Motion_3_WalkLoop:
            if (GetAnimation().GetFlipX())
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(25), ScaleToGridSize(GetSpriteScale())))
                {
                    return false;
                }
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }
            else
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(25), ScaleToGridSize(GetSpriteScale())))
                {
                    return false;
                }
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
            }

            if (field_124_bShould_IdleToWalk1)
            {
                mCurrentMotion = eElumMotions::Motion_7_IdleToWalk1;
                field_124_bShould_IdleToWalk1 = field_124_bShould_IdleToWalk1 == 0;
            }
            else
            {
                mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2;
                field_124_bShould_IdleToWalk1 = 1;
            }

            mNextMotion = eElumMotions::None_m1;
            return true;

        case eElumMotions::Motion_29_BeesStruggling:
            Elum_SFX(ElumSounds::eBeesStruggle_3, 0);
            mCurrentMotion = eElumMotions::Motion_29_BeesStruggling;
            field_110_timer = MakeTimer(25);
            mNextMotion = eElumMotions::None_m1;
            return true;

        case eElumMotions::Motion_4_Turn:
        case eElumMotions::Motion_14_Speak:
        case eElumMotions::Motion_15_Speak:
        case eElumMotions::Motion_16_Speak:
            mCurrentMotion = mNextMotion;
            mNextMotion = eElumMotions::None_m1;
            return true;

        case eElumMotions::Motion_10_Yell:
            mCurrentMotion = eElumMotions::Motion_9_ToYell;
            mNextMotion = eElumMotions::None_m1;
            return true;

        case eElumMotions::Motion_45_ScratchLoop:
            mCurrentMotion = eElumMotions::Motion_44_ScratchBegin;
            mNextMotion = eElumMotions::None_m1;
            return true;

        default:
            return false;
    }
}

bool Elum::ToNextMotionAbeControlled()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform && pPlatform->Type() == ReliveTypes::eLiftPoint)
    {
        if (!static_cast<LiftPoint*>(pPlatform)->OnAnyFloor())
        {
            return false;
        }
    }

    if (Input().IsAnyHeld(InputCommands::eLeft | InputCommands::eRight))
    {
        FP gridSize = {};
        if (Input().IsAnyHeld(InputCommands::eRight))
        {
            gridSize = ScaleToGridSize(GetSpriteScale());

            if (GetAnimation().GetFlipX())
            {
                mCurrentMotion = eElumMotions::Motion_4_Turn;
                return true;
            }
        }
        else if (Input().IsAnyHeld(InputCommands::eLeft))
        {
            gridSize = -ScaleToGridSize(GetSpriteScale());

            if (!GetAnimation().GetFlipX())
            {
                mCurrentMotion = eElumMotions::Motion_4_Turn;
                return true;
            }
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(40), gridSize))
        {
            return false;
        }

        if (Input().IsAnyHeld(InputCommands::eRun))
        {
            mVelX = gridSize / FP_FromInteger(4);
            mCurrentMotion = eElumMotions::Motion_39_IdleToRun;
        }
        else
        {
            mVelX = gridSize / FP_FromInteger(9);
            mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2;
        }
        return true;
    }
    else
    {
        if (Input().IsAnyHeld(InputCommands::eLeftGameSpeak | InputCommands::eRightGameSpeak)
            || !(Input().IsAnyHeld(InputCommands::eHop)))
        {
            return false;
        }

        if (GetAnimation().GetFlipX())
        {
            mVelX = GetSpriteScale() * FP_FromInteger(-15);
        }
        else
        {
            mVelX = GetSpriteScale() * FP_FromInteger(15);
        }

        mCurrentMotion = eElumMotions::Motion_30_HopBegin;
        return true;
    }
}

void Elum::HandleElumPathTrans()
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);

    if (gAbe->GetAnimation().GetFlipX())
    {
        mXPos = ScaleToGridSize(GetSpriteScale()) + FP_FromInteger(camCoords.x + XGrid_Index_To_XPos_AO(GetSpriteScale(), MaxGridBlocks(GetSpriteScale())));
    }
    else
    {
        mXPos = FP_FromInteger(camCoords.x + XGrid_Index_To_XPos_AO(GetSpriteScale(), 0)) - ScaleToGridSize(GetSpriteScale());
    }

    if (gAbe->BaseAliveGameObjectCollisionLine)
    {
        mYPos = gAbe->mYPos;
    }
    else
    {
        mYPos = FP_FromInteger(camCoords.y + FP_GetExponent(mYPos) % 480);
    }

    if (BaseAliveGameObjectCollisionLine && BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32)
    {
        BaseAliveGameObject_PlatformId = Guid{};
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos - FP_FromInteger(40),
            mXPos,
            mYPos + FP_FromInteger(40),
            &pLine,
            &hitX,
            &hitY,
            GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
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

void Elum::Elum_SFX(ElumSounds soundId, BaseAliveGameObject* pObj)
{
    switch (soundId)
    {
        case ElumSounds::eWalkingFootstep_0:
        {
            const s16 sfxId = Math_RandomRange(0, 2);
            const s16 pitch = Math_RandomRange(54, 58);
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[sfxId], pitch, -700, -573);
            break;
        }

        case ElumSounds::eRunningFootstep_1:
        {
            const s16 sfxId = Math_RandomRange(0, 2);
            const s16 pitch = Math_RandomRange(66, 70);
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[sfxId], pitch, -573, -446);
            break;
        }

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
            SFX_SfxDefinition_Play(sElumSfx_4C5398[3], (s16) volLeft, (s16) volRight, 0, 0);
            break;
        }

        case ElumSounds::eBeesStruggle_3:
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[4], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eHitGroundSoft_4:
            SFX_SfxDefinition_Play_Mono(sElumSfx_4C5398[7], 0, 0x7FFF, 0x7FFF);
            break;

        case ElumSounds::eRunSlide_5:
            Environment_SFX(EnvironmentSfx::eRunSlide_4, 0, 0x7FFF, 0);
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

void Elum::FindHoney()
{
    if (mCurrentMotion != eElumMotions::Motion_25_LickingHoney)
    {
        mFoundHoney = false;

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
                    mHoneyXPos = FP_GetExponent(pHoney->mXPos);
                    mHoneyYPos = FP_GetExponent(pHoney->mYPos);
                    mFoundHoney = true;
                    break;
                }
            }
        }
    }
}

bool Elum::NearHoney()
{
    if (mFoundHoney)
    {
        auto pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
        if (pLiftPoint && pLiftPoint->Type() == ReliveTypes::eLiftPoint)
        {
            if (!pLiftPoint->OnAnyFloor())
            {
                // We're on a lift that isn't on a floor
                return false;
            }
        }

        return abs(FP_GetExponent(mYPos) - mHoneyYPos) <= 24 ? true : false;
    }
    return false;
}

s16 Elum::Brain_0_WithoutAbe()
{
    const s32 last_event_index = gEventSystem->mLastEventIndex;

    GameSpeakEvents last_event_v1;
    GameSpeakEvents last_event_v2;

    if (field_158_last_event_idx == last_event_index)
    {
        const bool bLastEventMinus1 = gEventSystem->mLastEvent == GameSpeakEvents::eNone;
        last_event_v2 = bLastEventMinus1 ? GameSpeakEvents::eNone : GameSpeakEvents::eSameAsLast;
        last_event_v1 = bLastEventMinus1 ? GameSpeakEvents::eNone : GameSpeakEvents::eSameAsLast;
    }
    else
    {
        field_158_last_event_idx = last_event_index;
        last_event_v1 = gEventSystem->mLastEvent;
        last_event_v2 = gEventSystem->mLastEvent;
    }

    if (mStrugglingWithBees)
    {
        mNextMotion = eElumMotions::Motion_1_Idle;
        mBrainIdx = 1;
        return 0;
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    // TODO: A lot of duplicated logic below and a lot of simplifaction
    // can be done also

    switch (mBrainSubState)
    {
        case 0:
            return 2;

        case 1:
        {
            const FP xd = gAbe->mXPos - mXPos;
            if (FP_Abs(xd) < (kGridSize * FP_FromInteger(2)))
            {
                mNextMotion = eElumMotions::Motion_1_Idle;
                return 2;
            }

            if (xd > FP_FromInteger(0))
            {
                if (mVelX < FP_FromInteger(0))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle;
                    return 2;
                }
            }
            else if (xd < FP_FromInteger(0))
            {
                if (mVelX > FP_FromInteger(0))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle;
                    return 2;
                }
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle;
                    return 2;
                }
            }
            else if (mVelX > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle;
                    return 2;
                }
            }
            return mBrainSubState;
        }

        case 2:
        {
            if (NearHoney())
            {
                mBrainIdx = 1;
                mDontFollowAbe = 1;
                return 0;
            }

            const FP xd = gAbe->mXPos - mXPos;
            if (xd > (kGridSize / FP_FromInteger(2)) && GetAnimation().GetFlipX())
            {
                mNextMotion = eElumMotions::Motion_4_Turn;
                return 4;
            }
            else if (xd < -(kGridSize / FP_FromInteger(2)) && !GetAnimation().GetFlipX())
            {
                mNextMotion = eElumMotions::Motion_4_Turn;
                return 4;
            }

            auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
            if (pPlatform && pPlatform->Type() == ReliveTypes::eLiftPoint)
            {
                auto pLift = static_cast<LiftPoint*>(pPlatform);
                if (!pLift->OnAnyFloor()) // TODO: Check logic
                {
                    if (mXPos == gAbe->mXPos)
                    {
                        return 2;
                    }

                    if (VIsFacingMe(gAbe))
                    {
                        mNextMotion = eElumMotions::Motion_3_WalkLoop;
                        return 3;
                    }

                    mNextMotion = eElumMotions::Motion_4_Turn;
                    return 4;
                }
            }

            if (gAbe->mCurrentMotion == eAbeMotions::Motion_139_ElumMountBegin)
            {
                mCurrentMotion = eElumMotions::Motion_48_AbeMoutingBegin;
                return 16;
            }

            if (last_event_v1 == GameSpeakEvents::eAbe_FollowMe)
            {
                if (gMap.mCurrentLevel == mCurrentLevel
                    && gMap.mCurrentPath == mCurrentPath)
                {
                    if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                    {
                        if (FP_Abs(xd) > (kGridSize / FP_FromInteger(2)))
                        {
                            field_114_respond_timer = MakeTimer(14);
                            return 12;
                        }
                    }
                }
            }

            if (last_event_v1 == GameSpeakEvents::eAbe_Hello
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    field_114_respond_timer = MakeTimer(14);
                    return 9;
                }
            }

            if (last_event_v1 == GameSpeakEvents::eAbe_Fart
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    field_114_respond_timer = MakeTimer(28);
                    return 11;
                }
            }

            if (last_event_v1 == GameSpeakEvents::eAbe_Wait
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath
                && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                mDontFollowAbe = 1;
                field_114_respond_timer = MakeTimer(14);
                return 5;
            }

            if (FP_Abs(xd) > (kGridSize * FP_FromInteger(3)) && VOnSameYLevel(gAbe))
            {
                if (xd < FP_FromInteger(0))
                {
                    if (!Check_IsOnEndOfLine(1, 1))
                    {
                        if (!WallHit(GetSpriteScale() * FP_FromInteger(25), -kGridSize))
                        {
                            mNextMotion = eElumMotions::Motion_3_WalkLoop;
                            return 1;
                        }
                    }
                }
                else if (xd > FP_FromInteger(0))
                {
                    if (!Check_IsOnEndOfLine(0, 1))
                    {
                        if (!WallHit(GetSpriteScale() * FP_FromInteger(25), kGridSize))
                        {
                            mNextMotion = eElumMotions::Motion_3_WalkLoop;
                            return 1;
                        }
                    }
                }
            }
            return mBrainSubState;
        }

        case 3:
            if (mCurrentMotion == eElumMotions::Motion_3_WalkLoop)
            {
                mNextMotion = eElumMotions::Motion_1_Idle;
            }

            if (mCurrentMotion != eElumMotions::Motion_1_Idle)
            {
                return mBrainSubState;
            }

            if (mXPos == gAbe->mXPos)
            {
                return 2;
            }

            mNextMotion = eElumMotions::Motion_3_WalkLoop;
            return mBrainSubState;

        case 4:
            if (mCurrentMotion != eElumMotions::Motion_4_Turn || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            return 2;

        case 5:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eElumMotions::Motion_16_Speak;
            return 6;

        case 6:
        {
            if (NearHoney())
            {
                mBrainIdx = 1;
                mDontFollowAbe = 1;
                return 0;
            }

            const FP xd_1 = gAbe->mXPos - mXPos;
            if (xd_1 > (kGridSize / FP_FromInteger(2)) && GetAnimation().GetFlipX())
            {
                mNextMotion = eElumMotions::Motion_4_Turn;
                return 7;
            }
            else if (xd_1 < -(kGridSize / FP_FromInteger(2)) && !GetAnimation().GetFlipX())
            {
                mNextMotion = eElumMotions::Motion_4_Turn;
                return 7;
            }

            if (gAbe->mCurrentMotion == eAbeMotions::Motion_139_ElumMountBegin)
            {
                mCurrentMotion = eElumMotions::Motion_48_AbeMoutingBegin;
                return 16;
            }

            if (last_event_v2 == GameSpeakEvents::eAbe_FollowMe && gMap.mCurrentLevel == mCurrentLevel && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    mDontFollowAbe = 0;
                    if (FP_Abs(xd_1) < (kGridSize / FP_FromInteger(2)))
                    {
                        return 2;
                    }

                    field_114_respond_timer = MakeTimer(14);
                    return 12;
                }
            }

            if (last_event_v2 == GameSpeakEvents::eAbe_Wait
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    mDontFollowAbe = 1;
                    field_114_respond_timer = MakeTimer(14);
                    return 5;
                }
            }

            if (last_event_v2 == GameSpeakEvents::eAbe_Hello
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath)
            {
                if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
                {
                    field_114_respond_timer = MakeTimer(14);
                    return 8;
                }
            }

            if (last_event_v2 == GameSpeakEvents::eAbe_Fart
                && gMap.mCurrentLevel == mCurrentLevel
                && gMap.mCurrentPath == mCurrentPath
                && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                field_114_respond_timer = MakeTimer(28);
                return 10;
            }

            if (EventGet(kEventDeathReset) == 0)
            {
                return mBrainSubState;
            }
            mNextMotion = eElumMotions::Motion_1_Idle;
            return 2;
        }

        case 7:
            if (mCurrentMotion != eElumMotions::Motion_1_Idle)
            {
                return mBrainSubState;
            }
            mDontFollowAbe = 1;
            return 6;

        case 8:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eElumMotions::Motion_14_Speak;
            return 6;

        case 9:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eElumMotions::Motion_14_Speak;
            return 2;

        case 10:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eElumMotions::Motion_15_Speak;
            return 6;

        case 11:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eElumMotions::Motion_15_Speak;
            return 2;

        case 12:
            if (static_cast<s32>(sGnFrame) < field_114_respond_timer)
            {
                return mBrainSubState;
            }
            mNextMotion = eElumMotions::Motion_14_Speak;
            return 13;

        case 13:
        {
            if (mCurrentMotion == eElumMotions::Motion_1_Idle)
            {
                mNextMotion = eElumMotions::Motion_14_Speak;
            }

            if (mCurrentMotion != eElumMotions::Motion_14_Speak || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            const FP xd = gAbe->mXPos - mXPos;
            if (xd >= FP_FromInteger(0))
            {
                if (xd >= (kGridSize / FP_FromInteger(2)))
                {
                    mNextMotion = eElumMotions::Motion_3_WalkLoop;
                    return 14;
                }
            }
            else
            {
                if (-xd >= (kGridSize / FP_FromInteger(2)))
                {
                    mNextMotion = eElumMotions::Motion_3_WalkLoop;
                    return 14;
                }
            }
            mNextMotion = eElumMotions::Motion_1_Idle;
            return 2;
        }

        case 14:
        {
            if (last_event_v2 == GameSpeakEvents::eAbe_Wait)
            {
                mDontFollowAbe = 1;
                mNextMotion = eElumMotions::Motion_1_Idle;
                field_114_respond_timer = MakeTimer(14);
                return 5;
            }

            const FP xd = gAbe->mXPos - mXPos;
            if (FP_Abs(xd) < (kGridSize / FP_FromInteger(2)))
            {
                mNextMotion = eElumMotions::Motion_1_Idle;
                return 2;
            }

            if (xd > FP_FromInteger(0))
            {
                if (mVelX < FP_FromInteger(0))
                {
                    mNextMotion = eElumMotions::Motion_4_Turn;
                    return 15;
                }
            }
            else if (xd < FP_FromInteger(0))
            {
                if (mVelX > FP_FromInteger(0))
                {
                    mNextMotion = eElumMotions::Motion_4_Turn;
                    return 15;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle;
                    return 2;
                }
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eElumMotions::Motion_1_Idle;
                    return 2;
                }
            }
            return mBrainSubState;
        }

        case 15:
        {
            if (mCurrentMotion != eElumMotions::Motion_4_Turn || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (FP_Abs(gAbe->mXPos - mXPos) < (kGridSize / FP_FromInteger(2)))
            {
                mNextMotion = eElumMotions::Motion_1_Idle;
                return 2;
            }

            mNextMotion = eElumMotions::Motion_3_WalkLoop;
            return 14;
        }

        case 16:
            if (NearHoney())
            {
                mBrainIdx = 1;
                mDontFollowAbe = 1;
                return 0;
            }

            if (sControlledCharacter == this
                || mCurrentMotion == eElumMotions::Motion_48_AbeMoutingBegin
                || mCurrentMotion == eElumMotions::Motion_27_AbeMountingEnd)
            {
                return mBrainSubState;
            }
            mDontFollowAbe = 0;
            return 2;

        default:
            return mBrainSubState;
    }
}

s16 Elum::Brain_1_HoneyAddiction()
{
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    switch (mBrainSubState)
    {
        case 0:
        {
            if (mStrugglingWithBees)
            {
                mFoundHoney = false;
                mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                return 5;
            }

            const FP honey_xd = mXPos - FP_FromInteger(mHoneyXPos);
            if (honey_xd >= FP_FromInteger(0))
            {
                if (GetAnimation().GetFlipX())
                {
                    if (honey_xd >= (kGridSize * FP_FromInteger(2)))
                    {
                        mNextMotion = eElumMotions::Motion_3_WalkLoop;
                        Elum_SFX(ElumSounds::eSpottedHoney_6, 0);
                        return 2;
                    }
                    else
                    {
                        Elum_SFX(ElumSounds::eSpottedHoney_6, 0);
                        mNextMotion = eElumMotions::Motion_25_LickingHoney;
                        if (sControlledCharacter == this)
                        {
                            KnockAbeOffElum();
                        }
                        return 3;
                    }
                }
                mNextMotion = eElumMotions::Motion_4_Turn;
                return 1;
            }
            else
            {
                if (!GetAnimation().GetFlipX())
                {
                    if (honey_xd <= -(kGridSize * FP_FromInteger(2)))
                    {
                        mNextMotion = eElumMotions::Motion_3_WalkLoop;
                        Elum_SFX(ElumSounds::eSpottedHoney_6, 0);
                        return 2;
                    }
                    else
                    {
                        Elum_SFX(ElumSounds::eSpottedHoney_6, 0);
                        mNextMotion = eElumMotions::Motion_25_LickingHoney;
                        if (sControlledCharacter == this)
                        {
                            KnockAbeOffElum();
                        }
                        return 3;
                    }
                }
                mNextMotion = eElumMotions::Motion_4_Turn;
                return 1;
            }
            break;
        }

        case 1:
        {
            if (mCurrentMotion != eElumMotions::Motion_4_Turn || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            const FP honey_xd_1 = mXPos - FP_FromInteger(mHoneyXPos);

            if (honey_xd_1 >= FP_FromInteger(0))
            {
                if (honey_xd_1 < (kGridSize * FP_FromInteger(2)))
                {
                    Elum_SFX(ElumSounds::eSpottedHoney_6, 0);
                    mNextMotion = eElumMotions::Motion_25_LickingHoney;
                    if (sControlledCharacter == this)
                    {
                        KnockAbeOffElum();
                    }
                    return 3;
                }
            }
            else
            {
                if (honey_xd_1 > -(kGridSize * FP_FromInteger(2)))
                {
                    Elum_SFX(ElumSounds::eSpottedHoney_6, 0);
                    mNextMotion = eElumMotions::Motion_25_LickingHoney;
                    if (sControlledCharacter == this)
                    {
                        KnockAbeOffElum();
                    }
                    return 3;
                }
            }
            mNextMotion = eElumMotions::Motion_3_WalkLoop;
            Elum_SFX(ElumSounds::eSpottedHoney_6, 0);
            return 2;
        }

        case 2:
            if (!NearHoney())
            {
                mNextMotion = eElumMotions::Motion_1_Idle;
                mBrainIdx = 0;
                return 0;
            }

            if ((mVelX <= FP_FromInteger(0) && mXPos - FP_FromInteger(mHoneyXPos) >= (kGridSize * FP_FromInteger(2))) ||
                (mVelX >= FP_FromInteger(0) && FP_FromInteger(mHoneyXPos) - mXPos >= (kGridSize * FP_FromInteger(2))))
            {
                return mBrainSubState;
            }

            if (sControlledCharacter == this)
            {
                KnockAbeOffElum();
            }

            mNextMotion = eElumMotions::Motion_25_LickingHoney;
            return 3;

        case 3:
            if (!BaseAliveGameObjectCollisionLine)
            {
                mNextMotion = eElumMotions::Motion_1_Idle;
                mBrainIdx = 0;
                return 0;
            }

            if (!mStrugglingWithBees)
            {
                return mBrainSubState;
            }

            mFoundHoney = false;
            mStungByBees = true;

            mHoneyCamera = gMap.mCurrentCamera;

            mDontFollowAbe = 0;

            if (GetAnimation().GetFlipX())
            {
                mNextMotion = eElumMotions::Motion_4_Turn;
                return 4;
            }
            else
            {
                mNextMotion = eElumMotions::Motion_29_BeesStruggling;

                mStrugglingWithBees = false;
                return 5;
            }
            break;

        case 4:
            if (mCurrentMotion == eElumMotions::Motion_4_Turn && GetAnimation().GetIsLastFrame())
            {
                mStrugglingWithBees = false;
                mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                return 5;
            }
            return mBrainSubState;

        case 5:
            if (mCurrentMotion != eElumMotions::Motion_29_BeesStruggling || field_110_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mNextMotion = eElumMotions::Motion_44_ScratchBegin;
            return 6;

        case 6:
        {
            if (mCurrentMotion != eElumMotions::Motion_46_ScratchEnd || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (!mStrugglingWithBees)
            {
                mNextMotion = eElumMotions::Motion_1_Idle;
                mBrainIdx = 0;
                return 0;
            }

            if (!GetAnimation().GetFlipX())
            {
                if (Check_IsOnEndOfLine(0, 2))
                {
                    mStrugglingWithBees = false;
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                    return 5;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(1, 2))
                {
                    mStrugglingWithBees = false;
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                    return 5;
                }
            }

            FP gridSizeDirected = {};
            if (GetAnimation().GetFlipX())
            {
                gridSizeDirected = -kGridSize;
            }
            else
            {
                gridSizeDirected = kGridSize;
            }

            if (WallHit(GetSpriteScale() * FP_FromInteger(25), gridSizeDirected))
            {
                mStrugglingWithBees = false;
                mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                return 5;
            }
            else
            {
                mStrugglingWithBees = false;
                mNextMotion = eElumMotions::Motion_3_WalkLoop;
                return 7;
            }
            break;
        }

        case 7:
            if (mVelX > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                    field_110_timer = MakeTimer(40);
                    return 5;
                }
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                    field_110_timer = MakeTimer(40);
                    return 5;
                }
            }

            if (mCurrentMotion == eElumMotions::Motion_3_WalkLoop && GetAnimation().GetCurrentFrame() == 11)
            {
                mNextMotion = eElumMotions::Motion_44_ScratchBegin;
                return 9;
            }

            if (mCurrentMotion != eElumMotions::Motion_1_Idle)
            {
                return mBrainSubState;
            }

            mNextMotion = eElumMotions::Motion_29_BeesStruggling;
            field_110_timer = MakeTimer(40);
            return 5;

        case 8:
            if (field_110_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            if (!mStrugglingWithBees)
            {
                mNextMotion = eElumMotions::Motion_1_Idle;
                mBrainIdx = 0;
                return 0;
            }

            if (!GetAnimation().GetFlipX())
            {
                if (Check_IsOnEndOfLine(0, 2))
                {
                    mStrugglingWithBees = false;
                    mNextMotion = eElumMotions::Motion_44_ScratchBegin;
                    return 9;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(1, 2))
                {
                    mStrugglingWithBees = false;
                    mNextMotion = eElumMotions::Motion_44_ScratchBegin;
                    return 9;
                }
            }
            mStrugglingWithBees = false;
            mNextMotion = eElumMotions::Motion_3_WalkLoop;
            return 10;

        case 9:
            if (mCurrentMotion != eElumMotions::Motion_46_ScratchEnd || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            mNextMotion = eElumMotions::Motion_29_BeesStruggling;
            return 8;

        case 10:
            if (mVelX > FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                    field_110_timer = MakeTimer(40);
                    return 8;
                }
            }
            else if (mVelX < FP_FromInteger(0))
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                    field_110_timer = MakeTimer(40);
                    return 8;
                }
            }

            if (mCurrentMotion != eElumMotions::Motion_3_WalkLoop || GetAnimation().GetCurrentFrame() != 11)
            {
                if (mCurrentMotion == eElumMotions::Motion_1_Idle)
                {
                    mNextMotion = eElumMotions::Motion_29_BeesStruggling;
                    field_110_timer = MakeTimer(40);
                    return 8;
                }
                else
                {
                    return mBrainSubState;
                }
            }

            mNextMotion = eElumMotions::Motion_29_BeesStruggling;
            return 5;

        default:
            return mBrainSubState;
    }
}

void Elum::Motion_0_Respawn()
{
    Motion_21_Land();
}

void Elum::Motion_1_Idle()
{
    CheckLiftPointGoneAndSetCamera();

    if (gAbe->mCurrentMotion != eAbeMotions::Motion_115_ElumSpeak && !ToNextMotion())
    {
        if (gAbe->mCurrentMotion == eAbeMotions::Motion_150_Chant && sControlledCharacter == gAbe)
        {
            mCurrentMotion = eElumMotions::Motion_9_ToYell;
        }
        else if (gAbe->mCurrentMotion == eAbeMotions::Motion_137_ElumUnmountBegin)
        {
            mCurrentMotion = eElumMotions::Motion_49_AbeUnmountingBegin;
        }
        else if (sGnFrame - field_110_timer > 200 && sControlledCharacter != this)
        {
            mCurrentMotion = eElumMotions::Motion_44_ScratchBegin;
        }
    }
}

void Elum::Motion_2_Unknown()
{
    ToIdle();
}

void Elum::Motion_3_WalkLoop()
{
    field_10E_pressed |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (mCurrentMotion == eElumMotions::Motion_3_WalkLoop)
    {
        if (GetAnimation().GetCurrentFrame() == 2)
        {
            if (mNextMotion == eElumMotions::Motion_25_LickingHoney)
            {
                mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle;
            }
            else
            {
                FP offX_directed = {};
                if (GetAnimation().GetFlipX())
                {
                    offX_directed = -ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    offX_directed = ScaleToGridSize(GetSpriteScale());
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(25), offX_directed))
                {
                    mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle;
                }
                else if (sControlledCharacter == this && !mFoundHoney)
                {
                    MidWalkToNextMotion();
                }
                /*
                else if (field_104_pending_resource_count > 0)
                {
                    mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle;
                }
                */
                else
                {
                    if (mNextMotion == eElumMotions::Motion_1_Idle)
                    {
                        mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle;
                        mNextMotion = eElumMotions::None_m1;
                    }
                    else if (mNextMotion == eElumMotions::Motion_4_Turn)
                    {
                        mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle;
                    }
                }
            }
        }
        else if (GetAnimation().GetCurrentFrame() == 5)
        {
            if (!field_10C_bFootStep2)
            {
                field_10C_bFootStep2 = 1;
                MapFollowMe(1);
            }

            if (sControlledCharacter != this)
            {
                Elum_SFX(ElumSounds::eWalkingFootstep_0, 0);
                return;
            }

            if (Input().IsAnyHeld(InputCommands::eRun))
            {
                mCurrentMotion = eElumMotions::Motion_41_MidWalkToRun;
            }

            field_10E_pressed = 0;
            Elum_SFX(ElumSounds::eWalkingFootstep_0, 0);
        }
        else if (GetAnimation().GetCurrentFrame() == 11)
        {
            if (mNextMotion == eElumMotions::Motion_25_LickingHoney)
            {
                mCurrentMotion = eElumMotions::Motion_6_MidWalkToIdle;
            }
            else
            {
                FP offX_directed = {};
                if (GetAnimation().GetFlipX())
                {
                    offX_directed = -ScaleToGridSize(GetSpriteScale());
                }
                else
                {
                    offX_directed = ScaleToGridSize(GetSpriteScale());
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(25), offX_directed))
                {
                    mCurrentMotion = eElumMotions::Motion_5_WalkToIdle;
                }
                else if (sControlledCharacter == this && !mFoundHoney)
                {
                    WalkToNextMotion();
                }
                /*
                else if (field_104_pending_resource_count)
                {
                    mCurrentMotion = eElumMotions::Motion_5_WalkToIdle;
                }
                */
                else
                {
                    if (mNextMotion == eElumMotions::Motion_1_Idle)
                    {
                        mCurrentMotion = eElumMotions::Motion_5_WalkToIdle;
                        mNextMotion = eElumMotions::None_m1;
                    }
                    else if (mNextMotion == eElumMotions::Motion_4_Turn
                             || mNextMotion == eElumMotions::Motion_44_ScratchBegin
                             || mNextMotion == eElumMotions::Motion_29_BeesStruggling)
                    {
                        mCurrentMotion = eElumMotions::Motion_5_WalkToIdle;
                    }
                }
            }
        }
        else if (GetAnimation().GetCurrentFrame() == 14)
        {
            if (!field_10C_bFootStep2)
            {
                field_10C_bFootStep2 = 1;
                MapFollowMe(1);
            }

            if (sControlledCharacter != this)
            {
                Elum_SFX(ElumSounds::eWalkingFootstep_0, 0);
                return;
            }

            if (Input().IsAnyHeld(InputCommands::eRun))
            {
                mCurrentMotion = eElumMotions::Motion_40_WalkToRun;
            }

            field_10E_pressed = 0;
            Elum_SFX(ElumSounds::eWalkingFootstep_0, 0);
        }
        else
        {
            field_10C_bFootStep2 = 0;
        }
    }
}

void Elum::Motion_4_Turn()
{
    CheckLiftPointGoneAndSetCamera();

    if (!GetAnimation().GetCurrentFrame())
    {
        Environment_SFX(EnvironmentSfx::eRollingNoise_8, 0, 0x7FFF, this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(true);
        GetAnimation().ToggleFlipX();

        if (mNextMotion == eElumMotions::Motion_29_BeesStruggling)
        {
            Elum_SFX(ElumSounds::eBeesStruggle_3, 0);
            mCurrentMotion = eElumMotions::Motion_29_BeesStruggling;
            mNextMotion = eElumMotions::None_m1;
            field_110_timer = MakeTimer(25);
        }
        else if (ToNextMotion())
        {
            GetAnimation().Set_Animation_Data(GetAnimation().mAnimRes);
            if (sControlledCharacter == this)
            {
                gAbe->SyncToElum(static_cast<s16>(mCurrentMotion));
            }
        }
        else
        {
            ToIdle();
        }
    }
}

void Elum::Motion_5_WalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Elum_SFX(ElumSounds::eWalkingFootstep_0, 0);
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);

        if (mNextMotion == eElumMotions::Motion_29_BeesStruggling)
        {
            Elum_SFX(ElumSounds::eBeesStruggle_3, 0);
            mCurrentMotion = eElumMotions::Motion_29_BeesStruggling;
            field_110_timer = MakeTimer(25);
            mNextMotion = eElumMotions::None_m1;
        }
        else if (mNextMotion == eElumMotions::Motion_44_ScratchBegin)
        {
            mCurrentMotion = eElumMotions::Motion_44_ScratchBegin;
            mNextMotion = eElumMotions::None_m1;
        }
        else
        {
            ToIdle();
            /*
            if (field_104_pending_resource_count > 0)
            {
                mCurrentMotion = eElumMotions::Motion_2_Unknown;
            }
            else
            */
            {
                if (mNextMotion == eElumMotions::Motion_4_Turn || mNextMotion == eElumMotions::Motion_25_LickingHoney)
                {
                    mCurrentMotion = mNextMotion;
                    mNextMotion = eElumMotions::None_m1;
                }
            }
        }
    }
}

void Elum::Motion_6_MidWalkToIdle()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Elum_SFX(ElumSounds::eWalkingFootstep_0, 0);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);

        ToIdle();

        /*
        if (field_104_pending_resource_count)
        {
            mCurrentMotion = eElumMotions::Motion_2_Unknown;
        }
        else
        */
        {
            if (mNextMotion == eElumMotions::Motion_4_Turn || mNextMotion == eElumMotions::Motion_25_LickingHoney)
            {
                mCurrentMotion = mNextMotion;
                mNextMotion = eElumMotions::None_m1;
            }
        }
    }
}

void Elum::Motion_7_IdleToWalk2()
{
    field_10E_pressed |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        mPreviousMotion = eElumMotions::Motion_3_WalkLoop;
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_120_bUnknown = 1;
    }

    MoveOnLine(0);
}

void Elum::Motion_8_IdleToWalk1()
{
    field_10E_pressed |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_3_WalkLoop;
    }

    MoveOnLine(0);
}

void Elum::Motion_9_ToYell()
{
    CheckLiftPointGoneAndSetCamera();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_10_Yell;
    }
}

void Elum::Motion_10_Yell()
{
    EventBroadcast(kEventNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (GetAnimation().GetCurrentFrame() || !mCanSpeak)
    {
        mCanSpeak = true;
    }
    else
    {
        Elum_SFX(ElumSounds::eHowl_2, this);
        mCanSpeak = false;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (!ToNextMotion())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_11_Unknown()
{
    Motion_47_Unknown();
}

void Elum::Motion_12_RunTurn()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (!GetAnimation().GetCurrentFrame())
    {
        Elum_SFX(ElumSounds::eRunSlide_5, 0);
    }

    const FP offY = (sControlledCharacter == this) ? GetSpriteScale() * FP_FromInteger(40) : GetSpriteScale() * FP_FromInteger(25);
    if (WallHit(offY, mVelX))
    {
        mCurrentMotion = eElumMotions::Motion_50_Knockback;
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
        MapFollowMe(true);
        Environment_SFX(EnvironmentSfx::eKnockback_13, 95, -200, this);
        return;
    }

    SlowOnX(FP_FromDouble(2.125));

    if (mCurrentMotion == eElumMotions::Motion_12_RunTurn
        && GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(true);

        const FP gridSize = (GetAnimation().GetFlipX()) ? ScaleToGridSize(GetSpriteScale()) : -ScaleToGridSize(GetSpriteScale());
        if (Input().IsAnyHeld(InputCommands::eRun))
        {
            mVelX = gridSize / FP_FromInteger(4);
            mCurrentMotion = eElumMotions::Motion_38_RunTurnToRun;
            return;
        }
        mVelX = gridSize / FP_FromInteger(9);
        mCurrentMotion = eElumMotions::Motion_13_RunTurnToWalk;
    }
}

void Elum::Motion_13_RunTurnToWalk()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_3_WalkLoop;
        GetAnimation().ToggleFlipX();
    }
}

void Elum::Speak(EnvironmentSfx speak, bool setYellMotion)
{
    if (GetAnimation().GetCurrentFrame() == 3 && mCanSpeak)
    {
        Environment_SFX(speak, 75, 0, this);
        mCanSpeak = false;
    }
    else
    {
        mCanSpeak = true;
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (setYellMotion)
        {
            mNextMotion = eElumMotions::Motion_10_Yell;
        }

        if (!ToNextMotion())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_14_Speak()
{
    Speak(EnvironmentSfx::eElumOkay_21, false);
}

void Elum::Motion_15_Speak()
{
    Speak(EnvironmentSfx::eElumHowl_20, false);
}

void Elum::Motion_16_Speak()
{
    Speak(EnvironmentSfx::eElumHowl_20, true);
}

void Elum::Motion_17_Unknown()
{
    MoveOnLine(0);

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);

        if (GetAnimation().GetFlipX())
        {
            mVelX = (GetSpriteScale() * FP_FromInteger(-15));
        }
        else
        {
            mVelX = (GetSpriteScale() * FP_FromInteger(15));
        }

        mCurrentMotion = eElumMotions::Motion_30_HopBegin;
    }
}

void Elum::Motion_18_Unknown()
{
    Motion_17_Unknown();
}

void Elum::Motion_19_Dead()
{
    if (gAbe->mHealth > FP_FromInteger(0))
    {
        if (!gAbe->mShrivel && gAbe->mLandSoft /* && field_104_pending_resource_count == 0*/)
        {
            mXPos = FP_FromInteger(mContinuePointRect.x);
            mYPos = FP_FromInteger(mContinuePointRect.y);

            mVelY = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);

            mCurrentLevel = mContinuePointLevel;
            mCurrentPath = mContinuePointPath;
            SetSpriteScale(mContinuePointSpriteScale);

            mBrainIdx = 0;
            mBrainSubState = 6;
            mDontFollowAbe = 1;

            if (!gMap.Is_Point_In_Current_Camera(mContinuePointLevel, mContinuePointPath, mXPos, mYPos, 0))
            {
                Elum_SFX(ElumSounds::eHowl_2, this);
            }

            mStrugglingWithBees = false;
            mFoundHoney = false;
            field_110_timer = sGnFrame;

            GetAnimation().SetFlipX(gAbe->GetAnimation().GetFlipX());

            if (mStungByBees)
            {
                if (mContinueCamera < mHoneyCamera)
                {
                    mStungByBees = false;
                }
            }

            GetAnimation().SetRender(true);

            BaseAliveGameObjectLastLineYPos = mYPos;
            mCurrentMotion = eElumMotions::Motion_0_Respawn;
            mNextMotion = eElumMotions::None_m1;
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

void Elum::Motion_20_Fall()
{
    if (gAbe->mHealth <= FP_FromInteger(0))
    {
        mCurrentMotion = eElumMotions::Motion_19_Dead;
    }
}

void Elum::Motion_21_Land()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (GetSpriteScale() * field_118_jump_velx);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (GetSpriteScale() * field_118_jump_velx);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const s16 bHit = InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

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
            {
                Elum_SFX(ElumSounds::eHitGroundSoft_4, 0);
                BaseAliveGameObjectCollisionLine = pLine;
                if (mYPos - BaseAliveGameObjectLastLineYPos >= FP_FromInteger(20))
                {
                    mCurrentMotion = eElumMotions::Motion_22_RunOffEdge;
                }
                else
                {
                    mCurrentMotion = eElumMotions::Motion_1_Idle;
                }

                mXPos = hitX;
                mYPos = hitY;
                MapFollowMe(true);

                PSX_Point xy = {};
                xy.x = FP_GetExponent(mXPos - FP_FromInteger(10));
                xy.y = FP_GetExponent(mYPos - FP_FromInteger(10));

                PSX_Point wh = {};
                wh.x = FP_GetExponent(mXPos + FP_FromInteger(10));
                wh.y = FP_GetExponent(mYPos + FP_FromInteger(10));

                OnCollisionWith(
                    xy,
                    wh,
                    gPlatformsArray);
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

void Elum::Motion_22_RunOffEdge()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(1);
        if (!ToNextMotion())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_23_WalkOffEdge()
{
    Motion_24_JumpToFall();
}

void Elum::Motion_24_JumpToFall()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_21_Land;
    }

    Motion_21_Land();
}

void Elum::Motion_25_LickingHoney()
{
    EventBroadcast(kEventNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (GetAnimation().GetCurrentFrame() == 6)
    {
        if (gMap.GetDirection(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos)
            == CameraPos::eCamCurrent_0)
        {
            if (mCanSpeak)
            {
                Elum_SFX(ElumSounds::eLickingHoney_8, 0);
                mCanSpeak = false;
            }
        }
    }
    else
    {
        mCanSpeak = true;
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        if (mNextMotion == eElumMotions::Motion_4_Turn || mNextMotion == eElumMotions::Motion_29_BeesStruggling)
        {
            mCurrentMotion = eElumMotions::Motion_26_LickingToStruggling;
            /*
            if (!field_174_resources.res[30])
            {
                VLoadUnmountedResources_411260();
            }*/
        }
    }
}

void Elum::Motion_26_LickingToStruggling()
{
    Motion_47_Unknown();
}

void Elum::Motion_27_AbeMountingEnd()
{
    if (gAbe->mCurrentMotion != eAbeMotions::Motion_136_ElumMountEnd)
    {
        ToIdle();
    }
}

void Elum::Motion_28_AbeUnmountingEnd()
{
    if (gAbe->mCurrentMotion != eAbeMotions::Motion_138_ElumUnmountEnd)
    {
        ToIdle();
    }
}

void Elum::Motion_29_BeesStruggling()
{
    EventBroadcast(kEventNoise, this);

    if (mNextMotion == eElumMotions::Motion_44_ScratchBegin)
    {
        mCurrentMotion = eElumMotions::Motion_44_ScratchBegin;
        mNextMotion = eElumMotions::None_m1;
        return;
    }

    if (mNextMotion != eElumMotions::Motion_3_WalkLoop)
    {
        if (mNextMotion == eElumMotions::Motion_1_Idle)
        {
            ToIdle();
            mNextMotion = eElumMotions::None_m1;
        }
        return;
    }

    if (WallHit(
            GetSpriteScale() * FP_FromInteger(25),
            ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(GetAnimation().GetFlipX() != 0 ? -1 : 1)))
    {
        ToIdle();
        mNextMotion = eElumMotions::None_m1;
        return;
    }

    mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2;
    if (GetAnimation().GetFlipX())
    {
        mNextMotion = eElumMotions::None_m1;
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        mNextMotion = eElumMotions::None_m1;
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
}

void Elum::Motion_30_HopBegin()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckLiftPointGoneAndSetCamera();

    const FP xpos = GetAnimation().GetFlipX() ? GetSpriteScale() * FP_FromInteger(-56) : GetSpriteScale() * FP_FromInteger(56);
    const FP velX = GetAnimation().GetFlipX() ? GetSpriteScale() * FP_FromDouble(-9.85) : GetSpriteScale() * FP_FromDouble(9.85);
    const FP offX = GetAnimation().GetFlipX() ? FP_FromInteger(-56) : FP_FromInteger(56);

    if (WallHit(GetSpriteScale() * FP_FromInteger(40), offX))
    {
        ToKnockback();
        return;
    }

    if (!(GetAnimation().GetIsLastFrame()))
    {
        return;
    }

    mXPos += xpos;
    mVelX = velX;

    mVelY = GetSpriteScale() * FP_FromDouble(-2.7);
    BaseAliveGameObjectLastLineYPos = mYPos;

    if (WallHit(GetSpriteScale() * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
        return;
    }

    mXPos += mVelX;
    mYPos += mVelY;

    VOnTrapDoorOpen();
    mCurrentMotion = eElumMotions::Motion_31_HopMid;
    BaseAliveGameObjectCollisionLine = nullptr;
}

void Elum::RunJumpMidAndHopMid(MidType midType)
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(40), mVelX))
    {
        RunSlideStopKnockback();
    }
    else
    {
        FP hitX = {};
        FP hitY = {};

        const FP velY = midType == MidType::eRunJumpMid ? FP_FromDouble(0.8) : FP_FromDouble(0.9);
        const FP jump_velx = midType == MidType::eRunJumpMid ? FP_FromDouble(1.1) : FP_FromDouble(2.15);

        const auto bInAirCollision = InAirCollision(&BaseAliveGameObjectCollisionLine, &hitX, &hitY, velY);

        if (sControlledCharacter == this)
        {
            SetActiveCameraDelayedFromDir();
        }

        if (bInAirCollision)
        {
            switch (BaseAliveGameObjectCollisionLine->mLineType)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eDynamicCollision_32:
                case eLineTypes::eBackgroundDynamicCollision_36:
                    Elum_SFX(ElumSounds::eHitGroundSoft_4, 0);

                    switch (midType)
                    {
                        case MidType::eRunJumpMid:
                            mXPos = hitX;
                            mYPos = hitY;
                            mCurrentMotion = eElumMotions::Motion_35_RunJumpLand;
                            MapFollowMe(true);
                            break;

                        case MidType::eHopMid:
                            mVelY = FP_FromInteger(0);
                            mVelX = FP_FromInteger(0);
                            mCurrentMotion = eElumMotions::Motion_32_HopLand;
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
            mCurrentMotion = eElumMotions::Motion_24_JumpToFall;
        }
    }
}

void Elum::Motion_31_HopMid()
{
    RunJumpMidAndHopMid(MidType::eHopMid);
}

void Elum::Motion_32_HopLand()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (GetAnimation().GetIsLastFrame())
    {
        const PSX_RECT bRect = VGetBoundingRect();

        OnCollisionWith(
            {bRect.x, static_cast<s16>(bRect.y + 5)},
            {bRect.w, static_cast<s16>(bRect.h + 5)},
            gPlatformsArray);

        MapFollowMe(true);

        if (!ToNextMotion())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_33_RunJumpBegin()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (WallHit(GetSpriteScale() * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        FP velX = {};
        if (GetAnimation().GetFlipX())
        {
            velX = GetSpriteScale() * FP_FromDouble(-11.43);
        }
        else
        {
            velX = GetSpriteScale() * FP_FromDouble(11.43);
        }

        mVelX = velX;

        BaseAliveGameObjectLastLineYPos = mYPos;
        mVelY = GetSpriteScale() * FP_FromInteger(-4);

        mXPos += mVelX;
        mYPos += GetSpriteScale() * FP_FromInteger(-4);

        VOnTrapDoorOpen();
        mCurrentMotion = eElumMotions::Motion_34_RunJumpMid;
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

void Elum::Motion_34_RunJumpMid()
{
    RunJumpMidAndHopMid(MidType::eRunJumpMid);
}

void Elum::Motion_35_RunJumpLand()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (GetAnimation().GetIsLastFrame())
    {
        const PSX_RECT bRect = VGetBoundingRect();

        OnCollisionWith(
            {bRect.x, static_cast<s16>(bRect.y + 5)},
            {bRect.w, static_cast<s16>(bRect.h + 5)},
            gPlatformsArray);

        if (sControlledCharacter == this)
        {
            FP offX = {};
            if (GetAnimation().GetFlipX())
            {
                offX = -(GetSpriteScale() * FP_FromDouble(18.75));
            }
            else
            {
                offX = GetSpriteScale() * FP_FromDouble(18.75);
            }

            if (WallHit(GetSpriteScale() * FP_FromInteger(40), offX))
            {
                ToKnockback();
            }
            else
            {
                mPreviousMotion = eElumMotions::Motion_36_RunLoop;
                mBaseAliveGameObjectLastAnimFrame = 4;
                field_120_bUnknown = 1;
                if (GetAnimation().GetFlipX())
                {
                    mXPos -= GetSpriteScale() * FP_FromDouble(18.75);
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }
                else
                {
                    mXPos += GetSpriteScale() * FP_FromDouble(18.75);
                    mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
                }
            }
        }
        else
        {
            mVelX = (mVelX >= FP_FromInteger(0)) ? FP_FromDouble(13.3) : FP_FromDouble(-13.3);
            mCurrentMotion = eElumMotions::Motion_12_RunTurn;
        }
    }
}

void Elum::Motion_36_RunLoop()
{
    field_10E_pressed |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
        return;
    }

    s16 gridSizeDirected = 0;
    if (mVelX <= FP_FromInteger(0))
    {
        gridSizeDirected = FP_GetExponent(ScaleToGridSize(GetSpriteScale()));
    }
    else
    {
        gridSizeDirected = -FP_GetExponent(ScaleToGridSize(GetSpriteScale()));
    }

    MoveOnLine(gridSizeDirected);

    if (mCurrentMotion == eElumMotions::Motion_36_RunLoop)
    {
        if (GetAnimation().GetCurrentFrame() != 0 && GetAnimation().GetCurrentFrame() != 4)
        {
            field_10C_bFootStep2 = 0;
            return;
        }

        Elum_SFX(ElumSounds::eRunningFootstep_1, 0);

        if (!field_10C_bFootStep2)
        {
            field_10C_bFootStep2 = 1;
            MapFollowMe(true);
        }

        if (sControlledCharacter == this)
        {
            if (InputCommands::eHop & field_10E_pressed)
            {
                mCurrentMotion = eElumMotions::Motion_33_RunJumpBegin;
                field_10E_pressed = 0;
            }
            else if (Input().IsAnyHeld(InputCommands::eRight) && mVelX < FP_FromInteger(0))
            {
                mCurrentMotion = eElumMotions::Motion_12_RunTurn;
                mVelX = GetSpriteScale() * FP_FromDouble(-13.3);
            }
            else if (Input().IsAnyHeld(InputCommands::eLeft) && mVelX > FP_FromInteger(0))
            {
                mCurrentMotion = eElumMotions::Motion_12_RunTurn;
                mVelX = GetSpriteScale() * FP_FromDouble(13.3);
            }
            else if (!Input().IsAnyHeld(InputCommands::eRight | InputCommands::eLeft))
            {
                mCurrentMotion = eElumMotions::Motion_37_RunSlideStop;

                if (mVelX >= FP_FromInteger(0))
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(13.3));
                }
                else
                {
                    mVelX = (GetSpriteScale() * FP_FromDouble(-13.3));
                }
            }
            else if (!Input().IsAnyHeld(InputCommands::eRun))
            {
                if (GetAnimation().GetCurrentFrame() == 0)
                {
                    mCurrentMotion = eElumMotions::Motion_42_RunToWalk;
                }
                else
                {
                    mCurrentMotion = eElumMotions::Motion_43_MidRunToWalk;
                }
            }
            return;
        }
        else
        {
            mCurrentMotion = eElumMotions::Motion_12_RunTurn;

            if (mVelX >= FP_FromInteger(0))
            {
                mVelX = (GetSpriteScale() * FP_FromDouble(13.3));
            }
            else
            {
                mVelX = (GetSpriteScale() * FP_FromDouble(-13.3));
            }
        }

        field_10E_pressed = 0;
    }
}

void Elum::RunSlideStopKnockback()
{
    mCurrentMotion = eElumMotions::Motion_50_Knockback;
    if (BaseAliveGameObjectCollisionLine)
    {
        mVelX = -mVelX;
        MoveOnLine(false);
    }
    else
    {
        mXPos -= mVelX;
    }
    mVelX = FP_FromInteger(0);
    MapFollowMe(true);
    Environment_SFX(EnvironmentSfx::eKnockback_13, 95, -200, this);
}

void Elum::Motion_37_RunSlideStop()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Elum_SFX(ElumSounds::eRunSlide_5, 0);
    }

    const FP offY = (sControlledCharacter == this) ? GetSpriteScale() * FP_FromInteger(40) : GetSpriteScale() * FP_FromInteger(25);

    if (WallHit(offY, mVelX))
    {
        RunSlideStopKnockback();
        return;
    }

    SlowOnX(FP_FromDouble(2.125));

    if (sControlledCharacter == this && GetAnimation().GetCurrentFrame() < 7)
    {
        if ((GetAnimation().GetFlipX() && Input().IsAnyHeld(InputCommands::eRight)) || (!GetAnimation().GetFlipX() && Input().IsAnyHeld(InputCommands::eLeft)))
        {
            mPreviousMotion = eElumMotions::Motion_12_RunTurn;
            mBaseAliveGameObjectLastAnimFrame = GetAnimation().GetCurrentFrame();
            field_120_bUnknown = 1;
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        MapFollowMe(true);
        if (!ToNextMotion())
        {
            field_118_jump_velx = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            mCurrentMotion = eElumMotions::Motion_1_Idle;
            field_110_timer = sGnFrame;
            field_10E_pressed = 0;
            MapFollowMe(true);
        }
    }
}

void Elum::Motion_38_RunTurnToRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    MoveOnLine(0);

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_36_RunLoop;
        GetAnimation().ToggleFlipX();
    }
}

void Elum::Motion_39_IdleToRun()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10E_pressed |= Input().GetHeld();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_36_RunLoop;
    }

    MoveOnLine(0);
}

void Elum::Motion_40_WalkToRun()
{
    field_10E_pressed |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_36_RunLoop;
    }

    MoveOnLine(0);
}

void Elum::Motion_41_MidWalkToRun()
{
    field_10E_pressed |= Input().GetHeld();

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }
    else
    {
        mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mPreviousMotion = eElumMotions::Motion_36_RunLoop;
        mBaseAliveGameObjectLastAnimFrame = 4;
        field_120_bUnknown = 1;
    }

    MoveOnLine(0);
}

void Elum::Motion_42_RunToWalk()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10E_pressed |= Input().GetHeld();

    FP velX{};
    if (GetAnimation().GetFlipX())
    {
        velX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        velX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    mVelX = velX;

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_3_WalkLoop;
        VCheckCollisionLineStillValid(10);

        PSX_Point xy{FP_GetExponent(mXPos - FP_FromInteger(10)), FP_GetExponent(mYPos - FP_FromInteger(10))};
        PSX_Point wh{FP_GetExponent(mXPos + FP_FromInteger(10)), FP_GetExponent(mYPos + FP_FromInteger(10))};
        OnCollisionWith(
            xy,
            wh,
            gPlatformsArray);

        FP offX = {};
        if (GetAnimation().GetFlipX())
        {
            offX = FP_FromInteger(-2) * ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            offX = FP_FromInteger(2) * ScaleToGridSize(GetSpriteScale());
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(40), offX))
        {
            ToKnockback();
            return;
        }
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
        return;
    }

    MoveOnLine(0);
}

void Elum::Motion_43_MidRunToWalk()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    field_10E_pressed |= Input().GetHeld();

    FP velX = {};
    if (GetAnimation().GetFlipX())
    {
        velX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }
    else
    {
        velX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
    }

    mVelX = velX;

    if (GetAnimation().GetIsLastFrame())
    {
        mPreviousMotion = eElumMotions::Motion_3_WalkLoop;
        mBaseAliveGameObjectLastAnimFrame = 9;
        field_120_bUnknown = 1;
        VCheckCollisionLineStillValid(10);

        PSX_Point xy{FP_GetExponent(mXPos - FP_FromInteger(10)), FP_GetExponent(mYPos - FP_FromInteger(10))};
        PSX_Point wh{FP_GetExponent(mXPos + FP_FromInteger(10)), FP_GetExponent(mYPos + FP_FromInteger(10))};

        OnCollisionWith(
            xy,
            wh,
            gPlatformsArray);

        FP offX = {};
        if (GetAnimation().GetFlipX())
        {
            offX = FP_FromInteger(-2) * ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            offX = FP_FromInteger(2) * ScaleToGridSize(GetSpriteScale());
        }

        if (WallHit(GetSpriteScale() * FP_FromInteger(40), offX))
        {
            ToKnockback();
            return;
        }
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(40), mVelX))
    {
        ToKnockback();
        return;
    }
    MoveOnLine(0);
}

void Elum::Motion_44_ScratchBegin()
{
    CheckLiftPointGoneAndSetCamera();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_45_ScratchLoop;
        if (gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) == CameraPos::eCamCurrent_0)
        {
            SND_SEQ_PlaySeq(SeqId::Unknown_15, 1, 1);
        }
    }
}

void Elum::Motion_45_ScratchLoop()
{
    CheckLiftPointGoneAndSetCamera();

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eElumMotions::Motion_46_ScratchEnd;
    }
}

void Elum::Motion_46_ScratchEnd()
{
    CheckLiftPointGoneAndSetCamera();

    if (GetAnimation().GetIsLastFrame())
    {
        if (mNextMotion == eElumMotions::Motion_3_WalkLoop)
        {
            if (GetAnimation().GetFlipX())
            {
                if (!WallHit(GetSpriteScale() * FP_FromInteger(40), -ScaleToGridSize(GetSpriteScale())))
                {
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                    mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2;
                    mNextMotion = eElumMotions::None_m1;
                    return;
                }
            }
            else
            {
                if (!WallHit(GetSpriteScale() * FP_FromInteger(40), ScaleToGridSize(GetSpriteScale())))
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(9));
                    mCurrentMotion = eElumMotions::Motion_8_IdleToWalk2;
                    mNextMotion = eElumMotions::None_m1;
                    return;
                }
            }
            mNextMotion = eElumMotions::None_m1;
            return;
        }

        if (mNextMotion == eElumMotions::Motion_29_BeesStruggling)
        {
            Elum_SFX(ElumSounds::eBeesStruggle_3, 0);
            mCurrentMotion = eElumMotions::Motion_29_BeesStruggling;
            field_110_timer = MakeTimer(25);
            mNextMotion = eElumMotions::None_m1;
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
    mCurrentMotion = eElumMotions::Motion_1_Idle;
    field_10E_pressed = 0;
    MapFollowMe(true);
}

void Elum::Motion_47_Unknown()
{
    EventBroadcast(kEventNoise, this);

    CheckLiftPointGoneAndSetCamera();

    if (GetAnimation().GetIsLastFrame())
    {
        if (!ToNextMotion())
        {
            ToIdle();
        }
    }
}

void Elum::Motion_48_AbeMoutingBegin()
{
    if (gAbe->mCurrentMotion == eAbeMotions::Motion_136_ElumMountEnd)
    {
        mCurrentMotion = eElumMotions::Motion_27_AbeMountingEnd;
    }
}

void Elum::Motion_49_AbeUnmountingBegin()
{
    if (gAbe->mCurrentMotion != eAbeMotions::Motion_137_ElumUnmountBegin)
    {
        mCurrentMotion = eElumMotions::Motion_28_AbeUnmountingEnd;
    }
}

void Elum::Motion_50_Knockback()
{
    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_21_Land();
        if (mCurrentMotion == eElumMotions::Motion_1_Idle)
        {
            mCurrentMotion = eElumMotions::Motion_50_Knockback;
            field_110_timer = MakeTimer(10);
        }
    }

    if (GetAnimation().GetIsLastFrame())
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
    if (gDDCheat_FlyingEnabled && sControlledCharacter == this)
    {
        VOnTrapDoorOpen();

        mCurrentMotion = eElumMotions::Motion_21_Land;
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

        if (Input().IsAnyHeld(InputCommands::eUp | InputCommands::eDown | InputCommands::eLeft | InputCommands::eRight))
        {
            const s32 tableIdx = Input().Dir();
            mVelX = FP_FromRaw(sElum_velx_table_4FF968[tableIdx]);
            mVelY = FP_FromRaw(sElum_vely_table_4FF988[tableIdx]);

            if (Input().IsAnyHeld(InputCommands::eRun))
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

        SetActiveCameraDelayedFromDir();

        gAbe->mXPos = mXPos;
        gAbe->mYPos = mYPos;
        return;
    }

    if (!(mFoundHoney || mStungByBees) && mBrainIdx != 1)
    {
        FindHoney();
    }

    if (mChangedPathNotMounted)
    {
        HandleElumPathTrans();
        mChangedPathMounted = false;
        mChangedPathNotMounted = false;
    }

    if (mCurrentMotion == eElumMotions::Motion_19_Dead || mCurrentPath == gMap.mCurrentPath)
    {
        PathLine* pLine = nullptr;
        if (mChangedPathMounted)
        {
            FP hitX = {};
            FP hitY = {};
            if (gCollisions->Raycast(
                    mXPos,
                    mYPos - FP_FromInteger(40),
                    mXPos,
                    mYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
            }
            else
            {
                BaseAliveGameObjectCollisionLine = nullptr;
                mCurrentMotion = eElumMotions::Motion_21_Land;
            }
            mChangedPathMounted = false;
        }

        const auto oldMotion = mCurrentMotion;
        const FP old_x = mXPos;
        const FP old_y = mYPos;

        if (oldMotion != eElumMotions::Motion_19_Dead)
        {
            mBrainSubState = InvokeMemberFunction(this, sElum_brain_table_4C52E8, mBrainIdx);
        }

        if (mFalling)
        {
            VCheckCollisionLineStillValid(10);

            const PSX_RECT bRect = VGetBoundingRect();

            OnCollisionWith(
                {bRect.x, static_cast<s16>(bRect.y + 5)},
                {bRect.w, static_cast<s16>(bRect.h + 5)},
                gPlatformsArray);

            if (BaseAliveGameObject_PlatformId != Guid{})
            {
                mFalling = false;
            }
        }

        InvokeMemberFunction(this, sElumMotionTable, mCurrentMotion);

        if ((oldMotion != mCurrentMotion && oldMotion == eElumMotions::Motion_2_Unknown) || oldMotion == eElumMotions::Motion_11_Unknown || oldMotion == eElumMotions::Motion_47_Unknown)
        {
            //LOG_INFO("old motion: " << oldMotion << " | new motion: " << mCurrentMotion);
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

                GetAnimation().Set_Animation_Data(GetAnimRes(gElumMotionAnimIds[static_cast<u32>(mCurrentMotion)]));
                GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                field_120_bUnknown = 0;
                if (sControlledCharacter == this)
                {
                    gAbe->SyncToElum(static_cast<s16>(mCurrentMotion));
                }
            }
        }
        else
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(gElumMotionAnimIds[static_cast<u32>(mCurrentMotion)]));
            if (sControlledCharacter == this)
            {
                gAbe->SyncToElum(static_cast<s16>(mCurrentMotion));
            }
        }

        if (EventGet(kEventDeathReset))
        {
            if (!field_154_bAbeForcedDownFromElum)
            {
                if (gAbe->mContinuePointZoneNumber != mPreviousContinuePointZoneNumber)
                {
                    mBrainIdx = 0;
                    mBrainSubState = 6;
                    mDontFollowAbe = 1;

                    if (sControlledCharacter == this)
                    {
                        gAbe->mXPos = mXPos;
                        gAbe->mYPos = mYPos;
                        gAbe->GetAnimation().SetFlipX(GetAnimation().GetFlipX());
                    }
                    return;
                }
            }

            GetAnimation().SetRender(false);
            mCurrentMotion = eElumMotions::Motion_19_Dead;
            VOnTrapDoorOpen();
        }

        if (sControlledCharacter == this)
        {
            gAbe->mXPos = mXPos;
            gAbe->mYPos = mYPos;
            gAbe->GetAnimation().SetFlipX(GetAnimation().GetFlipX());
        }
        return;
    }
}

void Elum::VRender(OrderingTable& ot)
{
    if (mCurrentLevel == gMap.mCurrentLevel
        && mCurrentPath == gMap.mCurrentPath
        && !UpdateDelay())
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void Elum::VScreenChanged()
{
    if (gMap.LevelChanged())
    {
        SetDead(true);
    }
    else
    {
        if (gMap.PathChanged())
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eLines)
            {
                SetDead(true);
            }
            else if (gMap.mCurrentLevel == EReliveLevelIds::eDesert && gMap.mNextPath == 9)
            {
                SetDead(true);
            }
            else if (mCurrentPath == gMap.mCurrentPath)
            {
                auto pElumPathTrans = static_cast<relive::Path_ElumPathTrans*>(gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eElumPathTrans));

                if (pElumPathTrans)
                {
                    if (mDontFollowAbe != 1 && sControlledCharacter != this && pElumPathTrans->mNextLevel == gMap.mNextLevel && pElumPathTrans->mNextPath == gMap.mNextPath)
                    {
                        mChangedPathNotMounted = true;
                    }
                }
            }

            if (BaseAliveGameObject_PlatformId != Guid{})
            {
                VOnTrapDoorOpen();
                mFalling = true;
            }

            if (!mChangedPathNotMounted)
            {
                BaseAliveGameObjectCollisionLine = nullptr;
                mChangedPathMounted = true;
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
    mTlvId = tlvInfo;

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Elum_Land));

    SetSpriteScale(gAbe->GetSpriteScale());

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesElumMuds_28);
    }
    else
    {
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesMudsElum_Half_9);
    }

    SetCanSetOffExplosives(true);

    mStungByBees = false;
    mFoundHoney = false;
    mChangedPathNotMounted = false;
    mChangedPathMounted = false;

    gElum = this;

    mCurrentMotion = eElumMotions::Motion_21_Land;
    field_120_bUnknown = 0;

    mHoneyYPos = 0;
    mHoneyXPos = 0;

    mRespawnOnDead = 0;
    field_110_timer = sGnFrame;

    mXPos = gAbe->mXPos - (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2));
    mYPos = gAbe->mYPos - FP_FromInteger(5);

    mDontFollowAbe = 0;
    field_124_bShould_IdleToWalk1 = 1;
    field_10E_pressed = 0;
    SetCanBeesChase(true);
    mBaseAliveGameObjectLastAnimFrame = 0;

    mPreviousContinuePointZoneNumber = 0;
    mAbeZoneNumber = gAbe->mContinuePointZoneNumber;

    field_154_bAbeForcedDownFromElum = 0;
    mBrainIdx = 0;
    mBrainSubState = 0;

    SetDoPurpleLightEffect(true);


    mStrugglingWithBees = false;
    mFalling = false;

    mCanSpeak = false;

    relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 30);

    CreateShadow();
}

} // namespace AO
