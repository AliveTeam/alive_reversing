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
    const u8 throwableTypeIdx = Path_Get_Bly_Record(levelNumber, path)->mOverlayId & 0xFF;

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
    mThrowableTypeChanged = false;
    mNewThrowableTypeLoaded = false;
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
        if (!mThrowableTypeChanged)
        {
            return;
        }
    }

    if (mNewThrowableTypeLoaded)
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
        mNewThrowableTypeLoaded = false;
    }
}

void ThrowableArray::VUpdate()
{
    if (mThrowableTypeChanged)
    {
        LoadRockTypes(gMap.mCurrentLevel, gMap.mCurrentPath);
        Add(0);
        mThrowableTypeChanged = false;
        SetUpdatable(false);
    }
}

void ThrowableArray::VScreenChanged()
{
    if (gMap.mNextLevel != EReliveLevelIds::eMenu && gMap.mNextLevel != EReliveLevelIds::eCredits)
    {
        if (gThrowableFromOverlayId[gMap.mOverlayId] != gThrowableFromOverlayId[gMap.GetOverlayId()])
        {
            if (!mThrowableTypeChanged)
            {
                SetUpdatable(true);
                mThrowableTypeChanged = true;
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
    if (mCount == 0 || mThrowableTypeChanged)
    {
        if (!mNewThrowableTypeLoaded)
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

            mNewThrowableTypeLoaded = true;
        }
    }

    mCount += count;
}

void ThrowableArray::VRender(PrimHeader** /*ppOt*/)
{
    // Nothing to do here
}
} // namespace AO
