#include "stdafx.h"
#include "Error.hpp"
#include "Function.hpp"
#include "Sys.hpp"

#define ERROR_IMPL true

struct ErrorData
{
    int field_0_minusOne;
    char field_4_str[256];
    int field_104_line_num;
    const char *field_108_pSourceFileName;
};
ALIVE_ASSERT_SIZEOF(ErrorData, 0x10C);

ALIVE_ARY(1, 0xBBC570, ErrorData, 32, sErrors_BBC570, {});
ALIVE_VAR(1, 0xBBC564, int, sErrorIndex_BBC564, 0);

EXPORT void CC Error_PushErrorRecord_4F2920(const char* pSourceFileName, int lineNumber, int minusOne, const char* errMsg)
{
    if (sErrorIndex_BBC564 == 32)
    {
        // Move everything to the left so that the last item is free
        memcpy(&sErrors_BBC570[0], &sErrors_BBC570[1], sizeof(ErrorData) * 31);
        sErrorIndex_BBC564 = 31;
    }

    // Copy in the message and explicit null terminate it
    strncpy(sErrors_BBC570[sErrorIndex_BBC564].field_4_str, errMsg, sizeof(ErrorData::field_4_str));
    sErrors_BBC570[sErrorIndex_BBC564].field_4_str[ALIVE_COUNTOF(ErrorData::field_4_str) - 1] = 0;

    sErrors_BBC570[sErrorIndex_BBC564].field_0_minusOne = minusOne;
    sErrors_BBC570[sErrorIndex_BBC564].field_104_line_num = lineNumber;
    sErrors_BBC570[sErrorIndex_BBC564].field_108_pSourceFileName = pSourceFileName;

    sErrorIndex_BBC564++;
}

/*EXPORT not big enough to hook*/ void CC Error_NullPrint_4F28C0(const char* msg)
{
    LOG_ERROR(msg);
}

/*EXPORT not big enough to hook*/ void CC Error_NullPrint_4F28D0(const char* msg)
{
    LOG_ERROR(msg);
}

EXPORT void CC Error_DisplayMessageBox_4F2C80(const char* msg, int lineNum, const char* formatStr, ...)
{
    static char sErrorMessage_BBFBA8[2052];
    static char sErrorTitle_BC03B4[2048];

    va_list va;
    va_start(va, formatStr);
    vsprintf(sErrorMessage_BBFBA8, formatStr, va);
    if (lineNum < 0)
    {
        strcpy(sErrorTitle_BC03B4, msg);
    }
    else
    {
        sprintf(sErrorTitle_BC03B4, "%s : %ld", msg, lineNum);
    }
    ::MessageBoxA(Sys_GetHWnd_4F2C70(), sErrorMessage_BBFBA8, sErrorTitle_BC03B4, 0);
}

EXPORT void Error_MessageBox_4F2D00(const char* pFileName, int lineNum, const char* formatStr, ...)
{
    static char sErrorMsg_BBEEFC[2048];
    static char sErrorTitle_BBF6FC[1024];

    va_list va;
    va_start(va, formatStr);
    vsprintf(sErrorMsg_BBEEFC, formatStr, va);

    if (lineNum < 0)
    {
        // Copy file name directly if no line number
        strcpy(sErrorTitle_BBF6FC, pFileName);
    }
    else
    {
        // Else format in the line number after the file name
        sprintf(sErrorTitle_BBF6FC, "%s : %ld", pFileName, lineNum);
    }

    ::MessageBoxA(Sys_GetHWnd_4F2C70(), sErrorMsg_BBEEFC, sErrorTitle_BBF6FC, MB_OK);
}

EXPORT void Error_WarningMessageBox_4F2D80(const char* pWarningMsg, ...)
{
    static char sWarningMsg_BBE6FC[2048];
    va_list va;
    va_start(va, pWarningMsg);
    vsprintf(sWarningMsg_BBE6FC, pWarningMsg, va);
    ::MessageBoxA(Sys_GetHWnd_4F2C70(), sWarningMsg_BBE6FC, "Warning", MB_OK);
}
