#include "stdafx_ao.h"
#include "CreditsController.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

s16 gCreditsControllerExists = false;

constexpr s32 kShowCreditScreenForTicks = 60;

CreditsController::CreditsController(relive::Path_CreditsController* /*pTlv*/, const Guid& /*tlvId*/)
    : BaseGameObject(true, 0)
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
            sBreakGameLoop = 1;
        }
        else
        {
            gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, mCurrentCamera, CameraSwapEffects::eTopToBottom_3, 0, 0);
            mNextCameraTimer = sGnFrame + 60;
        }
    }
}

} // namespace AO
