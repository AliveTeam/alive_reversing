#include "stdafx_ao.h"
#include "PsxDisplay.hpp"
#include "Function.hpp"

START_NS_AO

const PSX_Display_Params kDisplayParams = { 640, 240, 16, 1, 42, 32, 1, 0 };
ALIVE_VAR(1, 0x4BB830, PSX_Display_Params, gPsxDisplayParams_4BB830, kDisplayParams);
ALIVE_VAR(1, 0x504C78, PsxDisplay, gPsxDisplay_504C78, {});

EXPORT PsxDisplay* PsxDisplay::ctor_40DAB0(const PSX_Display_Params* /*pParams*/)
{
    NOT_IMPLEMENTED();
    return this;
}

EXPORT void PsxDisplay::PSX_Display_Render_OT_40DD20()
{
    NOT_IMPLEMENTED();
}

END_NS_AO

