#include "stdafx.h"
#include "DemoPlayback.hpp"
#include "Input.hpp"
#include "../relive_lib/Events.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

DemoPlayback* gActiveDemoPlayback = nullptr;
char_type gActiveDemoName[32];

DemoPlayback::DemoPlayback()
    : BaseGameObject(true, 0)
{
    if (gActiveDemoPlayback)
    {
        SetDead(true);
    }
    else
    {
        gActiveDemoPlayback = this;
        SetSurviveDeathReset(true);

        // TODO: FIX ME - should pass this resource in
        
        SetUpdateDelay(1);
        Input().InitDemo(gActiveDemoName);
        SetType(ReliveTypes::eDemoPlayback);
    }
}

void DemoPlayback::VScreenChanged()
{
    // Keep alive
}

DemoPlayback::~DemoPlayback()
{
    if (gActiveDemoPlayback == this)
    {
        gActiveDemoPlayback = nullptr;
    }
}

void DemoPlayback::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        Input().UnsetDemoPlaying();
    }

    if (!Input().IsDemoPlaying())
    {
        // demo finished playing, go back to the appropriate menu
        if (gIsDemoStartedManually)
        {
            // go back to the demo selection menu
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, 30, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
        else
        {
            // go back to the main screen
            gMap.SetActiveCam(EReliveLevelIds::eMenu, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
        }

        gMap.mFreeAllAnimAndPalts = true;
        SetDead(true);
    }
}
