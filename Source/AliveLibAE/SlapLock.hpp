#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp" // reliveChoice only
#include "../relive_lib/SaveStateBase.hpp"

namespace relive
{
    struct Path_SlapLock;
}

enum class SlapLockStates : s16
{
    eShaking_0 = 0,
    eIdle_1 = 1,
    eSlapped_2 = 2,
    eBroken_3 = 3,
    eEmitInvisibilityPowerupRing_4 = 4,
    eFlickerHero_5 = 5,
    eGiveInvisibilityFromFlicker_6 = 6,
    eGiveInvisibility_7 = 7,
};

struct SlapLockSaveState final : public SaveStateBase
{
    SlapLockSaveState()
        : SaveStateBase(ReliveTypes::eSlapLock, sizeof(*this))
    { }
    s16 mAnimRender;
    Guid mTlvInfo;
    s8 mTlvState;
    s8 padding;
    SlapLockStates mState;
    s32 mTimer1;
    Guid mAbilityRingId;
    s32 mShinyParticleTimer;
};

class SlapLock final : public BaseAliveGameObject
{
public:
    SlapLock(relive::Path_SlapLock* pTlv, const Guid& tlvId);
    ~SlapLock();

    void LoadAnimations();

    virtual void VUpdate() override;
    
    virtual void VScreenChanged() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;

    static void CreateFromSaveState(SerializedObjectData& pBuffer);

private:
    void GiveInvisibility();
    void SetInvisibilityTarget();

private:
    relive::Path_SlapLock* mSlapLockTlv = nullptr;
    Guid mTlvInfo;
    SlapLockStates mState = SlapLockStates::eShaking_0;
    s32 mTimer1 = 0;
    relive::reliveChoice mHasGhost = relive::reliveChoice::eNo;
    Guid mAbilityRingId;
    Guid mPossessionFlickerId;
    s32 mShinyParticleTimer = 0;
};
