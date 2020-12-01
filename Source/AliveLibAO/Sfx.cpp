#include "stdafx_ao.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Sfx.hpp"
#include "Function.hpp"
#include "Midi.hpp"
#include "Map.hpp"

namespace AO {

const SfxDefinition sSfxEntries_4CCA38[] =
{
    { 0, 0, 36, 75, 0, 0, 0 },
    { 0, 0, 66, 75, 0, 0, 0 },
    { 0, 0, 67, 75, 0, 0, 0 },
    { 0, 0, 72, 65, 0, 0, 0 },
    { 0, 0, 73, 65, 0, 0, 0 },
    { 0, 1, 57, 40, 0, 0, 0 },
    { 0, 2, 60, 127, -1, 1, 0 },
    { 0, 2, 64, 55, -64, 64, 0 },
    { 0, 4, 36, 127, 0, 0, 0 },
    { 0, 6, 53, 127, 0, 0, 0 },
    { 0, 69, 47, 127, -1, 1, 0 },
    { 0, 69, 60, 127, -1, 1, 0 },
    { 0, 6, 60, 127, 0, 0, 0 },
    { 0, 6, 48, 90, 0, 0, 0 },
    { 0, 7, 59, 70, -127, 127, 0 },
    { 0, 7, 60, 70, -127, 127, 0 },
    { 0, 7, 64, 70, 0, 0, 0 },
    { 0, 7, 64, 70, -255, -255, 0 },
    { 0, 7, 64, 70, -127, -127, 0 },
    { 0, 7, 64, 70, 1, 1, 0 },
    { 0, 8, 62, 90, 0, 0, 0 },
    { 0, 8, 60, 90, 0, 0, 0 },
    { 0, 8, 60, 70, 1, 1, 0 },
    { 0, 8, 60, 90, -2, -2, 0 },
    { 0, 8, 57, 90, -1, 1, 0 },
    { 0, 8, 58, 90, -1, 1, 0 },
    { 0, 8, 61, 60, -1, 1, 0 },
    { 0, 9, 36, 50, -2, -2, 0 },
    { 0, 9, 37, 50, -2, -2, 0 },
    { 0, 9, 38, 60, -1, 1, 0 },
    { 0, 9, 39, 90, -1, 1, 0 },
    { 0, 9, 40, 60, -1, 1, 0 },
    { 0, 9, 41, 60, -1, 1, 0 },
    { 0, 9, 60, 115, 0, 0, 0 },
    { 0, 9, 61, 60, 0, 0, 0 },
    { 0, 10, 60, 80, -1, 1, 0 },
    { 0, 10, 61, 55, -1, 1, 0 },
    { 0, 11, 60, 60, -1, 1, 0 },
    { 0, 11, 63, 60, -1, 1, 0 },
    { 0, 12, 44, 90, -2, 2, 0 },
    { 0, 13, 60, 65, -1, 1, 0 },
    { 0, 14, 61, 20, -127, 127, 0 },
    { 0, 14, 62, 20, -127, 127, 0 },
    { 0, 15, 60, 70, -1, 1, 0 },
    { 0, 15, 36, 70, -1, 1, 0 },
    { 0, 16, 48, 70, -900, -900, 0 },
    { 0, 17, 36, 127, -1, 1, 0 },
    { 0, 17, 37, 45, -1, 1, 0 },
    { 0, 17, 59, 30, -1, 1, 0 },
    { 0, 19, 48, 70, -1, 1, 0 },
    { 0, 20, 48, 90, 0, 0, 0 },
    { 0, 20, 43, 90, 0, 0, 0 },
    { 0, 20, 36, 90, 0, 0, 0 },
    { 0, 21, 72, 127, 0, 0, 0 },
    { 0, 24, 60, 90, 0, 0, 0 },
    { 0, 24, 61, 90, 0, 0, 0 },
    { 0, 24, 62, 55, 0, 0, 0 },
    { 0, 25, 36, 127, 0, 0, 0 },
    { 0, 25, 37, 70, 0, 0, 0 },
    { 0, 26, 40, 46, 0, 0, 0 },
    { 0, 27, 60, 50, -127, 127, 0 },
    { 0, 28, 53, 50, 0, 0, 0 },
    { 0, 29, 48, 50, 0, 0, 0 },
    { 0, 29, 49, 20, 0, 0, 0 },
    { 0, 30, 60, 50, 0, 0, 0 },
    { 0, 31, 60, 50, -800, -800, 0 },
    { 0, 32, 61, 65, -1000, -1000, 0 },
    { 0, 33, 36, 45, -1860, -1860, 0 },
    { 0, 34, 60, 80, -1535, -1535, 0 },
    { 0, 34, 61, 60, 0, 0, 0 },
    { 0, 34, 62, 127, 0, 0, 0 },
    { 0, 35, 36, 90, 0, 0, 0 },
    { 0, 35, 37, 90, 0, 0, 0 },
    { 0, 36, 60, 127, 0, 0, 0 },
    { 0, 36, 61, 35, -1600, -1600, 0 },
    { 0, 38, 60, 60, 0, 0, 0 },
    { 0, 39, 36, 90, 0, 0, 0 },
    { 0, 39, 56, 90, 0, 0, 0 },
    { 0, 39, 65, 90, 0, 0, 0 },
    { 0, 40, 60, 55, -255, 127, 0 },
    { 0, 40, 61, 55, -255, 127, 0 },
    { 0, 42, 60, 60, 0, 0, 0 },
    { 0, 42, 64, 60, 0, 0, 0 },
    { 0, 44, 60, 60, 0, 0, 0 },
    { 0, 44, 61, 60, 0, 0, 0 },
    { 0, 44, 62, 45, -127, 127, 0 },
    { 0, 44, 63, 45, -127, 127, 0 },
    { 0, 46, 60, 13, 0, 0, 0 },
    { 0, 48, 36, 40, 0, 0, 0 },
    { 0, 48, 42, 40, 0, 0, 0 },
    { 0, 48, 62, 40, 0, 0, 0 },
    { 0, 48, 60, 40, 0, 0, 0 },
    { 0, 48, 61, 100, 0, 0, 0 },
    { 0, 49, 60, 90, -800, -800, 0 },
    { 0, 49, 60, 90, -400, -400, 0 },
    { 0, 49, 60, 60, 0, 0, 0 },
    { 0, 49, 61, 45, 0, 0, 0 },
    { 0, 49, 62, 45, 0, 0, 0 },
    { 0, 50, 60, 60, 0, 0, 0 },
    { 0, 50, 36, 60, 0, 0, 0 },
    { 0, 50, 48, 60, 0, 0, 0 },
    { 0, 67, 60, 60, 0, 0, 0 },
    { 0, 67, 61, 60, 0, 0, 0 },
    { 0, 67, 62, 60, 0, 0, 0 },
    { 0, 67, 63, 60, 0, 0, 0 },
    { 0, 67, 64, 60, 0, 0, 0 },
    { 0, 67, 65, 60, 0, 0, 0 },
    { 0, 80, 36, 90, 0, 0, 0 },
    { 0, 39, 60, 127, 0, 0, 0 },
    { 0, 39, 68, 127, 0, 0, 0 },
    { 0, 63, 38, 90, -1000, -1000, 0 },
    { 0, 62, 62, 90, -1000, -1000, 0 },
    { 0, 64, 39, 90, -1000, -1000, 0 }
};

int CC SFX_Play_43AE60(unsigned __int8 sfxIdx, int volume, int pitch, BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (volume > 0)
    {
        return SFX_SfxDefinition_Play_4770F0(&sSfxEntries_4CCA38[sfxIdx], volume, pitch, pitch);
    }
    if (pObj && pObj->field_BC_sprite_scale == FP_FromDouble(0.5))
    {
        volume = 2 * sSfxEntries_4CCA38[sfxIdx].field_C_default_volume / 3;
    }
    return SFX_SfxDefinition_Play_4770F0(&sSfxEntries_4CCA38[sfxIdx], volume, pitch, pitch);
}

int CC SFX_Play_43AD70(unsigned __int8 sfxIdx, int volume, BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (volume > 0)
    {
        return SFX_SfxDefinition_Play_4770F0(&sSfxEntries_4CCA38[sfxIdx], volume, 0x7FFF, 0x7FFF);
    }
    if (pObj && pObj->field_BC_sprite_scale == FP_FromDouble(0.5))
    {
        volume = 2 * sSfxEntries_4CCA38[sfxIdx].field_C_default_volume / 3;
    }
    return SFX_SfxDefinition_Play_4770F0(&sSfxEntries_4CCA38[sfxIdx], volume, 0x7FFF, 0x7FFF);
}

int CC SFX_Play_43ADE0(unsigned __int8 sfxId, int leftVol, int rightVol, BaseAnimatedWithPhysicsGameObject* pObj)
{
    short left = 0;
    short right = 0;

    if (pObj && pObj->field_BC_sprite_scale == FP_FromDouble(0.5))
    {
        left = static_cast<short>(2 * leftVol / 3);
        right = static_cast<short>(2 * rightVol / 3);
    }
    else
    {
        right = static_cast<short>(rightVol);
        left = static_cast<short>(leftVol);
    }
    return SFX_SfxDefinition_Play_477330(&sSfxEntries_4CCA38[sfxId], left, right, 0x7FFF, 0x7FFF);
}

int CC SFX_Play_43AED0(unsigned __int8 sfxId, signed int volume, CameraPos direction)
{
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
        {
            return SFX_SfxDefinition_Play_4770F0(&sSfxEntries_4CCA38[sfxId], volume, 0x7FFF, 0x7FFF);
        }
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            return SFX_SfxDefinition_Play_4770F0(&sSfxEntries_4CCA38[sfxId], 2 * volume / 3, 0x7FFF, 0x7FFF);
        }
        case CameraPos::eCamLeft_3:
        {
            return SFX_SfxDefinition_Play_477330(
                &sSfxEntries_4CCA38[sfxId],
                static_cast<short>(2 * volume / 3),
                static_cast<short>(2 * volume / 9),
                0x7FFF,
                0x7FFF
            );
        }
        case CameraPos::eCamRight_4:
        {
            return SFX_SfxDefinition_Play_477330(
                &sSfxEntries_4CCA38[sfxId],
                static_cast<short>(2 * volume / 9),
                static_cast<short>(2 * volume / 3),
                0x7FFF,
                0x7FFF
            );
        }
        default:
        {
            return 0;
        }
    }
}

}
