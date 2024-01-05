#include "../../../relive_lib/Primitives.hpp"
#include "Sdl2Renderer.hpp"

Sdl2Renderer::Sdl2Renderer(TWindowHandleType window)
    : IRenderer(window)
{
    mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
    mRenderTargetSupported = SDL_RenderTargetSupported(mRenderer);
}

Sdl2Renderer::~Sdl2Renderer()
{
}

void Sdl2Renderer::Clear(u8 r, u8 g, u8 b)
{
    SDL_SetRenderDrawColor(mRenderer, r, g, b, 255);
    //SDL_RenderClear(mRenderer);
}

void Sdl2Renderer::Draw(const Prim_GasEffect& gasEffect)
{
    // TODO: Implement this
    const f32 x = static_cast<f32>(gasEffect.x);
    const f32 y = static_cast<f32>(gasEffect.y);
    const f32 w = static_cast<f32>(gasEffect.w);
    const f32 h = static_cast<f32>(gasEffect.h);

    //const f32 gasWidth = std::floor(static_cast<f32>(gasEffect.w - gasEffect.x) / 4);
    //const f32 gasHeight = std::floor(static_cast<f32>(gasEffect.h - gasEffect.y) / 2);

    SDL_Color c = { 255, 255, 0, 127 };

    std::vector<SDL_Vertex> gasVerts = {
        { {x, y}, c, { 0, 0 } },
        { {w, y}, c, { 0, 0 } },
        { {x, h}, c, { 0, 0 } },

        { {x, h}, c, { 0, 0 } },
        { {w, y}, c, { 0, 0 } },
        { {w, h}, c, { 0, 0 } },
    };

    ScaleVertices(gasVerts);

    SDL_RenderGeometry(mRenderer, NULL, gasVerts.data(), 6, NULL, 0);
}

void Sdl2Renderer::Draw(const Line_G2& line)
{
    // FIXME: Handle thickness, and colour correctly (aka use a quad)
    SDL_SetRenderDrawColor(mRenderer, line.R0(), line.G0(), line.B0(), 255);
    SDL_RenderDrawLine(mRenderer, line.X0(), line.Y0(), line.X1(), line.Y1());
}

void Sdl2Renderer::Draw(const Line_G4& line)
{
    // FIXME: Handle thickness, and colour correctly (aka use a quad)
    SDL_Point points[] = {
        { line.X0(), line.Y0() },
        { line.X1(), line.Y1() },
        { line.X2(), line.Y2() },
        { line.X3(), line.Y3() },
    };

    SDL_SetRenderDrawColor(mRenderer, line.R0(), line.G0(), line.B0(), 255);
    SDL_RenderDrawLines(mRenderer, points, 4);
}

void Sdl2Renderer::Draw(const Poly_G3& poly)
{
    // FIXME: Obviously unfinished (no blending, etc.)
    std::vector<SDL_Vertex> vertices = {
        { { static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()) }, { poly.R0(), poly.G0(), poly.B0(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()) }, { poly.R1(), poly.G1(), poly.B1(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()) }, { poly.R2(), poly.G2(), poly.B2(), 255 }, { 0, 0 } },
    };

    ScaleVertices(vertices);

    SDL_RenderGeometry(mRenderer, NULL, vertices.data(), 3, NULL, 0);
}

void Sdl2Renderer::Draw(const Poly_FT4& poly)
{
    // FIXME: Obviously unfinished (no blending, etc.)
    std::vector<SDL_Vertex> vertices = {
        { { static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()) }, { poly.R0(), poly.G0(), poly.B0(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()) }, { poly.R1(), poly.G1(), poly.B1(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()) }, { poly.R2(), poly.G2(), poly.B2(), 255 }, { 0, 0 } },

        { { static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()) }, { poly.R1(), poly.G1(), poly.B1(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()) }, { poly.R2(), poly.G2(), poly.B2(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()) }, { poly.R3(), poly.G3(), poly.B3(), 255 }, { 0, 0 } },
    };

    ScaleVertices(vertices);

    SDL_RenderGeometry(mRenderer, NULL, vertices.data(), 6, NULL, 0);
}

void Sdl2Renderer::Draw(const Poly_G4& poly)
{
    // FIXME: Obviously unfinished (no blending, etc.)
    std::vector<SDL_Vertex> vertices = {
        { { static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()) }, { poly.R0(), poly.G0(), poly.B0(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()) }, { poly.R1(), poly.G1(), poly.B1(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()) }, { poly.R2(), poly.G2(), poly.B2(), 255 }, { 0, 0 } },

        { { static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()) }, { poly.R1(), poly.G1(), poly.B1(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()) }, { poly.R2(), poly.G2(), poly.B2(), 255 }, { 0, 0 } },
        { { static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()) }, { poly.R3(), poly.G3(), poly.B3(), 255 }, { 0, 0 } },
    };

    ScaleVertices(vertices);

    SDL_RenderGeometry(mRenderer, NULL, vertices.data(), 6, NULL, 0);
}

void Sdl2Renderer::EndFrame()
{
    SDL_RenderPresent(mRenderer);
}

void Sdl2Renderer::SetClip(const Prim_ScissorRect& clipper)
{
    // FIXME: Implement this
    SDL_Rect rect;

    rect.x = clipper.mRect.x;
    rect.y = clipper.mRect.y;
    rect.w = clipper.mRect.w;
    rect.h = clipper.mRect.h;

    SDL_SetClipRect(SDL_GetWindowSurface(mWindow), &rect);
}

void Sdl2Renderer::StartFrame()
{
    IRenderer::StartFrame();

    mOffsetX = 0;
    mOffsetY = 0;
}

SDL_FPoint Sdl2Renderer::PointToViewport(const SDL_FPoint& point)
{
    if (mRenderTargetSupported && !mUseOriginalResolution)
    {
        return point;
    }

    SDL_Rect   wndRect     = GetTargetDrawRect();
    SDL_FPoint scaledPoint = {
        point.x * (wndRect.w / kPsxFramebufferWidth),
        point.y * (wndRect.h / kPsxFramebufferHeight)
    };

    // If there's no backbuffer, then drawing must use offsets immediately
    // rather than in EndFrame()
    if (!mRenderTargetSupported)
    {
        scaledPoint.x = scaledPoint.x + mOffsetX;
        scaledPoint.y = scaledPoint.y + mOffsetY;
    }

    return scaledPoint;
}

void Sdl2Renderer::ScaleVertices(std::vector<SDL_Vertex>& vertices)
{
    for (u8 i = 0; i < vertices.size(); i++)
    {
        vertices[i].position = PointToViewport(vertices[i].position);
    }
}
