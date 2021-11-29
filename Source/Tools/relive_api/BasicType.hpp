#pragma once

#include "BasicTypeBase.hpp"

#include "../../AliveLibCommon/Types.hpp"

#include <string>
#include <typeinfo>

template <class T>
class BasicType final : public BasicTypeBase
{
public:
    explicit BasicType(const std::string& typeName, s32 minVal, s32 maxVal)
        : BasicTypeBase(typeid(T), typeName, minVal, maxVal)
    {
    }
};
