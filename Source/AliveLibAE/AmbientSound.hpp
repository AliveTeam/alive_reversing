#pragma once

void SND_Init_Ambiance_4CB480();
void SND_Reset_Ambiance_4CB4B0();

void Start_Sounds_For_Objects_In_Near_Cameras_4CBB60();

enum class CameraPos : s16;
struct Path_TLV;
void Start_Sounds_for_TLV_4CB530(CameraPos direction, Path_TLV* pTlv);
void Start_Slig_sounds_4CB980(CameraPos direction, s8 kZero);
