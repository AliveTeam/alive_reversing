#pragma once

#include "Soundbank.hpp"
#include "SequencePlayer.hpp"

namespace psx {

    struct VagAtr final
    {
        s8 field_0_priority;
        s8 field_1_mode;
        s8 field_2_vol;
        s8 field_3_pan;
        u8 field_4_centre;
        u8 field_5_shift;
        s8 field_6_min;
        s8 field_7_max;
        s8 field_8_vibW;
        s8 field_9_vibT;
        s8 field_A_porW;
        s8 field_B_porT;
        s8 field_C_pitch_bend_min;
        s8 field_D_pitch_bend_max;
        s8 field_E_reserved1;
        s8 field_F_reserved2;
        s16 field_10_adsr1;
        s16 field_12_adsr2;
        s16 field_14_prog;
        s16 field_16_vag;
        s16 field_18_reserved[4];
    };
    struct AoVag
    {
        u32 iSize;
        u32 iSampleRate;
        std::vector<unsigned char> iSampleData;
    };

    struct ProgAtr final
    {
        u8 field_0_num_tones;
        s8 field_1_vol;
        s8 field_2_priority;
        s8 field_3_mode;
        s8 field_4_pan;
        s8 field_5_reserved0;
        s16 field_6_attr;
        s32 field_8_reserved1;
        s32 field_C_reserved2;
    };

    struct VabHeader final
    {
        s32 field_0_form;
        s32 field_4_version;
        s32 field_8_id;
        s32 field_C_file_size;
        s16 field_10_reserved0;
        s16 field_12_num_progs;
        s16 field_14_num_tones;
        u16 field_16_num_vags;
        s8 field_18_master_vol;
        s8 field_19_master_pan;
        s8 field_1A_attr1;
        s8 field_1B_attr2;
        s32 field_1C_reserved1;
        ProgAtr field_20_progs[128];
    };

    struct SoundBlockInfo final
    {
        char_type* header_name;
        char_type* body_name;
        s32 vab_id;
        u8* VabHeader;
    };

    struct OpenSeqHandle final
    {
        char_type* mBsqName;
        s32 generated_res_id; // A hash of the named which matches the resource Id
        s8 sound_block_idx;
        s8 volume;
        s16 seqOpenId;
        u8* ppSeq_Data;
    };

    struct SfxDefinition final
    {
        s8 block_idx;
        s8 program;
        s8 note;
        s8 volume;
        s16 pitch_min;
        s16 pitch_max;
    };

    struct Vag final
    {
        u16 adsr_attack;
        u16 adsr_sustain_level;
        u16 adsr_decay;
        u16 adsr_release;
        u8 min;
        u8 max;
        s16 shift_cen;
        u8 C;
        u8 vol;
        u8 priority;
        u8 prog;
        u8 vag;
        s8 pad;
    };

    /*
        Represents some data read from disk
    */
    struct ResourceData final
    {
        u8* data;
        u32 size;
        s32 optionalHash;
    };

    /*
        Provides a way to read files from a "psx" disk.
    */
    class ResourceProvider
    {
    public:
        /* Read a file within the game, example a .VB file */
        virtual ResourceData* readFile(char_type* name)
        {
            name;
            throw std::invalid_argument("readFile is not overridden");
        }
        /* AO reads sequence inline with data. AE reads from a sounds.dat file */
        virtual ResourceData* readSeq(const char_type* fileName, const char_type* sequenceName)
        { 
            fileName;
            sequenceName;
            throw std::invalid_argument("readSeq is not overridden");
        }
        /* AO and AE seem to have different hardcoded sequence counts */
        virtual s32 sequenceCount()
        {
            throw std::invalid_argument("sequenceCount is not overridden");
        }
    };

    /*
        In the case of AO and AE sound samples are 
        loaded differently. In AO the samples are inline of
        the disk data file. In the case of AE it uses the disk
        data file but references an offset in a separate sounds.dat file.
        MidiPlayers default is AO style. AE provides its own 
    */
    class SoundSampleParser
    {
    public:
        virtual std::vector<Sample*> parseSamples(VabHeader* vabHeader, u8* ppVabBody)
        {
            vabHeader;
            ppVabBody;
            std::vector<Sample*> empty;
            throw std::invalid_argument("parseSamples is not overridden");
        }
    };

    /*
        The PSX midi player (SND LIB) API
    */
    class MidiPlayer
    {
    public:
        MidiPlayer(ResourceProvider* provider);
        MidiPlayer(ResourceProvider* provider, SoundSampleParser* sampleParser);
        ~MidiPlayer();

        void SND_Init();
        void SND_Shutdown();

        void SND_Load_VABS(SoundBlockInfo* pSoundBlockInfo, s32 reverb);
        void SND_Load_Seqs(OpenSeqHandle* pSeqTable, const char_type* bsqFileName);

        void SND_StopAll();
        void SND_Reset();
        void SND_Restart();

        void SND_Stop_Channels_Mask(u32 bitMask);

        void SND_SEQ_Stop(u16 idx);
        s8 SND_Seq_Table_Valid();
        s16 SND_SEQ_PlaySeq(u16 idx, s32 repeatCount, s16 bDontStop);
        void SND_SEQ_SetVol(s32 idx, s16 volLeft, s16 volRight);
        s16 SND_SEQ_Play(u16 idx, s32 repeatCount, s16 volLeft, s16 volRight);

        s16 SND_SsIsEos_DeInlined(u16 idx);
        s32 SFX_SfxDefinition_Play(SfxDefinition* sfxDef, s32 volLeft, s32 volRight, s32 pitch_min, s32 pitch_max);
        s32 SFX_SfxDefinition_Play(SfxDefinition* sfxDef, s32 volume, s32 pitch_min, s32 pitch_max);
        s32 SND(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max);
        void SsUtAllKeyOff(s32 mode);

    private:
        ResourceProvider* mResourceProvider;
        SoundSampleParser* mSoundSampleParser;

        Soundbank* mSoundbank = NULL;
        std::vector<std::vector<Uint8>> mSequences;
        std::unordered_map<s16, SequencePlayer*> mSequenceMap;
    };

} // namespace psx