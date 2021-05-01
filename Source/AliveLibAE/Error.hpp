#pragma once

#include "FunctionFwd.hpp"

EXPORT void CC Error_DisplayMessageBox_4F2C80(const char_type* msg, s32 lineNum, const char_type* formatStr, ...);
EXPORT void CC Error_PushErrorRecord_4F2920(const char_type* pSourceFileName, s32 lineNumber, s32 minusOne, const char_type* errMsg);
void CC Error_NullPrint_4F28C0(const char_type* msg);
void CC Error_NullPrint_4F28D0(const char_type* msg);
EXPORT void Error_MessageBox_4F2D00(const char_type* pFileName, s32 lineNum, const s8* formatStr, ...);
EXPORT void Error_WarningMessageBox_4F2D80(const char_type* pWarningMsg, ...);
EXPORT void CC Error_ShowErrorStackToUser_4F2A70(bool bDisplayAsOneMessage);
