#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/PathTlvsAO.hpp"

namespace AO {

class SwitchStateBooleanLogic final : public ::BaseGameObject
{
public:
    SwitchStateBooleanLogic(Path_SwitchStateBooleanLogic* pTlv, const Guid& tlvId);
    ~SwitchStateBooleanLogic();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    u16 mInput1 = 0;
    u16 mInput2 = 0;
    u16 mOutput = 0;
    Path_SwitchStateBooleanLogic::BooleanOperatorType mOperator = Path_SwitchStateBooleanLogic::BooleanOperatorType::eAllOn_0;
    Guid mTlvId;
};


} // namespace AO
