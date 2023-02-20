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
    idBank = new s32[idBankSize];
    for (int i = 0; i < idBankSize; i++)
    {
        idBank[i] = 0;
    }
}

MidiPlayer::MidiPlayer(ResourceProvider* provider, SoundSampleParser* sampleParser)
{
    mResourceProvider = provider;
    mSoundSampleParser = sampleParser;
    idBank = new s32[idBankSize];
    for (int i = 0; i < idBankSize; i++)
    {
        idBank[i] = 0;
    }
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
    reverb; // TODO - reverb - this seems to be the only spot

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
                    tone->mode = vagAttr->field_1_mode;
                    tone->f_Volume = vagAttr->field_2_vol / 127.0f;
                    tone->c_Center = vagAttr->field_4_centre;
                    //tone->c_Shift = (vagAttr->field_5_shift | (vagAttr->field_4_centre << 7));

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

                    f32 sustain_level = static_cast<f32>((2 * (~(u8) vagAttr->field_10_adsr1 & 0xF)));
                    tone->AttackTime = std::min(static_cast<u16>((powf(2.0f, ((vagAttr->field_10_adsr1 >> 8) & 0x7F) * 0.25f) * 0.09f)), static_cast<u16>(32767));
                    tone->DecayTime = static_cast<u16>((((vagAttr->field_10_adsr1 >> 4) & 0xF) / 15.0f) * 16.0);
                    tone->SustainTime = std::min(static_cast<u16>((sustain_level / 15.0f) * 600.0), static_cast<u16>(32767));
                    //tone->ReleaseTime = std::min(static_cast<u16>(pow(2, vagAttr->field_12_adsr2 & 0x1F) * 0.045f), static_cast<u16>(32767));

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
    for (SequencePlayer* player : mSequencePlayers)
    {
        player->StopSequence();
        delete player;
    }
    mSequencePlayers.clear();

    //AliveAudio::LockNotes();
    //AliveAudio::ClearAllVoices();
    //AliveAudio::UnlockNotes();
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
    std::cout << "bitmask " << bitMask << "\n";
    AliveAudio::LockNotes();
    AliveAudio::ClearAllTrackVoices(bitMask, true);
    AliveAudio::UnlockNotes();
    ReleaseId(bitMask);
}

void MidiPlayer::SND_SEQ_Stop(u16 idx)
{
    SequencePlayer* player = GetSequencePlayer(idx);
    if (player)
    {
        player->StopSequence();
        RemoveSequencePlayer(player);
        ReleaseId(player->m_PlayId);
        delete player;
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
    idx;

    SequencePlayer* player = GetSequencePlayer(idx);

    // When chanting starts bDontStop is 1
    // and then 0 is called every frame until chanting stops.
    // I think we can return if it's 0
    if (player && bDontStop == 0)
    {
        return 1; // still playing
    }

    if (!player)
    {
        player = new SequencePlayer();
        mSequencePlayers.push_back(player);
    }
    std::cout << "Play seq " << idx << "\n";

    player->LoadSequenceData(mSequences.at(s16(idx)), s32(idx), repeatCount);
    player->PlaySequence(idx);
    return s16(mSequencePlayers.size() - 1);
}

void MidiPlayer::sanitizeVolume(s32* src, s32 low, s32 high)
{
    if (*src < low)
    {
        *src = low;
    }
    else if (*src >= high)
    {
        *src = high;
    }
}

void MidiPlayer::sanitizePitch(s32* src, s16 defaultPitch)
{
    if (*src == 0x7FFF)
    {
        *src = defaultPitch;
    }

    if (*src == 0x7FFF)
    {
        *src = defaultPitch;
    }
}

void MidiPlayer::SND_SEQ_SetVol(s32 idx, s32 volLeft, s32 volRight)
{
    idx;
    volLeft;
    volRight;
    sanitizeVolume(&volLeft, 10, 127);
    sanitizeVolume(&volRight, 10, 127);
    SequencePlayer* player = GetSequencePlayer(u16(idx));
    if (player)
    {
        player->SetVolume(volLeft, volRight);
    }
}

s16 MidiPlayer::SND_SEQ_Play(u16 idx, s32 repeatCount, s16 volLeft, s16 volRight)
{
    repeatCount; // TODO
    volLeft;
    volRight;

    SequencePlayer* player = GetSequencePlayer(idx);
    if (player)
    {
        return 0;
    }
    std::cout << "Play seq " << idx << "\n";

    player = new SequencePlayer();
    player->LoadSequenceData(mSequences.at(s16(idx)), s32(idx), repeatCount);
    player->PlaySequence(idx);
    mSequencePlayers.push_back(player);
    return s16(mSequencePlayers.size() - 1);
}

s16 MidiPlayer::SND_SsIsEos_DeInlined(u16 idx)
{
    idx; // TODO

    SequencePlayer* player = GetSequencePlayer(idx);
    if (!player)
    {
        return 0;
    }
    if (player->mRepeatCount)
    {
        // 1 means we're still playing
        return player->completedRepeats() < player->mRepeatCount ? 1 : 0;
    }
    return 1;
}

s32 MidiPlayer::SFX_SfxDefinition_Play(SfxDefinition* sfxDef, s32 volLeft, s32 volRight, s32 pitch_min, s32 pitch_max)
{
    sanitizePitch(&pitch_min, sfxDef->pitch_min);
    sanitizePitch(&pitch_max, sfxDef->pitch_max);

    sanitizeVolume(&volLeft, 10, 127);
    sanitizeVolume(&volRight, 10, 127);

    // AliveAudio::PlayOneShot(sfxDef->program, sfxDef->note, volLeft, 0);
    AliveAudio::LockNotes();
    int playId = NextId();
    AliveAudio::PlayOneShot(playId, sfxDef->program, sfxDef->note, volLeft, volRight, 0, pitch_min, pitch_max);
    AliveAudio::UnlockNotes();
    return playId;
}

s32 MidiPlayer::SFX_SfxDefinition_Play(SfxDefinition* sfxDef, s32 volume, s32 pitch_min, s32 pitch_max)
{
    if (!volume)
    {
        volume = sfxDef->volume;
    }

    sanitizePitch(&pitch_min, sfxDef->pitch_min);
    sanitizePitch(&pitch_max, sfxDef->pitch_max);

    sanitizeVolume(&volume, 1, 127);

    //AliveAudio::PlayOneShot(sfxDef->program, sfxDef->note, volume, 0);
    AliveAudio::LockNotes();
    int playId = NextId();
    AliveAudio::PlayOneShot(playId, sfxDef->program, sfxDef->note, volume, volume, 0, pitch_min, pitch_max);
    AliveAudio::UnlockNotes();
    return playId;
}

s32 MidiPlayer::SND(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max)
{
    program; // TODO
    vabId;
    note;
    vol;
    min;
    max;
    AliveAudio::LockNotes();
    int playId = NextId();
    AliveAudio::NoteOn(program, note, char(vol), 0);
    AliveAudio::UnlockNotes();
    return playId;
}

 void MidiPlayer::SsUtAllKeyOff(s32 mode)
 {
     mode; // TODO
 }

 SequencePlayer* MidiPlayer::GetSequencePlayer(u16 idx)
 {
     for (SequencePlayer* player : mSequencePlayers)
     {
         if (player->m_TrackID == idx)
         {
             return player;
         }
     }
     return NULL;
 }

 void MidiPlayer::RemoveSequencePlayer(SequencePlayer* player)
 {
     int offset = 0;
     int found = 0;
     for (SequencePlayer* iter : mSequencePlayers)
     { 
         if (iter->m_TrackID == player->m_TrackID)
         {
             found = 1;
             break;
         }
         offset++;
     }
     if (found)
     {
         mSequencePlayers.erase(mSequencePlayers.begin() + offset);
     }
 }

s32 MidiPlayer::NextId()
{
    for (int i = 0; i < idBankSize; i++)
    {
        if (idBank[i] == 0)
        {
            idBank[i] = 1;
            return i + 255;
        }
    }
    return 55;
}

void MidiPlayer::ReleaseId(s32 id)
{
    idBank[id - 255] = 0;
}

} // namespace psx