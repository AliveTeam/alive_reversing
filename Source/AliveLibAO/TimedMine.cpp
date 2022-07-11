#include "stdafx_ao.h"
#include "Function.hpp"
#include "TimedMine.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "LiftPoint.hpp"
#include "BaseBomb.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"

namespace AO {

const TintEntry kTimedMineTints_4C3140[3] = {
    {EReliveLevelIds::eStockYards, 60u, 60u, 60u},
    {EReliveLevelIds::eStockYardsReturn, 60u, 60u, 60u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u},
};

TimedMine::TimedMine(Path_TimedMine* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eTimedMine;

    const AnimRecord rec = AO::AnimRec(AnimId::TimedMine_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::TimedMine_Idle, ppRes);

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mTimedMineFlags.Clear(TimedMineFlags::eStickToLiftPoint);
    mSlappedMine = 0;

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_Half_16;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_35;
    }

    InitTickAnimation();

    mTicksUntilExplosion = pTlv->mTicksUntilExplosion;

    mXPos = FP_FromInteger(pTlv->mTopLeft.x + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y + 24);

    mExplosionTimer = sGnFrame;
    mTlvInfo = tlvInfo;

    SetBaseAnimPaletteTint(kTimedMineTints_4C3140, gMap.mCurrentLevel, 1005);
    mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
    mCollectionRect.w = mXPos + (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
    mCollectionRect.h = mYPos;
    mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mLiftPoint = nullptr;
}

TimedMine::~TimedMine()
{
    if (mSlappedMine != 1 || static_cast<s32>(sGnFrame) < mExplosionTimer)
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
    }

    mTickAnim.VCleanUp();

    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
    }

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);
}

void TimedMine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mSlappedMine != 1)
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

s16 TimedMine::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->mBaseGameObjectTypeId)
    {
        case ReliveTypes::eAbe:
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eExplosion:
        case ReliveTypes::eShrykull:
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            relive_new BaseBomb(
                mXPos,
                mYPos,
                0,
                mSpriteScale);
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
            FP_GetExponent(mYPos + FP_FromInteger(pScreenManager->mCamYOff - FP_GetExponent(mSpriteScale * FP_FromInteger(14)))
                           - pScreenManager->mCamPos->y),
            ppOt,
            0,
            0);

        PSX_RECT pRect = {};
        mTickAnim.Get_Frame_Rect(&pRect);
        pScreenManager->InvalidateRectCurrentIdx(
            pRect.x,
            pRect.y,
            pRect.w,
            pRect.h);
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void TimedMine::InitTickAnimation()
{
    if (mTickAnim.Init(
        AnimId::Bomb_RedGreenTick,
        this,
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kBombflshAOResID, 1, 0)))
    {
        mTickAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
        mTickAnim.mFlags.Set(AnimFlags::eBit16_bBlending);
        mTickAnim.mRenderLayer = mAnim.mRenderLayer;
        mTickAnim.field_14_scale = mSpriteScale;
        mTickAnim.mRed = 128;
        mTickAnim.mGreen = 128;
        mTickAnim.mBlue = 128;
        mTickAnim.mRenderMode = TPageAbr::eBlend_1;
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

void TimedMine::StickToLiftPoint()
{
    FP hitY = {};
    FP hitX = {};
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
            (mSpriteScale != FP_FromDouble(0.5)) ? kFgWallsOrFloor : kBgWallsOrFloor))
    {
        if (pLine->mLineType == eLineTypes ::eDynamicCollision_32 ||
            pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            if (ObjListPlatforms_50766C)
            {
                for (s32 i = 0; i < ObjListPlatforms_50766C->Size(); i++)
                {
                    BaseGameObject* pObj = ObjListPlatforms_50766C->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint)
                    {
                        auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                        const PSX_RECT pObjRect = pLiftPoint->VGetBoundingRect();

                        if (FP_GetExponent(mXPos) > pObjRect.x && FP_GetExponent(mXPos) < pObjRect.w && FP_GetExponent(mYPos) < pObjRect.h)
                        {
                            mLiftPoint = pLiftPoint;
                            pLiftPoint->VAdd(this);
                            mLiftPoint->mBaseGameObjectRefCount++;
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
    auto pPlatform = static_cast<LiftPoint*>(mLiftPoint);
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!mTimedMineFlags.Get(TimedMineFlags::eStickToLiftPoint))
    {
        StickToLiftPoint();
    }

    if (pPlatform && pPlatform->OnAnyFloor())
    {
        mCollectionRect.x = mXPos - ScaleToGridSize(mSpriteScale) / FP_FromInteger(2);
        mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
        mCollectionRect.w = mXPos + ScaleToGridSize(mSpriteScale) / FP_FromInteger(2);
        mCollectionRect.h = mYPos;
    }
    if (mSlappedMine == 1)
    {
        if (static_cast<s32>(sGnFrame) > (mSingleTickTimer + mOldGnFrame))
        {
            mOldGnFrame = sGnFrame;
            auto soundDir = gMap.GetDirection(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos);
            SFX_Play_Camera(SoundEffect::GreenTick_3, 50, soundDir);

            //~7 limits the number to multiples of 8
            if (((mExplosionTimer - sGnFrame) & ~7) >= 18 * 8)
            {
                mSingleTickTimer = 18;
            }
            else
            {
                mSingleTickTimer = (mExplosionTimer - sGnFrame) / 8;
            }
        }
        if (static_cast<s32>(sGnFrame) >= mExplosionTimer)
        {
            relive_new BaseBomb(
                mXPos,
                mYPos,
                0,
                mSpriteScale);

            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

void TimedMine::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    relive_new BaseBomb(
        mXPos,
        mYPos,
        0,
        mSpriteScale);

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
        // TODO: missing animid
        mAnim.Set_Animation_Data(804, nullptr);
        mExplosionTimer = sGnFrame + mTicksUntilExplosion;
        mTickAnim.Set_Animation_Data(AnimId::Bomb_Flash, nullptr);
        SfxPlayMono(SoundEffect::GreenTick_3, 0, 0);
    }
}

} // namespace AO
