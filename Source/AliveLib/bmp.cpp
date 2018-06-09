#include "stdafx.h"
#include "bmp.hpp"
#include "Function.hpp"
#include "DDraw.hpp"
#include "Error.hpp"

#define BMP_IMPL true

ALIVE_VAR(1, 0xBBC3D4, LPDIRECTDRAW, sDDraw_BBC3D4, nullptr);

HRESULT CC BMP_New_create_surface_4F1C60(DDSURFACEDESC* pSurfaceDesc, LPDIRECTDRAWSURFACE* ppSurface)
{
    *ppSurface = nullptr;

    HRESULT hr = sDDraw_BBC3D4->CreateSurface(pSurfaceDesc, ppSurface, nullptr);

    if (hr == DDERR_INVALIDPARAMS)
    {
        const DWORD oldCaps = pSurfaceDesc->ddsCaps.dwCaps;
        if (!(oldCaps & 0x800)) // TODO: Flag constant
        {
            DWORD tempCaps = pSurfaceDesc->ddsCaps.dwCaps;
            // TODO
            //BYTE1(tempCaps) = BYTE1(tempCaps) & 0xBF | 8;
            pSurfaceDesc->ddsCaps.dwCaps = tempCaps;
            hr = sDDraw_BBC3D4->CreateSurface(pSurfaceDesc, ppSurface, nullptr);
            pSurfaceDesc->ddsCaps.dwCaps = oldCaps;
        }
    }
    return hr;
}
ALIVE_FUNC_IMPLEX(0x4F1C60, BMP_New_create_surface_4F1C60, false); // TODO

signed int CC BMP_Blt_4F1E50(Bitmap* pDstBmp, int xPos, int yPos, Bitmap* pSrcBmp, LPRECT pRect, int flags)
{
    HRESULT hr = S_OK;
    bool addError = true;
    for (;;)
    {
        hr = pDstBmp->field_0_pSurface->BltFast(
            xPos,
            yPos,
            pSrcBmp->field_0_pSurface,
            pRect,
            flags);

        if (SUCCEEDED(hr))
        {
            return 0;
        }

        if (hr == DDERR_SURFACELOST)
        {
            if (FAILED(pSrcBmp->field_0_pSurface->Restore()) && FAILED(pDstBmp->field_0_pSurface->Restore()))
            {
                addError = true;
            }
            else
            {
                addError = false;
            }
            break;
        }

        if (hr != DDERR_WASSTILLDRAWING && hr != DDERR_WRONGMODE)
        {
            addError = true;
            break;
        }
    }

    if (addError)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 307, -1, DX_HR_To_String_4F4EC0(hr));
    }
    return -1;
}
ALIVE_FUNC_IMPLEX(0x4F1E50, BMP_Blt_4F1E50, BMP_IMPL);

ALIVE_VAR(1, 0xBBC458, DDBLTFX, blt_fx_stru_BBC458, {});

signed int CC BMP_ClearRect_4F1EE0(Bitmap* pBmp, const RECT* pRect, DWORD fillColour)
{
    RECT rect = *pRect;

    if (rect.left < 0)
    {
        rect.left = 0;
    }

    if (rect.left > pBmp->field_8_width)
    {
        rect.left = pBmp->field_8_width;
    }

    if (rect.right < pRect->left)
    {
        rect.right = pRect->left;
    }

    if (rect.right > pBmp->field_C_height)
    {
        rect.right = pBmp->field_C_height;
    }

    if (rect.top < 0)
    {
        rect.top = 0;
    }

    if (rect.top > pBmp->field_C_height)
    {
        rect.top = pBmp->field_C_height;
    }

    if (rect.bottom < rect.top)
    {
        rect.bottom = rect.top;
    }

    if (rect.bottom > pBmp->field_C_height)
    {
        rect.bottom = pBmp->field_C_height;
    }

    blt_fx_stru_BBC458.dwSize = sizeof(DDBLTFX);
    blt_fx_stru_BBC458.dwFillColor = fillColour;

    HRESULT hr = S_OK;
    do
    {
        hr = pBmp->field_0_pSurface->Blt(&rect, 0, &rect, DDBLT_COLORFILL | DDBLT_WAIT, &blt_fx_stru_BBC458);
        if (FAILED(hr))
        {
            if (hr != DDERR_SURFACELOST)
            {
                continue;
            }

            hr = pBmp->field_0_pSurface->Restore();
            
            if (FAILED(hr))
            {
                continue;
            }
        }
        return 0;
    } while (hr == DDERR_WASSTILLDRAWING || hr == DDERR_WRONGMODE);

    Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 340, -1, DX_HR_To_String_4F4EC0(hr));
    return -1;
}
ALIVE_FUNC_IMPLEX(0x4F1EE0, BMP_ClearRect_4F1EE0, BMP_IMPL);

void CC BMP_unlock_4F2100(Bitmap* pBmp)
{
    if (pBmp->field_4_pLockedPixels && pBmp->field_0_pSurface)
    {
        const HRESULT hr = pBmp->field_0_pSurface->Unlock(pBmp->field_4_pLockedPixels);
        if (FAILED(hr))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 393, -1, DX_HR_To_String_4F4EC0(hr));
        }
        pBmp->field_4_pLockedPixels = nullptr;
    }
}
ALIVE_FUNC_IMPLEX(0x4F2100, BMP_unlock_4F2100, BMP_IMPL);

void CC BMP_Release_DC_4F21A0(Bitmap* pBmp, HDC hdc)
{
    if (pBmp && hdc)
    {
        const HRESULT hr = pBmp->field_0_pSurface->ReleaseDC(hdc);
        if (FAILED(hr))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 431, -1, DX_HR_To_String_4F4EC0(hr));
        }
    }
}
ALIVE_FUNC_IMPLEX(0x4F21A0, BMP_Release_DC_4F21A0, BMP_IMPL);

HDC CC BMP_Get_DC_4F2150(Bitmap* pBmp)
{
    if (!pBmp || pBmp->field_4_pLockedPixels || !pBmp->field_0_pSurface)
    {
        return nullptr;
    }

    HDC dc = nullptr;
    const HRESULT hr = pBmp->field_0_pSurface->GetDC(&dc);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 412, -1, DX_HR_To_String_4F4EC0(hr));
        return nullptr;
    }

    return dc;
}
ALIVE_FUNC_IMPLEX(0x4F2150, BMP_Get_DC_4F2150, BMP_IMPL);

LONG CC BMP_Get_Font_Height_4F21F0(Bitmap* pBmp)
{
    HDC dc = BMP_Get_DC_4F2150(pBmp);
    TEXTMETRICA tm = {};
    GetTextMetricsA(dc, &tm);
    BMP_Release_DC_4F21A0(pBmp, dc);

    // Invert if negative
    LONG textHeight = tm.tmHeight;
    if (tm.tmHeight < 0)
    {
        textHeight = -tm.tmHeight;
    }

    return textHeight;
}
ALIVE_FUNC_IMPLEX(0x4F21F0, BMP_Get_Font_Height_4F21F0, BMP_IMPL);

signed int CC BMP_New_convert_BPP_4F1CC0(int bpp)
{
    signed int converted = 0;
    switch (bpp)
    {
    case 1:
        converted = 1;
        break;
    case 2:
        converted = 2;
        break;
    case 4:
        converted = 4;
        break;
    case 8:
        converted = 8;
        break;
    case 15:
    case 16:
    case 115:
    case 116:
        converted = 16;
        break;
    case 24:
        converted = 24;
        break;
    case 32:
        converted = 32;
        break;
    default:
        converted = -1;
        break;
    }
    return converted;
}
ALIVE_FUNC_IMPLEX(0x4F1CC0, BMP_New_convert_BPP_4F1CC0, BMP_IMPL);

int CC Bmp_Convert_Colour_4F17D0(Bitmap* pBmp, int r, int g, int b)
{
    int converted = 0;
    switch (pBmp->field_15_pixel_format)
    {
    case 0xF:
        converted = (b >> 3) | 4 * (g & 0xF8 | 32 * (r & 0xF8));
        break;
    case 0x10:
        converted = (b >> 3) | 8 * (g & 0xFC | 32 * (r & 0xF8));
        break;
    case 0x18:
    case 0x20:
        converted = b | ((g | (r << 8)) << 8);
        break;
    case 0x73:
        converted = (r >> 3) | 4 * (g & 0xF8 | 32 * (b & 0xF8));
        break;
    case 0x74:
        converted = (r >> 3) | 8 * (g & 0xFC | 32 * (b & 0xF8));
        break;
    case 0x7C:
    case 0x84:
        converted = r | ((g | (b << 8)) << 8);
        break;
    default:
        converted = 0;
        break;
    }
    return converted;
}
ALIVE_FUNC_IMPLEX(0x4F17D0, Bmp_Convert_Colour_4F17D0, BMP_IMPL);

void CC Bmp_Free_4F1950(Bitmap* pBmp)
{
    if (pBmp && pBmp->field_0_pSurface)
    {
        if (pBmp->field_4_pLockedPixels)
        {
            BMP_unlock_4F2100(pBmp);
        }
        pBmp->field_0_pSurface->Release();
        memset(pBmp, 0, sizeof(Bitmap));
    }
}
ALIVE_FUNC_IMPLEX(0x4F1950, Bmp_Free_4F1950, BMP_IMPL);

#include "gmock/gmock.h"

class MockedDirectDraw
{
public:
    MOCK_METHOD3(CreateSurface, HRESULT(LPDDSURFACEDESC surfaceDesc, LPDIRECTDRAWSURFACE FAR * ppSurface, IUnknown FAR * pUnk));
};

class DirectDrawMock : public IDirectDraw
{
public:
    MockedDirectDraw& mMock;

    DirectDrawMock(MockedDirectDraw& mockDD)
        : mMock(mockDD)
    {

    }

    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface) (THIS_ REFIID, LPVOID FAR*)
    {
        return S_OK;
    }

    STDMETHOD_(ULONG, AddRef) (THIS)
    {
        return S_OK;
    }

    STDMETHOD_(ULONG, Release) (THIS)
    {
        return S_OK;
    }

    /*** IDirectDraw methods ***/
    STDMETHOD(Compact)(THIS)
    {
        return S_OK;
    }

    STDMETHOD(CreateSurface)(THIS_  LPDDSURFACEDESC surfaceDesc, LPDIRECTDRAWSURFACE FAR * ppSurface, IUnknown FAR * pUnk)
    {
        return mMock.CreateSurface(surfaceDesc, ppSurface, pUnk);
    }

    STDMETHOD(CreateClipper)(THIS_ DWORD, LPDIRECTDRAWCLIPPER FAR*, IUnknown FAR*)
    {
        return S_OK;
    }

    STDMETHOD(CreatePalette)(THIS_ DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR*, IUnknown FAR*)
    {
        return S_OK;
    }

    STDMETHOD(DuplicateSurface)(THIS_ LPDIRECTDRAWSURFACE, LPDIRECTDRAWSURFACE FAR*)
    {
        return S_OK;
    }

    STDMETHOD(EnumDisplayModes)(THIS_ DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMMODESCALLBACK)
    {
        return S_OK;
    }

    STDMETHOD(EnumSurfaces)(THIS_ DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMSURFACESCALLBACK)
    {
        return S_OK;
    }

    STDMETHOD(FlipToGDISurface)(THIS)
    {
        return S_OK;
    }

    STDMETHOD(GetCaps)(THIS_ LPDDCAPS, LPDDCAPS)
    {
        return S_OK;
    }

    STDMETHOD(GetDisplayMode)(THIS_ LPDDSURFACEDESC)
    {
        return S_OK;
    }

    STDMETHOD(GetFourCCCodes)(THIS_  LPDWORD, LPDWORD)
    {
        return S_OK;
    }

    STDMETHOD(GetGDISurface)(THIS_ LPDIRECTDRAWSURFACE FAR* )
    {
        return S_OK;
    }

    STDMETHOD(GetMonitorFrequency)(THIS_ LPDWORD)
    {
        return S_OK;
    }

    STDMETHOD(GetScanLine)(THIS_ LPDWORD)
    {
        return S_OK;
    }

    STDMETHOD(GetVerticalBlankStatus)(THIS_ LPBOOL)
    {
        return S_OK;
    }

    STDMETHOD(Initialize)(THIS_ GUID FAR* )
    {
        return S_OK;
    }

    STDMETHOD(RestoreDisplayMode)(THIS)
    {
        return S_OK;
    }

    STDMETHOD(SetCooperativeLevel)(THIS_ HWND, DWORD)
    {
        return S_OK;
    }

    STDMETHOD(SetDisplayMode)(THIS_ DWORD, DWORD, DWORD)
    {
        return S_OK;
    }

    STDMETHOD(WaitForVerticalBlank)(THIS_ DWORD, HANDLE)
    {
        return S_OK;
    }

};

using namespace ::testing;

void Test_BMP_New_create_surface_4F1C60()
{
    StrictMock<MockedDirectDraw> mocked;
    DirectDrawMock mock(mocked);

    sDDraw_BBC3D4 = &mock;

    DDSURFACEDESC desc = {};
    LPDIRECTDRAWSURFACE surf = nullptr;

    EXPECT_CALL(mocked, CreateSurface(&desc, _, nullptr))
        .WillOnce(Return(DDERR_INVALIDPARAMS))
        .WillOnce(Return(S_OK));

   ASSERT_EQ(S_OK, BMP_New_create_surface_4F1C60(&desc, &surf));

   sDDraw_BBC3D4 = nullptr;
}

void BmpTests()
{
    Test_BMP_New_create_surface_4F1C60();
}
