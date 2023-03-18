// SPDX-FileCopyrightText: 2019-2022 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#include "spu.hpp"
#include "cdrom.h"
#include "common/bitfield.h"
#include "common/fifo_queue.h"
#include "common/file_system.h"
#include "common/log.h"
#include "dma.h"
#include "host.h"
#include "imgui.h"
#include "interrupt_controller.h"
#include "system.h"
#include "util/audio_stream.h"
#include "util/state_wrapper.h"
#include "util/wav_writer.h"
#include <memory>
Log_SetChannel(SPU);

// Enable to dump all voices of the SPU audio individually.
// #define SPU_DUMP_ALL_VOICES 1

ALWAYS_INLINE static constexpr s32 Clamp16(s32 value)
{
    return (value < -0x8000) ? -0x8000 : (value > 0x7FFF) ? 0x7FFF
                                                          : value;
}

ALWAYS_INLINE static constexpr s32 ApplyVolume(s32 sample, s16 volume)
{
    return (sample * s32(volume)) >> 15;
}

namespace SPU {
enum : u32
{
    SPU_BASE = 0x1F801C00,
    NUM_CHANNELS = 2,
    NUM_VOICES = 24,
    NUM_VOICE_REGISTERS = 8,
    VOICE_ADDRESS_SHIFT = 3,
    NUM_SAMPLES_PER_ADPCM_BLOCK = 28,
    NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK = 3,
    SYSCLK_TICKS_PER_SPU_TICK = System::MASTER_CLOCK / SAMPLE_RATE, // 0x300
    CAPTURE_BUFFER_SIZE_PER_CHANNEL = 0x400,
    MINIMUM_TICKS_BETWEEN_KEY_ON_OFF = 2,
    NUM_REVERB_REGS = 32,
    FIFO_SIZE_IN_HALFWORDS = 32
};
enum : s16
{
    ENVELOPE_MIN_VOLUME = 0,
    ENVELOPE_MAX_VOLUME = 0x7FFF
};
enum : TickCount
{
    TRANSFER_TICKS_PER_HALFWORD = 16
};

enum class RAMTransferMode : u8
{
    Stopped = 0,
    ManualWrite = 1,
    DMAWrite = 2,
    DMARead = 3
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

union SPUSTAT
{
    u16 bits;

    BitField<u16, bool, 11, 1> second_half_capture_buffer;
    BitField<u16, bool, 10, 1> transfer_busy;
    BitField<u16, bool, 9, 1> dma_write_request;
    BitField<u16, bool, 8, 1> dma_read_request;
    BitField<u16, bool, 7, 1> dma_request;
    BitField<u16, bool, 6, 1> irq9_flag;
    BitField<u16, u8, 0, 6> mode;
};

union TransferControl
{
    u16 bits;

    BitField<u8, u8, 1, 3> mode;
};

union ADSRRegister
{
    u32 bits;
    struct
    {
        u16 bits_low;
        u16 bits_high;
    };

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

// organized so we can replace this with a u16 array in the future
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

union VoiceCounter
{
    // promoted to u32 because of overflow
    u32 bits;

    BitField<u32, u8, 4, 8> interpolation_index;
    BitField<u32, u8, 12, 5> sample_index;
};

union ADPCMFlags
{
    u8 bits;

    BitField<u8, bool, 0, 1> loop_end;
    BitField<u8, bool, 1, 1> loop_repeat;
    BitField<u8, bool, 2, 1> loop_start;
};

struct ADPCMBlock
{
    union
    {
        u8 bits;

        BitField<u8, u8, 0, 4> shift;
        BitField<u8, u8, 4, 3> filter;
    } shift_filter;
    ADPCMFlags flags;
    u8 data[NUM_SAMPLES_PER_ADPCM_BLOCK / 2];

    // For both 4bit and 8bit ADPCM, reserved shift values 13..15 will act same as shift=9).
    u8 GetShift() const
    {
        const u8 shift = shift_filter.shift;
        return (shift > 12) ? 9 : shift;
    }

    u8 GetFilter() const
    {
        return std::min<u8>(shift_filter.filter, 4);
    }

    u8 GetNibble(u32 index) const
    {
        return (data[index / 2] >> ((index % 2) * 4)) & 0x0F;
    }
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

struct VolumeSweep
{
    VolumeEnvelope envelope;
    bool envelope_active;
    s16 current_level;

    void Reset(VolumeRegister reg);
    void Tick();
};

enum class ADSRPhase : u8
{
    Off = 0,
    Attack = 1,
    Decay = 2,
    Sustain = 3,
    Release = 4
};

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

    // Updates the ADSR volume/phase.
    void TickADSR();
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

static ADSRPhase GetNextADSRPhase(ADSRPhase phase);

bool IsVoiceReverbEnabled(u32 i);
bool IsVoiceNoiseEnabled(u32 i);
bool IsPitchModulationEnabled(u32 i);
s16 GetVoiceNoiseLevel();

u16 ReadVoiceRegister(u32 offset);
void WriteVoiceRegister(u32 offset, u16 value);

static bool IsRAMIRQTriggerable();
static bool CheckRAMIRQ(u32 address);
static void TriggerRAMIRQ();
static void CheckForLateRAMIRQs();

static void WriteToCaptureBuffer(u32 index, s16 value);
static void IncrementCaptureBufferPosition();

static void ReadADPCMBlock(u16 address, ADPCMBlock* block);
static std::tuple<s32, s32> SampleVoice(u32 voice_index);

static void UpdateNoise();

static u32 ReverbMemoryAddress(u32 address);
static s16 ReverbRead(u32 address, s32 offset = 0);
static void ReverbWrite(u32 address, s16 data);
static void ProcessReverb(s16 left_in, s16 right_in, s32* left_out, s32* right_out);

static void Execute(void* param, TickCount ticks, TickCount ticks_late);
static void UpdateEventInterval();

static void ExecuteFIFOWriteToRAM(TickCount& ticks);
static void ExecuteFIFOReadFromRAM(TickCount& ticks);
static void ExecuteTransfer(void* param, TickCount ticks, TickCount ticks_late);
static void ManualTransferWrite(u16 value);
static void UpdateTransferEvent();
static void UpdateDMARequest();

static void CreateOutputStream();

static std::unique_ptr<TimingEvent> s_tick_event;
static std::unique_ptr<TimingEvent> s_transfer_event;
static std::unique_ptr<Common::WAVWriter> s_dump_writer;
static std::unique_ptr<AudioStream> s_audio_stream;
static std::unique_ptr<AudioStream> s_null_audio_stream;
static bool s_audio_output_muted = false;

static TickCount s_ticks_carry = 0;
static TickCount s_cpu_ticks_per_spu_tick = 0;
static TickCount s_cpu_tick_divider = 0;

static SPUCNT s_SPUCNT = {};
static SPUSTAT s_SPUSTAT = {};

static TransferControl s_transfer_control = {};
static u16 s_transfer_address_reg = 0;
static u32 s_transfer_address = 0;

static u16 s_irq_address = 0;
static u16 s_capture_buffer_position = 0;

static VolumeRegister s_main_volume_left_reg = {};
static VolumeRegister s_main_volume_right_reg = {};
static VolumeSweep s_main_volume_left = {};
static VolumeSweep s_main_volume_right = {};

static s16 s_cd_audio_volume_left = 0;
static s16 s_cd_audio_volume_right = 0;

static s16 s_external_volume_left = 0;
static s16 s_external_volume_right = 0;

static u32 s_key_on_register = 0;
static u32 s_key_off_register = 0;
static u32 s_endx_register = 0;
static u32 s_pitch_modulation_enable_register = 0;

static u32 s_noise_mode_register = 0;
static u32 s_noise_count = 0;
static u32 s_noise_level = 0;

static u32 s_reverb_on_register = 0;
static u32 s_reverb_base_address = 0;
static u32 s_reverb_current_address = 0;
static ReverbRegisters s_reverb_registers{};
static std::array<std::array<s16, 128>, 2> s_reverb_downsample_buffer;
static std::array<std::array<s16, 64>, 2> s_reverb_upsample_buffer;
static s32 s_reverb_resample_buffer_position = 0;

static std::array<Voice, NUM_VOICES> s_voices{};

static InlineFIFOQueue<u16, FIFO_SIZE_IN_HALFWORDS> s_transfer_fifo;

static std::array<u8, RAM_SIZE> s_ram{};

#ifdef SPU_DUMP_ALL_VOICES
// +1 for reverb output
static std::array<std::unique_ptr<Common::WAVWriter>, NUM_VOICES + 1> s_voice_dump_writers;
#endif
} // namespace SPU

void SPU::Initialize()
{
    // (X * D) / N / 768 -> (X * D) / (N * 768)
    s_cpu_ticks_per_spu_tick = System::ScaleTicksToOverclock(SYSCLK_TICKS_PER_SPU_TICK);
    s_cpu_tick_divider = static_cast<TickCount>(g_settings.cpu_overclock_numerator * SYSCLK_TICKS_PER_SPU_TICK);
    s_tick_event = TimingEvents::CreateTimingEvent("SPU Sample", s_cpu_ticks_per_spu_tick, s_cpu_ticks_per_spu_tick,
                                                   &SPU::Execute, nullptr, false);
    s_transfer_event = TimingEvents::CreateTimingEvent(
        "SPU Transfer", TRANSFER_TICKS_PER_HALFWORD, TRANSFER_TICKS_PER_HALFWORD, &SPU::ExecuteTransfer, nullptr, false);
    s_null_audio_stream = AudioStream::CreateNullStream(SAMPLE_RATE, NUM_CHANNELS, g_settings.audio_buffer_ms);

    CreateOutputStream();
    Reset();
}

void SPU::CreateOutputStream()
{
    Log_InfoPrintf(
        "Creating '%s' audio stream, sample rate = %u, channels = %u, buffer = %u, latency = %u, stretching = %s",
        Settings::GetAudioBackendName(g_settings.audio_backend), SAMPLE_RATE, NUM_CHANNELS, g_settings.audio_buffer_ms,
        g_settings.audio_output_latency_ms, AudioStream::GetStretchModeName(g_settings.audio_stretch_mode));

    s_audio_stream = Host::CreateAudioStream(g_settings.audio_backend, SAMPLE_RATE, NUM_CHANNELS, g_settings.audio_buffer_ms,
                                             g_settings.audio_output_latency_ms, g_settings.audio_stretch_mode);
    if (!s_audio_stream)
    {
        Host::ReportErrorAsync("Error", "Failed to create or configure audio stream, falling back to null output.");
        s_audio_stream.reset();
        s_audio_stream = AudioStream::CreateNullStream(SAMPLE_RATE, NUM_CHANNELS, g_settings.audio_buffer_ms);
    }

    s_audio_stream->SetOutputVolume(System::GetAudioOutputVolume());
    s_audio_stream->SetPaused(System::IsPaused());
}

void SPU::RecreateOutputStream()
{
    s_audio_stream.reset();
    CreateOutputStream();
}

void SPU::CPUClockChanged()
{
    // (X * D) / N / 768 -> (X * D) / (N * 768)
    s_cpu_ticks_per_spu_tick = System::ScaleTicksToOverclock(SYSCLK_TICKS_PER_SPU_TICK);
    s_cpu_tick_divider = static_cast<TickCount>(g_settings.cpu_overclock_numerator * SYSCLK_TICKS_PER_SPU_TICK);
    s_ticks_carry = 0;
    UpdateEventInterval();
}

void SPU::Shutdown()
{
    StopDumpingAudio();
    s_tick_event.reset();
    s_transfer_event.reset();
    s_audio_stream.reset();
}

void SPU::Reset()
{
    s_ticks_carry = 0;

    s_SPUCNT.bits = 0;
    s_SPUSTAT.bits = 0;
    s_transfer_address = 0;
    s_transfer_address_reg = 0;
    s_irq_address = 0;
    s_capture_buffer_position = 0;
    s_main_volume_left_reg.bits = 0;
    s_main_volume_right_reg.bits = 0;
    s_main_volume_left = {};
    s_main_volume_right = {};
    s_cd_audio_volume_left = 0;
    s_cd_audio_volume_right = 0;
    s_external_volume_left = 0;
    s_external_volume_right = 0;
    s_key_on_register = 0;
    s_key_off_register = 0;
    s_endx_register = 0;
    s_pitch_modulation_enable_register = 0;

    s_noise_mode_register = 0;
    s_noise_count = 0;
    s_noise_level = 1;

    s_reverb_on_register = 0;
    s_reverb_registers = {};
    s_reverb_registers.mBASE = 0;
    s_reverb_base_address = s_reverb_current_address = ZeroExtend32(s_reverb_registers.mBASE) << 2;
    s_reverb_downsample_buffer = {};
    s_reverb_upsample_buffer = {};
    s_reverb_resample_buffer_position = 0;

    for (u32 i = 0; i < NUM_VOICES; i++)
    {
        Voice& v = s_voices[i];
        v.current_address = 0;
        std::fill_n(v.regs.index, NUM_VOICE_REGISTERS, u16(0));
        v.counter.bits = 0;
        v.current_block_flags.bits = 0;
        v.is_first_block = 0;
        v.current_block_samples.fill(s16(0));
        v.adpcm_last_samples.fill(s32(0));
        v.adsr_envelope.Reset(0, false, false);
        v.adsr_phase = ADSRPhase::Off;
        v.adsr_target = 0;
        v.has_samples = false;
        v.ignore_loop_address = false;
    }

    s_transfer_fifo.Clear();
    s_transfer_event->Deactivate();
    s_ram.fill(0);
    UpdateEventInterval();
}

bool SPU::DoState(StateWrapper& sw)
{
    sw.Do(&s_ticks_carry);
    sw.Do(&s_SPUCNT.bits);
    sw.Do(&s_SPUSTAT.bits);
    sw.Do(&s_transfer_control.bits);
    sw.Do(&s_transfer_address);
    sw.Do(&s_transfer_address_reg);
    sw.Do(&s_irq_address);
    sw.Do(&s_capture_buffer_position);
    sw.Do(&s_main_volume_left_reg.bits);
    sw.Do(&s_main_volume_right_reg.bits);
    sw.DoPOD(&s_main_volume_left);
    sw.DoPOD(&s_main_volume_right);
    sw.Do(&s_cd_audio_volume_left);
    sw.Do(&s_cd_audio_volume_right);
    sw.Do(&s_external_volume_left);
    sw.Do(&s_external_volume_right);
    sw.Do(&s_key_on_register);
    sw.Do(&s_key_off_register);
    sw.Do(&s_endx_register);
    sw.Do(&s_pitch_modulation_enable_register);
    sw.Do(&s_noise_mode_register);
    sw.Do(&s_noise_count);
    sw.Do(&s_noise_level);
    sw.Do(&s_reverb_on_register);
    sw.Do(&s_reverb_base_address);
    sw.Do(&s_reverb_current_address);
    sw.Do(&s_reverb_registers.vLOUT);
    sw.Do(&s_reverb_registers.vROUT);
    sw.Do(&s_reverb_registers.mBASE);
    sw.DoArray(s_reverb_registers.rev, NUM_REVERB_REGS);
    for (u32 i = 0; i < 2; i++)
        sw.DoArray(s_reverb_downsample_buffer.data(), s_reverb_downsample_buffer.size());
    for (u32 i = 0; i < 2; i++)
        sw.DoArray(s_reverb_upsample_buffer.data(), s_reverb_upsample_buffer.size());
    sw.Do(&s_reverb_resample_buffer_position);
    for (u32 i = 0; i < NUM_VOICES; i++)
    {
        Voice& v = s_voices[i];
        sw.Do(&v.current_address);
        sw.DoArray(v.regs.index, NUM_VOICE_REGISTERS);
        sw.Do(&v.counter.bits);
        sw.Do(&v.current_block_flags.bits);
        sw.DoEx(&v.is_first_block, 47, false);
        sw.DoArray(&v.current_block_samples[NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK], NUM_SAMPLES_PER_ADPCM_BLOCK);
        sw.DoArray(&v.current_block_samples[0], NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK);
        sw.Do(&v.adpcm_last_samples);
        sw.Do(&v.last_volume);
        sw.DoPOD(&v.left_volume);
        sw.DoPOD(&v.right_volume);
        sw.DoPOD(&v.adsr_envelope);
        sw.Do(&v.adsr_phase);
        sw.Do(&v.adsr_target);
        sw.Do(&v.has_samples);
        sw.Do(&v.ignore_loop_address);
    }

    sw.Do(&s_transfer_fifo);
    sw.DoBytes(s_ram.data(), RAM_SIZE);

    if (sw.IsReading())
    {
        UpdateEventInterval();
        UpdateTransferEvent();
    }

    return !sw.HasError();
}

u16 SPU::ReadRegister(u32 offset)
{
    switch (offset)
    {
        case 0x1F801D80 - SPU_BASE:
            return s_main_volume_left_reg.bits;

        case 0x1F801D82 - SPU_BASE:
            return s_main_volume_right_reg.bits;

        case 0x1F801D84 - SPU_BASE:
            return s_reverb_registers.vLOUT;

        case 0x1F801D86 - SPU_BASE:
            return s_reverb_registers.vROUT;

        case 0x1F801D88 - SPU_BASE:
            return Truncate16(s_key_on_register);

        case 0x1F801D8A - SPU_BASE:
            return Truncate16(s_key_on_register >> 16);

        case 0x1F801D8C - SPU_BASE:
            return Truncate16(s_key_off_register);

        case 0x1F801D8E - SPU_BASE:
            return Truncate16(s_key_off_register >> 16);

        case 0x1F801D90 - SPU_BASE:
            return Truncate16(s_pitch_modulation_enable_register);

        case 0x1F801D92 - SPU_BASE:
            return Truncate16(s_pitch_modulation_enable_register >> 16);

        case 0x1F801D94 - SPU_BASE:
            return Truncate16(s_noise_mode_register);

        case 0x1F801D96 - SPU_BASE:
            return Truncate16(s_noise_mode_register >> 16);

        case 0x1F801D98 - SPU_BASE:
            return Truncate16(s_reverb_on_register);

        case 0x1F801D9A - SPU_BASE:
            return Truncate16(s_reverb_on_register >> 16);

        case 0x1F801D9C - SPU_BASE:
            return Truncate16(s_endx_register);

        case 0x1F801D9E - SPU_BASE:
            return Truncate16(s_endx_register >> 16);

        case 0x1F801DA2 - SPU_BASE:
            return s_reverb_registers.mBASE;

        case 0x1F801DA4 - SPU_BASE:
            Log_TracePrintf("SPU IRQ address -> 0x%04X", ZeroExtend32(s_irq_address));
            return s_irq_address;

        case 0x1F801DA6 - SPU_BASE:
            Log_TracePrintf("SPU transfer address register -> 0x%04X", ZeroExtend32(s_transfer_address_reg));
            return s_transfer_address_reg;

        case 0x1F801DA8 - SPU_BASE:
            Log_TracePrintf("SPU transfer data register read");
            return UINT16_C(0xFFFF);

        case 0x1F801DAA - SPU_BASE:
            Log_TracePrintf("SPU control register -> 0x%04X", ZeroExtend32(s_SPUCNT.bits));
            return s_SPUCNT.bits;

        case 0x1F801DAC - SPU_BASE:
            Log_TracePrintf("SPU transfer control register -> 0x%04X", ZeroExtend32(s_transfer_control.bits));
            return s_transfer_control.bits;

        case 0x1F801DAE - SPU_BASE:
            GeneratePendingSamples();
            Log_TracePrintf("SPU status register -> 0x%04X", ZeroExtend32(s_SPUCNT.bits));
            return s_SPUSTAT.bits;

        case 0x1F801DB0 - SPU_BASE:
            return s_cd_audio_volume_left;

        case 0x1F801DB2 - SPU_BASE:
            return s_cd_audio_volume_right;

        case 0x1F801DB4 - SPU_BASE:
            return s_external_volume_left;

        case 0x1F801DB6 - SPU_BASE:
            return s_external_volume_right;

        case 0x1F801DB8 - SPU_BASE:
            GeneratePendingSamples();
            return s_main_volume_left.current_level;

        case 0x1F801DBA - SPU_BASE:
            GeneratePendingSamples();
            return s_main_volume_right.current_level;

        default:
        {
            if (offset < (0x1F801D80 - SPU_BASE))
                return ReadVoiceRegister(offset);

            if (offset >= (0x1F801DC0 - SPU_BASE) && offset < (0x1F801E00 - SPU_BASE))
                return s_reverb_registers.rev[(offset - (0x1F801DC0 - SPU_BASE)) / 2];

            if (offset >= (0x1F801E00 - SPU_BASE) && offset < (0x1F801E60 - SPU_BASE))
            {
                const u32 voice_index = (offset - (0x1F801E00 - SPU_BASE)) / 4;
                GeneratePendingSamples();
                if (offset & 0x02)
                    return s_voices[voice_index].left_volume.current_level;
                else
                    return s_voices[voice_index].right_volume.current_level;
            }

            Log_DevPrintf("Unknown SPU register read: offset 0x%X (address 0x%08X)", offset, offset | SPU_BASE);
            return UINT16_C(0xFFFF);
        }
    }
}

void SPU::WriteRegister(u32 offset, u16 value)
{
    switch (offset)
    {
        case 0x1F801D80 - SPU_BASE:
        {
            Log_DebugPrintf("SPU main volume left <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_main_volume_left_reg.bits = value;
            s_main_volume_left.Reset(s_main_volume_left_reg);
            return;
        }

        case 0x1F801D82 - SPU_BASE:
        {
            Log_DebugPrintf("SPU main volume right <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_main_volume_right_reg.bits = value;
            s_main_volume_right.Reset(s_main_volume_right_reg);
            return;
        }

        case 0x1F801D84 - SPU_BASE:
        {
            Log_DebugPrintf("SPU reverb output volume left <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_reverb_registers.vLOUT = value;
            return;
        }

        case 0x1F801D86 - SPU_BASE:
        {
            Log_DebugPrintf("SPU reverb output volume right <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_reverb_registers.vROUT = value;
            return;
        }

        case 0x1F801D88 - SPU_BASE:
        {
            Log_DebugPrintf("SPU key on low <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_key_on_register = (s_key_on_register & 0xFFFF0000) | ZeroExtend32(value);
        }
        break;

        case 0x1F801D8A - SPU_BASE:
        {
            Log_DebugPrintf("SPU key on high <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_key_on_register = (s_key_on_register & 0x0000FFFF) | (ZeroExtend32(value) << 16);
        }
        break;

        case 0x1F801D8C - SPU_BASE:
        {
            Log_DebugPrintf("SPU key off low <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_key_off_register = (s_key_off_register & 0xFFFF0000) | ZeroExtend32(value);
        }
        break;

        case 0x1F801D8E - SPU_BASE:
        {
            Log_DebugPrintf("SPU key off high <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_key_off_register = (s_key_off_register & 0x0000FFFF) | (ZeroExtend32(value) << 16);
        }
        break;

        case 0x1F801D90 - SPU_BASE:
        {
            GeneratePendingSamples();
            s_pitch_modulation_enable_register = (s_pitch_modulation_enable_register & 0xFFFF0000) | ZeroExtend32(value);
            Log_DebugPrintf("SPU pitch modulation enable register <- 0x%08X", s_pitch_modulation_enable_register);
        }
        break;

        case 0x1F801D92 - SPU_BASE:
        {
            GeneratePendingSamples();
            s_pitch_modulation_enable_register = (s_pitch_modulation_enable_register & 0x0000FFFF) | (ZeroExtend32(value) << 16);
            Log_DebugPrintf("SPU pitch modulation enable register <- 0x%08X", s_pitch_modulation_enable_register);
        }
        break;

        case 0x1F801D94 - SPU_BASE:
        {
            Log_DebugPrintf("SPU noise mode register <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_noise_mode_register = (s_noise_mode_register & 0xFFFF0000) | ZeroExtend32(value);
        }
        break;

        case 0x1F801D96 - SPU_BASE:
        {
            Log_DebugPrintf("SPU noise mode register <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_noise_mode_register = (s_noise_mode_register & 0x0000FFFF) | (ZeroExtend32(value) << 16);
        }
        break;

        case 0x1F801D98 - SPU_BASE:
        {
            Log_DebugPrintf("SPU reverb on register <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_reverb_on_register = (s_reverb_on_register & 0xFFFF0000) | ZeroExtend32(value);
        }
        break;

        case 0x1F801D9A - SPU_BASE:
        {
            Log_DebugPrintf("SPU reverb on register <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_reverb_on_register = (s_reverb_on_register & 0x0000FFFF) | (ZeroExtend32(value) << 16);
        }
        break;

        case 0x1F801DA2 - SPU_BASE:
        {
            Log_DebugPrintf("SPU reverb base address < 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_reverb_registers.mBASE = value;
            s_reverb_base_address = ZeroExtend32(value << 2) & 0x3FFFFu;
            s_reverb_current_address = s_reverb_base_address;
        }
        break;

        case 0x1F801DA4 - SPU_BASE:
        {
            Log_DebugPrintf("SPU IRQ address register <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_irq_address = value;

            if (IsRAMIRQTriggerable())
                CheckForLateRAMIRQs();

            return;
        }

        case 0x1F801DA6 - SPU_BASE:
        {
            Log_DebugPrintf("SPU transfer address register <- 0x%04X", ZeroExtend32(value));
            s_transfer_event->InvokeEarly();
            s_transfer_address_reg = value;
            s_transfer_address = ZeroExtend32(value) * 8;
            if (IsRAMIRQTriggerable() && CheckRAMIRQ(s_transfer_address))
            {
                Log_DebugPrintf("Trigger IRQ @ %08X %04X from transfer address reg set", s_transfer_address,
                                s_transfer_address / 8);
                TriggerRAMIRQ();
            }
            return;
        }

        case 0x1F801DA8 - SPU_BASE:
        {
            Log_TracePrintf("SPU transfer data register <- 0x%04X (RAM offset 0x%08X)", ZeroExtend32(value),
                            s_transfer_address);

            ManualTransferWrite(value);
            return;
        }

        case 0x1F801DAA - SPU_BASE:
        {
            Log_DebugPrintf("SPU control register <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();

            const SPUCNT new_value{value};
            if (new_value.ram_transfer_mode != s_SPUCNT.ram_transfer_mode && new_value.ram_transfer_mode == RAMTransferMode::Stopped)
            {
                // clear the fifo here?
                if (!s_transfer_fifo.IsEmpty())
                {
                    if (s_SPUCNT.ram_transfer_mode == RAMTransferMode::DMAWrite)
                    {
                        // I would guess on the console it would gradually write the FIFO out. Hopefully nothing relies on this
                        // level of timing granularity if we force it all out here.
                        Log_WarningPrintf("Draining write SPU transfer FIFO with %u bytes left", s_transfer_fifo.GetSize());
                        TickCount ticks = std::numeric_limits<TickCount>::max();
                        ExecuteFIFOWriteToRAM(ticks);
                        DebugAssert(s_transfer_fifo.IsEmpty());
                    }
                    else
                    {
                        Log_DebugPrintf("Clearing read SPU transfer FIFO with %u bytes left", s_transfer_fifo.GetSize());
                        s_transfer_fifo.Clear();
                    }
                }
            }

            if (!new_value.enable && s_SPUCNT.enable)
            {
                // Mute all voices.
                // Interestingly, hardware tests found this seems to happen immediately, not on the next 44100hz cycle.
                for (u32 i = 0; i < NUM_VOICES; i++)
                    s_voices[i].ForceOff();
            }

            s_SPUCNT.bits = new_value.bits;
            s_SPUSTAT.mode = s_SPUCNT.mode.GetValue();

            if (!s_SPUCNT.irq9_enable)
                s_SPUSTAT.irq9_flag = false;
            else if (IsRAMIRQTriggerable())
                CheckForLateRAMIRQs();

            UpdateEventInterval();
            UpdateDMARequest();
            UpdateTransferEvent();
            return;
        }

        case 0x1F801DAC - SPU_BASE:
        {
            Log_DebugPrintf("SPU transfer control register <- 0x%04X", ZeroExtend32(value));
            s_transfer_control.bits = value;
            return;
        }

        case 0x1F801DB0 - SPU_BASE:
        {
            Log_DebugPrintf("SPU left cd audio register <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_cd_audio_volume_left = value;
        }
        break;

        case 0x1F801DB2 - SPU_BASE:
        {
            Log_DebugPrintf("SPU right cd audio register <- 0x%04X", ZeroExtend32(value));
            GeneratePendingSamples();
            s_cd_audio_volume_right = value;
        }
        break;

        case 0x1F801DB4 - SPU_BASE:
        {
            // External volumes aren't used, so don't bother syncing.
            Log_DebugPrintf("SPU left external volume register <- 0x%04X", ZeroExtend32(value));
            s_external_volume_left = value;
        }
        break;

        case 0x1F801DB6 - SPU_BASE:
        {
            // External volumes aren't used, so don't bother syncing.
            Log_DebugPrintf("SPU right external volume register <- 0x%04X", ZeroExtend32(value));
            s_external_volume_right = value;
        }
        break;

            // read-only registers
        case 0x1F801DAE - SPU_BASE:
        {
            return;
        }

        default:
        {
            if (offset < (0x1F801D80 - SPU_BASE))
            {
                WriteVoiceRegister(offset, value);
                return;
            }

            if (offset >= (0x1F801DC0 - SPU_BASE) && offset < (0x1F801E00 - SPU_BASE))
            {
                const u32 reg = (offset - (0x1F801DC0 - SPU_BASE)) / 2;
                Log_DebugPrintf("SPU reverb register %u <- 0x%04X", reg, value);
                GeneratePendingSamples();
                s_reverb_registers.rev[reg] = value;
                return;
            }

            Log_DevPrintf("Unknown SPU register write: offset 0x%X (address 0x%08X) value 0x%04X", offset, offset | SPU_BASE,
                          ZeroExtend32(value));
            return;
        }
    }
}

u16 SPU::ReadVoiceRegister(u32 offset)
{
    const u32 reg_index = (offset % 0x10) / 2; //(offset & 0x0F) / 2;
    const u32 voice_index = (offset / 0x10);   //((offset >> 4) & 0x1F);
    Assert(voice_index < 24);

    // ADSR volume needs to be updated when reading. A voice might be off as well, but key on is pending.
    const Voice& voice = s_voices[voice_index];
    if (reg_index >= 6 && (voice.IsOn() || s_key_on_register & (1u << voice_index)))
        GeneratePendingSamples();

    Log_TracePrintf("Read voice %u register %u -> 0x%02X", voice_index, reg_index, voice.regs.index[reg_index]);
    return voice.regs.index[reg_index];
}

void SPU::WriteVoiceRegister(u32 offset, u16 value)
{
    // per-voice registers
    const u32 reg_index = (offset % 0x10);
    const u32 voice_index = (offset / 0x10);
    DebugAssert(voice_index < 24);

    Voice& voice = s_voices[voice_index];
    if (voice.IsOn() || s_key_on_register & (1u << voice_index))
        GeneratePendingSamples();

    switch (reg_index)
    {
        case 0x00: // volume left
        {
            Log_DebugPrintf("SPU voice %u volume left <- 0x%04X", voice_index, value);
            voice.regs.volume_left.bits = value;
            voice.left_volume.Reset(voice.regs.volume_left);
        }
        break;

        case 0x02: // volume right
        {
            Log_DebugPrintf("SPU voice %u volume right <- 0x%04X", voice_index, value);
            voice.regs.volume_right.bits = value;
            voice.right_volume.Reset(voice.regs.volume_right);
        }
        break;

        case 0x04: // sample rate
        {
            Log_DebugPrintf("SPU voice %u ADPCM sample rate <- 0x%04X", voice_index, value);
            voice.regs.adpcm_sample_rate = value;
        }
        break;

        case 0x06: // start address
        {
            Log_DebugPrintf("SPU voice %u ADPCM start address <- 0x%04X", voice_index, value);
            voice.regs.adpcm_start_address = value;
        }
        break;

        case 0x08: // adsr low
        {
            Log_DebugPrintf("SPU voice %u ADSR low <- 0x%04X (was 0x%04X)", voice_index, value, voice.regs.adsr.bits_low);
            voice.regs.adsr.bits_low = value;
            if (voice.IsOn())
                voice.UpdateADSREnvelope();
        }
        break;

        case 0x0A: // adsr high
        {
            Log_DebugPrintf("SPU voice %u ADSR high <- 0x%04X (was 0x%04X)", voice_index, value, voice.regs.adsr.bits_low);
            voice.regs.adsr.bits_high = value;
            if (voice.IsOn())
                voice.UpdateADSREnvelope();
        }
        break;

        case 0x0C: // adsr volume
        {
            Log_DebugPrintf("SPU voice %u ADSR volume <- 0x%04X (was 0x%04X)", voice_index, value, voice.regs.adsr_volume);
            voice.regs.adsr_volume = value;
        }
        break;

        case 0x0E: // repeat address
        {
            // There is a short window of time here between the voice being keyed on and the first block finishing decoding
            // where setting the repeat address will *NOT* ignore the block/loop start flag. Games sensitive to this are:
            //  - The Misadventures of Tron Bonne
            //  - Re-Loaded - The Hardcore Sequel
            //  - Valkyrie Profile

            const bool ignore_loop_address = voice.IsOn() && !voice.is_first_block;
            Log_DebugPrintf("SPU voice %u ADPCM repeat address <- 0x%04X", voice_index, value);
            voice.regs.adpcm_repeat_address = value;
            voice.ignore_loop_address |= ignore_loop_address;

            if (!ignore_loop_address)
            {
                Log_DevPrintf("Not ignoring loop address, the ADPCM repeat address of 0x%04X for voice %u will be overwritten",
                              value, voice_index);
            }
        }
        break;

        default:
        {
            Log_ErrorPrintf("Unknown SPU voice %u register write: offset 0x%X (address 0x%08X) value 0x%04X", offset,
                            voice_index, offset | SPU_BASE, ZeroExtend32(value));
        }
        break;
    }
}

bool SPU::IsVoiceReverbEnabled(u32 i)
{
    return ConvertToBoolUnchecked((s_reverb_on_register >> i) & u32(1));
}

bool SPU::IsVoiceNoiseEnabled(u32 i)
{
    return ConvertToBoolUnchecked((s_noise_mode_register >> i) & u32(1));
}

bool SPU::IsPitchModulationEnabled(u32 i)
{
    return ((i > 0) && ConvertToBoolUnchecked((s_pitch_modulation_enable_register >> i) & u32(1)));
}

s16 SPU::GetVoiceNoiseLevel()
{
    return static_cast<s16>(static_cast<u16>(s_noise_level));
}

bool SPU::IsRAMIRQTriggerable()
{
    return s_SPUCNT.irq9_enable && !s_SPUSTAT.irq9_flag;
}

bool SPU::CheckRAMIRQ(u32 address)
{
    return ((ZeroExtend32(s_irq_address) * 8) == address);
}

void SPU::TriggerRAMIRQ()
{
    DebugAssert(IsRAMIRQTriggerable());
    s_SPUSTAT.irq9_flag = true;
    InterruptController::InterruptRequest(InterruptController::IRQ::SPU);
}

void SPU::CheckForLateRAMIRQs()
{
    if (CheckRAMIRQ(s_transfer_address))
    {
        Log_DebugPrintf("Trigger IRQ @ %08X %04X from late transfer", s_transfer_address, s_transfer_address / 8);
        TriggerRAMIRQ();
        return;
    }

    for (u32 i = 0; i < NUM_VOICES; i++)
    {
        // we skip voices which haven't started this block yet - because they'll check
        // the next time they're sampled, and the delay might be important.
        const Voice& v = s_voices[i];
        if (!v.has_samples)
            continue;

        const u32 address = v.current_address * 8;
        if (CheckRAMIRQ(address) || CheckRAMIRQ((address + 8) & RAM_MASK))
        {
            Log_DebugPrintf("Trigger IRQ @ %08X %04X from late", address, address / 8);
            TriggerRAMIRQ();
            return;
        }
    }
}

void SPU::WriteToCaptureBuffer(u32 index, s16 value)
{
    const u32 ram_address = (index * CAPTURE_BUFFER_SIZE_PER_CHANNEL) | ZeroExtend16(s_capture_buffer_position);
    // Log_DebugPrintf("write to capture buffer %u (0x%08X) <- 0x%04X", index, ram_address, u16(value));
    std::memcpy(&s_ram[ram_address], &value, sizeof(value));
    if (IsRAMIRQTriggerable() && CheckRAMIRQ(ram_address))
    {
        Log_DebugPrintf("Trigger IRQ @ %08X %04X from capture buffer", ram_address, ram_address / 8);
        TriggerRAMIRQ();
    }
}

void SPU::IncrementCaptureBufferPosition()
{
    s_capture_buffer_position += sizeof(s16);
    s_capture_buffer_position %= CAPTURE_BUFFER_SIZE_PER_CHANNEL;
    s_SPUSTAT.second_half_capture_buffer = s_capture_buffer_position >= (CAPTURE_BUFFER_SIZE_PER_CHANNEL / 2);
}

ALWAYS_INLINE_RELEASE void SPU::ExecuteFIFOReadFromRAM(TickCount& ticks)
{
    while (ticks > 0 && !s_transfer_fifo.IsFull())
    {
        u16 value;
        std::memcpy(&value, &s_ram[s_transfer_address], sizeof(u16));
        s_transfer_address = (s_transfer_address + sizeof(u16)) & RAM_MASK;
        s_transfer_fifo.Push(value);
        ticks -= TRANSFER_TICKS_PER_HALFWORD;

        if (IsRAMIRQTriggerable() && CheckRAMIRQ(s_transfer_address))
        {
            Log_DebugPrintf("Trigger IRQ @ %08X %04X from transfer read", s_transfer_address, s_transfer_address / 8);
            TriggerRAMIRQ();
        }
    }
}

ALWAYS_INLINE_RELEASE void SPU::ExecuteFIFOWriteToRAM(TickCount& ticks)
{
    while (ticks > 0 && !s_transfer_fifo.IsEmpty())
    {
        u16 value = s_transfer_fifo.Pop();
        std::memcpy(&s_ram[s_transfer_address], &value, sizeof(u16));
        s_transfer_address = (s_transfer_address + sizeof(u16)) & RAM_MASK;
        ticks -= TRANSFER_TICKS_PER_HALFWORD;

        if (IsRAMIRQTriggerable() && CheckRAMIRQ(s_transfer_address))
        {
            Log_DebugPrintf("Trigger IRQ @ %08X %04X from transfer write", s_transfer_address, s_transfer_address / 8);
            TriggerRAMIRQ();
        }
    }
}

void SPU::ExecuteTransfer(void* param, TickCount ticks, TickCount ticks_late)
{
    const RAMTransferMode mode = s_SPUCNT.ram_transfer_mode;
    DebugAssert(mode != RAMTransferMode::Stopped);

    if (mode == RAMTransferMode::DMARead)
    {
        while (ticks > 0 && !s_transfer_fifo.IsFull())
        {
            ExecuteFIFOReadFromRAM(ticks);

            // this can result in the FIFO being emptied, hence double the while loop
            UpdateDMARequest();
        }

        // we're done if we have no more data to read
        if (s_transfer_fifo.IsFull())
        {
            s_SPUSTAT.transfer_busy = false;
            s_transfer_event->Deactivate();
            return;
        }

        s_SPUSTAT.transfer_busy = true;
        const TickCount ticks_until_complete = TickCount(s_transfer_fifo.GetSpace() * u32(TRANSFER_TICKS_PER_HALFWORD)) + ((ticks < 0) ? -ticks : 0);
        s_transfer_event->Schedule(ticks_until_complete);
    }
    else
    {
        // write the fifo to ram, request dma again when empty
        while (ticks > 0 && !s_transfer_fifo.IsEmpty())
        {
            ExecuteFIFOWriteToRAM(ticks);

            // similar deal here, the FIFO can be written out in a long slice
            UpdateDMARequest();
        }

        // we're done if we have no more data to write
        if (s_transfer_fifo.IsEmpty())
        {
            s_SPUSTAT.transfer_busy = false;
            s_transfer_event->Deactivate();
            return;
        }

        s_SPUSTAT.transfer_busy = true;
        const TickCount ticks_until_complete = TickCount(s_transfer_fifo.GetSize() * u32(TRANSFER_TICKS_PER_HALFWORD)) + ((ticks < 0) ? -ticks : 0);
        s_transfer_event->Schedule(ticks_until_complete);
    }
}

void SPU::ManualTransferWrite(u16 value)
{
    if (!s_transfer_fifo.IsEmpty() && s_SPUCNT.ram_transfer_mode != RAMTransferMode::DMARead)
    {
        Log_WarningPrintf("FIFO not empty on manual SPU write, draining to hopefully avoid corruption. Game is silly.");
        if (s_SPUCNT.ram_transfer_mode != RAMTransferMode::Stopped)
            ExecuteTransfer(nullptr, std::numeric_limits<s32>::max(), 0);
    }

    std::memcpy(&s_ram[s_transfer_address], &value, sizeof(u16));
    s_transfer_address = (s_transfer_address + sizeof(u16)) & RAM_MASK;

    if (IsRAMIRQTriggerable() && CheckRAMIRQ(s_transfer_address))
    {
        Log_DebugPrintf("Trigger IRQ @ %08X %04X from manual write", s_transfer_address, s_transfer_address / 8);
        TriggerRAMIRQ();
    }
}

void SPU::UpdateTransferEvent()
{
    const RAMTransferMode mode = s_SPUCNT.ram_transfer_mode;
    if (mode == RAMTransferMode::Stopped)
    {
        s_transfer_event->Deactivate();
    }
    else if (mode == RAMTransferMode::DMARead)
    {
        // transfer event fills the fifo
        if (s_transfer_fifo.IsFull())
            s_transfer_event->Deactivate();
        else if (!s_transfer_event->IsActive())
            s_transfer_event->Schedule(TickCount(s_transfer_fifo.GetSpace() * u32(TRANSFER_TICKS_PER_HALFWORD)));
    }
    else
    {
        // transfer event copies from fifo to ram
        if (s_transfer_fifo.IsEmpty())
            s_transfer_event->Deactivate();
        else if (!s_transfer_event->IsActive())
            s_transfer_event->Schedule(TickCount(s_transfer_fifo.GetSize() * u32(TRANSFER_TICKS_PER_HALFWORD)));
    }

    s_SPUSTAT.transfer_busy = s_transfer_event->IsActive();
}

void SPU::UpdateDMARequest()
{
    switch (s_SPUCNT.ram_transfer_mode)
    {
        case RAMTransferMode::DMARead:
            s_SPUSTAT.dma_read_request = s_transfer_fifo.IsFull();
            s_SPUSTAT.dma_write_request = false;
            s_SPUSTAT.dma_request = s_SPUSTAT.dma_read_request;
            break;

        case RAMTransferMode::DMAWrite:
            s_SPUSTAT.dma_read_request = false;
            s_SPUSTAT.dma_write_request = s_transfer_fifo.IsEmpty();
            s_SPUSTAT.dma_request = s_SPUSTAT.dma_write_request;
            break;

        case RAMTransferMode::Stopped:
        case RAMTransferMode::ManualWrite:
        default:
            s_SPUSTAT.dma_read_request = false;
            s_SPUSTAT.dma_write_request = false;
            s_SPUSTAT.dma_request = false;
            break;
    }

    // This might call us back directly.
    DMA::SetRequest(DMA::Channel::SPU, s_SPUSTAT.dma_request);
}

void SPU::DMARead(u32* words, u32 word_count)
{
    /*
    From @JaCzekanski - behavior when block size is larger than the FIFO size
    for blocks <= 0x16 - all data is transferred correctly
    using block size 0x20 transfer behaves strange:
    % Writing 524288 bytes to SPU RAM to 0x00000000 using DMA... ok
    % Reading 256 bytes from SPU RAM from 0x00001000 using DMA... ok
    % 0x00001000: 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f ................
    % 0x00001010: 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f ................
    % 0x00001020: 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f  !"#$%&'()*+,-./
    % 0x00001030: 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f 0123456789:;<=>?
    % 0x00001040: 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f >?>?>?>?>?>?>?>?
    % 0x00001050: 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f >?>?>?>?>?>?>?>?
    % 0x00001060: 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f >?>?>?>?>?>?>?>?
    % 0x00001070: 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f 3e 3f >?>?>?>?>?>?>?>?
    % 0x00001080: 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f @ABCDEFGHIJKLMNO
    % 0x00001090: 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f PQRSTUVWXYZ[\]^_
    % 0x000010a0: 60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f `abcdefghijklmno
    % 0x000010b0: 70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f pqrstuvwxyz{|}~.
    % 0x000010c0: 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f ~.~.~.~.~.~.~.~.
    % 0x000010d0: 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f ~.~.~.~.~.~.~.~.
    % 0x000010e0: 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f ~.~.~.~.~.~.~.~.
    % 0x000010f0: 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f 7e 7f ~.~.~.~.~.~.~.~.
    Using Block size = 0x10 (correct data)
    % Reading 256 bytes from SPU RAM from 0x00001000 using DMA... ok
    % 0x00001000: 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f ................
    % 0x00001010: 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f ................
    % 0x00001020: 20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f  !"#$%&'()*+,-./
    % 0x00001030: 30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f 0123456789:;<=>?
    % 0x00001040: 40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f @ABCDEFGHIJKLMNO
    % 0x00001050: 50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f PQRSTUVWXYZ[\]^_
    % 0x00001060: 60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f `abcdefghijklmno
    % 0x00001070: 70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f pqrstuvwxyz{|}~.
    % 0x00001080: 80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f ................
    % 0x00001090: 90 91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f ................
    % 0x000010a0: a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af ................
    % 0x000010b0: b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf ................
    % 0x000010c0: c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf ................
    % 0x000010d0: d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df ................
    % 0x000010e0: e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef ................
    % 0x000010f0: f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff ................
   */

    u16* halfwords = reinterpret_cast<u16*>(words);
    u32 halfword_count = word_count * 2;

    const u32 size = s_transfer_fifo.GetSize();
    if (word_count > size)
    {
        u16 fill_value = 0;
        if (size > 0)
        {
            s_transfer_fifo.PopRange(halfwords, size);
            fill_value = halfwords[size - 1];
        }

        Log_WarningPrintf("Transfer FIFO underflow, filling with 0x%04X", fill_value);
        std::fill_n(&halfwords[size], halfword_count - size, fill_value);
    }
    else
    {
        s_transfer_fifo.PopRange(halfwords, halfword_count);
    }

    UpdateDMARequest();
    UpdateTransferEvent();
}

void SPU::DMAWrite(const u32* words, u32 word_count)
{
    const u16* halfwords = reinterpret_cast<const u16*>(words);
    u32 halfword_count = word_count * 2;

    const u32 words_to_transfer = std::min(s_transfer_fifo.GetSpace(), halfword_count);
    s_transfer_fifo.PushRange(halfwords, words_to_transfer);

    if (words_to_transfer != halfword_count)
        Log_WarningPrintf("Transfer FIFO overflow, dropping %u halfwords", halfword_count - words_to_transfer);

    UpdateDMARequest();
    UpdateTransferEvent();
}

void SPU::GeneratePendingSamples()
{
    if (s_transfer_event->IsActive())
        s_transfer_event->InvokeEarly();

    const TickCount ticks_pending = s_tick_event->GetTicksSinceLastExecution();
    TickCount frames_to_execute;
    if (g_settings.cpu_overclock_active)
    {
        frames_to_execute = static_cast<u32>((static_cast<u64>(ticks_pending) * g_settings.cpu_overclock_denominator) + static_cast<u32>(s_ticks_carry)) / static_cast<u32>(s_cpu_tick_divider);
    }
    else
    {
        frames_to_execute = (s_tick_event->GetTicksSinceLastExecution() + s_ticks_carry) / SYSCLK_TICKS_PER_SPU_TICK;
    }

    const bool force_exec = (frames_to_execute > 0);
    s_tick_event->InvokeEarly(force_exec);
}

//bool SPU::IsDumpingAudio()
//{
//    return static_cast<bool>(s_dump_writer);
//}
//
//bool SPU::StartDumpingAudio(const char* filename)
//{
//    s_dump_writer.reset();
//    s_dump_writer = std::make_unique<Common::WAVWriter>();
//    if (!s_dump_writer->Open(filename, SAMPLE_RATE, 2))
//    {
//        Log_ErrorPrintf("Failed to open '%s'", filename);
//        s_dump_writer.reset();
//        return false;
//    }
//
//#ifdef SPU_DUMP_ALL_VOICES
//    for (size_t i = 0; i < s_voice_dump_writers.size(); i++)
//    {
//        s_voice_dump_writers[i].reset();
//        s_voice_dump_writers[i] = std::make_unique<Common::WAVWriter>();
//
//        TinyString new_suffix;
//        if (i == NUM_VOICES)
//            new_suffix.Assign("reverb.wav");
//        else
//            new_suffix.Format("voice%u.wav", i);
//
//        std::string voice_filename(FileSystem::ReplaceExtension(filename, new_suffix));
//        if (!s_voice_dump_writers[i]->Open(voice_filename.c_str(), SAMPLE_RATE, 2))
//        {
//            Log_ErrorPrintf("Failed to open voice dump filename '%s'", voice_filename.c_str());
//            s_voice_dump_writers[i].reset();
//        }
//    }
//#endif
//
//    return true;
//}
//
//bool SPU::StopDumpingAudio()
//{
//    if (!s_dump_writer)
//        return false;
//
//    s_dump_writer.reset();
//
//#ifdef SPU_DUMP_ALL_VOICES
//    for (size_t i = 0; i < s_voice_dump_writers.size(); i++)
//        s_voice_dump_writers[i].reset();
//#endif
//
//    return true;
//}

const std::array<u8, SPU::RAM_SIZE>& SPU::GetRAM()
{
    return s_ram;
}

std::array<u8, SPU::RAM_SIZE>& SPU::GetWritableRAM()
{
    return s_ram;
}

bool SPU::IsAudioOutputMuted()
{
    return s_audio_output_muted;
}

void SPU::SetAudioOutputMuted(bool muted)
{
    s_audio_output_muted = muted;
}

AudioStream* SPU::GetOutputStream()
{
    return s_audio_stream.get();
}

void SPU::Voice::KeyOn()
{
    current_address = regs.adpcm_start_address & ~u16(1);
    counter.bits = 0;
    regs.adsr_volume = 0;
    adpcm_last_samples.fill(0);

    // Samples from the previous block for interpolation should be zero. Fixes clicks in audio in Breath of Fire III.
    std::fill_n(&current_block_samples[NUM_SAMPLES_PER_ADPCM_BLOCK], NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK,
                static_cast<s16>(0));

    has_samples = false;
    is_first_block = true;
    ignore_loop_address = false;
    adsr_phase = ADSRPhase::Attack;
    UpdateADSREnvelope();
}

void SPU::Voice::KeyOff()
{
    if (adsr_phase == ADSRPhase::Off || adsr_phase == ADSRPhase::Release)
        return;

    adsr_phase = ADSRPhase::Release;
    UpdateADSREnvelope();
}

void SPU::Voice::ForceOff()
{
    if (adsr_phase == ADSRPhase::Off)
        return;

    regs.adsr_volume = 0;
    adsr_phase = ADSRPhase::Off;
}

SPU::ADSRPhase SPU::GetNextADSRPhase(ADSRPhase phase)
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

struct ADSRTableEntry
{
    s32 ticks;
    s32 step;
};
enum : u32
{
    NUM_ADSR_TABLE_ENTRIES = 128,
    NUM_ADSR_DIRECTIONS = 2 // increasing, decreasing
};
using ADSRTableEntries = std::array<std::array<ADSRTableEntry, NUM_ADSR_TABLE_ENTRIES>, NUM_ADSR_DIRECTIONS>;

static constexpr ADSRTableEntries ComputeADSRTableEntries()
{
    ADSRTableEntries entries = {};
    for (u32 decreasing = 0; decreasing < 2; decreasing++)
    {
        for (u32 rate = 0; rate < NUM_ADSR_TABLE_ENTRIES; rate++)
        {
            if (rate < 48)
            {
                entries[decreasing][rate].ticks = 1;
                if (decreasing != 0)
                    entries[decreasing][rate].step = static_cast<s32>(static_cast<u32>(-8 + static_cast<s32>(rate & 3)) << (11 - (rate >> 2)));
                else
                    entries[decreasing][rate].step = (7 - static_cast<s32>(rate & 3)) << (11 - (rate >> 2));
            }
            else
            {
                entries[decreasing][rate].ticks = 1 << (static_cast<s32>(rate >> 2) - 11);
                if (decreasing != 0)
                    entries[decreasing][rate].step = (-8 + static_cast<s32>(rate & 3));
                else
                    entries[decreasing][rate].step = (7 - static_cast<s32>(rate & 3));
            }
        }
    }

    return entries;
}

static constexpr ADSRTableEntries s_adsr_table = ComputeADSRTableEntries();

void SPU::VolumeEnvelope::Reset(u8 rate_, bool decreasing_, bool exponential_)
{
    rate = rate_;
    decreasing = decreasing_;
    exponential = exponential_;

    const ADSRTableEntry& table_entry = s_adsr_table[BoolToUInt8(decreasing)][rate];
    counter = table_entry.ticks;
}

s16 SPU::VolumeEnvelope::Tick(s16 current_level)
{
    counter--;
    if (counter > 0)
        return current_level;

    const ADSRTableEntry& table_entry = s_adsr_table[BoolToUInt8(decreasing)][rate];
    s32 this_step = table_entry.step;
    counter = table_entry.ticks;

    if (exponential)
    {
        if (decreasing)
        {
            this_step = (this_step * current_level) >> 15;
        }
        else
        {
            if (current_level >= 0x6000)
            {
                if (rate < 40)
                {
                    this_step >>= 2;
                }
                else if (rate >= 44)
                {
                    counter >>= 2;
                }
                else
                {
                    this_step >>= 1;
                    counter >>= 1;
                }
            }
        }
    }

    return static_cast<s16>(
        std::clamp<s32>(static_cast<s32>(current_level) + this_step, ENVELOPE_MIN_VOLUME, ENVELOPE_MAX_VOLUME));
}

void SPU::VolumeSweep::Reset(VolumeRegister reg)
{
    if (!reg.sweep_mode)
    {
        current_level = reg.fixed_volume_shr1 * 2;
        envelope_active = false;
        return;
    }

    envelope.Reset(reg.sweep_rate, reg.sweep_direction_decrease, reg.sweep_exponential);
    envelope_active = true;
}

void SPU::VolumeSweep::Tick()
{
    if (!envelope_active)
        return;

    current_level = envelope.Tick(current_level);
    envelope_active = (envelope.decreasing ? (current_level > ENVELOPE_MIN_VOLUME) : (current_level < ENVELOPE_MAX_VOLUME));
}

void SPU::Voice::UpdateADSREnvelope()
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

void SPU::Voice::TickADSR()
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

void SPU::Voice::DecodeBlock(const ADPCMBlock& block)
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

s32 SPU::Voice::Interpolate() const
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

    const u8 i = counter.interpolation_index;
    const u32 s = NUM_SAMPLES_FROM_LAST_ADPCM_BLOCK + ZeroExtend32(counter.sample_index.GetValue());

    s32 out = s32(gauss[0x0FF - i]) * s32(current_block_samples[s - 3]);
    out += s32(gauss[0x1FF - i]) * s32(current_block_samples[s - 2]);
    out += s32(gauss[0x100 + i]) * s32(current_block_samples[s - 1]);
    out += s32(gauss[0x000 + i]) * s32(current_block_samples[s - 0]);
    return out >> 15;
}

void SPU::ReadADPCMBlock(u16 address, ADPCMBlock* block)
{
    u32 ram_address = (ZeroExtend32(address) * 8) & RAM_MASK;
    if (IsRAMIRQTriggerable() && (CheckRAMIRQ(ram_address) || CheckRAMIRQ((ram_address + 8) & RAM_MASK)))
    {
        Log_DebugPrintf("Trigger IRQ @ %08X %04X from ADPCM reader", ram_address, ram_address / 8);
        TriggerRAMIRQ();
    }

    // fast path - no wrap-around
    if ((ram_address + sizeof(ADPCMBlock)) <= RAM_SIZE)
    {
        std::memcpy(block, &s_ram[ram_address], sizeof(ADPCMBlock));
        return;
    }

    block->shift_filter.bits = s_ram[ram_address];
    ram_address = (ram_address + 1) & RAM_MASK;
    block->flags.bits = s_ram[ram_address];
    ram_address = (ram_address + 1) & RAM_MASK;
    for (u32 i = 0; i < 14; i++)
    {
        block->data[i] = s_ram[ram_address];
        ram_address = (ram_address + 1) & RAM_MASK;
    }
}

ALWAYS_INLINE_RELEASE std::tuple<s32, s32> SPU::SampleVoice(u32 voice_index)
{
    Voice& voice = s_voices[voice_index];
    if (!voice.IsOn() && !s_SPUCNT.irq9_enable)
    {
        voice.last_volume = 0;

#ifdef SPU_DUMP_ALL_VOICES
        if (s_voice_dump_writers[voice_index])
        {
            const s16 dump_samples[2] = {0, 0};
            s_voice_dump_writers[voice_index]->WriteFrames(dump_samples, 1);
        }
#endif

        return {};
    }

    if (!voice.has_samples)
    {
        ADPCMBlock block;
        ReadADPCMBlock(voice.current_address, &block);
        voice.DecodeBlock(block);
        voice.has_samples = true;

        if (voice.current_block_flags.loop_start && !voice.ignore_loop_address)
        {
            Log_TracePrintf("Voice %u loop start @ 0x%08X", voice_index, ZeroExtend32(voice.current_address));
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
    DebugAssert(voice.counter.sample_index < NUM_SAMPLES_PER_ADPCM_BLOCK);
    voice.counter.bits += step;

    if (voice.counter.sample_index >= NUM_SAMPLES_PER_ADPCM_BLOCK)
    {
        // next block
        voice.counter.sample_index -= NUM_SAMPLES_PER_ADPCM_BLOCK;
        voice.has_samples = false;
        voice.is_first_block = false;
        voice.current_address += 2;

        // handle flags
        if (voice.current_block_flags.loop_end)
        {
            s_endx_register |= (u32(1) << voice_index);
            voice.current_address = voice.regs.adpcm_repeat_address & ~u16(1);

            if (!voice.current_block_flags.loop_repeat)
            {
                Log_TracePrintf("Voice %u loop end+mute @ 0x%08X", voice_index, ZeroExtend32(voice.current_address));
                voice.ForceOff();
            }
            else
            {
                Log_TracePrintf("Voice %u loop end+repeat @ 0x%08X", voice_index, ZeroExtend32(voice.current_address));
            }
        }
    }

    // apply per-channel volume
    const s32 left = ApplyVolume(volume, voice.left_volume.current_level);
    const s32 right = ApplyVolume(volume, voice.right_volume.current_level);
    voice.left_volume.Tick();
    voice.right_volume.Tick();

#ifdef SPU_DUMP_ALL_VOICES
    if (s_voice_dump_writers[voice_index])
    {
        const s16 dump_samples[2] = {static_cast<s16>(Clamp16(left)), static_cast<s16>(Clamp16(right))};
        s_voice_dump_writers[voice_index]->WriteFrames(dump_samples, 1);
    }
#endif

    return std::make_tuple(left, right);
}

void SPU::UpdateNoise()
{
    // Dr Hell's noise waveform, implementation borrowed from pcsx-r.
    static constexpr std::array<u8, 64> noise_wave_add = {
        {1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0,
         0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1}};
    static constexpr std::array<u8, 5> noise_freq_add = {{0, 84, 140, 180, 210}};

    const u32 noise_clock = s_SPUCNT.noise_clock;
    const u32 level = (0x8000u >> (noise_clock >> 2)) << 16;

    s_noise_count += 0x10000u + noise_freq_add[noise_clock & 3u];
    if ((s_noise_count & 0xFFFFu) >= noise_freq_add[4])
    {
        s_noise_count += 0x10000;
        s_noise_count -= noise_freq_add[noise_clock & 3u];
    }

    if (s_noise_count < level)
        return;

    s_noise_count %= level;
    s_noise_level = (s_noise_level << 1) | noise_wave_add[(s_noise_level >> 10) & 63u];
}

/************************************************************************/
/* Reverb algorithm from Mednafen-PSX                                   */
/************************************************************************/

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

ALWAYS_INLINE static s32 Reverb4422(const s16* src)
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
ALWAYS_INLINE static s32 Reverb2244(const s16* src)
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

ALWAYS_INLINE static s16 ReverbSat(s32 val)
{
    return static_cast<s16>(std::clamp<s32>(val, -0x8000, 0x7FFF));
}

ALWAYS_INLINE static s16 ReverbNeg(s16 samp)
{
    if (samp == -32768)
        return 0x7FFF;

    return -samp;
}

ALWAYS_INLINE static s32 IIASM(const s16 IIR_ALPHA, const s16 insamp)
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

void SPU::Execute(void* param, TickCount ticks, TickCount ticks_late)
{
    u32 remaining_frames;
    if (g_settings.cpu_overclock_active)
    {
        // (X * D) / N / 768 -> (X * D) / (N * 768)
        const u64 num = (static_cast<u64>(ticks) * g_settings.cpu_overclock_denominator) + static_cast<u32>(s_ticks_carry);
        remaining_frames = static_cast<u32>(num / s_cpu_tick_divider);
        s_ticks_carry = static_cast<TickCount>(num % s_cpu_tick_divider);
    }
    else
    {
        remaining_frames = static_cast<u32>((ticks + s_ticks_carry) / SYSCLK_TICKS_PER_SPU_TICK);
        s_ticks_carry = (ticks + s_ticks_carry) % SYSCLK_TICKS_PER_SPU_TICK;
    }

    AudioStream* output_stream = s_audio_output_muted ? s_null_audio_stream.get() : s_audio_stream.get();

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

            if (!s_SPUCNT.mute_n)
            {
                left_sum = 0;
                right_sum = 0;
            }

            // Update noise once per frame.
            UpdateNoise();

            // Mix in CD audio.
            const auto [cd_audio_left, cd_audio_right] = CDROM::GetAudioFrame();
            if (s_SPUCNT.cd_audio_enable)
            {
                const s32 cd_audio_volume_left = ApplyVolume(s32(cd_audio_left), s_cd_audio_volume_left);
                const s32 cd_audio_volume_right = ApplyVolume(s32(cd_audio_right), s_cd_audio_volume_right);

                left_sum += cd_audio_volume_left;
                right_sum += cd_audio_volume_right;

                if (s_SPUCNT.cd_audio_reverb)
                {
                    reverb_in_left += cd_audio_volume_left;
                    reverb_in_right += cd_audio_volume_right;
                }
            }

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
            WriteToCaptureBuffer(0, cd_audio_left);
            WriteToCaptureBuffer(1, cd_audio_right);
            WriteToCaptureBuffer(2, static_cast<s16>(Clamp16(s_voices[1].last_volume)));
            WriteToCaptureBuffer(3, static_cast<s16>(Clamp16(s_voices[3].last_volume)));
            IncrementCaptureBufferPosition();

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

        if (s_dump_writer)
            s_dump_writer->WriteFrames(output_frame_start, frames_in_this_batch);

        output_stream->EndWrite(frames_in_this_batch);
        remaining_frames -= frames_in_this_batch;
    }
}

void SPU::UpdateEventInterval()
{
    // Don't generate more than the audio buffer since in a single slice, otherwise we'll both overflow the buffers when
    // we do write it, and the audio thread will underflow since it won't have enough data it the game isn't messing with
    // the SPU state.
    const u32 max_slice_frames = s_audio_stream->GetBufferSize();

    // TODO: Make this predict how long until the interrupt will be hit instead...
    const u32 interval = (s_SPUCNT.enable && s_SPUCNT.irq9_enable) ? 1 : max_slice_frames;
    const TickCount interval_ticks = static_cast<TickCount>(interval) * s_cpu_ticks_per_spu_tick;
    if (s_tick_event->IsActive() && s_tick_event->GetInterval() == interval_ticks)
        return;

    // Ensure all pending ticks have been executed, since we won't get them back after rescheduling.
    s_tick_event->InvokeEarly(true);
    s_tick_event->SetInterval(interval_ticks);

    TickCount downcount = interval_ticks;
    if (!g_settings.cpu_overclock_active)
        downcount -= s_ticks_carry;

    s_tick_event->Schedule(downcount);
}

//void SPU::DrawDebugStateWindow()
//{
//    static const ImVec4 active_color{1.0f, 1.0f, 1.0f, 1.0f};
//    static const ImVec4 inactive_color{0.4f, 0.4f, 0.4f, 1.0f};
//    const float framebuffer_scale = Host::GetOSDScale();
//
//    ImGui::SetNextWindowSize(ImVec2(800.0f * framebuffer_scale, 800.0f * framebuffer_scale), ImGuiCond_FirstUseEver);
//    if (!ImGui::Begin("SPU State", nullptr))
//    {
//        ImGui::End();
//        return;
//    }
//
//    // status
//    if (ImGui::CollapsingHeader("Status", ImGuiTreeNodeFlags_DefaultOpen))
//    {
//        static constexpr std::array<const char*, 4> transfer_modes = {
//            {"Transfer Stopped", "Manual Write", "DMA Write", "DMA Read"}};
//        const std::array<float, 6> offsets = {{100.0f * framebuffer_scale, 200.0f * framebuffer_scale,
//                                               300.0f * framebuffer_scale, 420.0f * framebuffer_scale,
//                                               500.0f * framebuffer_scale, 600.0f * framebuffer_scale}};
//
//        ImGui::Text("Control: ");
//        ImGui::SameLine(offsets[0]);
//        ImGui::TextColored(s_SPUCNT.enable ? active_color : inactive_color, "SPU Enable");
//        ImGui::SameLine(offsets[1]);
//        ImGui::TextColored(s_SPUCNT.mute_n ? inactive_color : active_color, "Mute SPU");
//        ImGui::SameLine(offsets[2]);
//        ImGui::TextColored(s_SPUCNT.external_audio_enable ? active_color : inactive_color, "External Audio");
//        ImGui::SameLine(offsets[3]);
//        ImGui::TextColored(s_SPUCNT.ram_transfer_mode != RAMTransferMode::Stopped ? active_color : inactive_color, "%s",
//                           transfer_modes[static_cast<u8>(s_SPUCNT.ram_transfer_mode.GetValue())]);
//
//        ImGui::Text("Status: ");
//        ImGui::SameLine(offsets[0]);
//        ImGui::TextColored(s_SPUSTAT.irq9_flag ? active_color : inactive_color, "IRQ9");
//        ImGui::SameLine(offsets[1]);
//        ImGui::TextColored(s_SPUSTAT.dma_request ? active_color : inactive_color, "DMA Request");
//        ImGui::SameLine(offsets[2]);
//        ImGui::TextColored(s_SPUSTAT.dma_read_request ? active_color : inactive_color, "DMA Read");
//        ImGui::SameLine(offsets[3]);
//        ImGui::TextColored(s_SPUSTAT.dma_write_request ? active_color : inactive_color, "DMA Write");
//        ImGui::SameLine(offsets[4]);
//        ImGui::TextColored(s_SPUSTAT.transfer_busy ? active_color : inactive_color, "Transfer Busy");
//        ImGui::SameLine(offsets[5]);
//        ImGui::TextColored(s_SPUSTAT.second_half_capture_buffer ? active_color : inactive_color, "Second Capture Buffer");
//
//        ImGui::Text("Interrupt: ");
//        ImGui::SameLine(offsets[0]);
//        ImGui::TextColored(s_SPUCNT.irq9_enable ? active_color : inactive_color,
//                           s_SPUCNT.irq9_enable ? "Enabled @ 0x%04X (actual 0x%08X)" : "Disabled @ 0x%04X (actual 0x%08X)",
//                           s_irq_address, (ZeroExtend32(s_irq_address) * 8) & RAM_MASK);
//
//        ImGui::Text("Volume: ");
//        ImGui::SameLine(offsets[0]);
//        ImGui::Text("Left: %d%%", ApplyVolume(100, s_main_volume_left.current_level));
//        ImGui::SameLine(offsets[1]);
//        ImGui::Text("Right: %d%%", ApplyVolume(100, s_main_volume_right.current_level));
//
//        ImGui::Text("CD Audio: ");
//        ImGui::SameLine(offsets[0]);
//        ImGui::TextColored(s_SPUCNT.cd_audio_enable ? active_color : inactive_color,
//                           s_SPUCNT.cd_audio_enable ? "Enabled" : "Disabled");
//        ImGui::SameLine(offsets[1]);
//        ImGui::TextColored(s_SPUCNT.cd_audio_enable ? active_color : inactive_color, "Left Volume: %d%%",
//                           ApplyVolume(100, s_cd_audio_volume_left));
//        ImGui::SameLine(offsets[3]);
//        ImGui::TextColored(s_SPUCNT.cd_audio_enable ? active_color : inactive_color, "Right Volume: %d%%",
//                           ApplyVolume(100, s_cd_audio_volume_left));
//
//        ImGui::Text("Transfer FIFO: ");
//        ImGui::SameLine(offsets[0]);
//        ImGui::TextColored(s_transfer_event->IsActive() ? active_color : inactive_color, "%u halfwords (%u bytes)",
//                           s_transfer_fifo.GetSize(), s_transfer_fifo.GetSize() * 2);
//    }
//
//    // draw voice states
//    if (ImGui::CollapsingHeader("Voice State", ImGuiTreeNodeFlags_DefaultOpen))
//    {
//        static constexpr u32 NUM_COLUMNS = 12;
//
//        ImGui::Columns(NUM_COLUMNS);
//
//        // headers
//        static constexpr std::array<const char*, NUM_COLUMNS> column_titles = {
//            {"#", "InterpIndex", "SampleIndex", "CurAddr", "StartAddr", "RepeatAddr", "SampleRate", "VolLeft", "VolRight",
//             "ADSRPhase", "ADSRVol", "ADSRTicks"}};
//        static constexpr std::array<const char*, 5> adsr_phases = {{"Off", "Attack", "Decay", "Sustain", "Release"}};
//        for (u32 i = 0; i < NUM_COLUMNS; i++)
//        {
//            ImGui::TextUnformatted(column_titles[i]);
//            ImGui::NextColumn();
//        }
//
//        // states
//        for (u32 voice_index = 0; voice_index < NUM_VOICES; voice_index++)
//        {
//            const Voice& v = s_voices[voice_index];
//            ImVec4 color = v.IsOn() ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
//            ImGui::TextColored(color, "%u", ZeroExtend32(voice_index));
//            ImGui::NextColumn();
//            if (IsVoiceNoiseEnabled(voice_index))
//                ImGui::TextColored(color, "NOISE");
//            else
//                ImGui::TextColored(color, "%u", ZeroExtend32(v.counter.interpolation_index.GetValue()));
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%u", ZeroExtend32(v.counter.sample_index.GetValue()));
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%04X", ZeroExtend32(v.current_address));
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%04X", ZeroExtend32(v.regs.adpcm_start_address));
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%04X", ZeroExtend32(v.regs.adpcm_repeat_address));
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%.2f", (float(v.regs.adpcm_sample_rate) / 4096.0f) * 44100.0f);
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%d%%", ApplyVolume(100, v.left_volume.current_level));
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%d%%", ApplyVolume(100, v.right_volume.current_level));
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%s", adsr_phases[static_cast<u8>(v.adsr_phase)]);
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%d%%", ApplyVolume(100, v.regs.adsr_volume));
//            ImGui::NextColumn();
//            ImGui::TextColored(color, "%d", v.adsr_envelope.counter);
//            ImGui::NextColumn();
//        }
//
//        ImGui::Columns(1);
//    }
//
//    if (ImGui::CollapsingHeader("Reverb", ImGuiTreeNodeFlags_DefaultOpen))
//    {
//        ImGui::TextColored(s_SPUCNT.reverb_master_enable ? active_color : inactive_color, "Master Enable: %s",
//                           s_SPUCNT.reverb_master_enable ? "Yes" : "No");
//        ImGui::Text("Voices Enabled: ");
//
//        for (u32 i = 0; i < NUM_VOICES; i++)
//        {
//            ImGui::SameLine(0.0f, 16.0f);
//
//            const bool active = IsVoiceReverbEnabled(i);
//            ImGui::TextColored(active ? active_color : inactive_color, "%u", i);
//        }
//
//        ImGui::TextColored(s_SPUCNT.cd_audio_reverb ? active_color : inactive_color, "CD Audio Enable: %s",
//                           s_SPUCNT.cd_audio_reverb ? "Yes" : "No");
//
//        ImGui::TextColored(s_SPUCNT.external_audio_reverb ? active_color : inactive_color, "External Audio Enable: %s",
//                           s_SPUCNT.external_audio_reverb ? "Yes" : "No");
//
//        ImGui::Text("Base Address: 0x%08X (%04X)", s_reverb_base_address, s_reverb_registers.mBASE);
//        ImGui::Text("Current Address: 0x%08X", s_reverb_current_address);
//        ImGui::Text("Current Amplitude: Input (%d, %d) Output (%d, %d)", s_last_reverb_input[0], s_last_reverb_input[1],
//                    s_last_reverb_output[0], s_last_reverb_output[1]);
//        ImGui::Text("Output Volume: Left %d%% Right %d%%", ApplyVolume(100, s_reverb_registers.vLOUT),
//                    ApplyVolume(100, s_reverb_registers.vROUT));
//
//        ImGui::Text("Pitch Modulation: ");
//        for (u32 i = 1; i < NUM_VOICES; i++)
//        {
//            ImGui::SameLine(0.0f, 16.0f);
//
//            const bool active = IsPitchModulationEnabled(i);
//            ImGui::TextColored(active ? active_color : inactive_color, "%u", i);
//        }
//    }
//
//    if (ImGui::CollapsingHeader("Hacks", ImGuiTreeNodeFlags_DefaultOpen))
//    {
//        if (ImGui::Button("Key Off All Voices"))
//        {
//            for (u32 i = 0; i < NUM_VOICES; i++)
//            {
//                s_voices[i].KeyOff();
//                s_voices[i].adsr_envelope.counter = 0;
//                s_voices[i].regs.adsr_volume = 0;
//            }
//        }
//    }
//
//    ImGui::End();
//}