#include "stdafx.h"
#include "Sfx.hpp"
#include "Function.hpp"
#include "DebugHelpers.hpp"

ALIVE_ARY(1, 0x55C2A0, SfxDefinition, 146, sSfxEntries_55C2A0, {});

EXPORT int CC SFX_4CA420(SfxDefinition *a1, __int16 a2, __int16 min, __int16 max)
{
    NOT_IMPLEMENTED();
}

int CC SFX_Play_46FBA0(unsigned __int8 sfxIdx, __int16 volume, int a3, int scale)
{
    if (!volume)
    {
        volume = (char)sSfxEntries_55C2A0[sfxIdx].field_3_default_volume;
    }
    if (scale == 0x8000)
    {
        volume /= 3;
    }
    return SFX_4CA420(&sSfxEntries_55C2A0[sfxIdx], volume, a3, a3);
}

int CC SFX_Play_46FA90(unsigned __int8 sfxIdx, __int16 volume, int scale)
{
    if (!volume)
    {
        volume = sSfxEntries_55C2A0[sfxIdx].field_3_default_volume;
    }
    if (scale == 0x8000)
    {
        volume /= 3;
    }
    return SFX_4CA420(&sSfxEntries_55C2A0[sfxIdx], volume, 0x7FFF, 0x7FFF);
}