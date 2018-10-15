#include "stdafx.h"
#include "Movie.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "stdlib.hpp"

void Movie::VDestructor(signed int flags)
{
    vdtor_4DFE80(flags);
}

void Movie::VUpdate()
{
    vUpdate_4E0030();
}

void Movie::VScreenChanged()
{
    // Null sub 0x4E02A0
}

EXPORT void Movie::Init_4DFF60(int /*a2*/, CdlLOC* /*pCdPos*/, __int16 /*bUnknown*/, __int16 /*a5*/, __int16 /*a6*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void Movie::ctor_4DFDE0(int id, DWORD pos, __int16 a4, __int16 a5, __int16 volume)
{
    NOT_IMPLEMENTED(); // TODO FIX ME - causes a hang

    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x547EF4); // vTbl_Movie_547EF4

    CdlLOC cdLoc = {};
    PSX_Pos_To_CdLoc_4FADD0(pos, &cdLoc);
    Init_4DFF60(id, &cdLoc, a4, a5, volume);
}

EXPORT void Movie::vUpdate_4E0030()
{
    NOT_IMPLEMENTED();
}

EXPORT void Movie::vdtor_4DFE80(signed int flags)
{
    BaseGameObject_dtor_4DBEC0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}
