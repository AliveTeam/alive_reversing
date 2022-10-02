#include "OpenGLRenderer.hpp"
#include "../Compression.hpp"
#include "../relive_lib/VRam.hpp"
#include "../relive_lib/Primitives.hpp"
#include "StbImageImplementation.hpp"
#include "../relive_lib/Animation.hpp"
#include "../Font.hpp"

#define GL_DEBUG 1

#if GL_DEBUG > 0
    #define GL_VERIFY(x) \
        (x);             \
        CheckGLError();
#else
    #define GL_VERIFY(x) (x);
#endif

#define GL_TO_IMGUI_TEX(v) *reinterpret_cast<ImTextureID*>(&v)

#define GL_PSX_DRAW_MODE_FLAT        0
#define GL_PSX_DRAW_MODE_DEFAULT_FT4 1
#define GL_PSX_DRAW_MODE_CAM         2
#define GL_PSX_DRAW_MODE_FG1         3

#define GL_FRAMEBUFFER_PSX_SRC 0
#define GL_FRAMEBUFFER_PSX_DST 1
#define GL_FRAMEBUFFER_SCREEN  -1

#define GL_FRAMEBUFFER_PSX_WIDTH  640
#define GL_FRAMEBUFFER_PSX_HEIGHT 240


static int gPalTextureID = 0;


static bool gRenderEnable_SPRT = true;
static bool gRenderEnable_GAS = false;
static bool gRenderEnable_TILE = false;
static bool gRenderEnable_FT4 = true;
static bool gRenderEnable_G4 = true;
static bool gRenderEnable_G3 = true;
static bool gRenderEnable_G2 = true;
static bool gRenderEnable_F4 = true;
static bool gRenderEnable_F3 = true;
static bool gRenderEnable_F2 = true;


#if GL_DEBUG > 0
static void CheckGLError()
{
    GLenum lastGLError = glGetError();

    if (lastGLError != GL_NO_ERROR)
    {
        std::string buf;
        auto msg = (char_type*) glewGetString(lastGLError);

        buf.append("OpenGL error raised: ");

        if (msg != nullptr)
        {
            buf.append(msg);
        }
        else
        {
            buf.append(std::to_string(lastGLError));
        }
        
        ALIVE_FATAL(buf.c_str());
    }
}
#endif


static GLuint Renderer_CreateTexture(GLenum interpolation = GL_NEAREST)
{
    GLuint textureId;
    
    GL_VERIFY(glGenTextures(1, &textureId));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, textureId));

    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation));
    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation));

    return textureId;
}

static void Renderer_DecodePalette(const u8* srcPalData, RGBAPixel* dst, s32 palDepth)
{
    const u16* palShortPtr = reinterpret_cast<const u16*>(srcPalData);
    for (s32 i = 0; i < palDepth; i++)
    {
        const u16 oldPixel = palShortPtr[i];

        dst[i].B = static_cast<u8>((((oldPixel >> 0) & 0x1F)) << 3);
        dst[i].G = static_cast<u8>((((oldPixel >> 5) & 0x1F)) << 3);
        dst[i].R = static_cast<u8>((((oldPixel >> 10) & 0x1F)) << 3);
        dst[i].A = static_cast<u8>((((((oldPixel) >> 15) & 0x1)) ? 255 : 0));
    }
}



void OpenGLRenderer::Renderer_BindPalette(AnimationPal& pCache)
{
    if (gPalTextureID == 0)
    {
        gPalTextureID = Renderer_CreateTexture();
    }

    RGBAPixel dst[256];
    Renderer_DecodePalette(reinterpret_cast<const u8*>(pCache.mPal), dst, 256);

    // Set palette to GL_TEXTURE1
    GL_VERIFY(glActiveTexture(GL_TEXTURE1));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, gPalTextureID));

    GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst));

    mStats.mPalUploadCount++;
}

/*
void set_pixel_8(u8* surface, int x, int y, int pitch, u8 pixel)
{
    Uint8* target_pixel = (Uint8*)surface + (y * pitch) + x * sizeof(u8);
    *(u8*) target_pixel = pixel;
}

u8 get_pixel_8(u8* surface, int x, int y, int pitch)
{
    Uint8* target_pixel = (Uint8*) surface + (y * pitch) + x * sizeof(u8);
    return *target_pixel;
}*/

u32 OpenGLRenderer::PrepareTextureFromAnim(Animation& anim)
{
    AnimResource& r = anim.mAnimRes;

    auto it = mTextureCache.find(r.mUniqueId.Id());
    u32 textureId = 0;
    bool uploadPixels = false;
    if (it == std::end(mTextureCache))
    {
        textureId = Renderer_CreateTexture();
        LastUsedFrame tmp;
        tmp.mLastUsedFrame = mFrameNumber;
        tmp.mTextureId = textureId;
        mTextureCache[r.mUniqueId.Id()] = tmp;
        uploadPixels = true;
    }
    else
    {
        textureId = it->second.mTextureId;

        // Update the last used frame to keep the texture alive a bit longer
        LastUsedFrame tmp;
        tmp.mLastUsedFrame = mFrameNumber;
        tmp.mTextureId = textureId;
        mTextureCache[r.mUniqueId.Id()] = tmp;
    }

    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, textureId));

    if (uploadPixels)
    {
        GL_VERIFY(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, r.mTgaPtr->mWidth, r.mTgaPtr->mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, r.mTgaPtr->mPixels.data()));

        mStats.mAnimUploadCount++;
    }

    return textureId;
}

u32 OpenGLRenderer::PrepareTextureFromPoly(Poly_FT4& poly)
{
    GL_VERIFY(glActiveTexture(GL_TEXTURE0));

    if (poly.mFg1)
    {
        if (poly.mFg1->mUniqueId.Id() != mFg1Texture.mUniqueResId)
        {
            mFg1Texture.mUniqueResId = poly.mFg1->mUniqueId.Id();

            if (!mFg1Texture.mTextureId)
            {
                mFg1Texture.mTextureId = Renderer_CreateTexture();
            }

            mStats.mFg1UploadCount++;

            GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mFg1Texture.mTextureId));
            GL_VERIFY(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
            GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, poly.mFg1->mImage.mWidth, poly.mFg1->mImage.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, poly.mFg1->mImage.mPixels->data()));
        }
        return mFg1Texture.mTextureId;
    }
    else if (poly.mCam)
    {
        if (poly.mCam->mUniqueId.Id() != mCamTexture.mUniqueResId)
        {
            mCamTexture.mUniqueResId = poly.mCam->mUniqueId.Id();

            if (!mCamTexture.mTextureId)
            {
                mCamTexture.mTextureId = Renderer_CreateTexture();
            }

            mStats.mCamUploadCount++;

            GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mCamTexture.mTextureId));
            GL_VERIFY(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
            GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, poly.mCam->mData.mWidth, poly.mCam->mData.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, poly.mCam->mData.mPixels->data()));
        }
        return mCamTexture.mTextureId;
    }
    else if (poly.mAnim)
    {
        return PrepareTextureFromAnim(*poly.mAnim);
    }
    else if (poly.mFont)
    {
        if (poly.mFont->field_C_resource_id.mUniqueId.Id() != mFontTexture.mUniqueResId)
        {
            mFontTexture.mUniqueResId = poly.mFont->field_C_resource_id.mUniqueId.Id();

            std::shared_ptr<TgaData> pTga = poly.mFont->field_C_resource_id.mTgaPtr;

            if (!mFontTexture.mTextureId)
            {
                mFontTexture.mTextureId = Renderer_CreateTexture();
            }

            mStats.mFontUploadCount++;

            GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mFontTexture.mTextureId));
            GL_VERIFY(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
            GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pTga->mWidth, pTga->mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, pTga->mPixels.data()));
        }
        return mFontTexture.mTextureId;
    }

    return 0;
}


void OpenGLRenderer::BltBackBuffer(const SDL_Rect* /*pCopyRect*/, const SDL_Rect* /*pDst*/)
{
}

void OpenGLRenderer::Clear(u8 /*r*/, u8 /*g*/, u8 /*b*/)
{
    // hacky hot reload shaders
    /* static s32 t = 999;
    if (t >= 10)
    {
        t = 0;
        mTextureShader.LoadFromFile("shaders/texture.vsh", "shaders/texture.fsh");
    }
    t++;*/

    static bool firstFrame = true;
    if (!firstFrame)
    {
    }
    else
    {
        firstFrame = false;
    }

    // FIXME: Find out what we're actually meant to do in here, yes it's called
    //        'Clear', but what are we clearing, and why? At the moment it does
    //        nothing and yet no issues appear to arise? Is it dead Jim?

    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
}

bool OpenGLRenderer::Create(TWindowHandleType window)
{
    mWindow = window;

    // Find the opengl driver
    const s32 numDrivers = SDL_GetNumRenderDrivers();
    if (numDrivers < 0)
    {
        LOG_ERROR("Failed to get driver count " << SDL_GetError());
    }

    LOG_INFO("Got " << numDrivers << " drivers");

    s32 index = -1;
    for (s32 i = 0; i < numDrivers; i++)
    {
        SDL_RendererInfo info = {};
        if (SDL_GetRenderDriverInfo(i, &info) < 0)
        {
            LOG_WARNING("Failed to get render " << i << " info " << SDL_GetError());
        }
        else
        {
            LOG_INFO(i << " name " << info.name);
            if (strstr(info.name, "opengl"))
            {
                index = i;
                break;
            }
        }
    }

    if (index == -1)
    {
        LOG_WARNING("OpenGL SDL2 driver not found");
        return false;
    }


    // We should attempt to load OpenGL 3.2 first, because this is the minimum
    // required version for RenderDoc captures so we can actually debug stuff
    char_type* glslVer = "#version 150";

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create context
    mContext = SDL_GL_CreateContext(window);

    if (mContext == NULL)
    {
        LOG_ERROR("OpenGL 3.2 context could not be created! SDL Error: " << SDL_GetError());

        // Our ACTUAL minimum OpenGL requirement is 3.1, though we will check
        // supported extensions on the GPU in a moment
        glslVer = "#version 140";

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Create context
        mContext = SDL_GL_CreateContext(window);
        if (mContext == NULL)
        {
            LOG_ERROR("OpenGL 3.1 context could not be created! SDL Error: " << SDL_GetError());
            return false;
        }
    }

    LOG_INFO("GL_VERSION = " << glGetString(GL_VERSION));

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
    {
        LOG_ERROR("Error initializing GLEW! " << glewGetErrorString(glewError));
    }

    // Use Vsync
    // FIXME: VSYNC disabled for now - remove before merge to master!
    if (SDL_GL_SetSwapInterval(0) < 0)
    {
        LOG_ERROR("Warning: Unable to set VSync! SDL Error: " << SDL_GetError());
    }

    // Check supported extensions by the GPU
    if (!glewIsSupported("GL_ARB_vertex_array_object GL_ARB_framebuffer_object GL_ARB_explicit_attrib_location"))
    {
        ALIVE_FATAL("Your graphics device is not supported, sorry!");
    }

    ImGui::CreateContext();

    // Setup IMGUI for texture debugging
    ImGui_ImplSDL2_InitForOpenGL(mWindow, mContext);
    ImGui_ImplOpenGL3_Init(glslVer);

    // Create and bind the VAO, and never touch it again! Wahey.
    GL_VERIFY(glGenVertexArrays(1, &mVAO));
    GL_VERIFY(glBindVertexArray(mVAO));

    //mTextureShader.LoadFromFile("shaders/texture.vsh", "shaders/texture.fsh");
    mPsxShader.LoadSource(gShader_PsxVSH, gShader_PsxFSH);

    // ROZZA Init passthru shader
    mPassthruShader.LoadSource(gShader_PassthruVSH, gShader_PassthruFSH);

    // ROZZA Blending
    GL_VERIFY(glEnable(GL_BLEND));
    GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // ROZZA Init framebuffer for render to texture
    InitPsxFramebuffer(0);
    InitPsxFramebuffer(1);

    return true;
}

void OpenGLRenderer::CreateBackBuffer(bool /*filter*/, s32 /*format*/, s32 /*w*/, s32 /*h*/)
{
}

void OpenGLRenderer::Destroy()
{
    ImGui_ImplSDL2_Shutdown();

    mPassthruShader.Free();
    mPsxShader.Free();

    for (auto& it : mTextureCache)
    {
        GL_VERIFY(glDeleteTextures(1, &it.second.mTextureId));
    }
    mTextureCache.clear();

    GL_VERIFY(glDeleteTextures(1, &mCamTexture.mTextureId));
    GL_VERIFY(glDeleteTextures(1, &mFg1Texture.mTextureId));
    GL_VERIFY(glDeleteTextures(1, &mFontTexture.mTextureId));
    GL_VERIFY(glUseProgram(0));

    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GL_VERIFY(glDeleteFramebuffers(2, mPsxFramebufferId));
    GL_VERIFY(glDeleteTextures(2, mPsxFramebufferTexId));

    if (mContext)
    {
        SDL_GL_DeleteContext(mContext);
        mContext = nullptr;
    }
}

void OpenGLRenderer::Draw(Prim_Sprt& sprt)
{
    if (!gRenderEnable_SPRT)
    {
        return;
    }

    if (sprt.mAnim == nullptr)
    {
        return;
    }

    const u32 textureId = PrepareTextureFromAnim(*sprt.mAnim);

    mPsxShader.Use();

    f32 r = sprt.mBase.header.rgb_code.r;
    f32 g = sprt.mBase.header.rgb_code.g;
    f32 b = sprt.mBase.header.rgb_code.b;

    // Bind the source framebuffer
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[GL_FRAMEBUFFER_PSX_SRC]));

    // Set sampler uniforms
    mPsxShader.Uniform1i("texTextureData", 0);     // Set texTextureData to GL_TEXTURE0
    mPsxShader.Uniform1i("texAdditionalData", 1);  // Set texAdditionalData to GL_TEXTURE1
    mPsxShader.Uniform1i("texFramebufferData", 2); // Set texFramebufferData to GL_TEXTURE2

    bool isSemiTrans = GetPolyIsSemiTrans(&sprt);
    bool isShaded = GetPolyIsShaded(&sprt);
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    mPsxShader.Uniform1i("fsIsSemiTrans", isSemiTrans);
    mPsxShader.Uniform1i("fsIsShaded", isShaded);
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    const GLuint indexData[6] = {1, 0, 3, 3, 0, 2};

    mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_DEFAULT_FT4);

    GL_VERIFY(glActiveTexture(GL_TEXTURE0));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, textureId));

    std::shared_ptr<TgaData> pTga = sprt.mAnim->mAnimRes.mTgaPtr;

    Renderer_BindPalette(pTga->mPal);

    f32 u0 = static_cast<f32>(U0(&sprt)) / static_cast<f32>(pTga->mWidth);
    f32 v0 = static_cast<f32>(V0(&sprt)) / static_cast<f32>(pTga->mHeight);

    f32 u1 = static_cast<f32>(U0(&sprt) + sprt.field_14_w) / static_cast<f32>(pTga->mWidth);
    f32 v1 = static_cast<f32>(V0(&sprt) + sprt.field_16_h) / static_cast<f32>(pTga->mHeight);

    VertexData verts[4] = {
        {(f32) sprt.mBase.vert.x, (f32) sprt.mBase.vert.y, 0, r, g, b, u0, v0},
        {(f32) sprt.mBase.vert.x + sprt.field_14_w, (f32) sprt.mBase.vert.y, 0, r, g, b, u1, v0},
        {(f32) sprt.mBase.vert.x, (f32) sprt.mBase.vert.y + sprt.field_16_h, 0, r, g, b, u0, v1},
        {(f32) sprt.mBase.vert.x + sprt.field_14_w, (f32) sprt.mBase.vert.y + sprt.field_16_h, 0, r, g, b, u1, v1}};
    DrawTriangles(verts, 4, indexData, 6);

    mPsxShader.UnUse();

    // Unbind the source framebuffer, just to be safe so drawing to it doesn't
    // blow up
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, 0));

    CompleteDraw();
}

static GLuint TempGasEffectTexture = 0;

void OpenGLRenderer::Draw(Prim_GasEffect& gasEffect)
{
    if (!gRenderEnable_GAS)
        return;

    if (TempGasEffectTexture == 0)
        TempGasEffectTexture = Renderer_CreateTexture(GL_LINEAR);

    if (gasEffect.pData == nullptr)
        return;

    s32 gasWidth = (gasEffect.w - gasEffect.x);
    s32 gasHeight = (gasEffect.h - gasEffect.y);

    glBindTexture(GL_TEXTURE_2D, TempGasEffectTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, gasWidth / 4, gasHeight / 2, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, gasEffect.pData);

    /*mTextureShader.Use();
    mTextureShader.Uniform1i("m_Dithered", 1);
    mTextureShader.Uniform1i("m_DitherWidth", gasWidth);
    mTextureShader.Uniform1i("m_DitherHeight", gasHeight);
    DrawTexture(TempGasEffectTexture, (f32) gasEffect.x, (f32) gasEffect.y, (f32) gasWidth, (f32) gasHeight);
    mTextureShader.Use();
    mTextureShader.Uniform1i("m_Dithered", 0);*/
}

void OpenGLRenderer::Draw(Prim_Tile& tile)
{
    if (!gRenderEnable_TILE)
        return;

    // todo: texturing ?
    const f32 r = tile.mBase.header.rgb_code.r / 255.0f;
    const f32 g = tile.mBase.header.rgb_code.g / 255.0f;
    const f32 b = tile.mBase.header.rgb_code.b / 255.0f;

    const VertexData verts[4] = {
        {0, 0, 0, r, g, b, 0, 0},
        {1, 0, 0, r, g, b, 1, 0},
        {1, 1, 0, r, g, b, 1, 1},
        {0, 1, 0, r, g, b, 0, 1}};

    mPsxShader.Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    const GLuint indexData[6] = {0, 1, 3, 3, 1, 2};
    DrawTriangles(verts, 4, indexData, 6);

    mPsxShader.UnUse();
}

void OpenGLRenderer::Draw(Line_F2& line)
{
    if (!gRenderEnable_F2)
        return;

    const VertexData verts[2] = {
        {(f32) X0(&line), (f32) Y0(&line), 0, (f32) R0(&line), (f32) G0(&line), (f32) B0(&line), 0, 0},
        {(f32) X1(&line), (f32) Y1(&line), 0, (f32) R0(&line), (f32) G0(&line), (f32) B0(&line), 0, 0}};

    bool isSemiTrans = GetPolyIsSemiTrans(&line);
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    mPsxShader.Use();

    // Bind the source framebuffer
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[GL_FRAMEBUFFER_PSX_SRC]));

    // Set sampler uniforms
    mPsxShader.Uniform1i("texFramebufferData", 2); // Set texFramebufferData to GL_TEXTURE2

    mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_FLAT);
    mPsxShader.Uniform1i("fsIsSemiTrans", isSemiTrans);
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    const GLuint indexData[2] = {0, 1};
    DrawLines(verts, 2, indexData, 2);

    mPsxShader.UnUse();

    // Unbind the source framebuffer, just to be safe so drawing to it doesn't
    // blow up
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, 0));
}

void OpenGLRenderer::Draw(Line_G2& line)
{
    if (!gRenderEnable_G2)
    {
        return;
    }

    const VertexData verts[2] = {
        {(f32) X0(&line), (f32) Y0(&line), 0, (f32) R0(&line), (f32) G0(&line), (f32) B0(&line), 0, 0},
        {(f32) X1(&line), (f32) Y1(&line), 0, (f32) R1(&line), (f32) G1(&line), (f32) B1(&line), 0, 0}};

    bool isSemiTrans = GetPolyIsSemiTrans(&line);
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    mPsxShader.Use();

    // Bind the source framebuffer
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[GL_FRAMEBUFFER_PSX_SRC]));

    // Set sampler uniforms
    mPsxShader.Uniform1i("texFramebufferData", 2); // Set texFramebufferData to GL_TEXTURE2

    mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_FLAT);
    mPsxShader.Uniform1i("fsIsSemiTrans", isSemiTrans);
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    const GLuint indexData[2] = {0, 1};
    DrawLines(verts, 2, indexData, 2);

    mPsxShader.UnUse();

    // Unbind the source framebuffer, just to be safe so drawing to it doesn't
    // blow up
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, 0));
}

void OpenGLRenderer::Draw(Line_G4& line)
{
    if (!gRenderEnable_G4)
    {
        return;
    }

    const VertexData verts[4] = {
        {(f32) X0(&line), (f32) Y0(&line), 0, (f32) R0(&line), (f32) G0(&line), (f32) B0(&line), 0, 0},
        {(f32) X1(&line), (f32) Y1(&line), 0, (f32) R1(&line), (f32) G1(&line), (f32) B1(&line), 0, 0},
        {(f32) X2(&line), (f32) Y2(&line), 0, (f32) R2(&line), (f32) G2(&line), (f32) B2(&line), 0, 0},
        {(f32) X3(&line), (f32) Y3(&line), 0, (f32) R3(&line), (f32) G3(&line), (f32) B3(&line), 0, 0}};

    bool isSemiTrans = GetPolyIsSemiTrans(&line);
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    mPsxShader.Use();

    // Bind the source framebuffer
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[GL_FRAMEBUFFER_PSX_SRC]));

    // Set sampler uniforms
    mPsxShader.Uniform1i("texFramebufferData", 2); // Set texFramebufferData to GL_TEXTURE2

    mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_FLAT);
    mPsxShader.Uniform1i("fsIsSemiTrans", isSemiTrans);
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    const GLuint indexData[4] = {0, 1, 2, 3};
    DrawLines(verts, 4, indexData, 4);

    mPsxShader.UnUse();

    // Unbind the source framebuffer, just to be safe so drawing to it doesn't
    // blow up
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, 0));
}

void OpenGLRenderer::Draw(Poly_F3& poly)
{
    if (!gRenderEnable_F3)
    {
        return;
    }

    const VertexData verts[3] = {
        {(f32) X0(&poly), (f32) Y0(&poly), 0, (f32) R0(&poly), (f32) G0(&poly), (f32) B0(&poly), 0, 0},
        {(f32) X1(&poly), (f32) Y1(&poly), 0, (f32) R0(&poly), (f32) G0(&poly), (f32) B0(&poly), 0, 0},
        {(f32) X2(&poly), (f32) Y2(&poly), 0, (f32) R0(&poly), (f32) G0(&poly), (f32) B0(&poly), 0, 0}};

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    mPsxShader.Use();

    // Bind the source framebuffer
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[GL_FRAMEBUFFER_PSX_SRC]));

    // Set sampler uniforms
    mPsxShader.Uniform1i("texFramebufferData", 2); // Set texFramebufferData to GL_TEXTURE2

    mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_FLAT);
    mPsxShader.Uniform1i("fsIsSemiTrans", isSemiTrans);
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    const GLuint indexData[3] = {0, 1, 2};
    DrawTriangles(verts, 3, indexData, 3);

    mPsxShader.UnUse();

    // Unbind the source framebuffer, just to be safe so drawing to it doesn't
    // blow up
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, 0));

    CompleteDraw();
}

void OpenGLRenderer::Draw(Poly_G3& poly)
{
    if (!gRenderEnable_G3)
    {
        return;
    }

    const VertexData verts[3] = {
        {(f32) X0(&poly), (f32) Y0(&poly), 0, (f32) R0(&poly), (f32) G0(&poly), (f32) B0(&poly), 0, 0},
        {(f32) X1(&poly), (f32) Y1(&poly), 0, (f32) R1(&poly), (f32) G1(&poly), (f32) B1(&poly), 0, 0},
        {(f32) X2(&poly), (f32) Y2(&poly), 0, (f32) R2(&poly), (f32) G2(&poly), (f32) B2(&poly), 0, 0}};

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    mPsxShader.Use();

    // Bind the source framebuffer
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[GL_FRAMEBUFFER_PSX_SRC]));

    // Set sampler uniforms
    mPsxShader.Uniform1i("texFramebufferData", 2); // Set texFramebufferData to GL_TEXTURE2

    mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_FLAT);
    mPsxShader.Uniform1i("fsIsSemiTrans", isSemiTrans);
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    const GLuint indexData[3] = {0, 1, 2};
    DrawTriangles(verts, 3, indexData, 3);

    mPsxShader.UnUse();

    // Unbind the source framebuffer, just to be safe so drawing to it doesn't
    // blow up
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, 0));

    CompleteDraw();
}

void OpenGLRenderer::Draw(Poly_F4& poly)
{
    if (!gRenderEnable_F4)
        return;

    const f32 r = poly.mBase.header.rgb_code.r;
    const f32 g = poly.mBase.header.rgb_code.g;
    const f32 b = poly.mBase.header.rgb_code.b;

    const VertexData verts[4] = {
        {(f32) X0(&poly), (f32) Y0(&poly), 0, r, g, b, 0, 0},
        {(f32) X1(&poly), (f32) Y1(&poly), 0, r, g, b, 0, 0},
        {(f32) X2(&poly), (f32) Y2(&poly), 0, r, g, b, 0, 0},
        {(f32) X3(&poly), (f32) Y3(&poly), 0, r, g, b, 0, 0}};

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    mPsxShader.Use();

    // Bind the source framebuffer
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[GL_FRAMEBUFFER_PSX_SRC]));

    // Set sampler uniforms
    mPsxShader.Uniform1i("texFramebufferData", 2); // Set texFramebufferData to GL_TEXTURE2

    mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_FLAT);
    mPsxShader.Uniform1i("fsIsSemiTrans", isSemiTrans);
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    const GLuint indexData[6] = {0, 1, 2, 2, 1, 3};
    DrawTriangles(verts, 4, indexData, 6);

    mPsxShader.UnUse();

    // Unbind the source framebuffer, just to be safe so drawing to it doesn't
    // blow up
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, 0));

    CompleteDraw();
}

void OpenGLRenderer::Draw(Poly_FT4& poly)
{
    if (!gRenderEnable_FT4)
        return;

    const u32 textureId = PrepareTextureFromPoly(poly);

    mPsxShader.Use();

    f32 r = poly.mBase.header.rgb_code.r;
    f32 g = poly.mBase.header.rgb_code.g;
    f32 b = poly.mBase.header.rgb_code.b;

    // Bind the source framebuffer
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[GL_FRAMEBUFFER_PSX_SRC]));

    // Set sampler uniforms
    mPsxShader.Uniform1i("texTextureData", 0);     // Set texTextureData to GL_TEXTURE0
    mPsxShader.Uniform1i("texAdditionalData", 1);  // Set texAdditionalData to GL_TEXTURE1
    mPsxShader.Uniform1i("texFramebufferData", 2); // Set texFramebufferData to GL_TEXTURE2

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    bool isShaded = GetPolyIsShaded(&poly);
    u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

    mPsxShader.Uniform1i("fsIsSemiTrans", isSemiTrans);
    mPsxShader.Uniform1i("fsIsShaded", isShaded);
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    const GLuint indexData[6] = {1, 0, 3, 3, 0, 2};

    if (poly.mFg1)
    {
        mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_FG1);

        GL_VERIFY(glActiveTexture(GL_TEXTURE0))
        GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mCamTexture.mTextureId))

        GL_VERIFY(glActiveTexture(GL_TEXTURE1))
        GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mFg1Texture.mTextureId))

        VertexData verts[4] = {
            {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, r, g, b, 0, 0},
            {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0, r, g, b, 1, 0},
            {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0, r, g, b, 0, 1},
            {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0, r, g, b, 1, 1}};
        DrawTriangles(verts, 4, indexData, 6);
    }
    else if (poly.mCam)
    {
        mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_CAM);

        GL_VERIFY(glActiveTexture(GL_TEXTURE0));
        GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mCamTexture.mTextureId));

        VertexData verts[4] = {
            {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, r, g, b, 0, 0},
            {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0, r, g, b, 1, 0},
            {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0, r, g, b, 0, 1},
            {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0, r, g, b, 1, 1}};
        DrawTriangles(verts, 4, indexData, 6);
    }
    else if (poly.mAnim)
    {
        mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_DEFAULT_FT4);

        GL_VERIFY(glActiveTexture(GL_TEXTURE0));
        GL_VERIFY(glBindTexture(GL_TEXTURE_2D, textureId));

        Renderer_BindPalette(poly.mAnim->mAnimRes.mTgaPtr->mPal);

        const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);

        std::shared_ptr<TgaData> pTga = poly.mAnim->mAnimRes.mTgaPtr;
        f32 u0 = static_cast<f32>(pHeader->mSpriteSheetX) / static_cast<f32>(pTga->mWidth);
        f32 v0 = static_cast<f32>(pHeader->mSpriteSheetY) / static_cast<f32>(pTga->mHeight);

        f32 u1 = u0 + static_cast<f32>(pHeader->mWidth) / static_cast<f32>(pTga->mWidth);
        f32 v1 = v0 + static_cast<f32>(pHeader->mHeight) / static_cast<f32>(pTga->mHeight);

        if (poly.mFlipX)
        {
            std::swap(u0, u1);
        }

        if (poly.mFlipY)
        {
            std::swap(v1, v0);
        }

        VertexData verts[4] = {
            {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, r, g, b, u0, v0},
            {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0, r, g, b, u1, v0},
            {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0, r, g, b, u0, v1},
            {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0, r, g, b, u1, v1}};
        DrawTriangles(verts, 4, indexData, 6);
    }
    else if (poly.mFont)
    {
        mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_DEFAULT_FT4);

        GL_VERIFY(glActiveTexture(GL_TEXTURE0));
        GL_VERIFY(glBindTexture(GL_TEXTURE_2D, textureId));

        std::shared_ptr<TgaData> pTga = poly.mFont->field_C_resource_id.mTgaPtr;

        Renderer_BindPalette(pTga->mPal);

        f32 u0 = static_cast<f32>(U0(&poly)) / static_cast<f32>(pTga->mWidth);
        f32 v0 = static_cast<f32>(V0(&poly)) / static_cast<f32>(pTga->mHeight);

        f32 u1 = static_cast<f32>(U3(&poly)) / static_cast<f32>(pTga->mWidth);
        f32 v1 = static_cast<f32>(V3(&poly)) / static_cast<f32>(pTga->mHeight);

        VertexData verts[4] = {
            {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, r, g, b, u0, v0},
            {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0, r, g, b, u1, v0},
            {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0, r, g, b, u0, v1},
            {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0, r, g, b, u1, v1}};
        DrawTriangles(verts, 4, indexData, 6);
    }
    else
    {
        return;
    }

    mPsxShader.UnUse();

    // Unbind the source framebuffer, just to be safe so drawing to it doesn't
    // blow up
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, 0));

    CompleteDraw();
}

void OpenGLRenderer::Draw(Poly_G4& poly)
{
    if (!gRenderEnable_G4)
        return;

    const VertexData verts[4] = {
        {(f32) X0(&poly), (f32) Y0(&poly), 0, (f32) R0(&poly), (f32) G0(&poly), (f32) B0(&poly), 0, 0},
        {(f32) X1(&poly), (f32) Y1(&poly), 0, (f32) R1(&poly), (f32) G1(&poly), (f32) B1(&poly), 0, 0},
        {(f32) X2(&poly), (f32) Y2(&poly), 0, (f32) R2(&poly), (f32) G2(&poly), (f32) B2(&poly), 0, 0},
        {(f32) X3(&poly), (f32) Y3(&poly), 0, (f32) R3(&poly), (f32) G3(&poly), (f32) B3(&poly), 0, 0}};

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    mPsxShader.Use();

    // Bind the source framebuffer
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[GL_FRAMEBUFFER_PSX_SRC]));

    // Set sampler uniforms
    mPsxShader.Uniform1i("texFramebufferData", 2); // Set texFramebufferData to GL_TEXTURE2

    mPsxShader.Uniform1i("fsDrawType", GL_PSX_DRAW_MODE_FLAT);
    mPsxShader.Uniform1i("fsIsSemiTrans", isSemiTrans);
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    const GLuint indexData[6] = {0, 1, 2, 1, 2, 3};
    DrawTriangles(verts, 4, indexData, 6);

    mPsxShader.UnUse();

    // Unbind the source framebuffer, just to be safe so drawing to it doesn't
    // blow up
    GL_VERIFY(glActiveTexture(GL_TEXTURE2));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, 0));

    CompleteDraw();
}

void OpenGLRenderer::EndFrame()
{
    if (!mFrameStarted)
    {
        return;
    }

    s32 wW, wH;
    SDL_GetWindowSize(mWindow, &wW, &wH);
    GL_VERIFY(glViewport(0, 0, wW, wH));

    // Draw the final composed framebuffer to the screen
    GL_VERIFY(glDisable(GL_SCISSOR_TEST));
    DrawFramebufferToFramebuffer(
        GL_FRAMEBUFFER_PSX_DST,
        GL_FRAMEBUFFER_SCREEN,
        mScreenOffsetX,
        mScreenOffsetY,
        GL_FRAMEBUFFER_PSX_WIDTH,
        GL_FRAMEBUFFER_PSX_HEIGHT,
        0,
        0,
        GL_FRAMEBUFFER_PSX_WIDTH,
        GL_FRAMEBUFFER_PSX_HEIGHT);

    // Switch back to the main frame buffer
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // Do ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(mWindow);
    ImGui::NewFrame();

    DebugWindow();

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Throw away any errors caused by ImGui - this is necessary for AMD GPUs
    // (AMD Radeon HD 7310 with driver 8.982.10.5000)
    glGetError();

    // Render end
    SDL_GL_SwapWindow(mWindow);

    mFrameStarted = false;
}

void OpenGLRenderer::OutputSize(s32* w, s32* h)
{
    *w = 640;
    *h = 480;
    //SDL_GetRendererOutputSize(mRenderer, w, h);
}

bool OpenGLRenderer::PalAlloc(PalRecord& record)
{
    PSX_RECT rect = {};
    // TODO: Stop depending on this
    const bool ret = Pal_Allocate(&rect, record.depth);
    record.x = rect.x;
    record.y = rect.y;
    return ret;
}

// This function should free both vrams allocations AND palettes, cause theyre kinda the same thing.
void OpenGLRenderer::PalFree(const PalRecord& record)
{
    Pal_free(PSX_Point{record.x, record.y}, record.depth); // TODO: Stop depending on this

    /*
    Renderer_FreePalette({
        record.x,
        record.y,
    });*/

    /*
    Renderer_FreeTexture({
        record.x,
        record.y,
    });*/
}

void OpenGLRenderer::PalSetData(const PalRecord& record, const u8* pPixels)
{
    PSX_RECT rect = {};
    rect.x = record.x;
    rect.y = record.y;
    rect.w = record.depth;
    rect.h = 1;
    Upload(IRenderer::BitDepth::e16Bit, rect, pPixels);
}

void OpenGLRenderer::SetClip(Prim_PrimClipper& clipper)
{
    SDL_Rect rect;
    rect.x = clipper.field_C_x;
    rect.y = clipper.field_E_y;
    rect.w = clipper.mBase.header.mRect.w;
    rect.h = clipper.mBase.header.mRect.h;

    if (rect.x == 0 && rect.y == 0 && rect.w == 1 && rect.h == 1)
    {
        GL_VERIFY(glDisable(GL_SCISSOR_TEST));
        return;
    }

    GL_VERIFY(glEnable(GL_SCISSOR_TEST));
    GL_VERIFY(glScissor(rect.x, GL_FRAMEBUFFER_PSX_HEIGHT - rect.y - rect.h, rect.w, rect.h));
}

void OpenGLRenderer::SetScreenOffset(Prim_ScreenOffset& offset)
{
    mScreenOffsetX = (s32) offset.field_C_xoff;
    mScreenOffsetY = (s32) offset.field_E_yoff;
}

void OpenGLRenderer::SetTPage(u16 tPage)
{
    mGlobalTPage = tPage;
}

void OpenGLRenderer::StartFrame(s32 xOff, s32 yOff)
{
    mStats.Reset();

    mFrameNumber++;

    FreeUnloadedAnimTextures();

    mFrameStarted = true;

    mScreenOffsetX = xOff;
    mScreenOffsetY = yOff;

    // Always render to destination buffer (1)
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId[GL_FRAMEBUFFER_PSX_DST]));
    GL_VERIFY(glViewport(0, 0, GL_FRAMEBUFFER_PSX_WIDTH, GL_FRAMEBUFFER_PSX_HEIGHT));
}

bool OpenGLRenderer::UpdateBackBuffer(const void* /*pPixels*/, s32 /*pitch*/)
{
    return true;
}

void OpenGLRenderer::Upload(BitDepth /*bitDepth*/, const PSX_RECT& /*rect*/, const u8* /*pPixels*/)
{
    /*
    // Palettes are the only texture that is 1 in height.
    // So we're gonna hook in here to steal palettes for our
    // new renderer.
    if (rect.h == 1)
    {
        if (bitDepth == BitDepth::e16Bit)
        {
            Renderer_LoadPalette({rect.x, rect.y}, reinterpret_cast<const u8*>(pPixels), rect.w);
        }
        return;
    }

    if (!Renderer_TexExists(rect))
    {
        TextureCache cache = {};
        cache.mTextureID = Renderer_CreateTexture();
        cache.mVramRect = rect;

        gRendererTextures.push_back(cache);
    }

    TextureCache* tc = Renderer_TexFromVRam(rect);
    tc->mVramRect = rect;

    if (ImGui::Begin("VRAM", nullptr, ImGuiWindowFlags_MenuBar))
    {
        ImGui::SetCursorPos(ImVec2(static_cast<f32>(tc->mVramRect.x), static_cast<f32>(tc->mVramRect.y + 50)));
        f32 textureWidth = static_cast<f32>(tc->mVramRect.w);
        f32 textureHeight = static_cast<f32>(tc->mVramRect.h);
        ImVec2 xpos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImVec2(xpos.x + textureWidth, xpos.y + textureHeight);
        ImGui::GetWindowDrawList()->AddRect(xpos, size, ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)));
    }
    ImGui::End();

    glBindTexture(GL_TEXTURE_2D, tc->mTextureID);

    bool aoFG1 = true;

    if (rect.h == 240)
    {
        bitDepth = BitDepth::e16Bit;
        aoFG1 = false;
    }

    switch (bitDepth)
    {
        case BitDepth::e16Bit:
            if (aoFG1)
            {
                RGBAPixel* pixelBuffer = reinterpret_cast<RGBAPixel*>(gDecodeBuffer);
                ConvertAOFG1(pPixels, pixelBuffer, rect.w * rect.h);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rect.w, rect.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, rect.w, rect.h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pPixels);

                if (rect.w == 16 && rect.h == 240)
                {
                    StitchAOCam(rect.x, rect.y - 272, rect.w, rect.h, pPixels);

                    if (rect.x == 624)
                    {
                        if (mBackgroundTexture == 0)
                            mBackgroundTexture = Renderer_CreateTexture();

                        glBindTexture(GL_TEXTURE_2D, mBackgroundTexture);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, 640, 240, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, gDecodeBuffer);
                    }
                }
            }
            break;

        case BitDepth::e8Bit:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, rect.w * 2, rect.h, 0, GL_RED, GL_UNSIGNED_BYTE, pPixels);
            break;
        case BitDepth::e4Bit: // Usually only fonts.
            Convert4bppTextureFont(rect, pPixels);
            break;

        default:
            ALIVE_FATAL("unknown bit depth");
            break;
    }*/
}


// ROZZA FRAMEBUFFER STUFF

void OpenGLRenderer::CompleteDraw()
{
    // Copy the current state of the framebuffer (post-draw) to the 'source'
    // framebuffer ready for the next draw call to use
    DrawFramebufferToFramebuffer(
        GL_FRAMEBUFFER_PSX_DST,
        GL_FRAMEBUFFER_PSX_SRC);
}

void OpenGLRenderer::FreeUnloadedAnimTextures()
{
    auto it = mTextureCache.begin();
    while (it != mTextureCache.end())
    {
        if (std::abs(it->second.mLastUsedFrame - mFrameNumber) > 30*200)
        {
            it = mTextureCache.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void OpenGLRenderer::DrawFramebufferToFramebuffer(int src, int dst)
{
    DrawFramebufferToFramebuffer(
        src,
        dst,
        0,
        0,
        GL_FRAMEBUFFER_PSX_WIDTH,
        GL_FRAMEBUFFER_PSX_HEIGHT,
        0,
        0,
        GL_FRAMEBUFFER_PSX_WIDTH,
        GL_FRAMEBUFFER_PSX_HEIGHT);
}

void OpenGLRenderer::DrawFramebufferToFramebuffer(int src, int dst, s32 x, s32 y, s32 width, s32 height, s32 clipX, s32 clipY, s32 clipWidth, s32 clipHeight)
{
    if (src == GL_FRAMEBUFFER_SCREEN)
    {
        ALIVE_FATAL("OpenGL: Cannot use the screen as the framebuffer source.");
    }

    // Retrieve the source framebuffer texture and destination framebuffer IDs
    GLuint srcFramebufferTexId = mPsxFramebufferTexId[src];
    GLuint dstFramebufferId = dst == GL_FRAMEBUFFER_SCREEN ? 0 : mPsxFramebufferId[dst];

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
        (f32) clipX, (f32) (clipY + clipHeight),
        (f32) clipX, (f32) clipY,
        (f32) (clipX + clipWidth), (f32) (clipY + clipHeight),

        (f32) (clipX + clipWidth), (f32) (clipY + clipHeight),
        (f32) clipX, (f32) clipY,
        (f32) (clipX + clipWidth), (f32) clipY};

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
    mPassthruShader.Use();

    mPassthruShader.Uniform1i("TextureSampler", 0);
    mPassthruShader.UniformVec2("vsTexSize", {GL_FRAMEBUFFER_PSX_WIDTH, GL_FRAMEBUFFER_PSX_HEIGHT});

    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, dstFramebufferId));

    // If we're drawing to the screen, then we do want to clear what's there,
    // otherwise we'll have leftovers during screen shake!
    if (dst == GL_FRAMEBUFFER_SCREEN)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    GL_VERIFY(glActiveTexture(GL_TEXTURE0));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, srcFramebufferTexId));

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

    mPassthruShader.UnUse();

    // Set the framebuffer target back to the destination
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId[GL_FRAMEBUFFER_PSX_DST]));
}

u32 OpenGLRenderer::GetTPageBlendMode(u16 tpage)
{
    return ((u32)tpage >> 4) & 3;
}

void OpenGLRenderer::InitPsxFramebuffer(int index)
{
    GLuint* pFbId = &mPsxFramebufferId[index];
    GLuint* pFbTexId = &mPsxFramebufferTexId[index];

    GL_VERIFY(glGenFramebuffers(1, pFbId));
    GL_VERIFY(glGenTextures(1, pFbTexId));

    // Texture init
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, *pFbTexId));

    GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, 0));

    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    // Framebuffer init
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, *pFbId));
    GL_VERIFY(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *pFbTexId, 0));

    GLenum fbTargets[1] = {GL_COLOR_ATTACHMENT0};
    GL_VERIFY(glDrawBuffers(1, fbTargets));
}

// END ROZZA FRAMEBUFFER STUFF


void OpenGLRenderer::DebugWindow()
{
    //ImGuiStyle& style = ImGui::GetStyle();
    //ImGuiIO& io = ImGui::GetIO();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Developer"))
        {
            if (ImGui::BeginMenu("Render Elements"))
            {
                ImGui::MenuItem("SPRT", nullptr, &gRenderEnable_SPRT);
                ImGui::MenuItem("TILE", nullptr, &gRenderEnable_TILE);
                ImGui::MenuItem("GAS", nullptr, &gRenderEnable_GAS);
                ImGui::MenuItem("FT4", nullptr, &gRenderEnable_FT4);
                ImGui::MenuItem("G4", nullptr, &gRenderEnable_G4);
                ImGui::MenuItem("G3", nullptr, &gRenderEnable_G3);
                ImGui::MenuItem("G2", nullptr, &gRenderEnable_G2);
                ImGui::MenuItem("F4", nullptr, &gRenderEnable_F4);
                ImGui::MenuItem("F3", nullptr, &gRenderEnable_F3);
                ImGui::MenuItem("F2", nullptr, &gRenderEnable_F2);

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    //ImGui::ShowDemoWindow();
    /*
    if (ImGui::Begin("Texture Window", nullptr, ImGuiWindowFlags_MenuBar))
    {
        f32 widthSpace = ImGui::GetContentRegionAvailWidth();
        f32 currentWidth = 0;
        for (size_t i = 0; i < gRendererTextures.size(); i++)
        {
            f32 textureWidth = static_cast<f32>(gRendererTextures[i].mVramRect.w);
            f32 textureHeight = static_cast<f32>(gRendererTextures[i].mVramRect.h);

            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

            if (currentWidth >= widthSpace)
                currentWidth = 0;
            else
                ImGui::SameLine();

            ImGui::Image(GL_TO_IMGUI_TEX(gRendererTextures[i].mTextureID), {textureWidth, textureHeight});
            ImVec2 pos = ImGui::GetCursorScreenPos();
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%d, %d, %d, %d", gRendererTextures[i].mVramRect.x, gRendererTextures[i].mVramRect.y, gRendererTextures[i].mVramRect.w, gRendererTextures[i].mVramRect.h);
                ImVec2 uv0 = ImVec2(0.0f, 0.0f);
                ImVec2 uv1 = ImVec2(1.0f, 1.0f);
                ImGui::Image(GL_TO_IMGUI_TEX(gRendererTextures[i].mTextureID), ImVec2(textureWidth * 4, textureHeight * 4), uv0, uv1, tint_col, border_col);
                ImGui::EndTooltip();
            }
            ImVec2 imgSize = ImGui::GetItemRectSize();
            currentWidth += imgSize.x + style.ItemSpacing.x;
        }
    }
    ImGui::End();*/

    if (ImGui::Begin("Render stats"))
    {
        ImGui::Text("Cams %d", mStats.mCamUploadCount);
        ImGui::Text("Fg1s %d", mStats.mFg1UploadCount);
        ImGui::Text("Anims %d", mStats.mAnimUploadCount);
        ImGui::Text("Pals %d", mStats.mPalUploadCount);
        ImGui::Text("Fonts %d", mStats.mFontUploadCount);
    }
    ImGui::End();

    /*
    if (ImGui::Begin("Palettes", nullptr, ImGuiWindowFlags_MenuBar))
    {
        f32 width = ImGui::GetWindowContentRegionWidth();
        for (auto& pal : gRendererPals)
        {
            ImGui::Image(GL_TO_IMGUI_TEX(pal.mPalTextureID), ImVec2(width, 16));
        }
    }
    ImGui::End();
    */

    /*
    if (ImGui::Begin("VRAM", nullptr, ImGuiWindowFlags_MenuBar))
    {
        ImVec2 pos = ImGui::GetWindowPos();

        for (s32 i = 0; i < (1500 / 64); i++)
        {
            ImVec2 pos1Line = ImVec2(pos.x + (i * 64), pos.y);
            ImVec2 pos2Line = ImVec2(pos.x + (i * 64), pos.y + 512);
            ImGui::GetWindowDrawList()->AddLine(pos1Line, pos2Line, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.2f)));
        }


        for (size_t i = 0; i < gRendererTextures.size(); i++)
        {
            ImGui::SetCursorPos(ImVec2(static_cast<f32>(gRendererTextures[i].mVramRect.x), static_cast<f32>(gRendererTextures[i].mVramRect.y + 50)));
            ImVec2 xpos = ImGui::GetCursorScreenPos();
            f32 textureWidth = static_cast<f32>(gRendererTextures[i].mVramRect.w);
            f32 textureHeight = static_cast<f32>(gRendererTextures[i].mVramRect.h);

            ImVec2 size = ImVec2(xpos.x + textureWidth, xpos.y + textureHeight);
            ImGui::Image(GL_TO_IMGUI_TEX(gRendererTextures[i].mTextureID), {textureWidth, textureHeight});
            ImGui::GetWindowDrawList()->AddRect(xpos, size, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.3f)));
        }
        if (ImGui::IsWindowHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("%d, %d", (s32)(io.MousePos.x - pos.x), (s32)(io.MousePos.y - pos.y));
            ImGui::EndTooltip();
        }
    }
    ImGui::End();
    */
}

void OpenGLRenderer::InitAttributes()
{
    // Tell GL how to transfer our Vertex struct to our shaders.
    //glBindVertexArray(mVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (s8*) NULL + 12);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (s8*) NULL + 24);
    glEnableVertexAttribArray(2);
}

void OpenGLRenderer::DrawLines(const VertexData* pVertData, s32 vertSize, const GLuint* pIndData, s32 indSize)
{
    GLuint iboId, vboId;

    // Set our new vectors
    GL_VERIFY(glGenBuffers(1, &iboId));
    GL_VERIFY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId));
    GL_VERIFY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * sizeof(GLuint), pIndData, GL_STREAM_DRAW));

    GL_VERIFY(glGenBuffers(1, &vboId));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, vboId));
    GL_VERIFY(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertSize, pVertData, GL_STREAM_DRAW));

    InitAttributes();

    // TODO: Make lines scale with Window
    //glLineWidth(2.0f);

    //Set index data and render
    GL_VERIFY(glDrawElements(GL_LINE_STRIP, indSize, GL_UNSIGNED_INT, NULL));

    GL_VERIFY(glDeleteBuffers(1, &iboId));
    GL_VERIFY(glDeleteBuffers(1, &vboId));

    GL_VERIFY(glDisableVertexAttribArray(0));
    GL_VERIFY(glDisableVertexAttribArray(1));
    GL_VERIFY(glDisableVertexAttribArray(2));
}

void OpenGLRenderer::DrawTexture(GLuint pTexture, f32 /*x*/, f32 /*y*/, f32 /*width*/, f32 /*height*/)
{
    const f32 r = 1.0f;
    const f32 g = 1.0f;
    const f32 b = 1.0f;

    const VertexData verts[4] = {
        {0, 0, 0, r, g, b, 0, 0},
        {1, 0, 0, r, g, b, 1, 0},
        {1, 1, 0, r, g, b, 1, 1},
        {0, 1, 0, r, g, b, 0, 1}};

    mPsxShader.Use();

    //mTextureShader.Uniform1i("m_Sprite", 0); // Set m_Sprite to GL_TEXTURE0

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pTexture);

    const GLuint indexData[6] = {0, 1, 3, 3, 1, 2};
    DrawTriangles(verts, 4, indexData, 6);

    mPsxShader.UnUse();
}

void OpenGLRenderer::DrawTriangles(const VertexData* pVertData, s32 vertSize, const GLuint* pIndData, s32 indSize)
{
    GLuint iboId, vboId;

    // Set our new vectors
    GL_VERIFY(glGenBuffers(1, &iboId));
    GL_VERIFY(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId));
    GL_VERIFY(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * sizeof(GLuint), pIndData, GL_STREAM_DRAW));

    GL_VERIFY(glGenBuffers(1, &vboId));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, vboId));
    GL_VERIFY(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertSize, pVertData, GL_STREAM_DRAW));

    InitAttributes();

    //Set index data and render
    GL_VERIFY(glDrawElements(GL_TRIANGLES, indSize, GL_UNSIGNED_INT, NULL));

    GL_VERIFY(glDeleteBuffers(1, &iboId));
    GL_VERIFY(glDeleteBuffers(1, &vboId));

    GL_VERIFY(glDisableVertexAttribArray(0));
    GL_VERIFY(glDisableVertexAttribArray(1));
    GL_VERIFY(glDisableVertexAttribArray(2));
}
