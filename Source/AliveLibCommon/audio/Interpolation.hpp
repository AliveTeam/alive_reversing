#pragma once

namespace SPU {

const u32 NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK = 3;
const u32 NUM_SAMPLES_PER_ADPCM_BLOCK = 28;

u32 mask(u32 num);

class VoiceCounter
{
public:
    virtual ~VoiceCounter() = default;
    virtual u32 CalculateNoteStep(s32 root, s32 rootFine, u32 srcRate, u8 note, u32 pitch) = 0;
    virtual u32 SampleIndex() = 0;
    virtual u32 InterpolationIndex() = 0;
    virtual bool NextSampleBlock(u32 sampleRate) = 0;
    virtual f32 Sample(s16* block) = 0;
    virtual void Reset() = 0;
};


/**
* This is the same implementation that duckstation uses (with some stuff taken from vgmtrans)
*/
class VoiceCounterDuckstation : public VoiceCounter
{
public:
    u32 CalculateNoteStep(s32 root, s32 rootFine, u32 srcRate, u8 note, u32 pitch) override;
    u32 SampleIndex() override;
    u32 InterpolationIndex() override;
    bool NextSampleBlock(u32 sampleRate) override;
    f32 Sample(s16* block) override;
    void Reset() override;

private:
    u32 bits;
};


/**
* This is Illeprih's implementation from discord server
*/
class VoiceCounterIlleprih : public VoiceCounter
{
public:
    u32 CalculateNoteStep(s32 root, s32 rootFine, u32 srcRate, u8 note, u32 pitch) override;
    u32 SampleIndex() override;
    u32 InterpolationIndex() override;
    bool NextSampleBlock(u32 sampleRate) override;
    f32 Sample(s16* block) override;
    void Reset() override;

private:
    u32 Counter = 0;
};

/// <summary>
/// A provider class for creating the different voice types
/// </summary>
class InterpolationProvider
{
public:
    virtual std::unique_ptr<VoiceCounter> Create() = 0;
};

class InterpolationProviderDuckstation : public InterpolationProvider
{
public:
    std::unique_ptr<VoiceCounter> Create() override
    {
        return std::make_unique<VoiceCounterDuckstation>();
    }
};

class InterpolationProviderIlleprih : public InterpolationProvider
{
public:
    std::unique_ptr<VoiceCounter> Create() override
    {
        return std::make_unique<VoiceCounterIlleprih>();
    }
};


} // namespace
