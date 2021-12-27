#include "stdafx.h"
#include "DemoPlayback.hpp"
#include "Input.hpp"
#include "ResourceManager.hpp"
#include "Events.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

ALIVE_VAR(1, 0x5D1E20, DemoPlayback*, sDemoObj_dword_5D1E20, nullptr);

void DemoPlayback::ctor()
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x54690C);

    if (sDemoObj_dword_5D1E20)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        sDemoObj_dword_5D1E20 = this;
        field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);

        u32** ppRes = reinterpret_cast<u32**>(Add_Resource_4DC130(ResourceManager::Resource_Demo, ResourceID::kDemoResID));
        SetUpdateDelay(1);
        sInputObject_5BD4E0.SetDemoResource_45F1E0(ppRes);
        field_4_typeId = AETypes::eDemoPlayback_98;
    }
}

BaseGameObject* DemoPlayback::VDestructor(s32 flags)
{
    return vdtor_497880(flags);
}

void DemoPlayback::VUpdate()
{
    vUpdate_4978E0();
}

void DemoPlayback::VScreenChanged()
{
    // Keep alive
}

void DemoPlayback::dtor_4978B0()
{
    SetVTable(this, 0x54690C);
    if (sDemoObj_dword_5D1E20 == this)
    {
        sDemoObj_dword_5D1E20 = nullptr;
    }
    BaseGameObject_dtor_4DBEC0();
}

DemoPlayback* DemoPlayback::vdtor_497880(s32 flags)
{
    dtor_4978B0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void DemoPlayback::vUpdate_4978E0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        sInputObject_5BD4E0.UnsetDemoPlaying_45F240();
    }

    if (!sInputObject_5BD4E0.Is_Demo_Playing_45F220())
    {
        // demo finished playing, go back to the appropriate menu
        if (gIsDemoStartedManually_5C1B9C)
        {
            // go back to the demo selection menu
            gMap_5C3030.SetActiveCam_480D30(LevelIds::eMenu_0, 1, 30, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
        else
        {
            // go back to the main screen
            gMap_5C3030.SetActiveCam_480D30(LevelIds::eMenu_0, 1, 1, CameraSwapEffects::eInstantChange_0, 0, 0);
        }

        gMap_5C3030.field_CE_free_all_anim_and_palts = 1;
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}
