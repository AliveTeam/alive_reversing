#pragma once

struct FixedPoint;
using FP = FixedPoint;

// For some reason OWI decided to change how square root works between games, call the right one to
// keep the same results for now
FP Math_SquareRoot_FP_Wrapper(FP value);
