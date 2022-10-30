#include "Sys_common.hpp"
#include "logger.hpp"
#include <stdarg.h>

[[noreturn]] void ALIVE_FATAL(const char_type* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char_type buf[2048] = {};
    vsnprintf(buf, sizeof(buf) - 1, fmt, args);
    va_end(args);

    LOG_ERROR(buf);

    Sys_MessageBox(nullptr, buf, "R.E.L.I.V.E fatal error.");
    abort();
}
