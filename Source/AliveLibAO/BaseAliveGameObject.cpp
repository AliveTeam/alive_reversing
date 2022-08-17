#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "PlatformBase.hpp"
#include "../relive_lib/Collisions.hpp"
#include "BeeSwarm.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "BirdPortal.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"

namespace AO {

void BaseAliveGameObject_ForceLink()
{ }

ALIVE_VAR(1, 0x4FC8A0, DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_4FC8A0, nullptr);

BaseAliveGameObject::BaseAliveGameObject()
{
    mBaseAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit1_Can_Be_Possessed);
    mBaseAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit2_bPossesed);
    mBaseAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit3);
    mBaseAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit4_SetOffExplosives);
    mBaseAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit5_Electrocuted);
    mBaseAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit6);

    BaseAliveGameObjectPathTLV = nullptr;
    BaseAliveGameObjectCollisionLine = nullptr;
    mHealth = FP_FromInteger(1);
    mLiftPoint = nullptr;
    field_106_shot = 0;
    field_108_bMotionChanged = 0;
    field_EC = 0;
    mCurrentMotion = 0;
    mNextMotion = 0;
    mPreviousMotion = 0;
    mBaseAliveGameObjectLastAnimFrame = 0;
    BaseAliveGameObjectLastLineYPos = FP_FromInteger(0);
    field_104_pending_resource_count = 0;
    
    gBaseAliveGameObjects_4FC8A0->Push_Back(this);

    mBaseGameObjectFlags.Set(Options::eIsBaseAliveGameObject_Bit6);
}


BaseAliveGameObject::~BaseAliveGameObject()
{
    gBaseAliveGameObjects_4FC8A0->Remove_Item(this);

    if (mLiftPoint)
    {
        mLiftPoint->VRemove(this);
        mLiftPoint->mBaseGameObjectRefCount--;
        mLiftPoint = nullptr;
    }

    if (field_104_pending_resource_count)
    {
        ResourceManager::WaitForPendingResources_41EA60(this);
    }
}

void BaseAliveGameObject::VUnPosses()
{
    // Empty
}

void BaseAliveGameObject::VPossessed()
{
    // Empty
}

void BaseAliveGameObject::VSetMotion(s16 state)
{
    field_108_bMotionChanged = TRUE;
    mCurrentMotion = state;
}

void BaseAliveGameObject::VSetXSpawn(s16 camWorldX, s32 screenXPos)
{
    const FP old_x = mXPos;
    const FP old_y = mYPos;

    mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(mSpriteScale, screenXPos));

    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(0, mXPos, old_y, mXPos, old_y);

    if (mLiftPoint)
    {
        mLiftPoint->mXPos += (mXPos - old_x);

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
                BaseAliveGameObjectPathTLV = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::StartController_28, 0);
                if (BaseAliveGameObjectPathTLV
                    && sCollisions->Raycast(
                        mXPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y),
                        mXPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRight.y),
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
                    mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos += hitY - BaseAliveGameObjectLastLineYPos;
            }
        }
    }
}

void BaseAliveGameObject::VSetYSpawn(s32 camWorldY, s16 bLeft)
{
    const FP oldx = mXPos;
    const FP oldy = mYPos;

    auto pFrameHeader = reinterpret_cast<FrameHeader*>(&(*mAnim.field_20_ppBlock)[mAnim.Get_FrameHeader(-1)->field_0_frame_header_offset]);

    if (bLeft == 1)
    {
        mYPos = FP_FromInteger(pFrameHeader->field_5_height + camWorldY + 356);
    }
    else
    {
        mYPos = FP_FromInteger(camWorldY + 124);
    }

    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(
        nullptr,
        mXPos,
        mYPos,
        mXPos,
        mYPos);

    if (mLiftPoint)
    {
        mLiftPoint->mXPos += mXPos - oldx;
        mLiftPoint->mYPos += mYPos - oldy;

        BaseAliveGameObjectCollisionLine->mRect.x += FP_GetExponent(mXPos - oldx);
        BaseAliveGameObjectCollisionLine->mRect.w += FP_GetExponent(mXPos - oldx);
        BaseAliveGameObjectCollisionLine->mRect.y += FP_GetExponent(mYPos - oldy);
        BaseAliveGameObjectCollisionLine->mRect.h += FP_GetExponent(mYPos - oldy);
    }
}

void BaseAliveGameObject::VOnPathTransition(s16 camWorldX, s32 camWorldY, CameraPos direction)
{
    VOnPathTransition_401470(camWorldX, camWorldY, direction);
}

s16 BaseAliveGameObject::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_401920(pFrom);
}

s16 BaseAliveGameObject::VTakeDamage_401920(BaseGameObject* /*pFrom*/)
{
    // Defaults to no damage.
    return 0;
}

void BaseAliveGameObject::VOnTlvCollision(Path_TLV* /*pTlv*/)
{
    // Empty
}

void BaseAliveGameObject::VCheckCollisionLineStillValid(s32 distance)
{
    VCheckCollisionLineStillValid_401A90(distance);
}

void BaseAliveGameObject::VOnTrapDoorOpen()
{
    // Empty
}

void BaseAliveGameObject::VCheckCollisionLineStillValid_401A90(s32 distance)
{
    if (BaseAliveGameObjectCollisionLine)
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        const CollisionMask mask = mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor;
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
            if (mLiftPoint)
            {
                if (pLine->mLineType == eLineTypes ::eDynamicCollision_32 ||
                    pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
                {
                    // OG bug fix: didn't remove ourself from the lift!
                    mLiftPoint->VRemove(this);
                    mLiftPoint->mBaseGameObjectRefCount--;
                    mLiftPoint = nullptr;

                    PSX_RECT bRect = VGetBoundingRect();
                    bRect.y += 5;
                    bRect.h += 5;

                    VOnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        ObjListPlatforms_50766C,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                }
            }
        }
        else
        {
            BaseAliveGameObjectPathTLV = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::StartController_28, 0);
            if (BaseAliveGameObjectPathTLV)
            {
                if (sCollisions->Raycast(
                        mXPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y),
                        mXPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->mBottomRight.y),
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
}

BirdPortal* BaseAliveGameObject::IntoBirdPortal_402350(s16 distance)
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eBirdPortal)
        {
            auto pPortal = static_cast<BirdPortal*>(pObjIter);
            if (pPortal->mXPos >= mXPos)
            {
                if (pPortal->mEnterSide == PortalSide::eLeft_1)
                {
                    if (pPortal->mXPos - mXPos <= (ScaleToGridSize(mSpriteScale) * FP_FromInteger(distance)))
                    {
                        if (!mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            if (FP_Abs(mYPos - pPortal->mHitY) < mSpriteScale * FP_FromInteger(10))
                            {
                                if (pPortal->VPortalClipper(1))
                                {
                                    mAnim.mRenderLayer = mSpriteScale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
                                    return pPortal;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (pPortal->mEnterSide == PortalSide::eRight_0)
                {
                    if (mXPos - pPortal->mXPos <= (ScaleToGridSize(mSpriteScale) * FP_FromInteger(distance)))
                    {
                        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            if (FP_Abs(mYPos - pPortal->mHitY) < (mSpriteScale * FP_FromInteger(10)))
                            {
                                if (pPortal->VPortalClipper(1))
                                {
                                    mAnim.mRenderLayer = mSpriteScale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
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


s16 BaseAliveGameObject::SetBaseAnimPaletteTint(const TintEntry* pTintArray, EReliveLevelIds lvl, s32 palId)
{
    const TintEntry* pIter = pTintArray;
    while (pIter->field_0_level != lvl)
    {
        if (pIter->field_0_level == EReliveLevelIds::eNone) // End of entries
        {
            return 0;
        }
        pIter++;
    }

    mRGB.SetRGB(pIter->field_1_r, pIter->field_2_g, pIter->field_3_b);

    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, palId, 1, 0);
    if (!ppRes)
    {
        return 0;
    }
    mAnim.LoadPal(ppRes, 0);
    ResourceManager::FreeResource_455550(ppRes);
    return 1;
}

s16 BaseAliveGameObject::Check_IsOnEndOfLine_4021A0(s16 direction, s16 dist)
{
    // Check if distance grid blocks from current snapped X is still on the line or not, if not then we are
    // about to head off an edge.
    const FP gridSize = ScaleToGridSize(mSpriteScale);

    FP xLoc = {};
    if (direction == 1)
    {
        xLoc = -(gridSize * FP_FromInteger(dist));
    }
    else
    {
        xLoc = gridSize * FP_FromInteger(dist);
    }

    const s16 xposRounded = FP_GetExponent(mXPos) & 1023;
    const FP xPosSnapped = FP_FromInteger((FP_GetExponent(mXPos) & 0xFC00) + SnapToXGrid(mSpriteScale, xposRounded));
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
               mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor)
        == 0;
}

void BaseAliveGameObject::VOnPathTransition_401470(s16 camWorldX, s32 camWorldY, CameraPos direction)
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
            const s32 frameH = reinterpret_cast<FrameHeader*>((*mAnim.field_20_ppBlock)[mAnim.Get_FrameHeader(-1)->field_0_frame_header_offset])->field_5_height;

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
            mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(mSpriteScale, MaxGridBlocks_41FA10(mSpriteScale) - 1));
            mYPos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
            break;

        case CameraPos::eCamRight_4:
            width = camWorldX + 356;
            xpos = camWorldX + 206;
            ypos = camWorldY + 70;
            height = camWorldY + 410;
            mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(mSpriteScale, 1));
            mYPos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
            break;

        default:
            break;
    }

    // Find the start controller at the position we will be at in the new map
    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446260(static_cast<s16>(xpos), static_cast<s16>(ypos), static_cast<s16>(width), static_cast<s16>(height), TlvTypes::StartController_28);

    if (!BaseAliveGameObjectPathTLV)
    {
        BaseAliveGameObjectPathTLV = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::StartController_28, 0);
        LOG_INFO("Flip direction after the path trans as we are not touching the start controller");
        mVelX = -mVelX;
        mAnim.mFlags.Toggle(AnimFlags::eBit5_FlipX);
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

    mXPos = FP_FromInteger((BaseAliveGameObjectPathTLV->mBottomRight.x + BaseAliveGameObjectPathTLV->mTopLeft.x) / 2);
    mYPos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeft.y);

    mXPos = FP_FromInteger(camLoc.x + SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos - FP_FromInteger(camLoc.x))));

    if (mLiftPoint)
    {
        // Move lift point into the new path
        const FP rect_left = mXPos - oldx;
        const FP rect_right = mYPos - oldy;

        mLiftPoint->mXPos += rect_left;
        mLiftPoint->mYPos += rect_right;

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
                    mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
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
            if (BaseAliveGameObjectPathTLV->mTlvType32 == TlvTypes::StartController_28)
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
                    mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos += hitY - BaseAliveGameObjectLastLineYPos;
            }
        }
    }

    if (mSpriteScale == FP_FromInteger(1) && mAnim.field_14_scale == FP_FromDouble(0.5))
    {
        mVelX = (mVelX * FP_FromInteger(2));
        return;
    }

    if (mSpriteScale == FP_FromDouble(0.5) && mAnim.field_14_scale == FP_FromInteger(1))
    {
        mVelX = (mVelX * FP_FromDouble(0.5));
        return;
    }
}


s16 BaseAliveGameObject::MapFollowMe_401D30(s16 snapToGrid)
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);

    // Are we "in" the current camera X bounds?
    if (mCurrentLevel == gMap.mCurrentLevel && mCurrentPath == gMap.mCurrentPath && mXPos > FP_FromInteger(camCoords.x) && mXPos < FP_FromInteger(camCoords.x + 1024))
    {
        // Snapped XPos in camera space
        const s32 snappedXLocalCoords = SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos - FP_FromInteger(camCoords.x)));

        // In the left camera void and moving left?
        if (snappedXLocalCoords < 256 && mVelX < FP_FromInteger(0))
        {
            if (sControlledCharacter == this && gMap.SetActiveCameraDelayed(Map::MapDirections::eMapLeft_0, this, -1))
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
                    const s32 cam1GridBeforeRight = XGrid_Index_To_XPos(mSpriteScale, (MaxGridBlocks_41FA10(mSpriteScale) - 1));
                    const s32 camRightEdge = x_i - camXIndex - 1024;
                    mXPos = FP_FromInteger(camRightEdge) + FP_FromInteger(cam1GridBeforeRight) + ScaleToGridSize(mSpriteScale);

                    VCheckCollisionLineStillValid(40);
                }
            }
        }
        // In the right camera void and moving right?
        else if (snappedXLocalCoords > 624 && mVelX > FP_FromInteger(0))
        {
            // Go to the right camera in under player control
            if (sControlledCharacter == this && gMap.SetActiveCameraDelayed(Map::MapDirections::eMapRight_1, this, -1))
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
                    const s32 cam1GridAfterLeft = XGrid_Index_To_XPos(mSpriteScale, 1);
                    const s32 camLeftEdge = x_i - camXIndex + 1024;
                    mXPos = FP_FromInteger(camLeftEdge) + FP_FromInteger(cam1GridAfterLeft) - ScaleToGridSize(mSpriteScale);

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

                const s32 camRightGrid = XGrid_Index_To_XPos(mSpriteScale, MaxGridBlocks_41FA10(mSpriteScale));
                const s32 camRightEdge = x_i - camXIndex - 1024;
                mXPos = FP_FromInteger(camRightEdge) + FP_FromInteger(camRightGrid) + ScaleToGridSize(mSpriteScale);

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

                const s32 camLeftGrid = XGrid_Index_To_XPos(mSpriteScale, 0);
                const s32 camLeftEdge = x_i - camXIndex + 1024;
                mXPos = FP_FromInteger(camLeftEdge) + FP_FromInteger(camLeftGrid) - ScaleToGridSize(mSpriteScale);

                VCheckCollisionLineStillValid(40);
            }
        }
        return 0;
    }
}

void BaseAliveGameObject::SetActiveCameraDelayedFromDir_401C90()
{
    if (sControlledCharacter == this)
    {
        switch (Is_In_Current_Camera())
        {
            case CameraPos::eCamTop_1:
                if (mVelY < FP_FromInteger(0))
                {
                    gMap.SetActiveCameraDelayed(Map::MapDirections::eMapTop_2, this, -1);
                }
                break;

            case CameraPos::eCamBottom_2:
                if (mVelY > FP_FromInteger(0))
                {
                    gMap.SetActiveCameraDelayed(Map::MapDirections::eMapBottom_3, this, -1);
                }
                break;

            case CameraPos::eCamLeft_3:
                if (mVelX < FP_FromInteger(0))
                {
                    gMap.SetActiveCameraDelayed(Map::MapDirections::eMapLeft_0, this, -1);
                }
                break;

            case CameraPos::eCamRight_4:
                if (mVelX > FP_FromInteger(0))
                {
                    gMap.SetActiveCameraDelayed(Map::MapDirections::eMapRight_1, this, -1);
                }
                break;

            default:
                return;
        }
    }
}



s16 BaseAliveGameObject::WallHit_401930(FP offY, FP offX)
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
               mSpriteScale != FP_FromDouble(0.5) ? kFgWalls : kBgWalls)
        != 0;
}

s16 BaseAliveGameObject::InAirCollision_4019C0(PathLine** ppLine, FP* hitX, FP* hitY, FP vely)
{
    mVelY += (mSpriteScale * vely);

    if (mVelY > (mSpriteScale * FP_FromInteger(20)))
    {
        mVelY = (mSpriteScale * FP_FromInteger(20));
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
        mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor) ? 1 : 0;
}

void BaseAliveGameObject::OnResourceLoaded_4019A0(BaseAliveGameObject* ppRes)
{
    ppRes->field_104_pending_resource_count--;
}

void BaseAliveGameObject::UsePathTransScale_4020D0()
{
    auto pPathTrans = static_cast<Path_PathTransition*>(gMap.TLV_Get_At_446260(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        TlvTypes::PathTransition_1));

    if (pPathTrans)
    {
        if (pPathTrans->field_22_next_path_scale == Scale_short::eHalf_1)
        {
            if (mSpriteScale != FP_FromDouble(0.5))
            {
                mSpriteScale = FP_FromDouble(0.5);
                mScale = Scale::Bg;
                mVelX = (mVelX * FP_FromDouble(0.5));
            }
        }
        else if (pPathTrans->field_22_next_path_scale == Scale_short::eFull_0)
        {
            if (mSpriteScale != FP_FromInteger(1))
            {
                mSpriteScale = FP_FromInteger(1);
                mScale = Scale::Fg;
                mVelX = (mVelX * FP_FromInteger(2));
            }
        }
    }
}

BaseGameObject* BaseAliveGameObject::FindObjectOfType_418280(ReliveTypes typeToFind, FP xpos, FP ypos)
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

        if (pObj->mBaseGameObjectTypeId == typeToFind)
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

s16 BaseAliveGameObject::OnTrapDoorIntersection_401C10(PlatformBase* pPlatform)
{
    const PSX_RECT rect = pPlatform->VGetBoundingRect();
    if (FP_GetExponent(mXPos) < rect.x || FP_GetExponent(mXPos) > rect.w || FP_GetExponent(mYPos) > rect.h)
    {
        return 1;
    }

    // OG bug fix, when we call VCheckCollisionLineStillValid it can place us on a new lift
    // but then we call VOnCollisionWith which can sometimes add us to the same lift again
    // result in the lift being leaked and then memory corruption/crash later.
    if (mLiftPoint != pPlatform)
    {
        mLiftPoint = pPlatform;
        mLiftPoint->VAdd(this);
        mLiftPoint->mBaseGameObjectRefCount++;
    }
    else
    {
        LOG_WARNING("Trying to add to a platform we are already on");
    }

    return 1;
}

} // namespace AO
