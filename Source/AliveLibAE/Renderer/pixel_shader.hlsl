R"(
sampler texPalette : register(s5);
sampler texGas : register(s6);
sampler texCamera : register(s4); // sX = sampler register X;
sampler texFG1Masks[4] : register(s0);
sampler texSpriteSheets[8] : register(s7);

static const int BLEND_MODE_HALF_DST_ADD_HALF_SRC = 0;
static const int BLEND_MODE_ONE_DST_ADD_ONE_SRC = 1;
static const int BLEND_MODE_ONE_DST_SUB_ONE_SRC = 2;
static const int BLEND_MODE_ONE_DST_ADD_QRT_SRC = 3;

static const int DRAW_FLAT = 0;
static const int DRAW_DEFAULT_FT4 = 1;
static const int DRAW_CAM = 2;
static const int DRAW_FG1 = 3;
static const int DRAW_GAS = 4;

float4 PixelToPalette(float v, int palIndex)
{
    return tex2D(texPalette, float2(v, palIndex / 255.0f));
}

float3 handle_shading(float4 fsShadeColor, float3 texelT, bool isShaded)
{
    float3 texelP = texelT;

    if (isShaded)
    {
        texelP.r = saturate((texelT.r * (fsShadeColor.r)) / 0.5f);
        texelP.g = saturate((texelT.g * (fsShadeColor.g)) / 0.5f);
        texelP.b = saturate((texelT.b * (fsShadeColor.b)) / 0.5f);
    }

    return texelP;
}

float4 handle_final_color(float4 fsShadeColor, float4 src, bool doShading, bool isShaded, int blendMode, bool isSemiTrans)
{
    // Overwrite with the pal alpha as our src alpha is actually the pal index to use
    fsShadeColor.a = src.a;

    float4 ret = src;

    if (all(src == float4(0.0, 0.0, 0.0, 0.0)))
    {
        return float4(0.0, 0.0, 0.0, 1.0);
    }

    if (doShading)
    {
        ret.rgb = handle_shading(fsShadeColor, src.rgb, isShaded);
    }

    if (isSemiTrans && src.a == 1.0)
    {
        if (blendMode == BLEND_MODE_HALF_DST_ADD_HALF_SRC)
        {
            ret = float4(ret.rgb * 0.5, 0.5);
        }
        else if (blendMode == BLEND_MODE_ONE_DST_ADD_ONE_SRC || blendMode == BLEND_MODE_ONE_DST_SUB_ONE_SRC)
        {
            ret = float4(ret.rgb, 1.0);
        }
        else if (blendMode == BLEND_MODE_ONE_DST_ADD_QRT_SRC)
        {
            ret = float4(ret.rgb * 0.25, 1.0);
        }
    }
    else
    {
        ret = float4(ret.rgb, 0.0);
    }

    return ret;
}

float4 draw_flat(float4 fsShadeColor, bool isShaded, int blendMode, bool isSemiTrans)
{
    return handle_final_color(float4(fsShadeColor.rgb, 1.0), float4(fsShadeColor.rgb, 1.0), false, isShaded, blendMode, isSemiTrans);
}

float4 draw_default_ft4(float4 fsShadeColor, int textureUnit, int palIndex, float2 fsUV, bool isShaded, int blendMode, bool isSemiTrans)
{
    float texelSprite = 0.0;

    if (textureUnit == 1)
    {
        texelSprite = tex2D(texSpriteSheets[0], fsUV).r;
    }

    float4 texelPal = PixelToPalette(texelSprite, palIndex);

    float4 finalCol = handle_final_color(fsShadeColor, texelPal, true, isShaded, blendMode, isSemiTrans);

    return finalCol;
}

float4 draw_cam(float2 fsUV)
{
    return float4(tex2D(texCamera, fsUV).rgb, 0.0);
}

float4 draw_fg1(int palIndex, float2 fsUV)
{
    float4 mask = float4(0.0, 0.0, 0.0, 0.0);

    if (palIndex == 0)
    {
        mask = tex2D(texFG1Masks[0], fsUV);
    }
    else if (palIndex == 1)
    {
        mask = tex2D(texFG1Masks[1], fsUV);
    }
    else if (palIndex == 2)
    {
        mask = tex2D(texFG1Masks[2], fsUV);
    }
    else if (palIndex == 3)
    {
        mask = tex2D(texFG1Masks[3], fsUV);
    }

    float4 outColor = float4(tex2D(texCamera, fsUV).rgb, 0.0);

    if (all(mask.rgb == float3(0.0, 0.0, 0.0)))
    {
        outColor = float4(0.0, 0.0, 0.0, 1.0);
    }

    return outColor;
}

float4 PS(
    float4 fsShadeColor
    : COLOR0,
      float2 fsUV
    : TEXCOORD0,
      float2 semiTransShaded
    : TEXCOORD1,
      float2 palIndexBlendMode
    : TEXCOORD2,
      float2 drawTypeTextureUnit
    : TEXCOORD3)
    : COLOR
{
    int drawType = drawTypeTextureUnit[0];
    int palIndex = palIndexBlendMode[0];
    int isShaded = semiTransShaded[0];

    int blendMode = palIndexBlendMode[1];
    int isSemiTrans = semiTransShaded[1];
    int textureUnit = drawTypeTextureUnit[1];

    if (drawType == DRAW_DEFAULT_FT4)
    {
        return draw_default_ft4(fsShadeColor, textureUnit, palIndex, fsUV, isShaded, blendMode, isSemiTrans);
    }
    else if (drawType == DRAW_FG1)
    {
        return draw_fg1(palIndex, fsUV);
    }
    else if (drawType == DRAW_FLAT)
    {
        return draw_flat(fsShadeColor, isShaded, blendMode, isSemiTrans);
    }
    else if (drawType == DRAW_GAS)
    {
        // TODO isn't it
    }

    // assume cam for now
    // if (drawType == DRAW_CAM)
    return draw_cam(fsUV);
}
)"
