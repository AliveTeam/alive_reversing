#pragma once

#include "FunctionFwd.hpp"

EXPORT void* CC malloc_4954D0(size_t size);
EXPORT void* CC malloc_4F4E60(size_t size);
EXPORT void CC mem_free_4F4EA0(void* ptr);
EXPORT void* CC malloc_non_zero_4954F0(size_t size);
EXPORT void CC Mem_Free_495560(void *ptr);
EXPORT void CC Mem_Free_495540(void* ptr);

template<typename T, typename... Args>
inline T* alive_new(Args&&... args)
{
    void* buffer = malloc_4954D0(sizeof(T));
    if (buffer)
    {
        return new (buffer) T(std::forward<Args>(args)...);
    }
    return nullptr;
}
