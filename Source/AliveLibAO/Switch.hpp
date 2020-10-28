#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "ScopedSeq.hpp"

START_NS_AO

enum class SwitchOp : __int16;

struct Path_Switch : public Path_TLV
{
    __int16 field_18_trigger_object;
    SwitchOp field_1A_trigger_object_action;
    unsigned __int16 field_1C_scale;
    __int16 field_1E_on_sound;
    __int16 field_20_off_sound;
    __int16 field_22_sound_direction;
};
ALIVE_ASSERT_SIZEOF(Path_Switch, 0x24);


class Switch : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Switch* ctor_481110(Path_Switch* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_481260();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Switch* Vdtor_481700(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4816F0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4812D0();

    // New virtual
    virtual int VPull(__int16 a2);

    EXPORT int vPull_481640(__int16 a2);

    int field_D4[4];
    __int16 field_E4_switchId;
    __int16 field_E6_pad;
    __int16 field_E8_state;
    __int16 field_EA_pad;
    int field_EC_tlvInfo;
    __int16 field_F0;
    SwitchOp field_F2_operation;
    __int16 field_F4_on_sound;
    __int16 field_F6_off_sound;
    __int16 field_F8_sound_direction;
    __int16 field_FA_pad;
};
ALIVE_ASSERT_SIZEOF(Switch, 0xFC);

END_NS_AO

