sampler texCamera : register(s4);
sampler texFG1Masks[4] : register(s0);

float4 draw_fg1(int fg1Idx, float2 fsUV)
{
    float4 mask = float4(0.0, 0.0, 0.0, 1.0);
    if (fg1Idx == 0)
    {
        mask = tex2D(texFG1Masks[0], fsUV);
    }
    else if (fg1Idx == 1)
    {
        mask = tex2D(texFG1Masks[1], fsUV);
    }
    else if (fg1Idx == 2)
    {
        mask = tex2D(texFG1Masks[2], fsUV);
    }
    else if (fg1Idx == 3)
    {
        mask = tex2D(texFG1Masks[3], fsUV);
    }
    //mask = tex2D(texFG1Masks[0], fsUV);
    float4 outColor;
    if (all(mask.rgb == float3(0.0, 0.0, 0.0)))
    {
        outColor = float4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        outColor = float4(tex2D(texCamera, fsUV).rgb, 0.0);
    }

    return outColor;
}

float4 draw_cam(float2 fsUV)
{
    float4 outColor = tex2D(texCamera, fsUV);
    outColor = float4(outColor.rgb, 0.0);
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
    if (drawTypeTextureUnit[0] == 2)
    {
        return draw_cam(fsUV);
    }
    return draw_fg1(drawTypeTextureUnit[1], fsUV);
}
