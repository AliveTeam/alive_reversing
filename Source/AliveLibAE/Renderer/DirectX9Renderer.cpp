#include "stdafx.h"
#include "DirectX9Renderer.hpp"

#ifdef _WIN32

#undef DIRECT3D_VERSION
#define DIRECT3D_VERSION         0x0900
#include <d3d9.h>



void DirectX9Renderer::Destroy()
{
    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
}

bool DirectX9Renderer::Create(TWindowHandleType window)
{
    // Find the directx9 driver
    const int numDrivers = SDL_GetNumRenderDrivers();
    if (numDrivers < 0)
    {
        LOG_ERROR("Failed to get driver count " << SDL_GetError());
    }

    LOG_INFO("Got " << numDrivers << " drivers");

    int index = -1;
    for (int i = 0; i < numDrivers; i++)
    {
        SDL_RendererInfo info = {};
        if (SDL_GetRenderDriverInfo(i, &info) < 0)
        {
            LOG_WARNING("Failed to get render " << i << " info " << SDL_GetError());
        }
        else
        {
            LOG_INFO(i << " name " << info.name);
            if (strstr(info.name, "direct3d"))
            {
                index = i;
                break;
            }
        }
    }

    if (index == -1)
    {
        LOG_WARNING("DirectX9 SDL2 driver not found");
        return false;
    }

    mRenderer = SDL_CreateRenderer(window, index, SDL_RENDERER_ACCELERATED);
    if (!mRenderer)
    {
        LOG_ERROR("Failed to create renderer " << SDL_GetError());
        return false;
    }

    mDevice = SDL_RenderGetD3D9Device(mRenderer);
    if (!mDevice)
    {
        Destroy();

        LOG_ERROR("Couldnt get DirectX9 device " << SDL_GetError());
        return false;
    }

    return true;
}

void DirectX9Renderer::Clear(BYTE r, BYTE g, BYTE b)
{
    mDevice->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(r, g, b), 1.0f, 0);
}

void DirectX9Renderer::StartFrame(int /*xOff*/, int /*yOff*/)
{

}

void DirectX9Renderer::EndFrame()
{
    SDL_RenderPresent(mRenderer);
}

void DirectX9Renderer::BltBackBuffer(const SDL_Rect* /*pCopyRect*/, const SDL_Rect* /*pDst*/)
{

}

void DirectX9Renderer::OutputSize(int* w, int* h)
{
    SDL_GetRendererOutputSize(mRenderer, w, h);
}

bool DirectX9Renderer::UpdateBackBuffer(const void* /*pPixels*/, int /*pitch*/)
{
    return true;
}

void DirectX9Renderer::CreateBackBuffer(bool /*filter*/, int /*format*/, int /*w*/, int /*h*/)
{

}

void DirectX9Renderer::SetTPage(short /*tPage*/)
{

}

void DirectX9Renderer::SetClip(Prim_PrimClipper& /*clipper*/)
{

}

void DirectX9Renderer::Draw(Prim_Sprt& /*sprt*/)
{

}

void DirectX9Renderer::Draw(Prim_GasEffect& /*gasEffect*/)
{

}

void DirectX9Renderer::Draw(Prim_Tile& /*tile*/)
{

}

void DirectX9Renderer::Draw(Line_F2& /*line*/)
{

}

void DirectX9Renderer::Draw(Line_G2& /*line*/)
{

}

void DirectX9Renderer::Draw(Line_G4& /*line*/)
{

}

void DirectX9Renderer::Draw(Poly_F3& /*poly*/)
{

}

void DirectX9Renderer::Draw(Poly_G3& /*poly*/)
{

}

void DirectX9Renderer::Draw(Poly_F4& /*poly*/)
{

}

void DirectX9Renderer::Draw(Poly_FT4& /*poly*/)
{

}

void DirectX9Renderer::Draw(Poly_G4& /*poly*/)
{

}

void DirectX9Renderer::Upload(BitDepth /*bitDepth*/, const PSX_RECT& /*rect*/, const BYTE* /*pPixels*/)
{

}

#endif
