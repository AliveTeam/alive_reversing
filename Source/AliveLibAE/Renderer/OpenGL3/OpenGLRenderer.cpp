#include <algorithm>

#include "../relive_lib/data_conversion/rgb_conversion.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Animation.hpp"
#include "../../Compression.hpp"
#include "../../Font.hpp"
#include "../AliveLibCommon/FatalError.hpp"
#include "GLDebug.hpp"
#include "GLFramebuffer.hpp"
#include "GLShader.hpp"
#include "GLShaderProgram.hpp"
#include "GLTexture2D.hpp"
#include "OpenGLRenderer.hpp"
#include "../AliveLibCommon/BaseGameAutoPlayer.hpp"

extern bool gDDCheat_FlyingEnabled;
namespace AO
{
    extern bool gDDCheat_FlyingEnabled;
}

#define GL_TO_IMGUI_TEX(v) *reinterpret_cast<ImTextureID*>(&v)


static bool gRenderEnable_Batching = true;

static bool gRenderEnable_GAS = true;
static bool gRenderEnable_FT4 = true;
static bool gRenderEnable_G4 = true;
static bool gRenderEnable_G3 = true;
static bool gRenderEnable_G2 = true;

u32 OpenGLRenderer::PreparePalette(AnimationPal& pCache)
{
    const PaletteCache::AddResult addRet = mPaletteCache.Add(pCache);

    if (addRet.mAllocated)
    {
        // Write palette data
        mPaletteTexture.LoadSubImage(0, addRet.mIndex, GL_PALETTE_DEPTH, 1, pCache.mPal);

        mStats.mPalUploadCount++;
    }

    return addRet.mIndex;
}

GLTexture2D OpenGLRenderer::PrepareTextureFromAnim(Animation& anim)
{
    const AnimResource& r = anim.mAnimRes;

    GLTexture2D texture = mTextureCache.GetCachedTexture(r.mUniqueId.Id(), GL_SPRITE_TEXTURE_LIFETIME);

    if (!texture.IsValid())
    {
        GLTexture2D animTex(r.mTgaPtr->mWidth, r.mTgaPtr->mHeight, GL_RED);

        animTex.LoadImage(r.mTgaPtr->mPixels.data());

        texture = mTextureCache.Add(r.mUniqueId.Id(), GL_SPRITE_TEXTURE_LIFETIME, std::move(animTex));

        mStats.mAnimUploadCount++;
    }

    return texture;
}

GLTexture2D OpenGLRenderer::PrepareTextureFromPoly(Poly_FT4& poly)
{
    GLTexture2D texture;

    if (poly.mFg1)
    {
        texture = mTextureCache.GetCachedTexture(poly.mFg1->mUniqueId.Id(), GL_CAM_TEXTURE_LIFETIME);

        if (!texture.IsValid())
        {
            GLTexture2D fg1Tex(poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight, GL_RGBA);

            fg1Tex.LoadImage(poly.mFg1->mImage.mPixels->data());

            texture = mTextureCache.Add(poly.mFg1->mUniqueId.Id(), GL_CAM_TEXTURE_LIFETIME, std::move(fg1Tex));

            mStats.mFg1UploadCount++;
        }
    }
    else if (poly.mCam)
    {
        texture = mTextureCache.GetCachedTexture(poly.mCam->mUniqueId.Id(), GL_CAM_TEXTURE_LIFETIME);

        if (!texture.IsValid())
        {
            GLTexture2D camTex(poly.mCam->mData.mWidth, poly.mCam->mData.mHeight, GL_RGBA);

            camTex.LoadImage(poly.mCam->mData.mPixels->data());

            texture = mTextureCache.Add(poly.mCam->mUniqueId.Id(), GL_CAM_TEXTURE_LIFETIME, std::move(camTex));

            mStats.mCamUploadCount++;
        }
    }
    else if (poly.mAnim)
    {
        return PrepareTextureFromAnim(*poly.mAnim);
    }
    else if (poly.mFont)
    {
        texture = mTextureCache.GetCachedTexture(poly.mFont->field_C_resource_id.mUniqueId.Id(), GL_SPRITE_TEXTURE_LIFETIME);

        if (!texture.IsValid())
        {
            std::shared_ptr<TgaData> pTga = poly.mFont->field_C_resource_id.mTgaPtr;

            GLTexture2D fontTex(pTga->mWidth, pTga->mHeight, GL_RED);

            fontTex.LoadImage(pTga->mPixels.data());

            texture = mTextureCache.Add(poly.mFont->field_C_resource_id.mUniqueId.Id(), GL_SPRITE_TEXTURE_LIFETIME, std::move(fontTex));

            mStats.mFontUploadCount++;
        }
    }

    return texture;
}

OpenGLRenderer::OpenGLRenderer(TWindowHandleType window)
    : IRenderer(window),
    mContext(window),
    mPsxFramebuffer(kPsxFramebufferWidth, kPsxFramebufferHeight),
    mPsxFbFramebuffer(kPsxFramebufferWidth, kPsxFramebufferHeight),
    mFilterFramebuffer(kTargetFramebufferWidth, kTargetFramebufferHeight),
    mPaletteCache(GL_AVAILABLE_PALETTES)
{
    mWindow = window;

    // Create and bind the VAO, and never touch it again! Wahey.
    GL_VERIFY(glGenVertexArrays(1, &mVAO));
    GL_VERIFY(glBindVertexArray(mVAO));

    // FIXME: Temp - init palette here for now
    const static RGBA32 black[256] = {};

    mPaletteTexture = GLTexture2D(GL_PALETTE_DEPTH, GL_AVAILABLE_PALETTES, GL_RGBA);

    for (s32 i = 0; i < GL_AVAILABLE_PALETTES; i++)
    {
        mPaletteTexture.LoadSubImage(0, i, GL_PALETTE_DEPTH, 1, black);
    }

    // Load shaders
    GLShader passthruVS(gShader_PassthruVSH, GL_VERTEX_SHADER);
    GLShader passthruFS(gShader_PassthruFSH, GL_FRAGMENT_SHADER);
    GLShader passthruFilterFS(gShader_PassthruFilterFSH, GL_FRAGMENT_SHADER);
    GLShader psxVS(gShader_PsxVSH, GL_VERTEX_SHADER);
    GLShader psxFS(gShader_PsxFSH, GL_FRAGMENT_SHADER);

    mPassthruShader.LinkShaders(passthruVS, passthruFS);
    mPassthruFilterShader.LinkShaders(passthruVS, passthruFilterFS);
    mPsxShader.LinkShaders(psxVS, psxFS);

    // ROZZA Blending
    GL_VERIFY(glEnable(GL_BLEND));
    GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // Init batch vectors
    mCurFG1Textures.reserve(4);
    mBatchData.reserve(kReserveFT4QuadCount * 4);
    mBatchIndicies.reserve(kReserveFT4QuadCount * 6);
    mBatchTextures.reserve(GL_USE_NUM_TEXTURE_UNITS);
    mFbData.reserve(kReserveScreenWaveQuadCount * 4);
    mFbIndicies.reserve(kReserveScreenWaveQuadCount * 6);

    // Init array we pass to texture uniform to specify the units we're using
    // which is the number of units starting at GL_TEXTURE7
    for (int i = 0; i < GL_USE_NUM_TEXTURE_UNITS; i++)
    {
        mTextureUnits[i] = i + 7;
    }
}

OpenGLRenderer::~OpenGLRenderer()
{
    mTextureCache.Clear();

    GL_VERIFY(glUseProgram(0));

    GLFramebuffer::BindScreenAsTarget(mWindow);
}

void OpenGLRenderer::Clear(u8 r, u8 g, u8 b)
{
    if (!mFrameStarted || SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED)
    {
        return;
    }

    GLboolean scissoring;

    GL_VERIFY(glGetBooleanv(GL_SCISSOR_TEST, &scissoring));

    // We clear the screen framebuffer here
    GLFramebuffer::BindScreenAsTarget(mWindow);

    if (scissoring)
    {
        GL_VERIFY(glDisable(GL_SCISSOR_TEST));
    }

    GL_VERIFY(glClearColor((f32) r, (f32) g, (f32) b, 1.0f));
    GL_VERIFY(glClear(GL_COLOR_BUFFER_BIT));

    // Set back to the PSX framebuffer
    mPsxFramebuffer.BindAsTarget();

    if (scissoring)
    {
        GL_VERIFY(glEnable(GL_SCISSOR_TEST));
    }
}

void OpenGLRenderer::Draw(Prim_GasEffect& gasEffect)
{
    if (!gRenderEnable_GAS)
    {
        return;
    }

    if (!mCurGasTexture.IsValid())
    {
        mCurGasTexture = GLTexture2D(kPsxFramebufferWidth, kPsxFramebufferHeight, GL_RGB);
    }

    if (gasEffect.pData == nullptr)
    {
        return;
    }

    const f32 x = static_cast<f32>(gasEffect.x);
    const f32 y = static_cast<f32>(gasEffect.y);
    const f32 w = static_cast<f32>(gasEffect.w);
    const f32 h = static_cast<f32>(gasEffect.h);

    const f32 r = 127;
    const f32 g = 127;
    const f32 b = 127;

    const f32 gasWidth = static_cast<f32>(gasEffect.w - gasEffect.x) / 4;
    const f32 gasHeight = static_cast<f32>(gasEffect.h - gasEffect.y) / 2;

    const bool isSemiTrans = true;
    const bool isShaded = true;
    const u32 blendMode = (u32) TPageAbr::eBlend_0;

    mCurGasTexture.LoadSubImage(0, 0, static_cast<GLsizei>(gasWidth), static_cast<GLsizei>(gasHeight), gasEffect.pData, GL_UNSIGNED_SHORT_5_6_5);

    PsxVertexData verts[4] = {
        { x, y, r, g, b, 0.0f, 0.0f, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, blendMode, 0, 0},
        { w, y, r, g, b, gasWidth, 0.0f, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, blendMode, 0, 0},
        { x, h, r, g, b, 0.0f, gasHeight, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, blendMode, 0, 0},
        { w, h, r, g, b, gasWidth, gasHeight, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, blendMode, 0, 0}};
    
    PushVertexData(verts, 4);
}

void OpenGLRenderer::Draw(Line_G2& line)
{
    if (!gRenderEnable_G2)
    {
        return;
    }

    bool isSemiTrans = GetPolyIsSemiTrans(&line);
    bool isShaded = true;
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    PsxVertexData verts[2] = {
        { (f32) X0(&line), (f32) Y0(&line), (f32) R0(&line), (f32) G0(&line), (f32) B0(&line), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0},
        { (f32) X1(&line), (f32) Y1(&line), (f32) R1(&line), (f32) G1(&line), (f32) B1(&line), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0}};

    PushLines(verts, 2);
}

void OpenGLRenderer::Draw(Line_G4& line)
{
    if (!gRenderEnable_G4)
    {
        return;
    }

    bool isSemiTrans = GetPolyIsSemiTrans(&line);
    bool isShaded = true;
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    PsxVertexData verts[4] = {
        { (f32) X0(&line), (f32) Y0(&line), (f32) R0(&line), (f32) G0(&line), (f32) B0(&line), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0},
        { (f32) X1(&line), (f32) Y1(&line), (f32) R1(&line), (f32) G1(&line), (f32) B1(&line), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0},
        { (f32) X2(&line), (f32) Y2(&line), (f32) R2(&line), (f32) G2(&line), (f32) B2(&line), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0},
        { (f32) X3(&line), (f32) Y3(&line), (f32) R3(&line), (f32) G3(&line), (f32) B3(&line), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0}};

    PushLines(verts, 4);
}

void OpenGLRenderer::Draw(Poly_G3& poly)
{
    if (!gRenderEnable_G3)
    {
        return;
    }

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    bool isShaded = true;
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    PsxVertexData verts[3] = {
        { (f32) X0(&poly), (f32) Y0(&poly), (f32) R0(&poly), (f32) G0(&poly), (f32) B0(&poly), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0},
        { (f32) X1(&poly), (f32) Y1(&poly), (f32) R1(&poly), (f32) G1(&poly), (f32) B1(&poly), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0},
        { (f32) X2(&poly), (f32) Y2(&poly), (f32) R2(&poly), (f32) G2(&poly), (f32) B2(&poly), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0}};

    PushVertexData(verts, 3);
}

void OpenGLRenderer::Draw(Poly_FT4& poly)
{
    if (!gRenderEnable_FT4)
    {
        return;
    }

    GLTexture2D texture = PrepareTextureFromPoly(poly);

    f32 r = (f32) poly.mBase.header.rgb_code.r;
    f32 g = (f32) poly.mBase.header.rgb_code.g;
    f32 b = (f32) poly.mBase.header.rgb_code.b;

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    bool isShaded = GetPolyIsShaded(&poly);
    u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

    if (poly.mFg1)
    {
        PsxVertexData verts[4] = {
            { (f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, r, g, b, 0.0f, 0.0f, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            { (f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, r, g, b, kPsxFramebufferWidth, 0.0f, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            { (f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, r, g, b, 0.0f, kPsxFramebufferHeight, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, blendMode, 0, 0},
            { (f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, r, g, b, kPsxFramebufferWidth, kPsxFramebufferHeight, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, blendMode, 0, 0}};

        PushVertexData(verts, 4, texture);
    }
    else if (poly.mCam)
    {
        PsxVertexData verts[4] = {
            { (f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, r, g, b, 0.0f, 0.0f, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, blendMode, 0, 0},
            { (f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, r, g, b, kPsxFramebufferWidth, 0.0f, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, blendMode, 0, 0},
            { (f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, r, g, b, 0.0f, kPsxFramebufferHeight, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, blendMode, 0, 0},
            { (f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, r, g, b, kPsxFramebufferWidth, kPsxFramebufferHeight, kPsxFramebufferWidth, kPsxFramebufferHeight, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, blendMode, 0, 0}};

        PushVertexData(verts, 4, texture);
    }
    else if (poly.mAnim)
    {
        const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);
        std::shared_ptr<TgaData> pTga = poly.mAnim->mAnimRes.mTgaPtr;

        const u32 palIndex = PreparePalette(*poly.mAnim->mAnimRes.mCurPal);

        f32 u0 = static_cast<f32>(pHeader->mSpriteSheetX);
        f32 v0 = static_cast<f32>(pHeader->mSpriteSheetY);

        f32 u1 = u0 + pHeader->mWidth;
        f32 v1 = v0 + pHeader->mHeight;

        if (poly.mFlipX)
        {
            std::swap(u0, u1);
        }

        if (poly.mFlipY)
        {
            std::swap(v1, v0);
        }

        PsxVertexData verts[4] = {
            { (f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, r, g, b, u0, v0, (f32) pTga->mWidth, (f32) pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
            { (f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, r, g, b, u1, v0, (f32) pTga->mWidth, (f32) pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
            { (f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, r, g, b, u0, v1, (f32) pTga->mWidth, (f32) pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
            { (f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, r, g, b, u1, v1, (f32) pTga->mWidth, (f32) pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0}};
        
        PushVertexData(verts, 4, texture);
    }
    else if (poly.mFont)
    {
        FontResource& fontRes = poly.mFont->field_C_resource_id;
        std::shared_ptr<TgaData> pTga = fontRes.mTgaPtr;

        auto pPal = fontRes.mCurPal;
        const u32 palIndex = PreparePalette(*pPal);

        f32 u0 = U0(&poly);
        f32 v0 = V0(&poly);

        f32 u1 = U3(&poly);
        f32 v1 = V3(&poly);

        PsxVertexData verts[4] = {
            { (f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, r, g, b, u0, v0, (f32) pTga->mWidth, (f32) pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
            { (f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, r, g, b, u1, v0, (f32) pTga->mWidth, (f32) pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
            { (f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, r, g, b, u0, v1, (f32) pTga->mWidth, (f32) pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0},
            { (f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, r, g, b, u1, v1, (f32) pTga->mWidth, (f32) pTga->mHeight, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, blendMode, palIndex, 0}};

        PushVertexData(verts, 4, texture);
    }
    else
    {
        // ScreenWave (Bell Song framebuffer effect)

        PassthruVertexData verts[4] = {
            { (f32) X0(&poly), (f32) Y0(&poly), (f32) U0(&poly), (f32) V0(&poly) },
            { (f32) X1(&poly), (f32) Y1(&poly), (f32) U1(&poly), (f32) V1(&poly) },
            { (f32) X2(&poly), (f32) Y2(&poly), (f32) U2(&poly), (f32) V2(&poly) },
            { (f32) X3(&poly), (f32) Y3(&poly), (f32) U3(&poly), (f32) V3(&poly) }
        };

        PushFramebufferVertexData(verts, 4);
    }
}

void OpenGLRenderer::Draw(Poly_G4& poly)
{
    if (!gRenderEnable_G4)
    {
        return;
    }

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    bool isShaded = true;
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    PsxVertexData verts[4] = {
        { (f32) X0(&poly), (f32) Y0(&poly), (f32) R0(&poly), (f32) G0(&poly), (f32) B0(&poly), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0},
        { (f32) X1(&poly), (f32) Y1(&poly), (f32) R1(&poly), (f32) G1(&poly), (f32) B1(&poly), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0},
        { (f32) X2(&poly), (f32) Y2(&poly), (f32) R2(&poly), (f32) G2(&poly), (f32) B2(&poly), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0},
        { (f32) X3(&poly), (f32) Y3(&poly), (f32) R3(&poly), (f32) G3(&poly), (f32) B3(&poly), 0.0f, 0.0f, 0.0f, 0.0f, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, blendMode, 0, 0}};

    PushVertexData(verts, 4);
}

void OpenGLRenderer::EndFrame()
{
    // Ensure any remaining data is drawn
    InvalidateBatch();

    // FIXME: Only do this if we actually have FB polys to draw
    //        ( call HasFramebufferPolysToDraw() )
    RenderFramebufferPolys();

    // Always decrease resource lifetimes regardless of drawing to prevent
    // memory leaks
    DecreaseResourceLifetimes();

    // The rest of this method writes to the screen, we early return now
    // because:
    //     Sometimes EndFrame is called before StartFrame
    //     When minimised, rendering to the screen blows up Intel HD 2000
    if (!mFrameStarted || SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED)
    {
        return;
    }

    // Draw the final composed framebuffer to the screen
    SDL_Rect drawRect = GetTargetDrawRect();

    GL_VERIFY(glDisable(GL_SCISSOR_TEST));
    DrawFramebufferToScreen(
        drawRect.x,
        drawRect.y,
        drawRect.w,
        drawRect.h);

    // Do ImGui


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(mWindow);
    ImGui::NewFrame();

    if (gDDCheat_FlyingEnabled || AO::gDDCheat_FlyingEnabled || GetGameAutoPlayer().IsPlaying())
    {
        DebugWindow();
    }

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Throw away any errors caused by ImGui - this is necessary for AMD GPUs
    // (AMD Radeon HD 7310 with driver 8.982.10.5000)
    glGetError();

    // Render end
    SDL_GL_SwapWindow(mWindow);

    mFrameStarted = false;

    // Set the framebuffer target back to the PSX framebuffer
    mPsxFramebuffer.BindAsTarget();
}

void OpenGLRenderer::SetClip(Prim_PrimClipper& clipper)
{
    SDL_Rect rect;
    rect.x = clipper.field_C_x;
    rect.y = clipper.field_E_y;
    rect.w = clipper.mBase.header.mRect.w;
    rect.h = clipper.mBase.header.mRect.h;

    InvalidateBatch();

    if (rect.x == 0 && rect.y == 0 && rect.w == 1 && rect.h == 1)
    {
        GL_VERIFY(glDisable(GL_SCISSOR_TEST));
        return;
    }

    GL_VERIFY(glEnable(GL_SCISSOR_TEST));
    GL_VERIFY(glScissor(rect.x, kPsxFramebufferHeight - rect.y - rect.h, rect.w, rect.h));
}

void OpenGLRenderer::SetTPage(u16 tPage)
{
    mGlobalTPage = tPage;
}

void OpenGLRenderer::StartFrame()
{
    if (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED)
    {
        return;
    }

    mStats.Reset();

    mFrameStarted = true;

    // Set offsets for the screen (this is for the screen shake effect)
    mOffsetX = 0;
    mOffsetY = 0;

    // Add the screen itself into the framebuffer polys, so it's always
    // drawn first
    static const PassthruVertexData screenVerts[4] = {
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, kPsxFramebufferHeight, 0.0f, kPsxFramebufferHeight },
        { kPsxFramebufferWidth, 0.0f, kPsxFramebufferWidth, 0.0f },
        { kPsxFramebufferWidth, kPsxFramebufferHeight, kPsxFramebufferWidth, kPsxFramebufferHeight }
    };

    PushFramebufferVertexData(screenVerts, 4);

    // Always render to PSX framebuffer
    mPsxFramebuffer.BindAsTarget();
}

void OpenGLRenderer::ToggleFilterScreen()
{
    mFramebufferFilter = !mFramebufferFilter;
}

// ROZZA FRAMEBUFFER STUFF

void OpenGLRenderer::DecreaseResourceLifetimes()
{
    mFbData.clear();
    mFbIndicies.clear();

    mTextureCache.DecreaseResourceLifetimes();

    mPaletteCache.ResetUseFlags();
}

void OpenGLRenderer::DrawFramebufferToScreen(s32 x, s32 y, s32 width, s32 height)
{
    // Ensure blend mode is back to normal alpha compositing
    GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_VERIFY(glBlendEquation(GL_FUNC_ADD));

    // Set up the texture we're going to draw...
    f32 texWidth = 0;
    f32 texHeight = 0;

    // FIXME: Do the filter in one go as an enum in the shader instead
    //        of an extra pass here
    if (mFramebufferFilter)
    {
        UpdateFilterFramebuffer();

        mFilterFramebuffer.BindAsSourceTextureTo(GL_TEXTURE0);

        texWidth = (f32) mFilterFramebuffer.GetWidth();
        texHeight = (f32) mFilterFramebuffer.GetHeight();
    }
    else
    {
        mPsxFbFramebuffer.BindAsSourceTextureTo(GL_TEXTURE0);
        texWidth = (f32) mPsxFbFramebuffer.GetWidth();
        texHeight = (f32) mPsxFbFramebuffer.GetHeight();
    }

    // Set up VBOs
    GLuint drawVboId = 0;
    GLuint uvVboId = 0;

    GLfloat drawVertices[] = {
        (f32) x, (f32) y,
        (f32) x, (f32) (y + height),
        (f32) (x + width), (f32) y,

        (f32) (x + width), (f32) y,
        (f32) x, (f32) (y + height),
        (f32) (x + width), (f32) (y + height)};
    GLfloat uvVertices[] = {
        0.0f, texHeight,
        0.0f, 0.0f,
        texWidth, texHeight,

        texWidth, texHeight,
        0.0f, 0.0f,
        texWidth, 0.0f};

    GL_VERIFY(glGenBuffers(1, &drawVboId));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, drawVboId));
    GL_VERIFY(
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(drawVertices),
            drawVertices,
            GL_STREAM_DRAW));

    GL_VERIFY(glGenBuffers(1, &uvVboId));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, uvVboId));
    GL_VERIFY(
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(uvVertices),
            uvVertices,
            GL_STREAM_DRAW));

    // Bind framebuffers and draw
    s32 viewportW, viewportH;

    GLFramebuffer::BindScreenAsTarget(mWindow, &viewportW, &viewportH);

    mPassthruShader.Use();

    mPassthruShader.Uniform1i("texTextureData", 0);
    mPassthruShader.UniformVec2("vsViewportSize", (f32) viewportW, (f32) viewportH);
    mPassthruShader.Uniform1i("fsFlipUV", false);
    mPassthruShader.UniformVec2("fsTexSize", texWidth, texHeight);

    GL_VERIFY(glEnableVertexAttribArray(0));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, drawVboId));
    GL_VERIFY(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));

    GL_VERIFY(glEnableVertexAttribArray(1));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, uvVboId));
    GL_VERIFY(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0));

    GL_VERIFY(glDrawArrays(GL_TRIANGLES, 0, 6));

    GL_VERIFY(glDeleteBuffers(1, &drawVboId));
    GL_VERIFY(glDeleteBuffers(1, &uvVboId));

    GL_VERIFY(glDisableVertexAttribArray(0));
    GL_VERIFY(glDisableVertexAttribArray(1));
}

void OpenGLRenderer::UpdateFilterFramebuffer()
{
    // Set up VBOs
    static GLuint drawVboId = 0;
    static GLuint uvVboId = 0;

    if (drawVboId == 0)
    {
        GLfloat drawVertices[] = {
            0.0f, 0.0f,
            0.0f, kTargetFramebufferHeight,
            kTargetFramebufferWidth, 0.0,

            kTargetFramebufferWidth, 0.0f,
            0.0f, kTargetFramebufferHeight,
            kTargetFramebufferWidth, kTargetFramebufferHeight};

        GLfloat uvVertices[] = {
            0.0f, kPsxFramebufferHeight,
            0.0f, 0.0f,
            kPsxFramebufferWidth, kPsxFramebufferHeight,

            kPsxFramebufferWidth, kPsxFramebufferHeight,
            0.0f, 0.0f,
            kPsxFramebufferWidth, 0.0f};

        GL_VERIFY(glGenBuffers(1, &drawVboId));
        GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, drawVboId));
        GL_VERIFY(
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(drawVertices),
                drawVertices,
                GL_STATIC_DRAW));

        GL_VERIFY(glGenBuffers(1, &uvVboId));
        GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, uvVboId));
        GL_VERIFY(
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(uvVertices),
                uvVertices,
                GL_STATIC_DRAW));
    }

    // Bind framebuffers and draw
    mPassthruFilterShader.Use();

    mPassthruFilterShader.Uniform1i("texTextureData", 0);
    mPassthruFilterShader.UniformVec2("vsViewportSize", kTargetFramebufferWidth, kTargetFramebufferHeight);
    mPassthruFilterShader.UniformVec2("fsTexSize", kPsxFramebufferWidth, kPsxFramebufferHeight);

    mFilterFramebuffer.BindAsTarget();
    mPsxFbFramebuffer.BindAsSourceTextureTo(GL_TEXTURE0);

    GL_VERIFY(glEnableVertexAttribArray(0));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, drawVboId));
    GL_VERIFY(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));

    GL_VERIFY(glEnableVertexAttribArray(1));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, uvVboId));
    GL_VERIFY(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0));

    GL_VERIFY(glDrawArrays(GL_TRIANGLES, 0, 6));

    GL_VERIFY(glDisableVertexAttribArray(0));
    GL_VERIFY(glDisableVertexAttribArray(1));
}

u16 OpenGLRenderer::GetTPageBlendMode(u16 tpage)
{
    return (tpage >> 4) & 3;
}

bool OpenGLRenderer::HasFramebufferPolysToDraw()
{
    // There's always 4 verts for the screen quad, if there's any more than
    // that then some FT4s have been pushed
    return mFbData.size() > 4;
}

void OpenGLRenderer::RenderFramebufferPolys()
{
    if (!mFrameStarted)
    {
        return;
    }

    // Ensure blend mode is back to normal alpha compositing
    GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_VERIFY(glBlendEquation(GL_FUNC_ADD));

    // Set up VBOs
    GLuint eboId, vboId;

    GL_VERIFY(glGenBuffers(1, &vboId));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, vboId));
    GL_VERIFY(glBufferData(GL_ARRAY_BUFFER, sizeof(PassthruVertexData) * mFbData.size(), &mFbData.front(), GL_STREAM_DRAW));

    GL_VERIFY(glGenBuffers(1, &eboId));
    GL_VERIFY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId));
    GL_VERIFY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * mFbIndicies.size(), &mFbIndicies.front(), GL_STREAM_DRAW));

    // Bind framebuffers and draw
    mPassthruShader.Use();

    mPassthruShader.Uniform1i("texTextureData", 0);
    mPassthruShader.UniformVec2("vsViewportSize", kPsxFramebufferWidth, kPsxFramebufferHeight);
    mPassthruShader.Uniform1i("fsFlipUV", true);
    mPassthruShader.UniformVec2("fsTexSize", kPsxFramebufferWidth, kPsxFramebufferHeight);

    mPsxFbFramebuffer.BindAsTarget();
    mPsxFramebuffer.BindAsSourceTextureTo(GL_TEXTURE0);

    GL_VERIFY(glEnableVertexAttribArray(0));
    GL_VERIFY(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PassthruVertexData), 0));

    GL_VERIFY(glEnableVertexAttribArray(1));
    GL_VERIFY(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PassthruVertexData), (void*) offsetof(PassthruVertexData, u)));

    GL_VERIFY(glDrawElements(GL_TRIANGLES, (u32) mFbIndicies.size(), GL_UNSIGNED_INT, NULL));

    GL_VERIFY(glDeleteBuffers(1, &eboId));
    GL_VERIFY(glDeleteBuffers(1, &vboId));

    GL_VERIFY(glDisableVertexAttribArray(0));
    GL_VERIFY(glDisableVertexAttribArray(1));

    mPsxFramebuffer.BindAsTarget();
}

void OpenGLRenderer::InvalidateBatch()
{
    if (!mFrameStarted || mBatchData.size() == 0)
    {
        return;
    }

    mPsxShader.Use();

    GLuint eboId, vboId;

    // Set our new vectors
    GL_VERIFY(glGenBuffers(1, &vboId));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, vboId));
    GL_VERIFY(glBufferData(GL_ARRAY_BUFFER, sizeof(PsxVertexData) * mBatchData.size(), &mBatchData.front(), GL_STREAM_DRAW));

    GL_VERIFY(glGenBuffers(1, &eboId));
    GL_VERIFY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId));
    GL_VERIFY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * mBatchIndicies.size(), &mBatchIndicies.front(), GL_STREAM_DRAW));

    // Set up vertex attributes
    GL_VERIFY(glEnableVertexAttribArray(0));
    GL_VERIFY(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PsxVertexData), 0));
    GL_VERIFY(glEnableVertexAttribArray(1));
    GL_VERIFY(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PsxVertexData), (void*) offsetof(PsxVertexData, r)));
    GL_VERIFY(glEnableVertexAttribArray(2));
    GL_VERIFY(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(PsxVertexData), (void*) offsetof(PsxVertexData, u)));
    GL_VERIFY(glEnableVertexAttribArray(3));
    GL_VERIFY(glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(PsxVertexData), (void*) offsetof(PsxVertexData, drawType)));
    GL_VERIFY(glEnableVertexAttribArray(4));
    GL_VERIFY(glVertexAttribIPointer(4, 2, GL_UNSIGNED_INT, sizeof(PsxVertexData), (void*) offsetof(PsxVertexData, paletteIndex)));

    // Inform our internal resolution
    mPsxShader.UniformVec2("vsViewportSize", kPsxFramebufferWidth, kPsxFramebufferHeight);

    // Bind palette texture
    mPaletteTexture.BindTo(GL_TEXTURE0);
    mPsxShader.Uniform1i("texPalette", 0);

    // Bind gas
    if (mCurGasTexture.IsValid())
    {
        mCurGasTexture.BindTo(GL_TEXTURE1);

        mPsxShader.Uniform1i("texGas", 1);
    }

    // Bind camera (if needed)
    if (mCurCamTexture.IsValid())
    {
        mCurCamTexture.BindTo(GL_TEXTURE2);

        mPsxShader.Uniform1i("texCamera", 2);
    }

    // Bind FG1 layers (if needed)
    s32 numLayers = (s32) mCurFG1Textures.size();

    for (int i = 0; i < numLayers; i++)
    {
        mCurFG1Textures[i].BindTo(GL_TEXTURE3 + i);
    }

    mPsxShader.Uniform1iv("texFG1Masks", 4, mFG1Units);

    // Bind sprite sheets
    s32 numTextures = (s32) mBatchTextures.size();

    for (int i = 0; i < numTextures; i++)
    {
        mBatchTextures[i].BindTo(GL_TEXTURE7 + i);
    }

    mPsxShader.Uniform1iv("texSpriteSheets", GL_USE_NUM_TEXTURE_UNITS, mTextureUnits);

    // Assign blend mode
    if (mBatchBlendMode <= (u32) TPageAbr::eBlend_3)
    {
        SetupBlendMode((u16) mBatchBlendMode);
    }

    // Set index data and render
    GL_VERIFY(glDrawElements(GL_TRIANGLES, (u32) mBatchIndicies.size(), GL_UNSIGNED_INT, NULL));

    // Tear down
    GL_VERIFY(glDeleteBuffers(1, &vboId));
    GL_VERIFY(glDeleteBuffers(1, &eboId));

    GL_VERIFY(glDisableVertexAttribArray(0));
    GL_VERIFY(glDisableVertexAttribArray(1));
    GL_VERIFY(glDisableVertexAttribArray(2));
    GL_VERIFY(glDisableVertexAttribArray(3));
    GL_VERIFY(glDisableVertexAttribArray(4));

    // Do not clear gas here - it's released later
    mCurFG1Textures.clear();

    mBatchBlendMode = BATCH_VALUE_UNSET;
    mBatchData.clear();
    mBatchIndicies.clear();
    mBatchTextures.clear();
    mStats.mInvalidationsCount++;
}

void OpenGLRenderer::PushLines(const PsxVertexData* vertices, int count)
{
    const int numLines = count - 1;

    for (int i = 0; i < numLines; i++)
    {
        PsxVertexData vertA = vertices[i];
        PsxVertexData vertB = vertices[i + 1];

        Quad2D quad = LineToQuad(Point2D(vertA.x, vertA.y), Point2D(vertB.x, vertB.y));

        PsxVertexData triangleVerts[4] = {
            {quad.verts[0].x, quad.verts[0].y, vertA.r, vertA.g, vertA.b, 0, 0, 0, 0, vertA.drawType, vertA.isSemiTrans, vertA.isShaded, vertA.blendMode, 0, 0},
            {quad.verts[1].x, quad.verts[1].y, vertA.r, vertA.g, vertA.b, 0, 0, 0, 0, vertA.drawType, vertA.isSemiTrans, vertA.isShaded, vertA.blendMode, 0, 0},
            {quad.verts[2].x, quad.verts[2].y, vertB.r, vertB.g, vertB.b, 0, 0, 0, 0, vertB.drawType, vertB.isSemiTrans, vertB.isShaded, vertB.blendMode, 0, 0},
            {quad.verts[3].x, quad.verts[3].y, vertB.r, vertB.g, vertB.b, 0, 0, 0, 0, vertB.drawType, vertB.isSemiTrans, vertB.isShaded, vertB.blendMode, 0, 0}};

        PushVertexData(triangleVerts, 4);
    }
}

void OpenGLRenderer::PushFramebufferVertexData(const PassthruVertexData *vertices, int count)
{
    if (!mFrameStarted)
    {
        return;
    }

    // Push indicies for this data
    u32 nextIndex = (u32) mFbData.size();
    s32 numTriangles = count - 2;

    if (numTriangles == 1)
    {
        mFbIndicies.push_back(nextIndex);
        mFbIndicies.push_back(nextIndex + 1);
        mFbIndicies.push_back(nextIndex + 2);
    }
    else if (numTriangles == 2)
    {
        mFbIndicies.push_back(nextIndex + 1);
        mFbIndicies.push_back(nextIndex);
        mFbIndicies.push_back(nextIndex + 3);

        mFbIndicies.push_back(nextIndex + 3);
        mFbIndicies.push_back(nextIndex);
        mFbIndicies.push_back(nextIndex + 2);
    }

    // Push vertex data
    for (int i = 0; i < count; i++)
    {
        mFbData.push_back(vertices[i]);
    }
}

void OpenGLRenderer::PushVertexData(PsxVertexData* pVertData, int count, GLTexture2D texture)
{
    if (!mFrameStarted)
    {
        return;
    }

    u32 blendMode = pVertData[0].blendMode;
    u32 drawType = pVertData[0].drawType;

    // Check if we need to invalidate the existing batched data
    //
    // The only reason we need to invalidate here is if the blend mode switches
    // to/from subtractive blending
    if (
        mBatchBlendMode != blendMode &&
        mBatchBlendMode != BATCH_VALUE_UNSET &&
        (mBatchBlendMode == 2 || blendMode == 2))
    {
        InvalidateBatch();
    }

    // Set up texture ... if it's a textured poly then we need to make sure we
    // have texture units leftover if it's not already assigned
    u32 targetTexUnit = 0;

    switch (drawType)
    {
        case GL_PSX_DRAW_MODE_DEFAULT_FT4:
        {
            auto iter = std::find(mBatchTextures.begin(), mBatchTextures.end(), texture);

            if (iter == mBatchTextures.end())
            {
                if (mBatchTextures.size() == GL_USE_NUM_TEXTURE_UNITS)
                {
                    InvalidateBatch();
                }

                targetTexUnit = (u32) mBatchTextures.size();
                mBatchTextures.push_back(texture);
            }
            else
            {
                targetTexUnit = (u32) std::distance(mBatchTextures.begin(), iter);
            }

            break;
        }

        case GL_PSX_DRAW_MODE_CAM:
        {
            mCurCamTexture = texture;
            break;
        }

        case GL_PSX_DRAW_MODE_FG1:
        {
            targetTexUnit = static_cast<u32>(mCurFG1Textures.size());

            if (targetTexUnit == 4)
            {
                ALIVE_FATAL("Out of texture units for FG1 layers.");
            }

            mCurFG1Textures.push_back(texture);

            break;
        }
    }

    mBatchBlendMode = blendMode;

    // Push indicies for this data
    u32 nextIndex = (u32) mBatchData.size();
    s32 numTriangles = count - 2;

    if (numTriangles == 1)
    {
        mBatchIndicies.push_back(nextIndex);
        mBatchIndicies.push_back(nextIndex + 1);
        mBatchIndicies.push_back(nextIndex + 2);
    }
    else if (numTriangles == 2)
    {
        mBatchIndicies.push_back(nextIndex + 1);
        mBatchIndicies.push_back(nextIndex);
        mBatchIndicies.push_back(nextIndex + 3);

        mBatchIndicies.push_back(nextIndex + 3);
        mBatchIndicies.push_back(nextIndex);
        mBatchIndicies.push_back(nextIndex + 2);
    }

    // Push vertex data
    for (int i = 0; i < count; i++)
    {
        pVertData[i].textureUnitIndex = targetTexUnit;
        mBatchData.push_back(pVertData[i]);
    }

    // DEBUGGING: If batching is disabled we invalidate immediately
    if (!gRenderEnable_Batching)
    {
        InvalidateBatch();
    }
}

void OpenGLRenderer::SetupBlendMode(u16 blendMode)
{
    if ((TPageAbr)blendMode == TPageAbr::eBlend_2)
    {
        GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
        GL_VERIFY(glBlendEquation(GL_FUNC_REVERSE_SUBTRACT));
    }
    else
    {
        GL_VERIFY(glBlendFunc(GL_ONE, GL_SRC_ALPHA));
        GL_VERIFY(glBlendEquation(GL_FUNC_ADD));
    }
}

// END ROZZA FRAMEBUFFER STUFF

void OpenGLRenderer::DebugWindow()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Developer"))
        {
            if (ImGui::BeginMenu("Renderer Debug"))
            {
                ImGui::MenuItem("Batching Enabled", nullptr, &gRenderEnable_Batching);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Render Elements"))
            {
                ImGui::MenuItem("GAS", nullptr, &gRenderEnable_GAS);
                ImGui::MenuItem("FT4", nullptr, &gRenderEnable_FT4);
                ImGui::MenuItem("G4", nullptr, &gRenderEnable_G4);
                ImGui::MenuItem("G3", nullptr, &gRenderEnable_G3);
                ImGui::MenuItem("G2", nullptr, &gRenderEnable_G2);

                #if GL_DEBUG > 0
                ImGui::MenuItem("gl_debug", nullptr, &gGlDebug);
                #endif

                ImGui::MenuItem("filter", nullptr, &mFramebufferFilter);

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (ImGui::Begin("Render stats"))
    {
        ImGui::Text("Cams %d", mStats.mCamUploadCount);
        ImGui::Text("Fg1s %d", mStats.mFg1UploadCount);
        ImGui::Text("Anims %d", mStats.mAnimUploadCount);
        ImGui::Text("Pals %d", mStats.mPalUploadCount);
        ImGui::Text("Fonts %d", mStats.mFontUploadCount);
        ImGui::Text("Invalidations %d", mStats.mInvalidationsCount);
    }
    ImGui::End();
}
