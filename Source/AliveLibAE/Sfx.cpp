#include "stdafx.h"
#include "Sfx.hpp"
#include "Function.hpp"
#include "DebugHelpers.hpp"
#include "Sound/Midi.hpp"
#include "PathData.hpp"
#include <assert.h>

const SfxDefinition sSfxEntries_55C2A0[] = {
    {0u, 0u, 66u, 75u, 0, 0},
    {0u, 0u, 67u, 75u, 0, 0},
    {0u, 0u, 72u, 55u, 0, 0},
    {0u, 0u, 73u, 55u, 0, 0},
    {0u, 1u, 57u, 40u, 0, 0},
    {0u, 39u, 60u, 127u, -1, 1},
    {0u, 2u, 64u, 55u, -64, 64},
    {0u, 7u, 36u, 127u, 0, 0},
    {0u, 6u, 53u, 127u, 0, 0},
    {0u, 69u, 47u, 127u, -1, 1},
    {0u, 69u, 60u, 127u, -1, 1},
    {0u, 6u, 60u, 127u, 0, 0},
    {0u, 6u, 48u, 90u, 0, 0},
    {0u, 7u, 64u, 70u, 0, 0},
    {0u, 7u, 64u, 70u, -255, -255},
    {0u, 7u, 64u, 70u, -127, -127},
    {0u, 9u, 62u, 90u, 0, 0},
    {0u, 9u, 62u, 90u, 0, 0},
    {0u, 9u, 62u, 70u, 1, 1},
    {0u, 9u, 62u, 90u, -2, -2},
    {0u, 9u, 58u, 90u, -1, 1},
    {0u, 9u, 57u, 90u, -1, 1},
    {0u, 9u, 36u, 45u, -2, -2},
    {0u, 9u, 37u, 50u, -2, -2},
    {0u, 9u, 38u, 60u, -1, 1},
    {0u, 9u, 39u, 90u, -1, 1},
    {0u, 9u, 40u, 60u, -1, 1},
    {0u, 9u, 41u, 60u, -1, 1},
    {0u, 9u, 60u, 115u, 0, 0},
    {0u, 9u, 61u, 60u, 0, 0},
    {0u, 10u, 60u, 80u, -1, 1},
    {0u, 10u, 61u, 55u, -1, 1},
    {0u, 3u, 60u, 60u, -1, 1},
    {0u, 4u, 63u, 60u, -1, 1},
    {0u, 12u, 44u, 90u, -2, 2},
    {0u, 13u, 60u, 65u, -1, 1},
    {0u, 4u, 58u, 70u, -1, 1},
    {0u, 4u, 59u, 70u, -1, 1},
    {0u, 0u, 44u, 70u, -900, -900},
    {0u, 2u, 38u, 127u, -1, 1},
    {0u, 2u, 39u, 45u, -1, 1},
    {0u, 2u, 49u, 30u, -1, 1},
    {0u, 10u, 48u, 70u, -1, 1},
    {0u, 10u, 48u, 70u, -127, -125},
    {0u, 20u, 48u, 90u, 0, 0},
    {0u, 20u, 43u, 90u, 0, 0},
    {0u, 20u, 36u, 90u, 0, 0},
    {0u, 0u, 74u, 127u, 0, 0},
    {0u, 0u, 63u, 55u, 0, 0},
    {0u, 2u, 36u, 127u, 0, 0},
    {0u, 2u, 37u, 70u, 0, 0},
    {0u, 26u, 40u, 46u, 0, 0},
    {0u, 1u, 53u, 25u, 0, 0},
    {0u, 29u, 48u, 50u, 0, 0},
    {0u, 29u, 49u, 20u, 0, 0},
    {0u, 10u, 59u, 90u, 0, 0},
    {0u, 10u, 62u, 50u, -800, -800},
    {0u, 10u, 64u, 65u, -1000, -1000},
    {0u, 10u, 36u, 45u, -1860, -1860},
    {0u, 34u, 61u, 60u, 0, 0},
    {0u, 11u, 36u, 90u, 0, 0},
    {0u, 11u, 37u, 90u, 0, 0},
    {0u, 11u, 60u, 127u, 0, 0},
    {0u, 10u, 63u, 60u, 0, 0},
    {0u, 4u, 65u, 90u, 0, 0},
    {0u, 5u, 58u, 55u, -255, 127},
    {0u, 5u, 59u, 55u, -255, 127},
    {0u, 0u, 84u, 127u, 0, 0},
    {0u, 0u, 64u, 60u, 0, 0},
    {0u, 44u, 60u, 60u, 0, 0},
    {0u, 44u, 61u, 60u, 0, 0},
    {0u, 3u, 62u, 45u, -127, 127},
    {0u, 3u, 63u, 45u, -127, 127},
    {0u, 0u, 70u, 6u, 0, 0},
    {0u, 48u, 36u, 40u, 0, 0},
    {0u, 48u, 42u, 40u, 0, 0},
    {0u, 10u, 65u, 90u, -800, -800},
    {0u, 10u, 65u, 90u, -400, -400},
    {0u, 10u, 65u, 60u, 0, 0},
    {0u, 10u, 66u, 45u, 0, 0},
    {0u, 10u, 66u, 45u, 0, 0},
    {0u, 18u, 60u, 60u, 0, 0},
    {0u, 18u, 38u, 60u, 0, 0},
    {0u, 18u, 48u, 60u, 0, 0},
    {0u, 10u, 35u, 90u, 0, 0},
    {0u, 4u, 60u, 127u, 0, 0},
    {0u, 4u, 68u, 127u, 0, 0},
    {0u, 25u, 38u, 90u, -1000, -1000},
    {0u, 25u, 39u, 90u, -1000, -1000},
    {0u, 6u, 60u, 80u, 600, 700},
    {0u, 6u, 61u, 20u, 2667, 3175},
    {0u, 3u, 48u, 100u, -50, 50},
    {0u, 10u, 64u, 65u, -1000, -1000},
    {0u, 1u, 51u, 127u, 0, 0},
    {0u, 7u, 72u, 40u, 0, 0},
    {0u, 7u, 73u, 40u, 0, 0},
    {0u, 7u, 74u, 40u, 0, 0},
    {0u, 10u, 40u, 80u, 800, 800},
    {0u, 10u, 41u, 127u, 0, 0},
    {0u, 10u, 42u, 90u, 0, 0},
    {0u, 7u, 60u, 75u, 0, 0},
    {0u, 7u, 61u, 100u, 0, 0},
    {0u, 1u, 58u, 127u, -2500, -2800},
    {0u, 5u, 72u, 127u, -500, 500},
    {0u, 5u, 73u, 127u, -500, 500},
    {0u, 6u, 37u, 60u, -200, 200},
    {0u, 6u, 36u, 127u, 0, 0},
    {0u, 19u, 36u, 127u, 0, 0},
    {0u, 19u, 60u, 127u, 0, 0},
    {0u, 9u, 62u, 60u, 3048, 3048},
    {0u, 9u, 62u, 60u, 3048, 3048},
    {0u, 9u, 62u, 60u, 3048, 3048},
    {0u, 9u, 62u, 60u, 3048, 3048},
    {0u, 9u, 62u, 60u, 3175, 3175},
    {0u, 9u, 62u, 60u, 3302, 3302},
    {0u, 9u, 62u, 60u, 3429, 3429},
    {0u, 1u, 51u, 100u, 0, 0},
    {0u, 1u, 52u, 100u, 0, 0},
    {0u, 1u, 57u, 60u, -1800, -1800},
    {0u, 1u, 86u, 120u, 0, 0},
    {0u, 6u, 67u, 127u, 0, 0},
    {0u, 6u, 64u, 127u, 0, 0}};


s32 CC SFX_Play_46FB10(SoundEffect sfxId, s32 leftVol, s32 rightVol, FP scale)
{
    if (scale == FP_FromDouble(0.5))
    {
        leftVol = 2 * leftVol / 3;
        rightVol = 2 * rightVol / 3;
    }

    return SFX_SfxDefinition_Play_4CA700(&sSfxEntries_55C2A0[sfxId], static_cast<s16>(leftVol), static_cast<s16>(rightVol), 0x7FFF, 0x7FFF, 64); // TODO
}

s32 CC SFX_Play_46FBA0(SoundEffect sfxIdx, s16 volume, s32 pitch, FP scale)
{
    if (!volume)
    {
        volume = (s8) sSfxEntries_55C2A0[sfxIdx].field_3_default_volume;
    }
    if (scale == FP_FromDouble(0.5))
    {
        volume = static_cast<s16>(volume / 1.5);
    }
    return SFX_SfxDefinition_Play_4CA420(&sSfxEntries_55C2A0[sfxIdx], volume, static_cast<s16>(pitch), static_cast<s16>(pitch));
}

s32 CC SFX_Play_46FA90(SoundEffect sfxIdx, s16 volume, FP scale)
{
    if (!volume)
    {
        volume = sSfxEntries_55C2A0[sfxIdx].field_3_default_volume;
    }
    if (scale == FP_FromDouble(0.5))
    {
        volume /= 3;
    }
    return SFX_SfxDefinition_Play_4CA420(&sSfxEntries_55C2A0[sfxIdx], volume, 0x7FFF, 0x7FFF);
}

s32 CC SFX_Play_46FC20(SoundEffect sfxId, s16 volume, CameraPos direction, FP scale)
{
    if (!volume)
    {
        volume = sSfxEntries_55C2A0[sfxId].field_3_default_volume;
    }

    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
            return SFX_Play_46FA90(sfxId, volume, scale);
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
            return SFX_Play_46FA90(sfxId, 2 * volume / 3, scale);
        case CameraPos::eCamLeft_3:
            return SFX_Play_46FB10(sfxId, 2 * volume / 3, 2 * volume / 9, scale);
        case CameraPos::eCamRight_4:
            return SFX_Play_46FB10(sfxId, 2 * volume / 9, 2 * volume / 3, scale);
        default:
            return 0;
    }
}

const SfxDefinition sSligGameSpeakEntries_560868[21] = {
    {0u, 25u, 60u, 127u, 0, 0},
    {0u, 25u, 62u, 127u, 0, 0},
    {0u, 25u, 61u, 115u, 0, 0},
    {0u, 25u, 38u, 120u, 0, 0},
    {0u, 25u, 63u, 127u, 0, 0},
    {0u, 25u, 66u, 127u, 0, 0},
    {0u, 25u, 37u, 127u, 0, 0},
    {0u, 25u, 67u, 127u, 0, 0},
    {0u, 25u, 57u, 127u, 0, 0},
    {0u, 25u, 58u, 127u, 0, 0},
    {0u, 25u, 59u, 127u, 0, 0},
    {0u, 25u, 39u, 127u, 0, 0},
    {0u, 25u, 64u, 127u, 0, 0},
    {0u, 25u, 65u, 127u, 0, 0},
    {0u, 25u, 68u, 127u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 0, 0}};

EXPORT s16 CC Calc_Slig_Sound_Direction_4C01B0(BaseAnimatedWithPhysicsGameObject* pObj, s16 defaultVol, const SfxDefinition* pSfx, s16* pLeftVol, s16* pRightVol, s32* pan)
{
    if (defaultVol == 0)
    {
        defaultVol = pSfx->field_3_default_volume;
    }

    if (pObj)
    {
        FP yOff = {};
        if (pObj->Type() == AETypes::eFlyingSlig_54)
        {
            yOff = FP_FromInteger(20); // 0xffec0000
        }

        const CameraPos dir = gMap_5C3030.GetDirection_4811A0(
            pObj->field_C2_lvl_number, pObj->field_C0_path_number,
            pObj->field_B8_xpos, pObj->field_BC_ypos - yOff);

        if (pObj->field_CC_sprite_scale != FP_FromInteger(1))
        {
            // Background layer stuff isn't as loud
            defaultVol = (2 * defaultVol) / 3;
        }

        PSX_RECT camRect = {};
        gMap_5C3030.Get_Camera_World_Rect_481410(dir, &camRect);
        *pan = 64;

        // TODO - AE sound
        const s32 volScaler = defaultVol / 3;
        switch (dir)
        {
            case CameraPos::eCamCurrent_0:
                *pRightVol = defaultVol;
                *pLeftVol = defaultVol;
                return 1;

            case CameraPos::eCamTop_1:
                *pRightVol = defaultVol - FP_GetExponent((FP_FromInteger(camRect.h) - pObj->field_BC_ypos) / FP_FromInteger(240) * FP_FromInteger(defaultVol - 1 * volScaler));
                *pLeftVol = *pRightVol;
                return 1;

            case CameraPos::eCamBottom_2:
            {
                const FP tmpVol = (pObj->field_BC_ypos - FP_FromInteger(camRect.y)) / FP_FromInteger(240);
                defaultVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol - 1 * volScaler));
                *pRightVol = defaultVol;
                *pLeftVol = defaultVol;
            }
                return 1;

            case CameraPos::eCamLeft_3:
            {
                FP tmpVol = (FP_FromInteger(camRect.w) - pObj->field_B8_xpos) / FP_FromInteger(368);
                *pLeftVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol - 1 * volScaler));
                *pRightVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol));

                double check = FP_GetDouble(tmpVol);
                double scale = 2 * (check);
                *pan = ((s32) (64 - (scale * 64)));
                *pan = *pan > 64 ? 64 : *pan;
            }
                return 1;

            case CameraPos::eCamRight_4:
            {
                FP tmpVol = (pObj->field_B8_xpos - FP_FromInteger(camRect.x)) / FP_FromInteger(368);
                *pLeftVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol));
                *pRightVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol - 1 * volScaler));

                double check = FP_GetDouble(tmpVol);
                double scale = 2 * (check);
                *pan = ((s32) (64 + (scale * 64)));
                *pan = *pan < 64 ? 64 : *pan;
            }
                return 1;

            default:
                return 0;
        }
    }
    else
    {
        *pRightVol = defaultVol;
        *pLeftVol = defaultVol;
        return 1;
    }
}

void CC Slig_GameSpeak_SFX_4C04F0(SligSpeak effectId, s16 defaultVol, s16 pitch_min, BaseAnimatedWithPhysicsGameObject* pObj)
{
    const s32 idx = static_cast<s32>(effectId);
    assert(idx < ALIVE_COUNTOF(sSligGameSpeakEntries_560868));
    const SfxDefinition* pEffect = &sSligGameSpeakEntries_560868[idx];

    s16 volLeft = 0;
    s16 volRight = 0;
    s32 pan = 64;
    if (Calc_Slig_Sound_Direction_4C01B0(pObj, defaultVol, pEffect, &volLeft, &volRight, &pan))
    {
        SFX_SfxDefinition_Play_4CA700(pEffect, volLeft, volRight, pitch_min, pitch_min, pan);
    }
}
