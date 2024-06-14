#pragma once

#include "Reverb.hpp"
#include <algorithm>
#include <array>

namespace SPU {
#pragma warning(disable : 4244)

//////////////////////////
// Duckstation's reverb
static const u32 NUM_REVERB_REGS = 32;
void ProcessReverb(s16 left_in, s16 right_in, s32* left_out, s32* right_out);
static s16 ReverbRead(u32 address, s32 offset = 0);

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
        } a;

        u16 rev[NUM_REVERB_REGS];
    };
};

// bit mask of voices with reverb
static const u32 RAM_SIZE = 512 * 1024;
static std::array<u8, RAM_SIZE> s_ram{};
//static u32 s_reverb_on_register = 0;
static u32 s_reverb_base_address = 0;
static u32 s_reverb_current_address = 0;
static ReverbRegisters s_reverb_registers{};
static std::array<std::array<s16, 128>, 2> s_reverb_downsample_buffer;
static std::array<std::array<s16, 64>, 2> s_reverb_upsample_buffer;
static s32 s_reverb_resample_buffer_position = 0;


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

u32 ReverbMemoryAddress(u32 address)
{
    // Ensures address does not leave the reverb work area.
    static constexpr u32 MASK = (RAM_SIZE - 1) / 2;
    u32 offset = s_reverb_current_address + (address & MASK);
    offset += s_reverb_base_address & ((s32) (offset << 13) >> 31);

    // We address RAM in bytes. TODO: Change this to words.
    return (offset & MASK) * 2u;
}

s16 ReverbRead(u32 address, s32 offset)
{
    // TODO: This should check interrupts.
    const u32 real_address = ReverbMemoryAddress((address << 2) + offset);

    s16 data;
    std::memcpy(&data, &s_ram[real_address], sizeof(data));
    return data;
}

void ReverbWrite(u32 address, s16 data)
{
    // TODO: This should check interrupts.
    const u32 real_address = ReverbMemoryAddress(address << 2);
    std::memcpy(&s_ram[real_address], &data, sizeof(data));
}

static constexpr s32 ApplyVolume(s32 sample, s16 volume)
{
    return (sample * s32(volume)) >> 15;
}

void ReverbDuckstation::ChangeVolume(u8 left, u8 right)
{
    left;
    right;
    // Not implemented
}

void ReverbDuckstation::ChangeSetting(u8 setting)
{
    setting;
    // Not implemented
}

std::pair<f32, f32> ReverbDuckstation::Process(f32 _left_in, f32 _right_in)
{
    s16 left_in = (s16) _left_in;
    s16 right_in = (s16) _right_in;

    s_last_reverb_input[0] = left_in;
    s_last_reverb_input[1] = right_in;
    s_reverb_downsample_buffer[0][s_reverb_resample_buffer_position | 0x00] = left_in;
    s_reverb_downsample_buffer[0][s_reverb_resample_buffer_position | 0x40] = left_in;
    s_reverb_downsample_buffer[1][s_reverb_resample_buffer_position | 0x00] = right_in;
    s_reverb_downsample_buffer[1][s_reverb_resample_buffer_position | 0x40] = right_in;
    std::array<std::array<s16, 128>, 2> test;

    // these values seem to be stable in duckstation - move them to be set once
    s_reverb_registers.vLOUT = 4128;
    s_reverb_registers.vROUT = 4128;
    s_reverb_registers.mBASE = 61956;
    s_reverb_registers.a.IIR_SRC_A[0] = 2905;
    s_reverb_registers.a.IIR_SRC_A[1] = 2266;
    s_reverb_registers.a.IIR_COEF = -22912;
    s_reverb_registers.a.IN_COEF[0] = -32768;
    s_reverb_registers.a.IN_COEF[1] = -32768;
    s_reverb_registers.a.IIR_SRC_B[0] = 1514;
    s_reverb_registers.a.IIR_SRC_B[1] = 797;
    s_reverb_registers.a.IIR_ALPHA = 28512;
    s_reverb_registers.a.IIR_DEST_A[0] = 3579;
    s_reverb_registers.a.IIR_DEST_A[1] = 2904;
    s_reverb_registers.a.IIR_DEST_B[0] = 2265;
    s_reverb_registers.a.IIR_DEST_B[1] = 1513;
    s_reverb_registers.a.ACC_SRC_A[0] = 3337;
    s_reverb_registers.a.ACC_SRC_A[1] = 2620;
    s_reverb_registers.a.ACC_SRC_B[0] = 3033;
    s_reverb_registers.a.ACC_SRC_B[1] = 2419;
    s_reverb_registers.a.ACC_SRC_C[0] = 2028;
    s_reverb_registers.a.ACC_SRC_C[1] = 1200;
    s_reverb_registers.a.ACC_SRC_D[0] = 1775;
    s_reverb_registers.a.ACC_SRC_D[1] = 978;
    s_reverb_registers.a.ACC_COEF_A = 20392;
    s_reverb_registers.a.ACC_COEF_B = -17184;
    s_reverb_registers.a.ACC_COEF_C = 17680;
    s_reverb_registers.a.ACC_COEF_D = -16656;
    s_reverb_registers.a.MIX_DEST_A[0] = 796;
    s_reverb_registers.a.MIX_DEST_A[1] = 568;
    s_reverb_registers.a.MIX_DEST_B[0] = 340;
    s_reverb_registers.a.MIX_DEST_B[1] = 170;
    s_reverb_registers.a.FB_SRC_A = 227;
    s_reverb_registers.a.FB_SRC_B = 169;
    s_reverb_registers.a.FB_ALPHA = 22144;
    s_reverb_registers.a.FB_X = 21184;


    test = s_reverb_downsample_buffer;
    //std::cout << "L:" << left_in << " R:" << right_in << std::endl;
    //std::cout << s_reverb_registers.a.IIR_SRC_A[0] << std::endl;

    s32 out[2];
    if (s_reverb_resample_buffer_position & 1u)
    {
        //std::cout << "here1" << std::endl;
        std::array<s32, 2> downsampled;
        for (unsigned lr = 0; lr < 2; lr++)
            downsampled[lr] = Reverb4422(&s_reverb_downsample_buffer[lr][(s_reverb_resample_buffer_position - 38) & 0x3F]);

        for (unsigned lr = 0; lr < 2; lr++)
        {
            //if (s_SPUCNT.a.reverb_master_enable)
            {
                const s16 IIR_INPUT_A = ReverbSat((((ReverbRead(s_reverb_registers.a.IIR_SRC_A[lr ^ 0]) * s_reverb_registers.a.IIR_COEF) >> 14) + ((downsampled[lr] * s_reverb_registers.a.IN_COEF[lr]) >> 14)) >> 1);
                const s16 IIR_INPUT_B = ReverbSat((((ReverbRead(s_reverb_registers.a.IIR_SRC_B[lr ^ 1]) * s_reverb_registers.a.IIR_COEF) >> 14) + ((downsampled[lr] * s_reverb_registers.a.IN_COEF[lr]) >> 14)) >> 1);
                const s16 IIR_A = ReverbSat((((IIR_INPUT_A * s_reverb_registers.a.IIR_ALPHA) >> 14) + (IIASM(s_reverb_registers.a.IIR_ALPHA, ReverbRead(s_reverb_registers.a.IIR_DEST_A[lr], -1)) >> 14)) >> 1);
                const s16 IIR_B = ReverbSat((((IIR_INPUT_B * s_reverb_registers.a.IIR_ALPHA) >> 14) + (IIASM(s_reverb_registers.a.IIR_ALPHA, ReverbRead(s_reverb_registers.a.IIR_DEST_B[lr], -1)) >> 14)) >> 1);
                ReverbWrite(s_reverb_registers.a.IIR_DEST_A[lr], IIR_A);
                ReverbWrite(s_reverb_registers.a.IIR_DEST_B[lr], IIR_B);
            }

            const s32 ACC = ((ReverbRead(s_reverb_registers.a.ACC_SRC_A[lr]) * s_reverb_registers.a.ACC_COEF_A) >> 14) + ((ReverbRead(s_reverb_registers.a.ACC_SRC_B[lr]) * s_reverb_registers.a.ACC_COEF_B) >> 14) + ((ReverbRead(s_reverb_registers.a.ACC_SRC_C[lr]) * s_reverb_registers.a.ACC_COEF_C) >> 14) + ((ReverbRead(s_reverb_registers.a.ACC_SRC_D[lr]) * s_reverb_registers.a.ACC_COEF_D) >> 14);

            const s16 FB_A = ReverbRead(s_reverb_registers.a.MIX_DEST_A[lr] - s_reverb_registers.a.FB_SRC_A);
            const s16 FB_B = ReverbRead(s_reverb_registers.a.MIX_DEST_B[lr] - s_reverb_registers.a.FB_SRC_B);
            const s16 MDA = ReverbSat((ACC + ((FB_A * ReverbNeg(s_reverb_registers.a.FB_ALPHA)) >> 14)) >> 1);
            const s16 MDB = ReverbSat(
                FB_A + ((((MDA * s_reverb_registers.a.FB_ALPHA) >> 14) + ((FB_B * ReverbNeg(s_reverb_registers.a.FB_X)) >> 14)) >> 1));
            const s16 IVB = ReverbSat(FB_B + ((MDB * s_reverb_registers.a.FB_X) >> 15));

            //if (s_SPUCNT.reverb_master_enable)
            {
                ReverbWrite(s_reverb_registers.a.MIX_DEST_A[lr], MDA);
                ReverbWrite(s_reverb_registers.a.MIX_DEST_B[lr], MDB);
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
        //std::cout << "here2" << std::endl;
        for (unsigned lr = 0; lr < 2; lr++)
            out[lr] = Reverb2244<true>(&s_reverb_upsample_buffer[lr][((s_reverb_resample_buffer_position >> 1) - 19) & 0x1F]);
    }

    s_reverb_resample_buffer_position = (s_reverb_resample_buffer_position + 1) & 0x3F;

    s32 left_out;
    s32 right_out;
    s_last_reverb_output[0] = left_out = ApplyVolume(out[0], s_reverb_registers.vLOUT);
    s_last_reverb_output[1] = right_out = ApplyVolume(out[1], s_reverb_registers.vROUT);

    return std::pair<f32, f32>(left_out, right_out);
}


//////////////////////////
// Illeprih's reverb
#pragma warning(disable : 4838)
#pragma warning(disable : 4309)
const ReverbType ReverbIlleprih::reverbTypes[10] = {
    {// OFF
     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
     0x0000, 0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
     0x0000, 0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000},
    {// ROOM
     0x007D, 0x005B, 0x6D80, 0x54B8, 0xBED0, 0x0000, 0x0000, 0xBA80,
     0x5800, 0x5300, 0x04D6, 0x0333, 0x03F0, 0x0227, 0x0374, 0x01EF,
     0x0334, 0x01B5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x01B4, 0x0136, 0x00B8, 0x005C, 0x8000, 0x8000},
    {// STUDIO_A (small)
     0x0033, 0x0025, 0x70F0, 0x4FA8, 0xBCE0, 0x4410, 0xC0F0, 0x9C00,
     0x5280, 0x4EC0, 0x03E4, 0x031B, 0x03A4, 0x02AF, 0x0372, 0x0266,
     0x031C, 0x025D, 0x025C, 0x018E, 0x022F, 0x0135, 0x01D2, 0x00B7,
     0x018F, 0x00B5, 0x00B4, 0x0080, 0x004C, 0x0026, 0x8000, 0x8000},
    {// STUDIO_B (medium)
     0x00B1, 0x007F, 0x70F0, 0x4FA8, 0xBCE0, 0x4510, 0xBEF0, 0xB4C0,
     0x5280, 0x4EC0, 0x0904, 0x076B, 0x0824, 0x065F, 0x07A2, 0x0616,
     0x076C, 0x05ED, 0x05EC, 0x042E, 0x050F, 0x0305, 0x0462, 0x02B7,
     0x042F, 0x0265, 0x0264, 0x01B2, 0x0100, 0x0080, 0x8000, 0x8000},
    {// STUDIO_C (large)
     0x00E3, 0x00A9, 0x6F60, 0x4FA8, 0xBCE0, 0x4510, 0xBEF0, 0xA680,
     0x5680, 0x52C0, 0x0DFB, 0x0B58, 0x0D09, 0x0A3C, 0x0BD9, 0x0973,
     0x0B59, 0x08DA, 0x08D9, 0x05E9, 0x07EC, 0x04B0, 0x06EF, 0x03D2,
     0x05EA, 0x031D, 0x031C, 0x0238, 0x0154, 0x00AA, 0x8000, 0x8000},
    {// HALL
     0x01A5, 0x0139, 0x6000, 0x5000, 0x4C00, 0xB800, 0xBC00, 0xC000,
     0x6000, 0x5C00, 0x15BA, 0x11BB, 0x14C2, 0x10BD, 0x11BC, 0x0DC1,
     0x11C0, 0x0DC3, 0x0DC0, 0x09C1, 0x0BC4, 0x07C1, 0x0A00, 0x06CD,
     0x09C2, 0x05C1, 0x05C0, 0x041A, 0x0274, 0x013A, 0x8000, 0x8000},
    {// SPACE
     0x033D, 0x0231, 0x7E00, 0x5000, 0xB400, 0xB000, 0x4C00, 0xB000,
     0x6000, 0x5400, 0x1ED6, 0x1A31, 0x1D14, 0x183B, 0x1BC2, 0x16B2,
     0x1A32, 0x15EF, 0x15EE, 0x1055, 0x1334, 0x0F2D, 0x11F6, 0x0C5D,
     0x1056, 0x0AE1, 0x0AE0, 0x07A2, 0x0464, 0x0232, 0x8000, 0x8000},
    {// ECHO
     0x0001, 0x0001, 0x7FFF, 0x7FFF, 0x0000, 0x0000, 0x0000, 0x8100,
     0x0000, 0x0000, 0x1FFF, 0x0FFF, 0x1005, 0x0005, 0x0000, 0x0000,
     0x1005, 0x0005, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x1004, 0x1002, 0x0004, 0x0002, 0x8000, 0x8000},
    {// DELAY
     0x0001, 0x0001, 0x7FFF, 0x7FFF, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x1FFF, 0x0FFF, 0x1005, 0x0005, 0x0000, 0x0000,
     0x1005, 0x0005, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
     0x0000, 0x0000, 0x1004, 0x1002, 0x0004, 0x0002, 0x8000, 0x8000},
    {// PIPE
     0x0017, 0x0013, 0x70F0, 0x4FA8, 0xBCE0, 0x4510, 0xBEF0, 0x8500,
     0x5F80, 0x54C0, 0x0371, 0x02AF, 0x02E5, 0x01DF, 0x02B0, 0x01D7,
     0x0358, 0x026A, 0x01D6, 0x011E, 0x012D, 0x00B1, 0x011F, 0x0059,
     0x01A0, 0x00E3, 0x0058, 0x0040, 0x0028, 0x0014, 0x8000, 0x8000}};

ReverbIlleprih::ReverbIlleprih()
{
    this->ChangeSetting(4);
    // TODO look into the acual code to find the proper reverb volume to be used
    this->ChangeVolume(0x18, 0x18);
}

std::pair<f32, f32> ReverbIlleprih::Process(f32 left, f32 right)
{
    // TODO consider diving input to fit the -1.0f - 1.0f range and then multiplying back to s16 range at the end. But it shouldn't make any difference.

    // Input from mixer
    const float Lin = this->currentSetting.vLIN * left;
    const float Rin = this->currentSetting.vRIN * right;

    // TODO Verify the -1 is actually correct in this case. Looks like most emulators might just get it wrong. Off addresses have 0001h there so that the value isn't negative, however, it's being << 3, so should't 8 be subtracted here??
    // Maybe the value is there just to 0 fill the work area and it's not really necessary and getting the previous address is intended.

    // Same side reflection L->L and R->R
    float mlSame = SaturateSample((Lin + LoadReverb(this->currentSetting.dLSAME) * this->currentSetting.vWALL - LoadReverb(this->currentSetting.mLSAME - 1)) * this->currentSetting.vIIR + LoadReverb(this->currentSetting.mLSAME - 1));
    float mrSame = SaturateSample((Rin + LoadReverb(this->currentSetting.dRSAME) * this->currentSetting.vWALL - LoadReverb(this->currentSetting.mRSAME - 1)) * this->currentSetting.vIIR + LoadReverb(this->currentSetting.mRSAME - 1));

    WriteReverb(this->currentSetting.mLSAME, mlSame);
    WriteReverb(this->currentSetting.mRSAME, mrSame);

    // Different side reflection L->R and R->L
    float mlDiff = SaturateSample((Lin + LoadReverb(this->currentSetting.dRDIFF) * this->currentSetting.vWALL - LoadReverb(this->currentSetting.mLDIFF - 1)) * this->currentSetting.vIIR + LoadReverb(this->currentSetting.mLDIFF - 1));
    float mrDiff = SaturateSample((Rin + LoadReverb(this->currentSetting.dLDIFF) * this->currentSetting.vWALL - LoadReverb(this->currentSetting.mRDIFF - 1)) * this->currentSetting.vIIR + LoadReverb(this->currentSetting.mRDIFF - 1));

    WriteReverb(this->currentSetting.mLDIFF, mlDiff);
    WriteReverb(this->currentSetting.mRDIFF, mrDiff);

    // Early echo (comb filter with input from buffer)
    float l = SaturateSample(this->currentSetting.vCOMB1 * LoadReverb(this->currentSetting.mLCOMB1) + this->currentSetting.vCOMB2 * LoadReverb(this->currentSetting.mLCOMB2) + this->currentSetting.vCOMB3 * LoadReverb(this->currentSetting.mLCOMB3) + this->currentSetting.vCOMB4 * LoadReverb(this->currentSetting.mLCOMB4));
    float r = SaturateSample(this->currentSetting.vCOMB1 * LoadReverb(this->currentSetting.mRCOMB1) + this->currentSetting.vCOMB2 * LoadReverb(this->currentSetting.mRCOMB2) + this->currentSetting.vCOMB3 * LoadReverb(this->currentSetting.mRCOMB3) + this->currentSetting.vCOMB4 * LoadReverb(this->currentSetting.mRCOMB4));

    // Late reverb APF1 (All pass filter 1 with input from COMB)
    l = SaturateSample(l - SaturateSample(this->currentSetting.vAPF1 * LoadReverb(this->currentSetting.mLAPF1 - this->currentSetting.dAPF1)));
    r = SaturateSample(r - SaturateSample(this->currentSetting.vAPF1 * LoadReverb(this->currentSetting.mRAPF1 - this->currentSetting.dAPF1)));

    WriteReverb(this->currentSetting.mLAPF1, l);
    WriteReverb(this->currentSetting.mRAPF1, r);

    l = SaturateSample(l * this->currentSetting.vAPF1 + LoadReverb(this->currentSetting.mLAPF1 - this->currentSetting.dAPF1));
    r = SaturateSample(r * this->currentSetting.vAPF1 + LoadReverb(this->currentSetting.mRAPF1 - this->currentSetting.dAPF1));

    // Late reverb APF2 (All pass filter 2 with input from APF1)
    l = SaturateSample(l - SaturateSample(this->currentSetting.vAPF2 * LoadReverb(this->currentSetting.mLAPF2 - this->currentSetting.dAPF2)));
    r = SaturateSample(r - SaturateSample(this->currentSetting.vAPF2 * LoadReverb(this->currentSetting.mRAPF2 - this->currentSetting.dAPF2)));

    WriteReverb(this->currentSetting.mLAPF2, l);
    WriteReverb(this->currentSetting.mRAPF2, r);

    l = SaturateSample(l * this->currentSetting.vAPF2 + LoadReverb(this->currentSetting.mLAPF2 - this->currentSetting.dAPF2));
    r = SaturateSample(r * this->currentSetting.vAPF2 + LoadReverb(this->currentSetting.mRAPF2 - this->currentSetting.dAPF2));

    l *= volumeLeft;
    r *= volumeRight;

    // Overflow here is intended. It wraps to the start of the workArea due to it's size being max value of u16 + 1;
    this->currentAddress++;

    return std::pair<f32, f32>(l, r);
}

float ReverbIlleprih::LoadReverb(u16 address)
{
    return this->workArea[(u16)(this->currentAddress + address)];
}

void ReverbIlleprih::WriteReverb(u16 address, float value)
{
    this->workArea[(u16)(currentAddress + address)] = value;
}

// TODO consider removing this. It shouldn't cause any overflow issues and should make reverb
// slightly less fuzzy in the edge cases
float ReverbIlleprih::SaturateSample(float sample)
{
    return std::clamp(sample, -32768.0f, 32767.0f);
}

// PSX is doing << 8 to get it to the s16 range, so we can just div by 128
void ReverbIlleprih::ChangeVolume(u8 _volumeLeft, u8 _volumeRight)
{
    this->volumeLeft = _volumeLeft / 128.0f;
    this->volumeRight = _volumeRight / 128.0f;
}


// PSX << 3 these to get the address into a s16 array. This would mean we only have to << 2,
// but since it's setup to tick at 44_100 Hz instead of 22_050 Hz, the area has to be doubled
// and so do the addresses, to get back the echo at the correct time.
void ReverbIlleprih::ChangeSetting(u8 reverbIndex)
{
    const ReverbType reverbType = this->reverbTypes[reverbIndex];

    this->currentSetting.dAPF1 = reverbType.dAPF1 << 3;
    this->currentSetting.dAPF2 = reverbType.dAPF2 << 3;
    this->currentSetting.vIIR = reverbType.vIIR / 32768.0f;
    this->currentSetting.vCOMB1 = reverbType.vCOMB1 / 32768.0f;
    this->currentSetting.vCOMB2 = reverbType.vCOMB2 / 32768.0f;
    this->currentSetting.vCOMB3 = reverbType.vCOMB3 / 32768.0f;
    this->currentSetting.vCOMB4 = reverbType.vCOMB4 / 32768.0f;
    this->currentSetting.vWALL = reverbType.vWALL / 32768.0f;
    this->currentSetting.vAPF1 = reverbType.vAPF1 / 32768.0f;
    this->currentSetting.vAPF2 = reverbType.vAPF2 / 32768.0f;
    this->currentSetting.mLSAME = reverbType.mLSAME << 3;
    this->currentSetting.mRSAME = reverbType.mRSAME << 3;
    this->currentSetting.mLCOMB1 = reverbType.mLCOMB1 << 3;
    this->currentSetting.mRCOMB1 = reverbType.mRCOMB1 << 3;
    this->currentSetting.mLCOMB2 = reverbType.mLCOMB2 << 3;
    this->currentSetting.mRCOMB2 = reverbType.mRCOMB2 << 3;
    this->currentSetting.dLSAME = reverbType.dLSAME << 3;
    this->currentSetting.dRSAME = reverbType.dRSAME << 3;
    this->currentSetting.mLDIFF = reverbType.mLDIFF << 3;
    this->currentSetting.mRDIFF = reverbType.mRDIFF << 3;
    this->currentSetting.mLCOMB3 = reverbType.mLCOMB3 << 3;
    this->currentSetting.mRCOMB3 = reverbType.mRCOMB3 << 3;
    this->currentSetting.mLCOMB4 = reverbType.mLCOMB4 << 3;
    this->currentSetting.mRCOMB4 = reverbType.mRCOMB4 << 3;
    this->currentSetting.dLDIFF = reverbType.dLDIFF << 3;
    this->currentSetting.dRDIFF = reverbType.dRDIFF << 3;
    this->currentSetting.mLAPF1 = reverbType.mLAPF1 << 3;
    this->currentSetting.mRAPF1 = reverbType.mRAPF1 << 3;
    this->currentSetting.mLAPF2 = reverbType.mLAPF2 << 3;
    this->currentSetting.mRAPF2 = reverbType.mRAPF2 << 3;
    this->currentSetting.vLIN = reverbType.vLIN / 32768.0f;
    this->currentSetting.vRIN = reverbType.vRIN / 32768.0f;
}

} // namespace SPU
