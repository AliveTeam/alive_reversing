#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
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
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "../relive_lib/GameObjects/Blood.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "DDCheat.hpp"
#include "Meat.hpp"
#include "CameraSwapper.hpp"
#include "PlatformBase.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Grid.hpp"
#include "BeeSwarm.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../AliveLibAE/Game.hpp"

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

Paramite::Paramite(relive::Path_Paramite* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eParamite);

    for (auto& animId : sParamiteMotionAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ParamiteWeb));

    mBaseGameObjectTlvInfo = tlvId;
    Animation_Init(GetAnimRes(AnimId::Paramite_Idle));

    field_12A_res_idx = 0;

    SetCanSetOffExplosives(true);
    field_114_timer = 0;
    mBrainSubState = 0;
    mNextMotion = eParamiteMotions::Motion_0_Idle;
    SetCanBeesChase(true);

    mCurrentMotion = eParamiteMotions::Motion_0_Idle;
    field_140_use_prev_motion = 0;

    mMeat = nullptr;

    mParamiteWeb = nullptr;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
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
    if (gCollisions->Raycast(
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
        mCurrentMotion = eParamiteMotions::Motion_0_Idle;
        mYPos = hitY;
    }

    mRGB.SetRGB(105, 105, 105);

    field_12C_tlvInfo = tlvId;

    if (!VIsFacingMe(gAbe))
    {
        GetAnimation().ToggleFlipX();
    }

    VStackOnObjectsOfType(ReliveTypes::eParamite);

    SetDoPurpleLightEffect(true);

    CreateShadow();
}

Paramite::~Paramite()
{
    if (mParamiteWeb)
    {
        mParamiteWeb->SetDead(true);
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
        Path::TLV_Delete(field_12C_tlvInfo);
    }
    else
    {
        Path::TLV_Reset(field_12C_tlvInfo);
    }

    SND_SEQ_Stop(SeqId::eParamiteNearby_30);

    MusicController::static_PlayMusic(MusicController::MusicTypes::eType0, this, 0, 0);
    MusicController::ClearObject(this);
}

void Paramite::VRender(OrderingTable& ot)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ot);
    }
}

bool Paramite::VTakeDamage(BaseGameObject* pFrom)
{
    if (mHealth <= FP_FromInteger(0))
    {
        return true;
    }

    if (mMeat)
    {
        mMeat->mBaseGameObjectRefCount--;
        mMeat = nullptr;
    }

    switch (pFrom->Type())
    {
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
            GetAnimation().SetRender(false);
            mHealth = FP_FromInteger(0);
            return true;
        }

        case ReliveTypes::eAbilityRing:
            return false;

        case ReliveTypes::eBeeSwarm:
            mHealth -= FP_FromDouble(0.2);
            if (mHealth > FP_FromInteger(0))
            {
                if (!BrainIs(&Paramite::Brain_2_Struggling))
                {
                    mNextMotion = eParamiteMotions::Motion_25_Death;
                    SetBrain(&Paramite::Brain_2_Struggling);
                    mBrainSubState = 0;
                }
            }
            else
            {
                mHealth = FP_FromInteger(0);
                SetBrain(&Paramite::Brain_3_Death);
                field_114_timer = MakeTimer(90);
                mCurrentMotion = eParamiteMotions::Motion_24_Struggle;
                VUpdateAnimData();
            }
            break;

        default:
        {
            mHealth = FP_FromInteger(0);
            SetBrain(&Paramite::Brain_3_Death);
            field_114_timer = MakeTimer(90);
            mCurrentMotion = eParamiteMotions::Motion_24_Struggle;
            VUpdateAnimData();

            relive_new Blood(
                mXPos,
                mYPos,
                FP_FromInteger(0),
                FP_FromInteger(5),
                GetSpriteScale(),
                50);
            break;
        }
    }
    return true;
}

void Paramite::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eDeathDrop)
        {
            SetDead(true);
            mHealth = FP_FromInteger(0);
        }
        pTlv = gMap.TLV_Get_At(pTlv, mXPos, mYPos, mXPos, mYPos);
    }
}

void Paramite::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

bool Paramite::VOnSameYLevel(BaseAnimatedWithPhysicsGameObject* pOther)
{
    return FP_Abs(pOther->mYPos - mYPos) < (GetSpriteScale() * FP_FromInteger(40));
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
        SND_SEQ_PlaySeq(SeqId::eParamiteNearby_30, 1, 0);
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
            SND_SEQ_Stop(SeqId::eParamiteNearby_30);
        }
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (FP_Abs(mXPos - gAbe->mXPos) > FP_FromInteger(1536) || FP_Abs(mYPos - gAbe->mYPos) > FP_FromInteger(480))
    {
        if (mDeleteWhenOutOfSight)
        {
            SetDead(true);
        }
        else
        {
            if (mMeat)
            {
                if (mMeat->VIsFalling() || mMeat->GetDead())
                {
                    mMeat->mBaseGameObjectRefCount--;
                    mMeat = nullptr;
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    SetBrain(&Paramite::Brain_0_Patrol);
                    mBrainSubState = 0;
                }
            }
        }

        GetAnimation().SetAnimate(false);
        GetAnimation().SetRender(false);

        // TODO: OG freed vram here
    }
    else
    {
        // TODO: OG alloc'd vram here

        GetAnimation().SetAnimate(true);
        GetAnimation().SetRender(true);


        const auto oldMotion = mCurrentMotion;
        mBrainSubState = (this->*mBrainState)();

        static auto oldBrain = mBrainState;
        if (mBrainState != oldBrain)
        {
            //LOG_INFO("Brain changed from " << GetOriginalFn(oldBrain, sParamiteBrainTable).fnName << " to " << GetOriginalFn(mBrainState, sParamiteBrainTable).fnName);
        }
        oldBrain = mBrainState;

        if (gDDCheat_ShowAI_Info)
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

        InvokeMemberFunction(this, sParamiteMotionTable, mCurrentMotion);

        if (oldx != mXPos || oldy != mYPos)
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
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
            mNextMotion = eParamiteMotions::None_m1;
            mCurrentMotion = mNextMotion;
            mbGotShot = false;
        }

        if (oldMotion == mCurrentMotion)
        {
            if (field_140_use_prev_motion)
            {
                mCurrentMotion = mPreviousMotion;
                VUpdateAnimData();
                GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
                field_140_use_prev_motion = 0;
            }
        }
        else
        {
            VUpdateAnimData();
            if (static_cast<eParamiteMotions>(oldMotion) == eParamiteMotions::Motion_4_Unknown)
            {
                GetAnimation().SetFrame(mBaseAliveGameObjectLastAnimFrame);
            }
        }
    }
}


void Paramite::ToIdle()
{
    GetAnimation().SetFlipY(false);
    GetAnimation().SetSwapXY(false);

    field_124_XSpeed = FP_FromInteger(0);
    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    mCurrentMotion = eParamiteMotions::Motion_0_Idle;

    MapFollowMe(true);
}

s16 Paramite::ToNextMotion()
{
    switch (mNextMotion)
    {
        case eParamiteMotions::Motion_5_Turn:
        case eParamiteMotions::Motion_23_Eating:
        case eParamiteMotions::Motion_25_Death:
        case eParamiteMotions::Motion_13_GameSpeakBegin:
            mCurrentMotion = mNextMotion;
            mNextMotion = eParamiteMotions::None_m1;
            return 1;

        case eParamiteMotions::Motion_3_Running:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromDouble(3.5));
                mCurrentMotion = eParamiteMotions::Motion_10_RunBegin;
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromDouble(3.5));
                mCurrentMotion = eParamiteMotions::Motion_10_RunBegin;
            }
            mNextMotion = eParamiteMotions::None_m1;
            return 1;

        case eParamiteMotions::Motion_2_Walking:
            if (GetAnimation().GetFlipX())
            {
                mVelX = -ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(7);
                mCurrentMotion = eParamiteMotions::Motion_1_WalkBegin;
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(7));
                mCurrentMotion = eParamiteMotions::Motion_1_WalkBegin;
            }
            mNextMotion = eParamiteMotions::None_m1;
            return 1;

        case eParamiteMotions::Motion_15_Hiss:
            mCurrentMotion = eParamiteMotions::Motion_13_GameSpeakBegin;
            return 1;

        case eParamiteMotions::Motion_18_RunningAttack:
            mCurrentMotion = eParamiteMotions::Motion_18_RunningAttack;
            mNextMotion = eParamiteMotions::None_m1;
            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            return 1;

        case eParamiteMotions::Motion_6_Hop:
            VOnTrapDoorOpen();
            mCurrentMotion = eParamiteMotions::Motion_6_Hop;
            mNextMotion = eParamiteMotions::None_m1;
            BaseAliveGameObjectLastLineYPos = mYPos;
            mVelY = FP_FromDouble(-7.2) * GetSpriteScale();
            mYPos += mVelY;
            BaseAliveGameObjectCollisionLine = nullptr;
            return 1;

        default:
            return 0;
    }
}

void Paramite::VUpdateAnimData()
{
    GetAnimation().Set_Animation_Data(GetAnimRes(sParamiteMotionAnimIds[static_cast<u32>(mCurrentMotion)]));
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
                if (FP_Abs(pOther->mYPos - mYPos) < (GetSpriteScale() * FP_FromInteger(100)))
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
    mXPos = FP_FromInteger((FP_GetExponent(nextX) & 0xFC00) + SnapToXGrid_AO(GetSpriteScale(), FP_GetExponent(nextX) & 0x3FF));

    MapFollowMe(1);

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
        mCurrentMotion = eParamiteMotions::Motion_12_Falling;
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
                    && !WallHit(mYPos, pMeat->mXPos - mXPos))
                {
                    if (!pMeat->mPathLine)
                    {
                        return pMeat;
                    }

                    if (FP_Abs(pMeat->mYPos - mYPos) < (GetSpriteScale() * FP_FromInteger(100)))
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
    if (!BaseAliveGameObjectCollisionLine)
    {
        mCurrentMotion = eParamiteMotions::Motion_12_Falling;
        BaseAliveGameObjectLastLineYPos = mYPos;
        return;
    }
    
    BaseGameObject* pPlatform = sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId);
    const FP old_x = mXPos;

    BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
        &mXPos,
        &mYPos,
        mVelX);
    if (BaseAliveGameObjectCollisionLine)
    {
        if (pPlatform)
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
                const PSX_RECT bRect = VGetBoundingRect();

                OnCollisionWith(
                    {bRect.x, static_cast<s16>(bRect.y + 5)},
                    {bRect.w, static_cast<s16>(bRect.h + 5)},
                    gPlatformsArray);
            }
        }
    }
    else
    {
        VOnTrapDoorOpen();
        BaseAliveGameObjectLastLineYPos = mYPos;
        mCurrentMotion = eParamiteMotions::Motion_12_Falling;
        mXPos = mVelX + old_x;
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

    s16 volRight = stru_4CDD98[static_cast<s16>(idx)].mDefaultVolume;
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
            const FP tempVol = (FP_FromRaw(stru_4CDD98[static_cast<s16>(idx)].mDefaultVolume) / FP_FromInteger(3));
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

    SFX_SfxDefinition_Play(stru_4CDD98[static_cast<s16>(idx)], volLeft, volRight, -520, -520);
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

    mCurrentMotion = eParamiteMotions::Motion_6_Hop;

    mNextMotion = eParamiteMotions::None_m1;

    BaseAliveGameObjectLastLineYPos = mYPos;
    mVelY = -FP_FromDouble(7.2) * GetSpriteScale();
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
        SetDead(true);
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    switch (mBrainSubState)
    {
        case Brain_0_Patrol::eBrain0_Inactive_0:
            if (gNumCamSwappers > 0)
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_0_Idle;
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

            if (!VOnSameYLevel(gAbe)
                || GetSpriteScale() != gAbe->GetSpriteScale()
                || WallHit(GetSpriteScale() * FP_FromInteger(5), gAbe->mXPos - mXPos))
            {
                mWaitTimer = MakeTimer(Math_RandomRange(45, 135));
                return Brain_0_Patrol::eBrain0_Idle_12;
            }

            if (EventGet(kEventAbeOhm) && VIsFacingMe(gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_15_Hiss;
                return Brain_0_Patrol::eBrain0_Panic_15;
            }

            // TODO: Inlined HandleEnemyStopper but the directional logic isn't quite the same
            // so can't use it.
            BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper);
            if (BaseAliveGameObjectPathTLV)
            {
                auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
                if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && gAbe->mXPos < mXPos) || (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && gAbe->mXPos > mXPos))
                {
                    if (!SwitchStates_Get(pStopper->mSwitchId))
                    {
                        return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                    }
                }
            }

            if ((AnotherParamiteNear() || gAbe->mHealth <= FP_FromInteger(0)) && GetSpriteScale() == gAbe->GetSpriteScale())
            {
                Sound(ParamiteSpeak::Howdy_5);
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                mAttackTimer = MakeTimer(mGroupChaseDelay);
                return Brain_0_Patrol::eBrain0_Inactive_0;
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(6), gAbe))
            {
                if (!VIsObjNearby(kGridSize * FP_FromInteger(8), gAbe))
                {
                    if (!VIsFacingMe(gAbe))
                    {
                        if ((GetAnimation().GetFlipX() && !Check_IsOnEndOfLine(0, 2)) || (!GetAnimation().GetFlipX() && !Check_IsOnEndOfLine(1, 2)))
                        {
                            mNextMotion = eParamiteMotions::Motion_5_Turn;
                            return Brain_0_Patrol::eBrain0_TurningForAbe_6;
                        }
                    }
                    else
                    {
                        if ((GetAnimation().GetFlipX() && !Check_IsOnEndOfLine(1, 2)) || (!GetAnimation().GetFlipX() && !Check_IsOnEndOfLine(0, 2)))
                        {
                            mNextMotion = eParamiteMotions::Motion_2_Walking;
                            return Brain_0_Patrol::eBrain0_ApproachingAbe_4;
                        }
                    }
                }

                if (!VIsFacingMe(gAbe))
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_0_Patrol::eBrain0_TurningForAbe_6;
                }

                return mBrainSubState;
            }
            else if (VIsFacingMe(gAbe))
            {
                if ((GetAnimation().GetFlipX() && Check_IsOnEndOfLine(0, 2)) || (!GetAnimation().GetFlipX() && Check_IsOnEndOfLine(1, 2)))
                {
                    mNextMotion = eParamiteMotions::Motion_13_GameSpeakBegin;
                    return Brain_0_Patrol::eBrain0_StuckToWall_8;
                }

                Sound(ParamiteSpeak::Howdy_5);
                mNextMotion = eParamiteMotions::Motion_5_Turn;
                return Brain_0_Patrol::eBrain0_TurningForAbe_6;
            }
            else if (!VIsObjNearby(kGridSize * FP_FromInteger(4), gAbe))
            {
                if ((GetAnimation().GetFlipX() && Check_IsOnEndOfLine(1, 2)) || (!GetAnimation().GetFlipX() && Check_IsOnEndOfLine(0, 2)))
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }

                mNextMotion = eParamiteMotions::Motion_2_Walking;
                return Brain_0_Patrol::eBrain0_FearingAbe_2;
            }
            else
            {
                if ((GetAnimation().GetFlipX() && Check_IsOnEndOfLine(1, 2)) || (!GetAnimation().GetFlipX() && Check_IsOnEndOfLine(0, 2)))
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
                mNextMotion = eParamiteMotions::Motion_3_Running;
                return Brain_0_Patrol::eBrain0_RunningFromAbe_3;
            }
            break;
        }

        case Brain_0_Patrol::eBrain0_FearingAbe_2:
            if (!VOnSameYLevel(gAbe) || GetSpriteScale() != gAbe->GetSpriteScale())
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine(1, 2))
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine(0, 2))
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
            }

            if (AnotherParamiteNear())
            {
                Sound(ParamiteSpeak::Howdy_5);
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                mAttackTimer = MakeTimer(mGroupChaseDelay);
                return 0;
            }

            if (VIsObj_GettingNear_On_X(gAbe) && VIsObjNearby(kGridSize * FP_FromInteger(4), gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_3_Running;
                return Brain_0_Patrol::eBrain0_RunningFromAbe_3;
            }

            if (VIsObj_GettingNear_On_X(gAbe))
            {
                return mBrainSubState;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(7), gAbe))
            {
                return mBrainSubState;
            }

            mNextMotion = eParamiteMotions::Motion_0_Idle;
            return Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case Brain_0_Patrol::eBrain0_RunningFromAbe_3:
            if (!VOnSameYLevel(gAbe) || GetSpriteScale() != gAbe->GetSpriteScale())
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine(1, 3))
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if ((WallHit(GetSpriteScale() * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine(0, 3)))
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_0_Patrol::eBrain0_HittingAbe_7;
                }
            }

            if (AnotherParamiteNear())
            {
                Sound(ParamiteSpeak::Howdy_5);
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                mAttackTimer = MakeTimer(mGroupChaseDelay);
                return 0;
            }

            if (VIsObj_GettingNear_On_X(gAbe))
            {
                return mBrainSubState;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(7), gAbe))
            {
                return mBrainSubState;
            }

            mNextMotion = eParamiteMotions::Motion_2_Walking;
            return Brain_0_Patrol::eBrain0_FearingAbe_2;

        case Brain_0_Patrol::eBrain0_ApproachingAbe_4:
            if (!VOnSameYLevel(gAbe) || GetSpriteScale() != gAbe->GetSpriteScale())
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (AnotherParamiteNear())
            {
                Sound(ParamiteSpeak::Howdy_5);
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                mAttackTimer = MakeTimer(mGroupChaseDelay);
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                return Brain_0_Patrol::eBrain0_Inactive_0;
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (HandleEnemyStopper(-2, relive::Path_EnemyStopper::StopDirection::Left))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine(1, 2))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (HandleEnemyStopper(2, relive::Path_EnemyStopper::StopDirection::Right))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine(0, 2))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }
            }

            if (VIsObj_GettingNear_On_X(gAbe) || VIsObjNearby(kGridSize * FP_FromInteger(10), gAbe))
            {
                if (!VIsObj_GettingNear_On_X(gAbe))
                {
                    return mBrainSubState;
                }

                if (!VIsObjNearby(kGridSize * FP_FromInteger(8), gAbe))
                {
                    return mBrainSubState;
                }
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }
            mNextMotion = eParamiteMotions::Motion_3_Running;
            return Brain_0_Patrol::eBrain0_StopApproachingAbe_5;

        case Brain_0_Patrol::eBrain0_StopApproachingAbe_5:
            if (!VOnSameYLevel(gAbe) || GetSpriteScale() != gAbe->GetSpriteScale())
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (AnotherParamiteNear())
            {
                Sound(ParamiteSpeak::Howdy_5);
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                mAttackTimer = MakeTimer(mGroupChaseDelay);
                SetBrain(&Paramite::Brain_4_ChasingAbe);
                return 0;
            }

            if (mVelX < FP_FromInteger(0))
            {
                if (HandleEnemyStopper(-2, relive::Path_EnemyStopper::StopDirection::Left))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(5), -kGridSize) || Check_IsOnEndOfLine(1, 3))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (HandleEnemyStopper(2, relive::Path_EnemyStopper::StopDirection::Right))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }

                if (WallHit(GetSpriteScale() * FP_FromInteger(5), kGridSize) || Check_IsOnEndOfLine(0, 3))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_0_Patrol::eBrain0_IdleForAbe_1;
                }
            }

            if (!VIsObj_GettingNear_On_X(gAbe))
            {
                return mBrainSubState;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(8), gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(10), gAbe))
            {
                return mBrainSubState;
            }

            mNextMotion = eParamiteMotions::Motion_2_Walking;
            return Brain_0_Patrol::eBrain0_ApproachingAbe_4;

        case Brain_0_Patrol::eBrain0_TurningForAbe_6:
            if (mCurrentMotion != eParamiteMotions::Motion_5_Turn || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_0_Idle;
            return Brain_0_Patrol::eBrain0_IdleForAbe_1;

        case Brain_0_Patrol::eBrain0_HittingAbe_7:
            if (mCurrentMotion != eParamiteMotions::Motion_5_Turn || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_13_GameSpeakBegin;
            return Brain_0_Patrol::eBrain0_StuckToWall_8;

        case Brain_0_Patrol::eBrain0_StuckToWall_8:
            mMeat = FindMeat();
            if (mMeat)
            {
                SetBrain(&Paramite::Brain_5_SpottedMeat);
                mMeat->mBaseGameObjectRefCount++;
                return 0;
            }

            if (!VOnSameYLevel(gAbe) || GetSpriteScale() != gAbe->GetSpriteScale())
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(3), gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_18_RunningAttack;
                return Brain_0_Patrol::eBrain0_Attacking_9;
            }

            if (VIsObj_GettingNear_On_X(gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_15_Hiss;
            }
            else
            {
                if (!VIsObjNearby(kGridSize * FP_FromInteger(8), gAbe))
                {
                    mNextMotion = eParamiteMotions::Motion_2_Walking;
                    return Brain_0_Patrol::eBrain0_ApproachingAbe_4;
                }
            }

            if (mCurrentMotion != eParamiteMotions::Motion_14_PreHiss)
            {
                return mBrainSubState;
            }

            if (Math_NextRandom() >= 6u)
            {
                return mBrainSubState;
            }

            mNextMotion = eParamiteMotions::Motion_15_Hiss;
            return mBrainSubState;

        case Brain_0_Patrol::eBrain0_Attacking_9:
            if (mCurrentMotion != eParamiteMotions::Motion_18_RunningAttack
                || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_0_Idle;
            if (gAbe->mHealth > FP_FromInteger(0))
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

            if (VOnSameYLevel(gAbe)
                && GetSpriteScale() == gAbe->GetSpriteScale()
                && !WallHit(GetSpriteScale() * FP_FromInteger(5), gAbe->mXPos - mXPos))
            {
                return Brain_0_Patrol::eBrain0_IdleForAbe_1;
            }

            if (EventGet(kEventAbeOhm))
            {
                mNextMotion = eParamiteMotions::Motion_15_Hiss;
                return Brain_0_Patrol::eBrain0_Panic_15;
            }

            if (mWaitTimer <= static_cast<s32>(sGnFrame))
            {
                if (Math_NextRandom() >= 6u)
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_0_Patrol::eBrain0_Turning_14;
                }
                else
                {
                    mNextMotion = eParamiteMotions::Motion_13_GameSpeakBegin;
                    return Brain_0_Patrol::eBrain0_IdleAnimation_13;
                }
            }
            return mBrainSubState;

        case Brain_0_Patrol::eBrain0_IdleAnimation_13:
            if (mCurrentMotion != eParamiteMotions::Motion_14_PreHiss
                || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_0_Idle;
            mWaitTimer = MakeTimer(Math_RandomRange(45, 135));
            return Brain_0_Patrol::eBrain0_Idle_12;

        case Brain_0_Patrol::eBrain0_Turning_14:
            if (mCurrentMotion != eParamiteMotions::Motion_5_Turn
                || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            mWaitTimer = MakeTimer(Math_RandomRange(45, 135));
            return Brain_0_Patrol::eBrain0_Idle_12;

        case Brain_0_Patrol::eBrain0_Panic_15:
            if (!EventGet(kEventAbeOhm))
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_0_Patrol::eBrain0_Idle_12;
            }
            mNextMotion = eParamiteMotions::Motion_15_Hiss;
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
        SetDead(true);
    }

    switch (mBrainSubState)
    {
        case Brain_1_SurpriseWeb::eBrain1_Loading_0:
        {
            if (gNumCamSwappers > 0)
            {
                return mBrainSubState;
            }

            mCurrentMotion = eParamiteMotions::Motion_0_Idle;

            MapFollowMe(true);
            auto pWeb = relive_new ParamiteWeb(
                mXPos,
                FP_GetExponent(mYPos) - 20,
                FP_GetExponent(mYPos) - 10,
                GetSpriteScale());
            if (pWeb)
            {
                pWeb->mBaseGameObjectRefCount++;
                mParamiteWeb = pWeb;
            }

            if (gAbe->mXPos >= mXPos)
            {
                return Brain_1_SurpriseWeb::eBrain1_AppearingLeft_2;
            }
            return Brain_1_SurpriseWeb::eBrain1_AppearingRight_1;
        }

        case Brain_1_SurpriseWeb::eBrain1_AppearingRight_1:
            if (gAbe->mXPos <= mXPos
                || GetSpriteScale() != gAbe->GetSpriteScale()
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
            GetAnimation().SetFlipX(false);
            field_114_timer = MakeTimer(mSurpriseWebDelayTimer);
            return Brain_1_SurpriseWeb::eBrain1_StartAnimation_3;

        case Brain_1_SurpriseWeb::eBrain1_AppearingLeft_2:
            if (gAbe->mXPos >= mXPos
                || GetSpriteScale() != gAbe->GetSpriteScale()
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
            GetAnimation().SetFlipX(true);
            field_114_timer = MakeTimer(mSurpriseWebDelayTimer);
            return Brain_1_SurpriseWeb::eBrain1_StartAnimation_3;

        case Brain_1_SurpriseWeb::eBrain1_StartAnimation_3:
            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mVelY = FP_FromInteger(0);
            mCurrentMotion = eParamiteMotions::Motion_20_SurpriseWeb;
            return Brain_1_SurpriseWeb::eBrain1_StateLoop1_4;

        case Brain_1_SurpriseWeb::eBrain1_StateLoop1_4:
            mParamiteWeb->mTtlRemainder = FP_GetExponent(FP_Abs(mYPos)) - 10;
            mParamiteWeb->mYPos = FP_FromInteger(mParamiteWeb->mTtlRemainder);
            if (mCurrentMotion == eParamiteMotions::Motion_0_Idle)
            {
                mParamiteWeb->mEnabled = true;
                mParamiteWeb->mBaseGameObjectRefCount--;
                mParamiteWeb = nullptr;
                SetBrain(&Paramite::Brain_0_Patrol);
                return Brain_0_Patrol::eBrain0_Inactive_0;
            }

            if (mVelY < (GetSpriteScale() * FP_FromInteger(8)))
            {
                mVelY += (GetSpriteScale() * FP_FromDouble(0.5));
                return mBrainSubState;
            }
            return Brain_1_SurpriseWeb::eBrain1_StateLoop2_5;

        case Brain_1_SurpriseWeb::eBrain1_StateLoop2_5:
            mParamiteWeb->mTtlRemainder = FP_GetExponent(FP_Abs(mYPos)) - 10;
            mParamiteWeb->mYPos = FP_FromInteger(mParamiteWeb->mTtlRemainder);
            if (mCurrentMotion != eParamiteMotions::Motion_0_Idle)
            {
                if (mVelY <= (GetSpriteScale() * FP_FromInteger(-1)))
                {
                    return Brain_1_SurpriseWeb::eBrain1_StateLoop1_4;
                }
                else
                {
                    mVelY -= (GetSpriteScale() * FP_FromInteger(1));
                    return mBrainSubState;
                }
            }
            else
            {
                mParamiteWeb->mEnabled = true;
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
        SetDead(true);
    }

    if (IsBeeSwarmChasingMe_4022B0())
    {
        switch (mBrainSubState)
        {
            case Brain_2_Struggling::eBrain2_SetTimer_0:
                field_114_timer = MakeTimer(30);
                return 1;

            case Brain_2_Struggling::eBrain2_Hiss_1:
                if (field_114_timer <= static_cast<s32>(sGnFrame))
                {
                    mCurrentMotion = eParamiteMotions::Motion_13_GameSpeakBegin;
                    mNextMotion = eParamiteMotions::Motion_15_Hiss;
                    return Brain_2_Struggling::eBrain2_Turn_2;
                }
                break;

            case Brain_2_Struggling::eBrain2_Turn_2:
                if (mCurrentMotion == eParamiteMotions::Motion_15_Hiss)
                {
                    if (GetAnimation().GetIsLastFrame())
                    {
                        mNextMotion = eParamiteMotions::Motion_5_Turn;
                        return Brain_2_Struggling::eBrain2_Death_3;
                    }
                }
                break;

            case Brain_2_Struggling::eBrain2_Death_3:
                if (mCurrentMotion == eParamiteMotions::Motion_5_Turn)
                {
                    if (GetAnimation().GetIsLastFrame())
                    {
                        mNextMotion = eParamiteMotions::Motion_25_Death;
                        field_114_timer = MakeTimer(30);
                        return 1;
                    }
                }
                break;
            default:
                break;
        }
        return mBrainSubState;
    }
    else if (mCurrentMotion == eParamiteMotions::Motion_5_Turn)
    {
        mNextMotion = eParamiteMotions::Motion_0_Idle;
        SetBrain(&Paramite::Brain_0_Patrol);
        return Brain_0_Patrol::eBrain0_Inactive_0;
    }
    else
    {
        mCurrentMotion = eParamiteMotions::Motion_0_Idle;
        mNextMotion = eParamiteMotions::None_m1;
        SetBrain(&Paramite::Brain_0_Patrol);
        return mBrainSubState;
    }
}

s16 Paramite::Brain_3_Death()
{
    if (static_cast<s32>(sGnFrame) < field_114_timer && (field_114_timer < static_cast<s32>(sGnFrame) + 80))
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.008));
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
        SetDead(true);
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
        SetDead(true);
    }

    if (mAttackTimer <= static_cast<s32>(sGnFrame)
        && (!VOnSameYLevel(gAbe)
            || GetSpriteScale() != gAbe->GetSpriteScale())
        && gAbe->mHealth > FP_FromInteger(0))
    {
        mNextMotion = eParamiteMotions::Motion_0_Idle;
        SetBrain(&Paramite::Brain_0_Patrol);
        return 0;
    }

    if (EventGet(kEventAbeOhm))
    {
        mNextMotion = eParamiteMotions::Motion_15_Hiss;
        return Brain_4_ChasingAbe::eBrain4_Panic_15;
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    switch (mBrainSubState)
    {
        case Brain_4_ChasingAbe::eBrain4_Inactive_0:
            if (!VOnSameYLevel(gAbe))
            {
                return Brain_4_ChasingAbe::eBrain4_Inactive_0;
            }

            if (gAbe->mHealth > FP_FromInteger(0))
            {
                if (VIsFacingMe(gAbe))
                {
                    if (mHissBeforeAttack)
                    {
                        field_114_timer = MakeTimer(Math_RandomRange(0, 6));
                        return Brain_4_ChasingAbe::eBrain4_ToWarning_2;
                    }
                    else
                    {
                        field_114_timer = MakeTimer(mAloneChaseDelay);
                        return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                    }
                }
                else
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_4_ChasingAbe::eBrain4_CloseAttack_4;
                }
            }
            else
            {
                if (VIsFacingMe(gAbe))
                {
                    if (VIsObjNearby(kGridSize, gAbe))
                    {
                        mNextMotion = eParamiteMotions::Motion_23_Eating;
                        field_114_timer = MakeTimer(28);
                        return Brain_4_ChasingAbe::eBrain4_Eating_13;
                    }

                    if (GetAnimation().GetFlipX())
                    {
                        if (Check_IsOnEndOfLine(1, 1))
                        {
                            mNextMotion = eParamiteMotions::Motion_6_Hop;
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    else
                    {
                        if (Check_IsOnEndOfLine(0, 1))
                        {
                            mNextMotion = eParamiteMotions::Motion_6_Hop;
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    mNextMotion = eParamiteMotions::Motion_2_Walking;
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
                else
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_4_ChasingAbe::eBrain4_Turning_10;
                }
            }
            break;

        case Brain_4_ChasingAbe::eBrain4_Attacking_1:
            if (mCurrentMotion != eParamiteMotions::Motion_18_RunningAttack
                || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (gAbe->mHealth > FP_FromInteger(0))
            {
                if (VIsFacingMe(gAbe))
                {
                    if (VIsObjNearby(kGridSize * FP_FromInteger(4), gAbe))
                    {
                        if (WallHit(GetSpriteScale() * FP_FromInteger(10), gAbe->mXPos - mXPos))
                        {
                            mNextMotion = eParamiteMotions::Motion_0_Idle;
                            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                        }
                        else
                        {
                            mNextMotion = eParamiteMotions::Motion_18_RunningAttack;
                            return Brain_4_ChasingAbe::eBrain4_Attacking_1;
                        }
                    }

                    if (GetAnimation().GetFlipX())
                    {
                        if (Check_IsOnEndOfLine(1, 1))
                        {
                            mNextMotion = eParamiteMotions::Motion_6_Hop;
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    else
                    {
                        if (Check_IsOnEndOfLine(0, 1))
                        {
                            mNextMotion = eParamiteMotions::Motion_6_Hop;
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    mNextMotion = eParamiteMotions::Motion_3_Running;
                    return Brain_4_ChasingAbe::eBrain4_Chasing_7;
                }
                else
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_4_ChasingAbe::eBrain4_QuickAttack_6;
                }
            }
            else
            {
                if (VIsFacingMe(gAbe))
                {
                    if (VIsObjNearby(kGridSize, gAbe))
                    {
                        mNextMotion = eParamiteMotions::Motion_23_Eating;
                        field_114_timer = MakeTimer(28);
                        return Brain_4_ChasingAbe::eBrain4_Eating_13;
                    }

                    if (GetAnimation().GetFlipX())
                    {
                        if (Check_IsOnEndOfLine(1, 1))
                        {
                            mNextMotion = eParamiteMotions::Motion_6_Hop;
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    else
                    {
                        if (Check_IsOnEndOfLine(0, 1))
                        {
                            mNextMotion = eParamiteMotions::Motion_6_Hop;
                            return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                        }
                    }
                    mNextMotion = eParamiteMotions::Motion_2_Walking;
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
                else
                {
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_4_ChasingAbe::eBrain4_Turning_10;
                }
            }
            break;

        case Brain_4_ChasingAbe::eBrain4_ToWarning_2:
            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_15_Hiss;
            field_114_timer = MakeTimer(mAloneChaseDelay);
            return Brain_4_ChasingAbe::eBrain4_Warning_3;

        case Brain_4_ChasingAbe::eBrain4_Warning_3:
            if (mCurrentMotion != eParamiteMotions::Motion_15_Hiss)
            {
                return mBrainSubState;
            }
            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;

        case Brain_4_ChasingAbe::eBrain4_CloseAttack_4:
            if (mCurrentMotion != eParamiteMotions::Motion_5_Turn
                || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (mHissBeforeAttack)
            {
                field_114_timer = MakeTimer(Math_RandomRange(0, 6));
                return Brain_4_ChasingAbe::eBrain4_ToWarning_2;
            }
            else
            {
                field_114_timer = MakeTimer(mAloneChaseDelay);
                return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
            }
            break;

        case Brain_4_ChasingAbe::eBrain4_ToChasing_5:
            if (!VOnSameYLevel(gAbe))
            {
                return Brain_4_ChasingAbe::eBrain4_Inactive_0;
            }

            if (!VIsFacingMe(gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_5_Turn;
                return 6;
            }

            // TODO: Inlined HandleEnemyStopper but the directional logic isn't quite the same
            // so can't use it.
            BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                FP_GetExponent(mXPos),
                FP_GetExponent(mYPos),
                ReliveTypes::eEnemyStopper);
            if (BaseAliveGameObjectPathTLV)
            {
                auto pStopper = static_cast<relive::Path_EnemyStopper*>(BaseAliveGameObjectPathTLV);
                if ((pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Left && gAbe->mXPos < mXPos) || (pStopper->mStopDirection == relive::Path_EnemyStopper::StopDirection::Right && gAbe->mXPos > mXPos))
                {
                    if (!SwitchStates_Get(pStopper->mSwitchId))
                    {
                        return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                    }
                }
            }

            if (VIsObjNearby(kGridSize * FP_FromInteger(4), gAbe))
            {
                if (WallHit(GetSpriteScale() * FP_FromInteger(10), gAbe->mXPos - mXPos))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                }
                else
                {
                    mNextMotion = eParamiteMotions::Motion_18_RunningAttack;
                    return Brain_4_ChasingAbe::eBrain4_Attacking_1;
                }
            }

            if (field_114_timer > static_cast<s32>(sGnFrame) && mHissBeforeAttack)
            {
                return mBrainSubState;
            }

            if (GetAnimation().GetFlipX())
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eParamiteMotions::Motion_6_Hop;
                    return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                }
            }
            else if (!GetAnimation().GetFlipX())
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eParamiteMotions::Motion_6_Hop;
                    return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                }
            }

            mNextMotion = eParamiteMotions::Motion_3_Running;
            return Brain_4_ChasingAbe::eBrain4_Chasing_7;

        case Brain_4_ChasingAbe::eBrain4_QuickAttack_6:
            if (mCurrentMotion != eParamiteMotions::Motion_5_Turn
                || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }

            if (mHissBeforeAttack)
            {
                mNextMotion = eParamiteMotions::Motion_15_Hiss;
            }
            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;

        case Brain_4_ChasingAbe::eBrain4_Chasing_7:
        {
            if (!VOnSameYLevel(gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_4_ChasingAbe::eBrain4_Inactive_0;
            }

            if (!VIsFacingMe(gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_5_Turn;
                return Brain_4_ChasingAbe::eBrain4_TurningWhileChasing_9;
            }

            const s16 x_exp = FP_GetExponent(mXPos);
            const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_AO(GetSpriteScale(), x_exp & 0x3FF);
            if (mVelX < FP_FromInteger(0))
            {
                if (HandleEnemyStopper(-2, relive::Path_EnemyStopper::StopDirection::Left))
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                }

                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine(1, 1))
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
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
                }

                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine(0, 1))
                {
                    ToHop();
                    return Brain_4_ChasingAbe::eBrain4_Jumping_8;
                }
                else
                {
                    // Drop through to idle/attack case
                }
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(4), gAbe))
            {
                return mBrainSubState;
            }

            if (WallHit(GetSpriteScale() * FP_FromInteger(10), gAbe->mXPos - mXPos))
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_4_ChasingAbe::eBrain4_ToChasing_5;
            }
            else
            {
                mNextMotion = eParamiteMotions::Motion_18_RunningAttack;
                return Brain_4_ChasingAbe::eBrain4_Attacking_1;
            }
        }
        break;

        case Brain_4_ChasingAbe::eBrain4_Jumping_8:
            if (mCurrentMotion != eParamiteMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;

        case Brain_4_ChasingAbe::eBrain4_TurningWhileChasing_9:
            if (mCurrentMotion != eParamiteMotions::Motion_5_Turn
                || !(GetAnimation().GetIsLastFrame()))
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_0_Idle;
            return Brain_4_ChasingAbe::eBrain4_ToChasing_5;

        case Brain_4_ChasingAbe::eBrain4_Turning_10:
            if (mCurrentMotion != eParamiteMotions::Motion_5_Turn
                || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (VIsObjNearby(kGridSize, gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_23_Eating;
                field_114_timer = MakeTimer(28);
                return Brain_4_ChasingAbe::eBrain4_Eating_13;
            }

            if (!GetAnimation().GetFlipX())
            {
                if (!Check_IsOnEndOfLine(0, 1))
                {
                    mNextMotion = eParamiteMotions::Motion_2_Walking;
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
            }
            else if (GetAnimation().GetFlipX())
            {
                if (!Check_IsOnEndOfLine(1, 1))
                {
                    mNextMotion = eParamiteMotions::Motion_2_Walking;
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
            }
            mNextMotion = eParamiteMotions::Motion_6_Hop;
            return Brain_4_ChasingAbe::eBrain4_Jumping_8;

        case Brain_4_ChasingAbe::eBrain4_Walking_11:
        {
            if (!VIsFacingMe(gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_5_Turn;
                return Brain_4_ChasingAbe::eBrain4_Turning_10;
            }

            if (!VOnSameYLevel(gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_4_ChasingAbe::eBrain4_Walking_11;
            }

            if (VIsObjNearby(kGridSize, gAbe))
            {
                mNextMotion = eParamiteMotions::Motion_23_Eating;
                field_114_timer = MakeTimer(28);
                return Brain_4_ChasingAbe::eBrain4_Eating_13;
            }

            const s16 x_exp = FP_GetExponent(mXPos);
            const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_AO(GetSpriteScale(), x_exp & 0x3FF);
            if (mVelX > FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine(0, 1))
                {
                    ToHop();
                    return Brain_4_ChasingAbe::eBrain4_WalkingToHop_12;
                }
            }
            else if (mVelX < FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine(1, 1))
                {
                    ToHop();
                    return Brain_4_ChasingAbe::eBrain4_WalkingToHop_12;
                }
            }
        }
            return mBrainSubState;

        case Brain_4_ChasingAbe::eBrain4_WalkingToHop_12:
            if (mCurrentMotion != eParamiteMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }

            if (VIsFacingMe(gAbe))
            {
                if (VIsObjNearby(kGridSize, gAbe))
                {
                    mNextMotion = eParamiteMotions::Motion_23_Eating;
                    field_114_timer = MakeTimer(28);
                    return Brain_4_ChasingAbe::eBrain4_Eating_13;
                }
                else
                {
                    mNextMotion = eParamiteMotions::Motion_2_Walking;
                    return Brain_4_ChasingAbe::eBrain4_Walking_11;
                }
            }
            else
            {
                mNextMotion = eParamiteMotions::Motion_5_Turn;
                return Brain_4_ChasingAbe::eBrain4_Turning_10;
            }
            break;

        case Brain_4_ChasingAbe::eBrain4_Eating_13:
            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_0_Idle;
            field_114_timer = MakeTimer(15);
            return Brain_4_ChasingAbe::eBrain4_Idle_14;

        case Brain_4_ChasingAbe::eBrain4_Idle_14:
            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_23_Eating;
            field_114_timer = MakeTimer(28);
            return Brain_4_ChasingAbe::eBrain4_Eating_13;

        case Brain_4_ChasingAbe::eBrain4_Panic_15:
            if (EventGet(kEventAbeOhm))
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_0_Idle;
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
        SetDead(true);
    }

    if (mMeat->VIsFalling() || mMeat->GetDead())
    {
        Sound(ParamiteSpeak::DetectedMeat_7);
        SetBrain(&Paramite::Brain_0_Patrol);
        mMeat->mBaseGameObjectRefCount--;
        mMeat = nullptr;
        mNextMotion = eParamiteMotions::Motion_0_Idle;
        return Brain_0_Patrol::eBrain0_Inactive_0;
    }

    if (mMeat->mPathLine)
    {
        if (FP_Abs(mMeat->mYPos - mYPos) > FP_FromInteger(20))
        {
            mMeat->mBaseGameObjectRefCount--;
            mMeat = nullptr;
            mNextMotion = eParamiteMotions::Motion_0_Idle;
            SetBrain(&Paramite::Brain_0_Patrol);
            return Brain_0_Patrol::eBrain0_Inactive_0;
        }
    }

    const FP kGridSize = ScaleToGridSize(GetSpriteScale());

    switch (mBrainSubState)
    {
        case Brain_5_SpottedMeat::eBrain5_Idle_0:
            if (!VIsFacingMe(mMeat))
            {
                if (FP_Abs(mMeat->mXPos - mXPos) > FP_FromInteger(5))
                {
                    Sound(ParamiteSpeak::DetectedMeat_7);
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_5_SpottedMeat::eBrain5_Turning_4;
                }
            }

            if (VIsObj_GettingNear_On_X(mMeat))
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
            }

            if (!GetAnimation().GetFlipX())
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }
            else if (GetAnimation().GetFlipX())
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(3), mMeat))
            {
                mNextMotion = eParamiteMotions::Motion_3_Running;
                return Brain_5_SpottedMeat::eBrain5_Running_1;
            }

            if (VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), mMeat))
            {
                if (mXPos == mMeat->mXPos)
                {
                    mNextMotion = eParamiteMotions::Motion_2_Walking;
                    return Brain_5_SpottedMeat::eBrain5_Walking_2;
                }

                if (!mMeat->mPathLine || !BaseAliveGameObjectCollisionLine)
                {
                    mNextMotion = eParamiteMotions::Motion_0_Idle;
                    return Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
                }

                mNextMotion = eParamiteMotions::Motion_23_Eating;
                field_114_timer = MakeTimer(mMeatEatingTime);
                return Brain_5_SpottedMeat::eBrain5_Eating_6;
            }

            mNextMotion = eParamiteMotions::Motion_2_Walking;
            return Brain_5_SpottedMeat::eBrain5_Walking_2;

        case Brain_5_SpottedMeat::eBrain5_Running_1:
        {
            const s16 x_exp = FP_GetExponent(mXPos);
            const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_AO(GetSpriteScale(), x_exp & 0x3FF);
            if (mVelX < FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine(1, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine(0, 1))
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
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_5_SpottedMeat::eBrain5_Turning_4;
                }
            }

            if (VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), mMeat))
            {
                if (mMeat->mPathLine)
                {
                    mNextMotion = eParamiteMotions::Motion_23_Eating;
                    field_114_timer = MakeTimer(mMeatEatingTime);
                    return Brain_5_SpottedMeat::eBrain5_Eating_6;
                }
                return Brain_5_SpottedMeat::eBrain5_Running_1;
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(3), mMeat))
            {
                return mBrainSubState;
            }
            mNextMotion = eParamiteMotions::Motion_2_Walking;
            return Brain_5_SpottedMeat::eBrain5_Walking_2;
        }

        case Brain_5_SpottedMeat::eBrain5_Walking_2:
        {
            const s16 x_exp = FP_GetExponent(mXPos);
            const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_AO(GetSpriteScale(), x_exp & 0x3FF);
            if (mVelX < FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) < 6 && Check_IsOnEndOfLine(1, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (mVelX > FP_FromInteger(0))
            {
                if (abs(xSnapped - x_exp) >= 6 && Check_IsOnEndOfLine(0, 1))
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
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_5_SpottedMeat::eBrain5_Turning_4;
                }
            }

            if (!VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), mMeat))
            {
                return mBrainSubState;
            }

            if (mMeat->mPathLine)
            {
                mNextMotion = eParamiteMotions::Motion_23_Eating;
                field_114_timer = MakeTimer(mMeatEatingTime);
                return Brain_5_SpottedMeat::eBrain5_Eating_6;
            }
            return Brain_5_SpottedMeat::eBrain5_Walking_2;
        }

        case Brain_5_SpottedMeat::eBrain5_Jumping_3:
            if (mCurrentMotion != eParamiteMotions::Motion_0_Idle)
            {
                return mBrainSubState;
            }
            return Brain_5_SpottedMeat::eBrain5_Idle_0;

        case Brain_5_SpottedMeat::eBrain5_Turning_4:
            if (mCurrentMotion != eParamiteMotions::Motion_5_Turn
                || !GetAnimation().GetIsLastFrame())
            {
                return mBrainSubState;
            }

            if (!VIsFacingMe(mMeat))
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_5_SpottedMeat::eBrain5_Idle_0;
            }

            if (FP_Abs(mMeat->mXPos - mXPos) > FP_FromInteger(5))
            {
                Sound(ParamiteSpeak::DetectedMeat_7);
                mNextMotion = eParamiteMotions::Motion_5_Turn;
                return 4;
            }
            else
            {
                mNextMotion = eParamiteMotions::Motion_0_Idle;
                return Brain_5_SpottedMeat::eBrain5_Idle_0;
            }
            break;

        case Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5:
            if (!VIsFacingMe(mMeat))
            {
                Sound(ParamiteSpeak::DetectedMeat_7);
                mNextMotion = eParamiteMotions::Motion_5_Turn;
                return Brain_5_SpottedMeat::eBrain5_Turning_4;
            }

            if (VIsObj_GettingNear_On_X(mMeat))
            {
                return mBrainSubState;
            }

            if (!GetAnimation().GetFlipX())
            {
                if (Check_IsOnEndOfLine(0, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }
            else if (GetAnimation().GetFlipX())
            {
                if (Check_IsOnEndOfLine(1, 1))
                {
                    ToHop();
                    return Brain_5_SpottedMeat::eBrain5_Jumping_3;
                }
            }

            if (!VIsObjNearby(kGridSize * FP_FromInteger(3), mMeat))
            {
                mNextMotion = eParamiteMotions::Motion_3_Running;
                return Brain_5_SpottedMeat::eBrain5_Running_1;
            }

            if (!VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), mMeat))
            {
                mNextMotion = eParamiteMotions::Motion_2_Walking;
                return Brain_5_SpottedMeat::eBrain5_Walking_2;
            }

            if (!mMeat->mPathLine || !BaseAliveGameObjectCollisionLine)
            {
                return Brain_5_SpottedMeat::eBrain5_AttentiveToMeat_5;
            }

            mNextMotion = eParamiteMotions::Motion_23_Eating;
            field_114_timer = MakeTimer(mMeatEatingTime);
            return Brain_5_SpottedMeat::eBrain5_Eating_6;

        case Brain_5_SpottedMeat::eBrain5_Eating_6:
            if (!VIsObjNearby(GetSpriteScale() * FP_FromInteger(40), mMeat))
            {
                mNextMotion = eParamiteMotions::Motion_2_Walking;
                return Brain_5_SpottedMeat::eBrain5_Walking_2;
            }

            if (!VIsFacingMe(mMeat))
            {
                if (FP_Abs(mMeat->mXPos - mXPos) > FP_FromInteger(5))
                {
                    Sound(ParamiteSpeak::DetectedMeat_7);
                    mNextMotion = eParamiteMotions::Motion_5_Turn;
                    return Brain_5_SpottedMeat::eBrain5_Turning_4;
                }
            }

            if (field_114_timer > static_cast<s32>(sGnFrame))
            {
                return mBrainSubState;
            }

            mMeat->mBaseGameObjectRefCount--;
            mMeat->SetDead(true);
            mMeat = nullptr;
            mNextMotion = eParamiteMotions::Motion_0_Idle;
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
    const FP kGridSize = ScaleToGridSize(GetSpriteScale());
    const FP numGridBlocksScaled = (kGridSize * FP_FromInteger(numGridBlocks));
    BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At_Of_Type(
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

    if (GetAnimation().GetFlipX())
    {
        mVelX = GetSpriteScale() * -sWalkBeginVelTable_4BBC88[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        mVelX = GetSpriteScale() * sWalkBeginVelTable_4BBC88[GetAnimation().GetCurrentFrame()];
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();

        if (GetAnimation().GetIsLastFrame())
        {
            mCurrentMotion = eParamiteMotions::Motion_2_Walking;
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

    if (GetAnimation().GetFlipX())
    {
        mVelX = GetSpriteScale() * -sWalkVelTable_4BBC50[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        mVelX = GetSpriteScale() * sWalkVelTable_4BBC50[GetAnimation().GetCurrentFrame()];
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion != eParamiteMotions::Motion_2_Walking)
    {
        return;
    }

    switch (GetAnimation().GetCurrentFrame())
    {
        case 0:
            [[fallthrough]];
        case 7:
            Sound(GetAnimation().GetCurrentFrame() == 7 ? ParamiteSpeak::LoudStep_3 : ParamiteSpeak::SlightStep_4);

            if (mNextMotion == eParamiteMotions::Motion_0_Idle
                || mNextMotion == eParamiteMotions::Motion_13_GameSpeakBegin
                || mNextMotion == eParamiteMotions::Motion_5_Turn
                || mNextMotion == eParamiteMotions::Motion_25_Death
                || mNextMotion == eParamiteMotions::Motion_15_Hiss
                || mNextMotion == eParamiteMotions::Motion_23_Eating)
            {
                mCurrentMotion = eParamiteMotions::Motion_9_WalkEnd;
            }
            break;

        case 3:
            [[fallthrough]];
        case 10:
            if (mNextMotion == eParamiteMotions::Motion_3_Running)
            {
                mPreviousMotion = eParamiteMotions::Motion_3_Running;
                mBaseAliveGameObjectLastAnimFrame = 11;
                field_140_use_prev_motion = 1;
                mNextMotion = eParamiteMotions::None_m1;
            }

            if (mNextMotion == eParamiteMotions::Motion_18_RunningAttack)
            {
                mCurrentMotion = eParamiteMotions::Motion_18_RunningAttack;
                mNextMotion = eParamiteMotions::None_m1;
                if (GetAnimation().GetFlipX())
                {
                    mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
                }
                else
                {
                    mVelX = ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4);
                }
            }

            if (!field_142_bSnapped)
            {
                MapFollowMe(1);
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
    if (GetAnimation().GetFlipX())
    {
        frameVelx = GetSpriteScale() * -sRunningTable_4BBCC0[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        frameVelx = GetSpriteScale() * sRunningTable_4BBCC0[GetAnimation().GetCurrentFrame()];
    }

    mVelX = frameVelx;

    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
        return;
    }

    MoveOnLine();

    if (mCurrentMotion != eParamiteMotions::Motion_3_Running)
    {
        return;
    }

    if (GetAnimation().GetCurrentFrame() == 3)
    {
        SFX_Play_Pitch(relive::SoundEffects::PickupItem, 45, -600);
        if (mNextMotion == eParamiteMotions::Motion_2_Walking)
        {
            mCurrentMotion = eParamiteMotions::Motion_8_WalkRunTransition;
            mNextMotion = eParamiteMotions::None_m1;
        }

        if (mNextMotion == eParamiteMotions::Motion_0_Idle
            || mNextMotion == eParamiteMotions::Motion_5_Turn
            || mNextMotion == eParamiteMotions::Motion_23_Eating
            || mNextMotion == eParamiteMotions::Motion_25_Death
            || mNextMotion == eParamiteMotions::Motion_15_Hiss)
        {
            mCurrentMotion = eParamiteMotions::Motion_11_RunEnd;
        }

        if (mNextMotion == eParamiteMotions::Motion_18_RunningAttack)
        {
            mCurrentMotion = eParamiteMotions::Motion_18_RunningAttack;
            mNextMotion = eParamiteMotions::None_m1;

            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
        }

        if (mNextMotion == eParamiteMotions::Motion_6_Hop)
        {
            VOnTrapDoorOpen();

            BaseAliveGameObjectLastLineYPos = mYPos;
            mCurrentMotion = eParamiteMotions::Motion_6_Hop;
            mNextMotion = eParamiteMotions::None_m1;
            mVelY = -FP_FromDouble(7.2) * GetSpriteScale();
            mYPos += mVelY;
            BaseAliveGameObjectCollisionLine = nullptr;

            if (field_142_bSnapped == 0)
            {
                MapFollowMe(1);
                field_142_bSnapped = 1;
            }

            SetMusic();
            return;
        }

        if (!field_142_bSnapped)
        {
            MapFollowMe(1);
            field_142_bSnapped = 1;
        }

        SetMusic();
        return;
    }

    if (GetAnimation().GetCurrentFrame() == 10)
    {
        Sound(ParamiteSpeak::LoudStep_3);
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);

        if (mNextMotion == eParamiteMotions::Motion_2_Walking)
        {
            mCurrentMotion = eParamiteMotions::Motion_8_WalkRunTransition;
            mNextMotion = eParamiteMotions::None_m1;
        }

        if (mNextMotion == eParamiteMotions::Motion_0_Idle
            || mNextMotion == eParamiteMotions::Motion_5_Turn
            || mNextMotion == eParamiteMotions::Motion_23_Eating
            || mNextMotion == eParamiteMotions::Motion_25_Death
            || mNextMotion == eParamiteMotions::Motion_15_Hiss)
        {
            mCurrentMotion = eParamiteMotions::Motion_11_RunEnd;
        }

        if (mNextMotion == eParamiteMotions::Motion_18_RunningAttack)
        {
            mCurrentMotion = eParamiteMotions::Motion_18_RunningAttack;
            mNextMotion = eParamiteMotions::None_m1;

            if (GetAnimation().GetFlipX())
            {
                mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
            else
            {
                mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
            }
        }

        if (mNextMotion == eParamiteMotions::Motion_6_Hop)
        {
            ToHop();
        }

        if (!field_142_bSnapped)
        {
            MapFollowMe(1);
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

void Paramite::Motion_5_Turn()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sound(ParamiteSpeak::SlightStep_4);
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
    if (GetAnimation().GetFlipX())
    {
        frameVelX = (GetSpriteScale() * -sHopVelTable_4BBD28[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        frameVelX = (GetSpriteScale() * sHopVelTable_4BBD28[GetAnimation().GetCurrentFrame()]);
    }

    mVelX = frameVelX;

    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        if (InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(0.9)))
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
                    OnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.y, bRect.h},
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
            field_124_XSpeed = FP_FromDouble(0.75);
            mCurrentMotion = eParamiteMotions::Motion_12_Falling;
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

    if (GetAnimation().GetFlipX())
    {
        mVelX = GetSpriteScale() * -State_7_Unknown_VelTable_4BBCA8[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        mVelX = GetSpriteScale() * State_7_Unknown_VelTable_4BBCA8[GetAnimation().GetCurrentFrame()];
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mPreviousMotion = eParamiteMotions::Motion_3_Running;
        mBaseAliveGameObjectLastAnimFrame = 2;
        field_140_use_prev_motion = 1;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
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

    if (GetAnimation().GetFlipX())
    {
        mVelX = GetSpriteScale() * -sWalkRunTransVelTable_4BBD18[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        mVelX = GetSpriteScale() * sWalkRunTransVelTable_4BBD18[GetAnimation().GetCurrentFrame()];
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eParamiteMotions::Motion_2_Walking;
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
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
    if (GetAnimation().GetFlipX())
    {
        mVelX = GetSpriteScale() * -sWalkEndVelTable_4BBC98[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        mVelX = GetSpriteScale() * sWalkEndVelTable_4BBC98[GetAnimation().GetCurrentFrame()];
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
    {
        ToKnockBack();
    }
    else
    {
        MoveOnLine();
        EventBroadcast(kEventNoise, this);

        if (!GetAnimation().GetIsLastFrame())
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
    if (GetAnimation().GetFlipX())
    {
        mVelX = (GetSpriteScale() * -sRunBeginVelTable_4BBCF8[GetAnimation().GetCurrentFrame()]);
    }
    else
    {
        mVelX = GetSpriteScale() * sRunBeginVelTable_4BBCF8[GetAnimation().GetCurrentFrame()];
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mPreviousMotion = eParamiteMotions::Motion_3_Running;
        mBaseAliveGameObjectLastAnimFrame = 2;
        field_140_use_prev_motion = 1;
    }

    EventBroadcast(kEventNoise, this);

    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
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
    if (GetAnimation().GetFlipX())
    {
        mVelX = GetSpriteScale() * -sRunEndVelTable_4BBD08[GetAnimation().GetCurrentFrame()];
    }
    else
    {
        mVelX = GetSpriteScale() * sRunEndVelTable_4BBD08[GetAnimation().GetCurrentFrame()];
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }

    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
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
        mVelX -= (GetSpriteScale() * field_124_XSpeed);
        if (mVelX < FP_FromInteger(0))
        {
            mVelX = FP_FromInteger(0);
        }
    }
    else if (mVelX < FP_FromInteger(0))
    {
        mVelX += (GetSpriteScale() * field_124_XSpeed);
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
                ToIdle();

                BaseAliveGameObjectCollisionLine = pLine;

                PSX_RECT bRect = VGetBoundingRect();
                bRect.y += 5;
                bRect.h += 5;

                OnCollisionWith(
                    {bRect.x, bRect.y},
                    {bRect.w, bRect.h},
                    gPlatformsArray);

                mXPos = hitX;
                mYPos = hitY;
                MapFollowMe(true);
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
    const PSX_RECT abeRect = gAbe->VGetBoundingRect();
    const PSX_RECT ourRect = VGetBoundingRect();

    if (abeRect.x <= ourRect.w
        && abeRect.w >= ourRect.x
        && abeRect.h >= ourRect.y
        && abeRect.y <= ourRect.h)
    {
        if (VIsObj_GettingNear_On_X(gAbe))
        {
            if (GetSpriteScale() == gAbe->GetSpriteScale())
            {
                if (gAbe->mHealth > FP_FromInteger(0))
                {
                    SfxPlayMono(relive::SoundEffects::KillEffect, 0);
                    Mudokon_SFX(MudSounds::eKnockbackOuch_10, 0, 0, gAbe);
                }
                gAbe->VTakeDamage(this);
            }
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eParamiteMotions::Motion_14_PreHiss;
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
    const PSX_RECT abeRect = gAbe->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(gAbe))
        {
            if (GetSpriteScale() == gAbe->GetSpriteScale())
            {
                gAbe->VTakeDamage(this);
            }
        }
    }

    if (mNextMotion == eParamiteMotions::Motion_18_RunningAttack)
    {
        mCurrentMotion = eParamiteMotions::Motion_18_RunningAttack;
        mNextMotion = eParamiteMotions::None_m1;
        if (GetAnimation().GetFlipX())
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }
        else
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }
    }

    if (mNextMotion == eParamiteMotions::Motion_22_Unknown)
    {
        mCurrentMotion = eParamiteMotions::Motion_22_Unknown;
        mNextMotion = eParamiteMotions::None_m1;
    }

    if (mNextMotion == eParamiteMotions::Motion_15_Hiss)
    {
        mCurrentMotion = eParamiteMotions::Motion_15_Hiss;
        mNextMotion = eParamiteMotions::None_m1;
    }

    if (mNextMotion != eParamiteMotions::None_m1)
    {
        mCurrentMotion = eParamiteMotions::Motion_17_GameSpeakEnd;
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
    const PSX_RECT abeRect = gAbe->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        Sound(ParamiteSpeak::Stay_1);
    }

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(gAbe))
        {
            if (GetSpriteScale() == gAbe->GetSpriteScale())
            {
                gAbe->VTakeDamage(this);
            }
        }
    }

    if (mNextMotion == eParamiteMotions::Motion_18_RunningAttack)
    {
        mCurrentMotion = eParamiteMotions::Motion_18_RunningAttack;
        mNextMotion = eParamiteMotions::None_m1;
        if (GetAnimation().GetFlipX())
        {
            mVelX = -(ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }
        else
        {
            mVelX = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(4));
        }
    }
    else if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eParamiteMotions::Motion_16_PostHiss;
    }
}

void Paramite::Motion_16_PostHiss()
{
    const PSX_RECT abeRect = gAbe->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(gAbe))
        {
            if (GetSpriteScale() == gAbe->GetSpriteScale())
            {
                gAbe->VTakeDamage(this);
            }
        }
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eParamiteMotions::Motion_14_PreHiss;
    }
}

void Paramite::Motion_17_GameSpeakEnd()
{
    const PSX_RECT abeRect = gAbe->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h)
    {
        if (VIsObj_GettingNear_On_X(gAbe))
        {
            if (GetSpriteScale() == gAbe->GetSpriteScale())
            {
                gAbe->VTakeDamage(this);
            }
        }
    }
    if (GetAnimation().GetIsLastFrame())
    {
        if (!ToNextMotion())
        {
            GetAnimation().SetFlipY(false);
            GetAnimation().SetSwapXY(false);
            field_124_XSpeed = FP_FromInteger(0);
            mVelX = FP_FromInteger(0);
            mVelY = FP_FromInteger(0);
            mCurrentMotion = eParamiteMotions::Motion_0_Idle;
            MapFollowMe(true);
        }
    }
}

void Paramite::Motion_18_RunningAttack()
{
    if (WallHit(GetSpriteScale() * FP_FromInteger(10), mVelX))
    {
        BaseAliveGameObjectCollisionLine = nullptr;

        ToKnockBack();
        return;
    }

    mXPos += mVelX;
    MapFollowMe(false);

    const PSX_RECT abeRect = gAbe->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h
        && GetSpriteScale() == gAbe->GetSpriteScale())
    {
        gAbe->VTakeDamage(this);
    }

    if (GetAnimation().GetCurrentFrame() == 3)
    {
        Sound(ParamiteSpeak::CMon_or_Attack_0);
    }

    if (GetAnimation().GetCurrentFrame() == 11)
    {
        FP gridSizeDirected = {};
        if (mVelX >= FP_FromInteger(0))
        {
            gridSizeDirected = ScaleToGridSize(GetSpriteScale());
        }
        else
        {
            gridSizeDirected = -ScaleToGridSize(GetSpriteScale());
        }

        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (gCollisions->Raycast(
                mXPos + gridSizeDirected,
                mYPos - FP_FromInteger(10),
                mXPos + gridSizeDirected,
                mYPos + FP_FromInteger(10),
                &pLine,
                &hitX,
                &hitY,
                GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
        {
            if (pLine->mLineType == eLineTypes ::eDynamicCollision_32 ||
                pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
            {
                PSX_RECT r = VGetBoundingRect();
                r.y += 5;
                r.h += 5;

                OnCollisionWith(
                    {r.x, r.y},
                    {r.w, r.h},
                    gPlatformsArray);
            }
            mYPos = hitY;
            BaseAliveGameObjectCollisionLine = pLine;
        }
        else
        {
            BaseAliveGameObjectCollisionLine = nullptr;
            mCurrentMotion = eParamiteMotions::Motion_12_Falling;
            field_124_XSpeed = FP_FromDouble(0.75);
            mYPos -= (GetSpriteScale() * FP_FromInteger(10));
        }
    }

    if (GetAnimation().GetIsLastFrame())
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

    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mVelY + mYPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetSpriteScale() != FP_FromDouble(0.5) ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
        mCurrentMotion = eParamiteMotions::Motion_21_WebLeaveDown;

        if (BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eDynamicCollision_32 || BaseAliveGameObjectCollisionLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            PSX_RECT bRect = VGetBoundingRect();
            bRect.y += 5;
            bRect.h += 5;

            OnCollisionWith(
                {bRect.x, bRect.y},
                {bRect.w, bRect.h},
                gPlatformsArray);
        }
    }
    else
    {
        mYPos += mVelY;
    }

    if (GetAnimation().GetCurrentFrame() == 0 || GetAnimation().GetCurrentFrame() == 3)
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
    if (GetAnimation().GetCurrentFrame() == 2)
    {
        Environment_SFX(EnvironmentSfx::eHitGroundSoft_6, 50, 600, 0);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        ToIdle();
    }
}

void Paramite::Motion_22_Unknown()
{
    const PSX_RECT abeRect = gAbe->VGetBoundingRect();
    const PSX_RECT rect = VGetBoundingRect();

    if (abeRect.x <= rect.w
        && abeRect.w >= rect.x
        && abeRect.h >= rect.y
        && abeRect.y <= rect.h
        && GetSpriteScale() == gAbe->GetSpriteScale())
    {
        gAbe->VTakeDamage(this);
    }

    if (GetAnimation().GetIsLastFrame())
    {
        mCurrentMotion = eParamiteMotions::Motion_14_PreHiss;
    }
}

void Paramite::Motion_23_Eating()
{
    if (GetAnimation().GetCurrentFrame() == 5)
    {
        SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Eating1, relive::SoundEffects::Eating2), 0);
    }

    if (mNextMotion != eParamiteMotions::None_m1 && !ToNextMotion())
    {
        ToIdle();
    }
}

void Paramite::Motion_24_Struggle()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        SfxPlayMono(relive::SoundEffects::KillEffect, 0);
    }
}

void Paramite::Motion_25_Death()
{
    if (GetAnimation().GetCurrentFrame() == 0)
    {
        Sound(ParamiteSpeak::DoIt_2);
    }
}

void Paramite::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
        mCurrentMotion = eParamiteMotions::Motion_12_Falling;
    }
}


} // namespace AO
