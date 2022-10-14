#include "stdafx.h"
#include "TimedMine.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "GroundExplosion.hpp"
#include "LiftPoint.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"

static TintEntry sTimedMineTint_550EB8[1] = {{EReliveLevelIds::eNone, 127u, 127u, 127u}};

void TimedMine::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bomb_Flash));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bomb_RedGreenTick));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::TimedMine_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::TimedMine_Activated));
}

TimedMine::TimedMine(relive::Path_TimedMine* pPath, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eTimedMine_or_MovingBomb);

    LoadAnimations();
    Animation_Init( GetAnimRes(AnimId::TimedMine_Idle));

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mTimedMineFlags.Clear(TimedMineFlags::eStickToLiftPoint);
    mSlappedMine = 0;

    if (pPath->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
    }
    else
    {
        SetSpriteScale(FP_FromDouble(1.0));
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
    }

    InitTickAnimation();

    mTicksUntilExplosion = pPath->mTicksUntilExplosion;

    mXPos = FP_FromInteger((pPath->mTopLeftX + pPath->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pPath->mTopLeftY);

    FP hitY = {};
    FP hitX = {};

    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
    {
        mYPos = hitY;
    }

    mTlvInfo = tlvId;
    mExplosionTimer = sGnFrame;
    SetBaseAnimPaletteTint(sTimedMineTint_550EB8, gMap.mCurrentLevel, PalId::Bomb);
 
    const FP gridSnap = ScaleToGridSize(GetSpriteScale());
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    mCollectionRect.x = mXPos - (gridSnap / FP_FromDouble(2.0));
    mCollectionRect.y = mYPos - gridSnap;
    mCollectionRect.w = (gridSnap / FP_FromDouble(2.0)) + mXPos;
    mCollectionRect.h = mYPos;

    BaseAliveGameObject_PlatformId = Guid{};
}

void TimedMine::VUpdate()
{
    auto pPlatform = static_cast<LiftPoint*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (!mTimedMineFlags.Get(TimedMineFlags::eStickToLiftPoint))
    {
        StickToLiftPoint();
    }

    if (pPlatform && pPlatform->vOnAnyFloor())
    {
        mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
        mCollectionRect.w = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2)) + mXPos;
        mCollectionRect.h = mYPos;
        mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
    }

    if (mSlappedMine == 1)
    {
        if (sGnFrame > (mOldGnFrame + mSingleTickTimer))
        {
            mOldGnFrame = sGnFrame;
            const CameraPos soundDir = gMap.GetDirection(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos);
            SFX_Play_Camera(relive::SoundEffects::GreenTick, 50, soundDir);

            // TODO: Modulus ?
            if (((mExplosionTimer - sGnFrame) & 0xFFFFFFF8) >= 144)
            {
                mSingleTickTimer = 18;
            }
            else
            {
                mSingleTickTimer = (mExplosionTimer - sGnFrame) >> 3;
            }
        }

        if (sGnFrame >= mExplosionTimer)
        {
            relive_new GroundExplosion(
                mXPos,
                mYPos,
                GetSpriteScale());

            mBaseGameObjectFlags.Set(Options::eDead);
        }
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
            FP_GetExponent((mXPos - pScreenManager->CamXPos())),
            FP_GetExponent((mYPos - pScreenManager->CamYPos() - FP_NoFractional(GetSpriteScale() * FP_FromDouble(14)))),
            ppOt,
            0,
            0);

        PSX_RECT frameRect = {};
        mTickAnim.Get_Frame_Rect(&frameRect);

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
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
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
            mXPos, mYPos + FP_FromInteger(20),
            &pLine, &hitX, &hitY,
            (GetScale() == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
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
                        PSX_RECT bRect = pLiftPoint->VGetBoundingRect();
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

    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
}

void TimedMine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    if (mSlappedMine != 1)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 TimedMine::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
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
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
            mSlappedMine = 1;
            mExplosionTimer = sGnFrame;
            return 1;
        }

        default:
            return 0;
    }
}

void TimedMine::VOnThrowableHit(BaseGameObject* /*pHitBy*/)
{
    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
    
    mSlappedMine = 1;
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
