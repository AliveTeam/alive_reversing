#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_LightEffect final : public Path_TLV
{
    enum class Type : s16
    {
        Star_0 = 0,
        GoldGlow_1 = 1,
        GreenGlow_2 = 2,
        FlintGlow_3 = 3,
        Switchable_RedGreenDoorLights_4 = 4,
        Switchable_RedGreenHubLight_5 = 5,
    };
    Type mType;
    s16 mSize;
    s16 mSwitchId;
    XDirection_short mDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LightEffect, 0x20);


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
