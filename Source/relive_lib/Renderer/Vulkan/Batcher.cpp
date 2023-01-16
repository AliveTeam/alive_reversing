#include "Batcher.hpp"
#include "VulkanRenderer.hpp"
#include "../OpenGL3/OpenGLRenderer.hpp"
#include "../DirectX9/DirectX9Renderer.hpp"
#include "../../Primitives.hpp"
#include "../../FG1.hpp"
#include "../../Animation.hpp"
#include "../../../AliveLibAE/Font.hpp"

// TODO: Copy pasted from GL renderer
inline u16 GetTPageBlendMode(u16 tpage)
{
    return (tpage >> 4) & 3;
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushVertexData(IRenderer::PsxVertexData* pVertData, s32 count, std::shared_ptr<TextureType>& texture, u32 textureResId)
{
    const u32 blendMode = pVertData[0].blendMode;

    // Check if we need to invalidate the existing batched data
    //
    // The only reason we need to invalidate here is if the blend mode switches
    // to/from subtractive blending
    if (
        mConstructingBatch.mBlendMode != blendMode && mConstructingBatch.mBlendMode != kBatchValueUnset && (mConstructingBatch.mBlendMode == 2 || blendMode == 2))
    {
        NewBatch();
    }

    if (texture)
    {
        mConstructingBatch.AddTexture(textureResId, mBatchTextures, texture);
    }

    mConstructingBatch.mBlendMode = blendMode;

    // Push indicies for this data
    const u16 nextIndex = mIndexBufferIndex;
    const s32 numTriangles = count - 2;

    if (numTriangles == 1)
    {
        mIndices.emplace_back(nextIndex);
        mIndices.emplace_back(nextIndex + 1);
        mIndices.emplace_back(nextIndex + 2);

        mIndexBufferIndex += 3;
    }
    else if (numTriangles == 2)
    {
        mIndices.emplace_back(nextIndex + 1);
        mIndices.emplace_back(nextIndex);
        mIndices.emplace_back(nextIndex + 3);

        mIndices.emplace_back(nextIndex + 3);
        mIndices.emplace_back(nextIndex);
        mIndices.emplace_back(nextIndex + 2);

        mIndexBufferIndex += 4;
    }

    // Push vertex data
    const u32 textureIdx = mConstructingBatch.TextureIdxForId(textureResId);
    for (int i = 0; i < count; i++)
    {
        // TODO: Do we even need this now ? (its looked up again in GL atm)
        pVertData[i].textureUnitIndex = textureIdx;
        mVertices.emplace_back(pVertData[i]);
    }

    mConstructingBatch.mNumTrisToDraw += numTriangles;

    mBatchInProgress = true;

    // DEBUGGING: If batching is disabled we invalidate immediately
    bool bNewBatch = !mBatchingEnabled;
    if (texture)
    {
        bNewBatch = mConstructingBatch.mTexturesInBatch >= kTextureBatchSize - 1;
    }

    if (bNewBatch)
    {
        // TODO: With a batch limit of 1 and > 1 batches using the same texture this will still create
        // 2 batches which is wrong, probably need to also not break when we used 1 texture
        // and rendering flat prims
        NewBatch();
    }
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushLines(const IRenderer::PsxVertexData* vertices, s32 count)
{
    const s32 numLines = count - 1;

    for (s32 i = 0; i < numLines; i++)
    {
        const IRenderer::PsxVertexData& vertA = vertices[i];
        const IRenderer::PsxVertexData& vertB = vertices[i + 1];

        const IRenderer::Quad2D quad = IRenderer::LineToQuad(IRenderer::Point2D(vertA.x, vertA.y), IRenderer::Point2D(vertB.x, vertB.y));

        IRenderer::PsxVertexData triangleVerts[4] = {
            {quad.verts[0].x, quad.verts[0].y, vertA.r, vertA.g, vertA.b, 0.0f, 0.0f, vertA.drawMode, vertA.isSemiTrans, vertA.isShaded, vertA.blendMode, 0, 0},
            {quad.verts[1].x, quad.verts[1].y, vertA.r, vertA.g, vertA.b, 0.0f, 0.0f, vertA.drawMode, vertA.isSemiTrans, vertA.isShaded, vertA.blendMode, 0, 0},
            {quad.verts[2].x, quad.verts[2].y, vertB.r, vertB.g, vertB.b, 0.0f, 0.0f, vertB.drawMode, vertB.isSemiTrans, vertB.isShaded, vertB.blendMode, 0, 0},
            {quad.verts[3].x, quad.verts[3].y, vertB.r, vertB.g, vertB.b, 0.0f, 0.0f, vertB.drawMode, vertB.isSemiTrans, vertB.isShaded, vertB.blendMode, 0, 0}};

        std::shared_ptr<TextureType> nullTex;
        PushVertexData(triangleVerts, ALIVE_COUNTOF(triangleVerts), nullTex, 0);
    }
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::NewBatch()
{
    const SDL_Rect oldScissor = mConstructingBatch.mScissor;
    mBatches.emplace_back(mConstructingBatch);
    mConstructingBatch = {};
    mConstructingBatch.mScissor = oldScissor;
    mBatchInProgress = false;
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::SetScissor(const SDL_Rect& scissor)
{
    NewBatch();
    mConstructingBatch.mScissor = scissor;
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushGas(const Prim_GasEffect& gasEffect)
{
    if (gasEffect.pData == nullptr)
    {
        return;
    }

    const f32 x = static_cast<f32>(gasEffect.x);
    const f32 y = static_cast<f32>(gasEffect.y);
    const f32 w = static_cast<f32>(gasEffect.w);
    const f32 h = static_cast<f32>(gasEffect.h);

    const f32 r = 127;
    const f32 g = 127;
    const f32 b = 127;

    const f32 gasWidth = std::floor(static_cast<f32>(gasEffect.w - gasEffect.x) / 4);
    const f32 gasHeight = std::floor(static_cast<f32>(gasEffect.h - gasEffect.y) / 2);

    const bool isSemiTrans = true;
    const bool isShaded = true;
    const u32 blendMode = static_cast<u32>(TPageAbr::eBlend_0);

    if (mUvMode == UvMode::UnNormalized)
    {
        IRenderer::PsxVertexData verts[4] = {
            {x, y, r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, blendMode, 0, 0},
            {w, y, r, g, b, gasWidth, 0.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, blendMode, 0, 0},
            {x, h, r, g, b, 0.0f, gasHeight, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, blendMode, 0, 0},
            {w, h, r, g, b, gasWidth, gasHeight, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, blendMode, 0, 0}};

        std::shared_ptr<TextureType> nullTex;
        PushVertexData(verts, ALIVE_COUNTOF(verts), nullTex, 0);
    }
    else
    {
        IRenderer::PsxVertexData verts[4] = {
            {x, y, r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, blendMode, 0, 0},
            {w, y, r, g, b, 1.0f, 0.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, blendMode, 0, 0},
            {x, h, r, g, b, 0.0f, 1.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, blendMode, 0, 0},
            {w, h, r, g, b, 1.0f, 1.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, blendMode, 0, 0}};

        std::shared_ptr<TextureType> nullTex;
        PushVertexData(verts, ALIVE_COUNTOF(verts), nullTex, 0);
    }
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushPolyG4(const Poly_G4& poly, u32 blendMode)
{
    const bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    const bool isShaded = true;

    IRenderer::PsxVertexData verts[4] = {
        {static_cast<f32>(X0(&poly)), static_cast<f32>(Y0(&poly)), static_cast<f32>(R0(&poly)), static_cast<f32>(G0(&poly)), static_cast<f32>(B0(&poly)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(X1(&poly)), static_cast<f32>(Y1(&poly)), static_cast<f32>(R1(&poly)), static_cast<f32>(G1(&poly)), static_cast<f32>(B1(&poly)), 0.0f, 0.0f, IRenderer::IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(X2(&poly)), static_cast<f32>(Y2(&poly)), static_cast<f32>(R2(&poly)), static_cast<f32>(G2(&poly)), static_cast<f32>(B2(&poly)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(X3(&poly)), static_cast<f32>(Y3(&poly)), static_cast<f32>(R3(&poly)), static_cast<f32>(G3(&poly)), static_cast<f32>(B3(&poly)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0}};

    std::shared_ptr<TextureType> nullTex;
    PushVertexData(verts, ALIVE_COUNTOF(verts), nullTex, 0);
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushPolyG3(const Poly_G3& poly, u32 blendMode)
{
    const bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    const bool isShaded = true;

    IRenderer::PsxVertexData verts[3] = {
        {static_cast<f32>(X0(&poly)), static_cast<f32>(Y0(&poly)), static_cast<f32>(R0(&poly)), static_cast<f32>(G0(&poly)), static_cast<f32>(B0(&poly)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(X1(&poly)), static_cast<f32>(Y1(&poly)), static_cast<f32>(R1(&poly)), static_cast<f32>(G1(&poly)), static_cast<f32>(B1(&poly)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(X2(&poly)), static_cast<f32>(Y2(&poly)), static_cast<f32>(R2(&poly)), static_cast<f32>(G2(&poly)), static_cast<f32>(B2(&poly)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0}};

    std::shared_ptr<TextureType> nullTex;
    PushVertexData(verts, ALIVE_COUNTOF(verts), nullTex, 0);
}


template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushLine(const Line_G2& line, u32 blendMode)
{
    const bool isSemiTrans = GetPolyIsSemiTrans(&line);
    const bool isShaded = true;

    IRenderer::PsxVertexData verts[2] = {
        {static_cast<f32>(X0(&line)), static_cast<f32>(Y0(&line)), static_cast<f32>(R0(&line)), static_cast<f32>(G0(&line)), static_cast<f32>(B0(&line)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(X1(&line)), static_cast<f32>(Y1(&line)), static_cast<f32>(R1(&line)), static_cast<f32>(G1(&line)), static_cast<f32>(B1(&line)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0}};

    PushLines(verts, ALIVE_COUNTOF(verts));
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushLine(const Line_G4& line, u32 blendMode)
{
    const bool isSemiTrans = GetPolyIsSemiTrans(&line);
    const bool isShaded = true;

    IRenderer::PsxVertexData verts[4] = {
        {static_cast<f32>(X0(&line)), static_cast<f32>(Y0(&line)), static_cast<f32>(R0(&line)), static_cast<f32>(G0(&line)), static_cast<f32>(B0(&line)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(X1(&line)), static_cast<f32>(Y1(&line)), static_cast<f32>(R1(&line)), static_cast<f32>(G1(&line)), static_cast<f32>(B1(&line)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(X2(&line)), static_cast<f32>(Y2(&line)), static_cast<f32>(R2(&line)), static_cast<f32>(G2(&line)), static_cast<f32>(B2(&line)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(X3(&line)), static_cast<f32>(Y3(&line)), static_cast<f32>(R3(&line)), static_cast<f32>(G3(&line)), static_cast<f32>(B3(&line)), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0}};

    PushLines(verts, ALIVE_COUNTOF(verts));
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushFont(const Poly_FT4& poly, u32 palIndex, std::shared_ptr<TextureType>& texture)
{
    const f32 r = static_cast<f32>(R0(&poly));
    const f32 g = static_cast<f32>(G0(&poly));
    const f32 b = static_cast<f32>(B0(&poly));

    const bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    const bool isShaded = GetPolyIsShaded(&poly);
    const u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

    f32 u0 = 0.0f;
    f32 v0 = 0.0f;
    f32 u1 = 0.0f;
    f32 v1 = 0.0f;

    if (mUvMode == UvMode::UnNormalized)
    {
        u0 = U0(&poly);
        v0 = V0(&poly);

        u1 = U3(&poly);
        v1 = V3(&poly);
    }
    else
    {
        std::shared_ptr<TgaData> pTga = poly.mFont->mFntResource.mTgaPtr;

        u0 = U0(&poly) / (f32) pTga->mWidth;
        v0 = V0(&poly) / (f32) pTga->mHeight;

        u1 = U3(&poly) / (f32) pTga->mWidth;
        v1 = V3(&poly) / (f32) pTga->mHeight;
    }

    IRenderer::PsxVertexData verts[4] = {
        {static_cast<f32>(poly.mBase.vert.x), static_cast<f32>(poly.mBase.vert.y), r, g, b, u0, v0, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.mVerts[0].mVert.x), static_cast<f32>(poly.mVerts[0].mVert.y), r, g, b, u1, v0, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.mVerts[1].mVert.x), static_cast<f32>(poly.mVerts[1].mVert.y), r, g, b, u0, v1, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.mVerts[2].mVert.x), static_cast<f32>(poly.mVerts[2].mVert.y), r, g, b, u1, v1, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0}};

    PushVertexData(verts, ALIVE_COUNTOF(verts), texture, poly.mFont->mFntResource.mUniqueId.Id());
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushFG1(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture)
{
    const f32 r = static_cast<f32>(R0(&poly));
    const f32 g = static_cast<f32>(G0(&poly));
    const f32 b = static_cast<f32>(B0(&poly));

    const bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    const bool isShaded = GetPolyIsShaded(&poly);
    const u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

    if (mUvMode == UvMode::UnNormalized)
    {
        IRenderer::PsxVertexData verts[4] = {
            {static_cast<f32>(poly.mBase.vert.x), static_cast<f32>(poly.mBase.vert.y), r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[0].mVert.x), static_cast<f32>(poly.mVerts[0].mVert.y), r, g, b, IRenderer::kPsxFramebufferWidth, 0.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[1].mVert.x), static_cast<f32>(poly.mVerts[1].mVert.y), r, g, b, 0.0f, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[2].mVert.x), static_cast<f32>(poly.mVerts[2].mVert.y), r, g, b, IRenderer::kPsxFramebufferWidth, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0}};

        PushVertexData(verts, ALIVE_COUNTOF(verts), texture, poly.mFg1->mUniqueId.Id());
    }
    else
    {
        // TODO: Need to take the texture size into account
        IRenderer::PsxVertexData verts[4] = {
            {static_cast<f32>(poly.mBase.vert.x), static_cast<f32>(poly.mBase.vert.y), r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[0].mVert.x), static_cast<f32>(poly.mVerts[0].mVert.y), r, g, b, 1.0f, 0.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[1].mVert.x), static_cast<f32>(poly.mVerts[1].mVert.y), r, g, b, 0.0f, 1.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[2].mVert.x), static_cast<f32>(poly.mVerts[2].mVert.y), r, g, b, 1.0f, 1.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0}};

        PushVertexData(verts, ALIVE_COUNTOF(verts), texture, poly.mFg1->mUniqueId.Id());
    }
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushCAM(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture)
{
    const f32 r = static_cast<f32>(R0(&poly));
    const f32 g = static_cast<f32>(G0(&poly));
    const f32 b = static_cast<f32>(B0(&poly));

    const u32 isShaded = GetPolyIsShaded(&poly) ? 1 : 0;
    const u32 isSemiTrans = GetPolyIsSemiTrans(&poly) ? 1 : 0;
    const u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

    if (mUvMode == UvMode::UnNormalized)
    {
        IRenderer::PsxVertexData verts[4] = {
            {static_cast<f32>(poly.mBase.vert.x), static_cast<f32>(poly.mBase.vert.y), r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[0].mVert.x), static_cast<f32>(poly.mVerts[0].mVert.y), r, g, b, IRenderer::kPsxFramebufferWidth, 0.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[1].mVert.x), static_cast<f32>(poly.mVerts[1].mVert.y), r, g, b, 0.0f, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[2].mVert.x), static_cast<f32>(poly.mVerts[2].mVert.y), r, g, b, IRenderer::kPsxFramebufferWidth, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0}};

        PushVertexData(verts, ALIVE_COUNTOF(verts), texture, texture ? poly.mCam->mUniqueId.Id() : 0);
    }
    else
    {
        // TODO: Need to take the texture size into account
        IRenderer::PsxVertexData verts[4] = {
            {static_cast<f32>(poly.mBase.vert.x), static_cast<f32>(poly.mBase.vert.y), r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[0].mVert.x), static_cast<f32>(poly.mVerts[0].mVert.y), r, g, b, 1.0f, 0.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[1].mVert.x), static_cast<f32>(poly.mVerts[1].mVert.y), r, g, b, 0.0f, 1.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.mVerts[2].mVert.x), static_cast<f32>(poly.mVerts[2].mVert.y), r, g, b, 1.0f, 1.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0}};

        PushVertexData(verts, ALIVE_COUNTOF(verts), texture, texture ? poly.mCam->mUniqueId.Id() : 0);
    }

    if (texture)
    {
        mCamTexture = texture;
    }
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushAnim(const Poly_FT4& poly, u32 palIndex, std::shared_ptr<TextureType>& texture)
{
    const f32 r = static_cast<f32>(R0(&poly));
    const f32 g = static_cast<f32>(G0(&poly));
    const f32 b = static_cast<f32>(B0(&poly));

    const u32 isShaded = GetPolyIsShaded(&poly) ? 1 : 0;
    const u32 isSemiTrans = GetPolyIsSemiTrans(&poly) ? 1 : 0;
    const u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

    const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);

    AnimResource& animRes = poly.mAnim->mAnimRes;
    auto pTga = animRes.mTgaPtr;

    f32 u0 = 0.0f;
    f32 v0 = 0.0f;
    f32 u1 = 0.0f;
    f32 v1 = 0.0f;

    if (mUvMode == UvMode::UnNormalized)
    {
        u0 = static_cast<f32>(pHeader->mSpriteSheetX);
        v0 = static_cast<f32>(pHeader->mSpriteSheetY);

        u1 = u0 + pHeader->mWidth;
        v1 = v0 + pHeader->mHeight;
    }
    else
    {
        u0 = (static_cast<f32>(pHeader->mSpriteSheetX) / pTga->mWidth);
        v0 = (static_cast<f32>(pHeader->mSpriteSheetY) / pTga->mHeight);

        u1 = u0 + ((f32) pHeader->mWidth / (f32) pTga->mWidth);
        v1 = v0 + ((f32) pHeader->mHeight / (f32) pTga->mHeight);
    }

    if (poly.mFlipX)
    {
        std::swap(u0, u1);
    }

    if (poly.mFlipY)
    {
        std::swap(v1, v0);
    }

    IRenderer::PsxVertexData verts[4] = {
        {static_cast<f32>(poly.mBase.vert.x), static_cast<f32>(poly.mBase.vert.y), r, g, b, u0, v0, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.mVerts[0].mVert.x), static_cast<f32>(poly.mVerts[0].mVert.y), r, g, b, u1, v0, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.mVerts[1].mVert.x), static_cast<f32>(poly.mVerts[1].mVert.y), r, g, b, u0, v1, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.mVerts[2].mVert.x), static_cast<f32>(poly.mVerts[2].mVert.y), r, g, b, u1, v1, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0}};

    PushVertexData(verts, ALIVE_COUNTOF(verts), texture, animRes.mUniqueId.Id());
}

template class Batcher<VulkanRenderer::Texture, VulkanRenderer::BatchData, 13>;
template class Batcher<GLTexture2D, OpenGLRenderer::BatchData, 12>;

#ifdef _WIN32
template class Batcher<ATL::CComPtr<IDirect3DTexture9>, DirectX9Renderer::BatchData, 1>;
#endif
