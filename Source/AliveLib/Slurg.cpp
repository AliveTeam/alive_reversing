#include "stdafx.h"
#include "Slurg.hpp"
#include "Function.hpp"

ALIVE_VAR(1, 0x5C1C08, WORD, sSlurg_Step_Watch_Points_Idx_5C1C08, 0);
ALIVE_ARY(1, 0x5BD4DC, char, 2, sSlurg_Step_Watch_Points_Count_5BD4DC, {});
ALIVE_ARY(1, 0x5C1B28, Slurg_Step_Watch_Points, 2, sSlurg_Step_Watch_Points_5C1B28, {});

EXPORT void CC Slurg::Clear_Slurg_Step_Watch_Points_449A90()
{
    sSlurg_Step_Watch_Points_Idx_5C1C08 = sSlurg_Step_Watch_Points_Idx_5C1C08 == 0;
    sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = 0;


    for (auto& point : sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08].field_0_points)
    {
        point = {};
    }
}