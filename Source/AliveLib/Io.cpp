#include "stdafx.h"
#include "Io.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0xBBC4BC, std::atomic<IO_Handle*>, sIOHandle_BBC4BC, {});
ALIVE_VAR(1, 0xBBC4C4, std::atomic<void*>, sIO_ReadBuffer_BBC4C4, {});
ALIVE_VAR(1, 0xBBC4C0, std::atomic<int>, sIO_Thread_Operation_BBC4C0, {});
ALIVE_VAR(1, 0xBBC4C8, std::atomic<size_t>, sIO_BytesToRead_BBC4C8, {});
ALIVE_VAR(1, 0xBBC558, DWORD, sIoThreadId_BBC558, 0);


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

    pHandle->field_8_hFile = fopen(fileName, mode);
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
    if (hFile && hFile->field_10_bDone)
    {
        do
        {
            Sleep(0);
        } while (hFile->field_10_bDone);
    }
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
    return fseek(hFile->field_8_hFile, offset, origin);
}

EXPORT void CC IO_fclose_4F24E0(IO_Handle* hFile)
{
    if (hFile)
    {
        IO_WaitForComplete_4F2510(hFile);
        fclose(hFile->field_8_hFile);
        mem_free_4F4EA0(hFile);
    }
}

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

        if (fread(sIO_ReadBuffer_BBC4C4, 1u, sIO_BytesToRead_BBC4C8, sIOHandle_BBC4BC.load()->field_8_hFile) == sIO_BytesToRead_BBC4C8)
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

EXPORT int CC IO_Read_4F23A0(IO_Handle* hFile, void* pBuffer, size_t bytesCount)
{
    if (!hFile || !hFile->field_8_hFile)
    {
        return -1;
    }

    if (hFile->field_0_flags & 4) // ASync flag
    {
        IO_WaitForComplete_4F2510(hFile);
        IO_Issue_ASync_Read_4F2430(hFile, 3, pBuffer, bytesCount, 0, 0, 0);
        ::PostThreadMessageA(sIoThreadId_BBC558, 0x400u, 0x115Cu, 5555); // TODO: Add constants
        return 0;
    }
    else
    {
        if (fread(pBuffer, 1u, bytesCount, hFile->field_8_hFile) == bytesCount)
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
