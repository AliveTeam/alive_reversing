#include "../AliveLibCommon/Types.hpp"
#include "../GLShader.hpp"


const char_type* gShader_PsxVSH = R"(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in ivec2 vsPos;
layout (location = 1) in uvec3 vsShadeColor;
layout (location = 2) in uvec4 vsUV;
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

    fsShadeColor = vsShadeColor;
    fsFlags = vsFlags;
    fsTexIndexing = vsTexIndexing;

    // This is basically for if there is no texture in this batch
    if (vsUV.z == 0u || vsUV.w == 0u)
    {
        fsUV = vec2(0.0);
    }
    else
    {
        fsUV = vec2(vsUV.xy) / vsUV.zw;
    }
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
uniform sampler2D texFG1Masks[4];
uniform sampler2D texSpriteSheets[8];

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
    return texture(texPalette, vec2(v, fsTexIndexing.x / 255.0));
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

void draw_default_ft4()
{
    float texelSprite = 0.0;

    switch (fsTexIndexing.y)
    {
        case 0u:
            texelSprite = texture(texSpriteSheets[0], fsUV).r;
            break;

        case 1u:
            texelSprite = texture(texSpriteSheets[1], fsUV).r;
            break;

        case 2u:
            texelSprite = texture(texSpriteSheets[2], fsUV).r;
            break;

        case 3u:
            texelSprite = texture(texSpriteSheets[3], fsUV).r;
            break;

        case 4u:
            texelSprite = texture(texSpriteSheets[4], fsUV).r;
            break;

        case 5u:
            texelSprite = texture(texSpriteSheets[5], fsUV).r;
            break;

        case 6u:
            texelSprite = texture(texSpriteSheets[6], fsUV).r;
            break;

        case 7u:
            texelSprite = texture(texSpriteSheets[7], fsUV).r;
            break;
    }

    vec4 texelPal = PixelToPalette(texelSprite);

    outColor = handle_final_color(texelPal, true);
}

void draw_cam()
{
    outColor = texture(texCamera, fsUV);

    outColor = vec4(outColor.rgb, 0.0);
}

void draw_fg1()
{
    vec4 mask = vec4(0.0);

    switch (fsTexIndexing.y)
    {
        case 0u:
            mask = texture(texFG1Masks[0], fsUV);
            break;

        case 1u:
            mask = texture(texFG1Masks[1], fsUV);
            break;

        case 2u:
            mask = texture(texFG1Masks[2], fsUV);
            break;

        case 3u:
            mask = texture(texFG1Masks[3], fsUV);
            break;
    }

    outColor = vec4(texture(texCamera, fsUV).rgb, 0.0);

    if (mask.rgb == vec3(0.0))
    {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void draw_gas()
{
    vec4 texelGas = texture(texGas, fsUV);

    if (dither())
    {
        outColor = texelGas * 0.5;
    }
    else
    {
        outColor = vec4(vec3(0.0), 1.0);
    }
}

void main()
{
    int drawType = int(fsFlags.x);

    switch (drawType)
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
)";
