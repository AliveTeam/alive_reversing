#include "Path.hpp"
#include "Map.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/BinaryPath.hpp"

namespace AO {

void Path::TLV_Reset(const Guid& tlvId, s16 hiFlags)
{
    Path::Set_TLVData(tlvId, hiFlags, 0, 0);
}

void Path::TLV_Persist(const Guid& tlvId, s16 hiFlags)
{
    Path::Set_TLVData(tlvId, hiFlags, 1, 0);
}

void Path::TLV_Delete(const Guid& tlvId, s16 hiFlags)
{
    Path::Set_TLVData(tlvId, hiFlags, 0, 1);
}

void Path::Set_TLVData(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    auto& paths = gMap.GetLoadedPaths();
    for (std::unique_ptr<BinaryPath>& pBinPath : paths)
    if (pBinPath)
    {
        relive::Path_TLV* pTlv = pBinPath->TlvsById(tlvId);
        if (pTlv)
        {
            pTlv->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed, bSetDestroyed & 1);
            pTlv->mTlvFlags.Set(relive::TlvFlags::eBit1_Created, bSetCreated & 1);

            if (hiFlags != -1)
            {
                // Seems to be a blob per TLV specific bits
                pTlv->mTlvSpecificMeaning = static_cast<u8>(hiFlags);
            }
            return;
        }
    }
}
} // namespace AO
