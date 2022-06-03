#include "stdafx.h"
#include "CreditsController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Map.hpp"

ALIVE_VAR(1, 0x5c1b90, s16, sDoesCreditsControllerExist_5C1B90, false);

constexpr s32 kShowCreditScreenForTicks = 160;

CreditsController::CreditsController(Path_TLV* /*pTlv*/, s32 /*tlvInfo*/)
   : BaseGameObject(TRUE, 0)
{
    field_24_camera_number = gMap.mCurrentCamera;
    field_20_next_cam_frame = sGnFrame + kShowCreditScreenForTicks;

    sDoesCreditsControllerExist_5C1B90 = true;
}

CreditsController::~CreditsController()
{
    sDoesCreditsControllerExist_5C1B90 = false;
}

void CreditsController::VUpdate()
{
    if (field_20_next_cam_frame <= static_cast<s32>(sGnFrame))
    {
        field_24_camera_number++;
        field_20_next_cam_frame = sGnFrame + kShowCreditScreenForTicks;

        if (gMap.mCurrentPath == 2)
        {
            if (field_24_camera_number > 22)
            {
                field_24_camera_number = 1;
                gMap.SetActiveCam(LevelIds::eCredits_16, 1, 1, CameraSwapEffects::eTopToBottom_3, 0, 0);
            }
            else
            {
                gMap.SetActiveCam(LevelIds::eCredits_16, 2, field_24_camera_number, CameraSwapEffects::eTopToBottom_3, 0, 0);
            }
            gMap.field_CE_free_all_anim_and_palts = 1;
        }
        else
        {
            if (field_24_camera_number > 36)
            {
                gMap.SetActiveCam(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
            }
            else
            {
                gMap.SetActiveCam(LevelIds::eCredits_16, 1, field_24_camera_number, CameraSwapEffects::eTopToBottom_3, 0, 0);
            }
            gMap.field_CE_free_all_anim_and_palts = 1;
        }
    }
}
