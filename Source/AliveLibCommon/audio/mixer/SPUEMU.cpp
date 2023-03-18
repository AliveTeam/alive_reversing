// Heavly modify version of duckstation spu
#pragma once

#include "SPUEMU.hpp"
#include <array>
#include "AudioStream.hpp"

// countof macro
#ifndef countof
    #ifdef _countof
        #define countof _countof
    #else
template <typename T, size_t N>
char (&__countof_ArraySizeHelper(T (&array)[N]))[N];
        #define countof(array) (sizeof(__countof_ArraySizeHelper(array)))
    #endif
#endif

u32 mask(u32 num)
{
    u32 res = 0;
    while (num-- > 0)
    {
        res = (res << 1) | 1;
    }
    return res;
}

static const u32 NUM_VOICES = 24;
static const u32 NUM_REVERB_REGS = 32;
static const u32 NUM_SAMPLES_PER_ADPCM_BLOCK = 28;
static const u32 NUM_VOICE_REGISTERS = 8;
static const u32 NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK = 3,

static const s16 ENVELOPE_MIN_VOLUME = 0;
static const s16 ENVELOPE_MAX_VOLUME = 0x7FFF;

union ADSRRegister
{
    u32 bits;
    struct
    {
        u16 bits_low;
        u16 bits_high;
    };

    //     adsr.attackRate = (adsr1 >> 8) & mask(7);
    BitField<u32, u8, 0, 4> sustain_level;
    BitField<u32, u8, 4, 4> decay_rate_shr2;
    BitField<u32, u8, 8, 7> attack_rate;
    BitField<u32, bool, 15, 1> attack_exponential;

    BitField<u32, u8, 16, 5> release_rate_shr2;
    BitField<u32, bool, 21, 1> release_exponential;
    BitField<u32, u8, 22, 7> sustain_rate;
    BitField<u32, bool, 30, 1> sustain_direction_decrease;
    BitField<u32, bool, 31, 1> sustain_exponential;
};

union VoiceCounter
{
    // promoted to u32 because of overflow
    u32 bits;

    BitField<u32, u8, 4, 8> interpolation_index;
    BitField<u32, u8, 12, 5> sample_index;
};

union VoiceRegisters
{
    u16 index[NUM_VOICE_REGISTERS];

    struct
    {
        VolumeRegister volume_left;
        VolumeRegister volume_right;

        u16 adpcm_sample_rate;   // VxPitch
        u16 adpcm_start_address; // multiply by 8

        ADSRRegister adsr;
        s16 adsr_volume;

        u16 adpcm_repeat_address; // multiply by 8
    };
};

struct ReverbRegisters
{
    s16 vLOUT;
    s16 vROUT;
    u16 mBASE;

    union
    {
        struct
        {
            u16 FB_SRC_A;
            u16 FB_SRC_B;
            s16 IIR_ALPHA;
            s16 ACC_COEF_A;
            s16 ACC_COEF_B;
            s16 ACC_COEF_C;
            s16 ACC_COEF_D;
            s16 IIR_COEF;
            s16 FB_ALPHA;
            s16 FB_X;
            u16 IIR_DEST_A[2];
            u16 ACC_SRC_A[2];
            u16 ACC_SRC_B[2];
            u16 IIR_SRC_A[2];
            u16 IIR_DEST_B[2];
            u16 ACC_SRC_C[2];
            u16 ACC_SRC_D[2];
            u16 IIR_SRC_B[2];
            u16 MIX_DEST_A[2];
            u16 MIX_DEST_B[2];
            s16 IN_COEF[2];
        };

        u16 rev[NUM_REVERB_REGS];
    };
};

struct VolumeEnvelope
{
    s32 counter;
    u8 rate;
    bool decreasing;
    bool exponential;

    void Reset(u8 rate_, bool decreasing_, bool exponential_);
    s16 Tick(s16 current_level);
};

union VolumeRegister
{
    u16 bits;

    BitField<u16, bool, 15, 1> sweep_mode;
    BitField<u16, s16, 0, 15> fixed_volume_shr1; // divided by 2

    BitField<u16, bool, 14, 1> sweep_exponential;
    BitField<u16, bool, 13, 1> sweep_direction_decrease;
    BitField<u16, bool, 12, 1> sweep_phase_negative;
    BitField<u16, u8, 0, 7> sweep_rate;
};

struct VolumeSweep
{
    VolumeEnvelope envelope;
    bool envelope_active;
    s16 current_level;

    void Reset(VolumeRegister reg);
    void Tick();
};

union SPUCNT
{
    u16 bits;

    BitField<u16, bool, 15, 1> enable;
    BitField<u16, bool, 14, 1> mute_n;
    BitField<u16, u8, 8, 6> noise_clock;
    BitField<u16, bool, 7, 1> reverb_master_enable;
    BitField<u16, bool, 6, 1> irq9_enable;
    BitField<u16, RAMTransferMode, 4, 2> ram_transfer_mode;
    BitField<u16, bool, 3, 1> external_audio_reverb;
    BitField<u16, bool, 2, 1> cd_audio_reverb;
    BitField<u16, bool, 1, 1> external_audio_enable;
    BitField<u16, bool, 0, 1> cd_audio_enable;

    BitField<u16, u8, 0, 6> mode;
};

union ADPCMFlags
{
    u8 bits;

    //BitField<u8, bool, 0, 1> loop_end;
    //BitField<u8, bool, 1, 1> loop_repeat;
    //BitField<u8, bool, 2, 1> loop_start;

    u8 loopEnd()
    {
        return (bits >> 0) & mask(1);
    }

    u8 loopRepeat()
    {
        return (bits >> 1) & mask(1);
    }

    u8 loopStart()
    {
        return (bits >> 2) & mask(1);
    }
};

struct ADPCMBlock
{
    union
    {
        u8 bits;

        //BitField<u8, u8, 0, 4> shift;
        //BitField<u8, u8, 4, 3> filter;

        u8 shift()
        {
            return (bits >> 0) & mask(4);
        }

        u8 filter()
        {
            return (bits >> 4) & mask(3);
        }

    } shift_filter;
    ADPCMFlags flags;
    u8 data[NUM_SAMPLES_PER_ADPCM_BLOCK / 2];

    // For both 4bit and 8bit ADPCM, reserved shift values 13..15 will act same as shift=9).
    u8 GetShift() const
    {
        const u8 shift = shift_filter.shift();
        return (shift > 12) ? 9 : shift;
    }

    u8 GetFilter() const
    {
        return std::min<u8>(shift_filter.filter(), 4);
    }

    u8 GetNibble(u32 index) const
    {
        return (data[index / 2] >> ((index % 2) * 4)) & 0x0F;
    }
};

template <typename TValue>
constexpr u16 Truncate16(TValue value)
{
    return static_cast<u16>(static_cast<typename std::make_unsigned<decltype(value)>::type>(value));
}

template <typename TValue>
bool ConvertToBoolUnchecked(TValue value)
{
    bool ret;
    std::memcpy(&ret, &value, sizeof(bool));
    return ret;
}

template <typename TReturn, typename TValue>
constexpr TReturn SignExtend(TValue value)
{
    return static_cast<TReturn>(
        static_cast<typename std::make_signed<TReturn>::type>(static_cast<typename std::make_signed<TValue>::type>(value)));
}

template <typename TValue>
constexpr u32 SignExtend32(TValue value)
{
    return SignExtend<u32, TValue>(value);
}

static constexpr s32 Clamp16(s32 value)
{
    return (value < -0x8000) ? -0x8000 : (value > 0x7FFF) ? 0x7FFF
                                                          : value;
}

template <typename TValue>
constexpr u32 ZeroExtend32(TValue value)
{
    return ZeroExtend<u32, TValue>(value);
}

template <typename TValue>
constexpr u16 ZeroExtend16(TValue value)
{
    return ZeroExtend<u16, TValue>(value);
}

static AudioStream* s_audio_stream = new AudioStream();
static std::array<Voice, NUM_VOICES> s_voices{};
static SPUCNT s_SPUCNT = {};

// bit mask of voices with reverb
static const u32 RAM_SIZE = 512 * 1024;
static std::array<u8, RAM_SIZE> s_ram{};
static u32 s_reverb_on_register = 0;
static u32 s_reverb_base_address = 0;
static u32 s_reverb_current_address = 0;
static ReverbRegisters s_reverb_registers{};
static std::array<std::array<s16, 128>, 2> s_reverb_downsample_buffer;
static std::array<std::array<s16, 64>, 2> s_reverb_upsample_buffer;
static s32 s_reverb_resample_buffer_position = 0;

static u32 s_key_on_register = 0;
static u32 s_key_off_register = 0;
static u32 s_endx_register = 0;
static u32 s_pitch_modulation_enable_register = 0;

static u32 s_noise_mode_register = 0;
static u32 s_noise_count = 0;
static u32 s_noise_level = 0;

static VolumeRegister s_main_volume_left_reg = {};
static VolumeRegister s_main_volume_right_reg = {};
static VolumeSweep s_main_volume_left = {};
static VolumeSweep s_main_volume_right = {};

struct Voice
{
    u16 current_address;
    VoiceRegisters regs;
    VoiceCounter counter;
    ADPCMFlags current_block_flags;
    bool is_first_block;
    std::array<s16, NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK + NUM_SAMPLES_PER_ADPCM_BLOCK> current_block_samples;
    std::array<s16, 2> adpcm_last_samples;
    s32 last_volume;

    VolumeSweep left_volume;
    VolumeSweep right_volume;

    VolumeEnvelope adsr_envelope;
    ADSRPhase adsr_phase;
    s16 adsr_target;
    bool has_samples;
    bool ignore_loop_address;

    bool IsOn() const
    {
        return adsr_phase != ADSRPhase::Off;
    }

    void KeyOn();
    void KeyOff();
    void ForceOff();

    void DecodeBlock(const ADPCMBlock& block);
    s32 Interpolate() const;

    // Switches to the specified phase, filling in target.
    void UpdateADSREnvelope();
    ADSRPhase GetNextADSRPhase(ADSRPhase phase);

    // Updates the ADSR volume/phase.
    void TickADSR();
};

void SPU::Execute(u32 remaining_frames)
{
    AudioStream* output_stream = s_audio_stream;


    // I NEED 
    // 1. sample interpolation
    // 2. reverb
    // 3. adsr


    while (remaining_frames > 0)
    {
        s16* output_frame_start;
        u32 output_frame_space = remaining_frames;
        output_stream->BeginWrite(&output_frame_start, &output_frame_space);

        s16* output_frame = output_frame_start;
        const u32 frames_in_this_batch = std::min(remaining_frames, output_frame_space);
        for (u32 i = 0; i < frames_in_this_batch; i++)
        {
            s32 left_sum = 0;
            s32 right_sum = 0;
            s32 reverb_in_left = 0;
            s32 reverb_in_right = 0;

            u32 reverb_on_register = s_reverb_on_register;
            for (u32 voice = 0; voice < NUM_VOICES; voice++)
            {
                const auto [left, right] = SampleVoice(voice);
                left_sum += left;
                right_sum += right;

                if (reverb_on_register & 1u)
                {
                    reverb_in_left += left;
                    reverb_in_right += right;
                }
                reverb_on_register >>= 1;
            }

            // What's this?
            //if (!s_SPUCNT.mute_n)
            //{
            //    left_sum = 0;
            //    right_sum = 0;
            //}

            // Update noise once per frame.
            //UpdateNoise();

            // Compute reverb.
            s32 reverb_out_left, reverb_out_right;
            ProcessReverb(static_cast<s16>(Clamp16(reverb_in_left)), static_cast<s16>(Clamp16(reverb_in_right)),
                          &reverb_out_left, &reverb_out_right);

            // Mix in reverb.
            left_sum += reverb_out_left;
            right_sum += reverb_out_right;

            // Apply main volume after clamping. A maximum volume should not overflow here because both are 16-bit values.
            *(output_frame++) = static_cast<s16>(ApplyVolume(Clamp16(left_sum), s_main_volume_left.current_level));
            *(output_frame++) = static_cast<s16>(ApplyVolume(Clamp16(right_sum), s_main_volume_right.current_level));
            s_main_volume_left.Tick();
            s_main_volume_right.Tick();

            // Write to capture buffers.
            //WriteToCaptureBuffer(0, cd_audio_left);
            //WriteToCaptureBuffer(1, cd_audio_right);
            // 
            // UNSURE IF THESE ARE NEEDED?
            //WriteToCaptureBuffer(2, static_cast<s16>(Clamp16(s_voices[1].last_volume)));
            //WriteToCaptureBuffer(3, static_cast<s16>(Clamp16(s_voices[3].last_volume)));
            //IncrementCaptureBufferPosition();

            // Key off/on voices after the first frame.
            if (i == 0 && (s_key_off_register != 0 || s_key_on_register != 0))
            {
                u32 key_off_register = s_key_off_register;
                s_key_off_register = 0;

                u32 key_on_register = s_key_on_register;
                s_key_on_register = 0;

                for (u32 voice = 0; voice < NUM_VOICES; voice++)
                {
                    if (key_off_register & 1u)
                        s_voices[voice].KeyOff();
                    key_off_register >>= 1;

                    if (key_on_register & 1u)
                    {
                        s_endx_register &= ~(1u << voice);
                        s_voices[voice].KeyOn();
                    }
                    key_on_register >>= 1;
                }
            }
        }

        output_stream->EndWrite(frames_in_this_batch);
        remaining_frames -= frames_in_this_batch;
    }
}




bool SPU::IsVoiceNoiseEnabled(u32 i)
{
    // TODO - another mask?
    return ConvertToBoolUnchecked((s_noise_mode_register >> i) & u32(1));
}

s16 SPU::GetVoiceNoiseLevel()
{
    return static_cast<s16>(static_cast<u16>(s_noise_level));
}

bool SPU::IsPitchModulationEnabled(u32 i)
{
    return ((i > 0) && ConvertToBoolUnchecked((s_pitch_modulation_enable_register >> i) & u32(1)));
}

static constexpr s32 ApplyVolume(s32 sample, s16 volume)
{
    return (sample * s32(volume)) >> 15;
}

std::tuple<s32, s32> SPU::SampleVoice(u32 voice_index)
{
    Voice& voice = s_voices[voice_index];


    // What's this?
    if (!voice.IsOn() && !s_SPUCNT.irq9_enable)
    {
        voice.last_volume = 0;
        return {};
    }

    if (!voice.IsOn())
    {
        voice.last_volume = 0;
    }

    // No need to read from block, we will keep a buffer - TODO
    if (!voice.has_samples)
    {
        ADPCMBlock block;
        ReadADPCMBlock(voice.current_address, &block);
        voice.DecodeBlock(block);
        voice.has_samples = true;
    
        if (voice.current_block_flags.loopStart() && !voice.ignore_loop_address)
        {
            // Log_TracePrintf("Voice %u loop start @ 0x%08X", voice_index, ZeroExtend32(voice.current_address));
            voice.regs.adpcm_repeat_address = voice.current_address;
        }
    }

    // skip interpolation when the volume is muted anyway
    s32 volume;
    if (voice.regs.adsr_volume != 0)
    {
        // interpolate/sample and apply ADSR volume
        s32 sample;
        if (IsVoiceNoiseEnabled(voice_index))
            sample = GetVoiceNoiseLevel();
        else
            sample = voice.Interpolate();

        volume = ApplyVolume(sample, voice.regs.adsr_volume);
    }
    else
    {
        volume = 0;
    }

    voice.last_volume = volume;

    if (voice.adsr_phase != ADSRPhase::Off)
        voice.TickADSR();

    // Pitch modulation
    u16 step = voice.regs.adpcm_sample_rate;
    if (IsPitchModulationEnabled(voice_index))
    {
        const s32 factor = std::clamp<s32>(s_voices[voice_index - 1].last_volume, -0x8000, 0x7FFF) + 0x8000;
        step = Truncate16(static_cast<u32>((SignExtend32(step) * factor) >> 15));
    }
    step = std::min<u16>(step, 0x3FFF);

    // Shouldn't ever overflow because if sample_index == 27, step == 0x4000 there won't be a carry out from the
    // interpolation index. If there is a carry out, bit 12 will never be 1, so it'll never add more than 4 to
    // sample_index, which should never be >27.
    //DebugAssert(voice.counter.sample_index < NUM_SAMPLES_PER_ADPCM_BLOCK);
    voice.counter.bits += step;

    if (voice.counter.sample_index >= NUM_SAMPLES_PER_ADPCM_BLOCK)
    {
        // next block
        voice.counter.sample_index -= NUM_SAMPLES_PER_ADPCM_BLOCK;
        voice.has_samples = false;
        voice.is_first_block = false;
        voice.current_address += 2;

        // handle flags
        if (voice.current_block_flags.loopEnd())
        {
            s_endx_register |= (u32(1) << voice_index);
            voice.current_address = voice.regs.adpcm_repeat_address & ~u16(1);

            if (!voice.current_block_flags.loopRepeat())
            {
                //Log_TracePrintf("Voice %u loop end+mute @ 0x%08X", voice_index, ZeroExtend32(voice.current_address));
                voice.ForceOff();
            }
            else
            {
                //Log_TracePrintf("Voice %u loop end+repeat @ 0x%08X", voice_index, ZeroExtend32(voice.current_address));
            }
        }
    }

    // apply per-channel volume
    const s32 left = ApplyVolume(volume, voice.left_volume.current_level);
    const s32 right = ApplyVolume(volume, voice.right_volume.current_level);
    voice.left_volume.Tick();
    voice.right_volume.Tick();

    //#ifdef SPU_DUMP_ALL_VOICES
    //    if (s_voice_dump_writers[voice_index])
    //    {
    //        const s16 dump_samples[2] = {static_cast<s16>(Clamp16(left)), static_cast<s16>(Clamp16(right))};
    //        s_voice_dump_writers[voice_index]->WriteFrames(dump_samples, 1);
    //    }
    //#endif

    return std::make_tuple(left, right);
}

//void SPU::UpdateNoise()
//{
//    // Dr Hell's noise waveform, implementation borrowed from pcsx-r.
//    static constexpr std::array<u8, 64> noise_wave_add = {
//        {1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0,
//         0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1}};
//    static constexpr std::array<u8, 5> noise_freq_add = {{0, 84, 140, 180, 210}};
//
//    const u32 noise_clock = s_SPUCNT.noise_clock;
//    const u32 level = (0x8000u >> (noise_clock >> 2)) << 16;
//
//    s_noise_count += 0x10000u + noise_freq_add[noise_clock & 3u];
//    if ((s_noise_count & 0xFFFFu) >= noise_freq_add[4])
//    {
//        s_noise_count += 0x10000;
//        s_noise_count -= noise_freq_add[noise_clock & 3u];
//    }
//
//    if (s_noise_count < level)
//        return;
//
//    s_noise_count %= level;
//    s_noise_level = (s_noise_level << 1) | noise_wave_add[(s_noise_level >> 10) & 63u];
//}


//void SPU::ReadADPCMBlock(u16 address, ADPCMBlock* block)
//{
//    u32 ram_address = (ZeroExtend32(address) * 8) & RAM_MASK;
//    if (IsRAMIRQTriggerable() && (CheckRAMIRQ(ram_address) || CheckRAMIRQ((ram_address + 8) & RAM_MASK)))
//    {
//        Log_DebugPrintf("Trigger IRQ @ %08X %04X from ADPCM reader", ram_address, ram_address / 8);
//        TriggerRAMIRQ();
//    }
//
//    // fast path - no wrap-around
//    if ((ram_address + sizeof(ADPCMBlock)) <= RAM_SIZE)
//    {
//        std::memcpy(block, &s_ram[ram_address], sizeof(ADPCMBlock));
//        return;
//    }
//
//    block->shift_filter.bits = s_ram[ram_address];
//    ram_address = (ram_address + 1) & RAM_MASK;
//    block->flags.bits = s_ram[ram_address];
//    ram_address = (ram_address + 1) & RAM_MASK;
//    for (u32 i = 0; i < 14; i++)
//    {
//        block->data[i] = s_ram[ram_address];
//        ram_address = (ram_address + 1) & RAM_MASK;
//    }
//}

////////////////////////////
// VOICE
////////////////////////////

void Voice::DecodeBlock(const ADPCMBlock& block)
{
    static constexpr std::array<s32, 5> filter_table_pos = {{0, 60, 115, 98, 122}};
    static constexpr std::array<s32, 5> filter_table_neg = {{0, 0, -52, -55, -60}};

    // store samples needed for interpolation
    current_block_samples[2] = current_block_samples[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK + NUM_SAMPLES_PER_ADPCM_BLOCK - 1];
    current_block_samples[1] = current_block_samples[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK + NUM_SAMPLES_PER_ADPCM_BLOCK - 2];
    current_block_samples[0] = current_block_samples[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK + NUM_SAMPLES_PER_ADPCM_BLOCK - 3];

    // pre-lookup
    const u8 shift = block.GetShift();
    const u8 filter_index = block.GetFilter();
    const s32 filter_pos = filter_table_pos[filter_index];
    const s32 filter_neg = filter_table_neg[filter_index];
    s16 last_samples[2] = {adpcm_last_samples[0], adpcm_last_samples[1]};

    // samples
    for (u32 i = 0; i < NUM_SAMPLES_PER_ADPCM_BLOCK; i++)
    {
        // extend 4-bit to 16-bit, apply shift from header and mix in previous samples
        s32 sample = s32(static_cast<s16>(ZeroExtend16(block.GetNibble(i)) << 12) >> shift);
        sample += (last_samples[0] * filter_pos) >> 6;
        sample += (last_samples[1] * filter_neg) >> 6;

        last_samples[1] = last_samples[0];
        current_block_samples[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK + i] = last_samples[0] = static_cast<s16>(Clamp16(sample));
    }

    std::copy(last_samples, last_samples + countof(last_samples), adpcm_last_samples.begin());
    current_block_flags.bits = block.flags.bits;
}

void Voice::TickADSR()
{
    regs.adsr_volume = adsr_envelope.Tick(regs.adsr_volume);

    if (adsr_phase != ADSRPhase::Sustain)
    {
        const bool reached_target = adsr_envelope.decreasing ? (regs.adsr_volume <= adsr_target) : (regs.adsr_volume >= adsr_target);
        if (reached_target)
        {
            adsr_phase = GetNextADSRPhase(adsr_phase);
            UpdateADSREnvelope();
        }
    }
}

ADSRPhase Voice::GetNextADSRPhase(ADSRPhase phase)
{
    switch (phase)
    {
        case ADSRPhase::Attack:
            // attack -> decay
            return ADSRPhase::Decay;

        case ADSRPhase::Decay:
            // decay -> sustain
            return ADSRPhase::Sustain;

        case ADSRPhase::Sustain:
            // sustain stays in sustain until key off
            return ADSRPhase::Sustain;

        default:
        case ADSRPhase::Release:
            // end of release disables the voice
            return ADSRPhase::Off;
    }
}

void Voice::UpdateADSREnvelope()
{
    switch (adsr_phase)
    {
        case ADSRPhase::Off:
            adsr_target = 0;
            adsr_envelope.Reset(0, false, false);
            return;

        case ADSRPhase::Attack:
            adsr_target = 32767; // 0 -> max
            adsr_envelope.Reset(regs.adsr.attack_rate, false, regs.adsr.attack_exponential);
            break;

        case ADSRPhase::Decay:
            adsr_target = static_cast<s16>(std::min<s32>((u32(regs.adsr.sustain_level.GetValue()) + 1) * 0x800,
                                                         ENVELOPE_MAX_VOLUME)); // max -> sustain level
            adsr_envelope.Reset(regs.adsr.decay_rate_shr2 << 2, true, true);
            break;

        case ADSRPhase::Sustain:
            adsr_target = 0;
            adsr_envelope.Reset(regs.adsr.sustain_rate, regs.adsr.sustain_direction_decrease, regs.adsr.sustain_exponential);
            break;

        case ADSRPhase::Release:
            adsr_target = 0;
            adsr_envelope.Reset(regs.adsr.release_rate_shr2 << 2, true, regs.adsr.release_exponential);
            break;

        default:
            break;
    }
}

s32 Voice::Interpolate() const
{
    static constexpr std::array<s16, 0x200> gauss = {{
        -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, //
        -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, //
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, //
        0x0001, 0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0003, 0x0003, //
        0x0003, 0x0004, 0x0004, 0x0005, 0x0005, 0x0006, 0x0007, 0x0007, //
        0x0008, 0x0009, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, //
        0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0015, 0x0016, 0x0018, // entry
        0x0019, 0x001B, 0x001C, 0x001E, 0x0020, 0x0021, 0x0023, 0x0025, // 000..07F
        0x0027, 0x0029, 0x002C, 0x002E, 0x0030, 0x0033, 0x0035, 0x0038, //
        0x003A, 0x003D, 0x0040, 0x0043, 0x0046, 0x0049, 0x004D, 0x0050, //
        0x0054, 0x0057, 0x005B, 0x005F, 0x0063, 0x0067, 0x006B, 0x006F, //
        0x0074, 0x0078, 0x007D, 0x0082, 0x0087, 0x008C, 0x0091, 0x0096, //
        0x009C, 0x00A1, 0x00A7, 0x00AD, 0x00B3, 0x00BA, 0x00C0, 0x00C7, //
        0x00CD, 0x00D4, 0x00DB, 0x00E3, 0x00EA, 0x00F2, 0x00FA, 0x0101, //
        0x010A, 0x0112, 0x011B, 0x0123, 0x012C, 0x0135, 0x013F, 0x0148, //
        0x0152, 0x015C, 0x0166, 0x0171, 0x017B, 0x0186, 0x0191, 0x019C, //
        0x01A8, 0x01B4, 0x01C0, 0x01CC, 0x01D9, 0x01E5, 0x01F2, 0x0200, //
        0x020D, 0x021B, 0x0229, 0x0237, 0x0246, 0x0255, 0x0264, 0x0273, //
        0x0283, 0x0293, 0x02A3, 0x02B4, 0x02C4, 0x02D6, 0x02E7, 0x02F9, //
        0x030B, 0x031D, 0x0330, 0x0343, 0x0356, 0x036A, 0x037E, 0x0392, //
        0x03A7, 0x03BC, 0x03D1, 0x03E7, 0x03FC, 0x0413, 0x042A, 0x0441, //
        0x0458, 0x0470, 0x0488, 0x04A0, 0x04B9, 0x04D2, 0x04EC, 0x0506, //
        0x0520, 0x053B, 0x0556, 0x0572, 0x058E, 0x05AA, 0x05C7, 0x05E4, // entry
        0x0601, 0x061F, 0x063E, 0x065C, 0x067C, 0x069B, 0x06BB, 0x06DC, // 080..0FF
        0x06FD, 0x071E, 0x0740, 0x0762, 0x0784, 0x07A7, 0x07CB, 0x07EF, //
        0x0813, 0x0838, 0x085D, 0x0883, 0x08A9, 0x08D0, 0x08F7, 0x091E, //
        0x0946, 0x096F, 0x0998, 0x09C1, 0x09EB, 0x0A16, 0x0A40, 0x0A6C, //
        0x0A98, 0x0AC4, 0x0AF1, 0x0B1E, 0x0B4C, 0x0B7A, 0x0BA9, 0x0BD8, //
        0x0C07, 0x0C38, 0x0C68, 0x0C99, 0x0CCB, 0x0CFD, 0x0D30, 0x0D63, //
        0x0D97, 0x0DCB, 0x0E00, 0x0E35, 0x0E6B, 0x0EA1, 0x0ED7, 0x0F0F, //
        0x0F46, 0x0F7F, 0x0FB7, 0x0FF1, 0x102A, 0x1065, 0x109F, 0x10DB, //
        0x1116, 0x1153, 0x118F, 0x11CD, 0x120B, 0x1249, 0x1288, 0x12C7, //
        0x1307, 0x1347, 0x1388, 0x13C9, 0x140B, 0x144D, 0x1490, 0x14D4, //
        0x1517, 0x155C, 0x15A0, 0x15E6, 0x162C, 0x1672, 0x16B9, 0x1700, //
        0x1747, 0x1790, 0x17D8, 0x1821, 0x186B, 0x18B5, 0x1900, 0x194B, //
        0x1996, 0x19E2, 0x1A2E, 0x1A7B, 0x1AC8, 0x1B16, 0x1B64, 0x1BB3, //
        0x1C02, 0x1C51, 0x1CA1, 0x1CF1, 0x1D42, 0x1D93, 0x1DE5, 0x1E37, //
        0x1E89, 0x1EDC, 0x1F2F, 0x1F82, 0x1FD6, 0x202A, 0x207F, 0x20D4, //
        0x2129, 0x217F, 0x21D5, 0x222C, 0x2282, 0x22DA, 0x2331, 0x2389, // entry
        0x23E1, 0x2439, 0x2492, 0x24EB, 0x2545, 0x259E, 0x25F8, 0x2653, // 100..17F
        0x26AD, 0x2708, 0x2763, 0x27BE, 0x281A, 0x2876, 0x28D2, 0x292E, //
        0x298B, 0x29E7, 0x2A44, 0x2AA1, 0x2AFF, 0x2B5C, 0x2BBA, 0x2C18, //
        0x2C76, 0x2CD4, 0x2D33, 0x2D91, 0x2DF0, 0x2E4F, 0x2EAE, 0x2F0D, //
        0x2F6C, 0x2FCC, 0x302B, 0x308B, 0x30EA, 0x314A, 0x31AA, 0x3209, //
        0x3269, 0x32C9, 0x3329, 0x3389, 0x33E9, 0x3449, 0x34A9, 0x3509, //
        0x3569, 0x35C9, 0x3629, 0x3689, 0x36E8, 0x3748, 0x37A8, 0x3807, //
        0x3867, 0x38C6, 0x3926, 0x3985, 0x39E4, 0x3A43, 0x3AA2, 0x3B00, //
        0x3B5F, 0x3BBD, 0x3C1B, 0x3C79, 0x3CD7, 0x3D35, 0x3D92, 0x3DEF, //
        0x3E4C, 0x3EA9, 0x3F05, 0x3F62, 0x3FBD, 0x4019, 0x4074, 0x40D0, //
        0x412A, 0x4185, 0x41DF, 0x4239, 0x4292, 0x42EB, 0x4344, 0x439C, //
        0x43F4, 0x444C, 0x44A3, 0x44FA, 0x4550, 0x45A6, 0x45FC, 0x4651, //
        0x46A6, 0x46FA, 0x474E, 0x47A1, 0x47F4, 0x4846, 0x4898, 0x48E9, //
        0x493A, 0x498A, 0x49D9, 0x4A29, 0x4A77, 0x4AC5, 0x4B13, 0x4B5F, //
        0x4BAC, 0x4BF7, 0x4C42, 0x4C8D, 0x4CD7, 0x4D20, 0x4D68, 0x4DB0, //
        0x4DF7, 0x4E3E, 0x4E84, 0x4EC9, 0x4F0E, 0x4F52, 0x4F95, 0x4FD7, // entry
        0x5019, 0x505A, 0x509A, 0x50DA, 0x5118, 0x5156, 0x5194, 0x51D0, // 180..1FF
        0x520C, 0x5247, 0x5281, 0x52BA, 0x52F3, 0x532A, 0x5361, 0x5397, //
        0x53CC, 0x5401, 0x5434, 0x5467, 0x5499, 0x54CA, 0x54FA, 0x5529, //
        0x5558, 0x5585, 0x55B2, 0x55DE, 0x5609, 0x5632, 0x565B, 0x5684, //
        0x56AB, 0x56D1, 0x56F6, 0x571B, 0x573E, 0x5761, 0x5782, 0x57A3, //
        0x57C3, 0x57E2, 0x57FF, 0x581C, 0x5838, 0x5853, 0x586D, 0x5886, //
        0x589E, 0x58B5, 0x58CB, 0x58E0, 0x58F4, 0x5907, 0x5919, 0x592A, //
        0x593A, 0x5949, 0x5958, 0x5965, 0x5971, 0x597C, 0x5986, 0x598F, //
        0x5997, 0x599E, 0x59A4, 0x59A9, 0x59AD, 0x59B0, 0x59B2, 0x59B3  //
    }};

    // the interpolation index is based on the source files sample rate
    const u8 i = counter.interpolation_index;
    const u32 s = NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK + ZeroExtend32(counter.sample_index.GetValue());

    // interpolate on 4 most recent samples
    s32 out = s32(gauss[0x0FF - i]) * s32(current_block_samples[s - 3]); // oldest
    out += s32(gauss[0x1FF - i]) * s32(current_block_samples[s - 2]);    // older
    out += s32(gauss[0x100 + i]) * s32(current_block_samples[s - 1]);    // old
    out += s32(gauss[0x000 + i]) * s32(current_block_samples[s - 0]);    // new
    return out >> 15;
}

////////////////////////////
// REVERB
////////////////////////////
// Zeroes optimized out; middle removed too(it's 16384)
static constexpr std::array<s16, 20> s_reverb_resample_coefficients = {
    -1,
    2,
    -10,
    35,
    -103,
    266,
    -616,
    1332,
    -2960,
    10246,
    10246,
    -2960,
    1332,
    -616,
    266,
    -103,
    35,
    -10,
    2,
    -1,
};
static s16 s_last_reverb_input[2];
static s32 s_last_reverb_output[2];

static s32 Reverb4422(const s16* src)
{
    s32 out = 0; // 32-bits is adequate(it won't overflow)
    for (u32 i = 0; i < 20; i++)
        out += s_reverb_resample_coefficients[i] * src[i * 2];

    // Middle non-zero
    out += 0x4000 * src[19];
    out >>= 15;
    return std::clamp<s32>(out, -32768, 32767);
}

template <bool phase>
static s32 Reverb2244(const s16* src)
{
    s32 out; // 32-bits is adequate(it won't overflow)
    if (phase)
    {
        // Middle non-zero
        out = src[9];
    }
    else
    {
        out = 0;
        for (u32 i = 0; i < 20; i++)
            out += s_reverb_resample_coefficients[i] * src[i];

        out >>= 14;
        out = std::clamp<s32>(out, -32768, 32767);
    }

    return out;
}

static s16 ReverbSat(s32 val)
{
    return static_cast<s16>(std::clamp<s32>(val, -0x8000, 0x7FFF));
}

static s16 ReverbNeg(s16 samp)
{
    if (samp == -32768)
        return 0x7FFF;

    return -samp;
}

static s32 IIASM(const s16 IIR_ALPHA, const s16 insamp)
{
    if (IIR_ALPHA == -32768)
    {
        if (insamp == -32768)
            return 0;
        else
            return insamp * -65536;
    }
    else
        return insamp * (32768 - IIR_ALPHA);
}

u32 SPU::ReverbMemoryAddress(u32 address)
{
    // Ensures address does not leave the reverb work area.
    static constexpr u32 MASK = (RAM_SIZE - 1) / 2;
    u32 offset = s_reverb_current_address + (address & MASK);
    offset += s_reverb_base_address & ((s32) (offset << 13) >> 31);

    // We address RAM in bytes. TODO: Change this to words.
    return (offset & MASK) * 2u;
}

s16 SPU::ReverbRead(u32 address, s32 offset)
{
    // TODO: This should check interrupts.
    const u32 real_address = ReverbMemoryAddress((address << 2) + offset);

    s16 data;
    std::memcpy(&data, &s_ram[real_address], sizeof(data));
    return data;
}

void SPU::ReverbWrite(u32 address, s16 data)
{
    // TODO: This should check interrupts.
    const u32 real_address = ReverbMemoryAddress(address << 2);
    std::memcpy(&s_ram[real_address], &data, sizeof(data));
}

void SPU::ProcessReverb(s16 left_in, s16 right_in, s32* left_out, s32* right_out)
{
    s_last_reverb_input[0] = left_in;
    s_last_reverb_input[1] = right_in;
    s_reverb_downsample_buffer[0][s_reverb_resample_buffer_position | 0x00] = left_in;
    s_reverb_downsample_buffer[0][s_reverb_resample_buffer_position | 0x40] = left_in;
    s_reverb_downsample_buffer[1][s_reverb_resample_buffer_position | 0x00] = right_in;
    s_reverb_downsample_buffer[1][s_reverb_resample_buffer_position | 0x40] = right_in;

    s32 out[2];
    if (s_reverb_resample_buffer_position & 1u)
    {
        std::array<s32, 2> downsampled;
        for (unsigned lr = 0; lr < 2; lr++)
            downsampled[lr] = Reverb4422(&s_reverb_downsample_buffer[lr][(s_reverb_resample_buffer_position - 38) & 0x3F]);

        for (unsigned lr = 0; lr < 2; lr++)
        {
            if (s_SPUCNT.reverb_master_enable)
            {
                const s16 IIR_INPUT_A = ReverbSat((((ReverbRead(s_reverb_registers.IIR_SRC_A[lr ^ 0]) * s_reverb_registers.IIR_COEF) >> 14) + ((downsampled[lr] * s_reverb_registers.IN_COEF[lr]) >> 14)) >> 1);
                const s16 IIR_INPUT_B = ReverbSat((((ReverbRead(s_reverb_registers.IIR_SRC_B[lr ^ 1]) * s_reverb_registers.IIR_COEF) >> 14) + ((downsampled[lr] * s_reverb_registers.IN_COEF[lr]) >> 14)) >> 1);
                const s16 IIR_A = ReverbSat((((IIR_INPUT_A * s_reverb_registers.IIR_ALPHA) >> 14) + (IIASM(s_reverb_registers.IIR_ALPHA, ReverbRead(s_reverb_registers.IIR_DEST_A[lr], -1)) >> 14)) >> 1);
                const s16 IIR_B = ReverbSat((((IIR_INPUT_B * s_reverb_registers.IIR_ALPHA) >> 14) + (IIASM(s_reverb_registers.IIR_ALPHA, ReverbRead(s_reverb_registers.IIR_DEST_B[lr], -1)) >> 14)) >> 1);

                ReverbWrite(s_reverb_registers.IIR_DEST_A[lr], IIR_A);
                ReverbWrite(s_reverb_registers.IIR_DEST_B[lr], IIR_B);
            }

            const s32 ACC = ((ReverbRead(s_reverb_registers.ACC_SRC_A[lr]) * s_reverb_registers.ACC_COEF_A) >> 14) + ((ReverbRead(s_reverb_registers.ACC_SRC_B[lr]) * s_reverb_registers.ACC_COEF_B) >> 14) + ((ReverbRead(s_reverb_registers.ACC_SRC_C[lr]) * s_reverb_registers.ACC_COEF_C) >> 14) + ((ReverbRead(s_reverb_registers.ACC_SRC_D[lr]) * s_reverb_registers.ACC_COEF_D) >> 14);

            const s16 FB_A = ReverbRead(s_reverb_registers.MIX_DEST_A[lr] - s_reverb_registers.FB_SRC_A);
            const s16 FB_B = ReverbRead(s_reverb_registers.MIX_DEST_B[lr] - s_reverb_registers.FB_SRC_B);
            const s16 MDA = ReverbSat((ACC + ((FB_A * ReverbNeg(s_reverb_registers.FB_ALPHA)) >> 14)) >> 1);
            const s16 MDB = ReverbSat(
                FB_A + ((((MDA * s_reverb_registers.FB_ALPHA) >> 14) + ((FB_B * ReverbNeg(s_reverb_registers.FB_X)) >> 14)) >> 1));
            const s16 IVB = ReverbSat(FB_B + ((MDB * s_reverb_registers.FB_X) >> 15));

            if (s_SPUCNT.reverb_master_enable)
            {
                ReverbWrite(s_reverb_registers.MIX_DEST_A[lr], MDA);
                ReverbWrite(s_reverb_registers.MIX_DEST_B[lr], MDB);
            }

            s_reverb_upsample_buffer[lr][(s_reverb_resample_buffer_position >> 1) | 0x20] = s_reverb_upsample_buffer[lr][s_reverb_resample_buffer_position >> 1] = IVB;
        }

        s_reverb_current_address = (s_reverb_current_address + 1) & 0x3FFFFu;
        if (s_reverb_current_address == 0)
            s_reverb_current_address = s_reverb_base_address;

        for (unsigned lr = 0; lr < 2; lr++)
            out[lr] = Reverb2244<false>(&s_reverb_upsample_buffer[lr][((s_reverb_resample_buffer_position >> 1) - 19) & 0x1F]);
    }
    else
    {
        for (unsigned lr = 0; lr < 2; lr++)
            out[lr] = Reverb2244<true>(&s_reverb_upsample_buffer[lr][((s_reverb_resample_buffer_position >> 1) - 19) & 0x1F]);
    }

    s_reverb_resample_buffer_position = (s_reverb_resample_buffer_position + 1) & 0x3F;

    s_last_reverb_output[0] = *left_out = ApplyVolume(out[0], s_reverb_registers.vLOUT);
    s_last_reverb_output[1] = *right_out = ApplyVolume(out[1], s_reverb_registers.vROUT);

#ifdef SPU_DUMP_ALL_VOICES
    if (s_voice_dump_writers[NUM_VOICES])
    {
        const s16 dump_samples[2] = {static_cast<s16>(Clamp16(s_last_reverb_output[0])),
                                     static_cast<s16>(Clamp16(s_last_reverb_output[1]))};
        s_voice_dump_writers[NUM_VOICES]->WriteFrames(dump_samples, 1);
    }
#endif
}



//void SPU::WriteToCaptureBuffer(u32 index, s16 value)
//{
//    const u32 ram_address = (index * CAPTURE_BUFFER_SIZE_PER_CHANNEL) | ZeroExtend16(s_capture_buffer_position);
//    // Log_DebugPrintf("write to capture buffer %u (0x%08X) <- 0x%04X", index, ram_address, u16(value));
//    std::memcpy(&s_ram[ram_address], &value, sizeof(value));
//    if (IsRAMIRQTriggerable() && CheckRAMIRQ(ram_address))
//    {
//        // Log_DebugPrintf("Trigger IRQ @ %08X %04X from capture buffer", ram_address, ram_address / 8);
//        TriggerRAMIRQ();
//    }
//}
//
//void SPU::IncrementCaptureBufferPosition()
//{
//    s_capture_buffer_position += sizeof(s16);
//    s_capture_buffer_position %= CAPTURE_BUFFER_SIZE_PER_CHANNEL;
//    s_SPUSTAT.second_half_capture_buffer = s_capture_buffer_position >= (CAPTURE_BUFFER_SIZE_PER_CHANNEL / 2);
//}