#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"

namespace AO {

struct Path_TLV;
enum class CameraPos : s16;

EXPORT void CC SND_Reset_Ambiance();

EXPORT void CC Start_Sounds_for_TLV(CameraPos direction, Path_TLV* pTlv);

EXPORT void CC SND_Init_Ambiance();

EXPORT void CC Start_Slig_sounds(CameraPos direction, s8 kZero);

EXPORT void CC Stop_slig_sounds(CameraPos direction, s8 kZero);

class ScopedSeq;
struct Sound_Ambiance final
{
    s32 field_0_padding;
    s32 field_4_padding;
    ScopedSeq* field_8_pScopedSeq;
};
ALIVE_ASSERT_SIZEOF(Sound_Ambiance, 0xC);

struct Sound_Ambiance_Array final
{
    Sound_Ambiance mArray[7];
};

} // namespace AO
