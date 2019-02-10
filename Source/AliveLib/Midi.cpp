#include "stdafx.h"
#include "Midi.hpp"
#include "Function.hpp"
#include "Sound.hpp"
#include "PathData.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "DDCheat.hpp" // TODO: Move Abe object to own file
#include "LvlArchive.hpp"
#include "stdlib.hpp"
#include "Sound.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "BackgroundMusic.hpp"
#include "Sys.hpp"
#include <gmock/gmock.h>

void Midi_ForceLink() { }

EXPORT void SND_Stop_All_Seqs_4CA850();

EXPORT void CC SND_StopAll_4CB060()
{
    MusicController::EnableMusic_47FE10(FALSE);
    BackgroundMusic::Stop_4CB000();
    SND_Reset_Ambiance_4CB4B0();
    SND_Stop_All_Seqs_4CA850();
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (!pObj->field_6_flags.Get(BaseGameObject::eDead))
        {
            pObj->VStopAudio();
        }
    }
    MIDI_Stop_All_Channels_4FDFE0();
}

EXPORT void SsUtReverbOff_4FE350()
{
    // Stub
}

EXPORT void SsUtSetReverbDepth_4FE380(int, int)
{
    // Stub
}

EXPORT void SpuClearReverbWorkArea_4FA690()
{
    // Stub
}

EXPORT void SsUtReverbOn_4FE340()
{
    // Stub
}

EXPORT void CC SsSetTableSize_4FE0B0(void*, int, int)
{
    // Stub
}

EXPORT void CC SsUtSetReverbType_4FE360(int)
{
    // Stub
}

EXPORT void CC SsSetTickMode_4FDC20(int)
{
    // Stub
}

EXPORT void CC SND_CallBack_4020A4()
{
    // Stub
}

using TVSyncCallBackFn = void(CC *)();

EXPORT void CC SND_Set_VSyncCallback_4F8C40(TVSyncCallBackFn)
{
    // Stub
}

EXPORT void SsVabTransCompleted_4FE060(int)
{
    // Stub
}

struct MIDI_3_Bytes
{
    char field_0_program;
    char field_1_left_vol;
    char field_2_right_vol;
};
ALIVE_ASSERT_SIZEOF(MIDI_3_Bytes, 3);

struct MIDI_Struct2
{
    BYTE *field_0_seq_data;
    unsigned int field_4_time;
    int field_8;
    int field_C_volume;
    int field_10;
    int field_14;
    int field_18;
    BYTE *field_1C_pSeqData;
    void* field_20_fn_ptr;
    BYTE *field_24_loop_start;
    __int16 field_seq_idx;
    unsigned char field_2A;
    char field_2B;
    char field_2C_loop_count;
    char field_2D;
    __int16 field_2E;
    char field_30;
    char field_31;
    MIDI_3_Bytes field_32[16];
   // char field_32;
    //MIDI_3_Bytes field_33[16];
    char field_62;
    char field_63;
};
ALIVE_ASSERT_SIZEOF(MIDI_Struct2, 100);

EXPORT void CC MIDI_SkipBytes_4FD6C0(MIDI_Struct2* pData, int length)
{
    pData->field_0_seq_data += length;
}

EXPORT BYTE CC MIDI_ReadByte_4FD6B0(MIDI_Struct2* pData)
{
    return *pData->field_0_seq_data++;
}

EXPORT int CC MIDI_Read_Var_Len_4FD0D0(MIDI_Struct2* pMidiStru)
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

ALIVE_VAR(1, 0xBD1CDE, __int16, sGlobalVolumeLevel_right_BD1CDE, 0);
ALIVE_VAR(1, 0xBD1CDC, __int16, sGlobalVolumeLevel_left_BD1CDC, 0);

struct VabUnknown
{
    char field_0[4][128];
};
ALIVE_ASSERT_SIZEOF(VabUnknown, 512);

const int kMaxVabs = 4;

ALIVE_VAR(1, 0xC13180, VabUnknown, s512_byte_C13180, {});
ALIVE_ARY(1, 0xBE6144, BYTE, kMaxVabs, sVagCounts_BE6144, {});
ALIVE_ARY(1, 0x0BDCD64, BYTE, kMaxVabs, sProgCounts_BDCD64, {});

ALIVE_ARY(1, 0xC13160, VabHeader*, 4, spVabHeaders_C13160, {});

struct ConvertedVagTable
{
    Converted_Vag table[kMaxVabs][128][16]; // 16 = max tones, 128 = max progs
};
ALIVE_ASSERT_SIZEOF(ConvertedVagTable, 147456);
ALIVE_VAR(1, 0xBEF160, ConvertedVagTable, sConvertedVagTable_BEF160, {});

EXPORT __int16 CC SND_Load_Vab_Header_4FC620(VabHeader* pVabHeader)
{
    if (!pVabHeader)
    {
        return -1;
    }

    MIDI_UpdatePlayer_4FDC80();

    const int vab_id = pVabHeader->field_8_id;
    assert(vab_id < 4);
    SND_SsVabClose_4FC5B0(vab_id);
    spVabHeaders_C13160[vab_id] = pVabHeader;
    if (sVagCounts_BE6144[vab_id] > 0)
    {
        SND_SsVabClose_4FC5B0(vab_id);
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

const int kSeqTableSize = 144;

struct SoundEntryTable
{
    SoundEntry table[kMaxVabs][256];
};
ALIVE_ASSERT_SIZEOF(SoundEntryTable, 36864);

ALIVE_VAR(1, 0xBE6160, SoundEntryTable, sSoundEntryTable16_BE6160, {});


const int kNumChannels = 24;
struct MidiChannels
{
    MIDI_Struct1 channels[kNumChannels];
};
ALIVE_ASSERT_SIZEOF(MidiChannels, 1056);
ALIVE_VAR(1, 0xC14080, MidiChannels, sMidi_Channels_C14080, {});

struct MidiTable
{
    MIDI_Struct2 table[32];
};
ALIVE_ASSERT_SIZEOF(MidiTable, 3200);
ALIVE_VAR(1, 0xC13400, MidiTable, sMidiStruct2Ary32_C13400, {});

static void SND_ResetData()
{
    memset(&s512_byte_C13180, 0, sizeof(VabUnknown));
    memset(&sSoundEntryTable16_BE6160, 0, sizeof(SoundEntryTable));
    memset(&sConvertedVagTable_BEF160, 0, sizeof(ConvertedVagTable));
    memset(&sMidi_Channels_C14080, 0, sizeof(MidiChannels));

    for (int i = 0; i < kMaxVabs; i++)
    {
        sVagCounts_BE6144[i] = 0;
        sProgCounts_BDCD64[i] = 0;
    }

    memset(&sMidiStruct2Ary32_C13400, 0, sizeof(MidiTable));
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            sMidiStruct2Ary32_C13400.table[i].field_32[j].field_1_left_vol = 112;
            sMidiStruct2Ary32_C13400.table[i].field_32[j].field_2_right_vol = 64;
        }
    }
}

EXPORT void CC SND_SsInitHot_4FC230()
{
    sGlobalVolumeLevel_right_BD1CDE = 127;
    sGlobalVolumeLevel_left_BD1CDC = 127;

    SND_CreateDS_4EEAA0(22050u, 16, 1);

    SND_ResetData();
}

struct SeqIds
{
    __int16 ids[16];
};
ALIVE_VAR(1, 0xBB2354, SeqIds, sSeq_Ids_word_BB2354, {});

ALIVE_VAR(1, 0xbd1cf4, int, sMidi_Inited_dword_BD1CF4, 0);
ALIVE_VAR(1, 0xbd1cec, unsigned int, sMidiTime_BD1CEC, 0);

EXPORT void CC PSX_SsStart_4FC320()
{
    sMidi_Inited_dword_BD1CF4 = 1;
    sMidiTime_BD1CEC = SYS_GetTicks();
}

EXPORT void CC PSX_SsSetMVol_4FC360(__int16 left, __int16 right)
{
    sGlobalVolumeLevel_left_BD1CDC = left;
    sGlobalVolumeLevel_right_BD1CDE = right;
}

ALIVE_VAR(1, 0xbb2e3c, __int16, sSeqsPlaying_count_word_BB2E3C, 0);

EXPORT void CC SND_Init_4CA1F0()
{
    SND_SsInitHot_4FC230();
    SsSetTableSize_4FE0B0(nullptr, 16, 1);
    SsUtSetReverbDepth_4FE380(0, 0);
    SsUtSetReverbType_4FE360(4);
    SsUtReverbOn_4FE340();
    SsSetTickMode_4FDC20(4096);
    SND_Set_VSyncCallback_4F8C40(SND_CallBack_4020A4);
    PSX_SsStart_4FC320();
    PSX_SsSetMVol_4FC360(100, 100);
    memset(&sSeq_Ids_word_BB2354, -1, sizeof(SeqIds));
    sSeqsPlaying_count_word_BB2E3C = 0;
}

// I think this is the burrrrrrrrrrrrrrrrrrrr loading sound
const SoundBlockInfo soundBlock = { "MONK.VH", "MONK.VB", -1, nullptr };
ALIVE_VAR(1, 0x560F48, SoundBlockInfo, sMonkVh_Vb_560F48, soundBlock);


EXPORT unsigned __int16 CC MIDI_SsIsEos_4FDA80(__int16 idx, __int16 kZero)
{
    if (!kZero)
    {
        if (sMidiStruct2Ary32_C13400.table[idx].field_0_seq_data)
        {
            if (sMidiStruct2Ary32_C13400.table[idx].field_2B)
            {
                return 1;
            }
        }
    }
    return 0;
}

ALIVE_VAR(1, 0xbb2e38, SeqDataRecord *, sSeqDataTable_BB2E38, nullptr);

EXPORT __int16 CC MIDI_Stop_Channel_4FE010(__int16 idx)
{
    MIDI_Struct1* pChannel = &sMidi_Channels_C14080.channels[idx];
    if (pChannel->field_1C.field_3)
    {
        pChannel->field_1C.field_3 = 4;
        pChannel->field_C = pChannel->field_8_left_vol;
        if (pChannel->field_1C.field_A_release < 300)
        {
            pChannel->field_1C.field_A_release = 300;
        }
        pChannel->field_14_time = sMidiTime_BD1CEC;
    }
    return 0;
}

EXPORT void CC MIDI_SsSeqPlay_4FD900(unsigned __int16 idx, char param_field_2B, __int16 param_field_18)
{
    if (idx < 32u)
    {
        MIDI_Struct2& rec = sMidiStruct2Ary32_C13400.table[idx];
        if (rec.field_1C_pSeqData)
        {
            rec.field_4_time = sMidiTime_BD1CEC;
            rec.field_8 = sMidiTime_BD1CEC;
            rec.field_18 = param_field_18;

            if (rec.field_2B != -1)
            {
                rec.field_0_seq_data = rec.field_1C_pSeqData;
                rec.field_2A = 0;
                unsigned int midiTime = MIDI_Read_Var_Len_4FD0D0(&rec);
                if (midiTime)
                {
                    midiTime = (midiTime * rec.field_14) / 1000;
                }
                rec.field_4_time += midiTime;
            }

            if (param_field_2B)
            {
                rec.field_2B = param_field_2B == 1;
            }
            else
            {
                rec.field_2B = -1;
            }
        }
    }
}

EXPORT void CC MIDI_SsSeqStop_4FD9C0(__int16 idx)
{
    if (sMidiStruct2Ary32_C13400.table[idx].field_0_seq_data)
    {
        sMidiStruct2Ary32_C13400.table[idx].field_2B = 0;
        if (sMidiStruct2Ary32_C13400.table[idx].field_2E >= 0)
        {
            MIDI_SsSeqPlay_4FD900(sMidiStruct2Ary32_C13400.table[idx].field_2E, 1, 1);
            sMidiStruct2Ary32_C13400.table[idx].field_2E = -1;
        }
    }

    for (unsigned int i = 0; i < kNumChannels; i++)
    {
        unsigned int field_C = sMidi_Channels_C14080.channels[i].field_1C.field_C;
        field_C = field_C >> 4;
        if (field_C == static_cast<unsigned int>(idx))
        {
            MIDI_Stop_Channel_4FE010(static_cast<short>(i));
            sMidi_Channels_C14080.channels[i].field_1C.field_C = 0;
        }
    }
}

EXPORT void CC SND_SEQ_Stop_4CAE60(unsigned __int16 idx)
{
    if (sSeqDataTable_BB2E38[idx].field_A_id != -1 && sSeqDataTable_BB2E38[idx].field_C_ppSeq_Data)
    {
        if (MIDI_SsIsEos_4FDA80(sSeqDataTable_BB2E38[idx].field_A_id, 0))
        {
            MIDI_SsSeqStop_4FD9C0(sSeqDataTable_BB2E38[idx].field_A_id);
        }
    }
}

EXPORT char CC SND_Seq_Table_Valid_4CAFE0()
{
    return sSeqDataTable_BB2E38 != 0;
}

EXPORT void CC MIDI_Stop_All_Channels_4FDFE0()
{
    // Stop all backwards
    short idx = kNumChannels - 1;
    do
    {
        MIDI_Stop_Channel_4FE010(idx--);
    }
    while (idx >= 0);
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
        MIDI_Struct1* pChannel = &sMidi_Channels_C14080.channels[i];
        MIDI_Struct1_Sub* pSub = &pChannel->field_1C;
        if (pSub->field_3
            && pSub->field_0_seq_idx == vabId
            && pSub->field_1_program == (VabIdAndProgram & 127)
            && pSub->field_2_note_byte1 == ((note >> 8) & 127))
        {
            MIDI_Stop_Channel_4FE010(i);
            return 0;
        }
    }
    return 0;
}

ALIVE_VAR(1, 0xbd1ce8, BOOL, sSoundDatIsNull_BD1CE8, TRUE);

EXPORT int CC MIDI_Play_Single_Note_Impl_4FCF10(int vabIdAndProgram, int note, unsigned __int16 leftVol, unsigned __int16 rightVol)
{
    MIDI_Stop_Existing_Single_Note_4FCFF0(vabIdAndProgram & 127 | (((vabIdAndProgram >> 8) & 31) << 8), note);
    
    if (sSoundDatIsNull_BD1CE8)
    {
        return 0;
    }

    const int channelBits = MIDI_PlayMidiNote_4FCB30((vabIdAndProgram >> 8) & 31, vabIdAndProgram & 127, note, leftVol, rightVol, 96);

    for (int idx = 0; idx < kNumChannels; idx++)
    {
        MIDI_Struct1* pChannel = &sMidi_Channels_C14080.channels[idx];
        if ((1 << idx) & channelBits)
        {
            pChannel->field_1C.field_C = 0xFFFF; // or both -1, but they appear to be unsigned ??
            pChannel->field_1C.field_E = 0xFF;
        }
    }

    return channelBits;
}

EXPORT int CC MIDI_Play_Single_Note_4CA1B0(int vabIdAndProgram, int note, int leftVol, int rightVol)
{
    // NOTE: word_BB2E40 is used as a guard here, but it is never read anywhere else
    return MIDI_Play_Single_Note_Impl_4FCF10(vabIdAndProgram, note, static_cast<unsigned short>(leftVol), static_cast<unsigned short>(rightVol));
}

EXPORT void CC MIDI_Stop_None_Ended_Seq_4FD8D0(__int16 idx)
{
    MIDI_SsSeqStop_4FD9C0(idx);
    sMidiStruct2Ary32_C13400.table[idx].field_C_volume = 0;
    sMidiStruct2Ary32_C13400.table[idx].field_0_seq_data = 0;
    sMidiStruct2Ary32_C13400.table[idx].field_1C_pSeqData = nullptr;
}

EXPORT void SND_Stop_All_Seqs_4CA850()
{
    // TODO: Why is there 16 of these but 32 of sMidiStruct2Ary32_C13400? Seems like they should match in size
    sSeqsPlaying_count_word_BB2E3C = 0;
    for (short i = 0; i < 16; i++)
    {
        if (sSeq_Ids_word_BB2354.ids[i] >= 0)
        {
            if (MIDI_SsIsEos_4FDA80(i, 0))
            {
                MIDI_SsSeqStop_4FD9C0(i);
            }
            MIDI_Stop_None_Ended_Seq_4FD8D0(i);
            sSeqDataTable_BB2E38[sSeq_Ids_word_BB2354.ids[i]].field_A_id = -1;
            sSeq_Ids_word_BB2354.ids[i] = -1;
        }
    }
}

EXPORT void SND_SsSeqClose_4CA8E0()
{
    for (short i = 0; i < 16; i++)
    {
        if (sSeq_Ids_word_BB2354.ids[i] >= 0)
        {
            if (!MIDI_SsIsEos_4FDA80(i, 0))
            {
                MIDI_Stop_None_Ended_Seq_4FD8D0(i);
                sSeqDataTable_BB2E38[sSeq_Ids_word_BB2354.ids[i]].field_A_id = -1;
                sSeq_Ids_word_BB2354.ids[i] = -1;
                sSeqsPlaying_count_word_BB2E3C--;
            }
        }
    }
}

EXPORT int CC SND_SsIsEos_DeInlined_4CACD0(unsigned __int16 idx)
{
    SeqDataRecord* pRec = &sSeqDataTable_BB2E38[idx];
    if (pRec->field_A_id != -1 && pRec->field_C_ppSeq_Data)
    {
        return MIDI_SsIsEos_4FDA80(pRec->field_A_id, 0) != 0;
    }
    return 0;
}

EXPORT void CC SND_Free_All_Seqs_4C9F40()
{
    for (int i = 0; i < kSeqTableSize; i++)
    {
        if (sSeqDataTable_BB2E38[i].field_C_ppSeq_Data)
        {
            BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Seq, sSeqDataTable_BB2E38[i].field_4_generated_res_id, 0, 0);
            ResourceManager::FreeResource_49C330(ppRes);
            sSeqDataTable_BB2E38[i].field_C_ppSeq_Data = nullptr;
        }
    }
}

ALIVE_VAR(1, 0xbb2e34, SoundBlockInfo *, sLastLoadedSoundBlockInfo_BB2E34, nullptr);


EXPORT void CC SND_SsVabClose_4FC5B0(int vabId)
{
    MIDI_Stop_All_Channels_4FDFE0();

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

EXPORT void CC SND_Free_All_VABS_4C9EB0()
{
    SoundBlockInfo* pIter = sLastLoadedSoundBlockInfo_BB2E34;
    while (pIter && pIter->field_4_vab_body_name)
    {
        ResourceManager::FreeResource_Impl_49C360(pIter->field_C_pVabHeader);
        pIter->field_C_pVabHeader = nullptr;
        SND_SsVabClose_4FC5B0(pIter->field_8_vab_id);
        pIter->field_8_vab_id = -1;
        pIter++;
    }
    sLastLoadedSoundBlockInfo_BB2E34 = nullptr;
}

EXPORT void CC MIDI_SsSeqSetVol_4FDAC0(__int16 idx, __int16 volLeft, __int16 volRight)
{
    if (sMidiStruct2Ary32_C13400.table[idx].field_0_seq_data)
    {
        // TODO: Refactor
        sMidiStruct2Ary32_C13400.table[idx].field_C_volume = 112 * ((volRight + volLeft) >> 1) >> 7;
    }
}

EXPORT void CC SND_SEQ_SetVol_4CAD20(int idx, __int16 volLeft, __int16 volRight)
{
    unsigned __int16 limitedIdx = idx & 0xFFFF;
    if (sSeqDataTable_BB2E38[limitedIdx].field_A_id != -1
        && sSeqDataTable_BB2E38[limitedIdx].field_C_ppSeq_Data
        && SND_SsIsEos_DeInlined_4CACD0(limitedIdx))
    {
        MIDI_SsSeqSetVol_4FDAC0(sSeqDataTable_BB2E38[limitedIdx].field_A_id, volLeft, volRight);
    }
}

ALIVE_VAR(1, 0x560f40, short, sNeedToHashSeqNames_560F40, 1);

EXPORT void CC SND_Load_Seqs_4CAED0(SeqDataRecord* pSeqTable, const char* bsqFileName)
{
    if (pSeqTable && bsqFileName)
    {
        sSeqDataTable_BB2E38 = pSeqTable;

        // Generate resource ids from hashing the name if we haven't already
        if (sNeedToHashSeqNames_560F40)
        {
            for (int i = 0; i < kSeqTableSize; i++)
            {
                sSeqDataTable_BB2E38[i].field_C_ppSeq_Data = nullptr;
                sSeqDataTable_BB2E38[i].field_A_id = -1;
                sSeqDataTable_BB2E38[i].field_4_generated_res_id = ResourceManager::SEQ_HashName_49BE30(sSeqDataTable_BB2E38[i].field_0_mName);
            }
            sNeedToHashSeqNames_560F40 = 0;
        }

        // Load the BSQ
        ResourceManager::Reclaim_Memory_49C470(0);
        ResourceManager::LoadResourceFile_49C170(bsqFileName, nullptr);

        // Get a pointer to each SEQ
        for (int i = 0; i < kSeqTableSize; i++)
        {
            BYTE** ppSeq = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Seq, sSeqDataTable_BB2E38[i].field_4_generated_res_id, 1, 1);
            if (ppSeq)
            {
                sSeqDataTable_BB2E38[i].field_C_ppSeq_Data = *ppSeq;
            }
            else
            {
                sSeqDataTable_BB2E38[i].field_C_ppSeq_Data = nullptr;
            }
        }
    }
}

EXPORT void SND_Reset_4C9FB0()
{
    SND_Stop_All_Seqs_4CA850();
    SND_Free_All_Seqs_4C9F40();
    SND_Free_All_VABS_4C9EB0();
    PSX_SsSetMVol_4FC360(100, 100);
}

EXPORT void MIDI_SsEnd_4FC350()
{
    sMidi_Inited_dword_BD1CF4 = 0;
}

EXPORT void SND_Shutdown_4CA280()
{
    SND_Reset_4C9FB0();

    if (sMonkVh_Vb_560F48.field_8_vab_id >= 0)
    {
        ResourceManager::FreeResource_Impl_49C360(sMonkVh_Vb_560F48.field_C_pVabHeader);
        sMonkVh_Vb_560F48.field_C_pVabHeader = nullptr;

        SND_SsVabClose_4FC5B0(sMonkVh_Vb_560F48.field_8_vab_id);
        sMonkVh_Vb_560F48.field_8_vab_id = -1;
    }

    PSX_SsSetMVol_4FC360(0, 0);
    SsUtReverbOff_4FE350();
    SsUtSetReverbDepth_4FE380(0, 0);
    MIDI_SsEnd_4FC350();

    for (int i = 0; i < kNumChannels; i++)
    {
        if (sMidi_Channels_C14080.channels[i].field_1C.field_3)
        {
            SND_Stop_Sample_At_Idx_4EFA90(sMidi_Channels_C14080.channels[i].field_0_sound_buffer_field_4);
        }
    }

    for (int i = 0; i < kMaxVabs; i++)
    {
        if (sVagCounts_BE6144[i] > 0)
        {
            SND_SsVabClose_4FC5B0(i);
        }
    }

    SND_SsQuit_4EFD50();
}

EXPORT void CC MIDI_ADSR_Update_4FDCE0()
{
    for (int i=0; i<kNumChannels; i++)
    {
        MIDI_Struct1* pChannel = &sMidi_Channels_C14080.channels[i];
        if (pChannel->field_1C.field_3)
        {
            signed int timeDiff1 = sMidiTime_BD1CEC - pChannel->field_14_time;
            signed int timeDiff2 = sMidiTime_BD1CEC - pChannel->field_14_time;

            if ((signed int)(sMidiTime_BD1CEC - pChannel->field_14_time) < 0)
            {
                timeDiff1 = 0;
                timeDiff2 = 0;
            }
            
            int timeDiffSquared = timeDiff1 * timeDiff1;
            switch (pChannel->field_1C.field_3 + 1) // ADSR state ?
            {
            case 0:
                if (timeDiff1 > 90000)
                {
                    MIDI_Stop_Channel_4FE010(static_cast<short>(i));
                }
                break;
            case 2:
                if (timeDiff1 >= pChannel->field_1C.field_4_attack)
                {
                    pChannel->field_1C.field_3 = 2;
                    pChannel->field_14_time += pChannel->field_1C.field_4_attack;
                    timeDiff1 -= pChannel->field_1C.field_4_attack;
                    // Fall through to case 3
                }
                else
                {
                    MIDI_Set_Volume_4FDE80(
                        pChannel,
                        (signed __int64)(((double)timeDiff2 / (double)pChannel->field_1C.field_4_attack
                            + (double)timeDiff2 / (double)pChannel->field_1C.field_4_attack
                            - (double)timeDiff2 / (double)pChannel->field_1C.field_4_attack * ((double)timeDiff2 / (double)pChannel->field_1C.field_4_attack))
                            * (double)pChannel->field_C));
                    break;
                }
            case 3:
                if (timeDiff1 < pChannel->field_1C.field_6_sustain)
                {
                    const int v8 = pChannel->field_C * (16 - pChannel->field_1C.field_8_decay) >> 4;
                    MIDI_Set_Volume_4FDE80(pChannel, pChannel->field_C - timeDiffSquared * v8 / (pChannel->field_1C.field_6_sustain * pChannel->field_1C.field_6_sustain));
                    break;
                }
                pChannel->field_1C.field_3 = 3;
                pChannel->field_14_time += pChannel->field_1C.field_6_sustain;
                timeDiff1 -= pChannel->field_1C.field_6_sustain;
                if (MIDI_Set_Volume_4FDE80(pChannel, pChannel->field_C * pChannel->field_1C.field_8_decay >> 4))
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
                    pChannel->field_1C.field_3 = 4;
                    pChannel->field_14_time = sMidiTime_BD1CEC;
                    timeDiff1 = 0;
                    timeDiffSquared = 0;
                    pChannel->field_C = pChannel->field_8_left_vol;
                    // Fall through to case 5
                }
                else
                {
                    break;
                }
            case 5:
                if (timeDiff1 >= pChannel->field_1C.field_A_release)
                {
                    pChannel->field_1C.field_3 = 0;
                    SND_Stop_Sample_At_Idx_4EFA90(pChannel->field_0_sound_buffer_field_4);
                }
                else
                {
                    MIDI_Set_Volume_4FDE80(pChannel, pChannel->field_C - timeDiffSquared * pChannel->field_C / (pChannel->field_1C.field_A_release * pChannel->field_1C.field_A_release));
                }
                break;
            default:
                break;
            }
        }
    }
}

ALIVE_VAR(1, 0xbd1ce4, char, sbDisableSeqs_BD1CE4, 0);
ALIVE_VAR(1, 0x578E20, DWORD, sLastTime_578E20, 0xFFFFFFFF);

EXPORT void CC MIDI_SetTempo_4FDB80(__int16 idx, __int16 kZero, __int16 tempo)
{
    if (!kZero)
    {
        if (sMidiStruct2Ary32_C13400.table[idx].field_10 >= 0)
        {
            sMidiStruct2Ary32_C13400.table[idx].field_14 = tempo;
        }
        else
        {
            sMidiStruct2Ary32_C13400.table[idx].field_14 = -1000000 / sMidiStruct2Ary32_C13400.table[idx].field_10;
        }
    }
}


ALIVE_VAR(1, 0xbd1cf0, DWORD, sMidi_WaitUntil_BD1CF0, 0);

EXPORT void CC MIDI_Wait_4FCE50()
{
    while (SYS_GetTicks() < sMidi_WaitUntil_BD1CF0)
    {

    }
    sMidi_WaitUntil_BD1CF0 = 0;
}

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

EXPORT void CC MIDI_Read_SEQ_Header_4FD870(BYTE** pSrc, SeqHeader* pDst, unsigned int size)
{
    memcpy(pDst, *pSrc, size);
    (*pSrc) += size;
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

EXPORT __int16 CC MIDI_SsSeqOpen_4FD6D0(BYTE* pSeqData, __int16 seqIdx)
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
    for (int i = 0; i<32; i++)
    {
        if (!sMidiStruct2Ary32_C13400.table[i].field_0_seq_data)
        {
            break;
        }
        freeIdx++;
    }

    // Steal the first entry if none free
    if (freeIdx == 32)
    {
        freeIdx = 0;
        MIDI_Stop_None_Ended_Seq_4FD8D0(0);
    }

    // Init its fields
    memset(&sMidiStruct2Ary32_C13400.table[freeIdx], 0, sizeof(MIDI_Struct2));
    for (int i = 0; i < 16; i++)
    {
        sMidiStruct2Ary32_C13400.table[freeIdx].field_32[i].field_1_left_vol = 112;
        sMidiStruct2Ary32_C13400.table[freeIdx].field_32[i].field_2_right_vol = 64;
    }

    // Set data based on SEQ header
    sMidiStruct2Ary32_C13400.table[freeIdx].field_2E = -1;
    sMidiStruct2Ary32_C13400.table[freeIdx].field_10 = SwapBytes(seqHeader.field_8_resolution_of_quater_note);
    sMidiStruct2Ary32_C13400.table[freeIdx].field_31 = seqHeader.field_E_time_signature_beats;
    sMidiStruct2Ary32_C13400.table[freeIdx].field_30 = seqHeader.field_D_time_signature_bars;
    
    const int quaterNoteRes = sMidiStruct2Ary32_C13400.table[freeIdx].field_10;
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
        const int calculatedQuaterNoteRes = -(static_cast<unsigned __int8>(quaterNoteRes) * (-sMidiStruct2Ary32_C13400.table[freeIdx].field_10 >> 8));
        sMidiStruct2Ary32_C13400.table[freeIdx].field_10 = calculatedQuaterNoteRes;
        calculatedTempo = -1000000 / calculatedQuaterNoteRes;
    }

    sMidiStruct2Ary32_C13400.table[freeIdx].field_14 = calculatedTempo;
    sMidiStruct2Ary32_C13400.table[freeIdx].field_1C_pSeqData = pSeqData;
    sMidiStruct2Ary32_C13400.table[freeIdx].field_0_seq_data = pSeqData;
    sMidiStruct2Ary32_C13400.table[freeIdx].field_C_volume = 112;
    sMidiStruct2Ary32_C13400.table[freeIdx].field_seq_idx = seqIdx;

    return static_cast<short>(freeIdx);
}

EXPORT void CC SND_Stop_Channels_Mask_4CA810(DWORD bitMask)
{
    for (int i = 0; i < kNumChannels; i++)
    {
        // Does the index match a bit in the bitmask?
        if ((1 << i) & bitMask)
        {
            // Turn it off
            MIDI_Stop_Channel_4FE010(static_cast<short>(i));
        }
    }
}

EXPORT __int16 CC SND_SEQ_Play_4CAB10(unsigned __int16 idx, __int16 a2, __int16 volLeft, __int16 volRight)
{
    SeqDataRecord& rec = sSeqDataTable_BB2E38[idx];
    if (!rec.field_C_ppSeq_Data)
    {
        return 0;
    }

    // SEQ isn't in use
    if (rec.field_A_id < 0)
    {
        // Too many playing
        if (sSeqsPlaying_count_word_BB2E3C >= 16)
        {
            // Stop any SEQs that are done
            SND_SsSeqClose_4CA8E0();

            // If none where done then can't continue
            if (sSeqsPlaying_count_word_BB2E3C >= 16)
            {
                return 0;
            }
        }

        // Open the SEQ
        const short vabId = static_cast<short>(sLastLoadedSoundBlockInfo_BB2E34[rec.field_8_sound_block_idx].field_8_vab_id);
        rec.field_A_id = MIDI_SsSeqOpen_4FD6D0(rec.field_C_ppSeq_Data, vabId);

        // Index into the IDS via the seq ID and map it to the index
        sSeq_Ids_word_BB2354.ids[rec.field_A_id] = idx;
        sSeqsPlaying_count_word_BB2E3C++;
    }
    else if (MIDI_SsIsEos_4FDA80(rec.field_A_id, 0))
    {
        MIDI_SsSeqStop_4FD9C0(rec.field_A_id);
    }

    // Clamp left
    signed __int16 clampedVolLeft = 0;
    if (volLeft < 10)
    {
        clampedVolLeft = 10;
    }
    else if (volLeft >= 127)
    {
        clampedVolLeft = 127;
    }
    else
    {
        clampedVolLeft = volLeft;
    }

    // Clamp right
    signed __int16 clampedVolRight = 0;
    if (volRight < 10)
    {
        clampedVolRight = 10;
    }
    else
    {
        clampedVolRight = volRight;
        if (volRight >= 127)
        {
            clampedVolRight = 127;
        }
    }

    MIDI_SsSeqSetVol_4FDAC0(rec.field_A_id, clampedVolLeft, clampedVolRight);

    if (a2)
    {
        MIDI_SsSeqPlay_4FD900(rec.field_A_id, 1, a2);
    }
    else
    {
        MIDI_SsSeqPlay_4FD900(rec.field_A_id, 1, 0);
    }

    return 1;
}

// TODO: Should be refactored/combined with SND_SEQ_Play_4CAB10
EXPORT signed __int16 CC SND_SEQ_PlaySeq_4CA960(unsigned __int16 idx, __int16 a2, __int16 bDontStop)
{
    SeqDataRecord& rec = sSeqDataTable_BB2E38[idx];
    if (!rec.field_C_ppSeq_Data)
    {
        return 0;
    }

    if (rec.field_A_id < 0)
    {
        if (sSeqsPlaying_count_word_BB2E3C >= 16)
        {
            SND_SsSeqClose_4CA8E0();
            if (sSeqsPlaying_count_word_BB2E3C >= 16)
            {
                return 0;
            }
        }

        const int vabId = sLastLoadedSoundBlockInfo_BB2E34[rec.field_8_sound_block_idx].field_8_vab_id;
        rec.field_A_id = MIDI_SsSeqOpen_4FD6D0(rec.field_C_ppSeq_Data, static_cast<short>(vabId));

        sSeq_Ids_word_BB2354.ids[rec.field_A_id] = idx;
        sSeqsPlaying_count_word_BB2E3C++;
    }
    else if (MIDI_SsIsEos_4FDA80(rec.field_A_id, 0))
    {
        if (!bDontStop)
        {
            return 0;
        }
        MIDI_SsSeqStop_4FD9C0(rec.field_A_id);
    }

    // Clamp vol
    __int16 clampedVol = rec.field_9;
    if (clampedVol <= 10)
    {
        clampedVol = 10;
    }
    else
    {
        if (clampedVol >= 127)
        {
            clampedVol = 127;
        }
    }

    MIDI_SsSeqSetVol_4FDAC0(rec.field_A_id, clampedVol, clampedVol);
    if (a2)
    {
        MIDI_SsSeqPlay_4FD900(rec.field_A_id, 1, a2);
    }
    else
    {
        MIDI_SsSeqPlay_4FD900(rec.field_A_id, 1, 0);
    }

    return 1;
}


ALIVE_VAR(1, 0xbb2e3e, WORD, sSnd_ReloadAbeResources_BB2E3E, 0);

struct VabBodyRecord
{
    int field_0_length_or_duration;
    int field_4_unused;
    DWORD field_8_fileOffset;
};

ALIVE_VAR(1, 0xbd1ce0, FILE *, sSoundDatFileHandle_BD1CE0, nullptr);

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

    fseek_521955(sSoundDatFileHandle_BD1CE0, sampleOffset, 0);
    fread_520B5C(pBuffer, 2 * sampleLen, 1u, sSoundDatFileHandle_BD1CE0);
    return sampleLen;
}

EXPORT void CC SND_LoadSoundDat_4FC840(VabBodyRecord* pVabBody, __int16 vabId)
{
    if (vabId < 0)
    {
        return;
    }

    sSoundDatFileHandle_BD1CE0 = fopen_520C64("sounds.dat", "rb");
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
            MIDI_UpdatePlayer_4FDC80();
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
                void* pTempBuffer = malloc_4F4E60(sampleLen * pEntry->field_1D_blockAlign);
                if (pTempBuffer)
                {
                    // Read the sample data
                    memset(pTempBuffer, 0, sampleLen * pEntry->field_1D_blockAlign);
                    if (SND_SoundsDat_Read_4FC4E0(pVabHeader, pVabBody, i, pTempBuffer))
                    {
                        // Load it into the sound buffer
                        SND_Load_4EF680(pEntry, pTempBuffer, sampleLen);
                    }
                    mem_free_4F4EA0(pTempBuffer);
                }
            }
        }
    }

    if (sSoundDatFileHandle_BD1CE0)
    {
        fclose_520CBE(sSoundDatFileHandle_BD1CE0);
        sSoundDatFileHandle_BD1CE0 = nullptr;
    }
}

EXPORT signed __int16 CC SND_VAB_Load_4C9FE0(SoundBlockInfo* pSoundBlockInfo, __int16 vabId)
{
    // Fail if no file name
    if (!pSoundBlockInfo->field_0_vab_header_name)
    {
        return 0;
    }

    // Find the VH file record
    int headerSize = 0;
    LvlFileRecord* pVabHeaderFile = sLvlArchive_5BC520.Find_File_Record_433160(pSoundBlockInfo->field_0_vab_header_name);
    if (sbEnable_PCOpen_5CA4B0)
    {
        headerSize = pVabHeaderFile->field_14_file_size;
    }
    else
    {
        headerSize = pVabHeaderFile->field_10_num_sectors << 11;
    }

    // Load the VH file data
    BYTE** ppVabHeader = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_VabHeader, vabId, headerSize);
    pSoundBlockInfo->field_C_pVabHeader = *ppVabHeader;
    sLvlArchive_5BC520.Read_File_4330A0(pVabHeaderFile, *ppVabHeader);
    pResourceManager_5C1BB0->LoadingLoop_465590(0);
    
    // Find the VB file record
    LvlFileRecord* pVabBodyFile = sLvlArchive_5BC520.Find_File_Record_433160(pSoundBlockInfo->field_4_vab_body_name);
    if (!pVabBodyFile)
    {
        // For some reason its acceptable to assume we have a VH with no VB, but the VH must always exist, this happens for MONK.VB
        return 0;
    }

    int vabBodySize = 0;
    if (sbEnable_PCOpen_5CA4B0)
    {
        vabBodySize = pVabBodyFile->field_14_file_size;
    }
    else
    {
        vabBodySize = pVabBodyFile->field_10_num_sectors << 11;
    }

    // Load the VB file data
    BYTE** ppVabBody = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_VabBody, vabId, vabBodySize);
    if (!ppVabBody)
    {
        // Maybe filed due to OOM cause its huge, free the abe resources and try again
        if (!sSnd_ReloadAbeResources_BB2E3E)
        {
            sSnd_ReloadAbeResources_BB2E3E = TRUE;
            sActiveHero_5C1B68->Free_Resources_44D420();
        }

        // Compact/reclaim any other memory we can too
        ResourceManager::Reclaim_Memory_49C470(0);

        // If it fails again there is no recovery, in either case caller will restore abes resources
        ppVabBody = ResourceManager::Alloc_New_Resource_49BED0(ResourceManager::Resource_VabBody, vabId, vabBodySize);
        if (!ppVabBody)
        {
            return 0;
        }
    }

    // Now we can read the actual VB data
    sLvlArchive_5BC520.Read_File_4330A0(pVabBodyFile, *ppVabBody);

    // Convert the records in the header to internal representation
    pSoundBlockInfo->field_8_vab_id = SND_Load_Vab_Header_4FC620(reinterpret_cast<VabHeader*>(pSoundBlockInfo->field_C_pVabHeader));

    // Load actual sample data
    SND_LoadSoundDat_4FC840(reinterpret_cast<VabBodyRecord*>(*ppVabBody), static_cast<short>(pSoundBlockInfo->field_8_vab_id));
    
    SsVabTransCompleted_4FE060(1); // SS_WAIT_COMPLETED

    // Now the sound samples are loaded we don't need the VB data anymore
    ResourceManager::FreeResource_49C330(ppVabBody);
    return 1;
}

EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, int reverb)
{
    SoundBlockInfo* pSoundBlockInfoIter = pSoundBlockInfo;
    sSnd_ReloadAbeResources_BB2E3E = FALSE;
    if (sLastLoadedSoundBlockInfo_BB2E34 != pSoundBlockInfo)
    {
        SsUtReverbOff_4FE350();
        SsUtSetReverbDepth_4FE380(0, 0);
        SpuClearReverbWorkArea_4FA690();
        
        if (sMonkVh_Vb_560F48.field_8_vab_id < 0)
        {
            SND_VAB_Load_4C9FE0(&sMonkVh_Vb_560F48, 32);
        }

        sLastLoadedSoundBlockInfo_BB2E34 = pSoundBlockInfo;

        __int16 vabId = 0;
        while (SND_VAB_Load_4C9FE0(pSoundBlockInfoIter, vabId))
        {
            ++vabId;
            ++pSoundBlockInfoIter;
        }

        // Put abes resources back if we had to unload them to fit the VB in memory
        if (sSnd_ReloadAbeResources_BB2E3E)
        {
            ResourceManager::Reclaim_Memory_49C470(0);
            sActiveHero_5C1B68->Load_Basic_Resources_44D460();
        }

        SsUtSetReverbDepth_4FE380(reverb, reverb);
        SsUtReverbOn_4FE340();
    }
}

EXPORT signed int CC MIDI_Set_Volume_4FDE80(MIDI_Struct1* pData, int vol)
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

    pData->field_1C.field_3 = 0;
    return 0;
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

ALIVE_VAR(1, 0xbd1cfc, BYTE, sControllerValue_BD1CFC, 0);

// TODO: Complete tests so this can be rewritten
EXPORT signed int CC MIDI_ParseMidiMessage_4FD100(int idx)
{
    //NOT_IMPLEMENTED();

    int idx2; // ebp
    MIDI_Struct2 *pCtx; // esi
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
    MIDI_3_Bytes  *v18; // ecx
    signed int v19; // ebx
    MIDI_Struct1_Sub *pSubChan2; // esi
    char v21; // bl
    char channelIdx_1; // cl
    
    MIDI_Struct1_Sub *pSubChan1; // esi
    unsigned __int8 refCount1; // dl
    MIDI_Struct1_Sub *pSub1; // esi
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
    MIDI_3_Bytes *v43; // [esp+18h] [ebp-18h]
    int v45; // [esp+20h] [ebp-10h]
    int leftVol; // [esp+2Ch] [ebp-4h]
    int v47; // [esp+2Ch] [ebp-4h]
    MIDI_3_Bytes* v32;

    idx2 = idx;
    pCtx = &sMidiStruct2Ary32_C13400.table[idx];
    if (sMidiStruct2Ary32_C13400.table[idx].field_4_time > sMidiTime_BD1CEC)
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
                    oldLoopCount = sMidiStruct2Ary32_C13400.table[idx2].field_18;
                    if (oldLoopCount)
                    {
                        newLoopCount = oldLoopCount - 1;
                        sMidiStruct2Ary32_C13400.table[idx2].field_18 = newLoopCount;
                        if (!newLoopCount)
                        {
                            MIDI_SsSeqStop_4FD9C0(static_cast<short>(idx));
                            return 1;
                        }
                    }
                    pCtx->field_0_seq_data = sMidiStruct2Ary32_C13400.table[idx2].field_1C_pSeqData;
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
        sMidiStruct2Ary32_C13400.table[idx2].field_2A = static_cast<unsigned char>(midiByte1_copy);// running status ?

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
            v32 = &sMidiStruct2Ary32_C13400.table[idx2].field_32[v29];
            for (int i = 0; i < 24; i++)
            {
                pSub1 = &sMidi_Channels_C14080.channels[i].field_1C;
                if (pSub1->field_3)
                {
                    if (pSub1->field_0_seq_idx == sMidiStruct2Ary32_C13400.table[idx].field_seq_idx
                        && pSub1->field_1_program == v32->field_0_program // or field_2_not_inited ??
                        && pSub1->field_C == v29 + 16 * idx
                        && pSub1->field_2_note_byte1 == BYTE1(v16))
                    {
                        v33 = pSub1->field_E - 1;
                        pSub1->field_E = v33;
                        if (!v33)
                        {
                            MIDI_Stop_Channel_4FE010(v31);
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
            v18 = &sMidiStruct2Ary32_C13400.table[idx2].field_32[v17];
            v43 = v18;
            leftVol = (signed __int16)((unsigned int)(v18->field_1_left_vol * sMidiStruct2Ary32_C13400.table[idx2].field_C_volume) >> 7);
            if (v16 >> 16)
            {
                v19 = 0;
               
                for (int i = 0; i < 24; i++)
                {
                    pSubChan2 = &sMidi_Channels_C14080.channels[i].field_1C;
                    if (pSubChan2->field_3
                        && pSubChan2->field_0_seq_idx == sMidiStruct2Ary32_C13400.table[idx2].field_seq_idx
                        && pSubChan2->field_1_program == v18->field_0_program
                        && pSubChan2->field_C == v45 + 16 * idx)
                    {
                        if (pSubChan2->field_2_note_byte1 == BYTE1(v16) && pSubChan2->field_E > v19)
                        {
                            v19 = pSubChan2->field_E;
                        }
                        v18 = v43;
                    }
                }

                v21 = static_cast<char>(v19 + 1);
                v47 = MIDI_PlayerPlayMidiNote_4FCE80(
                    sMidiStruct2Ary32_C13400.table[idx2].field_seq_idx,
                    v18->field_0_program,
                    v16 & 0xFF00,
                    leftVol,
                    v18->field_2_right_vol,
                    v16 >> 16);
                channelIdx_1 = 0;

                for (int i=0; i<24; i++)
                {
                    if ((1 << channelIdx_1) & v47)
                    {
                        sMidi_Channels_C14080.channels[i].field_1C.field_C = static_cast<WORD>(16 * idx + (v42 & 0xF));
                        sMidi_Channels_C14080.channels[i].field_1C.field_E = v21;
                    }
                    ++channelIdx_1;
                }
            }
            else
            {
                for (short i = 0; i < 24; i++)
                {
                    pSubChan1 = &sMidi_Channels_C14080.channels[i].field_1C;
                    if (pSubChan1->field_3)
                    {
                        if (pSubChan1->field_0_seq_idx == sMidiStruct2Ary32_C13400.table[idx2].field_seq_idx
                            && pSubChan1->field_1_program == v18->field_0_program
                            && pSubChan1->field_C == v45 + 16 * idx
                            && pSubChan1->field_2_note_byte1 == BYTE1(v16))
                        {
                            refCount1 = pSubChan1->field_E - 1;
                            pSubChan1->field_E = refCount1;
                            if (!refCount1)
                            {
                                MIDI_Stop_Channel_4FE010(i);
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
                sMidiStruct2Ary32_C13400.table[idx2].field_24_loop_start = pCtx->field_0_seq_data;
                sMidiStruct2Ary32_C13400.table[idx2].field_2C_loop_count = BYTE2(cmd);
                break;
            case 30:                              // Loop
                v36 = sMidiStruct2Ary32_C13400.table[idx2].field_24_loop_start;
                if (v36)
                {
                    if (sMidiStruct2Ary32_C13400.table[idx2].field_2C_loop_count > 0)
                    {
                        pCtx->field_0_seq_data = v36;
                        v37 = sMidiStruct2Ary32_C13400.table[idx2].field_2C_loop_count;
                        if (v37 < 127)
                        {
                            sControllerValue_BD1CFC = 0;
                            sMidiStruct2Ary32_C13400.table[idx2].field_2C_loop_count = v37 - 1;
                            goto next_time_stamp;
                        }
                    }
                }
                break;
            case 40:
                pFn = (void(CC *)(int, DWORD, DWORD))sMidiStruct2Ary32_C13400.table[idx2].field_20_fn_ptr;
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
            sMidiStruct2Ary32_C13400.table[idx2].field_32[v16 & 0xF].field_0_program = BYTE1(v16);
        }
        break;

        case 0xE0u:                               // Pitch bend
        {
            MIDI_PitchBend_4FDEC0(
                sMidiStruct2Ary32_C13400.table[idx2].field_32[v16 & 0xF].field_0_program,
                static_cast<short>(((v16 >> 8) - 0x4000) >> 4));
        }
            break;
        default:
            break;
        }

    next_time_stamp:

        pCtx = &sMidiStruct2Ary32_C13400.table[idx2];
        // Next time stamp
        v38 = MIDI_Read_Var_Len_4FD0D0(&sMidiStruct2Ary32_C13400.table[idx2]);
        if (v38)
        {
            v39 = sMidiTime_BD1CEC;
            v40 = v38 * sMidiStruct2Ary32_C13400.table[idx2].field_14 / 1000u + sMidiStruct2Ary32_C13400.table[idx2].field_4_time;
            sMidiStruct2Ary32_C13400.table[idx2].field_4_time = v40;
            if (v40 > v39)
            {
                return 1;
            }
        }
    } // Loop end

    if (sMidiStruct2Ary32_C13400.table[idx2].field_2A)
    {
        midiByte1_copy = (unsigned __int8)sMidiStruct2Ary32_C13400.table[idx2].field_2A;
        goto handle_next_event;
    }
    return 0;
}

EXPORT void CC MIDI_UpdatePlayer_4FDC80()
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
                if (sMidiStruct2Ary32_C13400.table[i].field_0_seq_data)
                {
                    if (sMidiStruct2Ary32_C13400.table[i].field_2B == 1)
                    {
                        MIDI_ParseMidiMessage_4FD100(i);
                    }
                }
            }
            MIDI_ADSR_Update_4FDCE0();
        }
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
        if (sMidi_Channels_C14080.channels[i].field_1C.field_3 == 0)
        {
            return i;
        }
        else
        {
            //const int v6 = sMidi_Channels_C14080.channels[idx].field_14_time;
            //const int v7 = sMidi_Channels_C14080.channels[idx].field_18_rightVol;
            const int inverted = MIDI_Invert_4FCA40(sMidi_Channels_C14080.channels[i].field_4, sMidi_Channels_C14080.channels[i].field_8_left_vol);
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
            sMidi_Channels_C14080.channels[i].field_1C.field_3 = 0;
            return i;
        }
    }

    // Take the channel which has sound that is ending soonest
    int idx = timeMod24;
    if (priority < sMidi_Channels_C14080.channels[idx].field_4)
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
                    MIDI_Struct1* pChannel = &sMidi_Channels_C14080.channels[midiChannel];
                    const BOOL bUnknown = pVagIter->field_0_adsr_attack || pVagIter->field_2_adsr_sustain_level || pVagIter->field_4_adsr_decay != 16 || pVagIter->field_6_adsr_release >= 33u;
                    pChannel->field_C = maxPan;
                    if (bUnknown)
                    {
                        pChannel->field_1C.field_3 = 1;

                        pChannel->field_1C.field_4_attack = static_cast<unsigned short>((pVagIter->field_0_adsr_attack * (127 - volume)) / 64);
                        pChannel->field_1C.field_6_sustain = pVagIter->field_2_adsr_sustain_level;
                        pChannel->field_1C.field_8_decay = pVagIter->field_4_adsr_decay;
                        pChannel->field_1C.field_A_release = pVagIter->field_6_adsr_release;

                        if (pChannel->field_1C.field_4_attack)
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
                        pChannel->field_1C.field_3 = -1;
                    }
                    else
                    {
                        pChannel->field_1C.field_3 = -2;
                    }

                    pChannel->field_8_left_vol = maxPan;
                    pChannel->field_4 = pVagIter->field_E_priority;
                    pChannel->field_18_rightVol = playFlags;
                    pChannel->field_14_time = sMidiTime_BD1CEC;
                    pChannel->field_1C.field_0_seq_idx = static_cast<unsigned char>(vabId);
                    pChannel->field_1C.field_1_program = static_cast<unsigned char>(program);
                    pChannel->field_1C.field_2_note_byte1 = static_cast<unsigned char>(noteKeyNumber);

#if ORIGINAL_PS1_BEHAVIOR
                    // Uses the correct way of determining the pitch float.
                    // Almost identical to PS1 versions pitches (at least from PS1 emulators)
                    // Todo: check real ps1 hardware.
                    pChannel->field_10_float = static_cast<float>(pow(1.059463094359, (double)((note / 256.0) - (pVagIter->field_A_shift_cen / 256.0))));
#else
                    pChannel->field_10_float = static_cast<float>(pow(1.059463094359, (double)(note - pVagIter->field_A_shift_cen) * 0.00390625));
#endif

                    if (sMidi_WaitUntil_BD1CF0)
                    {
                        MIDI_Wait_4FCE50();
                    }

#if USE_SDL2_SOUND
                    float pan = (pVagIter->field_11_pad - 64) / (127.0f / 2.0f);

                    if (panLeft > panRight)
                    {
                        pan -= (1.0f - (panRight / static_cast<float>(panLeft)));
                    }
                    else if (panRight > panLeft)
                    {
                        pan += (1.0f - (panLeft / static_cast<float>(panRight)));
                    }

                    pan = std::min(std::max(pan, -1.0f), 1.0f);

                    SND_Play_SDL(
                        &sSoundEntryTable16_BE6160.table[vabId][pVagIter->field_10_vag],
                        ((volume * std::max(leftVol2, rightVol2) * vagVol * sGlobalVolumeLevel_left_BD1CDC) >> 21) / 127.0f,
                        pan,
                        pChannel->field_10_float, // freq
                        pChannel,
                        playFlags,
                        pVagIter->field_E_priority);
#else
                    SND_PlayEx_4EF740(
                        &sSoundEntryTable16_BE6160.table[vabId][pVagIter->field_10_vag],
                        panLeft,
                        panRight,
                        pChannel->field_10_float, // freq
                        pChannel,
                        playFlags,
                        pVagIter->field_E_priority);
#endif

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

EXPORT __int16 CC MIDI_Set_Freq_4FDF70(__int16 idx, int /*program*/, int /*vabId*/, __int16 noteLo, __int16 kZero, __int16 noteHigh, __int16 a7)
{
    const float freq = pow(1.059463094359f, (float)(a7 + ((noteHigh - (signed int)noteLo) << 7) - kZero) * 0.0078125f);
    SND_Buffer_Set_Frequency_4EFC90(sMidi_Channels_C14080.channels[idx].field_0_sound_buffer_field_4, freq);
    return 0;
}

EXPORT __int16 CC MIDI_PitchBend_4FDEC0(__int16 field4_match, __int16 pitch)
{
    const float pitcha = pow(1.059463094359f, (float)pitch * 0.0078125f);
    for (int i = 0; i < kNumChannels; i++)
    {
        if (sMidi_Channels_C14080.channels[i].field_1C.field_1_program == field4_match)
        {
            const float freq = pitcha * sMidi_Channels_C14080.channels[i].field_10_float;
            SND_Buffer_Set_Frequency_4EFC00(sMidi_Channels_C14080.channels[i].field_C, freq);
        }
    }
    return 0;
}

namespace Test
{
    class MidiStreamBuilder
    {
    public:
        void AddNoteOn(BYTE channel, BYTE noteNumber, BYTE velocity)
        {
            mData.push_back(0x90 | channel);
            mData.push_back(noteNumber);
            mData.push_back(velocity);
            AddTimeStamp();
        }

        void AddTimeStamp()
        {
            mData.push_back(0x01);
        }

        BYTE* Data()
        {
            return mData.data();
        }
    private:
        std::vector<BYTE> mData;
    };

    static int CC Stub_MIDI_PlayerPlayMidiNote_4FCE80(int /*a1*/, int /*program*/, int /*note*/, int /*leftVol*/, int /*rightVol*/, int /*volume*/)
    {
        return 0;
    }

    //  TODO: Test all events
    static void Test_MIDI_ParseMidiMessage_4FD100()
    {
        SND_ResetData();

        SCOPED_REDIRECT(MIDI_PlayerPlayMidiNote_4FCE80, Stub_MIDI_PlayerPlayMidiNote_4FCE80);

        MIDI_Struct2* pCtx = &sMidiStruct2Ary32_C13400.table[0];

        // MIDI_SsSeqPlay_4FD900 already reads the first time stamp

        memset(pCtx, 0, sizeof(MIDI_Struct2));

        MidiStreamBuilder sb;
        sb.AddNoteOn(0x05, 0x77, 0x20);
        pCtx->field_0_seq_data = sb.Data();
        pCtx->field_2C_loop_count = 0;
        pCtx->field_14 = 1000;

        MIDI_ParseMidiMessage_4FD100(0);

        ASSERT_EQ(pCtx->field_2A, 0x95);
    }

    static void Test_SND_Load_Vab_Header_4FC620()
    {
        SND_ResetData();

        struct TestData
        {
            VabHeader header;
            VagAtr attr[128 * 16];
        };

        TestData t = {};
        int vab_id = 0;
        t.header.field_8_id = vab_id;
        t.header.field_12_num_progs = 1;
        t.header.field_14_num_tones = 1;
        t.header.field_16_num_vags = 1;

        VagAtr& atr = t.attr[0];
        atr.field_0_priority = 2;
        atr.field_1_mode = 3;
        atr.field_2_vol = 127;
        atr.field_3_pan = 4;
        atr.field_4_centre = 5;
        atr.field_5_shift = 6;
        atr.field_6_min = 7;
        atr.field_7_max = 8;
        atr.field_8_vibW = 9;
        atr.field_9_vibT = 10;
        atr.field_A_porW = 11;
        atr.field_B_porT = 12;
        atr.field_C_pitch_bend_min = 13;
        atr.field_D_pitch_bend_max = 14;
        atr.field_E_reserved1 = 15;
        atr.field_F_reserved2 = 16;
        atr.field_10_adsr1 = 17;
        atr.field_12_adsr2 = 18;
        atr.field_14_prog = 2;
        atr.field_16_vag = 12;
        for (short i = 0; i < 4; i++)
        {
            atr.field_18_reserved[i] = 8 + i;
        }

        SND_Load_Vab_Header_4FC620(&t.header);

        ASSERT_EQ(atr.field_14_prog, sConvertedVagTable_BEF160.table[vab_id][atr.field_14_prog][0].field_F_prog);
        ASSERT_EQ(atr.field_16_vag -1 , sConvertedVagTable_BEF160.table[vab_id][atr.field_14_prog][0].field_10_vag);

        // TODO: Test other converted fields

        SND_ResetData();
    }

    void MidiTests()
    {
        Test_MIDI_ParseMidiMessage_4FD100();
        Test_SND_Load_Vab_Header_4FC620();
    }
}
