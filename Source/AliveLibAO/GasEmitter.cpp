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

void GasEmitter::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void GasEmitter::VStopAudio()
{
    if (gGasEmitter_5009D4 == this)
    {
        gGasEmitter_5009D4 = 0;
        SND_Stop_Channels_Mask(gGasEmitterFx_5009D8);
        gGasEmitterFx_5009D8 = 0;
    }
}

GasEmitter::~GasEmitter()
{
    gMap.TLV_Reset(field_10_tlvInfo, -1, 0, 0);

    if (gGasEmitter_5009D4 == this)
    {
        gGasEmitter_5009D4 = 0;
        SND_Stop_Channels_Mask(gGasEmitterFx_5009D8);
        gGasEmitterFx_5009D8 = 0;
    }
}

GasEmitter::GasEmitter(Path_GasEmitter* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::eNone;

    field_10_tlvInfo = tlvInfo;

    field_18_xpos = FP_FromInteger(pTlv->mTopLeft.x);
    field_1C_ypos = FP_FromInteger(pTlv->mTopLeft.y);

    // Probably scale ?
    field_20_fp_not_used = FP_FromInteger(1);

    field_14_emit_power = Math_NextRandom() % 4;
}

void GasEmitter::VUpdate()
{
    if (gGasOn_4FF888 && !(sGnFrame + field_14_emit_power % 4))
    {
        New_Smoke_Particles_419A80(field_18_xpos, field_1C_ypos, field_20_fp_not_used, 3, 1);

        if (!gGasEmitter_5009D4)
        {
            SfxPlayMono(SoundEffect::Gas1_99, 127, 0);
            gGasEmitter_5009D4 = this;
            gGasEmitterFx_5009D8 = SfxPlayMono(SoundEffect::Gas2_100, 127, 0);
        }
    }
}

} // namespace AO
