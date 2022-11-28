#pragma once

#include "../relive_lib/Sfx.hpp"

class BaseGameObject;
class BaseAnimatedWithPhysicsGameObject;
enum class CameraPos : s16;

namespace AO {


class BaseAliveGameObject;



s32 SFX_Play_Stereo(relive::SoundEffects sfxId, s32 leftVol, s32 rightVol, BaseAnimatedWithPhysicsGameObject* pObj);
s32 SFX_Play_Camera(relive::SoundEffects sfxId, s32 volume, CameraPos direction);

} // namespace AO
