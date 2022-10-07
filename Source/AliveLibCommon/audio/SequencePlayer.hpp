#pragma once

#include <SDL.h>
#include <string>
#include <thread>
#include <vector>
#include "Stream.hpp"
#include "mixer/AliveAudio.hpp"

namespace psx {

struct SeqHeader
{
    Uint32 mMagic;   // SEQp
    Uint32 mVersion; // Seems to always be 1
    Uint16 mResolutionOfQuaterNote;
    Uint8 mTempo[3];
    Uint8 mTimeSignatureBars;
    Uint8 mTimeSignatureBeats;
};

struct SeqInfo
{
    Uint32 iLastTime = 0;
    Sint32 iNumTimesToLoop = 0;
    Uint8 running_status = 0;
};

// The types of Midi Messages the sequencer will play.
enum AliveAudioMidiMessageType
{
    ALIVE_MIDI_NOTE_ON = 1,
    ALIVE_MIDI_NOTE_OFF = 2,
    ALIVE_MIDI_PROGRAM_CHANGE = 3,
    ALIVE_MIDI_ENDTRACK = 4,
};

// The current state of a SequencePlayer.
enum AliveAudioSequencerState
{
    ALIVE_SEQUENCER_STOPPED = 1,
    ALIVE_SEQUENCER_PLAYING = 3,
    ALIVE_SEQUENCER_FINISHED = 4,
    ALIVE_SEQUENCER_INIT_VOICES = 5,
};

struct AliveAudioMidiMessage
{
    AliveAudioMidiMessage(AliveAudioMidiMessageType type, int timeOffset, int channel, int note, int velocity, int special = 0)
    {
        Type = type;
        Channel = channel;
        Note = note;
        Velocity = velocity;
        TimeOffset = timeOffset;
        Special = special;
    }
    AliveAudioMidiMessageType Type;
    int Channel;
    int Note;
    int Velocity;
    int TimeOffset;
    int Special = 0;
};

// Gets called every time the play position is at 1/4 of the song.
// Useful for changing sequences but keeping the time signature in sync.
typedef void (*AliveAudioQuarterCallback)();

class SequencePlayer
{
public:
    SequencePlayer();
    ~SequencePlayer();

    s32 mRepeatCount;

    int LoadSequenceData(std::vector<Uint8> seqData, s32 trackId, s32 repeatCount);
    int LoadSequenceStream(Stream& stream);
    void PlaySequence();
    void StopSequence();
    int completedRepeats();

    float MidiTimeToSample(int time);
    int GetPlaybackPositionSample();

    int m_TrackID = 1; // The track ID. Use this to seperate SoundFX from Music.
    AliveAudioSequencerState m_PlayerState = ALIVE_SEQUENCER_STOPPED;
    AliveAudioQuarterCallback m_QuarterCallback;

private:
    std::atomic<int> mCompletedRepeats;
    int m_KillThread = false;   // If true, loop thread will exit.
    int m_SongFinishSample = 0; // Not relative.
    int m_SongBeginSample = 0;  // Not relative.
    int m_PrevBar = 0;
    int m_TimeSignatureBars;
    float m_SongTempo;
    void m_PlayerThreadFunction();
    std::vector<AliveAudioMidiMessage> m_MessageList;
    std::thread* m_SequenceThread;
    std::mutex m_MessageListMutex;
    std::mutex m_PlayerStateMutex;
};

} // namespace psx