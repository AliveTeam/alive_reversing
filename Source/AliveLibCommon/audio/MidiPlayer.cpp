#pragma once

#include "MidiPlayer.hpp"
#include "Soundbank.hpp"
#include "mixer/ADSR.hpp"
#include "mixer/AliveAudio.hpp"
#include "SequencePlayer.hpp"
#include "mixer/psxadsr.hpp"

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

u64 timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void MidiPlayer::loop()
{
    // expected ticks = float(now - start) / 1000.0f * 768.0f

    u64 start = timeSinceEpochMillisec();
    u64 now = 0;
    u64 ticks = 0;
    u64 expectedTicks = 0; 
    while (running)
    {
        now = timeSinceEpochMillisec();
        expectedTicks = u64(float(now - start) / 1000.0f * 44100); 

        // tick sequence - i.e. new notes to play/stop?
        sequencer->tickSequence();

        while (ticks++ < expectedTicks)
        {
            // tick voices - just math calculations.
            // this is ticked 44100 times per second. Possibly this
            // can be done with a fast math calculation instead of 
            // a loop, but that's for another time...
            sequencer->tickVoice();
        }

        // sync voices with openal
        sequencer->syncVoice(); 
        
        // defer this thread some amount of time
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}


void MidiPlayer::SND_Init()
{
    mutex.lock();
    if (sequencer)
    {
        delete sequencer;
    }
    sequencer = new sean::Sequencer();
    mutex.unlock();

    running = true;
    thread = new std::thread(&MidiPlayer::loop, this);

    //// Create sequencer
    //AliveInitAudio();
}

void MidiPlayer::SND_Shutdown()
{
    running = false;
    thread->join();
    delete thread;

    delete sequencer;
}

void MidiPlayer::SND_Load_VABS(SoundBlockInfo* pSoundBlockInfo, s32 reverb)
{
    mutex.lock();
    reverb; // TODO - reverb - this seems to be the only spot

    sequencer->reset();
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
            // PATCH (Instruments)
            for (s32 x = 0; x < 16; x++)
            {
                // SAMPLE
                if (vagAttr->field_2_vol > 0)
                {
                    unsigned short ADSR1 = vagAttr->field_10_adsr1;
                    unsigned short ADSR2 = vagAttr->field_12_adsr2;
                    REAL_ADSR realADSR = {};
                    PSXConvADSR(&realADSR, ADSR1, ADSR2, false);

                    sean::ADSR adsr = sean::parseADSR(ADSR1, ADSR2);
                    //sean::REAL_ADSR adsr;
                    //sean::PSXConvADSR(&adsr, ADSR1, ADSR2, false);
                    

                    sean::Patch* patch = sequencer->createPatch(vagAttr->field_14_prog);
                    Sample* s = samples.at(vagAttr->field_16_vag - 1);
                    sean::Sample* t = new sean::Sample(s->m_SampleBuffer, s->i_SampleSize * 2);
                    patch->samples[x] = t;

                    u8 a = (u8) (vagAttr->field_10_adsr1 >> 8);
                    u8 d = (u8) vagAttr->field_10_adsr1;
                    a;
                    d;

                    t->adsr = adsr;
                    t->volume = vagAttr->field_2_vol / 127.0f;
                    t->pan = (vagAttr->field_3_pan / 64.0f) - 1.0f;
                    t->reverb = vagAttr->field_1_mode;
                    t->rootNote = vagAttr->field_4_centre;
                    t->rootNotePitchShift = vagAttr->field_5_shift;
                    t->minNote = vagAttr->field_6_min;
                    t->maxNote = vagAttr->field_7_max;


                    //program->prog_id = vagAttr->field_14_prog;
                    //tone->mode = vagAttr->field_1_mode;
                    //tone->f_Volume = vagAttr->field_2_vol / 127.0f;
                    //tone->c_Center = vagAttr->field_4_centre;
                    ////tone->c_Shift = (vagAttr->field_5_shift | (vagAttr->field_4_centre << 7));
                    //tone->c_Shift = vagAttr->field_5_shift;
                    //tone->f_Pan = (vagAttr->field_3_pan / 64.0f) - 1.0f;
                    //tone->Min = vagAttr->field_6_min;
                    //tone->Max = vagAttr->field_7_max;
                    //tone->Pitch = vagAttr->field_5_shift / 100.0f;
                    //tone->m_Sample = s;
                    //tone->sample = sampleBuffers.at(vagAttr->field_16_vag - 1);
                    ////alGenBuffers(1, &tone->sample);
                    ////alBufferData(tone->sample, AL_FORMAT_MONO16, tone->m_Sample->m_SampleBuffer, tone->m_Sample->i_SampleSize * 2, 22050);
                    //// These are time in seconds
                    //tone->AttackTime = realADSR.attack_time;
                    //tone->DecayTime = realADSR.decay_time;
                    //tone->ReleaseTime = realADSR.release_time;
                    //tone->SustainTime = realADSR.sustain_time;
                    //tone->SustainLevel = realADSR.sustain_level;
                    //tone->ReleaseExponential = false; // TODO - where does this come from?
                    ////f32 sustain_level = static_cast<f32>((2 * (~(u8) vagAttr->field_10_adsr1 & 0xF)));
                    ////tone->AttackTime = std::min(static_cast<u16>((powf(2.0f, ((vagAttr->field_10_adsr1 >> 8) & 0x7F) * 0.25f) * 0.09f)), static_cast<u16>(32767));
                    ////tone->DecayTime = static_cast<u16>((((vagAttr->field_10_adsr1 >> 4) & 0xF) / 15.0f) * 16.0);
                    ////tone->SustainTime = std::min(static_cast<u16>((sustain_level / 15.0f) * 600.0), static_cast<u16>(32767));
                    ////tone->ReleaseTime = std::min(static_cast<u16>(pow(2, vagAttr->field_12_adsr2 & 0x1F) * 0.045f), static_cast<u16>(32767));
                    //if (realADSR.attack_time > 1)
                    //{ // This works until the loop database is added.
                    //    tone->Loop = true;
                    //}
                }
                ++vagAttr;
            }
        }

        //AliveAudio::LockNotes();
        //delete mSoundbank;
        //mSoundbank = new Soundbank(samples, programs);
        //AliveAudio::ClearAllVoices();
        //AliveAudio::m_CurrentSoundbank = mSoundbank;
        //AliveAudio::UnlockNotes();

        delete[] ppVabBody;
        pSoundBlockInfo++;
    }
    mutex.unlock();
}

void MidiPlayer::SND_Load_Seqs(OpenSeqHandle* pSeqTable, const char_type* bsqFileName)
{
    if (!pSeqTable || !bsqFileName)
    {
        return;
    }

    mutex.lock();
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
            sean::Sequence* sequence = sequencer->createSequence();
            parseMidiStream(sequence, vec, i);
            mSequences.push_back(vec);
        }
        else
        {
            seq[i].ppSeq_Data = nullptr;
            std::vector<Uint8> test;
            mSequences.push_back(test);
        }
    }

    mutex.unlock();
}

void MidiPlayer::SND_StopAll()
{
    //for (SequencePlayer* player : mSequencePlayers)
    //{
    //    player->StopSequence();
    //    delete player;
    //}
    //mSequencePlayers.clear();

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
    //AliveAudio::LockNotes();
    //AliveAudio::ClearAllTrackVoices(bitMask, true);
    //AliveAudio::UnlockNotes();
    //ReleaseId(bitMask);
}

void MidiPlayer::SND_SEQ_Stop(u16 idx)
{
    idx;

    mutex.lock();
    sequencer->stopSeq(idx);
    mutex.unlock();
    //SequencePlayer* player = GetSequencePlayer(idx);
    //if (player)
    //{
    //    player->StopSequence();
    //    RemoveSequencePlayer(player);
    //    ReleaseId(player->m_PlayId);
    //    delete player;
    //}
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

    mutex.lock();
    sequencer->playSeq(idx);
    mutex.unlock();
    return 1;
    //SequencePlayer* player = GetSequencePlayer(idx);

    //// When chanting starts bDontStop is 1
    //// and then 0 is called every frame until chanting stops.
    //// I think we can return if it's 0
    //if (player && bDontStop == 0)
    //{
    //    return 1; // still playing
    //}

    //if (!player)
    //{
    //    player = new SequencePlayer();
    //    mSequencePlayers.push_back(player);
    //}
    //std::cout << "Play seq " << idx << "\n";

    //player->LoadSequenceData(mSequences.at(s16(idx)), s32(idx), repeatCount);
    //player->PlaySequence(idx);
    //return s16(mSequencePlayers.size() - 1);
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

    mutex.lock();
    sean::Sequence* seq = sequencer->getSequence((s32) idx);
    if (seq)
    {
        seq->volume = std::min(volLeft, volRight) / 127.0f;
    }
    mutex.unlock();

    //SequencePlayer* player = GetSequencePlayer(u16(idx));
    //if (player)
    //{
    //    player->SetVolume(volLeft, volRight);
    //}
}

s16 MidiPlayer::SND_SEQ_Play(u16 idx, s32 repeatCount, s16 volLeft, s16 volRight)
{
    repeatCount; // TODO
    volLeft;
    volRight;
    idx;

    mutex.lock();
    sean::Sequence* seq = sequencer->getSequence((s32) idx);
    if (seq)
    {
        seq->volume = std::min(volLeft, volRight) / 127.0f;
    }
    sequencer->playSeq(idx);
    mutex.unlock();
    return 0;

    //SequencePlayer* player = GetSequencePlayer(idx);
    //if (player)
    //{
    //    return 0;
    //}
    //std::cout << "Play seq " << idx << "\n";

    //player = new SequencePlayer();
    //player->LoadSequenceData(mSequences.at(s16(idx)), s32(idx), repeatCount);
    //player->PlaySequence(idx);
    //player->SetVolume(volLeft, volRight);
    //mSequencePlayers.push_back(player);
    //return s16(mSequencePlayers.size() - 1);
}

s16 MidiPlayer::SND_SsIsEos_DeInlined(u16 idx)
{
    idx; // TODO

    //SequencePlayer* player = GetSequencePlayer(idx);
    //if (!player)
    //{
    //    return 0;
    //}
    //if (player->mRepeatCount)
    //{
    //    // 1 means we're still playing
    //    return player->completedRepeats() < player->mRepeatCount ? 1 : 0;
    //}

    s16 res = 0;
    mutex.lock();
    sean::Sequence* seq = sequencer->getSequence((s32) idx);
    if (seq)
    {
        res = seq->repeats < seq->repeatLimit ? 1 : 0;
    }
    mutex.unlock();

    return res;
}

s32 MidiPlayer::SFX_SfxDefinition_Play(SfxDefinition* sfxDef, s32 volLeft, s32 volRight, s32 pitch_min, s32 pitch_max)
{
    sanitizePitch(&pitch_min, sfxDef->pitch_min);
    sanitizePitch(&pitch_max, sfxDef->pitch_max);

    sanitizeVolume(&volLeft, 10, 127);
    sanitizeVolume(&volRight, 10, 127);
    std::cout << volLeft << " " << volRight << std::endl;
    mutex.lock(); 
    // TODO - I don't think these pans and volumes are quite right
    float volume = std::max(volLeft, volRight) / 127.0f;
    float pan = (float(volRight) / float(volLeft)) - 1;
    s32 id = sequencer->playNote(sfxDef->program, sfxDef->note, volume, pan, (u8)std::max(pitch_min, pitch_max), pitch_min, pitch_max);
    mutex.unlock();
    return id;
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

    mutex.lock();
    s32 id = sequencer->playNote(sfxDef->program, sfxDef->note, volume / 127.0f, 0, (u8) std::max(pitch_min, pitch_max), pitch_min, pitch_max);
    mutex.unlock();
    return id;
}

s32 MidiPlayer::SND(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max)
{
    program; // TODO
    vabId;
    note;
    vol;
    min;
    max;
    return 0;
    //AliveAudio::LockNotes();
    //int playId = NextId();
    //AliveAudio::NoteOn(program, note, char(vol), 0);
    //AliveAudio::UnlockNotes();
    //return playId;
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



// Midi stuff
static void _SndMidiSkipLength(Stream& stream, int skip)
{
    stream.Seek(stream.Pos() + skip);
}

// Midi stuff
static Uint32 _MidiReadVarLen(Stream& stream)
{
    Uint32 ret = 0;
    Uint8 byte = 0;
    for (int i = 0; i < 4; ++i)
    {
        stream.ReadUInt8(byte);
        ret = (ret << 7) | (byte & 0x7f);
        if (!(byte & 0x80))
        {
            break;
        }
    }
    return ret;
}

void parseMidiStream(sean::Sequence* seq, std::vector<Uint8> seqData, s32 trackId)
{
    Stream stream(std::move(seqData));
    seq->id = trackId;

    SeqHeader seqHeader;

    // Read the header
    if (stream.Size() == 0)
    {
        std::cout << "no stream!?\n";
        return;
    }

    stream.ReadUInt32(seqHeader.mMagic);
    stream.ReadUInt32(seqHeader.mVersion);
    stream.ReadBytes(seqHeader.mResolutionOfQuaterNote, sizeof(seqHeader.mResolutionOfQuaterNote));
    stream.ReadBytes(seqHeader.mTempo, sizeof(seqHeader.mTempo));
    stream.ReadUInt8(seqHeader.mTimeSignatureBars);
    stream.ReadUInt8(seqHeader.mTimeSignatureBeats);

    int tempoValue = 0;
    for (int i = 0; i < 3; i++)
    {
        tempoValue += seqHeader.mTempo[2 - i] << (8 * i);
    }

    int ticksPerBeat = 0;
    for (int i = 0; i < 2; i++)
    {
        ticksPerBeat += seqHeader.mResolutionOfQuaterNote[1 - i] << (8 * i);
    }

    //m_TimeSignatureBars = seqHeader.mTimeSignatureBars;
    seq->tempoUs = (float) tempoValue; // tempo (length of quarter note in microseconds) 0.000001us/s  vs 0.001ms/s
    seq->ticksPerBeat = float(ticksPerBeat);

    unsigned int prevDeltaTime = 0;
    unsigned int deltaTime = 0;

    const size_t midiDataStart = stream.Pos();

    // Context state
    SeqInfo gSeqInfo = {};

    for (;;)
    {
        // Read event delta time
        Uint32 delta = _MidiReadVarLen(stream);
        deltaTime += delta;
        //std::cout << "Delta: " << delta << " over all " << deltaTime << std::endl;

        // Obtain the event/status byte
        Uint8 eventByte = 0;
        stream.ReadUInt8(eventByte);
        if (eventByte < 0x80)
        {
            // Use running status
            if (!gSeqInfo.running_status) // v1
            {
                return; // error if no running status?
            }
            eventByte = gSeqInfo.running_status;

            // Go back one byte as the status byte isn't a status
            stream.Seek(stream.Pos() - 1);
        }
        else
        {
            // Update running status
            gSeqInfo.running_status = eventByte;
        }

        if (eventByte == 0xff)
        {
            // Meta event
            Uint8 metaCommand = 0;
            stream.ReadUInt8(metaCommand);

            Uint8 metaCommandLength = 0;
            stream.ReadUInt8(metaCommandLength);

            switch (metaCommand)
            {
                case 0x2f:
                {
                    sean::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->type = sean::END_TRACK;
                    msg->tick = ticksPerBeat;
                    return;
                }

                case 0x51: // Tempo in microseconds per quarter note (24-bit value)
                {
                    //std::cout << "Temp change" << std::endl;
                    // TODO: Not sure if this is correct
                    Uint8 tempoByte = 0;
                    int t = 0;
                    for (int i = 0; i < 3; i++)
                    {
                        stream.ReadUInt8(tempoByte);
                        t = tempoByte << 8 * i;
                    }
                }
                break;

                default:
                {
                    //std::cout << "Unknown meta event " << Uint32(metaCommand) << std::endl;
                    // Skip unknown events
                    // TODO Might be wrong
                    _SndMidiSkipLength(stream, metaCommandLength);
                }
            }
        }
        else if (eventByte < 0x80)
        {
            // Error
            throw std::runtime_error("Unknown midi event");
        }
        else
        {
            const Uint8 channel = eventByte & 0xf;
            switch (eventByte >> 4)
            {
                case 0x9: // Note On
                {
                    Uint8 note = 0;
                    stream.ReadUInt8(note);

                    Uint8 velocity = 0;
                    stream.ReadUInt8(velocity);

                    sean::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->tick = deltaTime;
                    msg->channelId = channel;
                    msg->note = note;
                    msg->velocity = velocity;
                    if (velocity == 0) // If velocity is 0, then the sequence means to do "Note Off"
                    {
                        msg->type = sean::NOTE_OFF;
                    }
                    else
                    {
                        prevDeltaTime = deltaTime;
                        msg->type = sean::NOTE_ON;
                    }
                }
                break;
                case 0x8: // Note Off
                {
                    Uint8 note = 0;
                    stream.ReadUInt8(note);
                    Uint8 velocity = 0;
                    stream.ReadUInt8(velocity);

                    sean::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->type = sean::NOTE_OFF;
                    msg->tick = deltaTime;
                    msg->channelId = channel;
                    msg->note = note;
                    msg->velocity = velocity;
                }
                break;
                case 0xc: // Program Change
                {
                    Uint8 prog = 0;
                    stream.ReadUInt8(prog);
                    //prevDeltaTime = deltaTime;

                    sean::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->type = sean::PATCH_CHANGE;
                    msg->tick = deltaTime;
                    msg->channelId = channel;
                    msg->patchId = prog;
                }
                break;
                case 0xa: // Polyphonic key pressure (after touch)
                {
                    Uint8 note = 0;
                    Uint8 pressure = 0;

                    stream.ReadUInt8(note);
                    stream.ReadUInt8(pressure);
                }
                break;
                case 0xb: // Controller Change
                {
                    Uint8 controller = 0;
                    Uint8 value = 0;
                    stream.ReadUInt8(controller);
                    stream.ReadUInt8(value);
                }
                break;
                case 0xd: // After touch
                {
                    Uint8 value = 0;
                    stream.ReadUInt8(value);
                }
                break;
                case 0xe: // Pitch Bend
                {
                    Uint16 bend = 0;
                    stream.ReadUInt16(bend);
                }
                break;
                case 0xf: // Sysex len
                {
                    const Uint32 length = _MidiReadVarLen(stream);
                    _SndMidiSkipLength(stream, length);
                }
                break;
                default:
                    throw std::runtime_error("Unknown MIDI command");
            }
        }
    }
}

} // namespace psx