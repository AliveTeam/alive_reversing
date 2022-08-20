#include "stdafx.h"
#include "CreditsController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Map.hpp"

ALIVE_VAR(1, 0x5c1b90, s16, gCreditsControllerExists, false);

constexpr s32 kShowCreditScreenForTicks = 160;

CreditsController::CreditsController(Path_TLV* /*pTlv*/, s32 /*tlvInfo*/)
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
        mNextCameraTimer = sGnFrame + kShowCreditScreenForTicks;

        if (gMap.mCurrentPath == 2)
        {
            if (mCurrentCamera > 22)
            {
                mCurrentCamera = 1;
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, 1, CameraSwapEffects::eTopToBottom_3, 0, 0);
            }
            else
            {
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 2, mCurrentCamera, CameraSwapEffects::eTopToBottom_3, 0, 0);
            }
            gMap.mFreeAllAnimAndPalts = 1;
        }
        else
        {
            if (mCurrentCamera > 36)
            {
                gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
            }
            else
            {
                gMap.SetActiveCam(EReliveLevelIds::eCredits, 1, mCurrentCamera, CameraSwapEffects::eTopToBottom_3, 0, 0);
            }
            gMap.mFreeAllAnimAndPalts = 1;
        }
    }
}
