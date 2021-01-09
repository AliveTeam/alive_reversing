#include "stdafx_ao.h"

#include "config.h"

#include "Midi.hpp"
#include "Function.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "ResourceManager.hpp"
#include "LvlArchive.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "AmbientSound.hpp"
#include "Sound.hpp"

#include "Sfx.hpp"
#include "../AliveLibAE/Sfx.hpp"

#ifdef _WIN32
#define NO_WAVE
#endif

struct IO_FileHandleType;

#include "../AliveLibAE/Sound/PsxSpuApi.hpp"
#include "../AliveLibAE/Sound/Midi.hpp"
#include "../AliveLibAE/Sound/Sound.hpp"
#include "../AliveLibAE/PathData.hpp"


namespace AO {

const int kSeqTableSizeAO = 164;

ALIVE_VAR(1, 0x9F12D8, SeqIds, sSeq_Ids_word_9F12D8, {});
ALIVE_VAR(1, 0x9F1DC4, WORD, sSnd_ReloadAbeResources_9F1DC4, 0);
ALIVE_VAR(1, 0x9F1DBC, OpenSeqHandle *, sSeqDataTable_9F1DBC, nullptr);
ALIVE_VAR(1, 0x9F1DC0, __int16, sSeqsPlaying_count_word_9F1DC0, 0);
ALIVE_VAR(1, 0x9F1DB8, SoundBlockInfo *, sLastLoadedSoundBlockInfo_9F1DB8, nullptr);
ALIVE_VAR(1, 0x4D0018, __int16, sSFXPitchVariationEnabled_4D0018, true);
ALIVE_VAR(1, 0x4D0000, short, sNeedToHashSeqNames_4D0000, 1);

// I think this is the burrrrrrrrrrrrrrrrrrrr loading sound
const SoundBlockInfo soundBlock = { "MONK.VH", "MONK.VB", -1, nullptr };
ALIVE_VAR(1, 0x4D0008, SoundBlockInfo, sMonkVh_Vb_4D0008, soundBlock);

class AOMidiVars : public IMidiVars
{
public:
    virtual SeqIds& sSeq_Ids_word() override
    {
        return sSeq_Ids_word_9F12D8;
    }

    virtual WORD& sSnd_ReloadAbeResources() override
    {
        return sSnd_ReloadAbeResources_9F1DC4;
    }

    virtual OpenSeqHandle*& sSeqDataTable() override
    {
        return sSeqDataTable_9F1DBC;
    }

    virtual __int16& sSeqsPlaying_count_word() override
    {
        return sSeqsPlaying_count_word_9F1DC0;
    }

    virtual ::SoundBlockInfo*& sLastLoadedSoundBlockInfo() override
    {
        return reinterpret_cast<::SoundBlockInfo*&>(sLastLoadedSoundBlockInfo_9F1DB8);
    }

    virtual __int16& sSFXPitchVariationEnabled() override
    {
        return sSFXPitchVariationEnabled_4D0018;
    }

    virtual short& sNeedToHashSeqNames() override
    {
        return sNeedToHashSeqNames_4D0000;
    }

    virtual ::SoundBlockInfo& sMonkVh_Vb() override
    {
        return reinterpret_cast<::SoundBlockInfo&>(sMonkVh_Vb_4D0008);
    }

    virtual int MidiTableSize() override
    {
        return kSeqTableSizeAO;
    }

    virtual signed __int16 FreeResource_Impl(BYTE* handle) override
    {
        return ResourceManager::FreeResource_Impl_4555B0(handle);
    }

    virtual BYTE** GetLoadedResource(DWORD type, DWORD resourceID, unsigned __int16 addUseCount, unsigned __int16 bLock) override
    {
        return ResourceManager::GetLoadedResource_4554F0(type, resourceID, addUseCount, bLock);
    }

    virtual signed __int16 FreeResource(BYTE** handle) override
    {
        return ResourceManager::FreeResource_455550(handle);
    }

    virtual BYTE** Allocate_New_Locked_Resource(DWORD type, DWORD id, DWORD size) override
    {
        return ResourceManager::Allocate_New_Locked_Resource_454F80(type, id, size);
    }

    virtual void LoadingLoop(__int16 bShowLoadingIcon) override
    {
        ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);
    }

    virtual void Reclaim_Memory(unsigned int size) override
    {
        ResourceManager::Reclaim_Memory_455660(size);
    }

    virtual BYTE** Alloc_New_Resource(DWORD type, DWORD id, DWORD size) override
    {
        return ResourceManager::Alloc_New_Resource_454F20(type, id, size);
    }

    virtual signed __int16 LoadResourceFile(const char* pFileName, ::Camera* pCamera) override
    {
        return ResourceManager::LoadResourceFileWrapper(pFileName, reinterpret_cast<Camera*>(pCamera));
    }

};

static AOMidiVars sAoMidiVars;

ALIVE_VAR(1, 0xA8918E, __int16, sGlobalVolumeLevel_right_A8918E, 0);
ALIVE_VAR(1, 0xA8918C, __int16, sGlobalVolumeLevel_left_A8918C, 0);
ALIVE_VAR(1, 0xABF8C0, VabUnknown, s512_byte_ABF8C0, {});
ALIVE_ARY(1, 0xA9289C, BYTE, kMaxVabs, sVagCounts_A9289C, {});
ALIVE_ARY(1, 0xA92898, BYTE, kMaxVabs, sProgCounts_A92898, {});
ALIVE_ARY(1, 0xABF8A0, VabHeader*, 4, spVabHeaders_ABF8A0, {});
ALIVE_VAR(1, 0xA9B8A0, ConvertedVagTable, sConvertedVagTable_A9B8A0, {});
ALIVE_VAR(1, 0xA928A0, SoundEntryTable, sSoundEntryTable16_A928A0, {});
ALIVE_VAR(1, 0xAC07C0, MidiChannels, sMidi_Channels_AC07C0, {});
ALIVE_VAR(1, 0xABFB40, MidiSeqSongsTable, sMidiSeqSongs_ABFB40, {});
ALIVE_VAR(1, 0xA89198, int, sMidi_Inited_dword_A89198, 0); 
ALIVE_VAR(1, 0xA89194, unsigned int, sMidiTime_A89194, 0);
ALIVE_VAR(1, 0xA89190, char, sbDisableSeqs_A89190, 0);
ALIVE_VAR(1, 0x4E8FD8, DWORD, sLastTime_4E8FD8, 0xFFFFFFFF);
ALIVE_VAR(1, 0xA8919C, BYTE, sControllerValue_A8919C, 0);

EXPORT signed int CC MIDI_ParseMidiMessage_49DD30(int idx);
EXPORT void CC SsUtKeyOffV_49EE50(__int16 idx);

class AOPsxSpuApiVars : public IPsxSpuApiVars
{
public:
    virtual __int16& sGlobalVolumeLevel_right() override
    {
        return sGlobalVolumeLevel_right_A8918E;
    }

    virtual __int16& sGlobalVolumeLevel_left() override
    {
        return sGlobalVolumeLevel_left_A8918C;
    }

    virtual VabUnknown& s512_byte() override
    {
        return s512_byte_ABF8C0;
    }

    virtual BYTE* sVagCounts() override
    {
        return sVagCounts_A9289C;
    }

    virtual BYTE* sProgCounts() override
    {
        return sProgCounts_A92898;
    }

    virtual VabHeader** spVabHeaders() override
    {
        return spVabHeaders_ABF8A0;
    }

    virtual ConvertedVagTable& sConvertedVagTable() override
    {
        return sConvertedVagTable_A9B8A0;
    }

    virtual SoundEntryTable& sSoundEntryTable16() override
    {
        return sSoundEntryTable16_A928A0;
    }

    virtual MidiChannels& sMidi_Channels() override
    {
        return sMidi_Channels_AC07C0;
    }

    virtual MidiSeqSongsTable& sMidiSeqSongs() override
    {
        return sMidiSeqSongs_ABFB40;
    }

    virtual int& sMidi_Inited_dword() override
    {
        return sMidi_Inited_dword_A89198;
    }

    virtual unsigned int& sMidiTime() override
    {
        return sMidiTime_A89194;
    }

    virtual BOOL& sSoundDatIsNull() override
    {
        return mSoundDatIsNull;
    }

    virtual char& sbDisableSeqs() override
    {
        return sbDisableSeqs_A89190;
    }

    virtual DWORD& sLastTime() override
    {
        return sLastTime_4E8FD8;
    }

    virtual DWORD& sMidi_WaitUntil() override
    {
        // Always 0 in AO cause it dont exist 
        return mMidi_WaitUntil;
    }

    virtual IO_FileHandleType& sSoundDatFileHandle() override
    {
        // Should never be called
        throw std::logic_error("The method or operation is not implemented.");
    }

    virtual BYTE& sControllerValue() override
    {
        return sControllerValue_A8919C;
    }

    virtual void MIDI_ParseMidiMessage(int idx) override
    {
        MIDI_ParseMidiMessage_49DD30(idx);
    }
    
    virtual void SsUtKeyOffV(int idx) override
    {
        SsUtKeyOffV_49EE50(static_cast<short>(idx));
    }
private:
    BOOL mSoundDatIsNull = FALSE; // Pretend we have sounds dat opened so AE funcs work
    DWORD mMidi_WaitUntil = 0;
};

static AOPsxSpuApiVars sAoSpuVars;

EXPORT void CC SND_Reset_476BA0()
{
    AE_IMPLEMENTED();
    SND_Reset_4C9FB0();
}

EXPORT void CC SsUtAllKeyOff_49EDE0(int mode)
{
    AE_IMPLEMENTED();
    SsUtAllKeyOff_4FDFE0(mode);
}

EXPORT void CC SND_Stop_All_Seqs_4774D0()
{
    AE_IMPLEMENTED();
    SND_Stop_All_Seqs_4CA850();
}

EXPORT void CC SsSeqCalledTbyT_49E9F0()
{
    AE_IMPLEMENTED();
    SsSeqCalledTbyT_4FDC80();
}

EXPORT signed int CC SND_New_492790(SoundEntry* pSnd, int sampleLength, int sampleRate, int bitsPerSample, int isStereo)
{
    AE_IMPLEMENTED();
    return SND_New_4EEFF0(pSnd, sampleLength, sampleRate, bitsPerSample, isStereo);
}

EXPORT int CC SND_Load_492F40(SoundEntry* pSnd, const void* pWaveData, int waveDataLen)
{
    AE_IMPLEMENTED();
    return SND_Load_4EF680(pSnd, pWaveData, waveDataLen);
}

EXPORT __int16 CC SsVabOpenHead_49CFB0(VabHeader* pVabHeader)
{
    AE_IMPLEMENTED();
    return SsVabOpenHead_4FC620(pVabHeader);
}

EXPORT void CC SND_Stop_Channels_Mask_4774A0(int mask)
{
    AE_IMPLEMENTED();
    SND_Stop_Channels_Mask_4CA810(mask);
}

EXPORT signed __int16 CC SND_SEQ_PlaySeq_4775A0(SeqId idx, int repeatCount, __int16 bDontStop)
{
    AE_IMPLEMENTED();
    return SND_SEQ_PlaySeq_4CA960(static_cast<unsigned __int16>(idx), static_cast<short>(repeatCount), bDontStop);
}

EXPORT void CC SND_Seq_Stop_477A60(SeqId idx)
{
    AE_IMPLEMENTED();
    SND_SEQ_Stop_4CAE60(static_cast<unsigned __int16>(idx));
}
EXPORT __int16 CC SND_SsIsEos_DeInlined_477930(SeqId idx)
{
    AE_IMPLEMENTED();
    return static_cast<__int16>(SND_SsIsEos_DeInlined_4CACD0(static_cast<unsigned __int16>(idx)));
}

EXPORT int CC SND_PlayEx_493040(const SoundEntry* pSnd, int panLeft, int panRight, float freq, MIDI_Channel* pMidiStru, int playFlags, int priority)
{
    AE_IMPLEMENTED();
    return SND_PlayEx_4EF740(pSnd, panLeft, panRight, freq, pMidiStru, playFlags, priority);
}

EXPORT int CC SND_Get_Buffer_Status_491D40(int idx)
{
    AE_IMPLEMENTED();
    return SND_Get_Buffer_Status_4EE8F0(idx);
}

// TODO: Check correct one
EXPORT int CC SND_Buffer_Set_Frequency_493820(int idx, float freq)
{
    AE_IMPLEMENTED();
    return SND_Buffer_Set_Frequency_4EFC00(idx, freq);
}

// TODO: Check is 2nd one
EXPORT int CC SND_Buffer_Set_Frequency_493790(int idx, float freq)
{
    AE_IMPLEMENTED();
    return SND_Buffer_Set_Frequency_4EFC00(idx, freq);
}

EXPORT void CC SsSeqStop_49E6E0(short idx)
{
    AE_IMPLEMENTED();
    SsSeqStop_4FD9C0(idx);
}

EXPORT void CC MIDI_SetTempo_49E8F0(__int16 idx, __int16 kZero, __int16 tempo)
{
    AE_IMPLEMENTED();
    MIDI_SetTempo_4FDB80(idx, kZero, tempo);
}

EXPORT signed int CC MIDI_Allocate_Channel_49D660(int not_used, int priority)
{
    AE_IMPLEMENTED();
    return MIDI_Allocate_Channel_4FCA50(not_used, priority);
}

// NOTE: Impl is not the same as AE
EXPORT int CC MIDI_PlayerPlayMidiNote_49D730(int vabId, int program, int note, int leftVolume, int rightVolume, int volume)
{
    auto vabId_ = vabId;
    auto leftVolume_ = leftVolume;
    auto v7 = ((program | (vabId << 8)) >> 8) & 0x1F;
    auto noteKeyNumber = (note >> 8) & 0x7F;
    //auto v9 = 0;
    auto v32 = rightVolume;
    auto usedChannelBits = 0;

    if (GetSpuApiVars()->sVagCounts()[v7])
    {
        for (int i = 0; i < 24; i++)
        {
            auto pAdsr = &GetSpuApiVars()->sMidi_Channels().channels[i].field_1C_adsr;
            if (!pAdsr->field_3_state
                || pAdsr->field_0_seq_idx != v7
                || pAdsr->field_1_program != ((program | (vabId << 8)) & 0x7F)
                || pAdsr->field_2_note_byte1 != noteKeyNumber)
            {
                // No match
            }
            else
            {
                SsUtKeyOffV_49EE50(static_cast<short>(i));
                break;
            }
        }
    }

    auto bLeftVolLessThanZero = leftVolume < 0;
    if (!leftVolume)
    {
        if (!rightVolume)
        {
            return 0;
        }
        bLeftVolLessThanZero = 0;
    }

    if (bLeftVolLessThanZero || rightVolume < 0)
    {
        return 0;
    }

    auto volume_ = volume;
    if (!volume)
    {
        return 0;
    }

    if (!GetSpuApiVars()->sVagCounts()[vabId_])
    {
        return 0;
    }

    auto k16Counter = 16;
    auto pVagOff = &GetSpuApiVars()->sConvertedVagTable().table[0][program + (vabId_ << 7)][0];
    while (1)
    {
        if (!pVagOff->field_D_vol ||
            pVagOff->field_8_min > noteKeyNumber ||
            pVagOff->field_9_max < noteKeyNumber)
        {

        }
        else
        {
            auto vag_vol = pVagOff->field_D_vol;
            auto vag_num = pVagOff->field_10_vag;
            auto panLeft = vag_vol * (unsigned __int16)GetSpuApiVars()->sGlobalVolumeLevel_left() * volume_ * leftVolume_ >> 21;
            auto panRight = vag_vol * (unsigned __int16)GetSpuApiVars()->sGlobalVolumeLevel_right() * volume_ * v32 >> 21;
            auto bPanLeftLessThanZero = panLeft < 0;
            auto playFlags = ((unsigned int)pVagOff->field_C >> 2) & 1;

            if (panLeft || panRight)
            {
                if (!panLeft)
                {
                    bPanLeftLessThanZero = 0;
                }

                if (!bPanLeftLessThanZero && panRight >= 0)
                {
                    if (((unsigned int)pVagOff->field_C >> 2) & 1)
                    {
                        if (panLeft > 90)
                        {
                            panLeft = 90;
                        }
                        if (panRight > 90)
                        {
                            panRight = 90;
                        }
                    }
                    auto maxPan = panRight;
                    if (panLeft >= panRight)
                    {
                        maxPan = panLeft;
                    }
                    
                    auto midiChannel = MIDI_Allocate_Channel_49D660(maxPan, pVagOff->field_E_priority);
                    auto midiChannel_ = midiChannel;
                    if (midiChannel >= 0)
                    {
                        auto pChannel = &GetSpuApiVars()->sMidi_Channels().channels[midiChannel];
                        auto bUnknown = playFlags
                            && (pVagOff->field_0_adsr_attack
                                || pVagOff->field_2_adsr_sustain_level
                                || pVagOff->field_4_adsr_decay != 16
                                || pVagOff->field_6_adsr_release >= 33u);
                        pChannel->field_C_vol = maxPan;
                        if (bUnknown)
                        {
                            auto v23 = pVagOff->field_0_adsr_attack;
                            pChannel->field_1C_adsr.field_3_state = 1;
                            auto v24 = v23 * (127 - volume);
                            auto v25 = pVagOff->field_4_adsr_decay;
                            pChannel->field_1C_adsr.field_4_attack = static_cast<unsigned short>(v24 >> 6);
                            pChannel->field_1C_adsr.field_6_sustain = pVagOff->field_2_adsr_sustain_level;
                            v24 = pVagOff->field_6_adsr_release;
                            pChannel->field_1C_adsr.field_8_decay = v25;
                            pChannel->field_1C_adsr.field_A_release = (unsigned short)v24;
                            if (pChannel->field_1C_adsr.field_4_attack)
                            {
                                panLeft = 2;
                                maxPan = 2;
                                v32 = 2;
                                leftVolume_ = 2;
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
                        auto priority = pVagOff->field_E_priority;
                        pChannel->field_8_left_vol = maxPan;
                        auto priority_ = priority;
                        pChannel->field_4_priority = priority;
                        auto midi_time = GetSpuApiVars()->sMidiTime();
                        pChannel->field_18_rightVol = playFlags;
                        pChannel->field_14_time = midi_time;
                        pChannel->field_1C_adsr.field_0_seq_idx = (BYTE)vabId;
                        pChannel->field_1C_adsr.field_1_program = (BYTE)program;
                        auto v29 = pVagOff->field_A_shift_cen;
                        pChannel->field_1C_adsr.field_2_note_byte1 = BYTE1(note) & 0x7F;
                        auto freq = pow(1.059463094359, (double)(note - v29) * 0.00390625);
                        pChannel->field_10_freq = (float)freq;
                        SND_PlayEx_493040(
                            &GetSpuApiVars()->sSoundEntryTable16().table[vabId][vag_num],
                            panLeft,
                            panRight,
                            (float)freq,
                            pChannel,
                            playFlags,
                            priority_);
                        volume_ = volume;
                        usedChannelBits |= 1 << midiChannel_;
                    }
                }
            }
            noteKeyNumber = (note >> 8) & 0x7F;
        }
        ++pVagOff;
        if (!--k16Counter)
        {
            return usedChannelBits;
        }
    }
    return 0;
}

EXPORT int CC MIDI_PlayerPlayMidiNote_49DAD0(int vabId, int program, int note, int leftVol, signed int rightVol, int volume)
{
    if (rightVol >= 64)
    {
        return MIDI_PlayerPlayMidiNote_49D730(vabId, program, note, leftVol * (127 - rightVol) / 64, leftVol, volume);
    }
    else
    {
        return MIDI_PlayerPlayMidiNote_49D730(vabId, program, note, leftVol, leftVol * rightVol / 64, volume);
    }
}


EXPORT int CC SND_Stop_Sample_At_Idx_493570(int idx)
{
    AE_IMPLEMENTED();
    return SND_Stop_Sample_At_Idx_4EFA90(idx);
}

// NOTE!!! not the same as AE
EXPORT void CC SsUtKeyOffV_49EE50(__int16 idx)
{
    const auto adsr_state = GetSpuApiVars()->sMidi_Channels().channels[idx].field_1C_adsr.field_3_state;
    auto pChannel = &GetSpuApiVars()->sMidi_Channels().channels[idx];
    if ((adsr_state <= 0 || adsr_state >= 4) && adsr_state != -1)
    {
        if (adsr_state == 4)
        {
            pChannel->field_1C_adsr.field_3_state = 0;
            SND_Stop_Sample_At_Idx_493570(pChannel->field_0_sound_buffer_field_4);
        }
    }
    else
    {
        pChannel->field_1C_adsr.field_3_state = 4;
        pChannel->field_C_vol = pChannel->field_8_left_vol;
        if (!pChannel->field_1C_adsr.field_A_release)
        {
            pChannel->field_1C_adsr.field_A_release = 125;
        }
        pChannel->field_14_time = GetSpuApiVars()->sMidiTime();
    }
}

enum MidiEvent
{
    NoteOff_80 = 0x80,
    NoteOn_90 = 0x90,
    Aftertouch_A0 = 0xA0,
    ControllerChange_B0 = 0xB0,
    ProgramChange_C0 = 0xC0,
    ChannelPressure_D0 = 0xD0,
    PitchBend_E0 = 0xE0,
    OtherCommands_F0 = 0xF0
};

EXPORT signed int CC MIDI_ParseMidiMessage_49DD30(int idx)
{
    MIDI_SeqSong* pCtx = &GetSpuApiVars()->sMidiSeqSongs().table[idx];
    BYTE** ppSeqData = &pCtx->field_0_seq_data;
    if (pCtx->field_4_time <= GetSpuApiVars()->sMidiTime())
    {
        while (1)
        {
            const BYTE curMidiByte = MIDI_ReadByte_4FD6B0(pCtx);
            BYTE originalMidiByte = curMidiByte;
            BYTE statusByte = curMidiByte;
            if (originalMidiByte < MidiEvent::OtherCommands_F0)
            {
                BYTE param1 = 0;
                if (originalMidiByte >= MidiEvent::NoteOff_80)
                {
                    param1 = MIDI_ReadByte_4FD6B0(pCtx);
                    pCtx->field_2A_running_status = originalMidiByte;
                }
                else
                {
                    if (!pCtx->field_2A_running_status)
                    {
                        return 0;
                    }
                    param1 = originalMidiByte;
                    statusByte = pCtx->field_2A_running_status;
                }

                int midi2Bytes = 0;
                midi2Bytes |= (WORD)(param1 << 8);

                int midi3Bytes = statusByte | midi2Bytes;
                const int midiEvent = statusByte & 0xF0;

                if (midiEvent != MidiEvent::ProgramChange_C0 && midiEvent != MidiEvent::ChannelPressure_D0)
                {
                    // Read parameter 2
                    midi3Bytes |= (MIDI_ReadByte_4FD6B0(pCtx) << 16);
                }

                switch (midiEvent)
                {
                case MidiEvent::NoteOff_80:
                {
                    int vab_id = (((pCtx->field_seq_idx << 8) | *(&GetSpuApiVars()->sMidiSeqSongs().table[0].field_32_progVols[0].field_0_program
                        + 2 * (midi3Bytes & 0xF)
                        + (midi3Bytes & 0xF)
                        + idx * 100)) >> 8) & 0x1F;
                    if (GetSpuApiVars()->sVagCounts()[vab_id])
                    {
                        for (short i = 0; i < 24; i++)
                        {
                             MIDI_ADSR_State* pAdsr = &GetSpuApiVars()->sMidi_Channels().channels[i].field_1C_adsr;
                             if (!pAdsr->field_3_state
                                 || pAdsr->field_0_seq_idx != ((((pCtx->field_seq_idx << 8) | *(&GetSpuApiVars()->sMidiSeqSongs().table[0].field_32_progVols[0].field_0_program + 2 * (midi3Bytes & 0xF) + (midi3Bytes & 0xF) + idx * 100)) >> 8) & 0x1F)
                                 || pAdsr->field_1_program != (((pCtx->field_seq_idx << 8) | *(&GetSpuApiVars()->sMidiSeqSongs().table[0].field_32_progVols[0].field_0_program + 2 * (midi3Bytes & 0xF) + (midi3Bytes & 0xF) + idx * 100)) & 0x7F)
                                 || pAdsr->field_2_note_byte1 != (((signed int)midi3Bytes >> 8) & 0x7F))
                             {
                                 // not a match
                             }
                             else
                             {
                                 SsUtKeyOffV_49EE50(i);
                                 break;
                             }
                        }
                        goto next_time_stamp;
                    }
                    break;
                }

                case MidiEvent::NoteOn_90:
                {
                    auto note_vol = pCtx->field_C_volume;
                    auto prog_num_ = midi3Bytes & 0xF;
                    auto v27 = idx * 100 + 2 * prog_num_;
                    auto r_vol = *(&GetSpuApiVars()->sMidiSeqSongs().table[0].field_32_progVols[0].field_2_right_vol + prog_num_ + v27);
                    MIDI_ProgramVolume* pProgVol = (MIDI_ProgramVolume*)((char*)GetSpuApiVars()->sMidiSeqSongs().table[0].field_32_progVols + prog_num_ + v27);
                    auto note = midi3Bytes & 0xFF00;
                    auto program = pProgVol->field_0_program;
                    auto l_vol = (signed __int16)((unsigned int)(pProgVol->field_1_left_vol * note_vol) >> 7);

                    auto freq = midi3Bytes >> 16;
                    MIDI_PlayerPlayMidiNote_49DAD0(pCtx->field_seq_idx, program, note, l_vol, r_vol, freq); // Note: inlined
                    break;
                }

                case MidiEvent::ControllerChange_B0:
                    switch ((midiEvent >> 8) & 0x7F)
                    {
                    case 6u:
                    case 0x26u:
                        switch (GetSpuApiVars()->sControllerValue())
                        {
                        case 20:    // set loop
                            pCtx->field_24_loop_start = (BYTE*)ppSeqData; // ???
                            pCtx->field_2C_loop_count = BYTE2(midiEvent);
                            break;

                        case 30:    // loop
                            if (pCtx->field_24_loop_start)
                            {
                                if (pCtx->field_2C_loop_count > 0)
                                {
                                    *ppSeqData = pCtx->field_24_loop_start;
                                    if (pCtx->field_2C_loop_count < 127)
                                    {
                                        GetSpuApiVars()->sControllerValue() = 0;
                                        pCtx->field_2C_loop_count--;
                                        goto next_time_stamp;
                                    }
                                }
                            }
                            break;

                        case 40:
                        {
                            auto pFn = pCtx->field_20_fn_ptr;
                            if (pFn)
                            {
                                //((void(__cdecl*)(int, _DWORD, _DWORD))pFn)(idx, 0, BYTE2(cmd));
                                GetSpuApiVars()->sControllerValue() = 0;
                                goto next_time_stamp;
                            }
                            break;
                        }
                        }
                        GetSpuApiVars()->sControllerValue() = 0;
                        break;

                    case 0x63u:
                        GetSpuApiVars()->sControllerValue() = BYTE2(midiEvent);
                        goto next_time_stamp;

                    default:
                        goto next_time_stamp;
                    }
                    break;

                case MidiEvent::ProgramChange_C0:
                    *(&GetSpuApiVars()->sMidiSeqSongs().table[0].field_32_progVols[0].field_0_program
                        + 2 * (midi3Bytes & 0xF)
                        + (midi3Bytes & 0xF)
                        + idx * 100) = BYTE1(midi3Bytes);
                    break;

                case MidiEvent::PitchBend_E0:
                {
                    const int prog_num = *(&GetSpuApiVars()->sMidiSeqSongs().table[0].field_32_progVols[0].field_0_program
                        + 2 * (midi3Bytes & 0xF)
                        + (midi3Bytes & 0xF)
                        + idx * 100);

                    // Inlined MIDI_PitchBend
                    const float freq_conv = (float)pow(1.059463094359, (double)(signed __int16)(((midi3Bytes >> 8) - 0x4000) >> 4) * 0.0078125);

                    for (int i = 0; i < 24; i++)
                    {
                        MIDI_Channel* pChannel = &GetSpuApiVars()->sMidi_Channels().channels[i];
                        if (pChannel->field_1C_adsr.field_1_program == prog_num)
                        {
                            const float freq_1 = freq_conv * pChannel->field_10_freq;
                            SND_Buffer_Set_Frequency_493790(i, freq_1);
                        }
                    }
                    break;
                }

                default:
                    break;
                }
            }
            else if (curMidiByte == MidiEvent::OtherCommands_F0 || curMidiByte == 0xF7)
            {
                const int lenToSkip = MIDI_Read_Var_Len_4FD0D0(pCtx);
                MIDI_SkipBytes_4FD6C0(pCtx, lenToSkip);
            }
            else if (curMidiByte == 0xFF)  // Sysex len
            {
                BYTE metaEvent =  MIDI_ReadByte_4FD6B0(pCtx);
                if (metaEvent == 0x2F)               // End of track
                {
                    if (pCtx->field_18_repeatCount)
                    {
                        pCtx->field_18_repeatCount--;

                        if (!pCtx->field_18_repeatCount)
                        {
                            SsSeqStop_49E6E0(static_cast<short>(idx)); // Note: inlined 
                            return 1;
                        }
                    }
                    // Reset to start
                    pCtx->field_0_seq_data = pCtx->field_1C_pSeqData;
                }
                else
                {
                    const int tempoChange = MIDI_Read_Var_Len_4FD0D0(pCtx);
                    if (tempoChange)
                    {
                        if (metaEvent == 0x51)       // Tempo in microseconds per quarter note (24-bit value)
                        {
                            const int tempoByte3 = MIDI_ReadByte_4FD6B0(pCtx) << 16;
                            const int tempoByte2 = MIDI_ReadByte_4FD6B0(pCtx) << 8;
                            // TODO: Argument is truncated
                            const int fullTempo = tempoByte3 | tempoByte2 | MIDI_ReadByte_4FD6B0(pCtx);
                            MIDI_SetTempo_49E8F0(static_cast<short>(idx), 0, static_cast<short>(fullTempo));
                        }
                        else
                        {
                            MIDI_SkipBytes_4FD6C0(pCtx, tempoChange);
                        }
                    }
                }
            }

        next_time_stamp:
            const int timeStamp = MIDI_Read_Var_Len_4FD0D0(pCtx);
            if (timeStamp)
            {
                pCtx->field_4_time = timeStamp * pCtx->field_14_tempo / 1000 + pCtx->field_4_time;
                if (pCtx->field_4_time > GetSpuApiVars()->sMidiTime())
                {
                    return 1;
                }
            }
        }
    }
    return 1;
}

EXPORT void CC SND_Shutdown_476EC0()
{
    AE_IMPLEMENTED();
    SND_Shutdown_4CA280();
}

EXPORT void CC SND_SEQ_SetVol_477970(SeqId idx, __int16 volLeft, __int16 volRight)
{
    AE_IMPLEMENTED();
    SND_SEQ_SetVol_4CAD20(static_cast<unsigned __int16>(idx), volLeft, volRight);
}

static VabBodyRecord* IterateVBRecords(VabBodyRecord* ret, int i_3)
{
    for (int i = 0; i < i_3; i++)
    {
        ret = (VabBodyRecord*)((char*)ret
            + ret->field_0_length_or_duration
            + 8);
    }
    return ret;
}

// Loads vab body sample data to memory
EXPORT void CC SsVabTransBody_49D3E0(VabBodyRecord* pVabBody, __int16 vabId)
{
    if (vabId < 0)
    {
        return;
    }

    VabHeader* pVabHeader = GetSpuApiVars()->spVabHeaders()[vabId];
    const int vagCount = GetSpuApiVars()->sVagCounts()[vabId];

    for (int i = 0; i < vagCount; i++)
    {
        SoundEntry* pEntry = &GetSpuApiVars()->sSoundEntryTable16().table[vabId][i];

        if (!(i & 7))
        {
            SsSeqCalledTbyT_49E9F0();
        }

        memset(pEntry, 0, sizeof(SoundEntry));

        int sampleLen = -1;
        if (pVabHeader && i >= 0)
        {
            sampleLen = (8 * IterateVBRecords(pVabBody, i)->field_0_length_or_duration) / 16;
        }

        if (sampleLen > 0)
        {
            VabBodyRecord* v10 = nullptr;
            if (pVabHeader && i >= 0)
            {
                v10 = IterateVBRecords(pVabBody, i);
            }

            const BYTE unused_field = v10->field_4_unused >= 0 ? 0 : 4;
            for (int prog = 0; prog < 128; prog++)
            {
                for (int tone = 0; tone < 16; tone++)
                {
                    auto pVag = &GetSpuApiVars()->sConvertedVagTable().table[vabId][prog][tone];
                    if (pVag->field_10_vag == i)
                    {
                        pVag->field_C = unused_field;

                        if (!(unused_field & 4) && !pVag->field_0_adsr_attack && pVag->field_6_adsr_release)
                        {
                            pVag->field_6_adsr_release = 0;
                        }
                    }
                }
            }

            if (!SND_New_492790(pEntry, sampleLen, 44100, 16u, 0))
            {
                auto pTempBuffer = (DWORD*)malloc(sampleLen * pEntry->field_1D_blockAlign);
                if (pTempBuffer)
                {
                    DWORD* pSrcVB = nullptr;
                    if (pVabHeader && i >= 0)
                    {
                        pSrcVB = &IterateVBRecords(pVabBody, i)->field_8_fileOffset;
                    }
      
                    int sampleLen2 = -1;
                    if (pVabHeader && i >= 0)
                    {
                        sampleLen2 = (8 * IterateVBRecords(pVabBody, i)->field_0_length_or_duration) / 16;
                    }

                    const int len = (16 * sampleLen2) / 8;
                    memcpy(pTempBuffer, pSrcVB, len);

                    if (sampleLen2)
                    {
                        SND_Load_492F40(pEntry, pTempBuffer, sampleLen2);
                    }

                    free(pTempBuffer);
                }
            }
        }
    }
}

EXPORT signed __int16 CC SND_VAB_Load_476CB0(SoundBlockInfo* pSoundBlockInfo, __int16 vabId)
{
    // Fail if no file name
    if (!pSoundBlockInfo->field_0_vab_header_name)
    {
        return 0;
    }

    // Find the VH file record
    LvlFileRecord* pVabHeaderFile = sLvlArchive_4FFD60.Find_File_Record_41BED0(pSoundBlockInfo->field_0_vab_header_name);
    
    int headerSize = pVabHeaderFile->field_10_num_sectors << 11;

    BYTE** ppVabHeader = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_VabHeader, vabId, headerSize);

    pSoundBlockInfo->field_C_pVabHeader = *ppVabHeader;
    sLvlArchive_4FFD60.Read_File_41BE40(pVabHeaderFile, *ppVabHeader);
    
    // Find the VB file record
    LvlFileRecord* pVabBodyFile = sLvlArchive_4FFD60.Find_File_Record_41BED0(pSoundBlockInfo->field_4_vab_body_name);
    if (!pVabBodyFile)
    {
        // For some reason its acceptable to assume we have a VH with no VB, but the VH must always exist, this happens for MONK.VB
        return 0;
    }

    int vabBodySize = pVabBodyFile->field_10_num_sectors << 11;

    // Load the VB file data
    BYTE** ppVabBody = ResourceManager::Alloc_New_Resource_454F20(ResourceManager::Resource_VabBody, vabId, vabBodySize);
    if (!ppVabBody)
    {
        // Maybe filed due to OOM cause its huge, free the abe resources and try again
        if (!GetMidiVars()->sSnd_ReloadAbeResources())
        {
            GetMidiVars()->sSnd_ReloadAbeResources() = TRUE;
            sActiveHero_507678->Free_Resources_422870();
        }

        // Compact/reclaim any other memory we can too
        ResourceManager::Reclaim_Memory_455660(0);

        // If it fails again there is no recovery, in either case caller will restore abes resources
        ppVabBody = ResourceManager::Alloc_New_Resource_454F20(ResourceManager::Resource_VabBody, vabId, vabBodySize);
        if (!ppVabBody)
        {
            return 0;
        }
    }

    sLvlArchive_4FFD60.Read_File_41BE40(pVabBodyFile, *ppVabBody);
    pSoundBlockInfo->field_8_vab_id = SsVabOpenHead_49CFB0(reinterpret_cast<VabHeader*>(pSoundBlockInfo->field_C_pVabHeader));
    SsVabTransBody_49D3E0(reinterpret_cast<VabBodyRecord*>(*ppVabBody), static_cast<short>(pSoundBlockInfo->field_8_vab_id));
    SsVabTransCompleted_4FE060(SS_WAIT_COMPLETED);

    // Now the sound samples are loaded we don't need the VB data anymore
    ResourceManager::FreeResource_455550(ppVabBody);
    return 1;
}

EXPORT void CC SND_Load_VABS_477040(SoundBlockInfo* pSoundBlockInfo, int reverb)
{
    SoundBlockInfo* pSoundBlockInfoIter = pSoundBlockInfo;
    GetMidiVars()->sSnd_ReloadAbeResources() = FALSE;
    if (GetMidiVars()->sLastLoadedSoundBlockInfo() != reinterpret_cast<::SoundBlockInfo*>(pSoundBlockInfo))
    {
        SsUtReverbOff_4FE350();
        SsUtSetReverbDepth_4FE380(0, 0);
        SpuClearReverbWorkArea_4FA690(4);

        if (GetMidiVars()->sMonkVh_Vb().field_8_vab_id < 0)
        {
            SND_VAB_Load_476CB0(reinterpret_cast<SoundBlockInfo*>(&GetMidiVars()->sMonkVh_Vb()), 32);
        }

        GetMidiVars()->sLastLoadedSoundBlockInfo() = reinterpret_cast<::SoundBlockInfo*>(pSoundBlockInfo);

        __int16 vabId = 0;
        while (SND_VAB_Load_476CB0(pSoundBlockInfoIter, vabId))
        {
            ++vabId;
            ++pSoundBlockInfoIter;
        }

        if (GetMidiVars()->sSnd_ReloadAbeResources())
        {
            ResourceManager::Reclaim_Memory_455660(0);
            Abe::Load_Basic_Resources_4228A0();
        }

        SsUtSetReverbDepth_4FE380(reverb, reverb);
        SsUtReverbOn_4FE340();
    }
}

EXPORT void CC SND_Load_Seqs_477AB0(OpenSeqHandleAE* pSeqTable, const char* bsqFileName)
{
    AE_IMPLEMENTED();
    SND_Load_Seqs_Impl(
        reinterpret_cast<::OpenSeqHandle*>(pSeqTable),
        bsqFileName);
}

EXPORT signed __int16 CC SND_SEQ_Play_477760(SeqId idx, int repeatCount, __int16 volLeft, __int16 volRight)
{
    AE_IMPLEMENTED();
    const auto ret = SND_SEQ_PlaySeq_4CA960(static_cast<unsigned __int16>(idx), static_cast<short>(repeatCount), 1); // TODO ??
    SsSeqSetVol_4FDAC0(static_cast<unsigned __int16>(idx), volLeft, volRight);
    return ret;
}

static ::SfxDefinition ToAeSfxDef(const SfxDefinition* sfxDef)
{
    ::SfxDefinition aeDef = {};
    aeDef.field_0_block_idx = static_cast<char>(sfxDef->field_0_block_idx);
    aeDef.field_1_program = static_cast<char>(sfxDef->field_4_program);
    aeDef.field_2_note = static_cast<char>(sfxDef->field_8_note);
    aeDef.field_3_default_volume = static_cast<char>(sfxDef->field_C_default_volume);
    aeDef.field_4_pitch_min = sfxDef->field_E_pitch_min;
    aeDef.field_6_pitch_max = sfxDef->field_10_pitch_max;
    return aeDef;
}

EXPORT int CC SFX_SfxDefinition_Play_477330(const SfxDefinition* sfxDef, short volLeft, short volRight, short pitch_min, signed __int16 pitch_max)
{
    AE_IMPLEMENTED();
    const ::SfxDefinition aeDef = ToAeSfxDef(sfxDef);
    return SFX_SfxDefinition_Play_4CA700(&aeDef, volLeft, volRight, pitch_min, pitch_max);
}

EXPORT int CC SFX_SfxDefinition_Play_4770F0(const SfxDefinition* sfxDef, int vol, int pitch_min, int pitch_max)
{
    AE_IMPLEMENTED();
    const ::SfxDefinition aeDef = ToAeSfxDef(sfxDef);
    return SFX_SfxDefinition_Play_4CA420(&aeDef, static_cast<short>(vol), static_cast<short>(pitch_min), static_cast<short>(pitch_max));
}

EXPORT void CC SND_Init_Real_476E40()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC SND_Init_476E40()
{
    SetSpuApiVars(&sAoSpuVars);
    SetMidiApiVars(&sAoMidiVars);

    GetSoundAPI().SND_Load = SND_Load_492F40;
    GetSoundAPI().SND_PlayEx = SND_PlayEx_493040;
    GetSoundAPI().SND_Get_Buffer_Status = SND_Get_Buffer_Status_491D40;
    GetSoundAPI().SND_Stop_Sample_At_Idx = SND_Stop_Sample_At_Idx_493570;
    GetSoundAPI().SND_Buffer_Set_Frequency2 = SND_Buffer_Set_Frequency_493820;

    // SND_Buffer_Set_Frequency1 SND_Buffer_Set_Frequency_493790

    if (RunningAsInjectedDll())
    {
        SND_Init_Real_476E40();
    }
    else
    {
        SND_Init_4CA1F0();
        SND_Restart_SetCallBack(SND_Restart_476340);
        SND_StopAll_SetCallBack(SND_StopAll_4762D0);
    }
}

EXPORT void CC SND_StopAll_4762D0()
{
    MusicController::EnableMusic_443900(0);

    if (sBackgroundMusic_seq_id_4CFFF8 >= 0)
    {
        SND_Seq_Stop_477A60(static_cast<SeqId>(sBackgroundMusic_seq_id_4CFFF8));
    }

    SND_Reset_Ambiance_4765E0();
    SND_Stop_All_Seqs_4774D0();

    for (int i=0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (!pObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            pObj->VStopAudio();
        }
    }

    SsUtAllKeyOff_49EDE0(0);

}

}
