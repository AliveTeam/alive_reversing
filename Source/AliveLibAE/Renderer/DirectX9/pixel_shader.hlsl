sampler texPalette : register(s5);
sampler texSpriteSheet : register(s7);

static const int BLEND_MODE_HALF_DST_ADD_HALF_SRC = 0;
static const int BLEND_MODE_ONE_DST_ADD_ONE_SRC = 1;
static const int BLEND_MODE_ONE_DST_SUB_ONE_SRC = 2;
static const int BLEND_MODE_ONE_DST_ADD_QRT_SRC = 3;

float4 PixelToPalette(float v, float palIndex)
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

float4 draw_default_ft4(float4 fsShadeColor, float palIndex, float2 fsUV, bool isShaded, int blendMode, bool isSemiTrans)
{
    float texelSprite = tex2D(texSpriteSheet, fsUV).r;
    float4 texelPal = PixelToPalette(texelSprite, palIndex);
    float4 finalCol = handle_final_color(fsShadeColor, texelPal, true, isShaded, blendMode, isSemiTrans);
    return finalCol;
}


float4 PS(
    float4 fsShadeColor : COLOR0,
    float2 fsUV : TEXCOORD0,
    float2 semiTransShaded  : TEXCOORD1,
    float2 palIndexBlendMode : TEXCOORD2,
    float2 drawTypeTextureUnit : TEXCOORD3)
    : COLOR
{
    bool isSemiTrans = semiTransShaded[0] != 0.0;
    bool isShaded = semiTransShaded[1] != 0.0;

    int palIndex = (int) (palIndexBlendMode[0] + 0.5);
    int blendMode = (int) (palIndexBlendMode[1] + 0.5);

    return draw_default_ft4(fsShadeColor, palIndex, fsUV, isShaded, blendMode, isSemiTrans);
}
