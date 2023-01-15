#include "../../AliveLibCommon/stdafx_common.h"
#include "relive_api.hpp"
#include "../../AliveLibAE/Path.hpp"
#include "../../AliveLibAE/PathData.hpp"
#include "../../AliveLibAO/Collisions.hpp"
#include "../../AliveLibAE/Collisions.hpp"
#include "LvlReaderWriter.hpp"
#include "JsonUpgraderAO.hpp"
#include "JsonUpgraderAE.hpp"
#include "JsonModelTypes.hpp"
#include "JsonReaderAE.hpp"
#include "JsonReaderAO.hpp"
#include "JsonWriterAE.hpp"
#include "JsonWriterAO.hpp"
#include "JsonMapRootInfoReader.hpp"
#include "TypesCollectionBase.hpp"
#include "Base64.hpp"
#include "CamConverter.hpp"
#include "TypesCollectionAE.hpp"
#include "TypesCollectionAO.hpp"
#include "ApiContext.hpp"
#include "../../AliveLibCommon/FG1Reader.hpp"
#include "../../AliveLibCommon/PathDataExtensionsTypes.hpp"
#include <iostream>
#include <gmock/gmock.h>
#include <type_traits>
#include <typeindex>
#include <sstream>
#include <lodepng/lodepng.h>

bool RunningAsInjectedDll()
{
    return false;
}

// items defined to avoid having to link AliveLibAO
namespace AO {
    const PathFunctionTable kObjectFactory = {};

    void CC Collisions::Factory_40CEC0(const CollisionInfo* , const u8* )
    {

    }
} // namespace AO

// ditto for AliveLibAe
const PathFunctionTable kObjectFactory = {};


static ReliveAPI::TAliveFatalCb fnAliveFatalCb = nullptr;


// ditto for AliveLibCommon
[[noreturn]] void ALIVE_FATAL(const char_type* errMsg)
{
    if (fnAliveFatalCb)
    {
        fnAliveFatalCb(errMsg);
    }
    // fnAliveFatalCb should throw or exit - compiler doesn't know this and we can't
    // mark a function pointer as noreturn. So abort in the impossible case that the function
    // pointer impl does return.
    abort();
}

void CC Collisions::Factory_4188A0(const CollisionInfo*, const u8*)
{

}

namespace ReliveAPI {

enum class OpenPathBndResult;
struct PathBND;
namespace Detail {
[[nodiscard]] static OpenPathBndResult OpenPathBndGeneric(std::vector<u8>& fileDataBuffer, PathBND& ret, LvlReader& lvl, Game game, s32* pathId);
void ImportCameraAndFG1(std::vector<u8>& fileDataBuffer, LvlWriter& inputLvl, const std::string& camName, const CameraImageAndLayers& imageAndLayers, bool allowFullFG1Blocks, const std::vector<LvlFileChunk>& additionalResourceBlocks);
}

void SetAliveFatalCallBack(TAliveFatalCb callBack)
{
    fnAliveFatalCb = callBack;
}

struct PathBND
{
    std::string mPathBndName;
    std::vector<s32> mPaths;
    std::vector<u8> mFileData;
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

    info.mAbeStartXPos = 0; // doesn't apply to AO
    info.mAbeStartYPos = 0; // doesn't apply to AO

    info.mNumMudsInPath = 0; // doesn't apply to AO
    info.mTotalMuds = 99;
    info.mBadEndingMuds = 75;
    info.mGoodEndingMuds = 50;

    return info;
}


[[nodiscard]] static PathInfo ToPathInfo(const PathData& data, const CollisionInfo& collisionInfo, u32 idx)
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

    info.mAbeStartXPos = data.field_1A_abe_start_xpos;
    info.mAbeStartYPos = data.field_1C_abe_start_ypos;

    info.mNumMudsInPath = Path_GetMudsInLevel(static_cast<LevelIds>(idx), 0); // Can pass 0 for the path id as this is not overriding any hardcoded path data
    info.mTotalMuds = 300;
    info.mBadEndingMuds = 20;
    info.mGoodEndingMuds = 255;

    return info;
}

class PathBlyRecAdapter
{
public:
    PathBlyRecAdapter(const AO::PathBlyRec* pBlyRec, u32 idx)
        : mBlyRecAO(pBlyRec)
        , mIdx(idx)
    {
    }

    PathBlyRecAdapter(const PathBlyRec* pBlyRec, u32 idx)
        : mBlyRecAE(pBlyRec)
        , mIdx(idx)
    {
    }

    const char_type* BlyName() const
    {
        return mBlyRecAO ? mBlyRecAO->field_0_blyName : mBlyRecAE->field_0_blyName;
    }

    PathInfo ConvertPathInfo() const
    {
        return mBlyRecAO ? ToPathInfo(*mBlyRecAO->field_4_pPathData, *mBlyRecAO->field_8_pCollisionData) : ToPathInfo(*mBlyRecAE->field_4_pPathData, *mBlyRecAE->field_8_pCollisionData, mIdx);
    }

private:
    const AO::PathBlyRec* mBlyRecAO = nullptr;
    const PathBlyRec* mBlyRecAE = nullptr;
    u32 mIdx = 0;
};

class PathRootAdapter
{
public:
    explicit PathRootAdapter(const AO::PathRoot* pRoot)
        : mRootAO(pRoot)
    {
    }

    explicit PathRootAdapter(const PathRoot* pRoot)
        : mRootAE(pRoot)
    {
    }

    const char_type* BndName() const
    {
        return mRootAO ? mRootAO->field_38_bnd_name : mRootAE->field_38_bnd_name;
    }

    s32 PathCount() const
    {
        return mRootAO ? mRootAO->field_18_num_paths : mRootAE->field_18_num_paths;
    }

    PathBlyRecAdapter PathAt(s32 idx) const
    {
        return mRootAO ? PathBlyRecAdapter(&mRootAO->field_0_pBlyArrayPtr[idx], idx) : PathBlyRecAdapter(&mRootAE->field_0_pBlyArrayPtr[idx], idx);
    }

private:
    const AO::PathRoot* mRootAO = nullptr;
    const PathRoot* mRootAE = nullptr;
};

class PathRootContainerAdapter
{
public:
    explicit PathRootContainerAdapter(Game gameType)
        : mGameType(gameType)
    {
    }

    s32 PathRootCount() const
    {
        return mGameType == Game::AO ? AO::Path_Get_Paths_Count() : Path_Get_Paths_Count();
    }

    PathRootAdapter PathAt(s32 idx) const
    {
        return mGameType == Game::AO ? PathRootAdapter(AO::Path_Get_PathRoot(idx)) : PathRootAdapter(Path_Get_PathRoot(idx));
    }

private:
    Game mGameType = {};
};

enum class OpenPathBndResult
{
    OK,
    PathResourceChunkNotFound,
    NoPaths
};

[[nodiscard]] static PathBND OpenPathBnd(LvlReader& lvlReader, std::vector<u8>& fileDataBuffer, Game& game, s32* pathId)
{
    PathBND ret = {};

    // Find AE Path BND
    if (Detail::OpenPathBndGeneric(fileDataBuffer, ret, lvlReader, Game::AE, pathId) == OpenPathBndResult::OK)
    {
        game = Game::AE;
        return ret;
    }

    // Failed, look for AO Path BND
    if (Detail::OpenPathBndGeneric(fileDataBuffer, ret, lvlReader, Game::AO, pathId) == OpenPathBndResult::OK)
    {
        game = Game::AO;
        return ret;
    }

    // Both failed
    throw ReliveAPI::OpenPathException();
}

// Increment when a breaking change to the JSON is made and implement an
// upgrade step that converts from the last version to the current.
constexpr s32 kApiVersion = 4;

[[nodiscard]] s32 GetApiVersion()
{
    return kApiVersion;
}


void ExportPathBinaryToJson(IFileIO& fileIO, const std::string& jsonOutputFile, const std::string& inputLvlFile, s32 pathResourceId, Context& context)
{
    std::vector<u8> buffer;
    Detail::ExportPathBinaryToJson(buffer, fileIO, jsonOutputFile, inputLvlFile, pathResourceId, context);
}

std::string UpgradePathJson(IFileIO& fileIO, const std::string& jsonFile)
{
    // Read the game and version, we assume this never changes in the file format
    JsonMapRootInfoReader rootInfo;
    rootInfo.Read(fileIO, jsonFile);

    if (rootInfo.mMapRootInfo.mGame == "AO")
    {
        JsonUpgraderAO upgrader;
        TypesCollectionAO aoTypes;
        return upgrader.Upgrade(aoTypes, fileIO, jsonFile, rootInfo.mMapRootInfo.mVersion, GetApiVersion());
    }
    else
    {
        JsonUpgraderAE upgrader;
        TypesCollectionAE aeTypes;
        return upgrader.Upgrade(aeTypes, fileIO, jsonFile, rootInfo.mMapRootInfo.mVersion, GetApiVersion());
    }
}

template <typename ReturnType, typename ContainerType>
static ReturnType* ItemAtXY(ContainerType& container, s32 x, s32 y)
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

template <typename ItemType, typename ContainerType, typename FnOnItem>
static void ForEachItemAtXY(u32 xSize, u32 ySize, ContainerType& container, FnOnItem onItem)
{
    for (u32 y = 0; y < ySize; y++)
    {
        for (u32 x = 0; x < xSize; x++)
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

    s.Write(static_cast<u8>(line.field_8_type));
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

    s.Write(static_cast<u8>(line.field_8_type));
    s.Write(line.field_9_padding);

    s.Write(line.field_A_previous);
    s.Write(line.field_C_next);

    s.Write(line.field_E_previous2);
    s.Write(line.field_10_next2);

    s.Write(line.field_12_line_length);
}

static u16 RGB888ToRGB565(const u8* rgb888Pixel)
{
    const u8 red = rgb888Pixel[0];
    const u8 green = rgb888Pixel[1];
    const u8 blue = rgb888Pixel[2];

    const u16 b = (blue >> 3) & 0x1f;
    const u16 g = ((green >> 2) & 0x3f) << 5;
    const u16 r = ((red >> 3) & 0x1f) << 11;

    return (r | g | b);
}

static std::vector<u8> Base64Png2RawPixels(const std::string& base64EncodedPng)
{
    unsigned width = 0;
    unsigned height = 0;
    std::vector<u8> rawPixels;
    std::vector<u8> pngData = FromBase64(base64EncodedPng);
    const auto error = lodepng::decode(rawPixels, width, height, pngData, LCT_RGBA, 8);
    if (error)
    {
        // todo: throw
    }

    if (width != 640 || height != 240)
    {
        // todo: throw
    }

    return rawPixels;
}

static u32 ToFG1Layer(Layer layer)
{
    switch (layer)
    {
        case Layer::eLayer_Well_Half_4:
            return 0;
        case Layer::eLayer_FG1_Half_18:
            return 1;
        case Layer::eLayer_Well_23:
            return 2;
        case Layer::eLayer_FG1_37:
            return 3;
    }
    ALIVE_FATAL("Not a valid FG1 layer");
}

// Iterate in 32x16 blocks, creating full or partial blocks as needed
static u32 MakeFG1Layer(std::stringstream& byteStream, const std::vector<u8>& rawPixels, Layer layer, bool allowFullFG1Blocks)
{
    const u32 blocksX = 640 / 32;
    const u32 blocksY = 240 / 16;

    u32 numBlocksWritten = 0;
    for (u32 blockX = 0; blockX < blocksX; blockX++)
    {
        for (u32 blockY = 0; blockY < blocksY; blockY++)
        {
            u32 whitePixelCount = 0;
            u32 bitMaskData[16] = {};
            for (u32 x = 0; x < 32; x++)
            {
                for (u32 y = 0; y < 16; y++)
                {
                    const u32* pPixel32 = &reinterpret_cast<const u32*>(rawPixels.data())[To1dIndex(640, x + (blockX * 32), y + (blockY * 16))];
                    if (RGB888ToRGB565(reinterpret_cast<const u8*>(pPixel32)) != 0)
                    {
                        whitePixelCount++;
                        bitMaskData[y] |= (1 << x);
                    }
                }
            }

            // A full chunk
            if (allowFullFG1Blocks && (whitePixelCount == 32 * 16))
            {
                Fg1Chunk chunk = {};
                chunk.field_0_type = eFullChunk;
                chunk.field_2_layer_or_decompressed_size = static_cast<u16>(ToFG1Layer(layer));
                chunk.field_4_xpos_or_compressed_size = static_cast<u16>(blockX * 32);
                chunk.field_6_ypos = static_cast<u16>(blockY * 16);
                chunk.field_8_width = 32;
                chunk.field_A_height = 16;
                byteStream.write(reinterpret_cast<const char*>(&chunk), sizeof(Fg1Chunk));
                // note: Don't add to blocks written, this count is for partial blocks only
            }
            // A partial chunk
            else if (whitePixelCount != 0)
            {
                Fg1Chunk chunk = {};
                chunk.field_0_type = ePartialChunk;
                chunk.field_2_layer_or_decompressed_size = static_cast<u16>(ToFG1Layer(layer));
                chunk.field_4_xpos_or_compressed_size = static_cast<u16>(blockX * 32);
                chunk.field_6_ypos = static_cast<u16>(blockY * 16);
                chunk.field_8_width = 32;
                chunk.field_A_height = 16;
                byteStream.write(reinterpret_cast<const char*>(&chunk), sizeof(Fg1Chunk));

                byteStream.write(reinterpret_cast<const char*>(&bitMaskData), sizeof(bitMaskData));

                numBlocksWritten++;
            }
        }
    }

    return numBlocksWritten;
}

static std::vector<u8> ConstructFG1Data(const CameraImageAndLayers& imageAndLayers, bool allowFullFG1Blocks)
{
    std::stringstream byteStream;

    // Write magic "relive fg1" marker
    const u32 kMagic = ResourceManager::Resource_FG1;
    byteStream.write(reinterpret_cast<const char*>(&kMagic), sizeof(u32));

    // Write standard AE FG1 header
    u32 numBlocksWritten = 0;
    byteStream.write(reinterpret_cast<const char*>(&numBlocksWritten), sizeof(u32));

    if (!imageAndLayers.mBackgroundLayer.empty())
    {
        numBlocksWritten += MakeFG1Layer(byteStream, Base64Png2RawPixels(imageAndLayers.mBackgroundLayer), Layer::eLayer_FG1_Half_18, allowFullFG1Blocks);
    }

    if (!imageAndLayers.mBackgroundWellLayer.empty())
    {
        numBlocksWritten += MakeFG1Layer(byteStream, Base64Png2RawPixels(imageAndLayers.mBackgroundWellLayer), Layer::eLayer_Well_Half_4, allowFullFG1Blocks);
    }

    if (!imageAndLayers.mForegroundLayer.empty())
    {
        numBlocksWritten += MakeFG1Layer(byteStream, Base64Png2RawPixels(imageAndLayers.mForegroundLayer), Layer::eLayer_FG1_37, allowFullFG1Blocks);
    }

    if (!imageAndLayers.mForegroundWellLayer.empty())
    {
        numBlocksWritten += MakeFG1Layer(byteStream, Base64Png2RawPixels(imageAndLayers.mForegroundWellLayer), Layer::eLayer_Well_23, allowFullFG1Blocks);
    }

    if (numBlocksWritten == 0)
    {
        return {};
    }

    // End chunk
    Fg1Chunk endChunk = {};
    endChunk.field_0_type = eEndChunk;
    byteStream.write(reinterpret_cast<const char*>(&endChunk), sizeof(Fg1Chunk));

    // Rewrite the updated block count
    byteStream.seekp(sizeof(u32), std::ios::beg);
    byteStream.write(reinterpret_cast<const char*>(&numBlocksWritten), sizeof(u32));

    const std::string tmpStr = byteStream.str();
    return std::vector<u8>(tmpStr.begin(), tmpStr.end());
}

template<typename FnLoadResources>
static void ImportCamerasAndFG1(std::vector<u8>& fileDataBuffer, LvlWriter& inputLvl, const std::vector<CameraNameAndTlvBlob>& camerasAndMapObjects, bool allowFullFG1Blocks, FnLoadResources fnLoadResource)
{
    // Rebuild cameras/FG1 and embedded resource blocks
    for (const CameraNameAndTlvBlob& camIter : camerasAndMapObjects)
    {
        // Get camera ID from the name for the Bits chunk
        if (!camIter.mName.empty())
        {
            std::vector<LvlFileChunk> additionalResourceBlocks;
            for (const AnimId id : camIter.mRequiredResources)
            {
                std::optional<LvlFileChunk> res = fnLoadResource(id);
                if (res)
                {
                    additionalResourceBlocks.emplace_back(std::move(*res));
                }
            }
            Detail::ImportCameraAndFG1(fileDataBuffer, inputLvl, camIter.mName + ".CAM", camIter.mCameraAndLayers, allowFullFG1Blocks, additionalResourceBlocks);
        }
    }
}

static void WriteStringTable(const std::vector<std::string>& strings, ByteStream& s)
{
    // String count
    s.Write(static_cast<u64>(strings.size()));

    // String lengths (size of a pointer)
    for (const auto& str : strings)
    {
        s.Write(static_cast<u64>(str.length() + 1));
    }

    // String data
    for (const auto& str : strings)
    {
        s.Write(str);
        s.Write(u8(0));
    }
}

class ResourceLocator final
{
public:
    ResourceLocator(IFileIO& fileIO, const std::vector<std::string>& lvlResourceSources, Context& context)
        : mFileIO(fileIO)
        , mLvlResourceSources(lvlResourceSources)
        , mContext(context)
    {

    }

    std::optional<ChunkedLvlFile> FindFile(const AnimRecord& animRec)
    {
        auto file = FindInSourceLvls(animRec);
        if (!file)
        {
            mContext.LvlFileMissing(animRec.mBanName);
        }
        return file;
    }

    std::optional<LvlFileChunk> FindChunk(const AnimRecord& animRec)
    {
        auto file = FindInSourceLvls(animRec);
        if (!file)
        {
            mContext.LvlFileMissingForCam(animRec.mBanName);
            return {};
        }

        for (u32 i = 0; i < file->ChunkCount(); i++)
        {
            auto chunk = file->ChunkAt(i);
            // Palts and other resources can share ids, explicitly look for anims
            if (chunk.Id() == static_cast<u32>(animRec.mResourceId) && chunk.Header().field_8_type == ResourceManager::Resource_Animation)
            {
                return chunk;
            }
        }

        mContext.LvlChunkMissingForCam(animRec.mBanName, animRec.mResourceId);
        return {};
    }

private:
    std::optional<ChunkedLvlFile> FindInSourceLvls(const AnimRecord& animRec)
    {
        for (auto& lvl : mOpenLvls)
        {
            auto file = FindInLvl(*lvl, animRec);
            if (file)
            {
                return file;
            }
        }

        for (;;)
        {
            auto nextLvl = OpenNextLvl();
            if (nextLvl)
            {
                auto file = FindInLvl(*nextLvl, animRec);
                if (file)
                {
                    return file;
                }
            }
            else
            {
                // No more lvls to try
                return {};
            }
        };
    }

    LvlReader* OpenNextLvl()
    {
        if (mLvlResourceSources.empty())
        {
            return nullptr;
        }

        const auto lvlFileName = mLvlResourceSources[0];
        mLvlResourceSources.erase(mLvlResourceSources.begin());

        auto lvlReader = std::make_unique<LvlReader>(mFileIO, lvlFileName.c_str());
        if (lvlReader->IsOpen())
        {
            LvlReader* holdPtr = lvlReader.get();
            mOpenLvls.emplace_back(std::move(lvlReader));
            return holdPtr;
        }
        else
        {
            mContext.SourceLvlOpenFailure(lvlFileName);
        }
        return nullptr;
    }

    std::optional<ChunkedLvlFile> FindInLvl(LvlReader& reader, const AnimRecord& animRec)
    {
        auto data = reader.ReadFile(animRec.mBanName);
        if (data)
        {
            return ChunkedLvlFile(std::move(*data));
        }
        return {};
    }

    IFileIO& mFileIO;
    std::vector<std::string> mLvlResourceSources;
    Context& mContext;

    std::vector<std::unique_ptr<LvlReader>> mOpenLvls;
};

template <typename JsonReaderType>
static void SaveBinaryPathToLvl(IFileIO& fileIo, Game game, std::vector<u8>& fileDataBuffer, IFileIO& fileIO, const std::string& jsonInputFile, const std::string& inputLvlFile, const std::string& outputLvlFile, bool skipCamsAndFG1, bool allowFullFG1Blocks, Context& context, const std::vector<std::string>& lvlResourceSources)
{
    JsonReaderType doc;
    auto loadedJsonData = doc.Load(fileIO, jsonInputFile, context);

    LvlWriter inputLvl(fileIO, inputLvlFile.c_str());
    if (!inputLvl.IsOpen())
    {
        throw ReliveAPI::IOReadException(inputLvlFile.c_str());
    }

    std::optional<std::vector<u8>> oldPathBnd = inputLvl.ReadFile(doc.mRootInfo.mPathBnd.c_str());
    if (!oldPathBnd)
    {
        // XXPATH.BND should always be present
        throw ReliveAPI::OpenPathException();
    }

    ByteStream s;
    s.ReserveSize(1024 * 20); // 20 kb estimate

    // Write camera array
    for (s32 y = 0; y < doc.mRootInfo.mYSize; y++)
    {
        for (s32 x = 0; x < doc.mRootInfo.mXSize; x++)
        {
            CameraNameAndTlvBlob* pItem = ItemAtXY<CameraNameAndTlvBlob>(loadedJsonData.mPerCamData, x, y);
            if (pItem)
            {
                // We have a camera
                if (pItem->mName.empty())
                {
                    // With a blank name
                    const static u8 blank[8] = {};
                    s.Write(blank);
                }
                else if (pItem->mName.length() == 8)
                {
                    // With a non blank name
                    s.Write(pItem->mName);
                }
                else
                {
                    // With a name that isn't 8 chars
                    throw ReliveAPI::BadCameraNameException(pItem->mName);
                }
            }
            else
            {
                // No camera
                const static u8 blank[8] = {};
                s.Write(blank);
            }
        }
    }
    const size_t collisionOffsetPos = s.WritePos();

    // Write collision lines
    for (const auto& line : loadedJsonData.mCollisions)
    {
        WriteCollisionLine(s, line);
    }

    struct IndexTableEntry final
    {
        s32 x = 0;
        s32 y = 0;
        s32 objectsOffset = 0;
    };

    const size_t objectOffsetPos = s.WritePos();

    // Write TLVs
    std::vector<IndexTableEntry> indexTable;
    for (s32 y = 0; y < doc.mRootInfo.mYSize; y++)
    {
        for (s32 x = 0; x < doc.mRootInfo.mXSize; x++)
        {
            CameraNameAndTlvBlob* pItem = ItemAtXY<CameraNameAndTlvBlob>(loadedJsonData.mPerCamData, x, y);
            if (pItem)
            {
                if (pItem->mTlvBlobs.empty())
                {
                    // Dont add a table entry for a camera that has no TLV data
                    indexTable.push_back({x, y, -1});
                }
                else
                {
                    indexTable.push_back({x, y, static_cast<s32>(s.WritePos())});
                    for (const auto& tlv : pItem->mTlvBlobs)
                    {
                        s.Write(tlv);
                    }
                }
            }
            else
            {
                indexTable.push_back({x, y, -1});
            }
        }
    }

    // Write index table values we just populated, correcting their offsets
    const size_t indexTableOffSetPos = s.WritePos();
    for (auto& indexTableEntry : indexTable)
    {
        if (indexTableEntry.objectsOffset != -1)
        {
            indexTableEntry.objectsOffset -= static_cast<s32>(objectOffsetPos);
        }
    }

    ForEachItemAtXY<IndexTableEntry>(doc.mRootInfo.mXSize, doc.mRootInfo.mYSize, indexTable, [&](const IndexTableEntry& tableEntry)
                                     { s.Write(tableEntry.objectsOffset); });

    // Push the path resource into a file chunk
    std::vector<u8> tmpPathVec = s.GetBuffer();
    LvlFileChunk newPathBlock(doc.mRootInfo.mPathId, ResourceManager::ResourceType::Resource_Path, std::move(tmpPathVec));

    ChunkedLvlFile pathBndFile(*oldPathBnd);

    // Add or replace the original file chunk
    pathBndFile.AddChunk(std::move(newPathBlock));

    // Construct chunk with new "hard coded" info
    PerPathExtension pathExtData = {};
    pathExtData.mSize = sizeof(PerPathExtension);
    pathExtData.mNumCollisionLines = static_cast<u32>(loadedJsonData.mCollisions.size());
    pathExtData.mIndexTableOffset = static_cast<u32>(indexTableOffSetPos);
    pathExtData.mObjectOffset = static_cast<u32>(objectOffsetPos);
    pathExtData.mXSize = doc.mRootInfo.mXSize;
    pathExtData.mYSize = doc.mRootInfo.mYSize;
    pathExtData.mPathId = doc.mRootInfo.mPathId;
    pathExtData.mCollisionOffset = static_cast<u32>(collisionOffsetPos);
    pathExtData.mGridWidth = game == Game::AE ? 375 : 1024;
    pathExtData.mGridHeight = game == Game::AE ? 260 : 480;

    pathExtData.mAbeStartXPos = doc.mRootInfo.mAbeStartXPos;
    pathExtData.mAbeStartYPos = doc.mRootInfo.mAbeStartYPos;

    pathExtData.mNumMudsInPath = doc.mRootInfo.mNumMudsInPath;

    pathExtData.mTotalMuds = doc.mRootInfo.mTotalMuds;
    pathExtData.mBadEndingMuds = doc.mRootInfo.mBadEndingMuds;
    pathExtData.mGoodEndingMuds = doc.mRootInfo.mGoodEndingMuds;

    // Create the BLY name
    std::string strPathId = std::to_string(doc.mRootInfo.mPathId);
    std::string blyName = doc.mRootInfo.mPathBnd.substr(0, 2) + "P" + strPathId + ".BLY";
    strcpy(pathExtData.mBlyName, blyName.c_str());

    // Add the fixed sized data
    ByteStream perPathExtensionStream;
    perPathExtensionStream.WriteBytes(reinterpret_cast<const u8*>(&pathExtData), sizeof(PerPathExtension));

    WriteStringTable(doc.mRootInfo.mLCDScreenMessages, perPathExtensionStream);
    WriteStringTable(doc.mRootInfo.mHintFlyMessages, perPathExtensionStream);

    // Add it as a chunk
    std::vector<u8> extBuffer = perPathExtensionStream.GetBuffer();
    LvlFileChunk pathExtDataChunk(doc.mRootInfo.mPathId | doc.mRootInfo.mPathId << 8, ResourceManager::Resource_Pxtd, std::move(extBuffer));
    pathBndFile.AddChunk(std::move(pathExtDataChunk));

    // Add or replace the original path BND in the lvl
    inputLvl.AddFile(doc.mRootInfo.mPathBnd.c_str(), std::move(pathBndFile).Data());

    ResourceLocator resourceLocator(fileIO, lvlResourceSources, context);
    auto findResourceForCam = [&](AnimId id)
    {
        const AnimRecord& rec = game == Game::AE ? AnimRec(id) : AO::AnimRec(id);
        return resourceLocator.FindChunk(rec);
    };

    if (!skipCamsAndFG1)
    {
        ImportCamerasAndFG1(fileDataBuffer, inputLvl, loadedJsonData.mPerCamData, allowFullFG1Blocks, findResourceForCam);
    }

    // Add any BAN/BNDs required for objects in this path that are missing
    for (const AnimId& id : loadedJsonData.mResourcesRequiredInLvl)
    {
        const AnimRecord& rec = game == Game::AE ? AnimRec(id) : AO::AnimRec(id);
        if (!inputLvl.FindFile(rec.mBanName))
        {
            auto banFile = resourceLocator.FindFile(rec);
            if (banFile)
            {
                inputLvl.AddFile(rec.mBanName, banFile->Data());
            }
        }
    }

    // Write out the updated lvl to disk
    if (!inputLvl.Save(fileIo, fileDataBuffer, outputLvlFile.c_str()))
    {
        throw ReliveAPI::IOWriteException(outputLvlFile);
    }
}

void ImportPathJsonToBinary(IFileIO& fileIO, const std::string& jsonInputFile, const std::string& inputLvl, const std::string& outputLvlFile, const std::vector<std::string>& lvlResourceSources, Context& context)
{
    std::vector<u8> buffer;
    Detail::ImportPathJsonToBinary(buffer, fileIO, jsonInputFile, inputLvl, outputLvlFile, lvlResourceSources, false, context);
}


[[nodiscard]] EnumeratePathsResult EnumeratePaths(IFileIO& fileIO, const std::string& inputLvlFile)
{
    std::vector<u8> buffer;
    return Detail::EnumeratePaths(buffer, fileIO, inputLvlFile);
}

namespace Detail {

void DebugDumpTlvs(const std::string& prefix, IFileIO& fileIO, const std::string& lvlFile, s32 pathId)
{
    Game game = {};

    LvlReader lvl(fileIO, lvlFile.c_str());
    std::vector<u8> buffer;
    ReliveAPI::PathBND pathBnd = ReliveAPI::OpenPathBnd(lvl, buffer, game, &pathId);

    if (game == Game::AO)
    {
        JsonWriterAO doc(pathId, pathBnd.mPathBndName, pathBnd.mPathInfo);
        doc.DebugDumpTlvs(fileIO, prefix, pathBnd.mPathInfo, pathBnd.mFileData);
    }
    else
    {
        JsonWriterAE doc(pathId, pathBnd.mPathBndName, pathBnd.mPathInfo);
        doc.DebugDumpTlvs(fileIO, prefix, pathBnd.mPathInfo, pathBnd.mFileData);
    }
}


[[nodiscard]] static OpenPathBndResult OpenPathBndGeneric(std::vector<u8>& fileDataBuffer, PathBND& ret, LvlReader& lvl, Game game, s32* pathId)
{
    const PathRootContainerAdapter adapter(game);
    for (s32 i = 0; i < adapter.PathRootCount(); ++i)
    {
        const auto pathRoot = adapter.PathAt(i);
        if (!pathRoot.BndName())
        {
            continue;
        }

        // Try to open the BND
        const bool goodRead = lvl.ReadFileInto(fileDataBuffer, pathRoot.BndName());
        if (!goodRead)
        {
            continue;
        }

        ret.mPathBndName = pathRoot.BndName();

        ChunkedLvlFile pathChunks(fileDataBuffer);

        if (pathId)
        {
            // Open the specific path if we have one
            std::optional<LvlFileChunk> chunk = pathChunks.ChunkById(*pathId);
            if (!chunk)
            {
                return OpenPathBndResult::PathResourceChunkNotFound;
            }

            // Save the actual path resource block data
            ret.mFileData = std::move(chunk)->Data();

            // Path id in range?
            if (*pathId >= 0 && *pathId < 99)
            {
                // Path at this id have a name?
                const PathBlyRecAdapter pBlyRec = pathRoot.PathAt(*pathId);

                // See if we have extended info for this path entry
                std::optional<LvlFileChunk> extChunk = pathChunks.ChunkById(*pathId | *pathId << 8);
                if (extChunk)
                {
                    auto pChunkData = extChunk->Data().data();
                    auto pExt = reinterpret_cast<const PerPathExtension*>(pChunkData);
                    ret.mPathBndName = pathRoot.BndName();
                    ret.mPathInfo.mObjectOffset = pExt->mObjectOffset;
                    ret.mPathInfo.mIndexTableOffset = pExt->mIndexTableOffset;
                    ret.mPathInfo.mNumCollisionItems = pExt->mNumCollisionLines;
                    ret.mPathInfo.mHeight = pExt->mYSize;
                    ret.mPathInfo.mWidth = pExt->mXSize;
                    ret.mPathInfo.mCollisionOffset = pExt->mCollisionOffset;
                    ret.mPathInfo.mGridWidth = pExt->mGridWidth;
                    ret.mPathInfo.mGridHeight = pExt->mGridHeight;

                    ret.mPathInfo.mAbeStartXPos = pExt->mAbeStartXPos;
                    ret.mPathInfo.mAbeStartYPos = pExt->mAbeStartYPos;

                    ret.mPathInfo.mTotalMuds = pExt->mTotalMuds;

                    ret.mPathInfo.mNumMudsInPath = pExt->mNumMudsInPath;
                    ret.mPathInfo.mBadEndingMuds = pExt->mBadEndingMuds;
                    ret.mPathInfo.mGoodEndingMuds = pExt->mGoodEndingMuds;

                    pChunkData += sizeof(PerPathExtension);

                    // Read LCD Screen messages
                    auto pLCDScreenMsgs = reinterpret_cast<StringTable*>(pChunkData);
                    pChunkData = StringTable::MakeTable(pLCDScreenMsgs);
                    for (u64 j = 0u; j < pLCDScreenMsgs->mStringCount; j++)
                    {
                        ret.mPathInfo.mLCDScreenMessages.emplace_back(pLCDScreenMsgs->mStrings[j].string_ptr);
                    };

                    // Read hint fly messages, will be empty for AE
                    auto pHintFlyMsgs = reinterpret_cast<StringTable*>(pChunkData);
                    pChunkData = StringTable::MakeTable(pHintFlyMsgs);
                    for (u64 j = 0u; j < pHintFlyMsgs->mStringCount; j++)
                    {
                        ret.mPathInfo.mHintFlyMessages.emplace_back(pHintFlyMsgs->mStrings[j].string_ptr);
                    };

                    return OpenPathBndResult::OK;
                }
                else if (pBlyRec.BlyName())
                {
                    // Copy out its info
                    ret.mPathBndName = pathRoot.BndName();
                    ret.mPathInfo = pBlyRec.ConvertPathInfo();
                    return OpenPathBndResult::OK;
                }
            }

            // Path id out of bounds or the entry is blank
            return OpenPathBndResult::PathResourceChunkNotFound;
        }

        // Add all path ids
        for (s32 j = 1; j < 99; ++j)
        {
            // Only add paths that are not blank entries
            const PathBlyRecAdapter pBlyRec = pathRoot.PathAt(j);
            if (pBlyRec.BlyName())
            {
                ret.mPaths.push_back(j);
            }
            else
            {
                // OG has no path, but did one get added via the editor?
                std::optional<LvlFileChunk> extChunk = pathChunks.ChunkById(j | j << 8);
                if (extChunk)
                {
                    ret.mPaths.push_back(j);
                }
            }
        }

        // Return all of the file data
        ret.mFileData = fileDataBuffer;

        return OpenPathBndResult::OK;
    }

    return OpenPathBndResult::NoPaths;
}

void ImportPathJsonToBinary(std::vector<u8>& fileDataBuffer, IFileIO& fileIO, const std::string& jsonInputFile, const std::string& inputLvl, const std::string& outputLvlFile, const std::vector<std::string>& lvlResourceSources, bool skipCamerasAndFG1, Context& context)
{
    JsonMapRootInfoReader rootInfo;
    rootInfo.Read(fileIO, jsonInputFile);

    if (rootInfo.mMapRootInfo.mVersion != GetApiVersion())
    {
        context.JsonNeedsUpgrading(GetApiVersion(), rootInfo.mMapRootInfo.mVersion);
    }

    if (rootInfo.mMapRootInfo.mGame == "AO")
    {
        SaveBinaryPathToLvl<JsonReaderAO>(fileIO, Game::AO, fileDataBuffer, fileIO, jsonInputFile, inputLvl, outputLvlFile, skipCamerasAndFG1, false, context, lvlResourceSources);
    }
    else
    {
        SaveBinaryPathToLvl<JsonReaderAE>(fileIO, Game::AE, fileDataBuffer, fileIO, jsonInputFile, inputLvl, outputLvlFile, skipCamerasAndFG1, true, context, lvlResourceSources);
    }
}

[[nodiscard]] EnumeratePathsResult EnumeratePaths(std::vector<u8>& fileDataBuffer, IFileIO& fileIO, const std::string& inputLvlFile)
{
    EnumeratePathsResult ret = {};
    Game game = {};

    LvlReader lvl(fileIO, inputLvlFile.c_str());
    PathBND pathBnd = OpenPathBnd(lvl, fileDataBuffer, game, nullptr);
    ret.paths = pathBnd.mPaths;
    ret.pathBndName = pathBnd.mPathBndName;
    return ret;
}

void ExportPathBinaryToJson(std::vector<u8>& fileDataBuffer, IFileIO& fileIO, const std::string& jsonOutputFile, const std::string& inputLvlFile, s32 pathResourceId, Context& context)
{
    Game game = {};

    LvlReader lvl(fileIO, inputLvlFile.c_str());
    ReliveAPI::PathBND pathBnd = ReliveAPI::OpenPathBnd(lvl, fileDataBuffer, game, &pathResourceId);

    if (game == Game::AO)
    {
        JsonWriterAO doc(pathResourceId, pathBnd.mPathBndName, pathBnd.mPathInfo);
        doc.Save(fileDataBuffer, lvl, pathBnd.mPathInfo, pathBnd.mFileData, fileIO, jsonOutputFile, context);
    }
    else
    {
        JsonWriterAE doc(pathResourceId, pathBnd.mPathBndName, pathBnd.mPathInfo);
        doc.Save(fileDataBuffer, lvl, pathBnd.mPathInfo, pathBnd.mFileData, fileIO, jsonOutputFile, context);
    }
}

void ImportCameraAndFG1(std::vector<u8>& fileDataBuffer, LvlWriter& inputLvl, const std::string& camName, const CameraImageAndLayers& imageAndLayers, bool allowFullFG1Blocks, const std::vector<LvlFileChunk>& additionalResourceBlocks)
{
    const u32 bitsId = CamConverter::CamBitsIdFromName(camName);

    // Load existing .CAM if possible so existing additional resource blocks don't get removed by
    // re-creating the CAM from scratch.
    ChunkedLvlFile camFile;
    if (inputLvl.ReadFileInto(fileDataBuffer, camName.c_str()))
    {
        camFile = ChunkedLvlFile(fileDataBuffer);
    }

    // Arrage pixel data in strips and RGB888 -> RGB565
    struct CamStrip final
    {
        u16 mStripLen = 16 * 240 * sizeof(u16);
        u16 mStrip[240][16] = {};
    };

    struct CamImageStrips final
    {
    public:
        void SetPixel(u32 x, u32 y, u16 pixel)
        {
            const u32 stripNum = x / 16;
            const u32 xPixel = x % 16;
            mStrips[stripNum].mStrip[y][xPixel] = pixel;
        }

        std::vector<u8> ToVector() const
        {
            std::vector<u8> r;
            const u32 totalLengthDataSize = (640 / 16) * sizeof(u16);
            const u32 totalStripDataSize = (640 / 16) * (16 * 240) * sizeof(u16);
            r.resize(totalLengthDataSize + totalStripDataSize);

            u16* pStream = reinterpret_cast<u16*>(r.data());

            u32 pos = 0;
            for (u32 i = 0; i < 640 / 16; i++)
            {
                pStream[pos] = mStrips[i].mStripLen;
                pos++;

                memcpy(pStream + pos, &mStrips[i].mStrip[0][0], mStrips[i].mStripLen);
                pos += mStrips[i].mStripLen / sizeof(u16);
            }
            return r;
        }

    private:
        CamStrip mStrips[640 / 16];
    };

    // Some cams are valid but have no image e.g S1P01C23
    if (imageAndLayers.HaveCameraImage())
    {
        std::vector<u8> rawPixels = Base64Png2RawPixels(imageAndLayers.mCameraImage);
        auto bitsData = std::make_unique<CamImageStrips>(); // reduce stack usage
        for (u32 x = 0; x < 640; x++)
        {
            for (u32 y = 0; y < 240; y++)
            {
                const u32* pPixel32 = &reinterpret_cast<const u32*>(rawPixels.data())[To1dIndex(640, x, y)];
                bitsData->SetPixel(x, y, RGB888ToRGB565(reinterpret_cast<const u8*>(pPixel32)));
            }
        }

        LvlFileChunk bitsChunk(bitsId, ResourceManager::Resource_Bits, bitsData->ToVector());
        camFile.AddChunk(std::move(bitsChunk));
    }

    // Remove FG1 blocks
    camFile.RemoveChunksOfType(ResourceManager::Resource_FG1);

    // Add any additional resources if not already present
    for (const LvlFileChunk& chunkToAdd : additionalResourceBlocks)
    {
        if (!camFile.ChunkById(chunkToAdd.Id()))
        {
            camFile.AddChunk(chunkToAdd);
        }
    }

    if (imageAndLayers.HaveFG1Layers())
    {
        std::vector<u8> fg1Data = ConstructFG1Data(imageAndLayers, allowFullFG1Blocks);
        if (!fg1Data.empty())
        {
            // FG1 blocks use id BitsId << 8 + idx (or 255 max in this case as we only have 1 FG1 block)
            const u32 fg1ResId = (bitsId << 8) | 0xFF;

            LvlFileChunk fg1Chunk(fg1ResId, ResourceManager::Resource_FG1, std::move(fg1Data));

            // Add reconstructed single FG1 block
            camFile.AddChunk(std::move(fg1Chunk));
        }
    }

    // Add or update the CAM file
    inputLvl.AddFile(camName.c_str(), camFile.Data());
}

// Used by the integration tests - we don't want to publically expose LvlFileReader/Writer details
void ImportCameraAndFG1(std::vector<u8>& fileDataBuffer, LvlWriter& inputLvl, const std::string& camName, const CameraImageAndLayers& imageAndLayers, bool allowFullFG1Blocks)
{
    ImportCameraAndFG1(fileDataBuffer, inputLvl, camName, imageAndLayers, allowFullFG1Blocks, {});
}

[[nodiscard]] std::unique_ptr<ChunkedLvlFile> OpenPathBnd(IFileIO& fileIO, const std::string& inputLvlFile, std::vector<u8>& fileDataBuffer)
{
    Game game = {};
    s32* pathId = nullptr;
    LvlReader lvlReader(fileIO, inputLvlFile.c_str());
    PathBND pathBnd = OpenPathBnd(lvlReader, fileDataBuffer, game, pathId);
    return std::make_unique<ChunkedLvlFile>(pathBnd.mFileData);
}

} // namespace Detail


} // namespace ReliveAPI

