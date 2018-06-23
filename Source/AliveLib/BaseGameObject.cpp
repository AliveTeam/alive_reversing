#include "stdafx.h"
#include "BaseGameObject.hpp"
#include "Function.hpp"

void BaseGameObject::VUpdate()
{
    // Empty 0x4DC080
}

void BaseGameObject::VRender(int* /*pOrderingTable*/)
{
    // Empty 0x4DBF80
}

void BaseGameObject::vsub_4DC0A0()
{
    NOT_IMPLEMENTED();
}

void BaseGameObject::vnullsub_4DC0F0()
{
    // Empty 0x4DC0F0
}

int BaseGameObject::GetSaveState_4DC110(BYTE* /*pSaveBuffer*/)
{
    return 0;
}