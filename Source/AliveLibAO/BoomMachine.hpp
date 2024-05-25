#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_BoomMachine;
}

namespace AO {

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

} // namespace AO
