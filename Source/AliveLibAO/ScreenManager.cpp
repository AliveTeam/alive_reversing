#include "stdafx_ao.h"
#include "ScreenManager.hpp"
#include "Function.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FF7C8, ScreenManager*, pScreenManager_4FF7C8, nullptr);

EXPORT void Camera::dtor_444700()
{
    NOT_IMPLEMENTED();
}

EXPORT void AO::ScreenManager::MoveImage_406C40()
{
    NOT_IMPLEMENTED();
}

EXPORT void AO::ScreenManager::DecompressToVRam_407110(unsigned __int16** /*ppBits*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void AO::ScreenManager::InvalidateRect_406CC0(int /*x*/, int /*y*/, signed int /*width*/, signed int /*height*/)
{
    NOT_IMPLEMENTED();
}

EXPORT ScreenManager* AO::ScreenManager::ctor_406830(BYTE** /*ppBits*/, FP_Point* /*pCameraOffset*/)
{
    NOT_IMPLEMENTED();
    return this;
}

AO::BaseGameObject* ScreenManager::VDestructor(signed int flags)
{
    return vdtor_407290(flags);
}


EXPORT ScreenManager* AO::ScreenManager::vdtor_407290(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

END_NS_AO
