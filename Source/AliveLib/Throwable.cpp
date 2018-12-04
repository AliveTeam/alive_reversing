#include "stdafx.h"
#include "Throwable.hpp"
#include "Function.hpp"

void BaseThrowable::vnull_408180()
{
    vsub_4114D0();
}

void BaseThrowable::Vsub_4114B0()
{
    vsub_4114B0();
}

EXPORT void BaseThrowable::vsub_4114B0()
{
    NOT_IMPLEMENTED();
}

EXPORT void BaseThrowable::vsub_4114D0()
{
    NOT_IMPLEMENTED();
}

// ====================================================================

void Rock::VScreenChanged()
{
    vScreenChanged_49F030();
}

int Rock::GetSaveState_4DC110(BYTE* pSaveBuffer)
{
    return vGetSaveState_49F9A0(pSaveBuffer);
}

EXPORT void Rock::vScreenChanged_49F030()
{
    NOT_IMPLEMENTED();
}

EXPORT int Rock::vGetSaveState_49F9A0(BYTE* /*pSaveBuffer*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT BaseThrowable* __stdcall Make_Throwable_49AF30(FP /*xpos*/, FP /*ypos*/, FP /*scale*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}
