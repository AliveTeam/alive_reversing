#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

struct SoundBlockInfo;
struct OpenSeqHandle;
struct SfxDefinition;

EXPORT void CC SND_Reset_476BA0();

EXPORT signed __int16 CC SND_Load_VABS_477040(SoundBlockInfo* pSoundBlockInfo, int reverb);

EXPORT void CC SND_Stop_Channels_Mask_4774A0(int mask);

EXPORT void CC SND_Load_Seqs_477AB0(OpenSeqHandle* pSeqTable, const char* bsqFileName);

EXPORT signed __int16 CC SND_SEQ_PlaySeq_4775A0(unsigned __int16 idx, int repeatCount, __int16 bDontStop);

EXPORT void CC SND_Seq_Stop_477A60(unsigned __int16 idx);

EXPORT signed __int16 CC SND_SEQ_Play_477760(unsigned __int16 idx, int repeatCount, __int16 volLeft, __int16 volRight);

EXPORT __int16 CC SND_SsIsEos_DeInlined_477930(unsigned __int16 idx);

EXPORT int CC SFX_SfxDefinition_Play_477330(const SfxDefinition* sfxDef, short volLeft, short volRight, short pitch_min, signed __int16 pitch_max);

EXPORT int CC SFX_SfxDefinition_Play_4770F0(const SfxDefinition* sfxDef, signed int vol, int pitch_min, int pitch_max);

END_NS_AO
