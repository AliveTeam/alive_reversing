#include "stdafx.h"
#include "TouchController.hpp"
#include "Sys.hpp"
#include "VGA.hpp"

TouchController * gTouchController = nullptr;



void sdl_ellipse(SDL_Renderer* r, int x0, int y0, int radiusX, int radiusY, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    SDL_SetRenderDrawColor(r, red, green, blue, alpha);
    SDL_SetRenderDrawBlendMode(r, SDL_BlendMode::SDL_BLENDMODE_BLEND);

    float pi = 3.14159265f;
    float pih = pi / 2.0f; //half of pi

                          //drew  28 lines with   4x4  circle with precision of 150 0ms
                          //drew 132 lines with  25x14 circle with precision of 150 0ms
                          //drew 152 lines with 100x50 circle with precision of 150 3ms
    const int prec = 27; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0;     // angle that will be increased each loop

                         //starting point
    int x = (int)((float)radiusX * cos(theta));//start point
    int y = (int)((float)radiusY * sin(theta));//start point
    int x1 = x;
    int y1 = y;

    //repeat until theta >= 90;
    float step = pih / (float)prec; // amount to add to theta each time (degrees)
    for (theta = step; theta <= pih; theta += step)//step through only a 90 arc (1 quadrant)
    {
        //get new point location
        x1 = static_cast<int>((float)radiusX * cosf(theta) + 0.5); //new point (+.5 is a quick rounding method)
        y1 = static_cast<int>((float)radiusY * sinf(theta) + 0.5); //new point (+.5 is a quick rounding method)

                                                 //draw line from previous point to new point, ONLY if point incremented
        if ((x != x1) || (y != y1))//only draw if coordinate changed
        {
            SDL_RenderDrawLine(r, x0 + x, y0 - y, x0 + x1, y0 - y1);//quadrant TR
            SDL_RenderDrawLine(r, x0 - x, y0 - y, x0 - x1, y0 - y1);//quadrant TL
            SDL_RenderDrawLine(r, x0 - x, y0 + y, x0 - x1, y0 + y1);//quadrant BL
            SDL_RenderDrawLine(r, x0 + x, y0 + y, x0 + x1, y0 + y1);//quadrant BR
        }
        //save previous points
        x = x1;//save new previous point
        y = y1;//save new previous point
    }
    //arc did not finish because of rounding, so finish the arc
    if (x != 0)
    {
        x = 0;
        SDL_RenderDrawLine(r, x0 + x, y0 - y, x0 + x1, y0 - y1);//quadrant TR
        SDL_RenderDrawLine(r, x0 - x, y0 - y, x0 - x1, y0 - y1);//quadrant TL
        SDL_RenderDrawLine(r, x0 - x, y0 + y, x0 - x1, y0 + y1);//quadrant BL
        SDL_RenderDrawLine(r, x0 + x, y0 + y, x0 + x1, y0 + y1);//quadrant BR
    }
}

TouchController::TouchController()
{
    for (int i = 0; i < SDL_GetNumTouchDevices(); i++)
    {
        touchDevices.push_back(SDL_GetTouchDevice(i));
    }
    

    {
        const float offsetX = 1100;
        const float offsetY = 520;
        const float size = 100;
        const float spaceMult = 1.0f;
        const float spaceOffset = (size * spaceMult);
        touchButtons.push_back({ offsetX, offsetY - spaceOffset, size,TOUCH_CONTROLLER_BUTTON_Y });
        touchButtons.push_back({ offsetX, offsetY + spaceOffset, size,TOUCH_CONTROLLER_BUTTON_A });
        touchButtons.push_back({ offsetX - spaceOffset, offsetY, size,TOUCH_CONTROLLER_BUTTON_X });
        touchButtons.push_back({ offsetX + spaceOffset, offsetY, size,TOUCH_CONTROLLER_BUTTON_B });
    }

    {
        const float offsetX = 180;
        const float offsetY = 520;
        const float size = 100;
        const float spaceMult = 1.0f;
        const float spaceOffset = (size * spaceMult);
        touchButtons.push_back({ offsetX, offsetY - spaceOffset, size, TOUCH_CONTROLLER_BUTTON_DPAD_UP});
        touchButtons.push_back({ offsetX, offsetY + spaceOffset, size, TOUCH_CONTROLLER_BUTTON_DPAD_DOWN });
        touchButtons.push_back({ offsetX - spaceOffset, offsetY, size, TOUCH_CONTROLLER_BUTTON_DPAD_LEFT });
        touchButtons.push_back({ offsetX + spaceOffset, offsetY, size, TOUCH_CONTROLLER_BUTTON_DPAD_RIGHT });
    }

    touchButtons.push_back({ 540, 100, 60, TOUCH_CONTROLLER_BUTTON_BACK });
    touchButtons.push_back({ 740, 100, 60, TOUCH_CONTROLLER_BUTTON_START });

    touchButtons.push_back({ 80, 300, 80, TOUCH_CONTROLLER_BUTTON_LEFTTRIGGER });
    touchButtons.push_back({ 180, 300, 80, TOUCH_CONTROLLER_BUTTON_LEFTSHOULDER });

    touchButtons.push_back({ 1200, 300, 80, TOUCH_CONTROLLER_BUTTON_RIGHTTRIGGER });
    touchButtons.push_back({ 1100, 300, 80, TOUCH_CONTROLLER_BUTTON_RIGHTSHOULDER });

    touchButtons.push_back({ 80, 40, 80, TOUCH_CONTROLLER_BUTTON_LEFTTRIGGER });
    touchButtons.push_back({ 80, 40, 80, TOUCH_CONTROLLER_BUTTON_LEFTSHOULDER });

#if _WIN32
    s_VGA_KeepAspectRatio = true;
    SDL_SetWindowSize(Sys_GetWindowHandle_4EE180(), 1280, 720);
#endif
}

TouchController::~TouchController()
{
}

void TouchController::Init()
{
    if (gTouchController == nullptr)
    {
        gTouchController = new TouchController();
    }
}

void TouchController::Close()
{
    if (gTouchController != nullptr)
    {
        delete gTouchController;
    }
}

void TouchController::Update()
{
    SDL_ShowCursor(1);

    state = 0;


    for (SDL_TouchID tid : touchDevices)
    {
        const int touchCount = SDL_GetNumTouchFingers(tid);

        for (int i = 0; i < touchCount; i++)
        {
            SDL_Finger * pFinger = SDL_GetTouchFinger(tid, i);

            for (auto t : touchButtons)
            {
                const float x = t.x;
                const float y = t.y;
                const float s = t.size * 0.5f;

                const float fingerX = pFinger->x * 1280.0f;
                const float fingerY = pFinger->y * 720.0f;

                if (fingerX > x - s && fingerX < x + s &&
                    fingerY > y - s && fingerY < y + s)
                {
                    setState(t.button, true);
                }
            }
        }
    }
    

#if _WIN32
    for (auto t : touchButtons)
    {
        int mx, my;
        int ms = SDL_GetMouseState(&mx, &my);

        const float x = t.x * xRatio;
        const float y = t.y * yRatio;
        const float s = t.size * yRatio * 0.5f;

        if (mx > x - s && mx < x + s &&
            my > y - s && my < y + s && ms > 0)
        {
            setState(t.button, true);
        }
    }
#endif
}

void TouchController::Render()
{
    SDL_Renderer * pRenderer = SDL_GetRenderer(Sys_GetWindowHandle_4EE180());

    int w, h;
    SDL_GetWindowSize(Sys_GetWindowHandle_4EE180(), &w, &h);

    xRatio = w / 1280.0f;
    yRatio = h / 720.0f;

    for (auto t : touchButtons)
    {
        sdl_ellipse(pRenderer, static_cast<int>(t.x * xRatio), static_cast<int>(t.y * yRatio), static_cast<int>((t.size * yRatio) / 2), static_cast<int>((t.size * yRatio) / 2), 255, 0, 0, 127);
    }
}

int TouchController::GetGamePadData(float *pX1, float *pY1, float *pX2, float *pY2, DWORD *pButtons)
{
    *pButtons = 0;
    *pX1 = 0;
    *pY1 = 0;
    *pX2 = 0;
    *pY2 = 0;

    // DPad Movement
    if (getState(TOUCH_CONTROLLER_BUTTON_DPAD_RIGHT))
        *pX1 = 1;
    else if (getState(TOUCH_CONTROLLER_BUTTON_DPAD_LEFT))
        *pX1 = -1;
    if (getState(TOUCH_CONTROLLER_BUTTON_DPAD_UP))
        *pY1 = -1;
    else if (getState(TOUCH_CONTROLLER_BUTTON_DPAD_DOWN))
        *pY1 = 1;


#define M_SDLTOUCHPAD_BIND(BIT, PAD_BUTTON){if (getState(PAD_BUTTON) == true) {*pButtons |= (1 << BIT);} }


    M_SDLTOUCHPAD_BIND(0, TOUCH_CONTROLLER_BUTTON_X);
    M_SDLTOUCHPAD_BIND(1, TOUCH_CONTROLLER_BUTTON_A);
    M_SDLTOUCHPAD_BIND(2, TOUCH_CONTROLLER_BUTTON_B);
    M_SDLTOUCHPAD_BIND(3, TOUCH_CONTROLLER_BUTTON_Y);
    M_SDLTOUCHPAD_BIND(4, TOUCH_CONTROLLER_BUTTON_LEFTSHOULDER);
    M_SDLTOUCHPAD_BIND(5, TOUCH_CONTROLLER_BUTTON_RIGHTSHOULDER);
    M_SDLTOUCHPAD_BIND(8, TOUCH_CONTROLLER_BUTTON_BACK);
    M_SDLTOUCHPAD_BIND(9, TOUCH_CONTROLLER_BUTTON_START);

    if (getState(TOUCH_CONTROLLER_BUTTON_LEFTTRIGGER))
        *pButtons |= (1 << 6);
    if (getState(TOUCH_CONTROLLER_BUTTON_RIGHTTRIGGER))
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

    /*static float vibrationAmount = 0.0f;
    int screenShake = std::max(abs(sScreenXOffSet_BD30E4), abs(sScreenYOffset_BD30A4));

    if (screenShake > 0)
    {
        vibrationAmount = std::min(screenShake, 30) / 30.0f;
    }
    else if (Event_Get_422C00(kEventScreenShake))
    {
        vibrationAmount = 1.0f;
    }



    if (pSDLControllerHaptic != nullptr)
    {
        SDL_HapticRumblePlay(pSDLControllerHaptic, vibrationAmount, 200);
    }

    vibrationAmount -= 0.2f;
    vibrationAmount = std::max(0.0f, vibrationAmount);*/

    if (state == 0 && pX1 == 0 && pY1 == 0 && pX2 == 0 && pY2 == 0)
    {
        return 0;
    }

    return 1;
}

void TouchController::setState(TouchControllerButton b, bool s)
{
    if (s)
    {
        state |= (1 << b);
    }
    else
    {
        state &= ~(1 << b);
    }
}

bool TouchController::getState(TouchControllerButton b)
{
    return (state & (1 << b)) > 0;
}
