#pragma once

#include "ITypeBase.hpp"

template<class T>
class BasicType : public ITypeBase
{
public:
    explicit BasicType(const std::string& typeName, int minVal, int maxVal)
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

    void ToJson(jsonxx::Object& obj) const override
    {
        jsonxx::Object ret;

        ret << "min_value" << mMinVal;
        ret << "max_value" << mMaxVal;

        obj << Name() << ret;
    }

private:
    std::type_index mTypeIndex;
    int mMinVal = 0;
    int mMaxVal = 0;
};
