#include "stdafx_ao.h"
#include "Function.hpp"
#include "CreditsController.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

namespace AO {

ALIVE_VAR(1, 0x507684, s16, gCreditsControllerExists, false);

constexpr s32 kShowCreditScreenForTicks = 60;

CreditsController::CreditsController(relive::Path_CreditsController* /*pTlv*/, s32 /*tlvInfo*/)
    : BaseGameObject(TRUE, 0)
{
    mCurrentCamera = gMap.mCurrentCamera;
    mNextCameraTimer = sGnFrame + kShowCreditScreenForTicks;

    gCreditsControllerExists = true;
}

CreditsController::~CreditsController()
{
    gCreditsControllerExists = false;
}

void CreditsController::VUpdate()
{
    if (mNextCameraTimer <= static_cast<s32>(sGnFrame))
    {
        mCurrentCamera++;
        if (mCurrentCamera > 24u)
        {
            mNextCameraTimer = sGnFrame + 60;
            mCurrentCamera = 1;
            gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, 1, CameraSwapEffects::eTopToBottom_3, 0, 0);
            sBreakGameLoop_507B78 = 1;
        }
        else
        {
            gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, mCurrentCamera, CameraSwapEffects::eTopToBottom_3, 0, 0);
            mNextCameraTimer = sGnFrame + 60;
        }
    }
}

} // namespace AO
