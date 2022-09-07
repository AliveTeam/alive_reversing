#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

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
    s16 field_EE_switch_value = 0;
    s16 field_F0_switch_id = 0;
};
ALIVE_ASSERT_SIZEOF(DoorLight, 0xF4);

} // namespace AO
