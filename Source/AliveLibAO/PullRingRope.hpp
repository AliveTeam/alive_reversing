#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "SwitchStates.hpp"

START_NS_AO

struct Path_PullRingRope : public Path_TLV
{
    __int16 field_18_id;
    SwitchOp field_1A_action;
    unsigned __int16 field_1C_rope_length;
    unsigned __int16 field_1E_scale;
    __int16 field_20_on_sound;
    __int16 field_22_off_sound;
    __int16 field_24_sound_direction;
    __int16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF(Path_PullRingRope, 0x28);

class Rope;

class PulllRingRope : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT PulllRingRope* ctor_4546B0(Path_PullRingRope* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_454910();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT PulllRingRope* Vdtor_454D80(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_454D70();

    EXPORT BOOL sub_454D60();

    EXPORT __int16 Pull_454CB0(BaseAliveGameObject* pFrom);


    int field_D4[4];
    int field_E4_stay_in_state_ticks;
    int field_E8;
    __int16 field_EC_state;
    __int16 field_EE_id;
    SwitchOp field_F0_action;
    __int16 field_F2_pad;
    BaseAliveGameObject* field_F4_pPuller;
    Rope* field_F8_pRope;
    __int16 field_FC_on_sound;
    __int16 field_FE_off_sound;
    __int16 field_100_sound_direction;
    __int16 field_102;
};
ALIVE_ASSERT_SIZEOF(PulllRingRope, 0x104);


END_NS_AO
