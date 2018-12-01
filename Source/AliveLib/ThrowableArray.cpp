#include "stdafx.h"
#include "ThrowableArray.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"

ALIVE_VAR(1, 0x5d1e2c, ThrowableArray*, gpThrowableArray_5D1E2C, nullptr);

void __stdcall FreeResourceArray_49AEC0(DynamicArray*)
{
    NOT_IMPLEMENTED();
}

void CC LoadRockTypes_49AB30(unsigned __int16 /*a1*/, unsigned __int16 /*a2*/)
{
    NOT_IMPLEMENTED();
}

ThrowableArray* ThrowableArray::ctor_49A630()
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    field_24_throwables.ctor_40C9E0(0);
    field_6_flags.Clear(BaseGameObject::eUpdatable);
    SetVTable(this, 0x5469E4);
    field_20_count = 0;
    gpThrowableArray_5D1E2C = this;
    field_22_flags &= ~7u;
    return this;
}

void ThrowableArray::dtor_49A6F0()
{
    SetVTable(this, 0x5469E4);
    gpThrowableArray_5D1E2C = 0;
    FreeResourceArray_49AEC0(&field_24_throwables);
    field_24_throwables.dtor_40CAD0();
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* ThrowableArray::vdtor_49A6C0(signed int flags)
{
    dtor_49A6F0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ThrowableArray::Remove_49AA00(__int16 count)
{
    field_20_count -= count;
    if (field_20_count > 0)
    {
        if (field_22_flags & 1 && field_22_flags & 2)
        {
            FreeResourceArray_49AEC0(&field_10_resources_array);
            field_22_flags &= ~2u;
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void ThrowableArray::vUpdate_49AA50()
{
    if (field_22_flags & 1)
    {
        LoadRockTypes_49AB30(gMap_5C3030.sCurrentLevelId_5C3030, gMap_5C3030.sCurrentPathId_5C3032);
        Add_49A7A0(0);
        field_22_flags &= ~1u;
        field_6_flags.Clear(BaseGameObject::eUpdatable);
    }
}

signed int ThrowableArray::vGetSaveState_49B2A0(BYTE* pSaveBuffer)
{
    ThrowableArray_SaveState* pState = reinterpret_cast<ThrowableArray_SaveState*>(pSaveBuffer);
    pState->field_0_never_used = 102; // never gets read back, no idea what it means :)
    pState->field_2_item_count = field_20_count;
    return sizeof(ThrowableArray_SaveState);
}

void ThrowableArray::vScreenChange_49AAA0()
{
    NOT_IMPLEMENTED();
}

void ThrowableArray::Add_49A7A0(__int16 /*count*/)
{
    NOT_IMPLEMENTED();
}

signed int CC ThrowableArray::CreateFromSaveState_49B200(const BYTE* pState)
{
    LoadRockTypes_49AB30(gMap_5C3030.sCurrentLevelId_5C3030, gMap_5C3030.sCurrentPathId_5C3032);
    auto pArray = alive_new<ThrowableArray>();
    if (pArray)
    {
        pArray->ctor_49A630();
    }
    pArray->Add_49A7A0(reinterpret_cast<const ThrowableArray_SaveState*>(pState)->field_2_item_count);
    return sizeof(ThrowableArray_SaveState);
}

BaseGameObject* ThrowableArray::VDestructor(signed int flags)
{
    return vdtor_49A6C0(flags);
}

void ThrowableArray::VUpdate()
{
    vUpdate_49AA50();
}

void ThrowableArray::VRender(int** /*pOrderingTable*/)
{
    // Empty 0x4DBF80
}

int ThrowableArray::GetSaveState_4DC110(BYTE* pSaveBuffer)
{
    return vGetSaveState_49B2A0(pSaveBuffer);
}

void ThrowableArray::VScreenChanged()
{
    vScreenChange_49AAA0();
}
