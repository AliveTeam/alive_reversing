#pragma once

#include "../AliveLibCommon/Types.hpp"
#include "../AliveLibCommon/AnimResources.hpp"
#include "data_conversion/AnimationConverter.hpp" // TODO: Don't depend on this
#include <memory>
#include <map>

enum class AnimId;
enum class EReliveLevelIds : s16;

struct TgaData final
{
    AnimationPal mPal;
    std::vector<u8> mPixels;
    u32 mWidth;
    u32 mHeight;
};

struct Point32 final
{
    s32 x = 0;
    s32 y = 0;
};

struct IndexedPoint final
{
    u32 mIndex = 0;
    Point32 mPoint;
};

struct PerFrameInfo final
{
    s32 mXOffset = 0;
    s32 mYOffset = 0;
    u32 mWidth = 0;
    u32 mHeight = 0;
    u32 mSpriteSheetX = 0;
    u32 mSpriteSheetY = 0;
    Point32 mBoundMin;
    Point32 mBoundMax;
    u32 mPointCount = 0;
    IndexedPoint mPoints[2] = {};
};

struct AnimAttributes final
{
    u32 mFrameRate;
    bool mFlipX;
    bool mFlipY;
    bool mLoop;
    u32 mLoopStartFrame;
    u32 mMaxWidth;
    u32 mMaxHeight;
};

class AnimationAttributesAndFrames final
{
public:
    explicit AnimationAttributesAndFrames(const std::string& jsonData);
    AnimAttributes mAttributes;
    std::vector<PerFrameInfo> mFrames;
};

class PalResource final
{
public:
    PalId mId = PalId::Default;
    std::shared_ptr<AnimationPal> mPal;
};

class CamResource final
{
public:
    u32 mWidth;
    u32 mHeight;
    std::shared_ptr<std::vector<u32>> mPixels;
};

class AnimResource final
{
public:
    AnimResource()
    {

    }

    AnimResource(AnimId id, std::shared_ptr<AnimationAttributesAndFrames>& jsonRes, std::shared_ptr<TgaData>& tgaRes)
        : mId(id)
        , mJsonPtr(jsonRes)
        , mTgaPtr(tgaRes)
    {
    }

    void Clear()
    {
        mId = AnimId::None;
        mJsonPtr = nullptr;
        mTgaPtr = nullptr;
    }

public:
    AnimId mId = AnimId::None;
    std::shared_ptr<AnimationAttributesAndFrames> mJsonPtr;
    std::shared_ptr<TgaData> mTgaPtr;
};

// Temp adapter interface
class ResourceManagerWrapper
{
public:
    // TODO: Remove/unify when both games resource managers are merged into one object
    enum ResourceType : u32
    {
        Resource_PBuf = 0x66754250,
        Resource_CHNK = 0x4B4E4843,
        Resource_DecompressionBuffer = 0x66754244,
        Resource_VLC = 0x20434C56,
        Resource_Animation = 0x6D696E41,
        Resource_VabHeader = 0x48424156,
        Resource_VabBody = 0x42424156,
        Resource_Font = 0x746E6F46,
        Resource_Path = 0x68746150,
        Resource_Palt = 0x746C6150,
        Resource_FG1 = 0x20314746,
        Resource_Bits = 0x73746942,
        Resource_Blood = 0x64756C42,
        Resource_Sprx = 0x78727053,
        Resource_FntP = 0x50746E46,
        Resource_3DGibs = 0x65444433,
        Resource_HintFly = 0x796C4648,
        Resource_Spline = 0x6e6c7053,
        Resource_Wave = 0x65766157,
        Resource_Free = 0x65657246,
        Resource_Pend = 0x646E6550,
        Resource_End = 0x21646E45,
        Resource_Plbk = 0x6B626C50,
        Resource_Play = 0x79616C50,
    };

    struct Header final
    {
        u32 field_0_size;
        s16 field_4_ref_count;
        s16 field_6_flags;
        u32 mResourceType;
        u32 field_C_id;
    };

    static s16 FreeResource(u8** ppRes);
    static u8** GetLoadedResource(u32 type, u32 resourceID, u16 addUseCount, u16 bLock);
    static u8** Alloc_New_Resource(u32 type, u32 id, u32 size);

    static void Inc_Ref_Count(u8** ppRes);
    static Header* Get_Header(u8** ppRes);

    // == new res manager interface ==


    // TODO: needs to be async like og
    static AnimResource LoadAnimation(AnimId anim);

    static PalResource LoadPal(PalId pal);

    static CamResource LoadCam(EReliveLevelIds lvlId, u32 pathNumber, u32 camNumber);

private:
    // TODO: don't use stl directly
    static std::map<AnimId, std::pair<std::weak_ptr<AnimationAttributesAndFrames>, std::weak_ptr<TgaData>>> mAnims;
};

