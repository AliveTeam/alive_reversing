#pragma once

#include "FunctionFwd.hpp"

void Midi_ForceLink();

namespace Test
{
    void MidiTests();
}

struct SoundBlockInfo;
struct SeqDataRecord;

EXPORT void CC MIDI_UpdatePlayer_4FDC80();
EXPORT void CC SEQ_4CB060();
EXPORT void CC SND_Init_4CA1F0();
EXPORT void CC SND_Shutdown_4CA280();
EXPORT int CC MIDI_46FBA0(unsigned __int8 a1, int a2, int a3, int a4);
EXPORT void CC SND_SsVabClose_4FC5B0(int vabId);
EXPORT void CC SND_Stop_Channels_Mask_4CA810(DWORD bitMask);
EXPORT int CC SND_4CA5D0(int a1, int a2, int a3, __int16 vol, __int16 min, __int16 max);
EXPORT void SND_Reset_4C9FB0();
EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, int reverb);
EXPORT void CC SND_Load_Seqs_4CAED0(SeqDataRecord* pSeqTable, const char* bsqFileName);
EXPORT void CC SND_SEQ_Stop_4CAE60(unsigned __int16 idx);
EXPORT signed __int16 CC SND_SEQ_PlaySeq_4CA960(unsigned __int16 idx, __int16 a2, __int16 bDontStop);
EXPORT void CC MIDI_Stop_All_Channels_4FDFE0();
