#include "stdafx.h"
#include "bmp.hpp"
#include "Function.hpp"
#include "DDraw.hpp"
#include "Error.hpp"


EXPORT HRESULT CC BMP_New_create_surface_4F1C60(DDSURFACEDESC* pSurfaceDesc, LPDIRECTDRAWSURFACE* ppSurface)
{
    *ppSurface = nullptr;

    HRESULT hr = sDDraw_BBC3D4->CreateSurface(pSurfaceDesc, ppSurface, nullptr);

    if (hr == DDERR_INVALIDPARAMS)
    {
        const DWORD oldCaps = pSurfaceDesc->ddsCaps.dwCaps;
        if (!(oldCaps & DDSCAPS_SYSTEMMEMORY))
        {
            const DWORD tempCaps = (pSurfaceDesc->ddsCaps.dwCaps & ~DDSCAPS_VIDEOMEMORY) | DDSCAPS_SYSTEMMEMORY;
            pSurfaceDesc->ddsCaps.dwCaps = tempCaps;
            hr = sDDraw_BBC3D4->CreateSurface(pSurfaceDesc, ppSurface, nullptr);
            pSurfaceDesc->ddsCaps.dwCaps = oldCaps;
        }
    }
    return hr;
}

EXPORT signed int CC BMP_Blt_4F1E50(Bitmap* pDstBmp, int xPos, int yPos, Bitmap* pSrcBmp, LPRECT pRect, int flags)
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

ALIVE_VAR(1, 0xBBC458, DDBLTFX, blt_fx_stru_BBC458, {});

EXPORT signed int CC BMP_ClearRect_4F1EE0(Bitmap* pBmp, const RECT* pRect, DWORD fillColour)
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

EXPORT void CC BMP_unlock_4F2100(Bitmap* pBmp)
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

EXPORT void CC BMP_Release_DC_4F21A0(Bitmap* pBmp, HDC hdc)
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

EXPORT HDC CC BMP_Get_DC_4F2150(Bitmap* pBmp)
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

EXPORT LONG CC BMP_Get_Font_Height_4F21F0(Bitmap* pBmp)
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

EXPORT signed int CC BMP_New_convert_BPP_4F1CC0(int bpp)
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

EXPORT int CC Bmp_Convert_Colour_4F17D0(Bitmap* pBmp, int r, int g, int b)
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

EXPORT void CC Bmp_Free_4F1950(Bitmap* pBmp)
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

ALIVE_VAR(1, 0xBC0BB4, unsigned __int8, gVGA_force_sys_memory_surfaces_BC0BB4, 0);

EXPORT signed int CC BMP_New_4F1990(Bitmap* pBitmap, int width, int height, int pixelFormat, int createFlags)
{
    if (!pBitmap || !width || !height)
    {
        return -1;
    }

    memset(pBitmap, 0, sizeof(Bitmap));

    const int bpp = BMP_New_convert_BPP_4F1CC0(pixelFormat);
    if (bpp < 0)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 170, -1, "BMP_New: bits per pixel not supported");
        return -1;
    }

    DWORD bMask = 0;
    DWORD rMask = 0;
    DWORD gMask = 0;
    DWORD flags = 0;

    switch (bpp)
    {
    case 1:
        flags = DDPF_PALETTEINDEXED1; // 0x800
        break;
    case 2:
        flags = DDPF_PALETTEINDEXED2; // 0x1000
        break;
    case 4:
        flags = DDPF_PALETTEINDEXED4; // 0x8
        break;
    case 8:
        flags = DDPF_PALETTEINDEXED8; // 0x20
        break;
    case 15:
        rMask = 0x7C00;
        gMask = 0x3E0;
        bMask = 0x1F;
        break;
    case 16:
        rMask = 0xF800;
        gMask = 0x7E0;
        bMask = 0x1F;
        break;
    case 24:
    case 32:
        rMask = 0xFF0000;
        gMask = 0xFF00;
        bMask = 0xFF;
        break;
    case 115:
        rMask = 0x1F;
        gMask = 0x3E0;
        bMask = 0x7C00;
        break;
    case 116:
        rMask = 0x1F;
        gMask = 0x7E0;
        bMask = 0xF800;
        break;
    default:
        break;
    }

    DDSURFACEDESC pSurfaceDesc = {};
    pSurfaceDesc.dwSize = sizeof(DDSURFACEDESC);
    pSurfaceDesc.ddpfPixelFormat.dwBBitMask = bMask;
    pSurfaceDesc.ddpfPixelFormat.dwFlags = flags | DDPF_RGB; // 0x40
    pSurfaceDesc.ddckCKSrcBlt.dwColorSpaceHighValue = 0;
    pSurfaceDesc.ddckCKSrcBlt.dwColorSpaceLowValue = 0;

    pSurfaceDesc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    pSurfaceDesc.ddpfPixelFormat.dwRBitMask = rMask;
    pSurfaceDesc.ddpfPixelFormat.dwGBitMask = gMask;
    pSurfaceDesc.ddpfPixelFormat.dwRGBBitCount = bpp;

    pSurfaceDesc.dwFlags = DDSD_CAPS | DDSD_PIXELFORMAT | DDSD_CKSRCBLT | DDSD_WIDTH | DDSD_HEIGHT; // 0x11007;
    pSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; // 0x40

    if (gVGA_force_sys_memory_surfaces_BC0BB4)
    {
        pSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN; // 0x840
    }
    else if (createFlags & 1)
    {
        pSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN; // 0x840
    }
    else if (createFlags & 2)
    {
        pSurfaceDesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_OFFSCREENPLAIN; // 0x4040 
    }

    pSurfaceDesc.dwWidth = width;
    pSurfaceDesc.dwHeight = height;

    if (BMP_New_create_surface_4F1C60(&pSurfaceDesc, &pBitmap->field_0_pSurface))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 230, -1, "BMP_New: can't create the surface");
        return -1;
    }
    pBitmap->field_10_locked_pitch = pSurfaceDesc.lPitch;

    if (pBitmap->field_0_pSurface->GetSurfaceDesc(&pSurfaceDesc))
    {
        pBitmap->field_10_locked_pitch = 0;
        Error_NullPrint_4F28C0("BMP: GetSurfaceDesc failed on new BMP_t");
    }
    else
    {
        pBitmap->field_10_locked_pitch = pSurfaceDesc.lPitch;
    }

    pBitmap->field_C_height = height;
    pBitmap->field_14_bpp = static_cast<char>(pSurfaceDesc.ddpfPixelFormat.dwRGBBitCount);
    pBitmap->field_8_width = width;
    pBitmap->field_15_pixel_format = static_cast<char>(pixelFormat);
    pBitmap->field_18_create_flags = createFlags;
    return 0;
}

ALIVE_VAR(1, 0xBBC3E8, DDSURFACEDESC, sBmpSurfaceDesc_BBC3E8, {});

EXPORT LPVOID CC BMP_Lock_4F1FF0(Bitmap* pBitmap)
{
    // Already locked or we don't have a surface
    if (pBitmap->field_4_pLockedPixels || !pBitmap->field_0_pSurface)
    {
        return  pBitmap->field_4_pLockedPixels;
    }

    // Try to lock
    sBmpSurfaceDesc_BBC3E8.dwSize = sizeof(DDSURFACEDESC);
    HRESULT hr = pBitmap->field_0_pSurface->Lock(nullptr, &sBmpSurfaceDesc_BBC3E8, DDLOCK_WAIT, 0);
    if (hr == DDERR_SURFACELOST)
    {
        // Surface is gone, restore and try again
        hr = pBitmap->field_0_pSurface->Restore();
        if (SUCCEEDED(hr))
        {
            hr = pBitmap->field_0_pSurface->Lock(nullptr, &sBmpSurfaceDesc_BBC3E8, DDLOCK_WAIT, 0);
        }
    }

    if (SUCCEEDED(hr))
    {
        // OK locked
        pBitmap->field_10_locked_pitch = sBmpSurfaceDesc_BBC3E8.lPitch;
        pBitmap->field_4_pLockedPixels = sBmpSurfaceDesc_BBC3E8.lpSurface;
        return pBitmap->field_4_pLockedPixels;
    }
    else if (hr != DDERR_WRONGMODE)
    {
        // Push an error for everything bar wrong mode
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\BMP.C", 363, -1, DX_HR_To_String_4F4EC0(hr));
    }

    return nullptr;
}

EXPORT void CC BMP_Draw_String_4F2230(Bitmap* pBmp, int x, int y, unsigned int fgColour, int bgColour, LPCSTR lpString)
{
    const HDC dc = BMP_Get_DC_4F2150(pBmp);
    if (bgColour)
    {
        ::SetBkColor(dc, 0);
    }

    ::SetBkMode(dc, (bgColour != 0) + 1);

    unsigned int colour = 0;
    colour |= (BYTE0(fgColour) << 16);
    colour |= (BYTE1(fgColour) << 8);
    colour |= (BYTE2(fgColour) << 0);

    ::SetTextColor(dc, colour);
    ::TextOutA(dc, x, y, lpString, strlen(lpString));
    BMP_Release_DC_4F21A0(pBmp, dc);
}

#include "gmock/gmock.h"

namespace Test
{

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

        STDMETHOD(GetGDISurface)(THIS_ LPDIRECTDRAWSURFACE FAR*)
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

        STDMETHOD(Initialize)(THIS_ GUID FAR*)
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

    HDC CC Stub_BMP_Get_DC_4F2150(Bitmap*)
    {
        return nullptr;
    }

    void CC Stub_BMP_Release_DC_4F21A0(Bitmap*, HDC)
    {

    }

    COLORREF WINAPI Stub_SetBkColor(HDC, COLORREF)
    {
        return 0;
    }

    int WINAPI Stub_SetBkMode(HDC, int)
    {
        return 0;
    }

    static COLORREF sLastSetTextColour = 0;
    COLORREF WINAPI Stub_SetTextColor(HDC, COLORREF color)
    {
        sLastSetTextColour = color;
        return 0;
    }

    BOOL WINAPI Stub_TextOutA(HDC, _In_ int, _In_ int, LPCSTR, int)
    {
        return TRUE;
    }

    static void Test_BMP_Draw_String_4F2230()
    {
        SCOPED_REDIRECT(BMP_Get_DC_4F2150, Stub_BMP_Get_DC_4F2150);
        SCOPED_REDIRECT(BMP_Release_DC_4F21A0, Stub_BMP_Release_DC_4F21A0);
        SCOPED_REDIRECT(SetBkColor, Stub_SetBkColor);
        SCOPED_REDIRECT(SetBkMode, Stub_SetBkMode);
        SCOPED_REDIRECT(SetTextColor, Stub_SetTextColor);
        SCOPED_REDIRECT(TextOutA, Stub_TextOutA);
        BMP_Draw_String_4F2230(nullptr, 0, 0, 0xAABBCCDD, 0xFFAABBDD, "Hello");
        ASSERT_EQ(sLastSetTextColour, 0x00DDCCBBu);
    }


    using namespace ::testing;

    void Test_BMP_New_create_surface_4F1C60()
    {
        DDSURFACEDESC desc = {};
        LPDIRECTDRAWSURFACE surf = nullptr;

        DDSURFACEDESC actualData = {};
        for (int i = 0; i < 32; i++)
        {
            const DWORD flag = (1 << i);
            if (flag != DDSCAPS_SYSTEMMEMORY && flag != DDSCAPS_VIDEOMEMORY)
            {
                StrictMock<MockedDirectDraw> mocked;
                DirectDrawMock mock(mocked);

                sDDraw_BBC3D4 = &mock;

                memset(&actualData, 0, sizeof(DDSURFACEDESC));
                desc.ddsCaps.dwCaps = flag;

                EXPECT_CALL(mocked, CreateSurface(&desc, _, nullptr))
                    .WillOnce(Return(DDERR_INVALIDPARAMS))
                    .WillOnce(DoAll(SaveArgPointee<0>(&actualData), Return(S_OK)));
                ASSERT_EQ(S_OK, BMP_New_create_surface_4F1C60(&desc, &surf));

                ASSERT_EQ(actualData.ddsCaps.dwCaps, static_cast<DWORD>(DDSCAPS_SYSTEMMEMORY | flag));
            }
        }

        {
            StrictMock<MockedDirectDraw> mocked;
            DirectDrawMock mock(mocked);

            sDDraw_BBC3D4 = &mock;

            memset(&actualData, 0, sizeof(DDSURFACEDESC));
            desc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY;

            EXPECT_CALL(mocked, CreateSurface(&desc, _, nullptr))
                .WillOnce(Return(DDERR_INVALIDPARAMS))
                .WillOnce(DoAll(SaveArgPointee<0>(&actualData), Return(S_OK)));
            ASSERT_EQ(S_OK, BMP_New_create_surface_4F1C60(&desc, &surf));

            ASSERT_EQ(actualData.ddsCaps.dwCaps, static_cast<DWORD>(DDSCAPS_SYSTEMMEMORY));
        }

        sDDraw_BBC3D4 = nullptr;
    }

    void BmpTests()
    {
        Test_BMP_Draw_String_4F2230();
        Test_BMP_New_create_surface_4F1C60();
    }
}
