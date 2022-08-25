#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../AliveLibAO/Slig.hpp"

namespace AO {

struct Path_Slig;

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
    s16 field_2C_unused; // unused
    s16 mCode1;
    s16 mCode2;
    Choice_short mChaseAbeWhenSpotted;
    XDirection_short mFacing;
    s16 mPanicTimeout;
    s16 field_38_num_panic_sounds; // unused
    s16 field_3A_panic_sound_timeout; // unused
    s16 mStopChaseDelay;
    s16 mTimeToWaitBeforeChase;
    s16 mSligBoundId;
    s16 mAlertedListenTime;
    s16 mPercentSayWhat;
    s16 mPercentBeatMud;
    s16 field_48_talk_to_abe; // unused
    s16 field_4A_dont_shoot; // unused
    s16 mZShootDelay;
    Choice_short mStayAwake;
    s16 mDisabledResources;
    s16 mNoiseWakeUpDistance;
    s32 mSligSpawnerSwitchId;
};
// TODO: size

class SligSpawner final : public BaseGameObject
{
public:
    SligSpawner(relive::Path_Slig* pTlv, s32 tlvInfo);
    ~SligSpawner();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s32 mTlvInfo = 0;
    u16 mSligSpawnerSwitchId = 0;
    s16 mSpawnerFlags = 0;
    relive::Path_TLV mPathTlv = {};
};
ALIVE_ASSERT_SIZEOF(SligSpawner, 0x30);

} // namespace AO
