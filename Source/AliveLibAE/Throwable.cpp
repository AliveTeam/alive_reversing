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

u16 gInfiniteThrowables = 0;

// Overlay to throwable type table ??
const AETypes throwable_types_55FAFC[252] = {
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

BaseThrowable* Make_Throwable_49AF30(FP xpos, FP ypos, s16 count)
{
    switch (throwable_types_55FAFC[gMap.mOverlayId])
    {
        case AETypes::eBone_11:
            return relive_new Bone(xpos, ypos, count);
        case AETypes::eMetal_24:
            return relive_new Grenade(xpos, ypos, count, false, 1, nullptr);
        case AETypes::eGrenade_65:
            return relive_new Grenade(xpos, ypos, count, false, 0, nullptr);
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
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    mBaseThrowableDead = TRUE;
}

void BaseThrowable::VOnPickUpOrSlapped()
{
    VToDead();
}

s16 BaseThrowable::VGetCount()
{
    return mBaseThrowableCount;
}
