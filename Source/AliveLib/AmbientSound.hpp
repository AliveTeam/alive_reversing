#pragma once

EXPORT void CC SND_Init_Ambiance_4CB480();
EXPORT void CC SND_Reset_Ambiance_4CB4B0();

EXPORT void CC Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();

enum class CameraPos : __int16;
struct Path_TLV;
EXPORT void CC Start_Sounds_for_TLV_4CB530(CameraPos direction, Path_TLV* pTlv);
EXPORT void CC Start_Slig_sounds_4CB980(CameraPos direction, char kZero);
