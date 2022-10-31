#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class ZapSpark final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ZapSpark(FP xpos, FP ypos, FP scale);
    
    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 mSparkTimer = 0;
};

} // namespace AO
