#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class TestAnimation : public BaseAnimatedWithPhysicsGameObject
{
public:
    void ctor();
    void SyncToAbePos();

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VRender(int** pOrderingTable) override;
};
