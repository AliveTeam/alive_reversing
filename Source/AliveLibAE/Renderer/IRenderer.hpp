#pragma once

#include "Sys_common.hpp"
#include "Primitives.hpp"
#include <SDL.h>

struct PrimHeader;

class IRenderer
{
public:
    virtual ~IRenderer() {}
    virtual void Destroy() = 0;
    virtual bool Create(TWindowHandleType window) = 0;
    virtual void Clear(BYTE r, BYTE g, BYTE b) = 0;
    virtual void StartFrame(int xOff, int yOff) = 0;
    virtual void EndFrame() = 0;
    virtual void BltBackBuffer(const SDL_Rect* pCopyRect, const SDL_Rect* pDst) = 0;
    virtual void OutputSize(int* w, int* h) = 0;
    virtual bool UpdateBackBuffer(const void* pPixels, int pitch) = 0;
    virtual void CreateBackBuffer(bool filter, int format, int w, int h) = 0;

    virtual void SetTPage(short tPage) = 0;

    virtual void SetClip(Prim_PrimClipper& clipper) = 0;

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
