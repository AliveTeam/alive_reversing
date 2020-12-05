#include "stdafx.h"
#include "Sound.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "MusicController.hpp"
#include "Midi.hpp"
#include "BackgroundMusic.hpp"

#define NO_WAVE
#include "../AliveLibAE/Sound/Sound.hpp"

namespace AO
{
    EXPORT void SND_SsQuit_4938E0()
    {
        AE_IMPLEMENTED();
        SND_SsQuit_4EFD50();
    }

    EXPORT void CC SND_Restart_476340()
    {
        //NOT_IMPLEMENTED();
        MusicController::EnableMusic_443900(1);
        if (sBackgroundMusic_seq_id_4CFFF8 >= 0)
        {
            SND_SEQ_PlaySeq_4775A0(static_cast<SeqId>(sBackgroundMusic_seq_id_4CFFF8), 0, 0);
        }
        gMap_507BA8.Start_Sounds_For_Objects_In_Near_Cameras_4467D0();
    }

}
