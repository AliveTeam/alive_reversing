#include <windows.h>
#include "logger.hpp"
#include "Function.hpp"

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

BOOL WINAPI DllMain(
    _In_ HINSTANCE /*hinstDLL*/,
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
