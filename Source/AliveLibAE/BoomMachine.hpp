#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_BoomMachine;
}

enum class BoomMachineStates : s16
{
    eInactive_0,
    eAlreadyUsed_1,
    eDropGrenadeAnimation_2,
    eDropGrenade_3
};

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
    Guid mNozzleId;
    bool mIsButtonOn = false;
};
