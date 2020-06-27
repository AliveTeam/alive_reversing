#include "stdafx_ao.h"
#include "Function.hpp"
#include "CreditsController.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

START_NS_AO

ALIVE_VAR(1, 0x507684, __int16, gCreditsControllerExists_507684, false);

constexpr int kShowCreditScreenForTicks = 60;

void CreditsController::VUpdate()
{
    VUpdate_40D020();
}

BaseGameObject* CreditsController::VDestructor(signed int flags)
{
    return Vdtor_40D090(flags);
}

CreditsController* CreditsController::ctor_40CFC0(Path_TLV* /*pTlv*/, int /*tlvInfo*/)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA458);

    field_14_camera_number = gMap_507BA8.field_4_current_camera;
    field_10_next_cam_frame = gnFrameCount_507670 + kShowCreditScreenForTicks;

    gCreditsControllerExists_507684 = true;

    return this;
}

BaseGameObject* CreditsController::dtor_40D000()
{
    SetVTable(this, 0x4BA458);
    gCreditsControllerExists_507684 = false;
    return dtor_487DF0();
}

CreditsController* CreditsController::Vdtor_40D090(signed int flags)
{
    dtor_40D000();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void CreditsController::VUpdate_40D020()
{
    if (field_10_next_cam_frame <= static_cast<int>(gnFrameCount_507670))
    {
        field_14_camera_number++;
        if (field_14_camera_number > 24u)
        {
            field_10_next_cam_frame = gnFrameCount_507670 + 60;
            field_14_camera_number = 1;
            gMap_507BA8.SetActiveCam_444660(LevelIds::eCredits_10, 1, 1, CameraSwapEffects::eEffect3_TopToBottom, 0, 0);
            sBreakGameLoop_507B78 = 1;
        }
        else
        {
            gMap_507BA8.SetActiveCam_444660(LevelIds::eCredits_10, 1, field_14_camera_number, CameraSwapEffects::eEffect3_TopToBottom, 0, 0);
            field_10_next_cam_frame = gnFrameCount_507670 + 60;
        }
    }
}

END_NS_AO
