#include "Path.hpp"
#include "Factory.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

namespace AO {
void Path::TLV_Reset(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    if (tlvId.levelId == gMap.mCurrentLevel)
    {
        BinaryPath* pBinPath = gMap.GetPathResourceBlockPtr(tlvId.pathId);
        if (pBinPath)
        {
            relive::Path_TLV* pTlv = pBinPath->TlvsForCamera(tlvId.camX, tlvId.camY, tlvId.tlvOffset);

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
