#pragma once

#include "ObjectTypes.hpp"

struct SaveStateBase
{
    SaveStateBase(ReliveTypes type, u32 size)
        : mType(type)
        , mSize(size)
    { }
    ReliveTypes mType;
    u32 mSize = 0;
};
