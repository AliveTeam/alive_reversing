sampler texGas : register(s6);

bool dither(float2 pixelPos)
{
    return fmod(pixelPos.x + fmod(pixelPos.y, 2.0), 2.0) > 0.0;
}

float4 draw_gas(float2 fsUV)
{
    float4 texelGas = tex2D(texGas, fsUV);
    //if (dither(pixelPos))
    //{
        return texelGas * 0.5;
    //}
    //else
    //{
        return float4(0.0, 0.0, 0.0, 1.0);
    //}
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
    return draw_gas(fsUV);
}
