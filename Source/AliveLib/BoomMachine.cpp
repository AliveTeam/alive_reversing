#include "stdafx.h"
#include "BoomMachine.hpp"
#include "Function.hpp"
#include "ObjectIds.hpp"
#include "stdlib.hpp"
#include "Path.hpp"

BaseGameObject* BoomMachine::VDestructor(signed int flags)
{
    return vdtor_445E10(flags);
}

void BoomMachine::VUpdate()
{
    vUpdate_445F50();
}

void BoomMachine::VScreenChanged()
{
    vScreenChange_446020();
}

BOOL BoomMachine::Vsub_445DF0()
{
    return vsub_445DF0();
}

BaseGameObject* BoomMachine::Vsub_445F00()
{
    return vsub_445F00();
}

void BoomMachine::vUpdate_445F50()
{
    NOT_IMPLEMENTED();
}

void BoomMachine::vScreenChange_446020()
{
    NOT_IMPLEMENTED();
}

BOOL BoomMachine::vsub_445DF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

BaseGameObject* BoomMachine::vsub_445F00()
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void BoomMachine::dtor_445E40()
{
    SetVTable(this, 0x5455C4); // vTbl_GrenadeMachine_5455C4

    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_F8);
    if (pObj)
    {
        pObj->field_6_flags.Set(BaseGameObject::eDead);;
    }
    Path::TLV_Reset_4DB8E0(field_F4, -1, 0, 0);

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

BoomMachine* BoomMachine::vdtor_445E10(signed int flags)
{
    dtor_445E40();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

