#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

struct Slurg_Step_Watch_Point
{
    WORD field_0_xPos;
    WORD field_2_yPos;
};
ALIVE_ASSERT_SIZEOF(Slurg_Step_Watch_Point, 0x4);

struct Slurg_Step_Watch_Points
{
    Slurg_Step_Watch_Point field_0_points[5];
};
ALIVE_ASSERT_SIZEOF(Slurg_Step_Watch_Points, 0x14);


class Slurg : public BaseAliveGameObject
{
public:
    EXPORT static void CC Clear_Slurg_Step_Watch_Points_449A90();
};