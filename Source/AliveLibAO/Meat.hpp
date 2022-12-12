#pragma once

#include "BaseAliveGameObject.hpp"
#include "Throwable.hpp"

namespace relive
{
    struct Path_MeatSack;
}

namespace AO {

class MeatSack final : public BaseAliveGameObject
{
public:
    MeatSack(relive::Path_MeatSack* pTlv, const Guid& tlvId);
    ~MeatSack();
    
    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    Guid mTlvId;
    bool mDoMeatSackIdleAnim = false;
    s16 mMeatAmount = 0;
    bool mPlayWobbleSound = false;
    FP mTlvVelX = {};
    FP mTlvVelY = {};
};


class Meat final : public BaseThrowable
{
public:
    Meat(FP xpos, FP ypos, s16 count);
    ~Meat();

    virtual void VScreenChanged() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual s16 VCanThrow() override;
    virtual bool VCanEatMe();
    virtual s16 VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;
    virtual void VUpdate() override;
    virtual void VOnTrapDoorOpen() override;
    virtual s16 VGetCount() override;

    s16 VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override
    {
        return OnCollision(pPlatform);
    }

private:
    void InTheAir();
    void AddToPlatform();
    s16 OnCollision(BaseAnimatedWithPhysicsGameObject* pObj);

private:
    s16 mState = 0; // TODO: enum - we can probably use the AE enum
    FP mPreviousXPos = {};
    FP mPreviousYPos = {};
    s32 mShimmerTimer = 0;
    s32 mDeadTimer = 0;

public:
    PathLine* mPathLine = nullptr;
};


} // namespace AO
