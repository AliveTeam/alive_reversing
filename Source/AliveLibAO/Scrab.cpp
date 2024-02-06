#include "stdafx_ao.h"
#include "Scrab.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Collisions.hpp"
#include "DDCheat.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "MusicController.hpp"
#include "PlatformBase.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "CameraSwapper.hpp"
#include "LiftPoint.hpp"
#include "Game.hpp"
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "Midi.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Grid.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Path.hpp"
#include "Abe.hpp"
#include "../AliveLibAE/Game.hpp"

namespace AO {

using TScrabMotionFunction = decltype(&Scrab::Motion_0_Empty);

const TScrabMotionFunction sScrabMotionTable[30] = {
#define MAKE_FN(VAR) &Scrab::VAR,
    SCRAB_MOTIONS_ENUM_AO(MAKE_FN)
#undef MAKE_FN
};

Scrab::Scrab(relive::Path_Scrab* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eScrab);

    for (const auto& animId : sScrabMotionAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }

    Animation_Init(GetAnimRes(AnimId::Scrab_Idle));

    SetCanSetOffExplosives(true);

    field_118_timer = 0;

    SetBrain(&Scrab::Brain_Patrol);

    mBrainSubState = 0;
    mNextMotion = eScrabMotions::Motion_0_Empty;
    mCurrentMotion = eScrabMotions::Motion_1_Stand;

    SetFightTarget(nullptr);
    SetTarget(nullptr);

    field_140_last_shriek_timer = 0;

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
        SetScale(Scale::Fg);
    }

    mAttackDelay = pTlv->mAttackDelay;
    mPatrolType = pTlv->mPatrolType;
    mPauseLeftMin = pTlv->mPauseLeftMin;
    mPauseLeftMax = pTlv->mPauseLeftMax;
    mPauseRightMin = pTlv->mPauseRightMin;
    mPauseRightMax = pTlv->mPauseRightMax;

    field_138_spotting_abe_delay = pTlv->mSpottingAbeDelay;

    // TODO: flags
    field_188_flags = 32 * (pTlv->mRoarRandomly == relive::reliveChoice::eYes) | (((field_188_flags & ~0x11) | 4) & ~0x28);

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
            GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
        ToStand();

        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            PlatformCollide();
        }

        field_188_flags |= 8u;
    }

    SetDoPurpleLightEffect(true);

    mRGB.SetRGB(127, 127, 127);

    field_134_tlvInfo = tlvId;

    CreateShadow();
}

Scrab::~Scrab()
{
    SetFightTarget(nullptr);
    SetTarget(nullptr);
    VOnTrapDoorOpen();

    if (mHealth <= FP_FromInteger(0))
    {
        Path::TLV_Delete(field_134_tlvInfo);
    }
    else
    {
        Path::TLV_Reset(field_134_tlvInfo);
    }

    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
    MusicController::ClearObject(this);
}

void Scrab::VRender(OrderingTable& ot)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

void Scrab::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!(field_188_flags & 8))
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
                GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
            == 1)
        {
            mYPos = hitY;

            ToStand();

            if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PlatformCollide();
            }

            field_188_flags |= 8u;
        }
    }

    const FP hero_xd = FP_Abs(mXPos - gAbe->mXPos);
    const FP hero_yd = FP_Abs(mYPos - gAbe->mYPos);

    if (hero_xd > FP_FromInteger(2048) || hero_yd > FP_FromInteger(960))
    {
        GetAnimation().SetAnimate(false);
        GetAnimation().SetRender(false);

        // TODO: unloaded vram here
    }
    else
    {
        if (mHealth > FP_FromInteger(0))
        {
            // TODO: og loaded vram here
            GetAnimation().SetAnimate(true);
            GetAnimation().SetRender(true);
        }

        const auto old_motion = mCurrentMotion;

        mBrainSubState = (this->*mBrainState)();

        if (gDDCheat_ShowAI_Info)
        {
            DDCheat::DebugStr(
                "Scrab %d %d %d %d\n",
                mBrainSubState,
                field_118_timer,
                mCurrentMotion,
                mNextMotion);
        }


        const FP old_x = mXPos;
        const FP old_y = mYPos;

        InvokeMemberFunction(this, sScrabMotionTable, mCurrentMotion);

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

        if (old_motion != mCurrentMotion || field_188_flags & 0x10)
        {
            field_188_flags &= ~0x10u;
            vUpdateAnim();

            if (old_motion != eScrabMotions::Motion_0_Empty)
            {
                GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
            }
        }
    }
}

enum Brain_BatDeath
{
    eStartHowling_0 = 0,
    eWalkAway_1 = 1,
    eStartShrieking_2 = 2,
    eTurnAround_3 = 3,
    eDie_4 = 4
};

bool Scrab::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth > FP_FromInteger(0))
    {
        switch (pFrom->Type())
        {
            case ReliveTypes::eBat:
                if (BrainIs(&Scrab::Brain_BatDeath))
                {
                    return true;
                }

                mHealth = FP_FromInteger(0);
                mNextMotion = eScrabMotions::Motion_1_Stand;
                SetBrain(&Scrab::Brain_BatDeath);
                mBrainSubState = Brain_BatDeath::eStartHowling_0;
                return true;

            case ReliveTypes::eBullet:
            case ReliveTypes::eRollingBall:
                mHealth = FP_FromInteger(0);
                SetBrain(&Scrab::Brain_Death);
                field_118_timer = MakeTimer(90);
                mCurrentMotion = eScrabMotions::Motion_29_DeathBegin;
                vUpdateAnim();
                break;

            case ReliveTypes::eGroundExplosion:
            case ReliveTypes::eAirExplosion:
            {
                relive_new Gibs(
                    GibType::eSlog,
                    mXPos,
                    mYPos,
                    mVelX,
                    mVelY,
                    GetSpriteScale(),
                    false);

                SetDead(true);
                return true;
            }

            case ReliveTypes::eAbilityRing:
                return false;

            default:
                SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                SfxPlayMono(relive::SoundEffects::FallingItemHit, 90);
                mHealth = FP_FromInteger(0);
                SetBrain(&Scrab::Brain_Death);
                field_118_timer = MakeTimer(90);
                mCurrentMotion = eScrabMotions::Motion_29_DeathBegin;
                vUpdateAnim();
                break;
        }
    }
    return true;
}

void Scrab::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            Scrab_SFX(ScrabSounds::eYell_8, 127, -1000, 0);
            SetDead(true);
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

void Scrab::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetTarget(nullptr);
        SetFightTarget(nullptr);
        SetDead(true);
    }
    else
    {
        if (mAbeOrMudTarget)
        {
            if (mAbeOrMudTarget->GetDead())
            {
                SetTarget(nullptr);
                mNextMotion = eScrabMotions::Motion_1_Stand;
                SetBrain(&Scrab::Brain_WalkAround);
                mBrainSubState = 0;
            }
        }
    }
}

void Scrab::ToStand()
{
    field_128 = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);
    mCurrentMotion = eScrabMotions::Motion_1_Stand;
    MapFollowMe(1);
}

void Scrab::vUpdateAnim()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sScrabMotionAnimIds[static_cast<u32>(mCurrentMotion)]));
}

void Scrab::PlatformCollide()
{
    PSX_RECT bRect = VGetBoundingRect();

    bRect.y += 5;
    bRect.h += 5;

    OnCollisionWith(
        {bRect.x, bRect.y},
        {bRect.w, bRect.h},
        gPlatformsArray);
}

void Scrab::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        field_188_flags |= 0x10u;
        mCurrentMotion = eScrabMotions::Motion_15_ToFall;
    }
}

s16 Scrab::ToNextMotion()
{
    MapFollowMe(1);

    switch (mNextMotion)
    {
        case eScrabMotions::Motion_4_Turn:
        case eScrabMotions::Motion_16_Stamp:
        case eScrabMotions::Motion_19_Unused:
        case eScrabMotions::Motion_20_HowlBegin:
        case eScrabMotions::Motion_22_Shriek:
        case eScrabMotions::Motion_23_ScrabBattleAnim:
        case eScrabMotions::Motion_27_AttackLunge:
        case eScrabMotions::Motion_28_LegKick:
        case eScrabMotions::Motion_25_ToFeed:
            mCurrentMotion = mNextMotion;
            mNextMotion = eScrabMotions::None_m1;
            return 1;

        case eScrabMotions::Motion_3_Run:
            if (GetAnimation().GetFlipX())
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), -ScaleToGridSize(GetSpriteScale())))
                {
                    return 0;
                }
                else
                {
                    mCurrentMotion = eScrabMotions::Motion_11_StandToRun;
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromDouble(3.5));
                    mNextMotion = eScrabMotions::None_m1;
                    return 1;
                }
            }
            else
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), ScaleToGridSize(GetSpriteScale())))
                {
                    return 0;
                }
                else
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromDouble(3.5));
                    mCurrentMotion = eScrabMotions::Motion_11_StandToRun;
                    mNextMotion = eScrabMotions::None_m1;
                    return 1;
                }
            }
            break;

        case eScrabMotions::Motion_2_Walk:
            if (GetAnimation().GetFlipX())
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), -ScaleToGridSize(GetSpriteScale())))
                {
                    return 0;
                }
                else
                {
                    mCurrentMotion = eScrabMotions::Motion_10_StandToWalk;
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(7));
                    mNextMotion = eScrabMotions::None_m1;
                    return 1;
                }
            }
            else
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), ScaleToGridSize(GetSpriteScale())))
                {
                    return 0;
                }
                else
                {
                    mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(7));
                    mCurrentMotion = eScrabMotions::Motion_10_StandToWalk;
                    mNextMotion = eScrabMotions::None_m1;
                    return 1;
                }
            }
            break;

        case eScrabMotions::Motion_1_Stand:
            ToStand();
            return 1;

        case eScrabMotions::Motion_7_HopMidair:
            if (GetAnimation().GetFlipX())
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), -ScaleToGridSize(GetSpriteScale())))
                {
                    return 0;
                }
            }
            else
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), ScaleToGridSize(GetSpriteScale())))
                {
                    return 0;
                }
            }
            mCurrentMotion = eScrabMotions::Motion_6_HopBegin;
            mNextMotion = eScrabMotions::None_m1;
            return 1;

        default:
            return 0;
    }
}

const relive::SfxDefinition sScrabSfx_4CF798[9] = {
    {0, 39, 60, 55, 0, 0},
    {0, 39, 61, 70, 0, 0},
    {0, 39, 62, 80, 0, 0},
    {0, 39, 63, 80, 0, 0},
    {0, 39, 64, 60, -127, 127},
    {0, 39, 66, 90, 0, 0},
    {0, 39, 67, 50, -511, 0},
    {0, 39, 67, 50, 0, 511},
    {0, 39, 68, 110, -1791, -1791}};

s32 Scrab::Scrab_SFX(ScrabSounds soundId, s32 /*vol*/, s32 pitch, s16 applyDirection)
{
    s32 volumeLeft = 0;
    s32 volumeRight = 0;

    auto defaultSndIdxVol = sScrabSfx_4CF798[static_cast<s32>(soundId)].mDefaultVolume;
    if (GetSpriteScale() == FP_FromInteger(1))
    {
        volumeRight = defaultSndIdxVol;
    }
    else
    {
        volumeRight = defaultSndIdxVol / 2;
    }

    CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);
    PSX_RECT worldRect;
    gMap.Get_Camera_World_Rect(direction, &worldRect);
    volumeLeft = volumeRight;
    if (applyDirection)
    {
        switch (direction)
        {
            case CameraPos::eCamCurrent_0:
            {
                break;
            }
            case CameraPos::eCamTop_1:
            case CameraPos::eCamBottom_2:
            {
                volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
                volumeRight = volumeLeft;
            }
            break;
            case CameraPos::eCamLeft_3:
            {
                FP percentHowFar = (FP_FromInteger(worldRect.w) - mXPos) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
                volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
                break;
            }
            case CameraPos::eCamRight_4:
            {
                FP percentHowFar = (mXPos - FP_FromInteger(worldRect.x)) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
                volumeRight -= FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
                break;
            }
            default:
                return 0;
        }
    }
    return SFX_SfxDefinition_Play(sScrabSfx_4CF798[static_cast<s32>(soundId)],
                                         static_cast<s16>(volumeLeft),
                                         static_cast<s16>(volumeRight),
                                         static_cast<s16>(pitch),
                                         static_cast<s16>(pitch));
}

void Scrab::ToJump()
{
    BaseAliveGameObjectLastLineYPos = mYPos;

    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * FP_FromDouble(-5.7));
    }
    else
    {
        mVelX = (GetSpriteScale() * FP_FromDouble(5.7));
    }

    mVelY = (GetSpriteScale() * FP_FromDouble(-9.6));
    mYPos += mVelY;
    VOnTrapDoorOpen();
    mCurrentMotion = eScrabMotions::Motion_13_RunJumpBegin;
    BaseAliveGameObjectCollisionLine = nullptr;
}

void Scrab::MoveOnLine()
{
    BaseGameObject* pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    const FP oldX = mXPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mXPos,
            &mYPos,
            mVelX);

        if (BaseAliveGameObjectCollisionLine)
        {
            if (pObj)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const auto oldMotion = mCurrentMotion;
                    VOnTrapDoorOpen();
                    mCurrentMotion = oldMotion;
                }
            }
            else
            {
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    PlatformCollide();
                }
            }
        }
        else
        {
            VOnTrapDoorOpen();
            BaseAliveGameObjectLastLineYPos = mYPos;
            mCurrentMotion = eScrabMotions::Motion_15_ToFall;
            mXPos = mVelX + oldX;
            field_128 = FP_FromInteger(1);
        }
    }
    else
    {
        mCurrentMotion = eScrabMotions::Motion_9_JumpToFall;
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
}

bool Scrab::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pObj)
{
    const PSX_RECT ourRect = VGetBoundingRect();
    const PSX_RECT otherRect = pObj->VGetBoundingRect();

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

Scrab* Scrab::FindScrabToFight()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eScrab)
        {
            Scrab* pOther = static_cast<Scrab*>(pObj);
            if (pOther != this
                && !pOther->BrainIs(&Scrab::Brain_Death)
                && VOnSameYLevel(pOther)
                && gMap.Is_Point_In_Current_Camera(
                    pOther->mCurrentLevel,
                    pOther->mCurrentPath,
                    pOther->mXPos,
                    pOther->mYPos,
                    0)
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                return pOther;
            }
        }
    }
    return nullptr;
}


bool Scrab::FindAbeOrMud()
{
    if (CanSeeAbe(gAbe) && gAbe->mHealth > FP_FromInteger(0) && gAbe->GetSpriteScale() == GetSpriteScale() && !WallHit(gAbe->mXPos - mXPos, GetSpriteScale() * FP_FromInteger(35)))
    {
        SetTarget(gAbe);
        return true;
    }

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->GetIsBaseAliveGameObject())
        {
            BaseAliveGameObject* pObj = static_cast<BaseAliveGameObject*>(pObjIter);

            if (pObj->Type() == ReliveTypes::eRingOrLiftMud || pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::SlingMud)
            {
                if (CanSeeAbe(pObj) && pObj->mHealth > FP_FromInteger(0) && pObj->GetSpriteScale() == GetSpriteScale() && !WallHit(pObj->mXPos - mXPos, GetSpriteScale() * FP_FromInteger(35)))
                {
                    SetTarget(pObj);
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

    if (pObj == gAbe)
    {
        if (gAbe->mCurrentMotion == eAbeMotions::Motion_66_LedgeHang
            || gAbe->mCurrentMotion == eAbeMotions::Motion_68_LedgeHangWobble)
        {
            return VOnSameYLevel(pObj);
        }
    }
    return pObj->mYPos > (mYPos - GetSpriteScale() * FP_FromInteger(35));
}

void Scrab::Motion_0_Empty()
{
    if (gNumCamSwappers <= 0)
    {
        mCurrentMotion = mPreviousMotion;
        if (BaseAliveGameObject_PlatformId != Guid{})
        {
            mXPos = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
            mYPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
        }
    }
}

void Scrab::Motion_1_Stand()
{
    ToNextMotion();

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

const FP sWalkVelTable_4BC788[22] = {
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

void Scrab::Motion_2_Walk()
{
    switch (GetAnimation().GetCurrentFrame())
    {
        case 3:
        case 8:
        case 13:
        case 18:
            Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
            break;

        default:
            break;
    }

    FP vel = {};
    if (GetAnimation().GetFlipX())
    {
        vel = -sWalkVelTable_4BC788[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        vel = sWalkVelTable_4BC788[GetAnimation().GetCurrentFrame()];
    }

    mVelX = (GetSpriteScale() * vel);

    const FP xOff = mVelX + FP_FromRaw(mVelX.fpValue / 2);
    if (WallHit(GetSpriteScale() * FP_FromInteger(30), xOff))
    {
        ToStand();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion == eScrabMotions::Motion_2_Walk)
    {
        if (GetAnimation().GetCurrentFrame() == 5 || GetAnimation().GetCurrentFrame() == 15)
        {
            if (mNextMotion == eScrabMotions::Motion_1_Stand
                || mNextMotion == eScrabMotions::Motion_4_Turn
                || mNextMotion == eScrabMotions::Motion_16_Stamp
                || mNextMotion == eScrabMotions::Motion_19_Unused
                || mNextMotion == eScrabMotions::Motion_22_Shriek
                || mNextMotion == eScrabMotions::Motion_20_HowlBegin
                || mNextMotion == eScrabMotions::Motion_7_HopMidair
                || mNextMotion == eScrabMotions::Motion_27_AttackLunge
                || mNextMotion == eScrabMotions::Motion_28_LegKick)
            {
                mCurrentMotion = eScrabMotions::Motion_12_WalkToStand;
            }
        }
        else if (GetAnimation().GetCurrentFrame() == 7 || GetAnimation().GetCurrentFrame() == 18)
        {
            if (mNextMotion == eScrabMotions::Motion_3_Run)
            {
                mCurrentMotion = eScrabMotions::Motion_3_Run;
                mNextMotion = eScrabMotions::None_m1;
            }

            if (!(field_188_flags & 2))
            {
                MapFollowMe(1);
                field_188_flags |= 2u;
            }
        }
        else
        {
            field_188_flags &= ~2u;
        }

        if (gMap.GetDirection(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos)
            >= CameraPos::eCamCurrent_0)
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

const FP sRunVelTable_4BC800[14] = {
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


void Scrab::Motion_3_Run()
{
    switch (GetAnimation().GetCurrentFrame())
    {
        case 0:
        case 5:
        case 7:
        case 12:
            Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
            break;
        default:
            break;
    }

    FP vel = {};
    if (GetAnimation().GetFlipX())
    {
        vel = -sRunVelTable_4BC800[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        vel = sRunVelTable_4BC800[GetAnimation().GetCurrentFrame()];
    }
    mVelX = (GetSpriteScale() * vel);

    if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eScrabMotions::Motion_3_Run)
        {
            if (mAbeOrMudTarget == gAbe)
            {
                const PSX_RECT objRect = mAbeOrMudTarget->VGetBoundingRect();
                const PSX_RECT bRect = VGetBoundingRect();

                if (bRect.x <= objRect.w
                    && bRect.w >= objRect.x
                    && bRect.h >= objRect.y
                    && bRect.y <= objRect.h
                    && mAbeOrMudTarget->GetSpriteScale() == GetSpriteScale())
                {
                    if (VIsFacingMe(mAbeOrMudTarget))
                    {
                        if (mAbeOrMudTarget->VTakeDamage(this))
                        {
                            SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                            Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, mAbeOrMudTarget);
                        }
                    }
                }
            }

            if (GetAnimation().GetCurrentFrame() != 3 && GetAnimation().GetCurrentFrame() != 10)
            {
                field_188_flags &= ~2u;
            }
            else
            {
                if (mNextMotion == eScrabMotions::Motion_2_Walk)
                {
                    mCurrentMotion = eScrabMotions::Motion_2_Walk;
                    mNextMotion = eScrabMotions::None_m1;
                }

                if (mNextMotion == eScrabMotions::Motion_1_Stand
                    || mNextMotion == eScrabMotions::Motion_4_Turn
                    || mNextMotion == eScrabMotions::Motion_7_HopMidair
                    || mNextMotion == eScrabMotions::Motion_27_AttackLunge
                    || mNextMotion == eScrabMotions::Motion_19_Unused
                    || mNextMotion == eScrabMotions::Motion_20_HowlBegin)
                {
                    mCurrentMotion = eScrabMotions::Motion_5_RunToStand;
                }

                if (mNextMotion == eScrabMotions::Motion_23_ScrabBattleAnim)
                {
                    ToStand();
                    mCurrentMotion = eScrabMotions::Motion_23_ScrabBattleAnim;
                    mNextMotion = eScrabMotions::None_m1;
                }

                if (!(field_188_flags & 2))
                {
                    MapFollowMe(1);
                    field_188_flags |= 2;
                }
            }

            if (gMap.GetDirection(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos)
                >= CameraPos::eCamCurrent_0)
            {
                MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
            }
        }
    }
}

void Scrab::Motion_4_Turn()
{
    if (GetAnimation().GetCurrentFrame() == 5
        || GetAnimation().GetCurrentFrame() == 9
        || GetAnimation().GetCurrentFrame() == 11)
    {
        Scrab_SFX(ScrabSounds::eWalk1_6, Math_RandomRange(40, 50), 0x7FFF, 1);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        GetAnimation().ToggleFlipX();

        if (ToNextMotion())
        {
            // TODO: Check this
            GetAnimation().Set_Animation_Data(GetAnimation().mAnimRes);
        }
        else
        {
            ToStand();
        }
    }
}

const FP sRunToStandVelTable_4BC838[10] = {
    FP_FromDouble(7.49066162109375),
    FP_FromDouble(6.9139404296875),
    FP_FromDouble(3.889495849609375),
    FP_FromDouble(2.368621826171875),
    FP_FromDouble(0.7609710693359375),
    FP_FromDouble(0.64093017578125),
    FP_FromDouble(0.7079010009765625),
    FP_FromDouble(0.7738494873046875),
    FP_FromDouble(0.7254180908203125),
    };

void Scrab::Motion_5_RunToStand()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sRunToStandVelTable_4BC838[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sRunToStandVelTable_4BC838[GetAnimation().GetCurrentFrame()]);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        ToStand();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion == eScrabMotions::Motion_5_RunToStand)
    {
        if (mAbeOrMudTarget)
        {
            const PSX_RECT bObjRect = mAbeOrMudTarget->VGetBoundingRect();
            const PSX_RECT bRect = VGetBoundingRect();

            if (bObjRect.x <= bRect.w
                && bObjRect.w >= bRect.x
                && bObjRect.h >= bRect.y
                && bObjRect.y <= bRect.h
                && mAbeOrMudTarget->GetSpriteScale() == GetSpriteScale())
            {
                if (VIsFacingMe(mAbeOrMudTarget))
                {
                    if (mAbeOrMudTarget->VTakeDamage(this))
                    {
                        SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                        Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, mAbeOrMudTarget);
                    }
                }
            }
        }

        if (GetAnimation().GetIsLastFrame())
        {
            MapFollowMe(1);

            if (!ToNextMotion())
            {
                ToStand();
            }
        }
    }
}

const FP sHopBeginVelTable_4BC860[4] = {
    FP_FromDouble(9.889144897460938),
    FP_FromDouble(10.10455322265625),
    FP_FromDouble(10.10455322265625),
    FP_FromDouble(9.889144897460938) };

void Scrab::Motion_6_HopBegin()
{
    EventBroadcast(kEventNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sHopBeginVelTable_4BC860[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sHopBeginVelTable_4BC860[GetAnimation().GetCurrentFrame()]);
    }

    if (!WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        mXPos += mVelX;

        if (GetAnimation().GetIsLastFrame())
        {
            SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -800);

            BaseAliveGameObjectLastLineYPos = mYPos;

            if (GetAnimation().GetFlipX())
            {
                mVelX = (GetSpriteScale() * -FP_FromDouble(5.7));
            }
            else
            {
                mVelX = (GetSpriteScale() * FP_FromDouble(5.7));
            }

            mVelY = (GetSpriteScale() * -FP_FromDouble(9.6));
            mYPos += mVelY;
            VOnTrapDoorOpen();
            mCurrentMotion = eScrabMotions::Motion_7_HopMidair;
            BaseAliveGameObjectCollisionLine = nullptr;
        }
    }
}

const FP sHopMidAirVelTable_4BC870[8] = {
    FP_FromDouble(9.458358764648438),
    FP_FromDouble(8.812179565429688),
    FP_FromDouble(8.11224365234375),
    FP_FromDouble(7.560302734375),
    FP_FromDouble(7.0954742431640625),
    FP_FromDouble(6.7178192138671875),
    FP_FromDouble(6.4272918701171875),
    FP_FromDouble(6.2239532470703125)};

void Scrab::Motion_7_HopMidair()
{
    EventBroadcast(kEventNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sHopMidAirVelTable_4BC870[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sHopMidAirVelTable_4BC870[GetAnimation().GetCurrentFrame()]);
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        ToStand();
        mCurrentMotion = eScrabMotions::Motion_9_JumpToFall;
    }
    else
    {
        if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
        {
            switch (pLine->mLineType)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eDynamicCollision_32:
                case eLineTypes::eBackgroundDynamicCollision_36:
                {
                    ToStand();
                    BaseAliveGameObjectCollisionLine = pLine;
                    mCurrentMotion = eScrabMotions::Motion_8_HopLand;

                    PSX_RECT bRect = VGetBoundingRect();
                    bRect.y += 5;
                    bRect.h += 5;

                    OnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        gPlatformsArray);

                    mXPos = hitX;
                    mYPos = hitY;
                    return;
                }

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
            field_128 = FP_FromDouble(1.25);
            mCurrentMotion = eScrabMotions::Motion_9_JumpToFall;
        }
    }
}

const FP sLandVelXTable_4BC890[4] = {
    FP_FromDouble(6.1077423095703125),
    FP_FromDouble(6.078704833984375),
    FP_FromDouble(6.1367950439453125),
    FP_FromDouble(6.2820892333984375)};

void Scrab::Motion_8_HopLand()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
    }

    EventBroadcast(kEventNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sLandVelXTable_4BC890[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sLandVelXTable_4BC890[GetAnimation().GetCurrentFrame()]);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        ToStand();
        return;
    }

    MoveOnLine();

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }
}

void Scrab::Motion_9_JumpToFall()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (GetSpriteScale() * field_128);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (GetSpriteScale() * field_128);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                ToStand();

                BaseAliveGameObjectCollisionLine = pLine;

                PSX_RECT bRect = VGetBoundingRect();
                bRect.y += 5;
                bRect.h += 5;

                OnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.y, bRect.w},
                    gPlatformsArray);

                mXPos = hitX;
                mYPos = hitY;
                MapFollowMe(1);
                break;
            }

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
                mVelX = (-mVelX / FP_FromInteger(2));
                break;

            default:
                return;
        }
    }
}

const FP sStandToWalkVelTable_4BC778[3] = {
    FP_FromDouble(1.1195068359375),
    FP_FromDouble(2.3691558837890625),
    FP_FromDouble(3.2076568603515625)};

void Scrab::Motion_10_StandToWalk()
{
    FP vel = sStandToWalkVelTable_4BC778[GetAnimation().GetCurrentFrame()];

    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -vel);
    }
    else
    {
        mVelX = (GetSpriteScale() * vel);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eScrabMotions::Motion_10_StandToWalk)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                mCurrentMotion = eScrabMotions::Motion_2_Walk;
            }
        }
    }
}

const FP sStandToRunVel_4BC7F0[3] = {
    FP_FromDouble(2.29766845703125),
    FP_FromDouble(2.6963653564453125),
    FP_FromDouble(3.8794708251953125)};

void Scrab::Motion_11_StandToRun()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sStandToRunVel_4BC7F0[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sStandToRunVel_4BC7F0[GetAnimation().GetCurrentFrame()]);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        ToStand();
    }
    else
    {
        MoveOnLine();

        if (mCurrentMotion == eScrabMotions::Motion_11_StandToRun)
        {
            if (GetAnimation().GetIsLastFrame())
            {
                mCurrentMotion = eScrabMotions::Motion_3_Run;
            }
        }
    }
}

const FP sWalkToStandVel_4BC7E0[3] = {
    FP_FromDouble(1.041473388671875),
    FP_FromDouble(3.2936248779296875),
    FP_FromDouble(2.8589019775390625)};

void Scrab::Motion_12_WalkToStand()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sWalkToStandVel_4BC7E0[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sWalkToStandVel_4BC7E0[GetAnimation().GetCurrentFrame()]);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        ToStand();
    }

    MoveOnLine();

    if (mCurrentMotion == eScrabMotions::Motion_12_WalkToStand && GetAnimation().GetIsLastFrame() && !ToNextMotion())
    {
        ToStand();
    }
}

const FP sRunJumpBeginVelTable_4BC8A0[14] = {
    FP_FromDouble(13.124908447265625),
    FP_FromDouble(12.364852905273438),
    FP_FromDouble(11.541610717773438),
    FP_FromDouble(10.89239501953125),
    FP_FromDouble(9.345687866210938),
    FP_FromDouble(8.901473999023438),
    FP_FromDouble(8.55975341796875),
    FP_FromDouble(8.320602416992188),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)};

void Scrab::Motion_13_RunJumpBegin()
{
    if (GetAnimation().GetCurrentFrame() == 1)
    {
        SFX_Play_Pitch(relive::SoundEffects::PickupItem, 50, -800);
    }

    EventBroadcast(kEventNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sRunJumpBeginVelTable_4BC8A0[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sRunJumpBeginVelTable_4BC8A0[GetAnimation().GetCurrentFrame()]);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        ToStand();
        mCurrentMotion = eScrabMotions::Motion_9_JumpToFall;
    }
    else
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
        {
            switch (pLine->mLineType)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eDynamicCollision_32:
                case eLineTypes::eBackgroundDynamicCollision_36:
                {
                    ToStand();
                    BaseAliveGameObjectCollisionLine = pLine;
                    mCurrentMotion = eScrabMotions::Motion_14_RunJumpEnd;

                    PSX_RECT bRect = VGetBoundingRect();
                    bRect.y += 5;
                    bRect.h += 5;

                    OnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        gPlatformsArray);

                    mXPos = hitX;
                    mYPos = hitY;
                    return;
                }

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
            field_128 = FP_FromDouble(1.25);
            mCurrentMotion = eScrabMotions::Motion_9_JumpToFall;
        }

        if (gMap.GetDirection(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos)
            >= CameraPos::eCamCurrent_0)
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

const FP sRunJumpEndVelTable_4BC8C0[4] = {
    FP_FromDouble(4.1839141845703125),
    FP_FromDouble(4.1497650146484375),
    FP_FromDouble(4.21807861328125),
    FP_FromDouble(4.3889617919921875)};

void Scrab::Motion_14_RunJumpEnd()
{
    EventBroadcast(kEventNoise, this);

    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sRunJumpEndVelTable_4BC8C0[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = (GetSpriteScale() * sRunJumpEndVelTable_4BC8C0[GetAnimation().GetCurrentFrame()]);
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
    {
        ToStand();
        return;
    }

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

void Scrab::Motion_15_ToFall()
{
    Motion_9_JumpToFall();

    if (mCurrentMotion == eScrabMotions::Motion_1_Stand)
    {
        mCurrentMotion = eScrabMotions::Motion_8_HopLand;
    }
}

void Scrab::Motion_16_Stamp()
{
    if (GetAnimation().GetCurrentFrame() == 9)
    {
        Scrab_SFX(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
        SFX_Play_Pitch(relive::SoundEffects::KillEffect, 60, Math_RandomRange(-255, 255));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        Scrab_SFX(ScrabSounds::eHowl_0, 60, 511, 1);
        ToNextMotion();
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::Motion_17_Empty()
{
    // Empty
}

void Scrab::Motion_18_GetEaten()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_9_JumpToFall();

        if (mCurrentMotion != eScrabMotions::Motion_18_GetEaten)
        {
            mCurrentMotion = eScrabMotions::Motion_18_GetEaten;
        }
    }
}

void Scrab::Motion_19_Unused()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (!ToNextMotion())
        {
            ToStand();
        }
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::Motion_20_HowlBegin()
{
    if (GetAnimation().GetCurrentFrame() == 2)
    {
        Scrab_SFX(ScrabSounds::eYell_8, 0, Math_RandomRange(-1600, -900), 1);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (mNextMotion != eScrabMotions::None_m1)
        {
            mCurrentMotion = eScrabMotions::Motion_21_HowlEnd;
        }
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::Motion_21_HowlEnd()
{
    if (GetAnimation().GetIsLastFrame())
    {
        ToNextMotion();
    }
}

void Scrab::Motion_22_Shriek()
{
    if (GetAnimation().GetCurrentFrame() == 4)
    {
        Scrab_SFX(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        if (!ToNextMotion())
        {
            ToStand();
        }
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

void Scrab::Motion_23_ScrabBattleAnim()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        field_14C = Scrab_SFX(ScrabSounds::eShredding_5, 100, Math_RandomRange(-600, 200), 1);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToNextMotion();
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::Motion_24_FeedToGulp()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (mNextMotion == eScrabMotions::Motion_25_ToFeed)
        {
            field_188_flags &= ~4u;
            mCurrentMotion = eScrabMotions::Motion_25_ToFeed;
            mNextMotion = eScrabMotions::None_m1;
        }
        else
        {
            ToStand();
        }
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
    }
}

const u32 sFeedVelTable_4BC8D0[12] = {
    380127,
    526666,
    388557,
    295550,
    135669,
    8971,
    4294918160,
    4294924422,
    4294964851,
    0,
    4294966370,
    4294966453};


void Scrab::Motion_25_ToFeed()
{
    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -FP_FromRaw(sFeedVelTable_4BC8D0[GetAnimation().GetCurrentFrame()]));
    }
    else
    {
        mVelX = (GetSpriteScale() * FP_FromRaw(sFeedVelTable_4BC8D0[GetAnimation().GetCurrentFrame()]));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eScrabMotions::Motion_26_Feed;
    }

    if ((field_188_flags >> 2) & 1)
    {
        if (WallHit(GetSpriteScale() * FP_FromInteger(30), mVelX))
        {
            ToStand();
        }
        else
        {
            MoveOnLine();
        }
    }
}

void Scrab::Motion_26_Feed()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        if (Math_RandomRange(0, 100) >= 50)
        {
            SfxPlayMono(relive::SoundEffects::Eating2, 0);
        }
        else
        {
            SfxPlayMono(relive::SoundEffects::Eating1, 0);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eScrabMotions::Motion_24_FeedToGulp;
    }
}

void Scrab::Motion_27_AttackLunge()
{
    if (mAbeOrMudTarget)
    {
        const PSX_RECT objRect = mAbeOrMudTarget->VGetBoundingRect();
        const PSX_RECT bRect = VGetBoundingRect();

        if (objRect.x <= bRect.w
            && objRect.w >= bRect.x
            && objRect.h >= bRect.y
            && objRect.y <= bRect.h
            && mAbeOrMudTarget->GetSpriteScale() == GetSpriteScale())
        {
            if (VIsFacingMe(mAbeOrMudTarget))
            {
                if (mAbeOrMudTarget->VTakeDamage(this))
                {
                    SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, mAbeOrMudTarget);
                }

                mAbeOrMudTarget->VTakeDamage(this);
            }
        }

        if (GetAnimation().GetCurrentFrame() == 4)
        {
            Scrab_SFX(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToStand();
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Scrab::Motion_28_LegKick()
{
    Motion_27_AttackLunge();
}


void Scrab::Motion_29_DeathBegin()
{
    if (!BaseAliveGameObjectCollisionLine)
    {
        Motion_9_JumpToFall();

        if (mCurrentMotion != eScrabMotions::Motion_29_DeathBegin)
        {
            mCurrentMotion = eScrabMotions::Motion_29_DeathBegin;
        }
    }

    s32 tableVal = 0;
    if (GetAnimation().GetCurrentFrame() == -1)
    {
        tableVal = 0;
    }
    else
    {
        tableVal = sFeedVelTable_4BC8D0[GetAnimation().GetCurrentFrame()];
    }

    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -FP_FromRaw(tableVal));
    }
    else
    {
        mVelX = (GetSpriteScale() * FP_FromRaw(tableVal));
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eScrabMotions::Motion_18_GetEaten;
    }
}

s16 Scrab::Brain_Fighting()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    Scrab* pFighter = mScrabTarget;
    if (pFighter && (pFighter->GetDead() || !VOnSameYLevel(mScrabTarget)))
    {
        SetFightTarget(nullptr);
        field_188_flags &= ~1u;
        mNextMotion = eScrabMotions::Motion_1_Stand;
        SetBrain(&Scrab::Brain_WalkAround); // patrol ??
        return 0;
    }

    switch (mBrainSubState)
    {
        case 0:
            if (gNumCamSwappers > 0)
            {
                return mBrainSubState;
            }

            if (VIsFacingMe(mScrabTarget))
            {
                if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(8), mScrabTarget))
                {
                    if (mScrabTarget->mCurrentMotion == eScrabMotions::Motion_20_HowlBegin)
                    {
                        return mBrainSubState;
                    }
                    mNextMotion = eScrabMotions::Motion_20_HowlBegin;
                    return 7;
                }
                else
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 2;
                }
            }
            else
            {
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return 1;
            }
            break;

        case 1:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn || mHealth < FP_FromInteger(0))
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return mBrainSubState;
            }

            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (mScrabTarget->mXPos != mXPos ||
                (mScrabTarget->GetAnimation().GetFlipX() != GetAnimation().GetFlipX()) ||
                mScrabTarget->mCurrentMotion != eScrabMotions::Motion_4_Turn)
            {
                mNextMotion = eScrabMotions::Motion_1_Stand;
                return 0;
            }
            else
            {
                mNextMotion = eScrabMotions::Motion_2_Walk;
                return 3;
            }
            break;

        case 2:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn || mHealth < FP_FromInteger(0))
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return mBrainSubState;
            }

            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            mNextMotion = eScrabMotions::Motion_2_Walk;
            return 3;

        case 3:
        {
            FP xpos = {};
            if (GetAnimation().GetFlipX())
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), -(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2))))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }

                if (Check_IsOnEndOfLine(1, 2))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }

                if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(10), mScrabTarget))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }
                xpos = mXPos - FP_FromInteger(50);
            }
            else
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2)))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }

                if (Check_IsOnEndOfLine(0, 2))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }

                if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(10), mScrabTarget))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }
                xpos = mXPos + FP_FromInteger(50);
            }

            if (gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    xpos,
                    mYPos,
                    0))
            {
                return mBrainSubState;
            }

            mNextMotion = eScrabMotions::Motion_4_Turn;
            return 4;
        }

        case 4:
            if (mCurrentMotion == eScrabMotions::Motion_4_Turn)
            {
                if (!GetAnimation().GetIsLastFrame())
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_20_HowlBegin;
                return 7;
            }
            else
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn || mHealth < FP_FromInteger(0))
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return mBrainSubState;
            }
            break;

        case 5:
            if (mCurrentMotion != eScrabMotions::Motion_19_Unused
                || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand;
            field_118_timer = MakeTimer(30);
            return 6;

        case 6:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_20_HowlBegin;
            return 7;

        case 7:
            if (mCurrentMotion != eScrabMotions::Motion_20_HowlBegin
                || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand;
            field_118_timer = MakeTimer(20);
            return 8;

        case 8:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_22_Shriek;
            return 9;

        case 9:
            if (mCurrentMotion != eScrabMotions::Motion_22_Shriek
                || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            field_188_flags |= 1u;
            mNextMotion = eScrabMotions::Motion_1_Stand;
            return 10;

        case 10:
            if (!(mScrabTarget->field_188_flags & 1))
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_3_Run;
            return 11;

        case 11:
            if (!VIsObjNearby(ScaleToGridSize(GetSpriteScale()), mScrabTarget)
                || !BaseAliveGameObjectCollisionLine
                || !mScrabTarget->BaseAliveGameObjectCollisionLine)
            {
                return mBrainSubState;
            }

            MapFollowMe(true);

            if (mScrabTarget->GetAnimation().GetRender())
            {
                GetAnimation().SetRender(false);
                mHealth = FP_FromInteger(0);
            }
            mCurrentMotion = eScrabMotions::Motion_23_ScrabBattleAnim;
            field_188_flags &= ~1u;
            field_118_timer = MakeTimer(40);
            return 12;

        case 12:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            SND_Stop_Channels_Mask(field_14C);

            field_14C = 0;

            Scrab_SFX(ScrabSounds::eDeathHowl_1, 0, -1571, 1);
            Scrab_SFX(ScrabSounds::eYell_8, 0, -1571, 1);
            Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 0, -383, 0);
            if (GetAnimation().GetRender())
            {
                SetFightTarget(nullptr);
                mCurrentMotion = eScrabMotions::Motion_1_Stand;
                field_118_timer = MakeTimer(20);
                return 13;
            }
            else
            {
                GetAnimation().SetRender(true);
                mXPos = mScrabTarget->mXPos;
                SetFightTarget(nullptr);
                SetBrain(&Scrab::Brain_Death);
                mCurrentMotion = eScrabMotions::Motion_29_DeathBegin;
                field_118_timer = MakeTimer(90);
                return 0;
            }
            break;

        case 13:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_16_Stamp;
            field_118_timer = MakeTimer(75);
            return 14;

        case 14:
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_20_HowlBegin;
            field_118_timer = MakeTimer(45);
            return 15;

        case 15:
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand;
                SetBrain(&Scrab::Brain_WalkAround); // patrol ??
                return 0;
            }
            return mBrainSubState;

        default:
            return mBrainSubState;
    }
}

s16 Scrab::Brain_BatDeath()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mBrainSubState)
    {
        case Brain_BatDeath::eStartHowling_0:
            if (mCurrentMotion == eScrabMotions::Motion_1_Stand)
            {
                mNextMotion = eScrabMotions::Motion_20_HowlBegin;
                field_118_timer = MakeTimer(45);
                return Brain_BatDeath::eWalkAway_1;
            }
            break;

        case Brain_BatDeath::eWalkAway_1:
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                mNextMotion = eScrabMotions::Motion_2_Walk;
                return Brain_BatDeath::eStartShrieking_2;
            }
            break;

        case Brain_BatDeath::eStartShrieking_2:
            if (mCurrentMotion == eScrabMotions::Motion_2_Walk)
            {
                mNextMotion = eScrabMotions::Motion_22_Shriek;
                return Brain_BatDeath::eTurnAround_3;
            }
            break;

        case Brain_BatDeath::eTurnAround_3:
            if (mCurrentMotion == eScrabMotions::Motion_22_Shriek
                && GetAnimation().GetIsLastFrame())
            {
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return Brain_BatDeath::eDie_4;
            }
            break;

        case Brain_BatDeath::eDie_4:
            if (mCurrentMotion == eScrabMotions::Motion_4_Turn)
            {
                if (GetAnimation().GetIsLastFrame())
                {
                    SetBrain(&Scrab::Brain_Death);
                    field_118_timer = MakeTimer(90);
                    mCurrentMotion = eScrabMotions::Motion_29_DeathBegin;
                }
            }
            break;

        default:
            break;
    }

    return mBrainSubState;
}

s16 Scrab::Brain_Death()
{
    if (field_118_timer > static_cast<s32>(sGnFrame) && field_118_timer < static_cast<s32>(sGnFrame) + 80)
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
    }

    if (static_cast<s32>(sGnFrame) < field_118_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (field_118_timer < static_cast<s32>(sGnFrame))
    {
        SetDead(true);
    }

    return 100;
}

s16 Scrab::Brain_ChasingEnemy()
{
    // 0 to 17
    if (mBrainSubState == 8)
    {
        //return Brain_ChasingEnemy_Real_45CC90();
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    auto pFightTarget = FindScrabToFight();
    if (pFightTarget)
    {
        SetFightTarget(pFightTarget);
        SetTarget(nullptr);
        SetBrain(&Scrab::Brain_Fighting);
        mNextMotion = eScrabMotions::Motion_1_Stand;
        return 0;
    }

    if (mAbeOrMudTarget->GetDead()
        || (field_13C_spotting_timer <= static_cast<s32>(sGnFrame)
            && !CanSeeAbe(mAbeOrMudTarget)
            && mAbeOrMudTarget->mHealth > FP_FromInteger(0)
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0)))
    {
        bool bCloseToEdge = false;
        if (GetAnimation().GetFlipX())
        {
            if (!Check_IsOnEndOfLine(1, 2))
            {
                bCloseToEdge = true;
            }
        }
        else
        {
            if (!Check_IsOnEndOfLine(0, 2))
            {
                bCloseToEdge = true;
            }
        }

        if (bCloseToEdge)
        {
            SetTarget(nullptr);
            mNextMotion = eScrabMotions::Motion_1_Stand;
            SetBrain(&Scrab::Brain_WalkAround);
            return 0;
        }
    }

    if (EventGet(kEventAbeOhm))
    {
        mNextMotion = eScrabMotions::Motion_20_HowlBegin;
        return 18;
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    
    PlatformBase* pPlatformBase = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    LiftPoint* pLiftPoint = nullptr;
    if (pPlatformBase && pPlatformBase->Type() == ReliveTypes::eLiftPoint)
    {
        pLiftPoint = static_cast<LiftPoint*>(pPlatformBase);
    }
  
    switch (mBrainSubState)
    {
        case 0:
            if (gNumCamSwappers > 0)
            {
                return mBrainSubState;
            }
            field_13C_spotting_timer = MakeTimer(field_138_spotting_abe_delay);
            return 1;

        case 1:
        {
            if ((!CanSeeAbe(mAbeOrMudTarget)
                 && gMap.Is_Point_In_Current_Camera(
                     mCurrentLevel,
                     mCurrentPath,
                     mXPos,
                     mYPos,
                     0))
                || WallHit(GetSpriteScale() * FP_FromInteger(30), mAbeOrMudTarget->mXPos - mXPos)
                || mAbeOrMudTarget->mHealth <= FP_FromInteger(0))
            {
                if (mCurrentMotion == eScrabMotions::Motion_1_Stand && Math_NextRandom() < 26u && (field_188_flags & 0x20) && (sGnFrame - field_140_last_shriek_timer) > 60)
                {
                    field_140_last_shriek_timer = sGnFrame;
                    mNextMotion = eScrabMotions::Motion_22_Shriek;
                    return 17;
                }

                return 1;
            }

            if (!VIsFacingMe(mAbeOrMudTarget))
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                }
                else
                {
                    mCurrentMotion = eScrabMotions::Motion_4_Turn;
                    mNextMotion = eScrabMotions::None_m1;
                    MapFollowMe(true);
                }
                return 2;
            }

            if (pLiftPoint)
            {
                if (!pLiftPoint->OnAnyFloor())
                {
                    return 4;
                }
            }

            if (VIsObjNearby(kGridSize, mAbeOrMudTarget) && VOnSameYLevel(mAbeOrMudTarget))
            {
                mNextMotion = eScrabMotions::Motion_28_LegKick;
                return 10;
            }

            relive::Path_TLV* pTlv = nullptr;
            if (GetAnimation().GetFlipX())
            {
                pTlv = gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos - kGridSize),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos - kGridSize),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);
            }
            else
            {
                pTlv = gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos + kGridSize),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos + kGridSize),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);
            }

            if (!pTlv)
            {
                pTlv = gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);
            }

            auto pStopper = static_cast<relive::Path_EnemyStopper*>(pTlv);
            BaseAliveGameObjectPathTLV = pTlv;
            if (pStopper)
            {
                const bool bLeft = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && mAbeOrMudTarget->mXPos < mXPos;
                const bool bRight = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && mAbeOrMudTarget->mXPos > mXPos;
                const bool bBoth = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both;
                const bool bSwitchOn = SwitchStates_Get(pStopper->mSwitchId) ? true : false;
                if ((bLeft || bRight || bBoth) && !bSwitchOn)
                {
                    if (mCurrentMotion == eScrabMotions::Motion_1_Stand && Math_NextRandom() < 26u && (field_188_flags & 0x20) && (sGnFrame - field_140_last_shriek_timer) > 60)
                    {
                        field_140_last_shriek_timer = sGnFrame;
                        mNextMotion = eScrabMotions::Motion_22_Shriek;
                        return 17;
                    }
                    return 1;
                }
            }

            if (VIsObjNearby(kGridSize * FP_FromDouble(1.5), mAbeOrMudTarget) && VOnSameYLevel(mAbeOrMudTarget))
            {
                mNextMotion = eScrabMotions::Motion_27_AttackLunge;
                return 10;
            }

            field_118_timer = MakeTimer(mAttackDelay);
            return 8;
        }

        case 2:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn || mHealth < FP_FromInteger(0))
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return mBrainSubState;
            }

            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            mNextMotion = eScrabMotions::Motion_1_Stand;
            return 1;

        case 3:
        {
            if (mVelX > FP_FromInteger(0))
            {
                const s16 x_exp = FP_GetExponent(mXPos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_AO(GetSpriteScale(), x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine(0, 1))
                {
                    if (mAbeOrMudTarget->mYPos - mYPos < FP_FromInteger(5)
                        || gMap.VTLV_Get_At_Of_Type(
                            FP_GetExponent(mXPos + kGridSize),
                            FP_GetExponent(mYPos + FP_FromInteger(10)),
                            FP_GetExponent(mXPos + kGridSize),
                            FP_GetExponent(mYPos + FP_FromInteger(10)),
                            ReliveTypes::eScrabNoFall))
                    {
                        if (!Check_IsOnEndOfLine(0, 4))
                        {
                            ToJump();
                            mNextMotion = eScrabMotions::None_m1;
                            return 7;
                        }
                    }
                }

                BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos + (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos + (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);

                auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
                if (pStopper)
                {
                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
                    {
                        if (!SwitchStates_Get(pStopper->mSwitchId))
                        {
                            mNextMotion = eScrabMotions::Motion_1_Stand;
                            return 1;
                        }
                    }
                }
            }
            else if (mVelX < FP_FromInteger(0))
            {
                const s16 x_exp = FP_GetExponent(mXPos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_AO(GetSpriteScale(), x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine(1, 1))
                {
                    if ((mAbeOrMudTarget->mYPos - mYPos < FP_FromInteger(5))
                        || gMap.VTLV_Get_At_Of_Type(
                            FP_GetExponent(mXPos - kGridSize),
                            FP_GetExponent(mYPos + FP_FromInteger(10)),
                            FP_GetExponent(mXPos - kGridSize),
                            FP_GetExponent(mYPos + FP_FromInteger(10)),
                            ReliveTypes::eScrabNoFall))
                    {
                        if (!Check_IsOnEndOfLine(1, 4))
                        {
                            ToJump();
                            mNextMotion = eScrabMotions::None_m1;
                            return 7;
                        }
                    }
                }

                BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos - (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos - (kGridSize * FP_FromInteger(2))),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper);

                auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
                if (pStopper)
                {
                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
                    {
                        if (!SwitchStates_Get(pStopper->mSwitchId))
                        {
                            mNextMotion = eScrabMotions::Motion_1_Stand;
                            return 1;
                        }
                    }
                }
            }

            if (!VIsFacingMe(mAbeOrMudTarget))
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        mXPos,
                        mYPos,
                        0))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                }
                else
                {
                    mCurrentMotion = eScrabMotions::Motion_4_Turn;
                    mNextMotion = eScrabMotions::None_m1;
                    MapFollowMe(true);
                }
                return 2;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(3), mAbeOrMudTarget)
                && mCurrentMotion == eScrabMotions::Motion_3_Run
                && VOnSameYLevel(mAbeOrMudTarget))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), mAbeOrMudTarget->mXPos - mXPos))
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand;
                    return 1;
                }
                mNextMotion = eScrabMotions::Motion_27_AttackLunge;
                return 10;
            }

            if (pLiftPoint && !pLiftPoint->OnAnyFloor())
            {
                mNextMotion = eScrabMotions::Motion_1_Stand;
                return 4;
            }

            if (BaseAliveGameObjectCollisionLine)
            {
                return mBrainSubState;
            }
            return 5;
        }

        case 4:
            if (VIsObjNearby(kGridSize, mAbeOrMudTarget) && VOnSameYLevel(mAbeOrMudTarget))
            {
                mNextMotion = eScrabMotions::Motion_28_LegKick;
                return 10;
            }

            if (pLiftPoint)
            {
                if (!pLiftPoint->OnAnyFloor())
                {
                    return mBrainSubState;
                }
            }
            return 11;

        case 5:
        case 6:
            if (mCurrentMotion != eScrabMotions::Motion_1_Stand)
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::None_m1;
            return 8;

        case 7:
            if (mCurrentMotion != eScrabMotions::Motion_14_RunJumpEnd)
            {
                return mBrainSubState;
            }
            return 11;

        case 8:
        {
            if (!CanSeeAbe(mAbeOrMudTarget)
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand;
                return 1;
            }

            if (!VIsFacingMe(mAbeOrMudTarget))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return 9;
            }

            if (VIsObjNearby(kGridSize * FP_FromDouble(1.5), mAbeOrMudTarget) && VOnSameYLevel(mAbeOrMudTarget))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), mAbeOrMudTarget->mXPos - mXPos))
                {
                    return 1;
                }

                if (!VIsObjNearby(kGridSize, mAbeOrMudTarget))
                {
                    mNextMotion = eScrabMotions::Motion_27_AttackLunge;
                    return 10;
                }

                mNextMotion = eScrabMotions::Motion_28_LegKick;
                return 10;
            }

            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                if (mCurrentMotion != eScrabMotions::Motion_1_Stand || !GetAnimation().GetIsLastFrame())
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_22_Shriek;
                return mBrainSubState;
            }

            BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper);

            auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);

            if (pStopper)
            {
                const bool bLeft = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && mAbeOrMudTarget->mXPos < mXPos;
                const bool bRight = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && mAbeOrMudTarget->mXPos > mXPos;
                const bool bBoth = pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both;
                const bool bSwitchOn = SwitchStates_Get(pStopper->mSwitchId) ? true : false;
                if ((bLeft || bRight || bBoth) && !bSwitchOn)
                {
                    return 1;
                }
            }

            if (GetAnimation().GetFlipX())
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eScrabMotions::Motion_7_HopMidair;
                    return 6;
                }
            }
            else
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eScrabMotions::Motion_7_HopMidair;
                    return 6;
                }
            }

            mNextMotion = eScrabMotions::Motion_3_Run;
            return 3;
        }

        case 9:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn || mHealth < FP_FromInteger(0))
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return mBrainSubState;
            }

            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            mNextMotion = eScrabMotions::Motion_1_Stand;
            return 8;

        case 10:
            if ((mCurrentMotion == eScrabMotions::Motion_27_AttackLunge || mCurrentMotion == eScrabMotions::Motion_28_LegKick) && GetAnimation().GetIsLastFrame())
            {
                if (mAbeOrMudTarget->mHealth <= FP_FromInteger(0))
                {
                    if (!VIsFacingMe(mAbeOrMudTarget))
                    {
                        mNextMotion = eScrabMotions::Motion_4_Turn;
                        return 14;
                    }

                    if (!VIsObjNearby(kGridSize, mAbeOrMudTarget))
                    {
                        mNextMotion = eScrabMotions::Motion_2_Walk;
                        return 13;
                    }

                    field_188_flags &= ~4u;
                    mNextMotion = eScrabMotions::Motion_16_Stamp;
                    field_118_timer = MakeTimer(30);
                    return 15;
                }

                if (!VIsFacingMe(mAbeOrMudTarget))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 12;
                }
                return HandleRunning();
            }
            return mBrainSubState;

        case 11:
            if (!CanSeeAbe(mAbeOrMudTarget)
                && gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand;
                SetBrain(&Scrab::Brain_WalkAround);
                return 0;
            }

            if (mAbeOrMudTarget->mHealth <= FP_FromInteger(0))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand;
                return 1;
            }

            if (!VIsFacingMe(mAbeOrMudTarget))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return 12;
            }

            if (VIsObjNearby(kGridSize * FP_FromDouble(1.5), mAbeOrMudTarget))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(30), mAbeOrMudTarget->mXPos - mXPos))
                {
                    return 1;
                }

                if (VIsObjNearby(kGridSize, mAbeOrMudTarget))
                {
                    mNextMotion = eScrabMotions::Motion_28_LegKick;
                    return 10;
                }

                mNextMotion = eScrabMotions::Motion_27_AttackLunge;
                return 10;
            }

            return HandleRunning();


        case 12:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn || mHealth < FP_FromInteger(0))
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return mBrainSubState;
            }

            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            mNextMotion = eScrabMotions::Motion_1_Stand;
            return 11;

        case 13:
            if (VIsObjNearby(kGridSize, mAbeOrMudTarget))
            {
                field_188_flags &= ~4u;
                mNextMotion = eScrabMotions::Motion_16_Stamp;
                field_118_timer = MakeTimer(30);
                return 15;
            }
            return mBrainSubState;

        case 14:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn)
            {
                if (mNextMotion == eScrabMotions::Motion_4_Turn || mHealth < FP_FromInteger(0))
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return mBrainSubState;
            }

            if (!GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (!VIsObjNearby(kGridSize, mAbeOrMudTarget))
            {
                mNextMotion = eScrabMotions::Motion_2_Walk;
                return 13;
            }
            field_188_flags &= ~4u;
            mNextMotion = eScrabMotions::Motion_16_Stamp;
            field_118_timer = MakeTimer(30);
            return 15;

        case 15:
            if (field_118_timer <= static_cast<s32>(sGnFrame))
            {
                if (FP_Abs(mAbeOrMudTarget->mYPos - mYPos) >= FP_FromInteger(5))
                {
                    mNextMotion = eScrabMotions::Motion_22_Shriek;
                    return 17;
                }

                mNextMotion = eScrabMotions::Motion_25_ToFeed;
                return 16;
            }
            return mBrainSubState;

        case 16:
            if (mCurrentMotion != eScrabMotions::Motion_24_FeedToGulp || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_25_ToFeed;
            return mBrainSubState;

        case 17:
        {
            if (mCurrentMotion != eScrabMotions::Motion_22_Shriek || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            mNextMotion = eScrabMotions::Motion_1_Stand;

            if (pLiftPoint)
            {
                if (!pLiftPoint->OnAnyFloor())
                {
                    return 4;
                }
            }
            return 1;
        }

        case 18:
            if (EventGet(kEventAbeOhm))
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand;
            return 1;

        default:
            return mBrainSubState;
    }
}

s16 Scrab::Brain_Patrol()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    auto pFightTarget = FindScrabToFight();
    if (pFightTarget)
    {
        SetFightTarget(pFightTarget);
        mNextMotion = eScrabMotions::Motion_1_Stand;
        SetBrain(&Scrab::Brain_Fighting);
        return 0;
    }

    if (FindAbeOrMud())
    {
        if (CanSeeAbe(mAbeOrMudTarget))
        {
            mNextMotion = eScrabMotions::Motion_1_Stand;
            SetBrain(&Scrab::Brain_ChasingEnemy);
            return 0;
        }
        SetTarget(nullptr);
    }

    if (EventGet(kEventAbeOhm))
    {
        mNextMotion = eScrabMotions::Motion_20_HowlBegin;
        return 9;
    }

    switch (mBrainSubState)
    {
        case 0:
        {
            if (gNumCamSwappers > 0)
            {
                return mBrainSubState;
            }

            LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
            if (pLiftPoint)
            {
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
                {
                    if (!pLiftPoint->OnAnyFloor())
                    {
                        SetBrain(&Scrab::Brain_WalkAround);
                        return 0;
                    }
                }
            }

            auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper));
            BaseAliveGameObjectPathTLV = pStopper;

            if (pStopper)
            {
                if (!SwitchStates_Get(pStopper->mSwitchId))
                {
                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && !GetAnimation().GetFlipX())
                    {
                        return 0;
                    }

                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && GetAnimation().GetFlipX())
                    {
                        return 0;
                    }

                    if (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
                    {
                        return 0;
                    }
                }
            }

            mNextMotion = GetMotionForPatrolType(mPatrolType);

            if (GetAnimation().GetFlipX())
            {
                BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eScrabLeftBound);
                if (BaseAliveGameObjectPathTLV)
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 2;
                }
                return 1;
            }
            else
            {
                BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eScrabRightBound);
                if (BaseAliveGameObjectPathTLV)
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 5;
                }
                return 4;
            }
            break;
        }

        case 1:
        {
            BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLift)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand;
                    return 8;
                }
            }

            if (gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos - ScaleToGridSize(GetSpriteScale())),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos - ScaleToGridSize(GetSpriteScale())),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eScrabLeftBound))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return 2;
            }
            return mBrainSubState;
        }

        case 2:
        {
            BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLift)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand;
                    return 8;
                }
            }

            if (mCurrentMotion == eScrabMotions::Motion_4_Turn && GetAnimation().GetIsLastFrame())
            {
                mNextMotion = eScrabMotions::Motion_1_Stand;
                field_118_timer = MakeTimer(Math_RandomRange(mPauseLeftMin, mPauseLeftMax));
                return 3;
            }
            return mBrainSubState;
        }

        case 3:
        {
            BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLift)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand;
                    return 8;
                }
            }

            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            if (Math_NextRandom() < 30u)
            {
                mNextMotion = eScrabMotions::Motion_20_HowlBegin;
                field_118_timer = MakeTimer(30);
                return 7;
            }

            mNextMotion = GetMotionForPatrolType(mPatrolType);
            return 4;
        }

        case 4:
        {
            BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLift)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand;
                    return 8;
                }
            }

            if (gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(ScaleToGridSize(GetSpriteScale()) + mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(ScaleToGridSize(GetSpriteScale()) + mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eScrabRightBound))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return 5;
            }
            return mBrainSubState;
        }

        case 5:
        {
            BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLift)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand;
                    return 8;
                }
            }

            if (mCurrentMotion == eScrabMotions::Motion_4_Turn && GetAnimation().GetIsLastFrame())
            {
                mNextMotion = eScrabMotions::Motion_1_Stand;
                field_118_timer = MakeTimer(Math_RandomRange(mPauseRightMin, mPauseRightMax));
                return 6;
            }
            return mBrainSubState;
        }

        case 6:
        {
            BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLift)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand;
                    return 8;
                }
            }

            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            if (Math_NextRandom() < 30u)
            {
                mNextMotion = eScrabMotions::Motion_20_HowlBegin;
                field_118_timer = MakeTimer(30);
                return 7;
            }

            mNextMotion = GetMotionForPatrolType(mPatrolType);
            return 1;
        }

        case 7:
        {
            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            mNextMotion = GetMotionForPatrolType(mPatrolType);

            BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLift)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand;
                    return 8;
                }
            }

            if (GetAnimation().GetFlipX())
            {
                return 1;
            }
            return 4;
        }

        case 8:
        {
            BaseGameObject* pObj = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pObj)
            {
                auto pLift = static_cast<LiftPoint*>(pObj);
                if (!pLift->OnAnyFloor())
                {
                    return mBrainSubState;
                }
            }
            return 0;
        }

        case 9:
            if (!EventGet(kEventAbeOhm))
            {
                mNextMotion = eScrabMotions::Motion_1_Stand;
                return 0;
            }
            return mBrainSubState;

        default:
            return mBrainSubState;
    }
}

s16 Scrab::Brain_WalkAround()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    auto pFightTarget = FindScrabToFight();
    if (pFightTarget)
    {
        SetFightTarget(pFightTarget);
        SetBrain(&Scrab::Brain_Fighting);
        mNextMotion = eScrabMotions::Motion_1_Stand;
        return 0;
    }

    if (FindAbeOrMud())
    {
        if (CanSeeAbe(mAbeOrMudTarget))
        {
            mNextMotion = eScrabMotions::Motion_1_Stand;
            SetBrain(&Scrab::Brain_ChasingEnemy);
            return 0;
        }
        SetTarget(nullptr);
    }

    if (EventGet(kEventAbeOhm))
    {
        mNextMotion = eScrabMotions::Motion_20_HowlBegin;
        return 6;
    }

    switch (mBrainSubState)
    {
        case 0:
            if (gNumCamSwappers > 0)
            {
                return mBrainSubState;
            }
            field_12C = mXPos;
            return 1;

        case 1:
        {
            BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLift)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
                {
                    if (!pLiftPoint->OnAnyFloor())
                    {
                        return 1;
                    }
                }
            }

            if (field_118_timer > static_cast<s32>(sGnFrame))
            {
                if (Math_NextRandom() >= 8u || !(field_188_flags & 0x20) || sGnFrame - field_140_last_shriek_timer <= 90)
                {
                    return mBrainSubState;
                }
                mNextMotion = eScrabMotions::Motion_22_Shriek;
                field_140_last_shriek_timer = sGnFrame;
                return 5;
            }

            if (GetAnimation().GetFlipX())
            {
                auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper));

                BaseAliveGameObjectPathTLV = pStopper;

                if (pStopper)
                {
                    if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && !SwitchStates_Get(pStopper->mSwitchId))
                    {
                        mNextMotion = eScrabMotions::Motion_4_Turn;
                        return 4;
                    }
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(30), -ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(1.5)) || Check_IsOnEndOfLine(1, 2))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }
                mNextMotion = eScrabMotions::Motion_2_Walk;
                return 2;
            }
            else
            {
                auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.VTLV_Get_At_Of_Type(
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    FP_GetExponent(mXPos),
                    FP_GetExponent(mYPos),
                    ReliveTypes::eEnemyStopper));

                BaseAliveGameObjectPathTLV = pStopper;

                if (pStopper)
                {
                    if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && !SwitchStates_Get(pStopper->mSwitchId))
                    {
                        mNextMotion = eScrabMotions::Motion_4_Turn;
                        return 4;
                    }
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(30), ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(1.5))
                    || Check_IsOnEndOfLine(0, 2))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }
                mNextMotion = eScrabMotions::Motion_2_Walk;
                return 3;
            }
            break;
        }

        case 2:
        {
            auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper));

            BaseAliveGameObjectPathTLV = pStopper;

            if (pStopper)
            {
                if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && !SwitchStates_Get(pStopper->mSwitchId))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }
            }

            if (field_12C - mXPos > FP_FromInteger(368))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return 4;
            }

            if (WallHit(GetSpriteScale() * FP_FromInteger(30), -ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(1.5))
                || Check_IsOnEndOfLine(1, 2))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return 4;
            }

            if (Math_NextRandom() >= 8u || !(field_188_flags & 0x20) || sGnFrame - field_140_last_shriek_timer <= 90)
            {
                BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
                if (pLift)
                {
                    auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                    if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
                    {
                        if (!pLiftPoint->OnAnyFloor())
                        {
                            mNextMotion = eScrabMotions::Motion_1_Stand;
                            return 1;
                        }
                    }
                }
                return mBrainSubState;
            }

            field_140_last_shriek_timer = sGnFrame;
            mNextMotion = eScrabMotions::Motion_22_Shriek;
            return 5;
        }

        case 3:
        {
            auto pStopper = static_cast<relive::Path_EnemyStopper*>(gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper));

            BaseAliveGameObjectPathTLV = pStopper;

            if (pStopper)
            {
                if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both) && !SwitchStates_Get(pStopper->mSwitchId))
                {
                    mNextMotion = eScrabMotions::Motion_4_Turn;
                    return 4;
                }
            }

            if (mXPos - field_12C > FP_FromInteger(368)
                || WallHit(GetSpriteScale() * FP_FromInteger(30), ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(1.5))
                || Check_IsOnEndOfLine(0, 2))
            {
                mNextMotion = eScrabMotions::Motion_4_Turn;
                return 4;
            }

            if (Math_NextRandom() >= 8u || !(field_188_flags & 0x20) || sGnFrame - field_140_last_shriek_timer <= 90)
            {
                BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
                if (pLift)
                {
                    auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                    if (pLiftPoint->Type() == ReliveTypes::eLiftPoint)
                    {
                        if (!pLiftPoint->OnAnyFloor())
                        {
                            mNextMotion = eScrabMotions::Motion_1_Stand;
                            return 1;
                        }
                    }
                }
                return mBrainSubState;
            }

            field_140_last_shriek_timer = sGnFrame;
            mNextMotion = eScrabMotions::Motion_22_Shriek;
            return 5;
        }

        case 4:
            if (mCurrentMotion != eScrabMotions::Motion_4_Turn || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (GetAnimation().GetFlipX())
            {
                field_118_timer = MakeTimer(Math_RandomRange(mPauseLeftMin, mPauseLeftMax));
            }
            else
            {
                field_118_timer = MakeTimer(Math_RandomRange(mPauseRightMin, mPauseRightMax));
            }

            return 1;

        case 5:
        {
            BaseGameObject* pLift = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
            if (pLift)
            {
                auto pLiftPoint = static_cast<LiftPoint*>(pLift);
                if (pLiftPoint->Type() == ReliveTypes::eLiftPoint && !pLiftPoint->OnAnyFloor())
                {
                    mNextMotion = eScrabMotions::Motion_1_Stand;
                    return 1;
                }
            }

            if (mCurrentMotion != eScrabMotions::Motion_22_Shriek || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand;
            return 1;
        }

        case 6:
            if (EventGet(kEventAbeOhm))
            {
                return mBrainSubState;
            }
            mNextMotion = eScrabMotions::Motion_1_Stand;
            return 1;

        default:
            return mBrainSubState;
    }
}

void Scrab::SetBrain(TBrainType fn)
{
    mBrainState = fn;
}

bool Scrab::BrainIs(TBrainType fn)
{
    return mBrainState == fn;
}

void Scrab::SetFightTarget(Scrab* pTarget)
{
    if (!pTarget)
    {
        if (mScrabTarget)
        {
            mScrabTarget->mBaseGameObjectRefCount--;
            //LOG_INFO(this << " clear fight target " << mScrabTarget << " ref " << (u32) mScrabTarget->mBaseGameObjectRefCount);
            mScrabTarget = nullptr;
        }
    }
    else
    {
        mScrabTarget = pTarget;
        mScrabTarget->mBaseGameObjectRefCount++;
        //LOG_INFO(this << " set fight target " << mScrabTarget << " ref " << (u32) mScrabTarget->mBaseGameObjectRefCount);
    }
}

void Scrab::SetTarget(BaseAliveGameObject* pTarget)
{
    if (!pTarget)
    {
        if (mAbeOrMudTarget)
        {
            mAbeOrMudTarget->mBaseGameObjectRefCount--;
            //LOG_INFO(this << " clear target " << mAbeOrMudTarget << " ref " << (u32) mAbeOrMudTarget->mBaseGameObjectRefCount);
            mAbeOrMudTarget = nullptr;
        }
    }
    else
    {
        if (mAbeOrMudTarget != pTarget)
        {
            mAbeOrMudTarget = pTarget;
            mAbeOrMudTarget->mBaseGameObjectRefCount++;
            //LOG_INFO(this << " set target " << mAbeOrMudTarget << " ref " << (u32) mAbeOrMudTarget->mBaseGameObjectRefCount);
        }
        else
        {
            // Don't double ref count else the target will leak, this can be seen as abe not reappearing
            // in RF return after the bad ending, but other bad things probably happen too.
            LOG_INFO("Trying to set the same target - ignore");

        }
    }
}

s16 Scrab::HandleRunning()
{
    BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::eEnemyStopper);
    auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);

    if (pStopper)
    {
        if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && mAbeOrMudTarget->mXPos < mXPos) || (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && mAbeOrMudTarget->mXPos > mXPos) || pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Both)
        {
            if (!SwitchStates_Get(pStopper->mSwitchId))
            {
                return 1;
            }
        }
    }

    if (!GetAnimation().GetFlipX())
    {
        if (Check_IsOnEndOfLine(0, 1))
        {
            mNextMotion = eScrabMotions::Motion_7_HopMidair;
            return 6;
        }
    }
    else
    {
        if (Check_IsOnEndOfLine(1, 1))
        {
            mNextMotion = eScrabMotions::Motion_7_HopMidair;
            return 6;
        }
    }

    mNextMotion = eScrabMotions::Motion_3_Run;
    return 3;
}

eScrabMotions Scrab::GetMotionForPatrolType(relive::Path_Scrab::ScrabPatrolType ScrabPatrolType)
{
    switch (ScrabPatrolType)
    {
        case relive::Path_Scrab::ScrabPatrolType::eRunOrWalk192:
            if (Math_NextRandom() < 192u)
            {
                return eScrabMotions::Motion_2_Walk;
            }
            else
            {
                return eScrabMotions::Motion_3_Run;
            }
            break;

        case relive::Path_Scrab::ScrabPatrolType::eRunOrWalk128:
            if (Math_NextRandom() < 128u)
            {
                return eScrabMotions::Motion_2_Walk;
            }
            else
            {
                return eScrabMotions::Motion_3_Run;
            }
            break;

        case relive::Path_Scrab::ScrabPatrolType::eRunOrWalk64:
            if (Math_NextRandom() < 64u)
            {
                return eScrabMotions::Motion_2_Walk;
            }
            else
            {
                return eScrabMotions::Motion_3_Run;
            }
            break;

        case relive::Path_Scrab::ScrabPatrolType::eRun:
            return eScrabMotions::Motion_3_Run;

        default:
            return eScrabMotions::Motion_2_Walk;
    }
}

} // namespace AO
