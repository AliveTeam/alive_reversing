#include "stdafx.h"
#include "Input.hpp"
#include <windows.h>
#include "logger.hpp"
#include "Function.hpp"
#include "easylogging++.h"
#include "Game.hpp"
#include "Sys.hpp"
#include "DebugHelpers.hpp"
#include <joystickapi.h>
#include "Events.hpp"
#include "PsxRender.hpp"

#if XINPUT_SUPPORT
#include <Xinput.h>
#endif

#define INPUT_IMPL true

// -- Variables -- //

ALIVE_VAR(1, 0x5C2EF4, bool, sJoystickEnabled_5C2EF4, false);
ALIVE_VAR(1, 0x5C2EFC, int, sJoystickNumButtons_5C2EFC, 0);
ALIVE_VAR(1, 0x5C2F00, int, sJoystickID_5C2F00, 0);
ALIVE_VAR(1, 0x5c2edc, int, sJoystickCapFlags_5C2EDC, 0);
ALIVE_VAR(1, 0x5c2ee0, bool, sJoyStateIsInit_5C2EE0, 0);
ALIVE_VAR(1, 0x5c2eec, int, sJoyLastTick_5C2EEC, 0);
ALIVE_VAR(1, 0x5C2EF8, int, dword_5C2EF8, 0);
ALIVE_VAR(1, 0x5c2d10, tagJOYCAPSA, sJoystickCaps_5C2D10, {});
ALIVE_VAR(1, 0x5c2ea8, joyinfoex_tag, sJoystickInfo_5C2EA8, {});
ALIVE_VAR(1, 0x5c2f00, UINT, sJoyID_5C2F00, 0);

ALIVE_ARY(1, 0xBD2F60, unsigned char, 256, sInputKeyStates_BD2F60, {});
ALIVE_VAR(1, 0xBBB9D0, BYTE, sInputEnabled_BBB9D0, 0);
ALIVE_VAR(1, 0x5BD4E0, InputObject, sInputObject_5BD4E0, {});
ALIVE_VAR(1, 0x5C1BBE, unsigned __int16, sCurrentControllerIndex_5C1BBE, 0);
ALIVE_VAR(1, 0x5C1B9A, __int16, word_5C1B9A, 0);
ALIVE_VAR(1, 0xbd30a0, BOOL, sLastPressedKey_BD30A0, FALSE);
ALIVE_VAR(1, 0xbd309c, int, sIsAKeyDown_BD309C, 0);
ALIVE_ARY(1, 0x5C9D30, char, 256, sAllowedGameKeys_5C9D30, {});
ALIVE_ARY(1, 0x5C9394, const char *, 256, sKeyNames_5C9394, {});
ALIVE_ARY(1, 0x5C9908, const char *, 10, sJoyButtonNames_5C9908, {});
ALIVE_ARY(1, 0x5C9930, int, 256, sKeyBindings_5C9930, {});
ALIVE_ARY(1, 0x5C98E0, int, 10, dword_5C98E0, {});
ALIVE_VAR(1, 0xbd1870, t_InputCallback, sInputCallbackFunc_BD1870, 0);

ALIVE_ARY(1, 0x555708, char, 32, sGamePadStr_555708, { "Game Pad" });
ALIVE_ARY(1, 0x55E85C, char, 32, sGamePadStr_55E85C, { "Game Pad" });

ALIVE_VAR(1, 0x5c2ee8, int, dword_5C2EE8, 0);
ALIVE_VAR(1, 0x5c2ea4, int, dword_5C2EA4, 0);
ALIVE_VAR(1, 0x5c2ee4, int, dword_5C2EE4, 0);
ALIVE_VAR(1, 0x5c2ef0, int, dword_5C2EF0, 0);


ALIVE_ARY(1, 0x55EAD8, InputBinding, 36, sDefaultKeyBindings_55EAD8, {
    { VK_LEFT, eLeft },
    { VK_RIGHT, eRight },
    { VK_UP, eUp },
    { VK_DOWN, eDown },
    { VK_CONTROL, eDoAction },
    { VK_MENU, eSneak },
    { VK_SHIFT, eRun },
    { VK_SPACE, eHop },
    { 'Z', eThrowItem },
    { 'X', eUnPause | eFartOrRoll },
    { VK_ESCAPE, ePause | 0x200000 },
    { VK_RETURN, eUnPause | eFartOrRoll },
    { VK_TAB, eCheatMode },
    { '1', eGameSpeak1 },
    { '2', eGameSpeak2 },
    { '3', eGameSpeak3 },
    { '4', eGameSpeak4 },
    { '5', eGameSpeak5 },
    { '6', eGameSpeak6 },
    { '7', eGameSpeak7 },
    { '8', eGameSpeak8 },
    { '0', eChant },
    { VK_NUMPAD1, eGameSpeak1 },
    { VK_NUMPAD2, eGameSpeak2 },
    { VK_NUMPAD3, eGameSpeak3 },
    { VK_NUMPAD4, eGameSpeak4 },
    { VK_NUMPAD5, eGameSpeak5 },
    { VK_NUMPAD6, eGameSpeak6 },
    { VK_NUMPAD7, eGameSpeak7 },
    { VK_NUMPAD8, eGameSpeak8 },
    { VK_NUMPAD0, eChant },
    { 'C', 0x80000000 },
    { VK_PRIOR, 0x20000000 },
    { VK_NEXT, 0x40000000 },
    { VK_DELETE, 0x10000000 },
    { 0, 0 }
})

int sInputUnknown_55EA2C[] =
{ 32, 1049088, 128, 2097408, 8388608, 16, 16777216, 64, 0, 1572864 };



// -- Functions -- //

// For joysticks with very little buttons, depending on strength of joystick, will make abe
// automatically run/sneak.
EXPORT void CC Input_AutoRun_45FF60(float x, float y, DWORD *buttons)
{
    NOT_IMPLEMENTED();
}

void Input_GetJoyState_Impl(float *pX1, float *pY1, float *pX2, float *pY2, DWORD *pButtons)
{
    if (!sJoystickEnabled_5C2EF4)
    {
        *pY2 = 0.0f;
        *pX2 = 0.0f;
        *pY1 = 0.0f;
        *pX1 = 0.0f;
        *pButtons = 0;
        return;
    }

    if (!sJoyStateIsInit_5C2EE0)
    {
        sJoyStateIsInit_5C2EE0 = true;
        sJoyLastTick_5C2EEC = GetTickCount() - 1000;
    }

    // Only update joystick every 30 ticks
    auto const tickNow = GetTickCount();
    if (tickNow - sJoyLastTick_5C2EEC >= 30)
    {
        sJoyLastTick_5C2EEC = tickNow;
        sJoystickInfo_5C2EA8.dwSize = 52;
        sJoystickInfo_5C2EA8.dwFlags = sJoystickCapFlags_5C2EDC;

        if (joyGetPosEx(sJoyID_5C2F00, &sJoystickInfo_5C2EA8))
        {
            sJoystickEnabled_5C2EF4 = false;
            return;
        }
    }

    signed int xRange = (sJoystickCaps_5C2D10.wXmin + sJoystickCaps_5C2D10.wXmax) / 2;
    signed int xRangeDeadZone = (xRange - sJoystickCaps_5C2D10.wXmin) / 4;
    if (sJoystickInfo_5C2EA8.dwXpos < xRange - xRangeDeadZone || sJoystickInfo_5C2EA8.dwXpos > xRange + xRangeDeadZone)
    {
        *pX1 = static_cast<float>(static_cast<signed int>(sJoystickInfo_5C2EA8.dwXpos) - xRange) / static_cast<float>(xRange - sJoystickCaps_5C2D10.wXmin);
    }
    else
    {
        *pX1 = 0.0f;
    }

    signed int yRange = (sJoystickCaps_5C2D10.wYmax + sJoystickCaps_5C2D10.wYmin) / 2;
    signed int yRangeDeadZone = (yRange - sJoystickCaps_5C2D10.wYmin) / 4;
    if (sJoystickInfo_5C2EA8.dwYpos < yRange - yRangeDeadZone || sJoystickInfo_5C2EA8.dwYpos > yRange + yRangeDeadZone)
    {
        *pY1 = static_cast<float>(static_cast<signed int>(sJoystickInfo_5C2EA8.dwYpos) - yRange) / static_cast<float>(yRange - sJoystickCaps_5C2D10.wYmin);
    }
    else
    {
        *pY1 = 0.0f;
    }

    signed int zRange = (sJoystickCaps_5C2D10.wZmin + sJoystickCaps_5C2D10.wZmax) / 2;
    signed int zRangeDeadZone = (zRange - sJoystickCaps_5C2D10.wZmin) / 4;
    if (sJoystickCapFlags_5C2EDC & JOY_RETURNZ
        && (sJoystickInfo_5C2EA8.dwZpos < zRange - zRangeDeadZone || sJoystickInfo_5C2EA8.dwZpos > zRange + zRangeDeadZone))
    {
        *pX2 = static_cast<float>(static_cast<signed int>(sJoystickInfo_5C2EA8.dwZpos) - zRange) / static_cast<float>(zRange - sJoystickCaps_5C2D10.wZmin);
    }
    else
    {
        *pX2 = 0.0f;
    }

    signed int wRange = (sJoystickCaps_5C2D10.wRmax + sJoystickCaps_5C2D10.wRmin) / 2;
    signed int wRangeDeadZone = (wRange - sJoystickCaps_5C2D10.wRmin) / 4;
    if (sJoystickCapFlags_5C2EDC & JOY_RETURNR
        && (sJoystickInfo_5C2EA8.dwRpos < wRange - wRangeDeadZone || sJoystickInfo_5C2EA8.dwRpos > wRange + wRangeDeadZone))
    {
        *pY2 = static_cast<float>(static_cast<signed int>(sJoystickInfo_5C2EA8.dwRpos) - wRange) / static_cast<float>(wRange - sJoystickCaps_5C2D10.wRmin);
    }
    else
    {
        *pY2 = 0.0f;
    }

    if (sJoystickCapFlags_5C2EDC & JOY_RETURNPOV)
    {
        if (sJoystickInfo_5C2EA8.dwPOV == JOY_POVBACKWARD) // TODO: Double check if forward and backward are swapped? 
        {
            *pX2 = -1.0f;
        }
        else if (sJoystickInfo_5C2EA8.dwPOV == JOY_POVFORWARD)
        {
            *pX2 = 1.0f;
        }
        if (sJoystickInfo_5C2EA8.dwPOV == JOY_POVLEFT)
        {
            *pY2 = -1.0f;
        }
        else if (sJoystickInfo_5C2EA8.dwPOV == JOY_POVRIGHT)
        {
            *pY2 = 1.0f;
        }
    }

    // Clamp all our stick values
    *pX1 = min(1.0f, max(-1.0f, *pX1));
    *pY1 = min(1.0f, max(-1.0f, *pY1));
    *pX2 = min(1.0f, max(-1.0f, *pX2));
    *pY2 = min(1.0f, max(-1.0f, *pY2));

    *pButtons = sJoystickInfo_5C2EA8.dwButtons;
    Input_AutoRun_45FF60(*pX2, *pY2, pButtons);
}

#if XINPUT_SUPPORT
void Input_XINPUT(float *pX1, float *pY1, float *pX2, float *pY2, DWORD *pButtons)
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    float deadzone = 0.2f;

    *pButtons = 0;
    *pX1 = 0;
    *pY1 = 0;
    *pX2 = 0;
    *pY2 = 0;

    if (XInputGetState(sCurrentControllerIndex_5C1BBE, &state) == ERROR_SUCCESS)
    {
        float f_LX = state.Gamepad.sThumbLX / 32767.0f;
        float f_LY = state.Gamepad.sThumbLY / 32767.0f;

        float f_RX = state.Gamepad.sThumbRX / 32767.0f;
        float f_RY = state.Gamepad.sThumbRY / 32767.0f;

        // Joysticks
        if (abs(f_LX) > deadzone)
            *pX1 = f_LX;
        if (abs(f_LY) > deadzone)
            *pY1 = -f_LY;

        if (abs(f_RX) > deadzone)
            *pX2 = f_RX;
        if (abs(f_RY) > deadzone)
            *pY2 = -f_RY;

        // DPad Movement
        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
            *pX1 = 1;
        else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
            *pX1 = -1;
        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
            *pY1 = -1;
        else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
            *pY1 = 1;

#define M_XINPUT_BIND(BIT, PAD_BUTTON){if (state.Gamepad.wButtons & PAD_BUTTON) *pButtons |= (1 << BIT); }


        M_XINPUT_BIND(0, XINPUT_GAMEPAD_X);
        M_XINPUT_BIND(1, XINPUT_GAMEPAD_A);
        M_XINPUT_BIND(2, XINPUT_GAMEPAD_B);
        M_XINPUT_BIND(3, XINPUT_GAMEPAD_Y);
        M_XINPUT_BIND(4, XINPUT_GAMEPAD_LEFT_SHOULDER);
        M_XINPUT_BIND(5, XINPUT_GAMEPAD_RIGHT_SHOULDER);
        M_XINPUT_BIND(8, XINPUT_GAMEPAD_BACK);
        M_XINPUT_BIND(9, XINPUT_GAMEPAD_START);

        if (state.Gamepad.bLeftTrigger > 32)
            *pButtons |= (1 << 6);
        if (state.Gamepad.bRightTrigger > 32)
            *pButtons |= (1 << 7);

        // 0 Square
        // 1 Cross
        // 2 Circle
        // 3 Triangle
        // 4 L1
        // 5 R1
        // 6 L2
        // 7 R2
        // 8 Back
        // 9 Start

        static float vibrationAmount = 0.0f;
        int screenShake = max(abs(sScreenXOffSet_BD30E4), abs(sScreenYOffset_BD30A4));

        if (screenShake > 0)
        {
            vibrationAmount = min(screenShake, 30) / 30.0f;
        }
        else if (Event_Get_422C00(kEventScreenShake))
        {
            vibrationAmount = 1.0f;
        }
        

        XINPUT_VIBRATION vib;
        USHORT vibLR = vibrationAmount * 65535;

        vib.wLeftMotorSpeed = vibLR;
        vib.wRightMotorSpeed = vibLR;
        XInputSetState(0, &vib);

        vibrationAmount -= 0.2f;
        vibrationAmount = max(0, vibrationAmount);
    }
}
#endif

// TODO: Needs actual testing.
EXPORT void CC Input_GetJoyState_460280(float *pX1, float *pY1, float *pX2, float *pY2, DWORD *pButtons)
{
#if XINPUT_SUPPORT
    Input_XINPUT(pX1, pY1, pX2, pY2, pButtons);
#else
    Input_GetJoyState_Impl(pX1, pY1, pX2, pY2, pButtons);
#endif
}

EXPORT unsigned __int8 CC Input_GetInputEnabled_4EDDE0()
{
    return sInputEnabled_BBB9D0 != 0;
}

EXPORT char CC Input_GetKeyState_4EDD20(int key)
{
    const char keyState = sInputKeyStates_BD2F60[key] & 0x80;
    sInputKeyStates_BD2F60[key] = keyState;
    return keyState;
}

EXPORT void CC Input_EnableInput_4EDDD0()
{
    sInputEnabled_BBB9D0 = TRUE;
}

EXPORT void CC Input_DisableInput_4EDDC0()
{
    sInputEnabled_BBB9D0 = FALSE;
}

EXPORT void CC Input_491870()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Input_LoadSettingsIni_492D40()
{
    NOT_IMPLEMENTED();
}

EXPORT int Input_492150()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Input_SetCallback_4FA910(t_InputCallback pFunc)
{
    sInputCallbackFunc_BD1870 = pFunc;
}

EXPORT void CC Input_45FDF0(float x, float y, int a3, bool cap_has_r)
{
    if (sJoystickNumButtons_5C2EFC == 4 && (a3 || cap_has_r))
    {
        dword_5C2EE8 = 0;
        dword_5C2EA4 = 0;
        dword_5C2EE4 = 0;
        dword_5C2EF0 = 0;
        if (a3)
        {
            dword_5C2EE4 = 1;
            sJoystickNumButtons_5C2EFC = 5;
            if (x <= -0.25f || x >= 0.25f)
            {
                dword_5C2EE8 = 1;
                if (x <= 0.0f)
                {
                    dword_5C2EE8 = 0;
                }
            }
            else
            {
                sJoystickNumButtons_5C2EFC = 6;
                dword_5C2EE4 = 2;
            }
        }
        if (cap_has_r)
        {
            dword_5C2EF0 = 1;
            ++sJoystickNumButtons_5C2EFC;
            if (y <= -0.25f || y >= 0.25f)
            {
                dword_5C2EA4 = 1;
                if (y <= 0.0f)
                {
                    dword_5C2EA4 = 0;
                }
            }
            else
            {
                dword_5C2EF0 = 2;
                ++sJoystickNumButtons_5C2EFC;
            }
        }
    }
}

EXPORT void Input_InitJoyStick_460080()
{
    sJoystickEnabled_5C2EF4 = false;

    for (int i = 0; i < joyGetNumDevs(); i++)
    {
        if (!joyGetDevCapsA(i, &sJoystickCaps_5C2D10, 0x194u))
        {
            sJoystickEnabled_5C2EF4 = true;
            sJoyID_5C2F00 = i;
            break;
        }
    }

    if (_strnicmp(sJoystickCaps_5C2D10.szPname, "Microsoft PC Joystick Driver", 0xCu))
    {
        if (_strnicmp(sJoystickCaps_5C2D10.szPname, "Custom", 6u))
        {
            if (_strnicmp(sJoystickCaps_5C2D10.szPname, "Generic", 7u))
            {
                strncpy(sGamePadStr_555708, sJoystickCaps_5C2D10.szPname, 0x20u);
            }
        }
    }

    int joyFlags = 0xC83;

    if (sJoystickCaps_5C2D10.wCaps & JOYCAPS_HASZ)
    {
        joyFlags = 0xC87;
    }
    if (sJoystickCaps_5C2D10.wCaps & JOYCAPS_HASR)
    {
        joyFlags |= 8u;
    }
    if (sJoystickCaps_5C2D10.wCaps & JOYCAPS_POV4DIR)
    {
        joyFlags |= 0x40u;
    }

    sJoystickCapFlags_5C2EDC = joyFlags;

    if (joyFlags & 8)
    {
        dword_5C2EF8 |= 4;
    }
    if (joyFlags & 4)
    {
        dword_5C2EF8 |= 8;
    }
    if (joyFlags & 0x40)
    {
        dword_5C2EF8 |= 2;
    }
    sJoystickNumButtons_5C2EFC = sJoystickCaps_5C2D10.wNumButtons;
    if (sJoystickCaps_5C2D10.wNumButtons <= 2
        || sJoystickCaps_5C2D10.wNumButtons > 4 && sJoystickCaps_5C2D10.wNumAxes > 2)
    {
        dword_5C2EF8 |= 1u;
    }
    if (sJoystickCaps_5C2D10.wNumButtons == 4)
    {
        if (joyFlags & 0xC)
        {
            float pY2; // [esp+Ch] [ebp-14h]
            float pX2; // [esp+10h] [ebp-10h]
            DWORD pButtons; // [esp+14h] [ebp-Ch]
            float pY1; // [esp+18h] [ebp-8h]
            float pX1; // [esp+1Ch] [ebp-4h]

            Input_GetJoyState_460280(&pX1, &pY1, &pX2, &pY2, &pButtons);
            Input_45FDF0(pX2, pY2, sJoystickCapFlags_5C2EDC & 4, sJoystickCapFlags_5C2EDC & 8);
        }
    }

    DEV_CONSOLE_PRINTF("Joystick Initialized: Buttons: %i", sJoystickNumButtons_5C2EFC);
}

EXPORT void CC Input_Init_491BC0()
{
    // This seems so stupid
    strncpy(sGamePadStr_555708, sGamePadStr_55E85C, 32u);
    strncpy(sGamePadStr_55E85C, sGamePadStr_555708, 32u);

    memset(sKeyNames_5C9394, 0, 1024);
    memset(sAllowedGameKeys_5C9D30, 0, 256);

    sAllowedGameKeys_5C9D30[VK_SHIFT] = 1;
    sAllowedGameKeys_5C9D30[VK_CONTROL] = 1;
    sAllowedGameKeys_5C9D30[VK_MENU] = 1;
    sAllowedGameKeys_5C9D30[VK_BACK] = 1;
    sAllowedGameKeys_5C9D30[VK_INSERT] = 1;
    sAllowedGameKeys_5C9D30[VK_DELETE] = 1;
    sAllowedGameKeys_5C9D30[VK_HOME] = 1;
    sAllowedGameKeys_5C9D30[VK_END] = 1;
    sAllowedGameKeys_5C9D30[VK_PRIOR] = 1;
    sAllowedGameKeys_5C9D30[VK_NEXT] = 1;
    sAllowedGameKeys_5C9D30[VK_SPACE] = 1;
    sAllowedGameKeys_5C9D30['A'] = 1;
    sAllowedGameKeys_5C9D30['B'] = 1;
    sAllowedGameKeys_5C9D30['C'] = 1;
    sAllowedGameKeys_5C9D30['D'] = 1;
    sAllowedGameKeys_5C9D30['E'] = 1;
    sAllowedGameKeys_5C9D30['F'] = 1;
    sAllowedGameKeys_5C9D30['G'] = 1;
    sAllowedGameKeys_5C9D30['H'] = 1;
    sAllowedGameKeys_5C9D30['I'] = 1;
    sAllowedGameKeys_5C9D30['J'] = 1;
    sAllowedGameKeys_5C9D30['K'] = 1;
    sAllowedGameKeys_5C9D30['L'] = 1;
    sAllowedGameKeys_5C9D30['M'] = 1;
    sAllowedGameKeys_5C9D30['N'] = 1;
    sAllowedGameKeys_5C9D30['O'] = 1;
    sAllowedGameKeys_5C9D30['P'] = 1;
    sAllowedGameKeys_5C9D30['Q'] = 1;
    sAllowedGameKeys_5C9D30['R'] = 1;
    sAllowedGameKeys_5C9D30['S'] = 1;
    sAllowedGameKeys_5C9D30['T'] = 1;
    sAllowedGameKeys_5C9D30['U'] = 1;
    sAllowedGameKeys_5C9D30['V'] = 1;
    sAllowedGameKeys_5C9D30['W'] = 1;
    sAllowedGameKeys_5C9D30['X'] = 1;
    sAllowedGameKeys_5C9D30['Y'] = 1;
    sAllowedGameKeys_5C9D30['Z'] = 1;
    sAllowedGameKeys_5C9D30[WM_COMPAREITEM] = 1;
    sAllowedGameKeys_5C9D30[VK_SEPARATOR] = 1;
    sAllowedGameKeys_5C9D30[VK_DECIMAL] = 1;
    sAllowedGameKeys_5C9D30[0] = 1;

    sKeyNames_5C9394[VK_UP] = "\x10";
    sKeyNames_5C9394[VK_DOWN] = "\x11";
    sKeyNames_5C9394[VK_LEFT] = "\x12";
    sKeyNames_5C9394[VK_RIGHT] = "\x13";
    sKeyNames_5C9394[VK_SHIFT] = "shift";
    sKeyNames_5C9394[VK_CONTROL] = "ctrl";
    sKeyNames_5C9394[VK_RETURN] = "enter";
    sKeyNames_5C9394[VK_ESCAPE] = "esc";
    sKeyNames_5C9394[VK_BACK] = "bkspc";
    sKeyNames_5C9394[VK_INSERT] = "ins";
    sKeyNames_5C9394[VK_DELETE] = "del";
    sKeyNames_5C9394[VK_HOME] = "home";
    sKeyNames_5C9394[VK_END] = "end";
    sKeyNames_5C9394[VK_PRIOR] = "pgup";
    sKeyNames_5C9394[VK_NEXT] = "pgdn";
    sKeyNames_5C9394[VK_SPACE] = "space";
    sKeyNames_5C9394[VK_MENU] = "alt";
    sKeyNames_5C9394[VK_TAB] = "tab";
    sKeyNames_5C9394['A'] = "A";
    sKeyNames_5C9394['B'] = "B";
    sKeyNames_5C9394['C'] = "C";
    sKeyNames_5C9394['D'] = "D";
    sKeyNames_5C9394['E'] = "E";
    sKeyNames_5C9394['F'] = "F";
    sKeyNames_5C9394['G'] = "G";
    sKeyNames_5C9394['H'] = "H";
    sKeyNames_5C9394['I'] = "I";
    sKeyNames_5C9394['J'] = "J";
    sKeyNames_5C9394['K'] = "K";
    sKeyNames_5C9394['L'] = "L";
    sKeyNames_5C9394['M'] = "M";
    sKeyNames_5C9394['N'] = "N";
    sKeyNames_5C9394['O'] = "O";
    sKeyNames_5C9394['P'] = "P";
    sKeyNames_5C9394['Q'] = "Q";
    sKeyNames_5C9394['R'] = "R";
    sKeyNames_5C9394['S'] = "S";
    sKeyNames_5C9394['T'] = "T";
    sKeyNames_5C9394['U'] = "U";
    sKeyNames_5C9394['V'] = "V";
    sKeyNames_5C9394['W'] = "W";
    sKeyNames_5C9394['X'] = "X";
    sKeyNames_5C9394['Y'] = "Y";
    sKeyNames_5C9394['Z'] = "Z";
    sKeyNames_5C9394['0'] = "0";
    sKeyNames_5C9394['1'] = "1";
    sKeyNames_5C9394['2'] = "2";
    sKeyNames_5C9394['3'] = "3";
    sKeyNames_5C9394['4'] = "4";
    sKeyNames_5C9394['5'] = "5";
    sKeyNames_5C9394['6'] = "6";
    sKeyNames_5C9394['7'] = "7";
    sKeyNames_5C9394['8'] = "8";
    sKeyNames_5C9394['9'] = "9";
    sKeyNames_5C9394[VK_SEPARATOR] = ",";
    sKeyNames_5C9394[VK_DECIMAL] = ".";
    sKeyNames_5C9394[0] = "\\";
    sKeyNames_5C9394[VK_NUMPAD0] = "0";
    sKeyNames_5C9394[VK_NUMPAD1] = "1";
    sKeyNames_5C9394[VK_NUMPAD2] = "2";
    sKeyNames_5C9394[VK_NUMPAD3] = "3";
    sKeyNames_5C9394[VK_NUMPAD4] = "4";
    sKeyNames_5C9394[VK_NUMPAD5] = "5";
    sKeyNames_5C9394[VK_NUMPAD6] = "6";
    sKeyNames_5C9394[VK_NUMPAD7] = "7";
    sKeyNames_5C9394[VK_NUMPAD8] = "8";
    // Og game is missing vk_numpad 9 ? :s

#if XINPUT_SUPPORT
    strncpy(sGamePadStr_55E85C, "Xbox Controller", 32u);

    // These are to be used with the custom XBOX fonts.
    sJoyButtonNames_5C9908[0] = "\x81";
    sJoyButtonNames_5C9908[1] = "\x80";
    sJoyButtonNames_5C9908[2] = "\x82";
    sJoyButtonNames_5C9908[3] = "\x83";
    sJoyButtonNames_5C9908[4] = "\x84";
    sJoyButtonNames_5C9908[5] = "\x85";
    sJoyButtonNames_5C9908[6] = "\x86";
    sJoyButtonNames_5C9908[7] = "\x87";
    sJoyButtonNames_5C9908[8] = "";
    sJoyButtonNames_5C9908[9] = "";

    /*
    sJoyButtonNames_5C9908[0] = "X";
    sJoyButtonNames_5C9908[1] = "A";
    sJoyButtonNames_5C9908[2] = "B";
    sJoyButtonNames_5C9908[3] = "Y";
    sJoyButtonNames_5C9908[4] = "LB";
    sJoyButtonNames_5C9908[5] = "RB";
    sJoyButtonNames_5C9908[6] = "LT";
    sJoyButtonNames_5C9908[7] = "RT";
    sJoyButtonNames_5C9908[8] = "";
    sJoyButtonNames_5C9908[9] = "";
    */
#else
    sJoyButtonNames_5C9908[0] = "B1";
    sJoyButtonNames_5C9908[1] = "B2";
    sJoyButtonNames_5C9908[2] = "B3";
    sJoyButtonNames_5C9908[3] = "B4";
    sJoyButtonNames_5C9908[4] = "B5";
    sJoyButtonNames_5C9908[5] = "B6";
    sJoyButtonNames_5C9908[6] = "B7";
    sJoyButtonNames_5C9908[7] = "B8";
    sJoyButtonNames_5C9908[8] = "B9";
    sJoyButtonNames_5C9908[9] = "B0";
#endif

    Input_InitJoyStick_460080();
    memset(sKeyBindings_5C9930, 0, sizeof(*sKeyBindings_5C9930) * 256);

    for (auto kb = sDefaultKeyBindings_55EAD8; kb->key; kb++)
    {
        sKeyBindings_5C9930[kb->key] = kb->command;
    }

    memcpy(dword_5C98E0, &sInputUnknown_55EA2C, sizeof(sInputUnknown_55EA2C));
    Input_LoadSettingsIni_492D40();
    Input_491870();
    Input_SetCallback_4FA910(Input_492150);
}

EXPORT char Input_ReadKey_492610()
{
    if (!Sys_IsAnyKeyDown_4EDDF0())
        return 0;

    const auto lk = sLastPressedKey_BD30A0;

    sIsAKeyDown_BD309C = false;
    sLastPressedKey_BD30A0 = 0;

    return lk;
}

void Input_Reset_492660()
{
    Input_EnableInput_4EDDD0();
    Input_InitKeyStateArray_4EDD60();
}

EXPORT unsigned int Input_IsChanting_45F260()
{
    return (sInputObject_5BD4E0.field_0_pads[0].field_0_pressed & InputCommands::eChant) == InputCommands::eChant;
}

// Zeros the input key state array.
EXPORT void CC Input_InitKeyStateArray_4EDD60()
{
    memset(sInputKeyStates_BD2F60, 0, 256u);
}

EXPORT void CC Input_SetKeyState_4EDD80(int key, char bIsDown)
{
    if (bIsDown)
    {
        sInputKeyStates_BD2F60[key] = -127;
    }
    else
    {
        sInputKeyStates_BD2F60[key] &= 1u;
    }
}

// Returns true if a key was just pressed down.
EXPORT bool CC Input_IsVKPressed_4EDD40(int key)
{
    const auto keyState = sInputKeyStates_BD2F60[key];

    if (!keyState)
    {
        return false;
    }

    sInputKeyStates_BD2F60[key] = keyState & 0x80;

    return true;
}

EXPORT int CC sub_4FA9C0(int /*padNum*/)
{

    NOT_IMPLEMENTED();
    return 0;
}

int InputObject::Is_Demo_Playing_45F220()
{
    return field_38_bDemoPlaying & 1;
}

void InputObject::UnsetDemoPlaying_45F240()
{
    field_38_bDemoPlaying &= ~1;
}

void InputObject::SetDemoResource_45F1E0(DWORD** pDemoRes)
{
    field_34_demo_command_index = 2;
    field_30_pDemoRes = pDemoRes;
    field_38_bDemoPlaying |= 1u;
    field_40_command_duration = 0;
}

void InputObject::Update_45F040()
{
    const unsigned char byte_545A4C[20] =
    {
        0, // left?
        64, // up?
        192, // down?
        0,
        128, // right?
        96,
        160,
        128,
        0,
        32,
        224,
        0,
        0,
        64,
        192,
        0,
        0,
        0,
        0,
        0
    };

    field_0_pads[0].field_8_previous = field_0_pads[0].field_0_pressed;
    field_0_pads[0].field_0_pressed = sub_4FA9C0(0);

    if (Is_Demo_Playing_45F220())
    {
        // Stop if any button on any pad is pressed
        if (field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed)
        {
            word_5C1B9A = 0;
            UnsetDemoPlaying_45F240();
            return;
        }

        if (sGnFrame_5C1B84 >= field_40_command_duration)
        {
            const DWORD command = (*field_30_pDemoRes)[field_34_demo_command_index++];
            field_3C_command = command >> 16;
            field_40_command_duration = sGnFrame_5C1B84 + command & 0xFFFF;

            // End demo/quit command
            if (command & 0x8000)
            {
                UnsetDemoPlaying_45F240();
            }
        }

        // Will do nothing if we hit the end command..
        if (Is_Demo_Playing_45F220())
        {
            field_0_pads[0].field_0_pressed = Command_To_Raw_45EE40(field_3C_command);
        }
    }

    field_0_pads[0].field_10_released = field_0_pads[0].field_8_previous & ~field_0_pads[0].field_0_pressed;
    field_0_pads[0].field_C_held = field_0_pads[0].field_0_pressed & ~field_0_pads[0].field_8_previous;
    field_0_pads[0].field_4_dir = byte_545A4C[field_0_pads[0].field_0_pressed & 0xF];

    field_0_pads[1].field_8_previous = field_0_pads[1].field_0_pressed;
    field_0_pads[1].field_0_pressed = sub_4FA9C0(1);
    field_0_pads[1].field_10_released = field_0_pads[1].field_8_previous & ~field_0_pads[1].field_0_pressed;
    field_0_pads[1].field_C_held = field_0_pads[1].field_0_pressed & ~field_0_pads[1].field_8_previous;
    field_0_pads[1].field_4_dir = byte_545A4C[field_0_pads[1].field_0_pressed & 0xF];
}

DWORD CC InputObject::Command_To_Raw_45EE40(DWORD cmd)
{
    unsigned int shoulderButtonsPressedCount = 0;

    if (cmd & PsxButtonBits::eL2)
    {
        ++shoulderButtonsPressedCount;
    }

    if (cmd & PsxButtonBits::eR2)
    {
        ++shoulderButtonsPressedCount;
    }

    if (cmd & PsxButtonBits::eL1)
    {
        ++shoulderButtonsPressedCount;
    }

    if (cmd & PsxButtonBits::eR1)
    {
        ++shoulderButtonsPressedCount;
    }

    if (shoulderButtonsPressedCount > 1) // Any 2 shoulder button combo = chanting
    {
        return InputCommands::eChant;
    }

    DWORD rawInput = 0;
    if (cmd & PsxButtonBits::eDPadUp)
    {
        rawInput |= InputCommands::eUp;
    }

    if (cmd & PsxButtonBits::eDPadRight)
    {
        rawInput |= InputCommands::eRight;
    }

    if (cmd & PsxButtonBits::eDPadDown)
    {
        rawInput |= InputCommands::eDown;
    }

    if (cmd & eDPadLeft)
    {
        rawInput |= InputCommands::eLeft;
    }

    if (cmd & PsxButtonBits::eR1)
    {
        rawInput |= InputCommands::eRun;
    }

    if (cmd & PsxButtonBits::eR2)
    {
        rawInput |= InputCommands::eSneak;
    }

    if (cmd & PsxButtonBits::eL1)
    {
        if (cmd & PsxButtonBits::eTriangle)
        {
            rawInput |= InputCommands::eGameSpeak1;
        }

        if (cmd & PsxButtonBits::eCircle)
        {
            rawInput |= InputCommands::eGameSpeak4;
        }

        if (cmd & PsxButtonBits::eCross)
        {
            rawInput |= InputCommands::eGameSpeak3;
        }

        if (cmd & PsxButtonBits::eSquare)
        {
            rawInput |= InputCommands::eGameSpeak2;
        }
    }
    else if (cmd & PsxButtonBits::eL2)
    {
        if (cmd & PsxButtonBits::eTriangle)
        {
            rawInput |= InputCommands::eGameSpeak6;
        }

        if (cmd & PsxButtonBits::eCircle)
        {
            rawInput |= InputCommands::eGameSpeak7;
        }

        if (cmd & PsxButtonBits::eCross)
        {
            rawInput |= InputCommands::eGameSpeak5;
        }

        if (cmd & PsxButtonBits::eSquare)
        {
            rawInput |= InputCommands::eGameSpeak8;
        }
    }
    else // No shoulder buttons
    {
        if (cmd & PsxButtonBits::eTriangle)
        {
            rawInput |= InputCommands::eHop;
        }

        if (cmd & PsxButtonBits::eCircle)
        {
            rawInput |= InputCommands::eThrowItem;
        }

        if (cmd & PsxButtonBits::eCross)
        {
            rawInput |= InputCommands::eFartOrRoll;
        }

        if (cmd & PsxButtonBits::eSquare)
        {
            rawInput |= InputCommands::eDoAction;
        }
    }

    return rawInput;
}

void CC InputObject::ShutDown_45F020()
{
    // Empty
}
