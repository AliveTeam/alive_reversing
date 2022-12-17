#pragma once

#include "Layer.hpp"
#include "Psx.hpp"
#include "Function.hpp"
#include "RGB16.hpp"

struct Poly_G3;
struct Poly_G4;
struct Prim_SetTPage;
struct Prim_PrimClipper;
struct Prim_ScreenOffset;
struct PSX_RECT;
struct PSX_Pos16;
struct PrimHeader;
struct Poly_FT4;

void PolyG3_Init(Poly_G3* pPoly);
void PolyG4_Init(Poly_G4* pPoly);


void Init_PrimClipper(Prim_PrimClipper* pPrim, const PSX_RECT* pClipRect);
void InitType_ScreenOffset(Prim_ScreenOffset* pPrim, const PSX_Pos16* pOffset);

void Poly_FT4_Get_Rect(PSX_RECT* pRect, const Poly_FT4* pPoly);
void Poly_Set_Blending(PrimHeader* pPrim, s32 bBlending);
void Poly_Set_SemiTrans(PrimHeader* pPrim, s32 bSemiTrans);
PrimHeader** OtLayer(PrimHeader** ppOt, Layer layer);
void OrderingTable_Add(PrimHeader** ppOt, PrimHeader* pItem);

enum class TPageAbr : s8
{
    eBlend_0 = 0,
    eBlend_1 = 1,
    eBlend_2 = 2,
    eBlend_3 = 3,
};

void Init_SetTPage(Prim_SetTPage* pPrim, s32 tpage);
s32 PSX_getTPage(TPageAbr abr);

struct PrimHeaderPart_Normal final
{
    s8 field_4_num_longs;
    s8 field_5_unknown;
    s16 field_6_pad0;
};
ALIVE_ASSERT_SIZEOF(PrimHeaderPart_Normal, 0x4);

struct PrimHeaderPart_PsxRect final
{
    s16 w;
    s16 h;
};
ALIVE_ASSERT_SIZEOF(PrimHeaderPart_PsxRect, 0x4);

union PrimHeaderPart final
{
    PrimHeaderPart_Normal mNormal;
    PrimHeaderPart_PsxRect mRect;
};
ALIVE_ASSERT_SIZEOF(PrimHeaderPart, 4);

struct Prim_RGB final
{
    u8 r;
    u8 g;
    u8 b;
    u8 code_or_pad;
};
ALIVE_ASSERT_SIZEOF(Prim_RGB, 0x4);

struct PrimHeader
{
    struct PrimHeader* tag;
#if !_WIN32 || _WIN64
    const void* hackPtr;
#endif
    PrimHeaderPart header;
    Prim_RGB rgb_code;
};
ALIVE_ASSERT_SIZEOF(PrimHeader, 0xC);

struct FVert final
{
    s16 x;
    s16 y;
};
ALIVE_ASSERT_SIZEOF(FVert, 0x4);

struct GVert final
{
    Prim_RGB mRgb;
    FVert mVert;
};
ALIVE_ASSERT_SIZEOF(GVert, 0x8);

struct Poly_Base
{
    PrimHeader header;
    FVert vert;
};
ALIVE_ASSERT_SIZEOF(Poly_Base, 0x10);

struct UV final
{
    u8 u;
    u8 v;
    u16 tpage_clut_pad;
};
ALIVE_ASSERT_SIZEOF(UV, 0x4);

struct TVert final
{
    FVert mVert;
    UV mUv;
};
ALIVE_ASSERT_SIZEOF(TVert, 0x8);

struct TGVert final
{
    Prim_RGB mRgb;
    FVert mVert;
    UV mUv;
};
// TODO: Assert size

struct Poly_G3 final
{
    Poly_Base mBase;
    GVert mVerts[2];
};
ALIVE_ASSERT_SIZEOF(Poly_G3, 0x20);

struct FVertWrapper final
{
    FVert mVert;
};

struct Poly_FT4 final
{
    Poly_Base mBase;
    UV mUv;
    TVert mVerts[3];
    class Animation* mAnim;
    bool mFlipX;
    bool mFlipY;
    class CamResource* mCam;
    class FontContext* mFont;
    class Fg1Layer* mFg1;

};
// ALIVE_ASSERT_SIZEOF(Poly_FT4, 0x2C);

struct Poly_G4 final
{
    Poly_Base mBase;
    GVert mVerts[3];
};
ALIVE_ASSERT_SIZEOF(Poly_G4, 0x28);

// TODO: FIX ME - in hacked window mode screen offset doesn't actually work. Notice how explosion/screen shakes do nothing.
struct Prim_ScreenOffset final
{
    PrimHeader mBase;
    s16 field_C_xoff;
    s16 field_E_yoff;
};
ALIVE_ASSERT_SIZEOF(Prim_ScreenOffset, 0x10);

struct Prim_PrimClipper final
{
    PrimHeader mBase;
    s16 field_C_x;
    s16 field_E_y;
};
ALIVE_ASSERT_SIZEOF(Prim_PrimClipper, 0x10);

struct Prim_SetTPage final
{
    PrimHeader mBase;
    s32 field_C_tpage;
};
ALIVE_ASSERT_SIZEOF(Prim_SetTPage, 0x10);


struct Line_G2 final
{
    Poly_Base mBase;
    GVert mVerts[1];
};
// TODO: Assert size

struct Line_G4 final
{
    Poly_Base mBase;
    GVert mVerts[3];
    u32 field_28_pad;
};
ALIVE_ASSERT_SIZEOF(Line_G4, 0x2C);

enum PrimTypeCodes
{
    eSetTPage = 0x80,
    ePrimClipper = 0x81,
    eScreenOffset = 0x82,
    eLaughingGas = 0x84,

    //                         G  3  T
    ePolyG3 = 0x30,  // 0b1[1][0][0]00

    //                         F  4  T
    ePolyFT4 = 0x2C, // 0b1[0][1][1]00

    //                         G  4  T
    ePolyG4 = 0x38,  // 0b1[1][1][0]00

    // Line prims
    eLineG2 = 0x50,
    eLineG4 = 0x5C
};

// Could be used for other stuff but only seen for gas so far
struct Prim_GasEffect final
{
    PrimHeader mPrimHeader;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u16* pData;
};

union PrimAny
{
    void* mVoid;
    PrimHeader* mPrimHeader;

    Prim_SetTPage* mSetTPage;
    Prim_PrimClipper* mPrimClipper;
    Prim_ScreenOffset* mScreenOffset;

    Poly_G3* mPolyG3;

    Poly_FT4* mPolyFT4;
    Poly_G4* mPolyG4;

    Line_G2* mLineG2;
    Line_G4* mLineG4;

    Prim_GasEffect* mGas;
};
ALIVE_ASSERT_SIZEOF(PrimAny, sizeof(void*));

template <class T>
inline void SetRGB0(T* prim, const RGB16& rgb)
{
    prim->mBase.header.rgb_code.r = static_cast<u8>(rgb.r);
    prim->mBase.header.rgb_code.g = static_cast<u8>(rgb.g);
    prim->mBase.header.rgb_code.b = static_cast<u8>(rgb.b);
}

template <class T>
inline void SetRGB0(T* prim, u32 r, u32 g, u32 b)
{
    prim->mBase.header.rgb_code.r = static_cast<u8>(r);
    prim->mBase.header.rgb_code.g = static_cast<u8>(g);
    prim->mBase.header.rgb_code.b = static_cast<u8>(b);
}

template <class T>
inline u8 R0(T* prim)
{
    return prim->mBase.header.rgb_code.r;
}
template <class T>
inline u8 G0(T* prim)
{
    return prim->mBase.header.rgb_code.g;
}
template <class T>
inline u8 B0(T* prim)
{
    return prim->mBase.header.rgb_code.b;
}

template <class T>
inline u8 R_Generic(T* prim, s32 idx)
{
    return prim->mVerts[idx].mRgb.r;
}
template <class T>
inline u8 G_Generic(T* prim, s32 idx)
{
    return prim->mVerts[idx].mRgb.g;
}
template <class T>
inline u8 B_Generic(T* prim, s32 idx)
{
    return prim->mVerts[idx].mRgb.b;
}
template <class T>
inline u8 R1(T* prim)
{
    return R_Generic(prim, 0);
}
template <class T>
inline u8 G1(T* prim)
{
    return G_Generic(prim, 0);
}
template <class T>
inline u8 B1(T* prim)
{
    return B_Generic(prim, 0);
}
template <class T>
inline u8 R2(T* prim)
{
    return R_Generic(prim, 1);
}
template <class T>
inline u8 G2(T* prim)
{
    return G_Generic(prim, 1);
}
template <class T>
inline u8 B2(T* prim)
{
    return B_Generic(prim, 1);
}
template <class T>
inline u8 R3(T* prim)
{
    return R_Generic(prim, 2);
}
template <class T>
inline u8 G3(T* prim)
{
    return G_Generic(prim, 2);
}
template <class T>
inline u8 B3(T* prim)
{
    return B_Generic(prim, 2);
}

template <class T>
inline s16 X0(T* prim)
{
    return prim->mBase.vert.x;
}
template <class T>
inline s16 Y0(T* prim)
{
    return prim->mBase.vert.y;
}

template <class T>
inline s16 X_Generic(T* prim, s32 idx)
{
    return prim->mVerts[idx].mVert.x;
}
template <class T>
inline s16 Y_Generic(T* prim, s32 idx)
{
    return prim->mVerts[idx].mVert.y;
}
template <class T>
inline s16 X1(T* prim)
{
    return X_Generic(prim, 0);
}
template <class T>
inline s16 Y1(T* prim)
{
    return Y_Generic(prim, 0);
}
template <class T>
inline s16 X2(T* prim)
{
    return X_Generic(prim, 1);
}
template <class T>
inline s16 Y2(T* prim)
{
    return Y_Generic(prim, 1);
}
template <class T>
inline s16 X3(T* prim)
{
    return X_Generic(prim, 2);
}
template <class T>
inline s16 Y3(T* prim)
{
    return Y_Generic(prim, 2);
}

template <class T>
inline u8 U0(T* prim)
{
    return prim->mUv.u;
}
template <class T>
inline u8 V0(T* prim)
{
    return prim->mUv.v;
}
template <class T>
inline u8 U_Generic(T* prim, s32 idx)
{
    return prim->mVerts[idx].mUv.u;
}
template <class T>
inline u8 V_Generic(T* prim, s32 idx)
{
    return prim->mVerts[idx].mUv.v;
}
template <class T>
inline u8 U1(T* prim)
{
    return U_Generic(prim, 0);
}
template <class T>
inline u8 V1(T* prim)
{
    return V_Generic(prim, 0);
}
template <class T>
inline u8 U2(T* prim)
{
    return U_Generic(prim, 1);
}
template <class T>
inline u8 V2(T* prim)
{
    return V_Generic(prim, 1);
}
template <class T>
inline u8 U3(T* prim)
{
    return U_Generic(prim, 2);
}
template <class T>
inline u8 V3(T* prim)
{
    return V_Generic(prim, 2);
}

template <class T>
inline void SetXY_Generic(T* prim, s32 idx, s16 x, s16 y)
{
    prim->mVerts[idx].mVert.x = x;
    prim->mVerts[idx].mVert.y = y;
}

template <class T>
inline void SetUV_Generic(T* prim, s32 idx, u8 u, u8 v)
{
    prim->mVerts[idx].mUv.u = u;
    prim->mVerts[idx].mUv.v = v;
}

template <class T>
inline void SetRGB_Generic(T* prim, s32 idx, u8 r, u8 g, u8 b)
{
    prim->mVerts[idx].mRgb.r = r;
    prim->mVerts[idx].mRgb.g = g;
    prim->mVerts[idx].mRgb.b = b;
}

template <class T>
inline void SetRGB1(T* prim, u8 r, u8 g, u8 b)
{
    SetRGB_Generic(prim, 0, r, g, b);
}

template <class T>
inline void SetRGB2(T* prim, u8 r, u8 g, u8 b)
{
    SetRGB_Generic(prim, 1, r, g, b);
}

template <class T>
inline void SetRGB3(T* prim, u8 r, u8 g, u8 b)
{
    SetRGB_Generic(prim, 2, r, g, b);
}

template <class T>
inline void SetXY0(T* prim, s16 x, s16 y)
{
    prim->mBase.vert.x = x;
    prim->mBase.vert.y = y;
}

template <class T>
inline void SetUV0(T* prim, u8 u, u8 v)
{
    prim->mUv.u = u;
    prim->mUv.v = v;
}

template <class T>
inline void SetUV1(T* prim, u8 u, u8 v)
{
    SetUV_Generic(prim, 0, u, v);
}
template <class T>
inline void SetUV2(T* prim, u8 u, u8 v)
{
    SetUV_Generic(prim, 1, u, v);
}

template <class T>
inline void SetUV3(T* prim, u8 u, u8 v)
{
    SetUV_Generic(prim, 2, u, v);
}

template <class T>
inline void SetXY1(T* prim, s16 x, s16 y)
{
    SetXY_Generic(prim, 0, x, y);
}

template <class T>
inline void SetXY2(T* prim, s16 x, s16 y)
{
    SetXY_Generic(prim, 1, x, y);
}

template <class T>
inline void SetXY3(T* prim, s16 x, s16 y)
{
    SetXY_Generic(prim, 2, x, y);
}

template <class T>
inline void SetTPage(T* prim, u16 tpage)
{
    prim->mVerts[0].mUv.tpage_clut_pad = tpage;
}

template <class T>
inline u16 GetTPage(T* prim)
{
    return prim->mVerts[0].mUv.tpage_clut_pad;
}

template <class T>
inline void SetClut(T* prim, s16 clut)
{
    prim->mUv.tpage_clut_pad = clut;
}

template <class T>
inline s16 GetClut(T* prim)
{
    return prim->mUv.tpage_clut_pad;
}

template <class T>
inline void SetXYWH(T pPoly, s16 x, s16 y, s16 w, s16 h)
{
    SetXY0(pPoly, x, y);
    SetXY1(pPoly, x + w, y);
    SetXY2(pPoly, x, y + h);
    SetXY3(pPoly, x + w, y + h);
}

template <class T>
inline bool GetPolyIsSemiTrans(T* prim)
{
    return (prim->mBase.header.rgb_code.code_or_pad & 2) > 0;
}

template <class T>
inline bool GetPolyIsShaded(T* prim)
{
    return (prim->mBase.header.rgb_code.code_or_pad & 1) == 0;
}

void SetCode(PrimHeader* pPrim, u8 code);
void SetUnknown(PrimHeader* pPrim);
void SetNumLongs(PrimHeader* pPrim, s8 numLongs);

void LineG2_Init(Line_G2* pLine);
void LineG4_Init(Line_G4* pLine);

void PolyFT4_Init(Poly_FT4* pPrim);

s32 PSX_Prim_Code_Without_Blending_Or_SemiTransparency(s32 code);


void SetPrimExtraPointerHack(Poly_FT4* pPoly, const void* ptr);
const void* GetPrimExtraPointerHack(Poly_FT4* pPoly);
