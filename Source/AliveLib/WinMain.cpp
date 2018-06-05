#include "stdafx.h"
#include "WinMain.hpp"
#include <windows.h>
#include "logger.hpp"

#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP;

int New_WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
    LOG_INFO("New_WinMain");
    return 0;
}
