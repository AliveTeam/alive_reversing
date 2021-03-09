#include "../AliveLibCommon/stdafx_common.h"
#include "alive_api.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "LvlReaderWriter.hpp"

#include <iostream>
#include "JsonDocument.hpp"
#include "AETlvs.hpp"
#include "AOTlvs.hpp"
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
    struct PathBND
    {
        std::string mPathBndName;
        Error mResult = Error::None;
        std::vector<int> mPaths;
        std::vector<BYTE> mFileData;
        PathInfo mPathInfo;
    };

    [[nodiscard]] static PathInfo ToPathInfo(const AO::PathData& data, const AO::CollisionInfo& collisionInfo)
    {
        PathInfo info = {};
        info.mGridWidth = data.field_C_grid_width;
        info.mGridHeight = data.field_E_grid_height;
        info.mWidth = (data.field_8_bTop - data.field_4_bLeft) / data.field_C_grid_width;
        info.mHeight = (data.field_A_bBottom - data.field_6_bRight) / data.field_E_grid_height;
        info.mIndexTableOffset = data.field_18_object_index_table_offset;
        info.mObjectOffset = data.field_14_object_offset;

        info.mNumCollisionItems = collisionInfo.field_10_num_collision_items;
        info.mCollisionOffset = collisionInfo.field_C_collision_offset;

        return info;
    }

    [[nodiscard]] static PathBND OpenPathBnd(const std::string& inputLvlFile, int* pathId)
    {
        PathBND ret = {};

        // Open the LVL
        LvlReader lvl(inputLvlFile.c_str());
        if (!lvl.IsOpen())
        {
            ret.mResult = Error::LvlFileReadError;
            return ret;
        }

        // Find AE Path BND
        {
            for (int i = 0; i < ALIVE_COUNTOF(sPathData_559660.paths); i++)
            {
                auto pathRoot = &sPathData_559660.paths[i];
                if (pathRoot->field_38_bnd_name)
                {
                    std::optional<std::vector<BYTE>> pRec = lvl.ReadFile(pathRoot->field_38_bnd_name);
                    if (pRec)
                    {
                        ret.mPathBndName = pathRoot->field_38_bnd_name;
                        if (pathId)
                        {
                            ChunkedLvlFile pathChunks(*pRec);
                            std::optional<LvlFileChunk> chunk = pathChunks.ChunkById(*pathId);
                            if (!chunk)
                            {
                                abort();
                            }
                            ret.mFileData = chunk->Data();
                        }
                        ret.mResult = Error::None;
                        if (pathId)
                        {
                            if (*pathId >= 0 && *pathId <= pathRoot->field_18_num_paths)
                            {
                                const PathBlyRec& pBlyRec = pathRoot->field_0_pBlyArrayPtr[*pathId];
                                if (pBlyRec.field_0_blyName)
                                {
                                    ret.mPathBndName = pathRoot->field_38_bnd_name;
                                    // TODO: Fix fields/size calc
                                    ret.mPathInfo.mHeight = pBlyRec.field_4_pPathData->field_10_height;
                                    ret.mPathInfo.mWidth = pBlyRec.field_4_pPathData->field_E_width;
                                    ret.mPathInfo.mIndexTableOffset = pBlyRec.field_4_pPathData->field_16_object_indextable_offset;
                                    ret.mPathInfo.mObjectOffset = pBlyRec.field_4_pPathData->field_12_object_offset;

                                    ret.mPathInfo.mNumCollisionItems = pBlyRec.field_8_pCollisionData->field_10_num_collision_items;
                                    ret.mPathInfo.mCollisionOffset = pBlyRec.field_8_pCollisionData->field_C_collision_offset;

                                    return ret;
                                }
                            }
                            ret.mResult = Error::PathResourceNotFound;
                            return ret;
                        }
                        else
                        {
                            // Add all path ids
                            for (int i = 1; i < pathRoot->field_18_num_paths; i++)
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
                    std::optional<std::vector<BYTE>> pRec = lvl.ReadFile(pathRoot->field_38_bnd_name);
                    if (pRec)
                    {
                        ret.mPathBndName = pathRoot->field_38_bnd_name;
                        if (pathId)
                        {
                            ChunkedLvlFile pathChunks(*pRec);
                            std::optional<LvlFileChunk> chunk = pathChunks.ChunkById(*pathId);
                            if (!chunk)
                            {
                                abort();
                            }
                            ret.mFileData = chunk->Data();
                        }
                        ret.mResult = Error::None;
                        if (pathId)
                        {
                            if (*pathId >= 0 && *pathId <= pathRoot->field_18_num_paths)
                            {
                                const AO::PathBlyRec& pBlyRec = pathRoot->field_0_pBlyArrayPtr[*pathId];
                                if (pBlyRec.field_0_blyName)
                                {
                                    ret.mPathBndName = pathRoot->field_38_bnd_name;
                                    ret.mPathInfo = ToPathInfo(*pBlyRec.field_4_pPathData, *pBlyRec.field_8_pCollisionData);
                                    ret.mResult = Error::None;
                                    return ret;
                                }
                            }
                            ret.mResult = Error::PathResourceNotFound;
                            return ret;
                        }
                        else
                        {
                            // Add all path ids
                            for (int i = 1; i < pathRoot->field_18_num_paths; i++)
                            {
                                const AO::PathBlyRec& pBlyRec = pathRoot->field_0_pBlyArrayPtr[i];
                                if (pBlyRec.field_0_blyName)
                                {
                                    ret.mPaths.push_back(i);
                                }
                            }
                            return ret;
                        }
                    }
                }
            }
        }

        // Both failed
        ret.mResult = Error::PathResourceNotFound;
        return ret;
    }

    // Increment when a breaking change to the JSON is made and implement an 
    // upgrade step that converts from the last version to the current.
    constexpr int kApiVersion = 1;

    [[nodiscard]] int GetApiVersion()
    {
        return kApiVersion;
    }

    [[nodiscard]] Result ExportPathBinaryToJson(const std::string& jsonOutputFile, const std::string& inputLvlFile, int pathResourceId)
    {
        AliveAPI::Result ret = {};
        AliveAPI::PathBND pathBnd = AliveAPI::OpenPathBnd(inputLvlFile, &pathResourceId);
        ret.mResult = pathBnd.mResult;

        JsonDocument doc;
        doc.mVersion = GetApiVersion();
        doc.mGame = "AO";
        doc.SetPathInfo(pathBnd.mPathBndName, pathBnd.mPathInfo);
        doc.SaveAO(pathResourceId, pathBnd.mPathInfo, pathBnd.mFileData, jsonOutputFile);

        return ret;
    }

    [[nodiscard]] Result UpgradePathJson(const std::string& /*jsonFile*/)
    {
        return {};
    }

    [[nodiscard]] Result ImportPathJsonToBinary(const std::string& jsonInputFile, const std::string& outputLvlFile, const std::vector<std::string>& /*lvlResourceSources*/)
    {
        Result ret = {};

        JsonDocument doc;
        auto [camerasAndMapObjects, collisionLines] = doc.Load(jsonInputFile);

        if (doc.mVersion != GetApiVersion())
        {
            ret.mResult = Error::JsonFileNeedsUpgrading;
            return ret;
        }

        LvlWriter lvl(outputLvlFile.c_str());
        if (!lvl.IsOpen())
        {
            abort();
        }

        std::optional<std::vector<BYTE>> oldPathBnd = lvl.ReadFile(doc.mPathBnd.c_str());
        if (!oldPathBnd)
        {
            abort();
        }

        ChunkedLvlFile pathBndFile(*oldPathBnd);
        std::optional<LvlFileChunk> chunk = pathBndFile.ChunkById(doc.mPathId);
        if (!chunk)
        {
            abort();
        }

        const AO::PathBlyRec* pPathBlyRec = nullptr;
        for (int i = 0; i < ALIVE_COUNTOF(AO::gMapData_4CAB58.paths); i++)
        {
            auto pathRoot = &AO::gMapData_4CAB58.paths[i];
            if (doc.mPathBnd == pathRoot->field_38_bnd_name)
            {
                if (doc.mPathId >= 0 && doc.mPathId < pathRoot->field_18_num_paths)
                {
                    pPathBlyRec = &pathRoot->field_0_pBlyArrayPtr[doc.mPathId];
                    break;
                }
            }
        }

        if (!pPathBlyRec)
        {
            abort();
        }
       
        const PathInfo pathInfo = ToPathInfo(*pPathBlyRec->field_4_pPathData, *pPathBlyRec->field_8_pCollisionData);

        std::size_t pathResourceLen = 0;
        pathResourceLen += collisionLines.size() * sizeof(AO::PathLine); // TODO: Calc a better estimate
        
        ByteStream s;
        s.ReserveSize(pathResourceLen);

        // Write camera array
        for (int x = 0; x < pathInfo.mWidth; x++)
        {
            for (int y = 0; y < pathInfo.mHeight; y++)
            {
                bool found = false;
                for (const CameraNameAndTlvBlob& camIter : camerasAndMapObjects)
                {
                    if (camIter.x == x && camIter.y == y)
                    {
                        if (camIter.mName.length() != 8)
                        {
                            abort();
                        }
                        s.Write(camIter.mName);
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    const static BYTE blank[8] = {};
                    s.Write(blank);
                }
            }
        }

        if (collisionLines.size() > pathInfo.mNumCollisionItems)
        {
            abort();
        }

        // Write collision lines
        for (const AO::PathLine& line : collisionLines)
        {
            s.Write(line.field_0_rect.x);
            s.Write(line.field_0_rect.y);
            s.Write(line.field_0_rect.w);
            s.Write(line.field_0_rect.h);

            s.Write(line.field_8_type);
            s.Write(line.field_9_pad);
            s.Write(line.field_A_pad);
            s.Write(line.field_B_pad);

            s.Write(line.field_C_previous);
            s.Write(line.field_10_next);
        }

        struct IndexTableEntry
        {
            int x = 0;
            int y = 0;
            int objectsOffset = 0;
        };

        // Write object lists for each camera, either put everything before the index table if it fits, or put
        // everything after the index table as the game can enumerate ALL tlvs in one go. Thus splitting it across the index table
        // area would break that logic.
        // Additionally construct the index table offsets as we go.
        std::size_t allTlvsLen = 0;
        for (const CameraNameAndTlvBlob& camIter : camerasAndMapObjects)
        {
            for (const auto& tlv : camIter.mTlvBlobs)
            {
                allTlvsLen += tlv.size();
            }
        }

        // Data would overwrite the index table, put everything after it
        if (allTlvsLen + pathInfo.mObjectOffset > pathInfo.mIndexTableOffset)
        {
            s.SeekWrite(pathInfo.mObjectOffset + (sizeof(int) * pathInfo.mWidth * pathInfo.mHeight));
        }

        std::vector<IndexTableEntry> indexTable;
        for (int x = 0; x < pathInfo.mWidth; x++)
        {
            for (int y = 0; y < pathInfo.mHeight; y++)
            {
                bool found = false;
                for (const CameraNameAndTlvBlob& camIter : camerasAndMapObjects)
                {
                    if (camIter.x == x && camIter.y == y)
                    {
                        for (const auto& tlv : camIter.mTlvBlobs)
                        {
                            s.Write(tlv);
                        }

                        const int objDataOff = static_cast<int>(s.WritePos()) -  static_cast<int>(pathInfo.mObjectOffset);
                        indexTable.push_back({ x, y, objDataOff });
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    indexTable.push_back({ x, y, -1 });
                }
            }
        }

        // Write index table values we just populated at the correct offset
        s.SeekWrite(pathInfo.mIndexTableOffset);
        for (int x = 0; x < pathInfo.mWidth; x++)
        {
            for (int y = 0; y < pathInfo.mHeight; y++)
            {
                for (const IndexTableEntry& tableEntry : indexTable)
                {
                    if (tableEntry.x == x && tableEntry.y == y)
                    {
                        s.Write(tableEntry.objectsOffset);
                    }
                }
            }
        }

        // Push the path resource into a file chunk
        LvlFileChunk newPathBlock(doc.mPathId, ResourceManager::ResourceType::Resource_Path, s.GetBuffer());

        // Add or replace the original file chunk
        pathBndFile.AddChunk(newPathBlock);

        // Add or replace the original path BND in the lvl
        lvl.AddFile(doc.mPathBnd.c_str(), pathBndFile.Data());

        // Write out the updated lvl to disk
        if (!lvl.Save())
        {
            abort();
        }

        return ret;
    }

    [[nodiscard]] EnumeratePathsResult EnumeratePaths(const std::string& inputLvlFile)
    {
        EnumeratePathsResult ret = {};
        PathBND pathBnd = OpenPathBnd(inputLvlFile, nullptr);
        ret.mResult = pathBnd.mResult;
        ret.paths = pathBnd.mPaths;
        ret.pathBndName = pathBnd.mPathBndName;
        return ret;
    }
}
