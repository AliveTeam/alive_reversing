#include "stdafx_ao.h"
#include "Function.hpp"
#include "Paramite.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "ParamiteWeb.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "PlatformBase.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "Meat.hpp"
#include "CameraSwapper.hpp"
#include "PlatformBase.hpp"
#include "LiftPoint.hpp"
#include "SwitchStates.hpp"
#include "Grid.hpp"
#include "BeeSwarm.hpp"

void Paramite_ForceLink()
{ }

namespace AO {

using TParamiteMotionFunction = decltype(&Paramite::Motion_0_Idle);

const TParamiteMotionFunction sParamiteMotionTable[] = {
    &Paramite::Motion_0_Idle,
    &Paramite::Motion_1_WalkBegin,
    &Paramite::Motion_2_Walking,
    &Paramite::Motion_3_Running,
    &Paramite::Motion_4_Unknown,
    &Paramite::Motion_5_Turn,
    &Paramite::Motion_6_Hop,
    &Paramite::Motion_7_Unknown,
    &Paramite::Motion_8_WalkRunTransition,
    &Paramite::Motion_9_WalkEnd,
    &Paramite::Motion_10_RunBegin,
    &Paramite::Motion_11_RunEnd,
    &Paramite::Motion_12_Falling,
    &Paramite::Motion_13_GameSpeakBegin,
    &Paramite::Motion_14_PreHiss,
    &Paramite::Motion_15_Hiss,
    &Paramite::Motion_16_PostHiss,
    &Paramite::Motion_17_GameSpeakEnd,
    &Paramite::Motion_18_RunningAttack,
    &Paramite::Motion_19_Empty,
    &Paramite::Motion_20_SurpriseWeb,
    &Paramite::Motion_21_WebLeaveDown,
    &Paramite::Motion_22_Unknown,
    &Paramite::Motion_23_Eating,
    &Paramite::Motion_24_Struggle,
    &Paramite::Motion_25_Death,
};

const static AnimId sParamiteMotionAnimIds[] = {
    AnimId::Paramite_Idle,
    AnimId::Paramite_WalkBegin,
    AnimId::Paramite_Walking,
    AnimId::Paramite_Running,
    AnimId::Paramite_Idle,
    AnimId::Paramite_Turn,
    AnimId::Paramite_Hop,
    AnimId::Paramite_Hop,// HACK fix unused not converting // AnimId::Paramite_AO_M_7_Unknown,
    AnimId::Paramite_WalkRunTransition,
    AnimId::Paramite_WalkEnd,
    AnimId::Paramite_RunBegin,
    AnimId::Paramite_RunEnd,
    AnimId::Paramite_Falling,
    AnimId::Paramite_GameSpeakBegin,
    AnimId::Paramite_PreHiss,
    AnimId::Paramite_Hiss,
    AnimId::Paramite_PostHiss,
    AnimId::Paramite_GameSpeakEnd,
    AnimId::Paramite_RunningAttack,
    AnimId::Paramite_SurpriseWeb,
    AnimId::Paramite_SurpriseWeb,
    AnimId::Paramite_WebLeaveDown,
    AnimId::Paramite_AO_M_22_Unknown,
    AnimId::Paramite_Eating,
    AnimId::Paramite_Struggle,
    AnimId::Paramite_Death};

Paramite::Paramite(relive::Path_Paramite* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eParamite);

    for (auto& animId : sParamiteMotionAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ParamiteWeb));

    Animation_Init(GetAnimRes(AnimId::Paramite_Idle));

    field_12A_res_idx = 0;

    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit4_SetOffExplosives);
    field_114_timer = 0;
    mBrainSubState = 0;
    SetNextMotion(eParamiteMotions::Motion_0_Idle);
    field_EC = 3;
    mLiftPoint = nullptr;
    SetCurrentMotion(eParamiteMotions::Motion_0_Idle);
    field_140_use_prev_motion = 0;

    mMeat = nullptr;

    mParamiteWeb = nullptr;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_8;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_27;
        mScale = Scale::Fg;
    }

    if (pTlv->mEntranceType == relive::Path_Paramite::EntranceType::eSurpriseWeb)
    {
        SetBrain(&Paramite::Brain_1_SurpriseWeb);
    }
    else
    {
        SetBrain(&Paramite::Brain_0_Patrol);
    }

    mAloneChaseDelay = pTlv->mAloneChaseDelay;
    mSurpriseWebDelayTimer = pTlv->mSurpriseWebDelayTimer;
    mMeatEatingTime = pTlv->mMeatEatingTime;
    mGroupChaseDelay = pTlv->mGroupChaseDelay;
    mSurpriseWebSwitchId = pTlv->mSurpriseWebSwitchId;
    mHissBeforeAttack = pTlv->mHissBeforeAttack;
    mDeleteWhenOutOfSight = pTlv->mDeleteWhenOutOfSight;

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
            mSpriteScale != FP_FromInteger(0) ? kFgFloor : kBgFloor)
        == 1)
    {
        SetCurrentMotion(eParamiteMotions::Motion_0_Idle);
        mYPos = hitY;
    }

    mRGB.SetRGB(105, 105, 105);

    field_12C_tlvInfo = tlvId;

    if (!VIsFacingMe(sActiveHero))
    {
        mAnim.mFlags.Toggle(AnimFlags::eFlipX);
    }

    VStackOnObjectsOfType(ReliveTypes::eParamite);

    mBaseAliveGameObjectFlags.Set(Flags_10A::e10A_Bit6);

    mShadow = relive_new Shadow();
}

Paramite::~Paramite()
{
    if (mParamiteWeb)
    {
        mParamiteWeb->mBaseGameObjectFlags.Set(Options::eDead);
        mParamiteWeb->mBaseGameObjectRefCount--;
        mParamiteWeb = nullptr;
    }

    if (mMeat)
    {
        mMeat->mBaseGameObjectRefCount--;
    }

    VOnTrapDoorOpen();

    if (mHealth <= FP_FromInteger(0))
    {
        Path::TLV_Reset(field_12C_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_12C_tlvInfo, -1, 0, 0);
    }

    SND_Seq_Stop_477A60(SeqId::eParamiteNearby_30);

    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
    MusicController::ClearObject(this);
}

void Paramite::VRender(PrimHeader** ppOt)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

s16 Paramite::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return 1;
    }

    if (mMeat)
    {
        mMeat->mBaseGameObjectRefCount--;
        mMeat = nullptr;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eBaseBomb:
        case ReliveTypes::eExplosion:
        {
            relive_new Gibs(
                GibType::Slog_2,
                mXPos,
                mYPos,
                mVelX,
                mVelY,
                mSpriteScale);

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mAnim.mFlags.Clear(AnimFlags::eRender);
            mHealth = FP_FromInteger(0);
            return 1;
        }

        case ReliveTypes::eAbilityRing:
            return 0;

        case ReliveTypes::eBeeSwarm:
            mHealth -= FP_FromDouble(0.2);
            if (mHealth > FP_FromInteger(0))
            {
                if (!BrainIs(&Paramite::Brain_2_Struggling))
                {
                    SetNextMotion(eParamiteMotions::Motion_25_Death);
                    SetBrain(&Paramite::Brain_2_Struggling);
                    mBrainSubState = 0;
                }
            }
            else
            {
                mHealth = FP_FromInteger(0);
                SetBrain(&Paramite::Brain_3_Death);
                field_114_timer = sGnFrame + 90;
                SetCurrentMotion(eParamiteMotions::Motion_24_Struggle);
                VUpdateAnimData();
            }
            break;

        default:
        {
            mHealth = FP_FromInteger(0);
            SetBrain(&Paramite::Brain_3_Death);
            field_114_timer = sGnFrame + 90;
            SetCurrentMotion(eParamiteMotions::Motion_24_Struggle);
            VUpdateAnimData();

            relive_new Blood(
                mXPos,
                mYPos,
                FP_FromInteger(0),
                FP_FromInteger(5),
                mSpriteScale,
                50);
            break;
        }
    }
    return 1;
}

void Paramite::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            mHealth = FP_FromInteger(0);
        }
        pTlv = gMap.TLV_Get_At_446060(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

void Paramite::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId()
        || gMap.mCurrentPath != gMap.mNextPath
        || gMap.mCurrentLevel != gMap.mNextLevel)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 Paramite::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return FP_Abs(pOther->mYPos - mYPos) < (mSpriteScale * FP_FromInteger(40));
}

void Paramite::VOnTrapDoorOpen()
{
    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
        SetCurrentMotion(eParamiteMotions::Motion_12_Falling);
    }
}

void Paramite::VUpdate()
{
    if (mHealth > FP_FromInteger(0)
        && gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        SND_SEQ_PlaySeq_4775A0(SeqId::eParamiteNearby_30, 1, 0);
    }
    else
    {
        bool bFound = false;
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->Type() == ReliveTypes::eParamite && pObjIter != this)
            {
                Paramite* pOther = static_cast<Paramite*>(pObjIter);
                if (gMap.Is_Point_In_Current_Camera(
                        pOther->mCurrentLevel,
                        pOther->mCurrentPath,
                        pOther->mXPos,
                        pOther->mYPos,
                        0))
                {
                    bFound = true;
                    break;
                }
            }
        }

        if (!bFound)
        {
            SND_Seq_Stop_477A60(SeqId::eParamiteNearby_30);
        }
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (FP_Abs(mXPos - sActiveHero->mXPos) > FP_FromInteger(1536) || FP_Abs(mYPos - sActiveHero->mYPos) > FP_FromInteger(480))
    {
        if (mDeleteWhenOutOfSight == relive::reliveChoice::eYes)
        {
            mBaseGameObjectFlags.Set(Options::eDead);
        }
        else
        {
            if (mMeat)
            {
                if (mMeat->VIsFalling() || mMeat->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    mMeat->mBaseGameObjectRefCount--;
                    mMeat = nullptr;
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    SetBrain(&Paramite::Brain_0_Patrol);
                    mBrainSubState = 0;
                }
            }
        }

        mAnim.mFlags.Clear(AnimFlags::eAnimate);
        mAnim.mFlags.Clear(AnimFlags::eRender);

        // TODO: OG freed vram here
    }
    else
    {
        // TODO: OG alloc'd vram here

        mAnim.mFlags.Set(AnimFlags::eAnimate);
        mAnim.mFlags.Set(AnimFlags::eRender);


        const auto oldMotion = mCurrentMotion;
        mBrainSubState = (this->*mBrainState)();

        static auto oldBrain = mBrainState;
        if (mBrainState != oldBrain)
        {
            //LOG_INFO("Brain changed from " << GetOriginalFn(oldBrain, sParamiteBrainTable).fnName << " to " << GetOriginalFn(mBrainState, sParamiteBrainTable).fnName);
        }
        oldBrain = mBrainState;

        if (showDebugCreatureInfo_5076E0)
        {
            DDCheat::DebugStr(
                "Paramite %d %d %d %d\n",
                mBrainSubState,
                field_114_timer,
                mCurrentMotion,
                mNextMotion);
        }

        const FP oldx = mXPos;
        const FP oldy = mYPos;

        (this->*sParamiteMotionTable[mCurrentMotion])();

        if (oldx != mXPos || oldy != mYPos)
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(
                nullptr,
                mXPos,
                mYPos,
                mXPos,
                mYPos);

            VOnTlvCollision(BaseAliveGameObjectPathTLV);
        }

        if (field_106_shot)
        {
            ToKnockBack();
            SetNextMotion(-1);
            SetCurrentMotion(mNextMotion);
            field_106_shot = 0;
        }

        if (oldMotion == mCurrentMotion)
        {
            if (field_140_use_prev_motion)
            {
                SetCurrentMotion(mPreviousMotion);
                VUpdateAnimData();
                mAnim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
                field_140_use_prev_motion = 0;
            }
        }
        else
        {
            VUpdateAnimData();
            if (static_cast<eParamiteMotions>(oldMotion) == eParamiteMotions::Motion_4_Unknown)
            {
                mAnim.SetFrame(mBaseAliveGameObjectLastAnimFrame);
            }
        }
    }
}


void Paramite::ToIdle()
{
    mAnim.mFlags.Clear(AnimFlags::eFlipY);
    mAnim.mFlags.Clear(AnimFlags::eSwapXY);

    field_124_XSpeed = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    SetCurrentMotion(eParamiteMotions::Motion_0_Idle);

    MapFollowMe_401D30(TRUE);
}

s16 Paramite::ToNextMotion()
{
    switch (GetNextMotion())
    {
        case eParamiteMotions::Motion_5_Turn:
        case eParamiteMotions::Motion_23_Eating:
        case eParamiteMotions::Motion_25_Death:
        case eParamiteMotions::Motion_13_GameSpeakBegin:
            SetCurrentMotion(mNextMotion);
            SetNextMotion(-1);
            return 1;

        case eParamiteMotions::Motion_3_Running:
            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromDouble(3.5));
                SetCurrentMotion(eParamiteMotions::Motion_10_RunBegin);
            }
            else
            {
                mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromDouble(3.5));
                SetCurrentMotion(eParamiteMotions::Motion_10_RunBegin);
            }
            SetNextMotion(-1);
            return 1;

        case eParamiteMotions::Motion_2_Walking:
            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -ScaleToGridSize(mSpriteScale) / FP_FromInteger(7);
                SetCurrentMotion(eParamiteMotions::Motion_1_WalkBegin);
            }
            else
            {
                mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(7));
                SetCurrentMotion(eParamiteMotions::Motion_1_WalkBegin);
            }
            SetNextMotion(-1);
            return 1;

        case eParamiteMotions::Motion_15_Hiss:
            SetCurrentMotion(eParamiteMotions::Motion_13_GameSpeakBegin);
            return 1;

        case eParamiteMotions::Motion_18_RunningAttack:
            SetCurrentMotion(eParamiteMotions::Motion_18_RunningAttack);
            SetNextMotion(-1);
            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
            }
            return 1;

        case eParamiteMotions::Motion_6_Hop:
            VOnTrapDoorOpen();
            SetCurrentMotion(eParamiteMotions::Motion_6_Hop);
            SetNextMotion(-1);
            BaseAliveGameObjectLastLineYPos = mYPos;
            mVelY = FP_FromDouble(-7.2) * mSpriteScale;
            mYPos += mVelY;
            BaseAliveGameObjectCollisionLine = nullptr;
            return 1;

        default:
            return 0;
    }
}

void Paramite::VUpdateAnimData()
{
    mAnim.Set_Animation_Data(GetAnimRes(sParamiteMotionAnimIds[mCurrentMotion]));
}

s16 Paramite::AnotherParamiteNear()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->Type() == ReliveTypes::eParamite && pObjIter != this)
        {
            Paramite* pOther = static_cast<Paramite*>(pObjIter);
            if (gMap.Is_Point_In_Current_Camera(
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
                if (FP_Abs(pOther->mYPos - mYPos) < (mSpriteScale * FP_FromInteger(100)))
                {
                    if (pOther->BrainIs(&Paramite::Brain_0_Patrol) || pOther->BrainIs(&Paramite::Brain_4_ChasingAbe))
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}


void Paramite::ToKnockBack()
{
    const FP nextX = mXPos - mVelX;
    mXPos = FP_FromInteger((FP_GetExponent(nextX) & 0xFC00) + SnapToXGrid(mSpriteScale, FP_GetExponent(nextX) & 0x3FF));

    MapFollowMe_401D30(1);

    if (mVelY < FP_FromInteger(0))
    {
        mVelY = FP_FromInteger(0);
    }

    if (BaseAliveGameObjectCollisionLine)
    {
        ToIdle();
    }
    else
    {
        SetCurrentMotion(eParamiteMotions::Motion_12_Falling);
    }
}

Meat* Paramite::FindMeat()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->Type() == ReliveTypes::eMeat)
        {
            auto pMeat = static_cast<Meat*>(pObjIter);
            if (pMeat->VCanEatMe())
            {
                if (gMap.Is_Point_In_Current_Camera(
                        pMeat->mCurrentLevel,
                        pMeat->mCurrentPath,
                        pMeat->mXPos,
                        pMeat->mYPos,
                        0)
                    && !WallHit_401930(mYPos, pMeat->mXPos - mXPos))
                {
                    if (!pMeat->field_124_pLine)
                    {
                        return pMeat;
                    }

                    if (FP_Abs(pMeat->mYPos - mYPos) < (mSpriteScale * FP_FromInteger(100)))
                    {
                        return pMeat;
                    }
                }
            }
        }
    }
    return nullptr;
}

void Paramite::MoveOnLine()
{
    const FP old_x = mXPos;
    if (BaseAliveGameObjectCollisionLine)
    {
        BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
            &mXPos,
            &mYPos,
            mVelX);
        if (BaseAliveGameObjectCollisionLine)
        {
            if (mLiftPoint)
            {
                if (BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eDynamicCollision_32 && BaseAliveGameObjectCollisionLine->mLineType != eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const auto oldMotion = mCurrentMotion;
                    VOnTrapDoorOpen();
                    SetCurrentMotion(oldMotion);
                }
            }
            else
            {
                if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    const PSX_RECT bRect = VGetBoundingRect();

                    VOnCollisionWith(
                        {bRect.x, static_cast<s16>(bRect.y + 5)},
                        {bRect.w, static_cast<s16>(bRect.h + 5)},
                        ObjListPlatforms_50766C,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                }
            }
        }
        else
        {
            VOnTrapDoorOpen();
            BaseAliveGameObjectLastLineYPos = mYPos;
            SetCurrentMotion(eParamiteMotions::Motion_12_Falling);
            mXPos = mVelX + old_x;
        }
    }
    else
    {
        SetCurrentMotion(eParamiteMotions::Motion_12_Falling);
        BaseAliveGameObjectLastLineYPos = mYPos;
    }
}

const relive::SfxDefinition stru_4CDD98[9] = {
    {0, 37, 60, 90, -520, -520},
    {0, 37, 61, 75, -520, -520},
    {0, 37, 62, 50, -520, -520},
    {0, 37, 64, 40, -520, -520},
    {0, 37, 65, 40, -520, -520},
    {0, 37, 66, 30, -520, -520},
    {0, 37, 66, 50, 0, 0},
    {0, 37, 67, 40, -520, -520},
    {0, 37, 68, 30, -520, -520}};



void Paramite::Sound(ParamiteSpeak idx)
{
    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    s16 volRight = stru_4CDD98[static_cast<s16>(idx)].field_C_default_volume;
    s16 volLeft = 0;

    PSX_RECT rect = {};
    gMap.Get_Camera_World_Rect(direction, &rect);

    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
            volLeft = volRight;
            break;

        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            const FP tempVol = (FP_FromRaw(stru_4CDD98[static_cast<s16>(idx)].field_C_default_volume) / FP_FromInteger(3));
            volLeft = FP_GetExponent(tempVol);
            volRight = FP_GetExponent(tempVol);
            break;
        }

        case CameraPos::eCamLeft_3:
        {
            const FP numScreensAway = ((FP_FromInteger(rect.w) - mXPos) / FP_FromInteger(640));
            volLeft = volRight - FP_GetExponent(numScreensAway * FP_FromInteger(volRight - (volRight / 3)));
            volRight -= FP_GetExponent((numScreensAway * FP_FromInteger(volRight)));
            break;
        }

        case CameraPos::eCamRight_4:
        {
            const FP numScreensAway = ((mXPos - FP_FromInteger(rect.x)) / FP_FromInteger(640));
            volLeft = volRight - FP_GetExponent(numScreensAway * FP_FromInteger(volRight));
            volRight -= FP_GetExponent((numScreensAway * FP_FromInteger(volRight - (volRight / 3))));
            break;
        }

        default:
            return;
    }

    SFX_SfxDefinition_Play_477330(stru_4CDD98[static_cast<s16>(idx)], volLeft, volRight, -520, -520);
}

void Paramite::SetMusic()
{
    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        >= CameraPos::eCamCurrent_0)
    {
        if (BrainIs(&Paramite::Brain_4_ChasingAbe))
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
        }
        else
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

void Paramite::ToHop()
{
    VOnTrapDoorOpen();

    SetCurrentMotion(eParamiteMotions::Motion_6_Hop);

    SetNextMotion(-1);

    BaseAliveGameObjectLastLineYPos = mYPos;
    mVelY = -FP_FromDouble(7.2) * mSpriteScale;
    mYPos += mVelY;
    BaseAliveGameObjectCollisionLine = nullptr;
}
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

    eBrain0_Idle_12 = 12,
    eBrain0_IdleAnimation_13 = 13,
    eBrain0_Turning_14 = 14,
    eBrain0_Panic_15 = 15
};

s16 Paramite::Brain_0_Patrol()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const FP kGridSize = ScaleToGridSize(mSpriteScale);

    switch (mBrainSubState)
    {
        case Brain_0_Patrol::eBrain0_Inactive_0:
            if (sNumCamSwappers_507668 > 0)
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case Brain_0_Patrol::eBrain0_IdleForAbe_1:
        {
            mMeat = FindMeat();
            if (mMeat)
            {
                SetBrain(&Paramite::Brain_5_SpottedMeat);
                mMeat->mBaseGameObjectRefCount++;
                return Brain_0_Patrol::eBrain0_Inactive_0;
            }

            if (!VOnSameYLevel(sActiveHero)
                || mSpriteScale != sActiveHero->mSpriteScale
                || WallHit_401930(mSpriteScale * FP_FromInteger(5), sActiveHero->mXPos - mXPos))
            {
                mWaitTimer = sGnFrame + Math_RandomRange(45, 135);
                return Brain_0_Patrol::eBrain0_Idle_12;
            }

            if (EventGet(kEventAbeOhm) && VIsFacingMe(sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_15_Hiss);
                return Brain_0_Patrol::eBrain0_Panic_15;
            }

            // TODO: Inlined HandleEnemyStopper but the directional logic isn't quite the same
            // so can't use it.
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper);
            if (BaseAliveGameObjectPathTLV)
            {
                auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
                if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && sActiveHero->mXPos < mXPos) || (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && sActiveHero->mXPos > mXPos))
                {
                    if (!SwitchStates_Get(pStopper->mSwitchId))
                    {
                        return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                    }
                }
            }

            if ((AnotherParamiteNear() || sActiveHero->mHealth <= FP_FromInteger(0)) && mSpriteScale == sActiveHero->mSpriteScale)
            {
                Sound(ParamiteSpeak::Howdy_5);
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                mAttackTimer = sGnFrame + mGroupChaseDelay;
                return Brain_0_Patrol::eBrain0_Inactive_0;
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(6), sActiveHero))
            {
                if (!VIsObjNearby(kGridSize * FP_FromInteger(8), sActiveHero))
                {
                    if (!VIsFacingMe(sActiveHero))
                    {
                        if ((mAnim.mFlags.Get(AnimFlags::eFlipX) && !Check_IsOnEndOfLine_4021A0(0, 2)) || (!mAnim.mFlags.Get(AnimFlags::eFlipX) && !Check_IsOnEndOfLine_4021A0(1, 2)))
                        {
                            SetNextMotion(eParamiteMotions::Motion_5_Turn);
                            return Brain_0_Patrol::eBrain0_TurningForAbe_6;
                        }
                    }
                    else
                    {
                        if ((mAnim.mFlags.Get(AnimFlags::eFlipX) && !Check_IsOnEndOfLine_4021A0(1, 2)) || (!mAnim.mFlags.Get(AnimFlags::eFlipX) && !Check_IsOnEndOfLine_4021A0(0, 2)))
                        {
                            SetNextMotion(eParamiteMotions::Motion_2_Walking);
                            return Brain_0_Patrol::eBrain0_ApproachingAbe_4;
                        }
                    }
                }

                if (!VIsFacingMe(sActiveHero))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_0_Patrol::eBrain0_TurningForAbe_6;
                }

                return mBrainSubState;
            }
            else if (VIsFacingMe(sActiveHero))
            {
                if ((mAnim.mFlags.Get(AnimFlags::eFlipX) && Check_IsOnEndOfLine_4021A0(0, 2)) || (!mAnim.mFlags.Get(AnimFlags::eFlipX) && Check_IsOnEndOfLine_4021A0(1, 2)))
                {
                    SetNextMotion(eParamiteMotions::Motion_13_GameSpeakBegin);
                    return Brain_0_Patrol::eBrain0_StuckToWall_8;
                }

                Sound(ParamiteSpeak::Howdy_5);
                SetNextMotion(eParamiteMotions::Motion_5_Turn);
                return Brain_0_Patrol::eBrain0_TurningForAbe_6;
            }
            else if (!VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero))
            {
                if ((mAnim.mFlags.Get(AnimFlags::eFlipX) && Check_IsOnEndOfLine_4021A0(1, 2)) || (!mAnim.mFlags.Get(AnimFlags::eFlipX) && Check_IsOnEndOfLine_4021A0(0, 2)))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }

                SetNextMotion(eParamiteMotions::Motion_2_Walking);
                return Brain_0_Patrol::eBrain0_FearingAbe_2;
            }
            else
            {
                if ((mAnim.mFlags.Get(AnimFlags::eFlipX) && Check_IsOnEndOfLine_4021A0(1, 2)) || (!mAnim.mFlags.Get(AnimFlags::eFlipX) && Check_IsOnEndOfLine_4021A0(0, 2)))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
                SetNextMotion(eParamiteMotions::Motion_3_Running);
                return Brain_0_Patrol::eBrain0_RunningFromAbe_3;
            }
            break;
        }

        case Brain_0_Patrol::eBrain0_FearingAbe_2:
            if (!VOnSameYLevel(sActiveHero) || mSpriteScale != sActiveHero->mSpriteScale)
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine_4021A0(1, 2))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine_4021A0(0, 2))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
            }

            if (AnotherParamiteNear())
            {
                Sound(ParamiteSpeak::Howdy_5);
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                mAttackTimer = sGnFrame + mGroupChaseDelay;
                return 0;
            }

            if (VIsObj_GettingNear_On_X(sActiveHero) && VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_3_Running);
                return Brain_0_Patrol::eBrain0_RunningFromAbe_3;
            }

            if (VIsObj_GettingNear_On_X(sActiveHero))
            {
                return mBrainSubState;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(7), sActiveHero))
            {
                return mBrainSubState;
            }

            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case Brain_0_Patrol::eBrain0_RunningFromAbe_3:
            if (!VOnSameYLevel(sActiveHero) || mSpriteScale != sActiveHero->mSpriteScale)
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine_4021A0(1, 3))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if ((WallHit_401930(mSpriteScale * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine_4021A0(0, 3)))
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
            }

            if (AnotherParamiteNear())
            {
                Sound(ParamiteSpeak::Howdy_5);
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                mAttackTimer = sGnFrame + mGroupChaseDelay;
                return 0;
            }

            if (VIsObj_GettingNear_On_X(sActiveHero))
            {
                return mBrainSubState;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(7), sActiveHero))
            {
                return mBrainSubState;
            }

            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return Brain_0_Patrol::eBrain0_FearingAbe_2;

        case Brain_0_Patrol::eBrain0_ApproachingAbe_4:
            if (!VOnSameYLevel(sActiveHero) || mSpriteScale != sActiveHero->mSpriteScale)
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (AnotherParamiteNear())
            {
                Sound(ParamiteSpeak::Howdy_5);
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                mAttackTimer = sGnFrame + mGroupChaseDelay;
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                return Brain_0_Patrol::eBrain0_Inactive_0;
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (HandleEnemyStopper(-2, relive::Path_EnemyStopper::StopDirection::Left))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }

                if (WallHit_401930(mSpriteScale * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine_4021A0(1, 2))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (HandleEnemyStopper(2, relive::Path_EnemyStopper::StopDirection::Right))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }

                if (WallHit_401930(mSpriteScale * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine_4021A0(0, 2))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }
            }

            if (VIsObj_GettingNear_On_X(sActiveHero) || VIsObjNearby(kGridSize * FP_FromInteger(10), sActiveHero))
            {
                if (!VIsObj_GettingNear_On_X(sActiveHero))
                {
                    return mBrainSubState;
                }

                if (!VIsObjNearby(kGridSize * FP_FromInteger(8), sActiveHero))
                {
                    return mBrainSubState;
                }
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }
            SetNextMotion(eParamiteMotions::Motion_3_Running);
            return Brain_0_Patrol::eBrain0_StopApproachingAbe_5;

        case Brain_0_Patrol::eBrain0_StopApproachingAbe_5:
            if (!VOnSameYLevel(sActiveHero) || mSpriteScale != sActiveHero->mSpriteScale)
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (AnotherParamiteNear())
            {
                Sound(ParamiteSpeak::Howdy_5);
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                mAttackTimer = sGnFrame + mGroupChaseDelay;
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                return 0;
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (HandleEnemyStopper(-2, relive::Path_EnemyStopper::StopDirection::Left))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }

                if (WallHit_401930(mSpriteScale * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine_4021A0(1, 3))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (HandleEnemyStopper(2, relive::Path_EnemyStopper::StopDirection::Right))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }

                if (WallHit_401930(mSpriteScale * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine_4021A0(0, 3))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }
            }

            if (!VIsObj_GettingNear_On_X(sActiveHero))
            {
                return mBrainSubState;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(8), sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(10), sActiveHero))
            {
                return mBrainSubState;
            }

            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return Brain_0_Patrol::eBrain0_ApproachingAbe_4;

        case Brain_0_Patrol::eBrain0_TurningForAbe_6:
            if (GetCurrentMotion() != eParamiteMotions::Motion_5_Turn || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case Brain_0_Patrol::eBrain0_HittingAbe_7:
            if (GetCurrentMotion() != eParamiteMotions::Motion_5_Turn || !(mAnim.mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_13_GameSpeakBegin);
            return Brain_0_Patrol::eBrain0_StuckToWall_8;

        case Brain_0_Patrol::eBrain0_StuckToWall_8:
            mMeat = FindMeat();
            if (mMeat)
            {
                SetBrain(&Paramite::Brain_5_SpottedMeat);
                mMeat->mBaseGameObjectRefCount++;
                return 0;
            }

            if (!VOnSameYLevel(sActiveHero) || mSpriteScale != sActiveHero->mSpriteScale)
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(3), sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_18_RunningAttack);
                return Brain_0_Patrol::eBrain0_Attacking_9;
            }

            if (VIsObj_GettingNear_On_X(sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_15_Hiss);
            }
            else
            {
                if (!VIsObjNearby(kGridSize * FP_FromInteger(8), sActiveHero))
                {
                    SetNextMotion(eParamiteMotions::Motion_2_Walking);
                    return Brain_0_Patrol::eBrain0_ApproachingAbe_4;
                }
            }

            if (GetCurrentMotion() != eParamiteMotions::Motion_14_PreHiss)
            {
                return mBrainSubState;
            }

            if (Math_NextRandom() >= 6u)
            {
                return mBrainSubState;
            }

            SetNextMotion(eParamiteMotions::Motion_15_Hiss);
            return mBrainSubState;

        case Brain_0_Patrol::eBrain0_Attacking_9:
            if (GetCurrentMotion() != eParamiteMotions::Motion_18_RunningAttack
                || !(mAnim.mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            if (sActiveHero->mHealth > FP_FromInteger(0))
            {
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }
            SetBrain(&Paramite::Brain_4_ChasingAbe);
            return 0;

        case Brain_0_Patrol::eBrain0_Idle_12:
            mMeat = FindMeat();
            if (mMeat)
            {
                SetBrain(&Paramite::Brain_5_SpottedMeat);
                mMeat->mBaseGameObjectRefCount++;
                return 0;
            }

            if (VOnSameYLevel(sActiveHero)
                && mSpriteScale == sActiveHero->mSpriteScale
                && !WallHit_401930(mSpriteScale * FP_FromInteger(5), sActiveHero->mXPos - mXPos))
            {
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (EventGet(kEventAbeOhm))
            {
                SetNextMotion(eParamiteMotions::Motion_15_Hiss);
                return Brain_0_Patrol::eBrain0_Panic_15;
            }

            if (mWaitTimer <= static_cast<s32>(sGnFrame))
            {
                if (Math_NextRandom() >= 6u)
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_0_Patrol::eBrain0_Turning_14;
                }
                else
                {
                    SetNextMotion(eParamiteMotions::Motion_13_GameSpeakBegin);
                    return Brain_0_Patrol::eBrain0_IdleAnimation_13;
                }
            }
            return mBrainSubState;

        case Brain_0_Patrol::eBrain0_IdleAnimation_13:
            if (GetCurrentMotion() != eParamiteMotions::Motion_14_PreHiss
                || !(mAnim.mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            mWaitTimer = sGnFrame + Math_RandomRange(45, 135);
            return Brain_0_Patrol::eBrain0_Idle_12;

        case Brain_0_Patrol::eBrain0_Turning_14:
            if (GetCurrentMotion() != eParamiteMotions::Motion_5_Turn
                || !(mAnim.mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            mWaitTimer = sGnFrame + Math_RandomRange(45, 135);
            return Brain_0_Patrol::eBrain0_Idle_12;

        case Brain_0_Patrol::eBrain0_Panic_15:
            if (!EventGet(kEventAbeOhm))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_0_Patrol::eBrain0_Idle_12;
            }
            SetNextMotion(eParamiteMotions::Motion_15_Hiss);
            return mBrainSubState;

        default:
            return mBrainSubState;
    }
}

enum Brain_1_SurpriseWeb
{
    eBrain1_Loading_0 = 0,
    eBrain1_AppearingRight_1 = 1,
    eBrain1_AppearingLeft_2 = 2,
    eBrain1_StartAnimation_3 = 3,
    eBrain1_StateLoop1_4 = 4,
    eBrain1_StateLoop2_5 = 5
};

s16 Paramite::Brain_1_SurpriseWeb()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (mBrainSubState)
    {
        case Brain_1_SurpriseWeb::eBrain1_Loading_0:
        {
            if (sNumCamSwappers_507668 > 0)
            {
                return mBrainSubState;
            }

            SetCurrentMotion(eParamiteMotions::Motion_0_Idle);

            MapFollowMe_401D30(TRUE);
            auto pWeb = relive_new ParamiteWeb(
                mXPos,
                FP_GetExponent(mYPos) - 20,
                FP_GetExponent(mYPos) - 10,
                mSpriteScale);
            if (pWeb)
            {
                pWeb->mBaseGameObjectRefCount++;
                mParamiteWeb = pWeb;
            }

            if (sActiveHero->mXPos >= mXPos)
            {
                return Brain_1_SurpriseWeb::eBrain1_AppearingLeft_2;
            }
            return Brain_1_SurpriseWeb::eBrain1_AppearingRight_1;
        }

        case Brain_1_SurpriseWeb::eBrain1_AppearingRight_1:
            if (sActiveHero->mXPos <= mXPos
                || mSpriteScale != sActiveHero->mSpriteScale
                || !gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    1))
            {
                if (!SwitchStates_Get(mSurpriseWebSwitchId))
                {
                    return mBrainSubState;
                }
            }
            mAnim.mFlags.Clear(AnimFlags::eFlipX);
            field_114_timer = sGnFrame + mSurpriseWebDelayTimer;
            return Brain_1_SurpriseWeb::eBrain1_StartAnimation_3;

        case Brain_1_SurpriseWeb::eBrain1_AppearingLeft_2:
            if (sActiveHero->mXPos >= mXPos
                || mSpriteScale != sActiveHero->mSpriteScale
                || !gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    1))
            {
                if (!SwitchStates_Get(mSurpriseWebSwitchId))
                {
                    return mBrainSubState;
                }
            }
            mAnim.mFlags.Set(AnimFlags::eFlipX);
            field_114_timer = sGnFrame + mSurpriseWebDelayTimer;
            return Brain_1_SurpriseWeb::eBrain1_StartAnimation_3;

        case Brain_1_SurpriseWeb::eBrain1_StartAnimation_3:
            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mVelY = FP_FromInteger(0);
            SetCurrentMotion(eParamiteMotions::Motion_20_SurpriseWeb);
            return Brain_1_SurpriseWeb::eBrain1_StateLoop1_4;

        case Brain_1_SurpriseWeb::eBrain1_StateLoop1_4:
            mParamiteWeb->field_EA_ttl_remainder = FP_GetExponent(FP_Abs(mYPos)) - 10;
            mParamiteWeb->mYPos = FP_FromInteger(mParamiteWeb->field_EA_ttl_remainder);
            if (GetCurrentMotion() == eParamiteMotions::Motion_0_Idle)
            {
                mParamiteWeb->field_F0_bEnabled = TRUE;
                mParamiteWeb->mBaseGameObjectRefCount--;
                mParamiteWeb = nullptr;
                SetBrain(&Paramite::Brain_0_Patrol);
                return Brain_0_Patrol::eBrain0_Inactive_0;
            }

            if (mVelY < (mSpriteScale * FP_FromInteger(8)))
            {
                mVelY += (mSpriteScale * FP_FromDouble(0.5));
                return mBrainSubState;
            }
            return Brain_1_SurpriseWeb::eBrain1_StateLoop2_5;

        case Brain_1_SurpriseWeb::eBrain1_StateLoop2_5:
            mParamiteWeb->field_EA_ttl_remainder = FP_GetExponent(FP_Abs(mYPos)) - 10;
            mParamiteWeb->mYPos = FP_FromInteger(mParamiteWeb->field_EA_ttl_remainder);
            if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
            {
                if (mVelY <= (mSpriteScale * FP_FromInteger(-1)))
                {
                    return Brain_1_SurpriseWeb::eBrain1_StateLoop1_4;
                }
                else
                {
                    mVelY -= (mSpriteScale * FP_FromInteger(1));
                    return mBrainSubState;
                }
            }
            else
            {
                mParamiteWeb->field_F0_bEnabled = TRUE;
                mParamiteWeb->mBaseGameObjectRefCount--;
                mParamiteWeb = nullptr;
                SetBrain(&Paramite::Brain_0_Patrol);
                return Brain_0_Patrol::eBrain0_Inactive_0;
            }
            break;

        default:
            break;
    }

    return mBrainSubState;
}

enum Brain_2_Struggling
{
    eBrain2_SetTimer_0 = 0,
    eBrain2_Hiss_1 = 1,
    eBrain2_Turn_2 = 2,
    eBrain2_Death_3 = 3
};

s16 Paramite::IsBeeSwarmChasingMe_4022B0()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eBeeSwarm)
        {
            if (static_cast<BeeSwarm*>(pObj)->mChaseTarget == this)
            {
                return 1;
            }
        }
    }
    return 0;
}


s16 Paramite::Brain_2_Struggling()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (IsBeeSwarmChasingMe_4022B0())
    {
        switch (mBrainSubState)
        {
            case Brain_2_Struggling::eBrain2_SetTimer_0:
                field_114_timer = sGnFrame + 30;
                return 1;

            case Brain_2_Struggling::eBrain2_Hiss_1:
                if (field_114_timer <= static_cast<s32>(sGnFrame))
                {
                    SetCurrentMotion(eParamiteMotions::Motion_13_GameSpeakBegin);
                    SetNextMotion(eParamiteMotions::Motion_15_Hiss);
                    return Brain_2_Struggling::eBrain2_Turn_2;
                }
                break;

            case Brain_2_Struggling::eBrain2_Turn_2:
                if (GetCurrentMotion() == eParamiteMotions::Motion_15_Hiss)
                {
                    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
                    {
                        SetNextMotion(eParamiteMotions::Motion_5_Turn);
                        return Brain_2_Struggling::eBrain2_Death_3;
                    }
                }
                break;

            case Brain_2_Struggling::eBrain2_Death_3:
                if (GetCurrentMotion() == eParamiteMotions::Motion_5_Turn)
                {
                    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
                    {
                        SetNextMotion(eParamiteMotions::Motion_25_Death);
                        field_114_timer = sGnFrame + 30;
                        return 1;
                    }
                }
                break;
            default:
                break;
        }
        return mBrainSubState;
    }
    else if (GetCurrentMotion() == eParamiteMotions::Motion_5_Turn)
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        SetBrain(&Paramite::Brain_0_Patrol);
        return Brain_0_Patrol::eBrain0_Inactive_0;
    }
    else
    {
        SetCurrentMotion(eParamiteMotions::Motion_0_Idle);
        SetNextMotion(-1);
        SetBrain(&Paramite::Brain_0_Patrol);
        return mBrainSubState;
    }
}

s16 Paramite::Brain_3_Death()
{
    if (static_cast<s32>(sGnFrame) < field_114_timer && (field_114_timer < static_cast<s32>(sGnFrame) + 80))
    {
        mSpriteScale -= FP_FromDouble(0.008);
        mRGB.r -= 2;
        mRGB.g -= 2;
        mRGB.b -= 2;
        if (static_cast<s32>(sGnFrame) < field_114_timer - 24)
        {
            DeathSmokeEffect(true);
        }
    }

    if (field_114_timer < (s32) sGnFrame)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    return 100;
}

enum Brain_4_ChasingAbe
{
    eBrain4_Inactive_0 = 0,
    eBrain4_Attacking_1 = 1,
    eBrain4_ToWarning_2 = 2,
    eBrain4_Warning_3 = 3,
    eBrain4_CloseAttack_4 = 4,
    eBrain4_ToChasing_5 = 5,
    eBrain4_QuickAttack_6 = 6,
    eBrain4_Chasing_7 = 7,
    eBrain4_Jumping_8 = 8,
    eBrain4_TurningWhileChasing_9 = 9,
    eBrain4_Turning_10 = 10,
    eBrain4_Walking_11 = 11,
    eBrain4_WalkingToHop_12 = 12,
    eBrain4_Eating_13 = 13,
    eBrain4_Idle_14 = 14,
    eBrain4_Panic_15 = 15
};

s16 Paramite::Brain_4_ChasingAbe()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mAttackTimer <= static_cast<s32>(sGnFrame)
        && (!VOnSameYLevel(sActiveHero)
            || mSpriteScale != sActiveHero->mSpriteScale)
        && sActiveHero->mHealth > FP_FromInteger(0))
    {
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        SetBrain(&Paramite::Brain_0_Patrol);
        return 0;
    }

    if (EventGet(kEventAbeOhm))
    {
        SetNextMotion(eParamiteMotions::Motion_15_Hiss);
        return Brain_4_ChasingAbe::eBrain4_Panic_15;
    }

    const FP kGridSize = ScaleToGridSize(mSpriteScale);

    switch (mBrainSubState)
    {
        case Brain_4_ChasingAbe::eBrain4_Inactive_0:
            if (!VOnSameYLevel(sActiveHero))
            {
                return Brain_4_ChasingAbe::eBrain4_Inactive_0;
            }

            if (sActiveHero->mHealth > FP_FromInteger(0))
            {
                if (VIsFacingMe(sActiveHero))
                {
                    if (mHissBeforeAttack == relive::reliveChoice::eYes)
                    {
                        field_114_timer = sGnFrame + Math_RandomRange(0, 6);
                        return Brain_4_ChasingAbe::eBrain4_ToWarning_2;
                    }
                    else
                    {
                        field_114_timer = sGnFrame + mAloneChaseDelay;
                        return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                    }
                }
                else
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_4_ChasingAbe::eBrain4_CloseAttack_4;
                }
            }
            else
            {
                if (VIsFacingMe(sActiveHero))
                {
                    if (VIsObjNearby(kGridSize, sActiveHero))
                    {
                        SetNextMotion(eParamiteMotions::Motion_23_Eating);
                        field_114_timer = sGnFrame + 28;
                        return Brain_4_ChasingAbe::eBrain4_Eating_13;
                    }

                    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
                    {
                        if (Check_IsOnEndOfLine_4021A0(1, 1))
                        {
                            SetNextMotion(eParamiteMotions::Motion_6_Hop);
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    else
                    {
                        if (Check_IsOnEndOfLine_4021A0(0, 1))
                        {
                            SetNextMotion(eParamiteMotions::Motion_6_Hop);
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    SetNextMotion(eParamiteMotions::Motion_2_Walking);
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
                else
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_4_ChasingAbe::eBrain4_Turning_10;
                }
            }
            break;

        case Brain_4_ChasingAbe::eBrain4_Attacking_1:
            if (GetCurrentMotion() != eParamiteMotions::Motion_18_RunningAttack
                || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }

            if (sActiveHero->mHealth > FP_FromInteger(0))
            {
                if (VIsFacingMe(sActiveHero))
                {
                    if (VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero))
                    {
                        if (WallHit_401930(mSpriteScale * FP_FromInteger(10), sActiveHero->mXPos - mXPos))
                        {
                            SetNextMotion(eParamiteMotions::Motion_0_Idle);
                            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                        }
                        else
                        {
                            SetNextMotion(eParamiteMotions::Motion_18_RunningAttack);
                            return Brain_4_ChasingAbe::eBrain4_Attacking_1;
                        }
                    }

                    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
                    {
                        if (Check_IsOnEndOfLine_4021A0(1, 1))
                        {
                            SetNextMotion(eParamiteMotions::Motion_6_Hop);
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    else
                    {
                        if (Check_IsOnEndOfLine_4021A0(0, 1))
                        {
                            SetNextMotion(eParamiteMotions::Motion_6_Hop);
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    SetNextMotion(eParamiteMotions::Motion_3_Running);
                    return Brain_4_ChasingAbe::eBrain4_Chasing_7;
                }
                else
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_4_ChasingAbe::eBrain4_QuickAttack_6;
                }
            }
            else
            {
                if (VIsFacingMe(sActiveHero))
                {
                    if (VIsObjNearby(kGridSize, sActiveHero))
                    {
                        SetNextMotion(eParamiteMotions::Motion_23_Eating);
                        field_114_timer = sGnFrame + 28;
                        return Brain_4_ChasingAbe::eBrain4_Eating_13;
                    }

                    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
                    {
                        if (Check_IsOnEndOfLine_4021A0(1, 1))
                        {
                            SetNextMotion(eParamiteMotions::Motion_6_Hop);
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    else
                    {
                        if (Check_IsOnEndOfLine_4021A0(0, 1))
                        {
                            SetNextMotion(eParamiteMotions::Motion_6_Hop);
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    SetNextMotion(eParamiteMotions::Motion_2_Walking);
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
                else
                {
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_4_ChasingAbe::eBrain4_Turning_10;
                }
            }
            break;

        case Brain_4_ChasingAbe::eBrain4_ToWarning_2:
            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_15_Hiss);
            field_114_timer = sGnFrame + mAloneChaseDelay;
            return Brain_4_ChasingAbe::eBrain4_Warning_3;

        case Brain_4_ChasingAbe::eBrain4_Warning_3:
            if (GetCurrentMotion() != eParamiteMotions::Motion_15_Hiss)
            {
                return mBrainSubState;
            }
            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;

        case Brain_4_ChasingAbe::eBrain4_CloseAttack_4:
            if (GetCurrentMotion() != eParamiteMotions::Motion_5_Turn
                || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }

            if (mHissBeforeAttack == relive::reliveChoice::eYes)
            {
                field_114_timer = sGnFrame + Math_RandomRange(0, 6);
                return Brain_4_ChasingAbe::eBrain4_ToWarning_2;
            }
            else
            {
                field_114_timer = sGnFrame + mAloneChaseDelay;
                return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
            }
            break;

        case Brain_4_ChasingAbe::eBrain4_ToChasing_5:
            if (!VOnSameYLevel(sActiveHero))
            {
                return Brain_4_ChasingAbe::eBrain4_Inactive_0;
            }

            if (!VIsFacingMe(sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_5_Turn);
                return 6;
            }

            // TODO: Inlined HandleEnemyStopper but the directional logic isn't quite the same
            // so can't use it.
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper);
            if (BaseAliveGameObjectPathTLV)
            {
                auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
                if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && sActiveHero->mXPos < mXPos) || (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && sActiveHero->mXPos > mXPos))
                {
                    if (!SwitchStates_Get(pStopper->mSwitchId))
                    {
                        return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                    }
                }
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero))
            {
                if (WallHit_401930(mSpriteScale * FP_FromInteger(10), sActiveHero->mXPos - mXPos))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                }
                else
                {
                    SetNextMotion(eParamiteMotions::Motion_18_RunningAttack);
                    return Brain_4_ChasingAbe::eBrain4_Attacking_1;
                }
            }

            if (field_114_timer > static_cast<s32>(sGnFrame) && mHissBeforeAttack == relive::reliveChoice::eYes)
            {
                return mBrainSubState;
            }

            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_6_Hop);
                    return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                }
            }
            else if (!mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_6_Hop);
                    return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                }
            }

            SetNextMotion(eParamiteMotions::Motion_3_Running);
            return Brain_4_ChasingAbe::eBrain4_Chasing_7;

        case Brain_4_ChasingAbe::eBrain4_QuickAttack_6:
            if (GetCurrentMotion() != eParamiteMotions::Motion_5_Turn
                || !(mAnim.mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }

            if (mHissBeforeAttack == relive::reliveChoice::eYes)
            {
                SetNextMotion(eParamiteMotions::Motion_15_Hiss);
            }
            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;

        case Brain_4_ChasingAbe::eBrain4_Chasing_7:
        {
            if (!VOnSameYLevel(sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_4_ChasingAbe::eBrain4_Inactive_0;
            }

            if (!VIsFacingMe(sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_5_Turn);
                return Brain_4_ChasingAbe::eBrain4_TurningWhileChasing_9;
            }

            const s16 x_exp = FP_GetExponent(mXPos);
            const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(mSpriteScale, x_exp & 0x3FF);
            if (mVelX < FP_FromInteger(0))
            {
                if (HandleEnemyStopper(-2, relive::Path_EnemyStopper::StopDirection::Left))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                }

                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    ToHop();
                    return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                }
                else
                {
                    // Drop through to idle/attack case
                }
            }
            else if (mVelX > FP_FromInteger(0))
            {
                if (HandleEnemyStopper(2, relive::Path_EnemyStopper::StopDirection::Right))
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                }

                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    ToHop();
                    return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                }
                else
                {
                    // Drop through to idle/attack case
                }
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(4), sActiveHero))
            {
                return mBrainSubState;
            }

            if (WallHit_401930(mSpriteScale * FP_FromInteger(10), sActiveHero->mXPos - mXPos))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
            }
            else
            {
                SetNextMotion(eParamiteMotions::Motion_18_RunningAttack);
                return Brain_4_ChasingAbe::eBrain4_Attacking_1;
            }
        }
        break;

        case Brain_4_ChasingAbe::eBrain4_Jumping_8:
            if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;

        case Brain_4_ChasingAbe::eBrain4_TurningWhileChasing_9:
            if (GetCurrentMotion() != eParamiteMotions::Motion_5_Turn
                || !(mAnim.mFlags.Get(AnimFlags::eIsLastFrame)))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;

        case Brain_4_ChasingAbe::eBrain4_Turning_10:
            if (GetCurrentMotion() != eParamiteMotions::Motion_5_Turn
                || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }

            if (VIsObjNearby(kGridSize, sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_23_Eating);
                field_114_timer = sGnFrame + 28;
                return Brain_4_ChasingAbe::eBrain4_Eating_13;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (!Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_2_Walking);
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
            }
            else if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (!Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    SetNextMotion(eParamiteMotions::Motion_2_Walking);
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
            }
            SetNextMotion(eParamiteMotions::Motion_6_Hop);
            return Brain_4_ChasingAbe::eBrain4_Jumping_8;

        case Brain_4_ChasingAbe::eBrain4_Walking_11:
        {
            if (!VIsFacingMe(sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_5_Turn);
                return Brain_4_ChasingAbe::eBrain4_Turning_10;
            }

            if (!VOnSameYLevel(sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_4_ChasingAbe::eBrain4_Walking_11;
            }

            if (VIsObjNearby(kGridSize, sActiveHero))
            {
                SetNextMotion(eParamiteMotions::Motion_23_Eating);
                field_114_timer = sGnFrame + 28;
                return Brain_4_ChasingAbe::eBrain4_Eating_13;
            }

            const s16 x_exp = FP_GetExponent(mXPos);
            const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(mSpriteScale, x_exp & 0x3FF);
            if (mVelX > FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    ToHop();
                    return Brain_4_ChasingAbe::eBrain4_WalkingToHop_12;
                }
            }
            else if (mVelX < FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    ToHop();
                    return Brain_4_ChasingAbe::eBrain4_WalkingToHop_12;
                }
            }
        }
            return mBrainSubState;

        case Brain_4_ChasingAbe::eBrain4_WalkingToHop_12:
            if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }

            if (VIsFacingMe(sActiveHero))
            {
                if (VIsObjNearby(kGridSize, sActiveHero))
                {
                    SetNextMotion(eParamiteMotions::Motion_23_Eating);
                    field_114_timer = sGnFrame + 28;
                    return Brain_4_ChasingAbe::eBrain4_Eating_13;
                }
                else
                {
                    SetNextMotion(eParamiteMotions::Motion_2_Walking);
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
            }
            else
            {
                SetNextMotion(eParamiteMotions::Motion_5_Turn);
                return Brain_4_ChasingAbe::eBrain4_Turning_10;
            }
            break;

        case Brain_4_ChasingAbe::eBrain4_Eating_13:
            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            field_114_timer = sGnFrame + 15;
            return Brain_4_ChasingAbe::eBrain4_Idle_14;

        case Brain_4_ChasingAbe::eBrain4_Idle_14:
            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_23_Eating);
            field_114_timer = sGnFrame + 28;
            return Brain_4_ChasingAbe::eBrain4_Eating_13;

        case Brain_4_ChasingAbe::eBrain4_Panic_15:
            if (EventGet(kEventAbeOhm))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            return Brain_4_ChasingAbe::eBrain4_Inactive_0;

        default:
            return mBrainSubState;
    }
}

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

s16 Paramite::Brain_5_SpottedMeat()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mMeat->VIsFalling() || mMeat->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        Sound(ParamiteSpeak::DetectedMeat_7);
        SetBrain(&Paramite::Brain_0_Patrol);
        mMeat->mBaseGameObjectRefCount--;
        mMeat = nullptr;
        SetNextMotion(eParamiteMotions::Motion_0_Idle);
        return Brain_0_Patrol::eBrain0_Inactive_0;
    }

    if (mMeat->field_124_pLine)
    {
        if (FP_Abs(mMeat->mYPos - mYPos) > FP_FromInteger(20))
        {
            mMeat->mBaseGameObjectRefCount--;
            mMeat = nullptr;
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            SetBrain(&Paramite::Brain_0_Patrol);
            return Brain_0_Patrol::eBrain0_Inactive_0;
        }
    }

    const FP kGridSize = ScaleToGridSize(mSpriteScale);

    switch (mBrainSubState)
    {
        case Brain_5_SpottedMeat::eBrain5_Idle_0:
            if (!VIsFacingMe(mMeat))
            {
                if (FP_Abs(mMeat->mXPos - mXPos) > FP_FromInteger(5))
                {
                    Sound(ParamiteSpeak::DetectedMeat_7);
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_5_SpottedMeat::eBrain5_Turning_4;
                }
            }

            if (VIsObj_GettingNear_On_X(mMeat))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }
            else if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(3), mMeat))
            {
                SetNextMotion(eParamiteMotions::Motion_3_Running);
                return Brain_5_SpottedMeat::eBrain5_Running_1;
            }

            if (VIsObjNearby(mSpriteScale * FP_FromInteger(40), mMeat))
            {
                if (mXPos == mMeat->mXPos)
                {
                    SetNextMotion(eParamiteMotions::Motion_2_Walking);
                    return Brain_5_SpottedMeat::eBrain5_Walking_2;
                }

                if (!mMeat->field_124_pLine || !BaseAliveGameObjectCollisionLine)
                {
                    SetNextMotion(eParamiteMotions::Motion_0_Idle);
                    return Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
                }

                SetNextMotion(eParamiteMotions::Motion_23_Eating);
                field_114_timer = sGnFrame + mMeatEatingTime;
                return Brain_5_SpottedMeat::eBrain5_Eating_6;
            }

            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return Brain_5_SpottedMeat::eBrain5_Walking_2;

        case Brain_5_SpottedMeat::eBrain5_Running_1:
        {
            const s16 x_exp = FP_GetExponent(mXPos);
            const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(mSpriteScale, x_exp & 0x3FF);
            if (mVelX < FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (!VIsFacingMe(mMeat))
            {
                if (FP_Abs(mMeat->mXPos - mXPos) > FP_FromInteger(5))
                {
                    Sound(ParamiteSpeak::DetectedMeat_7);
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_5_SpottedMeat::eBrain5_Turning_4;
                }
            }

            if (VIsObjNearby(mSpriteScale * FP_FromInteger(40), mMeat))
            {
                if (mMeat->field_124_pLine)
                {
                    SetNextMotion(eParamiteMotions::Motion_23_Eating);
                    field_114_timer = sGnFrame + mMeatEatingTime;
                    return Brain_5_SpottedMeat::eBrain5_Eating_6;
                }
                return Brain_5_SpottedMeat::eBrain5_Running_1;
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(3), mMeat))
            {
                return mBrainSubState;
            }
            SetNextMotion(eParamiteMotions::Motion_2_Walking);
            return Brain_5_SpottedMeat::eBrain5_Walking_2;
        }

        case Brain_5_SpottedMeat::eBrain5_Walking_2:
        {
            const s16 x_exp = FP_GetExponent(mXPos);
            const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid(mSpriteScale, x_exp & 0x3FF);
            if (mVelX < FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) >= 6 && Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (!VIsFacingMe(mMeat))
            {
                if (FP_Abs(mMeat->mXPos - mXPos) > FP_FromInteger(5))
                {
                    Sound(ParamiteSpeak::DetectedMeat_7);
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_5_SpottedMeat::eBrain5_Turning_4;
                }
            }

            if (!VIsObjNearby(mSpriteScale * FP_FromInteger(40), mMeat))
            {
                return mBrainSubState;
            }

            if (mMeat->field_124_pLine)
            {
                SetNextMotion(eParamiteMotions::Motion_23_Eating);
                field_114_timer = sGnFrame + mMeatEatingTime;
                return Brain_5_SpottedMeat::eBrain5_Eating_6;
            }
            return Brain_5_SpottedMeat::eBrain5_Walking_2;
        }

        case Brain_5_SpottedMeat::eBrain5_Jumping_3:
            if (GetCurrentMotion() != eParamiteMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return Brain_5_SpottedMeat::eBrain5_Idle_0;

        case Brain_5_SpottedMeat::eBrain5_Turning_4:
            if (GetCurrentMotion() != eParamiteMotions::Motion_5_Turn
                || !mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                return mBrainSubState;
            }

            if (!VIsFacingMe(mMeat))
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_5_SpottedMeat::eBrain5_Idle_0;
            }

            if (FP_Abs(mMeat->mXPos - mXPos) > FP_FromInteger(5))
            {
                Sound(ParamiteSpeak::DetectedMeat_7);
                SetNextMotion(eParamiteMotions::Motion_5_Turn);
                return 4;
            }
            else
            {
                SetNextMotion(eParamiteMotions::Motion_0_Idle);
                return Brain_5_SpottedMeat::eBrain5_Idle_0;
            }
            break;

        case Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5:
            if (!VIsFacingMe(mMeat))
            {
                Sound(ParamiteSpeak::DetectedMeat_7);
                SetNextMotion(eParamiteMotions::Motion_5_Turn);
                return Brain_5_SpottedMeat::eBrain5_Turning_4;
            }

            if (VIsObj_GettingNear_On_X(mMeat))
            {
                return mBrainSubState;
            }

            if (!mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(0, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }
            else if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                if (Check_IsOnEndOfLine_4021A0(1, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(3), mMeat))
            {
                SetNextMotion(eParamiteMotions::Motion_3_Running);
                return Brain_5_SpottedMeat::eBrain5_Running_1;
            }

            if (!VIsObjNearby(mSpriteScale * FP_FromInteger(40), mMeat))
            {
                SetNextMotion(eParamiteMotions::Motion_2_Walking);
                return Brain_5_SpottedMeat::eBrain5_Walking_2;
            }

            if (!mMeat->field_124_pLine || !BaseAliveGameObjectCollisionLine)
            {
                return Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
            }

            SetNextMotion(eParamiteMotions::Motion_23_Eating);
            field_114_timer = sGnFrame + mMeatEatingTime;
            return Brain_5_SpottedMeat::eBrain5_Eating_6;

        case Brain_5_SpottedMeat::eBrain5_Eating_6:
            if (!VIsObjNearby(mSpriteScale * FP_FromInteger(40), mMeat))
            {
                SetNextMotion(eParamiteMotions::Motion_2_Walking);
                return Brain_5_SpottedMeat::eBrain5_Walking_2;
            }

            if (!VIsFacingMe(mMeat))
            {
                if (FP_Abs(mMeat->mXPos - mXPos) > FP_FromInteger(5))
                {
                    Sound(ParamiteSpeak::DetectedMeat_7);
                    SetNextMotion(eParamiteMotions::Motion_5_Turn);
                    return Brain_5_SpottedMeat::eBrain5_Turning_4;
                }
            }

            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            mMeat->mBaseGameObjectRefCount--;
            mMeat->mBaseGameObjectFlags.Set(Options::eDead);
            mMeat = nullptr;
            SetNextMotion(eParamiteMotions::Motion_0_Idle);
            SetBrain(&Paramite::Brain_0_Patrol);
            return Brain_0_Patrol::eBrain0_Inactive_0;

        default:
            return mBrainSubState;
    }
}

void Paramite::SetBrain(Paramite::TParamiteBrain fn)
{
    mBrainState = fn;
}

bool Paramite::BrainIs(Paramite::TParamiteBrain fn)
{
    return mBrainState == fn;
}

s16 Paramite::HandleEnemyStopper(s16 numGridBlocks, relive::Path_EnemyStopper::StopDirection dir)
{
    const FP kGridSize = ScaleToGridSize(mSpriteScale);
    const FP numGridBlocksScaled = (kGridSize * FP_FromInteger(numGridBlocks));
    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(
        FP_GetExponent(mXPos + numGridBlocksScaled),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos + numGridBlocksScaled),
        FP_GetExponent(mYPos),
        ReliveTypes::eEnemyStopper);

    if (BaseAliveGameObjectPathTLV)
    {
        // No stopper or its disabled
        auto pEnemyStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
        if (!pEnemyStopper || !SwitchStates_Get(pEnemyStopper->mSwitchId))
        {
            return 0;
        }

        // Matches the direction we want?
        if (pEnemyStopper->mStopDirection == dir)
        {
            return 1;
        }
    }
    return 0;
}

void Paramite::Motion_0_Idle()
{
    if (!ToNextMotion()
        && gMap.GetDirection(
               mCurrentLevel,
               mCurrentPath,
               mXPos,
               mYPos)
               >= CameraPos::eCamCurrent_0)
    {
        if (BrainIs(&Paramite::Brain_4_ChasingAbe))
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
        }
        else
        {
            MusicController::static_PlayMusic(MusicController::MusicTypes::eType11, this, 0, 0);
        }
    }
}

const FP sWalkBeginVelTable_4BBC88[3] = {
    FP_FromInteger(0),
    FP_FromDouble(1.402099609375),
    FP_FromDouble(9.025436401367188)};

void Paramite::Motion_1_WalkBegin()
{
    EventBroadcast(kEventNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = mSpriteScale * -sWalkBeginVelTable_4BBC88[mAnim.mCurrentFrame];
    }
    else
    {
        mVelX = mSpriteScale * sWalkBeginVelTable_4BBC88[mAnim.mCurrentFrame];
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();

        if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
        {
            SetCurrentMotion(eParamiteMotions::Motion_2_Walking);
        }
    }
}

const FP sWalkVelTable_4BBC50[14] = {
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
    EventBroadcast(kEventNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = mSpriteScale * -sWalkVelTable_4BBC50[mAnim.mCurrentFrame];
    }
    else
    {
        mVelX = mSpriteScale * sWalkVelTable_4BBC50[mAnim.mCurrentFrame];
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
        return;
    }

    MoveOnLine();

    if (GetCurrentMotion() != eParamiteMotions::Motion_2_Walking)
    {
        return;
    }

    switch (mAnim.mCurrentFrame)
    {
        case 0:
            [[fallthrough]];
        case 7:
            Sound(mAnim.mCurrentFrame == 7 ? ParamiteSpeak::LoudStep_3 : ParamiteSpeak::SlightStep_4);

            if (GetNextMotion() == eParamiteMotions::Motion_0_Idle
                || GetNextMotion() == eParamiteMotions::Motion_13_GameSpeakBegin
                || GetNextMotion() == eParamiteMotions::Motion_5_Turn
                || GetNextMotion() == eParamiteMotions::Motion_25_Death
                || GetNextMotion() == eParamiteMotions::Motion_15_Hiss
                || GetNextMotion() == eParamiteMotions::Motion_23_Eating)
            {
                SetCurrentMotion(eParamiteMotions::Motion_9_WalkEnd);
            }
            break;

        case 3:
            [[fallthrough]];
        case 10:
            if (GetNextMotion() == eParamiteMotions::Motion_3_Running)
            {
                SetPreviousMotion(eParamiteMotions::Motion_3_Running);
                mBaseAliveGameObjectLastAnimFrame = 11;
                field_140_use_prev_motion = 1;
                SetNextMotion(-1);
            }

            if (GetNextMotion() == eParamiteMotions::Motion_18_RunningAttack)
            {
                SetCurrentMotion(eParamiteMotions::Motion_18_RunningAttack);
                SetNextMotion(-1);
                if (mAnim.mFlags.Get(AnimFlags::eFlipX))
                {
                    mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
                }
                else
                {
                    mVelX = ScaleToGridSize(mSpriteScale) / FP_FromInteger(4);
                }
            }

            if (!field_142_bSnapped)
            {
                MapFollowMe_401D30(1);
                field_142_bSnapped = 1;
            }

            break;

        default:
            field_142_bSnapped = 0;
            break;
    }

    SetMusic();
}

const FP sRunningTable_4BBCC0[14] = {
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
    EventBroadcast(kEventNoise, this);

    FP frameVelx = {};
    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelx = mSpriteScale * -sRunningTable_4BBCC0[mAnim.mCurrentFrame];
    }
    else
    {
        frameVelx = mSpriteScale * sRunningTable_4BBCC0[mAnim.mCurrentFrame];
    }

    mVelX = frameVelx;

    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
        return;
    }

    MoveOnLine();

    if (GetCurrentMotion() != eParamiteMotions::Motion_3_Running)
    {
        return;
    }

    if (mAnim.mCurrentFrame == 3)
    {
        SFX_Play_Pitch(relive::SoundEffects::PickupItem, 45, -600);
        if (GetNextMotion() == eParamiteMotions::Motion_2_Walking)
        {
            SetCurrentMotion(eParamiteMotions::Motion_8_WalkRunTransition);
            SetNextMotion(-1);
        }

        if (!mNextMotion
            || GetNextMotion() == eParamiteMotions::Motion_5_Turn
            || GetNextMotion() == eParamiteMotions::Motion_23_Eating
            || GetNextMotion() == eParamiteMotions::Motion_25_Death
            || GetNextMotion() == eParamiteMotions::Motion_15_Hiss)
        {
            SetCurrentMotion(eParamiteMotions::Motion_11_RunEnd);
        }

        if (GetNextMotion() == eParamiteMotions::Motion_18_RunningAttack)
        {
            SetCurrentMotion(eParamiteMotions::Motion_18_RunningAttack);
            SetNextMotion(-1);

            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
            }
        }

        if (GetNextMotion() == eParamiteMotions::Motion_6_Hop)
        {
            VOnTrapDoorOpen();

            BaseAliveGameObjectLastLineYPos = mYPos;
            SetCurrentMotion(eParamiteMotions::Motion_6_Hop);
            SetNextMotion(-1);
            mVelY = -FP_FromDouble(7.2) * mSpriteScale;
            mYPos += mVelY;
            BaseAliveGameObjectCollisionLine = nullptr;

            if (field_142_bSnapped == 0)
            {
                MapFollowMe_401D30(1);
                field_142_bSnapped = 1;
            }

            SetMusic();
            return;
        }

        if (!field_142_bSnapped)
        {
            MapFollowMe_401D30(1);
            field_142_bSnapped = 1;
        }

        SetMusic();
        return;
    }

    if (mAnim.mCurrentFrame == 10)
    {
        Sound(ParamiteSpeak::LoudStep_3);
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);

        if (GetNextMotion() == eParamiteMotions::Motion_2_Walking)
        {
            SetCurrentMotion(eParamiteMotions::Motion_8_WalkRunTransition);
            SetNextMotion(-1);
        }

        if (GetNextMotion() == eParamiteMotions::Motion_0_Idle
            || GetNextMotion() == eParamiteMotions::Motion_5_Turn
            || GetNextMotion() == eParamiteMotions::Motion_23_Eating
            || GetNextMotion() == eParamiteMotions::Motion_25_Death
            || GetNextMotion() == eParamiteMotions::Motion_15_Hiss)
        {
            SetCurrentMotion(eParamiteMotions::Motion_11_RunEnd);
        }

        if (GetNextMotion() == eParamiteMotions::Motion_18_RunningAttack)
        {
            SetCurrentMotion(eParamiteMotions::Motion_18_RunningAttack);
            SetNextMotion(-1);

            if (mAnim.mFlags.Get(AnimFlags::eFlipX))
            {
                mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
            }
        }

        if (GetNextMotion() == eParamiteMotions::Motion_6_Hop)
        {
            ToHop();
        }

        if (!field_142_bSnapped)
        {
            MapFollowMe_401D30(1);
            field_142_bSnapped = 1;
        }

        SetMusic();
        return;
    }

    field_142_bSnapped = 0;
    SetMusic();
}

void Paramite::Motion_4_Unknown()
{
    if (sNumCamSwappers_507668 <= 0)
    {
        if (sControlledCharacter == this)
        {
            SetCurrentMotion(mPreviousMotion);
            if (mLiftPoint)
            {
                // TODO: Correct type ??
                static_cast<LiftPoint*>(mLiftPoint)->field_12C_bMoving |= 1u;
            }
        }
        else
        {
            SetCurrentMotion(mPreviousMotion);
            if (mLiftPoint)
            {
                mXPos = FP_FromInteger((BaseAliveGameObjectCollisionLine->mRect.x + BaseAliveGameObjectCollisionLine->mRect.w) / 2);
                mYPos = FP_FromInteger(BaseAliveGameObjectCollisionLine->mRect.y);
            }
        }
    }
}

void Paramite::Motion_5_Turn()
{
    if (mAnim.mCurrentFrame == 0)
    {
        Sound(ParamiteSpeak::SlightStep_4);
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mAnim.mFlags.Toggle(AnimFlags::eFlipX);

        if (ToNextMotion())
        {
            // TODO: Check this
            mAnim.Set_Animation_Data(mAnim.mAnimRes);
        }
        else
        {
            ToIdle();
        }
    }
}

const FP sHopVelTable_4BBD28[16] = {
    FP_FromDouble(5.33526611328125),
    FP_FromDouble(5.8092041015625),
    FP_FromDouble(3.43768310546875),
    FP_FromDouble(3.6483001708984375),
    FP_FromDouble(5.434295654296875),
    FP_FromDouble(10.9881591796875),
    FP_FromDouble(10.020431518554688),
    FP_FromDouble(5.2939453125),
    FP_FromDouble(5.3142852783203125),
    FP_FromDouble(4.234375),
    FP_FromDouble(4.7738189697265625),
    FP_FromDouble(6.2389373779296875),
    FP_FromDouble(5.5633392333984375),
    FP_FromDouble(7.9202728271484375),
    FP_FromDouble(10.229019165039062),
    FP_FromDouble(5.77606201171875)};


void Paramite::Motion_6_Hop()
{
    EventBroadcast(kEventNoise, this);

    FP frameVelX = {};
    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        frameVelX = (mSpriteScale * -sHopVelTable_4BBD28[mAnim.mCurrentFrame]);
    }
    else
    {
        frameVelX = (mSpriteScale * sHopVelTable_4BBD28[mAnim.mCurrentFrame]);
    }

    mVelX = frameVelX;

    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(0.9)))
        {
            switch (pLine->mLineType)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eDynamicCollision_32:
                case eLineTypes::eBackgroundDynamicCollision_36:
                {
                    ToIdle();

                    BaseAliveGameObjectCollisionLine = pLine;

                    PSX_RECT bRect = VGetBoundingRect();
                    bRect.y += 5;
                    bRect.h += 5;
                    VOnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.y, bRect.h},
                        ObjListPlatforms_50766C,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
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
            field_124_XSpeed = FP_FromDouble(0.75);
            SetCurrentMotion(eParamiteMotions::Motion_12_Falling);
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

const FP State_7_Unknown_VelTable_4BBCA8[2] = {
    FP_FromDouble(2.2522430419921875),
    FP_FromDouble(5.6306304931640625)};

void Paramite::Motion_7_Unknown()
{
    EventBroadcast(kEventNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = mSpriteScale * -State_7_Unknown_VelTable_4BBCA8[mAnim.mCurrentFrame];
    }
    else
    {
        mVelX = mSpriteScale * State_7_Unknown_VelTable_4BBCA8[mAnim.mCurrentFrame];
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mPreviousMotion = 3;
        mBaseAliveGameObjectLastAnimFrame = 2;
        field_140_use_prev_motion = 1;
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
    }
}

const FP sWalkRunTransVelTable_4BBD18[3] = {
    FP_FromDouble(5.3964691162109375),
    FP_FromDouble(5.396453857421875),
    FP_FromDouble(5.396484375)};

void Paramite::Motion_8_WalkRunTransition()
{
    EventBroadcast(kEventNoise, this);

    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = mSpriteScale * -sWalkRunTransVelTable_4BBD18[mAnim.mCurrentFrame];
    }
    else
    {
        mVelX = mSpriteScale * sWalkRunTransVelTable_4BBD18[mAnim.mCurrentFrame];
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_2_Walking);
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
    }
}

const FP sWalkEndVelTable_4BBC98[3] = {
    FP_FromDouble(2.3350982666015625),
    FP_FromDouble(5.0355224609375),
    FP_FromDouble(6.70159912109375)};

void Paramite::Motion_9_WalkEnd()
{
    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = mSpriteScale * -sWalkEndVelTable_4BBC98[mAnim.mCurrentFrame];
    }
    else
    {
        mVelX = mSpriteScale * sWalkEndVelTable_4BBC98[mAnim.mCurrentFrame];
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
        EventBroadcast(kEventNoise, this);

        if (!mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
        {
            return;
        }
    }

    ToIdle();
}

const FP sRunBeginVelTable_4BBCF8[3] = {
    FP_FromDouble(1.8789520263671875),
    FP_FromDouble(2.15924072265625),
    FP_FromDouble(3.330413818359375)};

void Paramite::Motion_10_RunBegin()
{
    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = (mSpriteScale * -sRunBeginVelTable_4BBCF8[mAnim.mCurrentFrame]);
    }
    else
    {
        mVelX = mSpriteScale * sRunBeginVelTable_4BBCF8[mAnim.mCurrentFrame];
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mPreviousMotion = 3;
        mBaseAliveGameObjectLastAnimFrame = 2;
        field_140_use_prev_motion = 1;
    }

    EventBroadcast(kEventNoise, this);

    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
    }
}

const FP sRunEndVelTable_4BBD08[3] = {
    FP_FromDouble(3.11126708984375),
    FP_FromDouble(3.063446044921875),
    FP_FromDouble(3.43255615234375)};

void Paramite::Motion_11_RunEnd()
{
    if (mAnim.mFlags.Get(AnimFlags::eFlipX))
    {
        mVelX = mSpriteScale * -sRunEndVelTable_4BBD08[mAnim.mCurrentFrame];
    }
    else
    {
        mVelX = mSpriteScale * sRunEndVelTable_4BBD08[mAnim.mCurrentFrame];
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }

    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
    }
}

void Paramite::Motion_12_Falling()
{
    if (mVelX > FP_FromInteger(0))
    {
        mVelX -= (mSpriteScale * field_124_XSpeed);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (mSpriteScale * field_124_XSpeed);
        if (mVelX > FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
    {
        switch (pLine->mLineType)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eDynamicCollision_32:
            case eLineTypes::eBackgroundDynamicCollision_36:
            {
                ToIdle();

                BaseAliveGameObjectCollisionLine = pLine;

                PSX_RECT bRect = VGetBoundingRect();
                bRect.y += 5;
                bRect.h += 5;

                VOnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.w, bRect.h},
                    ObjListPlatforms_50766C,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);

                mXPos = hitX;
                mYPos = hitY;
                MapFollowMe_401D30(TRUE);
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

void Paramite::Motion_13_GameSpeakBegin()
{
    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT ourRect = VGetBoundingRect();

    if (abeRect.x <= ourRect.w
        && abeRect.w >= ourRect.x
        && abeRect.h >= ourRect.y
        && abeRect.y <= ourRect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero))
        {
            if (mSpriteScale == sActiveHero->mSpriteScale)
            {
                if (sActiveHero->mHealth > FP_FromInteger(0))
                {
                    SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, sActiveHero);
                }
                sActiveHero->VTakeDamage(this);
            }
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_14_PreHiss);
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        == CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Paramite::Motion_14_PreHiss()
{
    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero))
        {
            if (mSpriteScale == sActiveHero->mSpriteScale)
            {
                sActiveHero->VTakeDamage(this);
            }
        }
    }

    if (GetNextMotion() == eParamiteMotions::Motion_18_RunningAttack)
    {
        SetCurrentMotion(eParamiteMotions::Motion_18_RunningAttack);
        SetNextMotion(-1);
        if (mAnim.mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
        }
        else
        {
            mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
        }
    }

    if (GetNextMotion() == eParamiteMotions::Motion_22_Unknown)
    {
        SetCurrentMotion(eParamiteMotions::Motion_22_Unknown);
        SetNextMotion(-1);
    }

    if (GetNextMotion() == eParamiteMotions::Motion_15_Hiss)
    {
        SetCurrentMotion(eParamiteMotions::Motion_15_Hiss);
        SetNextMotion(-1);
    }

    if (mNextMotion != -1)
    {
        SetCurrentMotion(eParamiteMotions::Motion_17_GameSpeakEnd);
    }

    if (gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos)
        == CameraPos::eCamCurrent_0)
    {
        MusicController::static_PlayMusic(MusicController::MusicTypes::eType12, this, 0, 0);
    }
}

void Paramite::Motion_15_Hiss()
{
    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (mAnim.mCurrentFrame == 2)
    {
        Sound(ParamiteSpeak::Stay_1);
    }

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero))
        {
            if (mSpriteScale == sActiveHero->mSpriteScale)
            {
                sActiveHero->VTakeDamage(this);
            }
        }
    }

    if (GetNextMotion() == eParamiteMotions::Motion_18_RunningAttack)
    {
        SetCurrentMotion(eParamiteMotions::Motion_18_RunningAttack);
        SetNextMotion(-1);
        if (mAnim.mFlags.Get(AnimFlags::eFlipX))
        {
            mVelX = -(ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
        }
        else
        {
            mVelX = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(4));
        }
    }
    else if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_16_PostHiss);
    }
}

void Paramite::Motion_16_PostHiss()
{
    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero))
        {
            if (mSpriteScale == sActiveHero->mSpriteScale)
            {
                sActiveHero->VTakeDamage(this);
            }
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_14_PreHiss);
    }
}

void Paramite::Motion_17_GameSpeakEnd()
{
    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(sActiveHero))
        {
            if (mSpriteScale == sActiveHero->mSpriteScale)
            {
                sActiveHero->VTakeDamage(this);
            }
        }
    }
    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        if (!ToNextMotion())
        {
            mAnim.mFlags.Clear(AnimFlags::eFlipY);
            mAnim.mFlags.Clear(AnimFlags::eSwapXY);
            field_124_XSpeed = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            SetCurrentMotion(eParamiteMotions::Motion_0_Idle);
            MapFollowMe_401D30(TRUE);
        }
    }
}

void Paramite::Motion_18_RunningAttack()
{
    if (WallHit_401930(mSpriteScale * FP_FromInteger(10), mVelX))
    {
        BaseAliveGameObjectCollisionLine = nullptr;

        ToKnockBack();
        return;
    }

    mXPos += mVelX;
    MapFollowMe_401D30(FALSE);

    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h
        && mSpriteScale == sActiveHero->mSpriteScale)
    {
        sActiveHero->VTakeDamage(this);
    }

    if (mAnim.mCurrentFrame == 3)
    {
        Sound(ParamiteSpeak::CMon_or_Attack_0);
    }

    if (mAnim.mCurrentFrame == 11)
    {
        FP gridSizeDirected = {};
        if (mVelX >= FP_FromInteger(0))
        {
            gridSizeDirected = ScaleToGridSize(mSpriteScale);
        }
        else
        {
            gridSizeDirected = -ScaleToGridSize(mSpriteScale);
        }

        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (sCollisions->Raycast(
                mXPos + gridSizeDirected,
                mYPos - FP_FromInteger(10),
                mXPos + gridSizeDirected,
                mYPos + FP_FromInteger(10),
                &pLine,
                &hitX,
                &hitY,
                mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
        {
            if (pLine->mLineType == eLineTypes ::eDynamicCollision_32 ||
                pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PSX_RECT r = VGetBoundingRect();
                r.y += 5;
                r.h += 5;

                VOnCollisionWith(
                    {r.x, r.y},
                    {r.w, r.h},
                    ObjListPlatforms_50766C,
                    (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
            }
            mYPos = hitY;
            BaseAliveGameObjectCollisionLine = pLine;
        }
        else
        {
            BaseAliveGameObjectCollisionLine = nullptr;
            SetCurrentMotion(eParamiteMotions::Motion_12_Falling);
            field_124_XSpeed = FP_FromDouble(0.75);
            mYPos -= (mSpriteScale * FP_FromInteger(10));
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
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

void Paramite::Motion_19_Empty()
{
    // Empty
}

void Paramite::Motion_20_SurpriseWeb()
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
            mSpriteScale != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
        SetCurrentMotion(eParamiteMotions::Motion_21_WebLeaveDown);

        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            PSX_RECT bRect = VGetBoundingRect();
            bRect.y += 5;
            bRect.h += 5;

            VOnCollisionWith(
                {bRect.x, bRect.y},
                {bRect.w, bRect.h},
                ObjListPlatforms_50766C,
                (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
        }
    }
    else
    {
        mYPos += mVelY;
    }

    if (mAnim.mCurrentFrame == 0 || mAnim.mCurrentFrame == 3)
    {
        Sound(ParamiteSpeak::ClimbingWeb_6);
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

void Paramite::Motion_21_WebLeaveDown()
{
    if (mAnim.mCurrentFrame == 2)
    {
        Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        ToIdle();
    }
}

void Paramite::Motion_22_Unknown()
{
    const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h
        && mSpriteScale == sActiveHero->mSpriteScale)
    {
        sActiveHero->VTakeDamage(this);
    }

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        SetCurrentMotion(eParamiteMotions::Motion_14_PreHiss);
    }
}

void Paramite::Motion_23_Eating()
{
    if (mAnim.mCurrentFrame == 5)
    {
        SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Eating1, relive::SoundEffects::Eating2), 0);
    }

    if (mNextMotion != -1 && !ToNextMotion())
    {
        ToIdle();
    }
}

void Paramite::Motion_24_Struggle()
{
    if (mAnim.mCurrentFrame == 0)
    {
        SfxPlayMono(relive::SoundEffects::KillEffect, 0);
    }
}

void Paramite::Motion_25_Death()
{
    if (mAnim.mCurrentFrame == 0)
    {
        Sound(ParamiteSpeak::DoIt_2);
    }
}

} // namespace AO
