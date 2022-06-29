#pragma once

#include "../AliveLibCommon/Function.hpp"

void Error_DisplayMessageBox_4F2C80(const char_type* msg, s32 lineNum, const char_type* formatStr, ...);
void Error_PushErrorRecord_4F2920(const char_type* pSourceFileName, s32 lineNumber, s32 minusOne, const char_type* errMsg);
void Error_NullPrint_4F28C0(const char_type* msg);
void Error_NullPrint_4F28D0(const char_type* msg);
void Error_MessageBox_4F2D00(const char_type* pFileName, s32 lineNum, const s8* formatStr, ...);
void Error_WarningMessageBox(const char_type* pWarningMsg, ...);
void Error_ShowErrorStackToUser(bool bDisplayAsOneMessage);
