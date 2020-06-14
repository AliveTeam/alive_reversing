#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

struct SoundBlockInfo;
struct OpenSeqHandle;

EXPORT void CC SND_Reset_476BA0();

EXPORT signed __int16 CC SND_Load_VABS_477040(SoundBlockInfo* pSoundBlockInfo, int reverb);

EXPORT void CC SND_Load_Seqs_477AB0(OpenSeqHandle* pSeqTable, const char* bsqFileName);

END_NS_AO
