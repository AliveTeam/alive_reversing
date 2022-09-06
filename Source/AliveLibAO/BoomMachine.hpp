#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class GrenadeMachineNozzle;

struct Path_BoomMachine final : public Path_TLV
{
    Scale_short mScale;
    enum class NozzleSide : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    NozzleSide mNozzleSide;
    s16 mDisabledResources;
    s16 mGrenadeAmount;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoomMachine, 0x20);

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

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual bool VIsButtonOn();
    virtual void VHandleButton();


public:
    Guid field_E4_tlvInfo;
    s16 field_E8_bIsButtonOn = 0;
    GrenadeMachineNozzle* field_EC_pNozzle = nullptr;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0xF0);

} // namespace AO
