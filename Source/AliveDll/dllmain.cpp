#include <windows.h>
#include "logger.hpp"
#include "Function.hpp"
#include "detours.h"

bool IsAlive()
{
    return true;
}

class outbuf : public std::streambuf
{
public:
    outbuf()
    {
        setp(0, 0);
    }

    virtual int_type overflow(int_type c = traits_type::eof()) override
    {
        return fputc(c, stdout) == EOF ? traits_type::eof() : c;
    }
};

outbuf ob;
std::streambuf *sb = nullptr;

extern "C"
{
    __declspec(dllexport) void __cdecl ForceThisDllToLoadInExoddusExe()
    {
        // Never called because:
        // The patched Exoddsue.exe loads this Dll and its WinMain calls this function but:
        // 1. The dll is statically loaded as the EXE has been patched.
        // 2. See that dll main hooks all reimplemented functions.
        // 3. WinMain is reimplemented so the real EXE's WinMain is now a jmp to WinMain_4EE631
        // 4. Therefore the patched WinMain that jmp's to this function is overwritten before 
        // it can ever be executed.
    }
}

class ExportHooker
{
public:
    explicit ExportHooker(HINSTANCE instance) : mhInstance(instance)
    {
        mExports.reserve(5000);
    }

    void Apply()
    {
        if (!DetourEnumerateExports(mhInstance, this, EnumExports))
        {
            ALIVE_FATAL("Export enumeration failed");
        }
        ProcessExports();
    }

private:
    void ProcessExports()
    {
        TRACE_ENTRYEXIT;
        LONG err = DetourTransactionBegin();

        if (err != NO_ERROR)
        {
            ALIVE_FATAL("DetourTransactionBegin failed");
        }

        err = DetourUpdateThread(GetCurrentThread());

        if (err != NO_ERROR)
        {
            ALIVE_FATAL("DetourUpdateThread failed");
        }

        for (const auto& e : mExports)
        {
            LOG_INFO("Hook: "
                << e.mName.c_str()
                << " From "
                << "0x" << std::hex << (e.mIsImplemented ? e.mGameFunctionAddr : (DWORD)e.mCode)
                << " To "
                << "0x" << std::hex << (e.mIsImplemented ? (DWORD)e.mCode : e.mGameFunctionAddr)
                << " Implemented: " << e.mIsImplemented);

            if (e.mIsImplemented)
            {
                err = DetourAttach(&(PVOID&)e.mGameFunctionAddr, e.mCode);
            }
            else
            {
                err = DetourAttach(&(PVOID&)e.mCode, (PVOID)e.mGameFunctionAddr);
            }

            if (err != NO_ERROR)
            {
                ALIVE_FATAL("DetourAttach failed");
            }
        }

        err = DetourTransactionCommit();
        if (err != NO_ERROR)
        {
            ALIVE_FATAL("DetourTransactionCommit failed");
        }
    }

    static bool IsHexDigit(char letter)
    {
        if (letter >= '0' && letter <= '9')
        {
            return true;
        }
        const char lower = ::tolower(letter);
        return (lower >= 'a' && lower <= 'f');
    }

    static bool IsImplemented(PVOID pCode)
    {
        // 4 nops, int 3, 4 nops
        const static BYTE kPatternToFind[] = { 0x90, 0x90, 0x90, 0x90, 0xCC, 0x90, 0x90, 0x90, 0x90};
        BYTE codeBuffer[32] = {};
        memcpy(codeBuffer, pCode, sizeof(codeBuffer));
        for (int i = 0; i < sizeof(codeBuffer) - sizeof(kPatternToFind); i++)
        {
            if (codeBuffer[i] == kPatternToFind[0])
            {
                if (memcmp(&codeBuffer[i], kPatternToFind, sizeof(kPatternToFind)) == 0)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void OnExport(PCHAR pszName, PVOID pCode)
    {
        std::string name(pszName);
        auto underScorePos = name.find_first_of('_');
        while (underScorePos != std::string::npos)
        {
            int hexNumLen = 0;
            for (size_t i = underScorePos + 1; i < name.length(); i++)
            {
                if (IsHexDigit(name[i]))
                {
                    hexNumLen++;
                }
                else
                {
                    break;
                }
            }

            if (hexNumLen >= 6 && hexNumLen <= 8)
            {
                unsigned long addr = std::stoul(name.substr(underScorePos + 1, hexNumLen), nullptr, 16);
                
                mExports.push_back({ name, pCode, addr, IsImplemented(pCode) });
                return;
            }

            underScorePos = name.find('_', underScorePos + 1);
        }
    }

    static BOOL CALLBACK EnumExports(PVOID pContext,
        ULONG /*nOrdinal*/,
        PCHAR pszName,
        PVOID pCode)
    {
        if (pszName && pCode)
        {
            // Resolve 1 level long jumps, not using DetourCodeFromPointer
            // as it appears to have a bug where it checks for 0xeb before 0xe9 and so
            // won't skip jmps that start with long jmps.
            BYTE* pbCode = (BYTE*)pCode;
            if (pbCode[0] == 0xe9)
            {
                // jmp +imm32
                PBYTE pbNew = pbCode + 5 + *(DWORD *)&pbCode[1];
                pCode = pbNew;
            }
            reinterpret_cast<ExportHooker*>(pContext)->OnExport(pszName, pCode);
        }
        return TRUE;
    }

    HINSTANCE mhInstance = nullptr;
    struct Export
    {
        std::string mName;
        LPVOID mCode;
        DWORD mGameFunctionAddr;
        bool mIsImplemented;
    };
    std::vector<Export> mExports;
};

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    /*lpvReserved*/
    )
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        SetConsoleTitleA("Debug Console");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

        // set std::cout to use my custom streambuf
        sb = std::cout.rdbuf(&ob);

        LOG_INFO("DLL_PROCESS_ATTACH");

        LOG_INFO("Applying detours...");
        ExportHooker hooker(hinstDLL);
        hooker.Apply();
        AliveFunctionBase::ApplyFunctions();
        LOG_INFO("Detours done");
    }
    else if(fdwReason == DLL_PROCESS_DETACH)
    {
        LOG_INFO("DLL_PROCESS_DETACH");
        
        // make sure to restore the original so we don't get a crash on close!
        if (sb)
        {
            std::cout.rdbuf(sb);
        }
    }

    return TRUE;
}
