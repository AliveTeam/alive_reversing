#pragma once

#include "BaseAliveGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

enum class EReliveLevelIds : s16;

enum class FartStates : s16
{
    eIdle_0 = 0,
    eFlying_1 = 1,
    eDechanting_2 = 2
};

struct EvilFartSaveState final
{
    ReliveTypes mType;
    s16 mRed;
    s16 mGreen;
    s16 mBlue;
    s16 mCurrentPath;
    EReliveLevelIds mCurrentLevel;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    FP mSpriteScale;
    s16 mCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mAnimRender;
    s8 mDrawable;
    EReliveLevelIds mAbeLevel;
    s16 mAbePath;
    s16 mAbeCamera;
    bool mControlled;
    bool mFartExploded;
    s16 mPossessedAliveTimer;
    FartStates mState;
    s16 field_32_padding;
    s32 mUnpossessionTimer;
    s32 mBackToAbeTimer;
};

class EvilFart final : public BaseAliveGameObject
{
public:
    EvilFart();

    virtual void VUpdate() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VPossessed() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    void InputControlFart();
    void ResetFartColour();
    void CalculateFartColour();
    void BlowUp();

private:
    bool mFartExploded = false;
    bool mPossessed = false;
    s16 mPossessedAliveTimer = 0;
    s16 mAbePath = 0;
    EReliveLevelIds mAbeLevel = EReliveLevelIds::eNone;
    s16 mAbeCamera = 0;
    FartStates mState = FartStates::eIdle_0;
    s32 mUnpossessionTimer = 0;
    s32 mBackToAbeTimer = 0;
    s32 mSoundChannels = 0;
};
