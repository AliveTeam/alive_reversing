#pragma once

#include <type_traits>
#include "logger.hpp"

#ifdef _MSC_VER
    #define CC __cdecl
    #if _WIN64
        #define ALIVE_ASSERT_SIZEOF(structureName, expectedSize) static_assert(true)
    #else
        #define ALIVE_ASSERT_SIZEOF(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize)
    #endif
#else
    #define CC
    #define ALIVE_ASSERT_SIZEOF(structureName, expectedSize) static_assert(true)

    // Replace with a function that actually exists but does the same thing
    #define strcmpi strcasecmp
    #define _strcmpi strcasecmp

#endif

#define ALIVE_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize)


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

template <class T>
struct BrainFunctionData final
{
    T mOurFn;
    u32 mOriginal;
    const char_type* fnName;
};

template <class BrainFunctionType, class BrainTable>
inline bool BrainIs(BrainFunctionType fn, BrainFunctionType& brainVar, const BrainTable&)
{
    return brainVar == fn;
}

template <class BrainFunctionType, class BrainTable>
void SetBrain(BrainFunctionType fn, BrainFunctionType& brainVar, const BrainTable&)
{
    brainVar = fn;
}

template <class BrainFunctionType, class BrainTable>
inline const BrainFunctionData<BrainFunctionType>& GetOriginalFn(BrainFunctionType fn, const BrainTable& table)
{
    // If not running as standalone set the address to be
    // the address of the real function rather than the reimpl as the real
    // game code compares the function pointer addresses (see IsBrain(x)).
    for (const auto& addrPair : table)
    {
        if (addrPair.mOurFn == fn)
        {
            return addrPair;
        }
    }
    LOG_ERROR("Couldn't find brain in the brain table!");
    abort();
}
