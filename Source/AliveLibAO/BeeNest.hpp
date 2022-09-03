#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

struct Path_BeeNest final : public Path_TLV
{
    s16 mSwitchId;
    s16 mSwarmSize;
    s16 mChaseTime;
    s16 mSpeed;
    s16 mDisabledResources;
    s16 mBeesAmount;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BeeNest, 0x24);

enum class BeeNestStates : u16
{
    eWaitForTrigger_0,
    eResetIfDead_1
};

class BeeSwarm;


class BeeNest final : public ::BaseGameObject
{
public:
    BeeNest(relive::Path_BeeNest* pTlv, const Guid& tlvId);
    ~BeeNest();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    FP mBeeSwarmX = {};
    FP mBeeSwarmY = {};
    Guid mTlvInfo;
    u16 mSwitchId = 0;
    s16 mSwarmSize = 0;
    u16 mTotalChaseTime = 0;
    BeeNestStates mState = BeeNestStates::eWaitForTrigger_0;
    FP mSpeed = {};
    BeeSwarm* mBeeSwarm = nullptr;
};
ALIVE_ASSERT_SIZEOF(BeeNest, 0x38);


} // namespace AO
