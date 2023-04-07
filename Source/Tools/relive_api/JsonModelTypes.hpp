#pragma once

#include "../../AliveLibCommon/Types.hpp"
#include <string>
#include <vector>
#include <set>

enum class AnimId;

namespace AO {
class PathLine;
enum class TlvTypes : s16;
} // namespace AO


class PathLine;
enum class TlvTypes : s16;

namespace jsonxx {
class Object;
class Array;
} // namespace jsonxx


namespace ReliveAPI {
enum class Game;
class TypesCollectionBase;
class TypesCollectionAO;
class TypesCollectionAE;
class CameraImageAndLayers;

struct CameraObject final
{
    std::string mName;
    s32 mId = 0;
    s32 mX = 0;
    s32 mY = 0;

    [[nodiscard]] jsonxx::Object ToJsonObject(jsonxx::Array mapObjectsArray, const CameraImageAndLayers& cameraImageAndLayers) const;
};

struct PathInfo final
{
    s32 mGridWidth = 0;
    s32 mWidth = 0;
    s32 mGridHeight = 0;
    s32 mHeight = 0;
    u32 mObjectOffset = 0;
    u32 mIndexTableOffset = 0;

    s32 mNumCollisionItems = 0;
    s32 mCollisionOffset = 0;

    s32 mAbeStartXPos = 0;
    s32 mAbeStartYPos = 0;

    s32 mNumMudsInPath = 0;

    s32 mTotalMuds = 0;
    s32 mBadEndingMuds = 0;
    s32 mGoodEndingMuds = 0;

    std::vector<std::string> mLCDScreenMessages;
    std::vector<std::string> mHintFlyMessages;
};

class CameraImageAndLayers final
{
public:
    std::string mCameraImage;
    std::string mForegroundLayer;
    std::string mBackgroundLayer;
    std::string mForegroundWellLayer;
    std::string mBackgroundWellLayer;

    bool HaveFG1Layers() const
    {
        return !mForegroundLayer.empty() || !mBackgroundLayer.empty() || !mForegroundWellLayer.empty() || !mBackgroundWellLayer.empty();
    }

    bool HaveCameraImage() const
    {
        return !mCameraImage.empty();
    }
};

struct CameraNameAndTlvBlob final
{
    s32 mId = 0;
    s32 x = 0;
    s32 y = 0;
    std::string mName;
    std::vector<std::vector<u8>> mTlvBlobs;

    CameraImageAndLayers mCameraAndLayers;

    [[nodiscard]] std::size_t TotalTlvSize() const;

    std::set<AnimId> mRequiredResources;
};

struct MapRootInfo final
{
    s32 mVersion = 0;
    std::string mGame;
};

struct MapInfo final
{
    std::string mPathBnd;
    s32 mPathId = 0;

    s32 mXGridSize = 0;
    s32 mXSize = 0;

    s32 mYGridSize = 0;
    s32 mYSize = 0;

    s32 mAbeStartXPos = 0;
    s32 mAbeStartYPos = 0;

    s32 mNumMudsInPath = 0;

    s32 mTotalMuds = 0;
    s32 mBadEndingMuds = 0;
    s32 mGoodEndingMuds = 0;

    std::vector<std::string> mLCDScreenMessages;
    std::vector<std::string> mHintFlyMessages;
};
} // namespace ReliveAPI