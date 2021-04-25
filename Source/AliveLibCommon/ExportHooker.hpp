#pragma once

#include <assert.h>
#include <vector>
#include <set>
#include <sstream>
#include "logger.hpp"
#include "Function.hpp"
#include "Types.hpp"

class ExportHooker
{
public:
    explicit ExportHooker(HINSTANCE instance);

    void Apply(bool saveImplementedFuncs = false);
    void OnExport(PCHAR pszName, PVOID pCode);

private:
    void LoadDisabledHooks();
    void ProcessExports();

    static bool IsHexDigit(s8 letter);

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
        u32 mGameFunctionAddr;
        u32 mHookedGameFunctionAddr;
        bool mIsImplemented;
    };
    std::vector<Export> mExports;
    std::map<u32, ExportInformation> mUsedAddrs;

    std::map<u32, u32> mRealStubs;
    std::set<u32> mDisabledImpls;
    s32 mExportCount = 0;
    s32 mHookedCount = 0;
};
