#include "stdafx_ao.h"
#include "Function.hpp"
#include "SlingMudokon.hpp"

void SlingMud_ForceLink() { }

START_NS_AO

void SlingMudokon::VUpdate_Real_46FBF0()
{
    NOT_IMPLEMENTED();
}

void SlingMudokon::VUpdate_46FBF0()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_46FBF0();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

END_NS_AO
