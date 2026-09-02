#pragma once

#include "Sdl3Context.hpp"
#include "Sdl3Texture.hpp"
#include "../IRenderer.hpp"
#include "../TextureCache.hpp"

class Sdl3Renderer final : public IRenderer
{
public:
    explicit Sdl3Renderer(TWindowHandleType window);
    ~Sdl3Renderer() override;

    void Clear(u8 r, u8 g, u8 b) override;
    void Draw(const Prim_GasEffect& gasEffect) override;
    void Draw(const Line_G2& line) override;
    void Draw(const Line_G4& line) override;
    void Draw(const Poly_G3& poly) override;
    void Draw(const Poly_FT4& poly) override;
    void Draw(const Poly_G4& poly) override;
    void EndFrame() override;
    void SetClip(const Prim_ScissorRect& clipper) override;
    void StartFrame() override;

private:
    void DrawLines(const IRenderer::Point2D points[], s32 numPoints, RGBA32 color, relive::TBlendModes blendMode);
    void DrawVertices(SDL_Vertex vertices[], s32 numVertices, const s32 indices[], s32 numIndices, SDL_Texture* texture, bool isSemiTrans, relive::TBlendModes blendMode);
    Sdl3Texture& GetActiveFbTexture();
    std::shared_ptr<Sdl3Texture> PrepareTextureFromPoly(const Poly_FT4& poly);
    SDL_FPoint PointToViewport(const SDL_FPoint& point);
    void ScaleVertices(SDL_Vertex vertices[], s32 numVertices);
    void SwitchActiveFbTexture();

private:
    Sdl3Context mContext;

    u8 mActiveFbTexture = 0;
    bool mCopiedFbThisFrame;
    Sdl3Texture mPsxFbTexture[2];

    Sdl3Texture mGasTexture;
    TextureCache<std::shared_ptr<Sdl3Texture>> mTextureCache;
};
