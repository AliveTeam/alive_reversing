#pragma once

#include <type_traits>
#include "logger.hpp"
#include "FunctionFwd.hpp"

#ifdef _MSC_VER

#if _WIN64
#define NOT_IMPLEMENTED() {  static bool __done__ = false; if (!__done__) { __done__ = true; LOG_WARNING("Not implemented"); } }
#else
#define NOT_IMPLEMENTED() { const static auto __kAddr__ = __FUNCTION__ "\0" __FUNCDNAME__; __asm push eax __asm mov eax, __kAddr__ __asm pop eax __asm nop __asm nop __asm nop __asm nop __asm int 3 __asm nop __asm nop __asm nop __asm nop } static bool __done__ = false; if (!__done__) { __done__ = true; LOG_WARNING("Not implemented"); }
#endif


#if _WIN64
#define STDLIB_FUNCTION()
#else
// Stdlib functions are marked as not implemented even though they are implemented. This is so that in the real game in the injection dll
// we call the games stdlib function due to being marked as not implemented. Then in the standalone exe we have to run whatever impl we have which
// is a forward call to the matching stdlib function. We use this instead of NOT_IMPLEMENTED() to highlite this odd corner case and so that we do not
// get log spam about stdlib functions not being implemented.
#define STDLIB_FUNCTION() { const static auto __kAddr__ = __FUNCTION__ "\0" __FUNCDNAME__; __asm push eax __asm mov eax, __kAddr__ __asm pop eax __asm nop __asm nop __asm nop __asm nop __asm int 3 __asm nop __asm nop __asm nop __asm nop }
#endif

#else
    #define NOT_IMPLEMENTED() {  static bool __done__ = false; if (!__done__) { __done__ = true; LOG_WARNING("Not implemented"); } }
    #define STDLIB_FUNCTION()
#endif

#define ALIVE_ARY(Redirect, Addr, TypeName, Size, VarName, ...)\
TypeName LocalArray_##VarName[Size]=__VA_ARGS__;\
AliveVar Var_##VarName(#VarName, Addr, sizeof(LocalArray_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
TypeName* VarName = (Redirect && IsAlive()) ? reinterpret_cast<TypeName*>(Addr) : reinterpret_cast<TypeName*>(&LocalArray_##VarName[0]);
#define ALIVE_ARY_SIZEOF(VarName) sizeof(LocalArray_##VarName)

// Only use this for pointers to arrays until it can be changed to ALIVE_ARY (so this is only used when the array size is not yet known)
#define ALIVE_PTR(Redirect, Addr, TypeName, VarName, Value)\
TypeName LocalPtr_##VarName = Value;\
AliveVar Var_##VarName(#VarName, Addr, sizeof(LocalPtr_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
std::remove_pointer<TypeName>::type * VarName = (Redirect && IsAlive()) ? reinterpret_cast<TypeName>(Addr) : LocalPtr_##VarName;

#define ALIVE_VAR(Redirect, Addr, TypeName, VarName, Value)\
TypeName LocalVar_##VarName = Value;\
AliveVar Var_##VarName(#VarName, Addr, sizeof(LocalVar_##VarName), std::is_pointer<TypeName>::value, std::is_const<TypeName>::value);\
TypeName& VarName = (Redirect && IsAlive()) ? *reinterpret_cast<TypeName*>(Addr) : LocalVar_##VarName;


void CheckVars();

class ScopedDetour
{
public:
    template<class T, class Y>
    ScopedDetour(T src, Y dst)
    {
        mReal = (PVOID)src;
        mDst = (PVOID)dst;
        Construct();
    }

    ~ScopedDetour();

private:
    void Construct();

    static void DoDetour(bool attach, PVOID* ppPointer, PVOID detour);

    void* mReal;
    void* mDst;
};

#define SCOPED_REDIRECT(real, stub) ScopedDetour real##_scoped(real, stub)

extern bool gVTableHack;

void SetVTable(void* thisPtr, DWORD vTable);

class DisableVTableHack
{
public:
    DisableVTableHack();

    ~DisableVTableHack();
};
