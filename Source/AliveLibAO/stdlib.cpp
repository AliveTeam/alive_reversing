#include "stdafx_ao.h"
#include "stdlib.hpp"
#include "Function.hpp"

void* alloc_450740(size_t size)
{
    
    return ::malloc(size);
}

void ao_delete_free_450770(void* ptr)
{
    
    ::free(ptr);
}

void* ao_new_malloc_447520(s32 size)
{
    
    return ::malloc(size);
}

void ao_delete_free_447540(void* ptr)
{
    
    ::free(ptr);
}
