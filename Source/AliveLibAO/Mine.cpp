#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Mine.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "GroundExplosion.hpp"
#include "Grid.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

static Mine* sMinePlayingSound = nullptr;


void Mine::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Flash));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine));
}

Mine::Mine(relive::Path_Mine* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eMine);
    
    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Mine));
    
    SetInteractive(true);
    SetCanExplode(true);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
        SetScale(Scale::Fg);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);
    mTlvId = tlvId;
    mExplosionTimer = sGnFrame;

    mFlashAnim.Init(GetAnimRes(AnimId::Mine_Flash), this);

    mFlashAnim.SetSemiTrans(true);
    mFlashAnim.SetBlending(true);

    mFlashAnim.SetRenderLayer(GetAnimation().GetRenderLayer());
    mFlashAnim.SetSpriteScale(GetSpriteScale());
    mFlashAnim.SetRGB(128, 128, 128);

    mFlashAnim.SetRenderMode(TPageAbr::eBlend_0);

    if (pTlv->mPersistOffscreen == relive::reliveChoice::eYes)
    {
        mPersistOffscreen = true;
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mRGB.SetRGB(50, 50, 50);
        // TODO: Set the pal
        //ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 1, 0);
        //ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 1, 0);
    }

    SetInteractive(true);

    mCollectionRect.x = mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
    mCollectionRect.y = mYPos - ScaleToGridSize(GetSpriteScale());
    mCollectionRect.w = mXPos + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
    mCollectionRect.h = mYPos;
}

Mine::~Mine()
{
    if (mDetonating == true)
    {
        Path::TLV_Reset(mTlvId, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(mTlvId, -1, 0, 0);
    }

    mFlashAnim.VCleanUp();

    SetInteractive(false);

    if (sMinePlayingSound == this)
    {
        sMinePlayingSound = nullptr;
    }
}

void Mine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || !mPersistOffscreen)
    {
        SetDead(true);
    }
}

bool Mine::VTakeDamage(BaseGameObject* pFrom)
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
            relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
            SetDead(true);
            mDetonating = true;
            mExplosionTimer = sGnFrame;
            return true;
        }

        default:
            return false;
    }
}

void Mine::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
    mDetonating = true;
}

void Mine::VOnPickUpOrSlapped()
{
    if (mDetonating != true)
    {
        mDetonating = true;
        mExplosionTimer = MakeTimer(5);
    }
}

void Mine::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mFlashAnim.VRender(
            FP_GetExponent(mXPos + (FP_FromInteger(gScreenManager->mCamXOff) - gScreenManager->mCamPos->x)),
            FP_GetExponent(mYPos + (FP_FromInteger(gScreenManager->mCamYOff + mYOffset)) - gScreenManager->mCamPos->y),
            ppOt,
            0,
            0);
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Mine::VUpdate()
{
    const s16 bInCamera = gMap.Is_Point_In_Current_Camera(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos,
        0);

    if (mDetonating)
    {
        if (mDetonating == true && static_cast<s32>(sGnFrame) >= mExplosionTimer)
        {
            relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
            SetDead(true);
        }
    }
    else
    {
        if (GetAnimation().GetCurrentFrame() == 1 && (sMinePlayingSound == nullptr || sMinePlayingSound == this))
        {
            if (bInCamera)
            {
                SfxPlayMono(relive::SoundEffects::RedTick, 35);
            }
            sMinePlayingSound = this;
        }

        if (IsColliding())
        {
            mDetonating = true;
            mExplosionTimer = sGnFrame;
        }
    }

    if (mDetonating != true
        && (EventGet(kEventDeathReset)
            || mCurrentLevel != gMap.mCurrentLevel
            || mCurrentPath != gMap.mCurrentPath))
    {
        SetDead(true);
    }
}

s16 Mine::IsColliding()
{
    const PSX_RECT mineBound = VGetBoundingRect();
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetCanSetOffExplosives() && pObj->GetAnimation().GetRender())
        {
            const PSX_RECT objBound = pObj->VGetBoundingRect();
            const s32 objX = FP_GetExponent(pObj->mXPos);
            const s32 objY = FP_GetExponent(pObj->mYPos);

            if (objX > mineBound.x && objX < mineBound.w && objY < mineBound.h + 5 && mineBound.x <= objBound.w && mineBound.w >= objBound.x && mineBound.h >= objBound.y && mineBound.y <= objBound.h && pObj->GetSpriteScale() == GetSpriteScale())
            {
                return 1;
            }
        }
    }
    return 0;
}

} // namespace AO
