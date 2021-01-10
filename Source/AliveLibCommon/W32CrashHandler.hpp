#pragma once

#ifdef WIN32

#include <windows.h>
#include <Dbghelp.h>
#include <tchar.h>
#include <stdio.h>
#include "SDL.h"
#include "config.h"

#pragma comment (lib, "dbghelp.lib")

inline void create_minidump(PEXCEPTION_POINTERS apExceptionInfo)
{
    wchar_t fileNameW[512] = {};
    char fileNameA[512] = {};

#ifdef BUILD_NUMBER
   _snwprintf(fileNameW, _countof(fileNameW), L"core_pid_%d_build_%S.dmp", ::GetCurrentProcessId(), BUILD_NUMBER);
   _snprintf(fileNameA, _countof(fileNameA), "core_pid_%d_build_%s.dmp", ::GetCurrentProcessId(), BUILD_NUMBER);
#else
   _snwprintf(fileNameW, _countof(fileNameW), L"core_pid_%d.dmp", ::GetCurrentProcessId());
   _snprintf(fileNameA, _countof(fileNameA), "core_pid_%d.dmp", ::GetCurrentProcessId());
#endif

    HANDLE hFile = ::CreateFileW(fileNameW, GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION exceptionInfo = {};
        exceptionInfo.ThreadId = ::GetCurrentThreadId();
        exceptionInfo.ExceptionPointers = apExceptionInfo;
        exceptionInfo.ClientPointers = FALSE;

        const int flags = MiniDumpWithFullMemory |
            MiniDumpWithFullMemoryInfo |
            MiniDumpWithHandleData |
            MiniDumpWithUnloadedModules |
            MiniDumpWithThreadInfo;

        ::MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, static_cast<MINIDUMP_TYPE>(flags), &exceptionInfo, nullptr, nullptr);
        ::CloseHandle(hFile);

#ifdef BUILD_NUMBER
        // Automated AppVeyor build title
        #define TITLE_STR "R.E.L.I.V.E. (AV Build: " BUILD_NUMBER ")"
#else
        #define TITLE_STR "R.E.L.I.V.E."
#endif
        char errMsg[1024] = {};
        _snprintf(errMsg, _countof(errMsg), "R.E.L.I.V.E. has crashed, dump written to %s in the game folder", fileNameA);

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITLE_STR, errMsg, nullptr);
    }
}

inline LONG WINAPI unhandled_handler(PEXCEPTION_POINTERS pExceptionInfo)
{
    create_minidump(pExceptionInfo);
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif

