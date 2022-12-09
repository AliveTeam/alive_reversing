#include "stdafx.h"
#include "VGA.hpp"
#include "Sys.hpp"
#include "Renderer/IRenderer.hpp"

static bool sbRendererCreated = false;

void VGA_Shutdown()
{
    IRenderer::FreeRenderer();

    sbRendererCreated = false;
}

void VGA_EndFrame()
{
    IRenderer::GetRenderer()->Clear(0, 0, 0);
    IRenderer::GetRenderer()->EndFrame();
}

void VGA_CreateRenderer()
{
    if (sbRendererCreated)
    {
        VGA_Shutdown();
    }

    //IRenderer::CreateRenderer(IRenderer::Renderers::Vulkan, Sys_GetHWnd());
    //IRenderer::CreateRenderer(IRenderer::Renderers::DirectX9, Sys_GetHWnd());
    IRenderer::CreateRenderer(IRenderer::Renderers::OpenGL, Sys_GetHWnd());
    IRenderer::GetRenderer()->Clear(0, 0, 0);
    sbRendererCreated = true;
}
