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


    [[nodiscard]] static PathInfo ToPathInfo(const PathData& data, const CollisionInfo& collisionInfo)
    {
        PathInfo info = {};
        info.mGridWidth = data.field_A_grid_width;
        info.mGridHeight = data.field_C_grid_height;
        info.mWidth = (data.field_4_bTop - data.field_0_bLeft) / data.field_A_grid_width;
        info.mHeight = (data.field_6_bBottom - data.field_2_bRight) / data.field_C_grid_height;
        info.mIndexTableOffset = data.field_16_object_indextable_offset;
        info.mObjectOffset = data.field_12_object_offset;

        info.mNumCollisionItems = collisionInfo.field_10_num_collision_items;
        info.mCollisionOffset = collisionInfo.field_C_collision_offset;

        return info;
    }

    class PathBlyRecAdapter
    {
    public:
        explicit PathBlyRecAdapter(const AO::PathBlyRec* pBlyRec)
            : mBlyRecAO(pBlyRec)
        {

        }

        explicit PathBlyRecAdapter(const PathBlyRec* pBlyRec)
            : mBlyRecAE(pBlyRec)
        {

        }

        const char* BlyName() const
        {
            return mBlyRecAO ? mBlyRecAO->field_0_blyName : mBlyRecAE->field_0_blyName;
        }

        PathInfo ConvertPathInfo() const
        {
            return mBlyRecAO ?
                ToPathInfo(*mBlyRecAO->field_4_pPathData, *mBlyRecAO->field_8_pCollisionData) :
                ToPathInfo(*mBlyRecAE->field_4_pPathData, *mBlyRecAE->field_8_pCollisionData);
        }

    private:
        const AO::PathBlyRec* mBlyRecAO = nullptr;
        const PathBlyRec* mBlyRecAE = nullptr;
    };

    class PathRootAdapter
    {
    public:
        explicit PathRootAdapter(AO::PathRoot* pRoot)
            : mRootAO(pRoot)
        {

        }

        explicit PathRootAdapter(PathRoot* pRoot)
            : mRootAE(pRoot)
        {

        }

        const char* BndName() const
        {
            return mRootAO ? mRootAO->field_38_bnd_name : mRootAE->field_38_bnd_name;
        }

        int PathCount() const
        {
            return mRootAO ? mRootAO->field_18_num_paths : mRootAE->field_18_num_paths;
        }

        PathBlyRecAdapter PathAt(int idx) const
        {
            return mRootAO ?
                PathBlyRecAdapter(&mRootAO->field_0_pBlyArrayPtr[idx]) :
                PathBlyRecAdapter(&mRootAE->field_0_pBlyArrayPtr[idx]);
        }

    private:
        AO::PathRoot* mRootAO = nullptr;
        PathRoot* mRootAE = nullptr;
    };

    class PathRootContainerAdapter
    {
    public:
        explicit PathRootContainerAdapter(Game gameType)
            : mGameType(gameType)
        {

        }

        int PathRootCount() const
        {
            return mGameType == Game::AO ? ALIVE_COUNTOF(AO::gMapData_4CAB58.paths) : ALIVE_COUNTOF(sPathData_559660.paths);
        }

        PathRootAdapter PathAt(int idx) const
        {
            return mGameType == Game::AO ?
                PathRootAdapter(&AO::gMapData_4CAB58.paths[idx]) :
                PathRootAdapter(&sPathData_559660.paths[idx]);
        }

    private:
        Game mGameType = {};
    };

    [[nodiscard]] static bool OpenPathBndGeneric(PathBND& ret, LvlReader& lvl, Game game, int* pathId)
    {
        const PathRootContainerAdapter adapter(game);
        for (int i = 0; i < adapter.PathRootCount(); i++)
        {
            const auto pathRoot = adapter.PathAt(i);
            if (pathRoot.BndName())
            {
                // Try to open the BND
                std::optional<std::vector<BYTE>> pRec = lvl.ReadFile(pathRoot.BndName());
                if (pRec)
                {
                    ret.mPathBndName = pathRoot.BndName();
                    if (pathId)
                    {
                        // Open the specific path if we have one
                        ChunkedLvlFile pathChunks(*pRec);
                        std::optional<LvlFileChunk> chunk = pathChunks.ChunkById(*pathId);
                        if (!chunk)
                        {
                            return false;
                        }

                        // Save the actual path resource block data
                        ret.mFileData = chunk->Data();

                        // Path id in range?
                        if (*pathId >= 0 && *pathId <= pathRoot.PathCount())
                        {
                            // Path at this id have a name?
                            const PathBlyRecAdapter pBlyRec = pathRoot.PathAt(*pathId);
                            if (pBlyRec.BlyName())
                            {
                                // Copy out its info
                                ret.mPathBndName = pathRoot.BndName();
                                ret.mPathInfo = pBlyRec.ConvertPathInfo();
                                ret.mResult = Error::None;
                                return true;
                            }
                        }

                        // Path id out of bounds or the entry is blank
                        ret.mResult = Error::PathResourceNotFound;
                        return false;
                    }

                    // Add all path ids
                    for (int i = 1; i < pathRoot.PathCount(); i++)
                    {
                        // Only add paths that are not blank entries
                        const PathBlyRecAdapter pBlyRec = pathRoot.PathAt(i);
                        if (pBlyRec.BlyName())
                        {
                            ret.mPaths.push_back(i);
                        }
                    }
                    return true;
                }
            }
        }
        return false;
    }

    [[nodiscard]] static PathBND OpenPathBnd(const std::string& inputLvlFile, Game& game, int* pathId)
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
        game = Game::AE;
        if (!OpenPathBndGeneric(ret, lvl, game, pathId))
        {
            // Failed, look for AO Path BND
            game = Game::AO;
            if (!OpenPathBndGeneric(ret, lvl, game, pathId))
            {
                // Both failed
                ret.mResult = Error::PathResourceNotFound;
            }
        }

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
        Game game = {};
        AliveAPI::PathBND pathBnd = AliveAPI::OpenPathBnd(inputLvlFile, game, &pathResourceId);
        ret.mResult = pathBnd.mResult;

        if (game == Game::AO)
        {
            JsonWriterAO doc(pathResourceId, pathBnd.mPathBndName, pathBnd.mPathInfo);
            doc.Save(pathBnd.mPathInfo, pathBnd.mFileData, jsonOutputFile);
        }
        else
        {
            JsonWriterAE doc(pathResourceId, pathBnd.mPathBndName, pathBnd.mPathInfo);
            doc.Save(pathBnd.mPathInfo, pathBnd.mFileData, jsonOutputFile);
        }
        return ret;
    }

    [[nodiscard]] Result UpgradePathJson(const std::string& /*jsonFile*/)
    {
        return {};
    }

    template<typename ReturnType, typename ContainerType>
    static ReturnType* ItemAtXY(ContainerType& container, int x, int y)
    {
        for (auto& iteratedItem : container)
        {
            if (iteratedItem.x == x && iteratedItem.y == y)
            {
                return &iteratedItem;
            }
        }
        return nullptr;
    }

    template<typename ItemType, typename ContainerType, typename FnOnItem>
    static void ForEachItemAtXY(const PathInfo& pathInfo, ContainerType& container, FnOnItem onItem)
    {
        for (int x = 0; x < pathInfo.mWidth; x++)
        {
            for (int y = 0; y < pathInfo.mHeight; y++)
            {
                auto item = ItemAtXY<ItemType>(container, x, y);
                if (item)
                {
                    onItem(*item);
                }
            }
        }
    }

    static void WriteCollisionLine(ByteStream& s, const AO::PathLine& line)
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

    static void WriteCollisionLine(ByteStream& s, const ::PathLine& line)
    {
        s.Write(line.field_0_rect.x);
        s.Write(line.field_0_rect.y);
        s.Write(line.field_0_rect.w);
        s.Write(line.field_0_rect.h);

        s.Write(line.field_8_type);
        s.Write(line.field_9_padding);

        s.Write(line.field_A_previous);
        s.Write(line.field_C_next);

        s.Write(line.field_E_previous2);
        s.Write(line.field_10_next2);

        s.Write(line.field_12_line_length);
    }

    template<typename JsonReaderType>
    static void SaveBinaryPathToLvl(Result& ret, Game gameType, const std::string& jsonInputFile, const std::string& inputLvlFile, const std::string& outputLvlFile)
    {
        JsonReaderType doc;
        auto [camerasAndMapObjects, collisionLines] = doc.Load(jsonInputFile);

        LvlWriter inputLvl(inputLvlFile.c_str());
        if (!inputLvl.IsOpen())
        {
            abort();
        }

        std::optional<std::vector<BYTE>> oldPathBnd = inputLvl.ReadFile(doc.mRootInfo.mPathBnd.c_str());
        if (!oldPathBnd)
        {
            abort();
        }

        ChunkedLvlFile pathBndFile(*oldPathBnd);
        std::optional<LvlFileChunk> chunk = pathBndFile.ChunkById(doc.mRootInfo.mPathId);
        if (!chunk)
        {
            abort();
        }

        PathRootContainerAdapter pathRootContainer(gameType);
        std::optional<PathBlyRecAdapter> pathBlyRecAdapter;
        for (int i = 0; i < pathRootContainer.PathRootCount(); i++)
        {
            PathRootAdapter pathRoot = pathRootContainer.PathAt(i);
            if (pathRoot.BndName())
            {
                if (doc.mRootInfo.mPathBnd == pathRoot.BndName())
                {
                    if (doc.mRootInfo.mPathId >= 0 && doc.mRootInfo.mPathId <= pathRoot.PathCount())
                    {
                        pathBlyRecAdapter = pathRoot.PathAt(doc.mRootInfo.mPathId);
                        break;
                    }
                }
            }
        }

        if (!pathBlyRecAdapter)
        {
            abort();
        }

        const PathInfo pathInfo = pathBlyRecAdapter->ConvertPathInfo();

        ByteStream s;
        s.ReserveSize(1024 * 20); // 20 kb estimate

        // Write camera array
        for (int x = 0; x < pathInfo.mWidth; x++)
        {
            for (int y = 0; y < pathInfo.mHeight; y++)
            {
                CameraNameAndTlvBlob* pItem = ItemAtXY<CameraNameAndTlvBlob>(camerasAndMapObjects, x, y);
                if (pItem)
                {
                    if (pItem->mName.length() == 8)
                    {
                        s.Write(pItem->mName);
                    }
                    else
                    {
                        abort();
                    }
                }
                else
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
        for (const auto& line : collisionLines)
        {
            WriteCollisionLine(s, line);
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
            allTlvsLen += camIter.TotalTlvSize();
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
                CameraNameAndTlvBlob* pItem = ItemAtXY<CameraNameAndTlvBlob>(camerasAndMapObjects, x, y);
                if (pItem)
                {
                    const int objDataOff = static_cast<int>(s.WritePos()) - static_cast<int>(pathInfo.mObjectOffset);
                    indexTable.push_back({ x, y, objDataOff });
                    for (const auto& tlv : pItem->mTlvBlobs)
                    {
                        s.Write(tlv);
                    }
                }
                else
                {
                    indexTable.push_back({ x, y, -1 });
                }
            }
        }

        // Write index table values we just populated at the correct offset
        s.SeekWrite(pathInfo.mIndexTableOffset);
        ForEachItemAtXY<IndexTableEntry>(pathInfo, indexTable, [&](const IndexTableEntry& tableEntry) {
            s.Write(tableEntry.objectsOffset);
            });

        // Push the path resource into a file chunk
        LvlFileChunk newPathBlock(doc.mRootInfo.mPathId, ResourceManager::ResourceType::Resource_Path, s.GetBuffer());

        // Add or replace the original file chunk
        pathBndFile.AddChunk(newPathBlock);

        // Add or replace the original path BND in the lvl
        inputLvl.AddFile(doc.mRootInfo.mPathBnd.c_str(), pathBndFile.Data());

        // Write out the updated lvl to disk
        if (!inputLvl.Save(outputLvlFile.c_str()))
        {
            abort();
        }
    }

    [[nodiscard]] Result ImportPathJsonToBinary(const std::string& jsonInputFile, const std::string& inputLvl, const std::string& outputLvlFile, const std::vector<std::string>& /*lvlResourceSources*/)
    {
        Result ret = {};

        JsonMapRootInfoReader rootInfo;
        if (!rootInfo.Read(jsonInputFile))
        {
            abort();
        }

        if (rootInfo.mMapRootInfo.mVersion != GetApiVersion())
        {
            ret.mResult = Error::JsonFileNeedsUpgrading;
            return ret;
        }

        if (rootInfo.mMapRootInfo.mGame == "AO")
        {
            SaveBinaryPathToLvl<JsonReaderAO>(ret, Game::AO, jsonInputFile, inputLvl, outputLvlFile);
        }
        else
        {
            SaveBinaryPathToLvl<JsonReaderAE>(ret, Game::AE, jsonInputFile, inputLvl, outputLvlFile);
        }

        return ret;
    }

    [[nodiscard]] EnumeratePathsResult EnumeratePaths(const std::string& inputLvlFile)
    {
        EnumeratePathsResult ret = {};
        Game game = {};
        PathBND pathBnd = OpenPathBnd(inputLvlFile, game, nullptr);
        ret.mResult = pathBnd.mResult;
        ret.paths = pathBnd.mPaths;
        ret.pathBndName = pathBnd.mPathBndName;
        return ret;
    }
}
