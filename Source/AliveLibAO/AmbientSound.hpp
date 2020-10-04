#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

struct Path_TLV;
enum class CameraPos : __int16;

EXPORT void CC SND_Reset_Ambiance_4765E0();

EXPORT void CC Start_Sounds_for_TLV_476640(CameraPos direction, Path_TLV* pTlv);

EXPORT void CC SND_Init_Ambiance_4765C0();

END_NS_AO
