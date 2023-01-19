//float2 fsTexSize : register(c12);
sampler texTextureData : register(s4);

uint ShiftLeft(uint value, int bits)
{
    return value * uint(pow(2, bits));
}

uint ShiftRight(uint value, int bits)
{
    return value / uint(pow(2, bits));
}

uint get888FromNormalized(float r, float g, float b)
{
    uint rValue = uint(abs(ceil(r * 255.0f)));
    uint gValue = uint(abs(ceil(g * 255.0f)));
    uint bValue = uint(abs(ceil(b * 255.0f)));

    rValue = rValue * uint(pow(2, 16)); // 2^ 16;
    gValue = gValue * uint(pow(2, 8));  // 2^ 8;

    return rValue + gValue + bValue;
}

float3 getNormalizedFrom888(uint rgbInput)
{
    uint firstByte = abs(rgbInput % 256u);
    uint secondByte = abs((rgbInput - firstByte) % 65536u);
    uint thirdByte = abs((rgbInput - firstByte - secondByte));

    float rValue = float(firstByte) / 255.0f;
    float gValue = float(secondByte) / 255.0f;
    float bValue = float(thirdByte) / 255.0f;

    return float3(rValue, gValue, bValue);
}

int Roll24Bits(int value)
{
    // value & 1

    // clears bit 0
    int valueShiftedRight = ShiftRight(value, 1);
    int temp = ShiftLeft(value, 31);
    int valueAnd1 = ShiftRight(temp, 31);

    // valueShiftedRight |= valueAnd1 ? 0x800000 : 0x0;

    // Drop the bit to roll
        // If the bit we removed was set then set it on the
        // other end
        valueShiftedRight += (valueAnd1 * pow(2, 23)); //     0x800000;
    return valueShiftedRight;
}

int ApplyF8Mask(int v)
{
    // same v = v & 0xF8
    v = ShiftRight(v, 3);
    v = ShiftLeft(v, 3);
    return v;
}

int ApplyMask(int r, int g, int b)
{
    r = ApplyF8Mask(r);
    g = ApplyF8Mask(g);
    b = ApplyF8Mask(b);

    r = r * pow(2, 16); // 2^ 16;
    g = g * pow(2, 8); // 2^ 8;

    return r + g + b;
}

int ApplyMask_t(int r, int g, int b)
{
    //r = ApplyF8Mask(r);
    //g = ApplyF8Mask(g);
    //b = ApplyF8Mask(b);

    r = r * pow(2, 16); // 2^ 16;
    g = g * pow(2, 8);  // 2^ 8;

    return r + g + b;
}

float Remove3Bits(float v)
{
    v -= fmod((v * 255.0f), 8.0f) / 255.0f;
    return v;
}

float4 PS(
    float4 fsShadeColor
          : COLOR0,
          float2 fsUV
          : TEXCOORD0) : COLOR
{
    float4 belowTexel = tex2D(texTextureData, fsUV);

    float oldy = fsUV.y;
    fsUV.y = ((fsUV.y * 240.0f) - 1.0f) / 240.0f;

    float4 aboveTexel = tex2D(texTextureData, fsUV);
    // bool scanline = int(mod(gl_FragCoord.y, 2.0f)) > 0;
    bool scanline = int(fmod((fsUV.y * 480.0f), 2.0f)) > 0;

    if (!scanline)
    {
        aboveTexel.r = Remove3Bits(aboveTexel.r);
        aboveTexel.g = Remove3Bits(aboveTexel.g);
        aboveTexel.b = Remove3Bits(aboveTexel.b);

        belowTexel.r = Remove3Bits(belowTexel.r);
        belowTexel.g = Remove3Bits(belowTexel.g);
        belowTexel.b = Remove3Bits(belowTexel.b);

        float4 tmp = aboveTexel + belowTexel;
        tmp = tmp / 2.0f;
        tmp.a = 1.0f;

        belowTexel = tmp;
        return belowTexel;
    }
    else
    {
        return belowTexel;
    }
    
   
}

float4 PS2(float4 fsShadeColor
           : COLOR0,
             float2 fsUV
           : TEXCOORD0)
    : COLOR
{
    return float4(0.0f, 1.0f, 0.0f, 1.0f);
}

technique Technique_0
{
    pass Pass0
    {
        SetPixelShader(CompileShader(ps_2_0, PS()));
    }

    pass Pass1
    {
        SetPixelShader(CompileShader(ps_2_0, PS2()));
    }
}
