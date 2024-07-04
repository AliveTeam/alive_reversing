#pragma once

#include "../relive_lib/GameObjects/BaseThrowable.hpp"

namespace AO {

class Grenade final : public ::BaseThrowable
{
public:
    Grenade(FP xpos, FP ypos, s16 numGrenades, bool bBlowUpOnCollision, BaseGameObject* pOwner);
    ~Grenade();

    virtual bool VCanThrow() override;
    virtual bool VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void AddToPlatform();
    void BlowUp(bool bSmallExplosion);
    bool TimeToBlowUp();
    bool OnCollision_BounceOff(BaseGameObject* pHit);
    bool OnCollision_InstantExplode(BaseGameObject* pHit);
    bool InTheAir(bool blowUpOnFloorTouch);
    void Init(FP xpos, FP ypos);

    virtual bool VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override
    {
        if (mDoBounceOff)
        {
            return OnCollision_BounceOff(pPlatform);
        }
        else
        {
            return OnCollision_InstantExplode(pPlatform);
        }
    }

public:
    enum class GrenadeStates : s16
    {
        eFallingToBeCollected_0 = 0,
        eWaitToBeCollected_1 = 1,
        eDoesNothing_2 = 2,
        eCountingDown_3 = 3,
        eFalling_4 = 4,
        eHitGround_5 = 5,
        eWaitForExplodeEnd_6 = 6,
        eExploded_7 = 7,
        eDoesNothing_8 = 8,
        eFallingBlowUpOnGround_9,
    };
    GrenadeStates mState = GrenadeStates::eFallingToBeCollected_0;
    s16 mExplodeCountdown = 0;
    s16 mBounceCount = 0;
    Guid mExplosionId = {};
    FP mPreviousXPos = {};
    FP mPreviousYPos = {};
    bool mBlowUpOnCollision = false;
    bool mExplodeNow = false;
    BaseGameObject* mGrenadeOwner = nullptr;
    bool mDoBounceOff = false;
};

extern bool gInfiniteGrenades;

} // namespace AO
