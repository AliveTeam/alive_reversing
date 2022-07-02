#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"

struct Path_Dove final : public Path_TLV
{
    s16 mDoveCount;
    Choice_short mPixelPerfect;
    Scale_short mScale;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Dove, 0x18);

class Dove final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, s32 tlvInfo, FP scale);
    Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, FP xpos, FP ypos, FP scale);
    ~Dove();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }


    void AsAlmostACircle(FP xpos, FP ypos, u8 angle);
    void AsACircle(FP xpos, FP ypos, u8 angle);
    void AsJoin(FP xpos, FP ypos);
    void FlyAway(bool spookedInstantly);

    static void All_FlyAway(bool spookedInstantly);

private:
    s16 mFlyAwayCounter = 0;
    s32 mTlvInfo = 0;
    s16 mKeepInGlobalArray = 0;
    enum class State : s16
    {
        eOnGround_0 = 0,
        eFlyAway_1 = 1,
        eJoin_2 = 2,
        eCircle_3 = 3,
        eAlmostACircle_4 = 4,
    };
    State mDoveState = State::eOnGround_0;
    FP mJoinX = {};
    FP mJoinY = {};
    s32 mJoinDeadTimer = 0;
    s8 mAngle = 0;
    FP mPrevX_Unused = {};
    FP mPrevY_Unused = {};
};
ALIVE_ASSERT_SIZEOF(Dove, 0x118);
