#include "stdafx.h"
#include "VGA.hpp"
#include "../relive_lib/Sys.hpp"
#include "../relive_lib/Renderer/IRenderer.hpp"
#include "../relive_lib/BaseGameAutoPlayer.hpp"

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

void VGA_CreateRenderer(const std::string& windowTitleBase)
{
    if (sbRendererCreated)
    {
        VGA_Shutdown();
    }

    std::string windowTitle = windowTitleBase;
    if (GetGameAutoPlayer().IsRecording())
    {
        windowTitle += " [Recording]";
    }
    else if (GetGameAutoPlayer().IsPlaying())
    {
        windowTitle += " [AutoPlay]";
    }

    //IRenderer::CreateRenderer(IRenderer::Renderers::Vulkan, windowTitle);
    //IRenderer::CreateRenderer(IRenderer::Renderers::DirectX9, windowTitle);
    IRenderer::CreateRenderer(IRenderer::Renderers::OpenGL, windowTitle);
    IRenderer::GetRenderer()->Clear(0, 0, 0);
    sbRendererCreated = true;
}
