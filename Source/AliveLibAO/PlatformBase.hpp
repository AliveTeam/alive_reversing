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

    void AddDynamicCollision(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, Path_TLV* pTlv, Map* pMap, s32 tlvInfo);
    void SyncCollisionLinePosition();
    void KeepThingsOnPlatform(FP xpos);

    s32 field_10C;
    s32 field_110;
    s32 mCount;
    s16 mXOffset;
    s16 mWidthOffset;
    s16 mYOffset;
    s16 mHeightOffset;
    PathLine* mCollisionLine;
    Map* mMap;
    s32 mTlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);

} // namespace AO
