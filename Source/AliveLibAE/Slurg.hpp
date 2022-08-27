#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

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

struct Slurg_Path_Data final
{
    s16 mMovingTimer;
    XDirection_short mFacing;
    Scale_short mScale;
    s16 mSwitchId;
};

struct Path_Slurg : public Path_TLV
{
    Slurg_Path_Data mSlurgData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slurg, 0x18);

enum class SlurgStates : s16
{
    eMoving_0 = 0,
    ePausing_1 = 1,
    eBurst_2 = 2
};

enum SlurgFlags
{
    eGoingRight = 0x1, // TODO: double check me
    eMoving = 0x2,
};

struct Slurg_State final
{
    AETypes mType;
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
    s32 mTlvInfo;
    SlurgStates mSlurgState;
    BitField16<SlurgFlags> mSlurgFlags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slurg_State, 0x2C);

class Slurg final : public BaseAliveGameObject
{
public:
    static void Clear_Slurg_Step_Watch_Points();

    Slurg(relive::Path_Slurg* pTlv, u32 tlvInfo);
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

    BitField16<SlurgFlags> mSlurgFlags = {};
    s16 mSlurgSwitchId = 0;
    SlurgStates mSlurgState = SlurgStates::eMoving_0;
    s16 mMovingTimer = 0;
    s16 mRngForMovingTimer = 0;
    PathLine* mSlurgLine = nullptr; // why not use the BaseAliveGameObject line?
    relive::Path_TLV* mSlurgTlv = nullptr; // dito
    s32 mTlvInfo = 0;
    FP mSlurgSpriteScale = {}; // just use BaseAnimatedWithPhysicsGameObject sprite scale?
};
ALIVE_ASSERT_SIZEOF(Slurg, 0x134);

ALIVE_VAR_EXTERN(u16, sSlurg_Step_Watch_Points_Idx_5C1C08);
ALIVE_ARY_EXTERN(s8, 2, sSlurg_Step_Watch_Points_Count_5BD4DC);
ALIVE_ARY_EXTERN(Slurg_Step_Watch_Points, 2, sSlurg_Step_Watch_Points_5C1B28);
