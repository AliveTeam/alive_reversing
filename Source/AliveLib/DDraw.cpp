#include "stdafx.h"
#include "DDraw.hpp"
#include "Function.hpp"
#include "Error.hpp"
#include "Sys.hpp"

EXPORT const char* CC DX_HR_To_String_4F4EC0(HRESULT hr)
{
    switch (hr)
    {
    case S_OK:
        return "No error.";
    case DDERR_NOHWND:
        return  "DDERR_NOHWND\n"
            "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.";
    case DDERR_HWNDSUBCLASSED:
        return  "DDERR_HWNDSUBCLASSED\n"
            "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.";
    case DDERR_HWNDALREADYSET:
        return "DDERR_HWNDALREADYSET\n"
            "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces "
            "or palettes created.";
    case DDERR_NOPALETTEATTACHED:
        return "DDERR_NOPALETTEATTACHED\nNo palette object attached to this surface.";
    case DDERR_NOPALETTEHW:
        return "DDERR_NOPALETTEHW\nNo hardware support for 16 or 256 color palettes.";
    case DDERR_BLTFASTCANTCLIP:
        return "DDERR_BLTFASTCANTCLIP\n"
            "Return if a clipper object is attached to the source surface passed into a BltFast call.";
    case DDERR_NOBLTHW:
        return "DDERR_NOBLTHW\nNo blitter hardware present.";
    case DDERR_NODDROPSHW:
        return "DDERR_NODDROPSHW\nNo DirectDraw ROP hardware.";
    case DDERR_OVERLAYNOTVISIBLE:
        return "DDERR_OVERLAYNOTVISIBLE\nReturned when GetOverlayPosition is called on a hidden overlay.";
    case DDERR_NOOVERLAYDEST:
        return "DDERR_NOOVERLAYDEST\n"
            "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called o"
            "n to establish a destination.";
    case DDERR_INVALIDPOSITION:
        return "DDERR_INVALIDPOSITION\n"
            "Returned when the position of the overlay on the destination is no longer legal for that destination.";
    case DDERR_NOTAOVERLAYSURFACE:
        return "DDERR_NOTAOVERLAYSURFACE\nReturned when an overlay member is called for a non-overlay surface.";
    case DDERR_EXCLUSIVEMODEALREADYSET:
        return "DDERR_EXCLUSIVEMODEALREADYSET\n"
            "An attempt was made to set the cooperative level when it was already set to exclusive.";
    case DDERR_NOTFLIPPABLE:
        return "DDERR_NOTFLIPPABLE\nAn attempt has been made to flip a surface that is not flippable.";
    case DDERR_CANTDUPLICATE:
        return "DDERR_CANTDUPLICATE\n"
            "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.";
    case DDERR_NOTLOCKED:
        return "DDERR_NOTLOCKED\n"
            "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.";
    case DDERR_CANTCREATEDC:
        return "DDERR_CANTCREATEDC\nWindows can not create any more DCs.";
    case DDERR_NODC:
        return "DDERR_NODC\nNo DC was ever created for this surface.";
    case DDERR_WRONGMODE:
        return "DDERR_WRONGMODE\nThis surface can not be restored because it was created in a different mode.";
    case DDERR_IMPLICITLYCREATED:
        return "DDERR_IMPLICITLYCREATED\n"
            "This surface can not be restored because it is an implicitly created surface.";
    case DDERR_NOTPALETTIZED:
        return "DDERR_NOTPALETTIZED\nThe surface being used is not a palette-based surface.";
    case DDERR_NOCLIPPERATTACHED:
        return "DDERR_NOCLIPPERATTACHED\nNo clipper object attached to surface object.";
    case DDERR_COLORKEYNOTSET:
        return "DDERR_COLORKEYNOTSET\nNo src color key specified for this operation.";
    case DDERR_SURFACEALREADYATTACHED:
        return "DDERR_SURFACEALREADYATTACHED\n"
            "This surface is already attached to the surface it is being attached to.";
    case DDERR_SURFACEALREADYDEPENDENT:
        return "DDERR_SURFACEALREADYDEPENDENT\n"
            "This surface is already a dependency of the surface it is being made a dependency of.";
    case DDERR_SURFACEBUSY:
        return "DDERR_SURFACEBUSY\n"
            "Access to this surface is being refused because the surface is already locked by another thread.";
    case DDERR_SURFACEISOBSCURED:
        return "DDERR_SURFACEISOBSCURED\nAccess to surface refused because the surface is obscured.";
    case DDERR_SURFACELOST:
        return "DDERR_SURFACELOST\n"
            "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface obj"
            "ect representing this surface should have Restore called on it.";
    case DDERR_SURFACENOTATTACHED:
        return "DDERR_SURFACENOTATTACHED\nThe requested surface is not attached.";
    case DDERR_TOOBIGHEIGHT:
        return "DDERR_TOOBIGHEIGHT\nHeight requested by DirectDraw is too large.";
    case DDERR_TOOBIGSIZE:
        return "DDERR_TOOBIGSIZE\n"
            "Size requested by DirectDraw is too large, but the individual height and width are OK.";
    case DDERR_TOOBIGWIDTH:
        return "DDERR_TOOBIGWIDTH\nWidth requested by DirectDraw is too large.";
    case DDERR_UNSUPPORTEDFORMAT:
        return "DDERR_UNSUPPORTEDFORMAT\nFOURCC format requested is unsupported by DirectDraw.";
    case DDERR_UNSUPPORTEDMASK:
        return "DDERR_UNSUPPORTEDMASK\nBitmask in the pixel format requested is unsupported by DirectDraw.";
    case DDERR_VERTICALBLANKINPROGRESS:
        return "DDERR_VERTICALBLANKINPROGRESS\nVertical blank is in progress.";
    case DDERR_WASSTILLDRAWING:
        return "DDERR_WASSTILLDRAWING\n"
            "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.";
    case DDERR_XALIGN:
        return "DDERR_XALIGN\nRectangle provided was not horizontally aligned on required boundary.";
    case DDERR_INVALIDDIRECTDRAWGUID:
        return "DDERR_INVALIDDIRECTDRAWGUID\n"
            "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.";
    case DDERR_DIRECTDRAWALREADYCREATED:
        return "DDERR_DIRECTDRAWALREADYCREATED\n"
            "A DirectDraw object representing this driver has already been created for this process.";
    case DDERR_NODIRECTDRAWHW:
        return "DDERR_NODIRECTDRAWHW\n"
            "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.";
    case DDERR_PRIMARYSURFACEALREADYEXISTS:
        return "DDERR_PRIMARYSURFACEALREADYEXISTS\nThis process already has created a primary surface.";
    case DDERR_NOEMULATION:
        return "DDERR_NOEMULATION\nSoftware emulation not available.";
    case DDERR_REGIONTOOSMALL:
        return "DDERR_REGIONTOOSMALL\nRegion passed to Clipper::GetClipList is too small.";
    case DDERR_CLIPPERISUSINGHWND:
        return "DDERR_CLIPPERISUSINGHWND\n"
            "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.";
    case DDERR_PALETTEBUSY:
        return "DDERR_PALETTEBUSY\n"
            "Access to this palette is being refused because the palette is already locked by another thread.";
    case DDERR_NOCOLORKEY:
        return "DDERR_NOCOLORKEY\nSurface doesn't currently have a color key";
    case DDERR_NOCOLORKEYHW:
        return "DDERR_NOCOLORKEYHW\n"
            "Operation could not be carried out because there is no hardware support of the destination color key.";
    case DDERR_NOEXCLUSIVEMODE:
        return "DDERR_NOEXCLUSIVEMODE\n"
            "Operation requires the application to have exclusive mode but the application does not have exclusive mode.";
    case DDERR_NOFLIPHW:
        return "DDERR_NOFLIPHW\nFlipping visible surfaces is not supported.";
    case DDERR_NOGDI:
        return "DDERR_NOGDI\nThere is no GDI present.";
    case DDERR_NOMIRRORHW:
        return "DDERR_NOMIRRORHW\n"
            "Operation could not be carried out because there is no hardware present or available.";
    case DDERR_NOTFOUND:
        return "DDERR_NOTFOUND\nRequested item was not found.";
    case DDERR_NOOVERLAYHW:
        return "DDERR_NOOVERLAYHW\n"
            "Operation could not be carried out because there is no overlay hardware present or available.";
    case DDERR_NORASTEROPHW:
        return "DDERR_NORASTEROPHW\n"
            "Operation could not be carried out because there is no appropriate raster op hardware present or available.";
    case DDERR_NOROTATIONHW:
        return "DDERR_NOROTATIONHW\n"
            "Operation could not be carried out because there is no rotation hardware present or available.";
    case DDERR_NOSTRETCHHW:
        return "DDERR_NOSTRETCHHW\n"
            "Operation could not be carried out because there is no hardware support for stretching.";
    case DDERR_NOT4BITCOLOR:
        return "DDERR_NOT4BITCOLOR\n"
            "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.";
    case DDERR_NOT4BITCOLORINDEX:
        return "DDERR_NOT4BITCOLORINDEX\n"
            "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit colo"
            "r index palette.";
    case DDERR_NOT8BITCOLOR:
        return "DDERR_NOT8BITCOLOR\n"
            "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.";
    case DDERR_NOTEXTUREHW:
        return "DDERR_NOTEXTUREHW\n"
            "Operation could not be carried out because there is no texture mapping hardware present or available.";
    case DDERR_NOVSYNCHW:
        return "DDERR_NOVSYNCHW\n"
            "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.";
    case DDERR_NOZBUFFERHW:
        return "DDERR_NOZBUFFERHW\n"
            "Operation could not be carried out because there is no hardware support for zbuffer blitting.";
    case DDERR_NOZOVERLAYHW:
        return "DDERR_NOZOVERLAYHW\n"
            "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support"
            " z layering of overlays.";
    case DDERR_OUTOFCAPS:
        return "DDERR_OUTOFCAPS\nThe hardware needed for the requested operation has already been allocated.";
    case DDERR_OUTOFVIDEOMEMORY:
        return "DDERR_OUTOFVIDEOMEMORY\nDirectDraw does not have enough memory to perform the operation.";
    case DDERR_OVERLAYCANTCLIP:
        return "DDERR_OVERLAYCANTCLIP\nThe hardware does not support clipped overlays.";
    case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
        return "DDERR_OVERLAYCOLORKEYONLYONEACTIVE\nCan only have ony color key active at one time for overlays.";
    case DDERR_NOCOOPERATIVELEVELSET:
        return "DDERR_NOCOOPERATIVELEVELSET\n"
            "Create function called without DirectDraw object method SetCooperativeLevel being called.";
    case DDERR_INVALIDCLIPLIST:
        return "DDERR_INVALIDCLIPLIST\nDirectDraw does not support the provided cliplist.";
    case DDERR_INVALIDMODE:
        return "DDERR_INVALIDMODE\nDirectDraw does not support the requested mode.";
    case DDERR_INVALIDOBJECT:
        return "DDERR_INVALIDOBJECT\nDirectDraw received a pointer that was an invalid DIRECTDRAW object.";
    case DDERR_INVALIDPIXELFORMAT:
        return "DDERR_INVALIDPIXELFORMAT\nThe pixel format was invalid as specified.";
    case DDERR_INVALIDRECT:
        return "DDERR_INVALIDRECT\nRectangle provided was invalid.";
    case DDERR_LOCKEDSURFACES:
        return "DDERR_LOCKEDSURFACES\nOperation could not be carried out because one or more surfaces are locked.";
    case DDERR_NO3D:
        return "DDERR_NO3D\nThere is no 3D present.";
    case DDERR_NOALPHAHW:
        return "DDERR_NOALPHAHW\n"
            "Operation could not be carried out because there is no alpha accleration hardware present or available.";
    case DDERR_NOCLIPLIST:
        return "DDERR_NOCLIPLIST\nNo cliplist available.";
    case DDERR_NOCOLORCONVHW:
        return "DDERR_NOCOLORCONVHW\n"
            "Operation could not be carried out because there is no color conversion hardware present or available.";
    case DDERR_INVALIDCAPS:
        return "DDERR_INVALIDCAPS\nOne or more of the caps bits passed to the callback are incorrect.";
    case DDERR_CANNOTATTACHSURFACE:
        return "DDERR_CANNOTATTACHSURFACE\nThis surface can not be attached to the requested surface.";
    case DDERR_INVALIDPARAMS:
        return "DDERR_INVALIDPARAMS\nOne or more of the parameters passed to the function are incorrect.";
    case DDERR_ALREADYINITIALIZED:
        return "DDERR_ALREADYINITIALIZED\nThis object is already initialized.";
    case DDERR_OUTOFMEMORY:
        return "DDERR_OUTOFMEMORY\nDirectDraw does not have enough memory to perform the operation.";
    case DDERR_UNSUPPORTED:
        return "DDERR_UNSUPPORTED\nAction not supported.";
    case DDERR_GENERIC:
        return "DDERR_GENERIC\nGeneric failure.";
    case DDERR_CANNOTDETACHSURFACE:
        return "DDERR_CANNOTDETACHSURFACE\nThis surface can not be detached from the requested surface.";
    case DDERR_CURRENTLYNOTAVAIL:
        return "DDERR_CURRENTLYNOTAVAIL\nSupport is currently not available.";
    case DDERR_EXCEPTION:
        return "DDERR_EXCEPTION\nAn exception was encountered while performing the requested operation.";
    case DDERR_HEIGHTALIGN:
        return "DDERR_HEIGHTALIGN\nHeight of rectangle provided is not a multiple of read alignment.";
    case DDERR_INCOMPATIBLEPRIMARY:
        return "DDERR_INCOMPATIBLEPRIMARY\n"
            "Unable to match primary surface creation request with existing primary surface.";
    }
    return "Unrecognized error value.";
}

ALIVE_VAR(1, 0xBBC3D4, LPDIRECTDRAW, sDDraw_BBC3D4, nullptr);
ALIVE_VAR(1, 0xBBC3B0, HWND, sDD_hWnd_BBC3B0, nullptr);
ALIVE_VAR(1, 0xBBC3DC, LPDIRECTDRAWCLIPPER, sDD_Clipper_BBC3DC, nullptr);
ALIVE_VAR(1, 0xBBC3C8, LPDIRECTDRAWSURFACE, sDD_Surface1_BBC3C8, nullptr);
ALIVE_VAR(1, 0xBBC3CC, LPDIRECTDRAWSURFACE, sDD_Surface2_BBC3CC, nullptr);
ALIVE_VAR(1, 0xBBC3D8, LPDIRECTDRAWPALETTE, sDD_Pal_BBC3D8, nullptr);

ALIVE_VAR(1, 0xBBC3AC, int, sbDD_FlipMode_BBC3AC, 0); // TODO: Make Enum
ALIVE_VAR(1, 0xBBC3E0, LONG, sDD_old_win_style_BBC3E0, 0);


EXPORT signed int CC DD_Shutdown_4F0790(int bDestroyDD)
{
    if (sDDraw_BBC3D4)
    {
        if (sDD_Clipper_BBC3DC)
        {
            sDD_Clipper_BBC3DC->Release();
            sDD_Clipper_BBC3DC = nullptr;
        }

        if (sDD_Surface2_BBC3CC)
        {
            sDD_Surface2_BBC3CC->Release();
            sDD_Surface2_BBC3CC = nullptr;
        }

        if (sDD_Surface1_BBC3C8)
        {
            sDD_Surface1_BBC3C8->Release();
            sDD_Surface1_BBC3C8 = nullptr;
        }

        /* Only used in unsupported 8 bpp mode
        if (sDD_Pal_BBC3D8)
        {
            sDD_Pal_BBC3D8->Release();
            sDD_Pal_BBC3D8 = nullptr;
        }*/
        
        if (bDestroyDD)
        {
            sDDraw_BBC3D4->RestoreDisplayMode();
            sDDraw_BBC3D4->SetCooperativeLevel(sDD_hWnd_BBC3B0, DDSCL_NORMAL); // 8
            sDDraw_BBC3D4->Release();
            sDDraw_BBC3D4 = nullptr;
        }
    }
    return 1;
}

ALIVE_VAR(1, 0xBBC3B8, BOOL, sDD_Caps_BBC3B8, FALSE); // Force ram surfaces?
ALIVE_VAR(1, 0xBBC3A0, BOOL, sDD_VideoMemory_BBC3A0, FALSE);
ALIVE_VAR(1, 0xBBC3C0, DWORD, sDDColourKey_BBC3C0, 0);
ALIVE_VAR(1, 0xBBC3BC, bool, sbFullScreen_BBC3BC, 0);

EXPORT LPDIRECTDRAWSURFACE CC DD_Create_Surface_4F0CB0(int width, int height, int bSetUnknownCaps)
{
    DDSURFACEDESC surfaceDesc = {};
    surfaceDesc.dwSize = sizeof(surfaceDesc);
    surfaceDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; // 7
    surfaceDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; // 0x40
    if (bSetUnknownCaps || sDD_Caps_BBC3B8)
    {
        surfaceDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN; // 0x840
    }
    surfaceDesc.dwWidth = width;
    surfaceDesc.dwHeight = height;

    LPDIRECTDRAWSURFACE pSurface = nullptr;
    const HRESULT hr = sDDraw_BBC3D4->CreateSurface(&surfaceDesc, &pSurface, 0);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 650, -1, DX_HR_To_String_4F4EC0(hr));
        return nullptr;
    }

    if (!sDD_VideoMemory_BBC3A0)
    {
        DDCOLORKEY colourKey = {};
        colourKey.dwColorSpaceLowValue = sDDColourKey_BBC3C0;
        colourKey.dwColorSpaceHighValue = sDDColourKey_BBC3C0;
        pSurface->SetColorKey(DDCKEY_SRCBLT, &colourKey); // 8
    }
    
    return pSurface;
}

EXPORT signed int CC DD_RestoreSurfacesIfRequired_4F01D0(HRESULT hr, IDirectDrawSurface* pSurface1, IDirectDrawSurface* pSurface2)
{
    if (hr == DDERR_SURFACELOST)
    {
        if (pSurface1)
        {
            hr = pSurface1->Restore();
            if (FAILED(hr))
            {
                if (hr != DDERR_WRONGMODE)
                {
                    Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 55, -1, DX_HR_To_String_4F4EC0(hr));
                    return -1;
                }
            }
        }

        if (pSurface2)
        {
            hr = pSurface2->Restore();
            if (FAILED(hr))
            {
                if (hr != DDERR_WRONGMODE)
                {
                    Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 67, -1, DX_HR_To_String_4F4EC0(hr));
                    return -1;
                }
            }
        }

    }
    else
    {
        DDSURFACEDESC surface1Desc = {};
        surface1Desc.dwSize = sizeof(DDSURFACEDESC);
        hr = pSurface1->GetSurfaceDesc(&surface1Desc);
        if (FAILED(hr))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 82, -1, DX_HR_To_String_4F4EC0(hr));
        }

        DDSURFACEDESC surface2Desc = {};
        surface2Desc.dwSize = sizeof(DDSURFACEDESC);
        hr = pSurface2->GetSurfaceDesc(&surface2Desc);
        if (FAILED(hr))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 86, -1, DX_HR_To_String_4F4EC0(hr));
        }
    }
    return 0;
}

#pragma comment(lib, "ddraw.lib") // DirectDrawCreate

EXPORT signed int CC DD_Init_4F02D0(HWND hwnd, bool bFullScreen, int forceSoftwareSurfaces)
{
    if (!sDDraw_BBC3D4)
    {
        static LPDIRECTDRAW sDirectDraw_BBC3D0 = nullptr;

        sDD_Caps_BBC3B8 = forceSoftwareSurfaces;
        HRESULT hr = DirectDrawCreate(0, &sDirectDraw_BBC3D0, 0);
        if (FAILED(hr))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 136, -1, DX_HR_To_String_4F4EC0(hr));
            return 0;
        }

        sbFullScreen_BBC3BC = bFullScreen;
        sDD_hWnd_BBC3B0 = hwnd;

        if (bFullScreen)
        {
            hr = sDirectDraw_BBC3D0->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_ALLOWMODEX); // 0x53
        }
        else
        {
            hr = sDirectDraw_BBC3D0->SetCooperativeLevel(hwnd, DDSCL_NORMAL); // 8
        }

        if (FAILED(hr))
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 151, -1, DX_HR_To_String_4F4EC0(hr));
            return 0;
        }
        sDDraw_BBC3D4 = sDirectDraw_BBC3D0;
    }
    return 1;
}

EXPORT void CC DD_Blt_4F0170(IDirectDrawSurface* pSourceSurface, LPRECT pSrcRect, IDirectDrawSurface* pTargetSurface, LPRECT pDstRect, int bltFlags)
{
    const HRESULT hr = pTargetSurface->Blt(pDstRect, pSourceSurface, pSrcRect, bltFlags | DDBLT_WAIT, nullptr); // 0x1000000
    if (FAILED(hr))
    {
        DD_RestoreSurfacesIfRequired_4F01D0(hr, pSourceSurface, pTargetSurface);
        pTargetSurface->Blt(pDstRect, pSourceSurface, pSrcRect, bltFlags | DDBLT_WAIT, nullptr); // 0x1000000
    }
}

EXPORT void CC DD_Flip_4F15D0()
{
    if (!sDD_Surface1_BBC3C8 || !sDD_Surface2_BBC3CC)
    {
        return;
    }

    // Windowed path
    if (!sbFullScreen_BBC3BC)
    {
        // Window rect in client coords
        RECT rect = {};
        ::GetClientRect(sDD_hWnd_BBC3B0, &rect);

        // Convert to screen coords
        POINT rectPoints[2] =
        {
            { rect.left, rect.top },
            { rect.right, rect.bottom }
        };
        ::ClientToScreen(sDD_hWnd_BBC3B0, &rectPoints[0]);
        ::ClientToScreen(sDD_hWnd_BBC3B0, &rectPoints[1]);

        // Make a new rect in screen coords
        RECT screenRect =
        {
            rectPoints[0].x, rectPoints[0].y,
            rectPoints[1].x, rectPoints[1].y
        };

        DD_Blt_4F0170(sDD_Surface2_BBC3CC, nullptr, sDD_Surface1_BBC3C8, &screenRect, 0);
        return;
    }
    
    // Full screen path
    if (sbDD_FlipMode_BBC3AC > 1)
    {
        HRESULT hr = S_OK;
        do
        {
            for (;;)
            {
                hr = sDD_Surface1_BBC3C8->Flip(nullptr, 0);
                if (SUCCEEDED(hr))
                {
                    // TODO / note 0xBBC3C4 appears to be unused
                    //byte_BBC3C4[0] = (byte_BBC3C4[0] + 1) % sbDD_FlipMode_BBC3AC;
                    return;
                }

                if (hr != DDERR_SURFACELOST)
                {
                    break;
                }

                if (DD_RestoreSurfacesIfRequired_4F01D0(DDERR_SURFACELOST, sDD_Surface1_BBC3C8, sDD_Surface2_BBC3CC))
                {
                    return;
                }

            }
        } while (hr == DDERR_WASSTILLDRAWING || hr == DDERR_WRONGMODE);
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 1292, -1, DX_HR_To_String_4F4EC0(hr));
    }
}

EXPORT int CC DD_SetDisplayMode_4F0730(DWORD width, DWORD height, DWORD bpp)
{
    const LONG winStyle = ::GetWindowLongA(Sys_GetHWnd_4F2C70(), GWL_STYLE);
    LONG newWinStyle = 0;
    if (sbFullScreen_BBC3BC)
    {
        sDD_old_win_style_BBC3E0 = winStyle;
        newWinStyle = winStyle & 0xFF3FFFFF; // TODO: Unset whatever win style this is removing
    }
    else
    {
        newWinStyle = sDD_old_win_style_BBC3E0;
    }
    ::SetWindowLongA(Sys_GetHWnd_4F2C70(), GWL_STYLE, newWinStyle);
    return sDDraw_BBC3D4->SetDisplayMode(width, height, bpp);
}

ALIVE_VAR(1, 0xBBC3A4, int, sDD_Width_BBC3A4, 0);
ALIVE_VAR(1, 0xBBC3A8, int, sDD_Height_BBC3A8, 0);

EXPORT signed int CC DD_Enable_4F0380(HWND /*hwnd*/, int width, int height, int bpp, int flipMode, int a6)
{
    sbDD_FlipMode_BBC3AC = flipMode;
    //byte_BBC3C4[0] = 0; // TODO: Never used?

    if (!sDDraw_BBC3D4)
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 206, -1, "DDEnable: no DD Object");
        return 0;
    }

    DWORD bitsPerPixelXPlanes = 0;
    int heightCopy = 0;
    int widthCopy = 0;
    if (sbFullScreen_BBC3BC)
    {
        bitsPerPixelXPlanes = bpp;
        heightCopy = height;
        widthCopy = width;
        HRESULT hr = DD_SetDisplayMode_4F0730(width, height, bitsPerPixelXPlanes);
        if (FAILED(hr))
        {
            if (width != 640 || height != 480)
            {
                if (height * width >= 640 * 480)
                {
                    Error_DisplayMessageBox_4F2C80("C:\\abe2\\code\\POS\\MYDDRAW.C", 223, "Can't set mode trying 640x480");
                }
                else
                {
                    Error_MessageBox_4F2D00(
                        "C:\\abe2\\code\\POS\\MYDDRAW.C",
                        216,
                        "Can't set mode %ldx%ld, trying 648x480.\n"
                        "This will visibly decrease the performance\n"
                        "Please make sure to have the latest version\n"
                        "of the video drivers.\n"
                        "\n"
                        "DirectDraw error:\n"
                        " %s",
                        width,
                        height,
                        DX_HR_To_String_4F4EC0(hr));
                }
                heightCopy = 480;
                widthCopy = 640;
                hr = DD_SetDisplayMode_4F0730(640u, 480u, bpp);
            }

            if (FAILED(hr))
            {
               
                // 8 bpp not supported
                if (bitsPerPixelXPlanes != 8)
                {
                    Error_DisplayMessageBox_4F2C80("C:\\abe2\\code\\POS\\MYDDRAW.C", 232, "Not trying 640x480x8 as 8 bpp mode not supported");

                    /*
                    Error_DisplayMessageBox_4F2C80("C:\\abe2\\code\\POS\\MYDDRAW.C", 232, "Can't set mode trying 640x480x8");
                    bitsPerPixelXPlanes = 8;
                    hr = DD_SetDisplayMode_4F0730(640u, heightCopy, 8u);
                    */
                }

                // Then try 16 bpp
                if (FAILED(hr))
                {
                    if (bitsPerPixelXPlanes != 16)
                    {
                        Error_DisplayMessageBox_4F2C80("C:\\abe2\\code\\POS\\MYDDRAW.C", 238, "Can't set mode trying 640x480x16");
                        bitsPerPixelXPlanes = 16;
                        hr = DD_SetDisplayMode_4F0730(640u, heightCopy, 16u);
                    }

                    // Finally give up
                    if (FAILED(hr))
                    {
                        Error_DisplayMessageBox_4F2C80("C:\\abe2\\code\\POS\\MYDDRAW.C", 244, "SetMode failed err=%d", hr);
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        HDC dc = ::GetDC(nullptr);
        bitsPerPixelXPlanes = ::GetDeviceCaps(dc, BITSPIXEL) * ::GetDeviceCaps(dc, PLANES); // 12, 14
        ::ReleaseDC(nullptr, dc);

        const LONG oldWinStyle = ::GetWindowLongA(sDD_hWnd_BBC3B0, GWL_STYLE);
        ::SetWindowLongA(sDD_hWnd_BBC3B0, GWL_STYLE, oldWinStyle & 0xFF39FFFF | 0x80000000); // TODO: use SDK constants
        heightCopy = height;
        widthCopy = width;

        RECT rect = {};
        ::SetRect(&rect, 0, 0, width, height);

        const LONG curExStyle = ::GetWindowLongA(sDD_hWnd_BBC3B0, GWL_EXSTYLE);
        const BOOL bMenu = ::GetMenu(sDD_hWnd_BBC3B0) != 0;
        const LONG curStyle = ::GetWindowLongA(sDD_hWnd_BBC3B0, GWL_STYLE);
        ::AdjustWindowRectEx(&rect, curStyle, bMenu, curExStyle);

        // TODO: use SDK constants
        ::SetWindowPos(sDD_hWnd_BBC3B0, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0x16u); 

        // TODO: use SDK constants
        ::SetWindowPos(sDD_hWnd_BBC3B0, (HWND)0xFFFFFFFE, 0, 0, 0, 0, 0x13u);
        RECT pvParam;
        ::SystemParametersInfoA(SPI_GETWORKAREA, 0, &pvParam, 0);
        ::GetWindowRect(sDD_hWnd_BBC3B0, &rect);

        int left = rect.left;
        if (rect.left < pvParam.left)
        {
            left = pvParam.left;
            rect.left = pvParam.left;
        }

        int top = rect.top;
        if (rect.top < pvParam.top)
        {
            top = pvParam.top;
            rect.top = pvParam.top;
        }

        // TODO: use SDK constants
        ::SetWindowPos(sDD_hWnd_BBC3B0, 0, left, top, 0, 0, 0x15u);
        
    }

    // Get DD caps
    DDCAPS ddCaps = {};
    ddCaps.dwSize = sizeof(DDSCAPS);
    HRESULT hr = sDDraw_BBC3D4->GetCaps(&ddCaps, nullptr);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 308, -1, DX_HR_To_String_4F4EC0(hr));
    }

    // TODO: What cap is this extracting, never read?
    //dword_BBC3B4 = (ddCaps.dwCaps >> 26) & 1;
    
    // TODO: Refactor + use SDK constants
    if (!sbDD_FlipMode_BBC3AC)
    {
        if (ddCaps.dwVidMemTotal <= (bitsPerPixelXPlanes << 20) / 8
            || (sbDD_FlipMode_BBC3AC = 3, widthCopy > 640))
        {
            sbDD_FlipMode_BBC3AC = 2;
        }
    }

    // TODO: Refactor + use SDK constants
    if (ddCaps.dwCaps & 0x400000
        && ddCaps.dwCKeyCaps & 1
        && (a6 || !(ddCaps.dwCKeyCaps & 0x200))
        && ddCaps.dwVidMemTotal >= bitsPerPixelXPlanes / 8 << 21)
    {
        sDD_VideoMemory_BBC3A0 = 1;
    }
    sDD_Width_BBC3A4 = widthCopy;
    sDD_Height_BBC3A8 = heightCopy;

    return 1;
}

// Not used, part of 8 bpp mode
//ALIVE_ARY(1, 0xBD2A80, PALETTEENTRY, 256, sDDPalEntry_BD2A80, {});

static signed int InitColourKeyAndPallete(LPDIRECTDRAWSURFACE pSurface)
{
    DDPIXELFORMAT pixelFormat = {};
    pixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    pSurface->GetPixelFormat(&pixelFormat);

    DWORD colourMask = 0;
    if (pixelFormat.dwRGBBitCount == 8)
    {
        colourMask = 255;
    }
    else
    {
        colourMask = pixelFormat.dwRBitMask | pixelFormat.dwGBitMask | pixelFormat.dwBBitMask;
    }

    sDDColourKey_BBC3C0 = colourMask;
    if (sDD_VideoMemory_BBC3A0)
    {
        DDCOLORKEY colourKey = {};
        colourKey.dwColorSpaceLowValue = colourMask;
        colourKey.dwColorSpaceHighValue = colourMask;
        sDD_Surface2_BBC3CC->SetColorKey(DDCKEY_DESTBLT, &colourKey); // 2
    }

    if (pixelFormat.dwRGBBitCount != 8)
    {
        return 1;
    }

    // TODO: 8 bit support code removed

    return 1;
}

static signed int CreateDDObjects(signed int a1)
{
    DDSURFACEDESC surfaceDesc = {};
    surfaceDesc.dwSize = sizeof(DDSURFACEDESC);

    surfaceDesc.dwFlags = 1; // TODO: Set constants
    surfaceDesc.ddsCaps.dwCaps = 512; // TODO: Set constants

    HRESULT hr = sDDraw_BBC3D4->CreateSurface(&surfaceDesc, &sDD_Surface1_BBC3C8, 0);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 499, -1, DX_HR_To_String_4F4EC0(hr));
        return 0;
    }

    if (a1 == 2)
    {
        sDD_Surface2_BBC3CC = DD_Create_Surface_4F0CB0(sDD_Width_BBC3A4, sDD_Height_BBC3A8, 0);
        if (!sDD_Surface2_BBC3CC)
        {
            Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 506, -1, DX_HR_To_String_4F4EC0(0));
            return 0;
        }
    }

    hr = sDDraw_BBC3D4->CreateClipper(0, &sDD_Clipper_BBC3DC, 0);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 513, -1, DX_HR_To_String_4F4EC0(hr));
        return 0;
    }

    hr = sDD_Clipper_BBC3DC->SetHWnd(0, sDD_hWnd_BBC3B0);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 519, -1, DX_HR_To_String_4F4EC0(hr));
        return 0;
    }

    hr = sDD_Surface1_BBC3C8->SetClipper(sDD_Clipper_BBC3DC);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 525, -1, DX_HR_To_String_4F4EC0(hr));
        return 0;
    }

    return InitColourKeyAndPallete(sDD_Surface1_BBC3C8);
}

EXPORT signed int CC DD_Init_4F0840(signed int a1)
{
    if (!sbFullScreen_BBC3BC)
    {
        return CreateDDObjects(a1);
    }

    if (a1 <= 1)
    {
        if (a1 == 1)
        {
            DDSURFACEDESC surfaceDesc = {};
            surfaceDesc.dwSize = sizeof(DDSURFACEDESC);
            surfaceDesc.dwFlags = 1; // TODO: Set constants
            surfaceDesc.ddsCaps.dwCaps = 512; // TODO: Set constants
            const HRESULT hr = sDDraw_BBC3D4->CreateSurface(&surfaceDesc, &sDD_Surface1_BBC3C8, 0);
            if (FAILED(hr))
            {
                Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 483, -1, DX_HR_To_String_4F4EC0(hr));
                return 0;
            }
            sDD_Surface1_BBC3C8->AddRef();
            sDD_Surface2_BBC3CC = sDD_Surface1_BBC3C8;
            return InitColourKeyAndPallete(sDD_Surface1_BBC3C8);
        }
        return CreateDDObjects(a1);
    }

    DDSURFACEDESC surfaceDesc = {};
    surfaceDesc.dwSize = sizeof(DDSURFACEDESC);
    surfaceDesc.dwFlags = 33; // TODO: Set constants
    surfaceDesc.dwBackBufferCount = a1 - 1;
    surfaceDesc.ddsCaps.dwCaps = 536; // TODO: Set constants
    HRESULT hr = sDDraw_BBC3D4->CreateSurface(&surfaceDesc, &sDD_Surface1_BBC3C8, 0);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 445, -1, DX_HR_To_String_4F4EC0(hr));
        return 0;
    }

    DDCAPS ddCaps = {};
    ddCaps.dwSize = sizeof(DDCAPS);
    hr = sDDraw_BBC3D4->GetCaps(&ddCaps, nullptr);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 459, -1, DX_HR_To_String_4F4EC0(hr));
        return 0;
    }

    ddCaps.ddsOldCaps.dwCaps = 4;
    hr = sDD_Surface1_BBC3C8->GetAttachedSurface(&ddCaps.ddsOldCaps, &sDD_Surface2_BBC3CC);
    if (FAILED(hr))
    {
        Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 470, -1, DX_HR_To_String_4F4EC0(hr));
        return 0;
    }

    return InitColourKeyAndPallete(sDD_Surface1_BBC3C8);
}

EXPORT void CC DD_mode_blt1_4F11E0(IDirectDrawSurface* /*pSurface*/, RECT* /*pRect*/, int /*screenMode*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC DD_mode_blt2_4F0F60(IDirectDrawSurface* /*pSurface*/, RECT* /*pRect*/, int /*screenMode*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC DD_render_back_buffer_4F0D90(IDirectDrawSurface* pSurf, RECT* pRect, int screenMode)
{
    if (sDD_Surface1_BBC3C8 && pSurf)
    {
        sDD_hWnd_BBC3B0 = Sys_GetHWnd_4F2C70();

        if (sbFullScreen_BBC3BC)
        {
            DDSURFACEDESC surfaceDesc1 = {};
            surfaceDesc1.dwSize = sizeof(DDSURFACEDESC);
            HRESULT hr = sDD_Surface1_BBC3C8->GetSurfaceDesc(&surfaceDesc1);
            if (FAILED(hr))
            {
                Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 1219, -1, DX_HR_To_String_4F4EC0(hr));
                return;
            }

            DDSURFACEDESC surfaceDesc2 = {};
            surfaceDesc2.dwSize = sizeof(DDSURFACEDESC);
            hr = pSurf->GetSurfaceDesc(&surfaceDesc2);
            if (FAILED(hr))
            {
                Error_PushErrorRecord_4F2920("C:\\abe2\\code\\POS\\MYDDRAW.C", 1226, -1, DX_HR_To_String_4F4EC0(hr));
                return;
            }

            RECT rect = {};
            RECT* pRectToUse = pRect;
            if (!pRectToUse)
            {
                rect.bottom = surfaceDesc2.dwHeight;
                rect.top = 0;
                rect.left = 0;
                rect.right = surfaceDesc2.dwWidth;
                pRectToUse = &rect;
            }

            const unsigned int rectW = pRectToUse->right - pRectToUse->left;
            const unsigned int rectH = pRectToUse->bottom - pRectToUse->top;

            if (surfaceDesc1.dwWidth != 2 * rectW || surfaceDesc1.dwHeight != 2 * rectH)
            {
                if (surfaceDesc1.dwWidth != rectW || surfaceDesc1.dwHeight != 2 * rectH)
                {
                    DD_Blt_4F0170(pSurf, pRectToUse, sDD_Surface1_BBC3C8, 0, 0);
                }
                else
                {
                    DD_mode_blt1_4F11E0(pSurf, pRectToUse, screenMode);
                }
            }
            else
            {
                DD_mode_blt2_4F0F60(pSurf, pRectToUse, screenMode);
            }
        }
        else
        {
            RECT rect = {};
            ::GetClientRect(Sys_GetHWnd_4F2C70(), &rect);
            POINT points[2] = 
            {
                { rect.left, rect.top },
                { rect.right, rect.bottom }
            };
            ::ClientToScreen(sDD_hWnd_BBC3B0, &points[0]);
            ::ClientToScreen(sDD_hWnd_BBC3B0, &points[1]);
            RECT screenRect =
            {
                points[0].x, points[0].y,
                points[1].x, points[1].y
            };
            DD_Blt_4F0170(pSurf, pRect, sDD_Surface1_BBC3C8, &screenRect, 0);
        }
    }
}
