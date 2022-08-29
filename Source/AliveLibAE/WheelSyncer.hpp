#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"

enum class WheelSyncerOutputRequirement : s16
{
    eAllOn_0 = 0,
    e1OnAnd2Off_1 = 1,
    e1Or2On_2 = 2,
    e1OnOr2Off_3 = 3
};

struct Path_WheelSyncer final : public Path_TLV
{
    s16 mInputSwitchId1;
    s16 mInputSwitchId2;
    s16 mOutputSwitchId;
    WheelSyncerOutputRequirement mOutputRequirement;
    s16 mInputSwitchId3;
    s16 mInputSwitchId4;
    s16 mInputSwitchId5;
    s16 mInputSwitchId6;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_WheelSyncer, 0x20);

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
