#include "ResourceManagerWrapper.hpp"
#include "GameType.hpp"
#include "../AliveLibAE/ResourceManager.hpp"
#include "../AliveLibAO/ResourceManager.hpp"

#include "data_conversion/file_system.hpp"

#include "nlohmann/json.hpp"

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 1000
#include <magic_enum/include/magic_enum.hpp>

std::map<AnimId, std::pair<std::weak_ptr<AnimationAttributesAndFrames>, std::weak_ptr<TgaData>>> ResourceManagerWrapper::mAnims;

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


AnimResource ResourceManagerWrapper::LoadAnimation(AnimId anim)
{
    auto it = mAnims.find(anim);
    if (it != std::end(mAnims))
    {
        auto jsonPtr = it->second.first.lock();
        auto tgaPtr = it->second.second.lock();

        if (jsonPtr && tgaPtr)
        {
            return AnimResource(anim, jsonPtr, tgaPtr);
        }
    }

    // One huge blocking func for now - needs to work like OG res man
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

    filePath.Append("animations");

    const char_type* enum_name = magic_enum::enum_name(anim).data();
    filePath.Append(enum_name);

    FileSystem fs;
    const std::string jsonStr = fs.LoadToString((filePath.GetPath() + ".json").c_str());
    if (jsonStr.empty())
    {
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

PalResource ResourceManagerWrapper::LoadPal(PalId /*pal*/)
{
    // TODO: Cache these
    PalResource newRes;

    // TODO: Load pal from disk

    return newRes;
}
