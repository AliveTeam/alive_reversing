#pragma once

#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/SaveStateBase.hpp"
#include "../relive_lib/GameObjects/IBirdPortal.hpp"

enum class CameraSwapEffects : s16;

struct BirdPortalSaveState final : public SaveStateBase
{
    BirdPortalSaveState()
        : SaveStateBase(ReliveTypes::eBirdPortal, sizeof(*this))
    { }
    PortalStates mState;
    u8 mMudCountForShrykull;
    Guid mTlvInfo;
};

enum class Event : s16;

class BirdPortal final : public IBirdPortal
{
public:
    BirdPortal(relive::Path_BirdPortal* pTlv, const Guid& tlvId);
    ~BirdPortal();
    
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;

    virtual void VGiveShrykull(s16 bPlaySound) override;
    virtual void VExitPortal() override;
    virtual void VGetMapChange(EReliveLevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId) override;

    static void CreateFromSaveState(SerializedObjectData& pBuffer);

private:
    s16 IsScaredAway();
    void KillTerminators();
    Event GetEvent();
};
