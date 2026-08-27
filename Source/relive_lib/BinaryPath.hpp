#pragma once

#include "Types.hpp"
#include "nlohmann/json_fwd.hpp"
#include "Collisions.hpp"
#include "FatalError.hpp"

class Guid;

namespace relive
{
    class Path_TLV;
}

struct PathSoundInfo final
{
    std::string mVhFile;
    std::string mVbFile;
    std::vector<std::string> mSeqFiles;

    // TODO: Runtime only vars - move out when sound refactor done
    s32 mVabId = -1;
    std::vector<u8> mVhFileData;
    std::vector<u8> mBsqFileData;
};

using UP_Path_TLV = std::unique_ptr<relive::Path_TLV>;

struct TlvIterator;
struct TlvList final
{
    TlvList() = default;

    // Prevent copying
    TlvList(const TlvList&) = delete;
    TlvList& operator=(const TlvList&) = delete;

    // Allow moving
    TlvList(TlvList&&) = default;
    TlvList& operator=(TlvList&&) = default;

    std::vector<UP_Path_TLV> mTlvs;

    TlvIterator FirstIterator() const;
};

struct TlvIterator final
{
public:
    TlvIterator(relive::Path_TLV* tlv, const TlvList* list, std::size_t index)
     : mTlv(tlv), mTlvList(list), mIndex(index)
    {

    }

    TlvIterator Next_TLV() const
    {
        if (mTlvList && mIndex + 1 < mTlvList->mTlvs.size())
        {
            return TlvIterator(mTlvList->mTlvs[mIndex + 1].get(), mTlvList, mIndex + 1);
        }
        return Invalid();
    }

    relive::Path_TLV* GetTlv() const 
    {
        return mTlv;
    }

    template<typename T>
    T* GetTlv() 
    {
        return static_cast<T*>(mTlv);
    }

    bool IsValid()
    {
        return mTlvList != nullptr;
    }

    static TlvIterator Invalid()
    {
        return TlvIterator(nullptr, nullptr, 0);
    }

private:
    relive::Path_TLV* mTlv = nullptr;
    const TlvList* mTlvList = nullptr;
    u32 mIndex = 0;
};

inline TlvIterator TlvList::FirstIterator() const
{
    relive::Path_TLV* pTlv = mTlvs.empty() ? nullptr : mTlvs[0].get();
    return TlvIterator(pTlv, this, 0);
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

        void AddTLV(UP_Path_TLV tlv)
        {
            mTlvs.mTlvs.emplace_back(std::move(tlv));
        }
        TlvList mTlvs;
        
    };

    BinaryPath(const std::string& jsonFileName, u32 pathId)
        : mJsonFileName(jsonFileName), mPathId(pathId)
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
            if (cam && cam->mX == x && cam->mY == y)
            {
                return cam->mName.c_str();
            }
        }
        return nullptr;
    }

    u32 CameraNameAsInteger(const char* pCamName) const
    {
        if (pCamName[1] != 0)
        {
            // Handle 10-99
            return 1 * (pCamName[1] - '0') + 10 * (pCamName[0] - '0');
        }
        else
        {
            // Handle 0-9
            return 1 * (pCamName[0] - '0');
        }
    }

    TlvIterator TlvById(const Guid& id);

    const TlvList& TlvsForCamera(s32 x, s32 y)
    {
        for (auto& cam : mCameras)
        {
            if (cam && cam->mX == x && cam->mY == y)
            {
                return cam->mTlvs;
            }
        }
        ALIVE_FATAL("Camera X/Y out of bounds");
    }

    std::vector<std::unique_ptr<CamEntry>>& GetCameras()
    {
        return mCameras;
    }

    std::vector<PathLine>& GetCollisions()
    {
        return mCollisions;
    }

    const std::shared_ptr<PathSoundInfo>& GetSoundInfo() const
    {
        return mSoundInfo;
    }

    std::shared_ptr<PathSoundInfo>& GetSoundInfo()
    {
        return mSoundInfo;
    }

    const std::string& JsonFileName() const
    {
        return mJsonFileName;
    }

private:
    std::string mJsonFileName;
    std::vector<std::unique_ptr<CamEntry>> mCameras;
    std::vector<PathLine> mCollisions;

    // TODO: Don't make this a ptr, just done for now because SND_Load_VABS keeps a ptr to it
    std::shared_ptr<PathSoundInfo> mSoundInfo;
    u32 mPathId = 0;
};
