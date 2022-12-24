#include "../stdafx.h"
#include "IRenderer.hpp"
#include "DirectX9/DirectX9Renderer.hpp"
#include "OpenGL3/OpenGLRenderer.hpp"
#include "Vulkan/VulkanRenderer.hpp"

#include "../../relive_lib/FatalError.hpp"
#include "../../relive_lib/Sys.hpp"

static IRenderer* gRenderer = nullptr;

IRenderer* IRenderer::GetRenderer()
{
    return gRenderer;
}

template<typename T>
static void MakeRenderer(const std::string& windowTitle, u32 windowAttributes)
{
    try
    {
        if (Sys_WindowClass_Register(windowTitle.c_str(), 32, 64, 640, 480, windowAttributes))
        {
            gRenderer = new T(Sys_GetHWnd());
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Failed to create renderer [%s]", e.what());
        Sys_DestroyWindow();
    }
}

static void AddRenderer(std::vector<IRenderer::Renderers>& renderers, IRenderer::Renderers toAdd)
{
    for (auto r : renderers)
    {
        if (r == toAdd)
        {
            return;
        }
    }
    renderers.emplace_back(toAdd);
}

bool IRenderer::CreateRenderer(Renderers type, const std::string& windowTitle)
{
    if (gRenderer)
    {
        ALIVE_FATAL("Renderer already created");
    }

    std::vector<Renderers> creationOrder{type};
    AddRenderer(creationOrder, Renderers::Vulkan);
#ifdef _WIN32
    AddRenderer(creationOrder, Renderers::DirectX9);
#endif
    AddRenderer(creationOrder, Renderers::OpenGL);

    for (Renderers typeToCreate : creationOrder)
    {
        switch (typeToCreate)
        {
            case Renderers::OpenGL:
                LOG_INFO("Create OpenGL");
                MakeRenderer<OpenGLRenderer>(windowTitle + " [OpenGL3]", SDL_WINDOW_OPENGL);
                break;

            case Renderers::Vulkan:
                LOG_INFO("Create Vulkan");
                MakeRenderer<VulkanRenderer>(windowTitle + " [Vulkan]", SDL_WINDOW_VULKAN);
                break;

#ifdef _WIN32
            // Windows only
            case Renderers::DirectX9:
                LOG_INFO("Create DirectX9");
                MakeRenderer<DirectX9Renderer>(windowTitle + " [DirectX9]", 0);
                break;
#endif

            default:
                ALIVE_FATAL("Unknown or unsupported renderer type");
                break;
        }

        if (gRenderer)
        {
            break;
        }
    }
    return gRenderer != nullptr;
}

void IRenderer::FreeRenderer()
{
    delete gRenderer;
    gRenderer = nullptr;
}

void IRenderer::SetScreenOffset(Prim_ScreenOffset& offset)
{
    mOffsetX = offset.field_C_xoff;
    mOffsetY = offset.field_E_yoff;
}

SDL_Rect IRenderer::GetTargetDrawRect()
{
    SDL_Rect rect = {};

    s32 wndWidth = 0;
    s32 wndHeight = 0;

    SDL_GetWindowSize(mWindow, &wndWidth, &wndHeight);

    // Calculate the draw size, aspect ratio dealt with here
    rect.w = wndWidth;
    rect.h = wndHeight;

    if (mKeepAspectRatio)
    {
        if (3 * wndWidth > 4 * wndHeight)
        {
            rect.w = (wndHeight * 4) / 3;
        }
        else
        {
            rect.h = (wndWidth * 3) / 4;
        }
    }

    // Calculate any screen shake
    const s32 shakeX = static_cast<s32>(mOffsetX * (rect.w / 640.0f));
    const s32 shakeY = static_cast<s32>(mOffsetY * (rect.h / 480.0f));

    rect.x = shakeX + ((wndWidth - rect.w) / 2);
    rect.y = shakeY + ((wndHeight - rect.h) / 2);

    return rect;
}

IRenderer::Quad2D IRenderer::LineToQuad(const Point2D p1, const Point2D p2)
{
    constexpr f32 halfPi = 1.57f;
    constexpr f32 halfThickness = 0.5f;

    f32 x0 = (f32) p1.x;
    f32 y0 = (f32) p1.y;

    f32 x1 = (f32) p2.x;
    f32 y1 = (f32) p2.y;

    f32 dx = x1 - x0;
    f32 dy = y1 - y0;

    f32 normal = halfPi - std::atan(dy / dx);

    f32 dxTarget = halfThickness * std::cos(normal);
    f32 dyTarget = halfThickness * std::sin(normal);

    f32 finalX0 = x0 + dxTarget;
    f32 finalY0 = y0 - dyTarget;

    f32 finalX1 = x0 - dxTarget;
    f32 finalY1 = y0 + dyTarget;

    f32 finalX2 = x1 + dxTarget;
    f32 finalY2 = y1 - dyTarget;

    f32 finalX3 = x1 - dxTarget;
    f32 finalY3 = y1 + dyTarget;

    return {
        {
            { finalX0, finalY0 },
            { finalX1, finalY1 },
            { finalX2, finalY2 },
            { finalX3, finalY3 }
        }
    };
}