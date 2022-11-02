#include "stdafx.h"
#include "GasEmitter.hpp"
#include "Math.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Particle.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "GasCountDown.hpp"
#include "Function.hpp"
#include "Path.hpp"

GasEmitter* sMainGasEmitter = nullptr;
u32 sGasEmiterAudioMask = 0;

GasEmitter::GasEmitter(relive::Path_GasEmitter* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eNone);

    field_28_draw_flipper = 1;

    field_2A_switch_id = pTlv->mSwitchId;
    field_2C_gas_colour = pTlv->mColour;

    mTlvId = tlvId;

    mEmitterXPos = FP_FromInteger(pTlv->mTopLeftX);
    mEmitterYPos = FP_FromInteger(pTlv->mTopLeftY);

    // Probably scale ?
    field_38_fp_not_used = FP_FromInteger(1);

    mEmitPower = Math_NextRandom() % 4;
}

GasEmitter::~GasEmitter()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);

    if (sMainGasEmitter == this)
    {
        sMainGasEmitter = 0;
        SND_Stop_Channels_Mask(sGasEmiterAudioMask);
        sGasEmiterAudioMask = 0;
    }
}

void GasEmitter::VStopAudio()
{
    if (sMainGasEmitter == this)
    {
        sMainGasEmitter = 0;
        SND_Stop_Channels_Mask(sGasEmiterAudioMask);
        sGasEmiterAudioMask = 0;
    }
}

void GasEmitter::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void GasEmitter::VUpdate()
{
    if ((gGasOn && !((sGnFrame + mEmitPower) % 4)) || (SwitchStates_Get(field_2A_switch_id) && field_28_draw_flipper && Math_RandomRange(0, 1)))
    {
        switch (field_2C_gas_colour)
        {
            case relive::Path_GasEmitter::GasColour::eYellow:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, FP_FromDouble(0.5), 1, RGB16{ 128, 128, 32 });
                break;

            case relive::Path_GasEmitter::GasColour::eRed:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, FP_FromDouble(0.5), 1, RGB16{ 128, 32, 32 });
                break;

            case relive::Path_GasEmitter::GasColour::eGreen:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, FP_FromDouble(0.5), 1, RGB16{ 32, 128, 32 });
                break;

            case relive::Path_GasEmitter::GasColour::eBlue:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, FP_FromDouble(0.5), 1, RGB16{ 32, 32, 128 });
                break;

            case relive::Path_GasEmitter::GasColour::eWhite:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, FP_FromDouble(0.5), 1, RGB16{ 128, 128, 128 });
                break;

            default:
                break;
        }

        if (!sMainGasEmitter)
        {
            SfxPlayMono(relive::SoundEffects::Gas1, 127);
            sMainGasEmitter = this;
            sGasEmiterAudioMask = SfxPlayMono(relive::SoundEffects::Gas2, 127);
        }
    }

    // Flip the value
    field_28_draw_flipper = !field_28_draw_flipper;
}
