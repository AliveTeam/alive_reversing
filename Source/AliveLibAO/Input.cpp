#include "stdafx_ao.h"
#include "Input.hpp"
#include "Function.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5009E8, InputObject, sInputObject_5009E8, {});
ALIVE_VAR(1, 0x5076B8, unsigned __int16, sCurrentControllerIndex_5076B8, 0);

const InputCommands sInputKey_Right_4C6590 = eRight;
const InputCommands sInputKey_Left_4C6594 = eLeft;
const InputCommands sInputKey_Up_4C6598 = eUp;
const InputCommands sInputKey_Down_4C659C = eDown;
const InputCommands sInputKey_Hop_4C65A0 = eHop;
const InputCommands sInputKey_DoAction_4C65A4 = eDoAction;
const InputCommands sInputKey_Run_4C65A8 = eRun;
const InputCommands sInputKey_Sneak_4C65AC = eSneak;
const InputCommands sInputKey_FartRoll_4C65B0 = eFartOrRoll;
const InputCommands sInputKey_ThrowItem_4C65B4 = eThrowItem;

const InputCommands sInputKey_LeftGameSpeakEnabler_4C65B8 = eGameSpeak6;
const InputCommands sInputKey_GameSpeak1_4C65C8 = eHop;
const InputCommands sInputKey_GameSpeak2_4C65BC = eDoAction;
const InputCommands sInputKey_GameSpeak3_4C65C0 = eThrowItem;
const InputCommands sInputKey_GameSpeak4_4C65C4 = eFartOrRoll;

const InputCommands sInputKey_RightGameSpeakEnabler_4C65DC = eGameSpeak3;
const InputCommands sInputKey_GameSpeak5_4C65EC = eFartOrRoll;
const InputCommands sInputKey_GameSpeak6_4C65E8 = eHop;
const InputCommands sInputKey_GameSpeak7_4C65E4 = eThrowItem;
const InputCommands sInputKey_GameSpeak8_4C65E0 = eDoAction;

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

void CC Input_SetKeyState_48E610(int /*key*/, char /*bIsDown*/)
{
    NOT_IMPLEMENTED();
}

void Input_InitKeyStateArray_48E5F0()
{
    NOT_IMPLEMENTED();
}

END_NS_AO
