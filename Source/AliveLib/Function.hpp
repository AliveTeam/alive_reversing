#pragma once

#include <windows.h>
#include <ostream>
#include <map>
#include <memory>
#include <fstream>
#include <set>
#include <vector>
#include <type_traits>
#include "logger.hpp"
#include "detours.h"

bool IsAlive();

#define ALIVE_FATAL(x)  ::MessageBox(NULL, x, "ALIVE Hook fatal error.", MB_ICONERROR | MB_OK); __debugbreak(); abort();
#define STATIC_EQUALS(src, dst) static_assert(src == dst, "Not equal!");
#define NOT_IMPLEMENTED { __asm {nop} _asm {nop} _asm {nop} _asm {nop} _asm {int 3} __asm {nop} _asm {nop} _asm {nop} _asm {nop}; static bool __done__ = false; if (!__done__) { __done__ = true; LOG_WARNING("Not implemented"); }}
#define EXPORT __declspec(dllexport)

class AliveVar
{
public:
    AliveVar(const char* name, DWORD addr, DWORD sizeInBytes, bool isDynamicallyAllocated, bool isConstData);
};

#define ALIVE_ARY(Redirect, Addr, TypeName, Size, VarName, ...)\
TypeName LocalArray_##VarName[Size]=__VA_ARGS__;\
AliveVar Var_##VarName(#VarName, Addr, sizeof(LocalArray_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
TypeName* VarName = (Redirect && IsAlive()) ? reinterpret_cast<TypeName*>(Addr) : reinterpret_cast<TypeName*>(&LocalArray_##VarName[0]);

// Only use this for pointers to arrays until it can be changed to ALIVE_ARY (so this is only used when the array size is not yet known)
#define ALIVE_PTR(Redirect, Addr, TypeName, VarName, Value)\
TypeName LocalPtr_##VarName = Value;\
AliveVar Var_##VarName(#VarName, Addr, sizeof(LocalPtr_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
std::remove_pointer<TypeName>::type * VarName = (Redirect && IsAlive()) ? reinterpret_cast<TypeName>(Addr) : LocalPtr_##VarName;

#define ALIVE_VAR(Redirect, Addr, TypeName, VarName, Value)\
TypeName LocalVar_##VarName = Value;\
AliveVar Var_##VarName(#VarName, Addr, sizeof(LocalVar_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
TypeName& VarName = (Redirect && IsAlive()) ? *reinterpret_cast<TypeName*>(Addr) : LocalVar_##VarName;

#define ALIVE_VAR_EXTERN(TypeName, VarName)\
extern TypeName LocalVar_##VarName;\
extern TypeName& VarName;

#define ALIVE_ARY_EXTERN(TypeName, Size, VarName)\
extern AliveVar Var_##VarName;\
extern TypeName* VarName ;

template<class T>
inline void DoDetour(DWORD addr, T func)
{
    LONG err = DetourTransactionBegin();

    if (err != NO_ERROR)
    {
        abort();
    }

    err = DetourUpdateThread(GetCurrentThread());

    if (err != NO_ERROR)
    {
        abort();
    }

    err = DetourAttach(&(PVOID&)addr, (PVOID)func);
    if (err != NO_ERROR)
    {
        abort();
    }

    err = DetourTransactionCommit();
    if (err != NO_ERROR)
    {
        abort();
    }
}


#define ALIVE_REDIRECT(addr, func) DoDetour((DWORD)addr, func)

#define ALIVE_ASSERT_SIZEOF(structureName, expectedSize) static_assert(sizeof(structureName) == expectedSize, "sizeof(" #structureName ") must be " #expectedSize)

#define CC __cdecl
#define ALIVE_COUNTOF(x) _countof(x)

#define BYTEn(x, n)   (*((BYTE*)&(x)+n))
#define BYTE0(x)   BYTEn(x,  0)
#define BYTE1(x)   BYTEn(x,  1)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))


void CheckVars();


class ScopedDetour
{
public:
    template<class T, class Y>
    ScopedDetour(T src, Y dst)
    {
        auto applyDetour = [this]()
        {
            return DetourAttach(&(PVOID&)mReal, (PVOID)mDst);
        };
        mReal = (PVOID)src;
        mDst = (PVOID)dst;
        DoDetour(applyDetour);
    }

    ~ScopedDetour()
    {
        auto removeDetour = [this]()
        {
            return  DetourDetach(&(PVOID&)mReal, (PVOID)mDst);
        };
        DoDetour(removeDetour);
    }

private:
    template<class T>
    void DoDetour(T fn)
    {
        LONG err = DetourTransactionBegin();

        if (err != NO_ERROR)
        {
            abort();
        }

        err = DetourUpdateThread(GetCurrentThread());

        if (err != NO_ERROR)
        {
            abort();
        }

        err = fn();
        if (err != NO_ERROR)
        {
            abort();
        }

        err = DetourTransactionCommit();
        if (err != NO_ERROR)
        {
            abort();
        }
    }

    void* mReal;
    void* mDst;
};

#define SCOPED_REDIRECT(real, stub) ScopedDetour real##_scoped(real, stub)
