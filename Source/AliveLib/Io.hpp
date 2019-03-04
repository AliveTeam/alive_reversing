#pragma once

#include "FunctionFwd.hpp"
#include <atomic>

struct IO_Handle
{
    int field_0_flags;
    int field_4;
    FILE* field_8_hFile;
    int field_C_last_api_result;
    std::atomic<bool> field_10_bDone; // Note: OG bug - appears to be no thread sync on this
    HANDLE field_14_hThread;
    HANDLE field_18_hEvent;
};
ALIVE_ASSERT_SIZEOF(IO_Handle, 0x1C);

struct IO_Movie_Handle
{
    FILE* field_0_hFile;
    void* field_4_readBuffer;
    DWORD field_8_readSize;
    std::atomic<bool> field_C_bQuit;
    DWORD field_10_read_ret;
    HANDLE field_14_hThread;
    HANDLE field_18_hEvent;
    DWORD field_1C_ThreadId;
};
ALIVE_ASSERT_SIZEOF(IO_Movie_Handle, 0x20);

struct Movie_IO
{
    void(CC* mIO_Close)(void* pHandle);
    BOOL(CC* mIO_Wait)(void* pHandle);
    BOOL(CC* mIO_Seek)(void* pHandle, DWORD offset, DWORD origin);
    void* (CC* mIO_Open)(const char* pFileName);
    BOOL(CC* mIO_Read)(void* pHandle, void* pBuffer, DWORD size);
};

ALIVE_VAR_EXTERN(Movie_IO, sMovie_IO_BBB314);

EXPORT IO_Handle* CC IO_Open_4F2320(const char* fileName, int modeFlag);
EXPORT void CC IO_WaitForComplete_4F2510(IO_Handle* hFile);
EXPORT int CC IO_Seek_4F2490(IO_Handle* hFile, int offset, int origin);
EXPORT void CC IO_fclose_4F24E0(IO_Handle* hFile);
EXPORT DWORD WINAPI FS_IOThread_4F25A0(LPVOID lpThreadParameter);
EXPORT signed int CC IO_Issue_ASync_Read_4F2430(IO_Handle *hFile, int always3, void* readBuffer, size_t bytesToRead, int /*notUsed1*/, int /*notUsed2*/, int /*notUsed3*/);
EXPORT int CC IO_Read_4F23A0(IO_Handle* hFile, void* pBuffer, size_t bytesCount);
EXPORT void IO_Init_494230();


EXPORT void CC IO_Stop_ASync_IO_Thread_4F26B0();
bool IO_CreateThread();
bool IO_DirectoryExists(const char* pDirName);

using TEnumCallBack = void(const char*, DWORD);
void IO_EnumerateDirectory(const char* fileName, TEnumCallBack cb);

ALIVE_VAR_EXTERN(DWORD, sIoThreadId_BBC558);
ALIVE_VAR_EXTERN(BOOL, sIOSyncReads_BD2A5C);
