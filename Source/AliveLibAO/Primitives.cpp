#include "stdafx_ao.h"
#include "Primitives.hpp"
#include "Function.hpp"
#include "../AliveLibAE/Primitives.hpp" // AE lib hack

namespace AO {

void CC Init_SetTPage_495FB0(Prim_SetTPage* pPrim, s32 /*notUsed1*/, s32 /*notUsed2*/, s32 tpage)
{
    SetUnknown(&pPrim->mBase);
    SetCode(&pPrim->mBase, PrimTypeCodes::eSetTPage);
    pPrim->field_C_tpage = tpage;
}

void CC Poly_Set_Blending_498A00(PrimHeader* pPrim, s32 bBlending)
{
    AE_IMPLEMENTED();
    Poly_Set_Blending_4F8A20(pPrim, bBlending);
}

s32 CC PSX_getClut_496840(s32 x, s32 y)
{
    return (y << 6) | ((x >> 4) & 63);
}

void CC Poly_Set_SemiTrans_498A40(PrimHeader* pPrim, s32 bSemiTrans)
{
    AE_IMPLEMENTED();
    Poly_Set_SemiTrans_4F8A60(pPrim, bSemiTrans);
}

void CC Sprt_Init(Prim_Sprt* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 4);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::eSprt);
}

void CC Init_PrimClipper_495FD0(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect)
{
    AE_IMPLEMENTED();
    Init_PrimClipper_4F5B80(pPrim, pClipRect);
}

void CC OrderingTable_Add_498A80(PrimHeader** ppOt, PrimHeader* pItem)
{
    AE_IMPLEMENTED();
    OrderingTable_Add_4F8AA0(ppOt, pItem);
}

void CC PolyG4_Init_498890(Poly_G4* pPoly)
{
    AE_IMPLEMENTED();
    PolyG4_Init_4F88B0(pPoly);
}

void CC Prim_Init_Poly_G3_498870(Poly_G3* pPrim)
{
    SetNumLongs(&pPrim->mBase.header, 6);
    SetUnknown(&pPrim->mBase.header);
    SetCode(&pPrim->mBase.header, PrimTypeCodes::ePolyG3);
}

void CC InitType_ScreenOffset_496000(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset)
{
    SetUnknown(&pPrim->mBase);
    SetCode(&pPrim->mBase, PrimTypeCodes::eScreenOffset);
    pPrim->field_C_xoff = pOffset->x;
    pPrim->field_E_yoff = pOffset->y;
}

} // namespace AO
