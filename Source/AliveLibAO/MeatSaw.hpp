#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

struct Path_MeatSaw : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_min_time_off1;
    __int16 field_1C_max_time_off1;
    __int16 field_1E_max_rise_time;
    __int16 field_20_id;
    __int16 field_22_type;
    __int16 field_24_speed;
    __int16 field_26_start_state;
    __int16 field_28_off_speed;
    __int16 field_2A_min_time_off2;
    __int16 field_2C_max_time_off2;
    __int16 field_2E_inital_position;
};
ALIVE_ASSERT_SIZEOF(Path_MeatSaw, 0x30);

class MeatSaw : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MeatSaw* ctor_439570(Path_MeatSaw* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_4398F0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT MeatSaw* Vdtor_43A0B0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_43A060();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4399D0();

    EXPORT void GrindUpObjects_439CD0();

    virtual void VRender(int** pOrderingTable) override;

    EXPORT void VRender_439F50(int** ppOt);

    int field_D4[4];
    __int16 field_E4_state;
    unsigned __int16 field_E6_max_rise_time;
    __int16 field_E8_speed2;
    __int16 field_EA_speed1;
    __int16 field_EC_off_speed;
    unsigned __int16 field_EE_switch_id;
    __int16 field_F0_switch_value;
    __int16 field_F2_switch_value;
    __int16 field_F4;
    __int16 field_F6_min_time_off1;
    __int16 field_F8_field_1C_max_time_off1;
    __int16 field_FA_min_time_off2;
    __int16 field_FC_max_time_off2;
    __int16 field_FE;
    int field_100_tlvInfo;
    int field_104;
    int field_108;
    int field_10C;
    Animation field_110_anim;
    unsigned __int16 field_1A8_flags;
    __int16 field_1AA;
};
ALIVE_ASSERT_SIZEOF(MeatSaw, 0x1AC);


}

