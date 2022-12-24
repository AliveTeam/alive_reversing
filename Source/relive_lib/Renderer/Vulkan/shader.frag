#version 450

// TODO use layout(set = 0, binding = X) and allocate more descriptor sets

layout(binding = 1) uniform sampler2D texPalette;
layout(binding = 2) uniform sampler2D texGas; // TODO: Remove
layout(binding = 3) uniform sampler2D texCamera;
layout(binding = 4) uniform sampler2D texFG1Masks[4]; // TODO: Remove
layout(binding = 8) uniform sampler2D texSpriteSheets[9];


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint samplerIndex;
layout(location = 3) flat in uint palIndex;
layout(location = 4) flat in uint drawType;

// TODO: These are not yet bound
layout(location = 5) flat in uint isShaded;
layout(location = 6) flat in uint blendMode;
layout(location = 7) flat in uint isSemiTrans;

layout(location = 0) out vec4 outColor;

const int BLEND_MODE_HALF_DST_ADD_HALF_SRC = 0;
const int BLEND_MODE_ONE_DST_ADD_ONE_SRC   = 1;
const int BLEND_MODE_ONE_DST_SUB_ONE_SRC   = 2;
const int BLEND_MODE_ONE_DST_ADD_QRT_SRC   = 3;

const int DRAW_FLAT        = 0;
const int DRAW_DEFAULT_FT4 = 1;
const int DRAW_CAM         = 2;
const int DRAW_FG1         = 3;
const int DRAW_GAS         = 4;


vec4 PixelToPalette(float v)
{
    // TODO: LOD
    return texture(texPalette, vec2(v, palIndex / 255.0f));
}

bool dither()
{
    return mod(gl_FragCoord.x + mod(gl_FragCoord.y, 2.0), 2.0) > 0.0;
}

vec3 handle_shading(in vec3 texelT)
{
    vec3 texelP = texelT;

    if (isShaded != 0)
    {
        texelP = clamp((texelT * (fragColor / 255.0)) / 0.5f, 0.0f, 1.0f);
    }

    return texelP;
}

vec4 handle_final_color(in vec4 src, in bool doShading)
{
    vec4 ret = src;
    if (src == vec4(0.0))
    {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }

    if (doShading)
    {
        ret.rgb = handle_shading(src.rgb);
    }

    if (isSemiTrans != 0 && src.a == 1.0)
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

vec4 draw_flat()
{
    outColor.rgb = fragColor / 255.0;

    return handle_final_color(vec4(outColor.rgb, 1.0), false);
}

vec4 draw_default_ft4()
{
    float texelSprite = 0.0;
    switch (samplerIndex)
    {
        case 0u:
            texelSprite = textureLod(texSpriteSheets[0], fragTexCoord, 0.0f).r;
            break;

        case 1u:
            texelSprite = textureLod(texSpriteSheets[1], fragTexCoord, 0.0f).r;
            break;

        case 2u:
            texelSprite = textureLod(texSpriteSheets[2], fragTexCoord, 0.0f).r;
            break;

        case 3u:
            texelSprite = textureLod(texSpriteSheets[3], fragTexCoord, 0.0f).r;
            break;

        case 4u:
            texelSprite = textureLod(texSpriteSheets[4], fragTexCoord, 0.0f).r;
            break;

        case 5u:
            texelSprite = textureLod(texSpriteSheets[5], fragTexCoord, 0.0f).r;
            break;

        case 6u:
            texelSprite = textureLod(texSpriteSheets[6], fragTexCoord, 0.0f).r;
            break;

        case 7u:
            texelSprite = textureLod(texSpriteSheets[7], fragTexCoord, 0.0f).r;
            break;

        case 8u:
            texelSprite = textureLod(texSpriteSheets[8], fragTexCoord, 0.0f).r;
            break;
    }

    vec4 texelPal = PixelToPalette(texelSprite);
    return handle_final_color(texelPal, true);
}

vec4 draw_cam()
{
    return vec4(texture(texCamera, fragTexCoord).rgb, 0.0);
}


vec4 draw_fg1()
{
    vec4 mask = vec4(0.0);
    switch (samplerIndex)
    {
    // TODO textureLod
        case 0u:
            mask = texture(texSpriteSheets[0], fragTexCoord);
            break;

        case 1u:
            mask = texture(texSpriteSheets[1], fragTexCoord);
            break;

        case 2u:
            mask = texture(texSpriteSheets[2], fragTexCoord);
            break;

        case 3u:
            mask = texture(texSpriteSheets[3], fragTexCoord);
            break;

        // TODO: All other cases
    }

    vec4 ret = vec4(texture(texCamera, fragTexCoord).rgb, 0.0);

    if (mask.rgb == vec3(0.0))
    {
        ret = vec4(0.0, 0.0, 0.0, 1.0);
    }

    return ret;
}

vec4 draw_gas()
{
    // TODO: Make generic, TODO textureLod
    vec4 texelGas = texture(texGas, fragTexCoord);
    if (dither())
    {
        texelGas = texelGas * 0.5;
    }
    else
    {
        texelGas = vec4(vec3(0.0), 1.0);
    }

    return texelGas;
}


void main()
{
    switch (drawType)
    {
        case DRAW_FLAT:
            outColor = draw_flat();
            break;

        case DRAW_DEFAULT_FT4:
            outColor = draw_default_ft4();
            break;

        case DRAW_CAM:
            outColor = draw_cam();
            break;

        case DRAW_FG1:
            outColor = draw_fg1();
            break;

        case DRAW_GAS:
            outColor = draw_gas();
            break;
    }
}
