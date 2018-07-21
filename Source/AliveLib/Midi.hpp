#pragma once

#include "FunctionFwd.hpp"

void Midi_ForceLink();

namespace Test
{
    void MidiTests();
}

EXPORT void CC MIDI_UpdatePlayer_4FDC80();
EXPORT void CC SEQ_4CB060();
EXPORT void CC SND_Init_4CA1F0();
EXPORT void CC SND_Shutdown_4CA280();
EXPORT int CC MIDI_46FBA0(unsigned __int8 a1, int a2, int a3, int a4);

class BackgroundMusic // TODO: Is a game object
{
public:
    EXPORT static void CC Stop_4CB000();
    EXPORT static void CC Play_4CB030();
};
