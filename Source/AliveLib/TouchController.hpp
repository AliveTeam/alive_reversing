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
    float x;
    float y;
    float size;
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

    int GetGamePadData(float *pX1, float *pY1, float *pX2, float *pY2, DWORD *pButtons);

private:

    void setState(TouchControllerButton b, bool s);
    bool getState(TouchControllerButton b);
    Uint32 state;
    std::vector<TouchButton> touchButtons;

    float xRatio;
    float yRatio;

    std::vector<SDL_TouchID> touchDevices;
};

extern TouchController * gTouchController;