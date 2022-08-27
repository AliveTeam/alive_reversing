#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"
#include "../relive_lib/BaseGameObject.hpp"

struct Path_FlyingSlig_Data final
{
    Scale_short mScale;
    enum class SpawnDelayStates : s16
    {
        eMoveImmediately_0 = 0,
        eUseCustomSpawnMoveDelay_1 = 1,
    };
    SpawnDelayStates mSpawnDelayState;
    s16 mSpawnMoveDelay;
    s16 mPatrolPauseMin;
    s16 mPatrolPauseMax;
    XDirection_short mFacing;
    s16 mPanicDelay;
    s16 mGiveUpChaseDelay;
    s16 mPrechaseDelay;
    s16 mSligBoundId;
    s16 mAlertedListenTime;
    s16 mSpawnerSwitchId;
    s16 mGrenadeDelay;
    s16 mMaxVelocity;
    s16 mLaunchGrenadeSwitchId;
    Choice_short mPersistant;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig_Data, 0x20);

struct Path_FlyingSlig final : public Path_TLV
{
    Path_FlyingSlig_Data mFlyingSligData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig, 0x30);

struct Path_FlyingSligSpawner final : public Path_TLV
{
    Path_FlyingSlig_Data mFlyingSligSpawnerData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSligSpawner, 0x30);

struct FlyingSligSpawner_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_tlvInfo;
    s32 field_8_bSpawned;
    s32 field_C_spawned_slig_obj_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSligSpawner_State, 0x10);

class FlyingSligSpawner final : public BaseGameObject
{
public:
    static s32 CreateFromSaveState(const u8* pBuffer);

    FlyingSligSpawner(relive::Path_FlyingSligSpawner* pTlv, s32 tlvInfo);
    ~FlyingSligSpawner();
    
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    s32 field_20_tlvInfo = 0;
    s32 field_24_spawned_slig_id = 0;
    s16 field_28_spawner_switch_id = 0;
    Path_TLV field_2C_tlv_header = {};
    s32 field_3C_bSpawned = 0;
    s16 field_40_bFirstUpdate = 0;
};
ALIVE_ASSERT_SIZEOF(FlyingSligSpawner, 0x44);
