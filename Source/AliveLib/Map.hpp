#pragma once

#include "FunctionFwd.hpp"
#include "Function.hpp"

void Map_ForceLink();

class Map
{
public:

    unsigned __int16 sCurrentLevelId_5C3030;
    unsigned __int16 sCurrentPathId_5C3032;
    unsigned __int16 sCurrentCamId_5C3034;

    __int16 field_6;
    __int16 field_8;

    unsigned __int16 field_A_5C303A_levelId;
    __int16 field_C_5C303C_pathId;
    __int16 field_E_cameraId;

    __int16 field_10[7];

    __int16 field_1E_5C304E;
    __int16 field_20_5C3050;
    __int16 field_22_5C3052;

    int field_24_5C3054;
    int field_28_5C3058;
    int field_2C_5C305C;

    EXPORT void sub_480B80();
    EXPORT int sub_4803F0(__int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7);
    EXPORT void sub_4804E0();
};
ALIVE_ASSERT_SIZEOF(Map, 0x30);

ALIVE_VAR_EXTERN(Map, gMap_5C3030);