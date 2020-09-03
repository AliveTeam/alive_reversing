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

    EXPORT void FlyTo_404E50(FP xpos, FP ypos, FP* xSpeed, FP* ySpeed);

    virtual void VUpdate() override;

    EXPORT void VUpdate_404950();

    int field_D4[4];
    PathLine* field_E4_pLine;
    FP field_E8_speed;
    int field_EC;
    int field_F0_tlvInfo;
    enum class States : __int16
    {
        e0 = 0,
        e1 = 1,
        e2 = 2,
        e3 = 3,
        e4 = 4,
        e5 = 5,
    };
    States field_F4_state;
    __int16 field_F6_attack_duration;
    int field_F8;
    int field_FC;
    FP field_100;
    FP field_104;
    FP field_108;
    BaseAliveGameObject* field_10C;
};
ALIVE_ASSERT_SIZEOF(Bat, 0x110);

END_NS_AO

