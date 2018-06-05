#include <windows.h>
#include <iostream>

bool IsAlive()
{
    return false;
}

int New_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    SetConsoleTitleA("Debug Console");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    return New_WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
