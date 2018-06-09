#include "stdafx.h"
#include "Error.hpp"
#include "Function.hpp"

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

void CC Error_PushErrorRecord_4F2920(const char* pSourceFileName, int lineNumber, int minusOne, const char* errMsg)
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
ALIVE_FUNC_IMPLEX(0x4F2920, Error_PushErrorRecord_4F2920, ERROR_IMPL);
