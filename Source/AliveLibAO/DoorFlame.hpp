#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

class FireBackgroundGlow;
class FlameSparks;

struct Path_DoorFlame : public Path_TLV
{
    s16 field_18_id;
    u16 field_1A_scale;
    enum class Colour : s16
    {
        red_1 = 1,
        green_2 = 2,
        blue_3 = 3
    };
    Colour field_1C_colour;
    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_DoorFlame, 0x20);

class DoorFlame : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT DoorFlame* ctor_432860(Path_DoorFlame* pTlv, s32 tlvInfo);

    virtual void VStopAudio() override;

    EXPORT void VStopAudio_432B60();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* dtor_432AA0();

    EXPORT DoorFlame* Vdtor_432DF0(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_432BA0();

    s32 field_D4_padding[4];
    s32 field_E4_tlvInfo;
    s16 field_E8_switch_id;
    s16 field_EA_frame_count;
    enum class States : u16
    {
        eDisabled_0 = 0,
        eEnabled_1 = 1
    };
    States field_EC_state;
    s16 field_EE_2_random;
    s32 field_F0_sounds_mask;
    s32 field_F4_padding;
    FireBackgroundGlow* field_F8_pFireBackgroundGlow;
    FlameSparks* field_FC_pFlameSparks;
};
ALIVE_ASSERT_SIZEOF(DoorFlame, 0x100);

} // namespace AO
