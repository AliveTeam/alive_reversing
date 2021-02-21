#include "alive_api.hpp"
#include "../AliveLibCommon/stdafx_common.h"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "../AliveLibAE/LvlArchive.hpp"

#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"

#include <iostream>
//#include "magic_enum.hpp"
#include "JsonDocument.hpp"
#include "AETlvs.hpp"
#include "TypesCollection.hpp"
#include <gmock/gmock.h>
#include <type_traits>
#include <typeindex>


bool RunningAsInjectedDll()
{
    return false;
}


namespace AliveAPI
{
    // Increment when a breaking change to the JSON is made and implement an 
    // upgrade step that converts from the last version to the current.
    constexpr int kApiVersion = 1;

    [[nodiscard]] int GetApiVersion()
    {
        return kApiVersion;
    }

    struct PathBND
    {
        std::string mPathBndName;
        Error mResult = Error::None;
        int mNumPaths = 0;
        std::vector<int> mPaths;
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

    [[nodiscard]] static PathBND OpenPathBnd(const std::string& inputLvlFile, int* pathId)
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
                                    ret.mPathBndName = pathRoot->field_38_bnd_name;
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
                        else
                        {
                            // Add all path ids
                            for (int i = 0; i < ret.mNumPaths; i++)
                            {
                                const PathBlyRec& pBlyRec = pathRoot->field_0_pBlyArrayPtr[i];
                                if (pBlyRec.field_0_blyName)
                                {
                                    ret.mPaths.push_back(i);
                                }
                            }
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
                                    ret.mPathBndName = pathRoot->field_38_bnd_name;
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
                    else
                    {
                        // Add all path ids
                        for (int i = 0; i < ret.mNumPaths; i++)
                        {
                            const AO::PathBlyRec& pBlyRec = pathRoot->field_0_pBlyArrayPtr[i];
                            if (pBlyRec.field_0_blyName)
                            {
                                ret.mPaths.push_back(i);
                            }
                        }
                    }
                }
            }
        }

        // Both failed
        ret.mResult = Error::PathResourceNotFound;
        return ret;
    }

    [[nodiscard]] Result ExportPathBinaryToJson(const std::string& /*jsonOutputFile*/, const std::string& inputLvlFile, int pathResourceId)
    {
        TypesCollection globalTypes;

        ResourceManager::Init_49BCE0();
        AliveAPI::Result ret = {};
        AliveAPI::PathBND pathBnd = AliveAPI::OpenPathBnd(inputLvlFile, &pathResourceId);
        ret.mResult = pathBnd.mResult;

        BYTE* pPathData = pathBnd.mFileData.data();
        const int* indexTable = reinterpret_cast<const int*>(pPathData + pathBnd.mPathInfo.mIndexTableOffset);
        int idx = 0;

        std::set<TlvTypes> usedTypes;

        for (int x = 0; x < pathBnd.mPathInfo.mWidth; x++)
        {
            for (int y = 0; y < pathBnd.mPathInfo.mHeight; y++)
            {
                const int objectTableIdx = indexTable[idx++];
                if (objectTableIdx == -1 || objectTableIdx >= 0x100000)
                {
                    continue;
                }

                BYTE* ptr = pPathData + objectTableIdx + pathBnd.mPathInfo.mObjectOffset;
                AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(ptr);
                pPathTLV->RangeCheck();
                if (pPathTLV->field_4_type <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
                {
                    for (;;)
                    {
                        // End of TLV list for current camera
                        if (pPathTLV->field_0_flags.Get(AO::TLV_Flags::eBit3_End_TLV_List))
                        {
                            break;
                        }

                        usedTypes.insert(static_cast<TlvTypes>(pPathTLV->field_4_type));
                        switch (pPathTLV->field_4_type)
                        {
                        case AO::TlvTypes::Hoist_3:
                        {
                            AETlvs::Path_Hoist hoist(globalTypes, (Path_TLV*)pPathTLV);
                            hoist.InstanceToJson(globalTypes);

                        }
                        break;
                        }

                        pPathTLV = AO::Path_TLV::Next_446460(pPathTLV);
                        pPathTLV->RangeCheck();
                    }
                }
            }
        }

        JsonDocument doc;
        doc.mGame = "AE";
        //doc.SaveAE(1, pathBnd.mPathInfo, pathBnd.mFileData, "lols.json");


        return {};
    }

    [[nodiscard]] Result UpgradePathJson(const std::string& /*jsonFile*/)
    {
        return {};
    }

    [[nodiscard]] Result ImportPathJsonToBinary(const std::string& /*jsonInputFile*/, const std::string& /*outputLvlFile*/, int /*pathResourceId*/, const std::vector<std::string>& /*lvlResourceSources*/)
    {
        return {};
    }

    [[nodiscard]] EnumeratePathsResult EnumeratePaths(const std::string& inputLvlFile)
    {
        ResourceManager::Init_49BCE0();
        EnumeratePathsResult ret = {};
        PathBND pathBnd = OpenPathBnd(inputLvlFile, nullptr);
        ret.mResult = pathBnd.mResult;
        ret.numPaths = pathBnd.mNumPaths;
        ret.paths = pathBnd.mPaths;
        ret.pathBndName = pathBnd.mPathBndName;
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

