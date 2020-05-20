#include "stdafx_ao.h"
#include "Primitives.hpp"


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
