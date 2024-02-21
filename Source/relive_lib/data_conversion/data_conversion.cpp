#include "stdafx.h"
#include "data_conversion.hpp"
#include "file_system.hpp"
#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/Path.hpp"
#include "../../AliveLibAE/Font.hpp"
#include "../AnimResources.hpp"
#include "../Animation.hpp"
#include "../MapWrapper.hpp"
#include <algorithm>
#include <cctype>

#include "nlohmann/json.hpp"
#include "LvlReaderWriter.hpp"
#include "../../Tools/relive_api/file_api.hpp"
#include "../../Tools/relive_api/CamConverter.hpp"
#include "../../Tools/relive_api/PathCamerasEnumerator.hpp"
#include "../../Tools/relive_api/ApiFG1Reader.hpp"
#include "../Collisions.hpp"
#include "AnimationConverter.hpp"
#include "relive_tlvs_conversion.hpp"
#include "Collisions.hpp"
#include "AnimConversionInfo.hpp"
#include "PNGFile.hpp"
#include "AESaveConverter.hpp"
#include "../BinaryPath.hpp"
#include "../../relive_lib/ResourceManagerWrapper.hpp"
#include "fmv_converter.hpp"
#include "file_system.hpp"
#include "ThreadPool.hpp"
#include "TLVConverter.hpp"
#include "CollisionLineConverter.hpp"
#include "PathDataExtensionsTypes.hpp"

static bool ReadLvlFileInto(ReliveAPI::LvlReader& archive, const char_type* fileName, std::vector<u8>& fileBuffer)
{
    if (!archive.ReadFileInto(fileBuffer, fileName))
    {
        fileBuffer.clear();
        return false;
    }
    return true;
}

static bool endsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static void to_json(nlohmann::json& j, const CameraEntry& p)
{
    j = nlohmann::json{
        {"x", p.mX},
        {"y", p.mY},
        {"id", p.mId}, // TODO: Can probably get rid of this in the future
        {"name", p.mName},
    };
}

static void to_json(nlohmann::json& j, const PathSoundInfo& p)
{
    j = nlohmann::json{
        {"vh_file", p.mVhFile},
        {"vb_file", p.mVbFile},
        {"seq_files", p.mSeqFiles},
    };
}

bool SaveJson(const nlohmann::json& j, FileSystem& fs, const char_type* path)
{
    std::string jsonStr = j.dump(4);

    std::vector<u8> data;
    data.resize(jsonStr.length());
    data.assign(jsonStr.begin(), jsonStr.end());
    return fs.Save(path, data);
}

bool SaveJson(const nlohmann::json& j, FileSystem& fs, const FileSystem::Path& path)
{
    return SaveJson(j, fs, path.GetPath().c_str());
}

namespace AO { 
extern OpenSeqHandle g_SeqTable_4C9E70[165];
}
extern SeqHandleTable sSeqData;

static void HashBSQTable(OpenSeqHandle* pTable, s32 size)
{
    for (s32 i = 0; i < size; i++)
    {
        if (pTable[i].field_0_mBsqName)
        {
            pTable[i].field_4_generated_res_id = ResourceManagerWrapper::SEQ_HashName(pTable[i].field_0_mBsqName);
        }
    }
}

static const char_type* FileNameFromSEQId(OpenSeqHandle* pTable, s32 size, s32 id)
{
    for (s32 i = 0; i < size; i++)
    {
        if (pTable[i].field_4_generated_res_id == id)
        {
            return pTable[i].field_0_mBsqName;
        }
    }
    ALIVE_FATAL("Unknown SEQ id");
}

static std::vector<std::string> ConvertBSQ(const FileSystem::Path& dataDir, const char_type* pBSQName, ReliveAPI::LvlReader& lvlReader, bool isAo)
{
    auto bsqData = lvlReader.ReadFile(pBSQName);
    if (!bsqData)
    {
        ALIVE_FATAL("Missing BSQ");
    }

    static bool seqNamesHashed = false;
    if (!seqNamesHashed)
    {
        if (isAo)
        {
            HashBSQTable(AO::g_SeqTable_4C9E70, ALIVE_COUNTOF(AO::g_SeqTable_4C9E70));
        }
        else
        {
            HashBSQTable(sSeqData.mSeqs, ALIVE_COUNTOF(sSeqData.mSeqs));
        }
        seqNamesHashed = true;
    }

    std::vector<std::string> seqs;
    ReliveAPI::ChunkedLvlFile bsq(*bsqData);
    for (u32 i = 0u; i < bsq.ChunkCount(); i++)
    {
        const ReliveAPI::LvlFileChunk& chunk = bsq.ChunkAt(i);
        if (chunk.Header().mResourceType == ResourceManagerWrapper::Resource_Seq)
        {
            const char_type* pSeqName = nullptr;
            if (isAo)
            {
                pSeqName = FileNameFromSEQId(AO::g_SeqTable_4C9E70, ALIVE_COUNTOF(AO::g_SeqTable_4C9E70), chunk.Header().field_C_id);
            }
            else
            {
                pSeqName = FileNameFromSEQId(sSeqData.mSeqs, ALIVE_COUNTOF(sSeqData.mSeqs), chunk.Header().field_C_id);
            }
            seqs.push_back(pSeqName);

            FileSystem::Path filePath = dataDir;
            filePath.Append(pSeqName);

            FileSystem fs;
            fs.Save(filePath, chunk.Data());
        }
    }
    return seqs;
}

template <typename LevelIdType>
static void SaveFileFromLvlDirect(const char_type* pFileName, const FileSystem::Path& dataDir, ReliveAPI::LvlReader& lvlReader, LevelIdType lvlIdxAsLvl, std::vector<u8>& fileBuffer)
{
    ReadLvlFileInto(lvlReader, pFileName, fileBuffer);

    FileSystem::Path filePath = dataDir;
    filePath.Append(ToString(lvlIdxAsLvl));
    
    FileSystem fs;
    fs.CreateDirectory(filePath);
    filePath.Append(pFileName);

    fs.Save(filePath, fileBuffer);
}

static void SetCollisionInfoFromPathExt(CollisionInfo& pColInfo, PerPathExtension* pathExt)
{
    pColInfo.mLeft = 0;
    pColInfo.mRight = 0;
    pColInfo.mTop = static_cast<s16>(pathExt->mXSize * pathExt->mGridWidth);
    pColInfo.mBottom = static_cast<s16>(pathExt->mYSize * pathExt->mGridHeight);
    pColInfo.mCollisionOffset = pathExt->mCollisionOffset;
    pColInfo.mNumCollisionItems = pathExt->mNumCollisionLines;
    pColInfo.mGridWidth = pathExt->mGridWidth;
    pColInfo.mGridHeight = pathExt->mGridHeight;
}

template <typename TlvType, typename LevelIdType>
static void ConvertPath(FileSystem& fs, const FileSystem::Path& path, const ReliveAPI::LvlFileChunk& pathBndChunk, EReliveLevelIds reliveLvl, LevelIdType lvlIdx, ReliveAPI::LvlReader& lvlReader, std::vector<u8>& fileBuffer, bool isAo, PerPathExtension* pPathExt)
{
    auto level = (isAo ? ToString(MapWrapper::ToAO(reliveLvl)) : ToString(MapWrapper::ToAE(reliveLvl)));
    LOG_INFO("Converting: %s; path %d", level, pathBndChunk.Id());

    s32 width = 0;
    s32 height = 0;
    CollisionInfo collisionInfo = {};
    s32 indexTableOffset = 0;
    s32 objectOffset = 0;

    if (pPathExt)
    {
        width = pPathExt->mXSize;
        height = pPathExt->mYSize;

        objectOffset = pPathExt->mObjectOffset;
        indexTableOffset = pPathExt->mIndexTableOffset;

        SetCollisionInfoFromPathExt(collisionInfo, pPathExt);
    }
    else
    {
        if (isAo)
        {
            const AO::PathBlyRec* pBlyRec = AO::Path_Get_Bly_Record(reliveLvl, static_cast<u16>(pathBndChunk.Id()));

            // Save cameras and map objects
            width = (pBlyRec->field_4_pPathData->field_8_bTop - pBlyRec->field_4_pPathData->field_4_bLeft) / pBlyRec->field_4_pPathData->field_C_grid_width;
            height = (pBlyRec->field_4_pPathData->field_A_bBottom - pBlyRec->field_4_pPathData->field_6_bRight) / pBlyRec->field_4_pPathData->field_E_grid_height;
            collisionInfo = *pBlyRec->field_8_pCollisionData;
            indexTableOffset = pBlyRec->field_4_pPathData->field_18_object_index_table_offset;
            objectOffset = pBlyRec->field_4_pPathData->field_14_object_offset;
        }
        else
        {
            const ::PathBlyRec* pBlyRec = ::Path_Get_Bly_Record(reliveLvl, static_cast<u16>(pathBndChunk.Id()));

            // Save cameras and map objects
            width = (pBlyRec->field_4_pPathData->field_4_bTop - pBlyRec->field_4_pPathData->field_0_bLeft) / pBlyRec->field_4_pPathData->field_A_grid_width;
            height = (pBlyRec->field_4_pPathData->field_6_bBottom - pBlyRec->field_4_pPathData->field_2_bRight) / pBlyRec->field_4_pPathData->field_C_grid_height;
            collisionInfo = *pBlyRec->field_8_pCollisionData;
            indexTableOffset = pBlyRec->field_4_pPathData->field_16_object_indextable_offset;
            objectOffset = pBlyRec->field_4_pPathData->field_12_object_offset;
        }
    }

    nlohmann::json camerasArray = nlohmann::json::array();
    PathCamerasEnumerator cameraEnumerator(width, height, pathBndChunk.Data());
    cameraEnumerator.Enumerate([&](const CameraEntry& tmpCamera)
                               {
        // Save map objects
        nlohmann::json mapObjectsArray = nlohmann::json::array();
        const u32 indexTableIdx = To1dIndex(width, tmpCamera.mX, tmpCamera.mY);
        //LOG_INFO(indexTableIdx);
        ConvertPathTLVs<TlvType>(mapObjectsArray, pathBndChunk.Id(), lvlIdx, indexTableOffset, objectOffset, pathBndChunk.Data(), indexTableIdx);

        // Its possible to have a camera with no objects (-1 index table)
        // But its also possible to have a blank camera with objects (blank camera name, non -1 index table)
        if (!tmpCamera.mName.empty() || !mapObjectsArray.empty())
        { 
            nlohmann::json camJson;
            to_json(camJson, tmpCamera);
            camJson["map_objects"] = mapObjectsArray;
            camerasArray.push_back(camJson);
        } });

    // Save collisions
    nlohmann::json collisionsArray = nlohmann::json::array();
    ConvertPathCollisions(collisionsArray, collisionInfo, pathBndChunk.Data(), isAo);

    FileSystem::Path seqsDir = path;
    seqsDir.Append(ToString(lvlIdx));
    fs.CreateDirectory(seqsDir);

    // Save sound info (per path rather than per LVL)
    PathSoundInfo soundInfo;
    if (isAo)
    {
        const AO::SoundBlockInfo* pSoundBlock = AO::Path_Get_MusicInfo(reliveLvl);
        soundInfo.mVhFile = pSoundBlock->field_0_vab_header_name;
        soundInfo.mVbFile = pSoundBlock->field_4_vab_body_name;
        soundInfo.mSeqFiles = ConvertBSQ(seqsDir, AO::Path_Get_BsqFileName(reliveLvl), lvlReader, isAo);
    }
    else
    {
        const SoundBlockInfo* pSoundBlock = Path_Get_MusicInfo(reliveLvl);

        // TODO: Convert to AO format instead of using sounds.dat for now (in the vh/vb/bsq copy)
        soundInfo.mVhFile = pSoundBlock->field_0_vab_header_name;
        soundInfo.mVbFile = pSoundBlock->field_4_vab_body_name;

        soundInfo.mSeqFiles = ConvertBSQ(seqsDir, Path_Get_BsqFileName(reliveLvl), lvlReader, isAo);

        // TODO
        //Path_Get_BackGroundMusicId(reliveLvl);
        //Path_Get_Reverb(reliveLvl);

        // TODO: Makes more sense to calculate this on loading by summing up the mud count(s)?
        //Path_GetMudsInLevel(reliveLvl, pathBndChunk.Id());
    }

    SaveFileFromLvlDirect(soundInfo.mVhFile.c_str(), path, lvlReader, lvlIdx, fileBuffer);
    SaveFileFromLvlDirect(soundInfo.mVbFile.c_str(), path, lvlReader, lvlIdx, fileBuffer);

    nlohmann::json j = {
        {"path_version", DataConversion::DataVersions::LatestVersion().mPathVersion},
        {"game", isAo ? "AO" : "AE"},
        {"map", {
            {"path_id", pathBndChunk.Id()}, // TODO: Try to remove this in the future, ids make adding new paths a bit more tricky
            {"cameras", camerasArray},
            {"collisions", collisionsArray},
            {"sound_info", soundInfo},
        }}
    };

    FileSystem::Path pathJsonFile = path;
    pathJsonFile.Append(ToString(lvlIdx)).Append("paths").Append(std::to_string(pathBndChunk.Id()));
    fs.CreateDirectory(pathJsonFile);
    pathJsonFile.Append(std::to_string(pathBndChunk.Header().field_C_id) + ".json");
    SaveJson(j, fs, pathJsonFile);
}


template <typename LevelIdType>
static void SaveLevelInfoJson(const FileSystem::Path& dataDir, EReliveLevelIds /*reliveLvl*/, LevelIdType lvlIdxAsLvl, FileSystem& fs, const ReliveAPI::ChunkedLvlFile& pathBndFile, bool /* isAo*/)
{
    FileSystem::Path pathDir = dataDir;
    pathDir.Append(ToString(lvlIdxAsLvl)).Append("paths");
    fs.CreateDirectory(pathDir);

    FileSystem::Path pathJsonFile = pathDir;
    pathJsonFile.Append("level_info.json");

    nlohmann::json jsonPathFilesArray;

    // Convert hard coded path data json
    for (u32 j = 0; j < pathBndFile.ChunkCount(); j++)
    {
        const ReliveAPI::LvlFileChunk& pathBndChunk = pathBndFile.ChunkAt(j);
        if (pathBndChunk.Header().mResourceType == ResourceManagerWrapper::Resource_Path)
        {
            nlohmann::json pathInfoObj = nlohmann::json::object();

            std::string pathId = std::to_string(pathBndChunk.Header().field_C_id);
            pathInfoObj["file"] = pathId + ".json";
            pathInfoObj["path_id"] = pathId;

            // Write out what paths exist so the game knows what files to load
            jsonPathFilesArray.push_back(pathInfoObj);
        }
    }

    // TODO: Misc info, back ground music id, reverb etc

    // TODO: Colour tints per object in this lvl

    // TODO: Music info - incl each SEQ name
    /*
    AO::SoundBlockInfo* pSoundBlock = AO::Path_Get_MusicInfo(reliveLvl);
    pSoundBlock->
    */

    nlohmann::json j;
    j["paths"] = jsonPathFilesArray;

    SaveJson(j, fs, pathJsonFile);
}

extern PalRecConversionInfo kPalConversionInfo[17];

static void ConvertPal(const FileSystem::Path& dataDir, const char* pFileName, const u16* pData, u32 len);

static void LogNonConvertedPals(bool isAo)
{
    for (auto& rec : kPalConversionInfo)
    {
        if (!rec.mConverted)
        {
            const auto palDetails = isAo ? AO::PalRec(rec.mPalId) : PalRec(rec.mPalId);
            if (palDetails.mBanName)
            {
                LOG_INFO("MISSING PAL: %s", ToString(rec.mPalId));
            }
        }
    }
}

static void ConvertPals(const FileSystem::Path& dataDir, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, bool isAo)
{
    for (auto& rec : kPalConversionInfo)
    {
        if (!rec.mConverted)
        {
            const auto palDetails = isAo ? AO::PalRec(rec.mPalId) : PalRec(rec.mPalId);
            if (palDetails.mBanName)
            {
                // Not every file is in every LVL - we might get it from a later LVL
                if (ReadLvlFileInto(lvlReader, palDetails.mBanName, fileBuffer))
                {
                    // A BAN/BND can have multiple chunks, make sure we pick the right one
                    ReliveAPI::ChunkedLvlFile palFile(fileBuffer);
                    for (u32 i = 0; i < palFile.ChunkCount(); i++)
                    {
                        const auto& res = palFile.ChunkAt(i);
                        if (res.Header().mResourceType == ResourceManagerWrapper::Resource_Palt && static_cast<s32>(res.Header().field_C_id) == palDetails.mResourceId)
                        {
                            LOG_INFO("Converting PAL: %s", ToString(rec.mPalId));

                            const auto& palData = res.Data();
                            u32 palLen = *reinterpret_cast<const u32*>(palData.data());
                            const u16* pPalData = reinterpret_cast<const u16*>(palData.data()) + 2;

                            ConvertPal(dataDir, ToString(rec.mPalId), pPalData, palLen);
                            rec.mConverted = true;
                        }
                    }
                }
            }
        }
    }
}


void ConvertAnimations(const FileSystem::Path& dataDir, FileSystem& fs, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, bool isAo)
{
    // Convert animations that exist in this LVL
    for (auto& rec : kAnimRecConversionInfo)
    {
        const auto animDetails = isAo ? AO::AnimRec(rec.mAnimId) : AnimRec(rec.mAnimId);

        // Skip animations that are only present in AO or AE
        if ((isAo && rec.mAoLvl == EReliveLevelIds::eNone) || (!isAo && rec.mAeLvl == EReliveLevelIds::eNone))
        {
            continue;
        }

        // Animation is in this LVL and not yet converted
        if (!rec.mConverted)
        {
            if ((isAo && rec.mAoLvl == reliveLvl) || (!isAo && rec.mAeLvl == reliveLvl))
            {
                // Not every file is in every LVL - we might get it from a later LVL
                if (ReadLvlFileInto(lvlReader, animDetails.mBanName, fileBuffer))
                {
                    // A BAN/BND can have multiple chunks, make sure we pick the right one
                    ReliveAPI::ChunkedLvlFile animFile(fileBuffer);
                    auto res = animFile.ChunkById(animDetails.mResourceId);
                    if (res)
                    {
                        const char_type* animName = AnimRecName(rec.mAnimId);
                        const char_type* groupName = AnimRecGroupName(rec.mAnimId);

                        LOG_INFO("Converting: %s", animName);

                        FileSystem::Path filePath = dataDir;
                        filePath.Append("animations");

                        // e.g "abe"
                        filePath.Append(groupName);

                        // Ensure the containing directory exists
                        fs.CreateDirectory(filePath);

                        // e.g "arm_gib"
                        filePath.Append(animName);

                        AnimationConverter animationConverter(filePath, animDetails, res->Data(), isAo);

                        // Track what is converted so we know what is missing at the end
                        rec.mConverted = true;
                    }
                }
            }
        }
    }
}

static void LogNonConvertedAnims(bool isAo)
{
    // Log skippeed/missing anims
    for (auto& rec : kAnimRecConversionInfo)
    {
        const auto animDetails = isAo ? AO::AnimRec(rec.mAnimId) : AnimRec(rec.mAnimId);
        if (animDetails.mFrameTableOffset != 0)
        {
            if (!rec.mConverted)
            {
                LOG_INFO("Didn't convert %s", AnimRecName(rec.mAnimId));
            }
        }
    }
}

template <typename LevelIdType, typename TlvType>
static void ConvertPathBND(const FileSystem::Path& dataDir, const std::string& fileName, FileSystem& fs, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, LevelIdType lvlIdxAsLvl, EReliveLevelIds reliveLvl, bool isAo)
{
    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);
    ReliveAPI::ChunkedLvlFile pathBndFile(fileBuffer);

    std::vector<PerPathExtension> pathsWithExt;
    for (u32 i = 0; i < pathBndFile.ChunkCount(); i++)
    {
        const ReliveAPI::LvlFileChunk& pathBndChunk = pathBndFile.ChunkAt(i);
        
        if (pathBndChunk.Header().mResourceType == ResourceManagerWrapper::ResourceType::Resource_Pxtd)
        {
            auto dataCopy = pathBndChunk.Data();
            auto pExt = reinterpret_cast<PerPathExtension*>(dataCopy.data());
            if (pExt->mSize != sizeof(PerPathExtension))
            {
                LOG_INFO("%s expected size %d but got %d", pExt->mBlyName, sizeof(PerPathExtension), pExt->mSize);
            }
            else
            {
                pathsWithExt.push_back(*pExt);

                auto pChunkData = reinterpret_cast<u8*>(pExt);
                pChunkData += sizeof(PerPathExtension);

                // TODO: do something with the strings
                auto pLCDScreenMsgs = reinterpret_cast<StringTable*>(pChunkData);
                pChunkData = StringTable::MakeTable(pLCDScreenMsgs);

                // Will be empty for AE
                auto pHintFlyMsgs = reinterpret_cast<StringTable*>(pChunkData);
                pChunkData = StringTable::MakeTable(pHintFlyMsgs);
            }
        }
    }

    for (u32 j = 0; j < pathBndFile.ChunkCount(); j++)
    {
        const ReliveAPI::LvlFileChunk& pathBndChunk = pathBndFile.ChunkAt(j);
        PerPathExtension* pPathExt = nullptr;

        if (pathBndChunk.Header().mResourceType == ResourceManagerWrapper::Resource_Path)
        {
            for (PerPathExtension extPath : pathsWithExt)
            {
                if (pathBndChunk.Id() == extPath.mPathId)
                {
                    pPathExt = &extPath;
                    break;
                }
            }

            ConvertPath<TlvType, LevelIdType>(fs, dataDir, pathBndChunk, reliveLvl, lvlIdxAsLvl, lvlReader, fileBuffer, isAo, pPathExt);
        }
    }

    SaveLevelInfoJson(dataDir, reliveLvl, lvlIdxAsLvl, fs, pathBndFile, isAo);
}

/*static*/ void SaveCameraJsonManifest(const std::string& baseName, ReliveAPI::ApiFG1Reader& reader, const FileSystem::Path& dataDir, u32 fg1ResBlockCount)
{
    nlohmann::json camManifest;
    nlohmann::json layersArray;
    for (u32 i = 0; i < 4; i++)
    {
        if (reader.LayerUsed(i))
        {
            layersArray.push_back(baseName + reader.NameForLayer(i) + ".png");
        }
    }
    camManifest["layers"] = layersArray;
    if (layersArray.empty())
    {
        // The FG1 blocks result in no actual FG1 data, add yet another fake/dummy
        // block so the FG1 object count matches OG for playbacks
        fg1ResBlockCount++;
    }
    camManifest["fg1_block_count"] = fg1ResBlockCount;

    FileSystem fs;
    SaveJson(camManifest, fs, dataDir);
}

class ConvertCameraJob final : public IJob
{
public:
    ConvertCameraJob(FileSystem::Path dirToSaveConvertedCamIn,
                     FileSystem::Path jsonFileName,
                     std::string camNameWithoutExtension,
                     ReliveAPI::ChunkedLvlFile camFileData,
                     bool isAo)
        : mDirToSaveConvertedCamIn(dirToSaveConvertedCamIn)
        , mJsonFileName(jsonFileName)
        , mCamNameWithoutExtension(camNameWithoutExtension)
        , mCamFileData(camFileData)
        , mIsAo(isAo)
    {

    }

    void Execute() override
    {
        // Convert camera images and FG layers
        ReliveAPI::CamConverter cc;
        auto fg1ReaderAndBlockCount = cc.Convert(mCamFileData, mDirToSaveConvertedCamIn.GetPath(), mIsAo);
        if (fg1ReaderAndBlockCount.first)
        {
            SaveCameraJsonManifest(mCamNameWithoutExtension, *fg1ReaderAndBlockCount.first, mJsonFileName, fg1ReaderAndBlockCount.second);
        }
    }

private:
    FileSystem::Path mDirToSaveConvertedCamIn;
    FileSystem::Path mJsonFileName;
    std::string mCamNameWithoutExtension;
    ReliveAPI::ChunkedLvlFile mCamFileData;
    bool mIsAo = false;
};

template <typename LevelIdType>
static void ConvertCamera(ThreadPool& tp, const FileSystem::Path& dataDir, const std::string& fileName, FileSystem& fs, std::vector<u8>& fileBuffer, ReliveAPI::LvlReader& lvlReader, LevelIdType lvlIdxAsLvl, bool isAo)
{
    LOG_INFO("%s", fileName.c_str());
    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);

    ReliveAPI::ChunkedLvlFile camFileData(fileBuffer);

    std::string camNameWithoutExtension = fileName.substr(0, fileName.length() - 4); // chop off .CAM
    camNameWithoutExtension = camNameWithoutExtension.substr(2); // Chop off the 2 letter lvl prefix

    FileSystem::Path dirToSaveConvertedCamIn = dataDir;

    dirToSaveConvertedCamIn.Append(ToString(lvlIdxAsLvl));

    // extract the path id from the cam name
    std::string pathId = std::to_string(std::stoi(camNameWithoutExtension.substr(0, camNameWithoutExtension.length() - 3).substr(1)));

    FileSystem::Path jsonFileName = dirToSaveConvertedCamIn;
    jsonFileName.Append("paths").Append(pathId).Append(camNameWithoutExtension + ".json");
    
    dirToSaveConvertedCamIn.Append("paths").Append(pathId); 
    fs.CreateDirectory(dirToSaveConvertedCamIn);
    dirToSaveConvertedCamIn.Append(camNameWithoutExtension);

    tp.AddJob(std::make_unique<ConvertCameraJob>(dirToSaveConvertedCamIn, jsonFileName, camNameWithoutExtension, camFileData, isAo));
}

static void ConvertFont(const FileSystem::Path& dataDir, const std::string& fileName, ReliveAPI::LvlReader& lvlReader, std::vector<u8>& fileBuffer, bool isPauseMenuFont)
{
    ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer);

    ReliveAPI::ChunkedLvlFile camFile(fileBuffer);

     std::optional<ReliveAPI::LvlFileChunk> font = camFile.ChunkByType(ResourceManagerWrapper::Resource_Font);
     if (!font)
     {
         ALIVE_FATAL("Font missing");
     }

     auto fontFile = reinterpret_cast<const File_Font*>(font->Data().data());

     FileSystem::Path path = dataDir;
     if (isPauseMenuFont)
     {
         path.Append("pause_menu_font.png");
     }
     else
     {
         path.Append("lcd_font.png");
     }
     PNGFile png;
     AnimationPal pal = {};

     for (s32 i = 0; i < 16; i++)
     {
         RGBA32 pixel = RGBConversion::RGBA555ToRGBA888Components(fontFile->mPalette[i]);

         pal.mPal[i].r = pixel.r;
         pal.mPal[i].g = pixel.g;
         pal.mPal[i].b = pixel.b;
         pal.mPal[i].a = pixel.a;
     }

     std::vector<u8> newData((fontFile->mWidth / 2) * fontFile->mHeight * 2);

     // Expand 4bit to 8bit
     std::size_t src = 0;
     std::size_t dst = 0;
     while (dst < newData.size())
     {
         newData[dst++] = (fontFile->mPixelBuffer[src] & 0xF);
         newData[dst++] = ((fontFile->mPixelBuffer[src++] & 0xF0) >> 4);
     }

     if (!isPauseMenuFont)
     {
         // Make it a square texture
         std::vector<u8> newDataTmp(256 * 256);
         for (s32 x = 0; x < fontFile->mWidth; x++)
         {
             for (s32 y = 0; y < fontFile->mHeight; y++)
             {
                 const u32 srcIdx = (y * fontFile->mWidth) + x;
                 const u32 dstIdx = (y * 256) + x;
                 newDataTmp[dstIdx] = newData[srcIdx];
             }
         }

         png.Save(path.GetPath().c_str(), pal, newDataTmp, 256, 256);
     }
     else
     {
         png.Save(path.GetPath().c_str(), pal, newData, fontFile->mWidth, fontFile->mHeight);
     }

     // TODO: Dump out the atlas for each char
}

static bool IsUnusedSaveFile(const std::string& saveName)
{
     static const char* unusedSaves[] = {
                                         // Demo playback saves
                                         "ATTR0011.SAV",
                                         "ATTR0013.SAV",
                                         "ATTR0020.SAV",
                                         "ATTR0021.SAV",
                                         "ATTR0026.SAV",
                                         
                                         // Path skip cheat saves
                                         "NXTP0000.SAV",
                                         "NXTP0001.SAV",
                                         "NXTP0323.SAV",
                                         "NXTP0422.SAV"};

     for (const auto& unusedSave : unusedSaves)
     {
         if (unusedSave == saveName)
         {
             return true;
         }
     }
     return false;
}

template<typename LevelIdType, typename TlvType>
static void ConvertFilesInLvl(ThreadPool& tp, const FileSystem::Path& dataDir, FileSystem& fs, ReliveAPI::LvlReader& lvlReader, std::vector<u8>& fileBuffer, LevelIdType lvlIdxAsLvl, EReliveLevelIds reliveLvl, const DataConversion::DataVersions& dv, bool isAo, bool onlySaves)
{
    // Iterate and convert specific file types in the LVL
    AESaveConverter::PathsCache pathsCache;
    for (s32 i = 0; i < lvlReader.FileCount(); i++)
    {
        auto fileName = lvlReader.FileNameAt(i);
        if (fileName.empty())
        {
            continue;
        }

        const bool bConvertLcdFont = endsWith(fileName, ".FNT") && dv.ConvertFonts() && fileName == "LCDFONT.FNT";
        const bool bConvertMenuFonts = dv.ConvertFonts() && (fileName == "S1P01C01.CAM" || fileName == "STP01C06.CAM");
        const bool bConvertDemos = endsWith(fileName, ".JOY") && dv.ConvertDemos();
        const bool bConvertPaths = endsWith(fileName, "PATH.BND") && dv.ConvertPaths();
        const bool bConvertCams = dv.ConvertCameras();

        if (onlySaves)
        {
            // caller checks if save conv is enabled
            if (!endsWith(fileName, ".SAV") || IsUnusedSaveFile(fileName) || isAo)
            {
                continue;
            }

            if (!ReadLvlFileInto(lvlReader, fileName.c_str(), fileBuffer))
            {
                continue;
            }

            // Remove the resource header
            fileBuffer.erase(fileBuffer.begin(), fileBuffer.begin() + 16);

            // TODO: Actually convert at some later point
            AESaveConverter saveConverter;
            saveConverter.Convert(fileBuffer, (fileName + ".json").c_str(), pathsCache);
        }
        else
        {
            if (bConvertLcdFont)
            {
                ConvertFont(dataDir, fileName, lvlReader, fileBuffer, false);
            }
            else if (endsWith(fileName, ".CAM"))
            {
                if (bConvertMenuFonts)
                {
                    ConvertFont(dataDir, fileName, lvlReader, fileBuffer, true);
                }

                if (bConvertCams)
                {
                    ConvertCamera(tp, dataDir, fileName, fs, fileBuffer, lvlReader, lvlIdxAsLvl, isAo);
                }
            }
            else if (bConvertDemos)
            {
                // TODO: Actually convert at some later point
                SaveFileFromLvlDirect(fileName.c_str(), dataDir, lvlReader, lvlIdxAsLvl, fileBuffer);
            }
            else if (bConvertPaths)
            {
                ConvertPathBND<LevelIdType, TlvType>(dataDir, fileName, fs, fileBuffer, lvlReader, lvlIdxAsLvl, reliveLvl, isAo);
            }
        }
    }
}


static void SavePal(const AnimationPal& pal, const FileSystem::Path& fileName)
{
    AutoFILE f;
    f.Open(fileName.GetPath().c_str(), "wb", false);
    f.Write(pal.mPal);
}

static void ConvertPal(const FileSystem::Path& dataDir, const char* pFileName, const u16* pData, u32 len)
{
    FileSystem::Path palFilePath = dataDir;
    palFilePath.Append(pFileName);

    AnimationPal pal;
    for (u32 i = 0; i < len; i++)
    {
        RGBA32 pixel = RGBConversion::RGBA555ToRGBA888Components(pData[i]);

        pal.mPal[i].r = pixel.r;
        pal.mPal[i].g = pixel.g;
        pal.mPal[i].b = pixel.b;
        pal.mPal[i].a = pixel.a;
    }
    SavePal(pal, palFilePath);
}

static void ConvertHardcodedPals(const FileSystem::Path& dataDir)
{
    const static u8 mainMenuFontPal[] = {
        0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
        0xA5, 0x94, 0xE7, 0x9C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
        0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
        0x73, 0x4E};

    const static u8 pauseMenuFontPal[] = {
        0x00, 0x00, 0x21, 0x84, 0x42, 0x88, 0x63, 0x8C, 0x84, 0x90,
        0xA5, 0x14, 0xE7, 0x1C, 0x08, 0x21, 0x29, 0x25, 0x4A, 0x29,
        0x6B, 0x2D, 0x8C, 0x31, 0xAD, 0x35, 0xEF, 0x3D, 0x10, 0x42,
        0x73, 0x4E};

    const static u8 pal_ColourfulMeter[32] = {
        0u, 0u, 1u, 128u, 1u, 132u, 32u, 132u, 33u,
        128u, 32u, 132u, 33u, 132u, 101u, 206u, 101u, 140u,
        140u, 177u, 19u, 148u, 100u, 206u, 101u, 206u, 215u,
        152u, 20u, 161u, 24u, 216u};

    const static u8 sLCDScreen_Palette[] = {
        0x00, 0x00, 0x01, 0x80, 0x01, 0x84, 0x20, 0x84, 0x21, 0x80,
        0x20, 0x84, 0x21, 0x84, 0x65, 0xCE, 0x65, 0x8C, 0x8C, 0xB1,
        0x60, 0x8E, 0x64, 0xCE, 0x65, 0xCE, 0xD7, 0x98, 0x14, 0xA1,
        0x18, 0xD8};

    const static u8 sLCDScreen_Palette2[] = {
        0x00, 0x00, 0x01, 0x80, 0x01, 0x84, 0x20, 0x84, 0x21, 0x80,
        0x20, 0x84, 0x21, 0x84, 0x05, 0x84, 0x65, 0x8C, 0x8C, 0xB1,
        0x13, 0x94, 0x64, 0xCE, 0x65, 0xCE, 0xD7, 0x98, 0x14, 0xA1,
        0x18, 0xD8};

    const static u8 pal_LCDStatusBoard[] = {
        0x00, 0x00, 0x01, 0x80, 0x01, 0x84, 0x20, 0x84, 0x21, 0x80,
        0x20, 0x84, 0x21, 0x84, 0x65, 0xCE, 0x65, 0x8C, 0x8C, 0xB1,
        0x13, 0x94, 0x64, 0xCE, 0x65, 0xCE, 0xD7, 0x98, 0x14, 0xA1,
        0x18, 0xD8};


    ConvertPal(dataDir, ToString(PalId::MainMenuFont_MainMenu), reinterpret_cast<const u16*>(mainMenuFontPal), ALIVE_COUNTOF(mainMenuFontPal) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::MainMenuFont_PauseMenu), reinterpret_cast<const u16*>(pauseMenuFontPal), ALIVE_COUNTOF(pauseMenuFontPal) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::LedFont_ColourfulMeter), reinterpret_cast<const u16*>(pal_ColourfulMeter), ALIVE_COUNTOF(pal_ColourfulMeter) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::LedFont_1), reinterpret_cast<const u16*>(sLCDScreen_Palette), ALIVE_COUNTOF(sLCDScreen_Palette) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::LedFont_2), reinterpret_cast<const u16*>(sLCDScreen_Palette2), ALIVE_COUNTOF(sLCDScreen_Palette2) / sizeof(u16));

    ConvertPal(dataDir, ToString(PalId::LedFont_Red), reinterpret_cast<const u16*>(pal_LCDStatusBoard), ALIVE_COUNTOF(pal_LCDStatusBoard) / sizeof(u16));
}

template<typename FnOnLvl>
static void IterateAELvls(FnOnLvl fnOnLvl)
{
    for (s32 lvlIdx = 0; lvlIdx < Path_Get_Paths_Count(); lvlIdx++)
    {
        // Skip entries that have no data
        const ::LevelIds lvlIdxAsLvl = static_cast<::LevelIds>(lvlIdx);
        if (lvlIdxAsLvl == ::LevelIds::eTestLevel_15)
        {
            continue;
        }

        // Open the LVL file
        const EReliveLevelIds reliveLvl = MapWrapper::FromAE(lvlIdxAsLvl);
        ReliveAPI::FileIO fileIo;

        auto lvName = std::string(::Path_Get_Lvl_Name(reliveLvl)) + ".LVL";
        ReliveAPI::LvlReader lvlReader(fileIo, lvName.c_str());

        if (!lvlReader.IsOpen())
        {
            // Fatal, missing LVL file
            ALIVE_FATAL("Couldn't open lvl file");
        }
        fnOnLvl(lvlReader, reliveLvl, lvlIdxAsLvl);
    }
}

template<typename FnOnLvl>
static void IterateAOLvls(FnOnLvl fnOnLvl)
{
    for (s32 lvlIdx = 0; lvlIdx < AO::Path_Get_Paths_Count(); lvlIdx++)
    {
        // Skip entries that have no data
        const AO::LevelIds lvlIdxAsLvl = static_cast<AO::LevelIds>(lvlIdx);
        if (lvlIdxAsLvl == AO::LevelIds::eRemoved_7 || lvlIdxAsLvl == AO::LevelIds::eRemoved_11)
        {
            continue;
        }

        const EReliveLevelIds reliveLvl = MapWrapper::FromAO(lvlIdxAsLvl);
        ReliveAPI::FileIO fileIo;
        ReliveAPI::LvlReader lvlReader(fileIo, (std::string(AO::Path_Get_Lvl_Name(reliveLvl)) + ".LVL").c_str(), false);

        if (!lvlReader.IsOpen())
        {
            // Fatal, missing LVL file
            //ALIVE_FATAL("Couldn't open lvl file");

            LOG_WARNING("Couldn't open lvl file");
        }

        fnOnLvl(lvlReader, reliveLvl, lvlIdxAsLvl);
    }
}

DataConversion::DataConversion()
{
    mThreadPool = std::make_unique<ThreadPool>();
}

DataConversion::~DataConversion()
{

}

bool DataConversion::AsyncTasksInProgress() const
{
    return mThreadPool->Busy();
}

std::optional<DataConversion::DataVersions> DataConversion::DataVersionAO()
{
    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ao");
    DataVersions dv;
    return dv.Load(dataDir) ? std::make_optional(dv) : std::nullopt;
}

std::optional<DataConversion::DataVersions> DataConversion::DataVersionAE()
{
    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ae");
    DataVersions dv;
    return dv.Load(dataDir) ? std::make_optional(dv) : std::nullopt;
}

static void WriteDataVersion(const FileSystem::Path& path)
{
    const DataConversion::DataVersions dv = DataConversion::DataVersions::LatestVersion();
    dv.Save(path);
}

void DataConversion::ConvertDataAO(const DataVersions& dv)
{
    FileSystem fs;

    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ao");
    fs.CreateDirectory(dataDir);

    if (dv.ConvertFmvs())
    {
        ConvertFMVs(dataDir, true);
    }

    // TODO: Prob diff data in AO, check me
    if (dv.ConvertPalettes())
    {
        ConvertHardcodedPals(dataDir);
    }

    std::vector<u8> fileBuffer;
    IterateAOLvls([&](ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, AO::LevelIds lvlIdxAsLvl)
    {
        if (dv.ConvertAnimations())
        {
            ConvertAnimations(dataDir, fs, fileBuffer, lvlReader, reliveLvl, true);
        }

        if (dv.ConvertPalettes())
        {
            ConvertPals(dataDir, fileBuffer, lvlReader, true);
        }

        ConvertFilesInLvl<AO::LevelIds, AO::Path_TLV>(*mThreadPool, dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, dv, true, false);
    });

    if (dv.ConvertSaves())
    {
        IterateAOLvls([&](ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, AO::LevelIds lvlIdxAsLvl)
        {
            ConvertFilesInLvl<AO::LevelIds, AO::Path_TLV>(*mThreadPool, dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, dv, true, true);
        });
    }

    WriteDataVersion(dataDir);

    LogNonConvertedAnims(true);
    LogNonConvertedPals(true);
}

void DataConversion::ConvertDataAE(const DataVersions& dv)
{
    FileSystem fs;

    FileSystem::Path dataDir;
    dataDir.Append("relive_data");
    dataDir.Append("ae");
    fs.CreateDirectory(dataDir);

    if (dv.ConvertFmvs())
    {
        ConvertFMVs(dataDir, false);
    }

    if (dv.ConvertPalettes())
    {
        ConvertHardcodedPals(dataDir);
    }

    std::vector<u8> fileBuffer;
    IterateAELvls([&](ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, LevelIds lvlIdxAsLvl) 
    {
        if (dv.ConvertAnimations())
        {
            ConvertAnimations(dataDir, fs, fileBuffer, lvlReader, reliveLvl, false);
        }

        if (dv.ConvertPalettes())
        {
            ConvertPals(dataDir, fileBuffer, lvlReader, false);
        }

        ConvertFilesInLvl<::LevelIds, ::Path_TLV>(*mThreadPool, dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, dv, false, false);
    });

    if (dv.ConvertSaves())
    {
        IterateAELvls([&](ReliveAPI::LvlReader& lvlReader, EReliveLevelIds reliveLvl, LevelIds lvlIdxAsLvl) 
        { 
            ConvertFilesInLvl<::LevelIds, ::Path_TLV>(*mThreadPool, dataDir, fs, lvlReader, fileBuffer, lvlIdxAsLvl, reliveLvl, dv, false, true);
        });
    }
    WriteDataVersion(dataDir);

    LogNonConvertedAnims(false);
}

void DataConversion::DataVersions::Save(const FileSystem::Path& dataDir) const
{
    auto j = nlohmann::json{
        {"fmv_version", mFmvVersion},
        {"path_version", mPathVersion},
        {"palette_version", mPaletteVersion},
        {"animation_version", mAnimationVersion},
        {"camera_version", mCameraVersion},
        {"save_file_version", mSaveFileVersion},
        {"font_file_version", mFontFileVersion},
        {"demo_file_version", mDemoFileVersion},
    };
    FileSystem fs;
    FileSystem::Path fileName = dataDir;
    fileName.Append(kDataVersionFileName);
    SaveJson(j, fs, fileName);
}

bool DataConversion::DataVersions::Load(const FileSystem::Path& dataDir)
{
    auto path = dataDir;
    path.Append(kDataVersionFileName);

    FileSystem fs;
    if (fs.FileExists(path.GetPath().c_str()))
    {
        const std::string jsonStr = fs.LoadToString(path);
        nlohmann::json j;
        try
        {
            DataVersions dv;
            j = nlohmann::json::parse(jsonStr);
            j.at("fmv_version").get_to(dv.mFmvVersion);
            j.at("path_version").get_to(dv.mPathVersion);
            j.at("palette_version").get_to(dv.mPaletteVersion);
            j.at("animation_version").get_to(dv.mAnimationVersion);
            j.at("camera_version").get_to(dv.mCameraVersion);
            j.at("save_file_version").get_to(dv.mSaveFileVersion);
            j.at("font_file_version").get_to(dv.mFontFileVersion);
            j.at("demo_file_version").get_to(dv.mDemoFileVersion);
            *this = dv;
        }
        catch (const nlohmann::json::exception&)
        {
            return false;
        }

        return true;
    }
    return false;
}
