#include "stdafx.h"
#include "DemoPlayback.hpp"
#include "Input.hpp"
#include "../relive_lib/Events.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

DemoPlayback* gActiveDemoPlayback = nullptr;

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
        //u32** ppRes = reinterpret_cast<u32**>(Add_Resource(ResourceManager::Resource_Demo, AEResourceID::kDemoResID));
        SetUpdateDelay(1);
        //Input().SetDemoResource_45F1E0(ppRes);
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
        Input().UnsetDemoPlaying_45F240();
    }

    if (!Input().Is_Demo_Playing_45F220())
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
