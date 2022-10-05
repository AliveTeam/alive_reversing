#pragma once

#include "Soundbank.hpp"

float Sample::GetSample(float sampleOffset)
{
    AliveAudioHelper helper;

    //if (!AliveAudio::Interpolation)
    return helper.SampleSint16ToFloat(m_SampleBuffer[(int) sampleOffset]); // No interpolation. Faster but sounds jaggy.

    //int roundedOffset = (int) floor(sampleOffset);
    //return helper.SampleSint16ToFloat(s16(helper.Lerp(m_SampleBuffer[roundedOffset], m_SampleBuffer[roundedOffset + 1], sampleOffset - float(roundedOffset))));
}

Soundbank::Soundbank(std::vector<Sample*> samples, std::vector<Program*> programs)
{
    m_Samples = samples;
    m_Programs = programs;
}
Soundbank::~Soundbank()
{

}