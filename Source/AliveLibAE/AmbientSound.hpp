#pragma once

#include "../relive_lib/data_conversion/relive_tlvs.hpp"

void SND_Init_Ambiance();
void SND_Reset_Ambiance();

void Start_Sounds_For_Objects_In_Near_Cameras();

enum class CameraPos : s16;
struct Path_TLV;
void Start_Sounds_for_TLV(CameraPos direction, relive::Path_TLV* pTlv);
void Start_Slig_sounds(CameraPos direction, s8 kZero);
