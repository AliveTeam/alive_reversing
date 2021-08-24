#include "stdafx.h"
#include "IRenderer.hpp"
#include "SoftwareRenderer.hpp"
#include "OpenGLRenderer.hpp"
#include "DirectX9Renderer.hpp"

IRenderer* gRenderer = nullptr;

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

    switch (type)
    {
        case Renderers::Software:
            gRenderer = new SoftwareRenderer();
            break;

        case Renderers::OpenGL:
            gRenderer = new OpenGLRenderer();
            break;

#ifdef _WIN32
        // Windows only
        case Renderers::DirectX9:
            gRenderer = new DirectX9Renderer();
            break;
#endif

        default:
            ALIVE_FATAL("Unknown or unsupported renderer type");
            break;
    }
}

void IRenderer::FreeRenderer()
{
    delete gRenderer;
    gRenderer = nullptr;
}
