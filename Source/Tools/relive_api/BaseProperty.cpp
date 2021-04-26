#include "BaseProperty.hpp"

#include <jsonxx/jsonxx.h>

#include <string>

BaseProperty::BaseProperty(const std::string& name, const std::string& typeName, bool isVisibleToEditor)
    : m_name(name), m_TypeName(typeName), m_isVisibleToEditor(isVisibleToEditor)
{
}

BaseProperty::~BaseProperty() = default;

[[nodiscard]] const std::string& BaseProperty::TypeName() const
{
    return m_TypeName;
}

[[nodiscard]] bool BaseProperty::IsVisibleToEditor() const
{
    return m_isVisibleToEditor;
}

[[nodiscard]] const std::string& BaseProperty::Name() const
{
    return m_name;
}
