#include "OpenGLRenderer.hpp"
#include "../Compression.hpp"
#include "../relive_lib/VRam.hpp"
#include "../relive_lib/Primitives.hpp"
#include "StbImageImplementation.hpp"
#include "../relive_lib/Animation.hpp"

#define GL_TO_IMGUI_TEX(v) *reinterpret_cast<ImTextureID*>(&v)

static GLuint mBackgroundTexture = 0;
static u8 gDecodeBuffer[640 * 256 * 2] = {};
static GLuint gCamTextureId = 0;
static GLuint gOtherTextureId = 0;

static TextureCache gFakeTextureCache = {};

static std::vector<TextureCache> gRendererTextures;
static std::vector<PaletteCache> gRendererPals;

static bool gRenderEnable_SPRT = false;
static bool gRenderEnable_GAS = false;
static bool gRenderEnable_TILE = false;
static bool gRenderEnable_FT4 = true;
static bool gRenderEnable_G4 = false;
static bool gRenderEnable_G3 = false;
static bool gRenderEnable_G2 = false;
static bool gRenderEnable_F4 = false;
static bool gRenderEnable_F3 = false;
static bool gRenderEnable_F2 = false;

static GLuint Renderer_CreateTexture(GLenum interpolation = GL_NEAREST)
{
    glEnable(GL_TEXTURE_2D);

    GLuint textureId;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

    return textureId;
}


static void Renderer_DecodePalette(const u8* srcPalData, RGBAPixel* dst, s32 palDepth)
{
    const u16* palShortPtr = reinterpret_cast<const u16*>(srcPalData);
    for (s32 i = 0; i < palDepth; i++)
    {
        const u16 oldPixel = palShortPtr[i];

        dst[i].B = static_cast<u8>((((oldPixel >> 0) & 0x1F)) << 2);
        dst[i].G = static_cast<u8>((((oldPixel >> 5) & 0x1F)) << 2);
        dst[i].R = static_cast<u8>((((oldPixel >> 10) & 0x1F)) << 2);
        dst[i].A = static_cast<u8>((((((oldPixel) >> 15) & 0xffff)) ? 127 : 255));
    }
}

static void Renderer_FreePalette(PSX_Point point)
{
    s32 i = 0;
    for (auto& c : gRendererPals)
    {
        if (point.x >= c.mPalPoint.x && point.x < (c.mPalPoint.x + c.mPalDepth) && c.mPalPoint.y == point.y)
        {
            gRendererPals.erase(gRendererPals.begin() + i);
            return;
        }
        i++;
    }
}

/*
static void Renderer_LoadPalette(PSX_Point point, const u8* palData, s16 palDepth)
{
    for (auto& c : gRendererPals)
    {
        if (point.x >= c.mPalPoint.x && point.x < (c.mPalPoint.x + c.mPalDepth) && c.mPalPoint.y == point.y)
        {
            s32 offset = point.x - c.mPalPoint.x;
            Renderer_DecodePalette(palData, c.mPalData + offset, palDepth);

            if (c.mPalDepth > 0)
            {
                c.mPalData[0].A = 0;
            }

            return;
        }
    }

    PaletteCache c = {};
    // Create if it doesnt exist
    c.mPalPoint = point;
    c.mPalDepth = palDepth;
    Renderer_DecodePalette(palData, c.mPalData, palDepth);

    if (c.mPalDepth > 0)
    {
        c.mPalData[0].A = 0;
    }

    gRendererPals.push_back(c);
}
*/

static int gPalTextureID = 0;

static void Renderer_BindPalette(AnimationPal& pCache)
{
    glEnable(GL_TEXTURE_2D);

    if (gPalTextureID == 0)
    {
        gPalTextureID = Renderer_CreateTexture();
    }

    //glBindTexture(GL_TEXTURE_2D, gPalTextureID);

    RGBAPixel dst[256];
    Renderer_DecodePalette(reinterpret_cast<const u8*>(pCache.mPal), dst, 256);

    

    // Set palette to GL_TEXTURE1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gPalTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst);
}


static void Renderer_SetBlendMode(TPageAbr blendAbr)
{
    switch (blendAbr)
    {
        case TPageAbr::eBlend_0:
            glBlendColor(1.0, 1.0, 1.0, 1.0);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case TPageAbr::eBlend_1:
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case TPageAbr::eBlend_2:
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case TPageAbr::eBlend_3:
            glBlendEquation(GL_FUNC_ADD);
            glBlendColor(1.0f, 1.0f, 1.0f, 0.25f);
            glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE);
            break;
    }
}



static void Renderer_ParseTPageBlendMode(u16 tPage)
{
    // TPageMode textureMode = static_cast<TPageMode>(((u32)tPage >> 7) & 3);
    TPageAbr pageAbr = static_cast<TPageAbr>(((u32) tPage >> 5) & 3);

    glEnable(GL_BLEND);

    Renderer_SetBlendMode(pageAbr);
}


void set_pixel_8(u8* surface, int x, int y, int pitch, u8 pixel)
{
    Uint8* target_pixel = (Uint8*)surface + (y * pitch) + x * sizeof(u8);
    *(u8*) target_pixel = pixel;
}

u8 get_pixel_8(u8* surface, int x, int y, int pitch)
{
    Uint8* target_pixel = (Uint8*) surface + (y * pitch) + x * sizeof(u8);
    return *target_pixel;
}


static TextureCache* Renderer_TextureFromAnim(Poly_FT4& poly)
{
   // const void* pAnimFg1Data = GetPrimExtraPointerHack(&poly);

    if (gOtherTextureId == 0)
    {
        gOtherTextureId = Renderer_CreateTexture();
    }

    if (gCamTextureId == 0)
    {
        gCamTextureId = Renderer_CreateTexture();
    }

    TPageMode textureMode = static_cast<TPageMode>(((u32) poly.mVerts[0].mUv.tpage_clut_pad >> 7) & 3);

    GLuint useTextureId = poly.mCam && !poly.mFg1 ? gCamTextureId : gOtherTextureId;

    gFakeTextureCache = {};
    gFakeTextureCache.mTextureID = useTextureId;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, useTextureId);

    if (poly.mFg1)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, poly.mFg1->mWidth, poly.mFg1->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, poly.mFg1->mPixels->data());
    }
    else if (poly.mCam)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, poly.mCam->mData.mWidth, poly.mCam->mData.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, poly.mCam->mData.mPixels->data());
    }
    else if (poly.mAnim)
    {
        AnimResource& r = poly.mAnim->mAnimRes;
        const PerFrameInfo* pHeader = poly.mAnim->Get_FrameHeader(-1);
        std::vector<u8> tmp(pHeader->mWidth * pHeader->mHeight);
        for (u32 y = 0; y < pHeader->mHeight; y++)
        {
            for (u32 x = 0; x < pHeader->mWidth; x++)
            {
                set_pixel_8(tmp.data(), x, y, pHeader->mWidth, get_pixel_8(r.mTgaPtr->mPixels.data(), pHeader->mSpriteSheetX + x, pHeader->mSpriteSheetY + y, r.mTgaPtr->mWidth));
            }
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pHeader->mWidth, pHeader->mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, tmp.data());
        
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, r.mTgaPtr->mWidth, r.mTgaPtr->mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, r.mTgaPtr->mPixels.data());
    }

    switch (textureMode)
    {
        case TPageMode::e8Bit_1:
        {
            /*
            gFakeTextureCache.mBitDepth = IRenderer::BitDepth::e16Bit;

            poly.mAnim->mAnimRes.mTgaPtr->mPixels.data();

            CompressionType_3Ae_Decompress_40A6A0((u8*) pAnimFg1Data, (u8*) poly.mAnim->mAnimRes.mTgaPtr->mPixels.data());
            glBindTexture(GL_TEXTURE_2D, gDecodedTextureCache);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tWidth, tHeight, 0, GL_RED, GL_UNSIGNED_BYTE, gDecodeBuffer);
            */
            break;
        }
        /*
        case TPageMode::e16Bit_2:
            // TODO:  FG1's get rendered here for AE.
            s16 fg1Width = poly.mVerts[0].mVert.x - poly.mBase.vert.x;
            s16 fg1Height = poly.mVerts[1].mVert.y - poly.mBase.vert.y;
            gFakeTextureCache.mVramRect = {0, 0, fg1Width, fg1Height};
            gFakeTextureCache.mBitDepth = IRenderer::BitDepth::e16Bit;
            gFakeTextureCache.mIsFG1 = true;
            glBindTexture(GL_TEXTURE_2D, gDecodedTextureCache);
            Renderer_ConvertFG1BitMask(fg1Width, fg1Height, (u8*) pAnimFg1Data);
            break;*/
    }

    return &gFakeTextureCache;
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

    mTextureShader.Use();

    mTextureShader.Uniform1i("m_Sprite", 0); // Set m_Sprite to GL_TEXTURE0

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pTexture);

    const GLuint indexData[6] = {0, 1, 3, 3, 1, 2};
    DrawTriangles(verts, 4, indexData, 6);

    mTextureShader.UnUse();
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

void OpenGLRenderer::DrawTriangles(const VertexData* pVertData, s32 vertSize, const GLuint* pIndData, s32 indSize)
{
    // Set our new vectors
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertSize, pVertData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * sizeof(GLuint), pIndData, GL_STATIC_DRAW);

    InitAttributes();

    //Set index data and render
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glDrawElements(GL_TRIANGLES, indSize, GL_UNSIGNED_INT, NULL);

    if (mWireframe)
    {
        glLineWidth(1.0f);
        mTextureShader.Uniform1i("m_Debug", 1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, indSize, GL_UNSIGNED_INT, NULL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        mTextureShader.Uniform1i("m_Debug", 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void OpenGLRenderer::DrawLines(const VertexData* pVertData, s32 vertSize, const GLuint* pIndData, s32 indSize)
{
    // Set our new vectors
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertSize, pVertData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * sizeof(GLuint), pIndData, GL_STATIC_DRAW);

    InitAttributes();

    // TODO: Make lines scale with Window
    glLineWidth(2.0f);

    //Set index data and render
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glDrawElements(GL_LINE_STRIP, indSize, GL_UNSIGNED_INT, NULL);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void OpenGLRenderer::RenderBackground()
{
    //Renderer_SetBlendMode(TPageAbr::eBlend_0);
    //DrawTexture(GetBackgroundTexture(), 0, 0, 640, 240);
}

void OpenGLRenderer::DebugWindow()
{
    //ImGuiStyle& style = ImGui::GetStyle();
    //ImGuiIO& io = ImGui::GetIO();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Developer"))
        {
            if (ImGui::BeginMenu("Render Mode"))
            {
                if (ImGui::MenuItem("Normal"))
                {
                    mWireframe = false;
                }
                if (ImGui::MenuItem("Wireframe"))
                {
                    mWireframe = true;
                }
                ImGui::EndMenu();
            }

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


    if (ImGui::Begin("Palettes", nullptr, ImGuiWindowFlags_MenuBar))
    {
        f32 width = ImGui::GetWindowContentRegionWidth();
        for (auto& pal : gRendererPals)
        {
            ImGui::Image(GL_TO_IMGUI_TEX(pal.mPalTextureID), ImVec2(width, 16));
        }
    }
    ImGui::End();

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

void OpenGLRenderer::Destroy()
{
    ImGui_ImplSDL2_Shutdown();

    mTextureShader.Free();

    for (auto& t : gRendererTextures)
    {
        glDeleteTextures(1, &t.mTextureID);
    }

    for (auto& t : gRendererPals)
    {
        glDeleteTextures(1, &t.mPalTextureID);
    }

    glDeleteTextures(1, &gCamTextureId);
    glDeleteTextures(1, &gOtherTextureId);

    if (mContext)
    {
        SDL_GL_DeleteContext(mContext);
        mContext = nullptr;
    }
}

bool OpenGLRenderer::Create(TWindowHandleType window)
{
    mWindow = window;
    mWireframe = false;

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


    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create context
    mContext = SDL_GL_CreateContext(window);

    if (mContext == NULL)
    {
        LOG_ERROR("OpenGL 3.2 context could not be created! SDL Error: " << SDL_GetError());

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

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        LOG_ERROR("Error initializing GLEW! " << glewGetErrorString(glewError));
    }

    // Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0)
    {
        LOG_ERROR("Warning: Unable to set VSync! SDL Error: " << SDL_GetError());
    }


    ImGui::CreateContext();

    // Setup IMGUI for texture debugging
    ImGui_ImplSDL2_InitForOpenGL(mWindow, mContext);
    ImGui_ImplOpenGL3_Init("#version 150");

    // Create our render buffers
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    glGenBuffers(1, &mIBO);
    glGenBuffers(1, &mVBO);

    //mTextureShader.LoadFromFile("shaders/texture.vsh", "shaders/texture.fsh");
    mTextureShader.LoadSource(gShader_TextureVSH, gShader_TextureFSH);

    // ROZZA Init passthru shader
    mPassthruShader.LoadSource(gShader_PassthruVSH, gShader_PassthruFSH);

    // ROZZA Init framebuffer for render to texture
    InitPsxFramebuffer(0);
    InitPsxFramebuffer(1);

    return true;
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
        //ImGui::Render();
        //ImGui::EndFrame();
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    else
    {
        firstFrame = false;
    }

    //ImGui_ImplOpenGL3_NewFrame();
    //ImGui_ImplSDL2_NewFrame(mWindow);
    //ImGui::NewFrame();

    //DebugWindow();

    // ROZZA LOGIC
    // We clear the destination buffer but do NOT copy it back over the original
    // because we will continue to draw (don't call CompleteDraw)

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    // TODO: FIX THIS ONCE UV IS CORRECT

    Renderer_SetBlendMode(TPageAbr::eBlend_0);

    if (mBackgroundTexture != 0)
    {
        DrawTexture(mBackgroundTexture, 0, 0, 640, 240);
    }
}

void OpenGLRenderer::StartFrame(s32 /*xOff*/, s32 /*yOff*/)
{
    // Clear backing framebuffers
    glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId[0]);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId[1]);
    glClear(GL_COLOR_BUFFER_BIT);

    // Always render to destination buffer (1)
    glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId[1]);
    glViewport(0, 0, 640, 240);
}

// This function should free both vrams allocations AND palettes, cause theyre kinda the same thing.
void OpenGLRenderer::PalFree(const PalRecord& record)
{
    Pal_free(PSX_Point{record.x, record.y}, record.depth); // TODO: Stop depending on this

    Renderer_FreePalette({
        record.x,
        record.y,
    });
    /*
    Renderer_FreeTexture({
        record.x,
        record.y,
    });*/
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

void OpenGLRenderer::PalSetData(const PalRecord& record, const u8* pPixels)
{
    PSX_RECT rect = {};
    rect.x = record.x;
    rect.y = record.y;
    rect.w = record.depth;
    rect.h = 1;
    Upload(IRenderer::BitDepth::e16Bit, rect, pPixels);
}

void OpenGLRenderer::EndFrame()
{
    s32 wW, wH;
    SDL_GetWindowSize(mWindow, &wW, &wH);
    glViewport(0, 0, wW, wH);

    // Draw the final composed framebuffer to the screen
    CompleteDraw(0);

    SDL_GL_SwapWindow(mWindow);
}

void OpenGLRenderer::BltBackBuffer(const SDL_Rect* /*pCopyRect*/, const SDL_Rect* /*pDst*/)
{
    // TODO: Render source framebuffer to dest!
}

void OpenGLRenderer::OutputSize(s32* w, s32* h)
{
    *w = 640;
    *h = 480;
    //SDL_GetRendererOutputSize(mRenderer, w, h);
}

bool OpenGLRenderer::UpdateBackBuffer(const void* /*pPixels*/, s32 /*pitch*/)
{
    return true;
}

void OpenGLRenderer::CreateBackBuffer(bool /*filter*/, s32 /*format*/, s32 /*w*/, s32 /*h*/)
{
}

void OpenGLRenderer::SetTPage(s16 tPage)
{
    Renderer_ParseTPageBlendMode(tPage);
    mLastTPage = tPage;
}

void OpenGLRenderer::SetClipDirect(s32 x, s32 y, s32 width, s32 height)
{
    mLastClip = glm::ivec4(x, y, width, height);

    s32 w, h;
    SDL_GetWindowSize(mWindow, &w, &h);

    if (width <= 1 && height <= 1)
    {
        glDisable(GL_SCISSOR_TEST);
        return;
    }

    glEnable(GL_SCISSOR_TEST);
    glScissor(static_cast<GLint>((x / 640.0f) * w),
              static_cast<GLint>(((240 - y - height) / 240.0f) * h),
              static_cast<GLsizei>((width / 640.0f) * w),
              static_cast<GLsizei>((height / 240.0f) * h));
}

void OpenGLRenderer::SetClip(Prim_PrimClipper& clipper)
{
    SetClipDirect(clipper.field_C_x, clipper.field_E_y, clipper.mBase.header.mRect.w, clipper.mBase.header.mRect.h);
}

void OpenGLRenderer::SetScreenOffset(Prim_ScreenOffset& offset)
{
    m_View = glm::ortho<f32>(static_cast<f32>(offset.field_C_xoff),
                             static_cast<f32>(640 + offset.field_C_xoff),
                             static_cast<f32>(240 + offset.field_E_yoff),
                             static_cast<f32>(offset.field_E_yoff), 0.0f, 1.0f);
}

void OpenGLRenderer::Draw(Prim_Sprt& /*sprt*/)
{
    if (!gRenderEnable_SPRT)
        return;

    /*
    // Detect our magic code and render our cam.
    if (sprt.mBase.header.rgb_code.r == 255 && sprt.mBase.header.rgb_code.g == 254 && sprt.mBase.header.rgb_code.b == 253)
    {
        RenderBackground();
        return;
    }

    PSX_Point vramPoint = Renderer_VRamFromTPage(mLastTPage);
    s16 textureMode = (mLastTPage >> 7) & 3;

    // FG1 Blocks
    if (vramPoint.x < 640)
    {
        glm::ivec4 lastClip = mLastClip;
        SetClipDirect(sprt.mBase.vert.x, sprt.mBase.vert.y, sprt.field_14_w + 1, sprt.field_16_h + 1);
        RenderBackground();
        SetClipDirect(lastClip.x, lastClip.y, lastClip.z, lastClip.w);
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    TextureCache* pTexture = Renderer_TexFromVRam({static_cast<s16>(vramPoint.x + WidthBppDivide(textureMode, sprt.mUv.u)), static_cast<s16>(vramPoint.y + sprt.mUv.v)});
    PaletteCache* pPal = Renderer_ClutToPalette(sprt.mUv.tpage_clut_pad);

    const VertexData verts[4] = {
        {0, 0, 0, 1.0f, 1.0f, 1.0f, 0, 0},
        {1, 0, 0, 1.0f, 1.0f, 1.0f, 1, 0},
        {1, 1, 0, 1.0f, 1.0f, 1.0f, 1, 1},
        {0, 1, 0, 1.0f, 1.0f, 1.0f, 0, 1}};

    mTextureShader.Use();

    Renderer_BindPalette(pPal);
    Renderer_BindTexture(pTexture);

    // Set our Projection Matrix, so stuff doesn't get rendered in the quantum realm.
    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP(sprt.mBase.vert.x, sprt.mBase.vert.y, sprt.field_14_w, sprt.field_16_h));

    mTextureShader.Uniform1i("m_Sprite", 0);  // Set m_Sprite to GL_TEXTURE0
    mTextureShader.Uniform1i("m_Palette", 1); // Set m_Palette to GL_TEXTURE1
    mTextureShader.Uniform1i("m_Textured", true);
    mTextureShader.Uniform1i("m_PaletteEnabled", pPal != nullptr);

    if (pPal != nullptr)
    {
        mTextureShader.Uniform1i("m_PaletteDepth", pPal->mPalDepth);
    }

    const GLuint indexData[6] = {0, 1, 3, 3, 1, 2};
    DrawTriangles(verts, 4, indexData, 6);

    mTextureShader.UnUse();
    */
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

    mTextureShader.Use();
    mTextureShader.Uniform1i("m_Dithered", 1);
    mTextureShader.Uniform1i("m_DitherWidth", gasWidth);
    mTextureShader.Uniform1i("m_DitherHeight", gasHeight);
    Renderer_SetBlendMode(TPageAbr::eBlend_1);
    DrawTexture(TempGasEffectTexture, (f32) gasEffect.x, (f32) gasEffect.y, (f32) gasWidth, (f32) gasHeight);
    mTextureShader.Use();
    mTextureShader.Uniform1i("m_Dithered", 0);
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

    mTextureShader.Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    const GLuint indexData[6] = {0, 1, 3, 3, 1, 2};
    DrawTriangles(verts, 4, indexData, 6);

    mTextureShader.UnUse();
}

void OpenGLRenderer::Draw(Line_F2& line)
{
    if (!gRenderEnable_F2)
        return;

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    const VertexData verts[2] = {
        {(f32) line.mVerts[0].mVert.x, (f32) line.mVerts[0].mVert.y, 0,
         line.mBase.header.rgb_code.r / 255.0f, line.mBase.header.rgb_code.g / 255.0f, line.mBase.header.rgb_code.b / 255.0f,
         0, 0},
        {(f32) line.mBase.vert.x, (f32) line.mBase.vert.y, 0,
         line.mBase.header.rgb_code.r / 255.0f, line.mBase.header.rgb_code.g / 255.0f, line.mBase.header.rgb_code.b / 255.0f,
         0, 0}};

    mTextureShader.Use();

    const GLuint indexData[2] = {0, 1};
    DrawLines(verts, 2, indexData, 2);

    mTextureShader.UnUse();
}

void OpenGLRenderer::Draw(Line_G2& line)
{
    if (!gRenderEnable_G2)
    {
        return;
    }

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    const VertexData verts[2] = {
        {(f32) line.mVerts[0].mVert.x, (f32) line.mVerts[0].mVert.y, 0,
         line.mVerts[0].mRgb.r / 255.0f, line.mVerts[0].mRgb.g / 255.0f, line.mVerts[0].mRgb.b / 255.0f,
         0, 0},
        {(f32) line.mBase.vert.x, (f32) line.mBase.vert.y, 0,
         line.mBase.header.rgb_code.r / 255.0f, line.mBase.header.rgb_code.g / 255.0f, line.mBase.header.rgb_code.b / 255.0f,
         0, 0}};

    mTextureShader.Use();

    const GLuint indexData[2] = {0, 1};
    DrawLines(verts, 2, indexData, 2);

    mTextureShader.UnUse();
}

void OpenGLRenderer::Draw(Line_G4& line)
{
    if (!gRenderEnable_G4)
    {
        return;
    }

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    const VertexData verts[4] = {
        {(f32) line.mBase.vert.x, (f32) line.mBase.vert.y, 0,
         line.mBase.header.rgb_code.r / 255.0f, line.mBase.header.rgb_code.g / 255.0f, line.mBase.header.rgb_code.b / 255.0f,
         0, 0},
        {(f32) line.mVerts[0].mVert.x, (f32) line.mVerts[0].mVert.y, 0,
         line.mVerts[0].mRgb.r / 255.0f, line.mVerts[0].mRgb.g / 255.0f, line.mVerts[0].mRgb.b / 255.0f,
         0, 0},
        {(f32) line.mVerts[1].mVert.x, (f32) line.mVerts[1].mVert.y, 0,
         line.mVerts[1].mRgb.r / 255.0f, line.mVerts[1].mRgb.g / 255.0f, line.mVerts[1].mRgb.b / 255.0f,
         0, 0},
        {(f32) line.mVerts[2].mVert.x, (f32) line.mVerts[2].mVert.y, 0,
         line.mVerts[2].mRgb.r / 255.0f, line.mVerts[2].mRgb.g / 255.0f, line.mVerts[2].mRgb.b / 255.0f,
         0, 0}};

    mTextureShader.Use();

    const GLuint indexData[4] = {0, 1, 2, 3};
    DrawLines(verts, 4, indexData, 4);

    mTextureShader.UnUse();
}

void OpenGLRenderer::Draw(Poly_F3& poly)
{
    if (!gRenderEnable_F3)
    {
        return;
    }

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    const VertexData verts[3] = {
        {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0,
         poly.mBase.header.rgb_code.r / 255.0f, poly.mBase.header.rgb_code.g / 255.0f, poly.mBase.header.rgb_code.b / 255.0f,
         0, 0},
        {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0,
         poly.mBase.header.rgb_code.r / 255.0f, poly.mBase.header.rgb_code.g / 255.0f, poly.mBase.header.rgb_code.b / 255.0f,
         1, 0},
        {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0,
         poly.mBase.header.rgb_code.r / 255.0f, poly.mBase.header.rgb_code.g / 255.0f, poly.mBase.header.rgb_code.b / 255.0f,
         0, 1}};

    mTextureShader.Use();

    mTextureShader.Uniform1i("m_Textured", false);

    const GLuint indexData[3] = {0, 1, 2};
    DrawTriangles(verts, 3, indexData, 3);

    mTextureShader.UnUse();
}

void OpenGLRenderer::Draw(Poly_G3& poly)
{
    if (!gRenderEnable_G3)
    {
        return;
    }

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    const VertexData verts[3] = {
        {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0,
         poly.mVerts[0].mRgb.r / 255.0f, poly.mVerts[0].mRgb.g / 255.0f, poly.mVerts[0].mRgb.b / 255.0f,
         1, 0},
        {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0,
         poly.mBase.header.rgb_code.r / 255.0f, poly.mBase.header.rgb_code.g / 255.0f, poly.mBase.header.rgb_code.b / 255.0f,
         0, 0},
        {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0,
         poly.mVerts[1].mRgb.r / 255.0f, poly.mVerts[1].mRgb.g / 255.0f, poly.mVerts[1].mRgb.b / 255.0f,
         0, 1}};

    mTextureShader.Use();

    mTextureShader.Uniform1i("m_Textured", false);

    const GLuint indexData[3] = {0, 1, 2};
    DrawTriangles(verts, 3, indexData, 3);

    mTextureShader.UnUse();
}

void OpenGLRenderer::Draw(Poly_F4& poly)
{
    if (!gRenderEnable_F4)
        return;

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    const f32 r = poly.mBase.header.rgb_code.r / 255.0f;
    const f32 g = poly.mBase.header.rgb_code.g / 255.0f;
    const f32 b = poly.mBase.header.rgb_code.b / 255.0f;

    const VertexData verts[4] = {
        {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0, r, g, b, 1, 0},
        {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, r, g, b, 0, 0},
        {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0, r, g, b, 0, 1},
        {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0, r, g, b, 1, 1}};

    mTextureShader.Use();

    const GLuint indexData[6] = {0, 1, 2, 0, 2, 3};
    DrawTriangles(verts, 4, indexData, 6);

    mTextureShader.UnUse();
}

void OpenGLRenderer::Draw(Poly_FT4& poly)
{
    if (!gRenderEnable_FT4)
        return;

    if (!poly.mAnim)
    {
        return;
    }

    if (poly.mAnim && poly.mAnim->mAnimRes.mTgaPtr->mWidth != 3537)
    {
        return;
    }

    TextureCache* pTexture = nullptr;

    // Some polys have their texture data directly attached to polys.
    //if (GetPrimExtraPointerHack(&poly))
    {
        pTexture = Renderer_TextureFromAnim(poly);
    }
    /*
    else
    {
        pTexture = Renderer_TexFromTPage(poly.mVerts[0].mUv.tpage_clut_pad, poly.mUv.u, poly.mUv.v);
    }*/

    if (pTexture == nullptr)
    {
        //LOG_WARNING("Trying to render FT4 with no texture!");
        return;
    }

    mTextureShader.Use();

    f32 r = poly.mBase.header.rgb_code.r / 64.0f;
    f32 g = poly.mBase.header.rgb_code.g / 64.0f;
    f32 b = poly.mBase.header.rgb_code.b / 64.0f;

    if (pTexture->mIgnoreColor)
    {
        r = 1.0f;
        g = 1.0f;
        b = 1.0f;
    }

    mTextureShader.Uniform1i("texTextureData", 0);  // Set m_Sprite to GL_TEXTURE0
    mTextureShader.Uniform1i("texAdditionalData", 1); // Set m_Palette to GL_TEXTURE1

    Renderer_ParseTPageBlendMode(poly.mVerts[0].mUv.tpage_clut_pad);

    const GLuint indexData[6] = {1, 0, 3, 3, 0, 2};

    if (poly.mFg1)
    {
        mTextureShader.Uniform1i("fsDrawType", 2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gCamTextureId);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gOtherTextureId);

        VertexData verts[4] = {
            {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, r, g, b, 0, 0},
            {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0, r, g, b, 1, 0},
            {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0, r, g, b, 0, 1},
            {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0, r, g, b, 1, 1}};
        DrawTriangles(verts, 4, indexData, 6);
        return;
    }
    else if (poly.mCam)
    {
        mTextureShader.Uniform1i("fsDrawType", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gCamTextureId);

        VertexData verts[4] = {
            {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, r, g, b, 0, 0},
            {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0, r, g, b, 1, 0},
            {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0, r, g, b, 0, 1},
            {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0, r, g, b, 1, 1}};
        DrawTriangles(verts, 4, indexData, 6);
    }
    else if (poly.mAnim)
    {
        mTextureShader.Uniform1i("fsDrawType", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gOtherTextureId);

        Renderer_BindPalette(poly.mAnim->mAnimRes.mTgaPtr->mPal);
        /*
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
        DrawTriangles(verts, 4, indexData, 6);*/
        VertexData verts[4] = {
            {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, r, g, b, 0, 0},
            {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0, r, g, b, 1, 0},
            {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0, r, g, b, 0, 1},
            {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0, r, g, b, 1, 1}};
        DrawTriangles(verts, 4, indexData, 6);
    }
    else
    {
        return;
    }

    mTextureShader.Uniform1i("m_FG1", false);

    mTextureShader.UnUse();

    CompleteDraw(mPsxFramebufferId[0]);
}

void OpenGLRenderer::Draw(Poly_G4& /*poly*/)
{
    /*
    if (!gRenderEnable_G4)
        return;

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    const VertexData verts[4] = {
        {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0,
         poly.mBase.header.rgb_code.r / 255.0f, poly.mBase.header.rgb_code.g / 255.0f, poly.mBase.header.rgb_code.b / 255.0f,
         0, 0},
        {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0,
         poly.mVerts[0].mRgb.r / 255.0f, poly.mVerts[0].mRgb.g / 255.0f, poly.mVerts[0].mRgb.b / 255.0f,
         1, 0},
        {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0,
         poly.mVerts[1].mRgb.r / 255.0f, poly.mVerts[1].mRgb.g / 255.0f, poly.mVerts[1].mRgb.b / 255.0f,
         0, 1},
        {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0,
         poly.mVerts[2].mRgb.r / 255.0f, poly.mVerts[2].mRgb.g / 255.0f, poly.mVerts[2].mRgb.b / 255.0f,
         1, 1}};

    mTextureShader.Use();

    // Set our Projection Matrix, so stuff doesn't get rendered in the quantum realm.
    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP());
    mTextureShader.Uniform1i("m_Textured", false);

    const GLuint indexData[6] = {1, 0, 2, 1, 2, 3};
    DrawTriangles(verts, 4, indexData, 6);

    mTextureShader.UnUse();
    */
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
        cache.mBitDepth = bitDepth;

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
        tc->mBitDepth = BitDepth::e16Bit;
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

void OpenGLRenderer::InitPsxFramebuffer(int index)
{
    GLuint* pFbId = &mPsxFramebufferId[index];
    GLuint* pFbTexId = &mPsxFramebufferTexId[index];

    glGenFramebuffers(1, pFbId);
    glGenTextures(1, pFbTexId);

    // Texture init
    glBindTexture(GL_TEXTURE_2D, *pFbTexId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Framebuffer init
    glBindFramebuffer(GL_FRAMEBUFFER, *pFbId);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, *pFbTexId, 0);

    GLenum fbTargets[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, fbTargets);
}

void OpenGLRenderer::CompleteDraw(GLuint target)
{
    static GLuint vboDrawId = 0;
    static GLuint vboUvId = 0;

    if (vboDrawId == 0)
    {
        static const GLfloat drawQuad[] = {
            -1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, -1.0f,

            1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, 1.0f};
        static const GLfloat uvQuad[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,

            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f
        };
        
        glGenBuffers(1, &vboDrawId);
        glBindBuffer(GL_ARRAY_BUFFER, vboDrawId);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(drawQuad),
            drawQuad,
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &vboUvId);
        glBindBuffer(GL_ARRAY_BUFFER, vboUvId);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(uvQuad),
            uvQuad,
            GL_STATIC_DRAW
        );
    }

    mPassthruShader.Use();

    mPassthruShader.Uniform1i("TextureSampler", 0);

    glBindFramebuffer(GL_FRAMEBUFFER, target);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mPsxFramebufferTexId[1]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vboDrawId);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vboUvId);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    mPassthruShader.UnUse();

    glBindFramebuffer(GL_FRAMEBUFFER, mPsxFramebufferId[1]);
}