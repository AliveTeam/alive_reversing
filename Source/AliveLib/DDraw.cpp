#include "stdafx.h"
#include "DDraw.hpp"
#include "Function.hpp"
#include "Error.hpp"

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

        if (sDD_Pal_BBC3D8)
        {
            sDD_Pal_BBC3D8->Release();
            sDD_Pal_BBC3D8 = nullptr;
        }
        
        if (bDestroyDD)
        {
            sDDraw_BBC3D4->RestoreDisplayMode();
            sDDraw_BBC3D4->SetCooperativeLevel(sDD_hWnd_BBC3B0, DISCL_BACKGROUND); // 8
            sDDraw_BBC3D4->Release();
            sDDraw_BBC3D4 = nullptr;
        }
    }
    return 1;
}

ALIVE_VAR(1, 0xBBC3B8, BOOL, sDD_Caps_BBC3B8, FALSE); // Force ram surfaces?
ALIVE_VAR(1, 0xBBC3A0, BOOL, sDD_VideoMemory_BBC3A0, FALSE);
ALIVE_VAR(1, 0xBBC3C0, DWORD, sDDColourKey_BBC3C0, 0);

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
