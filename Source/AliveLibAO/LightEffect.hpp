#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_LightEffect;
}

namespace AO {

class LightEffect final : public BaseAnimatedWithPhysicsGameObject
{
public:
    LightEffect(relive::Path_LightEffect* pTlv, const Guid& tlvId);
    ~LightEffect();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    Guid mTlvId;
    s32 mRnd1 = 0;
    s32 mRnd2 = 0;
    s32 mRnd3 = 0;
    s32 mRnd4 = 0;
};

} // namespace AO
