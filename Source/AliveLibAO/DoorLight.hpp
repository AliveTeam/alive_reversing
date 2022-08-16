#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
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
    Type field_18_type;
    s16 field_1A_size;
    s16 field_1C_switch_id;
    XDirection_short field_1E_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LightEffect, 0x20);


class DoorLight final : public BaseAnimatedWithPhysicsGameObject
{
public:
    DoorLight(Path_LightEffect* pTlv, s32 tlvInfo);
    ~DoorLight();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

public:
    s32 field_E4_tlvInfo = 0;
    s16 field_E8_width = 0;
    s16 field_EA_height = 0;
    s16 field_EC_bHasID = 0;
    s16 field_EE_switch_value = 0;
    s16 field_F0_switch_id = 0;
};
ALIVE_ASSERT_SIZEOF(DoorLight, 0xF4);

} // namespace AO
