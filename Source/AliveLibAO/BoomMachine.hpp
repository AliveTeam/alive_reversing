#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class GrenadeMachineNozzle;

struct Path_BoomMachine final : public Path_TLV
{
    Scale_short field_18_scale;
    enum class NozzleSide : s16
    {
        eRight_0 = 0,
        eLeft_1 = 1
    };
    NozzleSide field_1A_nozzle_side;
    s16 field_1C_disabled_resources;
    s16 field_1E_number_of_grenades;
};
ALIVE_ASSERT_SIZEOF(Path_BoomMachine, 0x20);

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
    BoomMachine(Path_BoomMachine* pTlv, s32 tlvInfo);
    ~BoomMachine();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual Bool32 VIsButtonOn();
    virtual void VHandleButton();


public:
    s32 field_E4_tlvInfo = 0;
    s16 field_E8_bIsButtonOn = 0;
    GrenadeMachineNozzle* field_EC_pNozzle = nullptr;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0xF0);

} // namespace AO
