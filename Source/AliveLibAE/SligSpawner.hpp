#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Psx.hpp"
#include "Slig.hpp"

enum class SpawnerStates : s16
{
    eInactive_0 = 0,
    eSligSpawned_1 = 1,
};

struct SligSpawnerSaveState final
{
    AETypes mType;
    s16 padding1;
    Guid mTlvInfo;
    SpawnerStates mState;
    s16 padding2;
    Guid mSpawnedSligId;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(SligSpawnerSaveState, 0x10);

class SligSpawner final : public BaseGameObject
{
public:
    SligSpawner(relive::Path_Slig* pTlv, const Guid& tlvId);
    ~SligSpawner();

    static s32 CreateFromSaveState(const u8* pBuffer);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    Guid mTlvInfo;
    s16 mSligSpawnerSwitchId = 0;
    enum SpawnerFlags : s16
    {
        eBit1_DontDestroyTLV = 0x1,
        eBit2_UnlimitedSpawns = 0x2,
    };
    BitField16<SpawnerFlags> mSpawnerFlags = {};
    relive::Path_TLV mPathTlv = {};
    SpawnerStates mState = SpawnerStates::eInactive_0;
    Guid mSpawnedSligId;
    s16 mFindSpawnedSlig = 0;
};
