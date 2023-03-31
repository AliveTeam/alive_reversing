#pragma once
#include "Voice.hpp"
#include "AliveAudio.hpp"

namespace psx {

float Voice::GetSample()
{
    if (b_Dead) // Dont return anything if dead. This voice should now be removed.
        return 0;

    // ActiveDecayLevel -= ((1.0 / AliveAudioSampleRate) / m_Tone->DecayTime);

    if (ActiveDecayLevel < 0)
        ActiveDecayLevel = 0;

    if (!b_NoteOn)
    {
        ActiveReleaseLevel -= ((1.0 / AliveAudioSampleRate) / m_Tone->ReleaseTime);

        ActiveReleaseLevel -= ((1.0 / AliveAudioSampleRate) / m_Tone->DecayTime); // Hacks?!?! --------------------

        if (ActiveReleaseLevel <= 0) // Release is done. So the voice is done.
        {
            b_Dead = true;
            ActiveReleaseLevel = 0;
        }
    }
    else
    {
        ActiveAttackLevel += ((1.0 / AliveAudioSampleRate) / m_Tone->AttackTime);

        if (ActiveAttackLevel > 1)
            ActiveAttackLevel = 1;
    }

    // For some reason, for samples that dont loop, they need to be cut off 1 sample earlier.
    // Todo: Revise this. Maybe its the loop flag at the end of the sample!?
    if ((m_Tone->Loop) ? f_SampleOffset >= m_Tone->m_Sample->i_SampleSize : (f_SampleOffset >= m_Tone->m_Sample->i_SampleSize - 1))
    {
        if (m_Tone->Loop)
            f_SampleOffset = 0;
        else
        {
            b_Dead = true;
            return 0; // Voice is dead now, so don't return anything.
        }
    }

    // From midi.cpp -- auto freq = pow(1.059463094359, (f64) (note - v29) * 0.00390625);
    double sampleFrameRate = pow(1.059463, i_Note - m_Tone->c_Center + m_Tone->Pitch + f_Pitch) * (44100.0 / AliveAudioSampleRate);
    f_SampleOffset += (sampleFrameRate);

    return ((float) (m_Tone->m_Sample->GetSample(float(f_SampleOffset)) * ActiveAttackLevel * ActiveDecayLevel * ((b_NoteOn) ? ActiveSustainLevel : ActiveReleaseLevel) * (f_Velocity * f_VelocityMulti)));
}

} // namespace psx