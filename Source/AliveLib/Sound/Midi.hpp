#pragma once

#include "FunctionFwd.hpp"


struct SoundBlockInfo;
struct SeqDataRecord;


EXPORT void CC SND_StopAll_4CB060();
EXPORT void CC SND_Init_4CA1F0();
EXPORT void CC SND_Shutdown_4CA280();
EXPORT void CC SND_Stop_Channels_Mask_4CA810(DWORD bitMask);
EXPORT void SND_Reset_4C9FB0();
EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, int reverb);
EXPORT void CC SND_Load_Seqs_4CAED0(SeqDataRecord* pSeqTable, const char* bsqFileName);
EXPORT void CC SND_SEQ_Stop_4CAE60(unsigned __int16 idx);
EXPORT char CC SND_Seq_Table_Valid_4CAFE0();
EXPORT signed __int16 CC SND_SEQ_PlaySeq_4CA960(unsigned __int16 idx, __int16 repeatCount, __int16 bDontStop);
EXPORT void CC SND_SEQ_SetVol_4CAD20(int idx, __int16 volLeft, __int16 volRight);

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
