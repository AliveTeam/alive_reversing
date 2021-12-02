#pragma once

#include "Sys_common.hpp"
#include "Primitives.hpp"
#include "Psx_common.hpp"
#include <SDL.h>

struct PrimHeader;

class IRenderer
{
public:
    enum class Renderers
    {
        None = 0,
        Software,
        DirectX9,
        OpenGL,
    };

    enum class BitDepth
    {
        e16Bit,
        e8Bit,
        e4Bit,
    };

    EXPORT static IRenderer* GetRenderer();
    EXPORT static void CreateRenderer(Renderers type);
    EXPORT static void FreeRenderer();

    struct PalRecord final
    {
        s16 x = 0;
        s16 y = 0;
        s16 depth = 0;
    };

public:
    virtual ~IRenderer()
    { }
    virtual void Destroy() = 0;
    virtual bool Create(TWindowHandleType window) = 0;
    virtual void Clear(u8 r, u8 g, u8 b) = 0;
    virtual void StartFrame(s32 xOff, s32 yOff) = 0;
    virtual void EndFrame() = 0;
    virtual void BltBackBuffer(const SDL_Rect* pCopyRect, const SDL_Rect* pDst) = 0;
    virtual void OutputSize(s32* w, s32* h) = 0;
    virtual bool UpdateBackBuffer(const void* pPixels, s32 pitch) = 0;
    virtual void CreateBackBuffer(bool filter, s32 format, s32 w, s32 h) = 0;

    virtual void SetTPage(s16 tPage) = 0;

    virtual void SetClip(Prim_PrimClipper& clipper) = 0;
    virtual void SetScreenOffset(Prim_ScreenOffset& offset) = 0;

    virtual void PalFree(const PalRecord& record) = 0; // Use to free textures/pals via a vram point.
    [[nodiscard]] virtual bool PalAlloc(PalRecord& record) = 0;
    virtual void PalSetData(const PalRecord& record, const u8* pPixels) = 0;

    virtual void Upload(BitDepth bitDepth, const PSX_RECT& rect, const u8* pPixels) = 0;

    virtual void LoadExternalCam(const char* path, const unsigned char* key, int keyLength) = 0;

    // FG1/zaplines/blood/hintfly
    virtual void Draw(Prim_Sprt& sprt) = 0;
    virtual void Draw(Prim_GasEffect& gasEffect) = 0;

    // CircularFade/EffectBase
    virtual void Draw(Prim_Tile& tile) = 0;

    // ThrowableTotal
    virtual void Draw(Line_F2& line) = 0;

    // AO: Spark/SnoozeParticle, AE: Spark/SnoozeParticle + ThrowableTotal
    virtual void Draw(Line_G2& line) = 0;

    // SnoozeParticle
    virtual void Draw(Line_G4& line) = 0;

    // MotionDetector
    virtual void Draw(Poly_F3& poly) = 0;

    // MainMenuTransistion
    virtual void Draw(Poly_G3& poly) = 0;

    // PauseMenu, ability ring
    virtual void Draw(Poly_F4& poly) = 0;

    // FG1, Animation, Font, ScreenWave, Water
    virtual void Draw(Poly_FT4& poly) = 0;

    // Fleech (tounge), DeathGas, ColourfulMeter
    virtual void Draw(Poly_G4& poly) = 0;
};

extern IRenderer* gRenderer;