#include "stdafx.h"
#include "Throwable.hpp"
#include "Function.hpp"
#include "Bone.hpp"
#include "Grenade.hpp"
#include "Rock.hpp"
#include "Meat.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "PlatformBase.hpp"
#include "Game.hpp"
#include "Map.hpp"

bool gInfiniteThrowables = false;

// Overlay to throwable type table ??
const AETypes gThrowableFromOverlayId[252] = {
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eRock_105, AETypes::eRock_105, AETypes::eRock_105, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eRock_105, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eRock_105, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eRock_105, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eRock_105, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eRock_105, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eMeat_84, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eMeat_84, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eMeat_84, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eRock_105, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eBone_11, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eGrenade_65, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0, AETypes::eNone_0,
    AETypes::eNone_0, AETypes::eNone_0};

BaseThrowable* Make_Throwable(FP xpos, FP ypos, s16 count)
{
    switch (gThrowableFromOverlayId[gMap.mOverlayId])
    {
        case AETypes::eBone_11:
            return relive_new Bone(xpos, ypos, count);
        case AETypes::eMetal_24:
            return relive_new Grenade(xpos, ypos, count, false, nullptr);
        case AETypes::eGrenade_65:
            return relive_new Grenade(xpos, ypos, count, false, nullptr);
        case AETypes::eMeat_84:
            return relive_new Meat(xpos, ypos, count);
        case AETypes::eRock_105:
            return relive_new Rock(xpos, ypos, count);
        default:
            return nullptr;
    }
}

BaseThrowable::BaseThrowable(s16 resourceArraySize)
    : BaseAliveGameObject(resourceArraySize)
{

}

void BaseThrowable::BaseAddToPlatform(BaseThrowable::FnTypeMatcher cb)
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions->Raycast(
            mXPos,
            mYPos - FP_FromInteger(20),
            mXPos,
            mYPos + FP_FromInteger(20),
            &pLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls)) // todo: mouze check me 0x10F changed to 0x0F
    {
        if (pLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            if (gPlatformsArray)
            {
                for (s32 idx = 0; idx < gPlatformsArray->Size(); idx++)
                {
                    BaseGameObject* pObjIter = gPlatformsArray->ItemAt(idx);
                    if (!pObjIter)
                    {
                        break;
                    }

                    if (cb(pObjIter->Type()))
                    {
                        auto pPlatformBase = static_cast<PlatformBase*>(pObjIter);

                        const PSX_RECT bRect = pPlatformBase->VGetBoundingRect();

                        if (FP_GetExponent(mXPos) > bRect.x && FP_GetExponent(mXPos) < bRect.w && FP_GetExponent(mYPos) < bRect.h)
                        {
                            pPlatformBase->VAdd(this);
                            BaseAliveGameObject_PlatformId = pPlatformBase->mBaseGameObjectId;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void BaseThrowable::VToDead()
{
    SetDead(true);
    mBaseThrowableDead = true;
}

void BaseThrowable::VOnPickUpOrSlapped()
{
    VToDead();
}

s16 BaseThrowable::VGetCount()
{
    return mBaseThrowableCount;
}
