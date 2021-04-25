#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class TestAnimation : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    void ctor();
    void SyncToAbePos();

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VRender(PrimHeader** ppOt) override;

    void DelayLoad();

    bool mLoaded;
};
