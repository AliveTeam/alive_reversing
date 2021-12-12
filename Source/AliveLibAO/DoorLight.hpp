#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
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
    s16 field_1C_id;
    Choice_short field_1E_direction;
};
ALIVE_ASSERT_SIZEOF(Path_LightEffect, 0x20);


class DoorLight final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT DoorLight* ctor_405D90(Path_LightEffect* pTlv, s32 tlvInfo);
    EXPORT BaseGameObject* dtor_4062F0();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT DoorLight* Vdtor_4064A0(s32 flags);
    EXPORT void VScreenChanged_406360();
    EXPORT void VUpdate_4060A0();
    EXPORT void VRender_406370(PrimHeader** ppOt);

public:
    s32 field_D4_padding[4];
    s32 field_E4_tlvInfo;
    s16 field_E8_width;
    s16 field_EA_height;
    s16 field_EC_bHasID;
    s16 field_EE_switch_value;
    s16 field_F0_id;
    s16 field_F2_padding;
};
ALIVE_ASSERT_SIZEOF(DoorLight, 0xF4);

} // namespace AO
