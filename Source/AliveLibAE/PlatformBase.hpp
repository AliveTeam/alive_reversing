#pragma once

#include "BaseAliveGameObject.hpp"

class PlatformBase : public BaseAliveGameObject
{
public:
    PlatformBase();
    ~PlatformBase();

    virtual void VAdd(BaseAliveGameObject* pObj);
    virtual void VRemove(BaseAliveGameObject* pObj);

protected:
    void AddDynamicCollision(AnimId animId, relive::Path_TLV* pTlv, const Guid& tlvId);
    void SyncCollisionLinePosition();

protected:
    s32 mPlatformBaseCount = 0;
    s16 mPlatformBaseXOffset = 0;
    s16 mPlatformBaseWidthOffset = 0;
    s16 mPlatformBaseYOffset = 0;
    s16 mPlatformBaseHeightOffset = 0;
    PathLine* mPlatformBaseCollisionLine = nullptr;
    Guid mPlatformBaseTlvInfo;
};
