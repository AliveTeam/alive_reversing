#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class Map;

class PlatformBase : public BaseAliveGameObject
{
public:
    ~PlatformBase();

    virtual void VAdd(BaseAliveGameObject* pObj);
    virtual void VRemove(BaseAliveGameObject* pObj);

    void AddDynamicCollision(AnimId animId, relive::Path_TLV* pTlv, Map* pMap, const Guid& tlvId);
    void SyncCollisionLinePosition();
    void KeepThingsOnPlatform(FP xpos);

    s32 field_10C = 0;
    s32 field_110 = 0;
    s32 mPlatformBaseCount = 0;
    s16 mPlatformBaseXOffset = 0;
    s16 mPlatformBaseWidthOffset = 0;
    s16 mPlatformBaseYOffset = 0;
    s16 mPlatformBaseHeightOffset = 0;
    PathLine* mPlatformBaseCollisionLine = nullptr;
    Map* mPlatformBaseMap = nullptr;
    Guid mPlatformBaseTlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);

} // namespace AO
