#include "stdafx.h"
#include "PsxSpuApi.hpp"
#include "Function.hpp"
#include "Io.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Sound.hpp" // SoundEntry structure
#include "Sys.hpp" // SYS_GetTicks
#include "PathData.hpp" // SoundBlockInfo, SeqPathDataRecord

const int kNumChannels = 24;

struct VagAtr
{
    char field_0_priority;
    char field_1_mode;
    char field_2_vol;
    char field_3_pan;
    unsigned __int8 field_4_centre;
    unsigned __int8 field_5_shift;
    char field_6_min;
    char field_7_max;
    char field_8_vibW;
    char field_9_vibT;
    char field_A_porW;
    char field_B_porT;
    char field_C_pitch_bend_min;
    char field_D_pitch_bend_max;
    char field_E_reserved1;
    char field_F_reserved2;
    __int16 field_10_adsr1;
    __int16 field_12_adsr2;
    __int16 field_14_prog;
    __int16 field_16_vag;
    __int16 field_18_reserved[4];
};

struct MIDI_ProgramVolume
{
    char field_0_program;
    char field_1_left_vol;
    char field_2_right_vol;
};
ALIVE_ASSERT_SIZEOF(MIDI_ProgramVolume, 3);

struct MIDI_SeqSong
{
    BYTE *field_0_seq_data;
    unsigned int field_4_time;
    int field_8_playTimeStamp;
    int field_C_volume;
    int field_10_quaterNoteRes;
    int field_14_tempo;
    int field_18_repeatCount;
    BYTE *field_1C_pSeqData;
    void* field_20_fn_ptr; // read but never written
    BYTE *field_24_loop_start;
    __int16 field_seq_idx;
    unsigned char field_2A_running_status;
    char field_2B_repeatMode; // TODO: Check
    char field_2C_loop_count;
    char field_2D_pad;
    __int16 field_2E_seqAccessNum; // never written in a meaningful way
    char field_30_timeSignatureBars;
    char field_31_timeSignatureBars2; // bug: maybe they should have assigned beats instead? but never read anyway
    MIDI_ProgramVolume field_32_progVols[16];
    char field_62_pad;
    char field_63_pad;
};
ALIVE_ASSERT_SIZEOF(MIDI_SeqSong, 100);

#pragma pack(push)
#pragma pack(1)
struct SeqHeader
{
    int field_0_magic;
    unsigned int field_4_version;
    WORD field_8_resolution_of_quater_note;
    BYTE field_A_tempo[3];
    // No padding byte here, hence 1 byte packing enabled
    BYTE field_D_time_signature_bars;
    BYTE field_E_time_signature_beats;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(SeqHeader, 0xF);

struct VabUnknown
{
    char field_0[4][128];
};
ALIVE_ASSERT_SIZEOF(VabUnknown, 512);



struct MidiChannels
{
    MIDI_Channel channels[kNumChannels];
};

struct MidiSeqSongsTable
{
    MIDI_SeqSong table[32];
};

ALIVE_VAR(1, 0xBD1CDE, __int16, sGlobalVolumeLevel_right_BD1CDE, 0);
ALIVE_VAR(1, 0xBD1CDC, __int16, sGlobalVolumeLevel_left_BD1CDC, 0);
ALIVE_VAR(1, 0xC13180, VabUnknown, s512_byte_C13180, {});
ALIVE_ARY(1, 0xBE6144, BYTE, kMaxVabs, sVagCounts_BE6144, {});
ALIVE_ARY(1, 0x0BDCD64, BYTE, kMaxVabs, sProgCounts_BDCD64, {});
ALIVE_ARY(1, 0xC13160, VabHeader*, 4, spVabHeaders_C13160, {});
ALIVE_VAR(1, 0xBEF160, ConvertedVagTable, sConvertedVagTable_BEF160, {});
ALIVE_VAR(1, 0xBE6160, SoundEntryTable, sSoundEntryTable16_BE6160, {});
ALIVE_ASSERT_SIZEOF(MidiChannels, 1056);
ALIVE_VAR(1, 0xC14080, MidiChannels, sMidi_Channels_C14080, {});
ALIVE_ASSERT_SIZEOF(MidiSeqSongsTable, 3200);
ALIVE_VAR(1, 0xC13400, MidiSeqSongsTable, sMidiSeqSongs_C13400, {});
ALIVE_VAR(1, 0xbd1cf4, int, sMidi_Inited_dword_BD1CF4, 0);
ALIVE_VAR(1, 0xbd1cec, unsigned int, sMidiTime_BD1CEC, 0);
ALIVE_VAR(1, 0xbd1ce8, BOOL, sSoundDatIsNull_BD1CE8, 0);
ALIVE_VAR(1, 0xbd1ce4, char, sbDisableSeqs_BD1CE4, 0);
ALIVE_VAR(1, 0x578E20, DWORD, sLastTime_578E20, 0xFFFFFFFF);
ALIVE_VAR(1, 0xbd1cf0, DWORD, sMidi_WaitUntil_BD1CF0, 0);

EXPORT VabHeader* GetVabHeaders()
{
    return *spVabHeaders_C13160;
}

EXPORT BYTE* GetVagCounts()
{
    return sVagCounts_BE6144;
}

EXPORT ConvertedVagTable& GetConvertedVagTable()
{
    return sConvertedVagTable_BEF160;
}

EXPORT SoundEntryTable& GetSoundEntryTable()
{
    return sSoundEntryTable16_BE6160;
}

template<typename T> T SwapBytes(T value);

template<>
inline WORD SwapBytes<WORD>(WORD value)
{
    return (value >> 8) | (value << 8);
}

template<>
inline unsigned SwapBytes<unsigned>(unsigned value)
{
    return unsigned(SwapBytes<WORD>(static_cast<WORD>(value)) << 16) | SwapBytes<WORD>(static_cast<WORD>(value >> 16));
}


ALIVE_VAR(1, 0xbd1ce0, IO_FileHandleType, sSoundDatFileHandle_BD1CE0, nullptr);
ALIVE_VAR(1, 0xbd1cfc, BYTE, sControllerValue_BD1CFC, 0);



EXPORT void CC MIDI_ADSR_Update_4FDCE0();
EXPORT __int16 CC MIDI_PitchBend_4FDEC0(__int16 field4_match, __int16 pitch);
EXPORT BYTE CC MIDI_ReadByte_4FD6B0(MIDI_SeqSong* pData);
EXPORT void CC MIDI_Read_SEQ_Header_4FD870(BYTE** pSrc, SeqHeader* pDst, unsigned int size);
EXPORT void CC MIDI_SetTempo_4FDB80(__int16 idx, __int16 kZero, __int16 tempo);
EXPORT signed int CC MIDI_Set_Volume_4FDE80(MIDI_Channel* pData, int vol);
EXPORT void CC MIDI_SkipBytes_4FD6C0(MIDI_SeqSong* pData, int length);
EXPORT int CC MIDI_Stop_Existing_Single_Note_4FCFF0(int VabIdAndProgram, int note);
EXPORT void CC MIDI_Wait_4FCE50();

void SsExt_CloseAllVabs()
{
    for (int i = 0; i < kMaxVabs; i++)
    {
        if (sVagCounts_BE6144[i] > 0)
        {
            SsVabClose_4FC5B0(i);
        }
    }
}

void SsExt_StopPlayingSamples()
{
    for (int i = 0; i < kNumChannels; i++)
    {
        if (sMidi_Channels_C14080.channels[i].field_1C_adsr.field_3_state)
        {
            SND_Stop_Sample_At_Idx_4EFA90(sMidi_Channels_C14080.channels[i].field_0_sound_buffer_field_4);
        }
    }
}

EXPORT void CC SSInit_4FC230()
{
    sGlobalVolumeLevel_right_BD1CDE = 127;
    sGlobalVolumeLevel_left_BD1CDC = 127;

    SND_CreateDS_4EEAA0(22050u, 16, 1);
}

EXPORT void CC SpuInitHot_4FC320()
{
    sMidi_Inited_dword_BD1CF4 = 1;
    sMidiTime_BD1CEC = SYS_GetTicks();
}

EXPORT void SsEnd_4FC350()
{
    sMidi_Inited_dword_BD1CF4 = 0;
}

EXPORT void CC SsSetMVol_4FC360(__int16 left, __int16 right)
{
    sGlobalVolumeLevel_left_BD1CDC = left;
    sGlobalVolumeLevel_right_BD1CDE = right;
}

EXPORT DWORD* CC SND_SoundsDat_Get_Sample_Offset_4FC3D0(VabHeader *pVabHeader, VabBodyRecord *pBodyRecords, int idx)
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
EXPORT int CC SND_SoundsDat_Get_Sample_Len_4FC400(VabHeader *pVabHeader, VabBodyRecord *pVabBody, int idx)
{
    int result; // eax

    if (pVabHeader && idx >= 0)
    {
        result = (signed int)(8 * *(SND_SoundsDat_Get_Sample_Offset_4FC3D0(pVabHeader, pVabBody, idx) - 2)) / 16;// -2 = field_0_length_or_duration
    }
    else
    {
        result = -1;
    }
    return result;
}

// TODO: Reverse/refactor properly
EXPORT int CC sub_4FC440(VabHeader *pVabHeader, VabBodyRecord* pVabBody, int idx)
{
    return *(SND_SoundsDat_Get_Sample_Offset_4FC3D0(pVabHeader, pVabBody, idx) - 1);// -1 = field_4_unused
}

// TODO: Reverse/refactor properly
EXPORT BOOL CC sub_4FC470(VabHeader *pVabHeader, VabBodyRecord* pVabBody, int idx)
{
    return sub_4FC440(pVabHeader, pVabBody, idx) < 0;
}

EXPORT int CC SND_SoundsDat_Read_4FC4E0(VabHeader *pVabHeader, VabBodyRecord *pVabBody, int idx, void *pBuffer)
{
    const int sampleOffset = *SND_SoundsDat_Get_Sample_Offset_4FC3D0(pVabHeader, pVabBody, idx); // = field_8_fileOffset
    const int sampleLen = SND_SoundsDat_Get_Sample_Len_4FC400(pVabHeader, pVabBody, idx);
    if (sampleOffset == -1 || !sSoundDatFileHandle_BD1CE0)
    {
        return 0;
    }

    IO_Seek(sSoundDatFileHandle_BD1CE0, sampleOffset, 0);
    IO_Read(sSoundDatFileHandle_BD1CE0, pBuffer, 2 * sampleLen, 1u);

    return sampleLen;
}

EXPORT void CC SsVabClose_4FC5B0(int vabId)
{
    SsUtAllKeyOff_4FDFE0(0); // TODO: Check argument ??

    if (sVagCounts_BE6144[vabId] - 1 >= 0)
    {
        // Free backwards
        for (int i = sVagCounts_BE6144[vabId] - 1; i >= 0; i--)
        {
            SND_Free_4EFA30(&sSoundEntryTable16_BE6160.table[vabId][i]);
        }
    }

    sVagCounts_BE6144[vabId] = 0;
    sProgCounts_BDCD64[vabId] = 0;
}


EXPORT __int16 CC SsVabOpenHead_4FC620(VabHeader* pVabHeader)
{
    if (!pVabHeader)
    {
        return -1;
    }

    SsSeqCalledTbyT_4FDC80();

    const int vab_id = pVabHeader->field_8_id;
    assert(vab_id < 4);
    SsVabClose_4FC5B0(vab_id);
    spVabHeaders_C13160[vab_id] = pVabHeader;
    if (sVagCounts_BE6144[vab_id] > 0)
    {
        SsVabClose_4FC5B0(vab_id);
    }

    int numVags = pVabHeader->field_16_num_vags;
    if (numVags > 256)
    {
        numVags = 255;
    }

    sVagCounts_BE6144[vab_id] = static_cast<BYTE>(numVags);
    sProgCounts_BDCD64[vab_id] = static_cast<BYTE>(pVabHeader->field_12_num_progs);
    memset(s512_byte_C13180.field_0[vab_id], 0, sizeof(char[128]));
    VagAtr* pVagAttr = (VagAtr *)&pVabHeader[1];
    memset(&sConvertedVagTable_BEF160.table[vab_id][0][0], 0, sizeof(Converted_Vag[128][16]));

    for (int i = 0; i < pVabHeader->field_12_num_progs; i++)
    {
        for (int toneCounter = 0; toneCounter < 16; toneCounter++)
        {
            if (pVagAttr->field_2_vol > 0)
            {
                Converted_Vag* pData = &sConvertedVagTable_BEF160.table[vab_id][pVagAttr->field_14_prog][toneCounter];

                pData->field_F_prog = static_cast<BYTE>(pVagAttr->field_14_prog);
                pData->field_10_vag = LOBYTE(pVagAttr->field_16_vag) - 1;
                pData->field_C = 0;
                pData->field_D_vol = pVagAttr->field_2_vol;
                pData->field_E_priority = pVagAttr->field_0_priority;
                pData->field_8_min = pVagAttr->field_6_min;
                pData->field_9_max = pVagAttr->field_7_max;

                const __int16 centre = pVagAttr->field_4_centre;
                pData->field_A_shift_cen = 2 * (pVagAttr->field_5_shift + (centre << 7));

                float sustain_level = static_cast<float>((2 * (~(unsigned __int8)pVagAttr->field_10_adsr1 & 0xF)));

                pData->field_0_adsr_attack = std::min(static_cast<WORD>((powf(2.0f, ((pVagAttr->field_10_adsr1 >> 8) & 0x7F) * 0.25f) * 0.09f)), static_cast<WORD>(32767));
                pData->field_4_adsr_decay = static_cast<WORD>((((pVagAttr->field_10_adsr1 >> 4) & 0xF) / 15.0f) * 16.0);
                pData->field_2_adsr_sustain_level = std::min(static_cast<WORD>((sustain_level / 15.0f) * 600.0), static_cast<WORD>(32767));
                pData->field_6_adsr_release = std::min(static_cast<WORD>(pow(2, pVagAttr->field_12_adsr2 & 0x1F) * 0.045f), static_cast<WORD>(32767));

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
    return static_cast<short>(vab_id);
}

// Loads sounds dat to memory
EXPORT void CC SsVabTransBody_4FC840(VabBodyRecord* pVabBody, __int16 vabId)
{
    if (vabId < 0)
    {
        return;
    }

    sSoundDatFileHandle_BD1CE0 = IO_Open("sounds.dat", "rb");
    sSoundDatIsNull_BD1CE8 = sSoundDatFileHandle_BD1CE0 == nullptr;

    assert(vabId < 4);
    VabHeader* pVabHeader = spVabHeaders_C13160[vabId];
    const int vagCount = sVagCounts_BE6144[vabId];
    for (int i = 0; i < vagCount; i++)
    {
        SoundEntry* pEntry = &sSoundEntryTable16_BE6160.table[vabId][i];
        memset(pEntry, 0, sizeof(SoundEntry));

        if (!(i & 7))
        {
            SsSeqCalledTbyT_4FDC80();
        }


        int sampleLen = SND_SoundsDat_Get_Sample_Len_4FC400(pVabHeader, pVabBody, i);
        if (sampleLen < 4000 && !sub_4FC470(pVabHeader, pVabBody, i))
        {
            sampleLen *= 2;
        }

        if (sampleLen > 0)
        {
            // Find matching converted vag to set field_C / field_6_adsr
            const int unused_field = sub_4FC470(pVabHeader, pVabBody, i);
            const BYTE unused_copy = unused_field != 0 ? 4 : 0;
            for (int prog = 0; prog < 128; prog++)
            {
                for (int tone = 0; tone < 16; tone++)
                {
                    Converted_Vag* pVag = &sConvertedVagTable_BEF160.table[vabId][prog][tone];
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
            if (SND_New_4EEFF0(pEntry, sampleLen, 44100, 16, 0) == 0)
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
                        SND_Load_4EF680(pEntry, pTempBuffer, sampleLen);
                    }
                    ae_free_4F4EA0(pTempBuffer);
                }
            }
        }
    }

    if (sSoundDatFileHandle_BD1CE0)
    {
        IO_Close(sSoundDatFileHandle_BD1CE0);
        sSoundDatFileHandle_BD1CE0 = nullptr;
    }
}

EXPORT int CC MIDI_Invert_4FCA40(int /*not_used*/, int value)
{
    return 127 - value;
}


EXPORT signed int CC MIDI_Allocate_Channel_4FCA50(int /*not_used*/, int priority)
{
    int lowestEndTime = -999999;
    unsigned int timeMod24 = sMidiTime_BD1CEC % 24;
    for (int i = 0; i < 24; i++)
    {
        if (sMidi_Channels_C14080.channels[i].field_1C_adsr.field_3_state == 0)
        {
            return i;
        }
        else
        {
            const int inverted = MIDI_Invert_4FCA40(sMidi_Channels_C14080.channels[i].field_4_priority, sMidi_Channels_C14080.channels[i].field_8_left_vol);
            if (inverted > lowestEndTime)
            {
                timeMod24 = i;
                lowestEndTime = inverted;
            }
        }
    }

    // Try to find a channel that isn't playing anything
    for (int i = 0; i < 24; i++)
    {
        if (SND_Get_Buffer_Status_4EE8F0(sMidi_Channels_C14080.channels[i].field_0_sound_buffer_field_4) == 0)
        {
            sMidi_Channels_C14080.channels[i].field_1C_adsr.field_3_state = 0;
            return i;
        }
    }

    // Take the channel which has sound that is ending soonest
    int idx = timeMod24;
    if (priority < sMidi_Channels_C14080.channels[idx].field_4_priority)
    {
        return -1;
    }
    SND_Stop_Sample_At_Idx_4EFA90(sMidi_Channels_C14080.channels[idx].field_0_sound_buffer_field_4);
    return idx;
}


EXPORT int CC MIDI_PlayMidiNote_4FCB30(int vabId, int program, int note, int leftVolume, int rightVolume, int volume)
{
    const int noteKeyNumber = (note >> 8) & 127;
    int leftVol2 = leftVolume;
    int rightVol2 = rightVolume;

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

    if (sVagCounts_BE6144[vabId] == 0)
    {
        return 0;
    }

    int usedChannelBits = 0;
    for (int i = 0; i < 16; i++)
    {
        Converted_Vag* pVagIter = &sConvertedVagTable_BEF160.table[vabId][program][i];
        if (pVagIter->field_D_vol > 0 && pVagIter->field_8_min <= noteKeyNumber && noteKeyNumber <= pVagIter->field_9_max)
        {
            const int vagVol = pVagIter->field_D_vol;
            int panLeft = (volume * leftVol2 * vagVol * sGlobalVolumeLevel_left_BD1CDC) >> 21;
            int panRight = (volume * rightVol2 * vagVol * sGlobalVolumeLevel_right_BD1CDE) >> 21;
            const unsigned int playFlags = (((unsigned int)pVagIter->field_C) >> 2) & 1;

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
                if ((((unsigned int)pVagIter->field_C >> 2)) & 1)
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
                int maxPan = panRight;
                if (panLeft >= panRight)
                {
                    maxPan = panLeft;
                }
                const int midiChannel = MIDI_Allocate_Channel_4FCA50(maxPan, pVagIter->field_E_priority);
                if (midiChannel >= 0)
                {
                    MIDI_Channel* pChannel = &sMidi_Channels_C14080.channels[midiChannel];
                    const BOOL bUnknown = pVagIter->field_0_adsr_attack || pVagIter->field_2_adsr_sustain_level || pVagIter->field_4_adsr_decay != 16 || pVagIter->field_6_adsr_release >= 33u;
                    pChannel->field_C_vol = maxPan;
                    if (bUnknown)
                    {
                        pChannel->field_1C_adsr.field_3_state = 1;

                        pChannel->field_1C_adsr.field_4_attack = static_cast<unsigned short>((pVagIter->field_0_adsr_attack * (127 - volume)) / 64);
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
                    pChannel->field_14_time = sMidiTime_BD1CEC;
                    pChannel->field_1C_adsr.field_0_seq_idx = static_cast<unsigned char>(vabId);
                    pChannel->field_1C_adsr.field_1_program = static_cast<unsigned char>(program);
                    pChannel->field_1C_adsr.field_2_note_byte1 = static_cast<unsigned char>(noteKeyNumber);

#if ORIGINAL_PS1_BEHAVIOR

                    // Note: 1.059463094359 is the 12th root of 2.
                    // https://en.wikipedia.org/wiki/Twelfth_root_of_two

                    // Uses the correct way of determining the pitch float.
                    // Almost identical to PS1 versions pitches (at least from PS1 emulators)
                    // Todo: check real ps1 hardware.
                    pChannel->field_10_freq = static_cast<float>(pow(1.059463094359, (double)((note / 256.0) - (pVagIter->field_A_shift_cen / 256.0))));

                    // This way seems to be more accurate to ps1 sound, but its actually not correct. So ps1 was limited because floats?
                    // pChannel->field_10_float = static_cast<float>(pow(1.059463094359, (double)((note / 256) - (pVagIter->field_A_shift_cen / 256))));
#else
                    pChannel->field_10_freq = static_cast<float>(pow(1.059463094359, (double)(note - pVagIter->field_A_shift_cen) * 0.00390625));
#endif

                    if (sMidi_WaitUntil_BD1CF0)
                    {
                        MIDI_Wait_4FCE50();
                    }

                    SND_PlayEx_4EF740(
                        &sSoundEntryTable16_BE6160.table[vabId][pVagIter->field_10_vag],
                        panLeft,
                        panRight,
                        pChannel->field_10_freq, // freq
                        pChannel,
                        playFlags,
                        pVagIter->field_E_priority);


                    if (program == 4 || program == 5 || program == 8 || program == 23 || program == 24 || program == 25)
                    {
                        sMidi_WaitUntil_BD1CF0 = SYS_GetTicks() + 10;
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
    while (SYS_GetTicks() < sMidi_WaitUntil_BD1CF0)
    {

    }
    sMidi_WaitUntil_BD1CF0 = 0;
}


EXPORT int CC MIDI_PlayerPlayMidiNote_4FCE80(int vabId, int program, int note, int leftVol, int rightVol, int volume)
{
    if (sSoundDatIsNull_BD1CE8)
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


EXPORT int CC SsVoKeyOn_4FCF10(int vabIdAndProgram, int pitch, unsigned __int16 leftVol, unsigned __int16 rightVol)
{
    MIDI_Stop_Existing_Single_Note_4FCFF0((vabIdAndProgram & 127) | (((vabIdAndProgram >> 8) & 31) << 8), pitch);

    if (sSoundDatIsNull_BD1CE8)
    {
        return 0;
    }

    const int channelBits = MIDI_PlayMidiNote_4FCB30((vabIdAndProgram >> 8) & 31, vabIdAndProgram & 127, pitch, leftVol, rightVol, 96);

    for (int idx = 0; idx < kNumChannels; idx++)
    {
        MIDI_Channel* pChannel = &sMidi_Channels_C14080.channels[idx];
        if ((1 << idx) & channelBits)
        {
            pChannel->field_1C_adsr.field_C = 0xFFFF; // or both -1, but they appear to be unsigned ??
            pChannel->field_1C_adsr.field_E_ref_count = 0xFF;
        }
    }

    return channelBits;
}


EXPORT int CC MIDI_Stop_Existing_Single_Note_4FCFF0(int VabIdAndProgram, int note)
{
    const int vabId = (VabIdAndProgram >> 8) & 31;
    if (!sVagCounts_BE6144[vabId])
    {
        return 0;
    }

    short i = 0;
    for (i = 0; i < kNumChannels; i++)
    {
        MIDI_Channel* pChannel = &sMidi_Channels_C14080.channels[i];
        MIDI_ADSR_State* pSub = &pChannel->field_1C_adsr;
        if (pSub->field_3_state
            && pSub->field_0_seq_idx == vabId
            && pSub->field_1_program == (VabIdAndProgram & 127)
            && pSub->field_2_note_byte1 == ((note >> 8) & 127))
        {
            SsUtKeyOffV_4FE010(i);
            return 0;
        }
    }
    return 0;
}

// TODO: Removed 4FD0C0

EXPORT int CC MIDI_Read_Var_Len_4FD0D0(MIDI_SeqSong* pMidiStru)
{
    int ret = 0;
    BYTE midiByte = 0;
    for (int i = 0; i < 4; ++i)
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
EXPORT signed int CC MIDI_ParseMidiMessage_4FD100(int idx)
{
    //NOT_IMPLEMENTED();

    int idx2; // ebp
    MIDI_SeqSong *pCtx; // esi
    unsigned __int8 midiByte1; // al
    int midiByte1_copy; // ebx
    unsigned __int8 metaEvent; // bl
    int oldLoopCount; // eax
    int newLoopCount; // eax
    int tempoChange; // eax
    char tempoByte3; // bl
    char tempoByte2; // ST2B_1
    unsigned __int8 fullTempo; // dl
    int v12; // eax
    unsigned __int16 v13; // cx
    int v14; // ecx
    unsigned int cmd; // ebx
    unsigned int v16; // eax
    int v17; // ecx
    MIDI_ProgramVolume  *v18; // ecx
    signed int v19; // ebx
    MIDI_ADSR_State *pSubChan2; // esi
    char v21; // bl
    char channelIdx_1; // cl

    MIDI_ADSR_State *pSubChan1; // esi
    unsigned __int8 refCount1; // dl
    MIDI_ADSR_State *pSub1; // esi
    int v29; // edi
    __int16 v31 = 0; // bp
    unsigned __int8 v33; // cl
    int v34; // eax
    void(CC *pFn)(int, DWORD, DWORD); // eax
    BYTE *v36; // eax
    char v37; // al
    int v38; // eax
    unsigned int v39; // ecx
    unsigned int v40; // edi
    int v42; // [esp+14h] [ebp-1Ch]
    MIDI_ProgramVolume *v43; // [esp+18h] [ebp-18h]
    int v45; // [esp+20h] [ebp-10h]
    int leftVol; // [esp+2Ch] [ebp-4h]
    int v47; // [esp+2Ch] [ebp-4h]
    MIDI_ProgramVolume* v32;

    idx2 = idx;
    pCtx = &sMidiSeqSongs_C13400.table[idx];
    if (sMidiSeqSongs_C13400.table[idx].field_4_time > sMidiTime_BD1CEC)
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
            else if (midiByte1 == 0xFF)             // Sysex len
            {
                metaEvent = MIDI_ReadByte_4FD6B0(pCtx);
                if (metaEvent == 0x2F)                // End of track
                {
                    oldLoopCount = sMidiSeqSongs_C13400.table[idx2].field_18_repeatCount;
                    if (oldLoopCount)
                    {
                        newLoopCount = oldLoopCount - 1;
                        sMidiSeqSongs_C13400.table[idx2].field_18_repeatCount = newLoopCount;
                        if (!newLoopCount)
                        {
                            SsSeqStop_4FD9C0(static_cast<short>(idx));
                            return 1;
                        }
                    }
                    pCtx->field_0_seq_data = sMidiSeqSongs_C13400.table[idx2].field_1C_pSeqData;
                }
                else
                {
                    tempoChange = MIDI_Read_Var_Len_4FD0D0(pCtx);
                    if (tempoChange)
                    {
                        if (metaEvent == 0x51)            // Tempo in microseconds per quarter note (24-bit value)
                        {
                            tempoByte3 = MIDI_ReadByte_4FD6B0(pCtx) << 16;
                            tempoByte2 = MIDI_ReadByte_4FD6B0(pCtx) << 8;
                            fullTempo = tempoByte3 | tempoByte2 | MIDI_ReadByte_4FD6B0(pCtx);
                            MIDI_SetTempo_4FDB80(static_cast<short>(idx), 0, static_cast<short>(fullTempo));
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
        sMidiSeqSongs_C13400.table[idx2].field_2A_running_status = static_cast<unsigned char>(midiByte1_copy);// running status ?

    handle_next_event:

        v13 = v31 & 0x00FF;
        v13 |= (WORD)(midiByte1 << 8);
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
        case 0x80u:                               // Note off

            v29 = v16 & 15;
            v32 = &sMidiSeqSongs_C13400.table[idx2].field_32_progVols[v29];
            for (int i = 0; i < 24; i++)
            {
                pSub1 = &sMidi_Channels_C14080.channels[i].field_1C_adsr;
                if (pSub1->field_3_state)
                {
                    if (pSub1->field_0_seq_idx == sMidiSeqSongs_C13400.table[idx].field_seq_idx
                        && pSub1->field_1_program == v32->field_0_program // or field_2_not_inited ??
                        && pSub1->field_C == v29 + 16 * idx
                        && pSub1->field_2_note_byte1 == BYTE1(v16))
                    {
                        v33 = pSub1->field_E_ref_count - 1;
                        pSub1->field_E_ref_count = v33;
                        if (!v33)
                        {
                            SsUtKeyOffV_4FE010(v31);
                            BYTE1(v16) = BYTE1(v42);
                            pSub1->field_C = 0;
                        }
                    }
                }
            }
            idx2 = idx;
            break;
        case 0x90u:                               // Note on
            v17 = v16 & 15;
            v45 = v17;
            v18 = &sMidiSeqSongs_C13400.table[idx2].field_32_progVols[v17];
            v43 = v18;
            leftVol = (signed __int16)((unsigned int)(v18->field_1_left_vol * sMidiSeqSongs_C13400.table[idx2].field_C_volume) >> 7);
            if (v16 >> 16)
            {
                v19 = 0;

                for (int i = 0; i < 24; i++)
                {
                    pSubChan2 = &sMidi_Channels_C14080.channels[i].field_1C_adsr;
                    if (pSubChan2->field_3_state
                        && pSubChan2->field_0_seq_idx == sMidiSeqSongs_C13400.table[idx2].field_seq_idx
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

                v21 = static_cast<char>(v19 + 1);
                v47 = MIDI_PlayerPlayMidiNote_4FCE80(
                    sMidiSeqSongs_C13400.table[idx2].field_seq_idx,
                    v18->field_0_program,
                    v16 & 0xFF00,
                    leftVol,
                    v18->field_2_right_vol,
                    v16 >> 16);
                channelIdx_1 = 0;

                for (int i = 0; i < 24; i++)
                {
                    if ((1 << channelIdx_1) & v47)
                    {
                        sMidi_Channels_C14080.channels[i].field_1C_adsr.field_C = static_cast<WORD>(16 * idx + (v42 & 0xF));
                        sMidi_Channels_C14080.channels[i].field_1C_adsr.field_E_ref_count = v21;
                    }
                    ++channelIdx_1;
                }
            }
            else
            {
                for (short i = 0; i < 24; i++)
                {
                    pSubChan1 = &sMidi_Channels_C14080.channels[i].field_1C_adsr;
                    if (pSubChan1->field_3_state)
                    {
                        if (pSubChan1->field_0_seq_idx == sMidiSeqSongs_C13400.table[idx2].field_seq_idx
                            && pSubChan1->field_1_program == v18->field_0_program
                            && pSubChan1->field_C == v45 + 16 * idx
                            && pSubChan1->field_2_note_byte1 == BYTE1(v16))
                        {
                            refCount1 = pSubChan1->field_E_ref_count - 1;
                            pSubChan1->field_E_ref_count = refCount1;
                            if (!refCount1)
                            {
                                SsUtKeyOffV_4FE010(i);
                                BYTE1(v16) = BYTE1(v42);
                                v18 = v43;
                                pSubChan1->field_C = 0;
                            }
                        }
                    }
                }
            }
            break;
        case 0xB0u:                               // Controller change
            v34 = (cmd >> 8) & 0x7F;
            if (v34 != 6 && v34 != 0x26)
            {
                if (v34 == 0x63)
                {
                    sControllerValue_BD1CFC = BYTE2(cmd);
                }
                break;
            }

            switch (sControllerValue_BD1CFC)
            {
            case 20:                              // Set loop
                sMidiSeqSongs_C13400.table[idx2].field_24_loop_start = pCtx->field_0_seq_data;
                sMidiSeqSongs_C13400.table[idx2].field_2C_loop_count = BYTE2(cmd);
                break;
            case 30:                              // Loop
                v36 = sMidiSeqSongs_C13400.table[idx2].field_24_loop_start;
                if (v36)
                {
                    if (sMidiSeqSongs_C13400.table[idx2].field_2C_loop_count > 0)
                    {
                        pCtx->field_0_seq_data = v36;
                        v37 = sMidiSeqSongs_C13400.table[idx2].field_2C_loop_count;
                        if (v37 < 127)
                        {
                            sControllerValue_BD1CFC = 0;
                            sMidiSeqSongs_C13400.table[idx2].field_2C_loop_count = v37 - 1;
                            goto next_time_stamp;
                        }
                    }
                }
                break;
            case 40:
                pFn = (void(CC *)(int, DWORD, DWORD))sMidiSeqSongs_C13400.table[idx2].field_20_fn_ptr;
                if (pFn)
                {
                    pFn(idx, 0, BYTE2(cmd));
                    sControllerValue_BD1CFC = 0;
                    goto next_time_stamp;
                }
                break;
            }
            sControllerValue_BD1CFC = 0;
            break;

        case 0xC0u:                               // Program change
        {
            sMidiSeqSongs_C13400.table[idx2].field_32_progVols[v16 & 0xF].field_0_program = BYTE1(v16);
        }
        break;

        case 0xE0u:                               // Pitch bend
        {
            MIDI_PitchBend_4FDEC0(
                sMidiSeqSongs_C13400.table[idx2].field_32_progVols[v16 & 0xF].field_0_program,
                static_cast<short>(((v16 >> 8) - 0x4000) >> 4));
        }
        break;
        default:
            break;
        }

    next_time_stamp:

        pCtx = &sMidiSeqSongs_C13400.table[idx2];
        // Next time stamp
        v38 = MIDI_Read_Var_Len_4FD0D0(&sMidiSeqSongs_C13400.table[idx2]);
        if (v38)
        {
            v39 = sMidiTime_BD1CEC;
            v40 = v38 * sMidiSeqSongs_C13400.table[idx2].field_14_tempo / 1000u + sMidiSeqSongs_C13400.table[idx2].field_4_time;
            sMidiSeqSongs_C13400.table[idx2].field_4_time = v40;
            if (v40 > v39)
            {
                return 1;
            }
        }
    } // Loop end

    if (sMidiSeqSongs_C13400.table[idx2].field_2A_running_status)
    {
        midiByte1_copy = (unsigned __int8)sMidiSeqSongs_C13400.table[idx2].field_2A_running_status;
        goto handle_next_event;
    }
    return 0;
}

EXPORT BYTE CC MIDI_ReadByte_4FD6B0(MIDI_SeqSong* pData)
{
    return *pData->field_0_seq_data++;
}

EXPORT void CC MIDI_SkipBytes_4FD6C0(MIDI_SeqSong* pData, int length)
{
    pData->field_0_seq_data += length;
}

EXPORT __int16 CC SsSeqOpen_4FD6D0(BYTE* pSeqData, __int16 seqIdx)
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
    int freeIdx = 0;
    for (int i = 0; i < 32; i++)
    {
        if (!sMidiSeqSongs_C13400.table[i].field_0_seq_data)
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
    memset(&sMidiSeqSongs_C13400.table[freeIdx], 0, sizeof(MIDI_SeqSong));
    for (int i = 0; i < 16; i++)
    {
        sMidiSeqSongs_C13400.table[freeIdx].field_32_progVols[i].field_1_left_vol = 112;
        sMidiSeqSongs_C13400.table[freeIdx].field_32_progVols[i].field_2_right_vol = 64;
    }

    // Set data based on SEQ header
    sMidiSeqSongs_C13400.table[freeIdx].field_2E_seqAccessNum = -1;
    sMidiSeqSongs_C13400.table[freeIdx].field_10_quaterNoteRes = SwapBytes(seqHeader.field_8_resolution_of_quater_note);
    sMidiSeqSongs_C13400.table[freeIdx].field_31_timeSignatureBars2 = seqHeader.field_E_time_signature_beats;
    sMidiSeqSongs_C13400.table[freeIdx].field_30_timeSignatureBars = seqHeader.field_D_time_signature_bars;

    const int quaterNoteRes = sMidiSeqSongs_C13400.table[freeIdx].field_10_quaterNoteRes;
    int calculatedTempo = 0;
    // TODO: Figure out what these tempo calcs are actually doing
    if (quaterNoteRes >= 0)
    {
        DWORD tempo24Bit = (seqHeader.field_A_tempo[0] << 16) | (seqHeader.field_A_tempo[1] << 8) | (seqHeader.field_A_tempo[2]);
        tempo24Bit = tempo24Bit / quaterNoteRes;
        calculatedTempo = ((((tempo24Bit >> 1) + 15000) / tempo24Bit >> 1) + 15000) / (((tempo24Bit >> 1) + 15000) / tempo24Bit);
    }
    else
    {
        const int calculatedQuaterNoteRes = -(static_cast<unsigned __int8>(quaterNoteRes) * (-sMidiSeqSongs_C13400.table[freeIdx].field_10_quaterNoteRes >> 8));
        sMidiSeqSongs_C13400.table[freeIdx].field_10_quaterNoteRes = calculatedQuaterNoteRes;
        calculatedTempo = -1000000 / calculatedQuaterNoteRes;
    }

    sMidiSeqSongs_C13400.table[freeIdx].field_14_tempo = calculatedTempo;
    sMidiSeqSongs_C13400.table[freeIdx].field_1C_pSeqData = pSeqData;
    sMidiSeqSongs_C13400.table[freeIdx].field_0_seq_data = pSeqData;
    sMidiSeqSongs_C13400.table[freeIdx].field_C_volume = 112;
    sMidiSeqSongs_C13400.table[freeIdx].field_seq_idx = seqIdx;

    return static_cast<short>(freeIdx);
}

EXPORT void CC MIDI_Read_SEQ_Header_4FD870(BYTE** pSrc, SeqHeader* pDst, unsigned int size)
{
    memcpy(pDst, *pSrc, size);
    (*pSrc) += size;
}


EXPORT void CC SsSeqClose_4FD8D0(__int16 idx)
{
    SsSeqStop_4FD9C0(idx);
    sMidiSeqSongs_C13400.table[idx].field_C_volume = 0;
    sMidiSeqSongs_C13400.table[idx].field_0_seq_data = 0;
    sMidiSeqSongs_C13400.table[idx].field_1C_pSeqData = nullptr;
}


EXPORT void CC SsSeqPlay_4FD900(unsigned __int16 idx, char repeatMode, __int16 repeatCount)
{
    if (idx < 32u)
    {
        MIDI_SeqSong& rec = sMidiSeqSongs_C13400.table[idx];
        if (rec.field_1C_pSeqData)
        {
            rec.field_4_time = sMidiTime_BD1CEC;
            rec.field_8_playTimeStamp = sMidiTime_BD1CEC;
            rec.field_18_repeatCount = repeatCount;

            if (rec.field_2B_repeatMode != -1)
            {
                rec.field_0_seq_data = rec.field_1C_pSeqData;
                rec.field_2A_running_status = 0;
                unsigned int midiTime = MIDI_Read_Var_Len_4FD0D0(&rec);
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


EXPORT void CC SsSeqStop_4FD9C0(__int16 idx)
{
    if (sMidiSeqSongs_C13400.table[idx].field_0_seq_data)
    {
        sMidiSeqSongs_C13400.table[idx].field_2B_repeatMode = 0;
        if (sMidiSeqSongs_C13400.table[idx].field_2E_seqAccessNum >= 0)
        {
            SsSeqPlay_4FD900(sMidiSeqSongs_C13400.table[idx].field_2E_seqAccessNum, 1, 1);
            sMidiSeqSongs_C13400.table[idx].field_2E_seqAccessNum = -1;
        }
    }

    for (unsigned int i = 0; i < kNumChannels; i++)
    {
        unsigned int field_C = sMidi_Channels_C14080.channels[i].field_1C_adsr.field_C;
        field_C = field_C >> 4;
        if (field_C == static_cast<unsigned int>(idx))
        {
            SsUtKeyOffV_4FE010(static_cast<short>(i));
            sMidi_Channels_C14080.channels[i].field_1C_adsr.field_C = 0;
        }
    }
}

// TODO: Removed 4FDA40

// TODO: Removed 4FDA60

EXPORT unsigned __int16 CC SsIsEos_4FDA80(__int16 idx, __int16 kZero)
{
    if (!kZero)
    {
        if (sMidiSeqSongs_C13400.table[idx].field_0_seq_data)
        {
            if (sMidiSeqSongs_C13400.table[idx].field_2B_repeatMode)
            {
                return 1;
            }
        }
    }
    return 0;
}


EXPORT void CC SsSeqSetVol_4FDAC0(__int16 idx, __int16 volLeft, __int16 volRight)
{
    if (sMidiSeqSongs_C13400.table[idx].field_0_seq_data)
    {
        // TODO: Refactor
        sMidiSeqSongs_C13400.table[idx].field_C_volume = 112 * ((volRight + volLeft) >> 1) >> 7;
    }
}

// TODO: Removed 4FDB00

// TODO: Removed 4FDB40

EXPORT void CC MIDI_SetTempo_4FDB80(__int16 idx, __int16 kZero, __int16 tempo)
{
    if (!kZero)
    {
        if (sMidiSeqSongs_C13400.table[idx].field_10_quaterNoteRes >= 0) // TODO: BUG - will div zero?
        {
            sMidiSeqSongs_C13400.table[idx].field_14_tempo = tempo;
        }
        else
        {
            sMidiSeqSongs_C13400.table[idx].field_14_tempo = -1000000 / sMidiSeqSongs_C13400.table[idx].field_10_quaterNoteRes;
        }
    }
}

EXPORT void CC SsSetTickMode_4FDC20(int)
{
    // Stub
}

// TODO: Removed 4FDC30


EXPORT void CC SsSeqCalledTbyT_4FDC80()
{
    if (!sbDisableSeqs_BD1CE4)
    {
        const DWORD currentTime = SYS_GetTicks();
        sMidiTime_BD1CEC = currentTime;
        // First time or 30 passed?
        if (sLastTime_578E20 == 0xFFFFFFFF || (signed int)(currentTime - sLastTime_578E20) >= 30)
        {
            sLastTime_578E20 = currentTime;
            for (int i = 0; i < kNumChannels; i++)
            {
                if (sMidiSeqSongs_C13400.table[i].field_0_seq_data)
                {
                    if (sMidiSeqSongs_C13400.table[i].field_2B_repeatMode == 1)
                    {
                        MIDI_ParseMidiMessage_4FD100(i);
                    }
                }
            }
            MIDI_ADSR_Update_4FDCE0();
        }
    }
}

EXPORT void CC MIDI_ADSR_Update_4FDCE0()
{
    for (int i = 0; i < kNumChannels; i++)
    {
        MIDI_Channel* pChannel = &sMidi_Channels_C14080.channels[i];
        if (pChannel->field_1C_adsr.field_3_state)
        {
            signed int timeDiff1 = sMidiTime_BD1CEC - pChannel->field_14_time;
            signed int timeDiff2 = sMidiTime_BD1CEC - pChannel->field_14_time;

            if ((signed int)(sMidiTime_BD1CEC - pChannel->field_14_time) < 0)
            {
                timeDiff1 = 0;
                timeDiff2 = 0;
            }

            int timeDiffSquared = timeDiff1 * timeDiff1;
            switch (pChannel->field_1C_adsr.field_3_state + 1) // ADSR state ?
            {
            case 0:
                if (timeDiff1 > 90000)
                {
                    SsUtKeyOffV_4FE010(static_cast<short>(i));
                }
                break;
            case 2:
                if (timeDiff1 >= pChannel->field_1C_adsr.field_4_attack)
                {
                    pChannel->field_1C_adsr.field_3_state = 2;
                    pChannel->field_14_time += pChannel->field_1C_adsr.field_4_attack;
                    timeDiff1 -= pChannel->field_1C_adsr.field_4_attack;
                    // Fall through to case 3
                }
                else
                {
                    MIDI_Set_Volume_4FDE80(
                        pChannel,
                        (signed __int64)(((double)timeDiff2 / (double)pChannel->field_1C_adsr.field_4_attack
                            + (double)timeDiff2 / (double)pChannel->field_1C_adsr.field_4_attack
                            - (double)timeDiff2 / (double)pChannel->field_1C_adsr.field_4_attack * ((double)timeDiff2 / (double)pChannel->field_1C_adsr.field_4_attack))
                            * (double)pChannel->field_C_vol));
                    break;
                }
            case 3:
                if (timeDiff1 < pChannel->field_1C_adsr.field_6_sustain)
                {
                    const int v8 = pChannel->field_C_vol * (16 - pChannel->field_1C_adsr.field_8_decay) >> 4;
                    MIDI_Set_Volume_4FDE80(pChannel, pChannel->field_C_vol - timeDiffSquared * v8 / (pChannel->field_1C_adsr.field_6_sustain * pChannel->field_1C_adsr.field_6_sustain));
                    break;
                }
                pChannel->field_1C_adsr.field_3_state = 3;
                pChannel->field_14_time += pChannel->field_1C_adsr.field_6_sustain;
                timeDiff1 -= pChannel->field_1C_adsr.field_6_sustain;
                if (MIDI_Set_Volume_4FDE80(pChannel, pChannel->field_C_vol * pChannel->field_1C_adsr.field_8_decay >> 4))
                {
                    // Fall through to case 4
                }
                else
                {
                    break;
                }
            case 4:
                if (timeDiff1 > 15000)
                {
                    pChannel->field_1C_adsr.field_3_state = 4;
                    pChannel->field_14_time = sMidiTime_BD1CEC;
                    timeDiff1 = 0;
                    timeDiffSquared = 0;
                    pChannel->field_C_vol = pChannel->field_8_left_vol;
                    // Fall through to case 5
                }
                else
                {
                    break;
                }
            case 5:
                if (timeDiff1 >= pChannel->field_1C_adsr.field_A_release)
                {
                    pChannel->field_1C_adsr.field_3_state = 0;
                    SND_Stop_Sample_At_Idx_4EFA90(pChannel->field_0_sound_buffer_field_4);
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


EXPORT signed int CC MIDI_Set_Volume_4FDE80(MIDI_Channel* pData, int vol)
{
    if (pData->field_8_left_vol == vol)
    {
        return 1;
    }

    pData->field_8_left_vol = static_cast<char>(vol);

    if (!SND_Buffer_Set_Volume_4EFAD0(pData->field_0_sound_buffer_field_4, vol))
    {
        return 1;
    }

    pData->field_1C_adsr.field_3_state = 0;
    return 0;
}


EXPORT __int16 CC MIDI_PitchBend_4FDEC0(__int16 program, __int16 pitch)
{
    const float pitcha = pow(1.059463094359f, (float)pitch * 0.0078125f);
    for (int i = 0; i < kNumChannels; i++)
    {
        if (sMidi_Channels_C14080.channels[i].field_1C_adsr.field_1_program == program)
        {
            const float freq = pitcha * sMidi_Channels_C14080.channels[i].field_10_freq;
            SND_Buffer_Set_Frequency_4EFC00(sMidi_Channels_C14080.channels[i].field_C_vol, freq);
        }
    }
    return 0;
}


EXPORT __int16 CC SsUtChangePitch_4FDF70(__int16 voice, int /*vabId*/, int /*prog*/, __int16 old_note, __int16 old_fine, __int16 new_note, __int16 new_fine)
{
    const float freq = pow(1.059463094359f, (float)(new_fine + ((new_note - (signed int)old_note) << 7) - old_fine) * 0.0078125f);
    SND_Buffer_Set_Frequency_4EFC90(sMidi_Channels_C14080.channels[voice].field_0_sound_buffer_field_4, freq);
    return 0;
}


EXPORT void CC SsUtAllKeyOff_4FDFE0(int)
{
    // Stop all backwards
    short idx = kNumChannels - 1;
    do
    {
        SsUtKeyOffV_4FE010(idx--);
    } while (idx >= 0);
}


EXPORT __int16 CC SsUtKeyOffV_4FE010(__int16 idx)
{
    MIDI_Channel* pChannel = &sMidi_Channels_C14080.channels[idx];
    if (pChannel->field_1C_adsr.field_3_state)
    {
        pChannel->field_1C_adsr.field_3_state = 4;
        pChannel->field_C_vol = pChannel->field_8_left_vol;
        if (pChannel->field_1C_adsr.field_A_release < 300)
        {
            pChannel->field_1C_adsr.field_A_release = 300;
        }
        pChannel->field_14_time = sMidiTime_BD1CEC;
    }
    return 0;
}

EXPORT void SsVabTransCompleted_4FE060(int)
{
    // Stub
}

// TODO: Removed 4FE081

EXPORT void CC SsSetTableSize_4FE0B0(void*, int, int)
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
EXPORT void CC SsUtSetReverbType_4FE360(int)
{
    // Stub
}

EXPORT void SsUtSetReverbDepth_4FE380(int, int)
{
    // Stub
}

EXPORT void SpuClearReverbWorkArea_4FA690(int)
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
