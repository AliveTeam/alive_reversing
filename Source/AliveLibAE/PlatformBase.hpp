#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"

class PlatformBase : public BaseAliveGameObject
{
public:
    PlatformBase();
    ~PlatformBase();

    virtual void VAdd(BaseAliveGameObject* pObj);
    virtual void VRemove(BaseAliveGameObject* pObj);

    // TODO: New virtuals

protected:
    void AddDynamicCollision(s32 frameTableOffset, s32 maxW, u16 maxH, u8** ppAnimData, Path_TLV* pTlv, Map* pMap, s32 tlvInfo);

    void SyncCollisionLinePosition();
    void vRemoveCount(BaseAliveGameObject* pObj);

    void vAddCount(BaseAliveGameObject* pObj);

protected:
    s32 mPlatformBaseCount;
    s16 mPlatformBaseXOffset;
    s16 mPlatformBaseWidthOffset;
    s16 mPlatformBaseYOffset;
    s16 mPlatformBaseHeightOffset;
    PathLine* field_124_pCollisionLine;
    s32 mPlatformBaseTlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);
