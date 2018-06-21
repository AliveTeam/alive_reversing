#include <windows.h>
#include <iostream>
#include "WinMain.hpp"
#include "ExportHooker.hpp"

bool IsAlive()
{
    return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    SetConsoleTitleA("Debug Console");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

    ExportHooker hooker(hInstance);
    hooker.Apply();

    return WinMain_4EE631(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
