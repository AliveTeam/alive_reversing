#pragma once

#include "../relive_lib/GameObjects/BaseThrowable.hpp"
#include "../relive_lib/SaveStateBase.hpp"

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
    eFallingBlowUpOnGround_9 = 9,
};

enum eLineTypes : s16;

struct GrenadeSaveState final : public SaveStateBase
{
    GrenadeSaveState()
        : SaveStateBase(ReliveTypes::eGrenade, sizeof(*this))
    { }
    Guid mTlvInfo;
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
    bool mExplodeNow;
    bool mBlowUpOnCollision;
    Guid mPlatformId;
    eLineTypes mCollisionLineType;
    s16 mThrowableCount;
    GrenadeStates mState;
    s16 mBounceCount;
    s16 mExplodeCountdown;
    FP mPreviousXPos;
    FP mPreviousYPos;
};

class Grenade final : public BaseThrowable
{
public:
    Grenade(FP xpos, FP ypos, s16 numGrenades, bool bBlowUpOnCollision, BaseGameObject* pOwner);
    ~Grenade();

    virtual void VScreenChanged() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    virtual void VUpdate() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual bool VCanThrow() override;
    virtual bool VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;


    static void CreateFromSaveState(SerializedObjectData& pBuffer);

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

private:
    void Init(FP xpos, FP ypos);
    void BlowUp(bool bSmallExplosion);
    bool TimeToBlowUp();
    bool InTheAir(bool blowUpOnFloorTouch);
    void AddToPlatform();
    bool OnCollision_BounceOff(BaseGameObject* pHit);
    bool OnCollision_InstantExplode(BaseGameObject* pHit);


private:
    Guid mExplosionId;
    GrenadeStates mState = GrenadeStates::eFallingToBeCollected_0;
    s16 mExplodeCountdown = 0;
    s16 mBounceCount = 0;
    FP mPreviousXPos = {};
    FP mPreviousYPos = {};
    bool mBlowUpOnCollision = false;
    bool mExplodeNow = false;
    BaseGameObject* mGrenadeOwner = nullptr;
    bool mDoBounceOff = false;
};
