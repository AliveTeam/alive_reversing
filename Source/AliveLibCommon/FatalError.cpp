#include "Sys_common.hpp"

[[noreturn]] void ALIVE_FATAL(const char_type* errMsg)
{
    Sys_MessageBox(nullptr, errMsg, "ALIVE Hook fatal error.");
    abort();
}
