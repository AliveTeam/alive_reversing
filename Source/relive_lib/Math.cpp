#include "Math.hpp"
#include "GameType.hpp"
#include "../AliveLibAE/Math.hpp"
#include "../AliveLibAO/Math.hpp"

FP Math_SquareRoot_FP_Wrapper(FP value)
{
    if (GetGameType() == GameType::eAo)
    {
        return AO::Math_SquareRoot_FP(value);
    }
    else
    {
        return Math_SquareRoot_FP(value);
    }
}
