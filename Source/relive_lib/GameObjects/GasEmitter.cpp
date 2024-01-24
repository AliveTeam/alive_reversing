#include "stdafx.h"
#include "GasEmitter.hpp"
#include "Math.hpp"
#include "../Sound/Midi.hpp"
#include "../SwitchStates.hpp"
#include "Particle.hpp"
#include "Sfx.hpp"
#include "../../AliveLibAE/GasCountDown.hpp"
#include "../GameType.hpp"

static GasEmitter* sMainGasEmitter = nullptr;
static u32 sGasEmitterAudioMask = 0;

GasEmitter::GasEmitter(relive::Path_GasEmitter* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eNone);

    if (GetGameType() == GameType::eAo)
    {
        mSmokeScale = FP_FromInteger(1);
        mParticleCount = 3;
        mGasColour = relive::Path_GasEmitter::GasColour::eGreen;
    }
    else
    {
        mSmokeScale = FP_FromDouble(0.5);
        mParticleCount = 1;
        mGasColour = pTlv->mColour;
        mSwitchId = pTlv->mSwitchId;
    }

    mTlvId = tlvId;

    mEmitterXPos = FP_FromInteger(pTlv->mTopLeftX);
    mEmitterYPos = FP_FromInteger(pTlv->mTopLeftY);

    mEmitPower = Math_NextRandom() % 4;
}

GasEmitter::~GasEmitter()
{
    GetMap().TLV_Reset(mTlvId);

    if (sMainGasEmitter == this)
    {
        sMainGasEmitter = 0;
        SND_Stop_Channels_Mask(sGasEmitterAudioMask);
        sGasEmitterAudioMask = 0;
    }
}

void GasEmitter::VStopAudio()
{
    if (sMainGasEmitter == this)
    {
        sMainGasEmitter = 0;
        SND_Stop_Channels_Mask(sGasEmitterAudioMask);
        sGasEmitterAudioMask = 0;
    }
}

void GasEmitter::VScreenChanged()
{
    SetDead(true);
}

void GasEmitter::VUpdate()
{
    if (gDeathGasOn && !((sGnFrame + mEmitPower) % 4) || (SwitchStates_Get(mSwitchId) && mDrawFlipper && Math_RandomRange(0, 1)))
    {
        switch (mGasColour)
        {
            case relive::Path_GasEmitter::GasColour::eYellow:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, mSmokeScale, mParticleCount, RGB16{128, 128, 32});
                break;

            case relive::Path_GasEmitter::GasColour::eRed:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, mSmokeScale, mParticleCount, RGB16{128, 32, 32});
                break;

            case relive::Path_GasEmitter::GasColour::eGreen:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, mSmokeScale, mParticleCount, RGB16{32, 128, 32});
                break;

            case relive::Path_GasEmitter::GasColour::eBlue:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, mSmokeScale, mParticleCount, RGB16{32, 32, 128});
                break;

            case relive::Path_GasEmitter::GasColour::eWhite:
                New_Smoke_Particles(mEmitterXPos, mEmitterYPos, mSmokeScale, mParticleCount, RGB16{128, 128, 128});
                break;

            default:
                break;
        }

        if (!sMainGasEmitter)
        {
            SfxPlayMono(relive::SoundEffects::Gas1, 127);
            sMainGasEmitter = this;
            sGasEmitterAudioMask = SfxPlayMono(relive::SoundEffects::Gas2, 127);
        }
    }

    // Flip the value
    mDrawFlipper = !mDrawFlipper;
}
