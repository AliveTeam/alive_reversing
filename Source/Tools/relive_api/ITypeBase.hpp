#pragma once

#include <string>
#include <typeindex>

namespace jsonxx {
class Array;
}

namespace ReliveAPI {
class ITypeBase
{
public:
    explicit ITypeBase(const std::string& typeName);
    virtual ~ITypeBase();

    [[nodiscard]] const std::string& Name() const;

    [[nodiscard]] virtual const std::type_index& TypeIndex() const = 0;
    [[nodiscard]] virtual bool IsBasicType() const = 0;
    virtual void ToJson(jsonxx::Array& obj) const = 0;

private:
    std::string mName;
};
} // namespace ReliveAPI