#include "Batcher.hpp"
#include "VulkanRenderer.hpp"
#include "../OpenGL3/OpenGLRenderer.hpp"
#include "../DirectX9/DirectX9Renderer.hpp"
#include "../../Primitives.hpp"
#include "../../FG1.hpp"
#include "../../Animation.hpp"
#include "../../../AliveLibAE/Font.hpp"

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushVertexData(IRenderer::PsxVertexData* pVertData, s32 count, std::shared_ptr<TextureType>& texture, u32 textureResId)
{
    const relive::TBlendModes blendMode = pVertData[0].blendMode;

    // Check if we need to invalidate the existing batched data
    //
    // We need to invalidate the batch when:
    //     - The blend mode switches to/from subtractive blending
    //     - The current batch draws from the framebuffer
    if (
        mConstructingBatch.mSourceIsFramebuffer ||
        (
            mConstructingBatch.mBlendMode != blendMode &&
            mConstructingBatch.mBlendMode != kBatchValueUnset && (mConstructingBatch.mBlendMode == relive::TBlendModes::eBlend_2 || blendMode == relive::TBlendModes::eBlend_2)
        )
    )
    {
        NewBatch();
    }

    if (texture)
    {
        mConstructingBatch.AddTexture(textureResId, mBatchTextures, texture);
    }

    mConstructingBatch.mBlendMode = blendMode;

    // Locate and update texture unit IDs for the buffer data
    const u32 textureIdx = mConstructingBatch.TextureIdxForId(textureResId);
    for (int i = 0; i < count; i++)
    {
        // TODO: Do we even need this now ? (its looked up again in GL atm)
        pVertData[i].textureUnitIndex = textureIdx;
    }

    InsertVertexData(pVertData, count);

    // DEBUGGING: If batching is disabled we invalidate immediately
    bool bNewBatch = !mBatchingEnabled;
    if (!bNewBatch)
    {
        // Note: We could batch flat polys into the textured batch
        // but this will break in DX9 as the shader can only render 1
        // draw type.
        // TODO: Probably there should be a batcher config option to say if we
        // should split batches on changing draw types.
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
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushFramebufferVertexData(const IRenderer::PsxVertexData* pVertData, s32 count)
{
    // We should invalidate here if the current batch is not drawing using
    // the framebuffer as the source texture
    if (!mConstructingBatch.mSourceIsFramebuffer)
    {
        NewBatch();

        mConstructingBatch.mSourceIsFramebuffer = true;

        // Add entire contents of the screen itself first
        if (mUvMode == UvMode::UnNormalized)
        {
            IRenderer::PsxVertexData verts[4] = {
                { 0.0f, 0.0f, 127.0f, 127.0f, 127.0f, 0.0f, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0 },
                {0.0f, IRenderer::kPsxFramebufferHeight, 127.0f, 127.0f, 127.0f, 0.0f, 0.0f, IRenderer::PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0},
                {IRenderer::kPsxFramebufferWidth, 0.0f, 127.0f, 127.0f, 127.0f, IRenderer::kPsxFramebufferWidth, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0},
                {IRenderer::kPsxFramebufferWidth, IRenderer::kPsxFramebufferHeight, 127.0f, 127.0f, 127.0f, IRenderer::kPsxFramebufferWidth, 0.0f, IRenderer::PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0}
            };

            InsertVertexData(verts, ALIVE_COUNTOF(verts));
        }
        else
        {
            IRenderer::PsxVertexData verts[4] = {
                {0.0f, 0.0f, 127.0f, 127.0f, 127.0f, 0.0f, 1.0f, IRenderer::PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0},
                {0.0f, 1.0f, 127.0f, 127.0f, 127.0f, 0.0f, 0.0f, IRenderer::PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0},
                {1.0f, 0.0f, 127.0f, 127.0f, 127.0f, 1.0f, 1.0f, IRenderer::PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0},
                {1.0f, 1.0f, 127.0f, 127.0f, 127.0f, 1.0f, 0.0f, IRenderer::PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0}
            };

            InsertVertexData(verts, ALIVE_COUNTOF(verts));
        }
    }

    InsertVertexData(pVertData, count);

    // DEBUGGING: If batching is disabled we invalidate immediately
    if (!mBatchingEnabled)
    {
        NewBatch();
    }
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::InsertVertexData(const IRenderer::PsxVertexData* pVertData, s32 count)
{
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

    for (int i = 0; i < count; i++)
    {
        mVertices.emplace_back(pVertData[i]);
    }

    mConstructingBatch.mNumTrisToDraw += numTriangles;

    mBatchInProgress = true;
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

    if (mUvMode == UvMode::UnNormalized)
    {
        IRenderer::PsxVertexData verts[4] = {
            {x, y, r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, relive::TBlendModes::eBlend_0, 0, 0},
            {w, y, r, g, b, gasWidth, 0.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, relive::TBlendModes::eBlend_0, 0, 0},
            {x, h, r, g, b, 0.0f, gasHeight, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, relive::TBlendModes::eBlend_0, 0, 0},
            {w, h, r, g, b, gasWidth, gasHeight, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, relive::TBlendModes::eBlend_0, 0, 0}};

        std::shared_ptr<TextureType> nullTex;
        PushVertexData(verts, ALIVE_COUNTOF(verts), nullTex, 0);
    }
    else
    {
        IRenderer::PsxVertexData verts[4] = {
            {x, y, r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, relive::TBlendModes::eBlend_0, 0, 0},
            {w, y, r, g, b, 1.0f, 0.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, relive::TBlendModes::eBlend_0, 0, 0},
            {x, h, r, g, b, 0.0f, 1.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, relive::TBlendModes::eBlend_0, 0, 0},
            {w, h, r, g, b, 1.0f, 1.0f, IRenderer::PsxDrawMode::Gas, isSemiTrans, isShaded, relive::TBlendModes::eBlend_0, 0, 0}};

        std::shared_ptr<TextureType> nullTex;
        PushVertexData(verts, ALIVE_COUNTOF(verts), nullTex, 0);
    }
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushPolyG4(const Poly_G4& poly, relive::TBlendModes blendMode)
{
    const bool isSemiTrans = poly.mSemiTransparent;
    const bool isShaded = true;

    IRenderer::PsxVertexData verts[4] = {
        {static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()), static_cast<f32>(poly.R0()), static_cast<f32>(poly.G0()), static_cast<f32>(poly.B0()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()), static_cast<f32>(poly.R1()), static_cast<f32>(poly.G1()), static_cast<f32>(poly.B1()), 0.0f, 0.0f, IRenderer::IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()), static_cast<f32>(poly.R2()), static_cast<f32>(poly.G2()), static_cast<f32>(poly.B2()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()), static_cast<f32>(poly.R3()), static_cast<f32>(poly.G3()), static_cast<f32>(poly.B3()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0}};

    std::shared_ptr<TextureType> nullTex;
    PushVertexData(verts, ALIVE_COUNTOF(verts), nullTex, 0);
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushPolyG3(const Poly_G3& poly, relive::TBlendModes blendMode)
{
    const bool isSemiTrans = poly.mSemiTransparent;
    const bool isShaded = true;

    IRenderer::PsxVertexData verts[3] = {
        {static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()), static_cast<f32>(poly.R0()), static_cast<f32>(poly.G0()), static_cast<f32>(poly.B0()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()), static_cast<f32>(poly.R1()), static_cast<f32>(poly.G1()), static_cast<f32>(poly.B1()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()), static_cast<f32>(poly.R2()), static_cast<f32>(poly.G2()), static_cast<f32>(poly.B2()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0}};

    std::shared_ptr<TextureType> nullTex;
    PushVertexData(verts, ALIVE_COUNTOF(verts), nullTex, 0);
}


template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushLine(const Line_G2& line, relive::TBlendModes blendMode)
{
    const bool isSemiTrans = line.mSemiTransparent;
    const bool isShaded = true;

    IRenderer::PsxVertexData verts[2] = {
        {static_cast<f32>(line.X0()), static_cast<f32>(line.Y0()), static_cast<f32>(line.R0()), static_cast<f32>(line.G0()), static_cast<f32>(line.B0()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(line.X1()), static_cast<f32>(line.Y1()), static_cast<f32>(line.R1()), static_cast<f32>(line.G1()), static_cast<f32>(line.B1()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0}};

    PushLines(verts, ALIVE_COUNTOF(verts));
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushLine(const Line_G4& line, relive::TBlendModes blendMode)
{
    const bool isSemiTrans = line.mSemiTransparent;
    const bool isShaded = true;

    IRenderer::PsxVertexData verts[4] = {
        {static_cast<f32>(line.X0()), static_cast<f32>(line.Y0()), static_cast<f32>(line.R0()), static_cast<f32>(line.G0()), static_cast<f32>(line.B0()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(line.X1()), static_cast<f32>(line.Y1()), static_cast<f32>(line.R1()), static_cast<f32>(line.G1()), static_cast<f32>(line.B1()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(line.X2()), static_cast<f32>(line.Y2()), static_cast<f32>(line.R2()), static_cast<f32>(line.G2()), static_cast<f32>(line.B2()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0},
        {static_cast<f32>(line.X3()), static_cast<f32>(line.Y3()), static_cast<f32>(line.R3()), static_cast<f32>(line.G3()), static_cast<f32>(line.B3()), 0.0f, 0.0f, IRenderer::PsxDrawMode::Flat, isSemiTrans, isShaded, blendMode, 0, 0}};

    PushLines(verts, ALIVE_COUNTOF(verts));
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushFont(const Poly_FT4& poly, u32 palIndex, std::shared_ptr<TextureType>& texture)
{
    const f32 r = static_cast<f32>(poly.R0());
    const f32 g = static_cast<f32>(poly.G0());
    const f32 b = static_cast<f32>(poly.B0());

    const bool isSemiTrans = poly.mSemiTransparent;
    const bool isShaded = poly.mIsShaded;
    const relive::TBlendModes blendMode = poly.mBlendMode;

    f32 u0 = 0.0f;
    f32 v0 = 0.0f;
    f32 u1 = 0.0f;
    f32 v1 = 0.0f;

    if (mUvMode == UvMode::UnNormalized)
    {
        u0 = poly.U0();
        v0 = poly.V0();

        u1 = poly.U3();
        v1 = poly.V3();
    }
    else
    {
        std::shared_ptr<PngData> pPng = poly.mFont->mFntResource.mPngPtr;

        u0 = poly.U0() / static_cast<f32>(pPng->mWidth);
        v0 = poly.V0() / static_cast<f32>(pPng->mHeight);

        u1 = poly.U3() / static_cast<f32>(pPng->mWidth);
        v1 = poly.V3() / static_cast<f32>(pPng->mHeight);
    }

    IRenderer::PsxVertexData verts[4] = {
        {static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()), r, g, b, u0, v0, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()), r, g, b, u1, v0, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()), r, g, b, u0, v1, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()), r, g, b, u1, v1, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0}};

    PushVertexData(verts, ALIVE_COUNTOF(verts), texture, poly.mFont->mFntResource.mUniqueId.Id());
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushFG1(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture)
{
    const f32 r = static_cast<f32>(poly.R0());
    const f32 g = static_cast<f32>(poly.G0());
    const f32 b = static_cast<f32>(poly.B0());

    const bool isSemiTrans = poly.mSemiTransparent;
    const bool isShaded = poly.mIsShaded;
    const relive::TBlendModes blendMode = poly.mBlendMode;

    if (mUvMode == UvMode::UnNormalized)
    {
        IRenderer::PsxVertexData verts[4] = {
            {static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()), r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()), r, g, b, IRenderer::kPsxFramebufferWidth, 0.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()), r, g, b, 0.0f, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()), r, g, b, IRenderer::kPsxFramebufferWidth, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0}};

        PushVertexData(verts, ALIVE_COUNTOF(verts), texture, poly.mFg1->mUniqueId.Id());
    }
    else
    {
        // TODO: Need to take the texture size into account
        IRenderer::PsxVertexData verts[4] = {
            {static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()), r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()), r, g, b, 1.0f, 0.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()), r, g, b, 0.0f, 1.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()), r, g, b, 1.0f, 1.0f, IRenderer::PsxDrawMode::FG1, isSemiTrans, isShaded, blendMode, 0, 0}};

        PushVertexData(verts, ALIVE_COUNTOF(verts), texture, poly.mFg1->mUniqueId.Id());
    }
}

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
void Batcher<TextureType, RenderBatchType, kTextureBatchSize>::PushCAM(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture)
{
    const f32 r = static_cast<f32>(poly.R0());
    const f32 g = static_cast<f32>(poly.G0());
    const f32 b = static_cast<f32>(poly.B0());

    const u32 isShaded = poly.mIsShaded;
    const u32 isSemiTrans = poly.mSemiTransparent;
    const relive::TBlendModes blendMode = poly.mBlendMode;

    if (mUvMode == UvMode::UnNormalized)
    {
        IRenderer::PsxVertexData verts[4] = {
            {static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()), r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()), r, g, b, IRenderer::kPsxFramebufferWidth, 0.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()), r, g, b, 0.0f, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()), r, g, b, IRenderer::kPsxFramebufferWidth, IRenderer::kPsxFramebufferHeight, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0}};

        PushVertexData(verts, ALIVE_COUNTOF(verts), texture, texture ? poly.mCam->mUniqueId.Id() : 0);
    }
    else
    {
        // TODO: Need to take the texture size into account
        IRenderer::PsxVertexData verts[4] = {
            {static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()), r, g, b, 0.0f, 0.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()), r, g, b, 1.0f, 0.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()), r, g, b, 0.0f, 1.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0},
            {static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()), r, g, b, 1.0f, 1.0f, IRenderer::PsxDrawMode::Camera, isSemiTrans, isShaded, blendMode, 0, 0}};

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
    const f32 r = static_cast<f32>(poly.R0());
    const f32 g = static_cast<f32>(poly.G0());
    const f32 b = static_cast<f32>(poly.B0());

    const u32 isShaded = poly.mIsShaded;
    const u32 isSemiTrans = poly.mSemiTransparent;
    const relive::TBlendModes blendMode = poly.mBlendMode;

    const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);

    AnimResource& animRes = poly.mAnim->mAnimRes;
    auto pPng = animRes.mPngPtr;

    f32 u0 = 0.0f;
    f32 v0 = 0.0f;
    f32 u1 = 0.0f;
    f32 v1 = 0.0f;

    if (mUvMode == UvMode::UnNormalized)
    {
        u0 = static_cast<f32>(pHeader->mSpriteSheetX);
        v0 = static_cast<f32>(pHeader->mSpriteSheetY);

        u1 = u0 + pHeader->mSpriteWidth - 1;
        v1 = v0 + pHeader->mSpriteHeight - 1;
    }
    else
    {
        u0 = (static_cast<f32>(pHeader->mSpriteSheetX) / pPng->mWidth);
        v0 = (static_cast<f32>(pHeader->mSpriteSheetY) / pPng->mHeight);

        u1 = u0 + ((f32) pHeader->mSpriteWidth / (f32) pPng->mWidth);
        v1 = v0 + ((f32) pHeader->mSpriteHeight / (f32) pPng->mHeight);
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
        {static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()), r, g, b, u0, v0, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()), r, g, b, u1, v0, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()), r, g, b, u0, v1, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
        {static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()), r, g, b, u1, v1, IRenderer::PsxDrawMode::DefaultFT4, isSemiTrans, isShaded, blendMode, palIndex, 0}};

    PushVertexData(verts, ALIVE_COUNTOF(verts), texture, animRes.mUniqueId.Id());
}

template class Batcher<VulkanRenderer::Texture, VulkanRenderer::BatchData, 13>;
template class Batcher<GLTexture2D, OpenGLRenderer::BatchData, 12>;

#ifdef _WIN32
template class Batcher<ATL::CComPtr<IDirect3DTexture9>, DirectX9Renderer::BatchData, 1>;
#endif
