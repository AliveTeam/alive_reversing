#pragma once

#include "IRenderer.hpp"

#include <GL/glew.h>
#include <SDL_opengl.h>

#define GLM_FORCE_SILENT_WARNINGS

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "GLShader.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/ResourceManagerWrapper.hpp"

enum class AnimId;

struct VertexData final
{
    s32 x, y;
    u32 r, g, b;
    u32 u, v;
    u32 drawType, isSemiTrans, isShaded;
};

struct RGBAPixel final
{
    u8 R, G, B, A;
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
    RGBAPixel mPalData[256];
};

struct TextureAndUniqueResId final
{
    GLuint mTextureId = 0;
    u32 mUniqueResId = 0;
};

class OpenGLRenderer final : public IRenderer
{
public:
    void BltBackBuffer(const SDL_Rect* pCopyRect, const SDL_Rect* pDst) override;
    void Clear(u8 r, u8 g, u8 b) override;
    bool Create(TWindowHandleType window) override;
    void CreateBackBuffer(bool filter, s32 format, s32 w, s32 h) override;
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
    bool PalAlloc(PalRecord& record) override;
    void PalFree(const PalRecord& record) override;
    void PalSetData(const PalRecord& record, const u8* pPixels) override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void SetScreenOffset(Prim_ScreenOffset& offset) override;
    void SetTPage(u16 tPage) override;
    void StartFrame(s32 xOff, s32 yOff) override;
    bool UpdateBackBuffer(const void* pPixels, s32 pitch) override;
    void Upload(BitDepth bitDepth, const PSX_RECT& rect, const u8* pPixels) override;

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

    SDL_Rect mBlitRect = {};

    GLShader mPassthruShader = {};
    GLShader mPsxShader = {};

    GLuint mPsxFramebufferId = 0;
    GLuint mPsxFramebufferTexId = 0;

    u32 mBatchBlendMode = 999;
    std::vector<VertexData> mBatchData = {};
    GLenum mBatchDrawMode = 999;
    GLuint mBatchPriTexId = 0;
    u32 mBatchPriTexWidth = 0;
    u32 mBatchPriTexHeight = 0;
    GLuint mBatchSecTexId = 0;

    GLuint mGasTextureId = 0;
    TextureAndUniqueResId mCamTexture;
    TextureAndUniqueResId mFg1Texture; // TODO: should probably be 4 of these
    TextureAndUniqueResId mFontTexture;

    void DrawFramebufferToScreen(s32 x, s32 y, s32 width, s32 height);
    u16 GetTPageBlendMode(u16 tPage);
    void InvalidateBatch();
    void PushVertexData(GLenum mode, const VertexData* pVertData, int count, u32 blendMode, GLuint priTexId = 0, u32 priTexWidth = 0, u32 priTexHeight = 0, GLuint secTexId = 0);
    void SetupBlendMode(u16 blendMode);
    
    u32 PreparePalette(AnimationPal& pCache);
    u32 PrepareTextureFromAnim(Animation& anim);
    u32 PrepareTextureFromPoly(Poly_FT4& poly);
    void FreeUnloadedAnimTextures();

    // END ROZZA STUFF

    GLuint mVAO = 0;

    s32 mFrameNumber = 0;
    struct LastUsedFrame final
    {
        GLuint mTextureId = 0;
        s32 mLastUsedFrame = 0;
    };
    std::map<u32, LastUsedFrame> mTextureCache;

    void DebugWindow();
};
