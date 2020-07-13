#include "stdafx_ao.h"
#include "Function.hpp"
#include "Scrab.hpp"

void Scrab_ForceLink() {}

START_NS_AO

void Scrab::VUpdate_Real_45B360()
{
    NOT_IMPLEMENTED();
}

void Scrab::VUpdate_45B360()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_45B360();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

END_NS_AO

