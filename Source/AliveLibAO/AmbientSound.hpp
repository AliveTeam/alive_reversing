#pragma once

#include "FunctionFwd.hpp"

namespace AO {

struct Path_TLV;
enum class CameraPos : __int16;

EXPORT void CC SND_Reset_Ambiance_4765E0();

EXPORT void CC Start_Sounds_for_TLV_476640(CameraPos direction, Path_TLV* pTlv);

EXPORT void CC SND_Init_Ambiance_4765C0();

class ScopedSeq;
struct Sound_Ambiance
{
    int field_0;
    int field_4;
    ScopedSeq* field_8_pScopedSeq;
};
ALIVE_ASSERT_SIZEOF(Sound_Ambiance, 0xC);

struct Sound_Ambiance_Array
{
    Sound_Ambiance mArray[7];
};

ALIVE_VAR_EXTERN(Sound_Ambiance_Array, sTopBottomAmbiance_9F11D0);
ALIVE_VAR_EXTERN(Sound_Ambiance_Array, sRightAmbiance_9F1228);
ALIVE_VAR_EXTERN(Sound_Ambiance_Array, sLeftAmbiance_9F1280);

}
