#pragma once

#include "MidiPlayer.hpp"
#include "Soundbank.hpp"
#include "mixer/ADSR.hpp"
#include "mixer/AliveAudio.hpp"
#include "SequencePlayer.hpp"

namespace psx {

/*
* Used for AO, but the assumption is this is the default psx implementation.
* AE uses a custom version reading from an external sounds.dat file
*/
class DefaultSoundSampleParser : public SoundSampleParser
{
public:
    std::vector<Sample*> parseSamples(VabHeader* vabHeader, u8* ppVabBody)
    {
        std::vector<Sample*> samples;
        int pos = 0;
        for (int i = 0; i < vabHeader->field_16_num_vags; ++i)
        {
            // SAMPLE
            u32 size = *reinterpret_cast<u32*>(&ppVabBody[pos]);
            pos += sizeof(u32);

            u32 sampleRate = *reinterpret_cast<u32*>(&ppVabBody[pos]);
            pos += sizeof(u32);
            sampleRate;

            u8* data = new u8[size];
            memcpy(data, &ppVabBody[pos], size);
            pos += size;

            Sample* sample = new Sample();
            sample->m_SampleBuffer = reinterpret_cast<u16*>(data);
            sample->i_SampleSize = size / 2;
            samples.push_back(sample);
        }

        return samples;
    }
};

MidiPlayer::MidiPlayer(ResourceProvider* provider)
{
    mResourceProvider = provider;
    mSoundSampleParser = new DefaultSoundSampleParser();
}

MidiPlayer::MidiPlayer(ResourceProvider* provider, SoundSampleParser* sampleParser)
{
    mResourceProvider = provider;
    mSoundSampleParser = sampleParser;
}


void MidiPlayer::SND_Init()
{
    AliveInitAudio();
}

void MidiPlayer::SND_Shutdown()
{
}

void MidiPlayer::SND_Load_VABS(SoundBlockInfo* pSoundBlockInfo, s32 reverb)
{
    reverb; // TODO - reverb

    while (1)
    {
        if (!pSoundBlockInfo->header_name)
        {
            break;
        }

        // Read header
        ResourceData* vabHeaderResource = mResourceProvider->readFile(pSoundBlockInfo->header_name);
        VabHeader* vabHeader = reinterpret_cast<VabHeader*>(vabHeaderResource->data);

        // Read body
        ResourceData* vabBodyResource = mResourceProvider->readFile(pSoundBlockInfo->body_name);
        u8* ppVabBody = vabBodyResource->data;

        ///////////
        // BODY
        std::vector<Sample*> samples = mSoundSampleParser->parseSamples(vabHeader, ppVabBody);

        ///////////
        // HEADER
        VagAtr* vagAttr = (VagAtr*) &vabHeader[1];
        std::vector<Program*> programs;

        for (s32 i = 0; i < vabHeader->field_12_num_progs; i++)
        {
            // PROGRAM
            Program* program = new Program;
            programs.push_back(program);
            for (s32 toneCounter = 0; toneCounter < 16; toneCounter++)
            {
                // TONE

                Tone* tone = new Tone();
                program->m_Tones.push_back(tone);

                if (vagAttr->field_2_vol > 0)
                {
                    program->prog_id = vagAttr->field_14_prog;
                    tone->f_Volume = vagAttr->field_2_vol / 127.0f;
                    tone->c_Center = vagAttr->field_4_centre;
                    tone->c_Shift = vagAttr->field_5_shift;
                    tone->f_Pan = (vagAttr->field_3_pan / 64.0f) - 1.0f;
                    tone->Min = vagAttr->field_6_min;
                    tone->Max = vagAttr->field_7_max;
                    tone->Pitch = vagAttr->field_5_shift / 100.0f;
                    tone->m_Sample = samples.at(vagAttr->field_16_vag - 1);

                    unsigned short ADSR1 = vagAttr->field_10_adsr1;
                    unsigned short ADSR2 = vagAttr->field_12_adsr2;

                    REAL_ADSR realADSR = {};
                    PSXConvADSR(&realADSR, ADSR1, ADSR2, false);

                    tone->AttackTime = realADSR.attack_time;
                    tone->DecayTime = realADSR.decay_time;
                    tone->ReleaseTime = realADSR.release_time;
                    tone->SustainTime = realADSR.sustain_time;

                    if (realADSR.attack_time > 1)
                    { // This works until the loop database is added.
                        tone->Loop = true;
                    }
                }
                ++vagAttr;
            }
        }

        AliveAudio::LockNotes();
        delete mSoundbank;
        mSoundbank = new Soundbank(samples, programs);
        AliveAudio::ClearAllVoices();
        AliveAudio::m_CurrentSoundbank = mSoundbank;
        AliveAudio::UnlockNotes();

        delete[] ppVabBody;
        pSoundBlockInfo++;
    }
}

void MidiPlayer::SND_Load_Seqs(OpenSeqHandle* pSeqTable, const char_type* bsqFileName)
{
    if (!pSeqTable || !bsqFileName)
    {
        return;
    }

    mSequences.clear();
    OpenSeqHandle* seq = pSeqTable;
    for (s32 i = 0; i < mResourceProvider->sequenceCount(); i++) // AO = 164   AE = 144
    {
        ResourceData* resource = mResourceProvider->readSeq(bsqFileName, seq[i].mBsqName);

        if (resource->data)
        {
            // 32 length
            seq[i].ppSeq_Data = resource->data;
            seq[i].generated_res_id = resource->optionalHash;

            std::vector<Uint8> vec;
            for (u32 x = 0; x < resource->size; x++)
            {
                vec.push_back((Uint8) resource->data[x]);
            }

            // SEQUENCE STREAM
            mSequences.push_back(vec);
        }
        else
        {
            seq[i].ppSeq_Data = nullptr;
            std::vector<Uint8> test;
            mSequences.push_back(test);
        }
    }
}

void MidiPlayer::SND_StopAll()
{
}

void MidiPlayer::SND_Reset()
{
}

void MidiPlayer::SND_Restart()
{
}

void MidiPlayer::SND_Stop_Channels_Mask(u32 bitMask)
{
    bitMask;
}

void MidiPlayer::SND_SEQ_Stop(u16 idx)
{
    if (mSequenceMap[idx])
    {
        mSequenceMap[idx]->StopSequence();
        delete mSequenceMap[idx];
        mSequenceMap[idx] = NULL;
    }
}

s8 MidiPlayer::SND_Seq_Table_Valid()
{
    return 1;
}

s16 MidiPlayer::SND_SEQ_PlaySeq(u16 idx, s32 repeatCount, s16 bDontStop)
{
    bDontStop; // TODO
    repeatCount;

    if (mSequenceMap[idx])
    {
        return 1;
    }

    mSequenceMap[idx] = new SequencePlayer();
    mSequenceMap[idx]->LoadSequenceData(mSequences.at(s16(idx)));
    mSequenceMap[idx]->PlaySequence();

    return 1;
}

void MidiPlayer::SND_SEQ_SetVol(s32 idx, s16 volLeft, s16 volRight)
{
    idx;
    volLeft;
    volRight;
}

s16 MidiPlayer::SND_SEQ_Play(u16 idx, s32 repeatCount, s16 volLeft, s16 volRight)
{
    repeatCount; // TODO
    volLeft;
    volRight;

    if (mSequenceMap[idx])
    {
        return 1;
    }

    mSequenceMap[idx] = new SequencePlayer();
    mSequenceMap[idx]->LoadSequenceData(mSequences.at(s16(idx)));
    mSequenceMap[idx]->PlaySequence();
    return 1;
}

s16 MidiPlayer::SND_SsIsEos_DeInlined(u16 idx)
{
    idx; // TODO
    return 1;
}

s32 MidiPlayer::SFX_SfxDefinition_Play(SfxDefinition* sfxDef, s32 volLeft, s32 volRight, s32 pitch_min, s32 pitch_max)
{
    volLeft; // TODO
    volRight;
    pitch_min;
    pitch_max;
    AliveAudio::PlayOneShot(sfxDef->program, sfxDef->note, volLeft, 0);
    return 1;
}

s32 MidiPlayer::SFX_SfxDefinition_Play(SfxDefinition* sfxDef, s32 volume, s32 pitch_min, s32 pitch_max)
{
    pitch_min; // TODO
    pitch_max;
    AliveAudio::PlayOneShot(sfxDef->program, sfxDef->note, volume, 0);
    return 1;
}

s32 MidiPlayer::SND(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max)
{
    program; // TODO
    vabId;
    note;
    vol;
    min;
    max;
    //AliveAudio::PlayOneShot(program, note, vol, 0);
    return 1;
}

 void MidiPlayer::SsUtAllKeyOff(s32 mode)
 {
     mode; // TODO
 }

} // namespace psx