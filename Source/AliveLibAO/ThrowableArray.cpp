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
    field_12_flags &= ~7u;
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
        if (!(field_12_flags & 1))
        {
            return;
        }
    }
    else
    {
        if (field_12_flags & 4)
        {
            field_12_flags &= ~4;
        }
    }

    if (field_12_flags & 2)
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
        field_12_flags &= ~2;
    }
}

void ThrowableArray::VUpdate()
{
    if (field_12_flags & 1)
    {
        LoadRockTypes(gMap.mCurrentLevel, gMap.mCurrentPath);
        Add(0);
        field_12_flags &= ~1u;
        SetUpdatable(false);
    }
}

void ThrowableArray::VScreenChanged()
{
    if (gMap.mNextLevel != EReliveLevelIds::eMenu && gMap.mNextLevel != EReliveLevelIds::eCredits)
    {
        if (gThrowableFromOverlayId[gMap.mOverlayId] != gThrowableFromOverlayId[gMap.GetOverlayId()])
        {
            if (!(field_12_flags & 1))
            {
                SetUpdatable(true);
                field_12_flags |= 1;
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
        if (!(field_12_flags & 4))
        {
            field_12_flags |= 4;
        }
    }

    if (mCount == 0 || (field_12_flags & 1))
    {
        if (!(field_12_flags & 2))
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

            field_12_flags |= 2;
        }
    }

    mCount += count;
}

void ThrowableArray::VRender(PrimHeader** /*ppOt*/)
{
    // Nothing to do here
}
} // namespace AO
