#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_LightEffect;
}

namespace AO {

class DoorLight final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DoorLight(relive::Path_LightEffect* pTlv, const Guid& tlvId);
    ~DoorLight();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

public:
    Guid field_E4_tlvInfo;
    s16 field_E8_width = 0;
    s16 field_EA_height = 0;
    s16 field_EC_bHasID = 0;
    s32 field_EE_switch_value = 0;
    s16 field_F0_switch_id = 0;
};

} // namespace AO
