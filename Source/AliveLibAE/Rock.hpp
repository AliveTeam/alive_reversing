#pragma once

#include "Throwable.hpp"
#include "../relive_lib/SaveStateBase.hpp"

enum class LevelIds : s16;
enum eLineTypes : s16;

enum class RockStates : s16
{
    eNone_0 = 0,
    eFallingOutOfRockSack_1 = 1,
    eRolling_2 = 2,
    eOnGround_3 = 3,
    eBouncing_4 = 4,
    eFallingOutOfWorld_5 = 5,
};

struct RockSaveState final : public SaveStateBase
{
    RockSaveState()
        : SaveStateBase(ReliveTypes::eRock, sizeof(*this))
    { }
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
    eLineTypes mCollisionLineType;
    s16 mThrowableCount;
    RockStates mState;
    s16 mBounceCount;
    FP mPreviousXPos;
    FP mPreviousYPos;
};

class Rock final : public BaseThrowable
{
public:
    Rock(FP xpos, FP ypos, s16 count);
    ~Rock();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual bool VCanThrow() override;
    virtual bool VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;

    static void CreateFromSaveState(SerializedObjectData& pData);

    virtual bool VOnPlatformIntersection(BaseAnimatedWithPhysicsGameObject* pPlatform) override
    {
        return OnCollision(pPlatform);
    }

private:
    void InTheAir();
    bool OnCollision(BaseAnimatedWithPhysicsGameObject* pObj);

    void BounceHorizontally( FP hitX, FP hitY );

private:
    RockStates mState = RockStates::eNone_0;
    s16 mBounceCount = 0;
    FP mPreviousXPos = {};
    FP mPreviousYPos = {};
    s32 mShimmerTimer = 0;
};
