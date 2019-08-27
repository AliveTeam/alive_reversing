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
    unsigned __int16 field_4_attack;
    unsigned __int16 field_6_sustain;
    unsigned __int16 field_8_decay;
    unsigned __int16 field_A_release;
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

struct ProgAtr
{
    BYTE field_0_num_tones;
    char field_1_vol;
    char field_2_priority;
    char field_3_mode;
    char field_4_pan;
    char field_5_reserved0;
    __int16 field_6_attr;
    int field_8_reserved1;
    int field_C_reserved2;
};
ALIVE_ASSERT_SIZEOF(ProgAtr, 0x10);

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
    ProgAtr field_20_progs[128];
};
ALIVE_ASSERT_SIZEOF(VabHeader, 0x820);

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

struct Converted_Vag
{
    WORD field_0_adsr_attack;
    WORD field_2_adsr_sustain_level;
    WORD field_4_adsr_decay;
    WORD field_6_adsr_release;
    BYTE field_8_min;
    BYTE field_9_max;
    __int16 field_A_shift_cen;
    BYTE field_C;
    BYTE field_D_vol;
    BYTE field_E_priority;
    BYTE field_F_prog;
    BYTE field_10_vag;
    char field_11_pad;
};
ALIVE_ASSERT_SIZEOF(Converted_Vag, 0x12);

ALIVE_VAR_EXTERN(SoundBlockInfo*, sLastLoadedSoundBlockInfo_BB2E34);
ALIVE_ARY_EXTERN(VabHeader*, 4, spVabHeaders_C13160);

EXPORT void CC MIDI_UpdatePlayer_4FDC80();
EXPORT void CC SND_StopAll_4CB060();
EXPORT void CC SND_Init_4CA1F0();
EXPORT void CC SND_Shutdown_4CA280();
EXPORT void CC SND_SsVabClose_4FC5B0(int vabId);
EXPORT void CC SND_Stop_Channels_Mask_4CA810(DWORD bitMask);
EXPORT void SND_Reset_4C9FB0();
EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, int reverb);
EXPORT void CC SND_Load_Seqs_4CAED0(SeqDataRecord* pSeqTable, const char* bsqFileName);
EXPORT void CC SND_SEQ_Stop_4CAE60(unsigned __int16 idx);
EXPORT char CC SND_Seq_Table_Valid_4CAFE0();
EXPORT signed __int16 CC SND_SEQ_PlaySeq_4CA960(unsigned __int16 idx, __int16 a2, __int16 bDontStop);
EXPORT void CC MIDI_Stop_All_Channels_4FDFE0();
EXPORT int CC MIDI_Play_Single_Note_4CA1B0(int vabIdAndProgram, int note, int leftVol, int rightVol);
EXPORT int CC MIDI_PlayMidiNote_4FCB30(int vabId, int program, int note, int leftVolume, int rightVolume, int volume);
EXPORT signed int CC MIDI_Set_Volume_4FDE80(MIDI_Struct1* pData, int vol);
EXPORT void CC SND_SEQ_SetVol_4CAD20(int idx, __int16 volLeft, __int16 volRight);
EXPORT __int16 CC MIDI_PitchBend_4FDEC0(__int16 field4_match, __int16 pitch);
EXPORT __int16 CC MIDI_Set_Freq_4FDF70(__int16 idx, int /*program*/, int /*vabId*/, __int16 noteLo, __int16 kZero, __int16 noteHigh, __int16 a7);

enum SeqId
{
    MainMenuAmbient_0 = 0,
    MinesAmbient_1 = 1,
    MudomoAmbient_2 = 2,
    MudancheeAmbient_3 = 3,
    FeecoAmbient_4 = 4,
    BreweryAmbient_5 = 5,
    BonewerkzAmbient_6 = 6,
    BarracksAmbient_7 = 7,
    NecrumAmbient1_8 = 8,
    HitBottomOfDeathPit_9 = 9,
    MudokonChant1_10 = 10,
    MudokonChant_11 = 11,
    Scratching_12 = 12,
    Empty_13 = 13,
    Explosion1_14 = 14,
    Explosion2_15 = 15,
    AbeOops_16 = 16,
    NecrumAmbient2_17 = 17,
    AbeStopIt_18 = 18,
    WheelSqueak_19 = 19,
    MainMenuParamiteAttack_20 = 20,
    WalkingSligOffscreen_21 = 21,
    SleepingSligOffscreen_22 = 22,
    AwakeSlogOffscreen_23 = 23,
    SleepingSlogOffscreen_24 = 24,
    ParamiteOffscreen_25 = 25,
    SleepingFleechOffscreen_26 = 26,
    ToCalm1_27 = 27, //Unused, from Oddysee
    ToCalm2_28 = 28, //Unused, from Oddysee
    DeathDrums_29 = 29,
    AbeDeathDrums_30 = 30,
    SaveTriggerMusic_31 = 31,
    SecretMusic_32 = 32
};

EXPORT __int16 CC SND_SEQ_Play_4CAB10(unsigned __int16 idx, __int16 a2, __int16 volLeft, __int16 volRight);
EXPORT int CC SND_SsIsEos_DeInlined_4CACD0(unsigned __int16 idx);
