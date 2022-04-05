#pragma once

template <class T>
[[nodiscard]] inline T RoundUp(T numToRound, T multiple)
{
    if (multiple == 0)
    {
        return numToRound;
    }

    auto remainder = numToRound % multiple;
    if (remainder == 0)
    {
        return numToRound;
    }

    return numToRound + multiple - remainder;
}

template <class T>
[[nodiscard]] inline T RoundUp(T offset)
{
    return RoundUp(offset, static_cast<T>(2048));
}