#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_BoomMachine;
}

class BoomMachine final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BoomMachine(relive::Path_BoomMachine* pTlv, const Guid& tlvId);
    ~BoomMachine();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual bool VIsButtonOn();
    virtual void VHandleButton();

private:
    void LoadAnimations();

public:
    Guid mTlvId;
    Guid mPipeId;
    bool mIsButtonOn = false;
};
