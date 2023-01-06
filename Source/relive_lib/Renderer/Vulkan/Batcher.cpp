#include "Batcher.hpp"
#include "VulkanRenderer.hpp"
#include "../../Primitives.hpp"
#include "../../FG1.hpp"
#include "../../Animation.hpp"
#include "../../../AliveLibAE/Font.hpp"

// TODO: Copy pasted from GL renderer
inline u16 GetTPageBlendMode(u16 tpage)
{
    return (tpage >> 4) & 3;
}

template <typename TextureType, typename RenderBatchType, typename VertexType>
void Batcher<TextureType, RenderBatchType, VertexType>::PushQuad(IRenderer::PsxDrawMode drawMode, const Batcher::QuadVerts& verts, const Batcher::QuadUvs& uvs, const Batcher::RGB* rgb, u32 palIdx, u32 isShaded, u32 blendMode, u32 isSemiTrans, u32 textureIdx)
{
    mVertices.push_back({{verts.x0, verts.y0}, {rgb[0].r, rgb[0].g, rgb[0].b}, {uvs.u0, uvs.v0}, textureIdx, palIdx, drawMode, isShaded, blendMode, isSemiTrans});
    mVertices.push_back({{verts.x1, verts.y1}, {rgb[1].r, rgb[1].g, rgb[1].b}, {uvs.u1, uvs.v0}, textureIdx, palIdx, drawMode, isShaded, blendMode, isSemiTrans});
    mVertices.push_back({{verts.x2, verts.y2}, {rgb[2].r, rgb[2].g, rgb[2].b}, {uvs.u0, uvs.v1}, textureIdx, palIdx, drawMode, isShaded, blendMode, isSemiTrans});
    mVertices.push_back({{verts.x3, verts.y3}, {rgb[3].r, rgb[3].g, rgb[3].b}, {uvs.u1, uvs.v1}, textureIdx, palIdx, drawMode, isShaded, blendMode, isSemiTrans});

    mIndices.emplace_back((u16) (mIndexBufferIndex + 1));
    mIndices.emplace_back((u16) (mIndexBufferIndex + 0));
    mIndices.emplace_back((u16) (mIndexBufferIndex + 3));

    mIndices.emplace_back((u16) (mIndexBufferIndex + 3));
    mIndices.emplace_back((u16) (mIndexBufferIndex + 0));
    mIndices.emplace_back((u16) (mIndexBufferIndex + 2));

    mIndexBufferIndex += 4;

    mConstructingBatch.mNumTrisToDraw += 2;
    mBatchInProgress = true;
}

template <typename TextureType, typename RenderBatchType, typename VertexType>
void Batcher<TextureType, RenderBatchType, VertexType>::NewBatch()
{
    mBatches.push_back(mConstructingBatch);
    mConstructingBatch = {};
    mBatchInProgress = false;
}

template <typename TextureType, typename RenderBatchType, typename VertexType>
void Batcher<TextureType, RenderBatchType, VertexType>::SetScissor(const SDL_Rect& scissor)
{
    mConstructingBatch.mScissor = scissor;
}

template <typename TextureType, typename RenderBatchType, typename VertexType>
typename Batcher<TextureType, RenderBatchType, VertexType>::RenderBatch& Batcher<TextureType, RenderBatchType, VertexType>::PushFG1(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture)
{
    const QuadUvs uvs = {
        0.0f, 0.0f,
        1.0f, 1.0f};

    const QuadVerts verts = {
        static_cast<f32>(poly.mBase.vert.x),
        static_cast<f32>(poly.mBase.vert.y),
        static_cast<f32>(poly.mVerts[0].mVert.x),
        static_cast<f32>(poly.mVerts[0].mVert.y),
        static_cast<f32>(poly.mVerts[1].mVert.x),
        static_cast<f32>(poly.mVerts[1].mVert.y),
        static_cast<f32>(poly.mVerts[2].mVert.x),
        static_cast<f32>(poly.mVerts[2].mVert.y),
    };

    const u8 r = 255;
    const u8 g = 255;
    const u8 b = 255;

    const RGB rgbs[4] = {
        {r, g, b},
        {r, g, b},
        {r, g, b},
        {r, g, b},
    };

    const u32 textureIdx = mConstructingBatch.TextureIdxForId(poly.mFg1->mUniqueId.Id());
    PushQuad(IRenderer::PsxDrawMode::FG1, verts, uvs, rgbs, 0, 0, 0, 0, textureIdx);
    if (mConstructingBatch.AddTexture(poly.mFg1->mUniqueId.Id()))
    {
        mBatchTextures.emplace_back(texture);
    }

    RenderBatch& addedTo = mConstructingBatch;

    // Over the texture limit or changed to/from subtractive blending
    const bool bNewBatch = (mConstructingBatch.mTexturesInBatch == kTextureBatchSize);
    if (bNewBatch)
    {
        NewBatch();
    }

    return addedTo;
}

template <typename TextureType, typename RenderBatchType, typename VertexType>
typename Batcher<TextureType, RenderBatchType, VertexType>::RenderBatch& Batcher<TextureType, RenderBatchType, VertexType>::PushCAM(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture)
{
    const QuadUvs uvs = {
        0.0f, 0.0f,
        1.0f, 1.0f};

    const QuadVerts verts = {
        static_cast<f32>(poly.mBase.vert.x),
        static_cast<f32>(poly.mBase.vert.y),
        static_cast<f32>(poly.mVerts[0].mVert.x),
        static_cast<f32>(poly.mVerts[0].mVert.y),
        static_cast<f32>(poly.mVerts[1].mVert.x),
        static_cast<f32>(poly.mVerts[1].mVert.y),
        static_cast<f32>(poly.mVerts[2].mVert.x),
        static_cast<f32>(poly.mVerts[2].mVert.y),
    };

    const u8 r = 255;
    const u8 g = 255;
    const u8 b = 255;

    const RGB rgbs[4] = {
        {r, g, b},
        {r, g, b},
        {r, g, b},
        {r, g, b},
    };

    PushQuad(IRenderer::PsxDrawMode::Camera, verts, uvs, rgbs, 0, 0, 0, 0, 0);
    mCamTexture = texture;

    RenderBatch& addedTo = mConstructingBatch;

    // Over the texture limit or changed to/from subtractive blending
    const bool bNewBatch = (mConstructingBatch.mTexturesInBatch == kTextureBatchSize);
    if (bNewBatch)
    {
        NewBatch();
    }

    return addedTo;
}

template <typename TextureType, typename RenderBatchType, typename VertexType>
typename Batcher<TextureType, RenderBatchType, VertexType>::RenderBatch& Batcher<TextureType, RenderBatchType, VertexType>::PushFont(const Poly_FT4& poly, u32 palIndex, std::shared_ptr<TextureType>& texture)
{
    FontContext* fontRes = poly.mFont;

    const u8 r = poly.mBase.header.rgb_code.r;
    const u8 g = poly.mBase.header.rgb_code.g;
    const u8 b = poly.mBase.header.rgb_code.b;

    const RGB rgbs[4] = {
        {r, g, b},
        {r, g, b},
        {r, g, b},
        {r, g, b},
    };

    const u32 isShaded = GetPolyIsShaded(&poly) ? 1 : 0;
    const u32 isSemiTrans = GetPolyIsSemiTrans(&poly) ? 1 : 0;
    const u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

    // Changing to or from mode 2
    if (mConstructingBatch.mBlendMode != 2 && blendMode == 2 || mConstructingBatch.mBlendMode == 2 && blendMode != 2)
    {
        NewBatch();
    }
    mConstructingBatch.mBlendMode = blendMode;

    auto pTga = fontRes->mFntResource.mTgaPtr;

    f32 u0 = U0(&poly) / (f32) pTga->mWidth;
    f32 v0 = V0(&poly) / (f32) pTga->mHeight;

    f32 u1 = U3(&poly) / (f32) pTga->mWidth;
    f32 v1 = V3(&poly) / (f32) pTga->mHeight;

    const u32 textureIdx = mConstructingBatch.TextureIdxForId(fontRes->mFntResource.mUniqueId.Id());

    const QuadUvs uvs = {
        u0, v0,
        u1, v1};

    const QuadVerts verts = {
        static_cast<f32>(poly.mBase.vert.x),
        static_cast<f32>(poly.mBase.vert.y),
        static_cast<f32>(poly.mVerts[0].mVert.x),
        static_cast<f32>(poly.mVerts[0].mVert.y),
        static_cast<f32>(poly.mVerts[1].mVert.x),
        static_cast<f32>(poly.mVerts[1].mVert.y),
        static_cast<f32>(poly.mVerts[2].mVert.x),
        static_cast<f32>(poly.mVerts[2].mVert.y),
    };

    PushQuad(IRenderer::PsxDrawMode::DefaultFT4, verts, uvs, rgbs, palIndex, isShaded, blendMode, isSemiTrans, textureIdx);

    if (mConstructingBatch.AddTexture(fontRes->mFntResource.mUniqueId.Id()))
    {
        mBatchTextures.emplace_back(texture);
    }

    RenderBatch& addedTo = mConstructingBatch;

    // Over the texture limit or changed to/from subtractive blending
    const bool bNewBatch = (mConstructingBatch.mTexturesInBatch == kTextureBatchSize);
    if (bNewBatch)
    {
        NewBatch();
    }

    return addedTo;
}

template <typename TextureType, typename RenderBatchType, typename VertexType>
typename Batcher<TextureType, RenderBatchType, VertexType>::RenderBatch& Batcher<TextureType, RenderBatchType, VertexType>::PushAnim(const Poly_FT4& poly, u32 palIndex, std::shared_ptr<TextureType>& texture)
{
    AnimResource& animRes = poly.mAnim->mAnimRes;

    const u8 r = poly.mBase.header.rgb_code.r;
    const u8 g = poly.mBase.header.rgb_code.g;
    const u8 b = poly.mBase.header.rgb_code.b;

    const RGB rgbs[4] = {
        {r, g, b},
        {r, g, b},
        {r, g, b},
        {r, g, b},
    };

    const u32 isShaded = GetPolyIsShaded(&poly) ? 1 : 0;
    const u32 isSemiTrans = GetPolyIsSemiTrans(&poly) ? 1 : 0;
    const u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

    // Changing to or from mode 2
    if (mConstructingBatch.mBlendMode != 2 && blendMode == 2 || mConstructingBatch.mBlendMode == 2 && blendMode != 2)
    {
        NewBatch();
    }
    mConstructingBatch.mBlendMode = blendMode;

    const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);

    auto pTga = animRes.mTgaPtr;

    f32 u0 = (static_cast<float>(pHeader->mSpriteSheetX) / pTga->mWidth);
    f32 v0 = (static_cast<float>(pHeader->mSpriteSheetY) / pTga->mHeight);

    f32 u1 = u0 + ((float) pHeader->mWidth / (float) pTga->mWidth);
    f32 v1 = v0 + ((float) pHeader->mHeight / (float) pTga->mHeight);

    if (poly.mFlipX)
    {
        std::swap(u0, u1);
    }

    if (poly.mFlipY)
    {
        std::swap(v1, v0);
    }

    const u32 textureIdx = mConstructingBatch.TextureIdxForId(animRes.mUniqueId.Id());

    const QuadUvs uvs = {
        u0, v0,
        u1, v1};

    const QuadVerts verts = {
        static_cast<f32>(poly.mBase.vert.x),
        static_cast<f32>(poly.mBase.vert.y),
        static_cast<f32>(poly.mVerts[0].mVert.x),
        static_cast<f32>(poly.mVerts[0].mVert.y),
        static_cast<f32>(poly.mVerts[1].mVert.x),
        static_cast<f32>(poly.mVerts[1].mVert.y),
        static_cast<f32>(poly.mVerts[2].mVert.x),
        static_cast<f32>(poly.mVerts[2].mVert.y),
    };

    PushQuad(IRenderer::PsxDrawMode::DefaultFT4, verts, uvs, rgbs, palIndex, isShaded, blendMode, isSemiTrans, textureIdx);

    if (mConstructingBatch.AddTexture(animRes.mUniqueId.Id()))
    {
        mBatchTextures.emplace_back(texture);
    }

    RenderBatch& addedTo = mConstructingBatch;

    // Over the texture limit or changed to/from subtractive blending
    const bool bNewBatch = (mConstructingBatch.mTexturesInBatch == kTextureBatchSize);
    if (bNewBatch)
    {
        NewBatch();
    }

    return addedTo;
}

template class Batcher<VulkanRenderer::Texture, VulkanRenderer::BatchData, VulkanRenderer::Vertex>;
