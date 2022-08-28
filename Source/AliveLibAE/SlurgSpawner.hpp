#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Slurg.hpp"

struct Slurg_Spawner_Path_Data final
{
    s16 mSpawnInterval;
    s16 mMaxSlurgs;
    s16 mSwitchId;
    s16 padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slurg_Spawner_Path_Data, 0x8);

struct Path_SlurgSpawner final : public Path_Slurg
{
    Slurg_Spawner_Path_Data mSpawnerData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlurgSpawner, 0x20);

// it reads the slurg spawned count from the TLV but never updates it.
// Also it hasn't got quiksave support.
// https://github.com/AliveTeam/alive_reversing/issues/1526

class SlurgSpawner final : public BaseGameObject
{
public:
    SlurgSpawner(relive::Path_SlurgSpawner* pTlv, s32 tlvInfo);
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s32 mTlvInfo = 0;
    relive::Path_SlurgSpawner* mSlurgTlv = nullptr;
    Slurg_Spawner_Path_Data mSlurgPathData = {};
    s16 mSpawnCounter = 0;
    u8 mSpawnedCount = 0;
};
ALIVE_ASSERT_SIZEOF(SlurgSpawner, 0x3C);
