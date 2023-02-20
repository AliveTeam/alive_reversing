#include "SequencePlayer.hpp"

namespace psx {

SequencePlayer::SequencePlayer()
{
    // Start the Sequencer thread.
    m_SequenceThread = new std::thread(&SequencePlayer::m_PlayerThreadFunction, this);
    m_QuarterCallback = nullptr;
}

SequencePlayer::~SequencePlayer()
{
    m_KillThread = true;
    m_SequenceThread->join();
    delete m_SequenceThread;
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

float SequencePlayer::MidiTimeToSample(int time)
{
    // This may, or may not be correct. // TODO: Revise
    // Oct 7, 2022 - added (x1.041f). For some reason this seems to match AE playback speed...
    // AO might be better with just times 1.000?
    return ((60.0f * float(time)) / float(m_SongTempo)) * (float(AliveAudioSampleRate) / 500.0f) * 1000.0f;
}

s32 SequencePlayer::completedRepeats()
{
    return mCompletedRepeats.load();
}

u64 timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void SequencePlayer::m_PlayerThreadFunction()
{
    int channels[16];
    for (int i = 0; i < 16; i++)
    {
        channels[i] = 0;
    }

    u64 ms_tick = timeSinceEpochMillisec();
    int trackPosition = 0;
    while (!m_KillThread)
    {

        m_PlayerStateMutex.lock();
        AliveAudio::LockNotes();
        float multi = float(std::min(mVolLeft, mVolRight)) / 127.0f;

        if ((int) m_MessageList.size() > 0)
        {
            if (trackPosition >= (int) m_MessageList.size())
            {
                trackPosition = 0;
                ms_tick = timeSinceEpochMillisec();
            }

            u64 ms_now = timeSinceEpochMillisec() - ms_tick;
            float tickDurationUs = m_RawTempo / m_ticksPerBeat;
            u64 diffUs = ms_now * 1000;
            u64 track_tick = u64(diffUs / tickDurationUs);


            while (trackPosition < (int) m_MessageList.size())
            {
                AliveAudioMidiMessage m = m_MessageList[trackPosition];
                if (m.TimeOffset > track_tick)
                {
                    break;
                }

                trackPosition++;
                switch (m.Type)
                {
                    case ALIVE_MIDI_NOTE_ON:
                        AliveAudio::NoteOn(channels[m.Channel], m.Note, (char) m.Velocity, m_PlayId, MidiTimeToSample(m.TimeOffset), multi);
                        break;
                    case ALIVE_MIDI_NOTE_OFF:
                        AliveAudio::NoteOffDelay(channels[m.Channel], m.Note, m_PlayId, MidiTimeToSample(m.TimeOffset)); // Fix this. Make note off's have an offset in the voice timeline.
                        break;
                    case ALIVE_MIDI_PROGRAM_CHANGE:
                        channels[m.Channel] = m.Special;
                        break;
                        //break;
                    case ALIVE_MIDI_ENDTRACK:
                        m_PlayerState = ALIVE_SEQUENCER_PLAYING;
                        m_SongFinishSample = ((int) (AliveAudio::currentSampleIndex + MidiTimeToSample(m.TimeOffset)));
                        mCompletedRepeats.store(mCompletedRepeats.load() + 1);
                        m_PlayerState = ALIVE_SEQUENCER_FINISHED;
                        break;
                    default:
                    {
                    }
                        // Nothin
                }
            }
        }

        AliveAudio::UnlockNotes();
        m_PlayerStateMutex.unlock();

        // THIS IS RIGHT!
        // int t = (int) (m_RawTempo * m.TimeOffset / m_ticksPerBeat);
        //std::this_thread::sleep_for(std::chrono::microseconds((int) t));

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
      

}

int SequencePlayer::GetPlaybackPositionSample()
{
    return ((int) (AliveAudio::currentSampleIndex - m_SongBeginSample));
}

void SequencePlayer::SetVolume(s32 volLeft, s32 volRight)
{
    m_PlayerStateMutex.lock();
    mVolLeft = volLeft;
    //AliveAudio::LockNotes();
    //AliveAudio::SetVolume(m_TrackID, mVolLeft, mVolRight);
    //AliveAudio::UnlockNotes();
    mVolRight = volRight;
    m_PlayerStateMutex.unlock();
}

void SequencePlayer::StopSequence()
{
    m_PlayerStateMutex.lock();
    AliveAudio::LockNotes();
    AliveAudio::ClearAllTrackVoices(m_PlayId, false);
    m_PlayerState = ALIVE_SEQUENCER_STOPPED;
    mCompletedRepeats.store(1);
    m_PrevBar = 0;
    AliveAudio::UnlockNotes();
    m_PlayerStateMutex.unlock();
}

void SequencePlayer::PlaySequence(s32 playId)
{
    m_PlayerStateMutex.lock();
    m_PlayId = playId;
    if (m_PlayerState == ALIVE_SEQUENCER_STOPPED || m_PlayerState == ALIVE_SEQUENCER_FINISHED)
    {
        m_PrevBar = 0;
        mCompletedRepeats.store(0);
        m_PlayerState = ALIVE_SEQUENCER_INIT_VOICES;
    }
    m_PlayerStateMutex.unlock();
}

int SequencePlayer::LoadSequenceData(std::vector<Uint8> seqData, s32 trackId, s32 repeatCount)
{
    m_TrackID = trackId;
    mRepeatCount = repeatCount;
    Stream stream(std::move(seqData));

    return LoadSequenceStream(stream);
}

int SequencePlayer::LoadSequenceStream(Stream& stream)
{
    StopSequence();
    m_MessageList.clear();

    SeqHeader seqHeader;

    // Read the header
    if (stream.Size() == 0)
    {
        std::cout << "no stream!?\n";
        return 1;
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

    int q = 0;
    for (int i = 0; i < 2; i++)
    {
        q += seqHeader.mResolutionOfQuaterNote[1 - i] << (8 * i);
    }

    m_TimeSignatureBars = seqHeader.mTimeSignatureBars;

    m_SongTempo = ((float) (60000000.0 / tempoValue));
    m_RawTempo = (float)tempoValue; // tempo (length of quarter note in microseconds) 0.000001us/s  vs 0.001ms/s
    m_ticksPerBeat = float(q);

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
                return 0; // error if no running status?
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
                    //std::cout << "end of track" << std::endl;

                    // This may not be right, but I've found it lines up new sections well.
                    // If not, pass deltaTime instead of nextQuarter
                    unsigned int quarterDur = int(m_SongTempo);
                    unsigned int nextQuarter = 0;

                    while (nextQuarter < prevDeltaTime)
                    {
                        nextQuarter += quarterDur;
                    }
                    //nextQuarter -= quarterDur;

                    if (nextQuarter - quarterDur <= 0)
                    {
                        nextQuarter = deltaTime;
                    }

                    m_MessageList.push_back(AliveAudioMidiMessage(ALIVE_MIDI_ENDTRACK, 480, 0, 0, 0));
                    return 0;
                    //Sint32 loopCount = gSeqInfo.iNumTimesToLoop; // v1 some hard coded data?? or just a local static?
                    //if (loopCount)                            // If zero then loop forever
                    //{
                    //    --loopCount;

                    //    //char buf[256];
                    //    //sprintf(buf, "EOT: %d loops left\n", loopCount);
                    //    // OutputDebugString(buf);

                    //    gSeqInfo.iNumTimesToLoop = loopCount; //v1
                    //    if (loopCount <= 0)
                    //    {
                    //        //getNext_q(aSeqIndex); // Done playing? Ptr not reset to start
                    //        return 1;
                    //    }
                    //}

                    ////OutputDebugString("EOT: Loop forever\n");
                    //// Must be a loop back to the start?
                    //stream.Seek(midiDataStart);
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
                    if (velocity == 0) // If velocity is 0, then the sequence means to do "Note Off"
                    {
                        m_MessageList.push_back(AliveAudioMidiMessage(ALIVE_MIDI_NOTE_OFF, deltaTime, channel, note, velocity));
                    }
                    else
                    {
                        prevDeltaTime = deltaTime;
                        m_MessageList.push_back(AliveAudioMidiMessage(ALIVE_MIDI_NOTE_ON, deltaTime, channel, note, velocity));
                    }
                }
                break;
                case 0x8: // Note Off
                {
                    Uint8 note = 0;
                    stream.ReadUInt8(note);
                    Uint8 velocity = 0;
                    stream.ReadUInt8(velocity);

                    m_MessageList.push_back(AliveAudioMidiMessage(ALIVE_MIDI_NOTE_OFF, deltaTime, channel, note, velocity));
                }
                break;
                case 0xc: // Program Change
                {
                    Uint8 prog = 0;
                    stream.ReadUInt8(prog);
                    //prevDeltaTime = deltaTime;
                    m_MessageList.push_back(AliveAudioMidiMessage(ALIVE_MIDI_PROGRAM_CHANGE, deltaTime, channel, 0, 0, prog));
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