#pragma once

#include "../relive_lib/GameObjects/BaseThrowable.hpp"

namespace AO {

class Grenade final : public ::BaseThrowable
{
public:
    Grenade(FP xpos, FP ypos, s16 numGrenades);
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
    bool InTheAir();
    
    virtual bool VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override
    {
        return OnCollision_BounceOff(pPlatform);
    }

public:
    enum class States : s16
    {
        eFallingToBeCollected_0 = 0,
        eWaitToBeCollected_1 = 1,
        eDoesNothing_2 = 2,
        eCountingDown_3 = 3,
        eFalling_4 = 4,
        eHitGround_5 = 5,
        eWaitForExplodeEnd_6 = 6,
        eExploded_7 = 7
    };
    States mState = States::eFallingToBeCollected_0;
    s16 mExplodeCountdown = 0;
    s16 mBounceCount = 0;
    Guid mExplosionId = {};
    FP mPreviousXPos = {};
    FP mPreviousYPos = {};
};

extern bool gInfiniteGrenades;

} // namespace AO
