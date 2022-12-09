#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "PlatformBase.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "BirdPortal.hpp"
#include "Grid.hpp"
#include "../AliveLibCommon/FatalError.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"
#include "../AliveLibAE/FixedPoint.hpp"

namespace AO {

BaseAliveGameObject::BaseAliveGameObject()
    : IBaseAliveGameObject(0)
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

    BaseAliveGameObjectPathTLV = nullptr;
    BaseAliveGameObjectCollisionLine = nullptr;
    mHealth = FP_FromInteger(1);
    BaseAliveGameObject_PlatformId = Guid{};
    mbGotShot = false;
    mbMotionChanged = false;
    field_EC_bBeesCanChase = 0;
    mCurrentMotion = 0;
    mNextMotion = 0;
    mPreviousMotion = 0;
    mBaseAliveGameObjectLastAnimFrame = 0;
    BaseAliveGameObjectLastLineYPos = FP_FromInteger(0);

    gBaseAliveGameObjects->Push_Back(this);

    SetIsBaseAliveGameObject(true);
}

BaseAliveGameObject::~BaseAliveGameObject()
{

}

void BaseAliveGameObject::VSetXSpawn(s16 camWorldX, s32 screenXPos)
{
    BaseAliveGameObject* pLiftPoint = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    const FP old_x = mXPos;
    const FP old_y = mYPos;

    mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(GetSpriteScale(), screenXPos));

    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(0, mXPos, old_y, mXPos, old_y);

    if (pLiftPoint)
    {
        pLiftPoint->mXPos += (mXPos - old_x);

        BaseAliveGameObjectCollisionLine->mRect.x += FP_GetExponent(mXPos - old_x);
        BaseAliveGameObjectCollisionLine->mRect.w += FP_GetExponent(mXPos - old_x);
        BaseAliveGameObjectCollisionLine->mRect.y = BaseAliveGameObjectCollisionLine->mRect.y;
        BaseAliveGameObjectCollisionLine->mRect.h = BaseAliveGameObjectCollisionLine->mRect.h;
    }
    else
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (BaseAliveGameObjectCollisionLine)
        {
            if (sCollisions->Raycast(
                    mXPos,
                    old_y - FP_FromInteger(40),
                    mXPos,
                    old_y + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    CollisionMask(BaseAliveGameObjectCollisionLine->mLineType)))
            {
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = hitY;
            }
            else
            {
                BaseAliveGameObjectPathTLV = gMap.TLV_First_Of_Type_In_Camera(ReliveTypes::eStartController, 0);
                if (BaseAliveGameObjectPathTLV
                    && sCollisions->Raycast(
                        mXPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY),
                        mXPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY),
                        &pLine,
                        &hitX,
                        &hitY,
                        CollisionMask(BaseAliveGameObjectCollisionLine->mLineType)))
                {
                    BaseAliveGameObjectCollisionLine = pLine;
                    mYPos = hitY;
                }
                else
                {
                    BaseAliveGameObjectCollisionLine = nullptr;
                }
            }
        }
        else
        {
            if (sCollisions->Raycast(
                    mXPos,
                    BaseAliveGameObjectLastLineYPos - FP_FromInteger(40),
                    mXPos,
                    BaseAliveGameObjectLastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos += hitY - BaseAliveGameObjectLastLineYPos;
            }
        }
    }
}

void BaseAliveGameObject::VSetYSpawn(s32 camWorldY, s16 bLeft)
{
    BaseAliveGameObject* pLiftPoint = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    const FP oldx = mXPos;
    const FP oldy = mYPos;

    const PerFrameInfo* pFrameInfo = GetAnimation().Get_FrameHeader(-1);

    if (bLeft == 1)
    {
        mYPos = FP_FromInteger(pFrameInfo->mHeight + camWorldY + 356);
    }
    else
    {
        mYPos = FP_FromInteger(camWorldY + 124);
    }

    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At(
        nullptr,
        mXPos,
        mYPos,
        mXPos,
        mYPos);

    if (pLiftPoint)
    {
        pLiftPoint->mXPos += mXPos - oldx;
        pLiftPoint->mYPos += mYPos - oldy;

        BaseAliveGameObjectCollisionLine->mRect.x += FP_GetExponent(mXPos - oldx);
        BaseAliveGameObjectCollisionLine->mRect.w += FP_GetExponent(mXPos - oldx);
        BaseAliveGameObjectCollisionLine->mRect.y += FP_GetExponent(mYPos - oldy);
        BaseAliveGameObjectCollisionLine->mRect.h += FP_GetExponent(mYPos - oldy);
    }
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

    const CollisionMask mask = GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor;
    if (sCollisions->Raycast(
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
        if (pLine->mLineType == eLineTypes ::eDynamicCollision_32 || pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            PSX_RECT bRect = VGetBoundingRect();
            bRect.y += 5;
            bRect.h += 5;

            OnCollisionWith(
                {bRect.x, bRect.y},
                {bRect.w, bRect.h},
                gPlatformsArray);
        }
    }
    else
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_First_Of_Type_In_Camera(ReliveTypes::eStartController, 0);
        if (BaseAliveGameObjectPathTLV)
        {
            if (sCollisions->Raycast(
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY),
                    mXPos,
                    FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRightY),
                    &pLine,
                    &hitX,
                    &hitY,
                    mask))
            {
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = hitY;
            }
        }
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

bool BaseAliveGameObject::Check_IsOnEndOfLine(s16 direction, s16 distance)
{
    // Check if distance grid blocks from current snapped X is still on the line or not, if not then we are
    // about to head off an edge.

    const FP gridSize = ScaleToGridSize(GetSpriteScale());

    FP xLoc = {};
    if (direction == 1)
    {
        xLoc = -(gridSize * FP_FromInteger(distance));
    }
    else
    {
        xLoc = gridSize * FP_FromInteger(distance);
    }

    const s16 xposRounded = FP_GetExponent(mXPos) & 1023;
    const FP xPosSnapped = FP_FromInteger((FP_GetExponent(mXPos) & 0xFC00) + SnapToXGrid(GetSpriteScale(), xposRounded));
    if (xposRounded < (240 + 16) || xposRounded > (640 - 16))
    {
        return 0;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions->Raycast(
               xLoc + xPosSnapped,
               mYPos - FP_FromInteger(4),
               xLoc + xPosSnapped,
               mYPos + FP_FromInteger(4),
               &pLine,
               &hitX,
               &hitY,
               GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor)
        == 0;
}

void BaseAliveGameObject::VOnPathTransition(s32 camWorldX, s32 camWorldY, CameraPos direction)
{
    const FP oldx = mXPos;
    const FP oldy = mYPos;

    // TODO: This was probably a rect ??
    s32 xpos = 0;
    s32 ypos = 0;
    s32 height = 0;
    s32 width = 0;

    switch (direction)
    {
        case CameraPos::eCamTop_1:
        {
            xpos = camWorldX + 206;
            width = camWorldX + 674;
            ypos = camWorldY + 70;
            height = camWorldY + 170;

            // Get the fame header for the first frame in the animation and take its height
            const PerFrameInfo* pFrameInfo = GetAnimation().Get_FrameHeader(-1);
            const s32 frameH = pFrameInfo->mHeight;

            mXPos = FP_FromInteger(camWorldX + (FP_GetExponent(oldx) % 1024));
            mYPos = FP_FromInteger(frameH + camWorldY + 356);
            break;
        }

        case CameraPos::eCamBottom_2:
            xpos = camWorldX + 206;
            width = camWorldX + 674;
            ypos = camWorldY + 310;
            height = camWorldY + 410;
            mXPos = FP_FromInteger(camWorldX + (FP_GetExponent(oldx) % 1024));
            mYPos = FP_FromInteger(camWorldY + 124);
            break;

        case CameraPos::eCamLeft_3:
            width = camWorldX + 674;
            xpos = camWorldX + 524;
            ypos = camWorldY + 70;
            height = camWorldY + 410;
            mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(GetSpriteScale(), MaxGridBlocks(GetSpriteScale()) - 1));
            mYPos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
            break;

        case CameraPos::eCamRight_4:
            width = camWorldX + 356;
            xpos = camWorldX + 206;
            ypos = camWorldY + 70;
            height = camWorldY + 410;
            mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(GetSpriteScale(), 1));
            mYPos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
            break;

        default:
            break;
    }

    // Find the start controller at the position we will be at in the new map
    BaseAliveGameObjectPathTLV = gMap.VTLV_Get_At(static_cast<s16>(xpos), static_cast<s16>(ypos), static_cast<s16>(width), static_cast<s16>(height), ReliveTypes::eStartController);

    if (!BaseAliveGameObjectPathTLV)
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_First_Of_Type_In_Camera(ReliveTypes::eStartController, 0);
        LOG_INFO("Flip direction after the path trans as we are not touching the start controller");
        mVelX = -mVelX;
        GetAnimation().ToggleFlipX();
    }
    else
    {
        LOG_INFO("Not changing direction in the new path trans as we are touching the start controller");
    }

    if (!BaseAliveGameObjectPathTLV)
    {
        LOG_ERROR("Did a path transition to a camera with no start controller");
        ALIVE_FATAL("Did a path transition to a camera with no start controller (add one if this is an edited level)");
    }

    PSX_Point camLoc = {};
    gMap.GetCurrentCamCoords(&camLoc);

    mXPos = FP_FromInteger((BaseAliveGameObjectPathTLV->mBottomRightX + BaseAliveGameObjectPathTLV->mTopLeftX) / 2);
    mYPos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY);

    mXPos = FP_FromInteger(camLoc.x + SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos - FP_FromInteger(camLoc.x))));

    // TODO: Think a path changes deletes the platforms...
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pPlatform)
    {
        // Move lift point into the new path
        const FP rect_left = mXPos - oldx;
        const FP rect_right = mYPos - oldy;

        pPlatform->mXPos += rect_left;
        pPlatform->mYPos += rect_right;

        BaseAliveGameObjectCollisionLine->mRect.x += FP_GetExponent(rect_left);
        BaseAliveGameObjectCollisionLine->mRect.w += FP_GetExponent(rect_left);
        BaseAliveGameObjectCollisionLine->mRect.y += FP_GetExponent(rect_right);
        BaseAliveGameObjectCollisionLine->mRect.h += FP_GetExponent(rect_right);
    }
    else
    {
        // OG FIX: After a path trans the collision items are free so we can't possibly have a line
        // so try to find a new one under our feet.
        if (!BaseAliveGameObjectCollisionLine)
        {
            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mXPos,
                    mYPos - FP_FromInteger(40),
                    mXPos,
                    mYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                BaseAliveGameObjectCollisionLine = pLine;
                mYPos = hitY;
            }
            else
            {
                BaseAliveGameObjectCollisionLine = nullptr;
            }
        }

        // If we still didn't get a line then look for a start controller
        if (!BaseAliveGameObjectCollisionLine)
        {
            if (BaseAliveGameObjectPathTLV->mTlvType == ReliveTypes::eStartController)
            {
                BaseAliveGameObjectLastLineYPos += mYPos - oldy;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mXPos,
                    BaseAliveGameObjectLastLineYPos - FP_FromInteger(40),
                    mXPos,
                    BaseAliveGameObjectLastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos += hitY - BaseAliveGameObjectLastLineYPos;
            }
        }
    }

    if (GetSpriteScale() == FP_FromInteger(1) && GetAnimation().GetSpriteScale() == FP_FromDouble(0.5))
    {
        mVelX = (mVelX * FP_FromInteger(2));
        return;
    }

    if (GetSpriteScale() == FP_FromDouble(0.5) && GetAnimation().GetSpriteScale() == FP_FromInteger(1))
    {
        mVelX = (mVelX * FP_FromDouble(0.5));
        return;
    }
}


s16 BaseAliveGameObject::MapFollowMe(s16 snapToGrid)
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);

    // Are we "in" the current camera X bounds?
    if (mCurrentLevel == gMap.mCurrentLevel && mCurrentPath == gMap.mCurrentPath && mXPos > FP_FromInteger(camCoords.x) && mXPos < FP_FromInteger(camCoords.x + 1024))
    {
        // Snapped XPos in camera space
        const s32 snappedXLocalCoords = SnapToXGrid(GetSpriteScale(), FP_GetExponent(mXPos - FP_FromInteger(camCoords.x)));

        // In the left camera void and moving left?
        if (snappedXLocalCoords < 256 && mVelX < FP_FromInteger(0))
        {
            if (sControlledCharacter == this && gMap.SetActiveCameraDelayed(MapDirections::eMapLeft_0, this, -1))
            {
                mCurrentPath = gMap.mCurrentPath;
                mCurrentLevel = gMap.mCurrentLevel;
                return 1;
            }
            else
            {
                const s32 x_i = abs(FP_GetExponent(mXPos));
                const s32 camXIndex = x_i % 1024;
                if (x_i > 1024)
                {
                    UsePathTransScale_4020D0();

                    // Put at the right side of the camera to the left
                    const s32 cam1GridBeforeRight = XGrid_Index_To_XPos(GetSpriteScale(), (MaxGridBlocks(GetSpriteScale()) - 1));
                    const s32 camRightEdge = x_i - camXIndex - 1024;
                    mXPos = FP_FromInteger(camRightEdge) + FP_FromInteger(cam1GridBeforeRight) + ScaleToGridSize(GetSpriteScale());

                    VCheckCollisionLineStillValid(40);
                }
            }
        }
        // In the right camera void and moving right?
        else if (snappedXLocalCoords > 624 && mVelX > FP_FromInteger(0))
        {
            // Go to the right camera in under player control
            if (sControlledCharacter == this && gMap.SetActiveCameraDelayed(MapDirections::eMapRight_1, this, -1))
            {
                mCurrentPath = gMap.mCurrentPath;
                mCurrentLevel = gMap.mCurrentLevel;
                return 1;
            }
            else
            {
                const s32 x_i = abs(FP_GetExponent(mXPos));
                const s32 camXIndex = x_i % 1024;

                gMap.Get_map_size(&camCoords);
                if (x_i < (camCoords.x - 1024))
                {
                    UsePathTransScale_4020D0();

                    // Put at the left side of the camera to the right
                    const s32 cam1GridAfterLeft = XGrid_Index_To_XPos(GetSpriteScale(), 1);
                    const s32 camLeftEdge = x_i - camXIndex + 1024;
                    mXPos = FP_FromInteger(camLeftEdge) + FP_FromInteger(cam1GridAfterLeft) - ScaleToGridSize(GetSpriteScale());

                    VCheckCollisionLineStillValid(40);
                }
            }
        }
        else if (snapToGrid)
        {
            // Not in the voids of the camera, just snap to the x grid
            mXPos = FP_FromInteger(snappedXLocalCoords + camCoords.x);
        }
        return 0;
    }
    else
    {
        const s32 x_i = abs(FP_GetExponent(mXPos));
        const s32 camXIndex = x_i % 1024;

        // In the left camera void and moving left?
        if (camXIndex < 256 && mVelX < FP_FromInteger(0))
        {
            if (x_i > 1024)
            {
                UsePathTransScale_4020D0();

                const s32 camRightGrid = XGrid_Index_To_XPos(GetSpriteScale(), MaxGridBlocks(GetSpriteScale()));
                const s32 camRightEdge = x_i - camXIndex - 1024;
                mXPos = FP_FromInteger(camRightEdge) + FP_FromInteger(camRightGrid) + ScaleToGridSize(GetSpriteScale());

                VCheckCollisionLineStillValid(40);
            }
        }
        // In the right camera void and moving right?
        else if (camXIndex > 624 && mVelX > FP_FromInteger(0)) // Never hit as velx is < 0
        {
            gMap.Get_map_size(&camCoords);
            if (x_i < (camCoords.x - 1024))
            {
                UsePathTransScale_4020D0();

                const s32 camLeftGrid = XGrid_Index_To_XPos(GetSpriteScale(), 0);
                const s32 camLeftEdge = x_i - camXIndex + 1024;
                mXPos = FP_FromInteger(camLeftEdge) + FP_FromInteger(camLeftGrid) - ScaleToGridSize(GetSpriteScale());

                VCheckCollisionLineStillValid(40);
            }
        }
        return 0;
    }
}

s16 BaseAliveGameObject::WallHit(FP offY, FP offX)
{
    PathLine* pLine = nullptr;
    return sCollisions->Raycast(
               mXPos,
               mYPos - offY,
               mXPos + offX,
               mYPos - offY,
               &pLine,
               &offX,
               &offY,
               GetSpriteScale() != FP_FromDouble(0.5) ? kFgWalls : kBgWalls)
        != 0;
}

bool BaseAliveGameObject::InAirCollision(PathLine** ppLine, FP* hitX, FP* hitY, FP vely)
{
    mVelY += GetSpriteScale() * vely;

    if (mVelY > (GetSpriteScale() * FP_FromInteger(20)))
    {
        mVelY = GetSpriteScale() * FP_FromInteger(20);
    }

    const FP old_xpos = mXPos;
    const FP old_ypos = mYPos;

    mXPos += mVelX;
    mYPos += mVelY;

    return sCollisions->Raycast(
        old_xpos,
        old_ypos,
        mXPos,
        mYPos,
        ppLine,
        hitX,
        hitY,
        GetSpriteScale() != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor) ? 1 : 0;
}

void BaseAliveGameObject::OnResourceLoaded_4019A0(BaseAliveGameObject* /*ppRes*/)
{
    //ppRes->field_104_pending_resource_count--;
}

void BaseAliveGameObject::UsePathTransScale_4020D0()
{
    auto pPathTrans = static_cast<relive::Path_PathTransition*>(gMap.VTLV_Get_At(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::ePathTransition));

    if (pPathTrans)
    {
        if (pPathTrans->mNextPathScale == relive::reliveScale::eHalf)
        {
            if (GetSpriteScale() != FP_FromDouble(0.5))
            {
                SetSpriteScale(FP_FromDouble(0.5));
                SetScale(Scale::Bg);
                mVelX = (mVelX * FP_FromDouble(0.5));
            }
        }
        else if (pPathTrans->mNextPathScale == relive::reliveScale::eFull)
        {
            if (GetSpriteScale() != FP_FromInteger(1))
            {
                SetSpriteScale(FP_FromInteger(1));
                SetScale(Scale::Fg);
                mVelX = (mVelX * FP_FromInteger(2));
            }
        }
    }
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

        if (pObj->Type() == typeToFind)
        {
            auto pObj2 = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

            const PSX_RECT bRect = pObj2->VGetBoundingRect();
            if (xpos_int >= bRect.x && xpos_int <= bRect.w && ypos_int >= bRect.y && ypos_int <= bRect.h)
            {
                return pObj;
            }
        }
    }
    return nullptr;
}

s16 BaseAliveGameObject::VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform)
{
    const PSX_RECT rect = pPlatform->VGetBoundingRect();
    if (FP_GetExponent(mXPos) < rect.x || FP_GetExponent(mXPos) > rect.w || FP_GetExponent(mYPos) > rect.h)
    {
        return 1;
    }

    // OG bug fix, when we call VCheckCollisionLineStillValid it can place us on a new lift
    // but then we call OnCollisionWith which can sometimes add us to the same lift again
    // result in the lift being leaked and then memory corruption/crash later.
    BaseAliveGameObject* pCurrentPlatform = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (pCurrentPlatform != pPlatform)
    {
        static_cast<PlatformBase*>(pPlatform)->VAdd(this);
        BaseAliveGameObject_PlatformId = pPlatform->mBaseGameObjectId;
    }
    else
    {
        LOG_WARNING("Trying to add to a platform we are already on");
    }

    return 1;
}

} // namespace AO
