#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

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

    s32 field_D4 = 0;
    s32 field_D8 = 0;
    s32 field_DC = 0;
    s32 field_E0 = 0;

    Guid field_E4_tlvInfo;
    s16 field_E8 = 0;
    s16 field_EA = 0;
    s32 field_EC_rnd1= 0;
    s32 field_F0_rnd2= 0;
    s32 field_F4_rnd3= 0;
    s32 field_F8_rnd4= 0;
};

} // namespace AO
