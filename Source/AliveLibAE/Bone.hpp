#pragma once

#include "Throwable.hpp"

enum class BoneStates : s16
{
    eSpawned_0 = 0,
    eAirborne_1 = 1,
    eCollided_2 = 2,
    eOnGround_3 = 3,
    eEdible_4 = 4,
    eFalling_5 = 5
};

struct BoneSaveState final
{
    ReliveTypes mType;
    Guid mBaseTlvId;
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
    bool mHitObject;
    Guid mPlatformId;
    s16 mCollisionLineType;
    s16 mThrowableCount;
    BoneStates mState;
    s16 mBounceCount;
    FP mPreviousXPos;
    FP mPreviousYPos;
    s32 mTimeToLiveTimer;
};

class Bone final : public BaseThrowable
{
public:
    Bone(FP xpos, FP ypos, s16 countId);
    ~Bone();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual void VOnTrapDoorOpen() override;
    virtual bool VCanThrow() override;
    virtual bool VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;
    virtual bool VCanBeEaten();
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VGetCount() override;
    static s32 CreateFromSaveState(const u8* pData);

    virtual s16 VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override
    {
        return OnCollision(pPlatform);
    }

private:
    void AddToPlatform();
    s16 OnCollision(BaseAnimatedWithPhysicsGameObject* pObj);
    void InTheAir();

private:
    BoneStates mState = BoneStates::eSpawned_0;
    s16 mBounceCount = 0;
    FP mPreviousXPos = {};
    FP mPreviousYPos = {};
    s32 mShimmerTimer = 0;
    s32 mDeadTimer = 0;
    bool mHitObject = false;
};

