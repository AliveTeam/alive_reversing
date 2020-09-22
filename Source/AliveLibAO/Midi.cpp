#include "stdafx_ao.h"
#include "Midi.hpp"
#include "Function.hpp"

START_NS_AO

EXPORT void CC SND_Reset_476BA0()
{
    NOT_IMPLEMENTED();
}

EXPORT signed __int16 CC SND_Load_VABS_477040(SoundBlockInfo* /*pSoundBlockInfo*/, int /*reverb*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC SND_Stop_Channels_Mask_4774A0(int /*mask*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC SND_Load_Seqs_477AB0(OpenSeqHandle* /*pSeqTable*/, const char* /*bsqFileName*/)
{
    NOT_IMPLEMENTED();
}

EXPORT signed __int16 CC SND_SEQ_PlaySeq_4775A0(unsigned __int16 /*idx*/, int /*repeatCount*/, __int16 /*bDontStop*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC SND_Seq_Stop_477A60(unsigned __int16 /*idx*/)
{
    NOT_IMPLEMENTED();
}

EXPORT signed __int16 CC SND_SEQ_Play_477760(unsigned __int16 /*idx*/, int /*repeatCount*/, __int16 /*volLeft*/, __int16 /*volRight*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT __int16 CC SND_SsIsEos_DeInlined_477930(unsigned __int16 /*idx*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC SFX_SfxDefinition_Play_477330(const SfxDefinition* /*sfxDef*/, short /*volLeft*/, short /*volRight*/, short /*pitch_min*/, signed __int16 /*pitch_max*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT int CC SFX_SfxDefinition_Play_4770F0(const SfxDefinition* /*sfxDef*/, signed int /*vol*/, int /*pitch_min*/, int /*pitch_max*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

END_NS_AO
