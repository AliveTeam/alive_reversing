#include "stdafx.h"
#include "Sfx.hpp"
#include "Function.hpp"
#include "DebugHelpers.hpp"
#include "Midi.hpp"
#include "PathData.hpp"

ALIVE_VAR(1, 0x560f58, __int16, sSFXPitchVariationEnabled_560F58, true);

ALIVE_ARY(1, 0x55C2A0, SfxDefinition, 146, sSfxEntries_55C2A0, {
    { 0u, 0u, 66u, 75u, 0, 0 },
    { 0u, 0u, 67u, 75u, 0, 0 },
    { 0u, 0u, 72u, 55u, 0, 0 },
    { 0u, 0u, 73u, 55u, 0, 0 },
    { 0u, 1u, 57u, 40u, 0, 0 },
    { 0u, 39u, 60u, 127u, 65535, 1 },
    { 0u, 2u, 64u, 55u, 65472, 64 },
    { 0u, 7u, 36u, 127u, 0, 0 },
    { 0u, 6u, 53u, 127u, 0, 0 },
    { 0u, 69u, 47u, 127u, 65535, 1 },
    { 0u, 69u, 60u, 127u, 65535, 1 },
    { 0u, 6u, 60u, 127u, 0, 0 },
    { 0u, 6u, 48u, 90u, 0, 0 },
    { 0u, 7u, 64u, 70u, 0, 0 },
    { 0u, 7u, 64u, 70u, 65281, 65281 },
    { 0u, 7u, 64u, 70u, 65409, 65409 },
    { 0u, 9u, 62u, 90u, 0, 0 },
    { 0u, 9u, 62u, 90u, 0, 0 },
    { 0u, 9u, 62u, 70u, 1, 1 },
    { 0u, 9u, 62u, 90u, 65534, 65534 },
    { 0u, 9u, 58u, 90u, 65535, 1 },
    { 0u, 9u, 57u, 90u, 65535, 1 },
    { 0u, 9u, 36u, 45u, 65534, 65534 },
    { 0u, 9u, 37u, 50u, 65534, 65534 },
    { 0u, 9u, 38u, 60u, 65535, 1 },
    { 0u, 9u, 39u, 90u, 65535, 1 },
    { 0u, 9u, 40u, 60u, 65535, 1 },
    { 0u, 9u, 41u, 60u, 65535, 1 },
    { 0u, 9u, 60u, 115u, 0, 0 },
    { 0u, 9u, 61u, 60u, 0, 0 },
    { 0u, 10u, 60u, 80u, 65535, 1 },
    { 0u, 10u, 61u, 55u, 65535, 1 },
    { 0u, 3u, 60u, 60u, 65535, 1 },
    { 0u, 4u, 63u, 60u, 65535, 1 },
    { 0u, 12u, 44u, 90u, 65534, 2 },
    { 0u, 13u, 60u, 65u, 65535, 1 },
    { 0u, 4u, 58u, 70u, 65535, 1 },
    { 0u, 4u, 59u, 70u, 65535, 1 },
    { 0u, 0u, 44u, 70u, 64636, 64636 },
    { 0u, 2u, 38u, 127u, 65535, 1 },
    { 0u, 2u, 39u, 45u, 65535, 1 },
    { 0u, 2u, 49u, 30u, 65535, 1 },
    { 0u, 10u, 48u, 70u, 65535, 1 },
    { 0u, 10u, 48u, 70u, 65409, 65411 },
    { 0u, 20u, 48u, 90u, 0, 0 },
    { 0u, 20u, 43u, 90u, 0, 0 },
    { 0u, 20u, 36u, 90u, 0, 0 },
    { 0u, 0u, 74u, 127u, 0, 0 },
    { 0u, 0u, 63u, 55u, 0, 0 },
    { 0u, 2u, 36u, 127u, 0, 0 },
    { 0u, 2u, 37u, 70u, 0, 0 },
    { 0u, 26u, 40u, 46u, 0, 0 },
    { 0u, 1u, 53u, 25u, 0, 0 },
    { 0u, 29u, 48u, 50u, 0, 0 },
    { 0u, 29u, 49u, 20u, 0, 0 },
    { 0u, 10u, 59u, 90u, 0, 0 },
    { 0u, 10u, 62u, 50u, 64736, 64736 },
    { 0u, 10u, 64u, 65u, 64536, 64536 },
    { 0u, 10u, 36u, 45u, 63676, 63676 },
    { 0u, 34u, 61u, 60u, 0, 0 },
    { 0u, 11u, 36u, 90u, 0, 0 },
    { 0u, 11u, 37u, 90u, 0, 0 },
    { 0u, 11u, 60u, 127u, 0, 0 },
    { 0u, 10u, 63u, 60u, 0, 0 },
    { 0u, 4u, 65u, 90u, 0, 0 },
    { 0u, 5u, 58u, 55u, 65281, 127 },
    { 0u, 5u, 59u, 55u, 65281, 127 },
    { 0u, 0u, 84u, 127u, 0, 0 },
    { 0u, 0u, 64u, 60u, 0, 0 },
    { 0u, 44u, 60u, 60u, 0, 0 },
    { 0u, 44u, 61u, 60u, 0, 0 },
    { 0u, 3u, 62u, 45u, 65409, 127 },
    { 0u, 3u, 63u, 45u, 65409, 127 },
    { 0u, 0u, 70u, 6u, 0, 0 },
    { 0u, 48u, 36u, 40u, 0, 0 },
    { 0u, 48u, 42u, 40u, 0, 0 },
    { 0u, 10u, 65u, 90u, 64736, 64736 },
    { 0u, 10u, 65u, 90u, 65136, 65136 },
    { 0u, 10u, 65u, 60u, 0, 0 },
    { 0u, 10u, 66u, 45u, 0, 0 },
    { 0u, 10u, 66u, 45u, 0, 0 },
    { 0u, 18u, 60u, 60u, 0, 0 },
    { 0u, 18u, 38u, 60u, 0, 0 },
    { 0u, 18u, 48u, 60u, 0, 0 },
    { 0u, 10u, 35u, 90u, 0, 0 },
    { 0u, 4u, 60u, 127u, 0, 0 },
    { 0u, 4u, 68u, 127u, 0, 0 },
    { 0u, 25u, 38u, 90u, 64536, 64536 },
    { 0u, 25u, 39u, 90u, 64536, 64536 },
    { 0u, 6u, 60u, 80u, 600, 700 },
    { 0u, 6u, 61u, 20u, 2667, 3175 },
    { 0u, 3u, 48u, 100u, 65486, 50 },
    { 0u, 10u, 64u, 65u, 64536, 64536 },
    { 0u, 1u, 51u, 127u, 0, 0 },
    { 0u, 7u, 72u, 40u, 0, 0 },
    { 0u, 7u, 73u, 40u, 0, 0 },
    { 0u, 7u, 74u, 40u, 0, 0 },
    { 0u, 10u, 40u, 80u, 800, 800 },
    { 0u, 10u, 41u, 127u, 0, 0 },
    { 0u, 10u, 42u, 90u, 0, 0 },
    { 0u, 7u, 60u, 75u, 0, 0 },
    { 0u, 7u, 61u, 100u, 0, 0 },
    { 0u, 1u, 58u, 127u, 63036, 62736 },
    { 0u, 5u, 72u, 127u, 65036, 500 },
    { 0u, 5u, 73u, 127u, 65036, 500 },
    { 0u, 6u, 37u, 60u, 65336, 200 },
    { 0u, 6u, 36u, 127u, 0, 0 },
    { 0u, 19u, 36u, 127u, 0, 0 },
    { 0u, 19u, 60u, 127u, 0, 0 },
    { 0u, 9u, 62u, 60u, 3048, 3048 },
    { 0u, 9u, 62u, 60u, 3048, 3048 },
    { 0u, 9u, 62u, 60u, 3048, 3048 },
    { 0u, 9u, 62u, 60u, 3048, 3048 },
    { 0u, 9u, 62u, 60u, 3175, 3175 },
    { 0u, 9u, 62u, 60u, 3302, 3302 },
    { 0u, 9u, 62u, 60u, 3429, 3429 },
    { 0u, 1u, 51u, 100u, 0, 0 },
    { 0u, 1u, 52u, 100u, 0, 0 },
    { 0u, 1u, 57u, 60u, 63736, 63736 },
    { 0u, 1u, 86u, 120u, 0, 0 },
    { 0u, 6u, 67u, 127u, 0, 0 },
    { 0u, 6u, 64u, 127u, 0, 0 }
});

EXPORT void CC SFX_SetPitch_4CA510(SfxDefinition *sfxEntry, int a2, __int16 pitch)
{
    NOT_IMPLEMENTED();
}

int CC SFX_SfxDefinition_Play_4CA420(SfxDefinition *sfxDef, __int16 volume, __int16 pitch_min, __int16 pitch_max)
{
    if (!volume)
    {
        volume = sfxDef->field_3_default_volume;
    }

    if (pitch_min == 0x7FFF)
    {
        pitch_min = sfxDef->field_4_pitch_min;
    }

    if (pitch_max == 0x7FFF)
    {
        pitch_max = sfxDef->field_6_pitch_max;
    }

    if (volume < 1)
    {
        volume = 1;
    }
    else if (volume >= 127)
    {
        volume = 127;
    }

    auto midiHandle = MIDI_Play_Single_Note_4CA1B0(
        sfxDef->field_1_program | (sLastLoadedSoundBlockInfo_BB2E34[sfxDef->field_0_block_idx].field_8_vab_id << 8),
        sfxDef->field_2_note << 8,
        volume,
        volume);

    if (!sSFXPitchVariationEnabled_560F58)
    {
        return 0;
    }

    if (pitch_min || pitch_max)
    {
        SFX_SetPitch_4CA510(sfxDef, midiHandle, Math_RandomRange_496AB0(pitch_min, pitch_max));
    }

    return midiHandle;
}

int CC SFX_SfxDefinition_Play_4CA700(SfxDefinition *sfxDef, __int16 volLeft, __int16 volRight, __int16 pitch_min, __int16 pitch_max)
{
    if (pitch_min == 0x7FFF)
    {
        pitch_min = sfxDef->field_4_pitch_min;
    }

    if (pitch_max == 0x7FFF)
    {
        pitch_max = sfxDef->field_6_pitch_max;
    }

    if (volLeft < 10)
    {
        volLeft = 10;
    }
    else if (volLeft >= 127)
    {
        volLeft = 127;
    }

    if (volRight < 10)
    {
        volRight = 10;
    }
    else if (volRight >= 127)
    {
        volRight = 127;
    }

    auto midiHandle = MIDI_Play_Single_Note_4CA1B0(
        sfxDef->field_1_program | (sLastLoadedSoundBlockInfo_BB2E34[sfxDef->field_0_block_idx].field_8_vab_id << 8),
        sfxDef->field_2_note << 8,
        volLeft,
        volRight);

    if (!sSFXPitchVariationEnabled_560F58)
    {
        return 0;
    }

    if (pitch_min || pitch_max)
    {
        SFX_SetPitch_4CA510(sfxDef, midiHandle, Math_RandomRange_496AB0(pitch_min, pitch_max));
    }

    return midiHandle;
}

int CC SFX_Play_46FB10(unsigned __int8 sfxId, int leftVol, int rightVol, int scale)
{
    if (scale == 0x8000)
    {
        leftVol = 2 * leftVol / 3;
        rightVol = 2 * rightVol / 3;
    }

    return SFX_SfxDefinition_Play_4CA700(&sSfxEntries_55C2A0[sfxId], leftVol, rightVol, 0x7FFF, 0x7FFF);
}

int CC SFX_Play_46FBA0(unsigned __int8 sfxIdx, __int16 volume, int pitch, int scale)
{
    if (!volume)
    {
        volume = (char)sSfxEntries_55C2A0[sfxIdx].field_3_default_volume;
    }
    if (scale == 0x8000)
    {
        volume /= 3;
    }
    return SFX_SfxDefinition_Play_4CA420(&sSfxEntries_55C2A0[sfxIdx], volume, pitch, pitch);
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
    return SFX_SfxDefinition_Play_4CA420(&sSfxEntries_55C2A0[sfxIdx], volume, 0x7FFF, 0x7FFF);
}