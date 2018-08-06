#include "stdafx.h"
#include "Sfx.hpp"
#include "Function.hpp"
#include "DebugHelpers.hpp"

ALIVE_ARY(1, 0x55C2A0, SfxDefinition, 146, sSfxEntries_55C2A0, {});

EXPORT int CC SFX_4CA420(SfxDefinition *a1, __int16 a2, __int16 min, __int16 max)
{
    NOT_IMPLEMENTED();
}

int CC SFX_Play_46FBA0(unsigned __int8 /*a1*/, int /*a2*/, int /*a3*/, int /*a4*/)
{
    NOT_IMPLEMENTED();
}

int CC SFX_Play_46FA90(unsigned __int8 sfxIdx, __int16 a2, int a3)
{
    __int16 v3; // ax

    v3 = a2;
    if (!a2)
    {
        v3 = (char)sSfxEntries_55C2A0[sfxIdx].field_3;
    }
    if (a3 == 0x8000)
    {
        v3 /= 3;
    }
    return SFX_4CA420(&sSfxEntries_55C2A0[sfxIdx], v3, 0x7FFF, 0x7FFF);
}