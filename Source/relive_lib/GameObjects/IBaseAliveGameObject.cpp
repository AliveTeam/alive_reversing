#include "stdafx.h"
#include "IBaseAliveGameObject.hpp"
#include "../GameType.hpp"
#include "../ObjectIds.hpp"
#include "../Collisions.hpp"
#include "../Grid.hpp"
#include "../Events.hpp"

#include "../../AliveLibAO/PlatformBase.hpp"

DynamicArrayT<IBaseAliveGameObject>* gBaseAliveGameObjects = nullptr;

// TODO: Remove after abe.cpp merge
IBaseAliveGameObject* sControlledCharacter = nullptr;


IBaseAliveGameObject::~IBaseAliveGameObject()
{
    IBaseAliveGameObject* pLiftPoint = static_cast<IBaseAliveGameObject*>(sObjectIds.Find_Impl(BaseAliveGameObject_PlatformId));
    gBaseAliveGameObjects->Remove_Item(this);

    if (pLiftPoint)
    {
        // TODO: Only call VOnTrapDoorOpen when PlatformBase is merged
        if (GetGameType() == GameType::eAe)
        {
            pLiftPoint->VOnTrapDoorOpen();
        }
        else
        {
            static_cast<AO::PlatformBase*>(pLiftPoint)->VRemove(static_cast<AO::BaseAliveGameObject*>(this));
        }
        BaseAliveGameObject_PlatformId = Guid{};
    }
}

void IBaseAliveGameObject::VUnPosses()
{
    // Empty
}

void IBaseAliveGameObject::VPossessed()
{
    // Empty
}

bool IBaseAliveGameObject::VTakeDamage(BaseGameObject* /*pFrom*/)
{
    // Defaults to no damage.
    return false;
}

void IBaseAliveGameObject::VOnTlvCollision(relive::Path_TLV* /*pTlv*/)
{
    // Empty
}

void IBaseAliveGameObject::VOnTrapDoorOpen()
{
    // Empty
}

void IBaseAliveGameObject::OnCollisionWith(PSX_Point xy, PSX_Point wh, DynamicArrayT<BaseGameObject>* pObjList)
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

s16 IBaseAliveGameObject::SetBaseAnimPaletteTint(const TintEntry* pTintArray, EReliveLevelIds level_id, PalId palId)
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


void IBaseAliveGameObject::SetActiveCameraDelayedFromDir()
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

Scale IBaseAliveGameObject::PerGameScale()
{
    if (GetGameType() == GameType::eAo)
    {
        return GetSpriteScale() != FP_FromDouble(0.5) ? Scale::Fg : Scale::Bg;
    }
    return GetScale();
}

bool IBaseAliveGameObject::WallHit(FP offY, FP offX)
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

bool IBaseAliveGameObject::Check_IsOnEndOfLine(s16 direction, s16 distance)
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

bool IBaseAliveGameObject::IsInInvisibleZone(IBaseAliveGameObject* pObj)
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
