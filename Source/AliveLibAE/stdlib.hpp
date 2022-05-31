#pragma once

#include "../AliveLibCommon/Function.hpp"

void ae_internal_free_521334(void* ptr);
void* ae_internal_malloc_5212C0(size_t size);
void* ae_realloc_522335(void* ptr, size_t size);
s32 ae_fseek_521955(FILE* stream, s32 offset, s32 origin);
size_t ae_fread_520B5C(void* ptr, size_t size, size_t count, FILE* stream);
FILE* ae_fopen_520C64(const char_type* filename, const char_type* mode);
s32 ae_fclose_520CBE(FILE* stream);
void* ae_malloc_4F4E60(size_t size);
void* ae_new_malloc_4954D0(size_t size);
void* ae_malloc_non_zero_4954F0(size_t size);
void ae_free_4F4EA0(void* ptr);
void ae_delete_free_495540(void* ptr);
void ae_non_zero_free_495560(void* ptr);
void* realloc_4F4E80(void* ptr, size_t size);
s32 ae_remove_520B27(const char* lpFileName);

template <typename T, typename... Args>
inline T* ae_new(Args&&... args)
{
    void* buffer = ae_new_malloc_4954D0(sizeof(T));
    memset(buffer, 0, sizeof(T));
    if constexpr (sizeof...(args) == 0)
    {
        return new (buffer) T();
    }
    else
    {
        return new (buffer) T(std::forward<Args>(args)...);
    }
}

s32 access_impl(char_type const* fileName, s32 accessMode);
