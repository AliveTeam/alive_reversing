#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class SlogSpawner final : public BaseGameObject
{
public:
    SlogSpawner(relive::Path_SlogSpawner* pTlv, const Guid& tlvId);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    Guid mTlvInfo;
    s16 mSpawnedSlogsCount = 0;
    FP mXPos = {};
    FP mYPos = {};
    s32 mSpawnTimer = 0;
    relive::reliveScale mScale = relive::reliveScale::eFull;
    s16 mMaxSlogs = 0;
    s16 mMaxSlogsAtATime = 0;
    relive::Path_SlogSpawner::StartDirection mStartDirection = relive::Path_SlogSpawner::StartDirection::eRight;
    s16 mSlogSpawnInterval = 0;
    s16 mSpawnerSwitchId = 0;
    relive::reliveChoice mListenToSligs = relive::reliveChoice::eNo; // AE only
    s16 mChaseDelay = 0; // AE only
};
ALIVE_ASSERT_SIZEOF(SlogSpawner, 0x44);
