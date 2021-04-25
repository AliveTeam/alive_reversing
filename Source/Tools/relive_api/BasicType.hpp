#pragma once

#include "ITypeBase.hpp"

template<class T>
class BasicType : public ITypeBase
{
public:
    explicit BasicType(const std::string& typeName, s32 minVal, s32 maxVal)
        : ITypeBase(typeName), mTypeIndex(typeid(T)), mMinVal(minVal), mMaxVal(maxVal)
    {

    }

    std::type_index TypeIndex() const override
    {
        return mTypeIndex;
    }

    bool IsBasicType() const override
    {
        return true;
    }

    void ToJson(jsonxx::Array& obj) const override
    {
        jsonxx::Object ret;
        ret << "min_value" << mMinVal;
        ret << "max_value" << mMaxVal;
        ret << "name" << Name();
        obj << ret;
    }

private:
    std::type_index mTypeIndex;
    s32 mMinVal = 0;
    s32 mMaxVal = 0;
};
