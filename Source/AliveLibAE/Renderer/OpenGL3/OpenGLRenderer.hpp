#pragma once

#include "../IRenderer.hpp"
#include "../TextureCache.hpp"
#include "../PaletteCache.hpp"

#include <GL/glew.h>
#include <memory>
#include <SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "../relive_lib/data_conversion/rgb_conversion.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"
#include "GLContext.hpp"
#include "GLFramebuffer.hpp"
#include "GLShader.hpp"
#include "GLShaderProgram.hpp"
#include "GLTexture2D.hpp"

#define BATCH_VALUE_UNSET 999

#define GL_PSX_DRAW_MODE_FLAT 0
#define GL_PSX_DRAW_MODE_DEFAULT_FT4 1
#define GL_PSX_DRAW_MODE_CAM 2
#define GL_PSX_DRAW_MODE_FG1 3
#define GL_PSX_DRAW_MODE_GAS 4

#define GL_AVAILABLE_PALETTES 256
#define GL_PALETTE_DEPTH 256

#define GL_CAM_TEXTURE_LIFETIME 300
#define GL_SPRITE_TEXTURE_LIFETIME 300

#define GL_USE_NUM_TEXTURE_UNITS 8

enum class AnimId;

struct VertexData final
{
    s32 x, y;
    u32 r, g, b;
    u32 u, v, texWidth, texHeight;
    u32 drawType, isSemiTrans, isShaded, blendMode;
    u32 paletteIndex, textureUnitIndex;
};

class OpenGLTextureCache final : public TextureCache<GLuint>
{
public:
    void DeleteTexture(GLuint texture) override;
};


class OpenGLRenderer final : public IRenderer
{
public:
    explicit OpenGLRenderer(TWindowHandleType window);
    ~OpenGLRenderer() override;

    void Clear(u8 r, u8 g, u8 b) override;
    void Draw(Prim_GasEffect& gasEffect) override;
    void Draw(Line_G2& line) override;
    void Draw(Line_G4& line) override;
    void Draw(Poly_G3& poly) override;
    void Draw(Poly_FT4& poly) override;
    void Draw(Poly_G4& poly) override;
    void EndFrame() override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void SetTPage(u16 tPage) override;
    void StartFrame() override;
    void ToggleFilterScreen() override;

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

    GLContext mContext;

    u16 mGlobalTPage = 0;

    // ROZZA STUFF

    GLShaderProgram mPassthruShader;
    GLShaderProgram mPassthruIntShader;
    GLShaderProgram mPassthruFilterShader;
    GLShaderProgram mPsxShader;

    GLFramebuffer mPsxFramebuffer;
    GLFramebuffer mPsxFramebufferSecond;
    GLFramebuffer mFilterFramebuffer;

    bool mFramebufferFilter = true;

    u32 mBatchBlendMode = BATCH_VALUE_UNSET;
    std::vector<VertexData> mBatchData;
    std::vector<u32> mBatchIndicies;

    std::vector<VertexData> mScreenWaveData;
    std::vector<u32> mScreenWaveIndicies;

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
    u16 GetTPageBlendMode(u16 tPage);
    void InvalidateBatch();
    void PushLines(const VertexData* vertices, int count);
    void PushScreenWaveData(const VertexData* vertices);
    void PushVertexData(VertexData* pVertData, int count, GLuint textureId = 0);
    void RenderScreenWave();
    void SetupBlendMode(u16 blendMode);
    void UpdateFilterFramebuffer();
    
    u32 PreparePalette(AnimationPal& pCache);
    u32 PrepareTextureFromAnim(Animation& anim);
    u32 PrepareTextureFromPoly(Poly_FT4& poly);

    // END ROZZA STUFF

    GLuint mVAO = 0;

    std::unique_ptr<GLTexture2D> mPaletteTexture;
    PaletteCache mPaletteCache;

    OpenGLTextureCache mTextureCache;

    void DebugWindow();
};
