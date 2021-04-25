#pragma once

#include "SDL.h"

typedef enum
{
    TOUCH_CONTROLLER_BUTTON_INVALID = -1,
    TOUCH_CONTROLLER_BUTTON_A,
    TOUCH_CONTROLLER_BUTTON_B,
    TOUCH_CONTROLLER_BUTTON_X,
    TOUCH_CONTROLLER_BUTTON_Y,
    TOUCH_CONTROLLER_BUTTON_BACK,
    TOUCH_CONTROLLER_BUTTON_GUIDE,
    TOUCH_CONTROLLER_BUTTON_START,
    TOUCH_CONTROLLER_BUTTON_LEFTSTICK,
    TOUCH_CONTROLLER_BUTTON_RIGHTSTICK,
    TOUCH_CONTROLLER_BUTTON_LEFTSHOULDER,
    TOUCH_CONTROLLER_BUTTON_RIGHTSHOULDER,
    TOUCH_CONTROLLER_BUTTON_LEFTTRIGGER,
    TOUCH_CONTROLLER_BUTTON_RIGHTTRIGGER,
    TOUCH_CONTROLLER_BUTTON_DPAD_UP,
    TOUCH_CONTROLLER_BUTTON_DPAD_DOWN,
    TOUCH_CONTROLLER_BUTTON_DPAD_LEFT,
    TOUCH_CONTROLLER_BUTTON_DPAD_RIGHT,
    TOUCH_CONTROLLER_BUTTON_MAX
} TouchControllerButton;

struct TouchButton
{
    f32 x;
    f32 y;
    f32 size;
    TouchControllerButton button;
};


class TouchController
{
public:
    TouchController();
    ~TouchController();

    static void Init();
    static void Close();

    void Update();
    void Render();

    s32 GetGamePadData(f32 *pX1, f32 *pY1, f32 *pX2, f32 *pY2, DWORD *pButtons);

private:

    void setState(TouchControllerButton b, bool s);
    bool getState(TouchControllerButton b);
    Uint32 state;
    std::vector<TouchButton> touchButtons;

    f32 xRatio;
    f32 yRatio;

    std::vector<SDL_TouchID> touchDevices;
};

extern TouchController * gTouchController;