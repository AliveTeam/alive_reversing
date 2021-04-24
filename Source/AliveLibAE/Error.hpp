#pragma once

#include "FunctionFwd.hpp"

EXPORT void CC Error_DisplayMessageBox_4F2C80(const s8* msg, s32 lineNum, const s8* formatStr, ...);
EXPORT void CC Error_PushErrorRecord_4F2920(const s8* pSourceFileName, s32 lineNumber, s32 minusOne, const s8* errMsg);
void CC Error_NullPrint_4F28C0(const s8* msg);
void CC Error_NullPrint_4F28D0(const s8* msg);
EXPORT void Error_MessageBox_4F2D00(const s8* pFileName, s32 lineNum, const s8* formatStr, ...);
EXPORT void Error_WarningMessageBox_4F2D80(const s8* pWarningMsg, ...);
EXPORT void CC Error_ShowErrorStackToUser_4F2A70(bool bDisplayAsOneMessage);
