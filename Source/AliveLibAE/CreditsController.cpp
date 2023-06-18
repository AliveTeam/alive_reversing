#include "stdafx.h"
#include "CreditsController.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Map.hpp"

s16 gCreditsControllerExists = false;

constexpr s32 kShowCreditScreenForTicks = 160;

CreditsController::CreditsController(relive::Path_TLV* /*pTlv*/, const Guid& /*tlvId*/)
   : BaseGameObject(true, 0)
{
    mCurrentCamera = gMap.mCurrentCamera;
    mNextCameraTimer = MakeTimer(kShowCreditScreenForTicks);

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
        mNextCameraTimer = MakeTimer(kShowCreditScreenForTicks);

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
            gMap.mFreeAllAnimAndPalts = true;
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
            gMap.mFreeAllAnimAndPalts = true;
        }
    }
}
