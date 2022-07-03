#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_Dove final : public Path_TLV
{
    s16 mDoveCount;
    Choice_short mPixelPerfect;
    Scale_short mScale;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Dove, 0x20);

class Dove final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Dove(AnimId animId, s32 tlvInfo, FP scale);
    Dove(AnimId animId, FP xpos, FP ypos, FP scale);
    ~Dove();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    static void All_FlyAway();

    void AsAlmostACircle(FP xpos, FP ypos, u8 angle);
    void AsACircle(FP xpos, FP ypos, u8 angle);
    void AsJoin(FP xpos, FP ypos);
    void FlyAway(bool spookedInstantly);

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
ALIVE_ASSERT_SIZEOF(Dove, 0x108);

} // namespace AO
