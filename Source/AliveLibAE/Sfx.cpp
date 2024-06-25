#include "stdafx.h"
#include "Sfx.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "Map.hpp"
#include <assert.h>
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/FixedPoint.hpp"

static const relive::SfxDefinition sSligGameSpeakEntries[21] = {
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

s16 Calc_Slig_Sound_Direction(BaseAnimatedWithPhysicsGameObject* pObj, s16 defaultVol, const relive::SfxDefinition& pSfx, s16* pLeftVol, s16* pRightVol)
{
    if (defaultVol == 0)
    {
        defaultVol = pSfx.mDefaultVolume;
    }

    if (pObj)
    {
        FP yOff = {};
        if (pObj->Type() == ReliveTypes::eFlyingSlig)
        {
            yOff = FP_FromInteger(20); // 0xffec0000
        }

        const CameraPos dir = gMap.GetDirection(
            pObj->mCurrentLevel, pObj->mCurrentPath,
            pObj->mXPos, pObj->mYPos - yOff);

        if (pObj->GetSpriteScale() != FP_FromInteger(1))
        {
            // Background layer stuff isn't as loud
            defaultVol = (2 * defaultVol) / 3;
        }

        PSX_RECT camRect = {};
        gMap.Get_Camera_World_Rect(dir, &camRect);

        const s32 volScaler = defaultVol / 3;
        switch (dir)
        {
            case CameraPos::eCamCurrent_0:
                *pRightVol = defaultVol;
                *pLeftVol = defaultVol;
                return 1;

            case CameraPos::eCamTop_1:
                *pRightVol = defaultVol - FP_GetExponent((FP_FromInteger(camRect.h) - pObj->mYPos) / FP_FromInteger(240) * FP_FromInteger(defaultVol - 1 * volScaler));
                *pLeftVol = *pRightVol;
                return 1;

            case CameraPos::eCamBottom_2:
            {
                const FP tmpVol = (pObj->mYPos - FP_FromInteger(camRect.y)) / FP_FromInteger(240);
                defaultVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol - 1 * volScaler));
                *pRightVol = defaultVol;
                *pLeftVol = defaultVol;
            }
                return 1;

            case CameraPos::eCamLeft_3:
            {
                const FP tmpVol = (FP_FromInteger(camRect.w) - pObj->mXPos) / FP_FromInteger(368);
                *pLeftVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol - 1 * volScaler));
                *pRightVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol));
            }
                return 1;

            case CameraPos::eCamRight_4:
            {
                const FP tmpVol = (pObj->mXPos - FP_FromInteger(camRect.x)) / FP_FromInteger(368);
                *pLeftVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol));
                *pRightVol = defaultVol - FP_GetExponent(tmpVol * FP_FromInteger(defaultVol - 1 * volScaler));
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

void Slig_GameSpeak_SFX(SligSpeak effectId, s16 defaultVol, s16 pitch_min, BaseAnimatedWithPhysicsGameObject* pObj)
{
    const s32 idx = static_cast<s32>(effectId);
    assert(idx < ALIVE_COUNTOF(sSligGameSpeakEntries));
    const relive::SfxDefinition& pEffect = sSligGameSpeakEntries[idx];

    s16 volLeft = 0;
    s16 volRight = 0;
    if (Calc_Slig_Sound_Direction(pObj, defaultVol, pEffect, &volLeft, &volRight))
    {
        SFX_SfxDefinition_Play_Stereo(pEffect, volLeft, volRight, pitch_min, pitch_min);
    }
}
