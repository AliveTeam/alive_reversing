#version 450

// Uniforms
layout(binding = 1) uniform sampler2D texPalette;
layout(binding = 2) uniform sampler2D texCamera;
layout(binding = 3) uniform sampler2D texSpriteSheets[14];

// Inputs from vertex shader
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint samplerIndex;
layout(location = 3) flat in uint palIndex;
layout(location = 4) flat in uint drawType;
layout(location = 5) flat in uint isShaded;
layout(location = 6) flat in uint blendMode;
layout(location = 7) flat in uint isSemiTrans;

// Output
layout(location = 0) out vec4 outColor;

// Blend constants
const int BLEND_MODE_HALF_DST_ADD_HALF_SRC = 0;
const int BLEND_MODE_ONE_DST_ADD_ONE_SRC   = 1;
const int BLEND_MODE_ONE_DST_SUB_ONE_SRC   = 2;
const int BLEND_MODE_ONE_DST_ADD_QRT_SRC   = 3;

// Draw type constants
const int DRAW_FLAT        = 0;
const int DRAW_DEFAULT_FT4 = 1;
const int DRAW_CAM         = 2;
const int DRAW_FG1         = 3;
const int DRAW_GAS         = 4;

vec4 PixelToPalette(float v)
{
    return textureLod(texPalette, vec2(v, palIndex / 255.0f), 0.0f);
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
        texelP = clamp((texelT * (fragColor)) / 0.5f, 0.0f, 1.0f); // TODO: Div frag color
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
    vec3 tmp = fragColor; // TODO: Div frag color
    return handle_final_color(vec4(tmp, 1.0), false);
}

vec4 SampleTexture()
{
    vec4 value = vec4(0.0f);
    switch (samplerIndex)
    {
        case 0u:
            value = textureLod(texSpriteSheets[0], fragTexCoord, 0.0f);
            break;

        case 1u:
            value = textureLod(texSpriteSheets[1], fragTexCoord, 0.0f);
            break;

        case 2u:
            value = textureLod(texSpriteSheets[2], fragTexCoord, 0.0f);
            break;

        case 3u:
            value = textureLod(texSpriteSheets[3], fragTexCoord, 0.0f);
            break;

        case 4u:
            value = textureLod(texSpriteSheets[4], fragTexCoord, 0.0f);
            break;

        case 5u:
            value = textureLod(texSpriteSheets[5], fragTexCoord, 0.0f);
            break;

        case 6u:
            value = textureLod(texSpriteSheets[6], fragTexCoord, 0.0f);
            break;

        case 7u:
            value = textureLod(texSpriteSheets[7], fragTexCoord, 0.0f);
            break;

        case 8u:
            value = textureLod(texSpriteSheets[8], fragTexCoord, 0.0f);
            break;

        case 9u:
            value = textureLod(texSpriteSheets[9], fragTexCoord, 0.0f);
            break;

        case 10u:
            value = textureLod(texSpriteSheets[10], fragTexCoord, 0.0f);
            break;

        case 11u:
            value = textureLod(texSpriteSheets[11], fragTexCoord, 0.0f);
            break;

        case 12u:
            value = textureLod(texSpriteSheets[12], fragTexCoord, 0.0f);
            break;

        case 13u:
            value = textureLod(texSpriteSheets[13], fragTexCoord, 0.0f);
            break;
    }
    return value;
}

vec4 draw_default_ft4()
{
    vec4 texelPal = PixelToPalette(SampleTexture().r);
    return handle_final_color(texelPal, true);
}

vec4 draw_cam()
{
    return vec4(textureLod(texCamera, fragTexCoord, 0.0f).rgb, 0.0);
}

vec4 draw_fg1()
{
    vec4 mask = SampleTexture();
    vec4 ret = vec4(textureLod(texCamera, fragTexCoord, 0.0f).rgb, 0.0);

    if (mask.rgb == vec3(0.0))
    {
        ret = vec4(0.0, 0.0, 0.0, 1.0);
    }

    return ret;
}

vec4 draw_gas()
{
    vec4 texelGas = SampleTexture();
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
