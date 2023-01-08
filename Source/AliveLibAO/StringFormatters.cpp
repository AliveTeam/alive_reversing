#include "stdafx_ao.h"
#include "StringFormatters.hpp"
#include "Input.hpp"

namespace AO {

static const s32 sInputTable[30] = {
    // NOTE: diversion from OG!
    // the sneak - speak1 and run - speak2 pairs are now decoupled
    // so that they can each be remapped to separate buttons
    InputCommands::eBack,
    InputCommands::eCheatMode,
    InputCommands::eLeftGameSpeak,
    InputCommands::eRightGameSpeak,
    InputCommands::eRun,
    InputCommands::eSneak,
    InputCommands::eHop,
    InputCommands::eDoAction,
    InputCommands::eThrowItem,
    InputCommands::eCrouchOrRoll,
    InputCommands::eUp,
    InputCommands::eDown,
    InputCommands::eLeft,
    InputCommands::eRight,
    0,
    0,
    InputCommands::eUnPause_OrConfirm,
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
    0};

static const char_type* kButtonNamesOrAtlasNums[] = {
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
    kAO_Up,
    kAO_Down,
    kAO_Left,
    kAO_Right,
    kAO_Or,
    kAO_DirectionalButtons,
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
    kAO_Action,
    kAO_Jump_Or_Hello,
    kAO_Jump_Or_Hello,
    kAO_Action,
    kAO_Speak1,
    kAO_Speak2,
    kAO_Run,
    kAO_Sneak,
    kAO_Up,
    kAO_Down,
    kAO_Left,
    kAO_Right,
    kAO_Or,
    kAO_DirectionalButtons,
    "\x16",
    "\x17",
    "\x18",
    "\x19",
    "\x1A", // corruption
    "\x1B", // blank
    "\x1C", // blank
    "\x1D", // blank
    "\x1E", // blank
    "\x1F", // blank
    "\x2D", // todo: might not be part of this
};


void String_FormatString(const char_type* pInput, char_type* pOutput)
{
    char_type* pOutIter = pOutput;
    const char_type* pInIter = pInput;
    while (1)
    {
        const char_type in_char = *pInIter;
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
            const char_type in_char_m6 = in_char - 6;
            const char_type* pConverted = nullptr;

            // NOTE: diversion from OG!
            if (in_char == kAO_ConfirmContinue[0])
            {
                pConverted = Input_GetButtonString(static_cast<InputCommands>(sInputTable[in_char_m6]));
            }
            else if (in_char_m6 < 0 || in_char_m6 >= 14)
            {
                pConverted = kButtonNamesOrAtlasNums[in_char_m6];
            }
            else
            {
                pConverted = Input_GetButtonString(static_cast<InputCommands>(sInputTable[in_char_m6]));
            }

            strcpy(pOutIter, pConverted);

            if (*pOutIter)
            {
                char_type next_char = 0;
                do
                {
                    next_char = (pOutIter++)[1];
                }
                while (next_char);
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

} // namespace AO
