#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

struct Slurg_Step_Watch_Point
{
	WORD field_0_xPos;
	WORD field_2_yPos;
};

struct Slurg_Step_Watch_Points
{
	Slurg_Step_Watch_Point field_0_points[5];
};


class Slurg : public BaseAliveGameObject
{
public:
	static WORD sSlurg_Step_Watch_Points_Idx_5C1C08;
	static char sSlurg_Step_Watch_Points_Count_5BD4DC[2];
	static Slurg_Step_Watch_Points sSlurg_Step_Watch_Points_5C1B28[2];

	static void CC Clear_Slurg_Step_Watch_Points_449A90();
};