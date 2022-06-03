#include "stdafx.h"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "ObjectIds.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "PathData.hpp"
#include "PlatformBase.hpp"
#include "Game.hpp"
#include "BirdPortal.hpp"
#include "Events.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"

ALIVE_VAR(1, 0x5C1B7C, DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_5C1B7C, nullptr);

BaseAliveGameObject::BaseAliveGameObject(s16 resourceArraySize)
    : BaseAnimatedWithPhysicsGameObject(resourceArraySize)
{
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
    mAliveGameObjectFlags.Clear(Flags_114::e114_MotionChanged_Bit2);
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit5_ZappedByShrykull);
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit6_SetOffExplosives);
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit7_Electrocuted);
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit8_bInvisible);
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit10_Teleporting);
    mAliveGameObjectFlags.Clear(Flags_114::e114_Bit11_Electrocuting);

    mPathTLV = nullptr;
    mCollisionLine = nullptr;
    mHealth = FP_FromDouble(1.0);
    mObjectId = -1;
    mCurrentMotion = 0;
    mNextMotion = 0;
    mPreviousMotion = 0;
    mAnimFrame = 0;
    mLastLineYPos = FP_FromInteger(0);
    field_10A_unused = 0;

    gBaseAliveGameObjects_5C1B7C->Push_Back(this);

    mGameObjectFlags.Set(BaseGameObject::eIsBaseAliveGameObject_Bit6);
}

BaseAliveGameObject::~BaseAliveGameObject()
{
    BaseAliveGameObject* pField_110 = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mObjectId));
    gBaseAliveGameObjects_5C1B7C->Remove_Item(this);

    if (pField_110)
    {
        pField_110->VOnTrapDoorOpen();
        mObjectId = -1;
    }

    if (field_10A_unused)
    {
        pResourceManager_5C1BB0->Shutdown_465610();
    }
}

void BaseAliveGameObject::VRender(PrimHeader** ppOt)
{
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}

void BaseAliveGameObject::VUnPosses()
{
    // Empty
}

void BaseAliveGameObject::VPossessed()
{
    // Empty
}

s16 BaseAliveGameObject::IsInInvisibleZone(BaseAliveGameObject* pObj)
{
    /* OG unused feature to always appear as if you are in an invisible zone
    if (word_5C1BE4)
    {
        return TRUE;
    }*/

    if (Event_Get(kEventAbeOhm))
    {
        return FALSE;
    }

    PSX_RECT bRect = {};
    pObj->VGetBoundingRect(&bRect, 1);

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        bRect.x,
        bRect.y,
        bRect.w,
        bRect.h,
        TlvTypes::InvisibleZone_33);

    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::InvisibleZone_33)
        {
            if (bRect.x >= pTlv->field_8_top_left.field_0_x && bRect.x <= pTlv->field_C_bottom_right.field_0_x && bRect.y >= pTlv->field_8_top_left.field_2_y)
            {
                if (bRect.y <= pTlv->field_C_bottom_right.field_2_y && bRect.w >= pTlv->field_8_top_left.field_0_x && bRect.w <= pTlv->field_C_bottom_right.field_0_x && bRect.h >= pTlv->field_8_top_left.field_2_y && bRect.h <= pTlv->field_C_bottom_right.field_2_y)
                {
                    return TRUE;
                }
            }
        }

        // Check for stacked/overlaping TLV's
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv,
                                                     FP_FromInteger(bRect.x),
                                                     FP_FromInteger(bRect.y),
                                                     FP_FromInteger(bRect.w),
                                                     FP_FromInteger(bRect.h));
    }
    return FALSE;
}

// =======

void BaseAliveGameObject::VSetMotion(s16 state)
{
    mAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
    mCurrentMotion = state;
}

s32 MaxGridBlocks_449880(FP scale)
{
    if (scale == FP_FromDouble(0.5))
    {
        return 30; // (29+1) * 13 (grid block size) for 377/390
    }
    else if (scale == FP_FromInteger(1))
    {
        return 16; // (15+1) * 25 (grid block size) for 375/400
    }
    else
    {
        LOG_ERROR("Scale should be 0.5 or 1 but got " << FP_GetDouble(scale));
        ALIVE_FATAL("Invalid scale");
    }
}

void BaseAliveGameObject::VOnPathTransition(s16 cameraWorldXPos, s16 cameraWorldYPos, CameraPos direction)
{
    const FP oldY = mYPos;
    switch (direction)
    {
        case CameraPos::eCamTop_1:
            mXPos = FP_FromInteger(cameraWorldXPos + FP_GetExponent(mXPos) % 375);
            if (mAnim.mAnimFlags.Get(AnimFlags::eBit22_DeadMode))
            {
                ALIVE_FATAL("Impossible case BaseAliveGameObject::vOnPathTransition_408320 AnimFlags::eBit22_DeadMode");
            }
            else
            {
                // TODO: This is actually wrong!!
                const u32 off = mAnim.Get_FrameHeader(-1)->field_0_frame_header_offset;
                mYPos = FP_FromInteger((*mAnim.field_20_ppBlock)[off + 5] + cameraWorldYPos + 236);
            }
            break;

        case CameraPos::eCamBottom_2:
            mXPos = FP_FromInteger(cameraWorldXPos + FP_GetExponent(mXPos) % 375);
            mYPos = FP_FromInteger(cameraWorldYPos + 4);
            break;

        case CameraPos::eCamLeft_3:
            mXPos = FP_FromInteger(cameraWorldXPos + (XGrid_Index_To_XPos_4498F0(mSpriteScale, MaxGridBlocks_449880(mSpriteScale) - 1)));
            mYPos = FP_FromInteger(cameraWorldYPos + FP_GetExponent(mYPos) % 260);
            break;

        case CameraPos::eCamRight_4:
            mXPos = FP_FromInteger(cameraWorldXPos + XGrid_Index_To_XPos_4498F0(mSpriteScale, 1));
            mYPos = FP_FromInteger(cameraWorldYPos + FP_GetExponent(mYPos) % 260);
            break;

        default:
            break;
    }

    mXPos = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)));

    if (sActiveHero == this && gMap.mCurrentLevel == LevelIds::eNecrum_2 && gMap.mCurrentPath == 2 && (mCurrentMotion == eAbeMotions::Motion_23_RollLoop_453A90 || mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0))
    {
        // Yummy OWI hack - hard code Abe's location when path change to Necrum's first path after the Mines :)
        mCollisionLine = nullptr;
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
        if (mCollisionLine)
        {
            if (sCollisions_DArray_5C1128->Raycast(
                    mXPos, mYPos - FP_FromInteger(40),
                    mXPos, mYPos + FP_FromInteger(40),
                    &pLine, &hitX, &hitY, mScale != 0 ? 1 : 16))
            {
                mYPos = hitY;
                mCollisionLine = pLine;
            }
            else
            {
                mCollisionLine = nullptr;
            }
        }
        else
        {
            mLastLineYPos = mYPos - oldY + mLastLineYPos;
            if (sCollisions_DArray_5C1128->Raycast(
                    mXPos, mLastLineYPos - FP_FromInteger(40),
                    mXPos, mLastLineYPos + FP_FromInteger(40),
                    &pLine, &hitX, &hitY, mScale != 0 ? 1 : 16))
            {
                mYPos = hitY - mLastLineYPos + mYPos;
            }
            else
            {
                // Not set to nullptr in this case ??
            }
        }
    }

    if (mSpriteScale == FP_FromInteger(1) && mAnim.field_14_scale == FP_FromDouble(0.5))
    {
        // From 0.5 to 1 scale, f64 velx
        mVelX = mVelX * FP_FromInteger(2);
    }

    if (mSpriteScale == FP_FromDouble(0.5) && mAnim.field_14_scale == FP_FromInteger(1))
    {
        // From 1 to 0.5 scale, halve velx
        mVelX = mVelX * FP_FromDouble(0.5);
    }
}

s16 BaseAliveGameObject::VTakeDamage(BaseGameObject* /*pFrom*/)
{
    // Defaults to no damage.
    return 0;
}

void BaseAliveGameObject::VOn_TLV_Collision(Path_TLV* /*pTlv*/)
{
    // Empty
}

void BaseAliveGameObject::VCheckCollisionLineStillValid(s16 distance)
{
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(mObjectId));
    if (!mCollisionLine)
    {
        return;
    }

    if (pPlatform)
    {
        mObjectId = -1;
        pPlatform->VRemove(this);
    }

    const FP distanceFp = FP_FromInteger(distance);
    const FP yTop = mYPos + distanceFp;
    const FP yBottom = mYPos - distanceFp;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            mXPos,
            yBottom,
            mXPos,
            yTop,
            &pLine,
            &hitX,
            &hitY,
            mScale == 1 ? 0x0F : 0xF0))
    {
        mCollisionLine = pLine;
        mYPos = hitY;

        if (pLine->field_8_type == eLineTypes::eUnknown_32 ||
            pLine->field_8_type == eLineTypes::eUnknown_36)
        {
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);

            PSX_Point xy = {bRect.x, bRect.y};
            PSX_Point wh = {bRect.w, bRect.h};
            xy.field_2_y += 5;
            wh.field_2_y += 5;

            VOnCollisionWith(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
        }
    }
    else
    {
        mCollisionLine = nullptr;
    }
}

BirdPortal* BaseAliveGameObject::VIntoBirdPortal(s16 numGridBlocks)
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eBirdPortal_99)
        {
            auto pBirdPortal = static_cast<BirdPortal*>(pObj);
            if (pBirdPortal->field_2C_xpos >= mXPos)
            {
                if (pBirdPortal->field_26_side == PortalSide::eLeft_1)
                {
                    if (pBirdPortal->field_2C_xpos - mXPos <= (ScaleToGridSize(mSpriteScale) * FP_FromInteger(numGridBlocks)) && !(mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
                    {
                        if (FP_Abs(mYPos - pBirdPortal->field_3C_YPos) < mSpriteScale * FP_FromInteger(10) && pBirdPortal->VPortalClipper(1))
                        {
                            mAnim.mRenderLayer = mSpriteScale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
                            return pBirdPortal;
                        }
                    }
                }
            }
            else if (pBirdPortal->field_26_side == PortalSide::eRight_0)
            {
                if (mXPos - pBirdPortal->field_2C_xpos <= ScaleToGridSize(mSpriteScale) * FP_FromInteger(numGridBlocks))
                {
                    if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        if (FP_Abs(mYPos - pBirdPortal->field_3C_YPos) < mSpriteScale * FP_FromInteger(10) && pBirdPortal->VPortalClipper(1))
                        {
                            mAnim.mRenderLayer = mSpriteScale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
                            return pBirdPortal;
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

void BaseAliveGameObject::VOnTrapDoorOpen()
{
    // Empty
}

s16 BaseAliveGameObject::SetBaseAnimPaletteTint(TintEntry* pTintArray, LevelIds level_id, s32 resourceID)
{
    SetTint(pTintArray, level_id);

    u8** pPalResource = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, resourceID, 1u, 0);

    if (!pPalResource)
    {
        return 0;
    }

    mAnim.Load_Pal(pPalResource, 0);
    ResourceManager::FreeResource_49C330(pPalResource);
    return 1;
}

Bool32 BaseAliveGameObject::Check_IsOnEndOfLine(s16 direction, s16 distance)
{
    // Check if distance grid blocks from current snapped X is still on the line or not, if not then we are
    // about to head off an edge.

    const FP gridSize = ScaleToGridSize(mSpriteScale);

    FP xLoc = {};
    if (direction == 1)
    {
        xLoc = -(gridSize * FP_FromInteger(distance));
    }
    else
    {
        xLoc = gridSize * FP_FromInteger(distance);
    }

    const FP xPosSnapped = FP_FromInteger(SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos)));

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions_DArray_5C1128->Raycast(
               xLoc + xPosSnapped,
               mYPos - FP_FromInteger(4),
               xLoc + xPosSnapped,
               mYPos + FP_FromInteger(4),
               &pLine,
               &hitX,
               &hitY,
               mScale != 0 ? 0xF : 0xF0)
        == 0;
}

BaseAliveGameObject* BaseAliveGameObject::GetStackedSlapTarget(s32 idToFind, AETypes typeToFind, FP xpos, FP ypos)
{
    const s16 xposD = FP_GetExponent(xpos);
    const s16 yposD = FP_GetExponent(ypos);

    Bool32 bFound = FALSE;
    for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == typeToFind && pObj != this)
        {
            if (pObj->field_8_object_id == idToFind)
            {
                // So that we pick the one AFTER this
                bFound = TRUE;
            }
            else if (bFound)
            {
                PSX_RECT bRect = {};
                BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
                pAliveObj->VGetBoundingRect(&bRect, 1);
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

void BaseAliveGameObject::SetActiveCameraDelayedFromDir()
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

            case CameraPos::eCamCurrent_0:
            case CameraPos::eCamNone_5:
                return;

            default:
                return;
        }
    }
}

s16 BaseAliveGameObject::MapFollowMe(s16 snapToGrid)
{
    const s32 xposSnapped = SnapToXGrid(mSpriteScale, FP_GetExponent(mXPos));
    if (snapToGrid)
    {
        mXPos = FP_FromInteger(xposSnapped);
    }

    PSX_Point currentCamXY = {};
    gMap.GetCurrentCamCoords(&currentCamXY);

    // Gone off the left edge of the current screen
    if (xposSnapped < currentCamXY.field_0_x && (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) || mVelX < FP_FromInteger(0)))
    {
        if (sControlledCharacter == this && gMap.SetActiveCameraDelayed(Map::MapDirections::eMapLeft_0, this, -1))
        {
            mLvlNumber = gMap.mCurrentLevel;
            mPathNumber = gMap.mCurrentPath;
            return 1;
        }
    }
    // Gone off the right edge of the current screen
    else if (xposSnapped > currentCamXY.field_0_x + 368 && (!(mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) || mVelX > FP_FromInteger(0)))
    {
        if (sControlledCharacter == this && gMap.SetActiveCameraDelayed(Map::MapDirections::eMapRight_1, this, -1))
        {
            mLvlNumber = gMap.mCurrentLevel;
            mPathNumber = gMap.mCurrentPath;
            return 1;
        }
    }
    return 0;
}

Bool32 BaseAliveGameObject::WallHit(FP offY, FP offX)
{
    PathLine* pLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast(
               mXPos,
               mYPos - offY,
               mXPos + offX,
               mYPos - offY,
               &pLine,
               &offX,
               &offY,
               mScale != 0 ? 6 : 96)
        != 0; // TODO: Enum for line types
}

Bool32 BaseAliveGameObject::InAirCollision(PathLine** ppPathLine, FP* hitX, FP* hitY, FP velY)
{
    mVelY += mSpriteScale * velY;
    if (mVelY > (mSpriteScale * FP_FromInteger(20)))
    {
        mVelY = mSpriteScale * FP_FromInteger(20);
    }

    const FP oldYPos = mYPos;
    const FP oldXPos = mXPos;

    mXPos += mVelX;
    mYPos += mVelY;

    auto bCollision = sCollisions_DArray_5C1128->Raycast(
        oldXPos,
        oldYPos,
        mXPos,
        mYPos,
        ppPathLine,
        hitX,
        hitY,
        (mScale == 1) ? 15 : 240);

    if (bCollision)
    {
        return bCollision;
    }

    FP velYClamped = mVelY;
    if (Type() == AETypes::eMudokon_110 && velYClamped >= FP_FromInteger(0) && velYClamped < FP_FromInteger(4))
    {
        velYClamped = FP_FromInteger(4);
    }

    bCollision = sCollisions_DArray_5C1128->Raycast(
        mXPos,
        mYPos,
        mXPos + mVelX,
        velYClamped + mYPos,
        ppPathLine,
        hitX,
        hitY,
        mScale != 0 ? 1 : 16);

    if (bCollision)
    {
        if ((*ppPathLine)->field_8_type == eLineTypes::eUnknown_32 ||
            (*ppPathLine)->field_8_type == eLineTypes::eUnknown_36)
        {
            return bCollision;
        }

        bCollision = FALSE;
        *ppPathLine = nullptr;
    }

    if (this != sActiveHero)
    {
        return bCollision;
    }

    const FP k10Scaled = mSpriteScale * FP_FromInteger(10);
    return sCollisions_DArray_5C1128->Raycast(
        oldXPos,
        oldYPos - k10Scaled,
        mXPos,
        mYPos - k10Scaled,
        ppPathLine,
        hitX,
        hitY,
        mScale != 0 ? 6 : 96);
}

BaseGameObject* BaseAliveGameObject::FindObjectOfType(AETypes typeToFind, FP xpos, FP ypos)
{
    const s32 xposI = FP_GetExponent(xpos);
    const s32 yposI = FP_GetExponent(ypos);

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
            if (pCasted->mScale == mScale)
            {
                PSX_RECT bRect = {};
                pCasted->VGetBoundingRect(&bRect, 1);

                if (xposI >= bRect.x && xposI <= bRect.w && yposI >= bRect.y && yposI <= bRect.h)
                {
                    return pObj;
                }
            }
        }
    }
    return nullptr;
}

s16 BaseAliveGameObject::OnTrapDoorIntersection(PlatformBase* pPlatform)
{
    PSX_RECT bRect = {};
    pPlatform->VGetBoundingRect(&bRect, 1);

    if (FP_GetExponent(mXPos) < bRect.x || FP_GetExponent(mXPos) > bRect.w || FP_GetExponent(mYPos) > bRect.h)
    {
        return 1;
    }

    pPlatform->VAdd(this);

    mObjectId = pPlatform->field_8_object_id;
    return 1;
}
