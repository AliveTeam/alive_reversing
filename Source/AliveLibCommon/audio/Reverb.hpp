#pragma once

#include <Types.hpp>

namespace SPU {

class Reverb
{
public:
    virtual std::pair<f32, f32> Process(f32 left, f32 right) = 0;
    virtual void ChangeVolume(u8 left, u8 right) = 0;
    virtual void ChangeSetting(u8 setting) = 0;
};

///////////////////////
// Duckstation
class ReverbDuckstation : public Reverb
{
public:
    std::pair<f32, f32> Process(f32 left, f32 right) override;
    void ChangeVolume(u8 left, u8 right) override;
    void ChangeSetting(u8 setting) override;
};

///////////////////////
// Illeprih
struct ReverbSetting
{
    u16 dAPF1;
    u16 dAPF2;
    f32 vIIR;
    f32 vCOMB1;
    f32 vCOMB2;
    f32 vCOMB3;
    f32 vCOMB4;
    f32 vWALL;
    f32 vAPF1;
    f32 vAPF2;
    u16 mLSAME;
    u16 mRSAME;
    u16 mLCOMB1;
    u16 mRCOMB1;
    u16 mLCOMB2;
    u16 mRCOMB2;
    u16 dLSAME;
    u16 dRSAME;
    u16 mLDIFF;
    u16 mRDIFF;
    u16 mLCOMB3;
    u16 mRCOMB3;
    u16 mLCOMB4;
    u16 mRCOMB4;
    u16 dLDIFF;
    u16 dRDIFF;
    u16 mLAPF1;
    u16 mRAPF1;
    u16 mLAPF2;
    u16 mRAPF2;
    f32 vLIN;
    f32 vRIN;
};

struct ReverbType
{
    u16 dAPF1;
    u16 dAPF2;
    s16 vIIR;
    s16 vCOMB1;
    s16 vCOMB2;
    s16 vCOMB3;
    s16 vCOMB4;
    s16 vWALL;
    s16 vAPF1;
    s16 vAPF2;
    u16 mLSAME;
    u16 mRSAME;
    u16 mLCOMB1;
    u16 mRCOMB1;
    u16 mLCOMB2;
    u16 mRCOMB2;
    u16 dLSAME;
    u16 dRSAME;
    u16 mLDIFF;
    u16 mRDIFF;
    u16 mLCOMB3;
    u16 mRCOMB3;
    u16 mLCOMB4;
    u16 mRCOMB4;
    u16 dLDIFF;
    u16 dRDIFF;
    u16 mLAPF1;
    u16 mRAPF1;
    u16 mLAPF2;
    u16 mRAPF2;
    s16 vLIN;
    s16 vRIN;
};

class ReverbIlleprih : public Reverb
{
public:
    ReverbIlleprih();
    std::pair<f32, f32> Process(f32 left, f32 right) override;
    void ChangeVolume(u8 left, u8 right) override;
    void ChangeSetting(u8 setting) override;

private:
    // This is max value of u16 + 1. Making it so overflow can handle the looping
    static const u32 workAreaSize = 0x10000;
    float workArea[workAreaSize] = {0.0f};
    u16 currentAddress = 0;
    static const ReverbType reverbTypes[10];
    ReverbSetting currentSetting;
    float volumeLeft = 0.0f;
    float volumeRight = 0.0f;

    float LoadReverb(u16 address);
    void WriteReverb(u16 address, float value);
    float SaturateSample(float sample);
};

}
