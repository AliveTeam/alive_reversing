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
    void AddDynamicCollision(AnimId animId, u8** ppAnimData, relive::Path_TLV* pTlv, const TLVUniqueId& tlvId);

    void SyncCollisionLinePosition();
    void vRemoveCount(BaseAliveGameObject* pObj);

    void vAddCount(BaseAliveGameObject* pObj);

protected:
    s32 mPlatformBaseCount = 0;
    s16 mPlatformBaseXOffset = 0;
    s16 mPlatformBaseWidthOffset = 0;
    s16 mPlatformBaseYOffset = 0;
    s16 mPlatformBaseHeightOffset = 0;
    PathLine* field_124_pCollisionLine = nullptr;
    TLVUniqueId mPlatformBaseTlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);
