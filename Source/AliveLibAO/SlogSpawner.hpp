#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum class StartDirection : s16
{
    eRight_0 = 0,
    eLeft_1 = 1
};

struct Path_SlogSpawner final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 mMaxSlogs;
    s16 mMaxSlogsAtATime;
    StartDirection mStartDirection;
    s16 mSlogSpawnInterval;
    s16 mSpawnerSwitchId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogSpawner, 0x24);

class SlogSpawner final : public BaseGameObject
{
public:
    SlogSpawner(Path_SlogSpawner* pTlv, s32 tlvInfo);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s32 mTlvInfo = 0;
    s16 mSpawnedSlogsCount = 0;
    FP mXPos = {};
    FP mYPos = {};
    s32 mSpawnTimer = 0;
    Scale_short mScale = Scale_short::eFull_0;
    u16 mMaxSlogs = 0;
    u16 mMaxSlogsAtATime = 0;
    StartDirection mStartDirection = StartDirection::eRight_0;
    u16 mSlogSpawnInterval = 0;
    u16 mSpawnerSwitchId = 0;
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x30);


} // namespace AO
