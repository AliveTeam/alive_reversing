#include "stdafx.h"
#include "Io.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

#if !_WIN32
#include <dirent.h>
#endif

ALIVE_VAR(1, 0xBBC4BC, std::atomic<IO_Handle*>, sIOHandle_BBC4BC, {});
ALIVE_VAR(1, 0xBBC4C4, std::atomic<void*>, sIO_ReadBuffer_BBC4C4, {});
ALIVE_VAR(1, 0xBBC4C0, std::atomic<int>, sIO_Thread_Operation_BBC4C0, {});
ALIVE_VAR(1, 0xBBC4C8, std::atomic<size_t>, sIO_BytesToRead_BBC4C8, {});
ALIVE_VAR(1, 0xBBC558, DWORD, sIoThreadId_BBC558, 0);

// I/O
ALIVE_VAR(1, 0xBD2A5C, BOOL, sIOSyncReads_BD2A5C, FALSE);
ALIVE_VAR(1, 0xBBC55C, HANDLE, sIoThreadHandle_BBC55C, nullptr);

EXPORT IO_Handle* CC IO_Open_4F2320(const char* fileName, int modeFlag)
{
    IO_Handle* pHandle = reinterpret_cast<IO_Handle*>(malloc_4F4E60(sizeof(IO_Handle)));
    if (!pHandle)
    {
        return nullptr;
    }

    memset(pHandle, 0, sizeof(IO_Handle));

    const char* mode = nullptr;
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
            mode = (const char *)modeFlag;
        }
    }

#if MOBILE
    if (strlen(fileName) >= 3 && fileName[0] == '.' && fileName[1] == '\\')
    {
        fileName += 2;
    }
#endif

#if USE_SDL2_IO
    pHandle->field_8_hFile = SDL_RWFromFile(fileName, mode);
#else
    pHandle->field_8_hFile = fopen(fileName, mode);
#endif

    if (pHandle->field_8_hFile)
    {
        pHandle->field_0_flags = modeFlag;
        return pHandle;
    }
    else
    {
        mem_free_4F4EA0(pHandle);
        return nullptr;
    }
}

EXPORT void CC IO_WaitForComplete_4F2510(IO_Handle* hFile)
{
#if _WIN32
    if (hFile && hFile->field_10_bDone)
    {
        do
        {

            Sleep(0);
        } while (hFile->field_10_bDone);
    }
#endif
}

EXPORT int CC IO_Seek_4F2490(IO_Handle* hFile, int offset, int origin)
{
    if (!hFile)
    {
        return 0;
    }

    if (origin != 1 && origin != 2)
    {
        origin = 0;
    }

#if USE_SDL2_IO
    return static_cast<int>(hFile->field_8_hFile->seek(hFile->field_8_hFile, offset, origin));
#else
    return fseek(hFile->field_8_hFile, offset, origin);
#endif
}

EXPORT void CC IO_fclose_4F24E0(IO_Handle* hFile)
{
    if (hFile)
    {
        IO_WaitForComplete_4F2510(hFile);

#if USE_SDL2_IO
        hFile->field_8_hFile->close(hFile->field_8_hFile);
#else
        fclose(hFile->field_8_hFile);
#endif
        
        mem_free_4F4EA0(hFile);
    }
}

#if _WIN32
EXPORT DWORD WINAPI FS_IOThread_4F25A0(LPVOID /*lpThreadParameter*/)
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

            } while (msg.wParam != 4444 || msg.lParam != 5555 || sIO_Thread_Operation_BBC4C0 != 3);

            if (sIOHandle_BBC4BC.load())
            {
                break;
            }

            sIO_Thread_Operation_BBC4C0 = 0;
        }

#if USE_SDL2_IO
        if (sIOHandle_BBC4BC.load()->field_8_hFile->read(sIOHandle_BBC4BC.load()->field_8_hFile, sIO_ReadBuffer_BBC4C4, 1u, sIO_BytesToRead_BBC4C8) == sIO_BytesToRead_BBC4C8)
#else
        if (fread(sIO_ReadBuffer_BBC4C4, 1u, sIO_BytesToRead_BBC4C8, sIOHandle_BBC4BC.load()->field_8_hFile) == sIO_BytesToRead_BBC4C8)
#endif
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

EXPORT signed int CC IO_Issue_ASync_Read_4F2430(IO_Handle *hFile, int always3, void* readBuffer, size_t bytesToRead, int /*notUsed1*/, int /*notUsed2*/, int /*notUsed3*/)
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
    //sIO_NotUsed1_dword_BBC4CC = notUsed1;
    //sIO_NotUsed2_dword_BBC4D0 = notUsed2;
    //sIO_NotUsed3_dword_BBC4D4 = notUsed3;
    return 0;
}
#endif

EXPORT int CC IO_Read_4F23A0(IO_Handle* hFile, void* pBuffer, size_t bytesCount)
{
    if (!hFile || !hFile->field_8_hFile)
    {
        return -1;
    }

#if _WIN32
    if (hFile->field_0_flags & 4) // ASync flag
    {
        IO_WaitForComplete_4F2510(hFile);
        IO_Issue_ASync_Read_4F2430(hFile, 3, pBuffer, bytesCount, 0, 0, 0);
        ::PostThreadMessageA(sIoThreadId_BBC558, 0x400u, 0x115Cu, 5555); // TODO: Add constants
        return 0;
    }
    else
#endif
    {
#if USE_SDL2_IO
        if (hFile->field_8_hFile->read(hFile->field_8_hFile, pBuffer, 1u, bytesCount) == bytesCount)
#else
        if (fread(pBuffer, 1u, bytesCount, hFile->field_8_hFile) == bytesCount)
#endif
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

ALIVE_VAR(1, 0xbbb314, Movie_IO, sMovie_IO_BBB314, {});

#if _WIN32
EXPORT DWORD CCSTD IO_ASync_Thread_4EAE20(LPVOID lpThreadParameter)
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
#if USE_SDL2_IO
            pHandle->field_10_read_ret = pHandle->field_0_hFile->read(
                pHandle->field_0_hFile,
                pHandle->field_4_readBuffer,
                1u,
                pHandle->field_8_readSize) == pHandle->field_8_readSize;
#else
            pHandle->field_10_read_ret = fread_520B5C(
                pHandle->field_4_readBuffer,
                1u,
                pHandle->field_8_readSize,
                pHandle->field_0_hFile) == pHandle->field_8_readSize;
#endif
            SetEvent(pHandle->field_18_hEvent);
        }
    } while (!pHandle->field_C_bQuit);
    return 0;
}

EXPORT int CC IO_Wait_ASync_4EACF0(void* hFile)
{
    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(hFile);

    DWORD dwRet = WaitForSingleObject(pHandle->field_18_hEvent, 0x3E8u);
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

EXPORT void CC IO_Close_ASync_4EAD40(void* hFile)
{
    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(hFile);
    IO_Wait_ASync_4EACF0(pHandle);
    if (pHandle->field_0_hFile)
    {
#if USE_SDL2_IO
        pHandle->field_0_hFile->close(pHandle->field_0_hFile);
#else
        fclose_520CBE(pHandle->field_0_hFile);
#endif
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
    Mem_Free_495540(pHandle);
}

EXPORT void* CC IO_Open_ASync_4EADA0(const char* filename)
{
    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(malloc_5212C0(sizeof(IO_Movie_Handle)));
    if (!pHandle)
    {
        return nullptr;
    }
    pHandle->field_C_bQuit = 0;
    pHandle->field_14_hThread = CreateThread(0, 0x4000u, IO_ASync_Thread_4EAE20, pHandle, 0, &pHandle->field_1C_ThreadId);
    pHandle->field_18_hEvent = CreateEventA(0, 1, 1, 0);
    pHandle->field_10_read_ret = 1;

#if USE_SDL2_IO
    pHandle->field_0_hFile = SDL_RWFromFile(filename, "rb");
#else
    pHandle->field_0_hFile = fopen_520C64(filename, "rb");
#endif
    
    if (pHandle->field_0_hFile)
    {
        return pHandle;
    }

    IO_Close_ASync_4EAD40(pHandle);
    return nullptr;
}

EXPORT BOOL CC IO_Read_ASync_4EAED0(void* hFile, void* pBuffer, DWORD readSize)
{
    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(hFile);
    if (!IO_Wait_ASync_4EACF0(pHandle))
    {
        return FALSE;
    }

    pHandle->field_4_readBuffer = pBuffer;
    pHandle->field_8_readSize = readSize;
    ResetEvent(pHandle->field_18_hEvent);

    while (!PostThreadMessageA(pHandle->field_1C_ThreadId, 0x400u, 4444, 5555))
    {
        Sleep(200u);
    }

    return TRUE;
}

EXPORT int CC IO_Sync_ASync_4EAF80(void* hFile, DWORD offset, DWORD origin)
{
    IO_Movie_Handle* pHandle = reinterpret_cast<IO_Movie_Handle*>(hFile);
    int result = IO_Wait_ASync_4EACF0(pHandle);
    if (result)
    {
#if USE_SDL2_IO
        result = pHandle->field_0_hFile->seek(pHandle->field_0_hFile, offset, origin) != 0;
#else
        result = fseek_521955(pHandle->field_0_hFile, offset, origin) != 0;
#endif
    }
    return result;
}
#endif

EXPORT void* CC IO_Open_Sync_4EAEB0(const char* pFileName)
{
#if USE_SDL2_IO
    return SDL_RWFromFile(pFileName, "rb");
#else
    return fopen_520C64(pFileName, "rb");
#endif
}

EXPORT void CC IO_Close_Sync_4EAD90(void* pHandle)
{
    IO_FileHandleType hFile = reinterpret_cast<IO_FileHandleType>(pHandle);
    if (hFile)
    {
#if USE_SDL2_IO
        hFile->close(hFile);
#else
        fclose_520CBE(hFile);
#endif   
    }
}

EXPORT BOOL CC IO_Read_Sync_4EAF50(void* pHandle, void* pBuffer, DWORD readSize)
{
    IO_FileHandleType hFile = reinterpret_cast<IO_FileHandleType>(pHandle);

#if USE_SDL2_IO
    return hFile->read(hFile, pBuffer, 1u, readSize) == readSize;
#else
    return fread_520B5C(pBuffer, 1u, readSize, hFile) == readSize;
#endif
    
}

EXPORT BOOL CC IO_Wait_Sync_4EAD30(void*)
{
    return 1;
}

EXPORT BOOL CC IO_Seek_Sync_4EAFC0(void* pHandle, DWORD offset, DWORD origin)
{
    IO_FileHandleType hFile = reinterpret_cast<IO_FileHandleType>(pHandle);

#if USE_SDL2_IO
    return hFile->seek(hFile, offset, origin) != 0;
#else
    return fseek_521955(hFile, offset, origin) != 0;
#endif
}


EXPORT void CC IO_Init_SyncOrASync_4EAC80(int bASync)
{
#if _WIN32
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
    // Don't support ASync on non Windows
    {
        sMovie_IO_BBB314.mIO_Open = IO_Open_Sync_4EAEB0;
        sMovie_IO_BBB314.mIO_Close = IO_Close_Sync_4EAD90;
        sMovie_IO_BBB314.mIO_Read = IO_Read_Sync_4EAF50;
        sMovie_IO_BBB314.mIO_Wait = IO_Wait_Sync_4EAD30;
        sMovie_IO_BBB314.mIO_Seek = IO_Seek_Sync_4EAFC0;
    }
}

EXPORT void* CC IO_fopen_494280(const char* pFileName)
{
    return IO_Open_4F2320(pFileName, 5);
}

EXPORT void CC IO_fclose_4942A0(void* pHandle)
{
    if (pHandle)
    {
        IO_fclose_4F24E0(reinterpret_cast<IO_Handle*>(pHandle));
    }
}

EXPORT BOOL CC IO_request_fread_4942C0(void* pHandle, void* pBuffer, DWORD size)
{
    return IO_Read_4F23A0(reinterpret_cast<IO_Handle*>(pHandle), pBuffer, size) == 0;
}

EXPORT BOOL CC IO_fwait_4942F0(void* pHandle)
{
    IO_Handle* hHandle = reinterpret_cast<IO_Handle*>(pHandle);
    IO_WaitForComplete_4F2510(hHandle);
    return hHandle->field_C_last_api_result == 0;
}

EXPORT void IO_Init_494230()
{
    // NOTE: These are dead given they are instantly overwritten
    sMovie_IO_BBB314.mIO_Open = IO_fopen_494280;
    sMovie_IO_BBB314.mIO_Close = IO_fclose_4942A0;
    sMovie_IO_BBB314.mIO_Read = IO_request_fread_4942C0;
    sMovie_IO_BBB314.mIO_Wait = IO_fwait_4942F0;

    IO_Init_SyncOrASync_4EAC80(TRUE);
}

EXPORT void CC IO_Stop_ASync_IO_Thread_4F26B0()
{
#if _WIN32
    if (sIoThreadHandle_BBC55C)
    {
        ::CloseHandle(sIoThreadHandle_BBC55C);
        sIoThreadHandle_BBC55C = nullptr;
    }
#endif
}

bool IO_CreateThread()
{
#if _WIN32
    if (!sIoThreadHandle_BBC55C)
    {
        sIoThreadHandle_BBC55C = ::CreateThread(
            0,
            0x4000u,
            FS_IOThread_4F25A0,
            0,
            0,
            &sIoThreadId_BBC558);
        if (!sIoThreadHandle_BBC55C)
        {
            return false;
        }
    }
#endif
    return true;
}

bool IO_DirectoryExists(const char* pDirName)
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

static void replace_all(std::string& input, char find, const char replace)
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
        caseSensitive ?
        std::regex_constants::ECMAScript :
        std::regex_constants::ECMAScript | std::regex_constants::icase);

    return std::regex_match(text, pattern);
}
#endif

void IO_EnumerateDirectory(const char* fileName, TEnumCallBack cb)
{
#if _WIN32
    _finddata_t findRec = {};
    intptr_t hFind = _findfirst(fileName, &findRec);
    if (hFind != -1)
    {
        for (;;)
        {
            if (!(findRec.attrib & FILE_ATTRIBUTE_DIRECTORY))
            {
                cb(findRec.name, static_cast<DWORD>(findRec.time_write)); // TODO: Chopping off a lot of time stamp resolution here
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
                    if (stat(( + "./" + itemName).c_str(), &statbuf) == 0)
                    {
                        const bool isFile = !S_ISDIR(statbuf.st_mode);
                        if (isFile)
                        {
                            cb(itemName.c_str(), statbuf.st_mtime);
                        }
                    }
                }
            }
        } while (ent);
        closedir(dir);
    }
#endif
}
