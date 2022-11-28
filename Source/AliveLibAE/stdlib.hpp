#pragma once

s32 access_impl(char_type const* fileName, s32 accessMode);

#define relive_remove(fileName) ::remove(fileName)
