#pragma once

#include "Psx_common.hpp"
#include "FunctionFwd.hpp"


struct PrimHeaderPart_Normal
{
    char field_4_num_longs;
    char field_5_unknown;
    __int16 field_6_pad0;
};
ALIVE_ASSERT_SIZEOF(PrimHeaderPart_Normal, 0x4);

struct PrimHeaderPart_PsxRect
{
    short w;
    short h;
};
ALIVE_ASSERT_SIZEOF(PrimHeaderPart_PsxRect, 0x4);

union PrimHeaderPart
{
    PrimHeaderPart_Normal mNormal;
    PrimHeaderPart_PsxRect mRect;
};
ALIVE_ASSERT_SIZEOF(PrimHeaderPart, 4);

struct Prim_RGB
{
    BYTE r;
    BYTE g;
    BYTE b;
    BYTE code_or_pad;
};
ALIVE_ASSERT_SIZEOF(Prim_RGB, 0x4);

struct PrimHeader
{
    int* tag;
#if !_WIN32 || _WIN64
    const void* hackPtr;
#endif
    PrimHeaderPart header;
    Prim_RGB rgb_code;
};
ALIVE_ASSERT_SIZEOF(PrimHeader, 0xC);

struct FVert
{
    __int16 x;
    __int16 y;
};
ALIVE_ASSERT_SIZEOF(FVert, 0x4);

struct GVert
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

struct UV
{
    BYTE u;
    BYTE v;
    WORD tpage_clut_pad;
};
ALIVE_ASSERT_SIZEOF(UV, 0x4);

struct TVert
{
    FVert mVert;
    UV mUv;
};
ALIVE_ASSERT_SIZEOF(TVert, 0x8);

struct TGVert
{
    Prim_RGB mRgb;
    FVert mVert;
    UV mUv;
};
// TODO: Assert size

struct Poly_G3
{
    Poly_Base mBase;
    GVert mVerts[2];
};
ALIVE_ASSERT_SIZEOF(Poly_G3, 0x20);

struct FVertWrapper
{
    FVert mVert;
};

struct Poly_F3
{
    Poly_Base mBase;
    FVertWrapper mVerts[2];
};
ALIVE_ASSERT_SIZEOF(Poly_F3, 0x18);


struct Poly_F4
{
    Poly_Base mBase;
    FVertWrapper mVerts[3];
};
ALIVE_ASSERT_SIZEOF(Poly_F4, 0x1C);

struct Poly_FT3
{
    Poly_Base mBase;
    UV mUv;
    TVert mVerts[2];
};
ALIVE_ASSERT_SIZEOF(Poly_FT3, 0x24);

struct Poly_FT4
{
    Poly_Base mBase;
    UV mUv;
    TVert mVerts[3];
};
ALIVE_ASSERT_SIZEOF(Poly_FT4, 0x2C);

struct Poly_G4
{
    Poly_Base mBase;
    GVert mVerts[3];
};
ALIVE_ASSERT_SIZEOF(Poly_G4, 0x28);

struct Poly_GT4
{
    Poly_Base mBase;
    UV mUv;
    TGVert mVerts[3];
};
// TODO: Assert size

struct Poly_GT3
{
    Poly_Base mBase;
    UV mUv;
    TGVert mVerts[2];
};
// TODO: Assert size

// TODO: FIX ME - in hacked window mode screen offset doesn't actually work. Notice how explosion/screen shakes do nothing.
struct Prim_ScreenOffset
{
    PrimHeader mBase;
    short field_C_xoff;
    short field_E_yoff;
};
ALIVE_ASSERT_SIZEOF(Prim_ScreenOffset, 0x10);

struct Prim_PrimClipper
{
    PrimHeader mBase;
    short field_C_x;
    short field_E_y;
};
ALIVE_ASSERT_SIZEOF(Prim_PrimClipper, 0x10);

struct Prim_SetTPage
{
    PrimHeader mBase;
    int field_C_tpage;
};
ALIVE_ASSERT_SIZEOF(Prim_SetTPage, 0x10);

struct Line_F2
{
    Poly_Base mBase;
    FVertWrapper mVerts[1];
};
// TODO: Assert size

struct Line_F3
{
    Poly_Base mBase;
    FVertWrapper mVerts[2];
    DWORD pad;
};
// TODO: Assert size

struct Line_F4
{
    Poly_Base mBase;
    FVertWrapper mVerts[3];
    DWORD pad;
};
// TODO: Assert size

struct Line_G2
{
    Poly_Base mBase;
    GVert mVerts[1];
};
// TODO: Assert size

struct Line_G3
{
    Poly_Base mBase;
    GVert mVerts[2];
};
// TODO: Assert size

struct Line_G4
{
    Poly_Base mBase;
    GVert mVerts[3];
    DWORD field_28_pad;
};
ALIVE_ASSERT_SIZEOF(Line_G4, 0x2C);

struct Prim_Sprt
{
    Poly_Base mBase;
    UV mUv;
    __int16 field_14_w;
    __int16 field_16_h;
};
ALIVE_ASSERT_SIZEOF(Prim_Sprt, 0x18);

struct Prim_Sprt_16
{
    Poly_Base mBase;
    UV mUv;
};

struct Prim_Sprt_8
{
    Poly_Base mBase;
    UV mUv;
};

struct Prim_Tile
{
    Poly_Base mBase;
    __int16 field_14_w;
    __int16 field_16_h;
};

struct Prim_Tile_16
{
    Poly_Base mBase;
};

struct Prim_Tile_8
{
    Poly_Base mBase;
};

struct Prim_Tile_1
{
    Poly_Base mBase;
};

enum PrimTypeCodes
{
    eSetTPage = 0x80,
    ePrimClipper = 0x81,
    eScreenOffset = 0x82,
    eGas = 0x84,

    // TODO: Type 2
    // TODO: Type 0x83 (move image?)

    // Sprite/tile prims
    eTile = 0x60,
    eSprt = 0x64,
    eTile1 = 0x68,
    eTile8 = 0x70,
    eSprt8 = 0x74,
    eTile16 = 0x78,
    eSprt16 = 0x7C,

    //                         F  3  T
    ePolyF3 = 0x20,     // 0b1[0][0][0]00
    ePolyFT3 = 0x24,    // 0b1[0][0][1]00

    //                         G  3  T
    ePolyG3 = 0x30,     // 0b1[1][0][0]00
    ePolyGT3 = 0x34,    // 0b1[1][0][1]00

    //                         F  4  T
    ePolyF4 = 0x28,     // 0b1[0][1][0]00
    ePolyFT4 = 0x2C,    // 0b1[0][1][1]00

    //                         G  4  T
    ePolyG4 = 0x38,     // 0b1[1][1][0]00
    ePolyGT4 = 0x3C,    // 0b1[1][1][1]00


    // Line prims
    eLineF2 = 0x40,
    eLineF3 = 0x48,
    eLineF4 = 0x4C,
    eLineG2 = 0x50,
    eLineG3 = 0x58,
    eLineG4 = 0x5C
};

struct Prim_MoveImage
{
    PrimHeader mPrimHeader;
    int xPos;
    int yPos;
    PSX_RECT* mpRect;
};

// Could be used for other stuff but only seen for gas so far
struct Prim_GasEffect
{
    PrimHeader mPrimHeader;
    int x;
    int y;
    int w;
    int h;
    WORD* pData;
};

union PrimAny
{
    void* mVoid;
    PrimHeader* mPrimHeader;

    Prim_SetTPage* mSetTPage;
    Prim_PrimClipper* mPrimClipper;
    Prim_ScreenOffset* mScreenOffset;
    // TODO: Type 2
    // TODO: Type 0x83 (move image?)
    // TODO: Type 0x84 (used in gas rendering)

    Prim_Sprt* mSprt;
    Prim_Sprt_16* mSprt16;
    Prim_Sprt_8* mSprt8;

    Prim_Tile* mTile;
    Prim_Tile_16* mTile16;
    Prim_Tile_8* mTile8;
    Prim_Tile_1* mTile1;

    Poly_F3* mPolyF3;
    Poly_FT3* mPolyFT3;
    Poly_G3* mPolyG3;
    Poly_GT3* mPolyGT3;

    Poly_F4* mPolyF4;
    Poly_FT4* mPolyFT4;
    Poly_G4* mPolyG4;
    Poly_GT4* mPolyGT4;

    Line_F2* mLineF2;
    Line_F3* mLineF3;
    Line_F4* mLineF4;

    Line_G2* mLineG2;
    Line_G3* mLineG3;
    Line_G4* mLineG4;

    Prim_MoveImage* mMoveImage;
    Prim_GasEffect* mGas;
};
ALIVE_ASSERT_SIZEOF(PrimAny, sizeof(void*));

template<class T>
inline void SetRGB0(T* prim, BYTE r, BYTE g, BYTE b)
{
    prim->mBase.header.rgb_code.r = r;
    prim->mBase.header.rgb_code.g = g;
    prim->mBase.header.rgb_code.b = b;
}

template<class T> inline BYTE R0(T* prim) { return prim->mBase.header.rgb_code.r; }
template<class T> inline BYTE G0(T* prim) { return prim->mBase.header.rgb_code.g; }
template<class T> inline BYTE B0(T* prim) { return prim->mBase.header.rgb_code.b; }

template<class T> inline BYTE R_Generic(T* prim, int idx) { return prim->mVerts[idx].mRgb.r; }
template<class T> inline BYTE G_Generic(T* prim, int idx) { return prim->mVerts[idx].mRgb.g; }
template<class T> inline BYTE B_Generic(T* prim, int idx) { return prim->mVerts[idx].mRgb.b; }
template<class T> inline BYTE R1(T* prim) { return R_Generic(prim, 0); }
template<class T> inline BYTE G1(T* prim) { return G_Generic(prim, 0); }
template<class T> inline BYTE B1(T* prim) { return B_Generic(prim, 0); }
template<class T> inline BYTE R2(T* prim) { return R_Generic(prim, 1); }
template<class T> inline BYTE G2(T* prim) { return G_Generic(prim, 1); }
template<class T> inline BYTE B2(T* prim) { return B_Generic(prim, 1); }
template<class T> inline BYTE R3(T* prim) { return R_Generic(prim, 2); }
template<class T> inline BYTE G3(T* prim) { return G_Generic(prim, 2); }
template<class T> inline BYTE B3(T* prim) { return B_Generic(prim, 2); }

template<class T> inline short X0(T* prim) { return prim->mBase.vert.x; }
template<class T> inline short Y0(T* prim) { return prim->mBase.vert.y; }

template<class T> inline short X_Generic(T* prim, int idx) { return prim->mVerts[idx].mVert.x; }
template<class T> inline short Y_Generic(T* prim, int idx) { return prim->mVerts[idx].mVert.y; }
template<class T> inline short X1(T* prim) { return X_Generic(prim, 0); }
template<class T> inline short Y1(T* prim) { return Y_Generic(prim, 0); }
template<class T> inline short X2(T* prim) { return X_Generic(prim, 1); }
template<class T> inline short Y2(T* prim) { return Y_Generic(prim, 1); }
template<class T> inline short X3(T* prim) { return X_Generic(prim, 2); }
template<class T> inline short Y3(T* prim) { return Y_Generic(prim, 2); }

template<class T> inline BYTE U0(T* prim) { return prim->mUv.u; }
template<class T> inline BYTE V0(T* prim) { return prim->mUv.v; }
template<class T> inline BYTE U_Generic(T* prim, int idx) { return prim->mVerts[idx].mUv.u; }
template<class T> inline BYTE V_Generic(T* prim, int idx) { return prim->mVerts[idx].mUv.v; }
template<class T> inline BYTE U1(T* prim) { return U_Generic(prim, 0); }
template<class T> inline BYTE V1(T* prim) { return V_Generic(prim, 0); }
template<class T> inline BYTE U2(T* prim) { return U_Generic(prim, 1); }
template<class T> inline BYTE V2(T* prim) { return V_Generic(prim, 1); }
template<class T> inline BYTE U3(T* prim) { return U_Generic(prim, 2); }
template<class T> inline BYTE V3(T* prim) { return V_Generic(prim, 2); }

template<class T>
inline void SetXY_Generic(T* prim, int idx, short x, short y)
{
    prim->mVerts[idx].mVert.x = x;
    prim->mVerts[idx].mVert.y = y;
}

template<class T>
inline void SetUV_Generic(T* prim, int idx, BYTE u, BYTE v)
{
    prim->mVerts[idx].mUv.u = u;
    prim->mVerts[idx].mUv.v = v;
}

template<class T>
inline void SetRGB_Generic(T* prim, int idx, BYTE r, BYTE g, BYTE b)
{
    prim->mVerts[idx].mRgb.r = r;
    prim->mVerts[idx].mRgb.g = g;
    prim->mVerts[idx].mRgb.b = b;
}

template<class T>
inline void SetRGB1(T* prim, BYTE r, BYTE g, BYTE b)
{
    SetRGB_Generic(prim, 0, r, g, b);
}

template<class T>
inline void SetRGB2(T* prim, BYTE r, BYTE g, BYTE b)
{
    SetRGB_Generic(prim, 1, r, g, b);
}

template<class T>
inline void SetRGB3(T* prim, BYTE r, BYTE g, BYTE b)
{
    SetRGB_Generic(prim, 2, r, g, b);
}

template<class T>
inline void SetXY0(T* prim, short x, short y)
{
    prim->mBase.vert.x = x;
    prim->mBase.vert.y = y;
}

template<class T>
inline void SetUV0(T* prim, BYTE u, BYTE v)
{
    prim->mUv.u = u;
    prim->mUv.v = v;
}

template<class T>
inline void SetUV1(T* prim, BYTE u, BYTE v)
{
    SetUV_Generic(prim, 0, u, v);
}
template<class T>
inline void SetUV2(T* prim, BYTE u, BYTE v)
{
    SetUV_Generic(prim, 1, u, v);
}

template<class T>
inline void SetUV3(T* prim, BYTE u, BYTE v)
{
    SetUV_Generic(prim, 2, u, v);
}

template<class T>
inline void SetXY1(T* prim, short x, short y)
{
    SetXY_Generic(prim, 0, x, y);
}

template<class T>
inline void SetXY2(T* prim, short x, short y)
{
    SetXY_Generic(prim, 1, x, y);
}

template<class T>
inline void SetXY3(T* prim, short x, short y)
{
    SetXY_Generic(prim, 2, x, y);
}

template<class T>
inline void SetTPage(T* prim, short tpage)
{
    prim->mVerts[0].mUv.tpage_clut_pad = tpage;
}

template<class T>
inline short GetTPage(T* prim)
{
    return prim->mVerts[0].mUv.tpage_clut_pad;
}

template<class T>
inline void SetClut(T* prim, short clut)
{
    prim->mUv.tpage_clut_pad = clut;
}

template<class T>
inline short GetClut(T* prim)
{
    return prim->mUv.tpage_clut_pad;
}

template<class T>
inline void SetXYWH(T pPoly, short x, short y, short w, short h)
{
    SetXY0(pPoly, x, y);
    SetXY1(pPoly, x + w, y);
    SetXY2(pPoly, x, y + h);
    SetXY3(pPoly, x + w, y + h);
}

void SetCode(PrimHeader* pPrim, BYTE code);
void SetUnknown(PrimHeader* pPrim);
void SetNumLongs(PrimHeader* pPrim, char numLongs);

void PolyF3_Init(Poly_F3* pPoly);
void PolyFT3_Init(Poly_FT3* pPoly);
void PolyGT3_Init(Poly_GT3* pPoly);

void Line_F2_Init(Line_F2* pLine);
void Line_F3_Init(Line_F3* pLine);
void Line_F4_Init(Line_F4* pLine);

void LineG2_Init(Line_G2* pLine);
void LineG3_Init(Line_G3* pLine);
void LineG4_Init(Line_G4* pLine);

void Init_Tile1(Prim_Tile_1* pTile);
void Init_Tile8(Prim_Tile_8* pTile);
void Init_Tile16(Prim_Tile_16* pTile);
void Init_Tile(Prim_Tile* pTile);

void Init_Sprt_8(Prim_Sprt_8* pPrim);
void Init_Sprt_16(Prim_Sprt_16* pPrim);

void PolyFT4_Init(Poly_FT4* pPrim);

void PolyF4_Init(Poly_F4* pPrim);