#include "stdafx_ao.h"
#include "Function.hpp"
#include "GasEmitter.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Math.hpp"
#include "Midi.hpp"
#include "GasCountDown.hpp"
#include "../relive_lib/Particle.hpp"
#include "Path.hpp"

namespace AO {

GasEmitter* gGasEmitter_5009D4 = nullptr;
u32 gGasEmitterFx_5009D8 = 0;

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
    Path::TLV_Reset(field_10_tlvInfo, -1, 0, 0);

    if (gGasEmitter_5009D4 == this)
    {
        gGasEmitter_5009D4 = 0;
        SND_Stop_Channels_Mask(gGasEmitterFx_5009D8);
        gGasEmitterFx_5009D8 = 0;
    }
}

GasEmitter::GasEmitter(relive::Path_GasEmitter* pTlv, const Guid& tlvId)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eNone);

    field_10_tlvInfo = tlvId;

    field_18_xpos = FP_FromInteger(pTlv->mTopLeftX);
    field_1C_ypos = FP_FromInteger(pTlv->mTopLeftY);

    // Probably scale ?
    field_20_fp_not_used = FP_FromInteger(1);

    field_14_emit_power = Math_NextRandom() % 4;
}

void GasEmitter::VUpdate()
{
    if (gGasOn_4FF888 && !(sGnFrame + field_14_emit_power % 4))
    {
        New_Smoke_Particles(field_18_xpos, field_1C_ypos, field_20_fp_not_used, 3, RGB16{ 32, 128, 32 });

        if (!gGasEmitter_5009D4)
        {
            SfxPlayMono(relive::SoundEffects::Gas1, 127);
            gGasEmitter_5009D4 = this;
            gGasEmitterFx_5009D8 = SfxPlayMono(relive::SoundEffects::Gas2, 127);
        }
    }
}

} // namespace AO
