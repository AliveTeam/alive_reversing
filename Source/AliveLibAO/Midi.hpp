#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

struct SoundBlockInfo;
struct OpenSeqHandle;

EXPORT void CC SND_Reset_476BA0();

EXPORT signed __int16 CC SND_Load_VABS_477040(SoundBlockInfo* pSoundBlockInfo, int reverb);

EXPORT void CC SND_Stop_Channels_Mask_4774A0(int mask);

EXPORT void CC SND_Load_Seqs_477AB0(OpenSeqHandle* pSeqTable, const char* bsqFileName);

EXPORT signed __int16 CC SND_SEQ_PlaySeq_4775A0(unsigned __int16 idx, int repeatCount, __int16 bDontStop);

EXPORT void CC SND_Seq_Stop_477A60(unsigned __int16 idx);

END_NS_AO
