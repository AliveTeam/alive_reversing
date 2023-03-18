#pragma once

enum class ADSRPhase : u8
{
    Off = 0,
    Attack = 1,
    Decay = 2,
    Sustain = 3,
    Release = 4
};

class SPU
{
public:

    // should be called at 44100hz
    void Execute(u32 remaining_frames);

    s16 GetVoiceNoiseLevel();
    bool IsVoiceNoiseEnabled(u32 i);
    bool IsPitchModulationEnabled(u32 i);

    ADSRPhase GetNextADSRPhase(ADSRPhase phase);

    std::tuple<s32, s32> SampleVoice(u32 voice_index);
    void UpdateNoise();
    void ProcessReverb(s16 left_in, s16 right_in, s32* left_out, s32* right_out);
    void WriteToCaptureBuffer(u32 index, s16 value);
    void IncrementCaptureBufferPosition();

    u32 ReverbMemoryAddress(u32 address);
    s16 ReverbRead(u32 address, s32 offset = 0);
    void ReverbWrite(u32 address, s16 data);
};