#include "stdafx_ao.h"
#include "ThrowableArray.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Throwable.hpp"
#include "PathData.hpp"

namespace AO {

ThrowableArray* gThrowableArray = nullptr;

void LoadRockTypes(EReliveLevelIds levelNumber, u16 path)
{
    bool bDoLoadingLoop = false;
    const u8 throwableTypeIdx = Path_Get_Bly_Record_434650(levelNumber, path)->field_C_overlay_id & 0xFF;

    switch (gThrowableFromOverlayId[throwableTypeIdx])
    {
        case AOTypes::eGrenade_40:
            break;

        case AOTypes::eMeat_54:
            break;

        case AOTypes::eRock_70:

            break;

        default:
            break;
    }

    if (bDoLoadingLoop)
    {
        ResourceManager::LoadingLoop(false);
    }
}

ThrowableArray::ThrowableArray()
    : BaseGameObject(true, 0)
{
    SetUpdatable(false);
    mCount = 0;
    gThrowableArray = this;
    mUnknown1 = false;
    mUnknown2 = false;
    mUnknown3 = false;
}

ThrowableArray::~ThrowableArray()
{
    gThrowableArray = nullptr;
    if (mCount > 0)
    {
        Remove(mCount);
    }

}

void ThrowableArray::Remove(s16 count)
{
    mCount -= count;

    if (mCount > 0)
    {
        if (!mUnknown1)
        {
            return;
        }
    }
    else
    {
        if (mUnknown3)
        {
            mUnknown3 = false;
        }
    }

    if (mUnknown2)
    {
        switch (gThrowableFromOverlayId[gMap.mOverlayId])
        {
            case AOTypes::eGrenade_40:
                break;

            case AOTypes::eMeat_54:
                break;

            case AOTypes::eRock_70:
                break;

            default:
                break;
        }
        mUnknown2 = false;
    }
}

void ThrowableArray::VUpdate()
{
    if (mUnknown1)
    {
        LoadRockTypes(gMap.mCurrentLevel, gMap.mCurrentPath);
        Add(0);
        mUnknown1 = false;
        SetUpdatable(false);
    }
}

void ThrowableArray::VScreenChanged()
{
    if (gMap.mNextLevel != EReliveLevelIds::eMenu && gMap.mNextLevel != EReliveLevelIds::eCredits)
    {
        if (gThrowableFromOverlayId[gMap.mOverlayId] != gThrowableFromOverlayId[gMap.GetOverlayId()])
        {
            if (!mUnknown1)
            {
                SetUpdatable(true);
                mUnknown1 = true;
                Remove(0);
            }
        }
    }
    else
    {
        SetDead(true);
    }
}

void ThrowableArray::Add(s16 count)
{
    if (mCount == 0)
    {
        if (!mUnknown3)
        {
            mUnknown3 = true;
        }
    }

    if (mCount == 0 || mUnknown1)
    {
        if (!mUnknown2)
        {
            switch (gThrowableFromOverlayId[gMap.mOverlayId])
            {
                case AOTypes::eGrenade_40:
                    break;

                case AOTypes::eMeat_54:
                    break;

                case AOTypes::eRock_70:
                    break;

                default:
                    break;
            }

            mUnknown2 = true;
        }
    }

    mCount += count;
}

void ThrowableArray::VRender(PrimHeader** /*ppOt*/)
{
    // Nothing to do here
}
} // namespace AO
