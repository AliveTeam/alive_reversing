#pragma once

#include "AnimResources.hpp"
#include <mutex>

enum class AnimId;
enum class EReliveLevelIds : s16;

class BinaryPath;
struct AnimationPal;

class UniqueResId final
{
public:
    UniqueResId()
        : mId(NextGlobalId())
    {
    }

    u32 Id() const
    {
        return mId;
    }

private:
    static u32 NextGlobalId()
    {
        mGlobalId++;
        return mGlobalId;
    }

    u32 mId = 0;
    static u32 mGlobalId;
};

struct PngData final
{
    std::shared_ptr<AnimationPal> mPal;
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
    u32 mSpriteWidth = 0;
    u32 mSpriteHeight = 0;
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

class RgbaData final
{
public:
    u32 mWidth = 0;
    u32 mHeight = 0;
    std::shared_ptr<std::vector<u8>> mPixels;
};

class CamResource final
{
public:
    UniqueResId mUniqueId;
    RgbaData mData;
};

class Fg1Layer final
{
public:
    UniqueResId mUniqueId;
    RgbaData mImage;
};

class Fg1Resource final
{
public:
    u32 mFg1ResBlockCount = 0;
    Fg1Layer mFg;
    Fg1Layer mFgWell;
    Fg1Layer mBg;
    Fg1Layer mBgWell;

    bool Any() const
    {
        return mFg.mImage.mPixels || mFgWell.mImage.mPixels || mBg.mImage.mPixels || mBgWell.mImage.mPixels;
    }
};

class AnimResource final
{
public:
    AnimResource()
    {

    }

    AnimResource(AnimId id, std::shared_ptr<AnimationAttributesAndFrames>& jsonRes, std::shared_ptr<PngData>& pngRes)
        : mId(id)
        , mJsonPtr(jsonRes)
        , mPngPtr(pngRes)
    {
        mCurPal = mPngPtr->mPal;
    }

    void Clear()
    {
        mId = AnimId::None;
        mJsonPtr = nullptr;
        mPngPtr = nullptr;
    }

public:
    UniqueResId mUniqueId;
    AnimId mId = AnimId::None;
    std::shared_ptr<AnimationAttributesAndFrames> mJsonPtr;
    std::shared_ptr<PngData> mPngPtr;
    // TODO: weak_ptr
    std::shared_ptr<AnimationPal> mCurPal;
};

enum class FontType
{
    None,
    PauseMenu,
    LcdFont,
};

class FontResource final
{
public:
    FontResource() = default;

    FontResource(FontType id, std::shared_ptr<PngData>& pngPtr)
        : mId(id), mPngPtr(pngPtr)
    {
        mCurPal = pngPtr->mPal;
    }

    UniqueResId mUniqueId;
    FontType mId = FontType::None;
    // TODO: Font atlas json ptpr
    std::shared_ptr<PngData> mPngPtr;

    // TODO: Really should be a weak_ptr
    std::shared_ptr<AnimationPal> mCurPal;
};


class ThreadPool;

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
        Resource_Seq = 0x20716553,
        Resource_SEQp = 0x53455170,
        Resource_Pxtd = 0x50787464, // Added for relive path extension blocks
    };


    // == new res manager interface ==


    // TODO: needs to be async like og
    static void PendAnimation(AnimId anim);
    static AnimResource LoadAnimation(AnimId anim);

    static PalResource LoadPal(PalId pal);

    static CamResource LoadCam(EReliveLevelIds lvlId, u32 pathNumber, u32 camNumber);
    static Fg1Resource LoadFg1(EReliveLevelIds lvlId, u32 pathNumber, u32 camNumber);

    static FontResource LoadFont(FontType fontId);

    static std::vector<std::unique_ptr<BinaryPath>> LoadPaths(EReliveLevelIds lvlId);

    // TODO: Used only for vh/vb/bsq loading, will be changed when these file formats are updated
    static std::vector<u8> LoadFile(const char_type* pFileName, EReliveLevelIds lvlId);

    static void LoadingLoop(bool bShowLoadingIcon);

    // TODO: Call LoadingLoop after master/engine merge, LoadingLoop will
    // cause a de-sync due to calling sound funcs
    static void LoadingLoop2();

    static s32 SEQ_HashName(const char_type* seqFileName);

    static s16 bHideLoadingIcon;
    static s32 loading_ticks;

    static void ShowLoadingIcon();

    template <typename T, int size>
    static void PendAnims(const T (&anims)[size])
    {
        for (const auto& anim : anims)
        {
            if (anim != AnimId::None)
            {
                PendAnimation(anim);
            }
        }
    }

private:

    struct AnimCache final
    {
        // TODO: Need to be weak_ptrs
        std::shared_ptr<AnimationAttributesAndFrames> mAnimAttributes;
        std::shared_ptr<PngData> mAnimPng;
        UniqueResId mAnimUniqueId;
    };

public:
    static std::mutex mLoadedAnimationsMutex;
    // TODO: Remove dead entries at some point
    static std::map<AnimId, AnimCache> mLoadedAnimations;

private:
    // unique_ptr to avoid bringing the header in
    static std::unique_ptr<ThreadPool> mThreadPool;
};
