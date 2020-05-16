#pragma once

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
    void OnExport(PCHAR pszName, PVOID pCode);

private:
    void LoadDisabledHooks();
    void ProcessExports();

    static bool IsHexDigit(char letter);

    struct ExportInformation
    {
        bool mIsImplemented;

        std::string mExportedFunctionName;
        std::string mUnMangledFunctioName;

        const std::string& Name();
    };

    static ExportInformation GetExportInformation(PVOID pExportedFunctionAddress, const std::string& exportedFunctionName);

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
    int mExportCount = 0;
    int mHookedCount = 0;
};
