#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

enum class StartDirection : s16
{
    eRight_0 = 0,
    eLeft_1 = 1
};

struct Path_SlogSpawner final : public Path_TLV
{
    Scale_short mScale;
    s16 mMaxSlogs;
    s16 mMaxSlogsAtATime;
    StartDirection mStartDirection;
    s16 mSlogSpawnInterval;
    s16 mSpawnerSwitchId;
    Choice_short mListenToSligs;
    s16 mChaseDelay;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlogSpawner, 0x20);

class SlogSpawner final : public BaseGameObject
{
public:
    SlogSpawner(Path_SlogSpawner* pTlv, s32 tlvInfo);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s32 mTlvInfo = 0;
    s16 mSpawnedSlogsCount = 0;
    FP mXPos = {};
    FP mYPos = {};
    s32 mSpawnTimer = 0;
    Scale_short mScale = Scale_short::eFull_0;
    s16 mMaxSlogs = 0;
    s16 mMaxSlogsAtATime = 0;
    StartDirection mStartDirection = StartDirection::eRight_0;
    s16 mSlogSpawnInterval = 0;
    s16 mSpawnerSwitchId = 0;
    Choice_short mListenToSligs = Choice_short::eNo_0; // AE only
    s16 mChaseDelay = 0; // AE only
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x44);
