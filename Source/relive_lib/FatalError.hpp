#pragma once

#include "Types.hpp"

[[noreturn]] void ALIVE_FATAL(const char_type* fmt, ...) RELIVE_PRINTF_FMT(1, 2);
