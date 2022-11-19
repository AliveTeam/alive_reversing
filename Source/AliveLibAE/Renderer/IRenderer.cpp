#include "../stdafx.h"
#include "IRenderer.hpp"
#include "SoftwareRenderer.hpp"
#include "DirectX9Renderer.hpp"
#include "OpenGL3/OpenGLRenderer.hpp"

#include "../AliveLibCommon/FatalError.hpp"
#include "../AliveLibCommon/Sys_common.hpp"

static IRenderer* gRenderer = nullptr;

IRenderer* IRenderer::GetRenderer()
{
    return gRenderer;
}

void IRenderer::CreateRenderer(Renderers type, TWindowHandleType window)
{
    if (gRenderer)
    {
        ALIVE_FATAL("Renderer already created");
    }

    switch (type)
    {

#if SDL_VERTEX_IS_SUPPORTED
        case Renderers::Software:
            gRenderer = new SoftwareRenderer(window);
            break;
#endif

        case Renderers::OpenGL:
            gRenderer = new OpenGLRenderer(window);
            break;

#ifdef _WIN32
        // Windows only
        case Renderers::DirectX9:
            gRenderer = new DirectX9Renderer(window);
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
