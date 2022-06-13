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
    const FP old_x = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP old_y = mBaseAnimatedWithPhysicsGameObject_YPos;

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(mBaseAnimatedWithPhysicsGameObject_SpriteScale, screenXPos));

    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(0, mBaseAnimatedWithPhysicsGameObject_XPos, old_y, mBaseAnimatedWithPhysicsGameObject_XPos, old_y);

    if (mLiftPoint)
    {
        mLiftPoint->mBaseAnimatedWithPhysicsGameObject_XPos += (mBaseAnimatedWithPhysicsGameObject_XPos - old_x);

        BaseAliveGameObjectCollisionLine->field_0_rect.x += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - old_x);
        BaseAliveGameObjectCollisionLine->field_0_rect.w += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - old_x);
        BaseAliveGameObjectCollisionLine->field_0_rect.y = BaseAliveGameObjectCollisionLine->field_0_rect.y;
        BaseAliveGameObjectCollisionLine->field_0_rect.h = BaseAliveGameObjectCollisionLine->field_0_rect.h;
    }
    else
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (BaseAliveGameObjectCollisionLine)
        {
            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    old_y - FP_FromInteger(40),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    old_y + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    1 << BaseAliveGameObjectCollisionLine->field_8_type))
            {
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            }
            else
            {
                BaseAliveGameObjectPathTLV = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::StartController_28, 0);
                if (BaseAliveGameObjectPathTLV
                    && sCollisions->Raycast(
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->field_10_top_left.field_2_y),
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->field_14_bottom_right.field_2_y),
                        &pLine,
                        &hitX,
                        &hitY,
                        1 << BaseAliveGameObjectCollisionLine->field_8_type))
                {
                    BaseAliveGameObjectCollisionLine = pLine;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
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
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    BaseAliveGameObjectLastLineYPos - FP_FromInteger(40),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    BaseAliveGameObjectLastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos += hitY - BaseAliveGameObjectLastLineYPos;
            }
        }
    }
}

void BaseAliveGameObject::VSetYSpawn(s32 camWorldY, s16 bLeft)
{
    const FP oldx = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP oldy = mBaseAnimatedWithPhysicsGameObject_YPos;

    auto pFrameHeader = reinterpret_cast<FrameHeader*>(&(*mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock)[mBaseAnimatedWithPhysicsGameObject_Anim.Get_FrameHeader(-1)->field_0_frame_header_offset]);

    if (bLeft == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pFrameHeader->field_5_height + camWorldY + 356);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(camWorldY + 124);
    }

    BaseAliveGameObjectPathTLV = gMap.TLV_Get_At_446060(
        nullptr,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos);

    if (mLiftPoint)
    {
        mLiftPoint->mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_XPos - oldx;
        mLiftPoint->mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_YPos - oldy;

        BaseAliveGameObjectCollisionLine->field_0_rect.x += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - oldx);
        BaseAliveGameObjectCollisionLine->field_0_rect.w += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - oldx);
        BaseAliveGameObjectCollisionLine->field_0_rect.y += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - oldy);
        BaseAliveGameObjectCollisionLine->field_0_rect.h += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - oldy);
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
    if (BaseAliveGameObjectCollisionLine)
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        const s32 mask = mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 7 : 0x70;
        if (sCollisions->Raycast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(distance),
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(distance),
                &pLine,
                &hitX,
                &hitY,
                mask))
        {
            BaseAliveGameObjectCollisionLine = pLine;
            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            if (mLiftPoint)
            {
                if (pLine->field_8_type == eLineTypes ::eDynamicCollision_32 ||
                    pLine->field_8_type == eLineTypes::eBackgroundDynamicCollision_36)
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
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->field_10_top_left.field_2_y),
                        mBaseAnimatedWithPhysicsGameObject_XPos,
                        FP_FromInteger(BaseAliveGameObjectPathTLV->field_14_bottom_right.field_2_y),
                        &pLine,
                        &hitX,
                        &hitY,
                        mask))
                {
                    BaseAliveGameObjectCollisionLine = pLine;
                    mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
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
            if (pPortal->field_18_xpos >= mBaseAnimatedWithPhysicsGameObject_XPos)
            {
                if (pPortal->field_12_side == PortalSide::eLeft_1)
                {
                    if (pPortal->field_18_xpos - mBaseAnimatedWithPhysicsGameObject_XPos <= (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(distance)))
                    {
                        if (!mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pPortal->field_28_ypos) < mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10))
                            {
                                if (pPortal->VPortalClipper(1))
                                {
                                    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
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
                    if (mBaseAnimatedWithPhysicsGameObject_XPos - pPortal->field_18_xpos <= (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(distance)))
                    {
                        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
                        {
                            if (FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pPortal->field_28_ypos) < (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(10)))
                            {
                                if (pPortal->VPortalClipper(1))
                                {
                                    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
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

    mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(pIter->field_1_r, pIter->field_2_g, pIter->field_3_b);

    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, palId, 1, 0);
    if (!ppRes)
    {
        return 0;
    }
    mBaseAnimatedWithPhysicsGameObject_Anim.LoadPal(ppRes, 0);
    ResourceManager::FreeResource_455550(ppRes);
    return 1;
}

s16 BaseAliveGameObject::Check_IsOnEndOfLine_4021A0(s16 direction, s16 dist)
{
    // Check if distance grid blocks from current snapped X is still on the line or not, if not then we are
    // about to head off an edge.
    const FP gridSize = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

    FP xLoc = {};
    if (direction == 1)
    {
        xLoc = -(gridSize * FP_FromInteger(dist));
    }
    else
    {
        xLoc = gridSize * FP_FromInteger(dist);
    }

    const s16 xposRounded = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) & 1023;
    const FP xPosSnapped = FP_FromInteger((FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) & 0xFC00) + SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, xposRounded));
    if (xposRounded < (240 + 16) || xposRounded > (640 - 16))
    {
        return 0;
    }

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
               mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 7 : 0x70)
        == 0;
}

void BaseAliveGameObject::VOnPathTransition_401470(s16 camWorldX, s32 camWorldY, CameraPos direction)
{
    const FP oldx = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP oldy = mBaseAnimatedWithPhysicsGameObject_YPos;

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
            const s32 frameH = reinterpret_cast<FrameHeader*>((*mBaseAnimatedWithPhysicsGameObject_Anim.field_20_ppBlock)[mBaseAnimatedWithPhysicsGameObject_Anim.Get_FrameHeader(-1)->field_0_frame_header_offset])->field_5_height;

            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camWorldX + (FP_GetExponent(oldx) % 1024));
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(frameH + camWorldY + 356);
            break;
        }

        case CameraPos::eCamBottom_2:
            xpos = camWorldX + 206;
            width = camWorldX + 674;
            ypos = camWorldY + 310;
            height = camWorldY + 410;
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camWorldX + (FP_GetExponent(oldx) % 1024));
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(camWorldY + 124);
            break;

        case CameraPos::eCamLeft_3:
            width = camWorldX + 674;
            xpos = camWorldX + 524;
            ypos = camWorldY + 70;
            height = camWorldY + 410;
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(mBaseAnimatedWithPhysicsGameObject_SpriteScale, MaxGridBlocks_41FA10(mBaseAnimatedWithPhysicsGameObject_SpriteScale) - 1));
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
            break;

        case CameraPos::eCamRight_4:
            width = camWorldX + 356;
            xpos = camWorldX + 206;
            ypos = camWorldY + 70;
            height = camWorldY + 410;
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos(mBaseAnimatedWithPhysicsGameObject_SpriteScale, 1));
            mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
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
        mBaseAnimatedWithPhysicsGameObject_VelX = -mBaseAnimatedWithPhysicsGameObject_VelX;
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Toggle(AnimFlags::eBit5_FlipX);
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

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((BaseAliveGameObjectPathTLV->field_14_bottom_right.field_0_x + BaseAliveGameObjectPathTLV->field_10_top_left.field_0_x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(BaseAliveGameObjectPathTLV->field_10_top_left.field_2_y);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camLoc.field_0_x + SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(camLoc.field_0_x))));

    if (mLiftPoint)
    {
        // Move lift point into the new path
        const FP rect_left = mBaseAnimatedWithPhysicsGameObject_XPos - oldx;
        const FP rect_right = mBaseAnimatedWithPhysicsGameObject_YPos - oldy;

        mLiftPoint->mBaseAnimatedWithPhysicsGameObject_XPos += rect_left;
        mLiftPoint->mBaseAnimatedWithPhysicsGameObject_YPos += rect_right;

        BaseAliveGameObjectCollisionLine->field_0_rect.x += FP_GetExponent(rect_left);
        BaseAliveGameObjectCollisionLine->field_0_rect.w += FP_GetExponent(rect_left);
        BaseAliveGameObjectCollisionLine->field_0_rect.y += FP_GetExponent(rect_right);
        BaseAliveGameObjectCollisionLine->field_0_rect.h += FP_GetExponent(rect_right);
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
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(40),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                BaseAliveGameObjectCollisionLine = pLine;
                mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            }
            else
            {
                BaseAliveGameObjectCollisionLine = nullptr;
            }
        }

        // If we still didn't get a line then look for a start controller
        if (!BaseAliveGameObjectCollisionLine)
        {
            if (BaseAliveGameObjectPathTLV->field_4_type == TlvTypes::StartController_28)
            {
                BaseAliveGameObjectLastLineYPos += mBaseAnimatedWithPhysicsGameObject_YPos - oldy;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions->Raycast(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    BaseAliveGameObjectLastLineYPos - FP_FromInteger(40),
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    BaseAliveGameObjectLastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                mBaseAnimatedWithPhysicsGameObject_YPos += hitY - BaseAliveGameObjectLastLineYPos;
            }
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1) && mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale == FP_FromDouble(0.5))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(2));
        return;
    }

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5) && mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(0.5));
        return;
    }
}


s16 BaseAliveGameObject::MapFollowMe_401D30(s16 snapToGrid)
{
    PSX_Point camCoords = {};
    gMap.GetCurrentCamCoords(&camCoords);

    // Are we "in" the current camera X bounds?
    if (mBaseAnimatedWithPhysicsGameObject_LvlNumber == gMap.mCurrentLevel && mBaseAnimatedWithPhysicsGameObject_PathNumber == gMap.mCurrentPath && mBaseAnimatedWithPhysicsGameObject_XPos > FP_FromInteger(camCoords.field_0_x) && mBaseAnimatedWithPhysicsGameObject_XPos < FP_FromInteger(camCoords.field_0_x + 1024))
    {
        // Snapped XPos in camera space
        const s32 snappedXLocalCoords = SnapToXGrid(mBaseAnimatedWithPhysicsGameObject_SpriteScale, FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(camCoords.field_0_x)));

        // In the left camera void and moving left?
        if (snappedXLocalCoords < 256 && mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            if (sControlledCharacter_50767C == this && gMap.SetActiveCameraDelayed(Map::MapDirections::eMapLeft_0, this, -1))
            {
                mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mCurrentPath;
                mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mCurrentLevel;
                return 1;
            }
            else
            {
                const s32 x_i = abs(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos));
                const s32 camXIndex = x_i % 1024;
                if (x_i > 1024)
                {
                    UsePathTransScale_4020D0();

                    // Put at the right side of the camera to the left
                    const s32 cam1GridBeforeRight = XGrid_Index_To_XPos(mBaseAnimatedWithPhysicsGameObject_SpriteScale, (MaxGridBlocks_41FA10(mBaseAnimatedWithPhysicsGameObject_SpriteScale) - 1));
                    const s32 camRightEdge = x_i - camXIndex - 1024;
                    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camRightEdge) + FP_FromInteger(cam1GridBeforeRight) + ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                    VCheckCollisionLineStillValid(40);
                }
            }
        }
        // In the right camera void and moving right?
        else if (snappedXLocalCoords > 624 && mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0))
        {
            // Go to the right camera in under player control
            if (sControlledCharacter_50767C == this && gMap.SetActiveCameraDelayed(Map::MapDirections::eMapRight_1, this, -1))
            {
                mBaseAnimatedWithPhysicsGameObject_PathNumber = gMap.mCurrentPath;
                mBaseAnimatedWithPhysicsGameObject_LvlNumber = gMap.mCurrentLevel;
                return 1;
            }
            else
            {
                const s32 x_i = abs(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos));
                const s32 camXIndex = x_i % 1024;

                gMap.Get_map_size(&camCoords);
                if (x_i < (camCoords.field_0_x - 1024))
                {
                    UsePathTransScale_4020D0();

                    // Put at the left side of the camera to the right
                    const s32 cam1GridAfterLeft = XGrid_Index_To_XPos(mBaseAnimatedWithPhysicsGameObject_SpriteScale, 1);
                    const s32 camLeftEdge = x_i - camXIndex + 1024;
                    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camLeftEdge) + FP_FromInteger(cam1GridAfterLeft) - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                    VCheckCollisionLineStillValid(40);
                }
            }
        }
        else if (snapToGrid)
        {
            // Not in the voids of the camera, just snap to the x grid
            mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(snappedXLocalCoords + camCoords.field_0_x);
        }
        return 0;
    }
    else
    {
        const s32 x_i = abs(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos));
        const s32 camXIndex = x_i % 1024;

        // In the left camera void and moving left?
        if (camXIndex < 256 && mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0))
        {
            if (x_i > 1024)
            {
                UsePathTransScale_4020D0();

                const s32 camRightGrid = XGrid_Index_To_XPos(mBaseAnimatedWithPhysicsGameObject_SpriteScale, MaxGridBlocks_41FA10(mBaseAnimatedWithPhysicsGameObject_SpriteScale));
                const s32 camRightEdge = x_i - camXIndex - 1024;
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camRightEdge) + FP_FromInteger(camRightGrid) + ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                VCheckCollisionLineStillValid(40);
            }
        }
        // In the right camera void and moving right?
        else if (camXIndex > 624 && mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0)) // Never hit as velx is < 0
        {
            gMap.Get_map_size(&camCoords);
            if (x_i < (camCoords.field_0_x - 1024))
            {
                UsePathTransScale_4020D0();

                const s32 camLeftGrid = XGrid_Index_To_XPos(mBaseAnimatedWithPhysicsGameObject_SpriteScale, 0);
                const s32 camLeftEdge = x_i - camXIndex + 1024;
                mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(camLeftEdge) + FP_FromInteger(camLeftGrid) - ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);

                VCheckCollisionLineStillValid(40);
            }
        }
        return 0;
    }
}

void BaseAliveGameObject::SetActiveCameraDelayedFromDir_401C90()
{
    if (sControlledCharacter_50767C == this)
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

            default:
                return;
        }
    }
}



s16 BaseAliveGameObject::WallHit_401930(FP offY, FP offX)
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
               mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 6 : 0x60)
        != 0;
}

s16 BaseAliveGameObject::InAirCollision_4019C0(PathLine** ppLine, FP* hitX, FP* hitY, FP vely)
{
    mBaseAnimatedWithPhysicsGameObject_VelY += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * vely);

    if (mBaseAnimatedWithPhysicsGameObject_VelY > (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20)))
    {
        mBaseAnimatedWithPhysicsGameObject_VelY = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(20));
    }

    const FP old_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP old_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    return sCollisions->Raycast(
        old_xpos,
        old_ypos,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        ppLine,
        hitX,
        hitY,
        mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 7 : 0x70);
}

void BaseAliveGameObject::OnResourceLoaded_4019A0(BaseAliveGameObject* ppRes)
{
    ppRes->field_104_pending_resource_count--;
}

void BaseAliveGameObject::UsePathTransScale_4020D0()
{
    auto pPathTrans = static_cast<Path_Change*>(gMap.TLV_Get_At_446260(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos),
        TlvTypes::PathTransition_1));

    if (pPathTrans)
    {
        if (pPathTrans->field_22_next_path_scale == Scale_short::eHalf_1)
        {
            if (mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5))
            {
                mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
                mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
                mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(0.5));
            }
        }
        else if (pPathTrans->field_22_next_path_scale == Scale_short::eFull_0)
        {
            if (mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1))
            {
                mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
                mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
                mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(2));
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
    if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) < rect.x || FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) > rect.w || FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) > rect.h)
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
