#pragma once

#include "../../AliveLibCommon/Sys_common.hpp"
#include "../../AliveLibCommon/Primitives_common.hpp"
#include "../../AliveLibCommon/Psx_common.hpp"
#include <SDL.h>

struct PrimHeader;

class IRenderer
{
public:
    enum class Renderers
    {
        Software,
        DirectX9,
        OpenGL,
    };

    static IRenderer* GetRenderer();
    static void CreateRenderer(Renderers type, TWindowHandleType window);
    static void FreeRenderer();

public:
    // Recommendations for reserving memory to fit 'peak' amounts of quads
    // during batching:
    //   - For regular Poly_FT4s, the peak tends to be about 300 when the game
    //     is rendering a Spline (chant orb zap made out of ~260 individual
    //     sprites)
    //   - For ScreenWave framebuffer effect there are 256 quads
    //
    static constexpr s32 kReserveFT4QuadCount = 300;
    static constexpr s32 kReserveScreenWaveQuadCount = 256;

    // Original game resolution - 640x240
    static constexpr s32 kPsxFramebufferHeight = 240;
    static constexpr s32 kPsxFramebufferWidth = 640;

    // Original game target resolution - 640x480
    static constexpr s32 kTargetFramebufferHeight = 480;
    static constexpr s32 kTargetFramebufferWidth = 640;

    explicit IRenderer(SDL_Window* window)
        : mWindow(window)
    { 

    }

    virtual ~IRenderer()
    {

    }

    void ToggleKeepAspectRatio()
    {
        mKeepAspectRatio = !mKeepAspectRatio;
    }

    virtual void Clear(u8 r, u8 g, u8 b) = 0;
    virtual void StartFrame() = 0;
    virtual void EndFrame() = 0;

    virtual void SetTPage(u16 tPage) = 0;

    virtual void SetClip(Prim_PrimClipper& clipper) = 0;
    void SetScreenOffset(Prim_ScreenOffset& offset);

    virtual void ToggleFilterScreen() = 0;

    virtual void Draw(Prim_GasEffect& gasEffect) = 0;

    // AO: Spark/SnoozeParticle, AE: Spark/SnoozeParticle + ThrowableTotal
    virtual void Draw(Line_G2& line) = 0;

    // SnoozeParticle
    virtual void Draw(Line_G4& line) = 0;

    // MainMenuTransistion
    virtual void Draw(Poly_G3& poly) = 0;

    // FG1, Animation, Font, ScreenWave, Water
    virtual void Draw(Poly_FT4& poly) = 0;

    // Fleech (tounge), DeathGas, ColourfulMeter
    virtual void Draw(Poly_G4& poly) = 0;

protected:
    SDL_Rect GetTargetDrawRect();

protected:
    SDL_Window* mWindow = nullptr;

    s32 mOffsetX = 0;
    s32 mOffsetY = 0;

    bool mKeepAspectRatio = true;
};
