#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

enum class CameraPos : s16;

namespace AO {

struct Path_TLV;

void SND_Reset_Ambiance();

void Start_Sounds_for_TLV(CameraPos direction, relive::Path_TLV* pTlv);

void SND_Init_Ambiance();

void Start_Slig_sounds(CameraPos direction, s8 kZero);

void Stop_slig_sounds(CameraPos direction, s8 kZero);

class ScopedSeq;
struct Sound_Ambiance final
{
    ScopedSeq* field_8_pScopedSeq;
};
ALIVE_ASSERT_SIZEOF(Sound_Ambiance, 0xC);

struct Sound_Ambiance_Array final
{
    Sound_Ambiance mArray[7];
};

} // namespace AO
