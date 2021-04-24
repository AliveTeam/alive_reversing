#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

namespace AO {

enum class BooleanOperatorType : s16
{
    eAnd_0 = 0,
    eAndNot_1 = 1,
    eOr_2 = 2,
    eOrNot_3 = 3,
};

struct Path_SwitchStateBooleanLogic : public Path_TLV
{
    s16 field_18_input1;
    s16 field_1A_input2;
    s16 field_1C_output;
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

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_436C60(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_436B60();

    u16 field_10_input_1;
    u16 field_12_input_2;
    u16 field_14_output;
    BooleanOperatorType field_16_operator;
    int field_18_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(SwitchStateBooleanLogic, 0x1C);


}

