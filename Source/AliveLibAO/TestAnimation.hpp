#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class TestAnimation final : public AO::BaseAnimatedWithPhysicsGameObject
{
public:
    void ctor();
    void SyncToAbePos();

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    

    virtual void VRender(PrimHeader** ppOt) override;

    void DelayLoad();

    bool mLoaded;
};
