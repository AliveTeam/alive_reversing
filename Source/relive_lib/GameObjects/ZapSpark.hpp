#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class ZapSpark final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ZapSpark(FP xpos, FP ypos, FP scale);
    
    void LoadAnimationsAO();
    void LoadAnimationsAE();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 mSparkTimer = 0;
};
