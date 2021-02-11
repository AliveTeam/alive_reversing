#include "alive_api.hpp"
#include "../AliveLibCommon/stdafx_common.h"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "../AliveLibAE/LvlArchive.hpp"

#include "../AliveLibAO/PathData.hpp"

#include <gmock/gmock.h>

bool RunningAsInjectedDll()
{
    return false;
}


namespace AliveAPI
{
    class JsonDocument
    {
    public:
        struct CollisionObject
        {
            int mX1 = 0;
            int mY1 = 0;
            int mX2 = 0;
            int mY2 = 0;
            // TODO: Other properties
        };

        struct ObjectStructureBasicType
        {
            std::string mName;
            int mMinValue = 0;
            int mMaxValue = 0;
        };
        
        struct ObjectStructurePropertyEnum
        {
            std::string mName;
            std::vector<std::string> mValues;
        };

        struct ObjectStructureBasicTypeOrEnum
        {
        public:
            ObjectStructureBasicType mBasicType;
            ObjectStructurePropertyEnum mEnum;
            int mIntValue = 0;
            std::string mStringValue;
        };

        struct Property
        {
            std::string mName;
            std::string mType;
        };

        struct ObjectStructure
        {
            std::string mName;
            std::vector<Property> mProperties;
        };
        std::vector<ObjectStructure> mObjectStructures;

        struct MapObject
        {
            int mXPos = 0;
            int mYPos = 0;
            int mWidth = 0;
            int mHeight = 0;
            std::string mName;
            std::string mObjectStructuresType;
            std::vector<ObjectStructureBasicTypeOrEnum> mProperties;
        };

        struct CameraObject
        {
            std::string mName;
            int mId = 0;
            int mX = 0;
            int mY = 0;
            std::vector<MapObject> mMapObjects;
        };

        int mVersion = 0;

        std::string mPathBnd;
        int mPathId = 0;

        int mXGridSize = 0;
        int mXSize = 0;

        int mYGridSize = 0;
        int mYSize = 0;

        std::vector<CollisionObject> mCollisions;
        std::vector<CameraObject> mCameras;
    };

    // Increment when a breaking change to the JSON is made and implement an 
    // upgrade step that converts from the last version to the current.
    constexpr int kApiVersion = 1;

    int GetApiVersion()
    {
        return kApiVersion;
    }

    struct PathInfo
    {
        int mGridWidth = 0;
        int mWidth = 0;
        int mGridHeight = 0;
        int mHeight = 0;
        unsigned int mObjectOffset = 0;
        unsigned int mIndexTableOffset = 0;
    };

    struct PathBND
    {
        Error mResult = Error::None;
        int mNumPaths = 0;
        std::vector<BYTE> mFileData;
        PathInfo mPathInfo;
    };

    [[nodiscard]] static std::vector<unsigned char> ReadLvlFile(LvlArchive& archive, LvlFileRecord& rFileRec)
    {
        std::vector<unsigned char> fileContent;
        fileContent.resize(rFileRec.field_10_num_sectors * 2048);
        sLvlArchive_5BC520.Read_File_4330A0(&rFileRec, fileContent.data());
        fileContent.resize(rFileRec.field_14_file_size);
        return fileContent;
    }

    static PathBND OpenPathBnd(const std::string& inputLvlFile, int* pathId)
    {
        PathBND ret = {};

        // Open the LVL
        LvlArchive lvl;
        if (!lvl.Open_Archive_432E80(inputLvlFile.c_str()))
        {
            ret.mResult = Error::LvlFileReadEror;
            return ret;
        }

        // Find AE Path BND
        {
            for (int i = 0; i < ALIVE_COUNTOF(sPathData_559660.paths); i++)
            {
                auto pathRoot = &sPathData_559660.paths[i];
                if (pathRoot->field_38_bnd_name)
                {
                    LvlFileRecord* pRec = lvl.Find_File_Record_433160(pathRoot->field_38_bnd_name);
                    if (pRec)
                    {
                        ret.mFileData = ReadLvlFile(lvl, *pRec);
                        ret.mResult = Error::None;
                        ret.mNumPaths = pathRoot->field_18_num_paths;
                        if (pathId)
                        {
                            if (*pathId >= 0 && *pathId <= ret.mNumPaths)
                            {
                                const PathBlyRec& pBlyRec = pathRoot->field_0_pBlyArrayPtr[*pathId];
                                if (pBlyRec.field_0_blyName)
                                {
                                    ret.mPathInfo.mHeight = pBlyRec.field_4_pPathData->field_10_height;
                                    ret.mPathInfo.mWidth = pBlyRec.field_4_pPathData->field_E_width;
                                    ret.mPathInfo.mIndexTableOffset = pBlyRec.field_4_pPathData->field_16_object_indextable_offset;
                                    ret.mPathInfo.mObjectOffset = pBlyRec.field_4_pPathData->field_12_object_offset;
                                    return ret;
                                }
                            }
                            ret.mResult = Error::PathResourceNotFound;
                            return ret;
                        }
                        return ret;
                    }
                }
            }
        }

        // Failed, look for AO Path BND
        {
            for (int i = 0; i < ALIVE_COUNTOF(AO::gMapData_4CAB58.paths); i++)
            {
                auto pathRoot = &AO::gMapData_4CAB58.paths[i];
                if (pathRoot->field_38_bnd_name)
                {
                    LvlFileRecord* pRec = lvl.Find_File_Record_433160(pathRoot->field_38_bnd_name);
                    if (pRec)
                    {
                        ret.mFileData = ReadLvlFile(lvl, *pRec);
                        ret.mResult = Error::None;
                        ret.mNumPaths = pathRoot->field_18_num_paths;
                        if (pathId)
                        {
                            if (*pathId >= 0 && *pathId <= ret.mNumPaths)
                            {
                                const AO::PathBlyRec& pBlyRec = pathRoot->field_0_pBlyArrayPtr[*pathId];
                                if (pBlyRec.field_0_blyName)
                                {
                                    ret.mPathInfo.mHeight = pBlyRec.field_4_pPathData->field_A_bBottom;
                                    ret.mPathInfo.mWidth = pBlyRec.field_4_pPathData->field_6_bRight;
                                    ret.mPathInfo.mIndexTableOffset = pBlyRec.field_4_pPathData->field_18_object_index_table_offset;
                                    ret.mPathInfo.mObjectOffset = pBlyRec.field_4_pPathData->field_14_object_offset;
                                }
                            }
                            ret.mResult = Error::PathResourceNotFound;
                            return ret;
                        }
                        return ret;
                    }
                }
            }
        }

        // Both failed
        ret.mResult = Error::PathResourceNotFound;
        return ret;
    }

    Result ExportPathBinaryToJson(const std::string& /*jsonOutputFile*/, const std::string& outputLvlFile, int pathResourceId)
    {
        ResourceManager::Init_49BCE0();
        Result ret = {};
        PathBND pathBnd = OpenPathBnd(outputLvlFile, &pathResourceId);
        ret.mResult = pathBnd.mResult;


        return {};
    }

    Result UpgradePathJson(const std::string& /*jsonFile*/)
    {
        return {};
    }

    Result ImportPathJsonToBinary(const std::string& /*jsonInputFile*/, const std::string& /*inputLvlFile*/, int /*pathResourceId*/, const std::vector<std::string>& /*lvlResourceSources*/)
    {
        return {};
    }

    EnumeratePathsResult EnumeratePaths(const std::string& inputLvlFile)
    {
        ResourceManager::Init_49BCE0();
        EnumeratePathsResult ret = {};
        PathBND pathBnd = OpenPathBnd(inputLvlFile, nullptr);
        ret.mResult = pathBnd.mResult;
        ret.numPaths = pathBnd.mNumPaths;
        return ret;
    }
}

int main(int argc, char* argv[])
{
   // AliveAPI::EnumeratePaths("C:\\GOG Games\\Abes Exoddus\\MI.LVL");

    AliveAPI::ExportPathBinaryToJson("Output.json", "C:\\GOG Games\\Abes Exoddus\\MI.LVL", 1);

    return 0;
}