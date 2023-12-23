#include "stdafx.h"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Map.hpp"
#include "../relive_lib/Shadow.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "../relive_lib/Collisions.hpp"
#include "PlatformBase.hpp"
#include "Game.hpp"
#include "BirdPortal.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Grid.hpp"
#include "Path.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/GameType.hpp"

BaseAliveGameObject::BaseAliveGameObject(s16 resourceArraySize)
    : IBaseAliveGameObject(resourceArraySize)
{
    SetCanBePossessed(false);
    SetPossessed(false);
    SetZappedByShrykull(false);
    SetCanSetOffExplosives(false);
    SetElectrocuted(false);
    SetInvisible(false);
    SetRestoredFromQuickSave(false);
    SetTeleporting(false);
    SetElectrocuting(false);

    gBaseAliveGameObjects->Push_Back(this);

    SetIsBaseAliveGameObject(true);
}

BaseAliveGameObject::~BaseAliveGameObject()
{

}

void BaseAliveGameObject::VCheckCollisionLineStillValid(s32 distance)
{
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (!BaseAliveGameObjectCollisionLine)
    {
        return;
    }

    if (pPlatform)
    {
        BaseAliveGameObject_PlatformId = Guid{};
        pPlatform->VRemove(this);
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};

    const CollisionMask mask = PerGameScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls;
    if (gCollisions->Raycast(
            mXPos,
            mYPos - FP_FromInteger(distance),
            mXPos,
            mYPos + FP_FromInteger(distance),
            &pLine,
            &hitX,
            &hitY,
            mask))
    {
        BaseAliveGameObjectCollisionLine = pLine;
        mYPos = hitY;

        if (pLine->mLineType == eLineTypes::eDynamicCollision_32 || pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            const PSX_RECT bRect = VGetBoundingRect();

            PSX_Point xy = {bRect.x, bRect.y};
            PSX_Point wh = {bRect.w, bRect.h};
            xy.y += 5;
            wh.y += 5;

            OnCollisionWith(xy, wh, gPlatformsArray);
        }
    }
    else
    {
        BaseAliveGameObjectCollisionLine = nullptr;
    }
}

BirdPortal* BaseAliveGameObject::VIntoBirdPortal(s16 numGridBlocks)
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->Type() == ReliveTypes::eBirdPortal)
        {
            auto pPortal = static_cast<BirdPortal*>(pObjIter);
            if (pPortal->mXPos >= mXPos)
            {
                if (pPortal->mEnterSide == relive::Path_BirdPortal::PortalSide::eLeft)
                {
                    if (pPortal->mXPos - mXPos <= (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(numGridBlocks)))
                    {
                        if (!GetAnimation().GetFlipX())
                        {
                            if (FP_Abs(mYPos - pPortal->mHitY) < GetSpriteScale() * FP_FromInteger(10))
                            {
                                if (pPortal->VPortalClipper(1))
                                {
                                    GetAnimation().SetRenderLayer(GetSpriteScale() != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30);
                                    return pPortal;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (pPortal->mEnterSide == relive::Path_BirdPortal::PortalSide::eRight)
                {
                    if (mXPos - pPortal->mXPos <= (ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(numGridBlocks)))
                    {
                        if (GetAnimation().GetFlipX())
                        {
                            if (FP_Abs(mYPos - pPortal->mHitY) < (GetSpriteScale() * FP_FromInteger(10)))
                            {
                                if (pPortal->VPortalClipper(1))
                                {
                                    GetAnimation().SetRenderLayer(GetSpriteScale() != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30);
                                    return pPortal;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

void BaseAliveGameObject::VOnPathTransition(s32 camWorldX, s32 camWorldY, CameraPos direction)
{
    const FP oldY = mYPos;
    switch (direction)
    {
        case CameraPos::eCamTop_1:
        {
            mXPos = FP_FromInteger(camWorldX + FP_GetExponent(mXPos) % 375);

            // TODO: This is actually wrong!!
            const u32 off = GetAnimation().Get_FrameHeader(-1)->mHeight;
            mYPos = FP_FromInteger(off + camWorldY + 236);
            break;
        }

        case CameraPos::eCamBottom_2:
            mXPos = FP_FromInteger(camWorldX + FP_GetExponent(mXPos) % 375);
            mYPos = FP_FromInteger(camWorldY + 4);
            break;

        case CameraPos::eCamLeft_3:
            mXPos = FP_FromInteger(camWorldX + (XGrid_Index_To_XPos_AE(GetSpriteScale(), MaxGridBlocks(GetSpriteScale()) - 1)));
            mYPos = FP_FromInteger(camWorldY + FP_GetExponent(mYPos) % 260);
            break;

        case CameraPos::eCamRight_4:
            mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos_AE(GetSpriteScale(), 1));
            mYPos = FP_FromInteger(camWorldY + FP_GetExponent(mYPos) % 260);
            break;

        default:
            break;
    }

    mXPos = FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos)));

    if (IsAbe(this) && gMap.mCurrentLevel == EReliveLevelIds::eNecrum && gMap.mCurrentPath == 2 && (mCurrentMotion == eAbeMotions::Motion_23_RollLoop_453A90 || mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0))
    {
        // Yummy OWI hack - hard code Abe's location when path change to Necrum's first path after the Mines :)
        BaseAliveGameObjectCollisionLine = nullptr;
        mVelY = FP_FromInteger(0);
        mVelX = FP_FromInteger(0);
        mXPos = FP_FromInteger(1011);
        mYPos = FP_FromInteger(784);
    }
    else
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (BaseAliveGameObjectCollisionLine)
        {
            if (gCollisions->Raycast(
                    mXPos, 
					mYPos - FP_FromInteger(40),
                    mXPos, 
					mYPos + FP_FromInteger(40),
                    &pLine, 
					&hitX, 
					&hitY, 
					PerGameScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mYPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
            }
            else
            {
                BaseAliveGameObjectCollisionLine = nullptr;
            }
        }
        else
        {
            BaseAliveGameObjectLastLineYPos = mYPos - oldY + BaseAliveGameObjectLastLineYPos;
            if (gCollisions->Raycast(
                    mXPos,
                    BaseAliveGameObjectLastLineYPos - FP_FromInteger(40),
                    mXPos,
                    BaseAliveGameObjectLastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    PerGameScale() == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mYPos = hitY - BaseAliveGameObjectLastLineYPos + mYPos;
            }
            else
            {
                // Not set to nullptr in this case ??
            }
        }
    }

    if (GetSpriteScale() == FP_FromInteger(1) && GetAnimation().GetSpriteScale() == FP_FromDouble(0.5))
    {
        // From 0.5 to 1 scale, f64 velx
        mVelX *= FP_FromInteger(2);
        return;
    }

    if (GetSpriteScale() == FP_FromDouble(0.5) && GetAnimation().GetSpriteScale() == FP_FromInteger(1))
    {
        // From 1 to 0.5 scale, halve velx
        mVelX *= FP_FromDouble(0.5);
        return;
    }
}

bool BaseAliveGameObject::MapFollowMe(bool snapToGrid)
{
    const s32 xposSnapped = SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos));
    if (snapToGrid)
    {
        mXPos = FP_FromInteger(xposSnapped);
    }

    PSX_Point currentCamCoords = {};
    gMap.GetCurrentCamCoords(&currentCamCoords);

    // Gone off the left edge of the current screen
    if (xposSnapped < currentCamCoords.x && (GetAnimation().GetFlipX() || mVelX < FP_FromInteger(0)))
    {
        if (sControlledCharacter == this && gMap.SetActiveCameraDelayed(MapDirections::eMapLeft_0, this, -1))
        {
            mCurrentLevel = gMap.mCurrentLevel;
            mCurrentPath = gMap.mCurrentPath;
            return true;
        }
    }
    // Gone off the right edge of the current screen
    else if (xposSnapped > currentCamCoords.x + 368 && (!(GetAnimation().GetFlipX()) || mVelX > FP_FromInteger(0)))
    {
        if (sControlledCharacter == this && gMap.SetActiveCameraDelayed(MapDirections::eMapRight_1, this, -1))
        {
            mCurrentLevel = gMap.mCurrentLevel;
            mCurrentPath = gMap.mCurrentPath;
            return true;
        }
    }
    return false;
}

bool BaseAliveGameObject::InAirCollision(PathLine** ppLine, FP* hitX, FP* hitY, FP velY)
{
    mVelY += GetSpriteScale() * velY;

    if (mVelY > (GetSpriteScale() * FP_FromInteger(20)))
    {
        mVelY = GetSpriteScale() * FP_FromInteger(20);
    }

    const FP old_xpos = mXPos;
    const FP old_ypos = mYPos;

    mXPos += mVelX;
    mYPos += mVelY;

    auto bCollision = gCollisions->Raycast(
        old_xpos,
        old_ypos,
        mXPos,
        mYPos,
        ppLine,
        hitX,
        hitY,
        PerGameScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls);

    if (bCollision)
    {
        return bCollision;
    }

    FP velYClamped = mVelY;
    if (Type() == ReliveTypes::eMudokon && velYClamped >= FP_FromInteger(0) && velYClamped < FP_FromInteger(4))
    {
        velYClamped = FP_FromInteger(4);
    }

    bCollision = gCollisions->Raycast(
        mXPos,
        mYPos,
        mXPos + mVelX,
        velYClamped + mYPos,
        ppLine,
        hitX,
        hitY,
        PerGameScale() == Scale::Fg ? kFgFloor : kBgFloor);

    if (bCollision)
    {
        if ((*ppLine)->mLineType == eLineTypes::eDynamicCollision_32 || (*ppLine)->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            return bCollision;
        }

        bCollision = false;
        *ppLine = nullptr;
    }

    if (!IsAbe(this))
    {
        return bCollision;
    }

    const FP k10Scaled = GetSpriteScale() * FP_FromInteger(10);
    return gCollisions->Raycast(
        old_xpos,
        old_ypos - k10Scaled,
        mXPos,
        mYPos - k10Scaled,
        ppLine,
        hitX,
        hitY,
        PerGameScale() == Scale::Fg ? kFgWalls : kBgWalls);
}

BaseGameObject* BaseAliveGameObject::FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos)
{
    const s32 xpos_int = FP_GetExponent(xpos);
    const s32 ypos_int = FP_GetExponent(ypos);

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == typeToFind && pObj != this)
        {
            auto pCasted = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);
            if (pCasted->GetScale() == GetScale())
            {
                const PSX_RECT bRect = pCasted->VGetBoundingRect();
                if (xpos_int >= bRect.x && xpos_int <= bRect.w && ypos_int >= bRect.y && ypos_int <= bRect.h)
                {
                    return pObj;
                }
            }
        }
    }
    return nullptr;
}

bool BaseAliveGameObject::VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform)
{
    const PSX_RECT bRect = pPlatform->VGetBoundingRect();
    if (FP_GetExponent(mXPos) < bRect.x || FP_GetExponent(mXPos) > bRect.w || FP_GetExponent(mYPos) > bRect.h)
    {
        return true;
    }

    static_cast<PlatformBase*>(pPlatform)->VAdd(this);

    BaseAliveGameObject_PlatformId = pPlatform->mBaseGameObjectId;
    return true;
}

BaseAliveGameObject* BaseAliveGameObject::GetStackedSlapTarget(const Guid& idToFind, ReliveTypes typeToFind, FP xpos, FP ypos)
{
    const s16 xposD = FP_GetExponent(xpos);
    const s16 yposD = FP_GetExponent(ypos);

    bool bFound = false;
    for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == typeToFind && pObj != this)
        {
            if (pObj->mBaseGameObjectId == idToFind)
            {
                // So that we pick the one AFTER this
                bFound = true;
            }
            else if (bFound)
            {
                BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
                const PSX_RECT bRect = pAliveObj->VGetBoundingRect();
                // TODO: Similar to PSX_Rects_overlap_no_adjustment
                if (xposD >= bRect.x && xposD <= bRect.w && yposD >= bRect.y && yposD <= bRect.h)
                {
                    return pAliveObj;
                }
            }
        }
    }
    return nullptr;
}

bool BaseAliveGameObject::IsInInvisibleZone(IBaseAliveGameObject* pObj)
{
    if (EventGet(kEventAbeOhm))
    {
        return false;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

    relive::Path_TLV* pTlv = gPathInfo->VTLV_Get_At_Of_Type(
        bRect.x,
        bRect.y,
        bRect.w,
        bRect.h,
        ReliveTypes::eInvisibleZone);

    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eInvisibleZone)
        {
            if (bRect.x >= pTlv->mTopLeftX && bRect.x <= pTlv->mBottomRightX && bRect.y >= pTlv->mTopLeftY)
            {
                if (bRect.y <= pTlv->mBottomRightY && bRect.w >= pTlv->mTopLeftX && bRect.w <= pTlv->mBottomRightX && bRect.h >= pTlv->mTopLeftY && bRect.h <= pTlv->mBottomRightY)
                {
                    return true;
                }
            }
        }

        // Check for stacked/overlaping TLV's
        pTlv = gPathInfo->TLV_Get_At(pTlv,
                                   FP_FromInteger(bRect.x),
                                   FP_FromInteger(bRect.y),
                                   FP_FromInteger(bRect.w),
                                   FP_FromInteger(bRect.h));
    }
    return false;
}