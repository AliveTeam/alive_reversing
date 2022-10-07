#include "Path.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "Map.hpp"

namespace AO {
void Path::TLV_Reset(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    BinaryPath* pBinPath = gMap.GetPathResourceBlockPtr(gMap.mCurrentPath);
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
        }
    }
}
} // namespace AO
