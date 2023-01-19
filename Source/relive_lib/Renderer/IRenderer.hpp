#pragma once

#include "SDL.h"

struct PrimHeader;
struct Prim_PrimClipper;
struct Prim_ScreenOffset;
struct Prim_GasEffect;
struct Line_G2;
struct Line_G4;
struct Poly_G3;
struct Poly_FT4;
struct Poly_G4;
struct SDL_Window;
using TWindowHandleType = SDL_Window*;

class RendererException final : public std::exception
{
public:
    explicit RendererException(char const* const message) noexcept
        : mMessage(message)
    {

    }

    [[nodiscard]] virtual const char* what() const noexcept override
    {
         return mMessage;
    }

private:
    const char* mMessage = nullptr;
};

class IRenderer
{
public: // TODO: Make protected later
    enum class PsxDrawMode : u32
    {
        Flat = 0,
        DefaultFT4 = 1,
        Camera = 2,
        FG1 = 3,
        Gas = 4
    };

    struct PsxVertexData final
    {
        f32 x, y;
        f32 r, g, b;
        f32 u, v;
        PsxDrawMode drawMode;
        u32 isSemiTrans;
        u32 isShaded;
        u32 blendMode;
        u32 paletteIndex;
        u32 textureUnitIndex;
    };

    // Original game resolution - 640x240
    static constexpr s32 kPsxFramebufferHeight = 240;
    static constexpr s32 kPsxFramebufferWidth = 640;

    struct Point2D final
    {
        f32 x;
        f32 y;

        Point2D(f32 x0, f32 y0)
            : x(x0)
            , y(y0)
        { }
    };

    struct Quad2D final
    {
        Point2D verts[4];
    };

    static Quad2D LineToQuad(const Point2D& p1, const Point2D& p2);

protected:
    bool mFramebufferFilter = true;


public:
    enum class Renderers
    {
        Vulkan,
        DirectX9,
        OpenGL,
    };

    static IRenderer* GetRenderer();
    static bool CreateRenderer(Renderers type, const std::string& windowTitle);
    static void FreeRenderer();

public:
    explicit IRenderer(SDL_Window* window)
        : mWindow(window)
    { 

    }

    virtual ~IRenderer()
    {

    }

    void ToggleFilterScreen()
    {
        mFramebufferFilter = !mFramebufferFilter;
    }

    void ToggleKeepAspectRatio()
    {
        mKeepAspectRatio = !mKeepAspectRatio;
    }

    virtual void Clear(u8 r, u8 g, u8 b) = 0;

    // Derived objects should always call this
    virtual void StartFrame()
    {
        if (mIsFirstStartFrame)
        {
            // Make the window visible only on the first frame otherwise you can see
            // some unclear framebuffer crap for a half second or so
            SDL_ShowWindow(mWindow);

            // Bring to front and give input focus
            SDL_RaiseWindow(mWindow);

            mIsFirstStartFrame = false;
        }
    }

    virtual void EndFrame() = 0;

    virtual void SetTPage(u16 tPage) = 0;

    virtual void SetClip(const Prim_PrimClipper& clipper) = 0;
    void SetScreenOffset(const Prim_ScreenOffset& offset);

    virtual void Draw(const Prim_GasEffect& gasEffect) = 0;

    // AO: Spark/SnoozeParticle, AE: Spark/SnoozeParticle + ThrowableTotal
    virtual void Draw(const Line_G2& line) = 0;

    // SnoozeParticle
    virtual void Draw(const Line_G4& line) = 0;

    // MainMenuTransistion
    virtual void Draw(const Poly_G3& poly) = 0;

    // FG1, Animation, Font, ScreenWave, Water
    virtual void Draw(const Poly_FT4& poly) = 0;

    // Fleech (tounge), DeathGas, ColourfulMeter
    virtual void Draw(const Poly_G4& poly) = 0;

    // Recommendations for reserving memory to fit 'peak' amounts of quads
    // during batching:
    //   - For regular Poly_FT4s, the peak tends to be about 300 when the game
    //     is rendering a Spline (chant orb zap made out of ~260 individual
    //     sprites)
    //   - For ScreenWave framebuffer effect there are 256 quads
    //
    static constexpr s32 kReserveFT4QuadCount = 300;
    static constexpr s32 kReserveScreenWaveQuadCount = 256;

protected:




    // Original game target resolution - 640x480
    static constexpr s32 kTargetFramebufferHeight = 480;
    static constexpr s32 kTargetFramebufferWidth = 640;

protected:
    SDL_Rect GetTargetDrawRect();

protected:
    bool mIsFirstStartFrame = true;

    SDL_Window* mWindow = nullptr;

    s32 mOffsetX = 0;
    s32 mOffsetY = 0;

    bool mKeepAspectRatio = true;
};
