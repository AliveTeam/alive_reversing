#include "stdafx_ao.h"
#include "Function.hpp"
#include "TimedMine.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "LiftPoint.hpp"
#include "GroundExplosion.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Grid.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../AliveLibAE/FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

static const TintEntry sTimedMineTints[3] = {
    {EReliveLevelIds::eStockYards, 60u, 60u, 60u},
    {EReliveLevelIds::eStockYardsReturn, 60u, 60u, 60u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u},
};

void TimedMine::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bomb_Flash));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bomb_RedGreenTick));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::TimedMine_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::TimedMine_Activated));
}

TimedMine::TimedMine(relive::Path_TimedMine* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eTimedMine);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::TimedMine_Idle));

    SetInteractive(true);
    mTimedMineFlags.Clear(TimedMineFlags::eStickToLiftPoint);
    mSlappedMine = 0;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
    }

    InitTickAnimation();

    mTicksUntilExplosion = pTlv->mTicksUntilExplosion;

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);

    mExplosionTimer = sGnFrame;
    mTlvInfo = tlvId;

    SetBaseAnimPaletteTint(sTimedMineTints, gMap.mCurrentLevel, PalId::Default); // TODO: Bomb pal removed, check correct
    mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
    mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
    mCollectionRect.w = mXPos + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
    mCollectionRect.h = mYPos;

    SetInteractive(true);
}

TimedMine::~TimedMine()
{
    auto pPlatform = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (mSlappedMine != 1 || sGnFrame < mExplosionTimer)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }

    mTickAnim.VCleanUp();

    if (pPlatform)
    {
        pPlatform->VRemove(this);
        BaseAliveGameObject_PlatformId = Guid{};
    }

    SetInteractive(false);
}

void TimedMine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        SetDead(true);
    }

    if (mSlappedMine != 1)
    {
        SetDead(true);
    }
}

s16 TimedMine::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead())
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case ReliveTypes::eAbe:
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eAirExplosion:
        case ReliveTypes::eShrykull:
        {
            SetDead(true);
            relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
            mSlappedMine = 1;
            mExplosionTimer = sGnFrame;
            return 1;
        }

        default:
            return 0;
    }
}

void TimedMine::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mTickAnim.VRender(
            FP_GetExponent(mXPos + FP_FromInteger(pScreenManager->mCamXOff) - pScreenManager->mCamPos->x),
            FP_GetExponent(mYPos + FP_FromInteger(pScreenManager->mCamYOff - FP_GetExponent(GetSpriteScale() * FP_FromInteger(14)))
                           - pScreenManager->mCamPos->y),
            ppOt,
            0,
            0);

        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void TimedMine::InitTickAnimation()
{
    if (mTickAnim.Init(GetAnimRes(AnimId::Bomb_RedGreenTick), this))
    {
        mTickAnim.SetRenderLayer(GetAnimation().GetRenderLayer());
        mTickAnim.mFlags.Set(AnimFlags::eSemiTrans);
        mTickAnim.mFlags.Set(AnimFlags::eBlending);
        mTickAnim.SetSpriteScale(GetSpriteScale());
        mTickAnim.SetRGB(128, 128, 128);
        mTickAnim.SetRenderMode(TPageAbr::eBlend_1);
    }
    else
    {
        SetListAddFailed(true);
    }
}

void TimedMine::StickToLiftPoint()
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    mTimedMineFlags.Set(TimedMineFlags::eStickToLiftPoint);
    if (sCollisions->Raycast(
            mXPos,
            mYPos - FP_FromInteger(20),
            mXPos,
            mYPos + FP_FromInteger(20),
            &pLine,
            &hitX,
            &hitY,
            (GetSpriteScale() != FP_FromDouble(0.5)) ? kFgWallsOrFloor : kBgWallsOrFloor))
    {
        if (pLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            if (gPlatformsArray)
            {
                for (s32 i = 0; i < gPlatformsArray->Size(); i++)
                {
                    BaseGameObject* pObj = gPlatformsArray->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->Type() == ReliveTypes::eLiftPoint)
                    {
                        auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                        const PSX_RECT bRect = pLiftPoint->VGetBoundingRect();
                        if (FP_GetExponent(mXPos) > bRect.x && FP_GetExponent(mXPos) < bRect.w && FP_GetExponent(mYPos) < bRect.h)
                        {
                            pLiftPoint->VAdd(this);
                            BaseAliveGameObject_PlatformId = pObj->mBaseGameObjectId;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void TimedMine::VUpdate()
{
    auto pPlatform = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!mTimedMineFlags.Get(TimedMineFlags::eStickToLiftPoint))
    {
        StickToLiftPoint();
    }

    if (pPlatform && pPlatform->OnAnyFloor())
    {
        mCollectionRect.x = mXPos - ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
        mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
        mCollectionRect.w = mXPos + ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
        mCollectionRect.h = mYPos;
    }

    if (mSlappedMine == 1)
    {
        if (static_cast<s32>(sGnFrame) > (mSingleTickTimer + mOldGnFrame))
        {
            mOldGnFrame = sGnFrame;
            const CameraPos soundDir = gMap.GetDirection(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos);
            SFX_Play_Camera(relive::SoundEffects::GreenTick, 50, soundDir);

            // ~7 limits the number to multiples of 8
            if (((mExplosionTimer - sGnFrame) & ~7) >= 18 * 8)
            {
                mSingleTickTimer = 18;
            }
            else
            {
                mSingleTickTimer = (mExplosionTimer - sGnFrame) / 8;
            }
        }

        if (sGnFrame >= mExplosionTimer)
        {
            relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
            SetDead(true);
        }
    }
}

void TimedMine::VOnThrowableHit(BaseGameObject* /*pHitBy*/)
{
    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());

    SetDead(true);
    mSlappedMine = 1;
    mExplosionTimer = sGnFrame;
}

void TimedMine::VOnPickUpOrSlapped()
{
    if (mSlappedMine != 1)
    {
        mSlappedMine = 1;
        if ((s32)(mTicksUntilExplosion & 0xFFFC) >= 72)
        {
            mSingleTickTimer = 18;
        }
        else
        {
            mSingleTickTimer = mTicksUntilExplosion >> 2;
        }
        mOldGnFrame = sGnFrame;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::TimedMine_Activated));
        mExplosionTimer = sGnFrame + mTicksUntilExplosion;
        mTickAnim.Set_Animation_Data(GetAnimRes(AnimId::Bomb_Flash));
        SfxPlayMono(relive::SoundEffects::GreenTick, 0);
    }
}

} // namespace AO
