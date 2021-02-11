#include "alive_api.hpp"
#include "../AliveLibCommon/stdafx_common.h"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "../AliveLibAE/LvlArchive.hpp"

#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"

#include <iostream>
#include "magic_enum.hpp"

#include <gmock/gmock.h>

bool RunningAsInjectedDll()
{
    return false;
}

struct PropertyTypeInfo
{
    std::string mMappedName;
    std::string mTypeName;
};
using TFieldToNameAndTypeMapping = std::map<std::string, PropertyTypeInfo>;


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
    ObjectStructure() = default;
    explicit ObjectStructure(const std::string& name) : mName(name) { }

    std::string mName;
    std::vector<Property> mProperties;

    void AddProps(const TFieldToNameAndTypeMapping& propsToAdd)
    {
        for (const auto& [key, value] : propsToAdd)
        {
            Property property;
            property.mName = value.mMappedName;
            property.mType = value.mTypeName;
            mProperties.push_back(property);
        }
    }

};

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

class JsonDocument
{
public:
    int mVersion = 0;
    std::string mGame;

    std::string mPathBnd;
    int mPathId = 0;

    int mXGridSize = 0;
    int mXSize = 0;

    int mYGridSize = 0;
    int mYSize = 0;

    std::vector<CollisionObject> mCollisions;
    std::vector<CameraObject> mCameras;

    std::vector<ObjectStructure> mObjectStructures;
    std::vector<ObjectStructureBasicType> mBasicTypes;
    std::vector<ObjectStructurePropertyEnum> mEnums;

    ObjectStructure& AddStructure(const std::string& typeName)
    {
        ObjectStructure structure;
        structure.mName = typeName;
        mObjectStructures.push_back(structure);

        for (auto& item : mObjectStructures)
        {
            if (item.mName == typeName)
            {
                return item;
            }
        }
        abort();
    }

    void AddEnum(const std::string& enumName, const std::map<std::string, std::string>& values)
    {
        ObjectStructurePropertyEnum newEnum;
        newEnum.mName = enumName;
        for (const auto& [key, value] : values)
        {
            newEnum.mValues.push_back(value);
        }
        mEnums.push_back(newEnum);
    }


};

namespace AliveAPI
{
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

const std::map<std::string, TlvTypes> kObjectNameToAeTlv = 
{
    { "Hoist", TlvTypes::Hoist_2 },
};

const std::map<std::string, AO::TlvTypes> kObjectNameToAoTlv =
{
    { "Hoist", AO::TlvTypes::Hoist_3 },
};


#define NAME_OF( v ) #v

class StructureDb
{
public:
    template<class T>
    static std::vector<std::string> AsStringVec(const T& data)
    {
        std::vector<std::string> ret;
        for (const auto& item : data)
        {
            ret.push_back(std::string(magic_enum::enum_name(item)));
        }
        return ret;
    }

    void AddBasicTypes(JsonDocument& doc)
    {
        ObjectStructureBasicType basicType;
        basicType.mName = "Byte";
        basicType.mMinValue = std::numeric_limits<BYTE>::min();
        basicType.mMaxValue = std::numeric_limits<BYTE>::max();
        doc.mBasicTypes.push_back(basicType);
    }

    template<class T>
    static std::string Value(const T& map, const std::string& key)
    {
        auto it = map.find(key);
        if (it == std::end(map))
        {
            abort();
        }
        return it->second;
    }

    static std::string TypeName(const std::string& typeName)
    {
        if (typeName == NAME_OF(BYTE))
        {
            return "Byte";
        }
        abort();
    }

    void Add_Path_Hoist(JsonDocument& doc)
    {
        const std::map<std::string, std::string> kTypeNames =
        {
            { NAME_OF(Path_Hoist), "Hoist" },
            { NAME_OF(Path_Hoist::Type), "Enum_HoistType" },
            { NAME_OF(Path_Hoist::EdgeType), "Enum_HoistEdgeType" },
            { NAME_OF(Path_Hoist::Scale), "Enum_HoistScale" },
        };
        auto& type = doc.AddStructure(Value(kTypeNames, NAME_OF(Path_Hoist)));

        const TFieldToNameAndTypeMapping kPath_Hoist =
        {
            { NAME_OF(Path_Hoist::field_10_type), {"HoistType", Value(kTypeNames, NAME_OF(Path_Hoist::Type)) } },
            { NAME_OF(Path_Hoist::field_12_edge_type), {"EdgeType", Value(kTypeNames, NAME_OF(Path_Hoist::EdgeType)) } },
            { NAME_OF(Path_Hoist::field_14_id), {"Id", TypeName(NAME_OF(BYTE)) } },
            { NAME_OF(Path_Hoist::field_16_scale), {"Scale", Value(kTypeNames, NAME_OF(Path_Hoist::Scale)) } },
        };
        type.AddProps(kPath_Hoist);

        const std::map<std::string, std::string> kEnum_HoistType =
        {
            { NAME_OF(Path_Hoist::Type::eNextFloor), "next_floor" },
            { NAME_OF(Path_Hoist::Type::eNextEdge), "next_edge" },
            { NAME_OF(Path_Hoist::Type::eOffScreen), "off_screen" },
        };
        doc.AddEnum(Value(kTypeNames, NAME_OF(Path_Hoist::Type)), kEnum_HoistType);

        const std::map<std::string, std::string> kEnum_HoistEdgeType =
        {
            { NAME_OF(Path_Hoist::EdgeType::eLeft), "left" },
            { NAME_OF(Path_Hoist::EdgeType::eRight), "right" },
            { NAME_OF(Path_Hoist::EdgeType::eBoth), "both" },
        };
        doc.AddEnum(Value(kTypeNames, NAME_OF(Path_Hoist::EdgeType)), kEnum_HoistEdgeType);

        const std::map<std::string, std::string> kEnum_HoistScale =
        {
            { NAME_OF(Path_Hoist::Scale::eFull), "full" },
            { NAME_OF(Path_Hoist::Scale::eHalf), "half" },
        };
        doc.AddEnum(Value(kTypeNames, NAME_OF(Path_Hoist::Scale)), kEnum_HoistScale);
    }

    // TODO: Not used yet
    void CreateTLVAe(const std::string& type)
    {
        auto it = kObjectNameToAeTlv.find(type);
        if (it == std::end(kObjectNameToAeTlv))
        {
            return;
        }

    }


};


int main(int argc, char* argv[])
{
    JsonDocument doc;
    StructureDb db;
    db.AddBasicTypes(doc);
    db.Add_Path_Hoist(doc);

   // AliveAPI::EnumeratePaths("C:\\GOG Games\\Abes Exoddus\\MI.LVL");

    AliveAPI::ExportPathBinaryToJson("Output.json", "C:\\GOG Games\\Abes Exoddus\\MI.LVL", 1);



    return 0;
}