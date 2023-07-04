#include <algorithm>

#include "../../../relive_lib/data_conversion/rgb_conversion.hpp"
#include "../../../relive_lib/Primitives.hpp"
#include "../../../relive_lib/Animation.hpp"
#include "../../Compression.hpp"
#include "../../../AliveLibAE/Font.hpp"
#include "../../../relive_lib/BaseGameAutoPlayer.hpp"
#include "../../../relive_lib/FatalError.hpp"
#include "GLDebug.hpp"
#include "GLFramebuffer.hpp"
#include "GLShader.hpp"
#include "GLShaderProgram.hpp"
#include "GLTexture2D.hpp"
#include "OpenGLRenderer.hpp"

#define GL_TO_IMGUI_TEX(v) *reinterpret_cast<ImTextureID*>(&v)

extern bool gDDCheat_FlyingEnabled;
namespace AO
{
    extern bool gDDCheat_FlyingEnabled;
}

static bool gRenderEnable_Batching = true;

static bool gRenderEnable_GAS = true;
static bool gRenderEnable_FT4 = true;
static bool gRenderEnable_G4 = true;
static bool gRenderEnable_G3 = true;
static bool gRenderEnable_G2 = true;

OpenGLRenderer::OpenGLRenderer(TWindowHandleType window)
    : IRenderer(window),
    mContext(window),
    mFilterFramebuffer(kTargetFramebufferWidth, kTargetFramebufferHeight),
    mPsxFramebuffer{
        GLFramebuffer(kPsxFramebufferWidth, kPsxFramebufferHeight),
        GLFramebuffer(kPsxFramebufferWidth, kPsxFramebufferHeight)
    },
    mBatcher(UvMode::UnNormalized),
    mPaletteCache(kAvailablePalettes)
{
    mWindow = window;

    // Create and bind the VAO, and never touch it again! Wahey.
    GL_VERIFY(glGenVertexArrays(1, &mVAO));
    GL_VERIFY(glBindVertexArray(mVAO));

    // Enable blending
    GL_VERIFY(glEnable(GL_BLEND));

    // Preload the cache texture with all black
    const static RGBA32 black[256] = {};

    mPaletteTexture = std::make_shared<GLTexture2D>(kPaletteDepth, kAvailablePalettes, GL_RGBA);

    for (u32 i = 0; i < kAvailablePalettes; i++)
    {
        mPaletteTexture->LoadSubImage(0, i, kPaletteDepth, 1, black);
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

    // Init array we pass to texture uniform to specify the units we're using
    // which is the number of units starting at GL_TEXTURE7
    for (u32 i = 0; i < kSpriteTextureUnitCount; i++)
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

    GL_VERIFY(glClearColor(static_cast<f32>(r), static_cast<f32>(g), static_cast<f32>(b), 1.0f));
    GL_VERIFY(glClear(GL_COLOR_BUFFER_BIT));

    // Set back to the dest PSX framebuffer
    GetDestinationPsxFramebuffer().BindAsTarget();

    if (scissoring)
    {
        GL_VERIFY(glEnable(GL_SCISSOR_TEST));
    }
}

void OpenGLRenderer::StartFrame()
{
    IRenderer::StartFrame();

    if (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED)
    {
        return;
    }

    mStats.Reset();

    mFrameStarted = true;

    mBatcher.StartFrame();

    // Set offsets for the screen (this is for the screen shake effect)
    mOffsetX = 0;
    mOffsetY = 0;

    // Check if we need to recreate the framebuffers if the viewport has
    // changed size
    s32 desiredW = kPsxFramebufferWidth;
    s32 desiredH = kPsxFramebufferHeight;

    if (!mUseOriginalResolution)
    {
        // If we're maintaining aspect ratio, then the framebuffer needs
        // to be equal to the size of the rect otherwise the result will
        // be a warped image
        if (mKeepAspectRatio)
        {
            SDL_Rect r = GetTargetDrawRect();

            desiredW = r.w;
            desiredH = r.h;
        }
        else
        {
            SDL_GL_GetDrawableSize(mWindow, &desiredW, &desiredH);
        }
    }

    if (
        mPsxFramebuffer[0].GetWidth() != desiredW ||
        mPsxFramebuffer[0].GetHeight() != desiredH
    )
    {
        mPsxFramebuffer[0].Resize(desiredW, desiredH);
        mPsxFramebuffer[1].Resize(desiredW, desiredH);
    }

    // Ensure bound to destination framebuffer
    GetDestinationPsxFramebuffer().BindAsTarget();
}

void OpenGLRenderer::EndFrame()
{
    mBatcher.EndFrame();

    DrawBatches();

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

    // Set the framebuffer target back to the destination PSX framebuffer
    GetDestinationPsxFramebuffer().BindAsTarget();
}

void OpenGLRenderer::SetClip(const Prim_ScissorRect& clipper)
{
    SDL_Rect rect;
    rect.x = clipper.mRect.x;
    rect.y = clipper.mRect.y;
    rect.w = clipper.mRect.w;
    rect.h = clipper.mRect.h;

    if (rect.x == 0 && rect.y == 0 && rect.w == 1 && rect.h == 1)
    {
        // No scissor
        rect = {};
    }

    mBatcher.SetScissor(rect);
}

void OpenGLRenderer::Draw(const Prim_GasEffect& gasEffect)
{
    if (!gRenderEnable_GAS)
    {
        return;
    }

    if (!mCurGasTexture || !mCurGasTexture->IsValid())
    {
        mCurGasTexture = std::make_shared<GLTexture2D>(kPsxFramebufferWidth, kPsxFramebufferHeight, GL_RGB);
    }

    const f32 gasWidth = std::floor(static_cast<f32>(gasEffect.w - gasEffect.x) / 4);
    const f32 gasHeight = std::floor(static_cast<f32>(gasEffect.h - gasEffect.y) / 2);
    mCurGasTexture->LoadSubImage(0, 0, static_cast<GLsizei>(gasWidth), static_cast<GLsizei>(gasHeight), gasEffect.pData, GL_UNSIGNED_SHORT_5_6_5);

    // TODO: If there is more than 1 gas in a frame break the batch ?
    mBatcher.PushGas(gasEffect);
}

void OpenGLRenderer::Draw(const Line_G2& line)
{
    if (!gRenderEnable_G2)
    {
        return;
    }

    mBatcher.PushLine(line, line.mBlendMode);
}

void OpenGLRenderer::Draw(const Line_G4& line)
{
    if (!gRenderEnable_G4)
    {
        return;
    }

    mBatcher.PushLine(line, line.mBlendMode);
}

void OpenGLRenderer::Draw(const Poly_G3& poly)
{
    if (!gRenderEnable_G3)
    {
        return;
    }

    mBatcher.PushPolyG3(poly, poly.mBlendMode);
}

void OpenGLRenderer::Draw(const Poly_FT4& poly)
{
    if (!gRenderEnable_FT4)
    {
        return;
    }

    std::shared_ptr<GLTexture2D> texture = PrepareTextureFromPoly(poly);

    if (poly.mFg1)
    {
        mBatcher.PushFG1(poly, texture);
    }
    else if (poly.mCam)
    {
        mBatcher.PushCAM(poly, texture);
    }
    else if (poly.mAnim)
    {
        const u32 palIndex = PreparePalette(*poly.mAnim->mAnimRes.mCurPal);
        mBatcher.PushAnim(poly, palIndex, texture);
    }
    else if (poly.mFont)
    {
        FontResource& fontRes = poly.mFont->mFntResource;

        auto pPal = fontRes.mCurPal;
        const u32 palIndex = PreparePalette(*pPal);

        mBatcher.PushFont(poly, palIndex, texture);
    }
    else
    {
        // ScreenWave (Bell Song framebuffer effect)
        const f32 baseUf = poly.uBase;
        const f32 baseVf = poly.vBase;

        PsxVertexData verts[4] = {
            {static_cast<f32>(poly.X0()), static_cast<f32>(poly.Y0()), 127.0f, 127.0f, 127.0f, baseUf + static_cast<f32>(poly.U0()), kPsxFramebufferHeight - (baseVf + static_cast<f32>(poly.V0())), PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0},
            {static_cast<f32>(poly.X1()), static_cast<f32>(poly.Y1()), 127.0f, 127.0f, 127.0f, baseUf + static_cast<f32>(poly.U1()), kPsxFramebufferHeight - (baseVf + static_cast<f32>(poly.V1())), PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0},
            {static_cast<f32>(poly.X2()), static_cast<f32>(poly.Y2()), 127.0f, 127.0f, 127.0f, baseUf + static_cast<f32>(poly.U2()), kPsxFramebufferHeight - (baseVf + static_cast<f32>(poly.V2())), PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0},
            {static_cast<f32>(poly.X3()), static_cast<f32>(poly.Y3()), 127.0f, 127.0f, 127.0f, baseUf + static_cast<f32>(poly.U3()), kPsxFramebufferHeight - (baseVf + static_cast<f32>(poly.V3())), PsxDrawMode::DefaultFT4, 0, 0, relive::TBlendModes::eBlend_0, 0, 0}
        };

        mBatcher.PushFramebufferVertexData(verts, ALIVE_COUNTOF(verts));
    }
}

void OpenGLRenderer::Draw(const Poly_G4& poly)
{
    if (!gRenderEnable_G4)
    {
        return;
    }

    mBatcher.PushPolyG4(poly, poly.mBlendMode);
}

u32 OpenGLRenderer::PreparePalette(AnimationPal& pCache)
{
    const PaletteCache::AddResult addRet = mPaletteCache.Add(pCache);

    if (addRet.mAllocated)
    {
        // Write palette data
        mPaletteTexture->LoadSubImage(0, addRet.mIndex, kPaletteDepth, 1, pCache.mPal);

        mStats.mPalUploadCount++;
    }

    return addRet.mIndex;
}

std::shared_ptr<GLTexture2D> OpenGLRenderer::PrepareTextureFromAnim(Animation& anim)
{
    const AnimResource& r = anim.mAnimRes;

    std::shared_ptr<GLTexture2D> texture = mTextureCache.GetCachedTexture(r.mUniqueId.Id(), kSpriteTextureLifetime);

    if (!texture || !texture->IsValid())
    {
        auto animTex = std::make_shared<GLTexture2D>(r.mPngPtr->mWidth, r.mPngPtr->mHeight, GL_RED);

        animTex->LoadImage(r.mPngPtr->mPixels.data());

        texture = mTextureCache.Add(r.mUniqueId.Id(), kSpriteTextureLifetime, std::move(animTex));

        mStats.mAnimUploadCount++;
    }

    return texture;
}

std::shared_ptr<GLTexture2D> OpenGLRenderer::PrepareTextureFromPoly(const Poly_FT4& poly)
{
    std::shared_ptr<GLTexture2D> texture;

    if (poly.mFg1)
    {
        texture = mTextureCache.GetCachedTexture(poly.mFg1->mUniqueId.Id(), kCamTextureLifetime);

        if (!texture || !texture->IsValid())
        {
            auto fg1Tex = std::make_shared<GLTexture2D>(poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight, GL_RGBA);

            fg1Tex->LoadImage(poly.mFg1->mImage.mPixels->data());

            texture = mTextureCache.Add(poly.mFg1->mUniqueId.Id(), kCamTextureLifetime, fg1Tex);

            mStats.mFg1UploadCount++;
        }
    }
    else if (poly.mCam)
    {
        texture = mTextureCache.GetCachedTexture(poly.mCam->mUniqueId.Id(), kCamTextureLifetime);

        if (!texture || !texture->IsValid())
        {
            auto camTex = std::make_shared<GLTexture2D>(poly.mCam->mData.mWidth, poly.mCam->mData.mHeight, GL_RGBA);

            camTex->LoadImage(poly.mCam->mData.mPixels->data());

            texture = mTextureCache.Add(poly.mCam->mUniqueId.Id(), kCamTextureLifetime, camTex);

            mStats.mCamUploadCount++;
        }
    }
    else if (poly.mAnim)
    {
        return PrepareTextureFromAnim(*poly.mAnim);
    }
    else if (poly.mFont)
    {
        texture = mTextureCache.GetCachedTexture(poly.mFont->mFntResource.mUniqueId.Id(), kSpriteTextureLifetime);

        if (!texture || !texture->IsValid())
        {
            std::shared_ptr<PngData> pPng = poly.mFont->mFntResource.mPngPtr;

            auto fontTex = std::make_shared<GLTexture2D>(pPng->mWidth, pPng->mHeight, GL_RED);

            fontTex->LoadImage(pPng->mPixels.data());

            texture = mTextureCache.Add(poly.mFont->mFntResource.mUniqueId.Id(), kSpriteTextureLifetime, fontTex);

            mStats.mFontUploadCount++;
        }
    }

    return texture;
}

void OpenGLRenderer::DrawFramebufferToScreen(s32 x, s32 y, s32 width, s32 height)
{
    // Ensure blend mode is back to normal alpha compositing
    GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_VERIFY(glBlendEquation(GL_FUNC_ADD));

    // Set up the texture we're going to draw...
    f32 texWidth = 0;
    f32 texHeight = 0;

    // Handle filtering (do not filter if not using original game res, it looks
    // terrible)
    if (mFramebufferFilter && mUseOriginalResolution)
    {
        UpdateFilterFramebuffer();

        mFilterFramebuffer.BindAsSourceTextureTo(GL_TEXTURE0);

        texWidth = static_cast<f32>(mFilterFramebuffer.GetWidth());
        texHeight = static_cast<f32>(mFilterFramebuffer.GetHeight());
    }
    else
    {
        GetDestinationPsxFramebuffer().BindAsSourceTextureTo(GL_TEXTURE0);
        texWidth = static_cast<f32>(GetDestinationPsxFramebuffer().GetWidth());
        texHeight = static_cast<f32>(GetDestinationPsxFramebuffer().GetHeight());
    }

    // Set up VBOs
    GLuint drawVboId = 0;
    GLuint uvVboId = 0;

    const f32 fX = static_cast<f32>(x);
    const f32 fY = static_cast<f32>(y);
    const f32 fWidth = static_cast<f32>(width);
    const f32 fHeight = static_cast<f32>(height);

    const GLfloat drawVertices[] = {
        fX, fY,
        fX, fY + fHeight,
        fX + fWidth, fY,

        fX + fWidth, fY,
        fX, fY + fHeight,
        fX + fWidth, fY + fHeight};
    const GLfloat uvVertices[] = {
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
    mPassthruShader.UniformVec2("vsViewportSize", static_cast<f32>(viewportW), static_cast<f32>(viewportH));
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

void OpenGLRenderer::SetupBlendMode(relive::TBlendModes blendMode)
{
    if (blendMode == relive::TBlendModes::eBlend_2)
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

void OpenGLRenderer::UpdateFilterFramebuffer()
{
    // Set up VBOs
    static GLuint drawVboId = 0;
    static GLuint uvVboId = 0;

    if (drawVboId == 0)
    {
        constexpr GLfloat drawVertices[] = {
            0.0f, 0.0f,
            0.0f, kTargetFramebufferHeight,
            kTargetFramebufferWidth, 0.0,

            kTargetFramebufferWidth, 0.0f,
            0.0f, kTargetFramebufferHeight,
            kTargetFramebufferWidth, kTargetFramebufferHeight};

        constexpr GLfloat uvVertices[] = {
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
    GetDestinationPsxFramebuffer().BindAsSourceTextureTo(GL_TEXTURE0);

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

GLFramebuffer& OpenGLRenderer::GetSourcePsxFramebuffer()
{
    return mPsxFramebuffer[mSrcPsxFramebufferIdx];
}

GLFramebuffer& OpenGLRenderer::GetDestinationPsxFramebuffer()
{
    return mSrcPsxFramebufferIdx == 0 ? mPsxFramebuffer[1] : mPsxFramebuffer[0];
}

void OpenGLRenderer::SwapSrcDstForPsxFramebuffers()
{
    mSrcPsxFramebufferIdx = mSrcPsxFramebufferIdx == 0 ? 1 : 0;
}

void OpenGLRenderer::DecreaseResourceLifetimes()
{
    mTextureCache.DecreaseResourceLifetimes();

    mPaletteCache.ResetUseFlags();
}

void OpenGLRenderer::DebugWindow()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Developer"))
        {
            if (ImGui::BeginMenu("Renderer Debug"))
            {
                ImGui::MenuItem("Batching Enabled", nullptr, &gRenderEnable_Batching);
                mBatcher.SetBatching(gRenderEnable_Batching);

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

void OpenGLRenderer::DrawBatches()
{
    if (!mFrameStarted || mBatcher.mBatches.size() == 0)
    {
        return;
    }

    mPsxShader.Use();

    GLuint eboId, vboId;

    // Upload vertices
    GL_VERIFY(glGenBuffers(1, &vboId));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, vboId));
    GL_VERIFY(glBufferData(GL_ARRAY_BUFFER, sizeof(PsxVertexData) * mBatcher.mVertices.size(), mBatcher.mVertices.data(), GL_STREAM_DRAW));

    // Upload indices
    GL_VERIFY(glGenBuffers(1, &eboId));
    GL_VERIFY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId));
    GL_VERIFY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * mBatcher.mIndices.size(), mBatcher.mIndices.data(), GL_STREAM_DRAW));

    // Set up vertex attributes
    GL_VERIFY(glEnableVertexAttribArray(0));
    GL_VERIFY(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PsxVertexData), 0));
    GL_VERIFY(glEnableVertexAttribArray(1));
    GL_VERIFY(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PsxVertexData), (void*) offsetof(PsxVertexData, r)));
    GL_VERIFY(glEnableVertexAttribArray(2));
    GL_VERIFY(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(PsxVertexData), (void*) offsetof(PsxVertexData, u)));
    GL_VERIFY(glEnableVertexAttribArray(3));
    GL_VERIFY(glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(PsxVertexData), (void*) offsetof(PsxVertexData, drawMode)));
    GL_VERIFY(glEnableVertexAttribArray(4));
    GL_VERIFY(glVertexAttribIPointer(4, 2, GL_UNSIGNED_INT, sizeof(PsxVertexData), (void*) offsetof(PsxVertexData, paletteIndex)));

    // Inform our internal resolution
    mPsxShader.UniformVec2("vsViewportSize", kPsxFramebufferWidth, kPsxFramebufferHeight);

    // Bind palette texture
    mPaletteTexture->BindTo(GL_TEXTURE0);
    mPsxShader.Uniform1i("texPalette", 0);

    // Bind camera (if needed)
    if (mBatcher.mCamTexture && mBatcher.mCamTexture->IsValid())
    {
        mBatcher.mCamTexture->BindTo(GL_TEXTURE2);
        mPsxShader.Uniform1i("texCamera", 2);
    }

    // Bind gas
    if (mCurGasTexture && mCurGasTexture->IsValid())
    {
        mCurGasTexture->BindTo(GL_TEXTURE1);
        mPsxShader.Uniform1i("texGas", 1);
    }

    u32 idxOffset = 0;
    u32 baseTextureIdx = 0;
    for (Batcher<GLTexture2D, BatchData, kSpriteTextureUnitCount>::RenderBatch& batch : mBatcher.mBatches)
    {
        if (batch.mScissor.x == 0 && batch.mScissor.y == 0 && batch.mScissor.w == 0 && batch.mScissor.h == 0)
        {
            // Disable scissor
            GL_VERIFY(glDisable(GL_SCISSOR_TEST));
        }
        else
        {
            GL_VERIFY(glEnable(GL_SCISSOR_TEST));
            ScaledScissor(batch.mScissor.x, batch.mScissor.y, batch.mScissor.w, batch.mScissor.h);
        }

        if (batch.mSourceIsFramebuffer)
        {
            SwapSrcDstForPsxFramebuffers();

            // We use GL_TEXTURE7 because it will always be overwritten
            // by the next batch, so it's safe to use
            GetSourcePsxFramebuffer().BindAsSourceTextureTo(GL_TEXTURE7);
            GetDestinationPsxFramebuffer().BindAsTarget();

            mPsxShader.Uniform1i("texFramebuffer", 7);

            SetupBlendMode(relive::TBlendModes::eBlend_0); // Ensure we're using additive blend mode
        }
        else
        {
            // Bind sprite sheets
            f32 texSizes[kSpriteTextureUnitCount * 2] = {};

            for (u32 i = 0; i < batch.mTexturesInBatch; i++)
            {
                const u32 textureId = batch.mTextureIds[i];
                const u32 batchTextureIdx = batch.TextureIdxForId(textureId);
                auto pTex = mBatcher.mBatchTextures[baseTextureIdx + batchTextureIdx];
                pTex->BindTo(GL_TEXTURE7 + batchTextureIdx);

                texSizes[i * 2] = static_cast<f32>(pTex->GetWidth());
                texSizes[(i * 2) + 1] = static_cast<f32>(pTex->GetHeight());
            }

            mPsxShader.Uniform1iv("texSpriteSheets", kSpriteTextureUnitCount, mTextureUnits);
            mPsxShader.Uniform2fv("fsSpriteSheetSize", kSpriteTextureUnitCount, texSizes);

            // Assign blend mode
            if (batch.mBlendMode != relive::TBlendModes::None)
            {
                SetupBlendMode(batch.mBlendMode);
            }
        }

        mPsxShader.Uniform1i("bDrawingFramebuffer", static_cast<GLint>(batch.mSourceIsFramebuffer));

        // Set index data and render
        GL_VERIFY(glDrawElements(GL_TRIANGLES, (batch.mNumTrisToDraw) * 3, GL_UNSIGNED_INT, (void*) (idxOffset * sizeof(GLuint))));

        idxOffset += (batch.mNumTrisToDraw) * 3;
        baseTextureIdx += batch.mTexturesInBatch;

        mStats.mInvalidationsCount++;
    }

    // Tear down
    GL_VERIFY(glDeleteBuffers(1, &vboId));
    GL_VERIFY(glDeleteBuffers(1, &eboId));

    GL_VERIFY(glDisableVertexAttribArray(0));
    GL_VERIFY(glDisableVertexAttribArray(1));
    GL_VERIFY(glDisableVertexAttribArray(2));
    GL_VERIFY(glDisableVertexAttribArray(3));
    GL_VERIFY(glDisableVertexAttribArray(4));

    // Do not clear gas here - it's released later
}

void OpenGLRenderer::ScaledScissor(s32 x, s32 y, s32 width, s32 height)
{
    f32 wScale = static_cast<f32>(mPsxFramebuffer[0].GetWidth()) / kPsxFramebufferWidth;
    f32 hScale = static_cast<f32>(mPsxFramebuffer[0].GetHeight()) / kPsxFramebufferHeight;

    s32 scaledX = static_cast<s32>(x * wScale);
    s32 scaledY = static_cast<s32>(y * hScale);
    s32 scaledW = static_cast<s32>(width * wScale);
    s32 scaledH = static_cast<s32>(height * hScale);

    GL_VERIFY(glScissor(scaledX, mPsxFramebuffer[0].GetHeight() - scaledY - scaledH, scaledW, scaledH));
}
