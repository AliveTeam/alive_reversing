#pragma once

#include <vector>
#include "../IRenderer.hpp"

class Sdl2Renderer final : public IRenderer
{
public:
    explicit Sdl2Renderer(TWindowHandleType window);
    ~Sdl2Renderer() override;

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
    SDL_FPoint PointToViewport(const SDL_FPoint& point);
    void ScaleVertices(std::vector<SDL_Vertex>& vertices);

private:
    SDL_Renderer* mRenderer;

    bool mRenderTargetSupported;
};
