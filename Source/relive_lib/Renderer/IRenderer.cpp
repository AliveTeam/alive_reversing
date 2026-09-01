#include "../stdafx.h"
#include "IRenderer.hpp"
#include "OpenGL3/OpenGLRenderer.hpp"
#include "SDL2/Sdl2Renderer.hpp"

#include "../../relive_lib/FatalError.hpp"
#include "../../relive_lib/Sys.hpp"

#include <cmath>

static IRenderer* gRenderer = nullptr;

IRenderer* IRenderer::GetRenderer()
{
    return gRenderer;
}

template<typename T>
static void MakeRenderer(const std::string& windowTitle, u32 windowAttributes)
{
    TRACE_ENTRYEXIT;
    try
    {
        if (Sys_WindowClass_Register(windowTitle.c_str(), 32, 64, 640, 480, windowAttributes))
        {
            gRenderer = new T(Sys_GetHWnd());
        }
        else
        {
            LOG_INFO("No window, skip creating instance");
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
    AddRenderer(creationOrder, Renderers::Sdl3);
    AddRenderer(creationOrder, Renderers::OpenGL);

    for (Renderers typeToCreate : creationOrder)
    {
        switch (typeToCreate)
        {
            case Renderers::Sdl3:
                LOG_INFO("Create SDL3 renderer");
                MakeRenderer<Sdl2Renderer>(windowTitle + " [SDL3]", 0);
                break;

            case Renderers::OpenGL:
                LOG_INFO("Create OpenGL renderer");
                MakeRenderer<OpenGLRenderer>(windowTitle + " [OpenGL3]", SDL_WINDOW_OPENGL);
                break;

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

void IRenderer::SetScreenOffset(const Prim_ScreenOffset& offset)
{
    mOffsetX = offset.field_C_xoff;
    mOffsetY = offset.field_E_yoff;
}

SDL_Rect IRenderer::GetFramebufferRect()
{
    SDL_Rect rect = {};

    s32 desiredW = kPsxFramebufferWidth;
    s32 desiredH = kPsxFramebufferHeight;

    if (!mUseOriginalResolution)
    {
        // If we're maintaining aspect ratio, then the framebuffer needs
        // to be equal to the size of the rect otherwise the result will
        // be a warped image
        if (mKeepAspectRatio)
        {
            SDL_Rect r = GetTargetDrawRect();

            desiredW = r.w;
            desiredH = r.h;
        }
        else
        {
            SDL_GetWindowSizeInPixels(mWindow, &desiredW, &desiredH);
        }
    }

    rect.w = desiredW;
    rect.h = desiredH;

    return rect;
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

IRenderer::Quad2D IRenderer::LineToQuad(const Point2D& p1, const Point2D& p2)
{
    constexpr f32 halfPi = 1.57f;
    constexpr f32 halfThickness = 0.5f;

    // Always orient the line to be drawn left to right, so don't need to faff
    // with trig more than necessary
    Point2D leftPoint = p1;
    Point2D rightPoint = p2;

    if (p1.x > p2.x)
    {
        leftPoint = p2;
        rightPoint = p1;
    }

    // Our trig expands the line out in both directions, so the actual line
    // itself is 'centered' - here we push the line out by half the thickness
    // so that the expansion lands on, or close to, whole number pixel values
    f32 x0 = (f32) leftPoint.x + halfThickness;
    f32 y0 = (f32) leftPoint.y + halfThickness;

    f32 x1 = (f32) rightPoint.x + halfThickness;
    f32 y1 = (f32) rightPoint.y + halfThickness;

    // Our trig here, we expand the line into a quad, the normals are for the
    // thickness along the line, and the tangent is used for the thickness
    // on either end of the line
    f32 dx = x1 - x0;
    f32 dy = y1 - y0;

    f32 angle = std::atan(dy / dx);
    f32 normal = halfPi - angle;

    f32 dxTargetTangent = halfThickness * std::cos(angle);
    f32 dyTargetTangent = halfThickness * std::sin(angle);
    f32 dxTargetNormal = halfThickness * std::cos(normal);
    f32 dyTargetNormal = halfThickness * std::sin(normal);

    f32 finalX0 = x0 + dxTargetNormal - dxTargetTangent;
    f32 finalY0 = y0 - dyTargetNormal - dyTargetTangent;

    f32 finalX1 = x0 - dxTargetNormal - dxTargetTangent;
    f32 finalY1 = y0 + dyTargetNormal - dyTargetTangent;

    f32 finalX2 = x1 + dxTargetNormal + dxTargetTangent;
    f32 finalY2 = y1 - dyTargetNormal + dyTargetTangent;

    f32 finalX3 = x1 - dxTargetNormal + dxTargetTangent;
    f32 finalY3 = y1 + dyTargetNormal + dyTargetTangent;

    // The quad is like so (original line in the center):
    //
    // xy0 ---------- xy2
    //  | \__________/ |
    //  | /          \ |
    // xy1 ---------- xy3
    return {
        {
            { finalX0, finalY0 },
            { finalX1, finalY1 },
            { finalX2, finalY2 },
            { finalX3, finalY3 }
        }
    };
}
