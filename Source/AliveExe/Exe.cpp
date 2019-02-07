#include <windows.h>
#include <iostream>
#include "WinMain.hpp"
#include "ExportHooker.hpp"

bool IsAlive()
{
    return false;
}

#if _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    SetConsoleTitleA("Debug Console");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);
    ExportHooker hooker(hInstance);
    hooker.Apply();

    // In the real game these are called before main, but shouldn't really matter in this case
    Static_Inits();

    return WinMain_4EE631(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
#else
int main()
{
    // TOOD: Convert command line arguments to what WinMain_4EE631 expects
    return WinMain_4EE631(0, 0, 0, 1);
}
#endif
