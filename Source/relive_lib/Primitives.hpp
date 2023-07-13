#pragma once

#include "Layer.hpp"
#include "Psx.hpp"
#include "Function.hpp"
#include "RGB16.hpp"

struct Poly_FT4;

void Poly_FT4_Get_Rect(PSX_RECT* pRect, const Poly_FT4* pPoly);

namespace relive {

enum class TBlendModes : u32
{
    eBlend_0,
    eBlend_1,
    eBlend_2,
    eBlend_3,
    None,
};
} // namespace relive


enum class PrimitivesTypes : u8
{
    eLaughingGas,
    eScreenOffset,

    ePolyG3,
    ePolyG4,

    eLineG2,
    eLineG4,

    ePolyFT4,
    eScissorRect,
};

struct Prim_RGB final
{
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
};

struct UV final
{
    u8 u = 0;
    u8 v = 0;
};

struct Vert final
{
    s16 x = 0;
    s16 y = 0;
};

// TODO: This still needs a big refactor
struct BasePrimitive
{
    explicit BasePrimitive(PrimitivesTypes type)
        : mType(type)
    {
    }

    void SetBlendMode(relive::TBlendModes blendMode)
    {
        mBlendMode = blendMode;
    }

    void SetSemiTransparent(bool bSemiTrans)
    {
        mSemiTransparent = bSemiTrans;
    }

    void DisableBlending(bool disableBlending)
    {
        // TODO: HACK check whatever uses this, seems wrong
        mDisableBlending = !disableBlending;
    }

    void SetRGB0(u8 r, u8 g, u8 b)
    {
        SetRGB(0, r, g, b);
    }

    void SetXY0(s16 x, s16 y)
    {
        SetXY(0, x, y);
    }

    void SetUV0(u8 u, u8 v)
    {
        SetUV(0, u, v);
    }

    void SetRGB1(u8 r, u8 g, u8 b)
    {
        SetRGB(1, r, g, b);
    }

    void SetXY1(s16 x, s16 y)
    {
        SetXY(1, x, y);
    }

    void SetUV1(u8 u, u8 v)
    {
        SetUV(1, u, v);
    }

    void SetRGB2(u8 r, u8 g, u8 b)
    {
        SetRGB(2, r, g, b);
    }

    void SetXY2(s16 x, s16 y)
    {
        SetXY(2, x, y);
    }

    void SetUV2(u8 u, u8 v)
    {
        SetUV(2, u, v);
    }

    void SetRGB3(u8 r, u8 g, u8 b)
    {
        SetRGB(3, r, g, b);
    }

    void SetXY3(s16 x, s16 y)
    {
        SetXY(3, x, y);
    }

    void SetUV3(u8 u, u8 v)
    {
        SetUV(3, u, v);
    }

    void SetXYWH(s16 x, s16 y, s16 w, s16 h)
    {
        SetXY0(x, y);
        SetXY1(x + w, y);
        SetXY2(x, y + h);
        SetXY3(x + w, y + h);
    }

    s16 X0() const
    {
        return mVerts[0].x;
    }

    s16 X1() const
    {
        return mVerts[1].x;
    }

    s16 X2() const
    {
        return mVerts[2].x;
    }

    s16 X3() const
    {
        return mVerts[3].x;
    }

    s16 Y0() const
    {
        return mVerts[0].y;
    }

    s16 Y1() const
    {
        return mVerts[1].y;
    }

    s16 Y2() const
    {
        return mVerts[2].y;
    }

    s16 Y3() const
    {
        return mVerts[3].y;
    }

    u8 R0() const
    {
        return mRgbs[0].r;
    }

    u8 G0() const
    {
        return mRgbs[0].g;
    }

    u8 B0() const
    {
        return mRgbs[0].b;
    }

    u8 R1() const
    {
        return mRgbs[1].r;
    }

    u8 G1() const
    {
        return mRgbs[1].g;
    }

    u8 B1() const
    {
        return mRgbs[1].b;
    }

    u8 R2() const
    {
        return mRgbs[2].r;
    }

    u8 G2() const
    {
        return mRgbs[2].g;
    }

    u8 B2() const
    {
        return mRgbs[2].b;
    }

    u8 R3() const
    {
        return mRgbs[3].r;
    }

    u8 G3() const
    {
        return mRgbs[3].g;
    }

    u8 B3() const
    {
        return mRgbs[3].b;
    }

    u8 U0() const
    {
        return mUvs[0].u;
    }

    u8 V0() const
    {
        return mUvs[0].v;
    }

    u8 U1() const
    {
        return mUvs[1].u;
    }

    u8 V1() const
    {
        return mUvs[1].v;
    }

    u8 U2() const
    {
        return mUvs[2].u;
    }

    u8 V2() const
    {
        return mUvs[2].v;
    }

    u8 U3() const
    {
        return mUvs[3].u;
    }

    u8 V3() const
    {
        return mUvs[3].v;
    }

    PrimitivesTypes mType = {};
    relive::TBlendModes mBlendMode = {};

    bool mSemiTransparent = false;
    bool mDisableBlending = false;

    Prim_RGB mRgbs[4];
    UV mUvs[4];
    Vert mVerts[4];

    BasePrimitive* mNext = nullptr;

private:
    void SetRGB(u32 idx, u8 r, u8 g, u8 b)
    {
        mRgbs[idx].r = r;
        mRgbs[idx].g = g;
        mRgbs[idx].b = b;
    }

    void SetXY(u32 idx, s16 x, s16 y)
    {
        mVerts[idx].x = x;
        mVerts[idx].y = y;
    }

    void SetUV(u32 idx, u8 u, u8 v)
    {
        mUvs[idx].u = u;
        mUvs[idx].v = v;
    }
};

struct Prim_GasEffect final : public BasePrimitive
{
    Prim_GasEffect()
        : BasePrimitive(PrimitivesTypes::eLaughingGas)
    {
    }

    s32 x = 0;
    s32 y = 0;
    s32 w = 0;
    s32 h = 0;
    u16* pData = nullptr;
};

struct Prim_ScreenOffset final : public BasePrimitive
{
    Prim_ScreenOffset()
        : BasePrimitive(PrimitivesTypes::eScreenOffset)
    {
    }

    void SetOffset(s16 x, s16 y)
    {
        field_C_xoff = x;
        field_E_yoff = y;
    }

    s16 field_C_xoff = 0;
    s16 field_E_yoff = 0;
};

struct Poly_G3 final : public BasePrimitive
{
    Poly_G3()
        : BasePrimitive(PrimitivesTypes::ePolyG3)
    {
    }
};

struct Poly_G4 final : public BasePrimitive
{
    Poly_G4()
        : BasePrimitive(PrimitivesTypes::ePolyG4)
    {
    }
};

struct Line_G2 final : public BasePrimitive
{
    Line_G2()
        : BasePrimitive(PrimitivesTypes::eLineG2)
    {
    }
};

struct Line_G4 final : public BasePrimitive
{
    Line_G4()
        : BasePrimitive(PrimitivesTypes::eLineG4)
    {
    }
};

struct Poly_FT4 final : public BasePrimitive
{
    Poly_FT4()
        : BasePrimitive(PrimitivesTypes::ePolyFT4)
    {
    }

    // TODO: Remove these
    class Animation* mAnim= nullptr;
    bool mFlipX = false;
    bool mFlipY = false;
    class CamResource* mCam = nullptr;
    class FontContext* mFont = nullptr;
    class Fg1Layer* mFg1 = nullptr;

    f32 uBase = 0.0f;
    f32 vBase = 0.0f;
};

struct Prim_ScissorRect final : public BasePrimitive
{
    Prim_ScissorRect()
        : BasePrimitive(PrimitivesTypes::eScissorRect)
    {
    }

    void SetRect(const PSX_RECT& r)
    {
        mRect = r;
    }

    PSX_RECT mRect = {};
};
