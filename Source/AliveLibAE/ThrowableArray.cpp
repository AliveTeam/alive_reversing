#include "stdafx.h"
#include "ThrowableArray.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "Throwable.hpp"
#include "ResourceManager.hpp"
#include "QuikSave.hpp"

ThrowableArray* gThrowableArray = nullptr;

void FreeResourceArray(DynamicArrayT<u8*>* pArray)
{
    while (pArray->Size())
    {
        auto item = pArray->ItemAt(0);
        pArray->Remove_Item(item);
    }
}

void LoadRockTypes(EReliveLevelIds levelNumber, u16 pathNumber)
{
    bool bDoLoadingLoop = false;
    const u8 throwableTypeIdx = Path_Get_Bly_Record(levelNumber, pathNumber)->mOverlayId & 0xFF;

    switch (gThrowableFromOverlayId[throwableTypeIdx])
    {
        case AETypes::eBone_11:
            break;

        case AETypes::eMetal_24:

            break;

        case AETypes::eGrenade_65:

            break;

        case AETypes::eMeat_84:
            break;

        case AETypes::eRock_105:
            break;

        default:
            break;
    }

    if (bDoLoadingLoop)
    {
        ResourceManagerWrapper::LoadingLoop(false);
    }
}

ThrowableArray::ThrowableArray()
    : BaseGameObject(true, 0)
    , field_24_throwables(0)
{
    SetType(ReliveTypes::eThrowableArray);
    SetUpdatable(false);
    mCount = 0;
    gThrowableArray = this;
}

ThrowableArray::~ThrowableArray()
{
    gThrowableArray = nullptr;
    FreeResourceArray(&field_24_throwables);
}

void ThrowableArray::Remove(s16 count)
{
    mCount -= count;
    if (mCount > 0)
    {
        if (mThrowableTypeChanged && mNewThrowableTypeLoaded)
        {
            FreeResourceArray(&mBaseGameObjectResArray);
            mNewThrowableTypeLoaded = false;
        }
    }
    else
    {
        SetDead(true);
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

void ThrowableArray::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    ThrowableArraySaveState data = {};
    data.mType = ReliveTypes::eThrowableArray;
    data.mCount = mCount;
    pSaveBuffer.Write(data);
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
    if (GetDead())
    {
        SetDead(false);
    }

    if (mCount == 0 || mThrowableTypeChanged)
    {
        if (!mNewThrowableTypeLoaded)
        {
            switch (gThrowableFromOverlayId[gMap.mOverlayId])
            {
                case AETypes::eBone_11:
                    break;

                case AETypes::eMetal_24:
                    break;

                case AETypes::eGrenade_65:
                    break;

                case AETypes::eMeat_84:
                    break;

                case AETypes::eRock_105:
                    break;

                default:
                    break;
            }

            mNewThrowableTypeLoaded = true;
        }
    }

    mCount += count;
}

void ThrowableArray::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<ThrowableArraySaveState>();
    LoadRockTypes(gMap.mCurrentLevel, gMap.mCurrentPath);
    auto pArray = relive_new ThrowableArray();
    pArray->Add(pState->mCount);
}

void ThrowableArray::VRender(PrimHeader** /*ppOt*/)
{
    // Nothing to do here
}

