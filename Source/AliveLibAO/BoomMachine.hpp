#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

class GrenadeMachineNozzle;

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
    Guid field_E4_tlvInfo;
    Guid field_F8_nozzle_id = {};
    s16 field_E8_bIsButtonOn = 0;
};

} // namespace AO
