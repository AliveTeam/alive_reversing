#pragma once

bool IsAlive();

#define ALIVE_FATAL(x)  ::MessageBox(NULL, x, "ALIVE Hook fatal error.", MB_ICONERROR | MB_OK); __debugbreak(); abort();
#define STATIC_EQUALS(src, dst) static_assert(src == dst, "Not equal!");
#define EXPORT __declspec(dllexport)
#define ALIVE_ASSERT_SIZEOF(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize)
#define CC __cdecl
#define ALIVE_COUNTOF(x) _countof(x)
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
