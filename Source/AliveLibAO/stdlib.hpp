#pragma once

#include "../AliveLibCommon/Function.hpp"

void* alloc_450740(size_t);


void ao_delete_free_450770(void*);

void* ao_new_malloc_447520(s32 size);

void ao_delete_free_447540(void* pMemory);

template <typename T, typename... Args>
inline T* ao_new(Args&&... args)
{
    void* buffer = ao_new_malloc_447520(sizeof(T));
    if (buffer)
    {
        if constexpr (sizeof...(args) == 0)
        {
            return new (buffer) T();
        }
        else
        {
            memset(buffer, 0, sizeof(T)); // HACK: Remove when vars are init'ed correctly
            return new (buffer) T(std::forward<Args>(args)...);
        }
    }
    return nullptr;
}
