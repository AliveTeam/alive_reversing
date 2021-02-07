#include "stdafx.h"
#include "IRenderer.hpp"
#include "SoftwareRenderer.hpp"
#include "DirectX9Renderer.hpp"

static IRenderer* gRenderer = nullptr;

IRenderer* IRenderer::GetRenderer()
{
    return gRenderer;
}

void IRenderer::CreateRenderer(Renderers type)
{
    if (gRenderer)
    {
        ALIVE_FATAL("Renderer already created");
    }

    if (type == Renderers::Software)
    {
        gRenderer = new SoftwareRenderer();
    }
#ifdef _WIN32
    else if (type == Renderers::DirectX9)
    {
        gRenderer = new DirectX9Renderer();
    }
#endif
    else
    {
        ALIVE_FATAL("Unknown renderer type");
    }
}

void IRenderer::FreeRenderer()
{
    delete gRenderer;
    gRenderer = nullptr;
}
