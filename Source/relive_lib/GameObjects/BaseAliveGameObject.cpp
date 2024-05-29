#include "stdafx.h"
#include "BaseAliveGameObject.hpp"
#include "../GameType.hpp"
#include "../ObjectIds.hpp"
#include "../Collisions.hpp"
#include "../Grid.hpp"
#include "../Events.hpp"

#include "PlatformBase.hpp"
#include "IBirdPortal.hpp"

#include "../../AliveLibAO/Map.hpp"
#include "../../AliveLibAE/Map.hpp"
#include "../FatalError.hpp"
#include "../../AliveLibAE/Abe.hpp"

DynamicArrayT<BaseAliveGameObject>* gBaseAliveGameObjects = nullptr;

// TODO: Remove after abe.cpp merge
BaseAliveGameObject* sControlledCharacter = nullptr;


BaseAliveGameObject::~BaseAliveGameObject()
{
    BaseAliveGameObject* pLiftPoint = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    gBaseAliveGameObjects->Remove_Item(this);

    if (pLiftPoint)
    {
        pLiftPoint->VOnTrapDoorOpen();
        BaseAliveGameObject_PlatformId = Guid{};
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

bool BaseAliveGameObject::VTakeDamage(BaseGameObject* /*pFrom*/)
{
    // Defaults to no damage.
    return false;
}

void BaseAliveGameObject::VOnTlvCollision(relive::Path_TLV* /*pTlv*/)
{
    // Empty
}

void BaseAliveGameObject::VOnTrapDoorOpen()
{
    // Empty
}

void BaseAliveGameObject::OnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList)
{
    if (pObjList)
    {
        for (s32 i = 0; i < pObjList->Size(); i++)
        {
            BaseGameObject* pObjIter = pObjList->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            if (pObjIter->GetIsBaseAnimatedWithPhysicsObj())
            {
                if (pObjIter->GetDrawable())
                {
                    BaseAnimatedWithPhysicsGameObject* pObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObjIter);
                    const PSX_RECT bRect = pObj->VGetBoundingRect();
                    if (xy.x <= bRect.w && wh.x >= bRect.x && wh.y >= bRect.y && xy.y <= bRect.h)
                    {
                        // NOTE: AO ignored scale here
                        if (GetGameType() == GameType::eAo || (GetGameType() == GameType::eAe && GetScale() == pObj->GetScale()))
                        {
                            if (!VOnPlatformIntersection(pObj))
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

s16 BaseAliveGameObject::SetBaseAnimPaletteTint(const TintEntry* pTintArray, EReliveLevelIds level_id, PalId palId)
{
    // NOTE: in AO's SetBaseAnimPaletteTint() function we don't want to set the RGB values
    // when we hit the end of the tint array
    if (GetGameType() == GameType::eAe)
    {
        SetTint(pTintArray, level_id);
    }
    else
    {
        const TintEntry* pIter = pTintArray;
        while (pIter->field_0_level != level_id)
        {
            if (pIter->field_0_level == EReliveLevelIds::eNone) // End of entries
            {
                return 0;
            }
            pIter++;
        }

        mRGB.SetRGB(pIter->field_1_r, pIter->field_2_g, pIter->field_3_b);
    }

    if (palId != PalId::Default)
    {
        PalResource res = ResourceManagerWrapper::LoadPal(palId);
        GetAnimation().LoadPal(res);
    }
    return 1;
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
                    GetMap().SetActiveCameraDelayed(MapDirections::eMapTop_2, this, -1);
                }
                break;

            case CameraPos::eCamBottom_2:
                if (mVelY > FP_FromInteger(0))
                {
                    GetMap().SetActiveCameraDelayed(MapDirections::eMapBottom_3, this, -1);
                }
                break;

            case CameraPos::eCamLeft_3:
                if (mVelX < FP_FromInteger(0))
                {
                    GetMap().SetActiveCameraDelayed(MapDirections::eMapLeft_0, this, -1);
                }
                break;

            case CameraPos::eCamRight_4:
                if (mVelX > FP_FromInteger(0))
                {
                    GetMap().SetActiveCameraDelayed(MapDirections::eMapRight_1, this, -1);
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

Scale BaseAliveGameObject::PerGameScale()
{
    if (GetGameType() == GameType::eAo)
    {
        return GetSpriteScale() != FP_FromDouble(0.5) ? Scale::Fg : Scale::Bg;
    }
    return GetScale();
}

bool BaseAliveGameObject::WallHit(FP offY, FP offX)
{
    PathLine* pLine = nullptr;
    return gCollisions->Raycast(
               mXPos,
               mYPos - offY,
               mXPos + offX,
               mYPos - offY,
               &pLine,
               &offX,
               &offY,
               PerGameScale() == Scale::Fg ? kFgWalls : kBgWalls)
        != 0;
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

    FP xPosSnapped = {};
    CollisionMask usedMask;
    if (GetGameType() == GameType::eAo)
    {
        const s16 xposRoundedAO = FP_GetExponent(mXPos) & 1023;
        const FP xPosSnappedAO = FP_FromInteger((FP_GetExponent(mXPos) & 0xFC00) + SnapToXGrid_AO(GetSpriteScale(), xposRoundedAO));
        if (xposRoundedAO < (240 + 16) || xposRoundedAO > (640 - 16))
        {
            return 0;
        }
        xPosSnapped = xPosSnappedAO;
        usedMask = PerGameScale() == Scale::Fg ? kFgWallsOrFloor : kBgWallsOrFloor;
    }
    else
    {
        xPosSnapped = FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos)));
        usedMask = PerGameScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return gCollisions->Raycast(
               xLoc + xPosSnapped,
               mYPos - FP_FromInteger(4),
               xLoc + xPosSnapped,
               mYPos + FP_FromInteger(4),
               &pLine,
               &hitX,
               &hitY,
               usedMask)
        == 0;
}

bool BaseAliveGameObject::IsInInvisibleZone(BaseAliveGameObject* pObj)
{
    /* TODO: Not used in AE but might be possible to activate in AO
    if (gAbeInvisibleCheat)
    {
        return true;
    }
    */

    if (EventGet(Event::kEventAbeOhm))
    {
        return false;
    }

    const PSX_RECT bRect = pObj->VGetBoundingRect();

    relive::Path_TLV* pTlv = GetMap().VTLV_Get_At_Of_Type(
        bRect.x,
        bRect.y,
        bRect.w,
        bRect.h,
        ReliveTypes::eInvisibleZone
    );

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
        pTlv = GetMap().TLV_Get_At(pTlv,
                                     FP_FromInteger(bRect.x),
                                     FP_FromInteger(bRect.y),
                                     FP_FromInteger(bRect.w),
                                     FP_FromInteger(bRect.h));
    }
    return false;
}

IBirdPortal* BaseAliveGameObject::VIntoBirdPortal(s16 numGridBlocks)
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
            auto pPortal = static_cast<IBirdPortal*>(pObjIter);
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
                                if (pPortal->ClipPortal(1))
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
                                if (pPortal->ClipPortal(1))
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

BaseAliveGameObject::BaseAliveGameObject(s16 resourceArraySize)
    : BaseAnimatedWithPhysicsGameObject(resourceArraySize)
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

    SetCanBeesChase(false);

    gBaseAliveGameObjects->Push_Back(this);

    SetIsBaseAliveGameObject(true);
}

// AO
void BaseAliveGameObject::VSetXSpawn(s16 camWorldX, s32 screenXPos)
{
    BaseAliveGameObject* pLiftPoint = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));

    const FP old_x = mXPos;
    const FP old_y = mYPos;

    mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos_AO(GetSpriteScale(), screenXPos));

    BaseAliveGameObjectPathTLV = GetMap().TLV_Get_At(0, mXPos, old_y, mXPos, old_y);

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
            if (gCollisions->Raycast(
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
                BaseAliveGameObjectPathTLV = GetMap().TLV_First_Of_Type_In_Camera(ReliveTypes::eStartController, 0);
                if (BaseAliveGameObjectPathTLV
                    && gCollisions->Raycast(
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
            if (gCollisions->Raycast(
                    mXPos,
                    BaseAliveGameObjectLastLineYPos - FP_FromInteger(40),
                    mXPos,
                    BaseAliveGameObjectLastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    PerGameScale() == Scale::Fg ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos += hitY - BaseAliveGameObjectLastLineYPos;
            }
        }
    }
}

// AO
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

    BaseAliveGameObjectPathTLV = GetMap().TLV_Get_At(
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

    CollisionMask mask;
    if (PerGameScale() == Scale::Fg)
    {
        mask = GetGameType() == GameType::eAe ? kFgFloorCeilingOrWalls : kFgWallsOrFloor;
    }
    else
    {
        mask = GetGameType() == GameType::eAe ? kBgFloorCeilingOrWalls : kBgWallsOrFloor;
    }

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

        BaseAliveGameObjectPathTLV = GetMap().TLV_First_Of_Type_In_Camera(ReliveTypes::eStartController, 0);
        if (BaseAliveGameObjectPathTLV)
        {
            if (gCollisions->Raycast(
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

void BaseAliveGameObject::VOnPathTransition(s32 camWorldX, s32 camWorldY, CameraPos direction)
{
    if (GetGameType() == GameType::eAe)
    {
        OnPathTransitionAE(camWorldX, camWorldY, direction);
    }
    else
    {
        OnPathTransitionAO(camWorldX, camWorldY, direction);
    }
}

// AE
void BaseAliveGameObject::OnPathTransitionAE(s32 camWorldX, s32 camWorldY, CameraPos direction)
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

    Abe* pAbe = nullptr;
    if (Type() == ReliveTypes::eAbe)
    {
        pAbe = static_cast<Abe*>(this);
    }

    if (pAbe && GetMap().mCurrentLevel == EReliveLevelIds::eNecrum && GetMap().mCurrentPath == 2 && (pAbe->mCurrentMotion == eAbeMotions::Motion_23_RollLoop_453A90 || pAbe->mCurrentMotion == eAbeMotions::Motion_17_CrouchIdle_456BC0))
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
            BaseAliveGameObjectLastLineYPos += mYPos - oldY;
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
                mYPos += hitY - BaseAliveGameObjectLastLineYPos;
            }
            else
            {
                // Not set to nullptr in this case ??
            }
        }
    }

    if (GetSpriteScale() == FP_FromInteger(1) && GetAnimation().GetSpriteScale() == FP_FromDouble(0.5))
    {
        // From 0.5 to 1 scale, double velx
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

// AO
void BaseAliveGameObject::OnPathTransitionAO(s32 camWorldX, s32 camWorldY, CameraPos direction)
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
            mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos_AO(GetSpriteScale(), MaxGridBlocks(GetSpriteScale()) - 1));
            mYPos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
            break;

        case CameraPos::eCamRight_4:
            width = camWorldX + 356;
            xpos = camWorldX + 206;
            ypos = camWorldY + 70;
            height = camWorldY + 410;
            mXPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos_AO(GetSpriteScale(), 1));
            mYPos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
            break;

        default:
            break;
    }

    // Find the start controller at the position we will be at in the new map
    BaseAliveGameObjectPathTLV = GetMap().VTLV_Get_At_Of_Type(static_cast<s16>(xpos), static_cast<s16>(ypos), static_cast<s16>(width), static_cast<s16>(height), ReliveTypes::eStartController);

    if (!BaseAliveGameObjectPathTLV)
    {
        BaseAliveGameObjectPathTLV = GetMap().TLV_First_Of_Type_In_Camera(ReliveTypes::eStartController, 0);
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
    AO::gMap.GetCurrentCamCoords(&camLoc);

    mXPos = FP_FromInteger((BaseAliveGameObjectPathTLV->mBottomRightX + BaseAliveGameObjectPathTLV->mTopLeftX) / 2);
    mYPos = FP_FromInteger(BaseAliveGameObjectPathTLV->mTopLeftY);

    mXPos = FP_FromInteger(camLoc.x + SnapToXGrid_AO(GetSpriteScale(), FP_GetExponent(mXPos - FP_FromInteger(camLoc.x))));

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
            if (gCollisions->Raycast(
                    mXPos,
                    mYPos - FP_FromInteger(40),
                    mXPos,
                    mYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    PerGameScale() == Scale::Fg ? kFgWallsOrFloor : kBgWallsOrFloor))
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
            if (gCollisions->Raycast(
                    mXPos,
                    BaseAliveGameObjectLastLineYPos - FP_FromInteger(40),
                    mXPos,
                    BaseAliveGameObjectLastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    PerGameScale() == Scale::Fg ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                mYPos += hitY - BaseAliveGameObjectLastLineYPos;
            }
        }
    }

    if (GetSpriteScale() == FP_FromInteger(1) && GetAnimation().GetSpriteScale() == FP_FromDouble(0.5))
    {
        // From 0.5 to 1 scale, double velx
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
    if (GetGameType() == GameType::eAe)
    {
        return MapFollowMeAE(snapToGrid);
    }
    else
    {
        return MapFollowMeAO(snapToGrid);
    }
}

// AO
bool BaseAliveGameObject::MapFollowMeAO(bool snapToGrid)
{
    PSX_Point currentCamCoords = {};
    AO::gMap.GetCurrentCamCoords(&currentCamCoords);

    // Are we "in" the current camera X bounds?
    if (mCurrentLevel == GetMap().mCurrentLevel && mCurrentPath == GetMap().mCurrentPath && mXPos > FP_FromInteger(currentCamCoords.x) && mXPos < FP_FromInteger(currentCamCoords.x + 1024))
    {
        // Snapped XPos in camera space
        const s32 snappedXLocalCoords = SnapToXGrid_AO(GetSpriteScale(), FP_GetExponent(mXPos - FP_FromInteger(currentCamCoords.x)));

        // In the left camera void and moving left?
        if (snappedXLocalCoords < 256 && mVelX < FP_FromInteger(0))
        {
            if (sControlledCharacter == this && GetMap().SetActiveCameraDelayed(MapDirections::eMapLeft_0, this, -1))
            {
                mCurrentPath = GetMap().mCurrentPath;
                mCurrentLevel = GetMap().mCurrentLevel;
                return true;
            }
            else
            {
                const s32 x_i = abs(FP_GetExponent(mXPos));
                const s32 camXIndex = x_i % 1024;
                if (x_i > 1024)
                {
                    UsePathTransScale();

                    // Put at the right side of the camera to the left
                    const s32 cam1GridBeforeRight = XGrid_Index_To_XPos_AO(GetSpriteScale(), (MaxGridBlocks(GetSpriteScale()) - 1));
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
            if (sControlledCharacter == this && GetMap().SetActiveCameraDelayed(MapDirections::eMapRight_1, this, -1))
            {
                mCurrentLevel = GetMap().mCurrentLevel;
                mCurrentPath = GetMap().mCurrentPath;
                return true;
            }
            else
            {
                const s32 x_i = abs(FP_GetExponent(mXPos));
                const s32 camXIndex = x_i % 1024;

                AO::gMap.Get_map_size(&currentCamCoords);
                if (x_i < (currentCamCoords.x - 1024))
                {
                    UsePathTransScale();

                    // Put at the left side of the camera to the right
                    const s32 cam1GridAfterLeft = XGrid_Index_To_XPos_AO(GetSpriteScale(), 1);
                    const s32 camLeftEdge = x_i - camXIndex + 1024;
                    mXPos = FP_FromInteger(camLeftEdge) + FP_FromInteger(cam1GridAfterLeft) - ScaleToGridSize(GetSpriteScale());

                    VCheckCollisionLineStillValid(40);
                }
            }
        }
        else if (snapToGrid)
        {
            // Not in the voids of the camera, just snap to the x grid
            mXPos = FP_FromInteger(snappedXLocalCoords + currentCamCoords.x);
        }
        return false;
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
                UsePathTransScale();

                const s32 camRightGrid = XGrid_Index_To_XPos_AO(GetSpriteScale(), MaxGridBlocks(GetSpriteScale()));
                const s32 camRightEdge = x_i - camXIndex - 1024;
                mXPos = FP_FromInteger(camRightEdge) + FP_FromInteger(camRightGrid) + ScaleToGridSize(GetSpriteScale());

                VCheckCollisionLineStillValid(40);
            }
        }
        // In the right camera void and moving right?
        else if (camXIndex > 624 && mVelX > FP_FromInteger(0)) // Never hit as velx is < 0
        {
            AO::gMap.Get_map_size(&currentCamCoords);
            if (x_i < (currentCamCoords.x - 1024))
            {
                UsePathTransScale();

                const s32 camLeftGrid = XGrid_Index_To_XPos_AO(GetSpriteScale(), 0);
                const s32 camLeftEdge = x_i - camXIndex + 1024;
                mXPos = FP_FromInteger(camLeftEdge) + FP_FromInteger(camLeftGrid) - ScaleToGridSize(GetSpriteScale());

                VCheckCollisionLineStillValid(40);
            }
        }
        return false;
    }
}

// AE
bool BaseAliveGameObject::MapFollowMeAE(bool snapToGrid)
{
    PSX_Point currentCamCoords = {};
    gMap.GetCurrentCamCoords(&currentCamCoords);

    const s32 xposSnapped = SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos));
    if (snapToGrid)
    {
        mXPos = FP_FromInteger(xposSnapped);
    }

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

    CollisionMask mask;
    if (PerGameScale() == Scale::Fg)
    {
        mask = GetGameType() == GameType::eAe ? kFgFloorCeilingOrWalls : kFgWallsOrFloor;
    }
    else
    {
        mask = GetGameType() == GameType::eAe ? kBgFloorCeilingOrWalls : kBgWallsOrFloor;
    }

    auto bCollision = gCollisions->Raycast(
        old_xpos,
        old_ypos,
        mXPos,
        mYPos,
        ppLine,
        hitX,
        hitY,
        mask);

    // AO doesn't clamp mudokon y velocity
    if (bCollision || GetGameType() == GameType::eAo)
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

    return true;
}

// AO
void BaseAliveGameObject::UsePathTransScale()
{
    auto pPathTrans = static_cast<relive::Path_PathTransition*>(GetMap().VTLV_Get_At_Of_Type(
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        FP_GetExponent(mXPos),
        FP_GetExponent(mYPos),
        ReliveTypes::ePathTransition));

    if (pPathTrans)
    {
        if (pPathTrans->mNextPathScale == relive::reliveScale::eHalf)
        {
            if (PerGameScale() == Scale::Fg)
            {
                SetSpriteScale(FP_FromDouble(0.5));
                SetScale(Scale::Bg);
                mVelX = (mVelX * FP_FromDouble(0.5));
            }
        }
        else if (pPathTrans->mNextPathScale == relive::reliveScale::eFull)
        {
            if (PerGameScale() == Scale::Bg)
            {
                SetSpriteScale(FP_FromInteger(1));
                SetScale(Scale::Fg);
                mVelX = (mVelX * FP_FromInteger(2));
            }
        }
    }
}

// AE
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
