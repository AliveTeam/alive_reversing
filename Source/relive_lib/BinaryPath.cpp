#include "stdafx.h"
#include "BinaryPath.hpp"
#include "data_conversion/relive_tlvs_serialization.hpp"
#include "../AliveLibAO/Path.hpp"
#include "nlohmann/json.hpp"

static void from_json(const nlohmann::json& j, PathSoundInfo& s)
{
    j.at("vh_file").get_to(s.mVhFile);
    j.at("vb_file").get_to(s.mVbFile);
    j.at("seq_files").get_to(s.mSeqFiles);
}

void BinaryPath::CreateFromJson(nlohmann::json& pathJson)
{
    // TODO: Do a pass to collect the total required buffer size

    nlohmann::json& collisions = pathJson["collisions"];
    mCollisions.resize(collisions.size());
    s32 idx = 0;
    for (auto& collision : collisions)
    {
        collision.get_to(mCollisions[idx]);
        idx++;
    }

    for (auto& cam : pathJson["cameras"])
    {
        auto camEntry = std::make_unique<CamEntry>();
        camEntry->mX = cam["x"];
        camEntry->mY = cam["y"];
        camEntry->mId = cam["id"];
        camEntry->mName = cam["name"];

        auto& mapObjects = cam["map_objects"];
        for (auto i = 0u; i < mapObjects.size(); i++)
        {
            const auto& type = mapObjects.at(i)["tlv_type"];

            camEntry->mLastAllocated->mLength = camEntry->mLastAllocatedSize;

            if (i == mapObjects.size() - 1)
            {
                camEntry->mLastAllocated->mTlvFlags.Set(relive::TlvFlags::eBit3_End_TLV_List);
            }
        }
        mCameras.emplace_back(std::move(camEntry));
    }

    mSoundInfo = std::make_shared<PathSoundInfo>();
    from_json(pathJson["sound_info"], *mSoundInfo);
}

relive::Path_TLV* BinaryPath::TlvsById(const Guid& id)
{
    for (auto& cam : mCameras)
    {
        auto pPathTLV = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
        while (pPathTLV)
        {
            if (pPathTLV->mId == id)
            {
                return pPathTLV;
            }

            if (pPathTLV->mTlvFlags.Get(relive::eBit3_End_TLV_List))
            {
                break;
            }
            pPathTLV = AO::Path_TLV::Next_446460(pPathTLV);
        }
    }
    return nullptr;
}
