#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"

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


enum class CameraPos : __int16;
class BaseAnimatedWithPhysicsGameObject;

EXPORT int CC SFX_SfxDefinition_Play_4CA700(const SfxDefinition* sfxDef, __int16 volLeft, __int16 volRight, __int16 pitch_min, __int16 pitch_max);
EXPORT int CC SFX_SfxDefinition_Play_4CA420(const SfxDefinition* sfxDef, __int16 volume, __int16 pitch_min, __int16 pitch_max);
EXPORT int CC SFX_Play_46FB10(unsigned __int8 sfxId, int leftVol, int rightVol, FP scale = FP_FromInteger(1));
EXPORT int CC SFX_Play_46FBA0(unsigned __int8 sfxIdx, __int16 volume, int pitch, FP scale = FP_FromInteger(1));
EXPORT int CC SFX_Play_46FA90(unsigned __int8 sfxIdx, __int16 volume, FP scale = FP_FromInteger(1));
EXPORT int CC SFX_Play_46FC20(unsigned __int8 sfxId, __int16 volume, CameraPos direction, FP scale = FP_FromInteger(1));
EXPORT int CC SND_4CA5D0(int a1, int a2, int a3, __int16 vol, __int16 min, __int16 max);

enum class SligSpeak : char
{
    None        = -1,
    Hi_0        =  0,
    HereBoy_1   =  1,
    GetHim_2    =  2,
    Laugh_3     =  3,
    Stay_4      =  4,
    Bullshit_5  =  5,
    LookOut_6   =  6,
    Bullshit2_7 =  7,
    Freeze_8    =  8,
    What_9      =  9,
    Help_10     =  10,
    Blurg_11    =  11,
    GotYa_12    =  12,
    Ouch1_13    =  13,
    Ouch2_14    =  14,
};

EXPORT void CC Sfx_Slig_GameSpeak_4C04F0(SligSpeak effectId, __int16 defaultVol, __int16 pitch_min, BaseAnimatedWithPhysicsGameObject* pObj);
EXPORT signed __int16 CC Calc_Slig_Sound_Direction_4C01B0(BaseAnimatedWithPhysicsGameObject* pObj, __int16 defaultVol, const SfxDefinition* pSfx, __int16* pLeftVol, __int16* pRightVol);
