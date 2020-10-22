#include "logger.hpp"
#include "Function.hpp"
#include "ExportHooker.hpp"
#include "../AliveLibAE/WinMain.hpp"

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
        Winmain_ForceLink(); // Else the entire AliveLibAE will be discarded by the linker because there are 0 calls to any of it
        abort();
    }
}

static bool isInjectedDll = true;

bool RunningAsInjectedDll()
{
    static bool aoChecked = false;
    if (!aoChecked)
    {
        auto hAODll = ::GetModuleHandle("AliveDllAO.dll");
        if (hAODll)
        {
            // Do not hook functions and use our own copy of data when being used by the AO dll for core/shared functions
            // trying to hook AE funcs in AO or using AE vars in AO would end badly
            LOG_INFO("AE DLL being used by AO dll for core funcs");
            isInjectedDll = false;
        }
        else
        {
            LOG_INFO("AE DLL running as injected dll");
        }
        aoChecked = true;
    }
    return isInjectedDll;
}


static void AEDllProcessAttach(HINSTANCE hinstDLL)
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

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    /*lpvReserved*/
    )
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        AEDllProcessAttach(hinstDLL);
    }
    else if(fdwReason == DLL_PROCESS_DETACH)
    {
        LOG_INFO("DLL_PROCESS_DETACH");
        
        RedirectIoStream(false);
    }

    return TRUE;
}
