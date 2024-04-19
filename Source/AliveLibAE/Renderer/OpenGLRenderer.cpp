#include "stdafx.h"

#include "OpenGLRenderer.hpp"
#include "Compression.hpp"
#include "VRam.hpp"
#include "AnimResources.hpp"
#include "ExternalAssets.hpp"
#include "../Tools/asset_tool/asset_common.hpp"

#include "StbImageImplementation.hpp"

#include <filesystem>
#include <jsonxx/jsonxx.h>

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 1024
#include "magic_enum/include/magic_enum.hpp"

namespace fs = std::filesystem;
using namespace std::string_literals;

#define GL_TO_IMGUI_TEX(v) *reinterpret_cast<ImTextureID*>(&v)

const char* gExternalAssetPath = "hd/";
static OpenGLRenderer* gGLInstance = nullptr;

struct FrameBuffer
{
    GLuint handle;
    GLuint texture_handle;
    int width;
    int height;
};

struct DebugDrawText
{
    std::string text;
    int screen_space_x;
    int screen_space_y;
    Uint32 color;
};

static std::vector<DebugDrawText> gDebugTexts;

static GLuint mHDBackgroundTexture = 0;
static GLuint mStitchedBackground = 0;
static FrameBuffer mWindowFrameBuffer = {};
static FrameBuffer mTempSpriteBuffer = {};
static u8 gDecodeBuffer[640 * 256 * 2] = {};
static GLuint gDecodedTextureCache = 0;

static TextureCache gFakeTextureCache = {};

static std::vector<TextureCache> gRendererTextures;
static std::vector<PaletteCache> gRendererPals;

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

static bool gExternalTexturesEnabled = true;
static bool gGLDebugInfo = false;
static bool gShowAnimIDs = false;

struct ExternalTexture final
{
    GLuint handle;
    int width;
    int height;
};

struct ExternalTextureMeta final
{
    std::vector<ExternalTexture> textures;
    std::map<int, ExternalTexture> textures_emissive;
    std::map<int, ExternalTexture> textures_flipped;
    std::map<int, ExternalTexture> textures_flipped_emissive;
    AssetMeta meta;
};

std::map<AnimId, ExternalTextureMeta> gLoadedExternalTextures;

std::string ReadFileToString(std::string fileName)
{
    std::ifstream file;
    file.open(fileName);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

ExternalTexture LoadTextureCacheFile(const std::string& path)
{
    int x = 0, y = 0, comp = 0;

    std::vector<BYTE> fileData;

    // Try to keep all paths and filenames lowercase for our linux friends.
    std::ifstream file(path, std::ios::binary);

    if (file.is_open())
    {
        fileData = std::vector<BYTE>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    }

    const unsigned char* data = stbi_load_from_memory(fileData.data(), static_cast<int>(fileData.size()), &x, &y, &comp, 4);

    if (comp == 0)
    {
        ExternalTexture nullt;
        nullt.handle = 0;
        nullt.width = 10;
        nullt.height = 10;
        return nullt;
    }

    GLuint newTexture = 0;
    glGenTextures(1, &newTexture);

    glBindTexture(GL_TEXTURE_2D, newTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free((void*)data);

    ExternalTexture textureCache;
    textureCache.handle = newTexture;
    textureCache.width = x;
    textureCache.height = y;

    return textureCache;
}

void LoadAllExternalTextures(const std::string& dir = "hd/sprites")
{
    const std::string asset_suffix_emissive = "_emissive.png";
    const std::string asset_suffix_flipped = "_flipped.png";
    const std::string asset_suffix_flipped_emissive = "_flipped_emissive.png";

    for (const auto& rootDir : fs::directory_iterator(dir))
    {
        if (rootDir.is_directory())
        {
            const std::string folderName = rootDir.path().lexically_relative(dir).string();

            printf("External Dir: %s\n", folderName.c_str());

            std::string assetDirectory = dir + "/" + folderName + "/";

            // check if with magic_enum if we have a valid enum
            static const auto invalidEnum = magic_enum::enum_cast<AnimId>(-1);
            if (magic_enum::enum_cast<AnimId>(folderName) != invalidEnum)
            {
                AnimId id = magic_enum::enum_cast<AnimId>(folderName).value();

                if (!gLoadedExternalTextures[id].meta.LoadJSONFromFile(assetDirectory + "meta.json"))
                {
                    LOG_ERROR("Failed to load animation meta json for " << folderName);
                    ALIVE_FATAL("Failed to load animation meta json");
                }

                gLoadedExternalTextures[id].textures = std::vector<ExternalTexture>();
                gLoadedExternalTextures[id].textures.reserve(gLoadedExternalTextures[id].meta.frame_count);

                for (int i = 0; i < gLoadedExternalTextures[id].meta.frame_count; i++)
                {
                    std::string frameBaseName = assetDirectory + std::to_string(i);

                    gLoadedExternalTextures[id].textures.push_back(LoadTextureCacheFile(frameBaseName + ".png"));

                    std::string emissive = frameBaseName + asset_suffix_emissive;
                    std::string flipped = frameBaseName + asset_suffix_flipped;
                    std::string flippedEmissive = frameBaseName + asset_suffix_flipped_emissive;

                    // check for our alternative textures
                    if (fs::exists(emissive))
                        gLoadedExternalTextures[id].textures_emissive[i] = LoadTextureCacheFile(emissive);
                    if (fs::exists(flipped))
                        gLoadedExternalTextures[id].textures_flipped[i] = LoadTextureCacheFile(flipped);
                    if (fs::exists(flippedEmissive))
                        gLoadedExternalTextures[id].textures_flipped_emissive[i] = LoadTextureCacheFile(flippedEmissive);
                }
            }
            else
            {
                printf("Invalid External Dir Name: %s\n", folderName.c_str());
            }
        }
    }
}

/*

#define GL_CHECK(stmt) do { \
    stmt; \
    CheckOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)


#define glEnable(...) GL_CHECK(glEnable(__VA_ARGS__));
#define glGenTextures(...) GL_CHECK(glGenTextures(__VA_ARGS__));
#define glBindTexture(...) GL_CHECK(glBindTexture(__VA_ARGS__));
#define glTexParameteri(...) GL_CHECK(glTexParameteri(__VA_ARGS__));
#define glTexImage2D(...) GL_CHECK(glTexImage2D(__VA_ARGS__));
#define glBlendFunc(...) GL_CHECK(glBlendFunc(__VA_ARGS__));
#define glDrawElements(...) GL_CHECK(glDrawElements(__VA_ARGS__));
*/

void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::string message;
        message.resize(5000);
        sprintf(message.data(), "OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
        ALIVE_FATAL(message.c_str());
    }
}

static GLuint TextureFromFile(const char_type* path)
{
    GLuint texHandle = 0;
    FILE* fh = fopen(path, "rb");
    ;

    if (fh == NULL)
    {
        return 0;
    }

    s32 x = 0, y = 0;
    s32 comp = 0;
    const u8* data = stbi_load_from_file(fh, &x, &y, &comp, 4);

    glGenTextures(1, &texHandle);

    glBindTexture(GL_TEXTURE_2D, texHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free((void*) data);

    fclose(fh);

    return texHandle;
}

static GLuint GetBackgroundTexture()
{
    if (mHDBackgroundTexture != 0 && gExternalTexturesEnabled)
    {
        return mHDBackgroundTexture;
    }

    return mStitchedBackground;
}

static TextureCache* GetBackgroundTextureCache()
{
    gFakeTextureCache.mPalXY = {};
    gFakeTextureCache.mVramRect = {0, 0, 640, 240};
    gFakeTextureCache.mTextureID = GetBackgroundTexture();
    gFakeTextureCache.mPalNormMulti = 0;
    gFakeTextureCache.mIsFG1 = false;
    gFakeTextureCache.mUvOffset = {};
    gFakeTextureCache.mBitDepth = IRenderer::BitDepth::e16Bit;

    return &gFakeTextureCache;
}

static GLuint Renderer_CreateTexture(GLenum interpolation = GL_NEAREST)
{
    glEnable(GL_TEXTURE_2D);

    GLuint textureId;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

    return textureId;
}

static bool Renderer_TexExists(const PSX_RECT& rect)
{
    for (TextureCache& c : gRendererTextures)
    {
        if (c.mVramRect.x == rect.x && c.mVramRect.y == rect.y)
        {
            return true;
        }
    }

    return false;
}

static TextureCache* Renderer_TexFromTPage(u16 tPage, u8 u, u8 v)
{
    s32 textureMode = static_cast<s32>(((u32) tPage >> 7) & 3);
    s16 tpagex = ((tPage & 0xF) << 6);
    s16 tpagey = (16 * (tPage & 0x10) + (((u32) tPage >> 2) & 0x200)) + v;

    // Lets prims use background texture as a src even tho we dont have vram anymore.
    if (tpagex < 640 && tpagey < 240)
    {
        TextureCache* tc = GetBackgroundTextureCache();
        tc->mUvOffset.field_0_x = tpagex;
        tc->mIgnoreColor = true;
        return tc;
    }

    switch (textureMode)
    {
        case 0: // e4Bit_0
            tpagex += u / 4;
            break;
        case 1: // e8Bit_1
            tpagex += u / 2;
            break;
        case 2: // e16Bit_2
            tpagex += u;
            break;
    }

    for (size_t i = 0; i < gRendererTextures.size(); i++)
    {
        TextureCache* c = &gRendererTextures[i];

        if (tpagex >= c->mVramRect.x && tpagex < c->mVramRect.x + c->mVramRect.w && tpagey >= c->mVramRect.y && tpagey < c->mVramRect.y + c->mVramRect.h)
        {
            return c;
        }
    }

    return 0;
}

static PSX_Point Renderer_ClutToCoords(s32 tClut)
{
    s32 x = (tClut & 63) << 4;
    s32 y = ((tClut >> 6) & 0xff);

    return {(s16) x, (s16) y};
}

static PaletteCache* Renderer_ClutToPalette(s32 tClut)
{
    s16 x = (tClut & 63) << 4;
    s16 y = ((tClut >> 6) & 0xff);

    for (size_t i = 0; i < gRendererPals.size(); i++)
    {
        PaletteCache* c = &gRendererPals[i];

        if (x >= c->mPalPoint.field_0_x && x < (c->mPalPoint.field_0_x + c->mPalDepth) && c->mPalPoint.field_2_y == y)
        {
            return c;
        }
    }

    return nullptr;
}

static TextureCache* Renderer_TexFromVRam(const PSX_RECT& rect)
{
    for (size_t i = 0; i < gRendererTextures.size(); i++)
    {
        TextureCache* c = &gRendererTextures[i];

        if (c->mVramRect.x == rect.x && c->mVramRect.y == rect.y)
        {
            return c;
        }
    }

    return 0;
}

static void Renderer_FreeTexture(PSX_Point point)
{
    for (size_t i = 0; i < gRendererTextures.size(); i++)
    {
        TextureCache* c = &gRendererTextures[i];

        if (c->mVramRect.x == point.field_0_x && c->mVramRect.y == point.field_2_y)
        {
            glDeleteTextures(1, &c->mTextureID);
            gRendererTextures.erase(gRendererTextures.begin() + i);
            return;
        }
    }
}

static void Renderer_DecodePalette(const u8* srcPalData, RGBAPixel* dst, s32 palDepth)
{
    const u16* palShortPtr = reinterpret_cast<const u16*>(srcPalData);
    for (s32 i = 0; i < palDepth; i++)
    {
        const u16 oldPixel = palShortPtr[i];

        dst[i].R = static_cast<u8>((((oldPixel >> 0) & 0x1F)) << 2);
        dst[i].G = static_cast<u8>((((oldPixel >> 5) & 0x1F)) << 2);
        dst[i].B = static_cast<u8>((((oldPixel >> 10) & 0x1F)) << 2);
        dst[i].A = static_cast<u8>((((((oldPixel) >> 15) & 0xffff)) ? 127 : 255));
    }
}

static void Renderer_FreePalette(PSX_Point point)
{
    s32 i = 0;
    for (auto& c : gRendererPals)
    {
        if (point.field_0_x >= c.mPalPoint.field_0_x && point.field_0_x < (c.mPalPoint.field_0_x + c.mPalDepth) && c.mPalPoint.field_2_y == point.field_2_y)
        {
            gRendererPals.erase(gRendererPals.begin() + i);
            return;
        }
        i++;
    }
}

static void Renderer_LoadPalette(PSX_Point point, const u8* palData, s16 palDepth)
{
    for (auto& c : gRendererPals)
    {
        if (point.field_0_x >= c.mPalPoint.field_0_x && point.field_0_x < (c.mPalPoint.field_0_x + c.mPalDepth) && c.mPalPoint.field_2_y == point.field_2_y)
        {
            s32 offset = point.field_0_x - c.mPalPoint.field_0_x;
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

static void Renderer_BindPalette(PaletteCache* pCache)
{
    glEnable(GL_TEXTURE_2D);

    if (pCache != nullptr)
    {
        if (pCache->mPalTextureID == 0)
        {
            pCache->mPalTextureID = Renderer_CreateTexture();
        }

        glBindTexture(GL_TEXTURE_2D, pCache->mPalTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pCache->mPalDepth, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pCache->mPalData);

        // Set palette to GL_TEXTURE1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pCache->mPalTextureID);
    }
}

static void Renderer_BindTexture(TextureCache* pTexture)
{
    glEnable(GL_TEXTURE_2D);

    if (pTexture != nullptr)
    {
        // Set main sprite to GL_TEXTURE0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pTexture->mTextureID);
    }
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

static PSX_Point Renderer_VRamFromTPage(u16 tPage)
{
    s16 tpagex = (tPage & 0xF) << 6;
    s16 tpagey = 16 * (tPage & 0x10) + (((u32) tPage >> 2) & 0x200);

    return {tpagex, tpagey};
}

static void Renderer_ParseTPageBlendMode(u16 tPage)
{
    // TPageMode textureMode = static_cast<TPageMode>(((u32)tPage >> 7) & 3);
    TPageAbr pageAbr = static_cast<TPageAbr>(((u32) tPage >> 5) & 3);

    glEnable(GL_BLEND);

    Renderer_SetBlendMode(pageAbr);
}

//static s32 WidthBpp(s32 textureMode, s32 width)
//{
//    switch (textureMode)
//    {
//    case 1:
//        return width * 2;
//    case 0:
//        return width * 4;
//    default:
//        return width;
//    }
//}

static s32 WidthBppDivide(s32 textureMode, s32 width)
{
    switch (textureMode)
    {
        case 1:
            return width / 2;
        case 0:
            return width / 4;
        default:
            return width;
    }
}

static void Convert4bppTextureFont(const PSX_RECT& rect, const u8* pPixels)
{
    std::vector<u8> buffer(rect.w * rect.h * 4);

    s32 pIndex = 0;
    for (s32 i = 0; i < rect.w * 4 * rect.h; i += 2)
    {
        buffer[i] = pPixels[pIndex] & 0xf;
        buffer[i + 1] = (pPixels[pIndex] & 0xf0) >> 4;
        pIndex++;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, rect.w * 4, rect.h, 0, GL_RED, GL_UNSIGNED_BYTE, buffer.data());
}

static void Renderer_ConvertFG1BitMask(s32 width, s32 height, const u8* pPixels)
{
    RGBAPixel* mDst = reinterpret_cast<RGBAPixel*>(gDecodeBuffer);
    const u32* mSrc = reinterpret_cast<const u32*>(pPixels);

    s32 pSrcIndex = 0;
    s32 dstIndex = 0;

    for (s32 y = 0; y < height; y++)
    {
        u32 bitMask = mSrc[pSrcIndex];
        for (s32 x = 0; x < width; x++)
        {
            u8 v = (bitMask & 1) * 255;

            bitMask >>= 1;

            mDst[dstIndex].R = v;
            mDst[dstIndex].G = v;
            mDst[dstIndex].B = v;
            mDst[dstIndex].A = v;
            dstIndex++;
        }
        pSrcIndex++;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, gDecodeBuffer);
}

static TextureCache* Renderer_TextureFromAnim(Poly_FT4& poly)
{
    const void* pAnimFg1Data = GetPrimExtraPointerHack(&poly);

    if (gDecodedTextureCache == 0)
        gDecodedTextureCache = Renderer_CreateTexture();

    u16 tWidth = static_cast<u16>(ceil(reinterpret_cast<const u16*>(GetPrimExtraPointerHack(&poly))[0] / 4.0f) * 4);;
    u16 tHeight = reinterpret_cast<const u16*>(GetPrimExtraPointerHack(&poly))[1];

    /*if (tWidth > 4096 || tHeight > 4096)
    {
        LOG_INFO("Tried decompressing an anim but got an unusual width/height\nReturning to prevent crash.");
        return nullptr;
    }*/

    TPageMode textureMode = static_cast<TPageMode>(((u32) poly.mVerts[0].mUv.tpage_clut_pad >> 7) & 3);

    gFakeTextureCache = {};
    gFakeTextureCache.mPalXY = Renderer_ClutToCoords(poly.mUv.tpage_clut_pad);
    gFakeTextureCache.mVramRect = {0, 0, (s16) tWidth, (s16) tHeight};
    gFakeTextureCache.mTextureID = gDecodedTextureCache;


    switch (textureMode)
    {
        case TPageMode::e4Bit_0:
            gFakeTextureCache.mBitDepth = IRenderer::BitDepth::e16Bit;
            CompressionType6Ae_Decompress_40A8A0((u8*) pAnimFg1Data, (u8*) gDecodeBuffer);
            // Hacky because palette is 16 width, but converted sprite is normalized for 16 -> 256
            // So we scale it back down for the shader.
            gFakeTextureCache.mPalNormMulti = 16;
            glBindTexture(GL_TEXTURE_2D, gDecodedTextureCache);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tWidth / 2, tHeight, 0, GL_RED, GL_UNSIGNED_BYTE, gDecodeBuffer);
            break;
        case TPageMode::e8Bit_1:
            gFakeTextureCache.mBitDepth = IRenderer::BitDepth::e16Bit;
            CompressionType_3Ae_Decompress_40A6A0((u8*) pAnimFg1Data, (u8*) gDecodeBuffer);
            glBindTexture(GL_TEXTURE_2D, gDecodedTextureCache);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tWidth, tHeight, 0, GL_RED, GL_UNSIGNED_BYTE, gDecodeBuffer);
            break;
        case TPageMode::e16Bit_2:
            // TODO:  FG1's get rendered here for AE.
            s16 fg1Width = poly.mVerts[0].mVert.x - poly.mBase.vert.x;
            s16 fg1Height = poly.mVerts[1].mVert.y - poly.mBase.vert.y;
            gFakeTextureCache.mVramRect = {0, 0, fg1Width, fg1Height};
            gFakeTextureCache.mBitDepth = IRenderer::BitDepth::e16Bit;
            gFakeTextureCache.mIsFG1 = true;
            glBindTexture(GL_TEXTURE_2D, gDecodedTextureCache);
            Renderer_ConvertFG1BitMask(fg1Width, fg1Height, (u8*) pAnimFg1Data);
            break;
    }

    return &gFakeTextureCache;
}

void OpenGLRenderer::DrawTexturePalette(GLuint pTexture, f32 x, f32 y, f32 width, f32 height, glm::vec3 color, glm::vec2 uv0, glm::vec2 uv1, PaletteCache * palette, int palDepth)
{
    const f32 r = color.r;
    const f32 g = color.g;
    const f32 b = color.b;

    const VertexData verts[4] = {
        {0, 0, 0, r, g, b, uv0.x, uv0.y},
        {1, 0, 0, r, g, b, uv1.x, uv0.y},
        {1, 1, 0, r, g, b, uv1.x, uv1.y},
        {0, 1, 0, r, g, b, uv0.x, uv1.y} };

    mTextureShader.Use();

    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP(x, y, width, height));
    mTextureShader.Uniform1i("m_Sprite", 0);  // Set m_Sprite to GL_TEXTURE0
    mTextureShader.Uniform1i("m_Palette", 1); // Set m_Sprite to GL_TEXTURE1
    mTextureShader.Uniform1i("m_PaletteEnabled", true);
    mTextureShader.Uniform1i("m_Textured", true);
    mTextureShader.Uniform1i("m_PaletteDepth", palDepth);

    Renderer_BindPalette(palette);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pTexture);

    

    const GLuint indexData[6] = { 0, 1, 3, 3, 1, 2 };
    DrawTriangles(verts, 4, indexData, 6);

    mTextureShader.UnUse();
}

void OpenGLRenderer::DrawTexture(GLuint pTexture, f32 x, f32 y, f32 width, f32 height, glm::vec3 color, glm::vec2 uv0, glm::vec2 uv1)
{
    const f32 r = color.r;
    const f32 g = color.g;
    const f32 b = color.b;

    const VertexData verts[4] = {
        {0, 0, 0, r, g, b, uv0.x, uv0.y},
        {1, 0, 0, r, g, b, uv1.x, uv0.y},
        {1, 1, 0, r, g, b, uv1.x, uv1.y},
        {0, 1, 0, r, g, b, uv0.x, uv1.y} };

    mTextureShader.Use();

    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP(x, y, width, height));
    mTextureShader.Uniform1i("m_Sprite", 0); // Set m_Sprite to GL_TEXTURE0
    mTextureShader.Uniform1i("m_PaletteEnabled", false);
    mTextureShader.Uniform1i("m_Textured", true);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pTexture);

    const GLuint indexData[6] = { 0, 1, 3, 3, 1, 2 };
    DrawTriangles(verts, 4, indexData, 6);

    mTextureShader.UnUse();
}

void OpenGLRenderer::DrawTexture(GLuint pTexture, f32 x, f32 y, f32 width, f32 height, glm::vec2 uv0, glm::vec2 uv1)
{
    DrawTexture(pTexture, x, y, width, height, glm::vec3(1, 1, 1), uv0, uv1);
}


void OpenGLRenderer::InitAttributes()
{
    // Tell GL how to transfer our Vertex struct to our shaders.
    glBindVertexArray(mVAO);
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
}

void OpenGLRenderer::RenderBackground()
{
    Renderer_SetBlendMode(TPageAbr::eBlend_0);
    DrawTexture(GetBackgroundTexture(), 0, 0, 640, 240);
}

glm::mat4 OpenGLRenderer::GetMVP()
{
    return m_View;
}

glm::mat4 OpenGLRenderer::GetMVP(f32 x, f32 y, f32 width, f32 height)
{
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(x, y, 0));
    model = glm::scale(model, glm::vec3(width, height, 1));
    return m_View * model;
}

void OpenGLRenderer::DebugWindow()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("Enable OpenGL Info", nullptr, &gGLDebugInfo);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Rendering"))
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

        if (ImGui::BeginMenu("External Assets"))
        {
            ImGui::MenuItem("Show Anim IDs", nullptr, &gShowAnimIDs);
            ImGui::MenuItem("External Textures Enabled", nullptr, &gExternalTexturesEnabled);

            if (ImGui::MenuItem("Reload External Textures"))
            {
                for (auto& extTexture : gLoadedExternalTextures)
                {
                    // Free up all our currently loaded textures

                    for (auto t : extTexture.second.textures)
                    {
                        glDeleteTextures(1, &t.handle);
                    }

                    for (auto t : extTexture.second.textures_flipped)
                    {
                        glDeleteTextures(1, &t.second.handle);
                    }

                    // Free emissive textures
                    for (auto t : extTexture.second.textures_emissive)
                    {
                        glDeleteTextures(1, &t.second.handle);
                    }

                    for (auto t : extTexture.second.textures_flipped_emissive)
                    {
                        glDeleteTextures(1, &t.second.handle);
                    }
                }

                gLoadedExternalTextures.clear();

                LoadAllExternalTextures();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    //ImGui::ShowDemoWindow();

    if (ImGui::Begin("External Meta Editor"))
    {
        // create a combo box to select meta in gLoadedExternalTextures
        static AnimId selectedMeta = AnimId::None;

        if (ImGui::BeginCombo("Texture", magic_enum::enum_name<AnimId>(selectedMeta).data()))
        {
            for (auto& meta : gLoadedExternalTextures)
            {
                bool is_selected = (selectedMeta == meta.first);
                if (ImGui::Selectable(magic_enum::enum_name<AnimId>(meta.first).data(), is_selected))
                {
                    selectedMeta = meta.first;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (selectedMeta != AnimId::None)
        {
            ImGui::Separator();

            // edit all the attributes of the selected meta
            ImGui::DragInt2("Ref Image Size", &gLoadedExternalTextures[selectedMeta].meta.size_width);
            ImGui::DragInt2("Offset", &gLoadedExternalTextures[selectedMeta].meta.offset_x);

            ImGui::Separator();

            if (ImGui::Button("Save"))
            {
                gLoadedExternalTextures[selectedMeta].meta.SaveJSONToFile(std::string(gExternalAssetPath) + "sprites/" + std::string(magic_enum::enum_name<AnimId>(selectedMeta)) + "/meta.json");
            }
        }
    }
    ImGui::End();

    if (gGLDebugInfo)
    {
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

                ImGui::Image(GL_TO_IMGUI_TEX(gRendererTextures[i].mTextureID), { textureWidth, textureHeight });
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
        ImGui::End();

        if (ImGui::Begin("GPU Info", nullptr, ImGuiWindowFlags_MenuBar))
        {

            ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
            ImGui::Text("Model: %s", glGetString(GL_RENDERER));
            ImGui::Text("GL Version: %s", glGetString(GL_VERSION));
            ImGui::Text("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
        }
        ImGui::End();

        if (ImGui::Begin("Palettes", nullptr, ImGuiWindowFlags_MenuBar))
        {
            f32 width = ImGui::GetWindowContentRegionWidth();
            for (auto& pal : gRendererPals)
            {
                ImGui::Image(GL_TO_IMGUI_TEX(pal.mPalTextureID), ImVec2(width, 16));
            }
        }
        ImGui::End();

        if (ImGui::Begin("Background Texture", nullptr, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Text("BG Handle: %i", GetBackgroundTexture());
            auto region = ImGui::GetContentRegionAvail();
            auto bgTexId = GetBackgroundTexture();
            ImGui::Image(GL_TO_IMGUI_TEX(bgTexId), region);
        }
        ImGui::End();

        if (ImGui::Begin("FrameBuffer Texture", nullptr, ImGuiWindowFlags_MenuBar))
        {
            auto region = ImGui::GetContentRegionAvail();
            ImGui::Image(GL_TO_IMGUI_TEX(mWindowFrameBuffer.texture_handle), region);
        }
        ImGui::End();

        if (ImGui::Begin("Last Temp Texture", nullptr, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Text("Size: %i, %i", mTempSpriteBuffer.width, mTempSpriteBuffer.height);
            auto region = ImGui::GetContentRegionAvail();
            ImGui::Image(GL_TO_IMGUI_TEX(mTempSpriteBuffer.texture_handle), region);
        }
        ImGui::End();

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
                ImGui::Image(GL_TO_IMGUI_TEX(gRendererTextures[i].mTextureID), { textureWidth, textureHeight });
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
    }
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

    glDeleteTextures(1, &gDecodedTextureCache);

    if (mContext)
    {
        SDL_GL_DeleteContext(mContext);
        mContext = nullptr;
    }
}

bool CreateRenderBuffer(FrameBuffer* pFrameBuffer, int width, int height, GLint interpolateMode, bool alpha = false)
{
    if (pFrameBuffer->handle == 0)
    {
        glGenFramebuffers(1, &pFrameBuffer->handle);
        glBindFramebuffer(GL_FRAMEBUFFER, pFrameBuffer->handle);
        glGenTextures(1, &pFrameBuffer->texture_handle);
        glBindTexture(GL_TEXTURE_2D, pFrameBuffer->texture_handle);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pFrameBuffer->texture_handle, 0);
    }

    if (width != pFrameBuffer->width && height != pFrameBuffer->height)
    {
        glBindTexture(GL_TEXTURE_2D, pFrameBuffer->texture_handle);
        glTexImage2D(GL_TEXTURE_2D, 0, (alpha) ? GL_RGBA : GL_RGB, width, height, 0, (alpha) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolateMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolateMode);

        pFrameBuffer->width = width;
        pFrameBuffer->height = height;
    }
    

    return true;
}

void OpenGLRenderer::CreateWindowFrameBuffer(int width, int height)
{
    CreateRenderBuffer(&mWindowFrameBuffer, width, height, GL_NEAREST);
}

void OpenGLRenderer::RenderFrameBuffer()
{
    //bool keepAspet = false;

    int w, h;
    float ratio = 640.0f / 480.0f;
    bool keepAspect = true;

    SDL_GetWindowSize(mWindow, &w, &h);
    m_View = glm::ortho<f32>(0, static_cast<f32>(w), static_cast<f32>(h), 0, 0, 1);

    // calculate the destination rectangle for the framebuffer using the window size and desired aspect ratio
    glm::vec4 destRect = glm::vec4(0, 0, w, h);
    
    if (keepAspect)
    {
        if (w > h * ratio)
        {
            destRect.x = (w - (h * ratio)) / 2;
            destRect.z = h * ratio;
        }
        else
        {
            destRect.y = (h - (w / ratio)) / 2;
            destRect.w = w / ratio;
        }
    }

    if (gShowAnimIDs)
    {
        ImDrawList* bgDrawList = ImGui::GetBackgroundDrawList();

        for (auto& text : gDebugTexts)
        {
            glm::vec2 pos = { destRect.x + (text.screen_space_x * (destRect.z / 640.0f)), destRect.y + (text.screen_space_y * (destRect.w / 240.0f)) };
            bgDrawList->AddText({ pos.x + 2 ,pos.y + 2 }, IM_COL32(0, 0, 0, 255), text.text.c_str());
            bgDrawList->AddText({ pos.x ,pos.y }, text.color, text.text.c_str());
        }
    }

    gDebugTexts.clear();

    Renderer_SetBlendMode(TPageAbr::eBlend_0);
    DrawTexture(mWindowFrameBuffer.texture_handle, destRect.x, destRect.y, destRect.z, destRect.w);
}

bool OpenGLRenderer::Create(TWindowHandleType window)
{
    mWindow = window;
    mWireframe = false;

    gGLInstance = this;

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create context
    mContext = SDL_GL_CreateContext(window);
    if (mContext == NULL)
    {
        LOG_ERROR("OpenGL context could not be created! SDL Error: " << SDL_GetError());
        return false;
    }
    else
    {
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

    
}

void OpenGLRenderer::StartFrame(s32 /*xOff*/, s32 /*yOff*/)
{
    static s32 oldWidth = 0;
    static s32 oldHeight = 0;

    SDL_GetWindowSize(mWindow, &mWindowWidth, &mWindowHeight);

    if (oldWidth != mWindowWidth || oldHeight != mWindowHeight)
    {
        CreateWindowFrameBuffer(mWindowWidth, mWindowHeight);
    }

    oldWidth = mWindowWidth;
    oldHeight = mWindowHeight;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(mWindow);
    ImGui::NewFrame();

    //SDL_GL_SwapWindow(mWindow);

    DebugWindow();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set our Projection Matrix, so stuff doesn't get rendered in the quantum realm.
    m_View = glm::ortho<f32>(0, 640, 0, 240, 0, 1);

    glViewport(0, 0, mWindowWidth, mWindowHeight);

    glBindFramebuffer(GL_FRAMEBUFFER, mWindowFrameBuffer.handle);
}

// This function should free both vrams allocations AND palettes, cause theyre kinda the same thing.
void OpenGLRenderer::PalFree(const PalRecord& record)
{
    Pal_free_483390(PSX_Point{record.x, record.y}, record.depth); // TODO: Stop depending on this

    Renderer_FreePalette({
        record.x,
        record.y,
    });
    Renderer_FreeTexture({
        record.x,
        record.y,
    });
}

bool OpenGLRenderer::PalAlloc(PalRecord& record)
{
    PSX_RECT rect = {};
    // TODO: Stop depending on this
    const bool ret = Pal_Allocate_483110(&rect, record.depth);
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_SCISSOR_TEST);

    RenderFrameBuffer();

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OpenGLRenderer::Present()
{
    SDL_GL_SwapWindow(mWindow);

    CustomRender_ClearCommands();
}

void OpenGLRenderer::BltBackBuffer(const SDL_Rect* /*pCopyRect*/, const SDL_Rect* /*pDst*/)
{
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
              static_cast<GLint>((y / 240.0f) * h),
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
                             static_cast<f32>(offset.field_E_yoff),
                             static_cast<f32>(240 + offset.field_E_yoff), 0.0f, 1.0f);
}

void OpenGLRenderer::Draw(Prim_Sprt& sprt)
{
    if (!gRenderEnable_SPRT)
        return;

    // Detect our magic code and render our cam.
    /*if (sprt.mBase.header.rgb_code.r == 255 && sprt.mBase.header.rgb_code.g == 254 && sprt.mBase.header.rgb_code.b == 253)
    {
        RenderBackground();
        return;
    }*/

    PSX_Point vramPoint = Renderer_VRamFromTPage(mLastTPage);
    s16 textureMode = (mLastTPage >> 7) & 3;

    // FG1 Blocks
    if (vramPoint.field_0_x < 640)
    {
        const glm::vec2 uv0 = glm::vec2(sprt.mBase.vert.x / 640.0f, sprt.mBase.vert.y / 240.0f);
        const glm::vec2 uv1 = glm::vec2((sprt.mBase.vert.x + sprt.field_14_w + 1) / 640.0f, (sprt.mBase.vert.y + sprt.field_16_h + 1) / 240.0f);

        DrawTexture(GetBackgroundTexture(), static_cast<f32>(sprt.mBase.vert.x), static_cast<f32>(sprt.mBase.vert.y), static_cast<f32>(sprt.field_14_w + 1), static_cast<f32>(sprt.field_16_h + 1), uv0, uv1);
        
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    TextureCache* pTexture = Renderer_TexFromVRam({static_cast<s16>(vramPoint.field_0_x + WidthBppDivide(textureMode, sprt.mUv.u)), static_cast<s16>(vramPoint.field_2_y + sprt.mUv.v)});
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

    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP(tile.mBase.vert.x, tile.mBase.vert.y, tile.field_14_w, tile.field_16_h));
    mTextureShader.Uniform1i("m_PaletteEnabled", false);
    mTextureShader.Uniform1i("m_Textured", false);

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

    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP());
    mTextureShader.Uniform1i("m_Textured", false);

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

    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP());
    mTextureShader.Uniform1i("m_Textured", false);

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

    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP());
    mTextureShader.Uniform1i("m_Textured", false);

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

    // Set our Projection Matrix, so stuff doesn't get rendered in the quantum realm.
    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP());

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

    // Set our Projection Matrix, so stuff doesn't get rendered in the quantum realm.
    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP());

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

    // Set our Projection Matrix, so stuff doesn't get rendered in the quantum realm.
    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP());
    mTextureShader.Uniform1i("m_Textured", false);

    const GLuint indexData[6] = {0, 1, 2, 0, 2, 3};
    DrawTriangles(verts, 4, indexData, 6);

    mTextureShader.UnUse();
}

void DrawCustomSprite(Poly_FT4& poly, CustomRenderSpriteFormat* sprite)
{
    static bool firstLoad = true;

    if (firstLoad)
    {
        firstLoad = false;

        LoadAllExternalTextures();
    }

    if (FrameTableOffsetExists(sprite->frametable_offset, gIsGameAE))
    {
        AnimRecord animRecord = AnimRecFrameTable(sprite->frametable_offset, sprite->resource_id, gIsGameAE);

        const Uint32 debugColors[] = {
            IM_COL32(0, 0xFF, 0, 0xFF),
            IM_COL32(0xFF, 0, 0, 0xFF),
            IM_COL32(0xFF, 0xC0, 0, 0xFF),
            IM_COL32(0xFF, 0xFC, 0, 0xFF),
            IM_COL32(0, 0xFF, 0xFF, 0xFF),
        };

        std::string animName = std::string(magic_enum::enum_name<AnimId>(animRecord.mId));

        if (animRecord.mId != AnimId::ObjectShadow)
        {
            gDebugTexts.push_back({ animName + " | " + std::to_string(sprite->frametable_offset), sprite->x, sprite->y, debugColors[static_cast<int>(animRecord.mId) % (sizeof(debugColors) / sizeof(Uint32))] });
        }

        // TODO: For now we don't draw custom ObjectShadows till we can properly render them ( direct use of triangle buffer )
        if (animRecord.mId == AnimId::ObjectShadow || !gExternalTexturesEnabled || gLoadedExternalTextures.find(animRecord.mId) == gLoadedExternalTextures.end() || sprite->resource_id != animRecord.mResourceId) {

            CustomRender_RemoveCommand(&poly);
            gGLInstance->Draw(poly);
            return;
        }

        ExternalTextureMeta& loadedTexture = gLoadedExternalTextures[animRecord.mId];

        if (sprite->frame >= loadedTexture.textures.size())
        {
            SetPrimExtraPointerHack(&poly, sprite->origPtr);
            gGLInstance->Draw(poly);
            return;
        }

        auto cache = loadedTexture.textures[sprite->frame];

        // if a flipped texture exists, then use it instead
        const bool flippedTextureExists = sprite->flip && loadedTexture.textures_flipped.find(sprite->frame) != loadedTexture.textures_flipped.end();
        if (flippedTextureExists)
        {
            cache = loadedTexture.textures_flipped[sprite->frame];
        }

        glm::vec2 imgSize = glm::vec2(loadedTexture.meta.size_width, loadedTexture.meta.size_height) * sprite->scale;
        imgSize.y *= 0.5f;

        Renderer_ParseTPageBlendMode(poly.mVerts[0].mUv.tpage_clut_pad);

        float offX = (loadedTexture.meta.offset_x - 1) * sprite->scale;
        float offY = (loadedTexture.meta.offset_y - 2) * sprite->scale * 0.5f;

        glm::vec2 uv0 = { 0.0f, 0.0f };
        glm::vec2 uv1 = { 1.0f, 1.0f };

        if (sprite->flip)
        {
            offX = (loadedTexture.meta.size_width - (loadedTexture.meta.offset_x - 1)) * sprite->scale;
            uv0 = { 1.0f,0.0f };
            uv1 = { 0.0f, 1.0f };
        }

        f32 r = sprite->r / 128.0f;
        f32 g = sprite->g / 128.0f;
        f32 b = sprite->b / 128.0f;

        // clamp rgb values to 0 - 1
        r = glm::clamp(r, 0.0f, 1.0f);
        g = glm::clamp(g, 0.0f, 1.0f);
        b = glm::clamp(b, 0.0f, 1.0f);

        //glm::vec3 color = glm::vec3(sprite->r / 128.0f, sprite->g / 128.0f, sprite->b / 128.0f);
        glm::vec3 color = glm::vec3(r, g, b);

        gGLInstance->DrawTexture(cache.handle, (f32)sprite->x - offX, (f32)sprite->y - offY, imgSize.x, imgSize.y, color, uv0, uv1);

        // if we have an emissive texture, draw it
        if (loadedTexture.textures_emissive.find(sprite->frame) != loadedTexture.textures_emissive.end())
        {
            auto emissiveTexture = loadedTexture.textures_emissive[sprite->frame];

            if (flippedTextureExists && loadedTexture.textures_flipped_emissive.find(sprite->frame) != loadedTexture.textures_flipped_emissive.end())
            {
                emissiveTexture = loadedTexture.textures_flipped_emissive[sprite->frame];
            }

            Renderer_SetBlendMode(TPageAbr::eBlend_1);

            gGLInstance->DrawTexture(emissiveTexture.handle, (f32)sprite->x - offX, (f32)sprite->y - offY, imgSize.x, imgSize.y, glm::vec3(1,1,1) - color, uv0, uv1);
        }
    }
}

glm::vec4 calc_bounds(glm::vec2* points, int numPoints) {
    glm::vec4 bounds = glm::vec4(points[0], points[0]);
    for (int i = 1; i < numPoints; i++) {
        bounds.x = glm::min(bounds.x, points[i].x);
        bounds.y = glm::min(bounds.y, points[i].y);
        bounds.z = glm::max(bounds.z, points[i].x);
        bounds.w = glm::max(bounds.w, points[i].y);
    }
    return bounds;
}

/* // Slow experimental pre rendering of paletted sprites to get linear filtering.
void OpenGLRenderer::Draw(Poly_FT4& poly)
{
    if (!gRenderEnable_FT4)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    TextureCache* pTexture = nullptr;

    // Some polys have their texture data directly attached to polys.
    auto ptrData = GetPrimExtraPointerHack(&poly);
    if (ptrData)
    {
        if (*(int*)ptrData == 0x12345678)
        {
            // HD Hack
            DrawCustomSprite(poly, (CustomRenderSpriteFormat*)ptrData);
            return;
        }
        else
        {
            pTexture = Renderer_TextureFromAnim(poly);
        }
    }
    else
        pTexture = Renderer_TexFromTPage(poly.mVerts[0].mUv.tpage_clut_pad, poly.mUv.u, poly.mUv.v);

    PaletteCache* pPal = Renderer_ClutToPalette(poly.mUv.tpage_clut_pad);

    int palDepth = 0;

    if (pPal != nullptr)
    {
        if (pTexture->mPalNormMulti != 0)
            palDepth = pPal->mPalDepth * gFakeTextureCache.mPalNormMulti;
        else
            palDepth = pPal->mPalDepth;
    }

    if (pTexture == nullptr)
    {
        //LOG_WARNING("Trying to render FT4 with no texture!");
        return;
    }

    const GLuint indexData[6] = { 1, 0, 3, 3, 0, 2 };

    f32 r = poly.mBase.header.rgb_code.r / 64.0f;
    f32 g = poly.mBase.header.rgb_code.g / 64.0f;
    f32 b = poly.mBase.header.rgb_code.b / 64.0f;

    if (pTexture->mIgnoreColor)
    {
        r = 1.0f;
        g = 1.0f;
        b = 1.0f;
    }

    s32 xOff = (pTexture->mVramRect.x & 63);
    s32 bppMulti = 1;

    switch (pTexture->mBitDepth)
    {
    case BitDepth::e8Bit:
        bppMulti = 2;
        break;
    case BitDepth::e4Bit:
        bppMulti = 4;
        break;
    default:
        break;
    }

    xOff *= bppMulti;

    // macros suck. todo: fix that
#define UV_U(v) (f32)(((pTexture->mUvOffset.field_0_x + v) - xOff) / (f32)(pTexture->mVramRect.w * bppMulti))
#define UV_V(v) (f32)(((pTexture->mUvOffset.field_2_y + v) - static_cast<u8>(pTexture->mVramRect.y)) / (f32) pTexture->mVramRect.h)

    VertexData verts[4] = {
        {(f32)poly.mBase.vert.x, (f32)poly.mBase.vert.y, 0, r, g, b, UV_U(poly.mUv.u), UV_V(poly.mUv.v)},
        {(f32)poly.mVerts[0].mVert.x, (f32)poly.mVerts[0].mVert.y, 0, r, g, b, UV_U(poly.mVerts[0].mUv.u), UV_V(poly.mVerts[0].mUv.v)},
        {(f32)poly.mVerts[1].mVert.x, (f32)poly.mVerts[1].mVert.y, 0, r, g, b, UV_U(poly.mVerts[1].mUv.u), UV_V(poly.mVerts[1].mUv.v)},
        {(f32)poly.mVerts[2].mVert.x, (f32)poly.mVerts[2].mVert.y, 0, r, g, b, UV_U(poly.mVerts[2].mUv.u), UV_V(poly.mVerts[2].mUv.v)} };

    // Todo: Calculate the bounds of the polygon, render to a temp frame buffer, then re render at full scale.
    // This lets us render fast palettes using hardware acceleration then be able to stretch using a linear filter.
    // Cause currently using linear interp screws up our shader (cause pixels are actually palette index, lerping between palette indexes
    // causes bad artifacting.

    std::vector<glm::vec2> points;
    for (auto p : verts)
    {
        points.push_back({ p.x, p.y });
    }

    glm::vec4 bounds = calc_bounds(points.data(), static_cast<int>(points.size()));


    float width = bounds.z - bounds.x;
    float height = bounds.w - bounds.y;

    // Start rendering into temp buffer
    CreateRenderBuffer(&mTempSpriteBuffer, 256, 256, GL_LINEAR, true);
    glBindFramebuffer(GL_FRAMEBUFFER, mTempSpriteBuffer.handle);
    glViewport(0, 0, mTempSpriteBuffer.width, mTempSpriteBuffer.height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the texture using the palette
    auto oldMView = m_View;
    m_View = glm::ortho<f32>(0, static_cast<f32>(mTempSpriteBuffer.width), 0, static_cast<f32>(mTempSpriteBuffer.height), 0, 1);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    DrawTexturePalette(pTexture->mTextureID, 0,0, mTempSpriteBuffer.width, mTempSpriteBuffer.height, glm::vec4(1,1,1,1), glm::vec2(0, 0), glm::vec2(1, 1), pPal, palDepth);
    m_View = oldMView;
    // Revert back to screen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, mWindowFrameBuffer.handle);
    glViewport(0, 0, mWindowWidth, mWindowHeight);

    mTextureShader.Use();

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTempSpriteBuffer.texture_handle);
    //Renderer_BindTexture(pTexture);

    if (pTexture->mIsFG1)
    {
        const f32 overdraw = 0.2f; // stops weird line rendering issues.
        // This is an FG1, so UV's are maxed;
        verts[0] = { (f32)poly.mBase.vert.x, (f32)poly.mBase.vert.y, 0, 1.0f, 1.0f, 1.0f, 0, 0 };
        verts[1] = { (f32)poly.mVerts[0].mVert.x + overdraw, (f32)poly.mVerts[0].mVert.y, 0, 1.0f, 1.0f, 1.0f, 1, 0 };
        verts[2] = { (f32)poly.mVerts[1].mVert.x, (f32)poly.mVerts[1].mVert.y + overdraw, 0, 1.0f, 1.0f, 1.0f, 0, 1 };
        verts[3] = { (f32)poly.mVerts[2].mVert.x + overdraw, (f32)poly.mVerts[2].mVert.y + overdraw, 0, 1.0f, 1.0f, 1.0f, 1, 1 };


        // Hack, set palette texture to our background.
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, GetBackgroundTexture());

        mTextureShader.UniformVec4("m_FG1Size", glm::vec4(poly.mBase.vert.x, poly.mBase.vert.y, pTexture->mVramRect.w + overdraw, pTexture->mVramRect.h + overdraw));
        mTextureShader.Uniform1i("m_FG1", true);
    }

    // Set our Projection Matrix, so stuff doesn't get rendered in the quantum realm.
    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP());

    mTextureShader.Uniform1i("m_Sprite", 0);  // Set m_Sprite to GL_TEXTURE0
    mTextureShader.Uniform1i("m_Palette", 1); // Set m_Palette to GL_TEXTURE1
    mTextureShader.Uniform1i("m_Textured", true);
    mTextureShader.Uniform1i("m_PaletteEnabled", false);
    mTextureShader.Uniform1i("m_PaletteDepth", 0);

    // Hack to use a HD menu font.
    if (pTexture->mVramRect.w == 64 && pTexture->mVramRect.h == 256)
    {
        static GLuint FontTexture = TextureFromFile("menufont.png");

        if (FontTexture != 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, FontTexture);
            mTextureShader.Uniform1i("m_PaletteEnabled", false);
        }
    }

    
    //for (auto& p : verts)
    //{
    //    p.x -= bounds.x;
    //    p.y -= bounds.y;
    //}
    
    

    Renderer_ParseTPageBlendMode(poly.mVerts[0].mUv.tpage_clut_pad);
    DrawTriangles(verts, 4, indexData, 6);
    //static GLuint testText = TextureFromFile("menufont.png");
    //DrawTexture(testText, bounds.x, bounds.y, width, height);

    mTextureShader.Uniform1i("m_FG1", false);
    // render temp frame buffer

    mTextureShader.UnUse();
}
*/


void OpenGLRenderer::Draw(Poly_FT4& poly)
{
    if (!gRenderEnable_FT4)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    TextureCache* pTexture = nullptr;

    auto customCommand = CustomRender_GetCommand(&poly);

    if (customCommand != nullptr)
    {
        DrawCustomSprite(poly, customCommand);
        return;
    }

    // Some polys have their texture data directly attached to polys.
    auto ptrData = GetPrimExtraPointerHack(&poly);
    if (ptrData)
    {
        pTexture = Renderer_TextureFromAnim(poly);
    }
    else
        pTexture = Renderer_TexFromTPage(poly.mVerts[0].mUv.tpage_clut_pad, poly.mUv.u, poly.mUv.v);

    PaletteCache* pPal = Renderer_ClutToPalette(poly.mUv.tpage_clut_pad);

    if (pTexture == nullptr)
    {
        LOG_WARNING("Trying to render FT4 with no texture!");
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

    s32 xOff = (pTexture->mVramRect.x & 63);
    s32 bppMulti = 1;

    switch (pTexture->mBitDepth)
    {
        case BitDepth::e8Bit:
            bppMulti = 2;
            break;
        case BitDepth::e4Bit:
            bppMulti = 4;
            break;
        default:
            break;
    }

    xOff *= bppMulti;

    // macros suck. todo: fix that
#define UV_U(v) (f32)(((pTexture->mUvOffset.field_0_x + v) - xOff) / (f32)(pTexture->mVramRect.w * bppMulti))
#define UV_V(v) (f32)(((pTexture->mUvOffset.field_2_y + v) - static_cast<u8>(pTexture->mVramRect.y)) / (f32) pTexture->mVramRect.h)

    VertexData verts[4] = {
        {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, r, g, b, UV_U(poly.mUv.u), UV_V(poly.mUv.v)},
        {(f32) poly.mVerts[0].mVert.x, (f32) poly.mVerts[0].mVert.y, 0, r, g, b, UV_U(poly.mVerts[0].mUv.u), UV_V(poly.mVerts[0].mUv.v)},
        {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y, 0, r, g, b, UV_U(poly.mVerts[1].mUv.u), UV_V(poly.mVerts[1].mUv.v)},
        {(f32) poly.mVerts[2].mVert.x, (f32) poly.mVerts[2].mVert.y, 0, r, g, b, UV_U(poly.mVerts[2].mUv.u), UV_V(poly.mVerts[2].mUv.v)}};

    Renderer_BindPalette(pPal);
    Renderer_BindTexture(pTexture);

    if (pTexture->mIsFG1)
    {
        const f32 overdraw = 0.2f; // stops weird line rendering issues.
        // This is an FG1, so UV's are maxed;
        verts[0] = {(f32) poly.mBase.vert.x, (f32) poly.mBase.vert.y, 0, 1.0f, 1.0f, 1.0f, 0, 0};
        verts[1] = {(f32) poly.mVerts[0].mVert.x + overdraw, (f32) poly.mVerts[0].mVert.y, 0, 1.0f, 1.0f, 1.0f, 1, 0};
        verts[2] = {(f32) poly.mVerts[1].mVert.x, (f32) poly.mVerts[1].mVert.y + overdraw, 0, 1.0f, 1.0f, 1.0f, 0, 1};
        verts[3] = {(f32) poly.mVerts[2].mVert.x + overdraw, (f32) poly.mVerts[2].mVert.y + overdraw, 0, 1.0f, 1.0f, 1.0f, 1, 1};


        // Hack, set palette texture to our background.
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, GetBackgroundTexture());

        mTextureShader.UniformVec4("m_FG1Size", glm::vec4(poly.mBase.vert.x, poly.mBase.vert.y, pTexture->mVramRect.w + overdraw, pTexture->mVramRect.h + overdraw));
        mTextureShader.Uniform1i("m_FG1", true);
    }

    // Set our Projection Matrix, so stuff doesn't get rendered in the quantum realm.
    mTextureShader.UniformMatrix4fv("m_MVP", GetMVP());

    mTextureShader.Uniform1i("m_Sprite", 0);  // Set m_Sprite to GL_TEXTURE0
    mTextureShader.Uniform1i("m_Palette", 1); // Set m_Palette to GL_TEXTURE1
    mTextureShader.Uniform1i("m_Textured", true);
    mTextureShader.Uniform1i("m_PaletteEnabled", pPal != nullptr);

    if (pPal != nullptr)
    {
        if (pTexture->mPalNormMulti != 0)
            mTextureShader.Uniform1i("m_PaletteDepth", pPal->mPalDepth * gFakeTextureCache.mPalNormMulti);
        else
            mTextureShader.Uniform1i("m_PaletteDepth", pPal->mPalDepth);
    }

    // Hack to use a HD menu font.
    if (pTexture->mVramRect.w == 64 && pTexture->mVramRect.h == 256)
    {
        static GLuint FontTexture = TextureFromFile("menufont.png");

        if (FontTexture != 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, FontTexture);
            mTextureShader.Uniform1i("m_PaletteEnabled", false);
        }
    }

    Renderer_ParseTPageBlendMode(poly.mVerts[0].mUv.tpage_clut_pad);

    const GLuint indexData[6] = {1, 0, 3, 3, 0, 2};
    DrawTriangles(verts, 4, indexData, 6);

    mTextureShader.Uniform1i("m_FG1", false);

    mTextureShader.UnUse();
}

void OpenGLRenderer::Draw(Poly_G4& poly)
{
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
}

void ConvertAOFG1(const u8* srcPalData, RGBAPixel* dst, s32 pixelCount)
{
    const u16* palShortPtr = reinterpret_cast<const u16*>(srcPalData);
    for (s32 i = 0; i < pixelCount; i++)
    {
        u16 oldPixel = palShortPtr[i];
        u8 semiTrans = (((oldPixel) >> 15) & 0x1);
        dst[i].G = ((oldPixel >> 5) & 0x1F) << 2;
        dst[i].R = ((oldPixel >> 0) & 0x1F) << 2;
        dst[i].B = ((oldPixel >> 10) & 0x1F) << 2;
        dst[i].A = ((semiTrans == 1) ? 0 : 255);
    }
}

void StitchAOCam(s32 x, s32 y, s32 width, s32 height, const u8* pPixels)
{
    u16* pDst = reinterpret_cast<u16*>(gDecodeBuffer);
    const u16* pSrc = reinterpret_cast<const u16*>(pPixels);

    for (s32 y1 = y; y1 < height; y1++)
    {
        memcpy(&pDst[x + (y1 * 640)], &pSrc[(y1 * width)], width * sizeof(s16));
    }
}

void OpenGLRenderer::Upload(BitDepth bitDepth, const PSX_RECT& rect, const u8* pPixels)
{
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
                        if (mStitchedBackground == 0)
                            mStitchedBackground = Renderer_CreateTexture(GL_NEAREST);

                        glBindTexture(GL_TEXTURE_2D, mStitchedBackground);
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
    }
}

void OpenGLRenderer::LoadExternalCam(const char* path, const unsigned char* key, int keyLength)
{
    std::vector<BYTE> fileData;
    bool encrypted = true;

    // Try to keep all paths and filenames lowercase for our linux friends.
    std::ifstream file(gExternalAssetPath + "cams/"s + std::string(path).substr(0, 8) + ".cam2", std::ios::binary);

    if (file.is_open())
    {
        fileData = std::vector<BYTE>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
    }

    if (fileData.size() == 0)
    {
        std::ifstream fileRaw(gExternalAssetPath + "cams/"s + std::string(path).substr(0, 8) + ".png", std::ios::binary);

        if (fileRaw.is_open())
        {
            fileData = std::vector<BYTE>((std::istreambuf_iterator<char>(fileRaw)), std::istreambuf_iterator<char>());
            fileRaw.close();
            encrypted = false;
        }
        else
        {
            glDeleteTextures(1, &mHDBackgroundTexture);
            mHDBackgroundTexture = 0;
            return;
        }
    }

    BYTE* fPtr = fileData.data();

    if (encrypted)
    {
        // XOR the custom cam file with the data from the original game.
        // You wouldn't steal an Abe? https://www.youtube.com/watch?v=HmZm8vNHBSU
        for (unsigned int i = 0; i < fileData.size(); i++)
        {
            fPtr[i] ^= key[i % keyLength];
        }
    }

    int x = 0, y = 0, comp = 0;
    const unsigned char* data = stbi_load_from_memory(fileData.data(), static_cast<int>(fileData.size()), &x, &y, &comp, 4);

    if (comp == 0)
    {
        // failed to load image
        std::string message;
        message.resize(1000);
        sprintf(message.data(), "%s corrupted! Make sure you're using the correct .lvl files for decryption!", path);
        Alive_Show_ErrorMsg(message.data());
        glDeleteTextures(1, &mHDBackgroundTexture);
        mHDBackgroundTexture = 0;
        return;
    }

    // Check if we've created a texture handle already, if not, do so.
    if (mHDBackgroundTexture == 0)
    {
        glGenTextures(1, &mHDBackgroundTexture);
    }

    glBindTexture(GL_TEXTURE_2D, mHDBackgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free((void*)data);
}