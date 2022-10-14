#include "stdafx_ao.h"
#include "Function.hpp"
#include "Mine.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "GroundExplosion.hpp"
#include "Grid.hpp"
#include "../relive_lib/ScreenManager.hpp"

namespace AO {

static Mine* sMinePlayingSound = nullptr;


void Mine::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Flash));
}

Mine::Mine(relive::Path_Mine* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eMine);
    
    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Mine));
    
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);

    field_10C_detonating = 0;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
        mScale = Scale::Fg;
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeftY + 24);
    field_110_tlv = tlvId;
    field_114_gnframe = sGnFrame;

    field_118_animation.Init(GetAnimRes(AnimId::Mine_Flash), this);

    field_118_animation.SetRenderLayer(GetAnimation().GetRenderLayer());
    field_118_animation.mFlags.Set(AnimFlags::eBlending);
    field_118_animation.mFlags.Set(AnimFlags::eSemiTrans);
    field_118_animation.SetSpriteScale(mSpriteScale);

    field_118_animation.SetRGB(128, 128, 128);

    field_118_animation.SetRenderMode(TPageAbr::eBlend_0);
    field_10E_disabled_resources = pTlv->mDisabledResources;

    // TODO
    field_1B0_flags = 2 * (pTlv->mPersistOffscreen == relive::reliveChoice::eYes) | (field_1B0_flags & ~2);

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mRGB.SetRGB(50, 50, 50);
        // TODO: Set the pal
        //ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 1, 0);
        //ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 1, 0);
    }

    mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
    mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
    mCollectionRect.w = mXPos + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
    mCollectionRect.h = mYPos;

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
}

Mine::~Mine()
{
    if (field_10C_detonating == 1)
    {
        Path::TLV_Reset(field_110_tlv, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_110_tlv, -1, 0, 0);
    }

    field_118_animation.VCleanUp();

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);

    if (sMinePlayingSound == this)
    {
        sMinePlayingSound = nullptr;
    }
}

void Mine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || !(field_1B0_flags & 2))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 Mine::VTakeDamage(BaseGameObject* pFrom)
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
            relive_new GroundExplosion(mXPos, mYPos, mSpriteScale);
            field_10C_detonating = 1;
            field_114_gnframe = sGnFrame;
            return 1;
        }

        default:
            return 0;
    }
}

void Mine::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new GroundExplosion(mXPos, mYPos, mSpriteScale);
    field_10C_detonating = 1;
}

void Mine::VOnPickUpOrSlapped()
{
    if (field_10C_detonating != 1)
    {
        field_10C_detonating = 1;
        field_114_gnframe = sGnFrame + 5;
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
        field_118_animation.VRender(
            FP_GetExponent(mXPos + (FP_FromInteger(pScreenManager->mCamXOff) - pScreenManager->mCamPos->x)),
            FP_GetExponent(mYPos + (FP_FromInteger(pScreenManager->mCamYOff + mYOffset)) - pScreenManager->mCamPos->y),
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

    if (field_10C_detonating)
    {
        if (field_10C_detonating == 1 && static_cast<s32>(sGnFrame) >= field_114_gnframe)
        {
            relive_new GroundExplosion(mXPos, mYPos, mSpriteScale);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        if (GetAnimation().GetCurrentFrame() == 1)
        {
            if (sMinePlayingSound == nullptr || sMinePlayingSound == this)
            {
                if (bInCamera)
                {
                    SfxPlayMono(relive::SoundEffects::RedTick, 35);
                }
                sMinePlayingSound = this;
            }
        }

        if (IsColliding())
        {
            field_10C_detonating = 1;
            field_114_gnframe = sGnFrame;
        }
    }

    if (field_10C_detonating != 1
        && (EventGet(kEventDeathReset)
            || mCurrentLevel != gMap.mCurrentLevel
            || mCurrentPath != gMap.mCurrentPath))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 Mine::IsColliding()
{
    const PSX_RECT bRect = VGetBoundingRect();

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit4_SetOffExplosives))
        {
            if (pObj->GetAnimation().mFlags.Get(AnimFlags::eRender))
            {
                const PSX_RECT bObjRect = pObj->VGetBoundingRect();

                if (FP_GetExponent(pObj->mXPos) > bRect.x && FP_GetExponent(pObj->mXPos) < bRect.w && FP_GetExponent(pObj->mYPos) < bRect.h + 5 && bRect.x <= bObjRect.w && bRect.w >= bObjRect.x && bRect.h >= bObjRect.y && bRect.y <= bObjRect.h && pObj->mSpriteScale == mSpriteScale)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

} // namespace AO
