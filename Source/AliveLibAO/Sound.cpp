#include "stdafx_ao.h"
#include "Sound.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "BackgroundMusic.hpp"

#include "../AliveLibAE/Sound/Sound.hpp"

namespace AO {
void SND_SsQuit()
{
    SND_SsQuit_4EFD50();
}

void SND_Restart()
{
    MusicController::EnableMusic_443900(1);
    if (sBackgroundMusic_seq_id_4CFFF8 >= 0)
    {
        SND_SEQ_PlaySeq_4775A0(static_cast<SeqId>(sBackgroundMusic_seq_id_4CFFF8), 0, 0);
    }
    gMap.Start_Sounds_For_Objects_In_Near_Cameras_4467D0();
}

} // namespace AO
