#include "stdafx.h"
#include "VGA.hpp"
#include "../relive_lib/bmp.hpp"
#include "DDraw.hpp"
#include "Function.hpp"
#include "../relive_lib/Error.hpp"
#include "Sys.hpp"
#include "PsxRender.hpp"
#include "Psx.hpp"
#include "TouchController.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/SoftwareRenderer.hpp"
#include "Renderer/DirectX9Renderer.hpp"
#include "../AliveLibCommon/FatalError.hpp"

bool sVGA_own_surfaces_BD0BFA = false;
bool sVGA_Inited_BC0BB8 = false;
bool sVGA_IsWindowMode_BD0BF8 = false;

Bitmap sVGA_Bmp0_BD0BD0 = {};
Bitmap sVGA_bmp_primary_BD2A20 = {};
Bitmap sVGA_bmp_back_buffer_BD2A40 = {};


s8 sVGA_BD0BBC = 0;
s8 sVGA_bpp_BD0BF9 = 0;
u16 sVGA_height_BD0BEC = 0;
u16 sVGA_width_BD0BC4 = 0;

s32 sbVga_LockedType_BD0BF0 = 0; // TODO: Enum
HDC sVga_HDC_BD0BC8 = 0;
s32 sVga_LockPType_BD0BC0 = 0;
LPVOID sVgaLockBuffer_BD0BF4 = 0;

bool s_VGA_KeepAspectRatio = true;
bool s_VGA_FilterScreen = false;

s32 VGA_FullScreenSet_4F31F0(bool /*bFullScreen*/)
{
    //  
    LOG_INFO("Stub"); // Can't be empty func otherwise NOT_IMPLEMENT'ed searcher will look into the next function
    return 0;
}

void VGA_Shutdown_4F3170()
{
    IRenderer::GetRenderer()->Destroy();
    IRenderer::FreeRenderer();

    sVGA_Inited_BC0BB8 = false;

    memset(&sVGA_bmp_primary_BD2A20, 0, sizeof(sVGA_bmp_primary_BD2A20));
    memset(&sVGA_bmp_back_buffer_BD2A40, 0, sizeof(sVGA_bmp_back_buffer_BD2A40));
}

bool VGA_IsWindowMode_4F31E0()
{
    return sVGA_IsWindowMode_BD0BF8;
}

Bitmap* VGA_GetBitmap_4F3F00()
{
    return &sVGA_bmp_primary_BD2A20;
}

void VGA_CopyToFront(RECT* pRect)
{
    SDL_Rect copyRect = {};
    if (pRect)
    {
        copyRect = {
            pRect->left,
            pRect->top,
            pRect->right,
            pRect->bottom};
    }

    SDL_Rect* pCopyRect = pRect ? &copyRect : nullptr;

    s32 w = 0;
    s32 h = 0;
    IRenderer::GetRenderer()->OutputSize(&w, &h);

    s32 renderedWidth = w;
    s32 renderedHeight = h;

    if (s_VGA_KeepAspectRatio)
    {
        if (3 * w > 4 * h)
        {
            renderedWidth = h * 4 / 3;
        }
        else
        {
            renderedHeight = w * 3 / 4;
        }
    }

    SDL_Rect dst = {};
    if (pCopyRect)
    {
        // Make sure our screen shake also sizes with the window.
        s32 screenShakeOffsetX = static_cast<s32>(sScreenXOffSet_BD30E4 * (renderedWidth / 640.0f));
        s32 screenShakeOffsetY = static_cast<s32>(sScreenYOffset_BD30A4 * (renderedHeight / 480.0f));

        dst.x = screenShakeOffsetX + ((w - renderedWidth) / 2);
        dst.y = screenShakeOffsetY + ((h - renderedHeight) / 2);
        dst.w = renderedWidth;
        dst.h = renderedHeight;
    }
    else
    {
        dst.x = (w - renderedWidth) / 2;
        dst.y = (h - renderedHeight) / 2;
        dst.w = renderedWidth;
        dst.h = renderedHeight;
    }

    IRenderer::GetRenderer()->Clear(0, 0, 0);
    IRenderer::GetRenderer()->BltBackBuffer(pCopyRect, &dst);
    IRenderer::GetRenderer()->EndFrame();
}

s32 VGA_DisplaySet_4F32C0(u16 width, u16 height, u8 bpp, u8 backbufferCount, TSurfaceType** ppSurface)
{
    // TODO: Window sub classing for VGA_WindowSubClass_4F2F50 removed as it only exists to support 8 bpp mode.

    if (sVGA_Inited_BC0BB8)
    {
        VGA_Shutdown_4F3170();
    }

    if (ppSurface)
    {
        sVGA_own_surfaces_BD0BFA = false;
        backbufferCount = 1;
    }
    else
    {
        sVGA_own_surfaces_BD0BFA = true;
    }

    sVGA_BD0BBC = backbufferCount;
    sVGA_bpp_BD0BF9 = bpp;
    memset(&sVGA_bmp_primary_BD2A20, 0, sizeof(sVGA_bmp_primary_BD2A20));
    memset(&sVGA_bmp_back_buffer_BD2A40, 0, sizeof(sVGA_bmp_back_buffer_BD2A40));
    sVGA_height_BD0BEC = height;
    sVGA_width_BD0BC4 = width;

    if (!sVGA_IsWindowMode_BD0BF8)
    {
        /*
        RECT rect = {};
        ::SetWindowPos(Sys_GetHWnd(), HWND_TOPMOST, 0, 0, width, height, 0x204u); // TODO: SDK constants
        ::GetClientRect(Sys_GetHWnd(), &rect);
        if (width != rect.right || height != rect.bottom)
        {
            ::SetWindowPos(Sys_GetHWnd(), HWND_TOPMOST, 0, 0, width - rect.right + width, height - rect.bottom + height, 0x204u); // TODO: SDK constants
        }
        */
    }

    if (sVGA_own_surfaces_BD0BFA /*|| DD_Init_4F0840(backbufferCount)*/)
    {
        // Create primary surface
        sVGA_bmp_primary_BD2A20.field_0_pSurface = SDL_CreateRGBSurface(0, width, height, bpp, 0x7c00, 0x03e0, 0x001f, 0x0); // TODO
                                                                                                                             //        sVGA_bmp_primary_BD2A20.field_0_pSurface = SDL_GetWindowSurface(Sys_GetHWnd());

        sVGA_bmp_primary_BD2A20.field_8_width = width;
        sVGA_bmp_primary_BD2A20.field_10_locked_pitch = sVGA_bmp_primary_BD2A20.field_0_pSurface->pitch; // TODO: Probably wrong ?
        sVGA_bmp_primary_BD2A20.field_C_height = height;
        sVGA_bmp_primary_BD2A20.field_14_bpp = static_cast<s8>(bpp);
        sVGA_bmp_primary_BD2A20.field_18_create_flags = 2;

        // TODO: Create back buffer surface

        /*
        memcpy(&sVGA_bmp_back_buffer_BD2A40, &sVGA_bmp_primary_BD2A20, sizeof(sVGA_bmp_back_buffer_BD2A40));
        sVGA_bmp_back_buffer_BD2A40.field_0_pSurface = sDD_surface_backbuffer_BBC3CC;
        */

        sVGA_Inited_BC0BB8 = 1;

        //IRenderer::CreateRenderer(IRenderer::Renderers::DirectX9);

    //#if RENDERER_OPENGL
        IRenderer::CreateRenderer(IRenderer::Renderers::OpenGL);
    //#else
    //    IRenderer::CreateRenderer(IRenderer::Renderers::DirectX9);
    //#endif

        if (!IRenderer::GetRenderer()->Create(Sys_GetHWnd()))
        {
            LOG_ERROR("Render create failed " << SDL_GetError());
            ALIVE_FATAL("Render create failed");
        }

        IRenderer::GetRenderer()->Clear(0, 0, 0);

        switch (sVGA_bmp_primary_BD2A20.field_0_pSurface->format->BitsPerPixel)
        {
            case 1u:
                sVGA_bmp_primary_BD2A20.field_15_pixel_format = 1;
                return 0;
            case 2u:
                sVGA_bmp_primary_BD2A20.field_15_pixel_format = 2;
                return 0;
            case 4u:
                sVGA_bmp_primary_BD2A20.field_15_pixel_format = 4;
                return 0;
            case 8u:
                sVGA_bmp_primary_BD2A20.field_15_pixel_format = 8;
                return 0;
            case 15u:
            case 16u:
                sVGA_bmp_primary_BD2A20.field_15_pixel_format = 16;
                return 0;
            case 32u:
                sVGA_bmp_primary_BD2A20.field_15_pixel_format = 32;
                return 0;
            default:
                break;
        }
    }


    return 0;
}

void VGA_BuffUnlockPtr_4F2FB0()
{
    // TODO
}

LPVOID VGA_BuffLockPtr_4F30A0(s32 /*always3*/)
{
    // TODO
    return nullptr;
}
