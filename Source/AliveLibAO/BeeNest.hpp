#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"

enum class EReliveLevelIds : s16;

namespace relive
{
    struct Path_BeeNest;
}

namespace AO {

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


} // namespace AO
