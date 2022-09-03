#include "stdafx_ao.h"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Sfx.hpp"
#include "Function.hpp"
#include "Midi.hpp"
#include "Map.hpp"

namespace AO {

s32 SFX_Play_Pitch(relive::SoundEffects sfxIdx, s32 volume, s32 pitch, BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (volume > 0)
    {
        return SFX_SfxDefinition_Play_4770F0(relive::GetSfx(sfxIdx), volume, pitch, pitch);
    }
    if (pObj && pObj->mSpriteScale == FP_FromDouble(0.5))
    {
        volume = 2 * relive::GetSfx(sfxIdx).field_C_default_volume / 3;
    }
    return SFX_SfxDefinition_Play_4770F0(relive::GetSfx(sfxIdx), volume, pitch, pitch);
}

s32 SfxPlayMono(relive::SoundEffects sfxIdx, s32 volume, BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (volume > 0)
    {
        return SFX_SfxDefinition_Play_4770F0(relive::GetSfx(sfxIdx), volume, 0x7FFF, 0x7FFF);
    }
    if (pObj && pObj->mSpriteScale == FP_FromDouble(0.5))
    {
        volume = 2 * relive::GetSfx(sfxIdx).field_C_default_volume / 3;
    }
    return SFX_SfxDefinition_Play_4770F0(relive::GetSfx(sfxIdx), volume, 0x7FFF, 0x7FFF);
}

s32 SFX_Play_Stereo(relive::SoundEffects sfxId, s32 leftVol, s32 rightVol, BaseAnimatedWithPhysicsGameObject* pObj)
{
    s16 left = 0;
    s16 right = 0;

    if (pObj && pObj->mSpriteScale == FP_FromDouble(0.5))
    {
        left = static_cast<s16>(2 * leftVol / 3);
        right = static_cast<s16>(2 * rightVol / 3);
    }
    else
    {
        right = static_cast<s16>(rightVol);
        left = static_cast<s16>(leftVol);
    }
    return SFX_SfxDefinition_Play_477330(relive::GetSfx(sfxId), left, right, 0x7FFF, 0x7FFF);
}

s32 SFX_Play_Camera(relive::SoundEffects sfxId, s32 volume, CameraPos direction)
{
    switch (direction)
    {
        case CameraPos::eCamCurrent_0:
        {
            return SFX_SfxDefinition_Play_4770F0(relive::GetSfx(sfxId), volume, 0x7FFF, 0x7FFF);
        }
        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
        {
            return SFX_SfxDefinition_Play_4770F0(relive::GetSfx(sfxId), 2 * volume / 3, 0x7FFF, 0x7FFF);
        }
        case CameraPos::eCamLeft_3:
        {
            return SFX_SfxDefinition_Play_477330(
                relive::GetSfx(sfxId),
                static_cast<s16>(2 * volume / 3),
                static_cast<s16>(2 * volume / 9),
                0x7FFF,
                0x7FFF);
        }
        case CameraPos::eCamRight_4:
        {
            return SFX_SfxDefinition_Play_477330(
                relive::GetSfx(sfxId),
                static_cast<s16>(2 * volume / 9),
                static_cast<s16>(2 * volume / 3),
                0x7FFF,
                0x7FFF);
        }
        default:
        {
            return 0;
        }
    }
}

} // namespace AO
