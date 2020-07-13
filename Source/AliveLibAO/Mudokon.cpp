#include "stdafx_ao.h"
#include "Function.hpp"
#include "Mudokon.hpp"

void Mud_ForceLink() {}

START_NS_AO

void Mudokon::VUpdate_Real_43F560()
{
    NOT_IMPLEMENTED();
}

void Mudokon::VUpdate_43F560()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_43F560();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

END_NS_AO

