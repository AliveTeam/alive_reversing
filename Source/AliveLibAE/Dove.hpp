#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

class Dove final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Dove(AnimId animId, const Guid& tlvId, FP scale);
    Dove(AnimId animId, FP xpos, FP ypos, FP scale);
    ~Dove();

    void LoadAnimations();
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
