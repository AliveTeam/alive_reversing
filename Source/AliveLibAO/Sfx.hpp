#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Sfx.hpp"

class BaseGameObject;

namespace AO {


class BaseAnimatedWithPhysicsGameObject;
class BaseAliveGameObject;

enum class CameraPos : s16;


s32 SFX_Play_Pitch(relive::SoundEffects sfxId, s32 volume, s32 pitch, BaseAnimatedWithPhysicsGameObject* pObj = nullptr);
s32 SfxPlayMono(relive::SoundEffects sfxId, s32 a2, BaseAnimatedWithPhysicsGameObject* a3 = nullptr);
s32 SFX_Play_Stereo(relive::SoundEffects sfxId, s32 leftVol, s32 rightVol, BaseAnimatedWithPhysicsGameObject* pObj);
s32 SFX_Play_Camera(relive::SoundEffects sfxId, s32 volume, CameraPos direction);

} // namespace AO
