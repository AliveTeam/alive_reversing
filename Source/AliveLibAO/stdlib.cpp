#include "stdafx_ao.h"
#include "stdlib.hpp"
#include "Function.hpp"

EXPORT void* alloc_450740(size_t size)
{
    STDLIB_FUNCTION();
    return ::malloc(size);
}

EXPORT void ao_delete_free_450770(void* ptr)
{
    STDLIB_FUNCTION();
    ::free(ptr);
}

EXPORT void* CC ao_new_malloc_447520(int size)
{
    STDLIB_FUNCTION();
    return ::malloc(size);
}

EXPORT void CC ao_delete_free_447540(void* ptr)
{
    STDLIB_FUNCTION();
    ::free(ptr);
}
