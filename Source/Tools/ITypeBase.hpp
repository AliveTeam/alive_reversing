#pragma once

#include <string>
#include <typeindex>
#include <jsonxx/jsonxx.h>

class ITypeBase
{
public:
    explicit ITypeBase(const std::string& typeName) : mName(typeName)
    {

    }

    std::string Name() const
    {
        return mName;
    }

    virtual std::type_index TypeIndex() const = 0;
    virtual bool IsBasicType() const = 0;
    virtual void ToJson(jsonxx::Object& obj) const = 0;
private:
    std::string mName;
};
