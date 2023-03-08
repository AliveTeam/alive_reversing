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

            u8* data = new u8[size];
            memcpy(data, &ppVabBody[pos], size);
            pos += size;

            Sample* sample = new Sample();
            sample->m_SampleBuffer = reinterpret_cast<u16*>(data);
            sample->i_SampleSize = size / 2;
            sample->sampleRate = sampleRate;
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
    if (sequencer)
    {
        delete sequencer;
    }
    sequencer = new sean::Sequencer();
}

void MidiPlayer::SND_Shutdown()
{
    delete sequencer;
}

void MidiPlayer::SND_Load_VABS(SoundBlockInfo* pSoundBlockInfo, s32 reverb)
{
    reverb; // TODO - what do we do with this? override the patch/sample?

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
            
            ///////////
            // PATCH (Instruments)
            for (s32 x = 0; x < 16; x++)
            {

                ///////////
                // SAMPLE
                if (vagAttr->field_2_vol > 0)
                {
                    unsigned short ADSR1 = vagAttr->field_10_adsr1;
                    unsigned short ADSR2 = vagAttr->field_12_adsr2;


                    sean::ADSR adsr = sean::parseADSR(ADSR1, ADSR2);
                    sean::Patch* patch = sequencer->createPatch(vagAttr->field_14_prog);
                    Sample* s = samples.at(vagAttr->field_16_vag - 1);
                    sean::Sample* sample = new sean::Sample(s->m_SampleBuffer, s->i_SampleSize * 2, 22050); // TODO sample-sampleRate?
                    patch->samples[x] = sample;

                    sample->adsr = adsr;
                    sample->volume = vagAttr->field_2_vol / 127.0f;
                    sample->pan = (vagAttr->field_3_pan / 64.0f) - 1.0f;
                    sample->reverb = vagAttr->field_1_mode;
                    sample->rootNote = vagAttr->field_4_centre;
                    sample->rootNotePitchShift = vagAttr->field_5_shift;
                    sample->minNote = vagAttr->field_6_min;
                    sample->maxNote = vagAttr->field_7_max;
                    //sample->loop = 
                }
                ++vagAttr;
            }
        }

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
}

void MidiPlayer::SND_StopAll()
{
    // called when pause menu is open

    std::cout << "stop all" << std::endl;
    sequencer->stopAll();
}

void MidiPlayer::SND_Reset()
{
    // called when quiting to main menu or going into a new level
    std::cout << "reset" << std::endl;
    sequencer->reset();
}

void MidiPlayer::SND_Restart()
{
    std::cout << "restart" << std::endl;
    // TODO - don't know when called
}

void MidiPlayer::SND_Stop_Channels_Mask(u32 bitMask)
{
    bitMask;
    std::cout << "bitmask " << bitMask << "\n";
    sequencer->stopNote(bitMask);
}

void MidiPlayer::SND_SEQ_Stop(u16 idx)
{
    sequencer->stopSeq(idx);
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
    // TODO - still broken for chanting - revist below commented code

    sean::Sequence* seq = sequencer->getSequence(idx);
    if (!seq)
    {
        return 0;
    }
    if (seq->play)
    {
        return 1;
    }
    sequencer->getSequence(idx)->volume = 1.0f;
    sequencer->getSequence(idx)->repeatLimit = repeatCount;
    sequencer->playSeq(idx);
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
    sanitizeVolume(&volLeft, 10, 127);
    sanitizeVolume(&volRight, 10, 127);

    sean::Sequence* seq = sequencer->getSequence((s32) idx);
    if (seq)
    {
        seq->volume = std::min(volLeft, volRight) / 127.0f;
    }
}

s16 MidiPlayer::SND_SEQ_Play(u16 idx, s32 repeatCount, s16 volLeft, s16 volRight)
{
    sean::Sequence* seq = sequencer->getSequence((s32) idx);
    if (seq)
    {
        seq->repeatLimit = repeatCount;
        seq->volume = std::min(volLeft, volRight) / 127.0f;
    }
    sequencer->playSeq(idx);
    return 1;
}

s16 MidiPlayer::SND_SsIsEos_DeInlined(u16 idx)
{
    s16 res = 0;
    sean::Sequence* seq = sequencer->getSequence((s32) idx);
    if (seq)
    {
        res = seq->repeats < seq->repeatLimit ? 1 : 0;
    }
    return res;
}

s32 MidiPlayer::SFX_SfxDefinition_Play(SfxDefinition* sfxDef, s32 volLeft, s32 volRight, s32 pitch_min, s32 pitch_max)
{
    sanitizePitch(&pitch_min, sfxDef->pitch_min);
    sanitizePitch(&pitch_max, sfxDef->pitch_max);

    sanitizeVolume(&volLeft, 10, 127);
    sanitizeVolume(&volRight, 10, 127);
    // TODO - I don't think these pans and volumes are quite right
    float volume = std::max(volLeft, volRight) / 127.0f;
    float pan;
    if (volLeft < volRight)
    {
        pan = 1.0f - (float(volLeft) / float(volRight));
    }
    else
    {
        pan = (float(volRight) / float(volLeft)) - 1.0f;
    }
    return sequencer->playNote(sfxDef->program, sfxDef->note, volume, pan, (u8) std::max(pitch_min, pitch_max), pitch_min, pitch_max);
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

    return sequencer->playNote(sfxDef->program, sfxDef->note, volume / 127.0f, 0, (u8) std::max(pitch_min, pitch_max), pitch_min, pitch_max);
}

s32 MidiPlayer::SND(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max)
{
    vabId; // TODO - why is this not used?
    return sequencer->playNote(program, (u8) note, vol / 127.0f, 0, 0, min, max);
}

 void MidiPlayer::SsUtAllKeyOff(s32 mode)
 {
     mode; // TODO
 }



 //////////////////////////////
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

    unsigned int deltaTime = 0;

    const size_t midiDataStart = stream.Pos();

    // Context state
    SeqInfo gSeqInfo = {};

    for (;;)
    {
        // Read event delta time
        deltaTime += _MidiReadVarLen(stream);
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
                    msg->tick = deltaTime;
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
                    s16 bend = 0;
                    stream.ReadSInt16(bend);

                    // 0 is x semitones down 
                    // 16383 is center
                    // 32767 is x semitones up.
                    // convert to 127 value where 0 is center

                    float multi = bend / 16383.0f;
                    multi = multi * (127 * 4); // (127*4) is 4 semitones (or 4 half steps).
                    multi = multi - (127 * 4); // Possibly 4 is not correct for pitch bend range?

                    sean::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->type = sean::PITCH_BEND;
                    msg->bend = (s16) multi;
                    msg->channelId = channel;
                    msg->tick = deltaTime;
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