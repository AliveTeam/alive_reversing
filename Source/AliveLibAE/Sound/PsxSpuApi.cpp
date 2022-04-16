#include "stdafx.h"
#include "PsxSpuApi.hpp"
#include "Function.hpp"
#include "Io.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Sound.hpp"    // SoundEntry structure
#include "Sys.hpp"      // SYS_GetTicks
#include "PathData.hpp" // SoundBlockInfo, SeqPathDataRecord
#include "../AliveLibAE/Io.hpp"
#include <assert.h>

struct VagAtr final
{
    s8 field_0_priority;
    s8 field_1_mode;
    s8 field_2_vol;
    s8 field_3_pan;
    u8 field_4_centre;
    u8 field_5_shift;
    s8 field_6_min;
    s8 field_7_max;
    s8 field_8_vibW;
    s8 field_9_vibT;
    s8 field_A_porW;
    s8 field_B_porT;
    s8 field_C_pitch_bend_min;
    s8 field_D_pitch_bend_max;
    s8 field_E_reserved1;
    s8 field_F_reserved2;
    s16 field_10_adsr1;
    s16 field_12_adsr2;
    s16 field_14_prog;
    s16 field_16_vag;
    s16 field_18_reserved[4];
};



#pragma pack(push)
#pragma pack(1)
struct SeqHeader final
{
    s32 field_0_magic;
    u32 field_4_version;
    u16 field_8_resolution_of_quater_note;
    u8 field_A_tempo[3];
    // No padding byte here, hence 1 byte packing enabled
    u8 field_D_time_signature_bars;
    u8 field_E_time_signature_beats;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(SeqHeader, 0xF);



ALIVE_VAR(1, 0xBD1CDE, s16, sGlobalVolumeLevel_right_BD1CDE, 0);
ALIVE_VAR(1, 0xBD1CDC, s16, sGlobalVolumeLevel_left_BD1CDC, 0);
ALIVE_VAR(1, 0xC13180, VabUnknown, s512_byte_C13180, {});
ALIVE_ARY(1, 0xBE6144, u8, kMaxVabs, sVagCounts_BE6144, {});
ALIVE_ARY(1, 0x0BDCD64, u8, kMaxVabs, sProgCounts_BDCD64, {});
ALIVE_ARY(1, 0xC13160, VabHeader*, 4, spVabHeaders_C13160, {});
ALIVE_VAR(1, 0xBEF160, ConvertedVagTable, sConvertedVagTable_BEF160, {});
ALIVE_VAR(1, 0xBE6160, SoundEntryTable, sSoundEntryTable16_BE6160, {});
ALIVE_VAR(1, 0xC14080, MidiChannels, sMidi_Channels_C14080, {});
ALIVE_VAR(1, 0xC13400, MidiSeqSongsTable, sMidiSeqSongs_C13400, {});
ALIVE_VAR(1, 0xbd1cf4, s32, sMidi_Inited_dword_BD1CF4, 0);
ALIVE_VAR(1, 0xbd1cec, u32, sMidiTime_BD1CEC, 0);
ALIVE_VAR(1, 0xbd1ce8, Bool32, sSoundDatIsNull_BD1CE8, 1);
ALIVE_VAR(1, 0xbd1ce4, s8, sbDisableSeqs_BD1CE4, 0);
ALIVE_VAR(1, 0x578E20, u32, sLastTime_578E20, 0xFFFFFFFF);
ALIVE_VAR(1, 0xbd1cf0, u32, sMidi_WaitUntil_BD1CF0, 0);
ALIVE_VAR(1, 0xbd1ce0, IO_FileHandleType, sSoundDatFileHandle_BD1CE0, nullptr);
ALIVE_VAR(1, 0xbd1cfc, u8, sControllerValue_BD1CFC, 0);


class AEPsxSpuApiVars final : public IPsxSpuApiVars
{
public:
    virtual s16& sGlobalVolumeLevel_right() override
    {
        return sGlobalVolumeLevel_right_BD1CDE;
    }

    virtual s16& sGlobalVolumeLevel_left() override
    {
        return sGlobalVolumeLevel_left_BD1CDC;
    }

    virtual VabUnknown& s512_byte() override
    {
        return s512_byte_C13180;
    }

    virtual u8* sVagCounts() override
    {
        return sVagCounts_BE6144;
    }

    virtual u8* sProgCounts() override
    {
        return sProgCounts_BDCD64;
    }

    virtual VabHeader** spVabHeaders() override
    {
        return spVabHeaders_C13160;
    }

    virtual ConvertedVagTable& sConvertedVagTable() override
    {
        return sConvertedVagTable_BEF160;
    }

    virtual SoundEntryTable& sSoundEntryTable16() override
    {
        return sSoundEntryTable16_BE6160;
    }

    virtual MidiChannels& sMidi_Channels() override
    {
        return sMidi_Channels_C14080;
    }

    virtual MIDI_SeqSong& sMidiSeqSongs(s32 idx) override
    {
        if (idx < 0 || idx >= 32)
        {
            ALIVE_FATAL("sMidiSeqSongs out of bounds");
        }
        return sMidiSeqSongs_C13400.table[idx];
    }

    virtual s32& sMidi_Inited_dword() override
    {
        return sMidi_Inited_dword_BD1CF4;
    }

    virtual u32& sMidiTime() override
    {
        return sMidiTime_BD1CEC;
    }

    virtual Bool32& sSoundDatIsNull() override
    {
        return sSoundDatIsNull_BD1CE8;
    }

    virtual s8& sbDisableSeqs() override
    {
        return sbDisableSeqs_BD1CE4;
    }

    virtual u32& sLastTime() override
    {
        return sLastTime_578E20;
    }

    virtual u32& sMidi_WaitUntil() override
    {
        return sMidi_WaitUntil_BD1CF0;
    }

    virtual IO_FileHandleType& sSoundDatFileHandle() override
    {
        return sSoundDatFileHandle_BD1CE0;
    }

    virtual u8& sControllerValue() override
    {
        return sControllerValue_BD1CFC;
    }

    virtual void MIDI_ParseMidiMessage(s32 idx) override
    {
        MIDI_ParseMidiMessage_4FD100(idx);
    }

    virtual void SsUtKeyOffV(s32 idx) override
    {
        SsUtKeyOffV_4FE010(static_cast<s16>(idx));
    }
};

static AEPsxSpuApiVars gAeSpuVars;
static IPsxSpuApiVars* gSpuVars = &gAeSpuVars; // Default to AE vars


EXPORT void SetSpuApiVars(IPsxSpuApiVars* pVars)
{
    gSpuVars = pVars;
}

EXPORT IPsxSpuApiVars* GetSpuApiVars()
{
    return gSpuVars;
}

template <typename T>
T SwapBytes(T value);

template <>
inline u16 SwapBytes<u16>(u16 value)
{
    return (value >> 8) | (value << 8);
}

template <>
inline unsigned SwapBytes<unsigned>(unsigned value)
{
    return unsigned(SwapBytes<u16>(static_cast<u16>(value)) << 16) | SwapBytes<u16>(static_cast<u16>(value >> 16));
}

EXPORT void CC MIDI_ADSR_Update_4FDCE0();
EXPORT s16 CC MIDI_PitchBend_4FDEC0(s16 field4_match, s16 pitch);
EXPORT void CC MIDI_Read_SEQ_Header_4FD870(u8** pSrc, SeqHeader* pDst, u32 size);
EXPORT void CC MIDI_SetTempo_4FDB80(s16 idx, s16 kZero, s16 tempo);
EXPORT s32 CC MIDI_Set_Volume_4FDE80(MIDI_Channel* pData, s32 vol);
EXPORT s32 CC MIDI_Stop_Existing_Single_Note_4FCFF0(s32 VabIdAndProgram, s32 note);
EXPORT void CC MIDI_Wait_4FCE50();

void SsExt_CloseAllVabs()
{
    for (s32 i = 0; i < kMaxVabs; i++)
    {
        if (gSpuVars->sVagCounts()[i] > 0)
        {
            SsVabClose_4FC5B0(i);
        }
    }
}

void SsExt_StopPlayingSamples()
{
    for (s32 i = 0; i < kNumChannels; i++)
    {
        if (gSpuVars->sMidi_Channels().channels[i].field_1C_adsr.field_3_state)
        {
            GetSoundAPI().SND_Stop_Sample_At_Idx(gSpuVars->sMidi_Channels().channels[i].field_0_sound_buffer_field_4);
        }
    }
}

EXPORT void CC SSInit_4FC230()
{
    gSpuVars->sGlobalVolumeLevel_right() = 127;
    gSpuVars->sGlobalVolumeLevel_left() = 127;

    GetSoundAPI().SND_CreateDS(22050u, 16, 1);
}

EXPORT void CC SpuInitHot_4FC320()
{
    gSpuVars->sMidi_Inited_dword() = 1;
    gSpuVars->sMidiTime() = SYS_GetTicks();
}

EXPORT void SsEnd_4FC350()
{
    gSpuVars->sMidi_Inited_dword() = 0;
}

EXPORT void CC SsSetMVol_4FC360(s16 left, s16 right)
{
    gSpuVars->sGlobalVolumeLevel_left() = left;
    gSpuVars->sGlobalVolumeLevel_right() = right;
}

EXPORT u32* CC SND_SoundsDat_Get_Sample_Offset_4FC3D0(VabHeader* pVabHeader, VabBodyRecord* pBodyRecords, s32 idx)
{
    if (!pVabHeader || idx < 0)
    {
        return nullptr;
    }

    VabBodyRecord* ret = pBodyRecords;
    if (idx - 1 >= 0)
    {
        ret = &pBodyRecords[idx];
    }

    return &ret->field_8_fileOffset;
}

// TODO: Reverse/refactor properly
EXPORT s32 CC SND_SoundsDat_Get_Sample_Len_4FC400(VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx)
{
    s32 result; // eax

    if (pVabHeader && idx >= 0)
    {
        result = (s32)(8 * *(SND_SoundsDat_Get_Sample_Offset_4FC3D0(pVabHeader, pVabBody, idx) - 2)) / 16; // -2 = field_0_length_or_duration
    }
    else
    {
        result = -1;
    }
    return result;
}

// TODO: Reverse/refactor properly
EXPORT s32 CC sub_4FC440(VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx)
{
    return *(SND_SoundsDat_Get_Sample_Offset_4FC3D0(pVabHeader, pVabBody, idx) - 1); // -1 = field_4_unused
}

// TODO: Reverse/refactor properly
EXPORT Bool32 CC sub_4FC470(VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx)
{
    return sub_4FC440(pVabHeader, pVabBody, idx) < 0;
}

EXPORT s32 CC SND_SoundsDat_Read_4FC4E0(VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx, void* pBuffer)
{
    const s32 sampleOffset = *SND_SoundsDat_Get_Sample_Offset_4FC3D0(pVabHeader, pVabBody, idx); // = field_8_fileOffset
    const s32 sampleLen = SND_SoundsDat_Get_Sample_Len_4FC400(pVabHeader, pVabBody, idx);
    if (sampleOffset == -1 || !gSpuVars->sSoundDatFileHandle())
    {
        return 0;
    }

    IO_Seek(gSpuVars->sSoundDatFileHandle(), sampleOffset, 0);
    IO_Read(gSpuVars->sSoundDatFileHandle(), pBuffer, 2 * sampleLen, 1u);

    return sampleLen;
}

EXPORT void CC SsVabClose_4FC5B0(s32 vabId)
{
    SsUtAllKeyOff_4FDFE0(0); // TODO: Check argument ??

    if (gSpuVars->sVagCounts()[vabId] - 1 >= 0)
    {
        // Free backwards
        for (s32 i = gSpuVars->sVagCounts()[vabId] - 1; i >= 0; i--)
        {
            GetSoundAPI().SND_Free(&gSpuVars->sSoundEntryTable16().table[vabId][i]);
        }
    }

    gSpuVars->sVagCounts()[vabId] = 0;
    gSpuVars->sProgCounts()[vabId] = 0;
}


EXPORT s16 CC SsVabOpenHead_4FC620(VabHeader* pVabHeader)
{
    if (!pVabHeader)
    {
        return -1;
    }

    SsSeqCalledTbyT_4FDC80();

    const s32 vab_id = pVabHeader->field_8_id;
    assert(vab_id < 4);
    SsVabClose_4FC5B0(vab_id);
    gSpuVars->spVabHeaders()[vab_id] = pVabHeader;
    if (gSpuVars->sVagCounts()[vab_id] > 0)
    {
        SsVabClose_4FC5B0(vab_id);
    }

    s32 numVags = pVabHeader->field_16_num_vags;
    if (numVags > 256)
    {
        numVags = 255;
    }

    gSpuVars->sVagCounts()[vab_id] = static_cast<u8>(numVags);
    gSpuVars->sProgCounts()[vab_id] = static_cast<u8>(pVabHeader->field_12_num_progs);
    memset(gSpuVars->s512_byte().field_0[vab_id], 0, sizeof(s8[128]));
    VagAtr* pVagAttr = (VagAtr*) &pVabHeader[1];
    memset(&gSpuVars->sConvertedVagTable().table[vab_id][0][0], 0, sizeof(Converted_Vag[128][16]));

    for (s32 i = 0; i < pVabHeader->field_12_num_progs; i++)
    {
        for (s32 toneCounter = 0; toneCounter < 16; toneCounter++)
        {
            if (pVagAttr->field_2_vol > 0)
            {
                Converted_Vag* pData = &gSpuVars->sConvertedVagTable().table[vab_id][pVagAttr->field_14_prog][toneCounter];

                pData->field_F_prog = static_cast<u8>(pVagAttr->field_14_prog);
                pData->field_10_vag = LOBYTE(pVagAttr->field_16_vag) - 1;
                pData->field_C = 0;
                pData->field_D_vol = pVagAttr->field_2_vol;
                pData->field_E_priority = pVagAttr->field_0_priority;
                pData->field_8_min = pVagAttr->field_6_min;
                pData->field_9_max = pVagAttr->field_7_max;

                const s16 centre = pVagAttr->field_4_centre;
                pData->field_A_shift_cen = 2 * (pVagAttr->field_5_shift + (centre << 7));

                f32 sustain_level = static_cast<f32>((2 * (~(u8) pVagAttr->field_10_adsr1 & 0xF)));

                pData->field_0_adsr_attack = std::min(static_cast<u16>((powf(2.0f, ((pVagAttr->field_10_adsr1 >> 8) & 0x7F) * 0.25f) * 0.09f)), static_cast<u16>(32767));
                pData->field_4_adsr_decay = static_cast<u16>((((pVagAttr->field_10_adsr1 >> 4) & 0xF) / 15.0f) * 16.0);
                pData->field_2_adsr_sustain_level = std::min(static_cast<u16>((sustain_level / 15.0f) * 600.0), static_cast<u16>(32767));
                pData->field_6_adsr_release = std::min(static_cast<u16>(pow(2, pVagAttr->field_12_adsr2 & 0x1F) * 0.045f), static_cast<u16>(32767));

                // If decay is at max, then nothing should play. So mute sustain too ?
                if (pData->field_4_adsr_decay == 16)
                {
                    pData->field_2_adsr_sustain_level = 0;
                }

#if ORIGINAL_PS1_BEHAVIOR
                // Stereo Hack.
                // PC version of the game COMPLETELY ignores tone pans.
                // This makes it sound very bland and mono. :(
                // That's okay though, we're gonna stick the pan value in some padding!

                pData->field_11_pad = pVagAttr->field_3_pan;
#endif
            }
            ++pVagAttr;
        }
    }
    return static_cast<s16>(vab_id);
}

// Loads sounds dat to memory
EXPORT void CC SsVabTransBody_4FC840(VabBodyRecord* pVabBody, s16 vabId)
{
    if (vabId < 0)
    {
        return;
    }

    gSpuVars->sSoundDatFileHandle() = IO_Open("sounds.dat", "rb");
    gSpuVars->sSoundDatIsNull() = gSpuVars->sSoundDatFileHandle() == nullptr;

    assert(vabId < 4);
    VabHeader* pVabHeader = gSpuVars->spVabHeaders()[vabId];
    const s32 vagCount = gSpuVars->sVagCounts()[vabId];
    for (s32 i = 0; i < vagCount; i++)
    {
        SoundEntry* pEntry = &gSpuVars->sSoundEntryTable16().table[vabId][i];
        memset(pEntry, 0, sizeof(SoundEntry));

        if (!(i & 7))
        {
            SsSeqCalledTbyT_4FDC80();
        }


        s32 sampleLen = SND_SoundsDat_Get_Sample_Len_4FC400(pVabHeader, pVabBody, i);
        if (sampleLen < 4000 && !sub_4FC470(pVabHeader, pVabBody, i))
        {
            sampleLen *= 2;
        }

        if (sampleLen > 0)
        {
            // Find matching converted vag to set field_C / field_6_adsr
            const s32 unused_field = sub_4FC470(pVabHeader, pVabBody, i);
            const u8 unused_copy = unused_field != 0 ? 4 : 0;
            for (s32 prog = 0; prog < 128; prog++)
            {
                for (s32 tone = 0; tone < 16; tone++)
                {
                    Converted_Vag* pVag = &gSpuVars->sConvertedVagTable().table[vabId][prog][tone];
                    if (pVag->field_10_vag == i)
                    {
                        pVag->field_C = unused_copy;
                        if (!(unused_copy & 4) && !pVag->field_0_adsr_attack && pVag->field_6_adsr_release)
                        {
                            pVag->field_6_adsr_release = 0;
                        }
                    }
                }
            }

            // Allocate pEntry
            if (GetSoundAPI().SND_New(pEntry, sampleLen, 44100, 16, 0) == 0)
            {
                // Allocate a temp buffer to read sounds.dat bytes into
                void* pTempBuffer = ae_malloc_4F4E60(sampleLen * pEntry->field_1D_blockAlign);
                if (pTempBuffer)
                {
                    // Read the sample data
                    memset(pTempBuffer, 0, sampleLen * pEntry->field_1D_blockAlign);
                    if (SND_SoundsDat_Read_4FC4E0(pVabHeader, pVabBody, i, pTempBuffer))
                    {
                        // Load it into the sound buffer
                        GetSoundAPI().SND_Load(pEntry, pTempBuffer, sampleLen);
                    }
                    ae_free_4F4EA0(pTempBuffer);
                }
            }
        }
    }

    if (gSpuVars->sSoundDatFileHandle())
    {
        IO_Close(gSpuVars->sSoundDatFileHandle());
        gSpuVars->sSoundDatFileHandle() = nullptr;
    }
}

EXPORT s32 CC MIDI_Invert_4FCA40(s32 /*not_used*/, s32 value)
{
    return 127 - value;
}


EXPORT s32 CC MIDI_Allocate_Channel_4FCA50(s32 /*not_used*/, s32 priority)
{
    s32 lowestEndTime = -999999;
    u32 timeMod24 = gSpuVars->sMidiTime() % 24;
    for (s32 i = 0; i < 24; i++)
    {
        if (gSpuVars->sMidi_Channels().channels[i].field_1C_adsr.field_3_state == 0)
        {
            return i;
        }
        else
        {
            const s32 inverted = MIDI_Invert_4FCA40(gSpuVars->sMidi_Channels().channels[i].field_4_priority, gSpuVars->sMidi_Channels().channels[i].field_8_left_vol);
            if (inverted > lowestEndTime)
            {
                timeMod24 = i;
                lowestEndTime = inverted;
            }
        }
    }

    // Try to find a channel that isn't playing anything
    for (s32 i = 0; i < 24; i++)
    {
        if (GetSoundAPI().SND_Get_Buffer_Status(gSpuVars->sMidi_Channels().channels[i].field_0_sound_buffer_field_4) == 0)
        {
            gSpuVars->sMidi_Channels().channels[i].field_1C_adsr.field_3_state = 0;
            return i;
        }
    }

    // Take the channel which has sound that is ending soonest
    s32 idx = timeMod24;
    if (priority < gSpuVars->sMidi_Channels().channels[idx].field_4_priority)
    {
        return -1;
    }
    GetSoundAPI().SND_Stop_Sample_At_Idx(gSpuVars->sMidi_Channels().channels[idx].field_0_sound_buffer_field_4);
    return idx;
}


EXPORT s32 CC MIDI_PlayMidiNote_4FCB30(s32 vabId, s32 program, s32 note, s32 leftVolume, s32 rightVolume, s32 volume)
{
    const s32 noteKeyNumber = (note >> 8) & 127;
    s32 leftVol2 = leftVolume;
    s32 rightVol2 = rightVolume;

    // TODO: Logic seems wrong even in OG - also is this actually panning ??
    if (leftVolume)
    {
        if (leftVolume < 0)
        {
            return 0;
        }
    }
    else if (!rightVolume)
    {
        return 0;
    }

    if (rightVolume < 0)
    {
        return 0;
    }

    if (volume <= 0)
    {
        return 0;
    }

    if (gSpuVars->sVagCounts()[vabId] == 0)
    {
        return 0;
    }

    s32 usedChannelBits = 0;
    for (s32 i = 0; i < 16; i++)
    {
        Converted_Vag* pVagIter = &gSpuVars->sConvertedVagTable().table[vabId][program][i];
        if (pVagIter->field_D_vol > 0 && pVagIter->field_8_min <= noteKeyNumber && noteKeyNumber <= pVagIter->field_9_max)
        {
            const s32 vagVol = pVagIter->field_D_vol;
            s32 panLeft = (volume * leftVol2 * vagVol * gSpuVars->sGlobalVolumeLevel_left()) >> 21;
            s32 panRight = (volume * rightVol2 * vagVol * gSpuVars->sGlobalVolumeLevel_right()) >> 21;
            const u32 playFlags = (((u32) pVagIter->field_C) >> 2) & 1;

            // TODO: Logic seems wrong even in OG
            if (panLeft)
            {
                if (panLeft < 0)
                {
                    continue;
                }
            }
            else if (!panRight)
            {
                continue;
            }

            if (panRight >= 0)
            {
                if ((((u32) pVagIter->field_C >> 2)) & 1)
                {
                    // Clamp pans
                    if (panLeft > 90)
                    {
                        panLeft = 90;
                    }

                    if (panRight > 90)
                    {
                        panRight = 90;
                    }
                }
                s32 maxPan = panRight;
                if (panLeft >= panRight)
                {
                    maxPan = panLeft;
                }
                const s32 midiChannel = MIDI_Allocate_Channel_4FCA50(maxPan, pVagIter->field_E_priority);
                if (midiChannel >= 0)
                {
                    MIDI_Channel* pChannel = &gSpuVars->sMidi_Channels().channels[midiChannel];
                    const Bool32 bUnknown = pVagIter->field_0_adsr_attack || pVagIter->field_2_adsr_sustain_level || pVagIter->field_4_adsr_decay != 16 || pVagIter->field_6_adsr_release >= 33u;
                    pChannel->field_C_vol = maxPan;
                    if (bUnknown)
                    {
                        pChannel->field_1C_adsr.field_3_state = 1;

                        pChannel->field_1C_adsr.field_4_attack = static_cast<u16>((pVagIter->field_0_adsr_attack * (127 - volume)) / 64);
                        pChannel->field_1C_adsr.field_6_sustain = pVagIter->field_2_adsr_sustain_level;
                        pChannel->field_1C_adsr.field_8_decay = pVagIter->field_4_adsr_decay;
                        pChannel->field_1C_adsr.field_A_release = pVagIter->field_6_adsr_release;

                        if (pChannel->field_1C_adsr.field_4_attack)
                        {
                            panLeft = 2;
                            maxPan = 2;
                            rightVol2 = 2;
                            leftVol2 = 2;
                            panRight = 2;
                        }
                    }
                    else if (playFlags)
                    {
                        pChannel->field_1C_adsr.field_3_state = -1;
                    }
                    else
                    {
                        pChannel->field_1C_adsr.field_3_state = -2;
                    }

                    pChannel->field_8_left_vol = maxPan;
                    pChannel->field_4_priority = pVagIter->field_E_priority;
                    pChannel->field_18_rightVol = playFlags;
                    pChannel->field_14_time = gSpuVars->sMidiTime();
                    pChannel->field_1C_adsr.field_0_seq_idx = static_cast<u8>(vabId);
                    pChannel->field_1C_adsr.field_1_program = static_cast<u8>(program);
                    pChannel->field_1C_adsr.field_2_note_byte1 = static_cast<u8>(noteKeyNumber);

#if ORIGINAL_PS1_BEHAVIOR

                    // Note: 1.059463094359 is the 12th root of 2.
                    // https://en.wikipedia.org/wiki/Twelfth_root_of_two

                    // Uses the correct way of determining the pitch f32.
                    // Almost identical to PS1 versions pitches (at least from PS1 emulators)
                    // Todo: check real ps1 hardware.
                    pChannel->field_10_freq = static_cast<f32>(pow(1.059463094359, (f64)((note / 256.0) - (pVagIter->field_A_shift_cen / 256.0))));

                    // This way seems to be more accurate to ps1 sound, but its actually not correct. So ps1 was limited because floats?
                    // pChannel->field_10_float = static_cast<f32>(pow(1.059463094359, (f64)((note / 256) - (pVagIter->field_A_shift_cen / 256))));
#else
                    pChannel->field_10_freq = static_cast<f32>(pow(1.059463094359, (f64)(note - pVagIter->field_A_shift_cen) * 0.00390625));
#endif

                    if (gSpuVars->sMidi_WaitUntil())
                    {
                        MIDI_Wait_4FCE50();
                    }

                    // Pan - L=0, C=64, R=127
                    s8 pan = pVagIter->field_11_pad;
                    if (pan == 127)
                    {
                        panLeft = 0;
                    }
                    else if (pan == 0)
                    {
                        panRight = 0;
                    }

                    GetSoundAPI().SND_PlayEx(
                        &gSpuVars->sSoundEntryTable16().table[vabId][pVagIter->field_10_vag],
                        panLeft,
                        panRight,
                        pChannel->field_10_freq, // freq
                        pChannel,
                        playFlags,
                        pVagIter->field_E_priority);

                    if (program == 4 || program == 5 || program == 8 || program == 23 || program == 24 || program == 25)
                    {
                        gSpuVars->sMidi_WaitUntil() = SYS_GetTicks() + 10;
                    }

                    usedChannelBits |= (1 << midiChannel);
                }
            }
        }
    }
    return usedChannelBits;
}


EXPORT void CC MIDI_Wait_4FCE50()
{
    while (SYS_GetTicks() < gSpuVars->sMidi_WaitUntil())
    {
    }
    gSpuVars->sMidi_WaitUntil() = 0;
}


EXPORT s32 CC MIDI_PlayerPlayMidiNote_4FCE80(s32 vabId, s32 program, s32 note, s32 leftVol, s32 rightVol, s32 volume)
{
    if (gSpuVars->sSoundDatIsNull())
    {
        return 0;
    }

    if (rightVol >= 64)
    {
        return MIDI_PlayMidiNote_4FCB30(vabId, program, note, leftVol * (127 - rightVol) / 64, leftVol, volume);
    }
    else
    {
        return MIDI_PlayMidiNote_4FCB30(vabId, program, note, leftVol, rightVol * leftVol / 64, volume);
    }
}


EXPORT s32 CC SsVoKeyOn_4FCF10(s32 vabIdAndProgram, s32 pitch, u16 leftVol, u16 rightVol)
{
    MIDI_Stop_Existing_Single_Note_4FCFF0((vabIdAndProgram & 127) | (((vabIdAndProgram >> 8) & 31) << 8), pitch);

    if (gSpuVars->sSoundDatIsNull())
    {
        return 0;
    }

    const s32 channelBits = MIDI_PlayMidiNote_4FCB30((vabIdAndProgram >> 8) & 31, vabIdAndProgram & 127, pitch, leftVol, rightVol, 96);

    for (s32 idx = 0; idx < kNumChannels; idx++)
    {
        MIDI_Channel* pChannel = &gSpuVars->sMidi_Channels().channels[idx];
        if ((1 << idx) & channelBits)
        {
            pChannel->field_1C_adsr.field_C = 0xFFFF; // or both -1, but they appear to be unsigned ??
            pChannel->field_1C_adsr.field_E_ref_count = 0xFF;
        }
    }

    return channelBits;
}


EXPORT s32 CC MIDI_Stop_Existing_Single_Note_4FCFF0(s32 VabIdAndProgram, s32 note)
{
    const s32 vabId = (VabIdAndProgram >> 8) & 31;
    if (!gSpuVars->sVagCounts()[vabId])
    {
        return 0;
    }

    s16 i = 0;
    for (i = 0; i < kNumChannels; i++)
    {
        MIDI_Channel* pChannel = &gSpuVars->sMidi_Channels().channels[i];
        MIDI_ADSR_State* pSub = &pChannel->field_1C_adsr;
        if (pSub->field_3_state
            && pSub->field_0_seq_idx == vabId
            && pSub->field_1_program == (VabIdAndProgram & 127)
            && pSub->field_2_note_byte1 == ((note >> 8) & 127))
        {
            gSpuVars->SsUtKeyOffV(i);
            return 0;
        }
    }
    return 0;
}

// TODO: Removed 4FD0C0

EXPORT s32 CC MIDI_Read_Var_Len_4FD0D0(MIDI_SeqSong* pMidiStru)
{
    s32 ret = 0;
    u8 midiByte = 0;
    for (s32 i = 0; i < 4; ++i)
    {
        midiByte = MIDI_ReadByte_4FD6B0(pMidiStru);
        ret = (ret << 7) | (midiByte & 0x7f);
        if (!(midiByte & 0x80))
        {
            break;
        }
    }
    return ret;
}


// TODO: Complete tests so this can be rewritten
EXPORT s32 CC MIDI_ParseMidiMessage_4FD100(s32 idx)
{
    //NOT_IMPLEMENTED();

    s32 idx2;                   // ebp
    MIDI_SeqSong* pCtx;         // esi
    u8 midiByte1;               // al
    s32 midiByte1_copy;         // ebx
    u8 metaEvent;               // bl
    s32 oldLoopCount;           // eax
    s32 newLoopCount;           // eax
    s32 tempoChange;            // eax
    s8 tempoByte3;              // bl
    s8 tempoByte2;              // ST2B_1
    u8 fullTempo;               // dl
    s32 v12;                    // eax
    u16 v13;                    // cx
    s32 v14;                    // ecx
    u32 cmd;                    // ebx
    u32 v16;                    // eax
    s32 v17;                    // ecx
    MIDI_ProgramVolume* v18;    // ecx
    s32 v19;                    // ebx
    MIDI_ADSR_State* pSubChan2; // esi
    s8 v21;                     // bl
    s8 channelIdx_1;            // cl

    MIDI_ADSR_State* pSubChan1;    // esi
    u8 refCount1;                  // dl
    MIDI_ADSR_State* pAdsr;        // esi
    s32 v29;                       // edi
    s16 v31 = 0;                   // bp
    u8 v33;                        // cl
    s32 v34;                       // eax
    void(CC * pFn)(s32, u32, u32); // eax
    u8* v36;                       // eax
    s8 v37;                        // al
    s32 v38;                       // eax
    u32 v39;                       // ecx
    u32 v40;                       // edi
    s32 v42 = 0;                   // [esp+14h] [ebp-1Ch]
    MIDI_ProgramVolume* v43;       // [esp+18h] [ebp-18h]
    s32 v45;                       // [esp+20h] [ebp-10h]
    s32 leftVol;                   // [esp+2Ch] [ebp-4h]
    s32 v47;                       // [esp+2Ch] [ebp-4h]
    MIDI_ProgramVolume* v32;

    idx2 = idx;
    pCtx = &gSpuVars->sMidiSeqSongs(idx);
    if (gSpuVars->sMidiSeqSongs(idx).field_4_time > gSpuVars->sMidiTime())
    {
        return 1;
    }

    for (;;)
    {
        midiByte1 = MIDI_ReadByte_4FD6B0(pCtx);
        midiByte1_copy = midiByte1;
        if (midiByte1 >= 0xF0u)
        {
            if (midiByte1 == 0xF0 || midiByte1 == 0xF7)
            {
                v12 = MIDI_Read_Var_Len_4FD0D0(pCtx);
                MIDI_SkipBytes_4FD6C0(pCtx, v12);
            }
            else if (midiByte1 == 0xFF) // Sysex len
            {
                metaEvent = MIDI_ReadByte_4FD6B0(pCtx);
                if (metaEvent == 0x2F) // End of track
                {
                    oldLoopCount = gSpuVars->sMidiSeqSongs(idx).field_18_repeatCount;
                    if (oldLoopCount)
                    {
                        newLoopCount = oldLoopCount - 1;
                        gSpuVars->sMidiSeqSongs(idx).field_18_repeatCount = newLoopCount;
                        if (!newLoopCount)
                        {
                            SsSeqStop_4FD9C0(static_cast<s16>(idx));
                            return 1;
                        }
                    }
                    pCtx->field_0_seq_data = gSpuVars->sMidiSeqSongs(idx).field_1C_pSeqData;
                }
                else
                {
                    tempoChange = MIDI_Read_Var_Len_4FD0D0(pCtx);
                    if (tempoChange)
                    {
                        if (metaEvent == 0x51) // Tempo in microseconds per quarter note (24-bit value)
                        {
                            tempoByte3 = MIDI_ReadByte_4FD6B0(pCtx) << 16;
                            tempoByte2 = MIDI_ReadByte_4FD6B0(pCtx) << 8;

                            // TODO: This is too s16
                            fullTempo = tempoByte3 | tempoByte2 | MIDI_ReadByte_4FD6B0(pCtx);
                            MIDI_SetTempo_4FDB80(static_cast<s16>(idx), 0, static_cast<s16>(fullTempo));
                        }
                        else
                        {
                            MIDI_SkipBytes_4FD6C0(pCtx, tempoChange);
                        }
                    }
                }
            }
            goto next_time_stamp;
        }

        if (midiByte1 < 0x80u)
        {
            // Running status else return 0
            break;
        }

        midiByte1 = MIDI_ReadByte_4FD6B0(pCtx);
        gSpuVars->sMidiSeqSongs(idx2).field_2A_running_status = static_cast<u8>(midiByte1_copy); // running status ?

    handle_next_event:

        v13 = v31 & 0x00FF;
        v13 |= (u16)(midiByte1 << 8);
        //LOBYTE(v13) = 0;
        //HIBYTE(v13) = midiByte1;
        v14 = midiByte1_copy | v13;
        cmd = midiByte1_copy & 0xF0;
        v16 = v14;
        //        LOWORD(v42) = v14;
        v42 = v42 & 0x0000FFFF;
        v42 |= v14 & 0xFFFF;

        if (cmd != 0xC0 && cmd != 0xD0)
        {
            v42 = (MIDI_ReadByte_4FD6B0(pCtx) << 16) | v14;
            v16 = v42;
        }

        switch (cmd)
        {
            case 0x80u: // Note off

                v29 = v16 & 15;
                v32 = &gSpuVars->sMidiSeqSongs(idx2).field_32_progVols[v29];
                for (s32 i = 0; i < 24; i++)
                {
                    pAdsr = &gSpuVars->sMidi_Channels().channels[i].field_1C_adsr;
                    if (pAdsr->field_3_state)
                    {
                        if (pAdsr->field_0_seq_idx == gSpuVars->sMidiSeqSongs(idx).field_seq_idx
                            && pAdsr->field_1_program == v32->field_0_program // or field_2_not_inited ??
                            && pAdsr->field_C == v29 + 16 * idx
                            && pAdsr->field_2_note_byte1 == BYTE1(v16))
                        {
                            v33 = pAdsr->field_E_ref_count - 1;
                            pAdsr->field_E_ref_count = v33;
                            if (!v33)
                            {
                                gSpuVars->SsUtKeyOffV(v31);
                                BYTE1(v16) = BYTE1(v42);
                                pAdsr->field_C = 0;
                            }
                        }
                    }
                }
                idx2 = idx;
                break;
            case 0x90u: // Note on
                v17 = v16 & 15;
                v45 = v17;
                v18 = &gSpuVars->sMidiSeqSongs(idx2).field_32_progVols[v17];
                v43 = v18;
                leftVol = (s16)((u32)(v18->field_1_left_vol * gSpuVars->sMidiSeqSongs(idx2).field_C_volume) >> 7);
                if (v16 >> 16)
                {
                    v19 = 0;

                    for (s32 i = 0; i < 24; i++)
                    {
                        pSubChan2 = &gSpuVars->sMidi_Channels().channels[i].field_1C_adsr;
                        if (pSubChan2->field_3_state
                            && pSubChan2->field_0_seq_idx == gSpuVars->sMidiSeqSongs(idx2).field_seq_idx
                            && pSubChan2->field_1_program == v18->field_0_program
                            && pSubChan2->field_C == v45 + 16 * idx)
                        {
                            if (pSubChan2->field_2_note_byte1 == BYTE1(v16) && pSubChan2->field_E_ref_count > v19)
                            {
                                v19 = pSubChan2->field_E_ref_count;
                            }
                            v18 = v43;
                        }
                    }

                    v21 = static_cast<s8>(v19 + 1);
                    v47 = MIDI_PlayerPlayMidiNote_4FCE80(
                        gSpuVars->sMidiSeqSongs(idx2).field_seq_idx,
                        v18->field_0_program,
                        v16 & 0xFF00,
                        leftVol,
                        v18->field_2_right_vol,
                        v16 >> 16);
                    channelIdx_1 = 0;

                    for (s32 i = 0; i < 24; i++)
                    {
                        if ((1 << channelIdx_1) & v47)
                        {
                            gSpuVars->sMidi_Channels().channels[i].field_1C_adsr.field_C = static_cast<u16>(16 * idx + (v42 & 0xF));
                            gSpuVars->sMidi_Channels().channels[i].field_1C_adsr.field_E_ref_count = v21;
                        }
                        ++channelIdx_1;
                    }
                }
                else
                {
                    for (s16 i = 0; i < 24; i++)
                    {
                        pSubChan1 = &gSpuVars->sMidi_Channels().channels[i].field_1C_adsr;
                        if (pSubChan1->field_3_state)
                        {
                            if (pSubChan1->field_0_seq_idx == gSpuVars->sMidiSeqSongs(idx2).field_seq_idx
                                && pSubChan1->field_1_program == v18->field_0_program
                                && pSubChan1->field_C == v45 + 16 * idx
                                && pSubChan1->field_2_note_byte1 == BYTE1(v16))
                            {
                                refCount1 = pSubChan1->field_E_ref_count - 1;
                                pSubChan1->field_E_ref_count = refCount1;
                                if (!refCount1)
                                {
                                    gSpuVars->SsUtKeyOffV(i);
                                    BYTE1(v16) = BYTE1(v42);
                                    v18 = v43;
                                    pSubChan1->field_C = 0;
                                }
                            }
                        }
                    }
                }
                break;
            case 0xB0u: // Controller change
                v34 = (cmd >> 8) & 0x7F;
                if (v34 != 6 && v34 != 0x26)
                {
                    if (v34 == 0x63)
                    {
                        gSpuVars->sControllerValue() = BYTE2(cmd);
                    }
                    break;
                }

                switch (gSpuVars->sControllerValue())
                {
                    case 20: // Set loop
                        gSpuVars->sMidiSeqSongs(idx2).field_24_loop_start = pCtx->field_0_seq_data;
                        gSpuVars->sMidiSeqSongs(idx2).field_2C_loop_count = BYTE2(cmd);
                        break;
                    case 30: // Loop
                        v36 = gSpuVars->sMidiSeqSongs(idx2).field_24_loop_start;
                        if (v36)
                        {
                            if (gSpuVars->sMidiSeqSongs(idx2).field_2C_loop_count > 0)
                            {
                                pCtx->field_0_seq_data = v36;
                                v37 = gSpuVars->sMidiSeqSongs(idx2).field_2C_loop_count;
                                if (v37 < 127)
                                {
                                    gSpuVars->sControllerValue() = 0;
                                    gSpuVars->sMidiSeqSongs(idx2).field_2C_loop_count = v37 - 1;
                                    goto next_time_stamp;
                                }
                            }
                        }
                        break;
                    case 40:
                        pFn = (void(CC*)(s32, u32, u32)) gSpuVars->sMidiSeqSongs(idx2).field_20_fn_ptr;
                        if (pFn)
                        {
                            pFn(idx, 0, BYTE2(cmd));
                            gSpuVars->sControllerValue() = 0;
                            goto next_time_stamp;
                        }
                        break;
                }
                gSpuVars->sControllerValue() = 0;
                break;

            case 0xC0u: // Program change
            {
                gSpuVars->sMidiSeqSongs(idx2).field_32_progVols[v16 & 0xF].field_0_program = BYTE1(v16);
            }
            break;

            case 0xE0u: // Pitch bend
            {
                MIDI_PitchBend_4FDEC0(
                    gSpuVars->sMidiSeqSongs(idx2).field_32_progVols[v16 & 0xF].field_0_program,
                    static_cast<s16>(((v16 >> 8) - 0x4000) >> 4));
            }
            break;
            default:
                break;
        }

    next_time_stamp:

        pCtx = &gSpuVars->sMidiSeqSongs(idx2);
        // Next time stamp
        v38 = MIDI_Read_Var_Len_4FD0D0(&gSpuVars->sMidiSeqSongs(idx2));
        if (v38)
        {
            v39 = gSpuVars->sMidiTime();
            v40 = v38 * gSpuVars->sMidiSeqSongs(idx2).field_14_tempo / 1000u + gSpuVars->sMidiSeqSongs(idx2).field_4_time;
            gSpuVars->sMidiSeqSongs(idx2).field_4_time = v40;
            if (v40 > v39)
            {
                return 1;
            }
        }
    } // Loop end

    if (gSpuVars->sMidiSeqSongs(idx2).field_2A_running_status)
    {
        midiByte1_copy = (u8) gSpuVars->sMidiSeqSongs(idx2).field_2A_running_status;
        goto handle_next_event;
    }
    return 0;
}

EXPORT u8 CC MIDI_ReadByte_4FD6B0(MIDI_SeqSong* pData)
{
    return *pData->field_0_seq_data++;
}

EXPORT void CC MIDI_SkipBytes_4FD6C0(MIDI_SeqSong* pData, s32 length)
{
    pData->field_0_seq_data += length;
}

EXPORT s16 CC SsSeqOpen_4FD6D0(u8* pSeqData, s16 seqIdx)
{
    // Read header
    SeqHeader seqHeader = {};
    MIDI_Read_SEQ_Header_4FD870(&pSeqData, &seqHeader, sizeof(SeqHeader));

    // Check magic matches
    if (seqHeader.field_0_magic != ResourceManager::Resource_SEQp)
    {
        return -1;
    }

    // Only version 0 and 1 are valid ?
    if (SwapBytes(seqHeader.field_4_version) > 1)
    {
        return -1;
    }

    // Find a free entry
    s32 freeIdx = 0;
    for (s32 i = 0; i < 32; i++)
    {
        if (!gSpuVars->sMidiSeqSongs(i).field_0_seq_data)
        {
            break;
        }
        freeIdx++;
    }

    // Steal the first entry if none free
    if (freeIdx == 32)
    {
        freeIdx = 0;
        SsSeqClose_4FD8D0(0);
    }

    // Init its fields
    memset(&gSpuVars->sMidiSeqSongs(freeIdx), 0, sizeof(MIDI_SeqSong));
    for (s32 i = 0; i < 16; i++)
    {
        gSpuVars->sMidiSeqSongs(freeIdx).field_32_progVols[i].field_1_left_vol = 112;
        gSpuVars->sMidiSeqSongs(freeIdx).field_32_progVols[i].field_2_right_vol = 64;
    }

    // Set data based on SEQ header
    gSpuVars->sMidiSeqSongs(freeIdx).field_2E_seqAccessNum = -1;
    gSpuVars->sMidiSeqSongs(freeIdx).field_10_quaterNoteRes = SwapBytes(seqHeader.field_8_resolution_of_quater_note);
    gSpuVars->sMidiSeqSongs(freeIdx).field_31_timeSignatureBars2 = seqHeader.field_E_time_signature_beats;
    gSpuVars->sMidiSeqSongs(freeIdx).field_30_timeSignatureBars = seqHeader.field_D_time_signature_bars;

    const s32 quaterNoteRes = gSpuVars->sMidiSeqSongs(freeIdx).field_10_quaterNoteRes;
    s32 calculatedTempo = 0;
    // TODO: Figure out what these tempo calcs are actually doing
    if (quaterNoteRes >= 0)
    {
        u32 tempo24Bit = (seqHeader.field_A_tempo[0] << 16) | (seqHeader.field_A_tempo[1] << 8) | (seqHeader.field_A_tempo[2]);
        tempo24Bit = tempo24Bit / quaterNoteRes;
        calculatedTempo = ((((tempo24Bit >> 1) + 15000) / tempo24Bit >> 1) + 15000) / (((tempo24Bit >> 1) + 15000) / tempo24Bit);
    }
    else
    {
        const s32 calculatedQuaterNoteRes = -(static_cast<u8>(quaterNoteRes) * (-gSpuVars->sMidiSeqSongs(freeIdx).field_10_quaterNoteRes >> 8));
        gSpuVars->sMidiSeqSongs(freeIdx).field_10_quaterNoteRes = calculatedQuaterNoteRes;
        calculatedTempo = -1000000 / calculatedQuaterNoteRes;
    }

    gSpuVars->sMidiSeqSongs(freeIdx).field_14_tempo = calculatedTempo;
    gSpuVars->sMidiSeqSongs(freeIdx).field_1C_pSeqData = pSeqData;
    gSpuVars->sMidiSeqSongs(freeIdx).field_0_seq_data = pSeqData;
    gSpuVars->sMidiSeqSongs(freeIdx).field_C_volume = 112;
    gSpuVars->sMidiSeqSongs(freeIdx).field_seq_idx = seqIdx;

    return static_cast<s16>(freeIdx);
}

EXPORT void CC MIDI_Read_SEQ_Header_4FD870(u8** pSrc, SeqHeader* pDst, u32 size)
{
    memcpy(pDst, *pSrc, size);
    (*pSrc) += size;
}


EXPORT void CC SsSeqClose_4FD8D0(s16 idx)
{
    SsSeqStop_4FD9C0(idx);
    gSpuVars->sMidiSeqSongs(idx).field_C_volume = 0;
    gSpuVars->sMidiSeqSongs(idx).field_0_seq_data = 0;
    gSpuVars->sMidiSeqSongs(idx).field_1C_pSeqData = nullptr;
}


EXPORT void CC SsSeqPlay_4FD900(u16 idx, s8 repeatMode, s16 repeatCount)
{
    if (idx < 32u)
    {
        MIDI_SeqSong& rec = gSpuVars->sMidiSeqSongs(idx);
        if (rec.field_1C_pSeqData)
        {
            rec.field_4_time = gSpuVars->sMidiTime();
            rec.field_8_playTimeStamp = gSpuVars->sMidiTime();
            rec.field_18_repeatCount = repeatCount;

            if (rec.field_2B_repeatMode != -1)
            {
                rec.field_0_seq_data = rec.field_1C_pSeqData;
                rec.field_2A_running_status = 0;
                u32 midiTime = MIDI_Read_Var_Len_4FD0D0(&rec);
                if (midiTime)
                {
                    midiTime = (midiTime * rec.field_14_tempo) / 1000;
                }
                rec.field_4_time += midiTime;
            }

            if (repeatMode)
            {
                rec.field_2B_repeatMode = repeatMode == 1;
            }
            else
            {
                rec.field_2B_repeatMode = -1;
            }
        }
    }
}


EXPORT void CC SsSeqStop_4FD9C0(s16 idx)
{
    if (gSpuVars->sMidiSeqSongs(idx).field_0_seq_data)
    {
        gSpuVars->sMidiSeqSongs(idx).field_2B_repeatMode = 0;
        if (gSpuVars->sMidiSeqSongs(idx).field_2E_seqAccessNum >= 0)
        {
            SsSeqPlay_4FD900(gSpuVars->sMidiSeqSongs(idx).field_2E_seqAccessNum, 1, 1);
            gSpuVars->sMidiSeqSongs(idx).field_2E_seqAccessNum = -1;
        }
    }

    for (u32 i = 0; i < kNumChannels; i++)
    {
        u32 field_C = gSpuVars->sMidi_Channels().channels[i].field_1C_adsr.field_C;
        field_C = field_C >> 4;
        if (field_C == static_cast<u32>(idx))
        {
            gSpuVars->SsUtKeyOffV(static_cast<s16>(i));
            gSpuVars->sMidi_Channels().channels[i].field_1C_adsr.field_C = 0;
        }
    }
}

// TODO: Removed 4FDA40

// TODO: Removed 4FDA60

EXPORT u16 CC SsIsEos_4FDA80(s16 idx, s16 kZero)
{
    if (!kZero)
    {
        if (gSpuVars->sMidiSeqSongs(idx).field_0_seq_data)
        {
            if (gSpuVars->sMidiSeqSongs(idx).field_2B_repeatMode)
            {
                return 1;
            }
        }
    }
    return 0;
}


EXPORT void CC SsSeqSetVol_4FDAC0(s16 idx, s16 volLeft, s16 volRight)
{
    if (gSpuVars->sMidiSeqSongs(idx).field_0_seq_data)
    {
        // TODO: Refactor
        gSpuVars->sMidiSeqSongs(idx).field_C_volume = 112 * ((volRight + volLeft) >> 1) >> 7;
    }
}

// TODO: Removed 4FDB00

// TODO: Removed 4FDB40

EXPORT void CC MIDI_SetTempo_4FDB80(s16 idx, s16 kZero, s16 tempo)
{
    if (!kZero)
    {
        if (gSpuVars->sMidiSeqSongs(idx).field_10_quaterNoteRes >= 0) // TODO: BUG - will div zero?
        {
            gSpuVars->sMidiSeqSongs(idx).field_14_tempo = tempo;
        }
        else
        {
            gSpuVars->sMidiSeqSongs(idx).field_14_tempo = -1000000 / gSpuVars->sMidiSeqSongs(idx).field_10_quaterNoteRes;
        }
    }
}

EXPORT void CC SsSetTickMode_4FDC20(s32)
{
    // Stub
}

// TODO: Removed 4FDC30


EXPORT void CC SsSeqCalledTbyT_4FDC80()
{
    if (!gSpuVars->sbDisableSeqs())
    {
        const u32 currentTime = SYS_GetTicks();
        gSpuVars->sMidiTime() = currentTime;

        // This seems to be limiting the rate of playback,
        // maybe to save some CPU cycles? Removing helps
        // audio playback stay in time
        // 
        //// First time or 30 passed?
        //if (gSpuVars->sLastTime() == 0xFFFFFFFF || (s32)(currentTime - gSpuVars->sLastTime()) >= 30)
        //{
        //    gSpuVars->sLastTime() = currentTime;
            for (s32 i = 0; i < kNumChannels; i++)
            {
                if (gSpuVars->sMidiSeqSongs(i).field_0_seq_data)
                {
                    if (gSpuVars->sMidiSeqSongs(i).field_2B_repeatMode == 1)
                    {
                        gSpuVars->MIDI_ParseMidiMessage(i);
                    }
                }
            }
            MIDI_ADSR_Update_4FDCE0();
        //}
    }
}

EXPORT void CC MIDI_ADSR_Update_4FDCE0()
{
    for (s32 i = 0; i < kNumChannels; i++)
    {
        MIDI_Channel* pChannel = &gSpuVars->sMidi_Channels().channels[i];
        if (pChannel->field_1C_adsr.field_3_state)
        {
            s32 timeDiff1 = gSpuVars->sMidiTime() - pChannel->field_14_time;
            s32 timeDiff2 = gSpuVars->sMidiTime() - pChannel->field_14_time;

            if ((s32)(gSpuVars->sMidiTime() - pChannel->field_14_time) < 0)
            {
                timeDiff1 = 0;
                timeDiff2 = 0;
            }

            s32 timeDiffSquared = timeDiff1 * timeDiff1;
            switch (pChannel->field_1C_adsr.field_3_state + 1) // ADSR state ?
            {
                case 0:
                    if (timeDiff1 > 90000)
                    {
                        gSpuVars->SsUtKeyOffV(static_cast<s16>(i));
                    }
                    break;
                case 2:
                    if (timeDiff1 < pChannel->field_1C_adsr.field_4_attack)
                    {
                        MIDI_Set_Volume_4FDE80(
                            pChannel,
                            (s64)(((f64) timeDiff2 / (f64) pChannel->field_1C_adsr.field_4_attack
                                   + (f64) timeDiff2 / (f64) pChannel->field_1C_adsr.field_4_attack
                                   - (f64) timeDiff2 / (f64) pChannel->field_1C_adsr.field_4_attack * ((f64) timeDiff2 / (f64) pChannel->field_1C_adsr.field_4_attack))
                                  * (f64) pChannel->field_C_vol));
                        break;
                    }

                    pChannel->field_1C_adsr.field_3_state = 2;
                    pChannel->field_14_time += pChannel->field_1C_adsr.field_4_attack;
                    timeDiff1 -= pChannel->field_1C_adsr.field_4_attack;
                    [[fallthrough]];
                case 3:
                    if (timeDiff1 < pChannel->field_1C_adsr.field_6_sustain)
                    {
                        const s32 v8 = pChannel->field_C_vol * (16 - pChannel->field_1C_adsr.field_8_decay) >> 4;
                        MIDI_Set_Volume_4FDE80(pChannel, pChannel->field_C_vol - timeDiffSquared * v8 / (pChannel->field_1C_adsr.field_6_sustain * pChannel->field_1C_adsr.field_6_sustain));
                        break;
                    }
                    pChannel->field_1C_adsr.field_3_state = 3;
                    pChannel->field_14_time += pChannel->field_1C_adsr.field_6_sustain;
                    timeDiff1 -= pChannel->field_1C_adsr.field_6_sustain;
                    if (!(MIDI_Set_Volume_4FDE80(pChannel, pChannel->field_C_vol * pChannel->field_1C_adsr.field_8_decay >> 4)))
                    {
                        break;
                    }
                    [[fallthrough]];
                case 4:
                    if (timeDiff1 <= 15000)
                    {
                        break;
                    }

                    pChannel->field_1C_adsr.field_3_state = 4;
                    pChannel->field_14_time = gSpuVars->sMidiTime();
                    timeDiff1 = 0;
                    timeDiffSquared = 0;
                    pChannel->field_C_vol = pChannel->field_8_left_vol;
                    [[fallthrough]];
                case 5:
                    if (timeDiff1 >= pChannel->field_1C_adsr.field_A_release)
                    {
                        pChannel->field_1C_adsr.field_3_state = 0;
                        GetSoundAPI().SND_Stop_Sample_At_Idx(pChannel->field_0_sound_buffer_field_4);
                    }
                    else
                    {
                        MIDI_Set_Volume_4FDE80(pChannel, pChannel->field_C_vol - timeDiffSquared * pChannel->field_C_vol / (pChannel->field_1C_adsr.field_A_release * pChannel->field_1C_adsr.field_A_release));
                    }
                    break;
                default:
                    break;
            }
        }
    }
}


EXPORT s32 CC MIDI_Set_Volume_4FDE80(MIDI_Channel* pData, s32 vol)
{
    if (pData->field_8_left_vol == vol)
    {
        return 1;
    }

    pData->field_8_left_vol = static_cast<s8>(vol);

    if (!GetSoundAPI().SND_Buffer_Set_Volume(pData->field_0_sound_buffer_field_4, vol))
    {
        return 1;
    }

    pData->field_1C_adsr.field_3_state = 0;
    return 0;
}


EXPORT s16 CC MIDI_PitchBend_4FDEC0(s16 program, s16 pitch)
{
    const f32 pitcha = pow(1.059463094359f, (f32) pitch * 0.0078125f);
    for (s32 i = 0; i < kNumChannels; i++)
    {
        if (gSpuVars->sMidi_Channels().channels[i].field_1C_adsr.field_1_program == program)
        {
            const f32 freq = pitcha * gSpuVars->sMidi_Channels().channels[i].field_10_freq;
            GetSoundAPI().SND_Buffer_Set_Frequency2(gSpuVars->sMidi_Channels().channels[i].field_C_vol, freq);
        }
    }
    return 0;
}


EXPORT s16 CC SsUtChangePitch_4FDF70(s16 voice, s32 /*vabId*/, s32 /*prog*/, s16 old_note, s16 old_fine, s16 new_note, s16 new_fine)
{
    const f32 freq = pow(1.059463094359f, (f32)(new_fine + ((new_note - (s32) old_note) << 7) - old_fine) * 0.0078125f);
    GetSoundAPI().SND_Buffer_Set_Frequency1(gSpuVars->sMidi_Channels().channels[voice].field_0_sound_buffer_field_4, freq);
    return 0;
}


EXPORT void CC SsUtAllKeyOff_4FDFE0(s32)
{
    // Stop all backwards
    s16 idx = kNumChannels - 1;
    do
    {
        gSpuVars->SsUtKeyOffV(idx--);
    }
    while (idx >= 0);
}


EXPORT s16 CC SsUtKeyOffV_4FE010(s16 idx)
{
    MIDI_Channel* pChannel = &gSpuVars->sMidi_Channels().channels[idx];
    if (pChannel->field_1C_adsr.field_3_state)
    {
        pChannel->field_1C_adsr.field_3_state = 4;
        pChannel->field_C_vol = pChannel->field_8_left_vol;
        if (pChannel->field_1C_adsr.field_A_release < 300)
        {
            pChannel->field_1C_adsr.field_A_release = 300;
        }
        pChannel->field_14_time = gSpuVars->sMidiTime();
    }
    return 0;
}

EXPORT void SsVabTransCompleted_4FE060(s32)
{
    // Stub
}

// TODO: Removed 4FE081

EXPORT void CC SsSetTableSize_4FE0B0(void*, s32, s32)
{
    // Stub
}

// TODO: Removed 4FE210

// TODO: Removed 4FE220

// TODO: Removed 4FE270

// TODO: Removed 4FE320

// TODO: Removed 4FE330

EXPORT void SsUtReverbOn_4FE340()
{
    // Stub
}

EXPORT void SsUtReverbOff_4FE350()
{
    // Stub
}
EXPORT void CC SsUtSetReverbType_4FE360(s32)
{
    // Stub
}

EXPORT void SsUtSetReverbDepth_4FE380(s32, s32)
{
    // Stub
}

EXPORT void SpuClearReverbWorkArea_4FA690(s32)
{
    // Stub
}


EXPORT void CC SND_CallBack_4020A4()
{
    // Stub
}


EXPORT void CC VSyncCallback_4F8C40(TVSyncCallBackFn)
{
    // Stub
}
