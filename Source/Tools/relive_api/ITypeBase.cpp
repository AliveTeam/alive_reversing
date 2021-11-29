#include "ITypeBase.hpp"

#include <string>

namespace ReliveAPI {
ITypeBase::ITypeBase(const std::string& typeName)
    : mName(typeName)
{
}

ITypeBase::~ITypeBase() = default;

[[nodiscard]] const std::string& ITypeBase::Name() const
{
    return mName;
}
} // namespace ReliveAPI
