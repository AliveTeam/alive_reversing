#pragma once

#include "relive_api_exceptions.hpp"
#include <vector>
#include <set>

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

        void LvlFileMissing(const std::string& lvlFileName)
        {
            mMissingLvlFiles.insert(lvlFileName);
        }

        void LvlFileMissingForCam(const std::string& lvlFileName)
        {
            mMissingLvlFilesForCams.insert(lvlFileName);
        }

        void LvlChunkMissingForCam(const std::string& lvlFileName, u32 resId)
        {
            mMissingCamResources[lvlFileName].insert(resId);
        }

        void SourceLvlOpenFailure(const std::string& lvlName)
        {
            mSourceLvlOpenFailures.insert(lvlName);
        }

        bool Ok() const
        {
            // mSourceLvlOpenFailures isn't an error if one lvl file to opened but we found all the resource we needed in some other lvl
            return mMissingJsonProperties.empty() && mRemappedEnumValues.empty() && mMissingCamResources.empty() && mMissingLvlFiles.empty() && mMissingLvlFilesForCams.empty();
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

        const std::set<std::string>& SourceLvlOpenFailures() const
        {
            return mSourceLvlOpenFailures;
        }

        const std::map<std::string, std::set<u32>>& MissingCamResources() const
        {
            return mMissingCamResources;
        }

        const std::set<std::string>& MissingLvlFiles() const
        {
            return mMissingLvlFiles;
        }

        const std::set<std::string>& MissingLvlFilesForCams() const
        {
            return mMissingLvlFilesForCams;
        }

    private:
        std::vector<MissingJsonPropertyRecord> mMissingJsonProperties;
        std::vector<UnknownEnumValueRecord> mRemappedEnumValues;
        std::set<std::string> mSourceLvlOpenFailures;
        std::map<std::string, std::set<u32>> mMissingCamResources;
        std::set<std::string> mMissingLvlFiles;
        std::set<std::string> mMissingLvlFilesForCams;
    };
}
