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

struct VertexData final
{
    f32 x, y, z;
    f32 r, g, b;
    f32 u, v;
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

class OpenGLRenderer final : public IRenderer
{
public:
    void Destroy() override;
    bool Create(TWindowHandleType window) override;
    void Clear(u8 r, u8 g, u8 b) override;
    void StartFrame(s32 xOff, s32 yOff) override;
    void EndFrame() override;
    void BltBackBuffer(const SDL_Rect* pCopyRect, const SDL_Rect* pDst) override;
    void OutputSize(s32* w, s32* h) override;
    bool UpdateBackBuffer(const void* pPixels, s32 pitch) override;
    void CreateBackBuffer(bool filter, s32 format, s32 w, s32 h) override;
    void PalFree(const PalRecord& record) override;
    bool PalAlloc(PalRecord& record) override;
    void PalSetData(const PalRecord& record, const u8* pPixels) override;
    void SetTPage(s16 tPage) override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void SetScreenOffset(Prim_ScreenOffset& offset) override;
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

    void Upload(BitDepth bitDepth, const PSX_RECT& rect, const u8* pPixels) override;

private:
    bool mFrameStarted = false;

    SDL_Window* mWindow = nullptr;
    SDL_GLContext mContext = nullptr;
    GLShader mTextureShader = {};
    u16 mLastTPage = 0;
    bool mWireframe = false;

    // ROZZA STUFF

    GLShader mPassthruShader = {};

    GLuint mPsxFramebufferId[2];
    GLuint mPsxFramebufferTexId[2];

    void InitPsxFramebuffer(int index);
    void CompleteDraw(GLuint target);

    // END ROZZA STUFF

    glm::mat4 m_View = {};

    glm::ivec4 mLastClip = {};

    GLuint mVBO = 0;
    GLuint mIBO = 0;
    GLuint mVAO = 0;

    void SetClipDirect(s32 x, s32 y, s32 width, s32 height);

    void DebugWindow();

    void InitAttributes();
    void DrawTexture(GLuint pTexture, f32 x, f32 y, f32 width, f32 height);
    void DrawTriangles(const VertexData* pVertData, s32 vertSize, const GLuint* pIndData, s32 indSize);
    void DrawLines(const VertexData* pVertData, s32 vertSize, const GLuint* pIndData, s32 indSize);

};
