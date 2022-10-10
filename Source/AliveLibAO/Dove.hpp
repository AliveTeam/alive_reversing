#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

class Dove final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Dove(AnimId animId, const Guid& tlvId, FP scale);
    Dove(AnimId animId, FP xpos, FP ypos, FP scale);
    ~Dove();
    
    void LoadAnimations();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    static void All_FlyAway();

    void AsAlmostACircle(FP xpos, FP ypos, u8 angle);
    void AsACircle(FP xpos, FP ypos, u8 angle);
    void AsJoin(FP xpos, FP ypos);
    void FlyAway(bool spookedInstantly);

    s16 mFlyAwayCounter = 0;
    Guid mTlvInfo;
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
};
ALIVE_ASSERT_SIZEOF(Dove, 0x108);

} // namespace AO
