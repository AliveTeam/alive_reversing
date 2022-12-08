#include "stdafx.h"
#include "Mine.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "GroundExplosion.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../AliveLibAE/FixedPoint.hpp"

Mine* sMineSFXOwner_5C3008 = nullptr;

void Mine::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine_Flash));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Mine));
}

Mine::Mine(relive::Path_Mine* pPath, const Guid& tlv)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eMine);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Mine));

    mDetonating = false;
    SetInteractive(true);
    SetCanExplode(true);

    if (pPath->mScale != relive::reliveScale::eFull)
    {
        if (pPath->mScale == relive::reliveScale::eHalf)
        {
            SetSpriteScale(FP_FromDouble(0.5));
            GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_Half_16);
            SetScale(Scale::Bg);
        }
    }
    else
    {
        SetSpriteScale(FP_FromDouble(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_RollingBallBombMineCar_35);
        SetScale(Scale::Fg);
    }

    const s32 v7 = pPath->mTopLeftX + pPath->mBottomRightX;
    mXPos = FP_FromInteger(v7 / 2);
    const FP v8 = FP_FromInteger(pPath->mTopLeftY);
    mYPos = v8;

    FP hitY;
    FP hitX;

    if (sCollisions->Raycast(
            FP_FromInteger(v7 / 2),
            v8,
            FP_FromInteger(v7 / 2),
            v8 + FP_FromInteger(24),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }
    mTlvId = tlv;
    mExplosionTimer = sGnFrame;
    mFlashAnim.Init(GetAnimRes(AnimId::Mine_Flash), this);

    mFlashAnim.mFlags.Set(AnimFlags::eSemiTrans);
    mFlashAnim.mFlags.Set(AnimFlags::eBlending);

    mFlashAnim.SetRenderLayer(GetAnimation().GetRenderLayer());
    mFlashAnim.SetSpriteScale(GetSpriteScale());
    mFlashAnim.SetRGB(128, 128, 128);

    field_1BC_flags.Clear(Mine_Flags_1BC::eBit1_PersistOffscreen);
    if (pPath->mPersistOffscreen == relive::reliveChoice::eYes)
    {
        field_1BC_flags.Set(Mine_Flags_1BC::eBit1_PersistOffscreen);
    }

    const FP gridSnap = ScaleToGridSize(GetSpriteScale());
    SetInteractive(true);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    mCollectionRect.x = mXPos - (gridSnap / FP_FromDouble(2.0));
    mCollectionRect.y = mYPos - gridSnap;
    mCollectionRect.w = (gridSnap / FP_FromDouble(2.0)) + mXPos;
    mCollectionRect.h = mYPos;
}


Mine::~Mine()
{
    if (mDetonating)
    {
        Path::TLV_Reset(mTlvId, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(mTlvId, -1, 0, 0);
    }

    mFlashAnim.VCleanUp();
    SetInteractive(false);

    if (sMineSFXOwner_5C3008 == this)
    {
        sMineSFXOwner_5C3008 = nullptr;
    }
}

void Mine::VUpdate()
{
    const s16 onScreen = gMap.Is_Point_In_Current_Camera(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos,
        0);

    if (mDetonating)
    {
        if (mDetonating == true && sGnFrame >= mExplosionTimer)
        {
            relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
            SetDead(true);
        }
    }
    else
    {
        if (GetAnimation().GetCurrentFrame() == 1
            && (!sMineSFXOwner_5C3008 || sMineSFXOwner_5C3008 == this))
        {
            if (onScreen)
            {
                SfxPlayMono(relive::SoundEffects::RedTick, 35);
                sMineSFXOwner_5C3008 = this;
            }
            else
            {
                sMineSFXOwner_5C3008 = nullptr;
            }
        }
        if (Mine::IsColliding())
        {
            mDetonating = true;
            mExplosionTimer = sGnFrame;
        }
    }
    if (mDetonating != true)
    {
        BaseGameObject* pEventObj = EventGet(kEventDeathReset);
        if (pEventObj || mCurrentLevel != gMap.mCurrentLevel || mCurrentPath != gMap.mCurrentPath)
        {
            SetDead(true);
        }
    }
}

void Mine::VRender(PrimHeader** ppOt)
{
    if (GetAnimation().mFlags.Get(AnimFlags::eRender))
    {
        if (gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            this->mFlashAnim.VRender(FP_GetExponent(mXPos - pScreenManager->CamXPos()),
                                                     FP_GetExponent(FP_FromInteger(mYOffset) + mYPos - pScreenManager->CamYPos()),
                                                     ppOt,
                                                     0,
                                                     0);

            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
    }
}

void Mine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel
        || gMap.mCurrentPath != gMap.mNextPath
        || !field_1BC_flags.Get(Mine_Flags_1BC::eBit1_PersistOffscreen))
    {
        SetDead(true);
    }
}

void Mine::VOnPickUpOrSlapped()
{
    if (mDetonating != true)
    {
        mDetonating = true;
        mExplosionTimer = sGnFrame + 5;
    }
}

void Mine::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
    SetDead(true);
    mDetonating = true;
}

s16 Mine::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead())
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        default:
            return 0;

        case ReliveTypes::eGreeter:
        case ReliveTypes::eAbe:
        case ReliveTypes::eMineCar:
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eAirExplosion:
        case ReliveTypes::eMudokon:
        case ReliveTypes::eShrykull:
            relive_new GroundExplosion(mXPos, mYPos, GetSpriteScale());
            SetDead(true);
            mDetonating = true;
            mExplosionTimer = sGnFrame;
            return 1;
    }
}

bool Mine::IsColliding()
{
    const PSX_RECT mineBound = VGetBoundingRect();

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);

        if (!pObj)
        {
            break;
        }

        // e114_Bit6 May be "can set off explosives?"
        if (pObj->mBaseAliveGameObjectFlags.Get(eCanSetOffExplosives) && pObj->GetAnimation().mFlags.Get(AnimFlags::eRender))
        {
            const PSX_RECT objBound = pObj->VGetBoundingRect();

            s32 objX = FP_GetExponent(pObj->mXPos);
            s32 objY = FP_GetExponent(pObj->mYPos);

            if (objX > mineBound.x && objX < mineBound.w && objY < mineBound.h + 12 && mineBound.x <= objBound.w && mineBound.w >= objBound.x && mineBound.h >= objBound.y && mineBound.y <= objBound.h && pObj->GetSpriteScale() == GetSpriteScale())
            {
                return 1;
            }
        }
    }

    return 0;
}
