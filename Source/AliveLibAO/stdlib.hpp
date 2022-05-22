#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"

EXPORT void* alloc_450740(size_t);


EXPORT void ao_delete_free_450770(void*);

EXPORT void* CC ao_new_malloc_447520(s32 size);

EXPORT void CC ao_delete_free_447540(void* pMemory);

template <typename T, typename... Args>
inline T* ao_new(Args&&... args)
{
    void* buffer = ao_new_malloc_447520(sizeof(T));
    if (buffer)
    {
        if constexpr (sizeof...(args) == 0)
        {
            return new (buffer) T;
        }
        else
        {
            return new (buffer) T(std::forward<Args>(args)...);
        }
    }
    return nullptr;
}
