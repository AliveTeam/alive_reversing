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

struct VertexData
{
    float x, y, z;
    float r, g, b;
    float u, v;
};

struct RGBAPixel
{
    u8 R, G, B, A;
};

struct TextureCache
{
    GLuint mTextureID;
    PSX_RECT mVramRect;
    PSX_Point mPalXY;
    IRenderer::BitDepth mBitDepth;
    s32 mPalNormMulti; // For special case when converted bitmap data is normalized.
    bool mIsFG1;
    bool mIgnoreColor;
    PSX_Point mUvOffset;
};

struct PaletteCache
{
    u32 mPalTextureID;
    PSX_Point mPalPoint;
    short mPalDepth;
    RGBAPixel mPalData[256];
};

class OpenGLRenderer : public IRenderer
{
public:
    void Destroy() override;
    bool Create(TWindowHandleType window) override;
    void Clear(BYTE r, BYTE g, BYTE b) override;
    void StartFrame(s32 xOff, s32 yOff) override;
    void EndFrame() override;
    void BltBackBuffer(const SDL_Rect* pCopyRect, const SDL_Rect* pDst) override;
    void OutputSize(s32* w, s32* h) override;
    bool UpdateBackBuffer(const void* pPixels, s32 pitch) override;
    void CreateBackBuffer(bool filter, s32 format, s32 w, s32 h) override;
    void PalFree(const PalRecord& record) override;
    bool PalAlloc(PalRecord& record) override;
    void PalSetData(const PalRecord& record, const BYTE* pPixels) override;
    void SetTPage(short tPage) override;
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

    void Upload(BitDepth bitDepth, const PSX_RECT& rect, const BYTE* pPixels) override;
private:
    SDL_Window* mWindow = nullptr;
    SDL_GLContext mContext = nullptr;
    GLShader mTextureShader = {};
    WORD mLastTPage = 0;
    bool mWireframe = false;

    glm::mat4 m_View = {};

    glm::ivec4 mLastClip = {};

    GLuint mVBO = 0;
    GLuint mIBO = 0;
    GLuint mVAO = 0;

    glm::mat4 GetMVP();
    glm::mat4 GetMVP(float x, float y, float width, float height);

    void SetClipDirect(s32 x, s32 y, s32 width, s32 height);

    void DebugWindow();

    void InitAttributes();
    void DrawTexture(GLuint pTexture, float x, float y, float width, float height);
    void DrawTriangles(const VertexData* pVertData, s32 vertSize, const GLuint* pIndData, s32 indSize);
    void DrawLines(const VertexData* pVertData, s32 vertSize, const GLuint* pIndData, s32 indSize);

    void RenderBackground();
};

void HackSetBackground(const char* path);