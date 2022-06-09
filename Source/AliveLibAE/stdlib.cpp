#include "stdafx.h"
#include "stdlib.hpp"

#if _WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif

s32 access_impl(char_type const* fileName, s32 accessMode)
{
#if _WIN32
    return _access(fileName, accessMode);
#else
    return access(fileName, accessMode);
#endif
}
