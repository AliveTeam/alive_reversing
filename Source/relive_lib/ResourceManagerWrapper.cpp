#include "stdafx.h"
#include "ResourceManagerWrapper.hpp"
#include "GameType.hpp"
#include "Psx.hpp"
#include "../AliveLibAE/PsxRender.hpp"

#include "data_conversion/file_system.hpp"

#include "data_conversion/data_conversion.hpp"
#include "data_conversion/AnimConversionInfo.hpp"
#include "data_conversion/PNGFile.hpp"
#include "data_conversion/AnimationConverter.hpp"

#include "BinaryPath.hpp"
#include "BaseGameAutoPlayer.hpp"
#include "FmvInfo.hpp"
#include "GameObjects/Particle.hpp"
#include "nlohmann/json.hpp"
#include "Sys.hpp"
#include "ThreadPool.hpp"
#include <FatalError.hpp>
#include <string>

u32 UniqueResId::mGlobalId = 1;

ResourceManagerWrapper::ResourceManagerWrapper(FileSystem& fs, const std::string& modPath)
    : mFs(fs)
    , mThreadPool(std::make_unique<ThreadPool>())
{
    bHideLoadingIcon = 0;
    loading_ticks = 0;

    AddSearchPaths(modPath);
}

void ResourceManagerWrapper::AddSearchPaths(const std::string& modPath)
{
    // Root of all data
    FileSystem::Path reliveDataPath;
    reliveDataPath.Append("relive_data");

    // Where the base dir of the game type we are running is
    FileSystem::Path primaryBaseGamePath = reliveDataPath;

    // Where the base dir of the opposite game might be - we check here if primary fails
    FileSystem::Path backupBaseGamePath = reliveDataPath;
    if (GetGameType() == GameType::eAe)
    {
        primaryBaseGamePath.Append("ae");
        backupBaseGamePath.Append("ao");
    }
    else
    {
        primaryBaseGamePath.Append("ao");
        backupBaseGamePath.Append("ae");
    }

    if (!modPath.empty())
    {
        mSearchPaths.push_back(modPath);
    }

    mSearchPaths.push_back(primaryBaseGamePath.GetPath());
    mSearchPaths.push_back(backupBaseGamePath.GetPath());
}


// Out of line so unique_ptr<ThreadPool> can be destroyed with an incomplete ThreadPool type
ResourceManagerWrapper::~ResourceManagerWrapper() = default;

static std::string LvlDirName(EReliveLevelIds lvlId)
{
    if (GetGameType() == GameType::eAe)
    {
        return ToString(MapWrapper::ToAE(lvlId));
    }
    return ToString(MapWrapper::ToAO(lvlId));
}

static FileSystem::Path PerLvlBasePath(const std::string& basePath, EReliveLevelIds lvlId)
{
    FileSystem::Path filePath(basePath);
    filePath.Append("levels");
    filePath.Append(LvlDirName(lvlId));
    return filePath;
}

class AnimationLoaderJob final : public IJob
{
private:
    static std::string GetAnimPath(const std::string& basePath, AnimId animId, const std::string& themeName)
    {
        // One huge blocking func for now - needs to work like OG res man
        FileSystem::Path filePath(basePath);

        filePath.Append("animations");

        const char_type* groupName = AnimRecGroupName(animId);
        filePath.Append(groupName);

        if (!themeName.empty())
        {
            filePath.Append(themeName);
        }

        const char_type* animName = AnimRecName(animId);
        filePath.Append(animName);

        return filePath.GetPath();
    }

    std::string Describe() const
    {
        std::string desc = std::string("Animation \"") + AnimRecName(mAnimId) + "\"";
        if (!mThemeName.empty())
        {
            desc += " (theme \"" + mThemeName + "\")";
        }
        return desc;
    }

public:
    explicit AnimationLoaderJob(ResourceManagerWrapper* pResMan, AnimId anim, const std::string& themeName)
        : mResMan(pResMan), mAnimId(anim), mThemeName(themeName)
    {

    }

    void Execute() override
    {
        // One huge blocking func for now - needs to work like OG res man

        FileSystem& fs = mResMan->mFs;
        std::string jsonStr;
        std::string filePath;
        std::vector<std::string> searchedJsonPaths;
        for (const auto& basePath : mResMan->mSearchPaths)
        {
            filePath = GetAnimPath(basePath, mAnimId, mThemeName);
            const std::string jsonPath = filePath + ".json";
            searchedJsonPaths.push_back(jsonPath);
            jsonStr = fs.LoadToString(jsonPath.c_str());
            if (!jsonStr.empty())
            {
                break;
            }
        }

        // Not found under any search path - report it (rather than letting PNGFile::Load below
        // hard-abort the process, possibly from one of several ThreadPool worker threads at
        // once) and bail out without touching the (non-existent) png or parsing an empty json
        // string. Exists()/LookUp() will keep reporting this animation as not loaded, same as
        // if this job had never run.
        if (jsonStr.empty())
        {
            mResMan->ReportMissingResource(Describe(), searchedJsonPaths);
            return;
        }

        const std::string pngPath = filePath + ".png";
        if (!fs.FileExists(pngPath.c_str()))
        {
            mResMan->ReportMissingResource(Describe(), {pngPath});
            return;
        }

        auto pPngData = std::make_shared<PngData>();
        PNGFile pngFile;
        pPngData->mPal = std::make_shared<AnimationPal>();
        pngFile.Load(fs, pngPath.c_str(), *pPngData->mPal, pPngData->mPixels, pPngData->mWidth, pPngData->mHeight);

        auto pAnimationAttributesAndFrames = std::make_shared<AnimationAttributesAndFrames>(jsonStr);

        AnimResource newRes;
        newRes.mId = mAnimId;
        newRes.mJsonPtr = pAnimationAttributesAndFrames;
        newRes.mPngPtr = pPngData;
        newRes.mCurPal = newRes.mPngPtr->mPal;

        std::unique_lock<std::mutex> lock(mResMan->mLoadedAnimationsMutex);

        mResMan->mLoadedAnimations[std::make_pair(mThemeName, mAnimId)] = {pAnimationAttributesAndFrames, pPngData, {}};
    }

private:
    ResourceManagerWrapper* mResMan = nullptr;
    AnimId mAnimId;
    std::string mThemeName;
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

void ResourceManagerWrapper::PendAnimation(AnimId animId, const std::string& theme)
{
    if (!Exists(animId, theme))
    {
        auto job = std::make_unique<AnimationLoaderJob>(this, animId, theme);
        mThreadPool->AddJob(std::move(job));
    }
}

std::string ResourceManagerWrapper::FmvPath(const std::string& fmvName)
{
    const std::string webmName = relive::FmvNameWithoutExtension(fmvName) + ".webm";
    for (const auto& basePath : mSearchPaths)
    {
        FileSystem::Path filePath(basePath);
        filePath.Append("fmvs");
        filePath.Append(webmName);
        if (mFs.FileExists(filePath.GetPath().c_str()))
        {
            return filePath.GetPath();
        }
    }
    return fmvName;
}

AnimResource ResourceManagerWrapper::LoadAnimation(AnimId anim, const std::string& themeName)
{
    // TODO: Remove this when all of factory etc is updated (since it will always already be loaded here)
    if (!Exists(anim, themeName))
    {
        if (static_cast<s32>(anim) <= 908) // ignore background animations for now
        {
            LOG_ERROR("Animation %d wasn't loaded async before calling LoadAnimation, or didn't wait for async loading to finish", static_cast<s32>(anim));
        }

        AnimationLoaderJob hack(this, anim, themeName);
        hack.Execute();

        // hack.Execute() only records a report and returns if the resource is missing (see
        // AnimationLoaderJob::Execute) - unlike PendAnimation's async jobs, this can't wait for
        // the next LoadingLoop to surface it, since the caller needs the animation right now, so
        // flush (and fatally abort, listing every location searched) immediately if it did.
        FlushMissingResourceReports();
    }

    AnimCache cache = LookUp(anim, themeName);
    auto jsonPtr = cache.mAnimAttributes;
    auto pngPtr = cache.mAnimPng;
    if (jsonPtr && pngPtr)
    {
        AnimResource res(anim, jsonPtr, pngPtr);
        res.mUniqueId = cache.mAnimUniqueId;
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

    FileSystem::Path filePath;
    std::vector<std::string> searchedPaths;
    bool found = false;
    for (auto& basePath : mSearchPaths)
    {
        filePath = FileSystem::Path(basePath);
        filePath.Append(ToString(newRes.mId));
        searchedPaths.push_back(filePath.GetPath());
        if (mFs.FileExists(filePath.GetPath().c_str()))
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        // The caller needs the palette right now, so (like LoadAnimation) flush and fatally abort
        // immediately instead of waiting for the next LoadingLoop.
        ReportMissingResource(std::string("Palette \"") + ToString(newRes.mId) + "\"", std::move(searchedPaths));
        FlushMissingResourceReports();
    }

    auto palData = mFs.LoadToVec(filePath.GetPath().c_str());
    if (palData.size() != 1024) // 256 RGBA entries
    {
        ALIVE_FATAL("Bad pal data size %zu but expected 1024", palData.size());
    }

    memcpy(newRes.mPal->mPal, palData.data(), palData.size());

    return newRes;
}


static FileSystem::Path CamBaseName(const std::string& basePath, EReliveLevelIds lvlId, u32 pathNumber, u32 camNumber)
{
    // No separate "paths" subdir under the level - a level's content already *is* its paths.
    FileSystem::Path filePath = PerLvlBasePath(basePath, lvlId);
    filePath.Append(std::to_string(pathNumber));
    filePath.Append(std::to_string(camNumber));
    return filePath;
}

static RgbaData LoadPng(FileSystem& fs, const std::string& filePath)
{
    std::vector<u8> vec;
    unsigned int w = 0;
    unsigned int h = 0;
    PNGFile png;

    png.Load(fs, filePath.c_str(), vec, w, h);

    RgbaData data;
    data.mWidth = w;
    data.mHeight = h;
    data.mPixels = std::make_shared<std::vector<u8>>(std::move(vec));
    return data;
}

CamResource ResourceManagerWrapper::LoadCam(EReliveLevelIds lvlId, u32 pathNumber, u32 camNumber)
{
    CamResource newRes;
    std::vector<std::string> searchedPaths;
    for (const auto& basePath : mSearchPaths)
    {
        const std::string pngPath = CamBaseName(basePath, lvlId, pathNumber, camNumber).GetPath() + ".png";
        searchedPaths.push_back(pngPath);
        if (mFs.FileExists(pngPath.c_str()))
        {
            newRes.mData = LoadPng(mFs, pngPath);
            return newRes;
        }
    }

    // The caller needs the camera right now, so (like LoadAnimation) flush and fatally abort
    // immediately instead of waiting for the next LoadingLoop.
    ReportMissingResource("Camera " + std::to_string(camNumber) + " of path " + std::to_string(pathNumber) + " of level \"" + LvlDirName(lvlId) + "\"", std::move(searchedPaths));
    FlushMissingResourceReports();
    return newRes;
}

Fg1Resource ResourceManagerWrapper::LoadFg1(EReliveLevelIds lvlId, u32 pathNumber, u32 camNumber)
{
    Fg1Resource newRes;
    
    // Load the json manifest
    for (const auto& basePath : mSearchPaths)
    {
        FileSystem::Path filePath = CamBaseName(basePath, lvlId, pathNumber, camNumber);
        const std::string jsonStr = mFs.LoadToString((filePath.GetPath() + ".json").c_str());
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
                    newRes.mFgWell.mImage = LoadPng(mFs, filePath.GetPath() + "fg_well.png");
                }
                else if (s.find("bg_well") != std::string::npos)
                {
                    newRes.mBgWell.mImage = LoadPng(mFs, filePath.GetPath() + "bg_well.png");
                }
                else if (s.find("fg") != std::string::npos)
                {
                    newRes.mFg.mImage = LoadPng(mFs, filePath.GetPath() + "fg.png");
                }
                else if (s.find("bg") != std::string::npos)
                {
                    newRes.mBg.mImage = LoadPng(mFs, filePath.GetPath() + "bg.png");
                }
            }
            break;
        }
    }
    return newRes;
}

FontResource ResourceManagerWrapper::LoadFont(FontType fontId)
{
    std::string fontName;
    switch (fontId)
    {
        case FontType::None:
            ALIVE_FATAL("Can't load none");
            break;

        case FontType::LcdFont:
            fontName = "lcd_font";
            break;

        case FontType::PauseMenu:
            fontName = "pause_menu_font";
            break;
    }

    auto pPngData = std::make_shared<PngData>();
    pPngData->mPal = std::make_shared<AnimationPal>();
    std::vector<std::string> searchedPaths;
    bool found = false;
    for (const auto& basePath : mSearchPaths)
    {
        FileSystem::Path filePath(basePath);
        filePath.Append(fontName);

        const std::string pngPath = filePath.GetPath() + ".png";
        searchedPaths.push_back(pngPath);
        if (!mFs.FileExists(pngPath.c_str()))
        {
            continue;
        }

        PNGFile pngFile;
        pngFile.Load(mFs, pngPath.c_str(), *pPngData->mPal, pPngData->mPixels, pPngData->mWidth, pPngData->mHeight);
        if (!pPngData->mPixels.empty())
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        // The caller needs the font right now, so (like LoadAnimation) flush and fatally abort
        // immediately instead of waiting for the next LoadingLoop.
        ReportMissingResource("Font \"" + fontName + "\"", std::move(searchedPaths));
        FlushMissingResourceReports();
    }

    FontResource newRes(fontId, pPngData);
    return newRes;
}

std::vector<std::unique_ptr<BinaryPath>> ResourceManagerWrapper::LoadPaths(EReliveLevelIds lvlId)
{
    std::vector<std::unique_ptr<BinaryPath>> ret;
    std::vector<std::string> searchedLevelInfoPaths;
    bool foundLevelInfo = false;

    for (const auto& basePath : mSearchPaths)
    {
        // TODO: Load level_info.json so we know which path jsons to load for this level
        FileSystem::Path pathDir = PerLvlBasePath(basePath, lvlId);

        FileSystem::Path levelInfo = pathDir;
        levelInfo.Append("level_info.json");
        searchedLevelInfoPaths.push_back(levelInfo.GetPath());

        const std::string jsonStr = mFs.LoadToString(levelInfo);
        if (!jsonStr.empty())
        {
            nlohmann::json j = nlohmann::json::parse(jsonStr);
            const auto& paths = j["paths"];
            for (const auto& path : paths)
            {
                const std::string pathId = path["path_id"];

                FileSystem::Path pathJsonFile = pathDir;
                pathJsonFile.Append(pathId).Append("path.json");
                const std::string pathJsonStr = mFs.LoadToString(pathJsonFile);

                // level_info.json listed this path so it should exist, and it can only live next
                // to that level_info.json - there is no other search path to fall back to.
                if (pathJsonStr.empty())
                {
                    ReportMissingResource("Path " + pathId + " of level \"" + LvlDirName(lvlId) + "\"", {pathJsonFile.GetPath()});
                    continue;
                }

                // TODO: set the res ptrs to the parsed json data
                // TODO: Handle exception on bad data

                nlohmann::json pathJson = nlohmann::json::parse(pathJsonStr);
                LOG_INFO("Cam count %zu", pathJson["map"]["cameras"].size());

                auto pathBuffer = std::make_unique<BinaryPath>(pathJsonFile.GetPath(), pathJson["map"]["path_id"]);
                pathBuffer->CreateFromJson(pathJson);

                PathSoundInfo& soundInfo = *pathBuffer->GetSoundInfo();
                const SoundThemeInfo& themeInfo = LoadSoundThemeInfo(soundInfo.mSoundTheme);
                soundInfo.mVhFile = themeInfo.mVhFile;
                soundInfo.mVbFile = themeInfo.mVbFile;
                soundInfo.mSeqFiles = themeInfo.mSeqFiles;

                ret.emplace_back(std::move(pathBuffer));
            }
            foundLevelInfo = true;
            break;
        }
    }

    if (!foundLevelInfo)
    {
        ReportMissingResource("Level info of level \"" + LvlDirName(lvlId) + "\"", std::move(searchedLevelInfoPaths));
    }

    return ret;
}

// Doesn't report anything itself - callers decide whether an empty result is fatal right now
// (LoadSoundFile) or just gets recorded for later (LoadSoundThemeInfo, see there).
static std::vector<u8> FindSoundFile(FileSystem& fs, const std::vector<std::string>& searchPaths, const char_type* pFileName, const std::string& soundTheme, std::vector<std::string>& searchedPaths)
{
    for (const auto& basePath : searchPaths)
    {
        FileSystem::Path soundFilePath(basePath);
        soundFilePath.Append("sounds").Append(soundTheme).Append(pFileName);
        searchedPaths.push_back(soundFilePath.GetPath());
        auto vec = fs.LoadToVec(soundFilePath.GetPath().c_str());
        if (!vec.empty())
        {
            return vec;
        }
    }
    return {};
}

std::vector<u8> ResourceManagerWrapper::LoadSoundFile(const char_type* pFileName, const std::string& soundTheme)
{
    std::vector<std::string> searchedPaths;
    std::vector<u8> vec = FindSoundFile(mFs, mSearchPaths, pFileName, soundTheme, searchedPaths);
    if (vec.empty())
    {
        // The callers (VH/VB/SEQ loading) dereference the data straight away, so like
        // LoadAnimation flush and fatally abort immediately instead of waiting for the next
        // LoadingLoop.
        ReportMissingResource("Sound file \"" + std::string(pFileName) + "\" of sound theme \"" + soundTheme + "\"", std::move(searchedPaths));
        FlushMissingResourceReports();
    }
    return vec;
}

const ResourceManagerWrapper::SoundThemeInfo& ResourceManagerWrapper::LoadSoundThemeInfo(const std::string& soundTheme)
{
    const auto existing = mSoundThemeInfoCache.find(soundTheme);
    if (existing != mSoundThemeInfoCache.end())
    {
        return existing->second;
    }

    SoundThemeInfo info;
    std::vector<std::string> searchedPaths;
    const std::vector<u8> bytes = FindSoundFile(mFs, mSearchPaths, "sound_info.json", soundTheme, searchedPaths);
    if (!bytes.empty())
    {
        const nlohmann::json j = nlohmann::json::parse(bytes.begin(), bytes.end());
        j.at("vh_file").get_to(info.mVhFile);
        j.at("vb_file").get_to(info.mVbFile);
        j.at("seq_files").get_to(info.mSeqFiles);
    }
    else
    {
        LOG_ERROR("Missing sound_info.json for sound theme '%s'", soundTheme.c_str());

        // Report only, no flush: LoadPaths (our caller) also runs on the data conversion worker
        // thread via AESaveConverter, where a modal + abort isn't safe - the game's callers
        // flush right after LoadPaths on the main thread.
        ReportMissingResource("Sound info of sound theme \"" + soundTheme + "\"", std::move(searchedPaths));
    }

    return mSoundThemeInfoCache.emplace(soundTheme, std::move(info)).first->second;
}


void ResourceManagerWrapper::LoadingLoop(bool bShowLoadingIcon, BaseMap* pMap)
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
            // Render everything in the ordering table including the loading icon
            ShowLoadingIcon(*pMap);
        }
    }

    GetGameAutoPlayer().EnableRecorder();

    // This batch of async loading has fully finished (successfully or not) - the main thread is
    // blocked right here waiting for it either way, so this is the natural place to surface
    // anything PendAnimation's worker-thread jobs couldn't find.
    FlushMissingResourceReports();
}

void ResourceManagerWrapper::LoadingLoop2()
{
    while (mThreadPool->Busy())
    {
        // Just block, hang everything
    }

    FlushMissingResourceReports();
}

void ResourceManagerWrapper::ReportMissingResource(std::string description, std::vector<std::string> searchedPaths)
{
    std::unique_lock<std::mutex> lock(mMissingResourcesMutex);
    mMissingResources.push_back({std::move(description), std::move(searchedPaths)});
}

void ResourceManagerWrapper::FlushMissingResourceReports()
{
    std::vector<MissingResourceReport> reports;
    {
        std::unique_lock<std::mutex> lock(mMissingResourcesMutex);
        if (mMissingResources.empty())
        {
            return;
        }
        reports = std::move(mMissingResources);
        mMissingResources.clear();
    }

    std::string message = "The following resources should exist but could not be found:\n";
    for (const auto& report : reports)
    {
        message += "\n" + report.mDescription + "\nSearched (in order):\n";
        for (const auto& path : report.mSearchedPaths)
        {
            message += "  " + path + "\n";
        }
    }

    // A batch listing every location searched for several missing resources can easily be
    // longer than ALIVE_FATAL's stack buffer - it falls back to a heap one sized to fit rather
    // than truncating, so passing the whole message through here is safe.
    ALIVE_FATAL("%s", message.c_str());
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

void ResourceManagerWrapper::ShowLoadingIcon(BaseMap& map)
{
    AnimResource res = LoadAnimation(AnimId::Loading_Icon2);
    auto pParticle = relive_new Particle(FP_FromInteger(0), FP_FromInteger(0), res, *this, map);
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
        bHideLoadingIcon = true;
    }
}

bool ResourceManagerWrapper::Exists(AnimId animId, const std::string& theme)
{
    std::unique_lock<std::mutex> lock(mLoadedAnimationsMutex);

    auto it = mLoadedAnimations.find(std::make_pair(theme, animId));
    if (it == std::end(mLoadedAnimations))
    {
        return false;
    }
    return true;
}

ResourceManagerWrapper::AnimCache ResourceManagerWrapper::LookUp(AnimId animId, const std::string& theme)
{
    std::unique_lock<std::mutex> lock(mLoadedAnimationsMutex);

    auto it = mLoadedAnimations.find(std::make_pair(theme, animId));
    if (it == std::end(mLoadedAnimations))
    {
        return {};
    }
    return it->second;
}
