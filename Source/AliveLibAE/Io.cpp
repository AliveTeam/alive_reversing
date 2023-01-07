#include "stdafx.h"
#include "../relive_lib/relive_config.h"
#include "Io.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Masher.hpp"
#include "../relive_lib/FatalError.hpp"

#if !_WIN32
    #include <dirent.h>
    #include <sys/stat.h>
#endif

// TODO: Remove this
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif

std::atomic<IO_Handle*> sIOHandle_BBC4BC = {};
std::atomic<void*> sIO_ReadBuffer_BBC4C4 = {};
std::atomic<s32> sIO_Thread_Operation_BBC4C0 = {};
std::atomic<size_t> sIO_BytesToRead_BBC4C8 = {};
u32 sIoThreadId_BBC558 = 0;

// I/O
void* sIoThreadHandle_BBC55C = nullptr;


// SDL/C IO Wrappers
IO_FileHandleType IO_Open(const char_type* fileName, const char_type* mode)
{
    if (strlen(fileName) >= 3 && fileName[0] == '.' && (fileName[1] == '/' || fileName[1] == '\\'))
    {
        fileName += 2;
    }

#if USE_SDL2_IO
    return SDL_RWFromFile(fileName, mode);
#else
    return ::fopen(fileName, mode);
#endif
}

s32 IO_Seek(IO_FileHandleType pHandle, s32 offset, s32 origin)
{
#if USE_SDL2_IO
    return static_cast<s32>(pHandle->seek(pHandle, offset, origin));
#else
    return ::fseek(pHandle, offset, origin);
#endif
}

s32 IO_Close(IO_FileHandleType pHandle)
{
#if USE_SDL2_IO
    return pHandle->close(pHandle);
#else
    return ::fclose(pHandle);
#endif
}

size_t IO_Read(IO_FileHandleType pHandle, void* ptr, size_t size, size_t maxnum)
{
#if USE_SDL2_IO
    return pHandle->read(pHandle, ptr, size, maxnum);
#else
    return ae_fread_520B5C(ptr, size, maxnum, pHandle);
#endif
}

IO_Handle* IO_Open_4F2320(const char_type* fileName, s32 modeFlag)
{
    IO_Handle* pHandle = relive_new IO_Handle();
    if (!pHandle)
    {
        return nullptr;
    }

    const char_type* mode = nullptr;
    if ((modeFlag & 3) == 3)
    {
        mode = "rwb";
    }
    else if (modeFlag & 1)
    {
        mode = "rb";
    }
    else
    {
        mode = "wb";
        if (!(modeFlag & 2))
        {
            // Somehow it can also be passed as string?? I don't think this case ever happens
            //mode = reinterpret_cast<const s8*>(modeFlag);
            ALIVE_FATAL("Unknown mode flag %d", modeFlag);
        }
    }

    pHandle->field_8_hFile = IO_Open(fileName, mode);

    if (pHandle->field_8_hFile)
    {
        pHandle->mTlvFlags = modeFlag;
        return pHandle;
    }
    else
    {
        relive_delete pHandle;
        return nullptr;
    }
}

void IO_WaitForComplete_4F2510(IO_Handle* hFile)
{
#if _WIN32 && !USE_SDL2_IO
    if (hFile && hFile->field_10_bDone)
    {
        do
        {
            Sleep(0);
        }
        while (hFile->field_10_bDone);
    }
#else
    (void) hFile;
#endif
}

s32 IO_Seek_4F2490(IO_Handle* hFile, s32 offset, s32 origin)
{
    if (!hFile)
    {
        return 0;
    }

    if (origin != 1 && origin != 2)
    {
        origin = 0;
    }

    return IO_Seek(hFile->field_8_hFile, offset, origin);
}

void IO_fclose_4F24E0(IO_Handle* hFile)
{
    if (hFile)
    {
        IO_WaitForComplete_4F2510(hFile);
        IO_Close(hFile->field_8_hFile);
        relive_delete hFile;
    }
}

#if _WIN32 && !USE_SDL2_IO
u32 WINAPI FS_IOThread_4F25A0(LPVOID /*lpThreadParameter*/)
{
    while (1)
    {
        while (1)
        {
            // Wait for a control message
            MSG msg = {};
            do
            {
                while (GetMessageA(&msg, 0, 0x400u, 0x400u) == -1)
                {
                }
            }
            while (msg.wParam != 4444 || msg.lParam != 5555 || sIO_Thread_Operation_BBC4C0 != 3);

            if (sIOHandle_BBC4BC.load())
            {
                break;
            }

            sIO_Thread_Operation_BBC4C0 = 0;
        }

        if (IO_Read(sIOHandle_BBC4BC.load()->field_8_hFile, sIO_ReadBuffer_BBC4C4, 1u, sIO_BytesToRead_BBC4C8) == sIO_BytesToRead_BBC4C8)
        {
            sIOHandle_BBC4BC.load()->field_C_last_api_result = 0;
        }
        else
        {
            sIOHandle_BBC4BC.load()->field_C_last_api_result = -1;
        }

        sIOHandle_BBC4BC.load()->field_10_bDone = false;
        sIOHandle_BBC4BC = nullptr;
        sIO_Thread_Operation_BBC4C0 = 0;
    }
}

s32 IO_Issue_ASync_Read_4F2430(IO_Handle* hFile, s32 always3, void* readBuffer, size_t bytesToRead, s32 /*notUsed1*/, s32 /*notUsed2*/, s32 /*notUsed3*/)
{
    if (sIOHandle_BBC4BC.load())
    {
        return -1;
    }

    hFile->field_10_bDone = true;

    // TODO: Should be made atomic for thread safety.
    sIOHandle_BBC4BC = hFile;
    sIO_ReadBuffer_BBC4C4 = readBuffer;
    sIO_Thread_Operation_BBC4C0 = always3;
    sIO_BytesToRead_BBC4C8 = bytesToRead;
    //sIO_NotUsed1_dword_BBC4= notUsed1;
    //sIO_NotUsed2_dword_BBC4D0 = notUsed2;
    //sIO_NotUsed3_dword_BBC4D4 = notUsed3;
    return 0;
}
#endif

s32 IO_Read_4F23A0(IO_Handle* hFile, void* pBuffer, size_t bytesCount)
{
    if (!hFile || !hFile->field_8_hFile)
    {
        return -1;
    }

#if _WIN32 && !USE_SDL2_IO
    if (hFile->mTlvFlags & 4) // ASync flag
    {
        IO_WaitForComplete_4F2510(hFile);
        IO_Issue_ASync_Read_4F2430(hFile, 3, pBuffer, bytesCount, 0, 0, 0);
        ::PostThreadMessageA(sIoThreadId_BBC558, 0x400u, 0x115Cu, 5555); // TODO: Add constants
        return 0;
    }
    else
#endif
    {
        if (IO_Read(hFile->field_8_hFile, pBuffer, 1u, bytesCount) == bytesCount)
        {
            hFile->field_C_last_api_result = 0;
        }
        else
        {
            hFile->field_C_last_api_result = -1;
        }

        return hFile->field_C_last_api_result;
    }
}


#if _WIN32 && !USE_SDL2_IO
u32 IO_ASync_Thread_4EAE20(LPVOID lpThreadParameter)
{
    MSG msg = {};

    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(lpThreadParameter);

    if (pHandle->field_C_bQuit)
    {
        return 0;
    }

    do
    {
        if (GetMessageA(&msg, 0, 0x400u, 0x400u) != -1 && msg.wParam == 4444 && msg.lParam == 5555)
        {
            pHandle->field_10_read_ret = IO_Read(pHandle->field_0_hFile, pHandle->field_4_readBuffer, 1u, pHandle->field_8_readSize) == pHandle->field_8_readSize;
            SetEvent(pHandle->field_18_hEvent);
        }
    }
    while (!pHandle->field_C_bQuit);
    return 0;
}

s32 IO_Wait_ASync_4EACF0(void* hFile)
{
    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(hFile);

    u32 dwRet = WaitForSingleObject(pHandle->field_18_hEvent, 0x3E8u);
    if (!dwRet)
    {
        return pHandle->field_10_read_ret;
    }

    while (dwRet == 258)
    {
        dwRet = WaitForSingleObject(pHandle->field_18_hEvent, 0x3E8u);
        if (!dwRet)
        {
            return pHandle->field_10_read_ret;
        }
    }
    return 0;
}

void IO_Close_ASync_4EAD40(void* hFile)
{
    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(hFile);
    IO_Wait_ASync_4EACF0(pHandle);
    if (pHandle->field_0_hFile)
    {
        IO_Close(pHandle->field_0_hFile);
    }
    pHandle->field_C_bQuit = 1;
    if (pHandle->field_14_hThread)
    {
        CloseHandle(pHandle->field_14_hThread);
    }
    if (pHandle->field_18_hEvent)
    {
        CloseHandle(pHandle->field_18_hEvent);
    }
    relive_delete pHandle;
}

void* IO_Open_ASync_4EADA0(const s8* filename)
{
    IO_Movie_Handle* pHandle = relive_new IO_Movie_Handle();
    if (!pHandle)
    {
        return nullptr;
    }
    pHandle->field_C_bQuit = 0;
    pHandle->field_14_hThread = CreateThread(0, 0x4000u, IO_ASync_Thread_4EAE20, pHandle, 0, &pHandle->field_1C_ThreadId);
    pHandle->field_18_hEvent = CreateEventA(0, 1, 1, 0);
    pHandle->field_10_read_ret = 1;


    pHandle->field_0_hFile = IO_Open(filename, "rb");

    if (pHandle->field_0_hFile)
    {
        return pHandle;
    }

    IO_Close_ASync_4EAD40(pHandle);
    return nullptr;
}

bool IO_Read_ASync_4EAED0(void* hFile, void* pBuffer, u32 readSize)
{
    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(hFile);
    if (!IO_Wait_ASync_4EACF0(pHandle))
    {
        return false;
    }

    pHandle->field_4_readBuffer = pBuffer;
    pHandle->field_8_readSize = readSize;
    ResetEvent(pHandle->field_18_hEvent);

    while (!PostThreadMessageA(pHandle->field_1C_ThreadId, 0x400u, 4444, 5555))
    {
        Sleep(200u);
    }

    return true;
}

s32 IO_Sync_ASync_4EAF80(void* hFile, u32 offset, u32 origin)
{
    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(hFile);
    s32 result = IO_Wait_ASync_4EACF0(pHandle);
    if (result)
    {
        result = IO_Seek(pHandle->field_0_hFile, offset, origin) != 0;
    }
    return result;
}
#endif

void* IO_Open_Sync_4EAEB0(const char_type* pFileName)
{
    return IO_Open(pFileName, "rb");
}

void IO_Close_Sync_4EAD90(void* pHandle)
{
    IO_FileHandleType hFile = reinterpret_cast<IO_FileHandleType>(pHandle);
    if (hFile)
    {
        IO_Close(hFile);
    }
}

bool IO_Read_Sync_4EAF50(void* pHandle, void* pBuffer, u32 readSize)
{
    IO_FileHandleType hFile = reinterpret_cast<IO_FileHandleType>(pHandle);
    return IO_Read(hFile, pBuffer, 1u, readSize) == readSize;
}

bool IO_Wait_Sync_4EAD30(void*)
{
    return 1;
}

bool IO_Seek_Sync_4EAFC0(void* pHandle, u32 offset, u32 origin)
{
    IO_FileHandleType hFile = reinterpret_cast<IO_FileHandleType>(pHandle);
    return IO_Seek(hFile, offset, origin) != 0;
}


void IO_Init_SyncOrASync_4EAC80(s32 bASync)
{
#if _WIN32 && !USE_SDL2_IO
    if (bASync)
    {
        sMovie_IO_BBB314.mIO_Open = IO_Open_ASync_4EADA0;
        sMovie_IO_BBB314.mIO_Close = IO_Close_ASync_4EAD40;
        sMovie_IO_BBB314.mIO_Read = IO_Read_ASync_4EAED0;
        sMovie_IO_BBB314.mIO_Wait = IO_Wait_ASync_4EACF0;
        sMovie_IO_BBB314.mIO_Seek = IO_Sync_ASync_4EAF80;
    }
    else
#endif
    // Don't support ASync on non Windows or when using SDL IO
    {
        (void) bASync;

        GetMovieIO().mIO_Open = IO_Open_Sync_4EAEB0;
        GetMovieIO().mIO_Close = IO_Close_Sync_4EAD90;
        GetMovieIO().mIO_Read = IO_Read_Sync_4EAF50;
        GetMovieIO().mIO_Wait = IO_Wait_Sync_4EAD30;
        GetMovieIO().mIO_Seek = IO_Seek_Sync_4EAFC0;
    }
}

void* IO_fopen_494280(const char_type* pFileName)
{
    return IO_Open_4F2320(pFileName, 5);
}

void IO_fclose_4942A0(void* pHandle)
{
    if (pHandle)
    {
        IO_fclose_4F24E0(reinterpret_cast<IO_Handle*>(pHandle));
    }
}

bool IO_request_fread_4942C0(void* pHandle, void* pBuffer, u32 size)
{
    return IO_Read_4F23A0(reinterpret_cast<IO_Handle*>(pHandle), pBuffer, size) == 0;
}

bool IO_fwait_4942F0(void* pHandle)
{
    IO_Handle* hHandle = reinterpret_cast<IO_Handle*>(pHandle);
    IO_WaitForComplete_4F2510(hHandle);
    return hHandle->field_C_last_api_result == 0;
}

void IO_Init_494230()
{
    // NOTE: These are dead given they are instantly overwritten
    GetMovieIO().mIO_Open = IO_fopen_494280;
    GetMovieIO().mIO_Close = IO_fclose_4942A0;
    GetMovieIO().mIO_Read = IO_request_fread_4942C0;
    GetMovieIO().mIO_Wait = IO_fwait_4942F0;

    IO_Init_SyncOrASync_4EAC80(true);
}

void IO_Stop_ASync_IO_Thread_4F26B0()
{
#if _WIN32 && !USE_SDL2_IO
    if (sIoThreadHandle_BBC55C)
    {
        ::CloseHandle(sIoThreadHandle_BBC55C);
        sIoThreadHandle_BBC55C = nullptr;
    }
#endif
}


bool IO_DirectoryExists(const char_type* pDirName)
{
#if _WIN32
    WIN32_FIND_DATA sFindData = {};
    HANDLE hFind = FindFirstFile(pDirName, &sFindData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    FindClose(hFind);
    return true;
#else
    DIR* dir = opendir(pDirName);
    if (dir)
    {
        closedir(dir);
        return true;
    }
    return false;
#endif
}

#if !_WIN32
    #include <string>
    #include <regex>

[[maybe_unused]] static void replace_all(std::string& input, s8 find, const s8 replace)
{
    size_t pos = 0;
    while ((pos = input.find(find, pos)) != std::string::npos)
    {
        input.replace(pos, 1, 1, replace);
        pos += 1;
    }
}

static void replace_all(std::string& input, const std::string& find, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = input.find(find, pos)) != std::string::npos)
    {
        input.replace(pos, find.length(), replace);
        pos += replace.length();
    }
}

static void EscapeRegex(std::string& regex)
{
    replace_all(regex, "\\", "\\\\");
    replace_all(regex, "^", "\\^");
    replace_all(regex, ".", "\\.");
    replace_all(regex, "$", "\\$");
    replace_all(regex, "|", "\\|");
    replace_all(regex, "(", "\\(");
    replace_all(regex, ")", "\\)");
    replace_all(regex, "[", "\\[");
    replace_all(regex, "]", "\\]");
    replace_all(regex, "*", "\\*");
    replace_all(regex, "+", "\\+");
    replace_all(regex, "?", "\\?");
    replace_all(regex, "/", "\\/");
}

static bool WildCardMatcher(const std::string& text, std::string wildcardPattern, bool caseSensitive)
{
    // Escape all regex special chars
    EscapeRegex(wildcardPattern);

    // Convert chars '*?' back to their regex equivalents
    replace_all(wildcardPattern, "\\?", ".");
    replace_all(wildcardPattern, "\\*", ".*");

    std::regex pattern(wildcardPattern,
                       caseSensitive ? std::regex_constants::ECMAScript : std::regex_constants::ECMAScript | std::regex_constants::icase);

    return std::regex_match(text, pattern);
}
#endif

void IO_EnumerateDirectory(const char_type* fileName, TEnumCallBack cb)
{
    TRACE_ENTRYEXIT;

#if _WIN32
    _finddata_t findRec = {};
    intptr_t hFind = _findfirst(fileName, &findRec);
    if (hFind != -1)
    {
        for (;;)
        {
            if (!(findRec.attrib & FILE_ATTRIBUTE_DIRECTORY))
            {
                cb(findRec.name, static_cast<u32>(findRec.time_write)); // TODO: Chopping off a lot of time stamp resolution here
            }

            if (_findnext(hFind, &findRec) == -1)
            {
                break;
            }
        }
        _findclose(hFind);
    }
#else
    DIR* dir(opendir("."));
    if (dir)
    {
        dirent* ent = nullptr;
        do
        {
            ent = readdir(dir);
            if (ent)
            {
                const std::string itemName = ent->d_name;
                const std::string strFilter(fileName);
                if (WildCardMatcher(itemName, strFilter, true))
                {
                    struct stat statbuf;
                    if (stat((+"./" + itemName).c_str(), &statbuf) == 0)
                    {
                        const bool isFile = !S_ISDIR(statbuf.st_mode);
                        if (isFile)
                        {
                            cb(itemName.c_str(), statbuf.st_mtime);
                        }
                    }
                }
            }
        }
        while (ent);
        closedir(dir);
    }
#endif
}
