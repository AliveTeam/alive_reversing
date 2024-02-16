#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/SaveStateBase.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

enum class SpawnerStates : s16
{
    eInactive_0 = 0,
    eSligSpawned_1 = 1,
};

struct SligSpawnerSaveState final : public SaveStateBase
{
    SligSpawnerSaveState()
        : SaveStateBase(ReliveTypes::eSligSpawner, sizeof(*this))
    { }
    Guid mTlvId;
    SpawnerStates mState;
    Guid mSpawnedSligId;
};

class SligSpawner final : public BaseGameObject
{
public:
    SligSpawner(relive::Path_Slig* pTlv, const Guid& tlvId);
    ~SligSpawner();

    static void CreateFromSaveState(SerializedObjectData& pBuffer);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;

private:
    Guid mTlvInfo;
    s16 mSligSpawnerSwitchId = 0;
    bool mDontDestroyTLV = true;
    bool mUnlimitedSpawns = false;
    relive::Path_TLV mPathTlv = {};
    SpawnerStates mState = SpawnerStates::eInactive_0;
    Guid mSpawnedSligId;
    bool mFindSpawnedSlig = false;
};
