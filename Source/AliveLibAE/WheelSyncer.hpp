#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class WheelSyncer final : public BaseGameObject
{
public:
    WheelSyncer(relive::Path_WheelSyncer* pTlv, const Guid& tlvId);
    ~WheelSyncer();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 mInputSwitchId1 = 0;
    s16 mInputSwitchId2 = 0;
    s16 mOutputSwitchId = 0;
    s16 mInputSwitchId3 = 0;
    s16 mInputSwitchId4 = 0;
    s16 mInputSwitchId5 = 0;
    s16 mInputSwitchId6 = 0;
    relive::Path_WheelSyncer::OutputRequirement mOutputRequirement = relive::Path_WheelSyncer::OutputRequirement::eAllOn;
    Guid mTlvInfo;
};
ALIVE_ASSERT_SIZEOF(WheelSyncer, 0x34);
