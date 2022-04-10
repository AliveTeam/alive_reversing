#pragma once

#include "relive_api_exceptions.hpp"

namespace ReliveAPI {

    class Context final
    {
    public:
        void MissingJsonProperty(const std::string& structName, const std::string& propertyName)
        {
            mMissingJsonProperties.emplace_back(MissingJsonPropertyRecord{structName, propertyName});
        }

        void MissingEnumType(const std::string& enumTypeName, const std::string& enumValue)
        {
            throw ReliveAPI::UnknownEnumTypeException(enumTypeName, enumValue);
        }

        void UnknownEnumValue(const std::string& enumTypeName, const std::string& enumValueToFind, const std::string& remappedToEnumValue)
        {
            mRemappedEnumValues.emplace_back(UnknownEnumValueRecord{enumTypeName, enumValueToFind, remappedToEnumValue});
        }

        void UnknownEnumValue(const std::string& enumTypeName, s64 enumValueToFind, s64 remappedToEnumValue)
        {
            mRemappedEnumValues.emplace_back(UnknownEnumValueRecord{enumTypeName, std::to_string(enumValueToFind), std::to_string(remappedToEnumValue)});
        }

        void JsonNeedsUpgrading(int apiVersion, int jsonVersion)
        {
            throw ReliveAPI::JsonNeedsUpgradingException(apiVersion, jsonVersion);
        }

        bool Ok() const
        {
            return mMissingJsonProperties.empty() && mRemappedEnumValues.empty();
        }

        struct MissingJsonPropertyRecord final
        {
            std::string mStructureTypeName;
            std::string mPropertyName;
        };

        struct UnknownEnumValueRecord final
        {
            std::string mEnumTypeName;
            std::string mEnumValueInJson;
            std::string mValueUsed;
        };

        const std::vector<MissingJsonPropertyRecord>& MissingJsonProperties() const
        {
            return mMissingJsonProperties;
        }

        const std::vector<UnknownEnumValueRecord>& RemappedEnumValues() const
        {
            return mRemappedEnumValues;
        }

    private:
        std::vector<MissingJsonPropertyRecord> mMissingJsonProperties;
        std::vector<UnknownEnumValueRecord> mRemappedEnumValues;
    };
}
