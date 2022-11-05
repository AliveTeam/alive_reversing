#pragma once

#include "IRenderer.hpp"

#include <GL/glew.h>
#include <SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "GLShader.hpp"
#include "../relive_lib/data_conversion/rgb_conversion.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"

#define BATCH_VALUE_UNSET 999

#define GL_PSX_DRAW_MODE_FLAT 0
#define GL_PSX_DRAW_MODE_DEFAULT_FT4 1
#define GL_PSX_DRAW_MODE_CAM 2
#define GL_PSX_DRAW_MODE_FG1 3
#define GL_PSX_DRAW_MODE_GAS 4

#define GL_FRAMEBUFFER_PSX_WIDTH 640
#define GL_FRAMEBUFFER_PSX_HEIGHT 240

#define GL_FRAMEBUFFER_FILTER_WIDTH 640
#define GL_FRAMEBUFFER_FILTER_HEIGHT 480

#define GL_AVAILABLE_PALETTES 256
#define GL_PALETTE_DEPTH 256

#define GL_CAM_TEXTURE_LIFETIME 300
#define GL_SPRITE_TEXTURE_LIFETIME 300

#define GL_USE_NUM_TEXTURE_UNITS 8

// We reserve space for X amount of VertexData structs for the batching (to
// lower the need to realloc per Paul's request)
//
// It's quite high because the game draws like 260 quads for the splines
#define GL_RESERVE_QUADS 300

enum class AnimId;

struct VertexData final
{
    s32 x, y;
    u32 r, g, b;
    u32 u, v, texWidth, texHeight;
    u32 drawType, isSemiTrans, isShaded, blendMode;
    u32 paletteIndex, textureUnitIndex;
};

struct TextureCache final
{
    GLuint mTextureID;
};

struct PaletteCache final
{
    u32 mPalTextureID;
    PSX_Point mPalPoint;
    s16 mPalDepth;
    RGBA32 mPalData[256];
};

struct TextureAndUniqueResId final
{
    GLuint mTextureId = 0;
    u32 mUniqueResId = 0;
};

class OpenGLRenderer final : public IRenderer
{
public:
    void Clear(u8 r, u8 g, u8 b) override;
    bool Create(TWindowHandleType window) override;
    void Destroy() override;
    void Draw(Prim_Sprt& sprt) override;
    void Draw(Prim_GasEffect& gasEffect) override;
    void Draw(Prim_Tile& tile) override;
    void Draw(Line_F2& line) override;
    void Draw(Line_G2& line) override;
    void Draw(Line_G4& line) override;
    void Draw(Poly_F3& poly) override;
    void Draw(Poly_G3& poly) override;
    void Draw(Poly_F4& poly) override;
    void Draw(Poly_FT4& poly) override;
    void Draw(Poly_G4& poly) override;
    void EndFrame() override;
    void OutputSize(s32* w, s32* h) override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void SetScreenOffset(Prim_ScreenOffset& offset) override;
    void SetTPage(u16 tPage) override;
    void StartFrame(s32 xOff, s32 yOff) override;
    void ToggleFilterScreen() override;
    void ToggleKeepAspectRatio() override;

private:
    struct Stats final
    {
        u32 mCamUploadCount = 0;
        u32 mFg1UploadCount = 0;
        u32 mAnimUploadCount = 0;
        u32 mPalUploadCount = 0;
        u32 mFontUploadCount = 0;
        u32 mInvalidationsCount = 0;

        void Reset()
        {
            mCamUploadCount = 0;
            mFg1UploadCount = 0;
            mAnimUploadCount = 0;
            mPalUploadCount = 0;
            mFontUploadCount = 0;
            mInvalidationsCount = 0;
        }
    };
    Stats mStats;

    bool mFrameStarted = false;

    SDL_Window* mWindow = nullptr;
    SDL_GLContext mContext = nullptr;
    u16 mGlobalTPage = 0;

    // ROZZA STUFF

    GLShader mPassthruShader = {};
    GLShader mPassthruFilterShader = {};
    GLShader mPsxShader = {};

    GLuint mPsxFramebufferId = 0;
    GLuint mPsxFramebufferTexId = 0;

    GLuint mFilterFramebufferId = 0;
    GLuint mFilterFramebufferTexId = 0;

    bool mFramebufferFilter = true;
    bool mKeepAspectRatio = true;
    s32 mOffsetX = 0;
    s32 mOffsetY = 0;
    
    u32 mBatchBlendMode = BATCH_VALUE_UNSET;
    std::vector<VertexData> mBatchData;
    std::vector<u32> mBatchIndicies;

    GLuint mPaletteTextureId = 0;
    struct PalCacheEntry final
    {
        u32 mIndex = 0;
        bool mInUse = false;
    };
    std::map<u32, PalCacheEntry> mPaletteCache;

    GLuint mCurGasTextureId = 0;

    GLuint mCurCamTextureId = 0;
    std::vector<GLuint> mCurFG1TextureIds;
    GLint mFG1Units[4] = {3, 4, 5, 6};

    std::vector<GLuint> mBatchTextureIds;
    GLint mTextureUnits[GL_USE_NUM_TEXTURE_UNITS];

    GLuint CreateCachedTexture(u32 uniqueId, u32 lifetime);
    void CreateFramebuffer(GLuint* outFramebufferId, GLuint* outTextureId, s32 width, s32 height);
    void DecreaseResourceLifetimes();
    void DrawFramebufferToScreen(s32 x, s32 y, s32 width, s32 height);
    GLuint GetCachedTextureId(u32 uniqueId, s32 bump = 0);
    SDL_Rect GetTargetDrawRect();
    u16 GetTPageBlendMode(u16 tPage);
    void InvalidateBatch();
    void PushLines(const VertexData* vertices, int count);
    void PushVertexData(VertexData* pVertData, int count, GLuint textureId = 0);
    void SetupBlendMode(u16 blendMode);
    void UpdateFilterFramebuffer();
    
    u32 PreparePalette(AnimationPal& pCache);
    u32 HashPalette(const AnimationPal* pPal);
    u32 PrepareTextureFromAnim(Animation& anim);
    u32 PrepareTextureFromPoly(Poly_FT4& poly);

    // END ROZZA STUFF

    GLuint mVAO = 0;

    struct CachedTexture final
    {
        GLuint mTextureId = 0;
        s32 mLifetime = 0;
    };
    std::map<u32, CachedTexture> mTextureCache;

    void DebugWindow();
};
