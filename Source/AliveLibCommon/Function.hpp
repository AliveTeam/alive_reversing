#pragma once

#include <type_traits>
#include "logger.hpp"

#ifdef _MSC_VER
    #define CC __cdecl
    #if _WIN64
        #define ALIVE_ASSERT_SIZEOF(structureName, expectedSize) static_assert(true)
    #else
        #define ALIVE_ASSERT_SIZEOF(structureName, expectedSize) static_assert(true)
    #endif
#else
    #define CC
    #define ALIVE_ASSERT_SIZEOF(structureName, expectedSize) static_assert(true)

    // Replace with a function that actually exists but does the same thing
    #define strcmpi strcasecmp
    #define _strcmpi strcasecmp

#endif

#define ALIVE_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize)

#define relive_delete delete
#define relive_new new (std::nothrow)

#define STATIC_EQUALS(src, dst) static_assert(src == dst, "Not equal!");
#define ALIVE_COUNTOF(x) (sizeof(x) / sizeof(*(x)))
#define BYTEn(x, n) (*((u8*) &(x) + n))
#define BYTE0(x) BYTEn(x, 0)
#define BYTE1(x) BYTEn(x, 1)
#define BYTE2(x) BYTEn(x, 2)
#define BYTE3(x) BYTEn(x, 3)

#define ALIVE_ARY(Redirect, Addr, TypeName, Size, VarName, ...) TypeName VarName[Size] = __VA_ARGS__

#define ALIVE_ARY_SIZEOF(VarName) sizeof(VarName)

#define ALIVE_PTR(Redirect, Addr, TypeName, VarName, Value)  TypeName VarName = Value
#define ALIVE_VAR(Redirect, Addr, TypeName, VarName, Value) TypeName VarName = Value

#define ALIVE_VAR_EXTERN(TypeName, VarName) extern TypeName VarName

#define ALIVE_ARY_EXTERN(TypeName, Size, VarName) extern TypeName VarName[Size]
