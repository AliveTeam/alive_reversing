#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

enum class BooleanOperatorType : __int16
{
    eAnd_0 = 0,
    eAndNot_1 = 1,
    eOr_2 = 2,
    eOrNot_3 = 3,
};

struct Path_SwitchStateBooleanLogic : public Path_TLV
{
    __int16 field_18_input1;
    __int16 field_1A_input2;
    __int16 field_1C_output;
    BooleanOperatorType field_1E_operator;
};
ALIVE_ASSERT_SIZEOF(Path_SwitchStateBooleanLogic, 0x20);

class SwitchStateBooleanLogic : public BaseGameObject
{
public:
    EXPORT SwitchStateBooleanLogic* ctor_436AB0(Path_SwitchStateBooleanLogic* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_436B00();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_436C40();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_436C60(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_436B60();

    unsigned __int16 field_10_input_1;
    unsigned __int16 field_12_input_2;
    unsigned __int16 field_14_output;
    BooleanOperatorType field_16_operator;
    int field_18_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(SwitchStateBooleanLogic, 0x1C);


END_NS_AO

