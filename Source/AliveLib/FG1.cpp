#include "stdafx.h"
#include "FG1.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"

ALIVE_VAR(1, 0x5D1E28, DynamicArrayT<FG1>*, gFG1List_5D1E28, nullptr);

void FG1::ctor_499FC0(BYTE** /*pFg1Res*/)
{
    NOT_IMPLEMENTED();
}

void FG1::VDestructor(signed int flags)
{
    vdtor_49A1E0(flags);
}

void FG1::vdtor_49A1E0(signed int flags)
{
    dtor_49A540();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void FG1::dtor_49A540()
{
    SetVTable(this, 0x5469C8); // vTbl_FG1_5469C8
    gFG1List_5D1E28->Remove_Item(this);
    ResourceManager::FreeResource_49C330(field_2C_ptr);
    BaseGameObject_dtor_4DBEC0();
}
