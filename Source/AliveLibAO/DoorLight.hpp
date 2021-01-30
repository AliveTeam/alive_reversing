#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

struct Path_LightEffect : public Path_TLV
{
    enum class Type : __int16
    {
        Star_0 = 0,
        RedGlow_1 = 1,
        GreenGlow_2 = 2,
        FlintGlow_3 = 3,
        Switchable_RedGreenDoorLights_4 = 4,
        Switchable_RedGreenHubLight_5 = 5,
    };
    Type field_18_type;
    __int16 field_1A_size;
    __int16 field_1C_id;
    __int16 field_1E_flip_x;
};
ALIVE_ASSERT_SIZEOF(Path_LightEffect, 0x20);


class DoorLight : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT DoorLight* ctor_405D90(Path_LightEffect* pTlv, int tlvInfo);
    EXPORT BaseGameObject* dtor_4062F0();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT DoorLight* Vdtor_4064A0(signed int flags);
    EXPORT void VScreenChanged_406360();
    EXPORT void VUpdate_4060A0();
    EXPORT void VRender_406370(PrimHeader** ppOt);

public:
    int field_D4_padding[4];
    int field_E4_tlvInfo;
    __int16 field_E8_width;
    __int16 field_EA_height;
    __int16 field_EC_bHasID;
    __int16 field_EE_switch_value;
    __int16 field_F0_id;
    __int16 field_F2_padding;
};
ALIVE_ASSERT_SIZEOF(DoorLight, 0xF4);

}

