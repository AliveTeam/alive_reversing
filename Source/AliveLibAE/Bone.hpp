#pragma once

#include "Throwable.hpp"

namespace relive
{
    struct Path_BoneBag;
}

enum class BoneStates : s16
{
    eSpawned_0 = 0,
    eAirborne_1 = 1,
    eCollided_2 = 2,
    eOnGround_3 = 3,
    eEdible_4 = 4,
    eFalling_5 = 5
};

struct Bone_SaveState final
{
    AETypes mAEType;
    s16 field_2_padding;
    Guid field_4_obj_id;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mVelY;
    FP mSpriteScale;
    s16 mCurrentPath;
    LevelIds mCurrentLevel;

    enum BoneStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8,
        eBit5_bHitObject = 0x10,
    };

    BitField16<BoneStateFlags> field_20_flags;
    s16 field_22_padding;
    Guid field_24_base_id;
    s16 mCollisionLineType;
    s16 mBaseThrowableCount;
    BoneStates mState;
    s16 mVolumeModifier;
    FP mInitialXPos;
    FP mInitialYPos;
    s32 mTimeToLiveTimer;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(Bone_SaveState, 0x3C);

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

private:
    void AddToPlatform();
    s16 OnCollision(BaseAnimatedWithPhysicsGameObject* pObj);
    void InTheAir();

private:
    BoneStates mState = BoneStates::eSpawned_0;
    s16 mVolumeModifier = 0;
    FP mInitialXPos = {};
    FP mInitialYPos = {};
    s32 mShineTimer = 0;
    s32 mTimeToLiveTimer = 0;
    s16 mHitObject = 0; // TODO: flag
};

class BoneBag final : public BaseAliveGameObject
{
public:
    BoneBag(relive::Path_BoneBag* pTlv, const Guid& tlvId);
    ~BoneBag();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
 
private:
    Guid mTlvInfo;
    bool mIsBagHit = false;
    s16 mBoneAmount = 0;
    bool mAllowSound = false;
    bool mForcePlayWobbleSound = false;
    FP mVelX = {};
    FP mVelY = {};
};
