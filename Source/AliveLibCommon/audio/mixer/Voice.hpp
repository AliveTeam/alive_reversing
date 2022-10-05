#pragma once
#include "../Soundbank.hpp"

namespace psx {

class Voice
{
public:
    Tone* m_Tone;
    int i_Program;
    int i_Note = 0;
    bool b_Dead = false;
    double f_SampleOffset = 0;
    bool b_NoteOn = true;
    double f_Velocity = 1.0f;
    double f_Pitch = 0.0f;

    int i_TrackID = 0;       // This is used to distinguish between sounds fx and music
    double f_TrackDelay = 0; // Used by the sequencer for perfect timing
    bool m_UsesNoteOffDelay = false;
    double f_NoteOffDelay = 0;

    // Active ADSR Levels

    double ActiveAttackLevel = 0;
    double ActiveReleaseLevel = 1;
    double ActiveDecayLevel = 1;
    double ActiveSustainLevel = 1;

    float GetSample();
};

} // namespace psx