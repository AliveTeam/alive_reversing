#include "stdafx.h"
#include "CreditsController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Map.hpp"

void CreditsController_ForceLink() { }

ALIVE_VAR(1, 0x5c1b90, __int16, sDoesCreditsControllerExist_5C1B90, false);
ALIVE_VAR(1, 0x5c30fe, __int16, word_5C30FE, 0);


void CreditsController::VDestructor(signed int flags)
{
    dtor_418A60(flags);
}

void CreditsController::VUpdate()
{
    Update_418AC0();
}

CreditsController * CreditsController::ctor_418A10(int a2, int a3)
{
    BaseGameObject::ctor_4DBFA0(1, 0);
    SetVTable(this, 0x544618);

    field_24 = gMap_5C3030.sCurrentCamId_5C3034;
    field_20_next_cam_frame = sGnFrame_5C1B84 + 160;

    sDoesCreditsControllerExist_5C1B90 = true;
    
    return this;
}

void CreditsController::dtor_418A90()
{
    sDoesCreditsControllerExist_5C1B90 = false;
    BaseGameObject::dtor_4DBEC0();
}

void CreditsController::dtor_418A60(char flags)
{
    dtor_418A90();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void CreditsController::Update_418AC0()
{
    if (field_20_next_cam_frame <= sGnFrame_5C1B84)
    {
        field_24++;
        field_20_next_cam_frame = sGnFrame_5C1B84 + 160;

        if (gMap_5C3030.sCurrentPathId_5C3032 == 2)
        {
            if (field_24 > 22)
            {
                field_24 = 1;
                gMap_5C3030.sub_480D30(16, 1, 1, 3, 0, 0);
            }
            else
            {
                gMap_5C3030.sub_480D30(16, 2, field_24, 3, 0, 0);
            }
            word_5C30FE = 1;
        }
        else
        {
            if (field_24 > 36)
            {
                gMap_5C3030.sub_480D30(0, 1, 1, 0, 0, 0);
            }
            else
            {
                gMap_5C3030.sub_480D30(16, 1, field_24, 3, 0, 0);
            }
            word_5C30FE = 1;
        }
    }
}
