#pragma once

#include "Throwable.hpp"

enum class MeatStates : s16
{
    eCreated_0 = 0,
    eIdle_1 = 1,
    eBeingThrown_2 = 2,
    eBecomeAPickUp_3 = 3,
    eWaitForPickUp_4 = 4,
    eFall_5 = 5,
};

struct MeatSaveState final
{
    ReliveTypes mType;
    Guid mTlvId;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    FP mSpriteScale;
    s16 mCurrentPath;
    EReliveLevelIds mCurrentLevel;
    bool mRender;
    bool mDrawable;
    bool mLoop;
    bool mInteractive;
    Guid mPlatformId;
    s16 mLineType;
    s16 mThrowableCount;
    MeatStates mState;
    FP mPreviousXPos;
    FP mPreviousYPos;
    s32 mDeadTimer;
};

class Meat final : public BaseThrowable
{
public:
    Meat(FP xpos, FP ypos, s16 count);
    ~Meat();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual bool VCanThrow() override;
    virtual bool VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;
    virtual s16 VGetCount() override;
    virtual bool VCanEatMe();
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

    virtual bool VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override
    {
        return OnCollision(pPlatform);
    }

private:
    void AddToPlatform();
    void InTheAir();
    bool OnCollision(BaseGameObject* pHit);

private:
    MeatStates mState = MeatStates::eCreated_0;
    FP mPreviousXPos = {};
    FP mPreviousYPos = {};
    s32 mShimmerTimer = 0;
    s32 mDeadTimer = 0;

public:
    PathLine* mPathLine = nullptr;
};

