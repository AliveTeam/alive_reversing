#include "stdafx_ao.h"
#include "ScreenManager.hpp"
#include "Function.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FF7C8, ScreenManager*, pScreenManager_4FF7C8, nullptr);

EXPORT Camera* Camera::ctor_4446E0()
{
    field_0_array.ctor_4043E0(10);
    field_30_flags &= ~1u;
    field_C_ppBits = nullptr;
    return this;
}

EXPORT void Camera::dtor_444700()
{
    NOT_IMPLEMENTED();
}


EXPORT void CC Camera::On_Loaded_4447A0(Camera* /*pThis*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void ScreenManager::MoveImage_406C40()
{
    NOT_IMPLEMENTED();
}

EXPORT void ScreenManager::DecompressToVRam_407110(unsigned __int16** /*ppBits*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void ScreenManager::InvalidateRect_406CC0(int /*x*/, int /*y*/, signed int /*width*/, signed int /*height*/)
{
    NOT_IMPLEMENTED();
}

EXPORT ScreenManager* ScreenManager::ctor_406830(BYTE** /*ppBits*/, FP_Point* /*pCameraOffset*/)
{
    NOT_IMPLEMENTED();
    return this;
}

BaseGameObject* ScreenManager::VDestructor(signed int flags)
{
    return vdtor_407290(flags);
}

void ScreenManager::UnsetDirtyBits_FG1_406EF0()
{
    NOT_IMPLEMENTED();
}

EXPORT ScreenManager* ScreenManager::vdtor_407290(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

END_NS_AO
