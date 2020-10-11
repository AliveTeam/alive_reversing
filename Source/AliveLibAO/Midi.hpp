#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

struct SoundBlockInfo;
struct OpenSeqHandle;
struct SfxDefinition;

enum class SeqId : __int16
{
	None_M1 = -1,
	Unknown_0 = 0,
	Unknown_1 = 1,
	Unknown_2 = 2,
	Unknown_3 = 3,
	Unknown_4 = 4,
	Unknown_5 = 5,
	Unknown_6 = 6,
	Unknown_7 = 7,
	Unknown_8 = 8,
	Unknown_9 = 9,
	Unknown_10 = 10,
	Unknown_11 = 11,
	Unknown_12 = 12,
	Unknown_13 = 13,
	Unknown_14 = 14,
	Unknown_15 = 15,
	Unknown_16 = 16,
	Unknown_17 = 17,
	Unknown_18 = 18,
	Unknown_19 = 19,
	Unknown_20 = 20,
	Unknown_21 = 21,
	Unknown_22 = 22,
	Unknown_23 = 23,
	Unknown_24 = 24,
	Unknown_25 = 25,
	Unknown_26 = 26,
	Unknown_27 = 27,
	Unknown_28 = 28,
	Unknown_29 = 29,
	Unknown_30 = 30,
	Unknown_31 = 31,
	Unknown_32 = 32,
	Unknown_33 = 33,
	Unknown_34 = 34,
	Unknown_35 = 35,
	Unknown_36 = 36,
	Unknown_37 = 37,
	Unknown_38 = 38,
	Unknown_39 = 39,
	Unknown_40 = 40,
	Unknown_41 = 41,
	Unknown_42 = 42,
	Unknown_43 = 43,
	Unknown_44 = 44,
	Unknown_45 = 45,
	Unknown_46 = 46
};

EXPORT void CC SND_Reset_476BA0();

EXPORT signed __int16 CC SND_Load_VABS_477040(SoundBlockInfo* pSoundBlockInfo, int reverb);

EXPORT void CC SND_Stop_Channels_Mask_4774A0(int mask);

EXPORT void CC SND_Load_Seqs_477AB0(OpenSeqHandle* pSeqTable, const char* bsqFileName);

EXPORT signed __int16 CC SND_SEQ_PlaySeq_4775A0(SeqId idx, int repeatCount, __int16 bDontStop);

EXPORT void CC SND_Seq_Stop_477A60(SeqId idx);

EXPORT signed __int16 CC SND_SEQ_Play_477760(SeqId idx, int repeatCount, __int16 volLeft, __int16 volRight);

EXPORT __int16 CC SND_SsIsEos_DeInlined_477930(SeqId idx);

EXPORT int CC SFX_SfxDefinition_Play_477330(const SfxDefinition* sfxDef, short volLeft, short volRight, short pitch_min, signed __int16 pitch_max);

EXPORT int CC SFX_SfxDefinition_Play_4770F0(const SfxDefinition* sfxDef, signed int vol, int pitch_min, int pitch_max);

EXPORT void CC SND_Init_476E40();

END_NS_AO
