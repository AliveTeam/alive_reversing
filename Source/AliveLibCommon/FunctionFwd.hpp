#pragma once

bool RunningAsInjectedDll();

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#define CC __cdecl
#define CCSTD __stdcall
#if _WIN64
#define ALIVE_ASSERT_SIZEOF(structureName, expectedSize)
#else
#define ALIVE_ASSERT_SIZEOF(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize)
#endif
#else
#define EXPORT
#define CC
#define CCSTD
#define ALIVE_ASSERT_SIZEOF(structureName, expectedSize)

// Replace with a function that actually exists but does the same thing
#define strcmpi strcasecmp
#define _strcmpi strcasecmp

#endif

#define ALIVE_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize)


[[noreturn]] void ALIVE_FATAL(const char* errMsg);

#define STATIC_EQUALS(src, dst) static_assert(src == dst, "Not equal!");
#define ALIVE_COUNTOF(x) (sizeof(x)/sizeof(*(x)))
#define BYTEn(x, n)   (*((BYTE*)&(x)+n))
#define BYTE0(x)   BYTEn(x,  0)
#define BYTE1(x)   BYTEn(x,  1)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))

class AliveVar
{
public:
    AliveVar(const char* name, DWORD addr, DWORD sizeInBytes, bool isDynamicallyAllocated, bool isConstData);
};

#define ALIVE_VAR_EXTERN(TypeName, VarName)\
extern TypeName LocalVar_##VarName;\
extern TypeName& VarName;

#define ALIVE_ARY_EXTERN(TypeName, Size, VarName)\
extern AliveVar Var_##VarName;\
extern TypeName* VarName ;


#define START_NS_AO namespace AO {
#define END_NS_AO }
