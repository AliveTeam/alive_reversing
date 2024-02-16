#include "stdafx.h"
#include "Sys.hpp"
#include "../AliveLibAE/Input.hpp"
#include "../relive/resource.h"
#include "Renderer/IRenderer.hpp"

#include "Sound/Sound.hpp"
#include "Sound/Midi.hpp"
#include "../AliveLibAE/PauseMenu.hpp"
#include "../AliveLibAE/GameAutoPlayer.hpp"
#include "relive_config.h"
#include <FatalError.hpp>

static bool sAppIsActivated = false;
static TWindowHandleType sHwnd = nullptr;

#if AUTO_SWITCH_CONTROLLER // OG Change - Used for Auto-switching active controller (gamepad/keyboard)
static int totalConnectedJoysticks = 0;
#endif

#if ORIGINAL_PS1_BEHAVIOR // OG Change - Allow for exiting save menu using controller
static bool saveMenuOpen = false;

void setSaveMenuOpen(bool val)
{
    saveMenuOpen = val;
}
#endif

std::string BuildString()
{
#ifdef BUILD_NUMBER
    // Automated AppVeyor build title
    return std::string("(") + CI_PROVIDER + " Build: " + std::to_string(BUILD_NUMBER) + ")";
#else
    return "";
#endif
}

std::string BuildAndBitnesString()
{
    std::string buildAndBitness;
    std::string buildStr = BuildString();
    if (!buildStr.empty())
    {
        buildAndBitness += " ";
        buildAndBitness += buildStr;
    }

    std::string kBitness = sizeof(void*) == 4 ? " (32 bit)" : " (64 bit)";
    buildAndBitness += kBitness;
    return buildAndBitness;
}

std::string WindowTitleAO()
{
    return "R.E.L.I.V.E. Oddworld Abe's Oddysee" + BuildAndBitnesString();
}

std::string WindowTitleAE()
{
    return "R.E.L.I.V.E. Oddworld Abe's Exoddus" + BuildAndBitnesString();
}

TWindowHandleType Sys_GetHWnd()
{
    return sHwnd;
}

bool Sys_IsAnyKeyDown()
{
    return sIsAKeyDown;
}

void Sys_SetWindowText(TWindowHandleType windowHandle, const char_type* title)
{
    SDL_SetWindowTitle(windowHandle, title);
}

bool Sys_IsMouseButtonDown(MouseButtons button)
{
    if (button == MouseButtons::eRight)
    {
        return !!(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT));
    }
    return !!(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT));
}

SoundEntry* gMovieSoundEntry = nullptr;

static s32 sdl_key_to_win32_vkey(SDL_Scancode key)
{
    switch (key)
    {
        default:
            return 0;
        case SDL_SCANCODE_UNKNOWN:
            return 0;
        case SDL_SCANCODE_A:
            return 0x41;
        case SDL_SCANCODE_B:
            return 0x42;
        case SDL_SCANCODE_C:
            return 0x43;
        case SDL_SCANCODE_D:
            return 0x44;
        case SDL_SCANCODE_E:
            return 0x45;
        case SDL_SCANCODE_F:
            return 0x46;
        case SDL_SCANCODE_G:
            return 0x47;
        case SDL_SCANCODE_H:
            return 0x48;
        case SDL_SCANCODE_I:
            return 0x49;
        case SDL_SCANCODE_J:
            return 0x4A;
        case SDL_SCANCODE_K:
            return 0x4B;
        case SDL_SCANCODE_L:
            return 0x4C;
        case SDL_SCANCODE_M:
            return 0x4D;
        case SDL_SCANCODE_N:
            return 0x4E;
        case SDL_SCANCODE_O:
            return 0x4F;
        case SDL_SCANCODE_P:
            return 0x50;
        case SDL_SCANCODE_Q:
            return 0x51;
        case SDL_SCANCODE_R:
            return 0x52;
        case SDL_SCANCODE_S:
            return 0x53;
        case SDL_SCANCODE_T:
            return 0x54;
        case SDL_SCANCODE_U:
            return 0x55;
        case SDL_SCANCODE_V:
            return 0x56;
        case SDL_SCANCODE_W:
            return 0x57;
        case SDL_SCANCODE_X:
            return 0x58;
        case SDL_SCANCODE_Y:
            return 0x59;
        case SDL_SCANCODE_Z:
            return 0x5A;
        case SDL_SCANCODE_1:
            return VK_NUMPAD1;
        case SDL_SCANCODE_2:
            return VK_NUMPAD2;
        case SDL_SCANCODE_3:
            return VK_NUMPAD3;
        case SDL_SCANCODE_4:
            return VK_NUMPAD4;
        case SDL_SCANCODE_5:
            return VK_NUMPAD5;
        case SDL_SCANCODE_6:
            return VK_NUMPAD6;
        case SDL_SCANCODE_7:
            return VK_NUMPAD7;
        case SDL_SCANCODE_8:
            return VK_NUMPAD8;
        case SDL_SCANCODE_9:
            return VK_NUMPAD9;
        case SDL_SCANCODE_0:
            return VK_NUMPAD0;
        case SDL_SCANCODE_RETURN:
            return VK_RETURN;
        case SDL_SCANCODE_ESCAPE:
            return VK_ESCAPE;
        case SDL_SCANCODE_BACKSPACE:
            return VK_BACK;
        case SDL_SCANCODE_TAB:
            return VK_TAB;
        case SDL_SCANCODE_SPACE:
            return VK_SPACE;
        case SDL_SCANCODE_MINUS:
            return VK_SUBTRACT;
        case SDL_SCANCODE_EQUALS:
            return 0;
        case SDL_SCANCODE_LEFTBRACKET:
            return 0;
        case SDL_SCANCODE_RIGHTBRACKET:
            return 0;
        case SDL_SCANCODE_BACKSLASH:
            return 0;
        case SDL_SCANCODE_SEMICOLON:
            return 0;
        case SDL_SCANCODE_APOSTROPHE:
            return 0;
        case SDL_SCANCODE_GRAVE:
            return VK_OEM_3; // Console key
        case SDL_SCANCODE_COMMA:
            return VK_OEM_COMMA;
        case SDL_SCANCODE_PERIOD:
            return VK_OEM_PERIOD;
        case SDL_SCANCODE_SLASH:
            return VK_OEM_102;
        case SDL_SCANCODE_CAPSLOCK:
            return VK_CAPITAL;
        case SDL_SCANCODE_F1:
            return VK_F1;
        case SDL_SCANCODE_F2:
            return VK_F2;
        case SDL_SCANCODE_F3:
            return VK_F3;
        case SDL_SCANCODE_F4:
            return VK_F4;
        case SDL_SCANCODE_F5:
            return VK_F5;
        case SDL_SCANCODE_F6:
            return VK_F6;
        case SDL_SCANCODE_F7:
            return VK_F7;
        case SDL_SCANCODE_F8:
            return VK_F8;
        case SDL_SCANCODE_F9:
            return VK_F9;
        case SDL_SCANCODE_F10:
            return VK_F10;
        case SDL_SCANCODE_F11:
            return VK_F11;
        case SDL_SCANCODE_F12:
            return VK_F12;
        case SDL_SCANCODE_PRINTSCREEN:
            return VK_PRINT;
        case SDL_SCANCODE_SCROLLLOCK:
            return VK_SCROLL;
        case SDL_SCANCODE_PAUSE:
            return VK_PAUSE;
        case SDL_SCANCODE_INSERT:
            return VK_INSERT;
        case SDL_SCANCODE_HOME:
            return VK_HOME;
        case SDL_SCANCODE_PAGEUP:
            return VK_PRIOR;
        case SDL_SCANCODE_DELETE:
            return VK_DELETE;
        case SDL_SCANCODE_END:
            return VK_END;
        case SDL_SCANCODE_PAGEDOWN:
            return VK_NEXT;
        case SDL_SCANCODE_RIGHT:
            return VK_RIGHT;
        case SDL_SCANCODE_LEFT:
            return VK_LEFT;
        case SDL_SCANCODE_DOWN:
            return VK_DOWN;
        case SDL_SCANCODE_UP:
            return VK_UP;
        case SDL_SCANCODE_NUMLOCKCLEAR:
            return 0;
        case SDL_SCANCODE_KP_DIVIDE:
            return 0;
        case SDL_SCANCODE_KP_MULTIPLY:
            return 0;
        case SDL_SCANCODE_KP_MINUS:
            return VK_SUBTRACT;
        case SDL_SCANCODE_KP_PLUS:
            return 0;
        case SDL_SCANCODE_KP_ENTER:
            return VK_RETURN;
        case SDL_SCANCODE_KP_1:
            return VK_NUMPAD1;
        case SDL_SCANCODE_KP_2:
            return VK_NUMPAD2;
        case SDL_SCANCODE_KP_3:
            return VK_NUMPAD3;
        case SDL_SCANCODE_KP_4:
            return VK_NUMPAD4;
        case SDL_SCANCODE_KP_5:
            return VK_NUMPAD5;
        case SDL_SCANCODE_KP_6:
            return VK_NUMPAD6;
        case SDL_SCANCODE_KP_7:
            return VK_NUMPAD7;
        case SDL_SCANCODE_KP_8:
            return VK_NUMPAD8;
        case SDL_SCANCODE_KP_9:
            return VK_NUMPAD9;
        case SDL_SCANCODE_KP_0:
            return VK_NUMPAD0;
        case SDL_SCANCODE_KP_PERIOD:
            return 0;
        case SDL_SCANCODE_APPLICATION:
            return 0;
        case SDL_SCANCODE_POWER:
            return 0;
        case SDL_SCANCODE_KP_EQUALS:
            return 0;
        case SDL_SCANCODE_F13:
            return 0;
        case SDL_SCANCODE_F14:
            return 0;
        case SDL_SCANCODE_F15:
            return 0;
        case SDL_SCANCODE_F16:
            return 0;
        case SDL_SCANCODE_F17:
            return 0;
        case SDL_SCANCODE_F18:
            return 0;
        case SDL_SCANCODE_F19:
            return 0;
        case SDL_SCANCODE_F20:
            return 0;
        case SDL_SCANCODE_F21:
            return 0;
        case SDL_SCANCODE_F22:
            return 0;
        case SDL_SCANCODE_F23:
            return 0;
        case SDL_SCANCODE_F24:
            return 0;
        case SDL_SCANCODE_EXECUTE:
            return 0;
        case SDL_SCANCODE_HELP:
            return 0;
        case SDL_SCANCODE_MENU:
            return 0;
        case SDL_SCANCODE_SELECT:
            return 0;
        case SDL_SCANCODE_STOP:
            return 0;
        case SDL_SCANCODE_AGAIN:
            return 0;
        case SDL_SCANCODE_UNDO:
            return 0;
        case SDL_SCANCODE_CUT:
            return 0;
        case SDL_SCANCODE_COPY:
            return 0;
        case SDL_SCANCODE_PASTE:
            return 0;
        case SDL_SCANCODE_FIND:
            return 0;
        case SDL_SCANCODE_MUTE:
            return 0;
        case SDL_SCANCODE_VOLUMEUP:
            return 0;
        case SDL_SCANCODE_VOLUMEDOWN:
            return 0;
        case SDL_SCANCODE_KP_COMMA:
            return 0;
        case SDL_SCANCODE_KP_EQUALSAS400:
            return 0;
        case SDL_SCANCODE_ALTERASE:
            return 0;
        case SDL_SCANCODE_SYSREQ:
            return 0;
        case SDL_SCANCODE_CANCEL:
            return 0;
        case SDL_SCANCODE_CLEAR:
            return 0;
        case SDL_SCANCODE_PRIOR:
            return 0;
        case SDL_SCANCODE_RETURN2:
            return 0;
        case SDL_SCANCODE_SEPARATOR:
            return 0;
        case SDL_SCANCODE_OUT:
            return 0;
        case SDL_SCANCODE_OPER:
            return 0;
        case SDL_SCANCODE_CLEARAGAIN:
            return 0;
        case SDL_SCANCODE_CRSEL:
            return 0;
        case SDL_SCANCODE_EXSEL:
            return 0;
        case SDL_SCANCODE_KP_00:
            return 0;
        case SDL_SCANCODE_KP_000:
            return 0;
        case SDL_SCANCODE_THOUSANDSSEPARATOR:
            return 0;
        case SDL_SCANCODE_DECIMALSEPARATOR:
            return 0;
        case SDL_SCANCODE_CURRENCYUNIT:
            return 0;
        case SDL_SCANCODE_CURRENCYSUBUNIT:
            return 0;
        case SDL_SCANCODE_KP_LEFTPAREN:
            return 0;
        case SDL_SCANCODE_KP_RIGHTPAREN:
            return 0;
        case SDL_SCANCODE_KP_LEFTBRACE:
            return 0;
        case SDL_SCANCODE_KP_RIGHTBRACE:
            return 0;
        case SDL_SCANCODE_KP_TAB:
            return 0;
        case SDL_SCANCODE_KP_BACKSPACE:
            return 0;
        case SDL_SCANCODE_KP_A:
            return 0;
        case SDL_SCANCODE_KP_B:
            return 0;
        case SDL_SCANCODE_KP_C:
            return 0;
        case SDL_SCANCODE_KP_D:
            return 0;
        case SDL_SCANCODE_KP_E:
            return 0;
        case SDL_SCANCODE_KP_F:
            return 0;
        case SDL_SCANCODE_KP_XOR:
            return 0;
        case SDL_SCANCODE_KP_POWER:
            return 0;
        case SDL_SCANCODE_KP_PERCENT:
            return 0;
        case SDL_SCANCODE_KP_LESS:
            return 0;
        case SDL_SCANCODE_KP_GREATER:
            return 0;
        case SDL_SCANCODE_KP_AMPERSAND:
            return 0;
        case SDL_SCANCODE_KP_DBLAMPERSAND:
            return 0;
        case SDL_SCANCODE_KP_VERTICALBAR:
            return 0;
        case SDL_SCANCODE_KP_DBLVERTICALBAR:
            return 0;
        case SDL_SCANCODE_KP_COLON:
            return 0;
        case SDL_SCANCODE_KP_HASH:
            return 0;
        case SDL_SCANCODE_KP_SPACE:
            return 0;
        case SDL_SCANCODE_KP_AT:
            return 0;
        case SDL_SCANCODE_KP_EXCLAM:
            return 0;
        case SDL_SCANCODE_KP_MEMSTORE:
            return 0;
        case SDL_SCANCODE_KP_MEMRECALL:
            return 0;
        case SDL_SCANCODE_KP_MEMCLEAR:
            return 0;
        case SDL_SCANCODE_KP_MEMADD:
            return 0;
        case SDL_SCANCODE_KP_MEMSUBTRACT:
            return 0;
        case SDL_SCANCODE_KP_MEMMULTIPLY:
            return 0;
        case SDL_SCANCODE_KP_MEMDIVIDE:
            return 0;
        case SDL_SCANCODE_KP_PLUSMINUS:
            return 0;
        case SDL_SCANCODE_KP_CLEAR:
            return 0;
        case SDL_SCANCODE_KP_CLEARENTRY:
            return 0;
        case SDL_SCANCODE_KP_BINARY:
            return 0;
        case SDL_SCANCODE_KP_OCTAL:
            return 0;
        case SDL_SCANCODE_KP_DECIMAL:
            return 0;
        case SDL_SCANCODE_KP_HEXADECIMAL:
            return 0;
        case SDL_SCANCODE_LCTRL:
            return VK_CONTROL;
        case SDL_SCANCODE_LSHIFT:
            return VK_SHIFT;
        case SDL_SCANCODE_LALT:
            return VK_MENU;
        case SDL_SCANCODE_LGUI:
            return VK_LWIN;
        case SDL_SCANCODE_RCTRL:
            return VK_CONTROL;
        case SDL_SCANCODE_RSHIFT:
            return VK_SHIFT;
        case SDL_SCANCODE_RALT:
            return VK_MENU;
        case SDL_SCANCODE_RGUI:
            return VK_RWIN;
        case SDL_SCANCODE_MODE:
            return 0;
        case SDL_SCANCODE_AUDIONEXT:
            return 0;
        case SDL_SCANCODE_AUDIOPREV:
            return 0;
        case SDL_SCANCODE_AUDIOSTOP:
            return 0;
        case SDL_SCANCODE_AUDIOPLAY:
            return 0;
        case SDL_SCANCODE_AUDIOMUTE:
            return 0;
        case SDL_SCANCODE_MEDIASELECT:
            return 0;
        case SDL_SCANCODE_WWW:
            return 0;
        case SDL_SCANCODE_MAIL:
            return 0;
        case SDL_SCANCODE_CALCULATOR:
            return 0;
        case SDL_SCANCODE_COMPUTER:
            return 0;
        case SDL_SCANCODE_AC_SEARCH:
            return 0;
        case SDL_SCANCODE_AC_HOME:
            return 0;
        case SDL_SCANCODE_AC_BACK:
            return 0;
        case SDL_SCANCODE_AC_FORWARD:
            return 0;
        case SDL_SCANCODE_AC_STOP:
            return 0;
        case SDL_SCANCODE_AC_REFRESH:
            return 0;
        case SDL_SCANCODE_AC_BOOKMARKS:
            return 0;
        case SDL_SCANCODE_BRIGHTNESSDOWN:
            return 0;
        case SDL_SCANCODE_BRIGHTNESSUP:
            return 0;
        case SDL_SCANCODE_DISPLAYSWITCH:
            return 0;
        case SDL_SCANCODE_KBDILLUMTOGGLE:
            return 0;
        case SDL_SCANCODE_KBDILLUMDOWN:
            return 0;
        case SDL_SCANCODE_KBDILLUMUP:
            return 0;
        case SDL_SCANCODE_EJECT:
            return 0;
        case SDL_SCANCODE_SLEEP:
            return 0;
        case SDL_NUM_SCANCODES:
            return 0;
    }
}

// This is a combination of the window proc and the window proc "filter"

static bool bNeedToQuit = false;

bool Sys_IsAppActive()
{
    return sAppIsActivated;
}

static void KeyDownEvent(SDL_Scancode scanCode)
{
#if ORIGINAL_PS1_BEHAVIOR                  // OG Change - Allow for exiting save menu using controller
    const bool allowTyping = saveMenuOpen; // Allow typing if save menu is open
#else
    const bool allowTyping = !Input_GetInputEnabled_4EDDE0(); // Old method: Allow typing only if all other inputs disabled
#endif
    if (allowTyping)
    {
        // "Typing" input
        const s32 vk = sdl_key_to_win32_vkey(scanCode);

        if (vk >= VK_F1 && vk <= VK_F12)
        {
            return;
        }
        sLastPressedKey = vk;

        // LOG_INFO("Key down (input disabled) " << sLastPressedKey);

        // Between A-Z
        if (sLastPressedKey >= 0x41 && sLastPressedKey <= 0x5A)
        {
            sLastPressedKey -= 0x41;

            if (SDL_GetModState() & (KMOD_SHIFT | KMOD_CAPS))
            {
                sLastPressedKey += 'A';
            }
            else
            {
                sLastPressedKey += 'a';
            }
        }
        // Between 0-9
        else if (sLastPressedKey >= VK_NUMPAD0 && sLastPressedKey <= VK_NUMPAD9)
        {
            sLastPressedKey -= VK_NUMPAD0;
            LOG_INFO("%d", sLastPressedKey);
            if (SDL_GetModState() & (KMOD_SHIFT) && sLastPressedKey == 1)
            {
                sLastPressedKey = '!';
            }
            else
            {
                sLastPressedKey += '0';
            }
        }

        else if (sLastPressedKey == VK_SUBTRACT)
        {
            sLastPressedKey = '-';
        }

        sIsAKeyDown = true;
    }
    else
    {
        // "Game button" input
        const s32 vk = sdl_key_to_win32_vkey(scanCode);
        // LOG_INFO("Key down " << vk);

        Input_SetKeyState(vk, 1);

        if (vk == VK_F5)
        {
            LOG_INFO("Save next frame for %d", VK_F5);
            gQuicksave_SaveNextFrame = true;
        }
        else if (vk == VK_F6)
        {
            LOG_INFO("Load next frame for %d", VK_F6);
            gQuicksave_LoadNextFrame = true;
        }
        else if (vk == VK_F9)
        {
            IRenderer::GetRenderer()->ToggleUseOriginalResolution();
        }
        else if (vk == VK_F10)
        {
            IRenderer::GetRenderer()->ToggleFilterScreen();
        }
        else if (vk == VK_F11)
        {
            IRenderer::GetRenderer()->ToggleKeepAspectRatio();
        }
        else if (vk == VK_F12)
        {
            const Uint32 flags = SDL_GetWindowFlags(Sys_GetWindowHandle());
            if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
            {
                SDL_SetWindowFullscreen(Sys_GetWindowHandle(), 0);
            }
            else
            {
                SDL_SetWindowFullscreen(Sys_GetWindowHandle(), SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
        }
    }
}

static void KeyUpEvent(SDL_Scancode scanCode)
{
    const s32 vk = sdl_key_to_win32_vkey(scanCode);
    // LOG_INFO("Key up " << vk);
    Input_SetKeyState(vk, 0);
    sIsAKeyDown = false;
    sLastPressedKey = 0;
}

static void QuitEvent(bool isRecordedEvent, bool isRecording)
{
    if (gMovieSoundEntry)
    {
#if !USE_SDL2_SOUND
        LPDIRECTSOUNDBUFFER pDSoundBuffer = gMovieSoundEntry->field_4_pDSoundBuffer;
        if (pDSoundBuffer)
        {
            pDSoundBuffer->Stop();
        }
#endif
    }
    if (SND_Seq_Table_Valid())
    {
        SND_StopAll();
    }

    // Full screen message boxes act really strange.. so force window mode before we show it
    const Uint32 flags = SDL_GetWindowFlags(Sys_GetWindowHandle());
    bool forcedWindowMode = false;
    if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
    {
        forcedWindowMode = true;
        SDL_SetWindowFullscreen(Sys_GetWindowHandle(), 0);
        //RECT rect = {0, 0, 640, 240};
        //VGA_EndFrame(&rect);
    }

    bool actuallyQuit = false;
    if (isRecordedEvent)
    {
        // Reading the SDL_QUIT event
        const RecordedEvent recordedEvent = GetGameAutoPlayer().GetEvent();
        actuallyQuit = recordedEvent.mData ? true : false;
    }

    const MessageBoxButton recordedButtonResult = actuallyQuit ? MessageBoxButton::eYes : MessageBoxButton::eNo;
    const MessageBoxButton button = isRecordedEvent ? recordedButtonResult : Sys_MessageBox(Sys_GetWindowHandle(), "Do you really want to quit?", "R.E.L.I.V.E.", MessageBoxType::eQuestion);

    if (isRecording)
    {
        RecordedEvent recEvent;
        recEvent.mType = SDL_QUIT;
        recEvent.mData = button == MessageBoxButton::eYes ? 1 : 0;
        GetGameAutoPlayer().RecordEvent(recEvent);
    }

    if (SND_Seq_Table_Valid())
    {
        GetSoundAPI().mSND_Restart();
    }

#if !USE_SDL2_SOUND
    if (gMovieSoundEntry && gMovieSoundEntry->field_4_pDSoundBuffer)
    {
        gMovieSoundEntry->field_4_pDSoundBuffer->Play(0, 0, 1);
    }
#endif

    if (button == MessageBoxButton::eYes)
    {
        // So Sys_PumpMessages thinks we got an quit
        bNeedToQuit = true;
    }
    else
    {
        if (forcedWindowMode)
        {
            SDL_SetWindowFullscreen(Sys_GetWindowHandle(), SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
    }
}

s8 Sys_PumpMessages()
{
    GetGameAutoPlayer().SyncPoint(SyncPoints::PumpEventsStart);

    SDL_Event event = {};
    const bool isRecording = GetGameAutoPlayer().IsRecording();
    const bool isPlaying = GetGameAutoPlayer().IsPlaying();

    // Replay any recorded events
    if (isPlaying)
    {
        while (GetGameAutoPlayer().PeekNextType() == RecordTypes::EventPoint)
        {
            const RecordedEvent recordedEvent = GetGameAutoPlayer().GetEvent();
            // TODO: Recording SDL types directly might break across diff platforms/SDL2 versions/ports
            switch (recordedEvent.mType)
            {
                case SDL_KEYDOWN:
                    KeyDownEvent(static_cast<SDL_Scancode>(recordedEvent.mData));
                    break;

                case SDL_KEYUP:
                    KeyUpEvent(static_cast<SDL_Scancode>(recordedEvent.mData));
                    break;

                case 0:
                    // Hack for quit events, quit writes other events before the actual event data
                    // this is used to mark the start
                    QuitEvent(true, false);
                    break;

                default:
                    ALIVE_FATAL("Unknown event type %d", recordedEvent.mType);
            }
        }
    }

    // Even if playing back we still need to pump events for things to actually work, but we ignore
    // inputs. Any attempt to quit while playing back is an instant quit to avoid desyncs.
    while (SDL_PollEvent(&event))
    {
#if AUTO_SWITCH_CONTROLLER // OG Change - Automatically switches active controller (gamepad/keyboard)
        // Auto switch off during recording or playback as reading the ini
        // file at random times will desync.
        const bool allowAutoSwitch = !isRecording && !isPlaying;
        if (allowAutoSwitch)
        {
            if (event.type == SDL_JOYDEVICEADDED && !isRecording)
            {
                totalConnectedJoysticks++;
                LOG_INFO("User just inserted joystick!");
                Input_Init();
                Input().SetJoyStickEnabled(true);
            }
            else if (event.type == SDL_JOYDEVICEREMOVED && !isRecording)
            {
                totalConnectedJoysticks--;
                LOG_INFO("User just removed joystick!");

                if (totalConnectedJoysticks > 0)
                {
                    Input_Init(); // Ensures next joystick is usable
                }
                else
                {
                    Input().SetJoyStickEnabled(false);
                }
            }
        }
#endif // AUTO_SWITCH_CONTROLLER

        if (event.type == SDL_KEYDOWN)
        {
            if (!isPlaying)
            {
                KeyDownEvent(event.key.keysym.scancode);
            }

            if (isRecording)
            {
                RecordedEvent recEvent;
                recEvent.mType = event.type;
                recEvent.mData = static_cast<u32>(event.key.keysym.scancode);
                GetGameAutoPlayer().RecordEvent(recEvent);
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            if (!isPlaying)
            {
                KeyUpEvent(event.key.keysym.scancode);
            }

            if (isRecording)
            {
                RecordedEvent recEvent;
                recEvent.mType = event.type;
                recEvent.mData = static_cast<u32>(event.key.keysym.scancode);
                GetGameAutoPlayer().RecordEvent(recEvent);
            }
        }
        else if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.type == SDL_WINDOWEVENT_FOCUS_GAINED)
            {
                sAppIsActivated = true;
            }
            else if (event.window.type == SDL_WINDOWEVENT_FOCUS_LOST)
            {
                sAppIsActivated = false;
            }
            else if (event.window.type == SDL_WINDOWEVENT_EXPOSED)
            {
                // Add_Dirty_Area_4ED970(0, 0, 640, 240);
            }
            // SDL_WINDOWEVENT_SIZE_CHANGED
        }
        else if (event.type == SDL_QUIT)
        {
            if (!isPlaying)
            {
                // Required to write a dummy event first because the QuitEvent can write other events first
                // which breaks the playback logic of Peeking and looping until the type isn't event.
                if (isRecording)
                {
                    RecordedEvent recEvent;
                    recEvent.mType = 0;
                    recEvent.mData = 0;
                    GetGameAutoPlayer().RecordEvent(recEvent);
                }

                // Else alllow normal quit behaviour + record the result
                QuitEvent(false, isRecording);
            }
            else
            {
                // Force quit if attempting to close the game during playback
                bNeedToQuit = true;
            }
        }
    }

    GetGameAutoPlayer().SyncPoint(SyncPoints::PumpEventsEnd);

    if (bNeedToQuit)
    {
        return 1;
    }

    return 0;
}

TWindowHandleType Sys_GetWindowHandle()
{
    return sHwnd;
}

void Sys_SetWindowPos_4EE1B1(s32 width, s32 height)
{
    SDL_SetWindowSize(Sys_GetWindowHandle(), width, height);
    SDL_SetWindowPosition(Sys_GetWindowHandle(), 0, 0);
}

void Sys_DestroyWindow()
{
    if (sHwnd)
    {
        SDL_DestroyWindow(sHwnd);
        sHwnd = nullptr;
    }
}

bool Sys_WindowClass_Register(const char_type* lpWindowName, s32 x, s32 y, s32 nWidth, s32 nHeight, s32 extraAttributes)
{
    TRACE_ENTRYEXIT;

    sHwnd = SDL_CreateWindow(lpWindowName, x, y, nWidth, nHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN | extraAttributes);
    if (sHwnd)
    {
        LOG_INFO("Window created");

        Input_InitKeyStateArray_4EDD60();

        SDL_ShowCursor(SDL_DISABLE);

        // SDL will not send a window focused message on start up, so default to activated
        sAppIsActivated = true;
    }
    else
    {
        LOG_ERROR("Window create with flags %d failed with %s", extraAttributes, SDL_GetError());
    }
    return sHwnd != nullptr;
}


MessageBoxButton Sys_MessageBox(TWindowHandleType windowHandle, const char_type* message, const char_type* title, MessageBoxType type)
{
    SDL_MessageBoxData data = {};
    data.title = title;
    data.message = message;

    if (type == MessageBoxType::eQuestion)
    {
        const static SDL_MessageBoxButtonData buttons[] = {
            {0, 1, "No"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Yes"},
        };

        data.numbuttons = SDL_arraysize(buttons);
        data.buttons = buttons;
    }
    else
    {
        const static SDL_MessageBoxButtonData buttons[] = {
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK"},
        };

        data.numbuttons = SDL_arraysize(buttons);
        data.buttons = buttons;
    }

    data.window = windowHandle;

    switch (type)
    {
        case MessageBoxType::eStandard:
            data.flags = SDL_MESSAGEBOX_ERROR;
            break;
        case MessageBoxType::eError:
            data.flags = SDL_MESSAGEBOX_INFORMATION;
            break;
        case MessageBoxType::eQuestion:
            data.flags = SDL_MESSAGEBOX_WARNING;
            break;
        default:
            data.flags = SDL_MESSAGEBOX_ERROR;
    }

    s32 button = 0;

    SDL_ShowMessageBox(&data, &button);

    if (type == MessageBoxType::eQuestion)
    {
        if (button == 1)
        {
            return MessageBoxButton::eNo;
        }

        return MessageBoxButton::eYes;
    }

    return MessageBoxButton::eOK;
}
