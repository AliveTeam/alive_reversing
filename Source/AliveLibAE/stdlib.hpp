#pragma once

#include "FunctionFwd.hpp"

EXPORT void CC ae_internal_free_521334(void* ptr);
EXPORT void* CC ae_internal_malloc_5212C0(size_t size);
EXPORT void* CC ae_realloc_522335(void* ptr, size_t size);
EXPORT int CC ae_fseek_521955(FILE* stream, int offset, int origin);
EXPORT size_t CC ae_fread_520B5C(void* ptr, size_t size, size_t count, FILE* stream);
EXPORT FILE* CC ae_fopen_520C64(const char* filename, const char* mode);
EXPORT int CC ae_fclose_520CBE(FILE* stream);
EXPORT void* CC ae_malloc_4F4E60(size_t size);
EXPORT void* CC ae_new_malloc_4954D0(size_t size);
EXPORT void* CC ae_malloc_non_zero_4954F0(size_t size);
EXPORT void CC ae_free_4F4EA0(void* ptr);
EXPORT void CC ae_delete_free_495540(void* ptr);
EXPORT void CC ae_non_zero_free_495560(void* ptr);
EXPORT void* CC realloc_4F4E80(void* ptr, size_t size);
EXPORT int CC ae_remove_520B27(LPCSTR lpFileName);

template<typename T, typename... Args>
inline T* ae_new(Args&&... args)
{
    void* buffer = ae_new_malloc_4954D0(sizeof(T));
    if (buffer)
    {
        return new (buffer) T(std::forward<Args>(args)...);
    }
    return nullptr;
}

int access_impl(char const* fileName, int accessMode);
