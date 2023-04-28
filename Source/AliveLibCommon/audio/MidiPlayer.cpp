#pragma once

#include "MidiPlayer.hpp"
#include "oddio.h"

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
            sample->m_SampleBuffer = reinterpret_cast<s16*>(data);
            sample->i_SampleSize = size / 2;
            sample->sampleRate = 8000;         // All PC samples expect 8000hz regardless of actual rate
            sample->loop = sampleRate > 44100; // non-standard?
            samples.push_back(sample);
        }
        return samples;
    }

    void applyFix(char_type* headerName, s32 vag, s32 vagOffset, SPU::Sample* sample)
    {
        std::vector<std::vector<u32>> lookupVag;
        std::vector<std::vector<s32>> lookupRate;

        if (strcmp(headerName, "D1SNDFX.VH") == 0)
        {
            lookupVag = ODDIO::AO_D1SNDFX_VH;
            lookupRate = ODDIO::AO_D1SNDFX_RATE;
        }
        else if (strcmp(headerName, "D2SNDFX.VH") == 0)
        {
            lookupVag = ODDIO::AO_D2SNDFX_VH;
            lookupRate = ODDIO::AO_D2SNDFX_RATE;
        }
        else if (strcmp(headerName, "D2ENDER.VH") == 0)
        {
            lookupVag = ODDIO::AO_D2ENDER_VH;
            lookupRate = ODDIO::AO_D2ENDER_RATE;
        }
        else if (strcmp(headerName, "E1SNDFX.VH") == 0)
        {
            lookupVag = ODDIO::AO_E1SNDFX_VH;
            lookupRate = ODDIO::AO_E1SNDFX_RATE;
        }
        else if (strcmp(headerName, "E2SNDFX.VH") == 0)
        {
            lookupVag = ODDIO::AO_E2SNDFX_VH;
            lookupRate = ODDIO::AO_E2SNDFX_RATE;
        }
        else if (strcmp(headerName, "F1SNDFX.VH") == 0)
        {
            lookupVag = ODDIO::AO_F1SNDFX_VH;
            lookupRate = ODDIO::AO_F1SNDFX_RATE;
        }
        else if (strcmp(headerName, "F2SNDFX.VH") == 0)
        {
            lookupVag = ODDIO::AO_F2SNDFX_VH;
            lookupRate = ODDIO::AO_F2SNDFX_RATE;
        }
        else if (strcmp(headerName, "F2ENDER.VH") == 0)
        {
            lookupVag = ODDIO::AO_F2ENDER_VH;
            lookupRate = ODDIO::AO_F2ENDER_RATE;
        }
        else if (strcmp(headerName, "MLSNDFX.VH") == 0)
        {
            lookupVag = ODDIO::AO_MLSNDFX_VH;
            lookupRate = ODDIO::AO_MLSNDFX_RATE;
        }
        else if (strcmp(headerName, "OPTSNDFX.VH") == 0)
        {
            lookupVag = ODDIO::AO_OPTSNDFX_VH;
            lookupRate = ODDIO::AO_OPTSNDFX_RATE;
        }
        else if (strcmp(headerName, "RFSNDFX.VH") == 0)
        {
            lookupVag = ODDIO::AO_RFSNDFX_VH;
            lookupRate = ODDIO::AO_RFSNDFX_RATE;
        }
        else if (strcmp(headerName, "RFENDER.VH") == 0)
        {
            lookupVag = ODDIO::AO_RFENDER_VH;
            lookupRate = ODDIO::AO_RFENDER_RATE;
        }

        for (auto entry : lookupVag)
        {
            if ((s32) entry[1] == vagOffset)
            {
                VagAtr* fixed = (VagAtr*) &entry[2];
                sample->adsr = SPU::parseADSR(fixed->field_10_adsr1, fixed->field_12_adsr2);
                sample->volume = fixed->field_2_vol == 0 ? 127 : fixed->field_2_vol;
                sample->pan = fixed->field_3_pan;
                sample->reverb = fixed->field_1_mode;
                sample->rootNote = fixed->field_4_centre;
                sample->rootNotePitchShift = fixed->field_5_shift;
                sample->minNote = fixed->field_6_min;
                sample->maxNote = fixed->field_7_max;
                sample->priority = fixed->field_0_priority;
                break;
            }
        }
       
        for (int i = 0; i < (int) lookupRate.size(); i++)
        {
            if ((s32) lookupRate[i][0] == vag)
            {
                sample->SampleRate = lookupRate[i][1];
                break;
            }
        }
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
    SPU::Init();
}

void MidiPlayer::SND_Shutdown()
{
    SPU::DeInit();
}

void MidiPlayer::SND_Load_VABS(SoundBlockInfo* pSoundBlockInfo, s32 reverb)
{
    reverb; // TODO - what do we do with this? override the patch/sample?

    SPU::Reset();

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
        s32 vagOffset = 0;
        for (s32 pIdx = 0; pIdx < vabHeader->field_12_num_progs; pIdx++)
        {
            
            SPU::Patch* patch = new SPU::Patch((u8) vagAttr->field_14_prog);
            SPU::PatchAdd(patch);

            ///////////
            // PATCH (Instruments)
            for (s32 vagIdx = 0; vagIdx < 16; vagIdx++)
            {
                ///////////
                // SAMPLE
                if (vagAttr->field_2_vol > 0 && vagAttr->field_16_vag > 0)
                {
                    unsigned short ADSR1 = vagAttr->field_10_adsr1;
                    unsigned short ADSR2 = vagAttr->field_12_adsr2;
                    Sample* s = samples.at(vagAttr->field_16_vag - 1);

                    SPU::ADSR adsr = SPU::parseADSR(ADSR1, ADSR2);
                    SPU::Sample* sample = new SPU::Sample(s->m_SampleBuffer, s->i_SampleSize, s->sampleRate);
                    
                    patch->samples[vagIdx] = sample;

                    sample->adsr = adsr;
                    sample->volume = vagAttr->field_2_vol == 0 ? 127 : vagAttr->field_2_vol;
                    sample->pan = vagAttr->field_3_pan;
                    sample->reverb = vagAttr->field_1_mode;
                    sample->rootNote = vagAttr->field_4_centre;
                    sample->rootNotePitchShift = vagAttr->field_5_shift;
                    sample->minNote = vagAttr->field_6_min;
                    sample->maxNote = vagAttr->field_7_max;
                    sample->priority = vagAttr->field_0_priority;
                    sample->loop = s->loop;

                    mSoundSampleParser->applyFix(pSoundBlockInfo->header_name, vagAttr->field_16_vag, vagOffset, sample);
                }
                vagOffset++;
                vagAttr++;
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
            SPU::Sequence* sequence = new SPU::Sequence();
            parseMidiStream(sequence, vec, i);
            SPU::SeqAdd(sequence);
        }
    }
}

void MidiPlayer::SND_StopAll()
{
    // called when pause menu is open
    SPU::StopAll();
}

void MidiPlayer::SND_Reset()
{
    // called when quiting to main menu or going into a new level
    SPU::Reset();
}

void MidiPlayer::SND_Restart()
{
    // TODO - don't know when called
    std::cout << "restart" << std::endl;
}

void MidiPlayer::SND_Stop_Channels_Mask(u32 mask)
{
    SPU::OneShotStop(mask);
}

void MidiPlayer::SND_SEQ_Stop(u16 idx)
{
    SPU::SeqStop(idx);
}

s8 MidiPlayer::SND_Seq_Table_Valid()
{
    return 1;
}

s16 MidiPlayer::SND_SEQ_PlaySeq(u16 idx, s32 repeatCount, s16 stopDuplicateSeq)
{
    // Exmaples
    // 1. Continuosly called while chanting
    // 2. called twice when you blow up two bombs quickly (beginning of Rupture Farms)
    //    The sequence is played twice for both bombs and must not stop duplicate sequence
    return SPU::SeqPlay(idx, repeatCount, stopDuplicateSeq) ? 1 : 0;
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
    sanitizeVolume(&volLeft, 0, 127);
    sanitizeVolume(&volRight, 0, 127);
    SPU::SeqSetVolume(idx, (s16) volLeft, (s16) volRight);
}

s16 MidiPlayer::SND_SEQ_Play(u16 idx, s32 repeatCount, s16 volLeft, s16 volRight)
{
    return SPU::SeqPlay(idx, repeatCount, volLeft, volRight) ? 1 : 0;
}

s16 MidiPlayer::SND_SsIsEos_DeInlined(u16 idx)
{
    return SPU::SeqIsDone(idx) ? 0 : 1;
}

s32 MidiPlayer::SFX_SfxDefinition_Play(SfxDefinition* sfxDef, s32 volLeft, s32 volRight, s32 pitch_min, s32 pitch_max)
{
    sanitizePitch(&pitch_min, sfxDef->pitch_min);
    sanitizePitch(&pitch_max, sfxDef->pitch_max);

    sanitizeVolume(&volLeft, 0, 127);
    sanitizeVolume(&volRight, 0, 127);
   
    return SPU::OneShotPlay(sfxDef->program, sfxDef->note, (s16) volLeft, (s16) volRight, (u8) std::max(pitch_min, pitch_max), pitch_min, pitch_max);
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

    return SPU::OneShotPlay(sfxDef->program, sfxDef->note, (s16) volume, (s16) volume, (u8) std::max(pitch_min, pitch_max), pitch_min, pitch_max);
}

s32 MidiPlayer::SND(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max)
{
    vabId; // TODO - why is this not used?
    return SPU::OneShotPlay(program, (u8) note, vol, vol, 0, min, max);
}

 void MidiPlayer::SsUtAllKeyOff(s32 mode)
 {
     // TODO - don't know when this is called
     mode; 
     // SPU::StopAll();
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

// https://github.com/mlgthatsme/AliveSoundLib/blob/master/MidiPlayer/src/SequencePlayer.cpp
// https://github.com/vgmtrans/vgmtrans/blob/master/src/main/formats/PS1Seq.cpp
void parseMidiStream(SPU::Sequence* seq, std::vector<Uint8> seqData, s32 trackId)
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
    midiDataStart;

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

                    // This will make sure END_TRACK is on time
                    s32 bars = 0;
                    u32 onTime = 0;
                    while (onTime < deltaTime || bars != 0)
                    {
                        onTime += ticksPerBeat;
                        bars = (bars + 1) % seqHeader.mTimeSignatureBeats;
                    }

                    SPU::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->type = SPU::END_TRACK;
                    msg->tick = ticksPerBeat;
                    msg->tick = deltaTime + (onTime - deltaTime);
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

                    SPU::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->tick = deltaTime;
                    msg->channelId = channel;
                    msg->note = note;
                    msg->velocity = velocity;
                    if (velocity == 0) // If velocity is 0, then the sequence means to do "Note Off"
                    {
                        msg->type = SPU::NOTE_OFF;
                    }
                    else
                    {
                        msg->type = SPU::NOTE_ON;
                    }
                }
                break;
                case 0x8: // Note Off
                {
                    Uint8 note = 0;
                    stream.ReadUInt8(note);
                    Uint8 velocity = 0;
                    stream.ReadUInt8(velocity);

                    SPU::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->type = SPU::NOTE_OFF;
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

                    SPU::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->type = SPU::PATCH_CHANGE;
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

                    SPU::MIDIMessage* msg = seq->createMIDIMessage();
                    msg->type = SPU::PITCH_BEND;
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