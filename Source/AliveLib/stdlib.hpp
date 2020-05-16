#pragma once

#include "FunctionFwd.hpp"

void* CC alive_new_malloc(size_t size);
void* CC alive_malloc(size_t size);
void* CC alive_internal_malloc(size_t size);
void CC alive_free(void* ptr);
void* CC alive_malloc_non_zero(size_t size);
void CC alive_non_zero_free(void *ptr);
void CC alive_delete_free(void* ptr);
int CC alive_fseek(FILE* stream, int offset, int origin);
size_t CC alive_fread(void* ptr, size_t size, size_t count, FILE* stream);
FILE* CC alive_fopen(const char* filename, const char* mode);
int CC alive_fclose(FILE* stream);
int CC alive_remove(LPCSTR lpFileName);

template<typename T, typename... Args>
inline T* alive_new(Args&&... args)
{
    void* buffer = alive_new_malloc(sizeof(T));
    if (buffer)
    {
        return new (buffer) T(std::forward<Args>(args)...);
    }
    return nullptr;
}

int access_impl(char const* fileName, int accessMode);
