#include "stdafx.h"
#include "TrapDoor.hpp"
#include "Function.hpp"

BaseGameObject* TrapDoor::VDestructor(signed int flags)
{
    return vdtor_4DD8A0(flags);
}

void TrapDoor::VUpdate()
{
    vUpdate_4DDA90();
}

void TrapDoor::VRender(int** pOrderingTable)
{
    vRender_4DDDD0(pOrderingTable);
}

void TrapDoor::VScreenChanged()
{
    vScreenChanged_4DDE40();
}

int TrapDoor::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_4DE050(pSaveBuffer);
}

PSX_RECT* TrapDoor::vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx)
{
    return vGetBoundingRect_4DD870(pRect, pointIdx);
}

EXPORT BaseGameObject* TrapDoor::vdtor_4DD8A0(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

EXPORT void TrapDoor::vUpdate_4DDA90()
{
    NOT_IMPLEMENTED();
}

EXPORT void TrapDoor::vRender_4DDDD0(int **ot)
{
    field_B8_xpos += FP_FromInteger(field_13A_xOff);
    BaseAliveGameObject::VRender(ot);
    field_B8_xpos -= FP_FromInteger(field_13A_xOff);
}

EXPORT void TrapDoor::vScreenChanged_4DDE40()
{
    NOT_IMPLEMENTED();
}

EXPORT int TrapDoor::vGetSaveState_4DE050(BYTE*)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT PSX_RECT* TrapDoor::vGetBoundingRect_4DD870(PSX_RECT* pRect, int /*not_used*/)
{
    *pRect = field_148_bounding_rect;
    return pRect;
}

int TrapDoor::Vsub_4DDD90(int a1)
{
    return sub_4DDD90(a1);
}

void TrapDoor::Vsub_4DDDB0(BaseGameObject* a1)
{
    sub_4DDDB0(a1);
}

EXPORT int TrapDoor::sub_4DDD90(int /*a1*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void TrapDoor::sub_4DDDB0(BaseGameObject* /*a1*/)
{
    NOT_IMPLEMENTED();
}
