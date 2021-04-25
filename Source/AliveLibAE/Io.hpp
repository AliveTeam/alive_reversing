#pragma once

#include "FunctionFwd.hpp"
#include <atomic>

#if USE_SDL2
#include "SDL.h"
#endif

#if USE_SDL2_IO
using IO_FileHandleType = struct SDL_RWops*;
#else
using IO_FileHandleType = struct FILE*;
#endif

struct IO_Handle
{
    s32 field_0_flags;
    s32 field_4;
    IO_FileHandleType field_8_hFile;
    s32 field_C_last_api_result;
    std::atomic<bool> field_10_bDone; // Note: OG bug - appears to be no thread sync on this
    HANDLE field_14_hThread;
    HANDLE field_18_hEvent;
};
ALIVE_ASSERT_SIZEOF(IO_Handle, 0x1C);

struct IO_Movie_Handle
{
    IO_FileHandleType field_0_hFile;
    void* field_4_readBuffer;
    u32 field_8_readSize;
    std::atomic<bool> field_C_bQuit;
    u32 field_10_read_ret;
    HANDLE field_14_hThread;
    HANDLE field_18_hEvent;
    u32 field_1C_ThreadId;
};
ALIVE_ASSERT_SIZEOF(IO_Movie_Handle, 0x20);


EXPORT IO_Handle* CC IO_Open_4F2320(const s8* fileName, s32 modeFlag);
EXPORT void CC IO_WaitForComplete_4F2510(IO_Handle* hFile);
EXPORT s32 CC IO_Seek_4F2490(IO_Handle* hFile, s32 offset, s32 origin);
EXPORT void CC IO_fclose_4F24E0(IO_Handle* hFile);
EXPORT u32 WINAPI FS_IOThread_4F25A0(LPVOID lpThreadParameter);
EXPORT s32 CC IO_Issue_ASync_Read_4F2430(IO_Handle *hFile, s32 always3, void* readBuffer, size_t bytesToRead, s32 /*notUsed1*/, s32 /*notUsed2*/, s32 /*notUsed3*/);
EXPORT s32 CC IO_Read_4F23A0(IO_Handle* hFile, void* pBuffer, size_t bytesCount);
EXPORT void IO_Init_494230();

IO_FileHandleType IO_Open(const s8* fileName, const s8 * mode);
s32 IO_Seek(IO_FileHandleType pHandle, s32 offset, s32 origin);
s32 IO_Close(IO_FileHandleType pHandle);
size_t IO_Read(IO_FileHandleType pHandle, void *ptr, size_t size, size_t maxnum);


EXPORT void CC IO_Stop_ASync_IO_Thread_4F26B0();
bool IO_CreateThread();
bool IO_DirectoryExists(const s8* pDirName);

using TEnumCallBack = void(const s8*, u32);
void EXPORT IO_EnumerateDirectory(const s8* fileName, TEnumCallBack cb);

ALIVE_VAR_EXTERN(u32, sIoThreadId_BBC558);
ALIVE_VAR_EXTERN(BOOL, sIOSyncReads_BD2A5C);
