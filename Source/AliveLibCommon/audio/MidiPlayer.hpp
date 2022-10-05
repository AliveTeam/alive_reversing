#pragma once

EXPORT void CC SND_Init();
EXPORT void CC SND_Shutdown();

EXPORT void CC SND_Load_VABS(SoundBlockInfo* pSoundBlockInfo, s32 reverb);
EXPORT void CC SND_Load_Seqs(OpenSeqHandle* pSeqTable, const char_type* bsqFileName);

EXPORT void CC SND_StopAll();
EXPORT void SND_Reset();
EXPORT void CC SND_Restart();

EXPORT void CC SND_Stop_Channels_Mask(u32 bitMask);

EXPORT void CC SND_SEQ_Stop(u16 idx);
EXPORT s8 CC SND_Seq_Table_Valid();
EXPORT s16 CC SND_SEQ_PlaySeq(u16 idx, s16 repeatCount, s16 bDontStop);
EXPORT void CC SND_SEQ_SetVol(s32 idx, s16 volLeft, s16 volRight);
EXPORT s16 CC SND_SEQ_Play(u16 idx, s16 repeatCount, s16 volLeft, s16 volRight);

EXPORT s32 CC SND_SsIsEos_DeInlined(u16 idx);
EXPORT s32 CC SFX_SfxDefinition_Play(const SfxDefinition* sfxDef, s16 volLeft, s16 volRight, s16 pitch_min, s16 pitch_max);
EXPORT s32 CC SFX_SfxDefinition_Play(const SfxDefinition* sfxDef, s16 volume, s16 pitch_min, s16 pitch_max);
EXPORT s32 CC SND(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max);

struct Vag final
{
    u16 field_0_adsr_attack;
    u16 field_2_adsr_sustain_level;
    u16 field_4_adsr_decay;
    u16 field_6_adsr_release;
    u8 field_8_min;
    u8 field_9_max;
    s16 field_A_shift_cen;
    u8 field_C;
    u8 field_D_vol;
    u8 field_E_priority;
    u8 field_F_prog;
    u8 field_10_vag;
    s8 field_11_pad;
};

//class Vab
//{
//public:
//}