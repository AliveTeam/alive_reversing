#include "stdafx.h"
#include "ScreenManager.hpp"
#include "Function.hpp"

ALIVE_VAR(1, 0x5BB5F4, ScreenManager*, pScreenManager_5BB5F4, nullptr);

Camera::Camera()
{
    ctor_480DD0();
}

void Camera::ctor_480DD0()
{
    NOT_IMPLEMENTED();
}

void Camera::dtor_480E00()
{
    NOT_IMPLEMENTED();
}

void ScreenManager::sub_40EE10()
{
    NOT_IMPLEMENTED();
}


void ScreenManager::sub_cam_vlc_40EF60(unsigned __int16** a2)
{
    NOT_IMPLEMENTED();
}

void ScreenManager::MoveImage_40EB70()
{
    NOT_IMPLEMENTED();
}

void ScreenManager::VDestructor(signed int flags)
{
    dtor_40E460(flags);
}

void ScreenManager::dtor_40E460(signed int flags)
{
    NOT_IMPLEMENTED();
}

void ScreenManager::InvalidateRect_40EC90(int x, int y, signed int width, signed int height, int a6)
{
    NOT_IMPLEMENTED();
}

void ScreenManager::InvalidateRect_40EC10(int x, int y, signed int width, signed int height)
{
    InvalidateRect_40EC90(x, y, width, height, field_3A);
}

ScreenManager::ScreenManager(int a2, int a3)
{
    ctor_40E3E0(a2, a3);
}

void ScreenManager::ctor_40E3E0(int a2, int a3)
{
    NOT_IMPLEMENTED();
}
