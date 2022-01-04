#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_DoorFlame final : public Path_TLV
{
    s16 field_10_switch_id;
    Scale_short field_12_scale;
    s16 field_14_colour;
    s16 field_16_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_DoorFlame, 0x18);

class DoorFlame final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT DoorFlame* ctor_45E460(Path_DoorFlame* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override
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
    EXPORT DoorFlame* vdtor_45E690(s32 flags);

    EXPORT void dtor_45E6C0();

    EXPORT void vStopAudio_45E7E0();

    EXPORT void vScreenChanged_45EA90();

    EXPORT void vUpdate_45E830();

private:
    s32 field_F4_tlvInfo;
    s16 field_F8_switch_id;
    s16 field_FA_frame_count;
    enum class States : s16
    {
        eDisabled_0 = 0,
        eEnabled_1 = 1,
    };
    States field_FC_state;
    s16 field_FE_2_random;
    s32 field_100_sounds_mask;
    s16 field_104_padding;
    s16 field_106_padding;
    s32 field_108_fire_background_glow_id;
    s32 field_10C_flame_sparks_id;
};
ALIVE_ASSERT_SIZEOF(DoorFlame, 0x110);
