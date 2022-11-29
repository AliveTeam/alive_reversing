#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/PathTlvsAO.hpp"

namespace AO {

class SwitchStateBooleanLogic final : public ::BaseGameObject
{
public:
    SwitchStateBooleanLogic(Path_SwitchStateBooleanLogic* pTlv, const Guid& tlvId);
    ~SwitchStateBooleanLogic();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    u16 field_10_input_1 = 0;
    u16 field_12_input_2 = 0;
    u16 field_14_output = 0;
    Path_SwitchStateBooleanLogic::BooleanOperatorType field_16_operator = Path_SwitchStateBooleanLogic::BooleanOperatorType::eAllOn_0;
    Guid field_18_tlvInfo;
};


} // namespace AO
