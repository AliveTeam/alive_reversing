#pragma once

#include "Throwable.hpp"

namespace AO {

class Rock final : public BaseThrowable
{
public:
    Rock(FP xpos, FP ypos, s16 count);
    ~Rock();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual s16 VCanThrow() override;
    virtual s16 VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;

    void InTheAir();
    s16 OnCollision(BaseAnimatedWithPhysicsGameObject* pObj);
    void BounceHorizontally( FP hitX, FP hitY );

    virtual s16 VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override
    {
        return OnCollision(pPlatform);
    }

    enum class States : s16
    {
        eNone_0 = 0,
        eFallingOutOfRockSack_1 = 1,
        eRolling_2 = 2,
        eOnGround_3 = 3,
        eBouncing_4 = 4,
        eFallingOutOfWorld_5 = 5,
    };
    States mState = States::eNone_0;
    PathLine* mPathLine = nullptr;
    s16 mBounceCount = 0;
    FP mPreviousXPos = {};
    FP mPreviousYPos = {};
    s32 mShimmerTimer = 0;
};


} // namespace AO
