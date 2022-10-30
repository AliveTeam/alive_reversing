#pragma once

#include "../AliveLibCommon/Function.hpp"
#include <atomic>

#if USE_SDL2
    #include "SDL.h"
#endif

#if USE_SDL2_IO
using IO_FileHandleType = struct SDL_RWops*;
#else
using IO_FileHandleType = struct FILE*;
#endif

struct IO_Handle final
{
    s32 mTlvFlags = 0;
    s32 field_4 = 0;
    IO_FileHandleType field_8_hFile = nullptr;
    s32 field_C_last_api_result = 0;
    std::atomic<bool> field_10_bDone = {}; // Note: OG bug - appears to be no thread sync on this
    void* field_14_hThread = 0;
    void* field_18_hEvent = 0;
};
ALIVE_ASSERT_SIZEOF(IO_Handle, 0x1C);

struct IO_Movie_Handle final
{
    IO_FileHandleType field_0_hFile;
    void* field_4_readBuffer;
    u32 field_8_readSize;
    std::atomic<bool> field_C_bQuit;
    u32 field_10_read_ret;
    void* field_14_hThread;
    void* field_18_hEvent;
    u32 field_1C_ThreadId;
};
ALIVE_ASSERT_SIZEOF(IO_Movie_Handle, 0x20);


IO_Handle* IO_Open_4F2320(const char_type* fileName, s32 modeFlag);
void IO_WaitForComplete_4F2510(IO_Handle* hFile);
s32 IO_Seek_4F2490(IO_Handle* hFile, s32 offset, s32 origin);
void IO_fclose_4F24E0(IO_Handle* hFile);
//u32 WINAPI FS_IOThread_4F25A0(LPVOID lpThreadParameter);
s32 IO_Issue_ASync_Read_4F2430(IO_Handle* hFile, s32 always3, void* readBuffer, size_t bytesToRead, s32 /*notUsed1*/, s32 /*notUsed2*/, s32 /*notUsed3*/);
s32 IO_Read_4F23A0(IO_Handle* hFile, void* pBuffer, size_t bytesCount);
void IO_Init_494230();

IO_FileHandleType IO_Open(const char_type* fileName, const char_type* mode);
s32 IO_Seek(IO_FileHandleType pHandle, s32 offset, s32 origin);
s32 IO_Close(IO_FileHandleType pHandle);
size_t IO_Read(IO_FileHandleType pHandle, void* ptr, size_t size, size_t maxnum);


void IO_Stop_ASync_IO_Thread_4F26B0();
bool IO_CreateThread();
bool IO_DirectoryExists(const char_type* pDirName);

using TEnumCallBack = void(const char_type*, u32);
void IO_EnumerateDirectory(const char_type* fileName, TEnumCallBack cb);

extern u32 sIoThreadId_BBC558;
