#include "stdafx_ao.h"
#include "Function.hpp"
#include "CreditsController.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

namespace AO {

ALIVE_VAR(1, 0x507684, s16, gCreditsControllerExists_507684, false);

constexpr s32 kShowCreditScreenForTicks = 60;

CreditsController::CreditsController(Path_CreditsController* /*pTlv*/, s32 /*tlvInfo*/)
    : BaseGameObject(1)
{
    field_14_camera_number = gMap.mCurrentCamera;
    field_10_next_cam_frame = sGnFrame + kShowCreditScreenForTicks;

    gCreditsControllerExists_507684 = true;
}

CreditsController::~CreditsController()
{
    gCreditsControllerExists_507684 = false;
}

void CreditsController::VUpdate()
{
    if (field_10_next_cam_frame <= static_cast<s32>(sGnFrame))
    {
        field_14_camera_number++;
        if (field_14_camera_number > 24u)
        {
            field_10_next_cam_frame = sGnFrame + 60;
            field_14_camera_number = 1;
            gMap.SetActiveCam(LevelIds::eCredits_10, 1, 1, CameraSwapEffects::eTopToBottom_3, 0, 0);
            sBreakGameLoop_507B78 = 1;
        }
        else
        {
            gMap.SetActiveCam(LevelIds::eCredits_10, 1, field_14_camera_number, CameraSwapEffects::eTopToBottom_3, 0, 0);
            field_10_next_cam_frame = sGnFrame + 60;
        }
    }
}

} // namespace AO
