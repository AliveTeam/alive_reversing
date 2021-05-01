#include "stdafx.h"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Sys_common.hpp"

s32 access_impl(char_type const* fileName, s32 accessMode)
{
#if _WIN32
    return _access(fileName, accessMode);
#else
    return access(fileName, accessMode);
#endif
}

// stdlib proxys, see comment on STDLIB_FUNCTION for details.
EXPORT void CC ae_internal_free_521334(void* ptr)
{
    STDLIB_FUNCTION();
    ::free(ptr);
}

EXPORT void* CC ae_internal_malloc_5212C0(size_t size)
{
    STDLIB_FUNCTION();
    return ::malloc(size);
}

EXPORT void* CC ae_realloc_522335(void* ptr, size_t size)
{
    STDLIB_FUNCTION();
    return ::realloc(ptr, size);
}

EXPORT s32 CC ae_fseek_521955(FILE* stream, s32 offset, s32 origin)
{
    STDLIB_FUNCTION();
    return fseek(stream, offset, origin);
}

EXPORT size_t CC ae_fread_520B5C(void* ptr, size_t size, size_t count, FILE* stream)
{
    STDLIB_FUNCTION();
    return fread(ptr, size, count, stream);
}

EXPORT FILE* CC ae_fopen_520C64(const char_type* filename, const char_type* mode)
{
    STDLIB_FUNCTION();
    return fopen(filename, mode);
}

EXPORT s32 CC ae_fclose_520CBE(FILE* stream)
{
    STDLIB_FUNCTION();
    return fclose(stream);
}

// Game specific stdlib wrappers
EXPORT void* CC ae_malloc_4F4E60(size_t size)
{
    if (size > 0)
    {
        return ae_internal_malloc_5212C0(size);
    }

    ALIVE_FATAL("0 bytes allocated");
}

EXPORT void* CC ae_new_malloc_4954D0(size_t size)
{
    return ae_internal_malloc_5212C0(size);
}

EXPORT void* CC ae_malloc_non_zero_4954F0(size_t size)
{
    if (size == 0)
    {
        size = 1;
    }
    return ae_internal_malloc_5212C0(size);
}

EXPORT void CC ae_free_4F4EA0(void* ptr)
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

EXPORT void CC ae_delete_free_495540(void* ptr)
{
    ae_internal_free_521334(ptr);
}

EXPORT void CC ae_non_zero_free_495560(void* ptr)
{
    ae_internal_free_521334(ptr);
}

EXPORT void* CC realloc_4F4E80(void* ptr, size_t size)
{
    return ae_realloc_522335(ptr, size);
}

EXPORT s32 CC ae_remove_520B27(LPCSTR lpFileName)
{
    STDLIB_FUNCTION();
    return ::remove(lpFileName);
}
