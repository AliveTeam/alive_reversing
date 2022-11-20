sampler texCamera : register(s4);
sampler texFG1Mask : register(s0);

float4 draw_fg1(float2 fsUV)
{
    float4 mask = tex2D(texFG1Mask, fsUV);
    if (all(mask.rgb == float3(0.0, 0.0, 0.0)))
    {
        return float4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        return float4(tex2D(texCamera, fsUV).rgb, 0.0);
    }
}

float4 draw_cam(float2 fsUV)
{
    return float4(tex2D(texCamera, fsUV).rgb, 0.0);
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
    if (drawTypeTextureUnit[0] == 2)
    {
        return draw_cam(fsUV);
    }
    else
    {
        return draw_fg1(fsUV);
    }
}
