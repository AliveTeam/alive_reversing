#include "stdafx.h"
#include "GasEmitter.hpp"
#include "Math.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Particle.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "GasCountDown.hpp"
#include "Function.hpp"

ALIVE_VAR(1, 0x5BD4C8, GasEmitter*, sMainGasEmitter_5BD4C8, nullptr);
ALIVE_VAR(1, 0x5BD4CC, u32, sGasEmiterAudioMask_5BD4CC, 0);

GasEmitter::GasEmitter(Path_GasEmitter* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eNone);

    field_28_draw_flipper = 1;

    field_2A_switch_id = pTlv->mSwitchId;
    field_2C_gas_colour = pTlv->mColour;

    field_20_tlvInfo = tlvInfo;

    field_30_xpos = FP_FromInteger(pTlv->mTopLeft.x);
    field_34_ypos = FP_FromInteger(pTlv->mTopLeft.y);

    // Probably scale ?
    field_38_fp_not_used = FP_FromInteger(1);

    field_24_emit_power = Math_NextRandom() % 4;
}

GasEmitter::~GasEmitter()
{
    Path::TLV_Reset(field_20_tlvInfo, -1, 0, 0);
    if (sMainGasEmitter_5BD4C8 == this)
    {
        sMainGasEmitter_5BD4C8 = 0;
        SND_Stop_Channels_Mask(sGasEmiterAudioMask_5BD4CC);
        sGasEmiterAudioMask_5BD4CC = 0;
    }
}

void GasEmitter::VStopAudio()
{
    if (sMainGasEmitter_5BD4C8 == this)
    {
        sMainGasEmitter_5BD4C8 = 0;
        SND_Stop_Channels_Mask(sGasEmiterAudioMask_5BD4CC);
        sGasEmiterAudioMask_5BD4CC = 0;
    }
}

void GasEmitter::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void GasEmitter::VUpdate()
{
    if ((gGasOn_5C1C00 && !((sGnFrame + field_24_emit_power) % 4)) || (SwitchStates_Get(field_2A_switch_id) && field_28_draw_flipper && Math_RandomRange(0, 1)))
    {
        switch (field_2C_gas_colour)
        {
            case GasColour::Yellow_0:
                New_Smoke_Particles(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 128, 128, 32);
                break;

            case GasColour::Red_1:
                New_Smoke_Particles(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 128, 32, 32);
                break;

            case GasColour::Green_2:
                New_Smoke_Particles(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 32, 128, 32);
                break;

            case GasColour::Blue_3:
                New_Smoke_Particles(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 32, 32, 128);
                break;

            case GasColour::White_4:
                New_Smoke_Particles(field_30_xpos, field_34_ypos, FP_FromDouble(0.5), 1, 128, 128, 128);
                break;

            default:
                break;
        }

        if (!sMainGasEmitter_5BD4C8)
        {
            SfxPlayMono(SoundEffect::Gas1_82, 127);
            sMainGasEmitter_5BD4C8 = this;
            sGasEmiterAudioMask_5BD4CC = SfxPlayMono(SoundEffect::Gas2_83, 127);
        }
    }

    // Flip the value
    field_28_draw_flipper = !field_28_draw_flipper;
}
