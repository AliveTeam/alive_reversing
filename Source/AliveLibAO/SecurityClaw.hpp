#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "DynamicArray.hpp"

START_NS_AO

struct Path_SecurityClaw : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_alarm_id;
    __int16 field_1C_alarm_time;
    __int16 field_1E_disabled_resources;
};
ALIVE_ASSERT_SIZEOF(Path_SecurityClaw, 0x20);

class Claw : public BaseAnimatedWithPhysicsGameObject
{
public:
    void ctor();

    BaseGameObject* VDestructor(signed int flags);

    BaseGameObject* Vdtor(signed int flags);

};

class MotionDetector;

class SecurityClaw : public BaseAliveGameObject
{
public:
    EXPORT SecurityClaw* ctor_418A70(Path_SecurityClaw* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_418CE0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT SecurityClaw* Vdtor_419700(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChange_4196F0();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT __int16 VTakeDamage_419520(BaseGameObject* pFrom);

    virtual void VUpdate() override;

    EXPORT void VUpdate_418DE0();

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    int field_10C_tlvInfo;
    __int16 field_110_state;
    __int16 field_112;
    int field_114;
    __int16 field_118_alarm_id;
    __int16 field_11A;
    FP field_11C_clawX;
    FP field_120_clawY;
    BYTE field_124;
    // 3 byte pad
    int field_128_sound_channels;
    __int16 field_12C_pDetector;
    __int16 field_12E;
    Claw* field_130_pClaw;
    PSX_Point field_134;
    PSX_Point field_138;
    DynamicArrayT<MotionDetector>* field_13C_pArray;
};
ALIVE_ASSERT_SIZEOF(SecurityClaw, 0x140);

END_NS_AO

