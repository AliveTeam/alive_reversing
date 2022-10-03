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
#define GL_PSX_DRAW_MODE_GAS         4

#define GL_FRAMEBUFFER_PSX_WIDTH  640
#define GL_FRAMEBUFFER_PSX_HEIGHT 240

#define GL_AVAILABLE_PALETTES 256
#define GL_PALETTE_DEPTH      256


static bool gRenderEnable_SPRT = true;
static bool gRenderEnable_GAS = true;
static bool gRenderEnable_TILE = true;
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



u32 OpenGLRenderer::PreparePalette(AnimationPal& pCache)
{
    if (mPaletteTextureId == 0)
    {
        mPaletteTextureId = Renderer_CreateTexture();

        GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPaletteTextureId));
        GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GL_PALETTE_DEPTH, GL_AVAILABLE_PALETTES, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));
    }

    // Check we don't already have this palette
    u32 paletteHash = HashPalette(&pCache);
    auto searchResult = mPaletteHashes.find(paletteHash);

    if (searchResult != std::end(mPaletteHashes))
    {
        mUsedPalettes.find(paletteHash)->second = true;

        return searchResult->second; // Palette index
    }

    // Decode the new palette
    RGBAPixel dst[256];
    Renderer_DecodePalette(reinterpret_cast<const u8*>(pCache.mPal), dst, 256);

    // Get an index for the new palette
    u32 nextIndex = (u32) mPaletteHashes.size();

    if (nextIndex >= GL_AVAILABLE_PALETTES)
    {
        // Look for a unused slot
        u32 unusedPaletteHash = 0;

        for (auto iter = mUsedPalettes.begin(); iter != mUsedPalettes.end(); iter++)
        {
            if (iter->second) // If still used
            {
                continue;
            }

            // Found an unused one
            unusedPaletteHash = iter->first;
        }

        if (unusedPaletteHash == 0)
        {
            ALIVE_FATAL("Ran out of palettes!");
        }

        // Acquire the index we're taking over
        auto toStealSearchResult = mPaletteHashes.find(unusedPaletteHash);

        nextIndex = toStealSearchResult->first;

        // Bin the old one
        mPaletteHashes.erase(unusedPaletteHash);
        mUsedPalettes.erase(unusedPaletteHash);
    }

    mPaletteHashes.insert(std::make_pair(paletteHash, nextIndex));
    mUsedPalettes.insert(std::make_pair(paletteHash, true));

    // Write palette data
    GL_VERIFY(glActiveTexture(GL_TEXTURE1));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPaletteTextureId));
    GL_VERIFY(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, nextIndex, GL_PALETTE_DEPTH, 1, GL_RGBA, GL_UNSIGNED_BYTE, dst));

    mStats.mPalUploadCount++;

    return nextIndex;
}

u32 OpenGLRenderer::HashPalette(const AnimationPal* pPal)
{
    // This is the 'djb2' algorithm
    const u8* data = reinterpret_cast<const u8*>(pPal->mPal);
    u32 hash = 5381;

    for (int i = 0; i < GL_PALETTE_DEPTH; i++)
    {
        hash = ((hash << 5) + hash) + data[i];
    }

    return hash;
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


void OpenGLRenderer::BltBackBuffer(const SDL_Rect* /*pCopyRect*/, const SDL_Rect* pDst)
{
    mBlitRect = *pDst;
}

void OpenGLRenderer::Clear(u8 r, u8 g, u8 b)
{
    GLboolean scissoring;

    GL_VERIFY(glGetBooleanv(GL_SCISSOR_TEST, &scissoring));

    // We clear the screen framebuffer here
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    if (scissoring)
    {
        GL_VERIFY(glDisable(GL_SCISSOR_TEST));
    }

    GL_VERIFY(glClearColor((f32) r, (f32) g, (f32) b, 1.0f));
    GL_VERIFY(glClear(GL_COLOR_BUFFER_BIT));

    // Set back to the PSX framebuffer
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId));

    if (scissoring)
    {
        GL_VERIFY(glEnable(GL_SCISSOR_TEST));
    }
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
    GL_VERIFY(glGenFramebuffers(1, &mPsxFramebufferId));
    GL_VERIFY(glGenTextures(1, &mPsxFramebufferTexId));

    // Texture init
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId));

    GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, 0));

    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    // Framebuffer init
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId));
    GL_VERIFY(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mPsxFramebufferId, 0));

    GLenum fbTargets[1] = {GL_COLOR_ATTACHMENT0};
    GL_VERIFY(glDrawBuffers(1, fbTargets));

    // Init batch vector
    mBatchData.reserve(sizeof(VertexData) * 200);

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
    GL_VERIFY(glDeleteFramebuffers(1, &mPsxFramebufferId));
    GL_VERIFY(glDeleteTextures(1, &mPsxFramebufferTexId));

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

    std::shared_ptr<TgaData> pTga = sprt.mAnim->mAnimRes.mTgaPtr;

    const u32 sprtTextureId = PrepareTextureFromAnim(*sprt.mAnim);
    const u32 palIndex = PreparePalette(pTga->mPal);

    u32 r = sprt.mBase.header.rgb_code.r;
    u32 g = sprt.mBase.header.rgb_code.g;
    u32 b = sprt.mBase.header.rgb_code.b;

    u32 u0 = U0(&sprt);
    u32 v0 = V0(&sprt);

    u32 u1 = U0(&sprt) + sprt.field_14_w;
    u32 v1 = V0(&sprt) + sprt.field_16_h;

    bool isSemiTrans = GetPolyIsSemiTrans(&sprt);
    bool isShaded = GetPolyIsShaded(&sprt);
    u16 blendMode = GetTPageBlendMode(mGlobalTPage);

    VertexData verts[6] = {
        {sprt.mBase.vert.x, sprt.mBase.vert.y, r, g, b, u0, v0, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
        {sprt.mBase.vert.x + sprt.field_14_w, sprt.mBase.vert.y, r, g, b, u1, v0, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
        {sprt.mBase.vert.x, sprt.mBase.vert.y + sprt.field_16_h, r, g, b, u0, v1, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},

        {sprt.mBase.vert.x + sprt.field_14_w, sprt.mBase.vert.y, r, g, b, u1, v0, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
        {sprt.mBase.vert.x, sprt.mBase.vert.y + sprt.field_16_h, r, g, b, u0, v1, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
        {sprt.mBase.vert.x + sprt.field_14_w, sprt.mBase.vert.y + sprt.field_16_h, r, g, b, u1, v1, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex}};

    PushVertexData(GL_TRIANGLES, verts, 6, blendMode, sprtTextureId, pTga->mWidth, pTga->mWidth, mPaletteTextureId);
}

void OpenGLRenderer::Draw(Prim_GasEffect& gasEffect)
{
    if (!gRenderEnable_GAS)
    {
        return;
    }

    if (mGasTextureId == 0)
    {
        mGasTextureId = Renderer_CreateTexture();
    }

    if (gasEffect.pData == nullptr)
    {
        return;
    }

    s32 gasWidth = (gasEffect.w - gasEffect.x);
    s32 gasHeight = (gasEffect.h - gasEffect.y);

    GL_VERIFY(glActiveTexture(GL_TEXTURE0));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mGasTextureId));
    GL_VERIFY(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, gasWidth / 4, gasHeight / 2, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, gasEffect.pData));

    u32 r = 127;
    u32 g = 127;
    u32 b = 127;

    bool isSemiTrans = true;
    bool isShaded = true;
    u32 blendMode = (u32) TPageAbr::eBlend_0;

    VertexData verts[6] = {
        {gasEffect.x, gasEffect.y, r, g, b, 0, 0, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, 0},
        {gasEffect.w, gasEffect.y, r, g, b, (u32) gasWidth, 0, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, 0},
        {gasEffect.x, gasEffect.h, r, g, b, 0, (u32) gasHeight, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, 0},

        {gasEffect.w, gasEffect.y, r, g, b, (u32) gasWidth, 0, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, 0},
        {gasEffect.x, gasEffect.h, r, g, b, 0, (u32) gasHeight, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, 0},
        {gasEffect.w, gasEffect.h, r, g, b, (u32) gasWidth, (u32) gasHeight, GL_PSX_DRAW_MODE_GAS, isSemiTrans, isShaded, 0}};
    
    PushVertexData(GL_TRIANGLES, verts, 6, blendMode, mGasTextureId, (u32) gasWidth, (u32) gasHeight);
}

void OpenGLRenderer::Draw(Prim_Tile& tile)
{
    if (!gRenderEnable_TILE)
    {
        return;
    }

    const u32 r = tile.mBase.header.rgb_code.r;
    const u32 g = tile.mBase.header.rgb_code.g;
    const u32 b = tile.mBase.header.rgb_code.b;

    bool isSemiTrans = GetPolyIsSemiTrans(&tile);
    bool isShaded = true;
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    const VertexData verts[6] = {
        {tile.mBase.vert.x, tile.mBase.vert.y, r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {tile.mBase.vert.x + tile.field_14_w, tile.mBase.vert.y, r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {tile.mBase.vert.x, tile.mBase.vert.y + tile.field_16_h, r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},

        {tile.mBase.vert.x + tile.field_14_w, tile.mBase.vert.y, r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {tile.mBase.vert.x, tile.mBase.vert.y + tile.field_16_h, r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {tile.mBase.vert.x + tile.field_14_w, tile.mBase.vert.y + tile.field_16_h, r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0}};

    PushVertexData(GL_TRIANGLES, verts, 6, blendMode);
}

void OpenGLRenderer::Draw(Line_F2& line)
{
    if (!gRenderEnable_F2)
    {
        return;
    }

    const u32 r = R0(&line);
    const u32 g = G0(&line);
    const u32 b = B0(&line);

    bool isSemiTrans = GetPolyIsSemiTrans(&line);
    bool isShaded = true;
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    const VertexData verts[2] = {
        {X0(&line), Y0(&line), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X1(&line), Y1(&line), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0}};

    PushVertexData(GL_LINES, verts, 2, blendMode);
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

    const VertexData verts[2] = {
        {X0(&line), Y0(&line), R0(&line), G0(&line), B0(&line), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X1(&line), Y1(&line), R1(&line), G1(&line), B1(&line), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0}};

    PushVertexData(GL_LINES, verts, 2, blendMode);
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

    const VertexData verts[6] = {
        {X0(&line), Y0(&line), R0(&line), G0(&line), B0(&line), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X1(&line), Y1(&line), R1(&line), G1(&line), B1(&line), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},

        {X1(&line), Y1(&line), R1(&line), G1(&line), B1(&line), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X2(&line), Y2(&line), R2(&line), G2(&line), B2(&line), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},

        {X2(&line), Y2(&line), R2(&line), G2(&line), B2(&line), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X3(&line), Y3(&line), R3(&line), G3(&line), B3(&line), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0}};

    PushVertexData(GL_LINES, verts, 6, blendMode);
}

void OpenGLRenderer::Draw(Poly_F3& poly)
{
    if (!gRenderEnable_F3)
    {
        return;
    }

    const u32 r = R0(&poly);
    const u32 g = G0(&poly);
    const u32 b = B0(&poly);

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    bool isShaded = true;
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    const VertexData verts[3] = {
        {X0(&poly), Y0(&poly), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X1(&poly), Y1(&poly), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X2(&poly), Y2(&poly), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0}};

    PushVertexData(GL_TRIANGLES, verts, 3, blendMode);
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

    const VertexData verts[3] = {
        {X0(&poly), Y0(&poly), R0(&poly), G0(&poly), B0(&poly), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X1(&poly), Y1(&poly), R1(&poly), G1(&poly), B1(&poly), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X2(&poly), Y2(&poly), R2(&poly), G2(&poly), B2(&poly), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0}};

    PushVertexData(GL_TRIANGLES, verts, 3, blendMode);
}

void OpenGLRenderer::Draw(Poly_F4& poly)
{
    if (!gRenderEnable_F4)
    {
        return;
    }

    const u32 r = R0(&poly);
    const u32 g = G0(&poly);
    const u32 b = B0(&poly);

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    bool isShaded = true;
    u32 blendMode = GetTPageBlendMode(mGlobalTPage);

    const VertexData verts[6] = {
        {X0(&poly), Y0(&poly), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X1(&poly), Y1(&poly), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X2(&poly), Y2(&poly), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
    
        {X1(&poly), Y1(&poly), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X2(&poly), Y2(&poly), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X3(&poly), Y3(&poly), r, g, b, 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0}};

    PushVertexData(GL_TRIANGLES, verts, 6, blendMode);
}

void OpenGLRenderer::Draw(Poly_FT4& poly)
{
    if (!gRenderEnable_FT4)
    {
        return;
    }

    const u32 textureId = PrepareTextureFromPoly(poly);

    u32 r = poly.mBase.header.rgb_code.r;
    u32 g = poly.mBase.header.rgb_code.g;
    u32 b = poly.mBase.header.rgb_code.b;

    bool isSemiTrans = GetPolyIsSemiTrans(&poly);
    bool isShaded = GetPolyIsShaded(&poly);
    u32 blendMode = GetTPageBlendMode(GetTPage(&poly));

    if (poly.mFg1)
    {
        VertexData verts[6] = {
            {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, 0, 0, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, 0},
            {poly.mVerts[0].mVert.x, poly.mVerts[0].mVert.y, r, g, b, GL_FRAMEBUFFER_PSX_WIDTH, 0, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, 0},
            {poly.mVerts[2].mVert.x, poly.mVerts[2].mVert.y, r, g, b, GL_FRAMEBUFFER_PSX_WIDTH, GL_FRAMEBUFFER_PSX_HEIGHT, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, 0},

            {poly.mVerts[2].mVert.x, poly.mVerts[2].mVert.y, r, g, b, GL_FRAMEBUFFER_PSX_WIDTH, GL_FRAMEBUFFER_PSX_HEIGHT, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, 0},
            {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, 0, 0, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, 0},
            {poly.mVerts[1].mVert.x, poly.mVerts[1].mVert.y, r, g, b, 0, GL_FRAMEBUFFER_PSX_HEIGHT, GL_PSX_DRAW_MODE_FG1, isSemiTrans, isShaded, 0}};

        PushVertexData(GL_TRIANGLES, verts, 6, blendMode, mCamTexture.mTextureId, GL_FRAMEBUFFER_PSX_WIDTH, GL_FRAMEBUFFER_PSX_HEIGHT, mFg1Texture.mTextureId);
    }
    else if (poly.mCam)
    {
        VertexData verts[6] = {
            {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, 0, 0, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, 0},
            {poly.mVerts[0].mVert.x, poly.mVerts[0].mVert.y, r, g, b, GL_FRAMEBUFFER_PSX_WIDTH, 0, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, 0},
            {poly.mVerts[2].mVert.x, poly.mVerts[2].mVert.y, r, g, b, GL_FRAMEBUFFER_PSX_WIDTH, GL_FRAMEBUFFER_PSX_HEIGHT, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, 0},

            {poly.mVerts[2].mVert.x, poly.mVerts[2].mVert.y, r, g, b, GL_FRAMEBUFFER_PSX_WIDTH, GL_FRAMEBUFFER_PSX_HEIGHT, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, 0},
            {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, 0, 0, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, 0},
            {poly.mVerts[1].mVert.x, poly.mVerts[1].mVert.y, r, g, b, 0, GL_FRAMEBUFFER_PSX_HEIGHT, GL_PSX_DRAW_MODE_CAM, isSemiTrans, isShaded, 0}};

        PushVertexData(GL_TRIANGLES, verts, 6, blendMode, mCamTexture.mTextureId, GL_FRAMEBUFFER_PSX_WIDTH, GL_FRAMEBUFFER_PSX_HEIGHT);
    }
    else if (poly.mAnim)
    {
        const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);
        std::shared_ptr<TgaData> pTga = poly.mAnim->mAnimRes.mTgaPtr;

        const u32 palIndex = PreparePalette(poly.mAnim->mAnimRes.mTgaPtr->mPal);

        u32 u0 = pHeader->mSpriteSheetX;
        u32 v0 = pHeader->mSpriteSheetY;

        u32 u1 = u0 + pHeader->mWidth;
        u32 v1 = v0 + pHeader->mHeight;

        if (poly.mFlipX)
        {
            std::swap(u0, u1);
        }

        if (poly.mFlipY)
        {
            std::swap(v1, v0);
        }

        VertexData verts[6] = {
            {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, u0, v0, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
            {poly.mVerts[0].mVert.x, poly.mVerts[0].mVert.y, r, g, b, u1, v0, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
            {poly.mVerts[2].mVert.x, poly.mVerts[2].mVert.y, r, g, b, u1, v1, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},

            {poly.mVerts[2].mVert.x, poly.mVerts[2].mVert.y, r, g, b, u1, v1, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
            {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, u0, v0, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
            {poly.mVerts[1].mVert.x, poly.mVerts[1].mVert.y, r, g, b, u0, v1, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex}};
        
        PushVertexData(GL_TRIANGLES, verts, 6, blendMode, textureId, pTga->mWidth, pTga->mHeight, mPaletteTextureId);
    }
    else if (poly.mFont)
    {
        std::shared_ptr<TgaData> pTga = poly.mFont->field_C_resource_id.mTgaPtr;

        const u32 palIndex = PreparePalette(pTga->mPal);

        u32 u0 = U0(&poly);
        u32 v0 = V0(&poly);

        u32 u1 = U3(&poly);
        u32 v1 = V3(&poly);

        VertexData verts[6] = {
            {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, u0, v0, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
            {poly.mVerts[0].mVert.x, poly.mVerts[0].mVert.y, r, g, b, u1, v0, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
            {poly.mVerts[2].mVert.x, poly.mVerts[2].mVert.y, r, g, b, u1, v1, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},

            {poly.mVerts[2].mVert.x, poly.mVerts[2].mVert.y, r, g, b, u1, v1, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
            {poly.mBase.vert.x, poly.mBase.vert.y, r, g, b, u0, v0, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex},
            {poly.mVerts[1].mVert.x, poly.mVerts[1].mVert.y, r, g, b, u0, v1, GL_PSX_DRAW_MODE_DEFAULT_FT4, isSemiTrans, isShaded, palIndex}};

        PushVertexData(GL_TRIANGLES, verts, 6, blendMode, textureId, pTga->mWidth, pTga->mHeight, mPaletteTextureId);
    }
    else
    {
        return;
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

    const VertexData verts[6] = {
        {X0(&poly), Y0(&poly), R0(&poly), G0(&poly), B0(&poly), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X1(&poly), Y1(&poly), R1(&poly), G1(&poly), B1(&poly), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X2(&poly), Y2(&poly), R2(&poly), G2(&poly), B2(&poly), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},

        {X1(&poly), Y1(&poly), R1(&poly), G1(&poly), B1(&poly), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X2(&poly), Y2(&poly), R2(&poly), G2(&poly), B2(&poly), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0},
        {X3(&poly), Y3(&poly), R3(&poly), G3(&poly), B3(&poly), 0, 0, GL_PSX_DRAW_MODE_FLAT, isSemiTrans, isShaded, 0}};

    PushVertexData(GL_TRIANGLES, verts, 6, blendMode);
}

void OpenGLRenderer::EndFrame()
{
    if (!mFrameStarted)
    {
        return;
    }

    // Ensure any remaining data is drawn
    InvalidateBatch();

    // Adjust the viewport for the window rather than the PSX resolution
    s32 wW, wH;
    SDL_GL_GetDrawableSize(mWindow, &wW, &wH);
    GL_VERIFY(glViewport(0, 0, wW, wH));

    // Draw the final composed framebuffer to the screen
    GL_VERIFY(glDisable(GL_SCISSOR_TEST));
    DrawFramebufferToScreen(
        mBlitRect.x,
        mBlitRect.y,
        mBlitRect.w,
        mBlitRect.h);

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
    SDL_GL_GetDrawableSize(mWindow, w, h);
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

    InvalidateBatch();

    if (rect.x == 0 && rect.y == 0 && rect.w == 1 && rect.h == 1)
    {
        GL_VERIFY(glDisable(GL_SCISSOR_TEST));
        return;
    }

    GL_VERIFY(glEnable(GL_SCISSOR_TEST));
    GL_VERIFY(glScissor(rect.x, GL_FRAMEBUFFER_PSX_HEIGHT - rect.y - rect.h, rect.w, rect.h));
}

void OpenGLRenderer::SetScreenOffset(Prim_ScreenOffset& /*offset*/)
{
    //mBlitRect.x = (s32) offset.field_C_xoff;
    //mBlitRect.y = (s32) offset.field_E_yoff;
}

void OpenGLRenderer::SetTPage(u16 tPage)
{
    mGlobalTPage = tPage;
}

void OpenGLRenderer::StartFrame(s32 /*xOff*/, s32 /*yOff*/)
{
    mStats.Reset();

    mFrameNumber++;

    FreeUnloadedAnimTextures();

    mFrameStarted = true;

    // Always render to PSX framebuffer
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId));
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

void OpenGLRenderer::DrawFramebufferToScreen(s32 x, s32 y, s32 width, s32 height)
{
    // Ensure blend mdoe is back to normal alpha compositing
    GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_VERIFY(glBlendEquation(GL_FUNC_ADD));

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
        0.0f, (f32) GL_FRAMEBUFFER_PSX_HEIGHT,
        0.0f, 0.0f,
        (f32) GL_FRAMEBUFFER_PSX_WIDTH, (f32) GL_FRAMEBUFFER_PSX_HEIGHT,

        (f32) GL_FRAMEBUFFER_PSX_WIDTH, (f32) GL_FRAMEBUFFER_PSX_HEIGHT,
        0.0f, 0.0f,
        (f32) GL_FRAMEBUFFER_PSX_WIDTH, 0.0f};

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

    SDL_GL_GetDrawableSize(mWindow, &viewportW, &viewportH);

    mPassthruShader.Use();

    mPassthruShader.Uniform1i("TextureSampler", 0);
    mPassthruShader.UniformVec2("vsViewportSize", {(f32) viewportW, (f32) viewportH});
    mPassthruShader.UniformVec2("vsTexSize", {GL_FRAMEBUFFER_PSX_WIDTH, GL_FRAMEBUFFER_PSX_HEIGHT});

    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    GL_VERIFY(glActiveTexture(GL_TEXTURE0));
    GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId));

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

    // Set the framebuffer target back to the PSX framebuffer
    GL_VERIFY(glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId));
}

u16 OpenGLRenderer::GetTPageBlendMode(u16 tpage)
{
    return (tpage >> 4) & 3;
}

void OpenGLRenderer::InvalidateBatch()
{
    if (mBatchData.size() == 0)
    {
        return;
    }

    mPsxShader.Use();

    GLuint vboId;

    // Set our new vectors
    GL_VERIFY(glGenBuffers(1, &vboId));
    GL_VERIFY(glBindBuffer(GL_ARRAY_BUFFER, vboId));
    GL_VERIFY(glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * mBatchData.size(), &mBatchData.front(), GL_STREAM_DRAW));

    // Set up vertex attributes
    GL_VERIFY(glEnableVertexAttribArray(0));
    GL_VERIFY(glVertexAttribIPointer(0, 2, GL_INT, sizeof(VertexData), 0));
    GL_VERIFY(glEnableVertexAttribArray(1));
    GL_VERIFY(glVertexAttribIPointer(1, 3, GL_UNSIGNED_INT, sizeof(VertexData), (void*) offsetof(VertexData, r)));
    GL_VERIFY(glEnableVertexAttribArray(2));
    GL_VERIFY(glVertexAttribIPointer(2, 2, GL_UNSIGNED_INT, sizeof(VertexData), (void*) offsetof(VertexData, u)));
    GL_VERIFY(glEnableVertexAttribArray(3));
    GL_VERIFY(glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(VertexData), (void*) offsetof(VertexData, drawType)));

    // Set up uniforms and texture samplers
    mPsxShader.UniformVec2("vsViewportSize", {(f32) GL_FRAMEBUFFER_PSX_WIDTH, (f32) GL_FRAMEBUFFER_PSX_HEIGHT});
    mPsxShader.UniformVec2("vsTexSize", {(f32) mBatchPriTexWidth, (f32) mBatchPriTexHeight});

    if (mBatchPriTexId != BATCH_VALUE_UNSET)
    {
        GL_VERIFY(glActiveTexture(GL_TEXTURE0));
        GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mBatchPriTexId));

        mPsxShader.Uniform1i("texTextureData", 0);
    }

    if (mBatchSecTexId != BATCH_VALUE_UNSET)
    {
        GL_VERIFY(glActiveTexture(GL_TEXTURE1));
        GL_VERIFY(glBindTexture(GL_TEXTURE_2D, mBatchSecTexId));

        mPsxShader.Uniform1i("texAdditionalData", 1);
    }

    if (mBatchBlendMode <= (u32) TPageAbr::eBlend_3)
    {
        SetupBlendMode((u16) mBatchBlendMode);
    }

    // Set index data and render
    GL_VERIFY(glDrawArrays(mBatchDrawMode, 0, (u32) mBatchData.size()));

    GL_VERIFY(glDeleteBuffers(1, &vboId));

    GL_VERIFY(glDisableVertexAttribArray(0));
    GL_VERIFY(glDisableVertexAttribArray(1));
    GL_VERIFY(glDisableVertexAttribArray(2));
    GL_VERIFY(glDisableVertexAttribArray(3));

    mPsxShader.UnUse();

    mBatchBlendMode = BATCH_VALUE_UNSET;
    mBatchData.clear();
    mBatchDrawMode = BATCH_VALUE_UNSET;
    mBatchPriTexId = BATCH_VALUE_UNSET;
    mBatchPriTexWidth = 0;
    mBatchPriTexHeight = 0;
    mBatchSecTexId = BATCH_VALUE_UNSET;

    for (auto iter = mUsedPalettes.begin(); iter != mUsedPalettes.end(); iter++)
    {
        // Default all palettes to unused for next draw
        iter->second = false;
    }

    mStats.mInvalidationsCount++;
}

void OpenGLRenderer::PushVertexData(GLenum mode, const VertexData* pVertData, int count, u32 blendMode, GLuint priTexId, u32 priTexWidth, u32 priTexHeight, GLuint secTexId)
{
    // Check if we need to invalidate the existing batched data
    // FIXME: For now we always invalidate if there's a secondary texture
    //        because we have no idea when the palette is replaced
    if (
        (mBatchBlendMode != blendMode && mBatchBlendMode != BATCH_VALUE_UNSET) ||
        (mBatchDrawMode != mode && mBatchDrawMode != BATCH_VALUE_UNSET) ||
        (priTexId > 0 && mBatchPriTexId != priTexId && mBatchPriTexId != BATCH_VALUE_UNSET) ||
        (secTexId > 0 && mBatchSecTexId != secTexId && mBatchSecTexId != BATCH_VALUE_UNSET))
    {
        InvalidateBatch();
    }

    mBatchBlendMode = blendMode;
    mBatchDrawMode = mode;
    mBatchPriTexId = priTexId;
    mBatchPriTexWidth = priTexWidth;
    mBatchPriTexHeight = priTexHeight;
    mBatchSecTexId = secTexId;

    for (int i = 0; i < count; i++)
    {
        mBatchData.push_back(pVertData[i]);
    }
}

void OpenGLRenderer::SetupBlendMode(u16 blendMode)
{
    mPsxShader.Uniform1i("fsBlendMode", blendMode);

    switch ((TPageAbr)blendMode)
    {
        case TPageAbr::eBlend_0:
            GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            GL_VERIFY(glBlendEquation(GL_FUNC_ADD));
            break;

        case TPageAbr::eBlend_1:
        case TPageAbr::eBlend_3:
            GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
            GL_VERIFY(glBlendEquation(GL_FUNC_ADD));
            break;

        case TPageAbr::eBlend_2:
            GL_VERIFY(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
            GL_VERIFY(glBlendEquation(GL_FUNC_REVERSE_SUBTRACT));
            break;
    }
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
        ImGui::Text("Invalidations %d", mStats.mInvalidationsCount);
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