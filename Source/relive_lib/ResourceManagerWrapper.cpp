#include "stdafx.h"
#include "ResourceManagerWrapper.hpp"
#include "GameType.hpp"
#include "Psx.hpp"
#include "../AliveLibAE/ResourceManager.hpp"
#include "../AliveLibAO/ResourceManager.hpp"
#include "../AliveLibAE/PsxRender.hpp"

#include "data_conversion/file_system.hpp"

#include "data_conversion/data_conversion.hpp"
#include "data_conversion/AnimConversionInfo.hpp"
#include "data_conversion/PNGFile.hpp"
#include "data_conversion/AnimationConverter.hpp"

#include "BinaryPath.hpp"
#include "BaseGameAutoPlayer.hpp"
#include "GameObjects/Particle.hpp"
#include "nlohmann/json.hpp"
#include "Sys.hpp"
#include "ThreadPool.hpp"

u32 UniqueResId::mGlobalId = 1;

std::unique_ptr<ThreadPool> ResourceManagerWrapper::mThreadPool = std::make_unique<ThreadPool>();
std::mutex ResourceManagerWrapper::mLoadedAnimationsMutex;
std::map<AnimId, ResourceManagerWrapper::AnimCache> ResourceManagerWrapper::mLoadedAnimations;

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

class AnimationLoaderJob final : public IJob
{
public:
    explicit AnimationLoaderJob(AnimId anim)
        : mAnimId(anim)
    {

    }

    void Execute() override
    {
        // One huge blocking func for now - needs to work like OG res man
        FileSystem::Path filePath = BasePath();

        filePath.Append("animations");

        const char_type* groupName = AnimRecGroupName(mAnimId);
        filePath.Append(groupName);

        const char_type* animName = AnimRecName(mAnimId);
        filePath.Append(animName);

        // TODO: fs instance should probably be shared and thread safe
        FileSystem fs;
        const std::string jsonStr = fs.LoadToString((filePath.GetPath() + ".json").c_str());
        if (jsonStr.empty())
        {
            ALIVE_FATAL("Missing anim json for anim: %s", animName);
        }

        // TODO: Use FS
        auto pPngData = std::make_shared<PngData>();
        PNGFile pngFile;
        pPngData->mPal = std::make_shared<AnimationPal>();
        pngFile.Load((filePath.GetPath() + ".png").c_str(), *pPngData->mPal, pPngData->mPixels, pPngData->mWidth, pPngData->mHeight);

        auto pAnimationAttributesAndFrames = std::make_shared<AnimationAttributesAndFrames>(jsonStr);

        AnimResource newRes;
        newRes.mId = mAnimId;
        newRes.mJsonPtr = pAnimationAttributesAndFrames;
        newRes.mPngPtr = pPngData;
        newRes.mCurPal = newRes.mPngPtr->mPal;

        std::unique_lock<std::mutex> lock(ResourceManagerWrapper::mLoadedAnimationsMutex);

        ResourceManagerWrapper::mLoadedAnimations[mAnimId] = {pAnimationAttributesAndFrames, pPngData, {}};
    }

private:
    ResourceManagerWrapper* mResMan = nullptr;
    AnimId mAnimId;
};


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
    j.at("sprite_width").get_to(p.mSpriteWidth);
    j.at("sprite_height").get_to(p.mSpriteHeight);
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

s16 ResourceManagerWrapper::bHideLoadingIcon = 0;
s32 ResourceManagerWrapper::loading_ticks = 0;

void ResourceManagerWrapper::PendAnimation(AnimId anim)
{
    std::map<AnimId, ResourceManagerWrapper::AnimCache>::iterator it;
    {
        std::unique_lock<std::mutex> lock(mLoadedAnimationsMutex);
        it = mLoadedAnimations.find(anim);
    }

    if (it == std::end(mLoadedAnimations))
    {
        auto job = std::make_unique<AnimationLoaderJob>(anim);
        mThreadPool->AddJob(std::move(job));
    }
}

AnimResource ResourceManagerWrapper::LoadAnimation(AnimId anim)
{
    std::map<AnimId, ResourceManagerWrapper::AnimCache>::iterator it;
    {
        std::unique_lock<std::mutex> lock(mLoadedAnimationsMutex);
        it = mLoadedAnimations.find(anim);
    }

    if (it == std::end(mLoadedAnimations))
    {
        if (static_cast<s32>(anim) <= 908) // ignore background animations for now
        {
            LOG_ERROR("Animation %d wasn't loaded async before calling LoadAnimation, or didn't wait for async loading to finish", static_cast<s32>(anim));
        }

        // TODO: Remove this when all of factory etc is updated
        AnimationLoaderJob hack(anim);
        hack.Execute();
        std::unique_lock<std::mutex> lock(mLoadedAnimationsMutex);
        it = mLoadedAnimations.find(anim);
    }

    auto jsonPtr = it->second.mAnimAttributes;
    auto pngPtr = it->second.mAnimPng;
    if (jsonPtr && pngPtr)
    {
        AnimResource res(anim, jsonPtr, pngPtr);
        res.mUniqueId = it->second.mAnimUniqueId;
        return res;
    }

    ALIVE_FATAL("Json or PNG resources have gone out of scope");
}

PalResource ResourceManagerWrapper::LoadPal(PalId pal)
{
    // TODO: Cache these
    PalResource newRes;
    newRes.mId = pal;
    newRes.mPal = std::make_shared<AnimationPal>();

    FileSystem::Path filePath = BasePath();

    filePath.Append(ToString(newRes.mId));

    FileSystem fs;
    auto palData = fs.LoadToVec(filePath.GetPath().c_str());
    if (palData.size() != 1024) // 256 RGBA entries
    {
        ALIVE_FATAL("Bad pal data size %d but expected 1024", palData.size());
    }

    memcpy(newRes.mPal->mPal, palData.data(), palData.size());

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
    PNGFile png;

    png.Load(filePath.c_str(), vec, w, h);

    RgbaData data;
    data.mWidth = w;
    data.mHeight = h;
    data.mPixels = std::make_shared<std::vector<u8>>(std::move(vec));
    return data;
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
    auto pPngData = std::make_shared<PngData>();
    PNGFile pngFile;
    pPngData->mPal = std::make_shared<AnimationPal>();
    pngFile.Load((filePath.GetPath() + ".png").c_str(), *pPngData->mPal, pPngData->mPixels, pPngData->mWidth, pPngData->mHeight);

    FontResource newRes(fontId, pPngData);

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
        LOG_INFO("Cam count %d", pathJson["cameras"].size());

        auto pathBuffer = std::make_unique<BinaryPath>(pathJson["path_id"]);
        pathBuffer->CreateFromJson(pathJson);
        ret.emplace_back(std::move(pathBuffer));
    }

    return ret;
}

std::vector<u8> ResourceManagerWrapper::LoadFile(const char_type* pFileName, EReliveLevelIds lvlId)
{
    FileSystem::Path pathDir = BasePath();
    if (GetGameType() == GameType::eAe)
    {
        pathDir.Append(ToString(MapWrapper::ToAE(lvlId)));
    }
    else
    {
        pathDir.Append(ToString(MapWrapper::ToAO(lvlId)));
    }

    pathDir.Append(pFileName);

    FileSystem fs;
    return fs.LoadToVec(pathDir.GetPath().c_str());
}


void ResourceManagerWrapper::LoadingLoop(bool bShowLoadingIcon)
{
    GetGameAutoPlayer().DisableRecorder();

    const u32 startTime = SYS_GetTicks();
    while (mThreadPool->Busy())
    {
        SYS_EventsPump();

        // If not uncapped fps playback then actually wait for 1 frame on each iteration of the loop
        const bool unCappedFps = GetGameAutoPlayer().IsPlaying() && GetGameAutoPlayer().NoFpsLimitPlayBack();

        PSX_VSync(unCappedFps ? VSyncMode::UncappedFps : VSyncMode::LimitTo30Fps);
        const u32 k1Second = 1000; // Show loading icon after 1 second of loading
        if (bShowLoadingIcon && !bHideLoadingIcon && SYS_GetTicks() > startTime + k1Second)
        {
            // master branch never hits the below code path so don't let the playback hit it
            // if it does hit the loading icon Particle object will bump the object list count
            // by 1 and desync the playback.
            if (!GetGameAutoPlayer().IsPlaying())
            {
                // Render everything in the ordering table including the loading icon
                ResourceManagerWrapper::ShowLoadingIcon();
            }
        }
    }

    GetGameAutoPlayer().EnableRecorder();
}

void ResourceManagerWrapper::LoadingLoop2()
{
    while (mThreadPool->Busy())
    {
        // Just block, hang everything
    }
}


s32 ResourceManagerWrapper::SEQ_HashName(const char_type* seqFileName)
{
    // Clamp max len
    size_t seqFileNameLength = strlen(seqFileName) - 1;
    if (seqFileNameLength > 8)
    {
        seqFileNameLength = 8;
    }

    // Iterate each s8 to calculate hash
    u32 hashId = 0;
    for (size_t index = 0; index < seqFileNameLength; index++)
    {
        char_type letter = seqFileName[index];
        if (letter == '.')
        {
            break;
        }

        const u32 temp = 10 * hashId;
        if (letter < '0' || letter > '9')
        {
            if (letter >= 'a')
            {
                if (letter <= 'z')
                {
                    letter -= ' ';
                }
            }
            hashId = letter % 10 + temp;
        }
        else
        {
            hashId = index || letter != '0' ? temp + letter - '0' : temp + 9;
        }
    }
    return hashId;
}

void ResourceManagerWrapper::ShowLoadingIcon()
{
    AnimResource res = ResourceManagerWrapper::LoadAnimation(AnimId::Loading_Icon2);
    auto pParticle = relive_new Particle(FP_FromInteger(0), FP_FromInteger(0), res);
    if (pParticle)
    {
        pParticle->GetAnimation().SetSemiTrans(false);
        pParticle->GetAnimation().SetBlending(true);

        pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_0);

        OrderingTable local_ot;

        pParticle->GetAnimation().VRender(320, 220, local_ot, 0, 0);
        PSX_DrawOTag(local_ot);

        PSX_PutDispEnv_4F5890();
        pParticle->SetDead(true);
        ResourceManagerWrapper::bHideLoadingIcon = true;
    }
}