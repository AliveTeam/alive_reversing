#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class BaseAnimatedWithPhysicsGameObject;
class BaseAliveGameObject;
class BaseGameObject;

enum class CameraPos : __int16;

struct SfxDefinition
{
    int field_0_block_idx;
    int field_4_program;
    int field_8_note;
    __int16 field_C_default_volume;
    __int16 field_E_pitch_min;
    __int16 field_10_pitch_max;
    __int16 field_12_pad;
};
ALIVE_ASSERT_SIZEOF(SfxDefinition, 0x14);

EXPORT int CC SFX_Play_43AE60(unsigned __int8 sfxId, int volume, int pitch, BaseAnimatedWithPhysicsGameObject* pObj = nullptr);

EXPORT int CC SFX_Play_43AD70(unsigned __int8 sfxId, int a2, BaseGameObject* a3 = nullptr);

EXPORT int CC SFX_Play_43ADE0(unsigned __int8 sfxId, int leftVol, int rightVol, BaseAliveGameObject* pObj);

EXPORT int CC SFX_Play_43AED0(unsigned __int8 sfxId, signed int volume, CameraPos direction);

END_NS_AO
