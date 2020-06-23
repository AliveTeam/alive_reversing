#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO

struct Path_LightEffect : public Path_TLV
{
    enum class Type : __int16
    {
        Star_0 = 0,
        RedGoldGlow_1 = 1,
        GreenGlow_2 = 2,
        FlintGlow_3 = 3,
        FlintDoor_4 = 4,
        FlintHub_5 = 5,
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

    EXPORT DoorLight* Vdtor_4064A0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_406360();

    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4_tlvInfo;
    __int16 field_E8_size1;
    __int16 field_EA_size1;
    __int16 field_EC;
    __int16 field_EE_switch_value;
    __int16 field_F0;
    __int16 field_F2;
};
ALIVE_ASSERT_SIZEOF(DoorLight, 0xF4);

END_NS_AO

