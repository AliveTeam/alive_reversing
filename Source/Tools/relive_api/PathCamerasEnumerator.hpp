#pragma once

#include "../../AliveLibAO/Map.hpp"

inline s32 To1dIndex(s32 width, s32 x, s32 y)
{
    return x + (y * width);
}

struct CameraEntry final
{
    std::string mName;
    s32 mId = 0;
    s32 mX = 0;
    s32 mY = 0;
};

class PathCamerasEnumerator final
{
public:
    PathCamerasEnumerator(s32 width, s32 height, const std::vector<u8>& pathResource);

    using TFnOnCamera = std::function<void(CameraEntry& cam)>;
    void Enumerate(TFnOnCamera onCamera);

private:
    const s32 mWidth = 0;
    const s32 mHeight = 0;
    const std::vector<u8>& mPathResource;
};

inline PathCamerasEnumerator::PathCamerasEnumerator(s32 width, s32 height, const std::vector<u8>& pathResource)
    : mWidth(width),
      mHeight(height),
      mPathResource(pathResource)
{
}

inline void PathCamerasEnumerator::Enumerate(TFnOnCamera onCamera)
{
    const u8* pPathData = mPathResource.data();
    for (s32 y = 0; y < mHeight; y++)
    {
        for (s32 x = 0; x < mWidth; x++)
        {
            // AO::CameraName is the same as AE structure
            auto pCamName = reinterpret_cast<const AO::CameraName*>(&pPathData[To1dIndex(mWidth, x, y) * sizeof(AO::CameraName)]);
            CameraEntry tmpCamera;
            tmpCamera.mX = x;
            tmpCamera.mY = y;
            if (pCamName->name[0])
            {
                tmpCamera.mName = std::string(pCamName->name, 8);
                tmpCamera.mId = 1 * (pCamName->name[7] - '0') + 10 * (pCamName->name[6] - '0') + 100 * (pCamName->name[4] - '0') + 1000 * (pCamName->name[3] - '0');
            }

            onCamera(tmpCamera);
        }
    }
}