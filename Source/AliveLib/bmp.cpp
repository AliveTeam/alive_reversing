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
        hr = pBmp->field_0_pSurface->Blt(&rect, 0, &rect, 0x1000400, &blt_fx_stru_BBC458);
        if (SUCCEEDED(hr))
        {
            if (hr != DDERR_SURFACELOST)
            {
                continue;
            }

            hr = pBmp->field_0_pSurface->Restore();
            
            if (SUCCEEDED(hr))
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
