#pragma once

#include "Types.hpp"

struct RGB16 final
{
    s16 r = 0;
    s16 g = 0;
    s16 b = 0;

    void SetRGB(s16 rValue, s16 gValue, s16 bValue)
    {
        r = rValue;
        g = gValue;
        b = bValue;
    }
};
