#include "stdafx_ao.h"
#include "Function.hpp"
#include "StringFormatters.hpp"
#include "Input.hpp"

namespace AO {

const int dword_4CEE78[30] =
{
    2048,
    256,
    6,
    9,
    9,
    6,
    16,
    128,
    32,
    64,
    4096,
    16384,
    32768,
    8192,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

const char* off_4CEDA8[] =
{
    "esc",
    "tab",
    "alt",
    "shift",
    "shift",
    "alt",
    "space",
    "ctrl",
    "Z",
    "X",
    "\x10",
    "\x11",
    "\x12",
    "\x13",
    "\x14",
    "\x15",
    "\x16",
    "\x17",
    "\x18",
    "\x19",
    "\x1A",
    "\x1B",
    "\x1C",
    "\x1D",
    "\x1E",
    "\x1F",
    "esc",
    "tab",
    "\x0D",
    "\x0C",
    "\x0C",
    "\x0D",
    "\x08",
    "\x09",
    "\x0A",
    "\x0B",
    "\x10",
    "\x11",
    "\x12",
    "\x13",
    "\x14",
    "\x15",
    "\x16",
    "\x17",
    "\x18",
    "\x19",
    "\x1A",
    "\x1B",
    "\x1C",
    "\x1D",
    "\x1E",
    "\x1F",
    "\x2D", // todo: might not be part of this
};


EXPORT void CC String_FormatString_450DC0(const char* pInput, char* pOutput)
{
    char* pOutIter = pOutput;
    const char* pInIter = pInput;
    while (1)
    {
        const char in_char = *pInIter;
        if (!*pInIter)
        {
            break;
        }

        if (in_char >= ' ')
        {
            *pOutIter++ = in_char;
            pInIter++;
        }
        else
        {
            const char in_char_m6 = in_char - 6;
            const char* pConverted = nullptr;
            if (in_char_m6 < 0 || in_char_m6 >= 14)
            {
                pConverted = off_4CEDA8[in_char_m6];
            }
            else
            {
                pConverted = Input_GetButtonString_44F1C0(static_cast<InputCommands>(dword_4CEE78[in_char_m6]));
            }

            strcpy(pOutIter, pConverted);

            if (*pOutIter)
            {
                char next_char = 0;
                do
                {
                    next_char = (pOutIter++)[1];
                } while (next_char);
                pInIter++;
            }
            else
            {
                pInIter++;
            }
        }
    }

    // Null terminate destination string
    *pOutIter = 0;
}

}

