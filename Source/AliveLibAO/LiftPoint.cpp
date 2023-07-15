#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Rope.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

struct LiftPointCoord final
{
    s32 field_0;
    s32 field_4;
};

struct LiftPointData final
{
    AnimId mPlatformAnimId;
    AnimId mLiftBottomWheelAnimId;
    AnimId mLiftTopWheelAnimId;
};

static const LiftPointData sLiftPointAnimIds[16] = {
    {AnimId::LiftPlatform_Forest, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest},                    // menu
    {AnimId::LiftPlatform_RuptureFarms, AnimId::LiftBottomWheel_RuptureFarms, AnimId::LiftTopWheel_RuptureFarms},  // ruputure farms
    {AnimId::LiftPlatform_Lines, AnimId::LiftBottomWheel_Lines, AnimId::LiftTopWheel_Lines},                       // lines
    {AnimId::LiftPlatform_Forest, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest},                    // forest
    {AnimId::LiftPlatform_Forest, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest},                    // forest temple
    {AnimId::LiftPlatform_Forest, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest},                    // stock yards
    {AnimId::LiftPlatform_Forest, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest},                    // stock yards return
    {AnimId::LiftPlatform_Lines, AnimId::LiftBottomWheel_Lines, AnimId::LiftTopWheel_Lines},                       // removed
    {AnimId::LiftPlatform_Desert, AnimId::LiftBottomWheel_Desert, AnimId::LiftTopWheel_Desert},                    // desert
    {AnimId::LiftPlatform_Desert2, AnimId::LiftBottomWheel_Desert2, AnimId::LiftTopWheel_Desert2},                 // desert temple
    {AnimId::LiftPlatform_Forest, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest},                    // credits
    {AnimId::LiftPlatform_Forest, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest},                    // removed
    {AnimId::LiftPlatform_RuptureFarms, AnimId::LiftBottomWheel_RuptureFarms, AnimId::LiftTopWheel_RuptureFarms},  // board room
    {AnimId::LiftPlatform_RuptureFarms, AnimId::LiftBottomWheel_RuptureFarms, AnimId::LiftTopWheel_RuptureFarms},  // rupture farms return
    {AnimId::LiftPlatform_Forest, AnimId::LiftBottomWheel_Forest, AnimId::LiftTopWheel_Forest},                    // forest chase
    {AnimId::LiftPlatform_Desert2, AnimId::LiftBottomWheel_Desert2, AnimId::LiftTopWheel_Desert2}};                // desert escape

static const LiftPointCoord sRopeOffsets[16] = {
    {0, 0}, // menu
    {0, 0}, // ruputure farms
    {0, 0}, // lines
    {0, 0}, // forest
    {0, 0}, // forest temple
    {0, 0}, // stock yards
    {0, 0}, // stock yards return
    {0, 0}, // removed
    {2, 1}, // desert
    {2, 1}, // desert temple
    {0, 0}, // credits
    {0, 0}, // removed
    {0, 0}, // board room
    {0, 0}, // rupture farms return
    {0, 0}, // forest chase
    {2, 1}};// desert escape

void LiftPoint::LoadAnimations()
{
    for (u32 i = 0; i < ALIVE_COUNTOF(sLiftPointAnimIds); i++)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sLiftPointAnimIds[i].mPlatformAnimId));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sLiftPointAnimIds[i].mLiftTopWheelAnimId));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sLiftPointAnimIds[i].mLiftBottomWheelAnimId));
    }
}

LiftPoint::LiftPoint(relive::Path_LiftPoint* pTlv, const Guid& tlvId)
    : PlatformBase()
{
    mBaseGameObjectTlvInfo = tlvId;
    SetType(ReliveTypes::eLiftPoint);

    LoadAnimations();

    pTlv->mTlvSpecificMeaning = 3;

    const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
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

    AddDynamicCollision(
        sLiftPointAnimIds[lvl_idx].mPlatformAnimId,
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

    const FP oldX = mXPos;
    MapFollowMe(true);
    const FP newX = mXPos;

    GetAnimation().SetSemiTrans(true);

    const auto xMovedBy = FP_GetExponent(newX - oldX);
    mPlatformBaseXOffset -= xMovedBy;
    mPlatformBaseWidthOffset -= xMovedBy;

    if (mLiftWheelAnim.Init(
            GetAnimRes(sLiftPointAnimIds[lvl_idx].mLiftBottomWheelAnimId),
            this))
    {
        if (pTlv->mScale == relive::reliveScale::eHalf)
        {
            mLiftWheelAnim.SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        }
        else
        {
            mLiftWheelAnim.SetRenderLayer(Layer::eLayer_BeforeShadow_25);
        }

        mLiftWheelAnim.SetSpriteScale(GetSpriteScale());

        mMoveToFloorLevel = false;
        mBottomFloor = false;
        mMiddleFloor = false;
        mTopFloor = false;

        mLiftWheelAnim.SetSemiTrans(false);
        mLiftWheelAnim.SetBlending(false);
        mLiftWheelAnim.SetAnimate(false);

        mMoving = false;
        mVelX = FP_FromInteger(0);
        mLiftWheelAnim.SetRGB(128, 128, 128);
        mLiftWheelAnim.SetBlendMode(relive::TBlendModes::eBlend_0);

        mVelY = FP_FromInteger(0);
        mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;

        auto pRopeMem = relive_new Rope(
            FP_GetExponent(mXPos + (FP_FromInteger(13) * GetSpriteScale()) + FP_FromInteger(sRopeOffsets[lvl_idx].field_4)),
            0,
            FP_GetExponent(mYPos + (FP_FromInteger(25) * GetSpriteScale())),
            GetSpriteScale());
        if (pRopeMem)
        {
            pRopeMem->mBaseGameObjectRefCount++;
            mRope1 = pRopeMem;
        }

        auto pRopeMem2 = relive_new Rope(
            FP_GetExponent(mXPos + (FP_FromInteger(-10) * GetSpriteScale()) + FP_FromInteger(sRopeOffsets[lvl_idx].field_0)),
            0,
            FP_GetExponent(mYPos + (FP_FromInteger(25) * GetSpriteScale())),
            GetSpriteScale());;
        if (pRopeMem2)
        {
            pRopeMem2->mBaseGameObjectRefCount++;
            mRope2 = pRopeMem2;
        }

        mRope2->mBottom = FP_GetExponent((FP_FromInteger(25) * GetSpriteScale()) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y));
        mRope1->mBottom = FP_GetExponent((FP_FromInteger(25) * GetSpriteScale()) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y));

        const FP v29 = FP_FromRaw(FP_GetExponent((mYPos * FP_FromDouble(1.5)) * GetSpriteScale()) % FP_FromInteger(mRope2->mRopeLength).fpValue);
        mRope2->mYPos = FP_NoFractional(mYPos + v29 + (FP_FromInteger(25) * GetSpriteScale()) + FP_FromInteger(mRope2->mRopeLength));
        mRope1->mYPos = FP_NoFractional(mYPos + v29 - (FP_FromInteger(25) * GetSpriteScale()) + FP_FromInteger(mRope1->mRopeLength));

        mHasPulley = false;

        CreatePulleyIfExists(0, 0);

        mLiftPointId = pTlv->mLiftPointId;
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
        }

        mKeepOnMiddleFloor = false;
        mIgnoreLiftMover = true;
    }
    else
    {
        SetListAddFailed(true);
    }
}

void LiftPoint::Move(FP xSpeed, FP ySpeed)
{
    mVelX = xSpeed;
    mVelY = ySpeed;
    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        mMoving = true;
    }
}


void LiftPoint::StayOnFloor(s16 floor, relive::Path_LiftPoint* pLiftTlv)
{
    if (!floor)
    {
        mYPos = FP_FromInteger(pLiftTlv->mTopLeftY - mPlatformBaseYOffset);
        SfxPlayMono(relive::SoundEffects::LiftStop, 0);
        SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -2000);
    }

    mMoving = false;
    pLiftTlv->mTlvSpecificMeaning = 3;
    pLiftTlv->mLiftPointId = mLiftPointId;
    mVelY = FP_FromInteger(0);

    EventBroadcast(kEventNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);
}


void LiftPoint::ClearTlvFlags(relive::Path_TLV* pTlv)
{
    pTlv->mTlvFlags.Clear(relive::eBit1_Created);
    pTlv->mTlvFlags.Clear(relive::eBit2_Destroyed);
    pTlv->mTlvSpecificMeaning |= 1;
}

bool LiftPoint::OnTopFloor() const
{
    return mTopFloor && !mMoveToFloorLevel;
}

bool LiftPoint::OnBottomFloor() const
{
    return mBottomFloor && !mMoveToFloorLevel;
}

bool LiftPoint::OnMiddleFloor() const
{
    return mMiddleFloor && !mMoveToFloorLevel;
}

bool LiftPoint::OnAnyFloor() const
{
    return OnBottomFloor() || OnTopFloor() || OnMiddleFloor();
}


bool LiftPoint::OnAFloorLiftMoverCanUse() const
{
    return (OnMiddleFloor() && !mIgnoreLiftMover) || OnBottomFloor() || OnTopFloor();
}

void LiftPoint::VUpdate()
{
    if (mMoving)
    {
        if (mMoveToFloorLevel)
        {
            const bool bOnFloor = mFloorLevelY == mYPos;
            const FP distToFloor = mFloorLevelY - mYPos;
            FP absDistToFloor = distToFloor;
            if (distToFloor < FP_FromInteger(0) || bOnFloor)
            {
                absDistToFloor = -distToFloor;
            }
            else
            {
                absDistToFloor = distToFloor;
            }

            if (absDistToFloor >= (FP_FromInteger(2) * GetSpriteScale()))
            {
                if (distToFloor >= FP_FromInteger(0))
                {
                    mVelY = (FP_FromInteger(2) * GetSpriteScale());
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

            relive::Path_LiftPoint* pLiftTlv = nullptr;
            relive::Path_TLV* pTlvIter = gMap.TLV_Get_At(
                nullptr,
                mXPos,
                lineY,
                mXPos,
                (GetSpriteScale() * FP_FromInteger(30)) + lineY);
            while (pTlvIter)
            {
                if (pTlvIter->mTlvType == ReliveTypes::eLiftPoint)
                {
                    pLiftTlv = static_cast<relive::Path_LiftPoint*>(pTlvIter);
                    mLiftPointStopType = pLiftTlv->mLiftPointStopType;
                    break;
                }
                pTlvIter = gMap.TLV_Get_At(
                    pTlvIter,
                    mXPos,
                    lineY,
                    mXPos,
                    lineY + (GetSpriteScale() * FP_FromInteger(30)));
            }

            if (pLiftTlv && pLiftTlv->mIsStartPoint == relive::reliveChoice::eYes)
            {
                mIgnoreLiftMover = true;
            }
            else
            {
                mIgnoreLiftMover = false;
            }

            if (pLiftTlv)
            {
                ClearTlvFlags(pLiftTlv);

                mFloorLevelY = FP_FromInteger(pTlvIter->mTopLeftY + -mPlatformBaseYOffset);
            }
            else
            {
                mFloorLevelY = FP_FromInteger(0);
                mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly;
            }

            const auto distanceToFloor = mFloorLevelY - mYPos;
            const FP kMinus25Scaled = (GetSpriteScale() * FP_FromInteger(-25));
            const FP k30Scaled = (GetSpriteScale() * FP_FromInteger(30));

            switch (mLiftPointStopType)
            {
                case relive::Path_LiftPoint::LiftPointStopType::eTopFloor:
                    if (mVelY >= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || (distanceToFloor <= kMinus25Scaled) || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;
                            mTopFloor = false;
                        }
                        else
                        {
                            mMoveToFloorLevel = true;
                            mMoving = true;
                            mYPos = mFloorLevelY - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            pLiftTlv->mLiftPointId = mLiftPointId;
                            mTopFloor = true;
                        }
                    }
                    else if (mVelY + lineY <= FP_FromInteger(pTlvIter->mTopLeftY))
                    {
                        StayOnFloor(mTopFloor, pLiftTlv);

                        mTopFloor = true;
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eBottomFloor:
                    if (mVelY <= FP_FromInteger(0))
                    {
                        if (mVelY != FP_FromInteger(0) || (distanceToFloor <= kMinus25Scaled) || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->mTlvSpecificMeaning = 1;
                            mBottomFloor = false;
                        }
                        else
                        {
                            mMoving = true;
                            mMoveToFloorLevel = true;
                            mYPos = mFloorLevelY - distanceToFloor;

                            pLiftTlv->mTlvSpecificMeaning = 3;

                            pLiftTlv->mLiftPointId = mLiftPointId;
                            mBottomFloor = true;
                        }
                    }
                    else if (mVelY + lineY >= FP_FromInteger(pTlvIter->mTopLeftY))
                    {
                        StayOnFloor(mBottomFloor, pLiftTlv);
                        mBottomFloor = true;
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eMiddleFloor:
                    if (distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                    {
                        pLiftTlv->mTlvSpecificMeaning = 1;
                    }
                    else
                    {
                        if (mKeepOnMiddleFloor)
                        {
                            StayOnFloor(mMiddleFloor, pLiftTlv);
                            mKeepOnMiddleFloor = false;
                        }

                        if (mVelY == FP_FromInteger(0))
                        {
                            mMoving = true;
                            mMoveToFloorLevel = true;
                            mYPos = mFloorLevelY - distanceToFloor;
                        }

                        pLiftTlv->mTlvSpecificMeaning = 3;
                        pLiftTlv->mLiftPointId = mLiftPointId;
                        mMiddleFloor = true;
                    }
                    break;

                case relive::Path_LiftPoint::LiftPointStopType::eStartPointOnly:
                    if (pLiftTlv)
                    {
                        pLiftTlv->mTlvSpecificMeaning = 1;
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

    const FP FP_25xScale = FP_FromInteger(25) * GetSpriteScale();
    const FP FP_m19xScale = FP_FromInteger(-19) * GetSpriteScale();

    Rope* pRope2 = mRope2;
    Rope* pRope1 = mRope1;

    const FP rope2_rope_length = FP_FromInteger(pRope2->mRopeLength);
    const FP rope1_rope_length = FP_FromInteger(pRope1->mRopeLength);

    const FP pColliRectY = FP_FromInteger(mPlatformBaseCollisionLine->mRect.y);

    pRope2->mBottom = FP_GetExponent(pColliRectY + FP_25xScale);
    pRope1->mBottom = FP_GetExponent(pColliRectY + FP_25xScale);

    if (mHasPulley)
    {
        const FP pulley_ypos = FP_FromInteger(mPulleyYPos);

        pRope2->mTop = FP_GetExponent(pulley_ypos + FP_m19xScale);
        pRope1->mTop = FP_GetExponent(pulley_ypos + FP_m19xScale);
    }

    const FP new_ypos = (mYPos * FP_FromDouble(1.5));
    const FP remaining_rope = FP_FromRaw(FP_GetExponent(new_ypos * GetSpriteScale()) % rope2_rope_length.fpValue);

    pRope2->mYPos = FP_NoFractional((mYPos + remaining_rope) + (FP_25xScale + rope2_rope_length));
    pRope1->mYPos = FP_NoFractional((mYPos + FP_25xScale) + (rope1_rope_length - remaining_rope));


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
    else if (mVelY < FP_FromInteger(0))
    {
        mLiftWheelAnim.SetLoopBackwards(true);
        mPulleyAnim.SetLoopBackwards(false);
    }

    if (mLiftWheelAnim.GetCurrentFrame() == 1)
    {
        if (mLiftWheelAnim.GetAnimate())
        {
            SfxPlayMono(relive::SoundEffects::WheelSqueak, 0);
        }
    }

    if (mCurrentLevel != gMap.mCurrentLevel || mCurrentPath != gMap.mCurrentPath || EventGet(kEventDeathReset))
    {
        if (mPlatformBaseCount <= 0)
        {
            SetDead(true);
        }
    }
}

void LiftPoint::VRender(OrderingTable& ot)
{
    PSX_Point mapCoord = {};
    gMap.GetCurrentCamCoords(&mapCoord);
    if (mCurrentLevel == gMap.mCurrentLevel)
    {
        if (mCurrentPath == gMap.mCurrentPath)
        {
            if (mXPos >= FP_FromInteger(mapCoord.x) && mXPos <= FP_FromInteger(mapCoord.x + 1024))
            {
                BaseAnimatedWithPhysicsGameObject::VRender(ot);

                s16 liftWheelR = mRGB.r;
                s16 liftWheelG = mRGB.g;
                s16 liftWheelB = mRGB.b;

                const PSX_RECT boundingRect = VGetBoundingRect();

                ShadowZone::ShadowZones_Calculate_Colour(
                    FP_GetExponent(mXPos),
                    (boundingRect.y + boundingRect.h) / 2,
                    GetScale(),
                    &liftWheelR,
                    &liftWheelG,
                    &liftWheelB);

                mLiftWheelAnim.SetRGB(liftWheelR, liftWheelG, liftWheelB);

                const FP xOff = (FP_FromInteger(3) * GetSpriteScale());
                const FP yOff = (FP_FromInteger(-5) * GetSpriteScale());

                mLiftWheelAnim.VRender(
                    FP_GetExponent(mXPos
                                   + xOff
                                   + FP_FromInteger(gScreenManager->mCamXOff) - gScreenManager->mCamPos->x),
                    FP_GetExponent(mYPos
                                   + yOff
                                   + FP_FromInteger(gScreenManager->mCamYOff) - gScreenManager->mCamPos->y),
                    ot,
                    0,
                    0);

                PSX_RECT liftWheelRect = {};
                mLiftWheelAnim.Get_Frame_Rect(&liftWheelRect);

                if (mHasPulley)
                {
                    const FP pulley_xpos = FP_FromInteger(mPulleyXPos);
                    const FP pulley_ypos = FP_FromInteger(mPulleyYPos);
                    if (gMap.Is_Point_In_Current_Camera(
                            mCurrentLevel,
                            mCurrentPath,
                            pulley_xpos,
                            pulley_ypos,
                            0))
                    {
                        s16 r = mRGB.r;
                        s16 g = mRGB.g;
                        s16 b = mRGB.b;

                        ShadowZone::ShadowZones_Calculate_Colour(
                            mPulleyXPos,
                            mPulleyYPos,
                            GetScale(),
                            &r,
                            &g,
                            &b);

                        mPulleyAnim.SetRGB(r, g, b);

                        mPulleyAnim.VRender(
                            FP_GetExponent(pulley_xpos + FP_FromInteger(gScreenManager->mCamXOff) - gScreenManager->mCamPos->x),
                            FP_GetExponent(pulley_ypos + FP_FromInteger(gScreenManager->mCamYOff) - gScreenManager->mCamPos->y),
                            ot,
                            0,
                            0);
                        PSX_RECT pulleyRect = {};
                        mPulleyAnim.Get_Frame_Rect(&pulleyRect);
                    }
                }
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
            // Do platforms ever move horizontally? This is always 0 it seems
            pObj->mXPos += xVelocity;

			// Keep ypos on the platform
            pObj->mYPos = FP_FromInteger(mPlatformBaseCollisionLine->mRect.y);
        }
    }
}


void LiftPoint::VScreenChanged()
{
    if (!mHasPulley)
    {
        CreatePulleyIfExists(0, -1);
    }

    if (gMap.LevelChanged())
    {
        SetDead(true);
    }

    if (gMap.PathChanged())
    {
        SetDead(true);
    }
}

void LiftPoint::CreatePulleyIfExists(s16 camX, s16 camY)
{
    auto pTlv = gMap.Get_First_TLV_For_Offsetted_Camera(camX, camY);
    if (pTlv)
    {
        while (1)
        {
            mPulleyYPos = pTlv->mTopLeftY;
            mPulleyXPos = pTlv->mTopLeftX;

            if (pTlv->mTlvType == ReliveTypes::ePulley)
            {
                if (mPlatformBaseCollisionLine->mRect.x <= pTlv->mTopLeftX && pTlv->mTopLeftX <= mPlatformBaseCollisionLine->mRect.w)
                {
                    break;
                }
            }

            pTlv = gMap.TLV_Get_At(pTlv, FP_FromInteger(-1), FP_FromInteger(-1), FP_FromInteger(-1), FP_FromInteger(-1));
            if (!pTlv)
            {
                return;
            }
        }

        const FP k13_scaled = FP_FromInteger(13) * GetSpriteScale();
        const FP kM10_scaled = FP_FromInteger(-10) * GetSpriteScale();

        mPulleyXPos = FP_GetExponent(((k13_scaled + kM10_scaled) / FP_FromInteger(2)) + FP_NoFractional(mXPos));

        const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

        mPulleyAnim.Init(GetAnimRes(sLiftPointAnimIds[lvl_idx].mLiftTopWheelAnimId), this);

        mPulleyAnim.SetSemiTrans(false);
        mPulleyAnim.SetBlending(false);
        mPulleyAnim.SetAnimate(false);

        mHasPulley = true;

        mPulleyAnim.SetRGB(128, 128, 128);

        mPulleyAnim.SetRenderLayer(GetAnimation().GetRenderLayer());
        mPulleyAnim.SetSpriteScale(GetSpriteScale());
        mPulleyAnim.SetBlendMode(relive::TBlendModes::eBlend_0);

        mRope2->mTop = FP_GetExponent(FP_FromInteger(mPulleyYPos) + (FP_FromInteger(-19) * mPulleyAnim.GetSpriteScale()));
        mRope1->mTop = FP_GetExponent(FP_FromInteger(mPulleyYPos) + (FP_FromInteger(-19) * mPulleyAnim.GetSpriteScale()));
    }
}


LiftPoint::~LiftPoint()
{
    if (GetListAddFailed())
    {
        return;
    }

    if (mRope2)
    {
        mRope2->mBaseGameObjectRefCount--;
        mRope2->SetDead(true);
    }

    if (mRope1)
    {
        mRope1->mBaseGameObjectRefCount--;
        mRope1->SetDead(true);
    }

    mRope2 = nullptr;
    mRope1 = nullptr;

    Path::TLV_Reset(mPlatformBaseTlvInfo, -1, 0, 0);

    auto pLiftPointTlv = gMap.VTLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(FP_FromInteger(mPlatformBaseCollisionLine->mRect.y)),
        FP_GetExponent(mXPos),
        FP_GetExponent((GetSpriteScale() * FP_FromInteger(30)) + FP_FromInteger(mPlatformBaseCollisionLine->mRect.y)),
        ReliveTypes::eLiftPoint);

    if (pLiftPointTlv)
    {
        pLiftPointTlv->mTlvFlags.Clear(relive::eBit1_Created);
        pLiftPointTlv->mTlvFlags.Clear(relive::eBit2_Destroyed);
    }

    mLiftWheelAnim.VCleanUp();

    if (mHasPulley)
    {
        mPulleyAnim.VCleanUp();
    }
}

} // namespace AO
