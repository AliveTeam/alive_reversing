#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_DoorBlocker;
}

class DoorBlocker final : public BaseAliveGameObject
{
public:
    DoorBlocker(relive::Path_DoorBlocker* pTlv, const Guid& tlvId);
    ~DoorBlocker();

    void LoadAnimations();
    virtual void VUpdate() override;

private:
    bool mDone = false;
    s16 mSwitchId = 0;
    Guid mTlvId;
};
