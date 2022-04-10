#pragma once

#include "relive_api_exceptions.hpp"

namespace ReliveAPI {

    class Context final
    {
    public:
        void MissingJsonProperty(const std::string& structName, const std::string& propertyName)
        {
            (void) structName;
            (void) propertyName;
        }

        void MissingEnumType(const std::string& enumTypeName, const std::string& enumValue)
        {
            (void) enumTypeName;
            (void) enumValue;
            throw ReliveAPI::UnknownEnumValueException();
        }

        void UnknownEnumValue(const std::string& enumTypeName, const std::string& enumValueToFind, const std::string& remappedToEnumValue)
        {
            (void) enumTypeName;
            (void) enumValueToFind;
            (void) remappedToEnumValue;
        }

        void UnknownEnumValue(const std::string& enumTypeName, s64 enumValueToFind, s64 remappedToEnumValue)
        {
            (void) enumTypeName;
            (void) enumValueToFind;
            (void) remappedToEnumValue;

            throw ReliveAPI::UnknownEnumValueException();
        }

        void JsonNeedsUpgrading(int apiVersion, int jsonVersion)
        {
            throw ReliveAPI::JsonNeedsUpgradingException(apiVersion, jsonVersion);
        }

    };
}
