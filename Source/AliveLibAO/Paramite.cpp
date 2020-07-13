#include "stdafx_ao.h"
#include "Function.hpp"
#include "Paramite.hpp"

void Paramite_ForceLink() {}

START_NS_AO

void Paramite::VUpdate_Real_44A490()
{
    NOT_IMPLEMENTED();
}

void Paramite::VUpdate_44A490()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_44A490();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

END_NS_AO

