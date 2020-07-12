#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

START_NS_AO

class FireBackgroundGlow;
class FlameSparks;

struct Path_DoorFlame : public Path_TLV
{
    __int16 field_18_id;
    unsigned __int16 field_1A_scale;
    __int16 field_1C_colour;
    __int16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_DoorFlame, 0x20);

class DoorFlame : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT DoorFlame* ctor_432860(Path_DoorFlame* pTlv, int tlvInfo);

    virtual void VStopAudio() override;

    EXPORT void VStopAudio_432B60();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* dtor_432AA0();

    EXPORT DoorFlame* Vdtor_432DF0(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_432BA0();

    int field_D4[4];
    int field_E4_tlvInfo;
    __int16 field_E8_switch_id;
    __int16 field_EA_frame_count;
    unsigned __int16 field_EC_state;
    __int16 field_EE_2_random;
    int field_F0_sounds_mask;
    int field_F4;
    FireBackgroundGlow* field_F8_pFireBackgroundGlow;
    FlameSparks* field_FC_pFlameSparks;
};
ALIVE_ASSERT_SIZEOF(DoorFlame, 0x100);

END_NS_AO
