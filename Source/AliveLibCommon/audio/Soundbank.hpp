#pragma once

namespace psx {

class AliveAudioHelper
{
public:
    float Lerp(u16 from, u16 to, float t)
    {
        return from + ((to - from) * t);
    }
     float SampleSint16ToFloat(s16 v)
    {
        return (v / 32767.0f);
    }

     float RandFloat(float a, float b)
    {
        return ((b - a) * ((float) rand() / RAND_MAX)) + a;
    }
};

/*
* Raw audio data. An audio sample.
*/
class Sample
{
public:
    s16* m_SampleBuffer;
    u32 i_SampleSize;
    u32 sampleRate;
    float GetSample(float sampleOffset);
};


/*
* Describes how to play a sample (ADSR curves and pitch shift)
*/
class Tone
{
public:
    // volume 0-1
    float f_Volume;
    s8 mode;

    // panning -1 - 1
    float f_Pan;

    // Root Key
    u8 c_Center;
    u8 c_Shift;

    // Key range
    unsigned char Min;
    unsigned char Max;

    float Pitch;

    double AttackTime;
    double ReleaseTime;
    bool ReleaseExponential = false;
    double DecayTime;
    double SustainTime;

    bool Loop = false;

    Sample* m_Sample;
};


/*
* A collection of tones (how to play samples)
*/
class Program
{
public:
    int prog_id;
    std::vector<Tone*> m_Tones;
};


/*
* Maintains a collection of Samples (raw audio) and
* Programs (how to play samples with ADSR and pitch)
*/
class Soundbank
{
public:
    ~Soundbank();
    Soundbank(std::vector<Sample*> samples, std::vector<Program*> programs);

    std::vector<Sample*> m_Samples;
    std::vector<Program*> m_Programs;
};

} // namespace psx
