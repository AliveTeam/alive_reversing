#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_DoorFlame : public Path_TLV
{
    __int16 field_10_id;
    __int16 field_12_scale;
    __int16 field_14_colour;
    __int16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorFlame, 0x18);

class DoorFlame : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT DoorFlame* ctor_45E460(Path_DoorFlame* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_45E690(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_45E830();
    }

    virtual void VStopAudio() override
    {
        vStopAudio_45E7E0();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_45EA90();
    }

private:
    EXPORT DoorFlame* vdtor_45E690(signed int flags);

    EXPORT void dtor_45E6C0();

    EXPORT void vStopAudio_45E7E0();

    EXPORT void vScreenChanged_45EA90();

    EXPORT void vUpdate_45E830();

private:
    int field_F4_tlvInfo;
    __int16 field_F8_switch_id;
    __int16 field_FA_frame_count;
    enum class State : __int16
    {
        State_0_Off = 0,
        State_1_On = 1,
    };
    State field_FC_state;
    __int16 field_FE_2_random;
    int field_100_sounds_mask;
    __int16 field_104;
    __int16 field_106;
    int field_108_fire_background_glow_id;
    int field_10C_flame_sparks_id;
};
ALIVE_ASSERT_SIZEOF(DoorFlame, 0x110);
