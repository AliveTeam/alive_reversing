#include "ITypeBase.hpp"

#include <string>

ITypeBase::ITypeBase(const std::string& typeName)
    : mName(typeName)
{
}

ITypeBase::~ITypeBase() = default;

[[nodiscard]] const std::string& ITypeBase::Name() const
{
    return mName;
}
