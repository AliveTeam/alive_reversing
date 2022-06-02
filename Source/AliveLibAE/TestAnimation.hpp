#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class TestAnimation final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    TestAnimation();
    void SyncToAbePos();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void DelayLoad();

    bool mLoaded;
};
