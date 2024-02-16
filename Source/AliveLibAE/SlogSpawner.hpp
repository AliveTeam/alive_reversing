#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/FixedPoint.hpp"

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
    bool mListenToSligs = false; // AE only
    s16 mChaseDelay = 0; // AE only
};
