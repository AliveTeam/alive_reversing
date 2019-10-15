#include "stdafx.h"
#include "StringFormatters.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Input.hpp"

void StringFormatters_ForceLink()
{
}

EXPORT const char* CC Input_GetButtonString_492530(const char* /*a1*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return "lol";
}

void CC String_FormatString_4969D0(const char* src, char* dst, int /*bufferLen*/, char bUnknown)
{
    const char* pSrcIter = src;
    char* pDstIter = dst;
    while (*pSrcIter)
    {
        char src_char = *pSrcIter;
        if (src_char <= 25u) // End of medium, first 25 ASCII values are for button mappings?
        {
            const char* pButtonStr = nullptr;
            if (sJoystickEnabled_5C9F70)
            {
                if (bUnknown)
                {
                    pButtonStr = Input_GetButtonString_492530(pSrcIter, 2);
                }
                else
                {
                    pButtonStr = Input_GetButtonString_492530(pSrcIter, 1);
                }
            }
            else
            {
                pButtonStr = Input_GetButtonString_492530(pSrcIter, 0);
            }

            if (pButtonStr)
            {
                strcpy(pDstIter, pButtonStr);
                pDstIter += strlen(pButtonStr);
            }
            ++pSrcIter;
        }
        else
        {
            if (src_char == 26) // Either blank or "4" ?
            {
                *pDstIter++ = 21; // Blank or strange diamond symbol
                ++pSrcIter;
            }
            else if (src_char == 27) // Either blank or "5"
            {
                *pDstIter++ = 20; // Blank or text that looks like "OR"
                ++pSrcIter;
            }
            else
            {
                if (static_cast<BYTE>(src_char) >= 224u) // 244 = always blank/empty
                {
                    src_char += 32;
                }
                *pDstIter++ = src_char;
                ++pSrcIter;
            }
        }
    }

    // Null terminate destination string
    *pDstIter = 0;
}
