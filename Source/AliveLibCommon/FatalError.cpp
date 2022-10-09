#include "Sys_common.hpp"

[[noreturn]] void ALIVE_FATAL(const char_type* errMsg)
{
    Sys_MessageBox(nullptr, errMsg, "R.E.L.I.V.E fatal error.");
    abort();
}
