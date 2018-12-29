#include "stdafx.h"
#include "stdlib.hpp"
#include "Function.hpp"
#include "Error.hpp"

// stdlib proxys, see comment on STDLIB_FUNCTION for details.
EXPORT void CC free_521334(void* ptr)
{
    STDLIB_FUNCTION();
    ::free(ptr);
}

EXPORT void* CC malloc_5212C0(size_t size)
{
    STDLIB_FUNCTION();
    return ::malloc(size);
}

EXPORT void* CC realloc_522335(void* ptr, size_t size)
{
    STDLIB_FUNCTION();
    return ::realloc(ptr, size);
}

EXPORT int CC fseek_521955(FILE* stream, int offset, int origin)
{
    STDLIB_FUNCTION();
    return fseek(stream, offset, origin);
}

EXPORT size_t CC fread_520B5C(void* ptr, size_t size, size_t count, FILE* stream)
{
    STDLIB_FUNCTION();
    return fread(ptr, size, count, stream);
}

EXPORT FILE* CC fopen_520C64(const char* filename, const char* mode)
{
    STDLIB_FUNCTION();
    return fopen(filename, mode);
}

EXPORT int CC fclose_520CBE(FILE* stream)
{
    STDLIB_FUNCTION();
    return fclose(stream);
}

// Game specific stdlib wrappers
EXPORT void* CC malloc_4F4E60(size_t size)
{
    if (size > 0)
    {
        return malloc_5212C0(size);
    }

    ALIVE_FATAL("0 bytes allocated");
}

EXPORT void* CC malloc_4954D0(size_t size) // Probably operator new
{
    return malloc_5212C0(size);
}

EXPORT void* CC malloc_non_zero_4954F0(size_t size)
{
    if (size == 0)
    {
        size = 1;
    }
    return malloc_5212C0(size);
}

EXPORT void CC mem_free_4F4EA0(void* ptr)
{
    if (ptr)
    {
        free_521334(ptr);
    }
    else
    {
        Error_NullPrint_4F28D0("mem_free: invalid pointer ( 0 )\n");
    }
}

EXPORT void CC Mem_Free_495540(void* ptr)
{
    free_521334(ptr);
}

EXPORT void CC Mem_Free_495560(void *ptr)
{
    free_521334(ptr);
}

EXPORT void* CC realloc_4F4E80(void* ptr, size_t size)
{
    return realloc_522335(ptr, size);
}

EXPORT int CC remove_520B27(LPCSTR lpFileName)
{
    STDLIB_FUNCTION();
    return ::remove(lpFileName);
}
