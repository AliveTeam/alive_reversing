#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_SlapLock final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_target_tomb_id1;
    s16 field_14_target_tomb_id2;
    Choice_short field_16_bPersistant;
    Choice_short field_18_has_ghost;
    Choice_short mGiveInvisibilityPowerup;
    s16 mInvisibilityDuration;
    s16 field_1E_toggle_switch_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlapLock, 0x20);

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

struct SlapLock_State final
{
    AETypes field_0_type;
    s16 mAnimRender;
    s32 mTlvInfo;
    s8 mTlvState;
    s8 field_9_padding;
    SlapLockStates mState;
    s32 mTimer1;
    s32 field_10_obj_id;
    s32 mShinyParticleTimer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLock_State, 0x18);

class SlapLock final : public BaseAliveGameObject
{
public:
    SlapLock(Path_SlapLock* pTlv, s32 tlvInfo);
    ~SlapLock();

    virtual void VUpdate() override;
    
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    void GiveInvisibility();
    void SetInvisibilityTarget();

private:
    Path_SlapLock* mSlapLockTlv = nullptr;
    s32 mTlvInfo = 0;
    SlapLockStates mState = SlapLockStates::eShaking_0;
    s32 mTimer1 = 0;
    Choice_short mHasGhost = Choice_short::eNo_0;
    s32 mAbilityRingId = 0;
    s32 mPossessionFlickerId = 0;
    s32 mShinyParticleTimer = 0;
};
ALIVE_ASSERT_SIZEOF(SlapLock, 0x140);
