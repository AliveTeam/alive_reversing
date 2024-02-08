#include "BaseProperty.hpp"

#include <string>

namespace ReliveAPI {
BaseProperty::BaseProperty(const std::string& name, const std::string& typeName, const char* idStr, bool isVisibleToEditor)
    : m_name(name)
    , m_typeName(typeName)
    , m_idStr(idStr)
    , m_isVisibleToEditor(isVisibleToEditor)
{
}

BaseProperty::~BaseProperty() = default;

[[nodiscard]] const std::string& BaseProperty::Name() const
{
    return m_name;
}

[[nodiscard]] const std::string& BaseProperty::TypeName() const
{
    return m_typeName;
}

[[nodiscard]] bool BaseProperty::IsVisibleToEditor() const
{
    return m_isVisibleToEditor;
}

[[nodiscard]] const char* BaseProperty::IdStr() const
{
    return m_idStr;
}


} // namespace ReliveAPI
