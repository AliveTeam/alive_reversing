#include "../../../../relive_lib/Types.hpp"
#include "../GLShader.hpp"


const char_type* gShader_PsxVSH = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec2 vsPos;
layout (location = 1) in vec3 vsShadeColor;
layout (location = 2) in vec2 vsUV;
layout (location = 3) in uvec4 vsFlags;
layout (location = 4) in uvec2 vsTexIndexing;

out vec3  fsShadeColor;
out vec2  fsUV;
flat out uvec4 fsFlags;
flat out uvec2 fsTexIndexing;

uniform vec2 vsViewportSize;


void main()
{
    gl_Position.x = ((vsPos.x / vsViewportSize.x) * 2) - 1;
    gl_Position.y = (1 - ((vsPos.y / vsViewportSize.y) * 2));
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fsUV = vsUV;
    fsShadeColor = vsShadeColor;
    fsFlags = vsFlags;
    fsTexIndexing = vsTexIndexing;
}
)";

const char_type* gShader_PsxFSH = R"(
#version 140

in vec3 fsShadeColor;
in vec2 fsUV;
flat in uvec4 fsFlags;
flat in uvec2 fsTexIndexing;

out vec4 outColor;

uniform sampler2D texPalette;
uniform sampler2D texGas;
uniform sampler2D texCamera;

uniform sampler2D texSpriteSheets[12];
uniform vec2 fsSpriteSheetSize[12];

uniform sampler2D texFramebuffer;

uniform bool bDrawingFramebuffer;

const int BLEND_MODE_HALF_DST_ADD_HALF_SRC = 0;
const int BLEND_MODE_ONE_DST_ADD_ONE_SRC   = 1;
const int BLEND_MODE_ONE_DST_SUB_ONE_SRC   = 2;
const int BLEND_MODE_ONE_DST_ADD_QRT_SRC   = 3;

const int DRAW_FLAT        = 0;
const int DRAW_DEFAULT_FT4 = 1;
const int DRAW_CAM         = 2;
const int DRAW_FG1         = 3;
const int DRAW_GAS         = 4;

const vec2 frameSize = vec2(640.0, 240.0);


vec4 PixelToPalette(float v)
{
    return texture(texPalette, vec2(v, float(fsTexIndexing.x) / 255.0));
}

bool dither()
{
    return mod(gl_FragCoord.x + mod(gl_FragCoord.y, 2.0), 2.0) > 0.0;
}

vec3 handle_shading(in vec3 texelT)
{
    bool isShaded = int(fsFlags.z) > 0;
    vec3 texelP = texelT;

    if (isShaded)
    {
        texelP = clamp((texelT * (fsShadeColor / 255.0)) / 0.5f, 0.0f, 1.0f);
    }

    return texelP;
}

vec4 handle_final_color(in vec4 src, in bool doShading)
{
    int blendMode = int(fsFlags.w);
    bool isSemiTrans = int(fsFlags.y) > 0;
    vec4 ret = src;

    if (src == vec4(0.0))
    {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }

    if (doShading)
    {
        ret.rgb = handle_shading(src.rgb);
    }

    if (isSemiTrans && src.a == 1.0)
    {
        switch (blendMode)
        {
            case BLEND_MODE_HALF_DST_ADD_HALF_SRC:
                ret = vec4(ret.rgb * 0.5, 0.5);
                break;

            case BLEND_MODE_ONE_DST_ADD_ONE_SRC:
            case BLEND_MODE_ONE_DST_SUB_ONE_SRC:
                ret = vec4(ret.rgb, 1.0);
                break;

            case BLEND_MODE_ONE_DST_ADD_QRT_SRC:
                ret = vec4(ret.rgb * 0.25, 1.0);
                break;
        }
    }
    else
    {
        ret = vec4(ret.rgb, 0.0);
    }

    return ret;
}

void draw_flat()
{
    outColor.rgb = fsShadeColor / 255.0;

    outColor = handle_final_color(vec4(outColor.rgb, 1.0), false);
}

vec4 sample_texture()
{
    ivec2 iUV = ivec2(fsUV);
    vec4 texelSprite = vec4(0.0);

    switch (fsTexIndexing.y)
    {
        case 0u:
            texelSprite = texelFetch(texSpriteSheets[0], iUV, 0);
            break;

        case 1u:
            texelSprite = texelFetch(texSpriteSheets[1], iUV, 0);
            break;

        case 2u:
            texelSprite = texelFetch(texSpriteSheets[2], iUV, 0);
            break;

        case 3u:
            texelSprite = texelFetch(texSpriteSheets[3], iUV, 0);
            break;

        case 4u:
            texelSprite = texelFetch(texSpriteSheets[4], iUV, 0);
            break;

        case 5u:
            texelSprite = texelFetch(texSpriteSheets[5], iUV, 0);
            break;

        case 6u:
            texelSprite = texelFetch(texSpriteSheets[6], iUV, 0);
            break;

        case 7u:
            texelSprite = texelFetch(texSpriteSheets[7], iUV, 0);
            break;

        case 8u:
            texelSprite = texelFetch(texSpriteSheets[8], iUV, 0);
            break;

        case 9u:
            texelSprite = texelFetch(texSpriteSheets[9], iUV, 0);
            break;

        case 10u:
            texelSprite = texelFetch(texSpriteSheets[10], iUV, 0);
            break;

        case 11u:
            texelSprite = texelFetch(texSpriteSheets[11], iUV, 0);
            break;
    }
    return texelSprite;
}

void draw_default_ft4()
{
    vec4 texelPal = PixelToPalette(sample_texture().r);
    outColor = handle_final_color(texelPal, true);
}

void draw_cam()
{
    vec2 scaledUV = fsUV / frameSize;

    outColor = texture(texCamera, scaledUV);

    outColor = vec4(outColor.rgb, 0.0);
}

void draw_fg1()
{
    vec2 scaledUV = fsUV / frameSize;
    vec4 mask = sample_texture();

    outColor = vec4(texture(texCamera, scaledUV).rgb, 0.0);

    if (mask.rgb == vec3(0.0))
    {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void draw_gas()
{
    vec2 scaledUV = fsUV / frameSize;
    vec4 texelGas = texture(texGas, scaledUV);

    if (dither())
    {
        outColor = texelGas * 0.5;
    }
    else
    {
        outColor = vec4(vec3(0.0), 1.0);
    }
}

void draw_framebuffer()
{
    vec2 scaledUV = fsUV / frameSize;

    outColor = texture(texFramebuffer, scaledUV);

    outColor = vec4(outColor.rgb, 0.0);
}

void main()
{
    if (bDrawingFramebuffer)
    {
        draw_framebuffer();
    }
    else
    {
        int drawMode = int(fsFlags.x);

        switch (drawMode)
        {
            case DRAW_FLAT:
                draw_flat();
                break;

            case DRAW_DEFAULT_FT4:
                draw_default_ft4();
                break;

            case DRAW_CAM:
                draw_cam();
                break;

            case DRAW_FG1:
                draw_fg1();
                break;

            case DRAW_GAS:
                draw_gas();
                break;
        }
    }
}
)";
