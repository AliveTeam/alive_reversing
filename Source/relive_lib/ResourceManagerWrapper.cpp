#include "ResourceManagerWrapper.hpp"
#include "GameType.hpp"
#include "../AliveLibAE/ResourceManager.hpp"
#include "../AliveLibAO/ResourceManager.hpp"

#include "data_conversion/file_system.hpp"

#include "data_conversion/data_conversion.hpp"
#include "BinaryPath.hpp"
#include "../AliveLibCommon/BaseGameAutoPlayer.hpp"

#include "nlohmann/json.hpp"

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 1000
#include <magic_enum/include/magic_enum.hpp>

#include <lodepng/lodepng.h>

u32 UniqueResId::mGlobalId = 1;
std::vector<PendingResource> ResourceManagerWrapper::mFilesPendingLoading;

std::map<AnimId, ResourceManagerWrapper::AnimCache> ResourceManagerWrapper::mAnims;

s16 ResourceManagerWrapper::FreeResource(u8** ppRes)
{
    if (GetGameType() == GameType::eAe)
    {
        return ResourceManager::FreeResource_49C330(ppRes);
    }
    else
    {
        return AO::ResourceManager::FreeResource_455550(ppRes);
    }
}


u8** ResourceManagerWrapper::GetLoadedResource(u32 type, u32 resourceID, u16 addUseCount, u16 bLock)
{
    if (GetGameType() == GameType::eAe)
    {
        return ResourceManager::GetLoadedResource(type, resourceID, addUseCount, bLock);
    }
    else
    {
        return AO::ResourceManager::GetLoadedResource(type, resourceID, addUseCount, bLock);
    }
}

u8** ResourceManagerWrapper::Alloc_New_Resource(u32 type, u32 id, u32 size)
{
    if (GetGameType() == GameType::eAe)
    {
        return ResourceManager::Alloc_New_Resource_49BED0(type, id, size);
    }
    else
    {
        return AO::ResourceManager::Alloc_New_Resource_454F20(type, id, size);
    }
}

void ResourceManagerWrapper::Inc_Ref_Count(u8** ppRes)
{
    Get_Header(ppRes)->field_4_ref_count++;
}

ResourceManagerWrapper::Header* ResourceManagerWrapper::Get_Header(u8** ppRes)
{
    return reinterpret_cast<Header*>((*ppRes - sizeof(Header)));
}

inline void from_json(const nlohmann::json& j, Point32& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
}

inline void from_json(const nlohmann::json& j, IndexedPoint& p)
{
    j.at("index").get_to(p.mIndex);
    j.at("point").get_to(p.mPoint);
}

inline void from_json(const nlohmann::json& j, PerFrameInfo& p)
{
    j.at("x_offset").get_to(p.mXOffset);
    j.at("y_offset").get_to(p.mYOffset);
    j.at("width").get_to(p.mWidth);
    j.at("height").get_to(p.mHeight);
    j.at("sprite_sheet_x").get_to(p.mSpriteSheetX);
    j.at("sprite_sheet_y").get_to(p.mSpriteSheetY);
    j.at("bound_max").get_to(p.mBoundMax);
    j.at("bound_min").get_to(p.mBoundMin);
    j.at("points_count").get_to(p.mPointCount);

    if (p.mPointCount > 0)
    {
        j.at("points").get_to(p.mPoints);
    }
}

inline void from_json(const nlohmann::json& j, AnimAttributes& p)
{
    j.at("frame_rate").get_to(p.mFrameRate);
    j.at("flip_x").get_to(p.mFlipX);
    j.at("flip_y").get_to(p.mFlipY);
    j.at("loop").get_to(p.mLoop);
    j.at("loop_start_frame").get_to(p.mLoopStartFrame);
    j.at("max_width").get_to(p.mMaxWidth);
    j.at("max_height").get_to(p.mMaxHeight);
}

AnimationAttributesAndFrames::AnimationAttributesAndFrames(const std::string& jsonData)
{
    nlohmann::json j = nlohmann::json::parse(jsonData);
    mFrames.resize(j["frames"].size());
    u32 i = 0;
    for (auto& frame : j["frames"])
    {
        from_json(frame, mFrames[i]);
        i++;
    }

    from_json(j["attributes"], mAttributes);
}

static FileSystem::Path BasePath()
{
    FileSystem::Path filePath;
    filePath.Append("relive_data");
    if (GetGameType() == GameType::eAe)
    {
        filePath.Append("ae");
    }
    else
    {
        filePath.Append("ao");
    }
    return filePath;
}

AnimResource ResourceManagerWrapper::LoadAnimation(AnimId anim)
{
    auto it = mAnims.find(anim);
    if (it != std::end(mAnims))
    {
        auto jsonPtr = it->second.mAnimAttributes.lock();
        auto tgaPtr = it->second.mAnimTga.lock();

        if (jsonPtr && tgaPtr)
        {
            AnimResource res(anim, jsonPtr, tgaPtr);
            res.mUniqueId = it->second.mAnimUniqueId;
            return res;
        }
    }

    // One huge blocking func for now - needs to work like OG res man
    FileSystem::Path filePath = BasePath();

    filePath.Append("animations");

    const char_type* enum_name = magic_enum::enum_name(anim).data();
    filePath.Append(enum_name);

    FileSystem fs;
    const std::string jsonStr = fs.LoadToString((filePath.GetPath() + ".json").c_str());
    if (jsonStr.empty())
    {
        LOG_ERROR("Missing anim json for anim: " << enum_name);
        ALIVE_FATAL("Missing anim json");
    }

    // TODO: Use FS
    auto pTgaData = std::make_shared<TgaData>();
    TgaFile tgaFile;
    tgaFile.Load((filePath.GetPath() + ".tga").c_str(), pTgaData->mPal, pTgaData->mPixels, pTgaData->mWidth, pTgaData->mHeight);

    auto pAnimationAttributesAndFrames = std::make_shared<AnimationAttributesAndFrames>(jsonStr);

    AnimResource newRes;
    newRes.mId = anim;
    newRes.mJsonPtr = pAnimationAttributesAndFrames;
    newRes.mTgaPtr = pTgaData;
    mAnims[anim] = {pAnimationAttributesAndFrames, pTgaData};

    return newRes;
}

PalResource ResourceManagerWrapper::LoadPal(PalId pal)
{
    // TODO: Cache these
    PalResource newRes;
    newRes.mId = pal;
    newRes.mPal = std::make_shared<AnimationPal>();
    // TODO: Load pal from disk

    return newRes;
}

static FileSystem::Path PerLvlBasePath(EReliveLevelIds lvlId)
{
    FileSystem::Path filePath = BasePath();
    if (GetGameType() == GameType::eAe)
    {
        filePath.Append(ToString(MapWrapper::ToAE(lvlId)));
    }
    else
    {
        filePath.Append(ToString(MapWrapper::ToAO(lvlId)));
    }
    return filePath;
}

static FileSystem::Path CamBaseName(EReliveLevelIds lvlId, u32 pathNumber, u32 camNumber)
{
    FileSystem::Path filePath = PerLvlBasePath(lvlId);

    char buffer[128] = {};
    sprintf(buffer, "P%02dC%02d", pathNumber, camNumber);
    filePath.Append(buffer);
    return filePath;
}

static RgbaData LoadPng(const std::string& filePath)
{
    std::vector<u8> vec;
    unsigned int w = 0;
    unsigned int h = 0;
    if (!lodepng::decode(vec, w, h, filePath))
    { 
        RgbaData data;
        data.mWidth = w;
        data.mHeight = h;
        data.mPixels = std::make_shared<std::vector<u8>>(std::move(vec));
        return data;
    }
    return {};
}

CamResource ResourceManagerWrapper::LoadCam(EReliveLevelIds lvlId, u32 pathNumber, u32 camNumber)
{
    FileSystem::Path filePath = CamBaseName(lvlId, pathNumber, camNumber);

    CamResource newRes;
    newRes.mData = LoadPng(filePath.GetPath() + ".png");
    return newRes;
}

Fg1Resource ResourceManagerWrapper::LoadFg1(EReliveLevelIds lvlId, u32 pathNumber, u32 camNumber)
{
    FileSystem::Path filePath = CamBaseName(lvlId, pathNumber, camNumber);

    Fg1Resource newRes;

    // Load the json manifest
    FileSystem fs;
    const std::string jsonStr = fs.LoadToString((filePath.GetPath() + ".json").c_str());
    if (!jsonStr.empty())
    {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        newRes.mFg1ResBlockCount = j["fg1_block_count"];

        // TODO: Make this more sane later
        for (auto& fg1File : j["layers"])
        {
            std::string s = fg1File;
            if (s.find("fg_well") != std::string::npos)
            {
                newRes.mFgWell.mImage = LoadPng(filePath.GetPath() + "fg_well.png");
            }
            else if (s.find("bg_well") != std::string::npos)
            {
                newRes.mBgWell.mImage = LoadPng(filePath.GetPath() + "bg_well.png");
            }
            else if (s.find("fg") != std::string::npos)
            {
                newRes.mFg.mImage = LoadPng(filePath.GetPath() + "fg.png");
            }
            else if (s.find("bg") != std::string::npos)
            {
                newRes.mBg.mImage = LoadPng(filePath.GetPath() + "bg.png");
            }
        }
    }

    return newRes;
}

FontResource ResourceManagerWrapper::LoadFont(FontType fontId)
{
    FileSystem::Path filePath = BasePath();

    switch (fontId)
    {
        case FontType::None:
            ALIVE_FATAL("Can't load none");
            break;

        case FontType::LcdFont:
        {
            filePath.Append("lcd_font");
            break;
        }

        case FontType::PauseMenu:
        {
            filePath.Append("pause_menu_font");
            break;
        }
    }

    // TODO: Use FS
    auto pTgaData = std::make_shared<TgaData>();
    TgaFile tgaFile;
    tgaFile.Load((filePath.GetPath() + ".tga").c_str(), pTgaData->mPal, pTgaData->mPixels, pTgaData->mWidth, pTgaData->mHeight);

    FontResource newRes(fontId, pTgaData);

    return newRes;
}

std::vector<std::unique_ptr<BinaryPath>> ResourceManagerWrapper::LoadPaths(EReliveLevelIds lvlId)
{
    std::vector<std::unique_ptr<BinaryPath>> ret;

    // TODO: Load level_info.json so we know which path jsons to load for this level
    FileSystem::Path pathDir = BasePath();
    if (GetGameType() == GameType::eAe)
    {
        pathDir.Append(ToString(MapWrapper::ToAE(lvlId))).Append("paths");
    }
    else
    {
        pathDir.Append(ToString(MapWrapper::ToAO(lvlId))).Append("paths");
    }

    FileSystem::Path levelInfo = pathDir;
    levelInfo.Append("level_info.json");

    FileSystem fs;
    const std::string jsonStr = fs.LoadToString(levelInfo);
    nlohmann::json j = nlohmann::json::parse(jsonStr);
    const auto& paths = j["paths"];
    for (const auto& path : paths)
    {
        FileSystem::Path pathJsonFile = pathDir;
        pathJsonFile.Append(path);
        const std::string pathJsonStr = fs.LoadToString(pathJsonFile);

        // TODO: set the res ptrs to the parsed json data
        nlohmann::json pathJson = nlohmann::json::parse(pathJsonStr);
        LOG_INFO("Cam count " << pathJson["cameras"].size());

        auto pathBuffer = std::make_unique<BinaryPath>(pathJson["id"]);
        pathBuffer->CreateFromJson(pathJson);
        ret.emplace_back(std::move(pathBuffer));
    }

    return ret;
}

void ResourceManagerWrapper::LoadingLoop(bool bShowLoadingIcon)
{
    GetGameAutoPlayer().DisableRecorder();

    while (!mFilesPendingLoading.empty())
    {
        // TODO: Fix
        //SYS_EventsPump_494580();
        ProcessLoadingFiles();
        PSX_VSync_4F6170(0);
        const s32 ticks = loading_ticks_5C1BAC++ + 1;
        if (bShowLoadingIcon && !bHideLoadingIcon_5C1BAA && ticks > 180)
        {
            // Render everything in the ordering table including the loading icon
            Game_ShowLoadingIcon_482D80();
        }
    }

    GetGameAutoPlayer().EnableRecorder();
}

void ResourceManagerWrapper::ProcessLoadingFiles()
{

}
