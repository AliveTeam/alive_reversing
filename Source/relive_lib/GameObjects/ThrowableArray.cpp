#include "stdafx.h"
#include "ThrowableArray.hpp"
#include "../Function.hpp"
#include "BaseThrowable.hpp"
#include "../../AliveLibAE/QuikSave.hpp"
#include "../GameType.hpp"

ThrowableArray* gThrowableArray = nullptr;

void LoadRockTypes(EReliveLevelIds /*level*/, u16 /*path*/)
{
    
}

ThrowableArray::ThrowableArray()
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eThrowableArray);
    SetUpdatable(false);
    gThrowableArray = this;
}

ThrowableArray::~ThrowableArray()
{
    gThrowableArray = nullptr;
}

void ThrowableArray::Remove(s16 count)
{
    mCount -= count;
    if (mCount > 0)
    {
        if (mThrowableTypeChanged && mNewThrowableTypeLoaded)
        {
            mNewThrowableTypeLoaded = false;
        }
    }
}

void ThrowableArray::VUpdate()
{
    if (mThrowableTypeChanged)
    {
        LoadRockTypes(GetMap().mCurrentLevel, GetMap().mCurrentPath);
        Add(0);
        mThrowableTypeChanged = false;
        SetUpdatable(false);
    }
}

void ThrowableArray::VScreenChanged()
{
    if (GetMap().mNextLevel != EReliveLevelIds::eMenu && GetMap().mNextLevel != EReliveLevelIds::eCredits)
    {
        bool typeChanged = false;
        if (GetGameType() == GameType::eAo)
        {
            typeChanged = gThrowableFromOverlayIdAO[GetMap().mOverlayId] != gThrowableFromOverlayIdAO[GetMap().GetOverlayId()];
        }
        else
        {
            typeChanged = gThrowableFromOverlayIdAE[GetMap().mOverlayId] != gThrowableFromOverlayIdAE[GetMap().GetOverlayId()];
        }

        if (typeChanged)
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
            mNewThrowableTypeLoaded = true;
        }
    }

    mCount += count;
}

void ThrowableArray::VRender(OrderingTable& /*ot*/)
{
    // Nothing to do here
}

void ThrowableArray::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    ThrowableArraySaveState data = {};
    data.mType = ReliveTypes::eThrowableArray;
    data.mCount = mCount;
    pSaveBuffer.Write(data);
}

void ThrowableArray::CreateFromSaveState(SerializedObjectData& pBuffer)
{
    const auto pState = pBuffer.ReadTmpPtr<ThrowableArraySaveState>();
    LoadRockTypes(GetMap().mCurrentLevel, GetMap().mCurrentPath);
    auto pArray = relive_new ThrowableArray();
    pArray->Add(pState->mCount);
}
