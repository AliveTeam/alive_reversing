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
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnTlvCollision(relive::Path_TLV* pTlv) override;

    static s32 CreateFromSaveState(const u8* pData);

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

extern u16 sSlurg_Step_Watch_Points_Idx_5C1C08;
extern s8 sSlurg_Step_Watch_Points_Count_5BD4DC[2];
extern Slurg_Step_Watch_Points sSlurg_Step_Watch_Points_5C1B28[2];
