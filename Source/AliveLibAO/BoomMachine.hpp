#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

START_NS_AO

class GrenadeMachineNozzel;

struct Path_BoomMachine : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_nozzel_side;
    __int16 field_1C_disabled_resources;
    __int16 field_1E_number_of_grenades;
    __int16 pad;
};
ALIVE_ASSERT_SIZEOF(Path_BoomMachine, 0x24);


class BoomMachine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BoomMachine* ctor_41E420(Path_BoomMachine* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_41E670();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BoomMachine* Vdtor_41E850(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41E7F0();

    EXPORT void VUpdate_41E750();

    EXPORT BOOL Vsub_41E840();

    EXPORT void Vsub_41E6F0();


    int field_D4[4];
    int field_E4_tlvInfo;
    __int16 field_E8_state;
    __int16 field_EA;
    GrenadeMachineNozzel* field_EC_pNozzel;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0xF0);

END_NS_AO

