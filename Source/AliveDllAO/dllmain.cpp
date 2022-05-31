#include "logger.hpp"
#include "Function.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"
#include "../AliveLibAO/WinMain.hpp"
#include "GameAutoPlayer.hpp"

extern "C"
{
    void ForceThisDllToLoadInAbeWinExe()
    {
        // Never called because:
        // The patched Exoddus.exe loads this Dll and its WinMain calls this function but:
        // 1. The dll is statically loaded as the EXE has been patched.
        // 2. See that dll main hooks all reimplemented functions.
        // 3. WinMain is reimplemented so the real EXE's WinMain is now a jmp to WinMain_4EE631
        // 4. Therefore the patched WinMain that jmp's to this function is overwritten before
        // it can ever be executed.
        LOG_ERROR("Entry point was not hooked!");
        AO::WinMain_ForceLink(); // Else the entire AliveLibAO will be discarded by the linker because there are 0 calls to any of it
        abort();
    }
}

INITIALIZE_EASYLOGGINGPP

BaseGameAutoPlayer& GetGameAutoPlayer()
{
    static AO::GameAutoPlayer autoPlayer;
    return autoPlayer;
}

static void AODllProcessAttach(HINSTANCE hinstDLL)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    SetConsoleTitleA("Debug Console");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

    RedirectIoStream(true);

    LOG_INFO("DLL_PROCESS_ATTACH");
}

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ u32 fdwReason,
    _In_ LPVOID /*lpvReserved*/
)
{
    AliveLibAO_ForceLink();

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        AODllProcessAttach(hinstDLL);
    }
    else if (fdwReason == DLL_PROCESS_DETACH)
    {
        LOG_INFO("DLL_PROCESS_DETACH");

        RedirectIoStream(false);
    }

    return TRUE;
}
