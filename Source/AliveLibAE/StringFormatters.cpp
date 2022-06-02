#include "stdafx.h"
#include "StringFormatters.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Input.hpp"

void String_FormatString(const char_type* src, char_type* dst, s32 /*bufferLen*/, s8 isAbeMotionsMenuWithAController)
{
    const char_type* pSrcIter = src;
    char_type* pDstIter = dst;
    while (*pSrcIter)
    {
        char_type src_char = *pSrcIter;
        if (src_char <= 25u) // End of medium, first 25 ASCII values are for button mappings?
        {
            const char_type* pButtonStr = nullptr;
            if (sJoystickEnabled_5C9F70)
            {
                if (isAbeMotionsMenuWithAController) //TODO probably get rid of it, somehow
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
                if (static_cast<u8>(src_char) >= 224u) // 244 = always blank/empty
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
