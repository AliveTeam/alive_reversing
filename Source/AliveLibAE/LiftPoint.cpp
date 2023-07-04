#include "stdafx.h"
#include "LiftPoint.hpp"
#include "../relive_lib/Function.hpp"
#include "Rope.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Abe.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "PathData.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "QuikSave.hpp"

struct LiftPointData final
{
    AnimId mPlatformAnimId;
    AnimId mLiftBottomWheelAnimId;
    AnimId mLiftTopWheelAnimId;
};

static const LiftPointData sLiftPointAnimIds[18] = {
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines},
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines},
    {AnimId::LiftPlatform_Necrum, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum},
    {AnimId::LiftPlatform_Necrum, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum},
    {AnimId::LiftPlatform_Necrum, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum},
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines},
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines},
    {AnimId::LiftPlatform_Necrum, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum},
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines},
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines},
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines},
    {AnimId::LiftPlatform_Necrum, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum},
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines},
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines},
    {AnimId::LiftPlatform_Mines, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines}};

static const TintEntry sLiftTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 107u, 107u, 107u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

void LiftPoint::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftPlatform_Mines));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftBottomWheel_Mines));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftTopWheel_Mines));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftPlatform_Necrum));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftBottomWheel_Necrum));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::LiftTopWheel_Necrum));
}

LiftPoint::LiftPoint(relive::Path_LiftPoint* pTlv, const Guid& tlvId)
{
    mBaseGameObjectTlvInfo = tlvId;
    SetType(ReliveTypes::eLiftPoint);

    LoadAnimations();

    pTlv->mTlvSpecificMeaning = 3;

    mTlvId = gPathInfo->TLVInfo_From_TLVPtr(pTlv);

   // u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kLiftResID);
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    const LiftPointData& rPlatformData = sLiftPointAnimIds[static_cast<u32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    AddDynamicCollision(
        rPlatformData.mPlatformAnimId,
        pTlv,
        tlvId);

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        mPlatformBaseCollisionLine->mLineType = eLineTypes::eBackgroundDynamicCollision_36;
    }

    SetTint(sLiftTints, gMap.mCurrentLevel);

    const FP oldX = mXPos;
    MapFollowMe(true);
    const s16 xSnapDelta = FP_GetExponent(mXPos - oldX);
    mPlatformBaseXOffset -= xSnapDelta;
    mPlatformBaseWidthOffset -= xSnapDelta;

    GetAnimation().SetSemiTrans(true);


    const LiftPointData& rLiftWheelData = sLiftPointAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    if (mLiftWheelAnim.Init(
            GetAnimRes(rLiftWheelData.mLiftBottomWheelAnimId),
            this))
    {
        if (pTlv->mScale == relive::reliveScale::eHalf)
        {
            mLiftWheelAnim.SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
            mLiftWheelAnim.SetSpriteScale(GetSpriteScale());
        }
        else
        {
            mLiftWheelAnim.SetRenderLayer(Layer::eLayer_BeforeShadow_25);
            mLiftWheelAnim.SetSpriteScale(GetSpriteScale());
        }

        mLiftWheelAnim.SetAnimate(false);
        mLiftWheelAnim.SetSemiTrans(false);
        mLiftWheelAnim.SetBlending(false);

        mMoving = false;
        mTopFloor = false;
        mMiddleFloor = false;
        mBottomFloor = false;
        mMoveToFloorLevel = false;

        // TODO: red is set to blue and vice versa - fix me
        //mLiftWheelAnim.mGreen = static_cast<u8>(mRGB.g);
        //mLiftWheelAnim.mRed = static_cast<u8>(mRGB.b);
        //mLiftWheelAnim.mBlue = static_cast<u8>(mRGB.r);

        mLiftWheelAnim.SetRGB(mRGB.r, mRGB.g, mRGB.b);
        mLiftWheelAnim.SetBlendMode(relive::TBlendModes::eBlend_0);

        mVelX = FP_FromInteger(0);
        mVelY = FP_FromInteger(0);

        const FP k25 = FP_FromInteger(25);
        const FP k13 = FP_FromInteger(13);
        const FP km10 = FP_FromInteger(-10);


        auto pRope1 = relive_new Rope(FP_GetExponent((k13 * GetSpriteScale() + mXPos)),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * GetSpriteScale()) + mYPos),
                                   GetSpriteScale());
        if (pRope1)
        {
            mRopeId1 = pRope1->mBaseGameObjectId;
        }

        auto pRope2 = relive_new Rope(FP_GetExponent((km10 * GetSpriteScale()) + mXPos),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * GetSpriteScale()) + mYPos),
                                   GetSpriteScale());
        if (pRope2)
        {
            mRopeId2 = pRope2->mBaseGameObjectId;
        }

        pRope2->mBottom = FP_GetExponent((k25 * GetSpriteScale()) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y));
        pRope1->mBottom = FP_GetExponent((k25 * GetSpriteScale()) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y));

        const FP v28 = mYPos * FP_FromDouble(1.5);
        const FP v29 = FP_FromRaw(FP_GetExponent(v28 * GetSpriteScale()) % FP_FromInteger(pRope2->mRopeLength).fpValue);

        pRope2->mYPos = FP_FromInteger(FP_GetExponent(v29 + (k25 * GetSpriteScale()) + mYPos + FP_FromInteger(pRope2->mRopeLength)));
        pRope1->mYPos = FP_FromInteger(FP_GetExponent((k25 * GetSpriteScale()) + mYPos + FP_FromInteger(pRope1->mRopeLength) - v29));

        mHasPulley = false;
        CreatePulleyIfExists();

        mLiftPointId = static_cast<s8>(pTlv->mLiftPointId);
        mLiftPointStopType = pTlv->mLiftPointStopType;

        switch (mLiftPointStopType)
        {
            case relive::Path_LiftPoint::LiftPointStopType::eTopFloor:
                mTopFloor = true;
                break;

            case relive::Path_LiftPoint::LiftPointStopType::eBottomFloor:
                mBottomFloor = true;
                break;

            case relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor:
                mMiddleFloor = true;
                break;

            default:
            case relive::Path_LiftPoint::LiftPointStopType::eMiddleLockFloor: // Not used ??
            case relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly:
                mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
                break;
        }

        mKeepOnMiddleFloor = false;
    }
    else
    {
        SetListAddFailed(true);
    }
}

void LiftPoint::CreateFromSaveState(SerializedObjectData& pData)
{
    const auto pState = pData.ReadTmpPtr<LiftPointSaveState>();

    relive::Path_LiftPoint* pTlv = static_cast<relive::Path_LiftPoint*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mPlatformId));

    auto pLiftPoint = relive_new LiftPoint(pTlv, pState->mPlatformId);
    if (pLiftPoint)
    {
        pLiftPoint->mXPos = pState->mXPos;
        pLiftPoint->mYPos = pState->mYPos;
        pLiftPoint->SyncCollisionLinePosition();

        Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->mRopeId2, ReliveTypes::eRope));
        Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->mRopeId1, ReliveTypes::eRope));

        pRope2->mBottom = FP_GetExponent(FP_FromInteger(pLiftPoint->mPlatformBaseCollisionLine->mRect.y) + (FP_FromInteger(25) * pLiftPoint->GetSpriteScale()));
        pRope1->mBottom = FP_GetExponent(FP_FromInteger(pLiftPoint->mPlatformBaseCollisionLine->mRect.y) + (FP_FromInteger(25) * pLiftPoint->GetSpriteScale()));

        if (pLiftPoint->mHasPulley)
        {
            pRope2->mTop = FP_GetExponent(FP_FromInteger(pLiftPoint->mPulleyYPos) + FP_FromInteger(-19) * pLiftPoint->GetSpriteScale());
            pRope1->mTop = FP_GetExponent(FP_FromInteger(pLiftPoint->mPulleyYPos) + FP_FromInteger(-19) * pLiftPoint->GetSpriteScale());
        }

        pLiftPoint->mPlatformBaseTlvInfo = pState->mPlatformId;
        pLiftPoint->mTlvId = pState->mTlvId;
        pLiftPoint->mFloorLevelY = pState->mFloorLevelY;
        pLiftPoint->mLiftPointStopType = pState->mLiftPointStopType;

        pLiftPoint->mMoving = pState->mMoving;
        pLiftPoint->mTopFloor = pState->mTopFloor;
        pLiftPoint->mMiddleFloor = pState->mMiddleFloor;
        pLiftPoint->mBottomFloor = pState->mBottomFloor;
        pLiftPoint->mMoveToFloorLevel = pState->mMoveToFloorLevel;
        pLiftPoint->mKeepOnMiddleFloor = pState->mKeepOnMiddleFloor;
    }

    if (pState->mTlvId == pState->mPlatformId)
    {
        return;
    }

    pTlv->mTlvSpecificMeaning = 1;
    if (pState->mTlvId == Guid{})
    {
        return;
    }

    relive::Path_TLV* pTlv2 = gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvId);
    pTlv2->mTlvSpecificMeaning = 3;
}

void LiftPoint::vKeepOnMiddleFloor()
{
    mKeepOnMiddleFloor = true;
}

bool LiftPoint::vOnTopFloor()
{
    return mTopFloor && !mMoveToFloorLevel;
}

bool LiftPoint::vOnMiddleFloor()
{
    return mMiddleFloor && !mMoveToFloorLevel;
}

bool LiftPoint::vOnBottomFloor()
{
    return mBottomFloor && !mMoveToFloorLevel;
}

bool LiftPoint::vOnAnyFloor()
{
    return vOnBottomFloor() || vOnTopFloor() || vOnMiddleFloor();
}

bool LiftPoint::vOnAFloorLiftMoverCanUse()
{
    // Top or bottom floor can still be activated by the lift mover?
    return (vOnMiddleFloor() && !mIgnoreLiftMover) || vOnBottomFloor() || vOnTopFloor();
}

bool LiftPoint::vMovingToFloorLevel()
{
    return mMoveToFloorLevel;
}

void LiftPoint::Move(FP xSpeed, FP ySpeed)
{
    mVelX = xSpeed * GetSpriteScale();
    mVelY = ySpeed * GetSpriteScale();

    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        mMoving = true;
    }
}

void LiftPoint::VUpdate()
{
    if (mMoving)
    {
        if (mMoveToFloorLevel)
        {
            const bool bOnFloor = mFloorLevelY == mYPos;
            const FP distToFloor = mFloorLevelY - mYPos;

            FP absDistToFloor = {};
            if (distToFloor < FP_FromInteger(0) || bOnFloor)
            {
                absDistToFloor = -distToFloor;
            }
            else
            {
                absDistToFloor = distToFloor;
            }

            if (absDistToFloor >= FP_FromInteger(2) * GetSpriteScale())
            {
                if (distToFloor >= FP_FromInteger(0))
                {
                    mVelY = FP_FromInteger(2) * GetSpriteScale();
                }
                else
                {
                    mVelY = -(FP_FromInteger(2) * GetSpriteScale());
                }
                mMoving = true;
            }
            else
            {
                mYPos = mFloorLevelY;
                mVelY = FP_FromInteger(0);
                mMoving = false;
                mMoveToFloorLevel = false;
                SfxPlayMono(relive::SoundEffects::LiftStop, 0);
                SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -2000);
                EventBroadcast(kEventNoise, this);
                EventBroadcast(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
            const FP lineY = FP_FromInteger(mPlatformBaseCollisionLine->mRect.y);
            relive::Path_TLV* pTlvIter = gPathInfo->TlvGetAt(
                nullptr,
                mXPos,
                lineY,
                mXPos,
                (GetSpriteScale() * FP_FromInteger(30)) + lineY);

            if (pTlvIter)
            {
                while (pTlvIter->mTlvType != ReliveTypes::eLiftPoint)
                {
                    pTlvIter = gPathInfo->TlvGetAt(
                        pTlvIter,
                        mXPos,
                        lineY,
                        mXPos,
                        lineY + (GetSpriteScale() * FP_FromInteger(30)));

                    if (!pTlvIter)
                    {
                        break;
                    }
                }
            }

            // TODO: Bugged if lift point not found - last TLV will get casted which could be anything
            auto pLiftTlv = static_cast<relive::Path_LiftPoint*>(pTlvIter);
            if (pLiftTlv)
            {
                mLiftPointStopType = pLiftTlv->mLiftPointStopType;

                mIgnoreLiftMover = false;
                if (pLiftTlv->mIgnoreLiftMover == relive::reliveChoice::eYes)
                {
                    mIgnoreLiftMover = true;
                }
            }

            // TODO: Also bugged because will always be true
            if (pLiftTlv)
            {
                ClearTlvFlags(pLiftTlv);
                mFloorLevelY = FP_FromInteger(pLiftTlv->mTopLeftY + -mPlatformBaseYOffset);
            }
            else
            {
                mFloorLevelY = FP_FromInteger(0);
                mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
            }

            const FP distanceToFloor = mFloorLevelY - mYPos;
            const FP kMinus25Scaled = GetSpriteScale() * FP_FromInteger(-25);
            const FP k30Scaled = GetSpriteScale() * FP_FromInteger(30);

            switch (mLiftPointStopType)
            {
                case relive::Path_LiftPoint::LiftPointStopType::eTopFloor:
                    if (mVelY >= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;
                            mTopFloor = false;
                            mTlvId = Guid{};
                        }
                        else
                        {
                            mMoveToFloorLevel = true;
                            mMoving = true;
                            mYPos = mFloorLevelY - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            mTlvId = gPathInfo->TLVInfo_From_TLVPtr(pLiftTlv);
                            pLiftTlv->mLiftPointId = mLiftPointId;
                            mTopFloor = true;
                        }
                    }
                    else if (mVelY + lineY <= FP_FromInteger(pLiftTlv->mTopLeftY))
                    {
                        vStayOnFloor(mTopFloor, pLiftTlv);
                        mTopFloor = true;
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eBottomFloor:
                    if (mVelY <= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;

                            mBottomFloor = false;
                            mTlvId = Guid{};
                        }
                        else
                        {
                            mMoving = true;
                            mMoveToFloorLevel = true;
                            mYPos = mFloorLevelY - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            mTlvId = gPathInfo->TLVInfo_From_TLVPtr(pLiftTlv);
                            pLiftTlv->mLiftPointId = mLiftPointId;
                            mBottomFloor = true;
                        }
                    }
                    else if (lineY + mVelY >= FP_FromInteger(pLiftTlv->mTopLeftY))
                    {
                        vStayOnFloor(mBottomFloor, pLiftTlv);
                        mBottomFloor = true;
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor:
                    if (distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                    {
                        pLiftTlv->mTlvSpecificMeaning = 1;
                        mTlvId = Guid{};
                    }
                    else
                    {
                        if (mKeepOnMiddleFloor)
                        {
                            vStayOnFloor(mMiddleFloor, pLiftTlv);
                            mKeepOnMiddleFloor = false;
                        }

                        if (mVelY == FP_FromInteger(0))
                        {
                            mMoving = true;
                            mMoveToFloorLevel = true;
                            mYPos = mFloorLevelY - distanceToFloor;
                        }

                        pLiftTlv->mTlvSpecificMeaning = 3;
                        mTlvId = gPathInfo->TLVInfo_From_TLVPtr(pLiftTlv);
                        pLiftTlv->mLiftPointId = mLiftPointId;
                        mMiddleFloor = true;
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly:
                    if (pLiftTlv)
                    {
                        pLiftTlv->mTlvSpecificMeaning = 1;
                        mTlvId = Guid{};
                    }
                    mTopFloor = false;
                    mMiddleFloor = false;
                    mBottomFloor = false;
                    break;
                default:
                    break;
            }
        }

        mXPos += mVelX;
        mYPos += mVelY;

        if (mPlatformBaseCollisionLine)
        {
            SyncCollisionLinePosition();
        }

        MoveObjectsOnLift(mVelX);
    }

    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(mRopeId2, ReliveTypes::eRope));
    pRope2->mBottom = FP_GetExponent((FP_FromInteger(mPlatformBaseCollisionLine->mRect.y) + (FP_FromInteger(25) * GetSpriteScale())));

    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(mRopeId1, ReliveTypes::eRope));
    pRope1->mBottom = FP_GetExponent((FP_FromInteger(mPlatformBaseCollisionLine->mRect.y) + (FP_FromInteger(25) * GetSpriteScale())));

    if (mHasPulley)
    {
        pRope2->mTop = FP_GetExponent((FP_FromInteger(mPulleyYPos) + (FP_FromInteger(-19) * GetSpriteScale())));
        pRope1->mTop = FP_GetExponent((FP_FromInteger(mPulleyYPos) + (FP_FromInteger(-19) * GetSpriteScale())));
    }

    const FP v39 = mYPos * FP_FromDouble(1.5);
    const FP v40 = FP_FromRaw(FP_GetExponent(v39 * GetSpriteScale()) % FP_FromInteger(pRope2->mRopeLength).fpValue);
    pRope2->mYPos = FP_NoFractional(v40 + (FP_FromInteger(25) * GetSpriteScale()) + mYPos + FP_FromInteger(pRope2->mRopeLength));
    pRope1->mYPos = FP_NoFractional((FP_FromInteger(25) * GetSpriteScale()) + mYPos + FP_FromInteger(pRope1->mRopeLength) - v40);

    mLiftWheelAnim.SetAnimate(true);
    mPulleyAnim.SetAnimate(true);

    if (mVelY == FP_FromInteger(0))
    {
        // Wheels are stopped if not moving
        mLiftWheelAnim.SetAnimate(false);
        mPulleyAnim.SetAnimate(false);
    }
    else if (mVelY > FP_FromInteger(0))
    {
        // Pulley/lift wheels spin opposite ways for up/down
        mLiftWheelAnim.SetLoopBackwards(false);
        mPulleyAnim.SetLoopBackwards(true);
    }
    else
    {
        mLiftWheelAnim.SetLoopBackwards(true);
        mPulleyAnim.SetLoopBackwards(false);
    }

    if (gMap.mCurrentLevel == EReliveLevelIds::eNecrum || gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault || gMap.mCurrentLevel == EReliveLevelIds::eMudomoVault_Ender || gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault || gMap.mCurrentLevel == EReliveLevelIds::eMudancheeVault_Ender)
    {
        if (mLiftWheelAnim.GetCurrentFrame() == 1 && mLiftWheelAnim.GetAnimate())
        {
            SfxPlayMono(relive::SoundEffects::WheelSqueak, 0);
        }
    }
    else if (mLiftWheelAnim.GetCurrentFrame() == 1 && mLiftWheelAnim.GetAnimate() && sGnFrame & 1)
    {
        SfxPlayMono(relive::SoundEffects::WheelSqueak, 0);
    }

    if ((mCurrentLevel != gMap.mCurrentLevel || mCurrentPath != gMap.mCurrentPath || EventGet(kEventDeathReset))
        && mPlatformBaseCount <= 0)
    {
        SetDead(true);
    }
}

void LiftPoint::VRender(BasePrimitive** ppOt)
{
    // Renders the pulley, lift platform and lift platform wheel

    // In the current level/map?
    if (mCurrentLevel == gMap.mCurrentLevel && mCurrentPath == gMap.mCurrentPath)
    {
        // Within the current camera X bounds?
        PSX_Point camPos = {};
        gMap.GetCurrentCamCoords(&camPos);

        if (mXPos >= FP_FromInteger(camPos.x) && mXPos <= FP_FromInteger(camPos.x + 640))
        {
            s16 r = mRGB.r;
            s16 g = mRGB.g;
            s16 b = mRGB.b;

            const PSX_RECT bRect = VGetBoundingRect();
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mXPos),
                (bRect.h + bRect.y) / 2,
                GetScale(),
                &r,
                &g,
                &b);

            mLiftWheelAnim.SetRGB(r, g, b);

            if (gMap.mCurrentLevel != EReliveLevelIds::eNecrum && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                mLiftWheelAnim.VRender(
                    FP_GetExponent(mXPos - gScreenManager->CamXPos() + (FP_FromInteger(3) * GetSpriteScale())),
                    FP_GetExponent(mYPos - gScreenManager->CamYPos() + (FP_FromInteger(-5) * GetSpriteScale())),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                mLiftWheelAnim.Get_Frame_Rect(&frameRect);
            }

            if (mHasPulley)
            {
                if (gMap.Is_Point_In_Current_Camera(
                        mCurrentLevel,
                        mCurrentPath,
                        FP_FromInteger(mPulleyXPos),
                        FP_FromInteger(mPulleyYPos),
                        0))
                {
                    r = mRGB.r;
                    g = mRGB.g;
                    b = mRGB.b;

                    ShadowZone::ShadowZones_Calculate_Colour(
                        mPulleyXPos,
                        mPulleyYPos,
                        GetScale(),
                        &r,
                        &g,
                        &b);

                    mPulleyAnim.SetRGB(r, g, b);

                    mPulleyAnim.VRender(
                        FP_GetExponent(FP_FromInteger(mPulleyXPos) - gScreenManager->CamXPos()),
                        FP_GetExponent(FP_FromInteger(mPulleyYPos) - gScreenManager->CamYPos()),
                        ppOt,
                        0,
                        0);

                    PSX_RECT frameRect = {};
                    mPulleyAnim.Get_Frame_Rect(&frameRect);
                }
            }

            // The base animation is the actual lift/platform itself
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

            if (gMap.mCurrentLevel == EReliveLevelIds::eNecrum && Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
            {
                mLiftWheelAnim.VRender(
                    FP_GetExponent(mXPos - gScreenManager->CamXPos() + (FP_FromInteger(3) * GetSpriteScale())),
                    FP_GetExponent(mYPos - gScreenManager->CamYPos() + (FP_FromInteger(-5) * GetSpriteScale())),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                mLiftWheelAnim.Get_Frame_Rect(&frameRect);
            }
        }
    }
}

void LiftPoint::MoveObjectsOnLift(FP xVelocity)
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        BaseGameObject* pObjectsLiftPoint = sObjectIds.Find_Impl(pObj->BaseAliveGameObject_PlatformId);
        if (pObjectsLiftPoint == this)
        {
            if (pObj == sControlledCharacter)
            {
                // Keep the player in the screen
                const FP oldVelY = pObj->mVelY;
                pObj->mVelY = mVelY;
                pObj->SetActiveCameraDelayedFromDir();

                // And keep the old velY
                pObj->mVelY = oldVelY;
            }

            // Do platforms ever move horizontally? This is always 0 it seems
            pObj->mXPos += xVelocity;

            // Keep ypos on the platform
            pObj->mYPos = FP_FromInteger(mPlatformBaseCollisionLine->mRect.y);

            if (pObj->GetInteractive())
            {
                pObj->mCollectionRect.x = pObj->mXPos - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                pObj->mCollectionRect.y = pObj->mYPos - ScaleToGridSize(GetSpriteScale());
                pObj->mCollectionRect.w = pObj->mXPos + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                pObj->mCollectionRect.h = pObj->mYPos;
            }
        }
    }
}

void LiftPoint::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath)
    {
        SetDead(true);
    }
}


void LiftPoint::ClearTlvFlags(relive::Path_TLV* pTlv)
{
    pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
    pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
    pTlv->mTlvSpecificMeaning |= 1;
}

void LiftPoint::vStayOnFloor(bool floor, relive::Path_LiftPoint* pTlv)
{
    if (!floor)
    {
        mYPos = FP_FromInteger(pTlv->mTopLeftY + -mPlatformBaseYOffset);
        SfxPlayMono(relive::SoundEffects::LiftStop, 0);
        SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -2000);
    }

    mMoving = false;
    pTlv->mTlvSpecificMeaning = 3;
    mTlvId = gPathInfo->TLVInfo_From_TLVPtr(pTlv);
    pTlv->mLiftPointId = mLiftPointId;
    mVelY = FP_FromInteger(0);

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}

void LiftPoint::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    LiftPointSaveState data = {};

    data.mType = ReliveTypes::eLiftPoint;
    data.mXPos = mXPos;
    data.mYPos = mYPos;
    data.mPlatformId = mPlatformBaseTlvInfo;
    data.mTlvId = mTlvId;
    data.mFloorLevelY = mFloorLevelY;
    data.mLiftPointStopType = mLiftPointStopType;

    data.mMoving = mMoving;
    data.mTopFloor = mTopFloor;
    data.mMiddleFloor = mMiddleFloor;
    data.mBottomFloor = mBottomFloor;
    data.mMoveToFloorLevel = mMoveToFloorLevel;
    data.mKeepOnMiddleFloor = mKeepOnMiddleFloor;

    pSaveBuffer.Write(data);
}

void LiftPoint::CreatePulleyIfExists()
{
    relive::Path_TLV* pFound = nullptr;

    const PathData* pPathData = gPathInfo->mPathData;
    s16 yCamIdx = FP_GetExponent(mYPos) / pPathData->field_C_grid_height;
    // If we are in the top row of cameras then there can't be a pulley in the screen above because there are no more screens above!
    while (yCamIdx >= 0)
    {
        const s16 xCamIdx = (FP_GetExponent(mXPos) / pPathData->field_A_grid_width) - gMap.mCamIdxOnX;
        // Keep looking up 1 camera for any camera that has TLVs in it.
        relive::Path_TLV* pTlvIter = gPathInfo->Get_First_TLV_For_Offsetted_Camera(xCamIdx, yCamIdx - gMap.mCamIdxOnY);
        while (pTlvIter)
        {
            if (pTlvIter->mTlvType == ReliveTypes::ePulley)
            {
                const FP left = FP_FromInteger(mPlatformBaseCollisionLine->mRect.x) + (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                if (left <= FP_FromInteger(pTlvIter->mTopLeftX))
                {
                    const FP right = FP_FromInteger(mPlatformBaseCollisionLine->mRect.w) - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
                    if (FP_FromInteger(pTlvIter->mTopLeftX) <= right)
                    {
                        pFound = pTlvIter;
                        break;
                    }
                }
            }
            pTlvIter = Path::TLV_Next_Of_Type(pTlvIter, ReliveTypes::ePulley);
        }

        if (pFound)
        {
            // Found a pulley, stop searching
            break;
        }

        // Go 1 more camera up
        yCamIdx--;
    }

    if (!pFound)
    {
        return;
    }

    // Calculate pulley position
    const FP k13_scaled = FP_FromInteger(13) * GetSpriteScale();
    const FP kM10_scaled = FP_FromInteger(-10) * GetSpriteScale();

    mPulleyXPos = FP_GetExponent(((kM10_scaled + k13_scaled) / FP_FromInteger(2)) + FP_NoFractional(mXPos));
    mPulleyYPos = pFound->mTopLeftY;

    const LiftPointData& data = sLiftPointAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))];
    mPulleyAnim.Init(
        GetAnimRes(data.mLiftTopWheelAnimId),
        this);

    mPulleyAnim.SetAnimate(false);
    mPulleyAnim.SetSemiTrans(false);
    mPulleyAnim.SetBlending(false);

    mHasPulley = true;

    mPulleyAnim.SetRenderLayer(GetAnimation().GetRenderLayer());
    mPulleyAnim.SetSpriteScale(GetSpriteScale());

    mPulleyAnim.SetRGB(mRGB.r, mRGB.g, mRGB.b);

    mPulleyAnim.SetBlendMode(relive::TBlendModes::eBlend_0);

    // Set the top of the ropes to be the bottom of the pulley
    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(mRopeId2, ReliveTypes::eRope));
    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(mRopeId1, ReliveTypes::eRope));

    pRope1->mTop = FP_GetExponent(FP_FromInteger(mPulleyYPos) + (FP_FromInteger(-19) * GetSpriteScale()));
    pRope2->mTop = FP_GetExponent(FP_FromInteger(mPulleyYPos) + (FP_FromInteger(-19) * GetSpriteScale()));
}

LiftPoint::~LiftPoint()
{
    BaseGameObject* pRope2 = sObjectIds.Find(mRopeId2, ReliveTypes::eRope);
    BaseGameObject* pRope1 = sObjectIds.Find(mRopeId1, ReliveTypes::eRope);
    if (pRope2)
    {
        pRope2->SetDead(true);
        mRopeId2 = Guid{};
    }

    if (pRope1)
    {
        pRope1->SetDead(true);
        mRopeId1 = Guid{};
    }

    Path::TLV_Reset(mPlatformBaseTlvInfo, -1, 0, 0);

    relive::Path_TLV* pTlv = gPathInfo->TLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(GetSpriteScale() * FP_FromInteger(30)),
        FP_GetExponent(mXPos),
        FP_GetExponent(FP_FromInteger(mPlatformBaseCollisionLine->mRect.y) + (GetSpriteScale() * FP_FromInteger(30))),
        ReliveTypes::eLiftPoint);

    if (pTlv)
    {
        pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
        pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
    }

    mLiftWheelAnim.VCleanUp();

    if (mHasPulley)
    {
        mPulleyAnim.VCleanUp();
    }
}
