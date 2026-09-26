#pragma once

#include <stdint.h>

using u8 = uint8_t;
using s8 = signed char;
using char_type = char;

using u16 = uint16_t;
using s16 = int16_t;

using u32 = uint32_t;
using s32 = int32_t;

using f32 = float;
using f64 = double;

using u64 = uint64_t;
using s64 = int64_t;

// Lets GCC/Clang type check the arguments of printf style functions against their format string.
// fmtIdx/firstArgIdx are 1 based and include the implicit `this` for non static member functions.
#if defined(__GNUC__) || defined(__clang__)
    #define RELIVE_PRINTF_FMT(fmtIdx, firstArgIdx) __attribute__((format(printf, fmtIdx, firstArgIdx)))
#else
    #define RELIVE_PRINTF_FMT(fmtIdx, firstArgIdx)
#endif

//using Bool32 = long;
