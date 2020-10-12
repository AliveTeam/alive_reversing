#include "stdafx_ao.h"
#include "Input.hpp"
#include "Function.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5009E8, InputObject, sInputObject_5009E8, {});
ALIVE_VAR(1, 0x5076B8, unsigned __int16, sCurrentControllerIndex_5076B8, 0);

EXPORT void InputObject::InitPad_4331A0(unsigned int /*padCount*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void InputObject::Update_433250()
{
    NOT_IMPLEMENTED();
}

EXPORT void CC InputObject::Shutdown_433230()
{
    NOT_IMPLEMENTED();
}

bool InputObject::isPressed(DWORD command)
{
    return (this->field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed & command) != 0;
}

bool InputObject::isHeld(DWORD command)
{
    return (this->field_0_pads[sCurrentControllerIndex_5076B8].field_6_held & command) != 0;
}

bool InputObject::isReleased(DWORD command)
{
     return (this->field_0_pads[sCurrentControllerIndex_5076B8].field_8_released & command) != 0;
}

BOOL CC Input_IsChanting_4334C0()
{
    return (sInputObject_5009E8.field_0_pads[sCurrentControllerIndex_5076B8].field_0_pressed & sInputKey_Chant) == sInputKey_Chant;
}

END_NS_AO
