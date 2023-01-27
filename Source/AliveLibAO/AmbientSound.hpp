#pragma once

#include "../relive_lib/Function.hpp"

namespace relive
{
    class Path_TLV;
}

enum class CameraPos : s16;

namespace AO {

void SND_Reset_Ambiance();

void Start_Sounds_for_TLV(CameraPos direction, relive::Path_TLV* pTlv);

void SND_Init_Ambiance();

void Start_Slig_sounds(CameraPos direction, s8 kZero);

void Stop_slig_sounds(CameraPos direction, s8 kZero);

class ScopedSeq;
struct Sound_Ambiance final
{
    ScopedSeq* mScopedSeq;
};

struct Sound_Ambiance_Array final
{
    Sound_Ambiance mArray[7];
};

} // namespace AO
