#include "stdafx.h"
#include "TimedMine.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "GroundExplosion.hpp"
#include "LiftPoint.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/FixedPoint.hpp"

static const TintEntry sTimedMineTint[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
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
    {EReliveLevelIds::eCredits, 127u, 127u, 127u},
};

void TimedMine::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bomb_Flash));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bomb_RedGreenTick));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::TimedMine_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::TimedMine_Activated));
}

TimedMine::TimedMine(relive::Path_TimedMine* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eTimedMine_or_MovingBomb);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::TimedMine_Idle));

    SetInteractive(true);
    mStickToLiftPoint = false;
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

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

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
    SetBaseAnimPaletteTint(sTimedMineTint, gMap.mCurrentLevel, PalId::Default); // TODO: Bomb pal removed, check correct
 
    const FP gridSnap = ScaleToGridSize(GetSpriteScale());
    SetInteractive(true);
    SetDoPurpleLightEffect(true);

    mCollectionRect.x = mXPos - (gridSnap / FP_FromDouble(2.0));
    mCollectionRect.y = mYPos - gridSnap;
    mCollectionRect.w = mXPos + (gridSnap / FP_FromDouble(2.0));
    mCollectionRect.h = mYPos;

    BaseAliveGameObject_PlatformId = Guid{};
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

bool TimedMine::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead())
    {
        return false;
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
            return true;
        }

        default:
            return false;
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
            FP_GetExponent((mXPos - gScreenManager->CamXPos())),
            FP_GetExponent((mYPos - gScreenManager->CamYPos() - FP_NoFractional(GetSpriteScale() * FP_FromDouble(14)))),
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
        mTickAnim.SetSemiTrans(true);
        mTickAnim.SetBlending(true);
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
    mStickToLiftPoint = true;
    if (sCollisions->Raycast(
            mXPos,
            mYPos - FP_FromInteger(20),
            mXPos,
            mYPos + FP_FromInteger(20),
            &pLine,
            &hitX,
            &hitY,
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

    if (!mStickToLiftPoint)
    {
        StickToLiftPoint();
    }

    if (pPlatform && pPlatform->vOnAnyFloor())
    {
        mCollectionRect.x = mXPos - ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
        mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
        mCollectionRect.w = mXPos + ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2);
        mCollectionRect.h = mYPos;
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
