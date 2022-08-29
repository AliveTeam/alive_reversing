#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

enum class BooleanOperatorType : s16
{
    eAnd_0 = 0,
    eAndNot_1 = 1,
    eOr_2 = 2,
    eOrNot_3 = 3,
};

struct Path_SwitchStateBooleanLogic final : public Path_TLV
{
    s16 field_18_input1;
    s16 field_1A_input2;
    s16 field_1C_output;
    BooleanOperatorType field_1E_operator;
};
ALIVE_ASSERT_SIZEOF(Path_SwitchStateBooleanLogic, 0x20);

class SwitchStateBooleanLogic final : public BaseGameObject
{
public:
    SwitchStateBooleanLogic(Path_SwitchStateBooleanLogic* pTlv, const Guid& tlvId);
    ~SwitchStateBooleanLogic();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    u16 field_10_input_1 = 0;
    u16 field_12_input_2 = 0;
    u16 field_14_output = 0;
    BooleanOperatorType field_16_operator = BooleanOperatorType::eAnd_0;
    Guid field_18_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(SwitchStateBooleanLogic, 0x1C);


} // namespace AO
