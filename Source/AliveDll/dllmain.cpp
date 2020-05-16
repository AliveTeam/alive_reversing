#include "logger.hpp"
#include "Function.hpp"
#include "ExportHooker.hpp"
#include "../AliveLib/WinMain.hpp"

bool RunningAsInjectedDll()
{
    return true;
}

extern "C"
{
    EXPORT void CC ForceThisDllToLoadInExoddusExe()
    {
        // Never called because:
        // The patched Exoddsue.exe loads this Dll and its WinMain calls this function but:
        // 1. The dll is statically loaded as the EXE has been patched.
        // 2. See that dll main hooks all reimplemented functions.
        // 3. WinMain is reimplemented so the real EXE's WinMain is now a jmp to WinMain_4EE631
        // 4. Therefore the patched WinMain that jmp's to this function is overwritten before 
        // it can ever be executed.
        LOG_ERROR("Entry point was not hooked!");
        Winmain_ForceLink(); // Else the entire AliveLib will be discarded by the linker because there are 0 calls to any of it
        abort();
    }
}


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

        RedirectIoStream(true);

        LOG_INFO("DLL_PROCESS_ATTACH");

        LOG_INFO("Applying detours...");
        ExportHooker hooker(hinstDLL);
        hooker.Apply(false); // Change to true to update decompiled_functions.txt
        LOG_INFO("Detours done");
    }
    else if(fdwReason == DLL_PROCESS_DETACH)
    {
        LOG_INFO("DLL_PROCESS_DETACH");
        
        RedirectIoStream(false);
    }

    return TRUE;
}
