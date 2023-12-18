#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "RollingBall.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "RollingBallShaker.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "ParticleBurst.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "../relive_lib/GameObjects/ScreenShake.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Midi.hpp"
#include "Grid.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

void RollingBall::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Stone_Ball));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Stone_Ball_Rolling));
}

RollingBall::~RollingBall()
{
    if (mState != States::eInactive)
    {
        Path::TLV_Reset(field_10C_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
    }

    KillRollingBallShaker();
}

RollingBall::RollingBall(relive::Path_RollingBall* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eRollingBall);
    
    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::Stone_Ball));

    GetAnimation().SetRenderLayer(Layer::eLayer_FallingItemDoorFlameRollingBallPortalClip_Half_31);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_DoorFlameRollingBallFallingItemPortalClip_Half_12);
        SetScale(Scale::Bg);
    }

    if (pTlv->mRollDirection == relive::reliveXDirection::eLeft)
    {
        GetAnimation().SetFlipX(true);
    }

    mReleaseSwitchId = pTlv->mReleaseSwitchId;
    mMaxSpeed = FP_FromRaw(pTlv->mMaxSpeed << 8);

    if (GetAnimation().GetFlipX())
    {
        mMaxSpeed = -FP_FromRaw(pTlv->mMaxSpeed << 8);
    }

    mAcceleration = FP_FromRaw(pTlv->mAcceleration << 8);

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    FP castX = {};
    FP castY = {};

    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &castX,
            &castY,
            // yeah this should be simplified, WTF!
            GetSpriteScale() - FP_FromDouble(0.5) != FP_FromInteger(0) ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = castY;
    }

    MapFollowMe(true);
    field_10C_tlvInfo = tlvId;
    mState = States::eInactive;
    mRollingBallShakerId = {};

    CreateShadow();

    if (!SwitchStates_Get(mReleaseSwitchId))
    {
        return;
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eForestTemple && gMap.mCurrentPath == 2)
    {
        GetAnimation().SetAnimate(false);
        mXPos = FP_FromInteger(2522);
        mYPos = FP_FromInteger(1300);
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
        mState = States::eCrushedBees;
    }
}

void RollingBall::VUpdate()
{
    switch (mState)
    {
        case States::eInactive:
            if (SwitchStates_Get(mReleaseSwitchId))
            {
                mVelY = FP_FromDouble(2.5);
                mState = States::eStartRolling;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Stone_Ball_Rolling));
                auto pRollingBallShaker = relive_new RollingBallShaker();
                if (pRollingBallShaker)
                {
                    mRollingBallShakerId = pRollingBallShaker->mBaseGameObjectId;
                }
            }
            else if (!gMap.Is_Point_In_Current_Camera(
                         mCurrentLevel,
                         mCurrentPath,
                         mXPos,
                         mYPos,
                         0))
            {
                SetDead(true);
            }
            return;

        case States::eStartRolling:
        {
            if (!(GetAnimation().GetCurrentFrame() % 3))
            {
                SfxPlayMono(relive::RandomSfx(relive::SoundEffects::RollingBallNoise1, relive::SoundEffects::RollingBallNoise2), 0);
            }

            Accelerate();

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            if (InAirCollision(&pLine, &hitX, &hitY, FP_FromInteger(0)))
            {
                if (pLine->mLineType != eLineTypes::eFloor_0 && pLine->mLineType != eLineTypes::eBackgroundFloor_4)
                {
                    return;
                }

                mVelY = FP_FromInteger(0);
                mXPos = hitX;
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
                mState = States::eRolling;
            }
            return;
        }

        case States::eRolling:
        {
            if (!(GetAnimation().GetCurrentFrame() % 3))
            {
                SfxPlayMono(relive::RandomSfx(relive::SoundEffects::RollingBallNoise1, relive::SoundEffects::RollingBallNoise2), 0);
            }

            Accelerate();

            BaseAliveGameObjectCollisionLine = BaseAliveGameObjectCollisionLine->MoveOnLine(
                &mXPos,
                &mYPos,
                mVelX);

            u16 result = 0;
            CamX_VoidSkipper(mXPos, mVelX, 50, &result);
            if (result == 1 || result == 2)
            {
                MapFollowMe(0);
            }

            CrushThingsInTheWay();

            if (EventGet(kEventDeathReset))
            {
                KillRollingBallShaker();
            }
            else if (!BaseAliveGameObjectCollisionLine)
            {
                mState = States::eFallingAndHittingWall;

                KillRollingBallShaker();

                mXPos += mVelX;
                BaseAliveGameObjectLastLineYPos = mYPos;
            }
            return;
        }

        case States::eFallingAndHittingWall:
        {
            if (WallHit(FP_FromInteger(30), mVelX))
            {
                relive_new ParticleBurst(
                    mXPos,
                    mYPos - FP_FromInteger(30),
                    150,
                    GetSpriteScale(),
                    BurstType::eRocks);

                relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, relive::TBlendModes::eBlend_1, 1);

                relive_new ScreenShake(false, false);

                SetDead(true);

                const CameraPos direction = gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos);
                SFX_Play_Camera(relive::SoundEffects::IngameTransition, 50, direction);

                switch (direction)
                {
                    case CameraPos::eCamCurrent_0:
                        SND_SEQ_Play(SeqId::eExplosion1_21, 1, 60, 60);
                        break;

                    case CameraPos::eCamTop_1:
                    case CameraPos::eCamBottom_2:
                        SND_SEQ_Play(SeqId::eExplosion1_21, 1, 40, 40);
                        break;

                    case CameraPos::eCamLeft_3:
                        SND_SEQ_Play(SeqId::eExplosion1_21, 1, 40, 13);
                        break;

                    case CameraPos::eCamRight_4:
                        SND_SEQ_Play(SeqId::eExplosion1_21, 1, 13, 40);
                        break;

                    default:
                        return;
                }
                return;
            }

            if (EventGet(kEventDeathReset))
            {
                SetDead(true);
                CrushThingsInTheWay();
                return;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (!InAirCollision(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
            {
                if (mYPos - BaseAliveGameObjectLastLineYPos > FP_FromInteger(240))
                {
                    if (gMap.mCurrentLevel == EReliveLevelIds::eForestTemple
                        && gMap.mCurrentPath == 2
                        && !gAbe->mShrivel)
                    {
                        GetAnimation().SetAnimate(false);
                        mXPos = FP_FromInteger(2522);
                        mYPos = FP_FromInteger(1300);
                        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
                        mState = States::eCrushedBees;
                        CrushThingsInTheWay();
                        return;
                    }
                    SetDead(true);
                }
                CrushThingsInTheWay();
                return;
            }

            mYPos = hitY;
            BaseAliveGameObjectLastLineYPos = mYPos;
            mVelY = (-mVelY * FP_FromDouble(0.8));

            relive_new ScreenShake(false, false);

            const CameraPos direction = gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos);
            SFX_Play_Camera(relive::SoundEffects::IngameTransition, 50, direction);

            switch (direction)
            {
                case CameraPos::eCamCurrent_0:
                    SND_SEQ_Play(SeqId::eExplosion1_21, 1, 60, 60);
                    CrushThingsInTheWay();
                    break;

                case CameraPos::eCamTop_1:
                case CameraPos::eCamBottom_2:
                    SND_SEQ_Play(SeqId::eExplosion1_21, 1, 40, 40);
                    CrushThingsInTheWay();
                    break;

                case CameraPos::eCamLeft_3:
                    SND_SEQ_Play(SeqId::eExplosion1_21, 1, 40, 13);
                    CrushThingsInTheWay();
                    break;

                case CameraPos::eCamRight_4:
                    SND_SEQ_Play(SeqId::eExplosion1_21, 1, 13, 40);
                    CrushThingsInTheWay();
                    break;

                default:
                    CrushThingsInTheWay();
                    return;
            }
            return;
        }

        case States::eCrushedBees:
            if (mCurrentLevel != gMap.mCurrentLevel || mCurrentPath != gMap.mCurrentPath || EventGet(kEventDeathReset))
            {
                SetDead(true);
            }
            return;

        default:
            return;
    }
}

void RollingBall::Accelerate()
{
    if (GetAnimation().GetFlipX())
    {
        if (mVelX > mMaxSpeed)
        {
            mVelX -= mAcceleration;
            mVelY = (-mVelX * FP_FromDouble(0.5));
        }
    }
    else
    {
        if (mVelX < mMaxSpeed)
        {
            mVelX += mAcceleration;
            mVelY = (mVelX * FP_FromDouble(0.5));
        }
    }
}

void RollingBall::CrushThingsInTheWay()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetIsBaseAliveGameObject())
        {
            const PSX_RECT bOurRect = VGetBoundingRect();

            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            const PSX_RECT bObjRect = pAliveObj->VGetBoundingRect();

            if (bOurRect.x <= bObjRect.w
                && bOurRect.w >= bObjRect.x
                && bOurRect.h >= bObjRect.y
                && bOurRect.y <= bObjRect.h)
            {
                pAliveObj->VTakeDamage(this);
            }
        }
    }
}

void RollingBall::KillRollingBallShaker() 
{
    if (mRollingBallShakerId.IsValid())
    {
        auto pShaker = static_cast<RollingBallShaker*>(sObjectIds.Find_Impl(mRollingBallShakerId));
        pShaker->mStopShaking = true;
        pShaker->SetDead(true);
        mRollingBallShakerId = {};
    }
}


} // namespace AO
