#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Throwable.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Grenade.hpp"
#include "Rock.hpp"
#include "Meat.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "PlatformBase.hpp"

namespace AO {

const AOTypes gThrowableFromOverlayId[54] = {
    AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eGrenade_40, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eRock_70,
    AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0,
    AOTypes::eMeat_54, AOTypes::eNone_0, AOTypes::eMeat_54, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eRock_70, AOTypes::eRock_70, AOTypes::eNone_0,
    AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eGrenade_40,
    AOTypes::eNone_0, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eNone_0, AOTypes::eGrenade_40,
    AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eNone_0};

BaseThrowable* Make_Throwable(FP xpos, FP ypos, s16 count)
{
    switch (gThrowableFromOverlayId[gMap.mOverlayId])
    {
        case AOTypes::eGrenade_40:
            return relive_new Grenade(xpos, ypos, count);
        case AOTypes::eMeat_54:
            return relive_new Meat(xpos, ypos, count);
        case AOTypes::eRock_70:
            return relive_new Rock(xpos, ypos, count);
        default:
            return nullptr;
    }
}


void BaseThrowable::BaseAddToPlatform()
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (gCollisions->Raycast(
            mXPos,
            mYPos - FP_FromInteger(20),
            mXPos,
            mYPos + FP_FromInteger(20),
            &pLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgWallsOrFloor : kBgWallsOrFloor))
    {
        if (pLine->mLineType == eLineTypes::eDynamicCollision_32 ||
            pLine->mLineType == eLineTypes::eBackgroundDynamicCollision_36)
        {
            for (s32 idx = 0; idx < gPlatformsArray->Size(); idx++)
            {
                BaseGameObject* pObjIter = gPlatformsArray->ItemAt(idx);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->Type() == ReliveTypes::eLiftPoint || pObjIter->Type() == ReliveTypes::eTrapDoor)
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

void BaseThrowable::VToDead()
{
    SetDead(true);
    mBaseThrowableDead = true;
}

void BaseThrowable::VOnAbeInteraction()
{
    VToDead();
}

s16 BaseThrowable::VGetCount()
{
    return mBaseThrowableCount;
}

} // namespace AO
