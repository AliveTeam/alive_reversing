#include "stdafx.h"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Sys_common.hpp"

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

s32 ae_fseek_521955(FILE* stream, s32 offset, s32 origin)
{
    
    return fseek(stream, offset, origin);
}

size_t ae_fread_520B5C(void* ptr, size_t size, size_t count, FILE* stream)
{
    
    return fread(ptr, size, count, stream);
}

FILE* ae_fopen_520C64(const char_type* filename, const char_type* mode)
{
    
    return fopen(filename, mode);
}

s32 ae_fclose_520CBE(FILE* stream)
{
    
    return fclose(stream);
}

s32 ae_remove_520B27(const char* lpFileName)
{
    return ::remove(lpFileName);
}
