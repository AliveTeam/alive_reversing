#pragma once

#include "../../Types.hpp"
#include "../IRenderer.hpp"
#include <vector>
#include <memory>

enum class UvMode
{
    UnNormalized,
    Normalized,
};

template <typename TextureType, typename RenderBatchType, std::size_t kTextureBatchSize>
class Batcher final
{
public:
    static constexpr u32 kBatchValueUnset = 999;

    bool mBatchingEnabled = true;

    void SetBatching(bool batching)
    {
        mBatchingEnabled = batching;
    }

    UvMode mUvMode = UvMode::UnNormalized;

    Batcher(UvMode uvMode = UvMode::Normalized)
        : mUvMode(uvMode)
    {
        mVertices.reserve(IRenderer::kReserveFT4QuadCount * 4);
        mIndices.reserve(IRenderer::kReserveFT4QuadCount * 6);
        mBatchTextures.reserve(kTextureBatchSize * 10);
    }

    std::shared_ptr<TextureType> mCamTexture; // TODO: remove ?

    std::vector<std::shared_ptr<TextureType>> mBatchTextures;

    struct RenderBatch final : public RenderBatchType
    {
        u32 mNumTrisToDraw = 0;
        u32 mTexturesInBatch = 0;
        u32 mTextureIds[kTextureBatchSize] = {};
        u32 mBlendMode = 0;
        SDL_Rect mScissor = {};

        void AddTexture(u32 id, std::vector<std::shared_ptr<TextureType>>& batchedTextures, std::shared_ptr<TextureType>& texture)
        {
            for (u32 i = 0; i < mTexturesInBatch; i++)
            {
                if (mTextureIds[i] == id)
                {
                    // Already have it
                    return;
                }
            }
            mTextureIds[mTexturesInBatch] = id;
            batchedTextures.emplace_back(texture);
            mTexturesInBatch++;
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

    void PushGas(const Prim_GasEffect& gasEffect);

    void PushPolyG4(const Poly_G4& poly, u32 blendMode);

    void PushPolyG3(const Poly_G3& poly, u32 blendMode);

    void PushLine(const Line_G2& line, u32 blendMode);

    void PushLine(const Line_G4& line, u32 blendMode);

    void PushFont(const Poly_FT4& poly, u32 palIndex, std::shared_ptr<TextureType>& texture);

    void PushFG1(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture);

    void PushCAM(const Poly_FT4& poly, std::shared_ptr<TextureType>& texture);

    void PushAnim(const Poly_FT4& poly, u32 palIndex, std::shared_ptr<TextureType>& texture);

    void PushVertexData(IRenderer::PsxVertexData* pVertData, s32 count, std::shared_ptr<TextureType>& texture, u32 textureResId);

    void PushLines(const IRenderer::PsxVertexData* vertices, s32 count);

    void NewBatch();

    void SetScissor(const SDL_Rect& scissor);

    void EndFrame()
    {
        // Ensure any remaining data is drawn
        if (mConstructingBatch.mNumTrisToDraw > 0)
        {
            NewBatch();
        }
    }

    void StartFrame()
    {
        mBatches.clear();
        mBatchTextures.clear();
        mIndices.clear();
        mVertices.clear();
        mIndexBufferIndex = 0;
        mConstructingBatch = {};
        mCamTexture = nullptr;
    }

    bool mBatchInProgress = false;
    RenderBatch mConstructingBatch;

    std::vector<RenderBatch> mBatches;

    std::vector<IRenderer::PsxVertexData> mVertices;
    u16 mIndexBufferIndex = 0;
    std::vector<u32> mIndices;
};
