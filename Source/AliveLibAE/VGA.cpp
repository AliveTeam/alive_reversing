#include "stdafx.h"
#include "VGA.hpp"
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


s8 sVGA_BD0BBC = 0;
s8 sVGA_bpp_BD0BF9 = 0;
u16 sVGA_height_BD0BEC = 0;
u16 sVGA_width_BD0BC4 = 0;

s32 sbVga_LockedType_BD0BF0 = 0; // TODO: Enum
HDC sVga_HDC_BD0BC8 = 0;
s32 sVga_LockPType_BD0BC0 = 0;
LPVOID sVgaLockBuffer_BD0BF4 = 0;


void VGA_Shutdown_4F3170()
{
    IRenderer::GetRenderer()->Destroy();
    IRenderer::FreeRenderer();

    sVGA_Inited_BC0BB8 = false;
}

void VGA_CopyToFront(RECT* /*pRect*/)
{
    IRenderer::GetRenderer()->Clear(0, 0, 0);
    IRenderer::GetRenderer()->EndFrame();
}

s32 VGA_DisplaySet_4F32C0(u16 width, u16 height, u8 bpp, u8 backbufferCount)
{
    // TODO: Window sub classing for VGA_WindowSubClass_4F2F50 removed as it only exists to support 8 bpp mode.

    if (sVGA_Inited_BC0BB8)
    {
        VGA_Shutdown_4F3170();
    }

   
    sVGA_own_surfaces_BD0BFA = true;
    

    sVGA_BD0BBC = backbufferCount;
    sVGA_bpp_BD0BF9 = bpp;
    sVGA_height_BD0BEC = height;
    sVGA_width_BD0BC4 = width;

    if (sVGA_own_surfaces_BD0BFA)
    {
        sVGA_Inited_BC0BB8 = 1;

        //IRenderer::CreateRenderer(IRenderer::Renderers::DirectX9);
        IRenderer::CreateRenderer(IRenderer::Renderers::OpenGL);

        if (!IRenderer::GetRenderer()->Create(Sys_GetHWnd()))
        {
            LOG_ERROR("Render create failed " << SDL_GetError());
            ALIVE_FATAL("Render create failed");
        }

        IRenderer::GetRenderer()->Clear(0, 0, 0);
    }

    return 0;
}
