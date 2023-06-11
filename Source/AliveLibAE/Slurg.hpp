#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_Slurg;
}

struct Slurg_Step_Watch_Point final
{
    u16 x;
    u16 y;
};
ALIVE_ASSERT_SIZEOF(Slurg_Step_Watch_Point, 0x4);

struct Slurg_Step_Watch_Points final
{
    Slurg_Step_Watch_Point mPoints[5];
};
ALIVE_ASSERT_SIZEOF(Slurg_Step_Watch_Points, 0x14);

enum class SlurgStates : s16
{
    eMoving_0 = 0,
    ePausing_1 = 1,
    eBurst_2 = 2
};

struct SlurgSaveState final
{
    ReliveTypes mType;
    s16 padding1;
    FP mXPos;
    FP mYPos;
    FP mVelX;
    FP mSlurgSpriteScale;
    s16 mFlipX;
    s16 mCurrentMotion;
    s16 mAnimCurrentFrame;
    s16 mFrameChangeCounter;
    s8 mDrawable;
    s8 mRender;
    s16 padding2;
    s32 mFrameTableOffset;
    Guid mTlvId;
    SlurgStates mSlurgState;
    bool mGoingRight; // TODO: double check me
    bool mMoving;
};

class Slurg final : public BaseAliveGameObject
{
public:
    static void Clear_Slurg_Step_Watch_Points();

    void LoadAnimations();

    Slurg(relive::Path_Slurg* pTlv, const Guid& tlvId);
    ~Slurg();

    virtual void VUpdate() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;

    static void CreateFromSaveState(SerializedObjectData& pData);

private:
    void GoLeft();
    void GoRight();
    void Burst();

    bool mGoingRight = false; // TODO: double check me
    bool mMoving = false;
    s16 mSlurgSwitchId = 0;
    SlurgStates mSlurgState = SlurgStates::eMoving_0;
    s16 mMovingTimer = 0;
    s16 mRngForMovingTimer = 0;
    PathLine* mSlurgLine = nullptr; // why not use the BaseAliveGameObject line?
    relive::Path_TLV* mSlurgTlv = nullptr; // dito
    Guid mTlvInfo;
    FP mSlurgSpriteScale = {}; // just use BaseAnimatedWithPhysicsGameObject sprite scale?
};

extern u16 gSlurgStepWatchPointsIdx;
extern s8 gSlurgStepWatchPointsCount[2];
extern Slurg_Step_Watch_Points gSlurgStepWatchPoints[2];
