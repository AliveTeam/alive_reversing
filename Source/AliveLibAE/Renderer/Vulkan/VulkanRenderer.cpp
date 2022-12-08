#include "../stdafx.h"
#include "VulkanRenderer.hpp"
#include "../../AliveLibCommon/FatalError.hpp"
#include "../../relive_lib/ResourceManagerWrapper.hpp"
#include "../../relive_lib/Animation.hpp"
#include "../Font.hpp"
#include <SDL_syswm.h>


// TODO: Copy pasted from DX/GL renderer
inline u16 GetTPageBlendMode(u16 tpage)
{
    return (tpage >> 4) & 3;
}

VulkanRenderer::VulkanRenderer(TWindowHandleType window)
  : IRenderer(window),
    mPaletteCache(256)
{

}

VulkanRenderer::~VulkanRenderer()
{
    TRACE_ENTRYEXIT;

    // TODO: Fix me, dtor can't call clear else pure call boom
    //mTextureCache.Clear();
}

void VulkanRenderer::Clear(u8 /*r*/, u8 /*g*/, u8 /*b*/)
{

}

void VulkanRenderer::StartFrame()
{
    if (!mFrameStarted)
    {
        mFrameStarted = true;

        // Set offsets for the screen (this is for the screen shake effect)
        mOffsetX = 0;
        mOffsetY = 0;

    }
}

void VulkanRenderer::EndFrame()
{
    if (mFrameStarted)
    {

        mFrameStarted = false;
    }

    // Always decrease resource lifetimes regardless of drawing to prevent
    // memory leaks
    //DecreaseResourceLifetimes();
}

void VulkanRenderer::SetTPage(u16 tPage)
{
    mGlobalTPage = tPage;
}

void VulkanRenderer::SetClip(Prim_PrimClipper&)
{
   
}

void VulkanRenderer::ToggleFilterScreen()
{
    // TODO
}

void VulkanRenderer::Draw(Prim_GasEffect& /*gasEffect*/)
{
    // TODO
}

void VulkanRenderer::Draw(Line_G2& /*line*/)
{
    // TODO
}

void VulkanRenderer::Draw(Line_G4& /*line*/)
{
    // TODO
}


void VulkanRenderer::Draw(Poly_G3&)
{
  
}

/*
void VulkanRenderer::SetupBlendMode(u16 blendMode)
{
    if ((TPageAbr) blendMode == TPageAbr::eBlend_2)
    {
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE));
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT));
    }
    else
    {
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE));
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA));
        //DX_VERIFY(mDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD));
    }
}*/

void VulkanRenderer::Draw(Poly_FT4&)
{

}

void VulkanRenderer::Draw(Poly_G4&)
{

}

/*
void VulkanRenderer::DecreaseResourceLifetimes()
{
    //mTextureCache.DecreaseResourceLifetimes();

    mPaletteCache.ResetUseFlags();
}
*/
