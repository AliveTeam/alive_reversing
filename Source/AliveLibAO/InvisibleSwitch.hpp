#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/Psx_common.hpp"

namespace AO {

enum class SwitchOp : s16;

class InvisibleSwitch final : public BaseGameObject
{
public:
    InvisibleSwitch(relive::Path_InvisibleSwitch* pTlv, const Guid& tlvId);
    ~InvisibleSwitch();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 mSwitchId = 0;
    relive::reliveSwitchOp mAction = relive::reliveSwitchOp::eSetTrue;
    Guid mTlvId;
    s32 mDelayTimer = 0;
    s32 mActivationDelay = 0;
    PSX_Point mTlvTopLeft = {};
    PSX_Point mTlvBottomRight = {};
    enum class States : u16
    {
        eWaitForTrigger_0 = 0,
        eWaitForDelayTimer_1 = 1,
    };
    States mState = States::eWaitForTrigger_0;
    relive::reliveChoice mSetOffAlarm = relive::reliveChoice::eNo;
    relive::Path_InvisibleSwitch::InvisibleSwitchScale mScale = relive::Path_InvisibleSwitch::InvisibleSwitchScale::eFull;
};


} // namespace AO
