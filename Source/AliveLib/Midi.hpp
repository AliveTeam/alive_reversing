#pragma once

#include "FunctionFwd.hpp"

void Midi_ForceLink();

namespace Test
{
    void MidiTests();
}

struct SoundBlockInfo;
struct SeqDataRecord;

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


ALIVE_VAR_EXTERN(SoundBlockInfo *, sLastLoadedSoundBlockInfo_BB2E34);

EXPORT void CC MIDI_UpdatePlayer_4FDC80();
EXPORT void CC SEQ_4CB060();
EXPORT void CC SND_Init_4CA1F0();
EXPORT void CC SND_Shutdown_4CA280();

EXPORT void CC SND_SsVabClose_4FC5B0(int vabId);
EXPORT void CC SND_Stop_Channels_Mask_4CA810(DWORD bitMask);
EXPORT int CC SND_4CA5D0(int a1, int a2, int a3, __int16 vol, __int16 min, __int16 max);
EXPORT void SND_Reset_4C9FB0();
EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, int reverb);
EXPORT void CC SND_Load_Seqs_4CAED0(SeqDataRecord* pSeqTable, const char* bsqFileName);
EXPORT void CC SND_SEQ_Stop_4CAE60(unsigned __int16 idx);
EXPORT signed __int16 CC SND_SEQ_PlaySeq_4CA960(unsigned __int16 idx, __int16 a2, __int16 bDontStop);
EXPORT void CC MIDI_Stop_All_Channels_4FDFE0();

EXPORT int CC MIDI_4CA1B0(int a1, int note, int leftVol, int rightVol);