#include "Path.hpp"
#include "Factory.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

namespace AO {
void Path::TLV_Reset(u32 tlvOffset_levelId_PathId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    TlvItemInfoUnion data;
    data.all = tlvOffset_levelId_PathId;

    if (data.parts.levelId == static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel)))
    {
        const auto pBlyRec = Path_Get_Bly_Record_434650(MapWrapper::FromAO(static_cast<LevelIds>(data.parts.levelId)), data.parts.pathId);

        u8** ppPathRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Path, data.parts.pathId, TRUE, FALSE);
        if (ppPathRes)
        {
            const s32 tlvOffset = data.parts.tlvOffset + pBlyRec->field_4_pPathData->field_14_object_offset;
            Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(&(*ppPathRes)[tlvOffset]);

            if (bSetDestroyed & 1)
            {
                pTlv->mTlvFlags.Set(TlvFlags::eBit2_Destroyed);
            }
            else
            {
                pTlv->mTlvFlags.Clear(TlvFlags::eBit2_Destroyed);
            }

            if (bSetCreated & 1)
            {
                pTlv->mTlvFlags.Set(TlvFlags::eBit1_Created);
            }
            else
            {
                pTlv->mTlvFlags.Clear(TlvFlags::eBit1_Created);
            }

            if (hiFlags != -1)
            {
                // Seems to be a blob per TLV specific bits
                pTlv->field_1_unknown = static_cast<u8>(hiFlags);
            }

            ResourceManager::FreeResource_455550(ppPathRes);
        }
    }
}
} // namespace AO
