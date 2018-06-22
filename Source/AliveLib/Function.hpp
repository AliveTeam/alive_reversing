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
#include "FunctionFwd.hpp"

#define NOT_IMPLEMENTED { __asm {nop} _asm {nop} _asm {nop} _asm {nop} _asm {int 3} __asm {nop} _asm {nop} _asm {nop} _asm {nop}; static bool __done__ = false; if (!__done__) { __done__ = true; LOG_WARNING("Not implemented"); }}

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
