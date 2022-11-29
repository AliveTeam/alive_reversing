#pragma once

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

} // namespace AO
