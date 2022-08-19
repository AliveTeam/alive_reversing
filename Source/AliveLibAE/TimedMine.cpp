#include "stdafx.h"
#include "TimedMine.hpp"
#include "UXB.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/VRam.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "BaseBomb.hpp"
#include "LiftPoint.hpp"
#include "Grid.hpp"

static TintEntry sTimedMineTint_550EB8[1] = {{EReliveLevelIds::eNone, 127u, 127u, 127u}};


TimedMine::TimedMine(Path_TimedMine* pPath, TlvItemInfoUnion tlv)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eTimedMine_or_MovingBomb);

    const AnimRecord& rec = AnimRec(AnimId::TimedMine_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::TimedMine_Idle, ppRes);

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mTimedMineFlags.Clear(TimedMineFlags::eStickToLiftPoint);
    mSlappedMine = 0;

    if (pPath->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_Half_16;
    }
    else
    {
        mSpriteScale = FP_FromDouble(1.0);
        mScale = Scale::Fg;
        mAnim.mRenderLayer = Layer::eLayer_RollingBallBombMineCar_35;
    }

    InitTickAnimation();

    mTicksUntilExplosion = pPath->mTicksUntilExplosion;

    mXPos = FP_FromInteger((pPath->mTopLeft.x + pPath->mBottomRight.x) / 2);
    mYPos = FP_FromInteger(pPath->mTopLeft.y);

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
            mScale == Scale::Fg ? kFgFloor : kBgFloor))
    {
        mYPos = hitY;
    }

    mTlvInfo = tlv.all;
    mExplosionTimer = sGnFrame;
    SetBaseAnimPaletteTint(sTimedMineTint_550EB8, gMap.mCurrentLevel, kBombResID);

    const FP gridSnap = ScaleToGridSize(mSpriteScale);
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);

    mCollectionRect.x = mXPos - (gridSnap / FP_FromDouble(2.0));
    mCollectionRect.y = mYPos - gridSnap;
    mCollectionRect.w = (gridSnap / FP_FromDouble(2.0)) + mXPos;
    mCollectionRect.h = mYPos;

    BaseAliveGameObject_PlatformId = -1;
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
        mCollectionRect.x = mXPos - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
        mCollectionRect.w = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2)) + mXPos;
        mCollectionRect.h = mYPos;
        mCollectionRect.y = mYPos - ScaleToGridSize(mSpriteScale);
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
            SFX_Play_Camera(SoundEffect::GreenTick_2, 50, soundDir);

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
            relive_new BaseBomb(
                mXPos,
                mYPos,
                0,
                mSpriteScale);

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
            FP_GetExponent((mYPos - pScreenManager->CamYPos() - FP_NoFractional(mSpriteScale * FP_FromDouble(14)))),
            ppOt,
            0,
            0);

        PSX_RECT frameRect = {};
        mTickAnim.Get_Frame_Rect(&frameRect);

        pScreenManager->InvalidateRectCurrentIdx(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h);

        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void TimedMine::InitTickAnimation()
{
    const AnimRecord& tickRec = AnimRec(AnimId::Bomb_RedGreenTick);
    if (mTickAnim.Init(AnimId::Bomb_RedGreenTick, this, Add_Resource(ResourceManager::Resource_Animation, tickRec.mResourceId)))
    {
        mTickAnim.mRenderLayer = mAnim.mRenderLayer;
        mTickAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
        mTickAnim.mFlags.Set(AnimFlags::eBit16_bBlending);
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
    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    mTimedMineFlags.Set(TimedMineFlags::eStickToLiftPoint);
    if (sCollisions->Raycast(
            mXPos,
            mYPos - FP_FromInteger(20),
            mXPos, mYPos + FP_FromInteger(20),
            &pLine, &hitX, &hitY,
            (mScale == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {
        if (pLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            if (ObjList_5C1B78)
            {
                for (s32 i = 0; i < ObjList_5C1B78->Size(); i++)
                {
                    BaseGameObject* pObj = ObjList_5C1B78->ItemAt(i);
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
        BaseAliveGameObject_PlatformId = -1;
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
        case ReliveTypes::eExplosion:
        case ReliveTypes::eShrykull:
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            relive_new BaseBomb(mXPos, mYPos, 0, mSpriteScale);
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
    relive_new BaseBomb(mXPos, mYPos, 0, mSpriteScale);
    
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
        mAnim.Set_Animation_Data(AnimId::TimedMine_Activated, nullptr);
        mExplosionTimer = sGnFrame + mTicksUntilExplosion;
        mTickAnim.Set_Animation_Data(AnimId::Bomb_Flash, nullptr);
        SfxPlayMono(SoundEffect::GreenTick_2, 0);
    }
}
