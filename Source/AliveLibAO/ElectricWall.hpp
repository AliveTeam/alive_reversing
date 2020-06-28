#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

START_NS_AO

struct Path_ElectricWall : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_id;
    __int16 field_1C_start_state;
    __int16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ElectricWall, 0x20);


class ElectricWall : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ElectricWall* ctor_40FCF0(Path_ElectricWall* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_40FE80();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ElectricWall* Vdtor_410280(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_410220();

    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4_tlv;
    __int16 field_E8_switch_id;
    __int16 field_EA_start_state;
    int field_EC_sound_timer;
};
ALIVE_ASSERT_SIZEOF(ElectricWall, 0xF0);

END_NS_AO

