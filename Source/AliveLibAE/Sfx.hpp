#pragma once

#include "FixedPoint.hpp"
#include "../relive_lib/Sfx.hpp"

enum class CameraPos : s16;
class BaseAnimatedWithPhysicsGameObject;

s32 SFX_Play_Stereo(relive::SoundEffects sfxId, s32 leftVol, s32 rightVol, FP scale = FP_FromInteger(1));
s32 SFX_Play_Camera(relive::SoundEffects sfxId, s16 volume, CameraPos direction, FP scale = FP_FromInteger(1));
s32 SND_MIDI(s32 program, s32 vabId, s32 note, s16 vol, s16 min, s16 max);

enum class SligSpeak : s8
{
    eNone = -1,
    eHi_0 = 0,
    eHereBoy_1 = 1,
    eGetHim_2 = 2,
    eLaugh_3 = 3,
    eStay_4 = 4,
    eBullshit_5 = 5,
    eLookOut_6 = 6,
    eBullshit2_7 = 7,
    eFreeze_8 = 8,
    eWhat_9 = 9,
    eHelp_10 = 10,
    eBlurgh_11 = 11,
    eGotYa_12 = 12,
    eOuch1_13 = 13,
    eOuch2_14 = 14,
};

void Slig_GameSpeak_SFX_4C04F0(SligSpeak effectId, s16 defaultVol, s16 pitch_min, BaseAnimatedWithPhysicsGameObject* pObj);
s16 Calc_Slig_Sound_Direction_4C01B0(BaseAnimatedWithPhysicsGameObject* pObj, s16 defaultVol, const relive::SfxDefinition& pSfx, s16* pLeftVol, s16* pRightVol);
