#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

START_NS_AO

struct Path_Bat : public Path_TLV
{
    __int16 field_18_ticks_before_moving;
    __int16 field_1A_speed;
    unsigned __int16 field_1C_scale;
    __int16 field_1E_attack_duration;
};
ALIVE_ASSERT_SIZEOF(Path_Bat, 0x20);

class PathLine;

class Bat : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Bat* ctor_4046E0(Path_Bat* pTlv, int tlvInfo);
    EXPORT BaseGameObject* dtor_404870();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_404FE0();
    EXPORT Bat* Vdtor_404FF0(signed int flags);

    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    PathLine* field_E4_pLine;
    int field_E8_speed;
    int field_EC;
    int field_F0_tlvInfo;
    __int16 field_F4_state;
    __int16 field_F6_attack_duration;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;
    BaseGameObject* field_10C;
};
ALIVE_ASSERT_SIZEOF(Bat, 0x110);

END_NS_AO

