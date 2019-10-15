#pragma once

#include "FunctionFwd.hpp"

EXPORT void* CC malloc_4954D0(size_t size);
EXPORT void* CC malloc_4F4E60(size_t size);
EXPORT void* CC malloc_5212C0(size_t size);
EXPORT void CC mem_free_4F4EA0(void* ptr);
EXPORT void* CC malloc_non_zero_4954F0(size_t size);
EXPORT void CC Mem_Free_495560(void *ptr);
EXPORT void CC Mem_Free_495540(void* ptr);
EXPORT int CC fseek_521955(FILE* stream, int offset, int origin);
EXPORT size_t CC fread_520B5C(void* ptr, size_t size, size_t count, FILE* stream);
EXPORT FILE* CC fopen_520C64(const char* filename, const char* mode);
EXPORT int CC fclose_520CBE(FILE* stream);
EXPORT int CC remove_520B27(LPCSTR lpFileName);

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

int access_impl(char const* fileName, int accessMode);