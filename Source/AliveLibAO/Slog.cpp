#include "stdafx_ao.h"
#include "Function.hpp"
#include "Slog.hpp"

void Slog_ForceLink() {}

START_NS_AO

void Slog::VUpdate_Real_4739C0()
{
    NOT_IMPLEMENTED();
}

void Slog::VUpdate_4739C0()
{
    const __int16 oldMotion = field_FC_current_motion;

    VUpdate_Real_4739C0();

    if (oldMotion != field_FC_current_motion)
    {
        LOG_INFO("oldMotion = " << oldMotion << " newMotion = " << field_FC_current_motion);
    }
}

END_NS_AO

