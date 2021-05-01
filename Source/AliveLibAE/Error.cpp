#include "stdafx.h"
#include "Error.hpp"
#include "Function.hpp"
#include "Sys.hpp"
#include "stdlib.hpp"

struct ErrorData final
{
    s32 field_0_minusOne;
    char_type field_4_str[256];
    s32 field_104_line_num;
    const char_type* field_108_pSourceFileName;
};
ALIVE_ASSERT_SIZEOF(ErrorData, 0x10C);

ALIVE_ARY(1, 0xBBC570, ErrorData, 32, sErrors_BBC570, {});
ALIVE_VAR(1, 0xBBC564, s32, sErrorIndex_BBC564, 0);

EXPORT void CC Error_PushErrorRecord_4F2920(const char_type* pSourceFileName, s32 lineNumber, s32 minusOne, const char_type* errMsg)
{
    if (sErrorIndex_BBC564 == 32)
    {
        // Move everything to the left so that the last item is free
        memcpy(&sErrors_BBC570[0], &sErrors_BBC570[1], sizeof(ErrorData) * 31);
        sErrorIndex_BBC564 = 31;
    }

    // Copy in the message and explicit null terminate it
    strncpy(sErrors_BBC570[sErrorIndex_BBC564].field_4_str, errMsg, sizeof(sErrors_BBC570[sErrorIndex_BBC564].field_4_str));
    sErrors_BBC570[sErrorIndex_BBC564].field_4_str[ALIVE_COUNTOF(sErrors_BBC570[sErrorIndex_BBC564].field_4_str) - 1] = 0;

    sErrors_BBC570[sErrorIndex_BBC564].field_0_minusOne = minusOne;
    sErrors_BBC570[sErrorIndex_BBC564].field_104_line_num = lineNumber;
    sErrors_BBC570[sErrorIndex_BBC564].field_108_pSourceFileName = pSourceFileName;

    sErrorIndex_BBC564++;
}

/*EXPORT not big enough to hook*/ void CC Error_NullPrint_4F28C0(const s8* msg)
{
    LOG_ERROR(msg);
}

/*EXPORT not big enough to hook*/ void CC Error_NullPrint_4F28D0(const char_type* msg)
{
    LOG_ERROR(msg);
}

EXPORT void CC Error_DisplayMessageBox_4F2C80(const char_type* msg, s32 lineNum, const char_type* formatStr, ...)
{
    static char_type sErrorMessage_BBFBA8[2052];
    static char_type sErrorTitle_BC03B4[2048];

    va_list va;
    va_start(va, formatStr);
    vsprintf(sErrorMessage_BBFBA8, formatStr, va);
    if (lineNum < 0)
    {
        strcpy(sErrorTitle_BC03B4, msg);
    }
    else
    {
        sprintf(sErrorTitle_BC03B4, "%s : %d", msg, lineNum);
    }

    Sys_MessageBox(Sys_GetHWnd_4F2C70(), sErrorMessage_BBFBA8, sErrorTitle_BC03B4);
}


EXPORT void Error_MessageBox_4F2D00(const char_type* pFileName, s32 lineNum, const char_type* formatStr, ...)
{
    static char_type sErrorMsg_BBEEFC[2048];
    static char_type sErrorTitle_BBF6FC[1024];

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
        sprintf(sErrorTitle_BBF6FC, "%s : %d", pFileName, lineNum);
    }

    Sys_MessageBox(Sys_GetHWnd_4F2C70(), sErrorMsg_BBEEFC, sErrorTitle_BBF6FC);
}

EXPORT void Error_WarningMessageBox_4F2D80(const char_type* pWarningMsg, ...)
{
    static char_type sWarningMsg_BBE6FC[2048];
    va_list va;
    va_start(va, pWarningMsg);
    vsprintf(sWarningMsg_BBE6FC, pWarningMsg, va);
    Sys_MessageBox(Sys_GetHWnd_4F2C70(), sWarningMsg_BBE6FC, "Warning");
}

EXPORT void CC Error_ShowErrorStackToUser_4F2A70(bool bDisplayAll)
{
    if (!sErrorIndex_BBC564)
    {
        return;
    }

    if (bDisplayAll)
    {
        // Add all errors to one huge message and display it
        char_type* allocatedString = reinterpret_cast<char_type*>(ae_malloc_4F4E60(sErrorIndex_BBC564 * 256));
        if (allocatedString)
        {
            allocatedString[0] = 0;
            for (s32 i = sErrorIndex_BBC564 - 1; i >= 0; i--)
            {
                char_type buffer[256 * 4] = {};
                sprintf(buffer, "%4d %s: %s\n",
                        sErrors_BBC570[sErrorIndex_BBC564].field_104_line_num,
                        sErrors_BBC570[sErrorIndex_BBC564].field_108_pSourceFileName,
                        sErrors_BBC570[sErrorIndex_BBC564].field_4_str);
                strcat(allocatedString, buffer);
            }
            Error_DisplayMessageBox_4F2C80("Error", 0, allocatedString);
            ae_free_4F4EA0(allocatedString);
            sErrorIndex_BBC564 = 0;
        }
    }
    else
    {
        // Show only the last error, next call displays the next error
        for (s32 i = sErrorIndex_BBC564 - 1; i >= 0; i--)
        {
            Error_DisplayMessageBox_4F2C80(
                sErrors_BBC570[sErrorIndex_BBC564].field_108_pSourceFileName,
                sErrors_BBC570[sErrorIndex_BBC564].field_104_line_num,
                sErrors_BBC570[sErrorIndex_BBC564].field_4_str);

            if (!bDisplayAll)
            {
                sErrorIndex_BBC564--;
                break;
            }
        }
    }
}
