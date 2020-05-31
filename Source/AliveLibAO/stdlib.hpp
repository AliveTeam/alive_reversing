#pragma once

#include "FunctionFwd.hpp"

EXPORT void* alloc_450740(size_t);


EXPORT void ao_delete_free_450770(void*);

EXPORT void* CC ao_new_malloc_447520(int size);

EXPORT void CC ao_delete_free_447540(void* pMemory);

template<typename T, typename... Args>
inline T* ao_new(Args&&... args)
{
    void* buffer = ao_new_malloc_447520(sizeof(T));
    if (buffer)
    {
        return new (buffer) T(std::forward<Args>(args)...);
    }
    return nullptr;
}