#pragma once

#include "../AliveLibCommon/Function.hpp"

s32 ae_fseek_521955(FILE* stream, s32 offset, s32 origin);
size_t ae_fread_520B5C(void* ptr, size_t size, size_t count, FILE* stream);
FILE* ae_fopen_520C64(const char_type* filename, const char_type* mode);
s32 ae_fclose_520CBE(FILE* stream);

s32 ae_remove_520B27(const char* lpFileName);
s32 access_impl(char_type const* fileName, s32 accessMode);

#define relive_remove(fileName) ::remove(fileName)
