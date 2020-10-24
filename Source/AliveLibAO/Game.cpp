#include "stdafx_ao.h"
#include "Game.hpp"
#include "logger.hpp"
#include "Function.hpp"
#include <set>
#include "FixedPoint.hpp"
#include "BaseGameObject.hpp"
#include "SwitchStates.hpp"
#include "DDCheat.hpp"

START_NS_AO

ALIVE_VAR(1, 0x507670, unsigned int, gnFrameCount_507670, 0);
ALIVE_VAR(1, 0x504618, DynamicArrayT<BaseGameObject>*, gObjList_drawables_504618, nullptr);

ALIVE_VAR(1, 0x50766C, DynamicArrayT<BaseGameObject>*, ObjListPlatforms_50766C, nullptr);

void Game_ForceLink()
{

}

ALIVE_VAR(1, 0x5076CC, short, gbKillUnsavedMudsDone_5076CC, 0);

// TODO: Move to game ender controller for AO sync
ALIVE_VAR(1, 0x5076C4, short, gRestartRuptureFarmsKilledMuds_5076C4, 0);
ALIVE_VAR(1, 0x5076C8, short, gRestartRuptureFarmsSavedMuds_5076C8, 0);


ALIVE_VAR(1, 0x5076D0, short, gOldKilledMuds_5076D0, 0);
ALIVE_VAR(1, 0x5076D4, short, gOldSavedMuds_5076D4, 0);


EXPORT void CC Init_GameStates_41CEC0()
{
    sKilledMudokons_5076BC = gRestartRuptureFarmsKilledMuds_5076C4;
    sRescuedMudokons_5076C0 = gRestartRuptureFarmsSavedMuds_5076C8;
    sSwitchStates_505568 = {};
    gOldKilledMuds_5076D0 = 0;
    gOldSavedMuds_5076D4 = 0;
    gbKillUnsavedMudsDone_5076CC = 0;
}

ALIVE_VAR(1, 0xAC6420, int*, psxemu_videomem_AC6420, nullptr);

class Bmp
{
public:
    // 0x00ac6420
    // 8
    // 16
    // 0x00ac6420 fff7
    // 0x00ac6420 1
    // str
    EXPORT static int CC DrawString_48FB90(int* /*pBmp*/, int /*x*/, int /*y*/, int /*colour*/, int /*backColour*/, const char* /*lpString*/)
    {
        NOT_IMPLEMENTED();
        return 0;
    }
};


END_NS_AO

