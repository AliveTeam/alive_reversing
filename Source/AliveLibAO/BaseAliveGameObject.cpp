#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "PlatformBase.hpp"
#include "Collisions.hpp"
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
    


    mAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit1_Can_Be_Possessed);
    mAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit2_bPossesed);
    mAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit3);
    mAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit4_SetOffExplosives);
    mAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit5_Electrocuted);
    mAliveGameObjectFlags.Clear(Flags_10A::e10A_Bit6);

    mPathTLV = nullptr;
    field_F8_pLiftPoint = nullptr;
    mCollisionLine = nullptr;
    mHealth = FP_FromInteger(1);
    field_106_shot = 0;
    field_108_bMotionChanged = 0;
    field_EC = 0;
    mCurrentMotion = 0;
    mNextMotion = 0;
    mPreviousMotion = 0;
    mAnimFrame = 0;
    mLastLineYPos = FP_FromInteger(0);
    field_104_pending_resource_count = 0;
    gBaseAliveGameObjects_4FC8A0->Push_Back(this);
    mGameObjectFlags.Set(Options::eIsBaseAliveGameObject_Bit6);
}


BaseAliveGameObject::~BaseAliveGameObject()
{
    gBaseAliveGameObjects_4FC8A0->Remove_Item(this);

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
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
    VSetMotion_402520(state);
}

void BaseAliveGameObject::VSetXSpawn(s16 camWorldX, s32 screenXPos)
{
    VSetXSpawn_401150(camWorldX, screenXPos);
}

void BaseAliveGameObject::VSetYSpawn(s32 camWorldY, s16 bLeft)
{
    VSetYSpawn_401380(camWorldY, bLeft);
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

void BaseAliveGameObject::VOn_TLV_Collision(Path_TLV* /*pTlv*/)
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
    if (mCollisionLine)
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        const s32 mask = mSpriteScale != FP_FromDouble(0.5) ? 7 : 0x70;
        if (sCollisions_DArray_504C6C->RayCast(
                mXPos,
                mYPos - FP_FromInteger(distance),
                mXPos,
                mYPos + FP_FromInteger(distance),
                &pLine,
                &hitX,
                &hitY,
                mask))
        {
            mCollisionLine = pLine;
            mYPos = hitY;
            if (field_F8_pLiftPoint)
            {
                if (pLine->field_8_type == eLineTypes ::eUnknown_32 ||
                    pLine->field_8_type == eLineTypes::eUnknown_36)
                {
                    // OG bug fix: didn't remove ourself from the lift!
                    field_F8_pLiftPoint->VRemove(this);
                    field_F8_pLiftPoint->field_C_refCount--;
                    field_F8_pLiftPoint = nullptr;

                    PSX_RECT bRect = {};
                    VGetBoundingRect(&bRect, 1);
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
            mPathTLV = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::StartController_28, 0);
            if (mPathTLV)
            {
                if (sCollisions_DArray_504C6C->RayCast(
                        mXPos,
                        FP_FromInteger(mPathTLV->field_10_top_left.field_2_y),
                        mXPos,
                        FP_FromInteger(mPathTLV->field_14_bottom_right.field_2_y),
                        &pLine,
                        &hitX,
                        &hitY,
                        mask))
                {
                    mCollisionLine = pLine;
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

        if (pObjIter->mTypeId == Types::eBirdPortal_65)
        {
            auto pPortal = static_cast<BirdPortal*>(pObjIter);
            if (pPortal->field_18_xpos >= mXPos)
            {
                if (pPortal->field_12_side == PortalSide::eLeft_1)
                {
                    if (pPortal->field_18_xpos - mXPos <= (ScaleToGridSize(mSpriteScale) * FP_FromInteger(distance)))
                    {
                        if (!mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            if (FP_Abs(mYPos - pPortal->field_28_ypos) < mSpriteScale * FP_FromInteger(10))
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
                if (pPortal->field_12_side == PortalSide::eRight_0)
                {
                    if (mXPos - pPortal->field_18_xpos <= (ScaleToGridSize(mSpriteScale) * FP_FromInteger(distance)))
                    {
                        if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            if (FP_Abs(mYPos - pPortal->field_28_ypos) < (mSpriteScale * FP_FromInteger(10)))
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
    return sCollisions_DArray_504C6C->RayCast(
               xLoc + xPosSnapped,
               mYPos - FP_FromInteger(4),
               xLoc + xPosSnapped,
               mYPos + FP_FromInteger(4),
               &pLine,
               &hitX,
               &hitY,
               mSpriteScale != FP_FromDouble(0.5) ? 7 : 0x70)
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
    mPathTLV = gMap.TLV_Get_At_446260(static_cast<s16>(xpos), static_cast<s16>(ypos), static_cast<s16>(width), static_cast<s16>(height), TlvTypes::StartController_28);

    if (!mPathTLV)
    {
        mPathTLV = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::StartController_28, 0);
        LOG_INFO("Flip direction after the path trans as we are not touching the start controller");
        mVelX = -mVelX;
        mAnim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
    }
    else
    {
        LOG_INFO("Not changing direction in the new path trans as we are touching the start controller");
    }

    if (!mPathTLV)
    {
        LOG_ERROR("Did a path transition to a camera with no start controller");
        ALIVE_FATAL("Did a path transition to a camera with no start controller (add one if this is an edited level)");
    }

    PSX_Point camLoc = {};
    gMap.GetCurrentCamCoords(&camLoc);

    mXPos = FP_FromInteger((mPathTLV->field_14_bottom_right.field_0_x + mPathTLV->field_10_top_left.field_0_x) / 2);
    mYPos = FP_FromInteger(mPathTLV->field_10_top_left.field_2_y);

    mXPos = FP_FromInteger(camLoc.field_0_x + SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos - FP_FromInteger(camLoc.field_0_x))));

    if (field_F8_pLiftPoint)
    {
        // Move lift point into the new path
        const FP rect_left = mXPos - oldx;
        const FP rect_right = mYPos - oldy;

        field_F8_pLiftPoint->mXPos += rect_left;
        field_F8_pLiftPoint->mYPos += rect_right;

        mCollisionLine->field_0_rect.x += FP_GetExponent(rect_left);
        mCollisionLine->field_0_rect.w += FP_GetExponent(rect_left);
        mCollisionLine->field_0_rect.y += FP_GetExponent(rect_right);
        mCollisionLine->field_0_rect.h += FP_GetExponent(rect_right);
    }
    else
    {
        // OG FIX: After a path trans the collision items are free so we can't possibly have a line
        // so try to find a new one under our feet.
        if (!mCollisionLine)
        {
            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_504C6C->RayCast(
                    mXPos,
                    mYPos - FP_FromInteger(40),
                    mXPos,
                    mYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    mSpriteScale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                mCollisionLine = pLine;
                mYPos = hitY;
            }
            else
            {
                mCollisionLine = nullptr;
            }
        }

        // If we still didn't get a line then look for a start controller
        if (!mCollisionLine)
        {
            if (mPathTLV->field_4_type == TlvTypes::StartController_28)
            {
                mLastLineYPos += mYPos - oldy;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_504C6C->RayCast(
                    mXPos,
                    mLastLineYPos - FP_FromInteger(40),
                    mXPos,
                    mLastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    mSpriteScale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                mYPos += hitY - mLastLineYPos;
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
    if (mLvlNumber == gMap.mCurrentLevel && mPathNumber == gMap.mCurrentPath && mXPos > FP_FromInteger(camCoords.field_0_x) && mXPos < FP_FromInteger(camCoords.field_0_x + 1024))
    {
        // Snapped XPos in camera space
        const s32 snappedXLocalCoords = SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos - FP_FromInteger(camCoords.field_0_x)));

        // In the left camera void and moving left?
        if (snappedXLocalCoords < 256 && mVelX < FP_FromInteger(0))
        {
            if (sControlledCharacter == this && gMap.SetActiveCameraDelayed(Map::MapDirections::eMapLeft_0, this, -1))
            {
                mPathNumber = gMap.mCurrentPath;
                mLvlNumber = gMap.mCurrentLevel;
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
                mPathNumber = gMap.mCurrentPath;
                mLvlNumber = gMap.mCurrentLevel;
                return 1;
            }
            else
            {
                const s32 x_i = abs(FP_GetExponent(mXPos));
                const s32 camXIndex = x_i % 1024;

                gMap.Get_map_size(&camCoords);
                if (x_i < (camCoords.field_0_x - 1024))
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
            mXPos = FP_FromInteger(snappedXLocalCoords + camCoords.field_0_x);
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
            if (x_i < (camCoords.field_0_x - 1024))
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
        switch (Is_In_Current_Camera_417CC0())
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

s16 BaseAliveGameObject::OnTrapDoorIntersection_401C10(PlatformBase* pPlatform)
{
    PSX_RECT rect = {};

    pPlatform->VGetBoundingRect(&rect, 1);
    if (FP_GetExponent(mXPos) < rect.x || FP_GetExponent(mXPos) > rect.w || FP_GetExponent(mYPos) > rect.h)
    {
        return 1;
    }

    // OG bug fix, when we call VCheckCollisionLineStillValid it can place us on a new lift
    // but then we call VOnCollisionWith which can sometimes add us to the same lift again
    // result in the lift being leaked and then memory corruption/crash later.
    if (field_F8_pLiftPoint != pPlatform)
    {
        field_F8_pLiftPoint = pPlatform;
        field_F8_pLiftPoint->VAdd(this);
        field_F8_pLiftPoint->field_C_refCount++;
    }
    else
    {
        LOG_WARNING("Trying to add to a platform we are already on");
    }

    return 1;
}

s16 BaseAliveGameObject::WallHit_401930(FP offY, FP offX)
{
    PathLine* pLine = nullptr;
    return sCollisions_DArray_504C6C->RayCast(
               mXPos,
               mYPos - offY,
               mXPos + offX,
               mYPos - offY,
               &pLine,
               &offX,
               &offY,
               mSpriteScale != FP_FromDouble(0.5) ? 6 : 0x60)
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

    return sCollisions_DArray_504C6C->RayCast(
        old_xpos,
        old_ypos,
        mXPos,
        mYPos,
        ppLine,
        hitX,
        hitY,
        mSpriteScale != FP_FromDouble(0.5) ? 7 : 0x70);
}

void BaseAliveGameObject::OnResourceLoaded_4019A0(BaseAliveGameObject* ppRes)
{
    ppRes->field_104_pending_resource_count--;
}

void BaseAliveGameObject::VSetXSpawn_401150(s16 camWorldX, s32 screenXPos)
{
    const FP old_x = mXPos;
    const FP old_y = mYPos;

    mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(mSpriteScale, screenXPos));

    mPathTLV = gMap.TLV_Get_At_446060(0, mXPos, old_y, mXPos, old_y);

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->mXPos += (mXPos - old_x);

        mCollisionLine->field_0_rect.x += FP_GetExponent(mXPos - old_x);
        mCollisionLine->field_0_rect.w += FP_GetExponent(mXPos - old_x);
        mCollisionLine->field_0_rect.y = mCollisionLine->field_0_rect.y;
        mCollisionLine->field_0_rect.h = mCollisionLine->field_0_rect.h;
    }
    else
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (mCollisionLine)
        {
            if (sCollisions_DArray_504C6C->RayCast(
                    mXPos,
                    old_y - FP_FromInteger(40),
                    mXPos,
                    old_y + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    1 << mCollisionLine->field_8_type))
            {
                mCollisionLine = pLine;
                mYPos = hitY;
            }
            else
            {
                mPathTLV = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::StartController_28, 0);
                if (mPathTLV
                    && sCollisions_DArray_504C6C->RayCast(
                        mXPos,
                        FP_FromInteger(mPathTLV->field_10_top_left.field_2_y),
                        mXPos,
                        FP_FromInteger(mPathTLV->field_14_bottom_right.field_2_y),
                        &pLine,
                        &hitX,
                        &hitY,
                        1 << mCollisionLine->field_8_type))
                {
                    mCollisionLine = pLine;
                    mYPos = hitY;
                }
                else
                {
                    mCollisionLine = nullptr;
                }
            }
        }
        else
        {
            if (sCollisions_DArray_504C6C->RayCast(
                    mXPos,
                    mLastLineYPos - FP_FromInteger(40),
                    mXPos,
                    mLastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    mSpriteScale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                mYPos += hitY - mLastLineYPos;
            }
        }
    }
}

void BaseAliveGameObject::VSetYSpawn_401380(s32 camWorldY, s16 bLeft)
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

    mPathTLV = gMap.TLV_Get_At_446060(
        nullptr,
        mXPos,
        mYPos,
        mXPos,
        mYPos);

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->mXPos += mXPos - oldx;
        field_F8_pLiftPoint->mYPos += mYPos - oldy;

        mCollisionLine->field_0_rect.x += FP_GetExponent(mXPos - oldx);
        mCollisionLine->field_0_rect.w += FP_GetExponent(mXPos - oldx);
        mCollisionLine->field_0_rect.y += FP_GetExponent(mYPos - oldy);
        mCollisionLine->field_0_rect.h += FP_GetExponent(mYPos - oldy);
    }
}

s16 BaseAliveGameObject::IsBeeSwarmChasingMe_4022B0()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mTypeId == Types::eBeeSwarm_95)
        {
            if (static_cast<BeeSwarm*>(pObj)->field_D98_pChaseTarget == this)
            {
                return 1;
            }
        }
    }
    return 0;
}

void BaseAliveGameObject::VSetMotion_402520(s16 state)
{
    field_108_bMotionChanged = TRUE;
    mCurrentMotion = state;
}

void BaseAliveGameObject::UsePathTransScale_4020D0()
{
    auto pPathTrans = static_cast<Path_Change*>(gMap.TLV_Get_At_446260(
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
                mScale = 0;
                mVelX = (mVelX * FP_FromDouble(0.5));
            }
        }
        else if (pPathTrans->field_22_next_path_scale == Scale_short::eFull_0)
        {
            if (mSpriteScale != FP_FromInteger(1))
            {
                mSpriteScale = FP_FromInteger(1);
                mScale = 1;
                mVelX = (mVelX * FP_FromInteger(2));
            }
        }
    }
}

BaseGameObject* BaseAliveGameObject::FindObjectOfType_418280(Types typeToFind, FP xpos, FP ypos)
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

        if (pObj->mTypeId == typeToFind)
        {
            auto pObj2 = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

            PSX_RECT bRect = {};
            pObj2->VGetBoundingRect(&bRect, 1);

            if (xpos_int >= bRect.x && xpos_int <= bRect.w && ypos_int >= bRect.y && ypos_int <= bRect.h)
            {
                return pObj;
            }
        }
    }
    return nullptr;
}

} // namespace AO
