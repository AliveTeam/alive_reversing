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

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual bool VIsButtonOn();
    virtual void VHandleButton();


public:
    Guid field_E4_tlvInfo;
    s16 field_E8_bIsButtonOn = 0;
    Guid field_F8_nozzle_id = {};
};

} // namespace AO
