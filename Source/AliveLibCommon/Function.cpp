#include "stdafx_common.h"
#include "Function.hpp"
#include <set>
#include <fstream>

#if defined(_WIN32) && !defined(_WIN64)
#include "detours.h"
#endif

bool gVTableHack = true;

void SetVTable(void* thisPtr, DWORD vTable)
{
#if !_WIN64
    if (RunningAsInjectedDll() && gVTableHack)
    {
        *reinterpret_cast<DWORD**>(thisPtr) = reinterpret_cast<DWORD*>(vTable);
    }
#endif
}

struct TVarInfo
{
    DWORD mAddr;
    DWORD mSize;
    bool mIsPointerType;
    bool mIsConstData;
    const char* mName;
};

static std::vector<TVarInfo>& Vars()
{
    static std::vector<TVarInfo> v;
    return v;
}

void CheckVars()
{
    TRACE_ENTRYEXIT;

    const auto& vars = Vars();

    std::set<DWORD> usedAddrs;
    std::vector<std::string> dupVars;
    for (const auto& varToCheck : vars)
    {
        auto it = usedAddrs.find(varToCheck.mAddr);
        if (it != std::end(usedAddrs))
        {
            std::stringstream s;
            s << "Var at addr 0x" << std::hex << varToCheck.mAddr << " (" << varToCheck.mName << ") is defined more than once";
            dupVars.push_back(s.str());
        }
        usedAddrs.insert(varToCheck.mAddr);
    }

    for (const auto& err : dupVars)
    {
        LOG_ERROR(err);
    }

    if (!dupVars.empty())
    {
        HOOK_FATAL("Duplicated vars found");
    }

    for (const auto& varToCheck : vars)
    {
        for (const auto& var : vars)
        {
            if (&varToCheck != &var)
            {
                // TODO: check size of varToCheck within range
                const DWORD varStart = var.mAddr;
                const DWORD varEnd = var.mAddr + var.mSize;

                const DWORD toCheckStart = varToCheck.mAddr;
                const DWORD toCheckEnd = varToCheck.mAddr + varToCheck.mSize;

                if (toCheckStart >= varStart && toCheckEnd < varEnd)
                {
                    // Var overlaps
                    std::stringstream s;
                    s << "Var at addr 0x" << std::hex << varToCheck.mAddr << " (" << varToCheck.mName << ") overlaps other data (" << var.mName << ") offset 0x" <<  std::hex << (std::abs((LONG)(varToCheck.mAddr - var.mAddr)));
                    HOOK_FATAL(s.str().c_str());
                }
            }
        }
    }
}

bool operator < (const TVarInfo& lhs, const TVarInfo& rhs)
{
    return lhs.mAddr < rhs.mAddr;
}

AliveVar::AliveVar(const char* name, DWORD addr, DWORD sizeInBytes, bool isPointerType, bool isConstData)
{
    Vars().push_back({ addr, sizeInBytes, isPointerType, isConstData, name });
}

ScopedDetour::~ScopedDetour()
{
    DoDetour(false, &(PVOID&)mReal, (PVOID)mDst);
}

void ScopedDetour::Construct()
{
    DoDetour(true, &(PVOID&)mReal, (PVOID)mDst);
}

void ScopedDetour::DoDetour(bool attach, PVOID* ppPointer, PVOID detour)
{
#if defined(_WIN32) && !defined(_WIN64)
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

    err = attach ? DetourAttach(ppPointer, detour) : DetourDetach(ppPointer, detour);
    if (err != NO_ERROR)
    {
        abort();
    }

    err = DetourTransactionCommit();
    if (err != NO_ERROR)
    {
        abort();
    }
#endif
}

DisableVTableHack::DisableVTableHack()
{
    gVTableHack = false;
}

DisableVTableHack::~DisableVTableHack()
{
    gVTableHack = true;
}
