#pragma once

#include "FunctionFwd.hpp"

enum Type1SFX
{
    eUXBGreen = 2,
    eUXBRed = 3,
};

struct SfxDefinition
{
    char field_0_block_idx;
    char field_1_program;
    char field_2_note;
    char field_3_default_volume;
    __int16 field_4_pitch_min;
    __int16 field_6_pitch_max;
};
ALIVE_ASSERT_SIZEOF(SfxDefinition, 0x8);

EXPORT int CC SFX_SfxDefinition_Play_4CA700(const SfxDefinition* sfxDef, __int16 volLeft, __int16 volRight, __int16 pitch_min, __int16 pitch_max);
EXPORT int CC SFX_SfxDefinition_Play_4CA420(const SfxDefinition* sfxDef, __int16 volume, __int16 pitch_min, __int16 pitch_max);
EXPORT int CC SFX_Play_46FB10(unsigned __int8 sfxId, int leftVol, int rightVol, int scale);
EXPORT int CC SFX_Play_46FBA0(unsigned __int8 sfxIdx, __int16 volume, int pitch, int scale);
EXPORT int CC SFX_Play_46FA90(unsigned __int8 sfxIdx, __int16 volume, int scale);
EXPORT int CC SFX_Play_46FC20(unsigned __int8 sfxId, __int16 volume, __int16 mode, int scale);
EXPORT int CC SND_4CA5D0(int a1, int a2, int a3, __int16 vol, __int16 min, __int16 max);
