#pragma once

// TODO: consider not using STL in her
#include "../AliveLibCommon/Types.hpp"
#include "nlohmann/json_fwd.hpp"

#include "Collisions.hpp"

class Guid;
namespace relive
{
    class Path_TLV;
}

class BinaryPath final
{
public:
    struct CamEntry final
    {
        s32 mX = 0;
        s32 mY = 0;
        s32 mId = 0;
        std::string mName;

        template <typename TlvType>
        TlvType* AllocTLV()
        {
            mBuffer.resize(mBuffer.size() + sizeof(TlvType));
            TlvType* pTlv = reinterpret_cast<TlvType*>(mBuffer.data() + mBuffer.size() - sizeof(TlvType));
            new (pTlv) TlvType(); // placement new
            mLastAllocated = pTlv;
            mLastAllocatedSize = sizeof(TlvType);
            return pTlv;
        }

        std::vector<u8> mBuffer;
        relive::Path_TLV* mLastAllocated = nullptr;
        u32 mLastAllocatedSize = 0;
    };

    explicit BinaryPath(u32 pathId)
        : mPathId(pathId)
    {
    }

    void CreateFromJson(nlohmann::json& pathJson);

    u32 GetPathId() const
    {
        return mPathId;
    }

    const char* CameraName(s32 x, s32 y) const
    {
        for (auto& cam : mCameras)
        {
            if (cam->mX == x && cam->mY == y)
            {
                return cam->mName.c_str();
            }
        }
        return nullptr;
    }

    relive::Path_TLV* TlvsById(const Guid& id);

    relive::Path_TLV* TlvsForCamera(s32 x, s32 y, u32 tlvOffset = 0)
    {
        for (auto& cam : mCameras)
        {
            if (cam->mX == x && cam->mY == y)
            {
                return reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data() + tlvOffset);
            }
        }
        return nullptr;
    }

    std::vector<std::unique_ptr<CamEntry>>& GetCameras()
    {
        return mCameras;
    }

    std::vector<PathLine>& GetCollisions()
    {
        return mCollisions;
    }

private:
    std::vector<std::unique_ptr<CamEntry>> mCameras;
    std::vector<PathLine> mCollisions;
    u32 mPathId = 0;
};
