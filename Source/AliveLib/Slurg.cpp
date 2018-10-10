#include "stdafx.h"
#include "Slurg.hpp"

WORD Slurg::sSlurg_Step_Watch_Points_Idx_5C1C08;
char Slurg::sSlurg_Step_Watch_Points_Count_5BD4DC[2];
Slurg_Step_Watch_Points Slurg::sSlurg_Step_Watch_Points_5C1B28[2];

void CC Slurg::Clear_Slurg_Step_Watch_Points_449A90()
{
	sSlurg_Step_Watch_Points_Idx_5C1C08 = sSlurg_Step_Watch_Points_Idx_5C1C08 == 0;
	sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = 0;

	Slurg_Step_Watch_Points* point = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
	point->field_0_points[0].field_0_xPos = 0;
	point->field_0_points[0].field_2_yPos = 0;
	
	point->field_0_points[1].field_0_xPos = 0;
	point->field_0_points[1].field_2_yPos = 0;

	point->field_0_points[2].field_0_xPos = 0;
	point->field_0_points[2].field_2_yPos = 0;

	point->field_0_points[3].field_0_xPos = 0;
	point->field_0_points[3].field_2_yPos = 0;

	point->field_0_points[4].field_0_xPos = 0;
	point->field_0_points[4].field_2_yPos = 0;
}

