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

// stdlib proxys, see comment on STDLIB_FUNCTION for details.
void ae_internal_free_521334(void* ptr)
{
    
    ::free(ptr);
}

void* ae_internal_malloc_5212C0(size_t size)
{
    
    return ::malloc(size);
}

void* ae_realloc_522335(void* ptr, size_t size)
{
    
    return ::realloc(ptr, size);
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

// Game specific stdlib wrappers
void* ae_malloc_4F4E60(size_t size)
{
    if (size > 0)
    {
        return ae_internal_malloc_5212C0(size);
    }

    ALIVE_FATAL("0 bytes allocated");
}

void* ae_new_malloc_4954D0(size_t size)
{
    return ae_internal_malloc_5212C0(size);
}

void* ae_malloc_non_zero_4954F0(size_t size)
{
    if (size == 0)
    {
        size = 1;
    }
    return ae_internal_malloc_5212C0(size);
}

void ae_free_4F4EA0(void* ptr)
{
    if (ptr)
    {
        ae_internal_free_521334(ptr);
    }
    else
    {
        Error_NullPrint_4F28D0("mem_free: invalid pointer ( 0 )\n");
    }
}

void ae_delete_free_495540(void* ptr)
{
    ae_internal_free_521334(ptr);
}

void ae_non_zero_free_495560(void* ptr)
{
    ae_internal_free_521334(ptr);
}

void* realloc_4F4E80(void* ptr, size_t size)
{
    return ae_realloc_522335(ptr, size);
}

s32 ae_remove_520B27(const char* lpFileName)
{
    
    return ::remove(lpFileName);
}
