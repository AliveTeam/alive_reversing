#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Psx.hpp"
#include "Path.hpp"
#include "Slig.hpp"

struct Path_SligSpawner final : public Path_TLV
{
    Scale_short mScale;
    Path_Slig::StartState mStartState;
    s16 mPauseTime;
    s16 mPauseLeftMin;
    s16 mPauseLeftMax;
    s16 mPauseRightMin;
    s16 mPauseRightMax;
    Path_Slig::ShootPossessedSligs mShootPossessedSligs;
    s16 mShootOnSightDelay;
    s16 mNumTimesToShoot;
    s16 padding; // unused
    s16 mCode1;
    s16 mCode2;
    Choice_short mChaseAbeWhenSpotted;
    XDirection_short mFacing;
    s16 mPanicTimeout;
    s16 num_panic_sounds; // unused
    s16 panic_sound_timeout; // unused
    s16 mStopChaseDelay;
    s16 mTimeToWaitBeforeChase;
    s16 mSligBoundId;
    s16 mAlertedListenTime;
    s16 mPercentSayWhat;
    s16 mPercentBeatMud;
    s16 talk_to_abe; // unused
    s16 dont_shoot; // unused
    s16 mZShootDelay;
    Choice_short mStayAwake;
    s16 mDisabledResources;
    s16 mNoiseWakeUpDistance;
    s16 mSligSpawnerSwitchId;
    Choice_short mUnlimitedSpawns;
};

enum class SpawnerStates : s16
{
    eInactive_0 = 0,
    eSligSpawned_1 = 1,
};

struct Slig_Spawner_State final
{
    AETypes mType;
    s16 padding1;
    s32 mTlvInfo;
    SpawnerStates mState;
    s16 padding2;
    s32 mSpawnedSligId;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slig_Spawner_State, 0x10);

class SligSpawner final : public BaseGameObject
{
public:
    SligSpawner(Path_Slig* pTlv, s32 tlvInfo);
    ~SligSpawner();

    static s32 CreateFromSaveState(const u8* pBuffer);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    s32 mTlvInfo = 0;
    s16 mSligSpawnerSwitchId = 0;
    enum SpawnerFlags : s16
    {
        eBit1_DontDestroyTLV = 0x1,
        eBit2_UnlimitedSpawns = 0x2,
    };
    BitField16<SpawnerFlags> mSpawnerFlags = {};
    Path_TLV mPathTlv = {};
    SpawnerStates mState = SpawnerStates::eInactive_0;
    s32 mSpawnedSligId = 0;
    s16 mFindSpawnedSlig = 0;
};
ALIVE_ASSERT_SIZEOF(SligSpawner, 0x44);
