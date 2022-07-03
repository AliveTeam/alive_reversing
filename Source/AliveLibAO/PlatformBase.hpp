#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

class Map;

class PlatformBase : public BaseAliveGameObject
{
public:
    ~PlatformBase();

    virtual void VAdd(BaseAliveGameObject* pObj);
    virtual void VRemove(BaseAliveGameObject* pObj);

    void AddDynamicCollision(AnimId animId, u8** ppAnimData, Path_TLV* pTlv, Map* pMap, s32 tlvInfo);
    void SyncCollisionLinePosition();
    void KeepThingsOnPlatform(FP xpos);

    s32 field_10C;
    s32 field_110;
    s32 mPlatformBaseCount;
    s16 mPlatformBaseXOffset;
    s16 mPlatformBaseWidthOffset;
    s16 mPlatformBaseYOffset;
    s16 mPlatformBaseHeightOffset;
    PathLine* mPlatformBaseCollisionLine;
    Map* mPlatformBaseMap;
    s32 mPlatformBaseTlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);

} // namespace AO
