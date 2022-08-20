#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

struct Path_BeeNest final : public Path_TLV
{
    s16 mSwitchId;
    s16 mBeesAmount;
    s16 mTotalChaseTime;
    s16 mSpeed;
    s16 field_20_disabled_resources;
    s16 field_22_num_bees;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BeeNest, 0x24);

enum class BeeNestStates : u16
{
    eWaitForTrigger_0,
    eResetIfDead_1
};

class BeeSwarm;


class BeeNest final : public BaseGameObject
{
public:
    BeeNest(Path_BeeNest* pTlv, s32 tlvInfo);
    ~BeeNest();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    FP mBeeSwarmX = {};
    FP mBeeSwarmY = {};
    s32 mTlvInfo = 0;
    u16 mSwitchId = 0;
    s16 mBeesAmount = 0;
    u16 mTotalChaseTime = 0;
    BeeNestStates mState = BeeNestStates::eWaitForTrigger_0;
    FP mSpeed = {};
    BeeSwarm* mBeeSwarm = nullptr;
};
ALIVE_ASSERT_SIZEOF(BeeNest, 0x38);


} // namespace AO
