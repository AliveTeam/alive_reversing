#include "stdafx.h"
#include "Map.hpp"
#include "PathData.hpp"
#include "Function.hpp"

void Map_ForceLink() { }

void Map::sub_480B80()
{
    NOT_IMPLEMENTED();
}

int Map::sub_4803F0(__int16 a2, __int16 a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7)
{
    NOT_IMPLEMENTED();
}

void Map::sub_4804E0()
{
    NOT_IMPLEMENTED();
}

void Map::GetCurrentCamCoords_480680(PSX_Point* pPoint)
{
    pPoint->field_0_x = field_D0_cam_x_idx * field_D4_ptr->field_A_grid_width;
    pPoint->field_2_y = field_D2_cam_y_idx * field_D4_ptr->field_C_grid_height;
}

void Map::sub_480D30(__int16 level, __int16 path, __int16 cam, __int16 a5, __int16 a6, __int16 a7)
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x5C3030, Map, gMap_5C3030, {});
