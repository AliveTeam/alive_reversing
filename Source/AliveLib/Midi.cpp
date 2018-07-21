#include "stdafx.h"
#include "Midi.hpp"
#include "Function.hpp"
#include "Sound.hpp"
#include "PathData.hpp"
#include "ResourceManager.hpp"
#include <timeapi.h>
#include <gmock/gmock.h>

void Midi_ForceLink() { }

EXPORT void CC SEQ_4CB060()
{
    NOT_IMPLEMENTED();
}

EXPORT int CC MIDI_46FBA0(unsigned __int8 a1, int a2, int a3, int a4)
{
    NOT_IMPLEMENTED();
}

struct MIDI_3_Bytes
{
    char field_0;
    char field_1;
    char field_2_not_inited;
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
    int field_20_fn_ptr;
    BYTE *field_24_loop_start;
    __int16 field_seq_idx;
    unsigned char field_2A;
    char field_2B;
    char field_2C_loop_count;
    char field_2D;
    __int16 field_2E;
    char field_30;
    char field_31;
    char field_32;
    MIDI_3_Bytes field_33[16];
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
ALIVE_ARY(1, 0xBE6144, char, kMaxVabs, sVagCounts_BE6144, {});
ALIVE_ARY(1, 0x0BDCD64, char, kMaxVabs, sProgCounts_BDCD64, {});


struct VabHeader
{
    int field_0_form;
    int field_4_version;
    int field_8_id;
    int field_C_file_size;
    __int16 field_10_reserved0;
    __int16 field_12_num_progs;
    __int16 field_14_num_tones;
    unsigned __int16 field_16_num_vags;
    char field_18_master_vol;
    char field_19_master_pan;
    char field_1A_attr1;
    char field_1B_attr2;
    int field_1C_reserved1;
};
ALIVE_ARY(1, 0xC13160, VabHeader*, 4, spVabHeaders_C13160, {});


EXPORT void CC SND_SsVabClose_4FC5B0(int /*vabId*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC SND_Load_Vab_Header_4FC620(VabHeader* /*pVabHeader*/)
{
    NOT_IMPLEMENTED();

    // TODO: Figure out vag conversion
    /*
    if (!pVabHeader)
    {
        return;
    }

    MIDI_UpdatePlayer_4FDC80();
    int numVags = pVabHeader->field_16_num_vags;
    const int vab_id = pVabHeader->field_8_id;
    SND_SsVabClose_4FC5B0(vab_id);
    int oldVabId = sVagCounts_BE6144[vab_id];
    spVabHeaders_C13160[vab_id] = pVabHeader;
    
    if (oldVabId)
    {
        SND_SsVabClose_4FC5B0(vab_id);
    }

    if (numVags > 256)
    {
        numVags = 0;
    }

    const int numProgs = pVabHeader->field_12_num_progs;
    sVagCounts_BE6144[vab_id] = static_cast<char>(numVags);
    sProgCounts_BDCD64[vab_id] = static_cast<char>(numProgs);
    //memset(s512_byte_C13180[vab_id], 0, sizeof(char[128]));

    BYTE* pIntoHeader = (BYTE*)pVabHeader + 65;
    //memset((char *)&unk_BEF160 + 36864 * vab_id, 0, 36864u);
    int numProgs2 = pVabHeader->field_12_num_progs;
    if (numProgs2 > 0)
    {

    }
    */
}

const int kSeqTableSize = 144;

struct SoundEntryTable
{
    SoundEntry table[kMaxVabs][256];
};
ALIVE_ASSERT_SIZEOF(SoundEntryTable, 36864);

ALIVE_VAR(1, 0xBE6160, SoundEntryTable, sSoundEntryTable16_BE6160, {});

struct Converted_Vag
{
    __int16 field_0_adsr1;
    __int16 field_2_adsr;
    __int16 field_4_adsr;
    __int16 field_6_adsr;
    char field_8_min;
    char field_9_max;
    __int16 field_A_shift_cen;
    char field_C;
    char field_D_vol;
    char field_E_priority;
    char field_F_prog;
    char field_10_vag;
    char field_11_pad;
};
ALIVE_ASSERT_SIZEOF(Converted_Vag, 0x12);

struct ConvertedVagTable
{
    Converted_Vag table[kMaxVabs][16][128]; // 16 = max tones, 128 = max progs
};
ALIVE_ASSERT_SIZEOF(ConvertedVagTable, 147456);
ALIVE_VAR(1, 0xBEF160, ConvertedVagTable, sConvertedVagTable_BEF160, {});

struct MIDI_Struct1_Sub
{
    unsigned __int8 field_0_seq_idx;
    unsigned __int8 field_1_program;
    unsigned __int8 field_2_note_byte1;
    char field_3;
    __int16 field_4;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A;
    unsigned __int16 field_C;
    unsigned __int8 field_E;
    char field_F;
};
ALIVE_ASSERT_SIZEOF(MIDI_Struct1_Sub, 0x10);

struct MIDI_Struct1
{
    int field_0_sound_buffer_field_4;
    int field_4;
    int field_8_left_vol;
    int field_C;
    float field_10_float;
    int field_14_time;
    int field_18_rightVol;
    MIDI_Struct1_Sub field_1C;
};
ALIVE_ASSERT_SIZEOF(MIDI_Struct1, 0x2C);

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

EXPORT void CC SND_SsInitHot_4FC230()
{
    sGlobalVolumeLevel_right_BD1CDE = 127;
    sGlobalVolumeLevel_left_BD1CDC = 127;

    SND_CreateDS_4EEAA0(22050u, 16, 1);

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
            sMidiStruct2Ary32_C13400.table[i].field_33[j].field_0 = 112;
            sMidiStruct2Ary32_C13400.table[i].field_33[j].field_1 = 64;
        }
    }
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
    sMidiTime_BD1CEC = timeGetTime();
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
    // NOTE: These are stubs in real
    //SsSetTableSize(&sTable_unk_BB2374, 16, 1);
    //SsUtSetReverbDepth_4FE380(0, 0);
    //SsUtSetReverbType_4FE360(4);
    //SsUtReverbOn_4FE340();
    //SsSetTickMode_4FDC20(4096);
    //SND_Set_VSyncCallback_4F8C40(SND_CallBack_4020A4);
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

struct SeqDataRecord
{
    const char *field_0_mName;
    int field_4_generated_res_id;
    char field_8_sound_block_idx;
    char field_9;
    __int16 field_A_id;
    BYTE *field_C_ppSeq_Data;
};
ALIVE_ASSERT_SIZEOF(SeqDataRecord, 0x10);

ALIVE_VAR(1, 0xbb2e38, SeqDataRecord *, sSeqDataTable_BB2E38, nullptr);

EXPORT __int16 CC MIDI_Stop_Channel_4FE010(__int16 idx)
{
    MIDI_Struct1* pChannel = &sMidi_Channels_C14080.channels[idx];
    if (pChannel->field_1C.field_3)
    {
        pChannel->field_1C.field_3 = 4;
        pChannel->field_C = pChannel->field_8_left_vol;
        if (pChannel->field_1C.field_A < 300)
        {
            pChannel->field_1C.field_A = 300;
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
        if (field_C == idx)
        {
            MIDI_Stop_Channel_4FE010(static_cast<short>(i));
            sMidi_Channels_C14080.channels[i].field_1C.field_C = 0;
        }
    }
}

EXPORT void CC SND_SEQ_Stop_4CAE60(unsigned __int16 idx)
{
    if (sSeqDataTable_BB2E38[idx].field_A_id != -1
        && sSeqDataTable_BB2E38[idx].field_C_ppSeq_Data)
    {
        if (MIDI_SsIsEos_4FDA80(sSeqDataTable_BB2E38[idx].field_A_id, 0))
        {
            MIDI_SsSeqStop_4FD9C0(sSeqDataTable_BB2E38[idx].field_A_id);
        }
    }
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
        if (sSeq_Ids_word_BB2354.ids[i] > 0)
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
        if (sSeq_Ids_word_BB2354.ids[i] > 0)
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

EXPORT signed int __cdecl SND_Stop_Sample_At_Idx_4EFA90(int /*idx*/)
{
    NOT_IMPLEMENTED();
    return 0;
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
    //SsUtReverbOff_4FE350();
    //SsUtSetReverbDepth_4FE380(0, 0);
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

EXPORT void CC MIDI_4FDCE0()
{
    NOT_IMPLEMENTED();
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
    while (timeGetTime() < sMidi_WaitUntil_BD1CF0)
    {

    }
    sMidi_WaitUntil_BD1CF0 = 0;
}

EXPORT __int16 CC MIDI_PitchBend_4FDEC0(__int16 /*a1*/, __int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC MIDI_PlayerPlayMidiNote_4FCE80(int /*a1*/, int /*program*/, int /*note*/, int /*leftVol*/, int /*rightVol*/, int /*volume*/)
{
    NOT_IMPLEMENTED();
    return 0;
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
    char *v18; // ecx
    signed int v19; // ebx
    MIDI_Struct1_Sub *pSubChan2; // esi
    char v21; // bl
    char channelIdx_1; // cl
    int v23; // edx
    signed int v24; // esi
    __int16 channelIdx; // di
    MIDI_Struct1_Sub *pSubChan1; // esi
    unsigned __int8 refCount1; // dl
    MIDI_Struct1_Sub *pSub1; // esi
    int v29; // edi
    int v30; // ecx
    __int16 v31 = 0; // bp
    char *v32; // ebx
    unsigned __int8 v33; // cl
    int v34; // eax
    void(__cdecl *pFn)(int, DWORD, DWORD); // eax
    BYTE *v36; // eax
    char v37; // al
    int v38; // eax
    unsigned int v39; // ecx
    unsigned int v40; // edi
    int v42; // [esp+14h] [ebp-1Ch]
    char *v43; // [esp+18h] [ebp-18h]
    signed int v44; // [esp+1Ch] [ebp-14h]
    int v45; // [esp+20h] [ebp-10h]
    int v46; // [esp+2Ch] [ebp-4h]
    int v47; // [esp+2Ch] [ebp-4h]

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
            pSub1 = &sMidi_Channels_C14080.channels[0].field_1C;
            v29 = v16 & 15;
            v30 = idx2 * 100 + 2 * v29;
            v31 = 0;
            v32 = &sMidiStruct2Ary32_C13400.table[0].field_32 + v29 + v30;
            do
            {
                if (pSub1->field_3)
                {
                    if (pSub1->field_0_seq_idx == sMidiStruct2Ary32_C13400.table[idx].field_seq_idx
                        && pSub1->field_1_program == *v32
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
                ++v31;
                pSub1 = (MIDI_Struct1_Sub *)((char *)pSub1 + 44);
            } while ((unsigned __int16)v31 < 24u);
            idx2 = idx;
            break;
        case 0x90u:                               // Note on
            v17 = v16 & 15;
            v45 = v17;
            v18 = &sMidiStruct2Ary32_C13400.table[0].field_32 + 2 * v17 + v17 + idx2 * 100;
            v43 = v18;
            v46 = (signed __int16)((unsigned int)(v18[1] * sMidiStruct2Ary32_C13400.table[idx2].field_C_volume) >> 7);
            if (v16 >> 16)
            {
                v19 = 0;
                pSubChan2 = &sMidi_Channels_C14080.channels[0].field_1C;
                v44 = 24;
                do
                {
                    if (pSubChan2->field_3
                        && pSubChan2->field_0_seq_idx == sMidiStruct2Ary32_C13400.table[idx2].field_seq_idx
                        && pSubChan2->field_1_program == *v18
                        && pSubChan2->field_C == v45 + 16 * idx)
                    {
                        if (pSubChan2->field_2_note_byte1 == BYTE1(v16) && pSubChan2->field_E > v19)
                            v19 = pSubChan2->field_E;
                        v18 = v43;
                    }
                    pSubChan2 = (MIDI_Struct1_Sub *)((char *)pSubChan2 + 0x2C);
                    --v44;
                } while (v44);
                v21 = static_cast<char>(v19 + 1);
                v47 = MIDI_PlayerPlayMidiNote_4FCE80(
                    sMidiStruct2Ary32_C13400.table[idx2].field_seq_idx,
                    *v18,
                    v16 & 0xFF00,
                    v46,
                    v18[2],
                    v16 >> 16);
                channelIdx_1 = 0;
                v23 = (int)&sMidi_Channels_C14080.channels[0].field_1C.field_E;
                v24 = 24;
                do
                {
                    if ((1 << channelIdx_1) & v47)
                    {
                        *(WORD *)(v23 - 2) = static_cast<WORD>(16 * idx + (v42 & 0xF));
                        *(BYTE *)v23 = v21;
                    }
                    ++channelIdx_1;
                    v23 += 0x2C;
                    --v24;
                } while (v24);
            }
            else
            {
                channelIdx = 0;
                pSubChan1 = &sMidi_Channels_C14080.channels[0].field_1C;
                do
                {
                    if (pSubChan1->field_3)
                    {
                        if (pSubChan1->field_0_seq_idx == sMidiStruct2Ary32_C13400.table[idx2].field_seq_idx
                            && pSubChan1->field_1_program == *v18
                            && pSubChan1->field_C == v45 + 16 * idx
                            && pSubChan1->field_2_note_byte1 == BYTE1(v16))
                        {
                            refCount1 = pSubChan1->field_E - 1;
                            pSubChan1->field_E = refCount1;
                            if (!refCount1)
                            {
                                MIDI_Stop_Channel_4FE010(channelIdx);
                                BYTE1(v16) = BYTE1(v42);
                                v18 = v43;
                                pSubChan1->field_C = 0;
                            }
                        }
                    }
                    ++channelIdx;
                    pSubChan1 = (MIDI_Struct1_Sub *)((char *)pSubChan1 + 44);
                } while ((unsigned __int16)channelIdx < 24u);
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
                pFn = (void(__cdecl *)(int, DWORD, DWORD))sMidiStruct2Ary32_C13400.table[idx2].field_20_fn_ptr;
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
            *(&sMidiStruct2Ary32_C13400.table[0].field_32 + 2 * (v16 & 0xF) + (v16 & 0xF) + idx2 * 100) = BYTE1(v16);
            break;
        case 0xE0u:                               // Pitch bend
            MIDI_PitchBend_4FDEC0(
                *(&sMidiStruct2Ary32_C13400.table[0].field_32 + 2 * (v16 & 0xF) + (v16 & 0xF) + idx2 * 100),
             static_cast<short>( ((v16 >> 8) - 0x4000) >> 4));
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
        const DWORD currentTime = timeGetTime();
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
            MIDI_4FDCE0();
        }
    }
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

    void MidiTests()
    {
        Test_MIDI_ParseMidiMessage_4FD100();
    }
}
