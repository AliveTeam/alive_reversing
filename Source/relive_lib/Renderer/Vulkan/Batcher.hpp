#pragma once

#include "../IRenderer.hpp"
#include <vector>
#include <memory>

template <typename TextureType, typename RenderBatchType, typename VertexType>
class Batcher final
{
public:
    std::unique_ptr<TextureType> mPaletteTexture;
    std::shared_ptr<TextureType> mCamTexture;
    std::vector<std::shared_ptr<TextureType>> mBatchTextures;

    using Type = Batcher<TextureType, RenderBatchType, VertexType>;

    struct RenderBatch final : public RenderBatchType
    {
        u32 mNumTrisToDraw = 0;
        u32 mTexturesInBatch = 0;
        u32 mTextureIds[14] = {};
        u32 mBlendMode = 0;
        SDL_Rect mScissor = {};

        bool AddTexture(u32 id)
        {
            for (u32 i = 0; i < mTexturesInBatch; i++)
            {
                if (mTextureIds[i] == id)
                {
                    // Already have it
                    return false;
                }
            }
            mTextureIds[mTexturesInBatch] = id;
            mTexturesInBatch++;
            return true;
        }

        u32 TextureIdxForId(u32 id) const
        {
            for (u32 i = 0; i < mTexturesInBatch; i++)
            {
                if (mTextureIds[i] == id)
                {
                    return i;
                }
            }
            return mTexturesInBatch;
        }
    };

    struct QuadVerts final
    {
        f32 x0;
        f32 y0;
        f32 x1;
        f32 y1;
        f32 x2;
        f32 y2;
        f32 x3;
        f32 y3;
    };

    struct QuadUvs final
    {
        f32 u0;
        f32 v0;
        f32 u1;
        f32 v1;
    };

    struct RGB final
    {
        u8 r;
        u8 g;
        u8 b;
    };

    RenderBatch& PushFG1(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture);

    RenderBatch& PushCAM(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture);

    RenderBatch& PushFont(const Poly_FT4& poly, u32 palIndex, std::shared_ptr<TextureType>& texture);

    RenderBatch& PushAnim(const Poly_FT4& poly, u32 palIndex, std::shared_ptr<TextureType>& texture);

    void PushQuad(IRenderer::PsxDrawMode drawMode, const QuadVerts& verts, const QuadUvs& uvs, const RGB* rgb, u32 palIdx, u32 isShaded, u32 blendMode, u32 isSemiTrans, u32 textureIdx);

    void NewBatch();

    void SetScissor(const SDL_Rect& scissor);

    const u32 kTextureBatchSize = 14;

    RenderBatch mConstructingBatch;
    std::vector<RenderBatch> mBatches;
    bool mBatchInProgress = false;
    u16 mIndexBufferIndex = 0;

    std::vector<VertexType> mVertices;
    std::vector<uint16_t> mIndices;
};
