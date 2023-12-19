#include "stdafx_ao.h"
#include "Function.hpp"
#include "GasEmitter.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Math.hpp"
#include "Midi.hpp"
#include "GasCountDown.hpp"
#include "Particle.hpp"

namespace AO {

ALIVE_VAR(1, 0x5009D4, GasEmitter*, gGasEmitter_5009D4, nullptr);
ALIVE_VAR(1, 0x5009D8, u32, gGasEmitterFx_5009D8, 0);


void GasEmitter::VUpdate()
{
    VUpdate_41D7D0();
}

void GasEmitter::VScreenChanged_41D900()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void GasEmitter::VScreenChanged()
{
    VScreenChanged_41D900();
}

GasEmitter* GasEmitter::Vdtor_41D910(s32 flags)
{
    dtor_41D840();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* GasEmitter::VDestructor(s32 flags)
{
    return Vdtor_41D910(flags);
}

void GasEmitter::VStopAudio_41D8D0()
{
    if (gGasEmitter_5009D4 == this)
    {
        gGasEmitter_5009D4 = 0;
        SND_Stop_Channels_Mask_4774A0(gGasEmitterFx_5009D8);
        gGasEmitterFx_5009D8 = 0;
    }
}

void GasEmitter::VStopAudio()
{
    VStopAudio_41D8D0();
}

BaseGameObject* GasEmitter::dtor_41D840()
{
    SetVTable(this, 0x4BAF20);

    gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, -1, 0, 0);

    if (gGasEmitter_5009D4 == this)
    {
        gGasEmitter_5009D4 = 0;
        SND_Stop_Channels_Mask_4774A0(gGasEmitterFx_5009D8);
        gGasEmitterFx_5009D8 = 0;
    }

    return dtor_487DF0();
}

GasEmitter* GasEmitter::ctor_41D760(Path_GasEmitter* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BAF20);

    field_4_typeId = Types::eNone_0;

    field_10_tlvInfo = tlvInfo;

    field_18_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_1C_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    // Probably scale ?
    field_20_fp_not_used = FP_FromInteger(1);

    field_14_emit_power = Math_NextRandom() % 4;

    return this;
}

void GasEmitter::VUpdate_41D7D0()
{
    if (gGasOn_4FF888 && !((gnFrameCount_507670 + field_14_emit_power) % 4))
    {
        New_Smoke_Particles_419A80(field_18_xpos, field_1C_ypos, field_20_fp_not_used, 3, 1);

        if (!gGasEmitter_5009D4)
        {
            SFX_Play_43AD70(SoundEffect::Gas1_99, 127, 0);
            gGasEmitter_5009D4 = this;
            gGasEmitterFx_5009D8 = SFX_Play_43AD70(SoundEffect::Gas2_100, 127, 0);
        }
    }
}

} // namespace AO
