#include "stdafx.h"
#include "CreditsController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Map.hpp"

ALIVE_VAR(1, 0x5c1b90, __int16, sDoesCreditsControllerExist_5C1B90, false);


BaseGameObject* CreditsController::VDestructor(signed int flags)
{
    return vdtor_418A60(flags);
}

void CreditsController::VUpdate()
{
    Update_418AC0();
}

constexpr int kShowCreditScreenForTicks = 160;

CreditsController * CreditsController::ctor_418A10(Path_TLV* /*pTlv*/, int /*tlvInfo*/)
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x544618);

    field_24_camera_number = gMap_5C3030.sCurrentCamId_5C3034;
    field_20_next_cam_frame = sGnFrame_5C1B84 + kShowCreditScreenForTicks;

    sDoesCreditsControllerExist_5C1B90 = true;
    
    return this;
}

void CreditsController::dtor_418A90()
{
    sDoesCreditsControllerExist_5C1B90 = false;
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* CreditsController::vdtor_418A60(signed int flags)
{
    dtor_418A90();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void CreditsController::Update_418AC0()
{
    if (field_20_next_cam_frame <= static_cast<int>(sGnFrame_5C1B84))
    {
        field_24_camera_number++;
        field_20_next_cam_frame = sGnFrame_5C1B84 + kShowCreditScreenForTicks;

        if (gMap_5C3030.sCurrentPathId_5C3032 == 2)
        {
            if (field_24_camera_number > 22)
            {
                field_24_camera_number = 1;
                gMap_5C3030.SetActiveCam_480D30(LevelIds::eCredits_16, 1, 1, CameraSwapEffects::eEffect3_TopToBottom, 0, 0);
            }
            else
            {
                gMap_5C3030.SetActiveCam_480D30(LevelIds::eCredits_16, 2, field_24_camera_number, CameraSwapEffects::eEffect3_TopToBottom, 0, 0);
            }
            gMap_5C3030.field_CE_free_all_anim_and_palts = 1;
        }
        else
        {
            if (field_24_camera_number > 36)
            {
                gMap_5C3030.SetActiveCam_480D30(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eEffect0_InstantChange, 0, 0);
            }
            else
            {
                gMap_5C3030.SetActiveCam_480D30(LevelIds::eCredits_16, 1, field_24_camera_number, CameraSwapEffects::eEffect3_TopToBottom, 0, 0);
            }
            gMap_5C3030.field_CE_free_all_anim_and_palts = 1;
        }
    }
}
