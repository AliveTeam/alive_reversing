#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

enum class BoomMachinePipeStates
{
    eInactive,
    eAlreadyUsed,
    eDropGrenadeAnimation,
    eDropGrenade
};

class BoomMachinePipe final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    BoomMachinePipe(FP xpos, FP ypos, FP scale, s16 numGrenades);

    void LoadAnimations();
    void DropGrenadeAnimation();
    void AlreadyUsed();

private:
    virtual void VUpdate() override;

    BoomMachinePipeStates mState = BoomMachinePipeStates::eInactive;
    s32 mTimer = 0;
    s16 mGrenadeCount = 0;
};
