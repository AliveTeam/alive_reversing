#include "stdafx_ao.h"
#include "Primitives.hpp"
#include "Function.hpp"

START_NS_AO

void CC Init_SetTPage_495FB0(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage)
{
    SetUnknown(&pPrim->mBase);
    SetCode(&pPrim->mBase, PrimTypeCodes::eSetTPage);
    pPrim->field_C_tpage = tpage;
}

int CC PSX_getTPage_4965D0(char tp, char abr, int x, __int16 y)
{
    return ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | (((y) & 0x200) << 2));
}

void CC Poly_Set_Blending_498A00(PrimHeader* /*pPrim*/, int /*bBlending*/)
{
    NOT_IMPLEMENTED();
}

int CC PSX_getClut_496840(int x, int y)
{
    return (y << 6) | (x >> 4) & 63;
}

void CC Poly_Set_SemiTrans_498A40(PrimHeader* /*pPrim*/, int /*bSemiTrans*/)
{
    NOT_IMPLEMENTED();
}

void CC Sprt_Init(Prim_Sprt* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eSprt);
}

void CC Init_PrimClipper_495FD0(Prim_PrimClipper* /*pPrim*/, const PSX_RECT* /*pClipRect*/)
{
    NOT_IMPLEMENTED();
}

void CC OrderingTable_Add_498A80(int** /*ppOt*/, PrimHeader* /*pItem*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO
