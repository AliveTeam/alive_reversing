#include "stdafx_ao.h"
#include "Function.hpp"
#include "Throwable.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Grenade.hpp"
#include "Rock.hpp"
#include "Meat.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "PlatformBase.hpp"

namespace AO {

// Overlay to throwable type table ??
const AOTypes word_4CF158[54] = {
    AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eGrenade_40, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eRock_70,
    AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0,
    AOTypes::eMeat_54, AOTypes::eNone_0, AOTypes::eMeat_54, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eRock_70, AOTypes::eRock_70, AOTypes::eNone_0,
    AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eRock_70, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eGrenade_40,
    AOTypes::eNone_0, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eGrenade_40, AOTypes::eNone_0, AOTypes::eGrenade_40,
    AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eNone_0, AOTypes::eNone_0};

BaseThrowable* Make_Throwable(FP xpos, FP ypos, s16 count)
{
    switch (word_4CF158[gMap.mOverlayId])
    {
        case AOTypes::eGrenade_40:
        {
            auto pGrenade = relive_new Grenade(xpos, ypos, count);
            if (pGrenade)
            {
                return pGrenade;
            }
            break;
        }

        case AOTypes::eMeat_54:
        {
            auto pMeat = relive_new Meat(xpos, ypos, count);
            if (pMeat)
            {
                return pMeat;
            }
            break;
        }

        case AOTypes::eRock_70:
        {
            auto pRock = relive_new Rock(xpos, ypos, count);
            if (pRock)
            {
                return pRock;
            }
            break;
        }

        default:
            break;
    }
    return nullptr;
}


void BaseThrowable::VToDead()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    field_10E_bDead = TRUE;
}

s16 BaseThrowable::VGetCount()
{
    return field_10C_count;
}

void BaseThrowable::VOnPickUpOrSlapped()
{
    VToDead();
}

void BaseThrowable::BaseAddToPlatform()
{
    const FP scale = field_BC_sprite_scale - FP_FromDouble(0.5);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->RayCast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - FP_FromInteger(20),
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(20),
            &pLine,
            &hitX,
            &hitY,
            scale != FP_FromInteger(0) ? 7 : 0x70))
    {
        if (pLine->field_8_type == eLineTypes::eUnknown_32 ||
            pLine->field_8_type == eLineTypes::eUnknown_36)
        {
            for (s32 i = 0; i < ObjListPlatforms_50766C->Size(); i++)
            {
                BaseGameObject* pObjIter = ObjListPlatforms_50766C->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eLiftPoint || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eTrapDoor)
                {
                    auto pPlatformBase = static_cast<PlatformBase*>(pObjIter);

                    PSX_RECT objRect = {};
                    pPlatformBase->VGetBoundingRect(&objRect, 1);

                    if (FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) > objRect.x && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) < objRect.w && FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) < objRect.h)
                    {
                        if (mLiftPoint)
                        {
                            if (mLiftPoint == pPlatformBase)
                            {
                                return;
                            }
                            mLiftPoint->VRemove(this);
                            mLiftPoint->mBaseGameObjectRefCount--;
                            mLiftPoint = nullptr;
                        }

                        mLiftPoint = pPlatformBase;
                        mLiftPoint->VAdd(this);
                        mLiftPoint->mBaseGameObjectRefCount++;
                        return;
                    }
                }
            }
        }
    }
}

} // namespace AO
