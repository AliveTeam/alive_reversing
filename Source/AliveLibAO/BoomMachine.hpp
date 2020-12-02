#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

class GrenadeMachineNozzle;

struct Path_BoomMachine : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_nozzle_side;
    __int16 field_1C_disabled_resources;
    __int16 field_1E_number_of_grenades;
    __int16 pad;
};
ALIVE_ASSERT_SIZEOF(Path_BoomMachine, 0x24);

enum class BoomMachineStates : __int16
{
    eInactive_0,
    eAlreadyUsed_1,
    eDropGrenadeAnimation_2,
    eDropGrenade_3
};

class BoomMachine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BoomMachine* ctor_41E420(Path_BoomMachine* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_41E670();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BoomMachine* Vdtor_41E850(signed int flags);

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual BOOL VIsButtonOn();

    virtual void VHandleButton();

private:
    EXPORT void VScreenChanged_41E7F0();

    EXPORT void VUpdate_41E750();

    EXPORT BOOL VIsButtonOn_41E840();

    EXPORT void VHandleButton_41E6F0();

public:

    int field_D4_padding[4];
    int field_E4_tlvInfo;
    __int16 field_E8_bIsButtonOn;
    __int16 field_EA_padding;
    GrenadeMachineNozzle* field_EC_pNozzle;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0xF0);

}

