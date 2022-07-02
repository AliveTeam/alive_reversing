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
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit4_bPossesed);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit5_ZappedByShrykull);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit6_SetOffExplosives);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit7_Electrocuted);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit8_bInvisible);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit10_Teleporting);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit11_Electrocuting);

    BaseAliveGameObjectPathTLV = nullptr;
    BaseAliveGameObjectCollisionLine = nullptr;
    mHealth = FP_FromInteger(1);
    BaseAliveGameObject_PlatformId = -1;
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit1_bShot);
    mBaseAliveGameObjectFlags.Clear(Flags_114::e114_MotionChanged_Bit2);
    mCurrentMotion = 0;
    mNextMotion = 0;
    mPreviousMotion = 0;
    mBaseAliveGameObjectLastAnimFrame = 0;
    BaseAliveGameObjectLastLineYPos = FP_FromInteger(0);
    field_10A_unused = 0;

    gBaseAliveGameObjects_5C1B7C->Push_Back(this);

    mBaseGameObjectFlags.Set(BaseGameObject::eIsBaseAliveGameObject_Bit6);
}

BaseAliveGameObject::~BaseAliveGameObject()
{
    BaseAliveGameObject* pLiftPoint = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    gBaseAliveGameObjects_5C1B7C->Remove_Item(this);

    if (pLiftPoint)
    {
        pLiftPoint->VOnTrapDoorOpen();
        BaseAliveGameObject_PlatformId = -1;
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

    if (EventGet(kEventAbeOhm))
    {
        return FALSE;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        bRect.x,
        bRect.y,
        bRect.w,
        bRect.h,
        TlvTypes::InvisibleZone_33);

    while (pTlv)
    {
        if (pTlv->mTlvType32 == TlvTypes::InvisibleZone_33)
        {
            if (bRect.x >= pTlv->mTopLeft.x && bRect.x <= pTlv->mBottomRight.x && bRect.y >= pTlv->mTopLeft.y)
            {
                if (bRect.y <= pTlv->mBottomRight.y && bRect.w >= pTlv->mTopLeft.x && bRect.w <= pTlv->mBottomRight.x && bRect.h >= pTlv->mTopLeft.y && bRect.h <= pTlv->mBottomRight.y)
                {
                    return TRUE;
                }
            }
        }

        // Check for stacked/overlaping TLV's
        pTlv = sPath_dword_BB47C0->TlvGetAt(pTlv,
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
    mBaseAliveGameObjectFlags.Set(Flags_114::e114_MotionChanged_Bit2);
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
    const FP oldY = mBaseAnimatedWithPhysicsGameObject_YPos;
    switch (direction)
    {
        case CameraPos::eCamTop_1:
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(cameraWorldXPos + FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) % 375);
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit22_DeadMode))
            {
                ALIVE_FATAL("Impossible case BaseAliveGameObject::vOnPathTransition_408320 AnimFlags::eBit22_DeadMode");
            }
            else
            {
                // TODO: This is actually wrong!!
                const u32 off = mBaseAnimatedWithPhysicsGameObject_Anim.Get_FrameHeader(-1)->field_0_frame_header_offset;
                mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger((*mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock)[off + 5] + cameraWorldYPos + 236);
            }
            break;

        case CameraPos::eCamBottom_2:
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(cameraWorldXPos + FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) % 375);
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(cameraWorldYPos + 4);
            break;

        case CameraPos::eCamLeft_3:
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(cameraWorldXPos + (XGrid_Index_To_XPos_4498F0(mBaseAnimatedWithPhysicsGameObject_SpriteScale, MaxGridBlocks_449880(mBaseAnimatedWithPhysicsGameObject_SpriteScale) - 1)));
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(cameraWorldYPos + FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) % 260);
            break;

        case CameraPos::eCamRight_4:
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(cameraWorldXPos + XGrid_Index_To_XPos_4498F0(mBaseAnimatedWithPhysicsGameObject_SpriteScale, 1));
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(cameraWorldYPos + FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) % 260);
            break;

        default:
            break;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)));

    if (IsActiveHero(this) && gMap.mCurrentLevel == EReliveLevelIds::eNecrum && gMap.mCurrentPath == 2 && (mCurrentMotion == eAbeMotions::Motion_23_RollLoop_453A90 || mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0))
    {
        // Yummy OWI hack - hard code Abe's location when path change to Necrum's first path after the Mines :)
        BaseAliveGameObjectCollisionLine = nullptr;
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(1011);
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(784);
    }
    else
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (BaseAliveGameObjectCollisionLine)
        {
            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(40),
                    mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(40),
                    &pLine, &hitX, &hitY, mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
                BaseAliveGameObjectCollisionLine = pLine;
            }
            else
            {
                BaseAliveGameObjectCollisionLine = nullptr;
            }
        }
        else
        {
            BaseAliveGameObjectLastLineYPos = mBaseAnimatedWithPhysicsGameObject_YPos - oldY + BaseAliveGameObjectLastLineYPos;
            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos, BaseAliveGameObjectLastLineYPos - FP_FromInteger(40),
                    mBaseAnimatedWithPhysicsGameObject_XPos, BaseAliveGameObjectLastLineYPos + FP_FromInteger(40),
                    &pLine, &hitX, &hitY, mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY - BaseAliveGameObjectLastLineYPos + mBaseAnimatedWithPhysicsGameObject_YPos;
            }
            else
            {
                // Not set to nullptr in this case ??
            }
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1) && mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale == FP_FromDouble(0.5))
    {
        // From 0.5 to 1 scale, f64 velx
        mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(2);
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5) && mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale == FP_FromInteger(1))
    {
        // From 1 to 0.5 scale, halve velx
        mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(0.5);
    }
}

s16 BaseAliveGameObject::VTakeDamage(BaseGameObject* /*pFrom*/)
{
    // Defaults to no damage.
    return 0;
}

void BaseAliveGameObject::VOnTlvCollision(Path_TLV* /*pTlv*/)
{
    // Empty
}

void BaseAliveGameObject::VCheckCollisionLineStillValid(s16 distance)
{
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    if (!BaseAliveGameObjectCollisionLine)
    {
        return;
    }

    if (pPlatform)
    {
        BaseAliveGameObject_PlatformId = -1;
        pPlatform->VRemove(this);
    }

    const FP distanceFp = FP_FromInteger(distance);
    const FP yTop = mBaseAnimatedWithPhysicsGameObject_YPos + distanceFp;
    const FP yBottom = mBaseAnimatedWithPhysicsGameObject_YPos - distanceFp;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            yBottom,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            yTop,
            &pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {
        BaseAliveGameObjectCollisionLine = pLine;
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;

        if (pLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            const PSX_RECT bRect = VGetBoundingRect();

            PSX_Point xy = {bRect.x, bRect.y};
            PSX_Point wh = {bRect.w, bRect.h};
            xy.y += 5;
            wh.y += 5;

            VOnCollisionWith(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection);
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
        auto pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == ReliveTypes::eBirdPortal)
        {
            auto pBirdPortal = static_cast<BirdPortal*>(pObj);
            if (pBirdPortal->field_2C_xpos >= mBaseAnimatedWithPhysicsGameObject_XPos)
            {
                if (pBirdPortal->field_26_side == PortalSide::eLeft_1)
                {
                    if (pBirdPortal->field_2C_xpos - mBaseAnimatedWithPhysicsGameObject_XPos <= (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(numGridBlocks)) && !(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)))
                    {
                        if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pBirdPortal->field_3C_YPos) < mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10) && pBirdPortal->VPortalClipper(1))
                        {
                            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
                            return pBirdPortal;
                        }
                    }
                }
            }
            else if (pBirdPortal->field_26_side == PortalSide::eRight_0)
            {
                if (mBaseAnimatedWithPhysicsGameObject_XPos - pBirdPortal->field_2C_xpos <= ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(numGridBlocks))
                {
                    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                    {
                        if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pBirdPortal->field_3C_YPos) < mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10) && pBirdPortal->VPortalClipper(1))
                        {
                            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
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

s16 BaseAliveGameObject::SetBaseAnimPaletteTint(TintEntry* pTintArray, EReliveLevelIds level_id, s32 resourceID)
{
    SetTint(pTintArray, level_id); // Actually bugged for inputs that never happen as it should return 0

    u8** pPalResource = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, resourceID, 1u, 0);

    if (!pPalResource)
    {
        return 0;
    }

    mBaseAnimatedWithPhysicsGameObject_Anim.LoadPal(pPalResource, 0);
    ResourceManager::FreeResource_49C330(pPalResource);
    return 1;
}

bool BaseAliveGameObject::Check_IsOnEndOfLine(s16 direction, s16 distance)
{
    // Check if distance grid blocks from current snapped X is still on the line or not, if not then we are
    // about to head off an edge.

    const FP gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    FP xLoc = {};
    if (direction == 1)
    {
        xLoc = -(gridSize * FP_FromInteger(distance));
    }
    else
    {
        xLoc = gridSize * FP_FromInteger(distance);
    }

    const FP xPosSnapped = FP_FromInteger(SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)));

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions->Raycast(
               xLoc + xPosSnapped,
               mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(4),
               xLoc + xPosSnapped,
               mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(4),
               &pLine,
               &hitX,
               &hitY,
               mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls)
        == 0;
}

BaseAliveGameObject* BaseAliveGameObject::GetStackedSlapTarget(s32 idToFind, ReliveTypes typeToFind, FP xpos, FP ypos)
{
    const s16 xposD = FP_GetExponent(xpos);
    const s16 yposD = FP_GetExponent(ypos);

    bool bFound = FALSE;
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

void BaseAliveGameObject::SetActiveCameraDelayedFromDir()
{
    if (sControlledCharacter_5C1B8C == this)
    {
        switch (Is_In_Current_Camera())
        {
            case CameraPos::eCamTop_1:
                if (mBaseAnimatedWithPhysicsGameObject_VelY < FP_FromInteger(0))
                {
                    gMap.SetActiveCameraDelayed(Map::MapDirections::eMapTop_2, this, -1);
                }
                break;

            case CameraPos::eCamBottom_2:
                if (mBaseAnimatedWithPhysicsGameObject_VelY > FP_FromInteger(0))
                {
                    gMap.SetActiveCameraDelayed(Map::MapDirections::eMapBottom_3, this, -1);
                }
                break;

            case CameraPos::eCamLeft_3:
                if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
                {
                    gMap.SetActiveCameraDelayed(Map::MapDirections::eMapLeft_0, this, -1);
                }
                break;

            case CameraPos::eCamRight_4:
                if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
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
    const s32 xposSnapped = SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos));
    if (snapToGrid)
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(xposSnapped);
    }

    PSX_Point currentCamXY = {};
    gMap.GetCurrentCamCoords(&currentCamXY);

    // Gone off the left edge of the current screen
    if (xposSnapped < currentCamXY.x && (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) || mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0)))
    {
        if (sControlledCharacter_5C1B8C == this && gMap.SetActiveCameraDelayed(Map::MapDirections::eMapLeft_0, this, -1))
        {
            mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mCurrentLevel;
            mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mCurrentPath;
            return 1;
        }
    }
    // Gone off the right edge of the current screen
    else if (xposSnapped > currentCamXY.x + 368 && (!(mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX)) || mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0)))
    {
        if (sControlledCharacter_5C1B8C == this && gMap.SetActiveCameraDelayed(Map::MapDirections::eMapRight_1, this, -1))
        {
            mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mCurrentLevel;
            mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mCurrentPath;
            return 1;
        }
    }
    return 0;
}

bool BaseAliveGameObject::WallHit(FP offY, FP offX)
{
    PathLine* pLine = nullptr;
    return sCollisions->Raycast(
               mBaseAnimatedWithPhysicsGameObject_XPos,
               mBaseAnimatedWithPhysicsGameObject_YPos - offY,
               mBaseAnimatedWithPhysicsGameObject_XPos + offX,
               mBaseAnimatedWithPhysicsGameObject_YPos - offY,
               &pLine,
               &offX,
               &offY,
               mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls)
        != 0;
}

bool BaseAliveGameObject::InAirCollision(PathLine** ppPathLine, FP* hitX, FP* hitY, FP velY)
{
    mBaseAnimatedWithPhysicsGameObject_VelY += mBaseAnimatedWithPhysicsGameObject_SpriteScale * velY;
    if (mBaseAnimatedWithPhysicsGameObject_VelY > (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20)))
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20);
    }

    const FP oldYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
    const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    auto bCollision = sCollisions->Raycast(
        oldXPos,
        oldYPos,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        ppPathLine,
        hitX,
        hitY,
        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls);

    if (bCollision)
    {
        return bCollision;
    }

    FP velYClamped = mBaseAnimatedWithPhysicsGameObject_VelY;
    if (Type() == ReliveTypes::eMudokon && velYClamped >= FP_FromInteger(0) && velYClamped < FP_FromInteger(4))
    {
        velYClamped = FP_FromInteger(4);
    }

    bCollision = sCollisions->Raycast(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_XPos + mBaseAnimatedWithPhysicsGameObject_VelX,
        velYClamped + mBaseAnimatedWithPhysicsGameObject_YPos,
        ppPathLine,
        hitX,
        hitY,
        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor);

    if (bCollision)
    {
        if ((*ppPathLine)->mLineType == eLineTypes::eDynamicCollision_32 ||
            (*ppPathLine)->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            return bCollision;
        }

        bCollision = FALSE;
        *ppPathLine = nullptr;
    }

    if (!IsActiveHero(this))
    {
        return bCollision;
    }

    const FP k10Scaled = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10);
    return sCollisions->Raycast(
        oldXPos,
        oldYPos - k10Scaled,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos - k10Scaled,
        ppPathLine,
        hitX,
        hitY,
        mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgWalls : kBgWalls);
}

BaseGameObject* BaseAliveGameObject::FindObjectOfType(ReliveTypes typeToFind, FP xpos, FP ypos)
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
            if (pCasted->mBaseAnimatedWithPhysicsGameObject_Scale == mBaseAnimatedWithPhysicsGameObject_Scale)
            {
                const PSX_RECT bRect = pCasted->VGetBoundingRect();
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
    const PSX_RECT bRect = pPlatform->VGetBoundingRect();

    if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) < bRect.x || FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) > bRect.w || FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) > bRect.h)
    {
        return 1;
    }

    pPlatform->VAdd(this);

    BaseAliveGameObject_PlatformId = pPlatform->field_8_object_id;
    return 1;
}
