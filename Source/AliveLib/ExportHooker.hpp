#pragma once

#include "detours.h"
#include <assert.h>
#include <vector>
#include <set>
#include <sstream>
#include "logger.hpp"
#include "Function.hpp"

class ExportHooker
{
public:
    explicit ExportHooker(HINSTANCE instance);

    void Apply(bool saveImplementedFuncs = false);

private:
    void LoadDisabledHooks();
    void ProcessExports();

    static bool IsHexDigit(char letter);

    struct ExportInformation
    {
        bool mIsImplemented;

        std::string mExportedFunctionName;
        std::string mUnMangledFunctioName;

        const std::string& Name()
        {
            if (!mUnMangledFunctioName.empty())
            {
                return mUnMangledFunctioName;
            }
            return mExportedFunctionName;
        }
    };

    static ExportInformation GetExportInformation(PVOID pExportedFunctionAddress, const std::string& exportedFunctionName);

    void OnExport(PCHAR pszName, PVOID pCode);

    static BOOL CALLBACK EnumExports(PVOID pContext,
        ULONG /*nOrdinal*/,
        PCHAR pszName,
        PVOID pCode);

    HINSTANCE mhInstance = nullptr;
    struct Export
    {
        std::string mName;
        LPVOID mCode;
        DWORD mGameFunctionAddr;
        DWORD mHookedGameFunctionAddr;
        bool mIsImplemented;
    };
    std::vector<Export> mExports;
    std::map<DWORD, ExportInformation> mUsedAddrs;

    std::map<DWORD, DWORD> mRealStubs;
    std::set<DWORD> mDisabledImpls;
};
