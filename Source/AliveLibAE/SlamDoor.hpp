#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_SlamDoor;
}

struct SlamDoorSaveState final
{
    ReliveTypes mType;
    Guid mTlvInfo;
};

class SlamDoor final : public BaseAliveGameObject
{
public:
    SlamDoor(relive::Path_SlamDoor* tlv_params, const Guid& tlvId);
    ~SlamDoor();

    void LoadAnimations();

    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    virtual void VUpdate() override;
    virtual void VOnThrowableHit(BaseGameObject*) override
    {
        // Empty
    }

    static void CreateFromSaveState(SerializedObjectData&);

private:
    void ClearInsideSlamDoor(IBaseAliveGameObject* pObj, s16 xPosition, s16 width);

private:
    bool mClosed = false;
    bool mOpen = false;
    bool mLastFrame = false;
    bool mSlamDoorFlipY = false;
    bool mDelete = false;
    PathLine* mCollisionLine1 = nullptr;
    PathLine* mCollisionLine2 = nullptr;
    s16 mCollisionX = 0;
    s16 mCollisionY = 0;
    s16 mSwitchId = 0;
    Guid mTlvInfo;
};
